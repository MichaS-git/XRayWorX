//#include "StdAfx.h"
#include "DataModuleEventSink.h"
#include "asynTubeInterfaceEventHandler.h"

ULONG DataModuleEventSink::eventSinkID = 0;

void DataModuleEventSink::OnDataUpdated()
{
	pTubeInterfaceEventHandler->OnDataUpdated(myEventSinkID);
}

void DataModuleEventSink::OnError(ULONG errorCode, BSTR errorText)
{
	pTubeInterfaceEventHandler->OnError(myEventSinkID, errorCode, errorText);
}

void DataModuleEventSink::OnIsAccessibleChanged()
{
	pTubeInterfaceEventHandler->OnIsAccessibleChanged(myEventSinkID);
}

void DataModuleEventSink::OnSaved()
{
	pTubeInterfaceEventHandler->OnSaved(myEventSinkID);
}

void DataModuleEventSink::OnUpdating()
{
	pTubeInterfaceEventHandler->OnUpdating(myEventSinkID);
}
