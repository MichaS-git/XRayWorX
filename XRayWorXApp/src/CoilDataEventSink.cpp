//#include "StdAfx.h"
#include "CoilDataEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG CoilDataEventSink::eventSinkID = 0;

void CoilDataEventSink::OnDataUpdated()
{
	pTubeInterfaceEventHandler->OnDataUpdated(myEventSinkID);
}

void CoilDataEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void CoilDataEventSink::OnUpdating()
{
	pTubeInterfaceEventHandler->OnUpdating(myEventSinkID);
}
