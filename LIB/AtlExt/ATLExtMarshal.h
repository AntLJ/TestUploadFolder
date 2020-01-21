/*
	Copyright(c) DevInstinct inc. 2002
	http://www.devinstinct.com

	Written by Martin Lapierre

	All rights reserved. No warrantees extended. Use at your own risk.
	You may use this code in your product in compiled form without royalty.
	You may not distribute it in source code form as part of a class library
	or product without the express written consent of DevInstinct inc.
*/

#pragma once

#include "ATLExtPersist.h"

namespace ATLExt
{
//////////////////////
// Marshalling options
//////////////////////
enum _MSHOPT
{
	MSHOPT_LOCAL            = 0x1, 
	MSHOPT_NOSHAREDMEM      = 0x2, 
	MSHOPT_DIFFERENTMACHINE = 0x4, 
	MSHOPT_INPROC           = 0x8 
}; 
#define MSH_ISMBV( dwDestContext, mshlopt ) \
	((dwDestContext == MSHCTX_LOCAL && (mshlopt) & MSHOPT_LOCAL) || \
	 (dwDestContext == MSHCTX_NOSHAREDMEM && (mshlopt) & MSHOPT_NOSHAREDMEM) || \
	 (dwDestContext == MSHCTX_DIFFERENTMACHINE && (mshlopt) & MSHOPT_DIFFERENTMACHINE) || \
	 (dwDestContext == MSHCTX_INPROC && (mshlopt) & MSHOPT_INPROC))


/////////////////////////////////////////////////
// AtlIMarshal_XXXX: global marshalling functions
/////////////////////////////////////////////////

template <class T>
inline HRESULT AtlIMarshal_GetUnmarshalClass
(
	REFIID riid, 
	void __RPC_FAR *pv, 
	DWORD dwDestContext, 
	void __RPC_FAR *pvDestContext, 
	DWORD mshlflags, 
	CLSID __RPC_FAR *pCid,
	T *pT,
	DWORD mshlopt = MSHOPT_NOSHAREDMEM|MSHOPT_DIFFERENTMACHINE
)
{
	ATLASSERT(pT != NULL);

	if (MSH_ISMBV(dwDestContext, mshlopt))
	{
		// Return the component's CLSID.
		*pCid = pT->GetObjectCLSID();
		return S_OK;
	}
	else 
	{
		// Use standard marshalling.
		CComPtr<IMarshal> ccpIMarshal;
		CComPtr<IUnknown> ccpIUnk(static_cast<IUnknown*>(pv));
		TestHR(CoGetStandardMarshal(riid, ccpIUnk, dwDestContext, pvDestContext, mshlflags, &ccpIMarshal))
		return ccpIMarshal->GetUnmarshalClass(riid, pv, dwDestContext, pvDestContext, mshlflags, pCid);
	}
}


//////////////////////////////////////////////////////
// AtlIMarshalOnSTL_XXXX: global marshalling functions
//////////////////////////////////////////////////////

template <class T, class CollType, class ItemType, class MarshalItem>
inline HRESULT AtlIMarshalOnSTL_GetMarshalSizeMax
( 
	REFIID riid,
	void __RPC_FAR *pv,
	DWORD dwDestContext,
	void __RPC_FAR *pvDestContext,
	DWORD mshlflags,
	DWORD __RPC_FAR *pSize,
	CollType *pColl,
	DWORD mshlopt = MSHOPT_NOSHAREDMEM|MSHOPT_DIFFERENTMACHINE
)
{
	ATLASSERT(pSize != NULL);
	ATLASSERT(pColl != NULL);

	if (MSH_ISMBV(dwDestContext, mshlopt))
	{
		// Use marshalling by value/smart proxy.
		ULARGE_INTEGER cbSize = {0};
		_MarshalInfo mshlinfo = {dwDestContext, pvDestContext, mshlflags};
		TestHR((AtlPersistOnSTL_GetSizeMax<CollType, ItemType, MarshalItem>(&cbSize, pColl, &mshlinfo)))
		*pSize = cbSize.LowPart;
		return S_OK;
	}
	else 
	{
		// Use standard marshalling.
		CComPtr<IMarshal> ccpIMarshal;
		CComPtr<IUnknown> ccpIUnk(static_cast<IUnknown*>(pv));
		TestHR(CoGetStandardMarshal(riid, ccpIUnk, dwDestContext, pvDestContext, mshlflags, &ccpIMarshal))
		return ccpIMarshal->GetMarshalSizeMax(riid, pv, dwDestContext, 
		                                      pvDestContext, mshlflags, pSize);
	}
}

template <class T, class CollType, class ItemType, class MarshalItem>
inline HRESULT AtlIMarshalOnSTL_MarshalInterface
( 
	IStream __RPC_FAR *pStm,
	REFIID riid,
	void __RPC_FAR *pv,
	DWORD dwDestContext,
	void __RPC_FAR *pvDestContext,
	DWORD mshlflags,
	CollType *pColl,
	DWORD mshlopt = MSHOPT_NOSHAREDMEM|MSHOPT_DIFFERENTMACHINE
)
{
	ATLASSERT(pColl != NULL);

	if (MSH_ISMBV(dwDestContext, mshlopt))
	{
		// Use marshalling by value/smart proxy.
		_MarshalInfo mshlinfo = {dwDestContext, pvDestContext, mshlflags};
		return AtlPersistOnSTL_Save<CollType, ItemType, MarshalItem>(pStm, FALSE, pColl, &mshlinfo);
	}
	else
	{
		// Use standard marshalling.
		CComPtr<IMarshal> ccpIMarshal;
		CComPtr<IUnknown> ccpIUnk(static_cast<IUnknown*>(pv));
		TestHR(CoGetStandardMarshal(riid, ccpIUnk, dwDestContext, pvDestContext, mshlflags, &ccpIMarshal))
		return ccpIMarshal->MarshalInterface(pStm, riid, pv, dwDestContext, pvDestContext, mshlflags);
	}
}

template <class T, class CollType, class ItemType, class MarshalItem>
inline HRESULT AtlIMarshalOnSTL_UnmarshalInterface
( 
	IStream __RPC_FAR *pStm,
	REFIID riid,
	void __RPC_FAR *__RPC_FAR *ppv,
	T *pT,
	CollType *pColl
)
{
	ATLASSERT(pT != NULL);
	ATLASSERT(pColl != NULL);

	TestHR((AtlPersistOnSTL_Load<CollType, ItemType, MarshalItem>(pStm, pColl)))
	return pT->GetUnknown()->QueryInterface( riid, ppv );
}


/////////////////////////////////////////////////////////
// AtlIMarshalOnStream_XXXX: global marshalling functions
/////////////////////////////////////////////////////////

template <class T>
inline HRESULT AtlIMarshalOnStream_GetMarshalSizeMax
( 
	REFIID riid,
	void __RPC_FAR *pv,
	DWORD dwDestContext,
	void __RPC_FAR *pvDestContext,
	DWORD mshlflags,
	DWORD __RPC_FAR *pSize,
	T *pT,
	DWORD mshlopt = MSHOPT_NOSHAREDMEM|MSHOPT_DIFFERENTMACHINE
)
{
	ATLASSERT(pT != NULL);

	if (MSH_ISMBV(dwDestContext, mshlopt))
	{
		// Use the persistence implementation.
		ULARGE_INTEGER cbSize = {0};
		CComQIPtr<IPersistStream> ccpIPS;
		CComQIPtr<IPersistStreamInit> ccpIPSI;

		if (ccpIPSI = pT->GetUnknown()) // Look for IPersistStreamInit.
			TestHR(ccpIPSI->GetSizeMax(&cbSize))
		else if (ccpIPS = pT->GetUnknown()) // Look for IPersistStream.
			TestHR(ccpIPS->GetSizeMax(&cbSize))
		else // Persistence not implemented.
			return E_FAIL;

		*pSize = cbSize.LowPart;
		return S_OK;
	}
	else 
	{ 
		// Use standard marshalling.
		CComPtr<IMarshal> ccpIMarshal;
		CComPtr<IUnknown> ccpIUnk(static_cast<IUnknown*>(pv));
		TestHR(CoGetStandardMarshal(riid, ccpIUnk, dwDestContext, pvDestContext, mshlflags, &ccpIMarshal))
		return ccpIMarshal->GetMarshalSizeMax(riid, pv, dwDestContext, pvDestContext, mshlflags, pSize);
	}
}

template <class T>
inline HRESULT AtlIMarshalOnStream_MarshalInterface
( 
	IStream __RPC_FAR *pStm,
	REFIID riid,
	void __RPC_FAR *pv,
	DWORD dwDestContext,
	void __RPC_FAR *pvDestContext,
	DWORD mshlflags,
	T *pT,
	DWORD mshlopt = MSHOPT_NOSHAREDMEM|MSHOPT_DIFFERENTMACHINE
)
{
	ATLASSERT(pT != NULL);

	if (MSH_ISMBV(dwDestContext, mshlopt))
	{
		// Use the persistence implementation.
		CComQIPtr<IPersistStream> ccpIPS;
		CComQIPtr<IPersistStreamInit> ccpIPSI;

		if (ccpIPSI = pT->GetUnknown()) // Look for IPersistStreamInit.
			return ccpIPSI->Save(pStm, FALSE);
		else if (ccpIPS = pT->GetUnknown()) // Look for IPersistStream.
			return ccpIPS->Save(pStm, FALSE);
		else // Persistence not implemented.
			return E_FAIL;
	}
	else 
	{
		// Use standard marshalling.
		CComPtr<IMarshal> ccpIMarshal;
		CComPtr<IUnknown> ccpIUnk(static_cast<IUnknown*>(pv));
		TestHR(CoGetStandardMarshal(riid, ccpIUnk, dwDestContext, pvDestContext, mshlflags, &ccpIMarshal))
		return ccpIMarshal->MarshalInterface(pStm, riid, pv, dwDestContext, pvDestContext, mshlflags);
	}
}

template <class T>
inline HRESULT AtlIMarshalOnStream_UnmarshalInterface
( 
	IStream __RPC_FAR *pStm,
	REFIID riid,
	void __RPC_FAR *__RPC_FAR *ppv,
	T *pT
)
{
	ATLASSERT(pT != NULL);

	CComQIPtr<IPersistStream> ccpIPS;
	CComQIPtr<IPersistStreamInit> ccpIPSI;

	if (ccpIPSI = pT->GetUnknown()) // Look for IPersistStreamInit.
		TestHR(ccpIPSI->Load(pStm))
	else if (ccpIPS = pT->GetUnknown()) // Look for IPersistStream.
		TestHR(ccpIPS->Load(pStm))
	else // Persistence not implemented.
		return E_FAIL;

	return pT->QueryInterface(riid, (void**)ppv);
}


///////////////////
// IMarshalBaseImpl
///////////////////

template <class T>
class ATL_NO_VTABLE IMarshalBaseImpl : public IMarshal
{
public:
	DWORD m_mshlopt;
	IMarshalBaseImpl() : m_mshlopt(MSHOPT_NOSHAREDMEM|MSHOPT_DIFFERENTMACHINE){}

	// IMarshal
	STDMETHOD(GetUnmarshalClass)
	(
		REFIID riid, 
		void __RPC_FAR *pv, 
		DWORD dwDestContext, 
		void __RPC_FAR *pvDestContext, 
		DWORD mshlflags, 
		CLSID __RPC_FAR *pCid
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalBaseImpl::GetUnmarshalClass\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshal_GetUnmarshalClass(riid, pv, dwDestContext, 
		                                          pvDestContext, mshlflags, pCid, pT->m_mshlopt);
	}
	STDMETHOD(ReleaseMarshalData)
	(
		IStream __RPC_FAR *pStm
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalBaseImpl::ReleaseMarshalData\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshal_ReleaseMarshalData(pStm);
	}
	STDMETHOD(DisconnectObject)
	(
		DWORD dwReserved
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalBaseImpl::DisconnectObject\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshal_DisconnectObject(dwReserved);
	}

	HRESULT IMarshal_GetUnmarshalClass
	(
		REFIID riid, 
		void __RPC_FAR *pv, 
		DWORD dwDestContext, 
		void __RPC_FAR *pvDestContext, 
		DWORD mshlflags, 
		CLSID __RPC_FAR *pCid,
		DWORD mshlopt
	)
	{
		T* pT = static_cast<T*>(this);
		return AtlIMarshal_GetUnmarshalClass<T>
		       (riid, pv, dwDestContext, pvDestContext, mshlflags, pCid, pT, mshlopt);
	}
	HRESULT IMarshal_ReleaseMarshalData
	(
		IStream __RPC_FAR *pStm
	)
	{
		return S_OK;
	}
	HRESULT IMarshal_DisconnectObject
	(
		DWORD dwReserved
	)
	{
		return S_OK;
	}
}; // class IMarshalBaseImpl


////////////////////
// IMarshalOnSTLImpl
////////////////////

template <class T, class CollType, class ItemType, class MarshalItem = _Persist<ItemType> >
class ATL_NO_VTABLE IMarshalOnSTLImpl : public IMarshalBaseImpl<T>
{
public:
	// IMarshal
	STDMETHOD(GetMarshalSizeMax)
	( 
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags,
		DWORD __RPC_FAR *pSize
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalOnSTLImpl::GetMarshalSizeMax\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshalOnSTL_GetMarshalSizeMax(riid, pv, dwDestContext, 
		                                           pvDestContext, mshlflags, pSize, &pT->m_coll, pT->m_mshlopt);
	}
	STDMETHOD(MarshalInterface)
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalOnSTLImpl::MarshalInterface\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshalOnSTL_MarshalInterface(pStm, riid, pv, dwDestContext,
		                                          pvDestContext, mshlflags, &pT->m_coll, pT->m_mshlopt);
	}
	STDMETHOD(UnmarshalInterface)
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *__RPC_FAR *ppv
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalOnSTLImpl::UnmarshalInterface\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshalOnSTL_UnmarshalInterface(pStm, riid, ppv, &pT->m_coll);
	}

	HRESULT IMarshalOnSTL_GetMarshalSizeMax
	( 
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags,
		DWORD __RPC_FAR *pSize,
		CollType *pColl,
		DWORD mshlopt
	)
	{
		return AtlIMarshalOnSTL_GetMarshalSizeMax<T, CollType, ItemType, MarshalItem>
		       (riid, pv, dwDestContext, pvDestContext, mshlflags, pSize, pColl, mshlopt);
	}
	HRESULT IMarshalOnSTL_MarshalInterface
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags,
		CollType *pColl,
		DWORD mshlopt
	)
	{
		return AtlIMarshalOnSTL_MarshalInterface<T, CollType, ItemType, MarshalItem>
		       (pStm, riid, pv, dwDestContext, pvDestContext, mshlflags, pColl, mshlopt);
	}
	HRESULT IMarshalOnSTL_UnmarshalInterface
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *__RPC_FAR *ppv,
		CollType *pColl
	)
	{
		T* pT = static_cast<T*>(this);
		return AtlIMarshalOnSTL_UnmarshalInterface<T, CollType, ItemType, MarshalItem>
		       (pStm, riid, ppv, pT, pColl);
	}
}; // class IMarshalOnSTLImpl


///////////////////////
// IMarshalOnStreamImpl
///////////////////////

template <class T>
class ATL_NO_VTABLE IMarshalOnStreamImpl : public IMarshalBaseImpl<T>
{
public:
	STDMETHOD(GetMarshalSizeMax)
	( 
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags,
		DWORD __RPC_FAR *pSize
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalOnStreamImpl::GetMarshalSizeMax\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshalOnStream_GetMarshalSizeMax(riid, pv, dwDestContext, 
		                                              pvDestContext, mshlflags, pSize, pT->m_mshlopt);
	}
	STDMETHOD(MarshalInterface)
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalOnStreamImpl::MarshalInterface\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshalOnStream_MarshalInterface(pStm, riid, pv, dwDestContext,
		                                             pvDestContext, mshlflags, pT->m_mshlopt);
	}
	STDMETHOD(UnmarshalInterface)
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *__RPC_FAR *ppv
	)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IMarshalOnStreamImpl::UnmarshalInterface\n"));
		T* pT = static_cast<T*>(this);
		return pT->IMarshalOnStream_UnmarshalInterface(pStm, riid, ppv);
	}

	HRESULT IMarshalOnStream_GetMarshalSizeMax
	( 
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags,
		DWORD __RPC_FAR *pSize,
		DWORD mshlopt
	)
	{
		T* pT = static_cast<T*>(this);
		return AtlIMarshalOnStream_GetMarshalSizeMax<T>
		       (riid, pv, dwDestContext, pvDestContext, mshlflags, pSize, pT, mshlopt);
	}
	HRESULT IMarshalOnStream_MarshalInterface
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *pv,
		DWORD dwDestContext,
		void __RPC_FAR *pvDestContext,
		DWORD mshlflags,
		DWORD mshlopt
	)
	{
		T* pT = static_cast<T*>(this);
		return AtlIMarshalOnStream_MarshalInterface<T>
		       (pStm, riid, pv, dwDestContext, pvDestContext, mshlflags, pT, mshlopt);
	}
	HRESULT IMarshalOnStream_UnmarshalInterface
	( 
		IStream __RPC_FAR *pStm,
		REFIID riid,
		void __RPC_FAR *__RPC_FAR *ppv
	)
	{
		T* pT = static_cast<T*>(this);
		return AtlIMarshalOnStream_UnmarshalInterface<T>(pStm, riid, ppv, pT);
	}
}; // class IMarshalOnStreamImpl

} // namespace ATLExt
