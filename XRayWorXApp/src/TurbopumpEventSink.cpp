//#include "StdAfx.h"
#include "TurbopumpEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG TurbopumpEventSink::eventSinkID = 0;

void TurbopumpEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void TurbopumpEventSink::OnIsSwitchedOnChanged()
{
	pTubeInterfaceEventHandler->OnIsSwitchedOnChanged();
}

void TurbopumpEventSink::OnIsVentilatedChanged()
{
	pTubeInterfaceEventHandler->OnIsVentilatedChanged();
}

void TurbopumpEventSink::OnRotationSpeedChanged()
{
	pTubeInterfaceEventHandler->OnRotationSpeedChanged();
}

void TurbopumpEventSink::OnStateChanged()
{
	pTubeInterfaceEventHandler->OnStateChanged(myEventSinkID);
}
