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

#include "stdafx.h"
#include "GlobalFunc.h"

namespace gf
{
	// ドメインマップ取得関数
	bool GetDomainMap( ITablePtr ipTable, LPCTSTR lpcszFieldName, DOMAINMAP* map )
	{
		// フィールド名からフィールドID取得
		long lFieldID = -1;
		if( FAILED(ipTable->FindField( CComBSTR(lpcszFieldName), &lFieldID )) )
			return false; 

		// IField取得する
		IFieldsPtr ipFields;
		if( FAILED(ipTable->get_Fields( &ipFields )) )
			return false;

		IFieldPtr ipField;
		if( FAILED(ipFields->get_Field( lFieldID, &ipField )) )
			return false;

		// IDomain取得して、mapを生成
		IDomainPtr ipDomain;
		if( FAILED(ipField->get_Domain( &ipDomain )) )
			return false;		// ドメイン無しがここに該当しないか？要確認。

		ICodedValueDomainPtr ipCVDomain( ipDomain );
		if( NULL == ipCVDomain )
			return true;		// ドメイン無し？

		LONG lDomainCount;
		if( FAILED( ipCVDomain->get_CodeCount( &lDomainCount )) )
			return false;

		BSTR bstr;
		for( long i = 0; i < lDomainCount; i++ )
		{
			CComVariant vaValue;
			if( FAILED( ipCVDomain->get_Value( i, &vaValue )) )
				return false;
			if( FAILED( ipCVDomain->get_Name( i, &bstr )) )
				return false;

			map->insert( std::pair<long, CString>( vaValue.lVal, CString(bstr) ) );
			::SysFreeString( bstr );
		}

		return true;
	}

	// 空間・属性検索(件数も取得版)
	bool SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long* pCount,
								 esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// クエリフィルタ作成
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		if( FAILED(ipFeatureClass->get_ShapeFieldName( &bstrFieldName )) )
			return false;
		if( FAILED(ipFilter->put_GeometryField( bstrFieldName )) )
			return false;
		if( FAILED(ipFilter->putref_Geometry( ipGeo )) )
			return false;
		if( FAILED(ipFilter->put_SpatialRel( spatialRel )) )
			return false;
		
		// Where句指定されていたら、セットする
		if( lpszWhereClause )
		{
			// Where句セット
			if( FAILED(ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) )) )
				return false;
			// 検索優先順位も指定する
			if( FAILED(ipFilter->put_SearchOrder( esriSearchOrderSpatial )) )
				return false;
		}

		// SubFields指定されていたら、セットする
		if( lpszSubFields )
			if( FAILED(ipFilter->put_SubFields( CComBSTR(lpszSubFields) )) )
				return false;

		// 検索結果が 0 なら正常リターン
		if( FAILED(ipFeatureClass->FeatureCount( ipFilter, pCount )) )
			return false;
		if( *pCount < 1 )
			return true;

		// 検索
		if( FAILED(ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor )) )
			return false;

		return true;
	}

	// 空間検索 (+ 属性検索) ... 件数のみ
	long SelectByShapeOnlyCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */ )
	{
		// クエリフィルタ作成
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		if( FAILED(ipFeatureClass->get_ShapeFieldName( &bstrFieldName )) )
			return -1;
		if( FAILED(ipFilter->put_GeometryField( bstrFieldName )) )
			return -1;
		if( FAILED(ipFilter->putref_Geometry( ipGeo )) )
			return -1;
		if( FAILED(ipFilter->put_SpatialRel( spatialRel )) )
			return -1;
		
		// Where句指定されていたら、セットする
		if( lpszWhereClause )
		{
			// Where句セット
			if( FAILED(ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) )) )
				return -1;
			// 検索優先順位も指定する
			if( FAILED(ipFilter->put_SearchOrder( esriSearchOrderSpatial )) )
				return -1;
		}

		// 検索結果の件数をリターン、異常なら-1をリターン
		long lCount = 0;
		return SUCCEEDED(ipFeatureClass->FeatureCount(ipFilter, &lCount)) ? lCount : -1;
	}
}
