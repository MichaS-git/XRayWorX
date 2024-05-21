#include <asynTubeInterfaceEventHandler.h>
//#include "CommonHeader.h"

static const char *driverName = "TubeInterfaceEventHandler";

#define TubeInitialize              "TUBE_INITIALIZE"
#define TubeInitialize_RBV          "TUBE_INITIALIZE_RBV"
#define TubeStartUpState	        "TUBE_START_UP_STATE"
#define TubeStartUp				    "TUBE_START_UP"
#define TubeXrayOnOff			    "TUBE_XRAY_ONOFF"
#define TubeXrayOnOff_RBV		    "TUBE_XRAY_ONOFF_RBV"
#define TubeHighVoltageMonitor      "TUBE_HIGH_VOLTAGE_MONITOR"
#define TubeTargetCurrentMonitor    "TUBE_TARGET_CURRENT_MONITOR"
#define TubeEmissionCurrentMonitor  "TUBE_EMISSION_CURRENT_MONITOR"

static void c_InitializeTask(void *arg)
{
    TubeInterfaceEventHandler *p = (TubeInterfaceEventHandler *)arg;
    p->InitializeTask();
}

void TubeInterfaceEventHandler::InitializeTask(void)
{
    highVoltageMonitor = 0;
	targetCurrentMonitor = 0;
	emissionCurrentMonitor = 0;
	// ![Init ITubeInterfacePtr]

    // Initialize our COM instance
    // in the X-COM example from XRayWorX it is "::CoInitialize(NULL);"
    // we need to use the following in order to NOT block multithreading:
    //::CoInitialize(NULL);
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	pTubeInterface = NULL;
	dwEventCookie = 0;
	pIConnectionPoint = NULL;
	//Create instance from COM interface
	XRAYWorXBaseCOM::ITubeLoaderCOMPtr loader = NULL;
	HRESULT hr = loader.CreateInstance(__uuidof(TubeLoaderCOM));
	if (hr == S_OK)
	{
		try
		{
            //For customizing paths used by XRAYWorXBaseCOM.dll uncomment the next line and change the paths accordingly.
			loader->SetCustomPaths(_bstr_t("C://epics//IOC//XRayWorX//bin//windows-x64-static"), _bstr_t("C://epics//IOC//XRayWorX//bin//windows-x64-static"), _bstr_t("C://epics//IOC//XRayWorX//bin//windows-x64-static"));
			_bstr_t ip = loader->DefaultIpAddress->Ip;
			//std::cout << "IP:" << ip << std::endl;
			pTubeInterface = loader->GetTubeInterface(ip);

			hr = InitTubeInterfaceEventSink();

			pDataModuleProvider = ((IDataModuleLoaderCOMPtr)loader)->GetDataModuleProvider(ip);

            MessageBoxW(NULL, L"Instantiating TubeInterface. Press ok to abort.", L"Info", MB_OK);
			//OnInitialized(); // this causes a crash ...
		}
		catch (_com_error& ex)
		{
			printf("Error: %s\n", ex.ErrorMessage());
		}
	}
	else
		printf("Error creating instance of TubeLoaderCOM.\n");
	// ![Init ITubeInterfacePtr]


    //MessageBoxW(NULL, L"Instantiating TubeInterface. Press ok to abort.", L"Info", MB_OK);

    // we need the MessageBox, otherwise the TubeInterface is not loaded (?!?)

    /*HWND hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"Instantiating TubeInterface. Press ok to abort.", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, NULL, NULL);
    if (hwnd == NULL)
    {
        printf("Fehler beim Erstellen des Fensters: %d\n", GetLastError());
    }
    else
    {
        // Fenster anzeigen
        ShowWindow(hwnd, SW_SHOW);

        // Optional: Main Loop für das Fenster
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Fenster zerstören
        DestroyWindow(hwnd);
    }*/
}

HRESULT TubeInterfaceEventHandler::InitTubeInterfaceEventSink()
{
	HRESULT hr = LinkEventSink(&tubeInterfaceEventSink, __uuidof(ITubeInterfaceEvents));
	if (hr == S_OK)
		tubeInterfaceEventSink.LinkTubeInterfaceEventHandler(this);
	return hr;
}

HRESULT TubeInterfaceEventHandler::LinkEventSink(IUnknown* eventSink, const IID &riid)
{	//the connection point container
	LPCONNECTIONPOINTCONTAINER pIConnectionPointContainer = NULL;
	pIConnectionPoint = NULL;
	HRESULT hr = pTubeInterface->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pIConnectionPointContainer);
	if (hr == S_OK)
	{	//Returns a pointer to the IConnectionPoint interface of a connection point for
		//a specific IID, if that IID describes a supported outgoing interface.
		hr = pIConnectionPointContainer->FindConnectionPoint(riid, &pIConnectionPoint);
		if (!pIConnectionPointContainer)
			pIConnectionPointContainer->Release();
		if (pIConnectionPoint)
		{	//Establishes a connection between the connection point object and the client's sink
			hr = pIConnectionPoint->Advise((LPUNKNOWN)eventSink, &dwEventCookie);
		}
	}
	return hr;
}

void TubeInterfaceEventHandler::LinkEventSink(EventSinkBase* eventSink, const IID &eventsInterfacePtr, IUnknown* eventSource)
{
	LPCONNECTIONPOINTCONTAINER pConnPointContainer = NULL;
	LPCONNECTIONPOINT pConnPoint = NULL;
	HRESULT hr = eventSource->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pConnPointContainer);
	if (hr == S_OK)
	{
		hr = pConnPointContainer->FindConnectionPoint(eventsInterfacePtr, &pConnPoint);
		if (!pConnPointContainer)
			pConnPointContainer->Release();
	}
	if (pConnPoint)
	{
		hr = pConnPoint->Advise((LPUNKNOWN)eventSink, &dwEventCookie);
		eventSink->LinkTubeInterfaceEventHandler(this);
	}
}

void TubeInterfaceEventHandler::OnInitialized()
{
	//add userdefined code here

	VARIANT_BOOL isInitialized = pTubeInterface->IsInitialized;

	LinkEventSink(&targetCurrentEventSink, __uuidof(ITubeCommandSingleEvents), pTubeInterface->TargetCurrent);
	LinkEventSink(&emissionCurrentEventSink, __uuidof(ITubeCommandSingleEvents), pTubeInterface->EmissionCurrent);
	LinkEventSink(&highVoltageEventSink, __uuidof(ITubeCommandSingleEvents), pTubeInterface->HighVoltage);
	LinkEventSink(&xRayOnEventSink, __uuidof(ITubeCommandBoolEvents), pTubeInterface->XRayOn);
	LinkEventSink(&xRayOffEventSink, __uuidof(ITubeCommandBoolEvents), pTubeInterface->XRayOff);
	LinkEventSink(&interlockEventSink, __uuidof(ITubeMonitorBoolEvents), pTubeInterface->Interlock);
	LinkEventSink(&vacuumOkEventSink, __uuidof(ITubeMonitorBoolEvents), pTubeInterface->VacuumOk);
	LinkEventSink(&cooling1OkEventSink, __uuidof(ITubeMonitorBoolEvents), pTubeInterface->CoolingOk);
	LinkEventSink(&cooling2OkEventSink, __uuidof(ITubeMonitorBoolEvents), pTubeInterface->CoolingTwoOk);
	LinkEventSink(&filamentAdjustEventSink, __uuidof(ITubeAutoCommandCOMEvents), pTubeInterface->FilamentAdjust);
	LinkEventSink(&startUpEventSink, __uuidof(ILimitableAutoCommandCOMEvents), pTubeInterface->StartUp);
	LinkEventSink(&xRayOutControlEventSink, __uuidof(ITubeCommandXocEvents), pTubeInterface->XrayOutControl);
	LinkEventSink(&flashoverEventSink, __uuidof(IFlashoverCOMEvents), pTubeInterface->Flashover);
	LinkEventSink(&modeListEventSink, __uuidof(ITubeListMonitorStringEvents), pTubeInterface->ModeList);
	LinkEventSink(&modeEventSink, __uuidof(ITubeCommandSingleEvents), pTubeInterface->Mode);
	LinkEventSink(&autoCommandInfoEventSink, __uuidof(IAutoCommandInfosCOMEvents), pTubeInterface->AutoCommandInfo);
	LinkEventSink(&filamentAdjustInfoEventSink, __uuidof(ICommandInfoMonitorCOMEvents), pTubeInterface->AutoCommandInfo->FilamentAdjust);
	LinkEventSink(&filamentLifetimeEventSink, __uuidof(ITubeServiceMonitorCOMEvents), pTubeInterface->FilamentLifetime);
	LinkEventSink(&defocListEventSink, __uuidof(ITubeListCOMEvents), pTubeInterface->DefocussingList);
	LinkEventSink(&initialStartupEventSink, __uuidof(IInitialStartupCOMEvents), pTubeInterface->InitialStartup);
	LinkEventSink(&standbyEventSink, __uuidof(IStandbyCOMEvents), pTubeInterface->StandbyEx);
	LinkEventSink(&runningTimerEventSink, __uuidof(ITubeTimeSpanCOMEvents), pTubeInterface->RunningTimer);
	LinkEventSink(&filamentStatsEventSink, __uuidof(IFilamentStatsCOMEvents), pTubeInterface->FilamentStats);
	LinkEventSink(&turbopumpEventSink, __uuidof(ITurbopumpCommandCOMEvents), pTubeInterface->Turbopump);

	LinkEventSink(&centeringOneTableXEventSink, __uuidof(ICoilDataCOMEvents), pDataModuleProvider->GetCenteringOneTableX());
	LinkEventSink(&centeringOneTableYEventSink, __uuidof(ICoilDataCOMEvents), pDataModuleProvider->GetCenteringOneTableY());
	LinkEventSink(&vacuumDataEventSink, __uuidof(IDataModuleCOMEvents), pDataModuleProvider->GetDataModule(DataModuleName_Vacuum));

	SetDlgInterlock(pTubeInterface->Interlock->MonitorValue);
	SetDlgVacuumOk(pTubeInterface->VacuumOk->MonitorValue);

	//printf("pTubeInterface initialized: %s\n", pTubeInterface->IsInitialized ? "true" : "false");

	if (pTubeInterface->IsInitialized) {
        setIntegerParam(TubeInitializeRBV_, 1);
	}
	if (!pTubeInterface->XRayOn->MonitorValue && pTubeInterface->XRayOff->MonitorValue) {
		//dlg->m_lblXrayState.SetWindowTextW((LPCTSTR)_T("X-RAY IS OFF"));
		printf("XrayState: X-RAY IS OFF\n");
		setIntegerParam(TubeXrayOnOffRBV_, 0);
	} else {
		//dlg->m_lblXrayState.SetWindowTextW((LPCTSTR)_T("X-RAY IS ON"));
		printf("XrayState: X-RAY IS ON\n");
		setIntegerParam(TubeXrayOnOffRBV_, 1);
	}

    callParamCallbacks();

    // Close the message box programmatically
    //::EndDialog(::GetActiveWindow(), IDOK);
}

void TubeInterfaceEventHandler::Initialize()
{
    highVoltageMonitor = 0;
	targetCurrentMonitor = 0;
	emissionCurrentMonitor = 0;
	// ![Init ITubeInterfacePtr]

    // Initialize our COM instance
    // in the X-COM example from XRayWorX it is "::CoInitialize(NULL);"
    // we need to use the following in order to NOT block multithreading:
    //::CoInitialize(NULL);
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	pTubeInterface = NULL;
	dwEventCookie = 0;
	pIConnectionPoint = NULL;
	//Create instance from COM interface
	XRAYWorXBaseCOM::ITubeLoaderCOMPtr loader = NULL;
	HRESULT hr = loader.CreateInstance(__uuidof(TubeLoaderCOM));
	if (hr == S_OK)
	{
		try
		{
            //For customizing paths used by XRAYWorXBaseCOM.dll uncomment the next line and change the paths accordingly.
			loader->SetCustomPaths(_bstr_t("C://epics//IOC//XRayWorX//bin//windows-x64-static"), _bstr_t("C://epics//IOC//XRayWorX//bin//windows-x64-static"), _bstr_t("C://epics//IOC//XRayWorX//bin//windows-x64-static"));
			_bstr_t ip = loader->DefaultIpAddress->Ip;
			//std::cout << "IP:" << ip << std::endl;
			pTubeInterface = loader->GetTubeInterface(ip);

			hr = InitTubeInterfaceEventSink();

			pDataModuleProvider = ((IDataModuleLoaderCOMPtr)loader)->GetDataModuleProvider(ip);

			//OnInitialized(); // this causes a crash ...
		}
		catch (_com_error& ex)
		{
			printf("Error: %s\n", ex.ErrorMessage());
		}
	}
	else
		printf("Error creating instance of TubeLoaderCOM.\n");
	// ![Init ITubeInterfacePtr]
}

void TubeInterfaceEventHandler::OnTubeStateChanged()
{
	//add userdefined code here
	printf("Function OnTubeStateChanged executed.\n");
}

void TubeInterfaceEventHandler::OnTubeInterfaceError(ULONG errorCode)
{
	//add userdefined code here
	//dlg->SetError(errorCode);
	printf("Error: %lu\n", errorCode);
}

void TubeInterfaceEventHandler::OnIsAccessibleChanged(ULONG eventSinkID)
{
	//add user defined code here
	printf("Function OnIsAccessibleChanged executed.\n");
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageAccessibleChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentAccessibleChanged();
	else if (eventSinkID == emissionCurrentEventSink.GetEventSinkID())
		OnEmissionCurrentAccessibleChanged();
	else if (eventSinkID == cooling1OkEventSink.GetEventSinkID())
		OnCooling1OkMonitorChanged();
	else if (eventSinkID == cooling2OkEventSink.GetEventSinkID())
		OnCooling2OkMonitorChanged();
	else if (eventSinkID == flashoverEventSink.GetEventSinkID())
	{
		OnFlashoverChanged();
		OnLockingXrayChanged();
	}
	else if (eventSinkID == modeEventSink.GetEventSinkID())
		OnModeChanged();
	else if (eventSinkID == autoCommandInfoEventSink.GetEventSinkID())
		AutoCommandInfo_AccessibleChanged();
	//else if (eventSinkID == turbopumpPumpEventSink.GetEventSinkID())
	//	Turbopump_AccessibleChanged()
	//else if (eventSinkID == vacuumDataEventSink.GetEventSinkID())
	//{
	//	std::wostringstream stream;
	//	stream << "VacuumData.IsAccessible changed...";
	//	MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
	//}
	else if (eventSinkID == filamentStatsEventSink.GetEventSinkID())
		OnFilamentStatsChanged();
	else if (eventSinkID == runningTimerEventSink.GetEventSinkID())
		OnRunningTimerChanged();
}

void TubeInterfaceEventHandler::AutoCommandInfo_AccessibleChanged()
{
	//InitCommandInfoMonitorEventSink(filamentAdjustInfoEventSink
}

void TubeInterfaceEventHandler::OnHighVoltageAccessibleChanged()
{
	ITubeCommandSinglePtr highVoltage = pTubeInterface->HighVoltage;
	if (highVoltage)
	{
		if (highVoltage->HasReadAccess())
		{
			UpdateHighVoltageMonitor();
		}
	}
}

void TubeInterfaceEventHandler::UpdateHighVoltageMonitor()
{
	ITubeCommandSinglePtr highVoltage = pTubeInterface->HighVoltage;
	if (highVoltage)
	{
		if (highVoltage->HasReadAccess())
		{
			float highVoltageDemand = highVoltage->PcDemandValue;
			highVoltageMonitor = highVoltage->MonitorValue;
			//dlg->SetHighVoltageMonitor(highVoltageMonitor);
			//printf("SetHighVoltageMonitor: %f\n", highVoltageMonitor);
			setDoubleParam(TubeHighVoltageMonitor_, highVoltageMonitor);
			callParamCallbacks();
			long highVoltageErrorCode = highVoltage->ErrorCode;
		}
	}
}

void TubeInterfaceEventHandler::OnTargetCurrentAccessibleChanged()
{
	ITubeCommandSinglePtr targetCurrent = pTubeInterface->TargetCurrent;
	if (targetCurrent)
	{
		if (targetCurrent->HasReadAccess())
			UpdateTargetCurrentMonitor();
	}
}

void TubeInterfaceEventHandler::OnEmissionCurrentAccessibleChanged()
{
	ITubeCommandSinglePtr emissionCurrent = pTubeInterface->EmissionCurrent;
	if (emissionCurrent)
	{
		if (emissionCurrent->HasReadAccess())
			UpdateEmissionCurrentMonitor();
	}
}

void TubeInterfaceEventHandler::OnMonitorValueChanged(ULONG eventSinkID)
{
    printf("Function OnMonitorValueChanged executed.\n");
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageMonitorValueChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentMonitorValueChanged();
	else if (eventSinkID == emissionCurrentEventSink.GetEventSinkID())
		OnEmissionCurrentMonitorValueChanged();
	else if (eventSinkID == xRayOffEventSink.GetEventSinkID() ||
		eventSinkID == xRayOnEventSink.GetEventSinkID())
		OnXRayOnOffMonitorValueChanged();
	else if (eventSinkID == interlockEventSink.GetEventSinkID())
		OnInterlockMonitorValueChanged();
	else if (eventSinkID == vacuumOkEventSink.GetEventSinkID())
		OnVacuumOkMonitorValueChanged();
	else if (eventSinkID == cooling1OkEventSink.GetEventSinkID())
		OnCooling1OkMonitorChanged();
	else if (eventSinkID == cooling2OkEventSink.GetEventSinkID())
		OnCooling2OkMonitorChanged();
	else if (eventSinkID == modeEventSink.GetEventSinkID())
		OnModeChanged();
	else if (eventSinkID == standbyEventSink.GetEventSinkID())
		OnStandbyChanged();
}

void TubeInterfaceEventHandler::OnHighVoltageMonitorValueChanged()
{
	ITubeCommandSinglePtr highVoltage = pTubeInterface->HighVoltage;
	if (highVoltage)
	{
		if (highVoltage->HasReadAccess())
			UpdateHighVoltageMonitor();
	}
}

void TubeInterfaceEventHandler::OnTargetCurrentMonitorValueChanged()
{
	UpdateTargetCurrentMonitor();
}

void TubeInterfaceEventHandler::UpdateTargetCurrentMonitor()
{
	ITubeCommandSinglePtr targetCurrent = pTubeInterface->TargetCurrent;
	if (targetCurrent)
	{
		if (targetCurrent->HasReadAccess())
		{
			float targetCurrentDemand = targetCurrent->PcDemandValue;
			targetCurrentMonitor = targetCurrent->MonitorValue;
			//dlg->SetTargetCurrentMonitor(targetCurrentMonitor);
			//printf("SetTargetCurrentMonitor: %f\n", targetCurrentMonitor);
			setDoubleParam(TubeTargetCurrentMonitor_, targetCurrentMonitor);
			callParamCallbacks();
			long targetErrorCode = targetCurrent->ErrorCode;
		}
	}
}

void TubeInterfaceEventHandler::OnEmissionCurrentMonitorValueChanged()
{
	UpdateEmissionCurrentMonitor();
}

void TubeInterfaceEventHandler::UpdateEmissionCurrentMonitor()
{
	ITubeCommandSinglePtr emissionCurrent = pTubeInterface->EmissionCurrent;
	if (emissionCurrent)
	{
		if (emissionCurrent->HasReadAccess())
		{
			float emissionCurrentDemand = emissionCurrent->PcDemandValue;
			emissionCurrentMonitor = emissionCurrent->MonitorValue;
			setDoubleParam(TubeEmissionCurrentMonitor_, emissionCurrentMonitor);
			callParamCallbacks();
			//long targetErrorCode = targetCurrent->ErrorCode;
		}
	}
}

void TubeInterfaceEventHandler::OnDemandLowerLimitChanged(ULONG eventSinkID)
{
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageDemandLowerLimitChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentDemandLowerLimitChanged();
}

void TubeInterfaceEventHandler::OnHighVoltageDemandLowerLimitChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnTargetCurrentDemandLowerLimitChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnDemandUpperLimitChanged(ULONG eventSinkID)
{
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageDemandUpperLimitChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentDemandUpperLimitChanged();
}

void TubeInterfaceEventHandler::OnHighVoltageDemandUpperLimitChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnTargetCurrentDemandUpperLimitChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnStateChanged(ULONG eventSinkID)
{
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageStateChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentStateChanged();
	else if (eventSinkID == startUpEventSink.GetEventSinkID())
		OnStartUpStateChanged();
}

void TubeInterfaceEventHandler::OnHighVoltageStateChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnTargetCurrentStateChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnStartUpStateChanged()
{	//user defined code
	printf("StartUp sequence entered\n");
	if (pTubeInterface->StartUp->State == CommandStates_OK) {
		printf("State: OK\n");
		setIntegerParam(TubeStartUpState_, 0);
        // Close the message box programmatically
        //::EndDialog(::GetActiveWindow(), IDOK);
	} else if (pTubeInterface->StartUp->State == CommandStates_Acknowledged) {
		printf("State: Acknowledged\n");
		setIntegerParam(TubeStartUpState_, 1);
	} else if (pTubeInterface->StartUp->State == CommandStates_Busy) {
		printf("State: Busy\n");
		setIntegerParam(TubeStartUpState_, 2);
	} else if (pTubeInterface->StartUp->State == CommandStates_Warning) {
		printf("State: Warning\n");
		setIntegerParam(TubeStartUpState_, 3);
    } else if (pTubeInterface->StartUp->State == CommandStates_Error) {
		printf("State: Error\n");
		setIntegerParam(TubeStartUpState_, 4);
        // Close the message box programmatically
        //::EndDialog(::GetActiveWindow(), IDOK);
    }
    callParamCallbacks();
}

void TubeInterfaceEventHandler::OnPlcDemandValueChanged(ULONG eventSinkID)
{
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltagePlcDemandValueChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentPlcDemandValueChanged();
}

void TubeInterfaceEventHandler::OnHighVoltagePlcDemandValueChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnTargetCurrentPlcDemandValueChanged()
{	//user defined code
}

void TubeInterfaceEventHandler::OnXRayOnOffMonitorValueChanged()
{
	if (!pTubeInterface->XRayOn->MonitorValue && pTubeInterface->XRayOff->MonitorValue) {
		//dlg->m_lblXrayState.SetWindowTextW((LPCTSTR)_T("X-RAY IS OFF"));
		printf("XrayState: X-RAY IS OFF\n");
		setIntegerParam(TubeXrayOnOffRBV_, 0);
	} else {
		//dlg->m_lblXrayState.SetWindowTextW((LPCTSTR)_T("X-RAY IS ON"));
		printf("XrayState: X-RAY IS ON\n");
		setIntegerParam(TubeXrayOnOffRBV_, 1);
	}
    callParamCallbacks();

    /*if (CloseWindow)
        // Close the message box programmatically
        ::EndDialog(::GetActiveWindow(), IDOK);
        CloseWindow = false;*/
}

void TubeInterfaceEventHandler::OnInterlockMonitorValueChanged()
{	//user defined code
	SetDlgInterlock(pTubeInterface->Interlock->MonitorValue);
}

void TubeInterfaceEventHandler::SetDlgInterlock(VARIANT_BOOL interlockClosed)
{
    if (interlockClosed)
        //dlg->m_edInterlock.SetWindowTextA("CLOSED");
		printf("Interlock: CLOSED\n");
    else
        //dlg->m_edInterlock.SetWindowTextA("OPEN");
		printf("Interlock: OPEN\n");
}


void TubeInterfaceEventHandler::OnVacuumOkMonitorValueChanged()
{	//user defined code
	SetDlgVacuumOk(pTubeInterface->VacuumOk->MonitorValue);
}

void TubeInterfaceEventHandler::SetDlgVacuumOk(VARIANT_BOOL vacuumOk)
{
	if (vacuumOk)
		//dlg->m_edVacuum.SetWindowTextA("OK");
		printf("Vacuum: OK\n");
	else
		//dlg->m_edVacuum.SetWindowTextA("INSUFFICIENT");
		printf("Vacuum: INSUFFICIENT\n");
}

void TubeInterfaceEventHandler::OnCooling1OkMonitorChanged()
{
    // User-defined code
    LPCTSTR coolingOk = GetCoolingOk(pTubeInterface->CoolingOk);

    // Check if coolingOk is null
    if (coolingOk)
    {
        //dlg->m_edCooling1.SetWindowTextA(coolingOk);
		printf("Cooling1: OK\n");
    }
    else
    {
        // Handle the case when coolingOk is null
        //dlg->m_edCooling1.SetWindowTextA("Unknown");
		printf("Cooling1: Unknown\n");
    }
}


void TubeInterfaceEventHandler::OnCooling2OkMonitorChanged()
{	//user defined code
	LPCTSTR coolingOk = GetCoolingOk(pTubeInterface->CoolingTwoOk);
	printf("Cooling2: OK\n");
}

/*LPCTSTR TubeInterfaceEventHandler::GetCoolingOk(ITubeMonitorBool *coolingOk)
{
	if (coolingOk->HasReadAccess())
	{
		if (coolingOk->MonitorValue)
			return (LPCTSTR)_T("GOOD");
		return (LPCTSTR)_T("BAD");
	}
	return (LPCTSTR)_T("NOT ACCESSIBLE");
}*/

const char* TubeInterfaceEventHandler::GetCoolingOk(ITubeMonitorBool *coolingOk)
{
    if (coolingOk->HasReadAccess())
    {
        if (coolingOk->MonitorValue)
            return "GOOD";
        return "BAD";
    }
    return "NOT ACCESSIBLE";
}


void TubeInterfaceEventHandler::OnIsActiveChanged(ULONG eventSink)
{	//user defined code
	//Example: Disable all controls except of X-RayOff-Button.
}

void TubeInterfaceEventHandler::OnFlashoverChanged()
{
	SetFlashovers(pTubeInterface->Flashover);
}

void TubeInterfaceEventHandler::SetFlashovers(IFlashoverCOM *flashover)
{
	if (flashover->HasReadAccess())
		//dlg->SetFlashoverCount(flashover->Count);
		printf("SetFlashoverCount: %i\n", flashover->Count);
	else
		//dlg->m_edFlashover.SetWindowTextW((LPCTSTR)_T("NOT ACCESSIBLE"));
		//dlg->m_edFlashover.SetWindowTextA("NOT ACCESSIBLE");
		printf("Flashover NOT ACCESSIBLE\n");
}

void TubeInterfaceEventHandler::OnLockingXrayChanged()
{
	//Example: Enter code here to lock input while xray on is locked due to
	//flashover handling at the tube's plc.
	//while (pTubeInterface->Flashover->IsLockingXray) xray input is locked.
}

void TubeInterfaceEventHandler::OnListChanged(ULONG eventSinkID)
{
	if (eventSinkID == modeListEventSink.GetEventSinkID())
		//OnModeListChanged();
		printf("OnModeListChanged()\n");
	else if (eventSinkID == defocListEventSink.GetEventSinkID())
		OnDefocListChanged();
	//else if (eventSinkID == targetList.GetEventSinkID())
	//	OnTargetListChanged();
	//else if (eventSinkID == headList.GetEventSinkID())
	//	OnHeadListChanged();
}

void TubeInterfaceEventHandler::OnChanged(ULONG eventSinkID)
{
	if (eventSinkID == defocListEventSink.GetEventSinkID())
		OnDefocListChanged();
	else if (eventSinkID == filamentStatsEventSink.GetEventSinkID())
		OnFilamentStatsChanged();
}

void TubeInterfaceEventHandler::OnDefocListChanged()
{
	ITubeListCOMPtr defocList = pTubeInterface->DefocussingList;
	if (defocList->HasReadAccess())
	{
		//SampleCode ringing throu the "Defocus tabs".
		//int index = defocList->ListIndex;
		//int length = defocList->List->cbElements;
		//index++;
		//if (index >= length)
		//	index = 0;
		//if (defocList->IsAccessible == AccessStates_ReadWrite)
		//	defocList->ChangeIndex(index);
	}
}

/*void TubeInterfaceEventHandler::OnModeListChanged()
{
	FillModeList();
}*/

/*void UnpackBstrArrayHelper(VARIANT* pvarArrayIn, CStringArray* pstrarrValues)
{
	if (!pstrarrValues || !pvarArrayIn || pvarArrayIn->vt == VT_EMPTY)
		return;

	pstrarrValues->RemoveAll();

	VARIANT* pvarArray = pvarArrayIn;
	SAFEARRAY* parrValues = NULL;

	SAFEARRAYBOUND arrayBounds[1];
	arrayBounds[0].lLbound = 0;
	arrayBounds[0].cElements = 0;

	if ((pvarArray->vt & (VT_VARIANT | VT_BYREF | VT_ARRAY)) == (VT_VARIANT | VT_BYREF) &&
		NULL != pvarArray->pvarVal &&
		(pvarArray->pvarVal->vt & VT_ARRAY))
	{
		pvarArray = pvarArray->pvarVal;
	}

	if (pvarArray->vt & VT_ARRAY)
	{
		if (VT_BYREF & pvarArray->vt)
			parrValues = *pvarArray->pparray;
		else
			parrValues = pvarArray->parray;
	}
	else
		return;

	if (parrValues != NULL)
	{
		HRESULT hr = SafeArrayGetLBound(parrValues, 1, &arrayBounds[0].lLbound);
		hr = SafeArrayGetUBound(parrValues, 1, (long*)&arrayBounds[0].cElements);
		arrayBounds[0].cElements -= arrayBounds[0].lLbound;
		arrayBounds[0].cElements += 1;
	}

	if (arrayBounds[0].cElements > 0)
	{
		for (ULONG i = 0; i < arrayBounds[0].cElements; i++)
		{
			LONG lIndex = (LONG)i;
			CString strValue = _T("");

			VARTYPE vType;
			BSTR bstrItem;

			::SafeArrayGetVartype(parrValues, &vType);
			HRESULT hr = ::SafeArrayGetElement(parrValues, &lIndex, &bstrItem);

			if (SUCCEEDED(hr))
			{
				switch (vType)
				{
					case VT_BSTR:
						strValue = (LPCTSTR)bstrItem;
						break;
				}

				::SysFreeString(bstrItem);
			}

			pstrarrValues->Add(strValue);
		}
	}
}

void UnpackBstrArray(const _variant_t &var, CStringArray &strarrValues)
{
	UnpackBstrArrayHelper(&(VARIANT)const_cast<_variant_t & >(var), &strarrValues);
}

void TubeInterfaceEventHandler::FillModeList()
{
	if (pTubeInterface->ModeList->HasReadAccess())
	{
		CStringArray modes;
		modes.Add(_T(""));
		SAFEARRAY *sarrList = pTubeInterface->ModeList->List;

		_variant_t list;
		list.parray = sarrList;
		list.vt = VT_ARRAY | VT_BSTR;

		UnpackBstrArray(list, modes);
		SafeArrayDestroy(sarrList);

		INT_PTR length = modes.GetCount();
		for (int i = 0; i < length; i++)
			dlg->cbModes.AddString(modes.GetAt(i));

		OnModeChanged();
	}
}*/

void TubeInterfaceEventHandler::OnModeChanged()
{
	if (pTubeInterface->Mode->HasReadAccess())
	{
		int index = (int)pTubeInterface->Mode->MonitorValue;
			//dlg->cbModes.SetCurSel(index);
			printf("cbModes.SetCurSel: %i\n", index);
	}
}

void TubeInterfaceEventHandler::OnRemainingTimeChanged(ULONG eventSinkID)
{
	if (eventSinkID == filamentLifetimeEventSink.GetEventSinkID())
	{
		long remainingTimeInSeconds = pTubeInterface->FilamentLifetime->RemainingTimeInSeconds;
		//Do something if remaining time changed...
	}
	else if (eventSinkID == filamentAdjustInfoEventSink.GetEventSinkID())
	{	//Keep in mind that the CommandInfoMonitor-objects are provided by AutoCommandInfo.
		long remainingTimeInSeconds = pTubeInterface->AutoCommandInfo->FilamentAdjust->RemainingTimeInSeconds;
		//Do somthing if remaining time changed...
	}
}

void TubeInterfaceEventHandler::OnServiceStateChanged(ULONG eventSinkID)
{
	if (eventSinkID == filamentLifetimeEventSink.GetEventSinkID())
	{
		ServiceState serviceState = pTubeInterface->FilamentLifetime->ServiceState;
		switch (serviceState)
		{
			case ServiceState_Bad:
			{
				//ReactionOn_ServiceState_Bad();
				break;
			}
			case ServiceState_Warning:
			{
				//ReactionOn_ServiceState_Warning();
				break;
			}
			case ServiceState_Good:
			{
				//ReactionOn_ServiceState_Good();
				break;
			}
		}
	}
}

void TubeInterfaceEventHandler::OnActionInfoChanged(ULONG eventSinkID)
{
	if (eventSinkID == filamentAdjustInfoEventSink.GetEventSinkID())
	{	//Keep in mind that the CommandInfoMonitor-objects are provided by AutoCommandInfo.
		CommandActionInfo actionInfo = pTubeInterface->AutoCommandInfo->FilamentAdjust->ActionInfo;
		switch (actionInfo)
		{
			case CommandActionInfo_Required:
			{
				//ReactionOn_CommandActionInfo_Required();
				break;
			}
			case CommandActionInfo_Recommended:
			{
				//ReactionOn_CommandActionInfo_Recommended();
				break;
			}
			case CommandActionInfo_Ok:
			{
				//ReactionOn_CommandActionInfo_Ok();
				break;
			}
		}
	}
}

void TubeInterfaceEventHandler::OnDataUpdated(ULONG eventSinkID)
{
	if (eventSinkID == centeringOneTableXEventSink.GetEventSinkID())
	{	//Get value for centering coil x from table with kv-index 23
		ICoilDataCOMPtr coilX = pDataModuleProvider->GetCenteringOneTableX();
		IResultFloatCOMPtr centerValueX = coilX->Get(23);
		if (centerValueX->IsSuccess)
		{
			//std::wostringstream stream;
			//stream << "CenterValueX: ";
			//stream << centerValueX->FloatValue;
			//MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
		}
	}
	else if (eventSinkID == centeringOneTableYEventSink.GetEventSinkID())
	{	//Get value for centering coil y from table with kv-index 23
		IResultFloatCOMPtr centerValueY = pDataModuleProvider->GetCenteringOneTableY()->Get(23);
		if (centerValueY->IsSuccess)
		{
			//std::wostringstream stream;
			//stream << "CenterValueY: ";
			//stream << centerValueY->FloatValue;
			//MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
		}
	}
	else if (eventSinkID == vacuumDataEventSink.GetEventSinkID())
	{
		IDataModuleCOMPtr vacuumData = pDataModuleProvider->GetDataModule(DataModuleName_Vacuum);
		IResultObjectCOMPtr result = vacuumData->Get(_bstr_t("LowerLimit"));
		if (result->IsSuccess)
		{
			//float lowerLimit = V_R4(&result->ObjectValue);
			//std::wostringstream stream;
			//stream << "Vacuum Lower Limit: ";
			//stream << lowerLimit;
			//MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);

			//pTubeInterface->LogOn(_bstr_t("Or@ngeRay"));

			//lowerLimit += 0.1e-5f;
			//VARIANT toSend = _variant_t(lowerLimit);
			//vacuumData->Set(_bstr_t("LowerLimit"), toSend);
			//vacuumData->Save();

			//pTubeInterface->LogOff();
		}
	}
}

void TubeInterfaceEventHandler::OnError(ULONG eventSinkID, ULONG errorCode, BSTR errorText)
{
	std::wostringstream stream;
	stream << "Fehler ";
	stream << errorCode;
	stream << " aufgetreten. ";
	stream << errorText;
	MessageBoxW(NULL, stream.str().c_str(), NULL, MB_OK);
}

void TubeInterfaceEventHandler::OnSaved(ULONG eventSinkID)
{
	if (eventSinkID == vacuumDataEventSink.GetEventSinkID())
	{
		std::wostringstream stream;
		stream << "Vacuum data module saved.";
		MessageBoxW(NULL, stream.str().c_str(), NULL, MB_OK);
	}
}

void TubeInterfaceEventHandler::OnUpdating(ULONG eventSinkID)
{
}

void TubeInterfaceEventHandler::OnIsSwitchedOnChanged()
{	//Add your code here if Turbopump.SwitchedOn has changed
	//pTubeInterface->Turbopump->IsSwitchedOn
	//For access handling see also void TubeInterfaceEventHandler::OnIsAccessibleChanged(ULONG eventSinkID)
}

void TubeInterfaceEventHandler::OnIsVentilatedChanged()
{	//Add your code here if Turbopump.IsVentilated has changed
	//pTubeInterface->Turbopump->IsVentilated
	//For access handling see also void TubeInterfaceEventHandler::OnIsAccessibleChanged(ULONG eventSinkID)
}

void TubeInterfaceEventHandler::OnRotationSpeedChanged()
{	//Add your code here if Turbopump.RotationSpeed has changed
	//pTubeInterface->Turbopump->RotationSpeed
	//For access handling see also void TubeInterfaceEventHandler::OnIsAccessibleChanged(ULONG eventSinkID)
}

void TubeInterfaceEventHandler::OnLimitedKVMinChanged(ULONG eventSinkID)
{	//Add your code here if StartUp.LimitedKVMin or Refresh.LimitedKVMin has changed
}

void TubeInterfaceEventHandler::OnStopped(ULONG eventSinkID)
{	//Add your code here if InitialStartup has been stopped.
	//if (eventSinkID == initialStartupEventSink.GetEventSinkID())
	//	if (pTubeInterface->InitialStartup->HasReadAccess())
	//		...
}

void TubeInterfaceEventHandler::OnStandbyChanged()
{	//Add your code here if you want to handle a change of standby
	if (pTubeInterface->StandbyEx->HasReadAccess())
	{
		StandbyState state = pTubeInterface->StandbyEx->MonitorValue;
	}
}

void TubeInterfaceEventHandler::OnTimeChanged(ULONG eventSinkID)
{	//Add your code here, if you want to handle a change of TubeTimeSpanCOM
	if (eventSinkID == runningTimerEventSink.GetEventSinkID())
		OnRunningTimerChanged();
}

void TubeInterfaceEventHandler::OnFilamentStatsChanged()
{
	//IFilamentStatsCOMPtr filamentStats = pTubeInterface->FilamentStats;
	//if (filamentStats->HasReadAccess())
	//{
	//	std::wostringstream stream;
	//	stream << "Current filament is in use for ";
	//	stream << (filamentStats->CurrentLifetimeInSeconds / 3600);
	//	stream << " hours.";
	//	MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
	//}
}

void TubeInterfaceEventHandler::OnRunningTimerChanged()
{
	//ITubeTimeSpanCOMPtr runningTimer = pTubeInterface->RunningTimer;
	//if (runningTimer->HasReadAccess())
	//{
	//	std::wostringstream stream;
	//	stream << "Tube is runnig for ";
	//	stream << (runningTimer->TimeInSeconds / 3600);
	//	stream << " hours now.";
	//	MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
	//}
}

asynStatus TubeInterfaceEventHandler::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int addr;
    int function = pasynUser->reason;
    int status=0;
    //static const char *functionName = "readInt32";

    this->getAddress(pasynUser, &addr);

    /*// Analog input function
    if (function == MotorSpeedRBV_)
    {
        // read the motor speed index at start and set the corresponding record
        intVal = MT_GetMotorSpeedIndex();
        *value = intVal;
        setIntegerParam(addr, MotorSpeedRBV_, *value);
    }
    if (function == MovingDone_)
    {
        intVal = MT_IsMotorRunning();
        *value = intVal;
        if (*value)
        {
            *value = 0;
            setIntegerParam(addr, MovingDone_, *value);
        }
        else
        {
            *value = 1;
            setIntegerParam(addr, MovingDone_, *value);
        }
    }

    // Other functions we call the base class method
    else
    {
        status = asynPortDriver::readInt32(pasynUser, value);
    }*/

    callParamCallbacks(addr);

    return (status==0) ? asynSuccess : asynError;
}

asynStatus TubeInterfaceEventHandler::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int addr;
    int function = pasynUser->reason;
    int status = 0;
    static const char *functionName = "writeInt32";

    this->getAddress(pasynUser, &addr);
    setIntegerParam(addr, function, value);

    if (function == TubeInitialize_) {
        if (value) {
            Initialize();
        }
    }

    if (function == TubeStartUp_) {
        if (value) {
            StartUp();
        }
    }
    if (function == TubeXrayOnOff_) {
        if (value == 1) {
            SwitchXRayOn();
        } else {
            SwitchXRayOff();
        }
    }

    callParamCallbacks(addr);

    if (status == 0)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                  "%s:%s, port %s, wrote %d to address %d\n",
                  driverName, functionName, this->portName, value, addr);
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                  "%s:%s, port %s, ERROR writing %d to address %d, status=%d\n",
                  driverName, functionName, this->portName, value, addr, status);
    }
    return (status==0) ? asynSuccess : asynError;
}

asynStatus TubeInterfaceEventHandler::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    int addr;
    int function = pasynUser->reason;
    int status = 0;
    double doubleVal = 0;
    //static const char *functionName = "readFloat64";

    this->getAddress(pasynUser, &addr);

    /*// Analog input function
    if (function == Force_)
    {
        doubleVal = MT_GetForce();
        //std::cout << "Force " << doubleVal << "N" << std::endl;
        *value = doubleVal;
        setDoubleParam(addr, Force_, *value);
    }
    if (function == ExtensionRBV_)
    {
        doubleVal = MT_GetExtension();
        *value = doubleVal;
        setDoubleParam(addr, ExtensionRBV_, *value);
    }
    if (function == StartPosition_)
    {
        doubleVal = MT_GetPosition();
        *value = doubleVal;
        setDoubleParam(addr, StartPosition_, *value);
    }

    // Other functions we call the base class method
    else
    {
        status = asynPortDriver::readFloat64(pasynUser, value);
    }*/

    callParamCallbacks(addr);

    return (status==0) ? asynSuccess : asynError;
}

asynStatus TubeInterfaceEventHandler::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int addr;
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    const char* functionName = "writeFloat64";

    getAddress(pasynUser, &addr);

    // Set the parameter in the parameter library.
    status = setDoubleParam(addr, function, value);

    /*if (function == AbsolutePosition_)
    {
        //std::cout << "Param " << value << std::endl;
        MT_GotoAbsolutePosition(value);
    }
    if (function == Load_)
    {
        MT_GotoLoad(value);
    }
    if (function == Extension_)
    {
        MT_GotoExtension(value);
    }*/

    // Do callbacks so higher layers see any changes
    status = callParamCallbacks();

    if (status)
        asynPrint(pasynUser, ASYN_TRACE_ERROR,
                  "%s:%s, status=%d function=%d, value=%f\n",
                  driverName, functionName, status, function, value);
    else
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
                  "%s:%s: function=%d, value=%f\n",
                  driverName, functionName, function, value);

    return (status==0) ? asynSuccess : asynError;
}

/* Constructor for the TubeInterfaceEventHandler class */
TubeInterfaceEventHandler::TubeInterfaceEventHandler(const char *portName)
    : asynPortDriver(portName, 1, 1,
                     asynInt32Mask | asynFloat64Mask | asynDrvUserMask,  // Interfaces that we implement
                     asynInt32Mask | asynFloat64Mask,    // Interfaces that do callbacks
                     //ASYN_CANBLOCK, 1, /* ASYN_CANBLOCK=1, autoConnect=1 */
                     ASYN_MULTIDEVICE | ASYN_CANBLOCK, 1, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=1, autoConnect=1 */
                     // this is how to start without ASYN_CANBLOCK
                     //ASYN_MULTIDEVICE, 1, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=1, autoConnect=1 */
                     0, 0) /* Default priority and stack size */
{
    createParam(TubeInitialize,			    asynParamInt32,		&TubeInitialize_);
    createParam(TubeInitialize_RBV,			asynParamInt32,		&TubeInitializeRBV_);
    createParam(TubeStartUpState,			asynParamInt32,		&TubeStartUpState_);
    createParam(TubeStartUp,			    asynParamInt32,		&TubeStartUp_);
    createParam(TubeXrayOnOff,			    asynParamInt32,		&TubeXrayOnOff_);
    createParam(TubeXrayOnOff_RBV,			asynParamInt32,		&TubeXrayOnOffRBV_);
    createParam(TubeHighVoltageMonitor,		asynParamFloat64,	&TubeHighVoltageMonitor_);
    createParam(TubeTargetCurrentMonitor,	asynParamFloat64,	&TubeTargetCurrentMonitor_);
    createParam(TubeEmissionCurrentMonitor,	asynParamFloat64,	&TubeEmissionCurrentMonitor_);

    setIntegerParam(TubeInitializeRBV_, 0);
    callParamCallbacks();

    /* launch initialize task */
    epicsThreadCreate("Initialize",
                      epicsThreadPriorityMedium,
                      epicsThreadGetStackSize(epicsThreadStackMedium),
                      c_InitializeTask, this);

}


/** Configuration command, called directly or from iocsh */
extern "C" int TubeInterfaceEventHandlerConfig(const char *portName)
{
    TubeInterfaceEventHandler *pTubeInterfaceEventHandler = new TubeInterfaceEventHandler(portName);
    pTubeInterfaceEventHandler = NULL;  /* This is just to avoid compiler warnings */
    return(asynSuccess);
}

static const iocshArg configArg0 = { "Port name",      iocshArgString};
static const iocshArg * const configArgs[] = {&configArg0};
static const iocshFuncDef configFuncDef = {"TubeInterfaceEventHandlerConfig", 1, configArgs};
static void configCallFunc(const iocshArgBuf *args)
{
    TubeInterfaceEventHandlerConfig(args[0].sval);
}

void drvTubeInterfaceEventHandlerRegister(void)
{
    iocshRegister(&configFuncDef,configCallFunc);
}

extern "C" {
    epicsExportRegistrar(drvTubeInterfaceEventHandlerRegister);
}
