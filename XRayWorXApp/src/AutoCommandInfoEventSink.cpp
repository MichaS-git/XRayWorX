//#include "StdAfx.h"
#include "AutoCommandInfoEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG AutoCommandInfoEventSink::eventSinkID = 0;

void AutoCommandInfoEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}
