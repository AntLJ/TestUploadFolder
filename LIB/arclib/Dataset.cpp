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

// Dataset.cpp: CDataset クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dataset.h"
#include <iostream>
#include <CStringTokenizer.h>
#include <Feature.h>
//#include <MapCommunity.h>
#include <TableTypeUtil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy
{

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CSelectedDataset::CSelectedDataset() : m_RowArrayMap()
{

}

CSelectedDataset::~CSelectedDataset()
{

}

void CSelectedDataset::put_Data(LPCTSTR lpcszLayerName, _IRowPtr ipRow)
{
	ROWARRAYMAP::iterator it = m_RowArrayMap.find(CString(lpcszLayerName));
	if (it == m_RowArrayMap.end())
	{
		ROWARRAY ary;
		ary.push_back(ipRow);
		m_RowArrayMap.insert(std::pair<CString,ROWARRAY>(lpcszLayerName,ary));
	}
	else
	{
		it->second.push_back(ipRow);
	}
}

long CSelectedDataset::get_RowCount(LPCTSTR lpcszLayerName)
{
	ROWARRAYMAP::iterator it = m_RowArrayMap.find(CString(lpcszLayerName));
	if (it == m_RowArrayMap.end())
		return 0;
	return (long)it->second.size();
}

_IRowPtr CSelectedDataset::get_Row(LPCTSTR lpcszLayerName, long nIndex)
{
	if (nIndex < 0 || nIndex >= get_RowCount(lpcszLayerName))
		return 0;
	ROWARRAYMAP::iterator it = m_RowArrayMap.find(CString(lpcszLayerName));
	if (it == m_RowArrayMap.end())
		return 0;
	return it->second[nIndex].m_T;
}

void CSelectedDataset::get_DatasetName(std::vector<CString> *cNames)
{
	for (ROWARRAYMAP::iterator it = m_RowArrayMap.begin(); it != m_RowArrayMap.end(); it++)
		cNames->push_back(it->first);
}

CDataset::CDataset() : CWorkspaces(), m_mapTable()
{

}

CDataset::~CDataset()
{

}

ITablePtr CDataset::OpenTable(LPCTSTR lpcszTableName, LPCTSTR lpcszConnectString)
{
	ITablePtr ipTable = 0;
	if ((ipTable = FindTable(lpcszTableName)) != 0)
		return ipTable;

	const CWorkspace *cWorkspace = 0;
	CStringTokenizer aTokenizer(CString(lpcszConnectString), CString('/'));
	CString strUserName(aTokenizer.GetNextToken());
	CString strPassword(aTokenizer.GetNextToken());
	CString strVersion(aTokenizer.GetNextToken());
	CString strInstance(aTokenizer.GetNextToken());
	CString strServerName(aTokenizer.GetNextToken());
	if (NULL != (cWorkspace = FindWorkspace(strUserName, strVersion, strServerName)))
	{
		if (NULL != (cWorkspace = Connect(lpcszConnectString)))
			return 0;
	}
	if ((ipTable = cWorkspace->OpenTable(lpcszTableName)) == 0)
		return 0;
	m_mapTable.insert(std::pair<CString, ITablePtr>(CString(lpcszTableName), ipTable));
	return ipTable;
}

ITablePtr CDataset::OpenTable(LPCTSTR lpcszTableName, LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName)
{
	CString strConnectString = CString(lpcszUserName) + CString(_T("/")) + CString(lpcszPassword) +
		CString(_T("/")) + CString(lpcszVersion) + CString(_T("/")) + CString(lpcszInstance) + CString(_T("/")) + CString(lpcszServerName);
	return OpenTable(lpcszTableName, LPCTSTR(strConnectString));
}

ITablePtr CDataset::FindTable(LPCTSTR lpcszTableName)
{
	std::map<CString, ITablePtr>::iterator it = m_mapTable.find(CString(lpcszTableName));
	if (it == m_mapTable.end())
		return 0;
	return it->second;
}

bool CDataset::Select(LPCTSTR lpcszTableName, IQueryFilterPtr ipQueryFilter, CSelectedDataset *cDataset)
{
	// テーブルを探します
	ITablePtr ipTable = FindTable(lpcszTableName);
	if (ipTable == 0)
		return false;
	IDatasetPtr ipDataset(ipTable);
	esriDatasetType aDatasetType;
	if (ipDataset->get_Type(&aDatasetType) != S_OK)
		return false;

	switch (aDatasetType)
	{
	case esriDTFeatureDataset:
	case esriDTFeatureClass:
		{
			IFeatureClassPtr ipFeatureClass(ipDataset);
			IFeatureCursorPtr ipFeatureCursor = 0;
			if (ipFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK)
				return false;
			IFeaturePtr ipFeature = 0;
			while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != 0)
				cDataset->put_Data(lpcszTableName, ipFeature);
		}
		break;
	case esriDTTable:
		{
			_ICursorPtr ipCursor = 0;
			if (ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor) != S_OK)
				return false;
			_IRowPtr ipRow = 0;
			while (ipCursor->NextRow(&ipRow) == S_OK && ipRow != 0)
				cDataset->put_Data(lpcszTableName, ipRow);
		}
		break;
	default:
		std::cerr << "Not supported dataset type." << std::endl;
		return false;
	}
	return true;
}

bool CDataset::SelectByWhereClause(LPCTSTR lpcszTableName, LPCTSTR lpcszWhereClause, CSelectedDataset *cDataset)
{
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (ipQueryFilter->put_WhereClause(_bstr_t(lpcszWhereClause)) != S_OK)
		return false;
	return Select(lpcszTableName, ipQueryFilter, cDataset);
}

bool CDataset::SelectByIDs(LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, std::set<long> &idSet, CSelectedDataset *cDataset)
{
	long nCounter = 0;
	CString strWhereClause = CString(lpcszFieldName) + CString(_T("IN ("));
	for (std::set<long>::iterator it = idSet.begin(); it != idSet.end(); it++)
	{
		if (nCounter) strWhereClause += CString(_T(","));
		CString strID(_T(""));
		strID.Format(_T("%d"), *it);
		strWhereClause += strID;
		nCounter++;
		if (nCounter == 250)
		{
			strWhereClause += CString(_T(")"));
			if (! SelectByWhereClause(lpcszTableName, strWhereClause, cDataset))
				return false;
			nCounter = 0;
			strWhereClause = CString(lpcszFieldName) + CString(_T("IN ("));
		}
	}

	if (nCounter)
	{
		strWhereClause += CString(_T(")"));
		if (! SelectByWhereClause(lpcszTableName, strWhereClause, cDataset))
			return false;
	}

	return true;
}

}	// sindy
