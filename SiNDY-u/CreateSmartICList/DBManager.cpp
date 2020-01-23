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

#include "StdAfx.h"
#include "DBManager.h"
#include <atl2/seal.h>

CDBManager::CDBManager(void)
{
}

CDBManager::~CDBManager(void)
{
}

//where句を指定してフィーチャカーソルの取得
IFeatureCursorPtr CDBManager::GetFeatureCursor(IFeatureClassPtr ipFeatureClass, CComBSTR bstrWhereClause)
{
	return ipFeatureClass->_Search(CreateQueryFilter(bstrWhereClause), VARIANT_FALSE);
}


//指定した形状とesriSpatialRelEnumでした条件に一致するフィーチャカーソルの取得
IFeatureCursorPtr CDBManager::GetFeatureCursor(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, esriSpatialRelEnum eStType, LPCTSTR lpcszSubFields /*NULL*/)
{	
	//指定ジオメトリにあるデータのみを検索する
	ISpatialFilterPtr	ipSpatialFilter(CLSID_SpatialFilter);
						ipSpatialFilter->putref_Geometry(ipGeom);
						ipSpatialFilter->put_SpatialRel(eStType);
	if(lpcszSubFields != NULL)
		ipSpatialFilter->put_SubFields(CComBSTR(lpcszSubFields));

	// テーブルを検索してカーソルに格納
	return ipFeatureClass->_Search( ipSpatialFilter, VARIANT_FALSE);
}

//クエリフィルタの作成
IQueryFilterPtr CDBManager::CreateQueryFilter(CComBSTR bstrWhereClause)
{
	IQueryFilterPtr		ipQueryFilter;
	ipQueryFilter.CreateInstance(CLSID_QueryFilter);
	if( FAILED(ipQueryFilter->put_WhereClause(bstrWhereClause)))
		throw std::runtime_error(_T("Where句の設定に失敗."));

	return ipQueryFilter;
}

//OIDの取得
long CDBManager::GetOID(const _IRowPtr& ipRow)
{
	long lOID;
	ipRow->get_OID(&lOID);

	return lOID;
}

//形状の取得
IGeometryPtr CDBManager::GetGeom(const IFeaturePtr& ipFeature)
{
	IGeometryPtr ipGeom;

	ipFeature->get_ShapeCopy(&ipGeom);
	
	return ipGeom;
}

//OIDとフィーチャクラスから形状の取得
IGeometryPtr CDBManager::GetGeomFromOID(long lOID, IFeatureClassPtr ipFC)
{
	CString strWhereClause;
	strWhereClause.Format(_T("OBJECTID = %ld"), lOID);	

	return GetGeom((GetFeatureCursor(ipFC, (CComBSTR)strWhereClause)->_NextFeature()));
}

//指定した形状に指定した条件でヒットした形状の特定の情報ををセットに格納して返す
std::set<CAdapt< CComVariant>> CDBManager::GetHitValSet(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, long lFieldIndex, esriSpatialRelEnum eStType)
{
	std::set<CAdapt< CComVariant>> vaSet;
	
	IFeatureCursorPtr ipFCr = GetFeatureCursor(ipFeatureClass, ipGeom, eStType);

	IFeaturePtr ipFeature;

	while((S_OK == ipFCr->NextFeature(&ipFeature))){
		vaSet.insert(GetValue(ipFeature, lFieldIndex));
	}

	return vaSet;
}

//値の取得
CComVariant CDBManager::GetValue(_IRowPtr ipRow, long lFieldIndex)
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;

	if( lFieldIndex >= 0 ){
		ipRow->get_Value( lFieldIndex, &vaValue );
	}
	return vaValue;
}
