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

// SindyLinkTable.cpp: SindyLinkTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyLinkTable.h"
#include "SindyTools.h"

SindyLinkTable::SindyLinkTable(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyTable(cDBConnection, cName),
mLinkList()
{

}

SindyLinkTable::~SindyLinkTable()
{

}

HRESULT SindyLinkTable::LoadByMeshCode(long cMeshCode)
{
	HRESULT hr;
	const char *func = "SindyLinkTable::LoadByMeshCode";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::string aWhereClause = sindy::meshcode_cls::MESHCODE + "=" + boost::lexical_cast<std::string>(cMeshCode);
	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, aWhereClause, &ipCursor)))
		return hr;

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		long aOID;
		if (FAILED(hr = ipRow->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipRow));

		CComVariant vaLinkID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_info::LINK_ID, &vaLinkID)))
			return hr;
		vaLinkID.ChangeType(VT_I4);
		mLinkList.insert(GDBOBJSET(vaLinkID.lVal, ipRow));
	}

	return S_OK;
}

HRESULT SindyLinkTable::SearchByLinkID(long cLinkID, GDBOBJRANGE *cRange)
{
	*cRange = mLinkList.equal_range(cLinkID);
	return S_OK;
}

HRESULT SindyLinkTable::Store(_ICursorPtr ipCursor)
{
	HRESULT hr;
	const char *func = "SindyLinkTable::Store";

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		long aOID;
		if (FAILED(hr = ipRow->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipRow));

		CComVariant vaLinkID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_info::LINK_ID, &vaLinkID)))
			return hr;
		vaLinkID.ChangeType(VT_I4);
		mLinkList.insert(GDBOBJSET(vaLinkID.lVal, ipRow));
	}

	return S_OK;
}

HRESULT SindyLinkTable::Store(const std::set<long> &cList)
{
	HRESULT hr;
	const char *func = "SindyLinkTable::Store";

	std::string aWhereClause = "";
	if (FAILED(hr = SindyGetWhereClause2(cList, sindy::link_info::LINK_ID, &aWhereClause))) return hr;

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, aWhereClause, &ipCursor))) return hr;

	return Store(ipCursor);
}

HRESULT SindyLinkTable::LoadByLinkID(const std::set<long> &cList)
{
	HRESULT hr;
	const char *func = "SindyLinkTable::LoadByLinkID";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::set<long> aList;
	for (std::set<long>::const_iterator aID = cList.begin(); aID != cList.end(); aID++)
	{
		aList.insert(*aID);
		if (aList.size() == 1000)
		{
			if (FAILED(hr = Store(aList))) return hr;
			aList.clear();
		}
	}

	if (aList.size())
	{
		if (FAILED(hr = Store(aList))) return hr;
	}

	return S_OK;
}
