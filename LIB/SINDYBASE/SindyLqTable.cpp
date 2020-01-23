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

// SindyLqTable.cpp: SindyLqTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyLqTable.h"


SindyLqTable::SindyLqTable(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyTable(cDBConnection, cName),
mLinkQueue(),
mLinkList()
{

}

SindyLqTable::~SindyLqTable()
{

}

HRESULT SindyLqTable::Store(_ICursorPtr ipCursor)
{
	HRESULT hr;
	const char *func = "SindyLqTable::Store";

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		long aOID;
		if (FAILED(hr = ipRow->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipRow));

		CComVariant vaInfID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::INF_ID, &vaInfID)))
			return hr;
		vaInfID.ChangeType(VT_I4);

		CComVariant vaSequence;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::SEQUENCE, &vaSequence)))
			return hr;
		vaSequence.ChangeType(VT_I4);

		CComVariant vaLinkID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::LINK_ID, &vaLinkID)))
			return hr;
		vaLinkID.ChangeType(VT_I4);

		GDBLQITER aIter = mLinkQueue.find(vaInfID.lVal);
		if (aIter == mLinkQueue.end())
		{
			GDBOBJLIST lq;
			lq.insert(GDBOBJSET(vaSequence.lVal, ipRow));
			mLinkQueue.insert(GDBLQSET(vaInfID.lVal, lq));
		}
		else
		{
			aIter->second.insert(GDBOBJSET(vaSequence.lVal, ipRow));
		}

		mLinkList.insert(GDBOBJSET(vaLinkID.lVal, ipRow));
	}

	return S_OK;
}

HRESULT SindyLqTable::Load()
{
	HRESULT hr;
	const char *func = "SindyLqTable::Load";

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, std::string(""), &ipCursor))) return hr;

	return Store(ipCursor);
}

HRESULT SindyLqTable::LoadByMeshCode(long cMeshCode, bool cStart)
{
	HRESULT hr;
	const char *func = "SindyLqTable::LoadByMeshCode";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::set<long> aInfID;
	if (FAILED(hr = SearchByMeshCode(cMeshCode, cStart, &aInfID)))
		return hr;

	if (! aInfID.size()) return S_OK;

	return LoadByInfID(aInfID);
}

HRESULT SindyLqTable::Store(const std::set<long> &cList)
{
	HRESULT hr;
	const char *func = "SindyLqTable::Store";

	std::string aWhereClause = "";
	if (FAILED(hr = SindyGetWhereClause2(cList, sindy::link_queue::INF_ID, &aWhereClause))) return hr;

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((ITablePtr)ipDataset, aWhereClause, &ipCursor))) return hr;

	return Store(ipCursor);
}

HRESULT SindyLqTable::LoadByInfID(const std::set<long> &cInfID)
{
	HRESULT hr;
	const char *func = "SindyLqTable::LoadByInfID";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::set<long> aBuff;
	for (std::set<long>::const_iterator aID = cInfID.begin(); aID != cInfID.end(); aID++)
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

HRESULT SindyLqTable::SearchByInfID(long cInfID, GDBOBJLIST *cList)
{
	cList->clear();
	GDBLQITER aIter = mLinkQueue.find(cInfID);
	if (aIter != mLinkQueue.end()) *cList = aIter->second;
	return S_OK;
}

HRESULT SindyLqTable::SearchByLinkID(long cLinkID, bool cStartLink, std::set<long> *cInfID)
{
	HRESULT hr;

	cInfID->clear();
	GDBOBJRANGE aRange = mLinkList.equal_range(cLinkID);
	GDB2OBJITER aIter = aRange.first;
	for (; aIter != aRange.second; aIter++)
	{
		_IRowPtr ipRow = aIter->second;
		if (cStartLink)
		{
			CComVariant vaSequence;
			if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::SEQUENCE, &vaSequence)))
				return hr;
			vaSequence.ChangeType(VT_I4);
			if (vaSequence.lVal != 1)
				continue;
		}
		CComVariant vaInfID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::INF_ID, &vaInfID)))
			return hr;
		vaInfID.ChangeType(VT_I4);
		cInfID->insert(vaInfID.lVal);
	}
	return S_OK;
}

HRESULT SindyLqTable::SearchByMeshCode(long cMeshCode, bool cStart, std::set<long> *cInfID)
{
	return SindyGetInfID((ITablePtr)ipDataset, cMeshCode, cStart, cInfID);
}
