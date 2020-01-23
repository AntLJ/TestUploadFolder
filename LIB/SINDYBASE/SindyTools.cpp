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

/**
 *	SindyTools.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/10/17		新規作成
 */

#include "SindyAttrdef.h"
#include "SindyTools.h"
#include <boost/utility.hpp>	// for next

// cStart == true : 先頭が指定メッシュ内に含まれている
// cStart == false : どれかが指定メッシュ内に含まれている
HRESULT SindyGetInfID(ITablePtr ipTable, long cMeshCode, bool cStart, std::set<long> *cList)
{
	HRESULT hr;
	const char *func = "SindyGetLqInfID";

	std::string aWhereClause = "";
	if (cStart)
	{
		aWhereClause = sindy::link_queue::SEQUENCE + "=1";
		aWhereClause += (" AND " + sindy::link_queue::MESHCODE + "=" + boost::lexical_cast<std::string>(cMeshCode));
	}
	else
	{
		aWhereClause = sindy::link_queue::MESHCODE + "=" + boost::lexical_cast<std::string>(cMeshCode);
	}

	_ICursorPtr ipCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause(ipTable, aWhereClause, &ipCursor)))
		return hr;

	_IRowPtr ipRow = NULL;
	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != NULL)
	{
		CComVariant vaInfID;
		if (FAILED(hr = GDBGetValue(ipRow, sindy::link_queue::INF_ID, &vaInfID)))
			return hr;
		vaInfID.ChangeType(VT_I4);
		cList->insert(vaInfID.lVal);
	}

	return S_OK;
}

HRESULT SindyGetWhereClause(const std::set<long> &cList, const std::string &cColumn, std::string *cWhereClause)
{
	*cWhereClause = "";
	std::set<long>::const_iterator aIter = cList.begin();
	for (; aIter != cList.end(); aIter++)
	{
		if (cWhereClause->size())
			(*cWhereClause) += " OR ";
		(*cWhereClause) += (cColumn + "=" + boost::lexical_cast<std::string>(*aIter));
	}
	return S_OK;
}

HRESULT SindyGetWhereClause2(const std::set<long> &cList, const std::string &cColumn, std::string *cWhereClause)
{
	const char *func = "SindyGetWhereClause2";

	if (cList.size() > 1000) return GDBERROR2(E_OUTOFMEMORY, func);

	*cWhereClause = cColumn + " IN (";
	std::string aTemp = "";
	for (std::set<long>::const_iterator aID = cList.begin(); aID != cList.end(); aID++)
	{
		if (aTemp.size()) aTemp += ",";
		aTemp += boost::lexical_cast<std::string>(*aID);
	}
	*cWhereClause += (aTemp + ")");
	return S_OK;
}

void SindyGetWhereClauses( const std::set<long> &cList, const std::string &cColumn, std::vector<std::string>& crWhereClauses )
{
	std::set<long> aSubList;
	for (std::set<long>::const_iterator itID = cList.begin(); itID != cList.end(); ++itID)
	{
		aSubList.insert( *itID );

		if (aSubList.size() == 1000 || boost::next( itID ) == cList.end())
		{
			std::string aWhereClause;
			SindyGetWhereClause2( aSubList, cColumn, &aWhereClause );
			crWhereClauses.push_back( aWhereClause );
			aSubList.clear();
		}
	}
}

