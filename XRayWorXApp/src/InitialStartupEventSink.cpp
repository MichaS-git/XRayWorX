//#include "StdAfx.h"
#include "InitialStartupEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG InitialStartupEventSink::eventSinkID = 0;

void InitialStartupEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void InitialStartupEventSink::OnStopped()
{
	pTubeInterfaceEventHandler->OnStopped(myEventSinkID);
}
