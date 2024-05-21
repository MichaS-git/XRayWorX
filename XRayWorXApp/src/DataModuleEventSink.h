#pragma once
#include "CommonHeader.h"

class DataModuleEventSink :
	public virtual EventSinkBase,
	public IDataModuleCOMEvents
{
private:
	static ULONG eventSinkID;
	static const ULONG eventSinkIDOffset = 1300000;

public:
	DataModuleEventSink()
		: EventSinkBase()
	{
		eventSinkID++;
		EventSinkBase::myEventSinkID = eventSinkIDOffset + eventSinkID;
	}

	~DataModuleEventSink()
	{
	}

	// IUnknown methods.
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
	   if(IsEqualGUID(riid, __uuidof(IDataModuleCOMEventsPtr)))
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
			case DispIds_Saved:
			{
				IResultCOMPtr res = pDispParams->rgvarg[0].pdispVal;
				if (res->IsSuccess == VARIANT_TRUE)
					OnSaved();
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

	void OnIsAccessibleChanged();
	void OnDataUpdated();
	void OnError(ULONG errorCode, BSTR errorText);
	void OnSaved();
	void OnUpdating();
};
