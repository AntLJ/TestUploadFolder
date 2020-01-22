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
#include "GlobalFunc.h"
#include <boost/filesystem.hpp>
#include <sindy/workspace.h>

namespace gf
{
	// ワークスペース取得
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp )
	{
		IUnknownPtr ipUnknown = NULL;
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

	// 空間・属性検索
	IFeatureCursorPtr SelectByShape( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, long* pCount /* = NULL */,
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

		// 検索結果数取得
		if( pCount )
		{
			if( FAILED(ipFeatureClass->FeatureCount(ipFilter, pCount)) )
				return NULL;
		}

		// 検索
		IFeatureCursorPtr ipFeatureCursor;
		ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor );
		return ipFeatureCursor;
	}

	// 属性検索
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount /* = NULL */, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// クエリフィルタ作成
		IQueryFilterPtr ipFilter( CLSID_QueryFilter );
			
		// Where句指定されていたら、セットする
		if( lpszWhereClause )
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where句セット

		// SubFields指定されていたら、セットする
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// 検索結果数取得([Bug 6175]任意に変更)
		if( pCount )
		{
			if( FAILED(ipTable->RowCount(ipFilter, pCount)) )
				return NULL;
		}

		// 検索
		_ICursorPtr ipCursor;
		ipTable->Search( ipFilter, VARIANT_FALSE, &ipCursor );
		return ipCursor;
	}	
}

// addr_func
namespace addr_func
{
	// 半角数字？
	bool IsNumStr( const CString& strData )
	{
		int iLen = strData.GetLength();
		for( int i = 0; i < iLen; ++i )
		{
			if( !_istdigit(strData.GetAt(i)) )
				return false;
		}
		return true;
	}

	// CGouRec比較関数(代表住所選択用)
	bool CompareGouRec( const CGouRec& lhs, const CGouRec& rhs )
	{
		// 以下の条件で優先付け(優先号は、絶対なのでここでは省く)
		// (0) 大字も生成でないもの[Bug7068]
		// (1) 号タイプ = GOU
		// (2) エリア内
		// (3) 家形の面積が大きいもの
		// (4) OBJECTIDの小さいもの
		// (5) エリア外
		// (6) 家形の面積が大きいもの
		// (7) OBJECTIDの小さいもの
		// (8) 号タイプ = TPG
		// (9) (2) ～ (7)と同様

		// 通称行政界での生成
		if( !lhs.m_bTsushoConv && rhs.m_bTsushoConv )	return true;
		else if( !(lhs.m_bTsushoConv ^ rhs.m_bTsushoConv) )
		{
			// 号タイプ
			if( lhs.m_lGouType < rhs.m_lGouType )	return true;
			else if( lhs.m_lGouType == rhs.m_lGouType )
			{
				// 	エリア内外
				if( lhs.m_bInArea && !rhs.m_bInArea )	return true;
				else if( !(lhs.m_bInArea ^ rhs.m_bInArea) )
				{
					// 面積
					if( lhs.m_dKakeiArea > rhs.m_dKakeiArea )		return true;
					else if( lhs.m_dKakeiArea == rhs.m_dKakeiArea )	return lhs.m_lOID < rhs.m_lOID;	// 面積も等しいときは、OID小さい方
					else											return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

	// 号レコード位置的な優先度決定関数
	int CompareGouRecPos( const CGouRec& lhs, const CGouRec& rhs )
	{		
		d_lonlat dLhs, dRhs;
		g_cCrdCnv.MeshtoLL( lhs.m_cMXY.m_nMeshCode, lhs.m_cMXY.m_nX, lhs.m_cMXY.m_nY, &dLhs.lon, &dLhs.lat );
		g_cCrdCnv.MeshtoLL( rhs.m_cMXY.m_nMeshCode, rhs.m_cMXY.m_nX, rhs.m_cMXY.m_nY, &dRhs.lon, &dRhs.lat );

		if( dLhs.lon != dRhs.lon )		return dLhs.lon > dRhs.lon? -1 : 1;
		else if( dLhs.lat != dRhs.lat )	return dLhs.lat > dRhs.lat? -1 : 1;
		else							return 0;
	}
}
