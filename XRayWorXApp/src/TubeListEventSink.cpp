//#include "StdAfx.h"
#include "TubeListEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TubeListEventSink::eventSinkID = 0;

void TubeListEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void TubeListEventSink::OnChanged()
{
	pTubeInterfaceEventHandler->OnChanged(myEventSinkID);
}

void TubeListEventSink::OnListChanged()
{
	pTubeInterfaceEventHandler->OnListChanged(myEventSinkID);
}
