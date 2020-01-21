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

// SiNDYTableFinder.cpp: SiNDYTableFinder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiNDYTableFinder.h"
#include <sindy/schema/reference.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// 設定/取得
//////////////////////////////////////////////////////////////////////

IWorkspacePtr SiNDYTableFinder::getWorkspace(LPCTSTR lpszTableName)
{
	IWorkspacePtr ipWorkspace;
	IDatasetPtr(getTable(lpszTableName))->get_Workspace(&ipWorkspace);
	return ipWorkspace;
}

/**
 * @brief 指定したオブジェクトを取得する
 *
 * @param lpszTableName [in] テーブル名
 * @param nObjectId [in] オブジェクトID
 * @return フィーチャクラスを指定した場合フィーチャ。テーブルを指定した場合オブジェクト。取得できなかった場合NULL。
 */
_IRowPtr SiNDYTableFinder::getRow(LPCTSTR lpszTableName, long nObjectId)
{
	ITablePtr ipTable = getTable(lpszTableName);
	IFeatureClassPtr ipFeatureClass = ipTable;

	if(ipFeatureClass != 0) {
		IFeaturePtr ipFeature;
		ipFeatureClass->GetFeature(nObjectId, &ipFeature);
		return ipFeature;
	}
	else if(ipTable != 0) {
		_IRowPtr ipRow;
		ipTable->GetRow(nObjectId, &ipRow);
		return ipRow;
	}
	
	return 0;
}

/**
 * @brief ２次メッシュ形状を取得する
 *
 * @note reference.BaseMeshフィーチャクラスを取得できる必要あり。
 *
 * @param nMeshcode [in] メッシュコード
 * @return ２次メッシュ形状。取得できなかった場合NULL。
 */
IGeometryPtr SiNDYTableFinder::getBaseMeshGeometry(long nMeshcode)
{
	using namespace schema::basemesh;

	IFeatureClassPtr ipFeatureClass = getTable(kTableName);
	if(ipFeatureClass == 0)
		return 0;

	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	
	CComBSTR bstrShapeFieldName;
	ipFeatureClass->get_ShapeFieldName(&bstrShapeFieldName);

	CString strWhere;
	strWhere.Format(_T("%s=%d"), kMeshCode, nMeshcode);
	ipQueryFilter->put_WhereClause(CComBSTR(strWhere));
	ipQueryFilter->put_SubFields(bstrShapeFieldName);

	IFeatureCursorPtr ipFeatureCursor;
	ipFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor);

	IFeaturePtr ipFeature;
	if(ipFeatureCursor->NextFeature(&ipFeature) != S_OK)
		return 0;

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	return ipGeometry;
}
