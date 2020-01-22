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

#include <sindy/workspace.h>

namespace gf
{	
	// ワークスペース取得
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp )
	{
		IUnknownPtr ipUnknown;
		for( int i = 0; i < 5; ++i )
		{
			INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
			if( NULL == ipName )
				return NULL;

			ipName->Open( &ipUnknown );
			if( ipUnknown )
				break;
		}
		return ipUnknown;
	}

	// 環境変数名から、ワークスペースとフィーチャクラスをオープン
	bool GetFCFromEnv( const _TCHAR* lpcszEnv, IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFC )
	{
		CString strTmp = GetEnvValue(lpcszEnv);
		if( !strTmp.IsEmpty() )
		{
			((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(strTmp), &ipFC );
			if( ipFC )
				return true;
			else
			{
				cerr << "#Error フィーチャクラスのオープンに失敗(" << lpcszEnv << ") : " << strTmp << endl;
				return false;
			}
		}
		else
		{
			cerr << "#Error 環境変数 " << lpcszEnv << " が未設定" << endl;
			return false;
		}
	}

	// 空間・属性検索
	IFeatureCursorPtr SelectByShape( IGeometryPtr& ipGeo, IFeatureClassPtr& ipFeatureClass, long* pCount /* = NULL */,
									 esriSpatialRelEnum spatialRel /* = esriSpatialRelIntersects */,
									 LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// クエリフィルタ作成
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeo );
		ipFilter->put_SpatialRel( spatialRel );
		
		// Where句指定されていたら、セットする
		if( lpszWhereClause )
		{
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット
			ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// 検索優先順位も指定する
		}

		// SubFields指定されていたら、セットする
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// 検索結果数取得([Bug 6175]任意に変更)
		if( pCount )
		{
			if( FAILED(ipFeatureClass->FeatureCount(ipFilter, pCount)) )
				*pCount = -1;

			// 検索レコード数わかっているときは、あえてSearchしない
			if( *pCount < 1 )
				return NULL;
		}

		// 検索
		IFeatureCursorPtr ipFeatureCursor;
		if( FAILED(ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipFeatureCursor)) && pCount )	// [Bug 6419]
			*pCount = -1;
		return ipFeatureCursor;
	}
}
