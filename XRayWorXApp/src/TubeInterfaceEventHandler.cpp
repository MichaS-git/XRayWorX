#include "StdAfx.h"
#include <sstream>
#include "CommonHeader.h"
#include "TubeInterfaceEventHandler.h"

TubeInterfaceEventHandler::TubeInterfaceEventHandler()
{
	highVoltageMonitor = 0;
	targetCurrentMonitor = 0;
	// ![Init ITubeInterfacePtr]
	::CoInitialize(NULL);

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
			//loader->SetCustomPaths(_bstr_t("C://Samples//DefaultData"), _bstr_t("C://Samples//AppData"), _bstr_t("C://Samples//UserData"));
			//For customizing paths used by XRAYWorXBaseCOM.dll (AND keep a copy of the log-files at the default path) uncomment the next line and change the path accordingly.
			//loader->SetCustomPathsWithAdditionalDefaultLogging(_bstr_t("C://Samples//DefaultData"), _bstr_t("C://Samples//AppData"), _bstr_t("C://Samples//UserData"));
			_bstr_t ip = loader->DefaultIpAddress->Ip;
			pTubeInterface = loader->GetTubeInterface(ip);
			InitTubeInterfaceEventSink();

			pDataModuleProvider = ((IDataModuleLoaderCOMPtr)loader)->GetDataModuleProvider(ip);
		}
		catch (_com_error& ex)
		{
			MessageBox(NULL, _T("Check TubeLoader.log"), NULL, MB_OK);
		}
	}
	else
		MessageBox(NULL, _T("Error creating instance of TubeLoaderCOM. Check TubeLoader.log"), NULL, MB_OK);

	// ![Init ITubeInterfacePtr]
}

TubeInterfaceEventHandler::~TubeInterfaceEventHandler()
{
	if (pIConnectionPoint)
	{
		pIConnectionPoint->Release();
		pIConnectionPoint->Unadvise(dwEventCookie);
	}
	::CoUninitialize();
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

void TubeInterfaceEventHandler::OnTubeStateChanged()
{
	//add userdefined code here
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
	LinkEventSink(&centerOffsetEventSink, __uuidof(ICenterOffsetCOMEvents), pTubeInterface->CenterOffset);

	//LinkEventSink(&centeringOneTableXEventSink, __uuidof(ICoilDataCOMEvents), pDataModuleProvider->GetCenteringOneTableX());
	//LinkEventSink(&centeringOneTableYEventSink, __uuidof(ICoilDataCOMEvents), pDataModuleProvider->GetCenteringOneTableY());
	//LinkEventSink(&vacuumDataEventSink, __uuidof(IDataModuleCOMEvents), pDataModuleProvider->GetDataModule(DataModuleName_Vacuum));

	SetDlgInterlock(pTubeInterface->Interlock->MonitorValue);
	SetDlgVacuumOk(pTubeInterface->VacuumOk->MonitorValue);

	//Sample how to access tube's serial number in an easy way
	_bstr_t serial = pTubeInterface->Serial;
	_bstr_t dateOfManufacture = pTubeInterface->DateOfManufacture;
	std::wostringstream stream;
	stream << "Serialnumber: ";
	stream << serial;
	stream << " (";
	stream << dateOfManufacture;
	stream << ")";
	MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
}

void TubeInterfaceEventHandler::OnTubeInterfaceError(ULONG errorCode)
{
	//add userdefined code here
	dlg->SetError(errorCode);
}

void TubeInterfaceEventHandler::OnIsAccessibleChanged(ULONG eventSinkID)
{
	//add user defined code here
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageAccessibleChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentAccessibleChanged();
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
	else if (eventSinkID == filamentAdjustInfoEventSink.GetEventSinkID())
		OnRemainingTimeChanged(eventSinkID);
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
			dlg->SetHighVoltageMonitor(highVoltageMonitor);
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

void TubeInterfaceEventHandler::OnMonitorValueChanged(ULONG eventSinkID)
{
	if (eventSinkID == highVoltageEventSink.GetEventSinkID())
		OnHighVoltageMonitorValueChanged();
	else if (eventSinkID == targetCurrentEventSink.GetEventSinkID())
		OnTargetCurrentMonitorValueChanged();
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
			dlg->SetTargetCurrentMonitor(targetCurrentMonitor);
			long targetErrorCode = targetCurrent->ErrorCode;
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
	if (pTubeInterface->StartUp->State == CommandStates_OK)
		dlg->m_edStartUpState.SetWindowTextW((LPCTSTR)_T("OK"));
	else if (pTubeInterface->StartUp->State == CommandStates_Acknowledged)
		dlg->m_edStartUpState.SetWindowTextW((LPCTSTR)_T("Acknowledged"));
	else if (pTubeInterface->StartUp->State == CommandStates_Busy)
		dlg->m_edStartUpState.SetWindowTextW((LPCTSTR)_T("Busy"));
	else if (pTubeInterface->StartUp->State == CommandStates_Warning)
		dlg->m_edStartUpState.SetWindowTextW((LPCTSTR)_T("Warning"));
	else if (pTubeInterface->StartUp->State == CommandStates_Error)
		dlg->m_edStartUpState.SetWindowTextW((LPCTSTR)_T("Error"));
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
	if (!pTubeInterface->XRayOn->MonitorValue && pTubeInterface->XRayOff->MonitorValue)
		dlg->m_lblXrayState.SetWindowTextW((LPCTSTR)_T("X-RAY IS OFF"));
	else
		dlg->m_lblXrayState.SetWindowTextW((LPCTSTR)_T("X-RAY IS ON"));
}

void TubeInterfaceEventHandler::OnInterlockMonitorValueChanged()
{	//user defined code
	SetDlgInterlock(pTubeInterface->Interlock->MonitorValue);
}

void TubeInterfaceEventHandler::SetDlgInterlock(VARIANT_BOOL interlockClosed)
{
	if (interlockClosed)
		dlg->m_edInterlock.SetWindowTextW((LPCTSTR)_T("CLOSED"));
	else
		dlg->m_edInterlock.SetWindowTextW((LPCTSTR)_T("OPEN"));
}

void TubeInterfaceEventHandler::OnVacuumOkMonitorValueChanged()
{	//user defined code
	SetDlgVacuumOk(pTubeInterface->VacuumOk->MonitorValue);
}

void TubeInterfaceEventHandler::SetDlgVacuumOk(VARIANT_BOOL vacuumOk)
{
	if (vacuumOk)
		dlg->m_edVacuum.SetWindowTextW((LPCTSTR)_T("OK"));
	else
		dlg->m_edVacuum.SetWindowTextW((LPCTSTR)_T("INSUFFICIENT"));
}

void TubeInterfaceEventHandler::OnCooling1OkMonitorChanged()
{	//user defined code
	LPCTSTR coolingOk = GetCoolingOk(pTubeInterface->CoolingOk);
	dlg->m_edCooling1.SetWindowTextW(coolingOk);
}

void TubeInterfaceEventHandler::OnCooling2OkMonitorChanged()
{	//user defined code
	LPCTSTR coolingOk = GetCoolingOk(pTubeInterface->CoolingTwoOk);
	dlg->m_edCooling2.SetWindowTextW(coolingOk);
}

LPCTSTR TubeInterfaceEventHandler::GetCoolingOk(ITubeMonitorBool *coolingOk)
{
	if (coolingOk->HasReadAccess())
	{
		if (coolingOk->MonitorValue)
			return (LPCTSTR)_T("GOOD");
		return (LPCTSTR)_T("BAD");
	}
	return (LPCTSTR)_T("NOT ACCESSIBLE");
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
		dlg->SetFlashoverCount(flashover->Count);
	else
		dlg->m_edFlashover.SetWindowTextW((LPCTSTR)_T("NOT ACCESSIBLE"));
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
		OnModeListChanged();
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

void TubeInterfaceEventHandler::OnModeListChanged()
{
	FillModeList();
}

void UnpackBstrArrayHelper(VARIANT* pvarArrayIn, CStringArray* pstrarrValues)
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
}

void TubeInterfaceEventHandler::OnModeChanged()
{
	if (pTubeInterface->Mode->HasReadAccess())
	{
		int index = (int)pTubeInterface->Mode->MonitorValue;
			dlg->cbModes.SetCurSel(index);
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
	MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
}

void TubeInterfaceEventHandler::OnSaved(ULONG eventSinkID)
{
	if (eventSinkID == vacuumDataEventSink.GetEventSinkID())
	{
		std::wostringstream stream;
		stream << "Vacuum data module saved.";
		MessageBox(NULL, stream.str().c_str(), NULL, MB_OK);
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

void TubeInterfaceEventHandler::OnEnabledChanged(ULONG eventSinkID)
{
	//Add your code here, if you want to handle a change of EnabledChanged event of PrewarningCOM
}

void TubeInterfaceEventHandler::OnOnChanged(ULONG eventSinkID)
{
	//Add your code here, if you want to handle a change of e.g. OnChanged event of PrewarningCOM
}

void TubeInterfaceEventHandler::OnProgressChanged(ULONG eventSinkID)
{
	//Add your code here, if you want to handle a change of e.g. ProgressChanged event of PrewarningCOM
}

void TubeInterfaceEventHandler::OnPlcChanged(ULONG eventSinkID)
{
	//Add your code here, if you want to handle a change of e.g. PlcChanged event of CenterOffsetCOM
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

void TubeInterfaceEventHandler::StartConditioning()
{
	IConditioningSettingsCOMPtr condSettings = NULL;
	condSettings.CreateInstance(__uuidof(ConditioningSettingsCOM));
	condSettings->DurationInMinutes = 15;
	condSettings->HighVoltage = 229;
	condSettings->EmissionCurrent = 50;
	condSettings->MaxFlashovers = 30;
	condSettings->AlternatingConditioningEnabled = VARIANT_FALSE;
	condSettings->FilamentAdjustEnabled = VARIANT_FALSE;
	condSettings->LeakageCurrentTestEnabled = VARIANT_FALSE;
	condSettings->HighVoltageStart = 30;
	condSettings->HighVoltageStepDelayInSeconds = 1;
	condSettings->HighVoltageStepWidth = 1;
	pTubeInterface->Conditioning->Start(condSettings);
}
