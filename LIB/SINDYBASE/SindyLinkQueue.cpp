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

// SindyLinkQueue.cpp: SindyLinkQueue クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyLinkQueue.h"


SindyLinkQueue::SindyLinkQueue(SindyDBConnection *cDBConnection, const std::string &cInfName, const std::string &cLqName) :
SindyInfTable(cDBConnection, cInfName, cLqName),
mLqTable(cDBConnection, cLqName)
{

}

SindyLinkQueue::~SindyLinkQueue()
{

}

HRESULT SindyLinkQueue::LoadByMeshCode(long cMeshCode, bool cStart)
{
	HRESULT hr;
	const char *func = "SindyLinkQueue::LoadByMeshCode";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);
	if (! mLqTable.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::set<long> aInfID;
	if (FAILED(hr = mLqTable.SearchByMeshCode(cMeshCode, cStart, &aInfID)))
		return hr;

	if (! aInfID.size()) return S_OK;

	if (FAILED(hr = SindyInfTable::LoadByObjectID(aInfID))) return hr;

	if (FAILED(hr = mLqTable.LoadByInfID(aInfID)))
		return hr;

	return hr;
}

HRESULT SindyLinkQueue::SearchLinkQueue(_IRowPtr ipInfRow, GDBOBJLIST *cLqList)
{
	HRESULT hr;
	const char *func = "SindyLinkQueue::SearchLinkQueue";

	cLqList->clear();

	long aOID;
	if (FAILED(hr = ipInfRow->get_OID(&aOID)))
		return GDBERROR(hr, func);

	if (FAILED(hr = mLqTable.SearchByInfID(aOID, cLqList)))
		return hr;

	return S_OK;
}

HRESULT SindyLinkQueue::SearchByLinkID(long cLinkID, bool cStartLink, GDBOBJLIST *cInfList)
{
	HRESULT hr;
	const char *func = "SindyLinkQueue::SearchByLinkID";

	cInfList->clear();

	std::set<long> aInfID;
	if (FAILED(hr = mLqTable.SearchByLinkID(cLinkID, cStartLink, &aInfID)))
		return hr;
	std::set<long>::iterator aIter = aInfID.begin();
	for (; aIter != aInfID.end(); aIter++)
	{
		_IRowPtr ipInfRow = NULL;
		if (FAILED(hr = SearchByObjectID(*aIter, &ipInfRow)))
			return hr;
		cInfList->insert(GDBOBJSET(*aIter, ipInfRow));
	}
	return S_OK;
}

static HRESULT GetInfList(ITablePtr ipTable, const std::set<long> &cList, std::set<long> *cInfList)
{
	HRESULT hr;
	const char *func = "GetInfList";

	std::string aWhereClause = "";
	if (FAILED(hr = SindyGetWhereClause2(cList, sindy::link_queue::LINK_ID, &aWhereClause))) return hr;

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause(ipTable, aWhereClause, &ipCursor))) return hr;

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		CComVariant vaInfID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::INF_ID, &vaInfID))) return hr;
		vaInfID.ChangeType(VT_I4);
		cInfList->insert(vaInfID.lVal);
	}

	return S_OK;
}

HRESULT SindyLinkQueue::LoadByLinkID(const std::set<long> &cIdList)
{
	HRESULT hr;
	const char *func = "SindyLinkQueue::LoadByLinkID";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);
	if (! mLqTable.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	std::string aName = "";
	if (FAILED(hr = mLqTable.GetName(&aName))) return hr;

	ITablePtr ipLqTable = NULL;
	if (FAILED(hr = mpDBConnection->OpenTable(aName, &ipLqTable))) return hr;

#ifdef	_DEBUG
	long counter = 0;
#endif;
	std::set<long> aList, aInfList;
	for (std::set<long>::const_iterator aID = cIdList.begin(); aID != cIdList.end(); aID++)
	{
#ifdef	_DEBUG
		std::cerr << "creating id-list... : " << ++counter << "/" << cIdList.size() << "\r";
#endif
		aList.insert(*aID);
		if (aList.size() == 1000)
		{
			if (FAILED(hr = GetInfList(ipLqTable, aList, &aInfList))) return hr;
			aList.clear();
		}
	}

#ifdef	_DEBUG
	std::cerr << std::endl;
#endif

	if (aList.size())
	{
		if (FAILED(hr = GetInfList(ipLqTable, aList, &aInfList))) return hr;
	}

	if (FAILED(hr = LoadByObjectID(aInfList))) return hr;
	if (FAILED(hr = mLqTable.LoadByInfID(aInfList))) return hr;

	return S_OK;
}
