#pragma once
#include "CommonHeader.h"

class TubeTimeSpanEventSink :
	public virtual EventSinkBase,
	public ITubeTimeSpanCOMEvents
{
private:
	static ULONG eventSinkID;
	static const ULONG eventSinkIDOffset = 1700000;

public:
	TubeTimeSpanEventSink(void)
		: EventSinkBase()
	{
		eventSinkID++;
		EventSinkBase::myEventSinkID = eventSinkIDOffset + eventSinkID;
	}
	~TubeTimeSpanEventSink(void)
	{
	}

	// IUnknown methods.
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
	   if(IsEqualGUID(riid, __uuidof(ITubeTimeSpanCOMEventsPtr)))
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
			case DispIds_TimeChanged:
			{
				OnTimeChanged();
				break;
			}
			default:
				break;
	   }
	   return S_OK;
	}

private:
	void OnIsAccessibleChanged();
	void OnTimeChanged();
};
