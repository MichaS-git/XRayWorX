//#include "StdAfx.h"
#include "TubeMonBoolEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeMonBoolEventSink::eventSinkID = 0;

void TubeMonBoolEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void TubeMonBoolEventSink::OnMonitorValueChanged()
{
	pTubeInterfaceEventHandler->OnMonitorValueChanged(myEventSinkID);
}
