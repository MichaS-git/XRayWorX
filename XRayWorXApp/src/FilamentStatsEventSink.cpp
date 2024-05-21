//#include "StdAfx.h"
#include "FilamentStatsEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG FilamentStatsEventSink::eventSinkID = 0;

void FilamentStatsEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(EventSinkBase::myEventSinkID);
}

void FilamentStatsEventSink::OnChanged()
{
	pTubeInterfaceEventHandler->OnChanged(EventSinkBase::myEventSinkID);
}
