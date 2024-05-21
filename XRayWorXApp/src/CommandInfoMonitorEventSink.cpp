//#include "StdAfx.h"
#include "CommandInfoMonitorEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG CommandInfoMonitorEventSink::eventSinkID = 0;

void CommandInfoMonitorEventSink::OnActionInfoChanged()
{
	pTubeInterfaceEventHandler->OnActionInfoChanged(myEventSinkID);
}

void CommandInfoMonitorEventSink::OnRemainingTimeChanged()
{
	pTubeInterfaceEventHandler->OnRemainingTimeChanged(myEventSinkID);
}
