//#include "StdAfx.h"
#include "StandbyEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG StandbyEventSink::eventSinkID = 0;

void StandbyEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void StandbyEventSink::OnMonitorValueChanged()
{
	pTubeInterfaceEventHandler->OnMonitorValueChanged(myEventSinkID);
}

void StandbyEventSink::OnStateChanged()
{
	pTubeInterfaceEventHandler->OnStateChanged(myEventSinkID);
}
