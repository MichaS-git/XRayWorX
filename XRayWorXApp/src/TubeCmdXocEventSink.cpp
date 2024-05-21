//#include "StdAfx.h"
#include "TubeCmdXocEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeCmdXocEventSink::eventSinkID = 0;

void TubeCmdXocEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void TubeCmdXocEventSink::OnMonitorValueChanged()
{
	pTubeInterfaceEventHandler->OnMonitorValueChanged(myEventSinkID);
}

void TubeCmdXocEventSink::OnStateChanged()
{
	pTubeInterfaceEventHandler->OnStateChanged(myEventSinkID);
}

void TubeCmdXocEventSink::OnPlcDemandValueChanged()
{
	pTubeInterfaceEventHandler->OnPlcDemandValueChanged(myEventSinkID);
}
