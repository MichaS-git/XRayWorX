//#include "StdAfx.h"
#include "TubeCmdSingleEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeCmdSingleEventSink::eventSinkID = 0;

void TubeCmdSingleEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(EventSinkBase::myEventSinkID);
}

void TubeCmdSingleEventSink::OnMonitorValueChanged()
{
	pTubeInterfaceEventHandler->OnMonitorValueChanged(EventSinkBase::myEventSinkID);
}

void TubeCmdSingleEventSink::OnDemandLowerLimitChanged()
{
	pTubeInterfaceEventHandler->OnDemandLowerLimitChanged(EventSinkBase::myEventSinkID);
}

void TubeCmdSingleEventSink::OnDemandUpperLimitChanged()
{
	pTubeInterfaceEventHandler->OnDemandUpperLimitChanged(EventSinkBase::myEventSinkID);
}

void TubeCmdSingleEventSink::OnStateChanged()
{
	pTubeInterfaceEventHandler->OnStateChanged(EventSinkBase::myEventSinkID);
}

void TubeCmdSingleEventSink::OnPlcDemandValueChanged()
{
	pTubeInterfaceEventHandler->OnPlcDemandValueChanged(EventSinkBase::myEventSinkID);
}
