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

// SindyInfTable.cpp: SindyInfTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyInfTable.h"


SindyInfTable::SindyInfTable(SindyDBConnection *cDBConnection, const std::string &cInfName, const std::string &cLqName) :
SindyTable(cDBConnection, cInfName),
mLqName(cLqName)
{
	
}

SindyInfTable::~SindyInfTable()
{

}

HRESULT SindyInfTable::Store(_ICursorPtr ipCursor)
{
	HRESULT hr;
	const char *func = "SindyInfTable::Store";

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		long aOID;
		if (FAILED(hr = ipRow->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipRow));
	}

	return S_OK;
}

HRESULT SindyInfTable::Load()
{
	HRESULT hr;
	const char *func = "SindyInfTable::Load";

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, std::string(""), &ipCursor))) return hr;

	return Store(ipCursor);
}

HRESULT SindyInfTable::Store(const std::set<long> &cBuff)
{
	HRESULT hr;
	const char *func = "SindyInfTable::Store";

	std::string aWhereClause = "";
	if (FAILED(hr = SindyGetWhereClause2(cBuff, sindy::link_queue::OBJECTID, &aWhereClause))) return hr;

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, aWhereClause, &ipCursor)))
		return hr;

	return Store(ipCursor);
}

HRESULT SindyInfTable::LoadByMeshCode(long cMeshCode, bool cStart)
{
	HRESULT hr;
	const char *func = "SindyInfTable::LoadByMeshCode";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	ITablePtr ipTable = NULL;
	if (FAILED(hr = mpDBConnection->OpenTable(mLqName, &ipTable)))
		return hr;

	std::set<long> aInfID;
	if (FAILED(hr = SindyGetInfID(ipTable, cMeshCode, cStart, &aInfID)))
		return hr;

	return LoadByObjectID(aInfID);
}

HRESULT SindyInfTable::LoadByObjectID(const std::set<long> &cList)
{
	HRESULT hr;
	const char *func = "SindyInfTable::LoadByObjectID";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::set<long> aBuff;
	for (std::set<long>::const_iterator aID = cList.begin(); aID != cList.end(); aID++)
	{
		aBuff.insert(*aID);
		if (aBuff.size() == 1000)
		{
			if (FAILED(hr = Store(aBuff))) return hr;
			aBuff.clear();
		}
	}
	if (aBuff.size())
	{
		if (FAILED(hr = Store(aBuff))) return hr;
	}

	return S_OK;
}
