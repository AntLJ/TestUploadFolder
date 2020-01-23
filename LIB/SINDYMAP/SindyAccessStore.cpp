/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

// SindyAccessStore.cpp: SindyAccessStore クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyAccessStore.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SindyAccessStore::SindyAccessStore(SindyDBConnection *cDBConnection) :
SindyTable(cDBConnection, sindy::layer::ACCESS_STORE)
{

}

SindyAccessStore::~SindyAccessStore()
{

}


HRESULT SindyAccessStore::Load()
{
	HRESULT hr;
	const char *func = "SindyAccessStore::Load";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, std::string(""), &ipCursor)))
		return hr;

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		long aOID;
		if (FAILED(hr = ipRow->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipRow));

		CComVariant vaAccessPointID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::access_store::ACCESSPOINT_ID, &vaAccessPointID)))
			return GDBERROR(hr, func);
		vaAccessPointID.ChangeType(VT_I4);

		mAccessStoreList.insert(GDBOBJSET(vaAccessPointID.lVal , ipRow));
	}

	return S_OK;
}

HRESULT SindyAccessStore::SearchByAccessPointID(long cID, GDBOBJRANGE* cpRange)
{
	*cpRange = mAccessStoreList.equal_range(cID);

	return (cpRange->first == cpRange->second) ? S_FALSE : S_OK;
}

