#pragma once
#include "CommonHeader.h"

class CoilDataEventSink :
	public virtual EventSinkBase,
	public ICoilDataCOMEvents
{
private:
	static ULONG eventSinkID;
	static const ULONG eventSinkIDOffset = 1400000;

public:
	CoilDataEventSink()
		: EventSinkBase()
	{
		eventSinkID++;
		EventSinkBase::myEventSinkID = eventSinkIDOffset + eventSinkID;
	}

	~CoilDataEventSink()
	{
	}

	// IUnknown methods.
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
	   if(IsEqualGUID(riid, __uuidof(ICoilDataCOMEventsPtr)))
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
			case DispIds_DataUpdated:
			{
				IResultCOMPtr res = pDispParams->rgvarg[0].pdispVal;
				if (res->IsSuccess == VARIANT_TRUE)
					OnDataUpdated();
				break;
			}
			case DispIds_IsAccessibleChanged:
			{
				OnIsAccessibleChanged();
				break;
			}
			case DispIds_Updating:
			{
				OnUpdating();
				break;
			}
			default:
			{
				EventSinkBase::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
				break;
			}
		}
	    return S_OK;
	}

	void OnDataUpdated();
	void OnIsAccessibleChanged();
	void OnUpdating();
};
