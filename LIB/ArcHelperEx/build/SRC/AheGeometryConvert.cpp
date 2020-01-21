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

/**
 * @file AheGeometryConvert.cpp
 * @brief <b>ジオメトリコンバート系実装ファイル</b>\n
 * @author コンテンツ本部技術開発部制作部開発グループ 古川貴宏
 * この関数群は、SiNDYLibのGeometryConvert.cppを参考に作りました。
 * @version $Id$
 */
#include "stdafx.h"
#include "AheGeometryConvert.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ジオメトリからマルチポイントを生成する
IGeometryPtr ToMultipoint( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// 返り値

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Multipoint );
	IPointCollectionPtr ipRetCol( ipRet );	// 失敗することは無いはずなのでエラーチェックしない（面倒だし）
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryPoint:
			// Point、MultiPointはポイントを取得する場合にコピーではなく参照を返す
			// この関数自体どこから呼ばれるかわからないため、ポイントの場合はCloneを作成した方が良い
			ipRetCol->AddPoint( (IPointPtr)AheCreateClone( (IUnknownPtr)ipGeom ) );
			break;
		case esriGeometryPolygon:
		case esriGeometryRing:
		case esriGeometryEnvelope:
		case esriGeometryPolyline:
		case esriGeometryPath:
		{
			IPointCollectionPtr ipCol( ipGeom );
			long lPointCount = 0;
			WKSPoint* pPoints = NULL;
			if( SUCCEEDED( hr = ipCol->get_PointCount( &lPointCount ) ) )
			{
				if( NULL != ( pPoints = new WKSPoint[lPointCount] ) )
				{
					if( SUCCEEDED( hr = ipCol->QueryWKSPoints( 0, lPointCount, pPoints ) ) )
						ATLVERIFY( SUCCEEDED( hr = ipRetCol->SetWKSPoints( lPointCount, pPoints ) ) );
					else
						_ASSERTE( SUCCEEDED(hr) );
				}
				else
					_ASSERTE( false );
			}
			else
				_ASSERTE( SUCCEEDED(hr) );

			if( pPoints ) 
				delete [] pPoints;

			break;
		}
		case esriGeometryMultipoint: 
			ipRet = AheCreateClone( ipGeom ); 
			break;
		default:
			_ASSERTE(false); // 未サポート
			break;
	}

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// ジオメトリからポリラインを生成する
IGeometryPtr ToPolyline( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// 返り値

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Polyline );
	IGeometryCollectionPtr ipRetCol( ipRet );	// 失敗することは無いはずなのでエラーチェックしない（面倒だし）
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryPath:
		case esriGeometryRing:
			ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( (IGeometryPtr)AheCreateClone( (IUnknownPtr)ipGeom ) ) ) );
			break;
		case esriGeometryEnvelope:
			ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToPath( ipGeom ) ) ) );
			break;
		case esriGeometryPolygon:
		{
			IGeometryCollectionPtr ipGeomCol( ipGeom );
			long lGeomCount = 0;
			if( SUCCEEDED( hr = ipGeomCol->get_GeometryCount( &lGeomCount ) ) )
			{
				for( long i = 0; i < lGeomCount; ++i )
				{
					IGeometryPtr ipPart;
					if( SUCCEEDED( hr = ipGeomCol->get_Geometry( i, &ipPart ) ) )
						ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToPath( ipPart ) ) ) );
					else
						_ASSERTE( SUCCEEDED(hr) );
				}
			}
			else
				_ASSERTE( SUCCEEDED(hr) );
			break;
		}
		case esriGeometryLine:
		case esriGeometryCircularArc:
			ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToPath( ipGeom ) ) ) );
			break;
		case esriGeometryPolyline:
			ipRet = AheCreateClone( ipGeom ); 
			break;
		default:
			_ASSERTE( false ); // 未サポート
			break;
	}

	// KnownSimpleなジオメトリが変換後にKnownSimpleじゃなくなるので、ここで
	// KnownSimpleにしてやる（KnownSimpleじゃないとWKB関連のメソッドが失敗？するらしい）
	((ITopologicalOperator2Ptr)ipRet)->put_IsKnownSimple( /*vbIsKnownSimple*/VARIANT_TRUE );

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// ジオメトリからパスを生成する
IGeometryPtr ToPath( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// 返り値

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	switch( emType )
	{
		case esriGeometryRing: // Ring -> Path
		{
			// 現状 Ring -> Path に変換できるものは ICurve::GetSubCurve しかしらない…
			double dLen = 0.0;
			ICurvePtr ipCurve( ipGeom );
			ipCurve->get_Length( &dLen );
			ICurvePtr ipPath;
			if( SUCCEEDED( hr = ipCurve->GetSubcurve( 0.0, dLen, VARIANT_FALSE, &ipPath ) ) )
				ipRet = ipPath;
			else
				_ASSERTE( SUCCEEDED(hr) );
			break;
		}
		case esriGeometryPath: // Path -> Path
			ipRet = AheCreateClone( ipGeom );
			break;
		default:
			ipRet.CreateInstance( CLSID__Path );
			IPointCollectionPtr ipRetCol( ipRet );	// 失敗することは無いはずなのでエラーチェックしない（面倒だし）
			ipRet->putref_SpatialReference( ipSpRef );

			switch( emType )
			{
				case esriGeometryEnvelope: // Envelope -> Path
				{
					IEnvelopePtr ipEnv( ipGeom );
					double XMin = 0.0, YMin = 0.0, XMax = 0.0, YMax = 0.0;
					if( SUCCEEDED( hr = ipEnv->QueryCoords( &XMin, &YMin, &XMax, &YMax ) ) )
					{
						WKSPoint* pPoints = new WKSPoint[5];
						if( pPoints )
						{
							pPoints[0].X = XMin; pPoints[0].Y = YMin;
							pPoints[1].X = XMin; pPoints[1].Y = YMax;
							pPoints[2].X = XMax; pPoints[2].Y = YMax;
							pPoints[3].X = XMax; pPoints[3].Y = YMin;
							pPoints[4] = pPoints[0];
							ATLVERIFY( SUCCEEDED( hr = ipRetCol->SetWKSPoints( 5, pPoints ) ));
						}
						else
							_ASSERTE( pPoints );

						if( pPoints ) 
							delete [] pPoints;	// 後始末
					}
					else
						_ASSERTE( SUCCEEDED(hr) );

					break;
				}
				case esriGeometryLine: // Line -> Path
				case esriGeometryCircularArc: // CircularArc -> Path
					ATLVERIFY( SUCCEEDED( hr = ((ISegmentCollectionPtr)ipRet)->AddSegment( (_ISegmentPtr)AheCreateClone( ipGeom ) ) ) );
					break;
				default: 
					_ASSERTE(false); // 未サポート
					break;
			}
			break;
	}

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
		
}

// ジオメトリからポリゴンを生成する
IGeometryPtr ToPolygon( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// 返り値

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Polygon );
	IGeometryCollectionPtr ipRetCol( ipRet );	// 失敗することは無いはずなのでエラーチェックしない（面倒だし）
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryRing:	ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( (IGeometryPtr)AheCreateClone( (IUnknownPtr)ipGeom ) ) ) ); break;
		case esriGeometryEnvelope: ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToRing( ipGeom ) ) ) ); break;
		case esriGeometryPolygon: ipRet = AheCreateClone( ipGeom ); break;
		default: _ASSERTE( false ); break; // 未サポート
	}

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// ジオメトリからリングを生成する
IGeometryPtr ToRing( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// 返り値

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;
	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Ring );
	IPointCollectionPtr ipRetCol( ipRet );	// 失敗することは無いはずなのでエラーチェックしない（面倒だし）
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryEnvelope:
		{
			IEnvelopePtr ipEnv( ipGeom );
			double XMin = 0.0, YMin = 0.0, XMax = 0.0, YMax = 0.0;
			if( SUCCEEDED( hr = ipEnv->QueryCoords( &XMin, &YMin, &XMax, &YMax ) ) )
			{
				WKSPoint* pPoints = new WKSPoint[5];
				if( pPoints )
				{
					pPoints[0].X = XMin; pPoints[0].Y = YMin;
					pPoints[1].X = XMin; pPoints[1].Y = YMax;
					pPoints[2].X = XMax; pPoints[2].Y = YMax;
					pPoints[3].X = XMax; pPoints[3].Y = YMin;
					pPoints[4] = pPoints[0];
					ATLVERIFY( SUCCEEDED( hr = ipRetCol->SetWKSPoints( 5, pPoints ) ) );
				}
				else
					_ASSERTE( pPoints );

				if( pPoints ) 
					delete [] pPoints;	// 後始末
			}
			break;
		}
		case esriGeometryRing: ipRet = AheCreateClone( ipGeom ); break;
		default: _ASSERTE( false ); break; // 未サポート
	}
	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// ジオメトリから与えられた次元のジオメトリを生成する
IGeometryPtr Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim )
{
	IGeometryPtr ipRet;	// 返り値

	if( ! ipGeom )
		return ipRet;

	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );

	switch( emGeomDim )
	{
		case esriGeometry0Dimension:
			switch( emType )
			{
				case esriGeometryPolygon:
				case esriGeometryPolyline:
				case esriGeometryRing:
				case esriGeometryPath:
				case esriGeometryEnvelope:
				case esriGeometryPoint:
					ipRet = ToMultipoint( ipGeom );
					break;
				default: _ASSERTE( false ); break; // 未サポート
			}
			break;
		case esriGeometry1Dimension:
			switch( emType )
			{
				case esriGeometryPolygon:
				case esriGeometryRing:
				case esriGeometryEnvelope:
				case esriGeometryPath:
					ipRet = ToPolyline( ipGeom );
					break;
				default: _ASSERTE( false ); break; // 未サポート
			}
			break;
		case esriGeometry2Dimension:
			switch( emType )
			{
				case esriGeometryRing:
				case esriGeometryEnvelope:
					ipRet = ToPolygon( ipGeom );
					break;
				default: _ASSERTE( false ); break; // 未サポート
			}
			break;
		default: _ASSERTE( false ); break; // 未サポート
	}
	return ipRet;
}
