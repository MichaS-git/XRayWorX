//#include "StdAfx.h"
#include "TubeTimeSpanEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeTimeSpanEventSink::eventSinkID = 0;

void TubeTimeSpanEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(EventSinkBase::myEventSinkID);
}

void TubeTimeSpanEventSink::OnTimeChanged()
{
	pTubeInterfaceEventHandler->OnTimeChanged(EventSinkBase::myEventSinkID);
}
