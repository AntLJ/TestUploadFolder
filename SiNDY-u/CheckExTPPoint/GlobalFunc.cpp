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
	// 空間・属性検索(件数も取得版)
	LONG SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, 
					esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		/// クエリフィルタ作成
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeo );
		ipFilter->put_SpatialRel( spatialRel );

		// Where句指定されていたら、セットする
		if( lpszWhereClause != NULL )
		{
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット
			ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// 検索優先順位も指定する
		}

		// SubFields指定されていたら、セットする
		if( lpszSubFields != NULL )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		/// 検索結果が 0 ならリターン
		LONG lHitCount = 0;
		ipFeatureClass->FeatureCount( ipFilter, &lHitCount );
		if( lHitCount < 1 )
			return 0;

		/// 検索
		ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
		if( NULL == *ppFeatureCursor )
			return 0;
				
		return lHitCount;
	}
}
