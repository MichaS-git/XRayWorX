#include "stdafx.h"
#include "CenterOffsetEventSink.h"
#include "TubeInterfaceEventHandler.h"

ULONG CenterOffsetEventSink::eventSinkID = 0;

void CenterOffsetEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void CenterOffsetEventSink::OnStateChanged()
{
	pTubeInterfaceEventHandler->OnStateChanged(myEventSinkID);
}

void CenterOffsetEventSink::OnDemandLowerLimitChanged()
{
	pTubeInterfaceEventHandler->OnDemandLowerLimitChanged(myEventSinkID);
}

void CenterOffsetEventSink::OnDemandUpperLimitChanged()
{
	pTubeInterfaceEventHandler->OnDemandUpperLimitChanged(myEventSinkID);
}

void CenterOffsetEventSink::OnEnabledChanged()
{
	pTubeInterfaceEventHandler->OnEnabledChanged(myEventSinkID);
}

void CenterOffsetEventSink::OnPlcChanged()
{
	pTubeInterfaceEventHandler->OnPlcChanged(myEventSinkID);
}
