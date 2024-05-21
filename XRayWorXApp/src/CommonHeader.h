#pragma once
#include <iostream>

#import "XRAYWorXBase.tlb" named_guids
#import "XRAYWorXBaseCOM.tlb" named_guids

using namespace XRAYWorXBaseCOM;

class TubeInterfaceEventHandler;

class EventSinkBase : 
	public IUnknown
{
protected:
	TubeInterfaceEventHandler* pTubeInterfaceEventHandler;
	ULONG myEventSinkID;
	ULONG refCount;

public:
	EventSinkBase()
	{
		pTubeInterfaceEventHandler = NULL;
		refCount = 1;
	}

	~EventSinkBase()
	{
	}

	ULONG GetEventSinkID()
	{
		return myEventSinkID;
	}

	void LinkTubeInterfaceEventHandler(TubeInterfaceEventHandler* tieh)
	{
		pTubeInterfaceEventHandler = tieh;
	}

	// IUnknown methods.
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
		if (IsEqualGUID(riid, __uuidof(ITubeCommandSingleEventsPtr)) ||
			IsEqualGUID(riid, IID_IUnknown))
		{
			this->AddRef();
			*ppvObject = this;
			return S_OK;
		}
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	virtual ULONG _stdcall AddRef(void)
	{
		return ++refCount;
	}

	virtual ULONG _stdcall Release(void)
	{
	   if(--refCount <= 0)
	   {  //Delete this;
		  return 0;
	   }
	   return refCount;
	}

	// IDispatch methods.
	virtual HRESULT _stdcall GetTypeInfoCount(UINT *pctinfo)
	{
		if(pctinfo)
		   *pctinfo = 0;
	   return E_NOTIMPL;
	}

	virtual HRESULT _stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
	   return E_NOTIMPL;
	}

	virtual HRESULT _stdcall GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames,
		LCID lcid, DISPID *rgDispId)
	{
	   return E_NOTIMPL;
	}

	virtual HRESULT _stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
		  DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr) 
	{
		return S_OK;
	}
};
