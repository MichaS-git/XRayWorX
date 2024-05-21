//#include "StdAfx.h"
#include "TubeAutoCmdEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeAutoCmdEventSink::eventSinkID = 0;

void TubeAutoCmdEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void TubeAutoCmdEventSink::OnMonitorValueChanged()
{
	pTubeInterfaceEventHandler->OnMonitorValueChanged(myEventSinkID);
}

void TubeAutoCmdEventSink::OnStateChanged()
{
	pTubeInterfaceEventHandler->OnStateChanged(myEventSinkID);
}

void TubeAutoCmdEventSink::OnIsActiveChanged()
{
	pTubeInterfaceEventHandler->OnIsActiveChanged(myEventSinkID);
}
