#pragma once

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
#include "CenterOffsetEventSink.h"
#include "ConnectionSampleCOM.h"
#include "ConnectionSampleCOMDlg.h"

using namespace XRAYWorXBaseCOM;
using namespace XRAYWorXBase;

class TubeInterfaceEventHandler
{
private:
	//Member
	CConnectionSampleCOMDlg* dlg;
	//Declare local pointer to ITubeInterface
	// ![Declare ITubeInterfacePtr]
	ITubeInterfacePtr pTubeInterface;
	// ![Declare ITubeInterfacePtr]
	IDataModuleProviderCOMPtr pDataModuleProvider;
	DWORD dwEventCookie;
	TubeInterfaceEventSink tubeInterfaceEventSink;
	TubeCmdSingleEventSink targetCurrentEventSink;
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
	CenterOffsetEventSink centerOffsetEventSink;
	LPCONNECTIONPOINT pIConnectionPoint;
	float highVoltageMonitor;
	float targetCurrentMonitor;

public:
	TubeInterfaceEventHandler(void);
	~TubeInterfaceEventHandler(void);

	void LinkDialog(CConnectionSampleCOMDlg* dialog)
	{
		dlg = dialog;
	}

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

	//Event handler
	void OnTubeStateChanged();
	void OnInitialized();
	void StartConditioning();
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
	//EventHandler for Prewarning
	void OnEnabledChanged(ULONG eventSinkID);
	void OnOnChanged(ULONG eventSinkID);
	void OnProgressChanged(ULONG eventSinkID);
	void OnPlcChanged(ULONG eventSinkID);

private:
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
	void SetDlgInterlock(VARIANT_BOOL interlockClosed);
	void SetDlgVacuumOk(VARIANT_BOOL vacuumOk);
	LPCTSTR GetCoolingOk(ITubeMonitorBool *coolingOk);
	void SetFlashovers(IFlashoverCOM *flashover);
	void FillModeList();
};
