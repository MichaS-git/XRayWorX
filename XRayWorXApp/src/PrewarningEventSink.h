#pragma once
#include "CommonHeader.h"

class PrewarningEventSink :
	public virtual EventSinkBase,
	public IPrewarningCOMEvents
{
private:
	static ULONG eventSinkID;
	static const ULONG eventSinkIDOffset = 1900000;

public:
	PrewarningEventSink(void)
		: EventSinkBase()
	{
		eventSinkID++;
		EventSinkBase::myEventSinkID = eventSinkIDOffset + eventSinkID;
	}

	~PrewarningEventSink(void) { }

	// IUnknown methods
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
		if (IsEqualGUID(riid, __uuidof(IPrewarningCOMEventsPtr)))
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
		switch (dispIdMember)
		{
		case DispIds_IsAccessibleChanged:
		{
			OnIsAccessibleChanged();
			break;
		}
		case DispIds_EnabledChanged:
		{
			OnEnabledChanged();
			break;
		}
		case DispIds_OnChanged:
		{
			OnOnChanged();
			break;
		}
		case DispIds_ProgressChanged:
		{
			OnProgressChanged();
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
	void OnEnabledChanged();
	void OnOnChanged();
	void OnProgressChanged();
};

