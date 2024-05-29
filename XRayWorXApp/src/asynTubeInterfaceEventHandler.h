#include <iostream>
#include <conio.h>
#include <sstream>

#include <iocsh.h>
#include <asynPortDriver.h>
#include <epicsExport.h>

#include "CommonHeader.h"
#include "TubeInterfaceEventSink.h"
#include "TubeCmdSingleEventSink.h"
#include "TubeCmdBoolEventSink.h"
#include "TubeMonBoolEventSink.h"
#include "TubeAutoCmdEventSink.h"
#include "TubeCmdXocEventSink.h"
#include "TubeFlashoverEventSink.h"
#include "TubeListMonStringEventSink.h"
#include "AutoCommandInfoEventSink.h"
#include "CommandInfoMonitorEventSink.h"
#include "TubeServiceMonitorEventSink.h"
#include "CoilDataEventSink.h"
#include "DataModuleEventSink.h"
#include "LimitableAutoCommandEventSink.h"
#include "TurbopumpEventSink.h"
#include "TubeListEventSink.h"
#include "InitialStartupEventSink.h"
#include "TubeTimeSpanEventSink.h"
#include "FilamentStatsEventSink.h"
#include "StandbyEventSink.h"

using namespace XRAYWorXBaseCOM;
using namespace XRAYWorXBase;

/* Class definition for the Tube Interface */
class TubeInterfaceEventHandler : public asynPortDriver
{
public:
    TubeInterfaceEventHandler(const char *portName);

    /* These are the methods that we override from asynPortDriver */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);

	//TubeInterfaceEventHandler(void);
	//~TubeInterfaceEventHandler(void);

	void InitializeTask();

	float GetHighVoltageMonitor()
	{
		UpdateHighVoltageMonitor();
		return highVoltageMonitor;
	}

	float GetTargetCurrentMonitor()
	{
		UpdateTargetCurrentMonitor();
		return targetCurrentMonitor;
	}

	// ![SwitchXrayOn]
	void SwitchXRayOn()
	{
		pTubeInterface->XRayOn->PcDemandValue = true;
	}
	// ![SwitchXrayOn]

	// ![SwitchXrayOff]
	void SwitchXRayOff()
	{
		pTubeInterface->XRayOff->PcDemandValue = true;
	}
	// ![SwitchXrayOff]

    void StartUp()
    {
        pTubeInterface->StartUp->PcDemandValue = true;
    }

	void SetHighVoltage(float value)
	{
		pTubeInterface->HighVoltage->PcDemandValue = value;
	}

	void SetTargetCurrent(float value)
	{
		if (pTubeInterface->XrayOutControl->MonitorValue != XrayOutControls_TargetCurrentControl)
			pTubeInterface->XrayOutControl->PcDemandValue = XrayOutControls_TargetCurrentControl;
		pTubeInterface->TargetCurrent->PcDemandValue = value;
	}

	void SetEmissionCurrent(float value)
	{
		pTubeInterface->EmissionCurrent->PcDemandValue = value;
	}

	void SetTargetPower(float value)
	{
		pTubeInterface->TargetPower->PcDemandValue = value;
	}

	//Event handler
	void OnTubeStateChanged();
	void OnInitialized();
	void OnTubeInterfaceError(ULONG errorCode);
	void OnIsAccessibleChanged(ULONG eventSinkID);
	void OnMonitorValueChanged(ULONG eventSinkID);
	void OnDemandLowerLimitChanged(ULONG eventSinkID);
	void OnDemandUpperLimitChanged(ULONG eventSinkID);
	void OnStateChanged(ULONG eventSinkID);
	void OnPlcDemandValueChanged(ULONG eventSinkID);
	void OnIsActiveChanged(ULONG eventSinkID);
	void OnListChanged(ULONG eventSinkID);
	void OnChanged(ULONG eventSinkID);
	//Flashover event handler
	void OnFlashoverChanged();
	void OnLockingXrayChanged();
	//EventHandler for AutoFunctionInfo and TubeServiceMonitors
	void OnActionInfoChanged(ULONG eventSinkID);
	void OnRemainingTimeChanged(ULONG eventSinkID);
	void OnServiceStateChanged(ULONG eventSinkID);
	//EventHandler for coils and data modules
	void OnDataUpdated(ULONG eventSinkID);
	void OnError(ULONG eventSinkID, ULONG errorCode, BSTR errorText);
	void OnSaved(ULONG eventSinkID);
	void OnUpdating(ULONG eventSinkID);
	//EventHandler for TurbopumpCommand
	void OnIsSwitchedOnChanged();
	void OnIsVentilatedChanged();
	void OnRotationSpeedChanged();
	//Additional EventHandler for StartUp and Refresh
	void OnLimitedKVMinChanged(ULONG eventSinkID);
	//EventHandler for InitialStartup
	void OnStopped(ULONG eventSinkID);
	//EventHandler for TubeTimeSpan
	void OnTimeChanged(ULONG eventSinkID);

protected:

    int TubeInitializeRBV_;
    int TubeStartUpState_;
    int TubeStartUp_;
    int TubeXrayOnOff_;
    int TubeXrayOnOffRBV_;
    int TubeHighVoltageDemand_;
    int TubeHighVoltageMonitor_;
    int TubeTargetCurrentDemand_;
    int TubeTargetCurrentMonitor_;
    int TubeEmissionCurrentDemand_;
    int TubeEmissionCurrentMonitor_;
    int TubeTargetPowerDemand_;
    int TubeTargetPowerMonitor_;

private:

	//Member
	//Declare local pointer to ITubeInterface
	// ![Declare ITubeInterfacePtr]
	ITubeInterfacePtr pTubeInterface;
	// ![Declare ITubeInterfacePtr]
	IDataModuleProviderCOMPtr pDataModuleProvider;
	DWORD dwEventCookie;
	TubeInterfaceEventSink tubeInterfaceEventSink;
	TubeCmdSingleEventSink targetCurrentEventSink;
	TubeCmdSingleEventSink emissionCurrentEventSink;
	TubeCmdSingleEventSink targetPowerEventSink;
	TubeCmdSingleEventSink highVoltageEventSink;
	TubeCmdBoolEventSink xRayOffEventSink;
	TubeCmdBoolEventSink xRayOnEventSink;
	TubeMonBoolEventSink interlockEventSink;
	TubeMonBoolEventSink vacuumOkEventSink;
	TubeMonBoolEventSink cooling1OkEventSink;
	TubeMonBoolEventSink cooling2OkEventSink;
	LimitableAutoCommandEventSink startUpEventSink;
	TubeAutoCmdEventSink filamentAdjustEventSink;
	TubeCmdXocEventSink xRayOutControlEventSink;
	TubeFlashoverEventSink flashoverEventSink;
	TubeListMonStringEventSink modeListEventSink;
	TubeCmdSingleEventSink modeEventSink;
	AutoCommandInfoEventSink autoCommandInfoEventSink;
	CommandInfoMonitorEventSink filamentAdjustInfoEventSink;
	TubeServiceMonitorEventSink filamentLifetimeEventSink;
	CoilDataEventSink centeringOneTableXEventSink;
	CoilDataEventSink centeringOneTableYEventSink;
	DataModuleEventSink systemDataEventSink;
	DataModuleEventSink vacuumDataEventSink;
	TurbopumpEventSink turbopumpEventSink;
	TubeListEventSink defocListEventSink;
	InitialStartupEventSink initialStartupEventSink;
	TubeTimeSpanEventSink runningTimerEventSink;
	FilamentStatsEventSink filamentStatsEventSink;
	StandbyEventSink standbyEventSink;
	LPCONNECTIONPOINT pIConnectionPoint;
    float highVoltageMonitor;
    float targetCurrentMonitor;
    float emissionCurrentMonitor;
    float targetPowerMonitor;

	HRESULT LinkEventSink(IUnknown* eventSink, const IID &riid);
	HRESULT InitTubeInterfaceEventSink();
	void LinkEventSink(EventSinkBase* eventSink, const IID &eventsInterfacePtr, IUnknown* eventSource);

	//HighVoltage event handler
	void OnHighVoltageAccessibleChanged();
	void OnHighVoltageMonitorValueChanged();
	void OnHighVoltageDemandLowerLimitChanged();
	void OnHighVoltageDemandUpperLimitChanged();
	void OnHighVoltageStateChanged();
	void OnHighVoltagePlcDemandValueChanged();
	//TargetCurrent event handler
	void OnTargetCurrentAccessibleChanged();
	void OnTargetCurrentMonitorValueChanged();
	void OnTargetCurrentDemandLowerLimitChanged();
	void OnTargetCurrentDemandUpperLimitChanged();
	void OnTargetCurrentStateChanged();
	void OnTargetCurrentPlcDemandValueChanged();
	//EmissionCurrent event handler
	void OnEmissionCurrentAccessibleChanged();
	void OnEmissionCurrentMonitorValueChanged();
	//TargetPower event handler
	void OnTargetPowerAccessibleChanged();
	void OnTargetPowerMonitorValueChanged();
	//XRayOn/Off event handler
	void OnXRayOnOffMonitorValueChanged();
	//Interlock event handler
	void OnInterlockMonitorValueChanged();
	//VacuumOk event handler
	void OnVacuumOkMonitorValueChanged();
	//StartUp event handler
	void OnStartUpStateChanged();
	//Cooling1Ok event handler
	void OnCooling1OkMonitorChanged();
	//Cooling2Ok event handler
	void OnCooling2OkMonitorChanged();
	//Mode/ModeList event handler
	void OnModeListChanged();
	void OnModeChanged();
	//AutoCommandInfo event handler
	void AutoCommandInfo_AccessibleChanged();
	//DefocList event handler
	void OnDefocListChanged();
	//Standby event handler
	void OnStandbyChanged();
	//Handler for TubeStatistics
	void OnFilamentStatsChanged();
	void OnRunningTimerChanged();

	void UpdateHighVoltageMonitor();
	void UpdateTargetCurrentMonitor();
	void UpdateEmissionCurrentMonitor();
	void UpdateTargetPowerMonitor();
	void SetDlgInterlock(VARIANT_BOOL interlockClosed);
	void SetDlgVacuumOk(VARIANT_BOOL vacuumOk);
	LPCTSTR GetCoolingOk(ITubeMonitorBool *coolingOk);
	void SetFlashovers(IFlashoverCOM *flashover);
	void FillModeList();
};
