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

#include "ATLExtPolicies.h"

namespace ATLExt
{
/////////////////////////////////////////////////////
// AtlPersistOnSTL_XXXX: global persistence functions
/////////////////////////////////////////////////////

template <class CollType, class ItemType, class PersistItem>
inline HRESULT AtlPersistOnSTL_Load
(
	LPSTREAM pStm, 
	CollType *pColl
)
{ 
	ATLASSERT(pStm != NULL);
	ATLASSERT(pColl != NULL);

	CollType::iterator it;

	// Clear the collection properly.
	if (!pColl->empty())
	{
		for (it = pColl->begin(); it != pColl->end(); it++)
			PersistItem::destroy(&*it);
		pColl->clear();
	}

	// Load the version number.
	DWORD dwVersion(-1);
	TestHR(pStm->Read(&dwVersion, sizeof(DWORD), NULL))
	if (dwVersion > _ATLEXT_VER)
		return E_FAIL; // Unsupported version.

	// Load the number of items in the collection
	CollType::size_type size(0);
	TestHR(pStm->Read(&size, sizeof(CollType::size_type), NULL))

	// Load the collection
	HRESULT hr(S_OK);
	ItemType item; // Temporary item
	for (CollType::size_type i(0); i < size; i++)
	{
		PersistItem::init(&item);
		hr = PersistItem::Load(&item, pStm);
		if (FAILED(hr))
		{
			// Clear the data properly.
			PersistItem::destroy(&item);
			for (it = pColl->begin(); it != pColl->end(); it++)
				PersistItem::destroy(&*it);
			pColl->clear();
			return hr;
		}
		it = pColl->insert(pColl->begin(), item);
		PersistItem::destroy(&item);
	}
	
	return hr;
}

template <class CollType, class ItemType, class PersistItem>
inline HRESULT AtlPersistOnSTL_Save
(
	LPSTREAM pStm, 
	BOOL /*fClearDirty*/, 
	CollType *pColl, 
	_MarshalInfo *pMshlInfo = NULL
)
{ 
	ATLASSERT(pStm != NULL);
	ATLASSERT(pColl != NULL);

	// Save the version number.
	DWORD dwVersion = _ATLEXT_VER;
	TestHR(pStm->Write(&dwVersion, sizeof(DWORD), NULL))

	// Save the number of items in the collection.
	CollType::size_type size(pColl->size());
	TestHR(pStm->Write(&size, sizeof(CollType::size_type), NULL))

	// Save the collection.
	CollType::reverse_iterator it;
	for (it = pColl->rbegin(); it != pColl->rend(); it++)
		TestHR(PersistItem::Save(&*it, pStm, pMshlInfo))
	
	return S_OK;
}

template <class CollType, class ItemType, class PersistItem>
inline HRESULT AtlPersistOnSTL_GetSizeMax
(
	ULARGE_INTEGER FAR *pcbSize, 
	CollType *pColl, 
	_MarshalInfo *pMshlInfo = NULL
)
{
	ATLASSERT(pcbSize != NULL);
	ATLASSERT(pColl != NULL);

	// The version number.
	pcbSize->QuadPart = sizeof(DWORD);

	// The number of items in the collection.
	pcbSize->QuadPart += sizeof(CollType::size_type);

	// The collection.
	ULARGE_INTEGER ulnItemSize;
	CollType::iterator it = pColl->begin();
	for (it; it != pColl->end(); it++)
	{
		ulnItemSize.QuadPart = 0;
		TestHR(PersistItem::GetSizeMax(&*it, &ulnItemSize, pMshlInfo))
		pcbSize->QuadPart += ulnItemSize.QuadPart;
	}

	return S_OK;
};


/////////////////////////////
// IPersistStreamInitBaseImpl
/////////////////////////////

template <class T>
class ATL_NO_VTABLE IPersistStreamInitBaseImpl : public IPersistStreamInit
{
public:
	// IPersist
	STDMETHOD(GetClassID)(CLSID *pClassID)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitBaseImpl::GetClassID\n"));
		T* pT = static_cast<T*>(this);
		return pT->IPersistStreamInitBase_GetClassID(pClassID);
	}

	// IPersistStreamInit
	STDMETHOD(IsDirty)()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitBaseImpl::IsDirty\n"));
		T* pT = static_cast<T*>(this);
		return (pT->m_bRequiresSave) ? S_OK : S_FALSE;
	}
	STDMETHOD(InitNew)()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitBaseImpl::InitNew\n"));
		T* pT = static_cast<T*>(this);
		return pT->IPersistStreamInitBase_InitNew();
	}

	HRESULT IPersistStreamInitBase_GetClassID(CLSID *pClassID)
	{
		*pClassID = T::GetObjectCLSID();
		return S_OK;
	}
	HRESULT IPersistStreamInitBase_InitNew()
	{
		T* pT = static_cast<T*>(this);
		pT->m_bRequiresSave = TRUE;
		return S_OK;
	}
}; // class IPersistStreamInitBaseImpl


//////////////////////////////
// IPersistStreamInitOnSTLImpl
//////////////////////////////

template <class T, class CollType, class ItemType, class PersistItem = _Persist<ItemType> >
class ATL_NO_VTABLE IPersistStreamInitOnSTLImpl : public IPersistStreamInitBaseImpl<T>
{
public:
	STDMETHOD(Load)(LPSTREAM pStm)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitOnSTLImpl::Load\n"));
		T* pT = static_cast<T*>(this);
		return pT->IPersistStreamInitOnSTL_Load(pStm, &pT->m_coll);
	}
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitOnSTLImpl::Save\n"));
		T* pT = static_cast<T*>(this);
		return pT->IPersistStreamInitOnSTL_Save(pStm, fClearDirty, &pT->m_coll);
	}
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR *pcbSize)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitOnSTLImpl::GetSizeMax\n"));
		T* pT = static_cast<T*>(this);
		return pT->IPersistStreamInitOnSTL_GetSizeMax(pcbSize, &pT->m_coll);
	}

	HRESULT IPersistStreamInitOnSTL_Load(LPSTREAM pStm, CollType *pColl)
	{
		T* pT = static_cast<T*>(this);
		HRESULT hr = AtlPersistOnSTL_Load<CollType, ItemType, PersistItem>(pStm, pColl);
		if (SUCCEEDED(hr))
			pT->m_bRequiresSave = FALSE;
		return hr;
	}
	HRESULT IPersistStreamInitOnSTL_Save(LPSTREAM pStm, BOOL fClearDirty, CollType *pColl)
	{
		T* pT = static_cast<T*>(this);
		HRESULT hr = AtlPersistOnSTL_Save<CollType, ItemType, PersistItem>(pStm, fClearDirty, pColl);
		if (SUCCEEDED(hr) && fClearDirty)
			pT->m_bRequiresSave = FALSE;
		return hr;
	}
	HRESULT IPersistStreamInitOnSTL_GetSizeMax(ULARGE_INTEGER FAR *pcbSize, CollType *pColl)
	{
		return AtlPersistOnSTL_GetSizeMax<CollType, ItemType, PersistItem>(pcbSize, pColl);
	}
}; // class IPersistStreamInitOnSTLImpl

} // namespace ATLExt
