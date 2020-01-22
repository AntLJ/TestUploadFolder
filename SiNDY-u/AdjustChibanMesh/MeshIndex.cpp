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

// MeshIndex.cpp: MeshIndex クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshIndex.h"

//#define _PASCO
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//　コンストラクタ
MeshIndex::MeshIndex()
: m_MeshIndex(-1)
{
}

//　コンストラクタ
MeshIndex::MeshIndex(IFeatureClassPtr ipFeature)
{
	m_ipFeature = ipFeature;
}

//デストラクタ
MeshIndex::~MeshIndex()
{
}

//　初期化。（条件で指定）
bool MeshIndex::init(LPCTSTR cMeshField, const CString &cWhere)
{
	//メッシュリストが無ければ都市地図エリア全部又はWHERE条件一致のもの
	/// クエリフィルタ作成
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	CString aWhere = cWhere;
	if(cWhere != _T("")){
		aWhere += _T(" AND CREATE_YEAR IS NOT NULL");
	}else{
		aWhere = _T("CREATE_YEAR IS NOT NULL");		
	}
	ipFilter->put_WhereClause(CComBSTR(aWhere));

	LONG	lFeatureCount = 0;
	/// 検索結果が 0 ならリターン
	m_ipFeature->FeatureCount( ipFilter, &lFeatureCount );
	if( lFeatureCount < 1 ) {
		_ftprintf(stderr, _T("該当メッシュが見つかりません。\n"));
		return false;
	}

	LONG	lFieldIndex = 0;
	CComBSTR bstrName = cMeshField;
	m_ipFeature->FindField(bstrName, &lFieldIndex);
	if(lFieldIndex < 0){
		m_MeshIndex = -1;
		_ftprintf(stderr, _T("メッシュコードのフィールド名を確認して下さい。:%s\n"), cMeshField);
		return false;
	}else{
		m_MeshIndex = lFieldIndex;
	}
#if 0
	/// 検索
	m_ipFeature->Search( ipFilter, VARIANT_FALSE, &m_ipCursor );
#else
	ITableSortPtr ipTableSort(CLSID_TableSort); 
	ipTableSort->put_Fields(bstrName);
	ipTableSort->putref_Table(ITablePtr(m_ipFeature));
	ipTableSort->putref_QueryFilter(ipFilter);
	ipTableSort->Sort(NULL);
	/// 検索
	_ICursorPtr	ipCursor;
	ipTableSort->get_Rows(&ipCursor);
	m_ipCursor = IFeatureCursorPtr(ipCursor);
	//	m_ipFeature->Search( ipFilter, VARIANT_FALSE, &m_ipFeatureCursor );

//	return lFeatureCount;
#endif
	return true;
}

//　初期化。（メッシュコードで指定）
bool MeshIndex::init(LONG cMeshCode, LPCTSTR cMeshField)
{
	/// クエリフィルタ作成
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	CString aWhere;
#ifdef _PASCO
	aWhere.Format("%s = %d", cMeshField, cMeshCode);
#else
//	aWhere.Format("%s = %d", cMeshField, cMeshCode);
	aWhere.Format(_T("%s = %d AND CREATE_YEAR IS NOT NULL"), cMeshField, cMeshCode);
#endif
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	CString aSubField;
	aSubField.Format(_T("SHAPE,%s"), cMeshField);
	ipFilter->put_SubFields(CComBSTR(aSubField));

	LONG	lFeatureCount = 0;
	/// 検索結果が 0 ならリターン
	m_ipFeature->FeatureCount( ipFilter, &lFeatureCount );
	if( lFeatureCount < 1 ) {
		_ftprintf(stderr, _T("都市地図エリア内に該当メッシュが見つかりません。,%d\n"), cMeshCode);
		return false;
	}
	LONG	lFieldIndex = 0;
	CComBSTR bstrName(cMeshField);
	m_ipFeature->FindField(bstrName, &lFieldIndex);
	if(lFieldIndex < 0){
		m_MeshIndex = -1;
		_ftprintf(stderr, _T("メッシュコードのフィールド名を確認して下さい。:%s\n"), cMeshField);
		return false;
	}else{
		m_MeshIndex = lFieldIndex;
	}
	/// 検索
	if(SUCCEEDED(m_ipFeature->Search( ipFilter, VARIANT_FALSE, &m_ipCursor ))){
		return true;
	}else{
		_ftprintf(stderr, _T("メッシュポリゴンの検索失敗。:%d\n"), cMeshCode);
		return false;
	}
}

//　（次の）対象メッシュポリゴンを取得。
IFeaturePtr MeshIndex::nextFeature(LONG *cpMeshCode)
{
	IFeaturePtr ipFeature;
	if(m_ipCursor->NextFeature( &ipFeature ) == S_OK){
		CComVariant vaValue;
		ipFeature->get_Value( m_MeshIndex, &vaValue );
		*cpMeshCode = vaValue.intVal;
	}else{
		*cpMeshCode = 0;
		ipFeature = NULL;
	}
	return ipFeature;
}
