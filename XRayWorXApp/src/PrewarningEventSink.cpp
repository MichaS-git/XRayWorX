#include "stdafx.h"
#include "PrewarningEventSink.h"
#include "TubeInterfaceEventHandler.h"

ULONG PrewarningEventSink::eventSinkID = 0;

void PrewarningEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void PrewarningEventSink::OnEnabledChanged()
{
	pTubeInterfaceEventHandler->OnEnabledChanged(myEventSinkID);
}

void PrewarningEventSink::OnOnChanged()
{
	pTubeInterfaceEventHandler->OnOnChanged(myEventSinkID);
}

void PrewarningEventSink::OnProgressChanged()
{
	pTubeInterfaceEventHandler->OnProgressChanged(myEventSinkID);
}
