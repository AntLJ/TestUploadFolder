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

// AdminTable.cpp: AdminTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdminTable.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AdminTable::AdminTable()
{

}

AdminTable::~AdminTable()
{

}

bool AdminTable::init(IWorkspacePtr ipWorkspace, LPCTSTR cGyosei, LPCTSTR cWhere)
{
	IQueryDefPtr ipQueryDef;	//ユニークにはできるけど、ソートはできない？
//	ITableSortPtr ipTableSort(CLSID_TableSort); //ソートはできるけど、ユニークにできない？
	CString aWhere;
	aWhere = cWhere;
	if(aWhere == _T("")){
		aWhere = _T("AREACLASS_C = 1"); //都市地図エリア内
	}else{
		aWhere += _T(" AND AREACLASS_C = 1"); //都市地図エリア内
	}
	IFeatureWorkspacePtr(ipWorkspace)->CreateQueryDef(&ipQueryDef);
	ipQueryDef->put_Tables(CComBSTR(cGyosei));
	ipQueryDef->put_SubFields(CComBSTR("DISTINCT CITYCODE"));
	ipQueryDef->put_WhereClause(CComBSTR(aWhere));
	_ICursorPtr ipCursor;
	if(FAILED(ipQueryDef->Evaluate(&ipCursor))){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CComVariant aCityCode;
		if(ipRow->get_Value(0, &aCityCode) == S_OK){
			m_CityCodeSet.insert(aCityCode.bstrVal);
		}
	}
	if(m_CityCodeSet.size()> 0){
		m_CurrentCode = m_CityCodeSet.begin();
		return true;
	}else{
		return false;
	}
}

bool AdminTable::next(CString& cpAddrCode)
{
	if(m_CurrentCode != m_CityCodeSet.end()){
		cpAddrCode = *m_CurrentCode;
		m_CurrentCode++;
	}else{
		cpAddrCode = _T("");
		return false;
	}
	return true;
}
