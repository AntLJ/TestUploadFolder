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

// SindyRoadCodeList.cpp: SindyRoadCodeList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyRoadCodeList.h"
#include "SindyRoadNetwork.h"

SindyRoadCodeList::SindyRoadCodeList(SindyDBConnection *cDBConnection) :
SindyTable(cDBConnection, sindy::layer::ROAD_CODE_LIST),
mRoadCodeList()
{

}

SindyRoadCodeList::~SindyRoadCodeList()
{

}

HRESULT SindyRoadCodeList::Load()
{
	HRESULT hr;
	const char *func = "SindyRoadCodeList::Load";

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

		CComVariant vaRoadCode;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::road_code_list::ROAD_CODE, &vaRoadCode)))
			return hr;
		vaRoadCode.ChangeType(VT_I4);
		mRoadCodeList.insert(GDBOBJSET(vaRoadCode.lVal, ipRow));
	}

	return S_OK;
}

HRESULT SindyRoadCodeList::SearchByRoadCode(long cRoadCode, _IRow **ipRow)
{
	*ipRow = NULL;

	GDBOBJITER aIter = mRoadCodeList.find(cRoadCode);
	if (aIter != mRoadCodeList.end())
	{
		(*ipRow) = aIter->second;
		(*ipRow)->AddRef();
	}

	return S_OK;
}
