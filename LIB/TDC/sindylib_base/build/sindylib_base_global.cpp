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
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/shared_ptr.hpp>
#include "AheLayerFunctions.h"
#include "Map.h"
#include "sindylib_base_global.h"
#include "TimeTrace.h"
#include "TableFinder.h"
#include "RowContainer.h"
#include "TableContainer.h"

typedef boost::tokenizer< boost::char_separator<_TCHAR>, uh::tstring::const_iterator, uh::tstring > Tokenizer;
#define foreach BOOST_FOREACH

using namespace sindy;

CTimeTrace g_trace;

// 渡された形状群をUNIONする
IGeometryPtr AheConstructUnion( _ISetPtr& ipGeometrySet )
{
	IGeometryPtr ipRet; // 戻り値

	if( ! ipGeometrySet )
		return ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;

	long lCount = 0;
	ipGeometrySet->Reset();
	ipGeometrySet->get_Count( &lCount );
	for( long i = 0; i < lCount; ++i )
	{
		IUnknownPtr ipUnk;
		ipGeometrySet->Next( &ipUnk );
		IGeometryPtr ipGeom( ipUnk );

		if( ipGeom )
		{
			esriGeometryType emType = esriGeometryAny;
			ipGeom->get_GeometryType( &emType );
			switch( emType )
			{
				case esriGeometryBag:
				{
					IEnumGeometryPtr ipEnumGeom( ipGeom );
					ipEnumGeom->Reset();
					IGeometryPtr ipGeomPart;
					while( S_OK == ipEnumGeom->Next( &ipGeomPart ) )
					{
						esriGeometryType emTypePart = esriGeometryAny;
						ipGeomPart->get_GeometryType( &emTypePart );
						switch( emTypePart )
						{
							case esriGeometryMultipoint:
							case esriGeometryPoint:
								if( ipPointsCol == NULL )
									ipPointsCol.CreateInstance( CLSID_GeometryBag );
								ipPointsCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolyline:
								if( ipPolylinesCol == NULL )
									ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
								ipPolylinesCol->AddGeometry( ipGeomPart );
								break;
							case esriGeometryPolygon:
								if( ipPolygonsCol == NULL )
									ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
								ipPolygonsCol->AddGeometry( ipGeomPart );
								break;
							default:
								_ASSERTE(false);
								break;
						}
					}
					break;
				}
				case esriGeometryMultipoint:
				case esriGeometryPoint:
					if( ipPointsCol == NULL )
						ipPointsCol.CreateInstance( CLSID_GeometryBag );
					ipPointsCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolyline:
					if( ipPolylinesCol == NULL )
						ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
					ipPolylinesCol->AddGeometry( ipGeom );
					break;
				case esriGeometryPolygon:
					if( ipPolygonsCol == NULL )
						ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
					ipPolygonsCol->AddGeometry( ipGeom );
					break;
				default:
					_ASSERTE(false);
					break;
			}
		}
	}
	// それぞれをUnionする
	ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
	if( ipPointsCol != NULL )
	{
		ipPointsTopo.CreateInstance( CLSID_Multipoint );
		ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
	}
	if( ipPolylinesCol != NULL )
	{
		ipPolylinesTopo.CreateInstance( CLSID_Polyline );
		ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
	}
	if( ipPolygonsCol != NULL )
	{
		ipPolygonsTopo.CreateInstance( CLSID_Polygon );
		ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
	}
	// どれか一つしかなければそれを返り値とする
	bool bPoints    = ( ipPointsTopo != NULL && ipPolylinesTopo == NULL && ipPolygonsTopo == NULL );
	bool bPolylines = ( ipPointsTopo == NULL && ipPolylinesTopo != NULL && ipPolygonsTopo == NULL );
	bool bPolygons  = ( ipPointsTopo == NULL && ipPolylinesTopo == NULL && ipPolygonsTopo != NULL );
	if( bPoints || bPolylines || bPolygons )
	{
		if( bPoints )
			ipRet = ipPointsTopo;
		else if( bPolylines )
			ipRet = ipPolylinesTopo;
		else if( bPolygons )
			ipRet = ipPolygonsTopo;
	}
	else if( ipPointsTopo != NULL || ipPolylinesTopo != NULL || ipPolygonsTopo != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsTopo );
		if( ipPolylinesTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesTopo );
		if( ipPolygonsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsTopo );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

// SDEに入れるための値とした時の同じかどうかの判定用関数
bool IsEqualSDEValue( esriFieldType type, const CComVariant& va1, const CComVariant& va2 )
{
	// フィールドの型が文字列の場合はSDE上はNULLと空文字は区別されない
	// PGDBの場合は異なるが、ここでも空文字＝NULLと見なす
	bool va1_is_null = ( VT_NULL == va1.vt ? true : false );
	bool va2_is_null = ( VT_NULL == va2.vt ? true : false );
	if( esriFieldTypeString == type && va1.vt == VT_BSTR && 0 == wcslen( va1.bstrVal ) )
		va1_is_null = true;
	if( esriFieldTypeString == type && va2.vt == VT_BSTR && 0 == wcslen( va2.bstrVal ) )
		va2_is_null = true;

	// 両方ともNULL判定されたならtrue
	if( va1_is_null && va2_is_null )
		return true;

	// 片方がNULL判定されたらfalse
	if( va1_is_null || va2_is_null )
		return false;

	if( va1.vt == va2.vt )
		return ( va1 == va2 );

	// 型が異なる場合は合わせる
	VARTYPE vt = VT_I4;
	switch( type )
	{
	case esriFieldTypeInteger:
	case esriFieldTypeSmallInteger:
		vt = VT_I4;
		break;
	case esriFieldTypeSingle:
	case esriFieldTypeDouble:
		vt = VT_R8;
		break;
	case esriFieldTypeDate:    // 日付は文字列に変換して比較
	case esriFieldTypeString:
		vt = VT_BSTR;
		break;
	default: break;
	}
	CComVariant va1cpy, va2cpy;
	va1cpy.ChangeType( vt, &va1 );
	va2cpy.ChangeType( vt, &va2 );
	return ( va1cpy == va2cpy );
}

// targetをsepで分解し、listに格納する
void get_separated_values( const uh::tstring& target, const uh::tstring& sep_char, std::list<uh::tstring>& values )
{
	boost::char_separator<_TCHAR> sep( sep_char.c_str(), _T(""), boost::keep_empty_tokens );
	Tokenizer tokens( target, sep );
	foreach( const uh::tstring& str, std::make_pair( tokens.begin(), tokens.end() ) )
		values.push_back( str );
}

bool g_bStoreShapeJudge_Native = false;
bool g_bStoreShapeJudge_Exact = false;
bool g_bStoreShapeJudge_PointZ_Exact = false;
bool g_bShapeChanged_UseRelationalOperator = false; //!< CRowBase::ShapeChangedでIRelationalOperator::Equalsを使用するかどうか（デフォルト：false）
