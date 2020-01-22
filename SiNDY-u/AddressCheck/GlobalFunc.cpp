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

using namespace sindy::schema;

namespace gf
{
	// ワークスペース取得
	IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
	{
		IUnknownPtr ipUnknown;

		INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
		if( !ipName )
			return nullptr;

		HRESULT hr = ipName->Open( &ipUnknown );
		if( !ipUnknown )
			return nullptr;
		
		return ipUnknown;
	}

	// 属性検索
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount /* = NULL */, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// クエリフィルタ作成
		IQueryFilterPtr ipFilter( CLSID_QueryFilter );
			
		// Where句指定されていたら、セットする
		if( lpszWhereClause )
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );

		// SubFields指定されていたら、セットする
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// 検索結果数取得
		if( pCount )
		{
			if( FAILED(ipTable->RowCount(ipFilter, pCount)) )
				*pCount = INVALID_VALUE;

			// 検索レコード数わかっているときは、あえてSearchしない
			if( *pCount < 1 )
				return nullptr;
		}

		// 検索
		_ICursorPtr ipCursor;
		if( FAILED(ipTable->Search(ipFilter, VARIANT_FALSE, &ipCursor)) && pCount )	
			*pCount = INVALID_VALUE;
		return ipCursor;
	}

	// マージ
	IGeometryPtr constructUnion( const IGeometryPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference )
	{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference( ipSpatialReference );		
		if( FAILED( ITopologicalOperatorPtr(ipPolygon)->ConstructUnion( IEnumGeometryPtr(ipGeometryCollection) ) ) )
		{
			return nullptr;
		}

		return ipPolygon;
	}	

	// ポリゴンの差分
	IGeometryPtr difference( const IGeometryPtr& ipMeshGeometry, const IGeometryPtr& ipMergeGeometry )
	{
		IGeometryPtr ipDifferentGeometry;
		if( FAILED(((ITopologicalOperatorPtr)ipMeshGeometry)->Difference( ipMergeGeometry, &ipDifferentGeometry )) )
		{
			return nullptr;
		}

		return ipDifferentGeometry;
	}

	// Touches判定
	bool isTouches( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		IRelationalOperatorPtr ipRel( ipBaseGeometry );
		if( S_OK != ipRel->Touches( ipOtherGeometry, &vb ) ) return false;
		return !( vb == VARIANT_FALSE );
	}

	// Contains判定
	bool IsContains( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		IRelationalOperatorPtr ipRel( ipBaseGeometry );
		if( S_OK != ipRel->Contains( ipOtherGeometry, &vb ) ) return false;
		return !( vb == VARIANT_FALSE );
	}

	// フィールドマップ作成
	FIELDSMAP createFieldMap( const ITablePtr& ipTable )
	{
		FIELDSMAP fieldMap;

		IFieldsPtr ipFields;
		if( S_OK !=ipTable->get_Fields( &ipFields ) || !ipFields ) return fieldMap;

		long lFieldCount(0L);
		ipFields->get_FieldCount( &lFieldCount );

		for( long index = 0; index < lFieldCount; ++index ){
		
			IFieldPtr ipField;
			if( S_OK != ipFields->get_Field( index, &ipField ) ) continue;

			CComBSTR bstr;
			if( S_OK != ipField->get_Name( &bstr ) ) continue;

			fieldMap.insert( pair<CString, long>(CString(bstr), index) );
		}
		return fieldMap;
	}

	// 属性値取得
	CComVariant getValue( const IFeaturePtr &ipFeature, long fieldIdx) 
	{
		CComVariant vaValue;
		if( fieldIdx == -1 ) return vaValue;
		ipFeature->get_Value( fieldIdx, &vaValue );

		return vaValue;
	}
} // gf
