//#include "StdAfx.h"
#include "TubeInterfaceEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

void TubeInterfaceEventSink::OnInitialized()
{
	pTubeInterfaceEventHandler->OnInitialized();
}

void TubeInterfaceEventSink::OnTubeStateChanged()
{
	pTubeInterfaceEventHandler->OnTubeStateChanged();
}

void TubeInterfaceEventSink::OnTubeInterfaceError(ULONG errorCode)
{
	pTubeInterfaceEventHandler->OnTubeInterfaceError(errorCode);
}
