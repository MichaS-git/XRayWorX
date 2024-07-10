#pragma once
#include "CommonHeader.h"

/// <summary>Event sink for events from TubeInterfaceCOM.</summary>
/// <remarks>For simple handling the events will call a corresponding
/// function from TubeInterfaceEventHandler.</remarks>
class TubeInterfaceEventSink :
	public ITubeInterfaceEvents
{
private:
	//Member
	TubeInterfaceEventHandler* pTubeInterfaceEventHandler;

public:
	ULONG refCount;
	
	TubeInterfaceEventSink()
	{
	   refCount = 1;
	   pTubeInterfaceEventHandler = NULL;
	}

	~TubeInterfaceEventSink()
	{
	}

	void LinkTubeInterfaceEventHandler(TubeInterfaceEventHandler* tieh)
	{
		pTubeInterfaceEventHandler = tieh;
	}

	// IUnknown methods.
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject)
	{
	   if(IsEqualGUID(riid, __uuidof(ITubeInterfaceEventsPtr)) ||
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
		switch(dispIdMember)
		{
			case DispIds_Initialized:
			{
				OnInitialized();
				break;
			}
			case DispIds_TubeStateChanged:
			{
				OnTubeStateChanged();
				break;
			}
			case DispIds_TubeInterfaceError:
			{
				ULONG errorCode = pDispParams->rgvarg[0].ulVal;
				OnTubeInterfaceError(errorCode);
				break;
			}
	   }
	   return S_OK;
	}

	void OnInitialized();
	void OnTubeStateChanged();
	void OnTubeInterfaceError(ULONG errorCode);
};