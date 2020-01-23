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

// SindyHighwayText.cpp: SindyHighwayText クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyHighwayText.h"
#include "SindyRoadNetwork.h"

SindyHighwayText::SindyHighwayText(SindyDBConnection *cDBConnection) :
SindyTable(cDBConnection, sindy::layer::HIGHWAY_TEXT),
mHwyFacilList()
{

}

SindyHighwayText::~SindyHighwayText()
{

}


HRESULT SindyHighwayText::Load()
{
	HRESULT hr;
	const char *func = "SindyHighwayText::Load";

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

		SindyHighwayFacil aFacil;
		if (FAILED(hr = aFacil.SetFacil(ipRow)))
			return hr;
		mHwyFacilList.insert(GDBHWYFACILSET(aFacil, ipRow));
	}

	return S_OK;
}

HRESULT SindyHighwayText::SearchByFacilCode(long cRoadCode, long cRoadSeq, GDBHWYFACILRANGE *cFacilRange)
{
	SindyHighwayFacil aKey(cRoadCode, cRoadSeq);
	*cFacilRange = mHwyFacilList.equal_range(aKey);
	return S_OK;
}
