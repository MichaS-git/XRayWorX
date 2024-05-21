//#include "StdAfx.h"
#include "TubeServiceMonitorEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeServiceMonitorEventSink::eventSinkID = 0;

void TubeServiceMonitorEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void TubeServiceMonitorEventSink::OnRemainingTimeChanged()
{
	pTubeInterfaceEventHandler->OnRemainingTimeChanged(myEventSinkID);
}

void TubeServiceMonitorEventSink::OnServiceStateChanged()
{
	pTubeInterfaceEventHandler->OnServiceStateChanged(myEventSinkID);
}
