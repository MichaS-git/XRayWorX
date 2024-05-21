#pragma once
#include "CommonHeader.h"

class TubeCmdSingleEventSink :
	public virtual EventSinkBase,
	public ITubeCommandSingleEvents
{
private:
	//Member
	static ULONG eventSinkID;
	static const ULONG eventSinkIDOffset = 0;

public:
	TubeCmdSingleEventSink()
		: EventSinkBase()
	{
		eventSinkID++;
		EventSinkBase::myEventSinkID = eventSinkIDOffset + eventSinkID;
	}

	~TubeCmdSingleEventSink()
	{
	}

	// IUnknown methods.
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
		if (IsEqualGUID(riid, __uuidof(ITubeCommandSingleEventsPtr)))
		{
			this->AddRef();
			*ppvObject = this;
			return S_OK;
		}
		return EventSinkBase::QueryInterface(riid, ppvObject);
	}

	virtual ULONG _stdcall AddRef(void)
	{
		return EventSinkBase::AddRef();
	}

	virtual ULONG _stdcall Release(void)
	{
		return EventSinkBase::Release();
	}

	// IDispatch methods.
	virtual HRESULT _stdcall GetTypeInfoCount(UINT *pctinfo)
	{
		return EventSinkBase::GetTypeInfoCount(pctinfo);
	}

	virtual HRESULT _stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		return EventSinkBase::GetTypeInfo(iTInfo, lcid, ppTInfo);
	}

	virtual HRESULT _stdcall GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames,
		LCID lcid, DISPID *rgDispId)
	{
		return EventSinkBase::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);
	}

	virtual HRESULT _stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
		  DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) 
	{
		switch(dispIdMember)
		{
			case DispIds_IsAccessibleChanged:
			{
				OnIsAccessibleChanged();
				break;
			}
			case DispIds_MonitorValueChanged:
			{
				OnMonitorValueChanged();
				break;
			}
			case DispIds_DemandLowerLimitChanged:
			{
				OnDemandLowerLimitChanged();
				break;
			}
			case DispIds_DemandUpperLimitChanged:
			{
				OnDemandUpperLimitChanged();
				break;
			}
			case DispIds_StateChanged:
			{
				OnStateChanged();
				break;
			}
			case DispIds_PlcDemandValueChanged:
			{
				OnPlcDemandValueChanged();
				break;
			}
			default:
				break;
	   }
	   return S_OK;
	}

private:
	void OnIsAccessibleChanged();
	void OnMonitorValueChanged();
	void OnDemandLowerLimitChanged();
	void OnDemandUpperLimitChanged();
	void OnStateChanged();
	void OnPlcDemandValueChanged();
};
