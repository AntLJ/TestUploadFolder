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
#include "AheGeometryOp.h"
#include "AheGlobals.h"
#include "GlobalFunctions.h"
#include "math2.h"
#include "WKSHelper.h"
#include "AheBugEvasion.h"
#include "AheMeshUtil.h"
#include "AheGeometryConvert.h"
#include "sde_ondemand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_FAIL_GET_WKSIZE  = _T("IWkb::get_WkbSizeに失敗");
	LPCTSTR ERROR_FAIL_REPAIR      = _T("修復に失敗しました。");
	LPCTSTR ERROR_GEO_IS_NOT_FIXED = _T("ジオメトリの修復失敗");

#else
	LPCTSTR ERROR_FAIL_GET_WKSIZE  = _T("Failed to invoke IWkb::get_WkbSize");
	LPCTSTR ERROR_FAIL_REPAIR      = _T("Failed to be repaired.");
	LPCTSTR ERROR_GEO_IS_NOT_FIXED = _T("The geometry could not be fixed.");

#endif // SINDY_FOR_ENGLISH
}


// エリアポイントを取得する
IPointPtr AheGetAreaPoint( IGeometry* ipGeom )
{
	WKSPoint p = {-1, -1};

	IAreaPtr ipArea( ipGeom );
	IPointPtr ipPoint( ipGeom );
	if( ipArea )
		ipArea->get_LabelPoint( &ipPoint );

	if( ipPoint )
		ipPoint->QueryCoords( &p.X, &p.Y );

	return ipPoint;
}

// 始点を取得する
IPointPtr AheGetFromPoint( IGeometry* ipGeom )
{
	ICurvePtr ipCurve( ipGeom );
	if( ! ipCurve )
	  return NULL;

	IPointPtr ret; // 戻り値
	ipCurve->get_FromPoint( &ret );
	return ret;
}

// 終点を取得する
IPointPtr AheGetToPoint( IGeometry* ipGeom )
{
	ICurvePtr ipCurve( ipGeom );
	if( ! ipCurve )
	  return NULL;

	IPointPtr ret; // 戻り値
	ipCurve->get_ToPoint( &ret );
	return ret;
}

// 端点同士の接続点を取得する
IPointPtr AheGetConnectPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	IPointPtr ipFromPoint1( AheGetFromPoint( ipGeom1 ) );
	IPointPtr ipFromPoint2( AheGetFromPoint( ipGeom2 ) );
	IPointPtr ipToPoint1( AheGetToPoint( ipGeom1 ) );
	IPointPtr ipToPoint2( AheGetToPoint( ipGeom2 ) );

	if( AheIsEqual( ipFromPoint1, ipFromPoint2 ) ||
		AheIsEqual( ipFromPoint1, ipToPoint2 ) )
		return ipFromPoint1;
	if( AheIsEqual( ipToPoint1, ipFromPoint2 ) ||
		AheIsEqual( ipToPoint1, ipToPoint2 ) )
		return ipToPoint1;

	return IPointPtr();
}

// WKSPointを作成する
WKSPoint MakeWKSPoint( IPoint* ipPoint )
{
	WKSPoint p = {-1.0,-1.0}; // 戻り値
	if( ipPoint )
		ipPoint->QueryCoords( &p.X, &p.Y );

	return p;
}

// WKSPointを作成する
WKSPoint MakeWKSPoint( const double& x, const double& y )
{
	WKSPoint p = { x, y };
	return p;
}

/////////////////////////////////////////////////////////////////////////////
// ジオメトリ変換等
/////////////////////////////////////////////////////////////////////////////
// IEnvelope -> IPolygon 変換
IPolygonPtr AheEnvelope2Polygon( IEnvelopePtr ipEnv )
{
	// 引数チェック
	if( ipEnv == NULL )
	{
		_ASSERTE( ipEnv != NULL );
		return NULL;
	}
	ISpatialReferencePtr ipSpRef;
	HRESULT hr = ipEnv->get_SpatialReference( &ipSpRef );

	IPointCollectionPtr ipPolygon( CLSID_Polygon );
	((IGeometryPtr)ipPolygon)->putref_SpatialReference( AheGetSpatialReference( ipEnv ) ); // 空間参照を付与 
	IPointPtr ipUpperLeft, ipUpperRight, ipLowerLeft, ipLowerRight;

	ipEnv->get_UpperLeft( &ipUpperLeft );
	ipEnv->get_UpperRight( &ipUpperRight );
	ipEnv->get_LowerLeft( &ipLowerLeft );
	ipEnv->get_LowerRight( &ipLowerRight );

	if( ipPolygon != NULL )
	{
		ipPolygon->AddPoint( ipUpperLeft );
		ipPolygon->AddPoint( ipUpperRight );
		ipPolygon->AddPoint( ipLowerRight );
		ipPolygon->AddPoint( ipLowerLeft );
		ipPolygon->AddPoint( ipUpperLeft );
	}
	
	_ASSERTE( ipPolygon != NULL );

	// [Bug 5041]ipPolygonは空間参照があたってないのであてておく
	IPolygonPtr ipRetPolygon( ipPolygon );
	if( ipRetPolygon != NULL )
		hr = ipRetPolygon->putref_SpatialReference( ipSpRef );
	return ipRetPolygon;
}

// マルチポリゴンからポリゴンのセットを取得する
_ISetPtr AheConvertMultiPolygon2Polygons( IPolygonPtr ipPolygon )
{
	if( ipPolygon == NULL ) return NULL;

	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	ipPolygon->get_SpatialReference( &ipSpRef );

	IPointCollectionPtr ipPointCol( ipPolygon );
	if( ipPointCol != NULL )
	{
		LONG lPointCount = -1;
		ipPointCol->get_PointCount( &lPointCount );
		WKSPoint* pPoints = new WKSPoint[lPointCount];
		ipPointCol->QueryWKSPoints( 0, lPointCount, pPoints );

		// 各リングに分割する
		std::list<RING> listRings;
		InitRing( lPointCount, pPoints, listRings );
		// 包含関係をチェック
		CheckExteriorInterior( pPoints, listRings );

		// 各ポリゴンに分割する
		_ISetPtr ipRetSet( CLSID_Set );
		if( listRings.size() < 2 )
		{
			ipRetSet->Add( AheGetClone(ipPolygon) );
			delete [] pPoints;
			return ipRetSet;
		}
		else {
			for( std::list<RING>::iterator it = listRings.begin(); it != listRings.end(); it++ )
			{
				IGeometryCollectionPtr ipMultiPolygon( CLSID_Polygon );	// これに格納する
				((IGeometryPtr)ipMultiPolygon)->putref_SpatialReference( AheGetSpatialReference( ipPolygon ) ); // 空間参照付与
				// Exterior のみ
				if( it->IsClockwise == FALSE ) continue;

				// CROSSやWITHINがあってはいけない！！
				if( it->Cross.size() > 0 || it->Within.size() > 0 )
				{
					_ASSERTE( it->Cross.size() < 1 && it->Within.size() < 1 );
					delete [] pPoints;
					return NULL;
				}
				
				// Exterior
				LONG lExPartPointsCount = it->END - it->START + 1;
				WKSPoint* pExPartPoints = new WKSPoint[lExPartPointsCount];
				// 点列をブロック単位でコピー
				::CopyMemory( &(pExPartPoints[0]), &(pPoints[it->START]), sizeof(WKSPoint) * lExPartPointsCount );
				// リングを作成する
				IPointCollectionPtr ipExRingCol( CLSID_Ring );
				((IGeometryPtr)ipExRingCol)->putref_SpatialReference( ipSpRef ); // 空間参照付与
				ipExRingCol->SetWKSPoints( lExPartPointsCount, pExPartPoints );
				IRingPtr ipExRing( ipExRingCol );
				ipExRing->putref_SpatialReference( ipSpRef );

				// マルチポリゴンとして格納する
				ipMultiPolygon->AddGeometry( ipExRing );

				delete [] pExPartPoints;

				// Interior
				for( std::list<SIMPLERING>::iterator itIn = it->Contain.begin(); itIn != it->Contain.end(); itIn++ )
				{
					LONG lInPartPointsCount = itIn->END - itIn->START + 1;
					WKSPoint* pInPartPoints = new WKSPoint[lInPartPointsCount];
					// 点列をブロック単位でコピー
					::CopyMemory( &(pInPartPoints[0]), &(pPoints[itIn->START]), sizeof(WKSPoint) * lInPartPointsCount );
					// リングを作成する
					IPointCollectionPtr ipInRingCol( CLSID_Ring );
					((IGeometryPtr)ipInRingCol)->putref_SpatialReference( ipSpRef ); // 空間参照付与
					ipInRingCol->SetWKSPoints( lInPartPointsCount, pInPartPoints );
					IRingPtr ipInRing( ipInRingCol );
					ipInRing->putref_SpatialReference( ipSpRef );

					// マルチポリゴンとして格納する
					ipMultiPolygon->AddGeometry( ipInRing );

					delete [] pInPartPoints;
				}
				IPolygonPtr ipRetPolygon( ipMultiPolygon );
/*#ifdef DEBUG
				AheAddPolygonElement( NULL, ipRetPolygon, RGB(0,255,0), esriSFSSolid );
#endif // ifdef DEBUG*/
				ipRetPolygon->putref_SpatialReference( ipSpRef );
				ipRetPolygon->SimplifyPreserveFromTo();
/*#ifdef DEBUG
				AheAddPolygonElement( NULL, ipRetPolygon, RGB(255,0,0), esriSFSSolid );
#endif // ifdef DEBUG*/
				// セットに追加
				ipRetSet->Add( ipRetPolygon );
			}
			delete [] pPoints;
			return ipRetSet;
		}
	}
	_ASSERTE( FALSE );
	return NULL;
}	

// ポリゴンをポリラインに変換する
IGeometryPtr AheConvertPolygon2Polyline( IPolygonPtr ipPolygon )
{
	ISpatialReferencePtr ipSpRef;
	ipPolygon->get_SpatialReference( &ipSpRef );
	IPointCollectionPtr ipPointCol( ipPolygon );
	if( ipPointCol != NULL )
	{
		WKSPoint* pPoints;
		LONG lPointCount = -1;

		ipPointCol->get_PointCount( &lPointCount );
		pPoints = new WKSPoint[lPointCount];
		ipPointCol->QueryWKSPoints( 0, lPointCount, pPoints );

		ISegmentCollectionPtr ipRetSegCol( CLSID_Polyline );
		((IGeometryPtr)ipRetSegCol)->putref_SpatialReference( ipSpRef ); // 空間参照付与

		// 各リングに分割する
		std::list<RING> listRings;
		InitRing( lPointCount, pPoints, listRings );

		for( std::list<RING>::iterator it = listRings.begin(); it != listRings.end(); it++ )
		{
			LONG lPartPoints = it->END - it->START + 1;
			WKSPoint* pPartPoints = new WKSPoint[lPartPoints];
			// 点列をブロック単位でコピー
			::ZeroMemory( pPartPoints, sizeof(WKSPoint) * lPartPoints );	// メモリブロック初期化
			::CopyMemory( &pPartPoints[0], &pPoints[it->START], sizeof(WKSPoint) * lPartPoints );
			// セグメント追加
			IPointCollectionPtr ipPartPointCol( CLSID_Polyline );
			((IGeometryPtr)ipPartPointCol)->putref_SpatialReference( ipSpRef ); // 空間参照付与
			ipPartPointCol->SetWKSPoints( lPartPoints, pPartPoints );
			delete [] pPartPoints;
			pPartPoints = NULL;
			ipRetSegCol->AddSegmentCollection( (ISegmentCollectionPtr)ipPartPointCol );
		}
		IPolylinePtr ipRetPolyline( ipRetSegCol );
		ipRetPolyline->putref_SpatialReference( ipSpRef );
		try {
			ITopologicalOperator2Ptr ipTopo( ipRetPolyline );
			ipTopo->put_IsKnownSimple( VARIANT_FALSE );
			ipTopo->Simplify();
			ipRetPolyline->SimplifyNetwork();
		}
		catch(...) {
			ATLASSERT( FALSE );			
		}
		delete [] pPoints;

		return ipRetPolyline;
	}

	_ASSERTE( FALSE );
	return NULL;
}	

// IGeometry -> list<SEGMENTITEM> 変換
BOOL AheGeometry2SegmentItems( IGeometryPtr ipGeometry, std::list<SEGMENTITEM>& listSegments )
{
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeometry ) );
	
	// 引数チェック
	if( ipGeomCopy == NULL )
	{
		_ASSERTE( ipGeomCopy != NULL );
		return FALSE;
	}

	// ジオメトリ -> セグメント分解
	IGeometryCollectionPtr ipGeomCol( ipGeomCopy );
	LONG lGeomCount = -1;

	ipGeomCol->get_GeometryCount( &lGeomCount );
	for( LONG i = 0; i < lGeomCount; i++ )
	{
		IGeometryPtr ipPartGeom;
		ipGeomCol->get_Geometry( i, &ipPartGeom );

		esriGeometryType type;
		ipPartGeom->get_GeometryType( &type );

		// ポリゴン、ポリラインの場合のみ
		switch( type )
		{
			case esriGeometryRing:
			case esriGeometryPath:
			case esriGeometryPolygon:
			case esriGeometryPolyline:
			{
				// ポリゴンは Close 解除
				IPointCollectionPtr ipPointCol( ipPartGeom );
				if( ipPointCol != NULL )
				{
					LONG lPointCount = -1;
					ipPointCol->get_PointCount( &lPointCount );
					if( type == esriGeometryPolygon || type == esriGeometryRing )
					{
						ipPointCol->RemovePoints( lPointCount - 1, 1 );
						lPointCount--;
					}
				}
				ISegmentCollectionPtr ipSegCol( ipPointCol );
				if( ipSegCol != NULL )
				{
					LONG lSegmentCount = -1;
					ipSegCol->get_SegmentCount( &lSegmentCount );
					for( LONG j = 0; j < lSegmentCount; j++ )
					{
						SEGMENTITEM item;
						
						ipSegCol->get_Segment( j, &(item.ipSegment) );
						if( item.ipSegment != NULL )
						{
							item.ipSegment->get_FromPoint( &(item.ipFromPoint) );
							item.ipSegment->get_ToPoint( &(item.ipToPoint) );
							item.ipFromPoint->QueryCoords( &(item.dFromX), &(item.dFromY) );
							item.ipToPoint->QueryCoords( &(item.dToX), &(item.dToY) );
							item.geometryType = type;
							item.lPartID = i;
							item.lSegmentID = j;
							item.bIsFirstSegment = ( j == 0 ? TRUE : FALSE );
							item.bIsLastSegment = ( j == lSegmentCount - 1 ? TRUE : FALSE );

							listSegments.push_back( item );
						}
						else
							_ASSERTE( FALSE );
					}
				}
				else
					_ASSERTE( FALSE );
			}
		}
	}

	return TRUE;
}

// ArcObjectsの空間参照オブジェクトをSDE C APIの空間参照オブジェクトに変換する関数
SE_COORDREF AheSpatialReference2SE_COORDREF( ISpatialReference* ipSpRef )
{
	// [bug 1809、5125、5152] 回避のための関数です。
	// この関数の返り値はSE_coordref_freeで使用後に削除する必要があります。
	// この関数は非公開関数です。
	if( ! ipSpRef )
		return NULL;

	// 空間参照作成
	SE_COORDREF coordref = NULL;
	SSE_coordref_create( &coordref );

	// ファクトリコードセット
	long lFCode = -1;
	ipSpRef->get_FactoryCode( &lFCode );
	SSE_coordref_set_by_id( coordref, lFCode );

	// XYセット
	SE_ENVELOPE env = {0};
	ipSpRef->GetDomain( &env.minx, &env.maxx, &env.miny, &env.maxy );
	SSE_coordref_set_xy_by_envelope( coordref, &env );

	// Mセット
	double min_value = 0.0, max_value = 0.0;
	ipSpRef->GetMDomain( &min_value, &max_value );
	SSE_coordref_set_m_by_range( coordref, min_value, max_value );

	// Zセット
	min_value = 0.0, max_value = 0.0;
	ipSpRef->GetZDomain( &min_value, &max_value );
	SSE_coordref_set_z_by_range( coordref, min_value, max_value );

	return coordref;
}

// ArcObjectsのジオメトリオブジェクトをSDE C API のジオメトリオブジェクトに変換する関数
std::pair<SE_SHAPE,SE_COORDREF> AheGeometry2SE_SHAPE( IGeometry* ipGeom )
{
	// [bug 1809、5125、5152] 回避のための関数です。
	// WKBを使用して変換を行います。
	// この関数の返り値はSE_shape_free,SE_coordref_freeで使用後に削除する必要があります。
	// この関数は非公開関数です。

	std::pair<SE_SHAPE,SE_COORDREF> pairRet = std::pair<SE_SHAPE,SE_COORDREF>(NULL,NULL); // 返り値

	// ipGeom -> WKB
	if( ! ipGeom )
		return pairRet;

	// ファクトリコードを取得
	long lFCode = -1;
	ISpatialReferencePtr ipSpRef;
	ipGeom->get_SpatialReference( &ipSpRef );
	if( ipSpRef != NULL )
		ipSpRef->get_FactoryCode( &lFCode );

	// KnownSimpleがFALSEだとget_WkbSizeに失敗する！！
	// ここで処理するのがいいのか分からないが、とりあえず
	// TRUEにしておく（問題は無いと思う）
	ITopologicalOperator2Ptr ipTopo( ipGeom );
	if( ipTopo )
	{
		VARIANT_BOOL vbIsKnownSimple = VARIANT_FALSE;
		if( SUCCEEDED( ipTopo->get_IsKnownSimple( &vbIsKnownSimple ) ) )
		{
			if( ! vbIsKnownSimple )
				if( FAILED( ipTopo->put_IsKnownSimple( VARIANT_TRUE ) ) )
					_ASSERTE(false); // put_IsKnownSimpleに失敗
		}
		else
			_ASSERTE(false); // get_IsKnownSimpleに失敗
	}

	// Well Known Binary のサイズ取得
	IWkbPtr ipWKB( ipGeom );
	long lBytes = 0;
	if( SUCCEEDED( ipWKB->get_WkbSize( &lBytes ) ) )
	{
		if( 0 < lBytes )
		{
			// バッファ確保
			BYTE* pBuf = new BYTE[lBytes];
			if( pBuf )
			{
				// バッファに書き込み
				if( SUCCEEDED( IWkb_ExportToWkb( ipWKB, &lBytes, pBuf ) ) )
				{
					pairRet.second = AheSpatialReference2SE_COORDREF( ipSpRef );
					SSE_shape_create( pairRet.second, &pairRet.first );
					SSE_shape_generate_from_WKB( (const char*)pBuf, pairRet.first );
				}
				delete [] pBuf;
				pBuf = NULL;
			}
		}
		else {
			//CGeometry::Trace( ipGeom, _T("lBytes == 0 !!") );
		}
	}
	else 
	{
		AheTraceGeometry( ipGeom, ERROR_FAIL_GET_WKSIZE );
		_ASSERTE(false); // IWkb::get_WkbSizeに失敗
	}

	return pairRet;
}

#ifdef _USE_GEOS
geos::geom::Geometry Geometry2Geometry( IGeometry* ipGeom )
{
	// ipGeom -> WKB
	if( ! ipGeom )
		return NULL;

	geos::geom::Geometry *pGeom = NULL; // 返り値

	// ファクトリコードを取得
	long lFCode = -1;
	ISpatialReferencePtr ipSpRef;
	ipGeom->get_SpatialReference( &ipSpRef );
	if( ipSpRef != NULL )
		ipSpRef->get_FactoryCode( &lFCode );

	// Well Known Binary のサイズ取得
	IWkbPtr ipWKB( ipGeom );
	long lBytes = 0;
	if( SUCCEEDED( ipWKB->get_WkbSize( &lBytes ) ) )
	{
		if( 0 < lBytes )
		{
			// バッファ確保
			BYTE* pBuf = new BYTE[lBytes];
			if( pBuf )
			{
				// バッファに書き込み
				if( SUCCEEDED( IWkb_ExportToWkb( ipWKB, &lBytes, pBuf ) ) )
				{
					// バイナリストリームにする
					std::string strStream;
					strStream.assign( (const char*)pBuf, lBytes );
					std::istringstream cStream( strStream, std::istringstream::in | std::istringstream::binary );

					// 作成したストリームを読み込み
					geos::io::WKBReader cReader;
					pGeom = cReader.read( cStream );

					// SRIDセット
					pGeom->setSRID( lFCode );
				}
				delete [] pBuf;
				pBuf = NULL;
			}
		}
		else {
			//CGeometry::Trace( ipGeom, _T("lBytes == 0 !!") );
		}
	}
	else
		_ASSERTE(false); // IWkb::get_WkbSizeに失敗

	return pGeom;
}
#endif // ifdef _USE_GEOS
/////////////////////////////////////////////////////////////////////////////
// 長さ、角度など
/////////////////////////////////////////////////////////////////////////////
// 3点のなす角（0<=x<=180）を求める
DOUBLE AheGetAngle( IPointPtr ipPoint1, IPointPtr ipPointCenter, IPointPtr ipPoint2 )
{
	if( ipPoint1 == NULL || ipPointCenter == NULL || ipPoint2 == NULL ) return 0;

	IConstructAnglePtr	ipConstructAngle(CLSID_GeometryEnvironment);
	DOUBLE				dRad = 0.0, dAngle = 0.0;

	if ( ipConstructAngle == NULL )
		return 0;
	if( FAILED( ipConstructAngle->ConstructThreePoint( ipPoint1, ipPointCenter, ipPoint2, &dRad ) ) )
	{
		AheTraceErrorMessage( _T("AheGetAngle") );
		return 0;
	}
	dAngle = ( 180.0 * dRad ) / M_PI;
	_ASSERTE( -180.0 <= dAngle && dAngle <= 180.0 );

	return dAngle;
}

// ラインの接続角を求める
DOUBLE AheGetAngle( ICurvePtr ipCurve1, ICurvePtr ipCurve2 )
{
	ISpatialReferencePtr ipSpRef;

	if( ipCurve1 != NULL )
		ipCurve1->get_SpatialReference( &ipSpRef );
	if( AheIsTouchEndPoint( ipCurve1, ipCurve2, ipSpRef ) == FALSE ) 
	{
		//ATLASSERT( FALSE );
		return 0;
	}

	IPointPtr ipFromPoint1, ipFromPoint2, ipToPoint1, ipToPoint2;
	LONG lPointCount1 = -1, lPointCount2 = -1;
	IPointPtr ipPoint1, ipPointCenter, ipPoint2;

	ipCurve1->get_FromPoint( &ipFromPoint1 );
	ipCurve1->get_ToPoint( &ipToPoint1 );
	ipCurve2->get_FromPoint( &ipFromPoint2 );
	ipCurve2->get_ToPoint( &ipToPoint2 );
	((IPointCollectionPtr)ipCurve1)->get_PointCount( &lPointCount1 );
	((IPointCollectionPtr)ipCurve2)->get_PointCount( &lPointCount2 );

	if( AheIsEqual( ipFromPoint1, ipFromPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( 1, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( 1, &ipPoint2 );
		ipPointCenter = ipFromPoint1;
	}
	else if( AheIsEqual( ipToPoint1, ipToPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( lPointCount1 - 2, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( lPointCount2 - 2, &ipPoint2 );
		ipPointCenter = ipToPoint1;
	}
	else if( AheIsEqual( ipFromPoint1, ipToPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( 1, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( lPointCount2 - 2, &ipPoint2 );
		ipPointCenter = ipFromPoint1;
	}
	else if( AheIsEqual( ipToPoint1, ipFromPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( lPointCount1 - 2, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( 1, &ipPoint2 );
		ipPointCenter = ipToPoint1;
	}

	return AheGetAngle( ipPoint1, ipPointCenter, ipPoint2 );
}

// 内積を求める（ipPoint1->ipPoint2 , ipPoint3->ipPoint4）
DOUBLE AheGetInnerProduct( IPointPtr ipPoint1, IPointPtr ipPoint2, IPointPtr ipPoint3, IPointPtr ipPoint4 )
{
	if( ipPoint1 == NULL || ipPoint2 == NULL || ipPoint3 == NULL || ipPoint4 == NULL ) return -1;

	DOUBLE x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
	DOUBLE x3 = 0.0, y3 = 0.0, x4 = 0.0, y4 = 0.0;

	ipPoint1->QueryCoords( &x1, &y1 );
	ipPoint2->QueryCoords( &x2, &y2 );
	ipPoint3->QueryCoords( &x3, &y3 );
	ipPoint4->QueryCoords( &x4, &y4 );

	return (x2-x1)*(x4-x3) + (y2-y1)*(y4-y3);
}

// ベクトル長を求める（ipPoint1->ipPoint2）
DOUBLE AheGetLength( IPointPtr ipPoint1, IPointPtr ipPoint2 )
{
	DOUBLE x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;

	ipPoint1->QueryCoords( &x1, &y1 );
	ipPoint2->QueryCoords( &x2, &y2 );

	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

// 距離（m）を求める
double AheGetMeterLength( IPointPtr& ipP1, IPointPtr& ipP2 )
{
	double dx1 = 0.0, dx2 = 0.0, dy1 = 0.0, dy2 = 0.0;
	ipP1->QueryCoords( &dx1, &dy1 );
	ipP2->QueryCoords( &dx2, &dy2 );
	return AheGETDIST( dx1, dy1, dx2, dy2 );
}

// 正規化座標距離を求める
double AheGetMeshXYLength( IPointPtr& ipP1, IPointPtr& ipP2 )
{
	double dx1 = 0.0, dx2 = 0.0, dy1 = 0.0, dy2 = 0.0;
	ipP1->QueryCoords( &dx1, &dy1 );
	ipP2->QueryCoords( &dx2, &dy2 );
	int x1 = 0, y1 = 0;
    int x2 = 0, y2 = 0;
	unsigned int mesh1 = AheLL2MESH( dx1, dy1, 2, x1, y1 );
	unsigned int mesh2 = AheLL2MESH( dx2, dy2, 2, x2, y2 );
	double delta_x = 0.0, delta_y = 0.0;
	AheGETDIFF(  mesh1, x1, y1, mesh2, x2, y2, delta_x, delta_y );
	return sqrt( delta_x*delta_x + delta_y*delta_y );
}
/////////////////////////////////////////////////////////////////////////////
// RelationalOperator系
/////////////////////////////////////////////////////////////////////////////
BOOL Geometry_Relationship( SE_SHAPE pBase, SE_SHAPE pTest, sindyeSpatialRel ope )
{
	// [bug 1809、5125、5152] 回避のための関数です。
	// WKBを使用して変換を行います。
	// この関数の返り値はSE_shape_freeで使用後に削除する必要があります。
	// この関数は非公開関数です。
	// NOTE: opeは複数指定不可

	if( ! ( pBase && pTest ) )
		return FALSE;

	switch( ope )
	{
	case sindyeSpatialRelEqual:    return SSE_shape_is_equal(       pBase, pTest ); break;
	case sindyeSpatialRelTouch:    return SSE_shape_is_touching(    pBase, pTest ); break;
	case sindyeSpatialRelWithin:   return SSE_shape_is_within(      pBase, pTest ); break;
	case sindyeSpatialRelContain:  return SSE_shape_is_containing(  pBase, pTest ); break;
	case sindyeSpatialRelCross:    return SSE_shape_is_crossing(    pBase, pTest ); break;
	case sindyeSpatialRelDisjoint: return SSE_shape_is_disjoint(    pBase, pTest ); break;
	case sindyeSpatialRelOverlap:  return SSE_shape_is_overlapping( pBase, pTest ); break;
	default: break;
	}
	return FALSE;
}

// ジオメトリの関連をチェックする（SDE C API版）
sindyeSpatialRel AheTestGeometryRelationship_SE( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption )
{
	// [bug 1809、5125、5152] 回避のための関数です。
	// SDE C API の判定関数を使用して判定を行います。
	// 上記APIでは判定結果の揺らぎが無いことは確認済みです。
	// この関数は非公開関数です。
	sindyeSpatialRel retRel = sindyeSpatialRelNone;	// 返り値

	// Geometry -> SDE API Shape変換
	std::pair<SE_SHAPE,SE_COORDREF> pairBase = std::pair<SE_SHAPE,SE_COORDREF>(NULL,NULL);
	std::pair<SE_SHAPE,SE_COORDREF> pairTest = std::pair<SE_SHAPE,SE_COORDREF>(NULL,NULL);
	try {
		pairBase = AheGeometry2SE_SHAPE( ipBaseGeom );
		pairTest = AheGeometry2SE_SHAPE( ipTestGeom );

		_ASSERTE( pairBase.first && pairTest.first );

		if( relOption & sindyeSpatialRelEqual    ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelEqual    ) ? sindyeSpatialRelEqual    : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelTouch    ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelTouch    ) ? sindyeSpatialRelTouch    : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelWithin   ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelWithin   ) ? sindyeSpatialRelWithin   : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelContain  ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelContain  ) ? sindyeSpatialRelContain  : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelCross    ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelCross    ) ? sindyeSpatialRelCross    : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelDisjoint ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelDisjoint ) ? sindyeSpatialRelDisjoint : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelOverlap  ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelOverlap  ) ? sindyeSpatialRelOverlap  : (sindyeSpatialRel)0 ) );
	}
	catch(...)
	{
		ATLTRACE(_T("[WARNING] AheTestGeometryRelationship_SE() : 例外発生\n") );
		_ASSERTE(false);
	}

	// 後始末（形状と空間参照削除する必要がある）
	SSE_shape_free( pairBase.first );
	SSE_shape_free( pairTest.first );
	SSE_coordref_free( pairBase.second );
	SSE_coordref_free( pairTest.second );

	return retRel;
}

// ジオメトリの関連をチェックする（ArcObjects版）
sindyeSpatialRel AheTestGeometryRelationship_AO( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// [bug 1809、5125、5152] 回避のため、SDE接続の場合は_SEの方を
	// 使用するようになりました。従って、この関数はSDE接続が一つもない
	// 場合に限り使用されます。
	// この関数は非公開関数です。
	sindyeSpatialRel retRel = sindyeSpatialRelNone;	// 返り値

	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipTestGeomCopy( AheGetClone( ipTestGeom ) );

	if( ( ! ipBaseGeomCopy ) || ( ! ipTestGeomCopy ) || relOption == 0 )
		return retRel;

	// 空間参照が与えられていたらそれをセット（あまり良くないか？？）
	if( ipSpRel != NULL )
	{
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRel );
		AheSnapToSpatialReference( ipTestGeomCopy, ipSpRel );
	}

	// 空間参照が違う場合は判定がちゃんとできないので、
	// 無理やり細かいほうの空間参照をセットする（点列には影響ないはず）
	if( ! AheHasSameSpatialReference( ipBaseGeomCopy, ipTestGeomCopy ) )
	{
		ATLTRACE(_T("AheTestGeometryRelationship() : 比較するジオメトリの空間参照が異なるため、一時的に空間参照のみ一致させます（点列には影響ありません）。\n"));
		ISpatialReferencePtr ipMinimumSpRef = AheGetMinimumSpatialReference( AheGetSpatialReference( ipBaseGeomCopy ), AheGetSpatialReference( ipTestGeomCopy ) );
		ipBaseGeomCopy->putref_SpatialReference( ipMinimumSpRef );
		ipTestGeomCopy->putref_SpatialReference( ipMinimumSpRef );
	}

	IRelationalOperatorPtr ipRelOpe( ipBaseGeomCopy );

	// 接触
	if( relOption & sindyeSpatialRelTouch )
	{
		VARIANT_BOOL vaIsTouch = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Touches( ipTestGeomCopy, &vaIsTouch ) ) )
		{
			if( vaIsTouch )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelTouch );
		}
		else
			_ASSERTE(false);
	}
	// オーバーラップ
	if( relOption & sindyeSpatialRelOverlap )
	{
		VARIANT_BOOL vaIsOverlap = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Overlaps( ipTestGeomCopy, &vaIsOverlap ) ) )
		{
			if( vaIsOverlap )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelOverlap );
		}
		else
			_ASSERTE(false);
	}
	// 交差
	if( relOption & sindyeSpatialRelCross )
	{
		VARIANT_BOOL vaIsCross = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Crosses( ipTestGeomCopy, &vaIsCross ) ) )
		{
			if( vaIsCross )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelCross );
		}
		else
			_ASSERTE(false);
	}
	// 含まれる
	if( relOption & sindyeSpatialRelWithin )
	{
		VARIANT_BOOL vaIsWithin = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Within( ipTestGeomCopy, &vaIsWithin ) ) )
		{
			if( vaIsWithin )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelWithin );
		}
		else
			_ASSERTE(false);
	}
	// 含んでいる
	if( relOption & sindyeSpatialRelContain )
	{ 
		VARIANT_BOOL vaIsContain = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Contains( ipTestGeomCopy, &vaIsContain ) ) )
		{
			if( vaIsContain )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelContain );
		}
		else
			_ASSERTE(false);
	}
	// 分離
	if( relOption & sindyeSpatialRelDisjoint )
	{
		VARIANT_BOOL vaIsDisjoint = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Disjoint( ipTestGeomCopy, &vaIsDisjoint ) ) )
		{
			if( vaIsDisjoint )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelDisjoint );
		}
		else
			_ASSERTE(false);
	}
	// 等しい
	if( relOption & sindyeSpatialRelEqual )
	{ 
		VARIANT_BOOL vaIsEqual = VARIANT_FALSE;
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
		if( SUCCEEDED( ipRelOpe->Equals( ipTestGeomCopy, &vaIsEqual ) ) )
		{
			if( vaIsEqual == VARIANT_TRUE )
			{
				// もしかして、構成点数が違っていても「等しい」くなるの？？
				// それっておかしくない？？ちなみに、未報告
				IPointCollectionPtr ipBaseCol( ipBaseGeomCopy ), ipTestCol( ipTestGeomCopy );
				if( ipBaseCol != NULL && ipTestCol != NULL )
				{
					LONG lBaseColCount = -1, lTestColCount = -1;
					ipBaseCol->get_PointCount( &lBaseColCount );
					ipTestCol->get_PointCount( &lTestColCount );
					if( lBaseColCount == lTestColCount )
						retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelEqual );
				}
				else
					retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelEqual );
			}
		}
#else
		// IRelationalOperator::EqualsはITopologicalOperator::Differenceを使用するのでかなり甘い結果がでる
		// それではうちの使用方法にはぜんぜん合わないのでIClone::IsEqualを使用する
		if( SUCCEEDED( ((IClonePtr)ipRelOpe)->IsEqual( (IClonePtr)ipTestGeomCopy, &vaIsEqual ) ) )
		{
			if( vaIsEqual )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelEqual );
		}
		else
			_ASSERTE(false);
#endif
	}

	return retRel;
}

// ジオメトリの関連をチェックする
sindyeSpatialRel AheTestGeometryRelationship( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// sdelibが使用できるかどうか確認
	// 空間参照が作成できるかどうかで判断する
	// SDE接続のレイヤがTOCから無くなった段階でエラー
	// になるため、無駄でも毎回チェックする必要がある…
#ifdef _NOT_USE_SDE_RELATIONSHIP
	return AheTestGeometryRelationship_AO( ipBaseGeom, ipTestGeom, relOption, ipSpRel );
#else
	bool bCanUseSDELIB = false;
	SE_COORDREF pRef = NULL;
	if( SE_SUCCESS == SSE_coordref_create(&pRef) )
	{
		SSE_coordref_free(pRef);
		bCanUseSDELIB = true;
	}

	if( bCanUseSDELIB )
		return AheTestGeometryRelationship_SE( ipBaseGeom, ipTestGeom, relOption );
	else
		return AheTestGeometryRelationship_AO( ipBaseGeom, ipTestGeom, relOption, ipSpRel );
#endif
}

// 基本ジオメトリが比較ジオメトリを含むか（同一形状含む）
BOOL AheIsContain3( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// 基本機能は AheIsContain と同じですが、AheTestGeometryRelationship で sindyRelNone が返って来た場合
	// に PointCollection に変換して比較します。
	BOOL bRet = TRUE;

	bRet = AheIsContain(ipBaseGeom, ipTestGeom, ipSpRel);

	if ( FALSE == bRet ) {
		bRet = TRUE;

		IRelationalOperatorPtr pIRelationalOperator;
		IPointCollectionPtr pIPointCollection;
		IPointPtr pIPoint;
		LONG lCount;
		VARIANT_BOOL bFlg = VARIANT_FALSE, bFlg2 = VARIANT_FALSE;

		pIPointCollection = ipTestGeom;
		pIRelationalOperator = ipBaseGeom;

		if ( pIPointCollection ) {
			pIPointCollection->get_PointCount(&lCount);

			for ( int i = 0; i < lCount; i++ ) {
				pIPointCollection->get_Point(i, &pIPoint);
				pIRelationalOperator->Touches(pIPoint, &bFlg);
				pIRelationalOperator->Contains(pIPoint, &bFlg2);

				if (( VARIANT_FALSE == bFlg ) && ( VARIANT_FALSE == bFlg2 )) { 
					bRet = FALSE;	
					break;
				}
			}
		} else {
			pIPoint = ipTestGeom;
			pIRelationalOperator->Touches(pIPoint, &bFlg);
			pIRelationalOperator->Contains(pIPoint, &bFlg2);

			if (( VARIANT_FALSE == bFlg ) && ( VARIANT_FALSE == bFlg2 )) { 
				bRet = FALSE;	
			}
		}
	}

	return bRet;
}

// 基本ジオメトリが比較ジオメトリを含むか（同一形状含む）
BOOL AheIsContain4( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// 基本機能は AheIsContain と同じですが、ipTestGeom が ポイント 以外の時は セグメント 毎に
	// 分解して比較します。
	ISpatialReferencePtr 	ipSpatialReference = ipSpRel;
	IGeometryPtr			ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr 			ipTestGeomCopy( AheGetClone( ipTestGeom ) );
	esriGeometryType		esriGeomType;

	if ( AheIsContain( ipBaseGeomCopy, ipTestGeomCopy, ipSpatialReference ) == FALSE ) {
		if ( ipBaseGeomCopy == NULL || ipTestGeomCopy == NULL )
			return FALSE;

		ipTestGeomCopy->get_GeometryType( &esriGeomType );
		if ( esriGeomType != esriGeometryPoint ) {
			IGeometryCollectionPtr 	ipGeometryCollection;
			IGeometryPtr 			ipSegmentGeometry;
			LONG 					lGeomCount = -1;

			if ( ipSpatialReference != NULL ) {
				AheSnapToSpatialReference( ipBaseGeomCopy, ipSpatialReference );
				AheSnapToSpatialReference( ipTestGeomCopy, ipSpatialReference );
			} else {
				ipBaseGeomCopy->get_SpatialReference( &ipSpatialReference );
			}

			if ( AheHasSameSpatialReference( ipBaseGeomCopy, ipTestGeomCopy ) == FALSE ) {
				ipSpatialReference = AheGetMinimumSpatialReference( AheGetSpatialReference( ipBaseGeomCopy ), AheGetSpatialReference( ipTestGeomCopy ) );
				ipBaseGeomCopy->putref_SpatialReference( ipSpatialReference );
				ipTestGeomCopy->putref_SpatialReference( ipSpatialReference );
			}

			ipSegmentGeometry = ipTestGeomCopy;
			AheSnapToSpatialReferenceAndForceSimplify( ipSegmentGeometry, ipSpatialReference ); // TODO: やめたほうが良いんじゃない？
			ipGeometryCollection = ipSegmentGeometry;
			if ( ipGeometryCollection == NULL )
				return FALSE;

			ISpatialReferencePtr ipTestSpRef( AheGetSpatialReference( ipTestGeomCopy ) ); // Test側の空間参照
			ipGeometryCollection->get_GeometryCount( &lGeomCount );
			for ( LONG iCnt = 0; iCnt < lGeomCount; iCnt++ ) {
				IGeometryPtr 			ipPartGeom;

				ipGeometryCollection->get_Geometry( iCnt, &ipPartGeom );

				ISegmentCollectionPtr 	ipSegmentCollection( ipPartGeom );
				LONG 					lSegCount = -1;

				if( ipSegmentCollection == NULL )
					return FALSE;

				ipSegmentCollection->get_SegmentCount( &lSegCount );
				for( LONG jCnt = 0; jCnt < lSegCount; jCnt++ ) {
					_ISegmentPtr 			ipSegment, ipSegmentCopy;
					IPolylinePtr			ipTestPolyline( CLSID_Polyline );
					ipTestPolyline->putref_SpatialReference( ipTestSpRef ); // Test側の空間参照付与
					IPointPtr				ipTestFromPoint( CLSID_Point ), ipTestToPoint( CLSID_Point );
					ipTestFromPoint->putref_SpatialReference( ipTestSpRef ); // Test側の空間参照付与
					ipTestToPoint->putref_SpatialReference( ipTestSpRef ); // Test側の空間参照付与
					DOUBLE					dbTestFromX = 0.0f, dbTestFromY = 0.0f, dbTestToX = 0.0f, dbTestToY = 0.0f;

					ipSegmentCollection->get_Segment( jCnt, &ipSegment );
					if( ipSegment == NULL )
						return FALSE;

					ipSegmentCopy = AheGetClone( (_ISegmentPtr)ipSegment );
					if ( ipSegmentCopy == NULL )
						return FALSE;

					ipTestFromPoint->PutCoords( 0.0f, 0.0f );
					ipTestToPoint->PutCoords( 0.0f, 0.0f );

					if ( FAILED( ipSegmentCopy->QueryFromPoint( ipTestFromPoint ) ) || FAILED( ipSegmentCopy->QueryToPoint( ipTestToPoint ) ) )
						return FALSE;
					ipTestFromPoint->QueryCoords( &dbTestFromX, &dbTestFromY );
					ipTestToPoint->QueryCoords( &dbTestToX, &dbTestToY );

					ipTestPolyline->SetEmpty();
					((IPointCollectionPtr)ipTestPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestFromPoint ) );
					((IPointCollectionPtr)ipTestPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestToPoint ) );

					AheSnapToSpatialReferenceAndForceSimplify( ipTestPolyline, ipSpatialReference ); // TODO: やめたほうが良いんじゃない？？

					if ( AheIsContain( ipBaseGeomCopy, ipTestPolyline, ipSpatialReference ) == FALSE ) {
						if ( AheIsWithin( ipTestGeomCopy, ipBaseGeomCopy, ipSpatialReference ) == FALSE )
							return FALSE;
					}
				}
			}
		} else {
			if ( AheIsContain3( ipBaseGeomCopy, ipTestGeomCopy, ipSpatialReference ) == FALSE )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL AheIsEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr /*ipSpRel = NULL*/ ){
	// Equal判定についてはIClone::IsEqualが使用できることが分かっているので、
	// それを使用する
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return FALSE;

	VARIANT_BOOL vbRet = VARIANT_FALSE;
	esriGeometryType emTypeBase = esriGeometryAny;
	esriGeometryType emTypeTest = esriGeometryAny;
	ipBaseGeom->get_GeometryType( &emTypeBase );
	ipTestGeom->get_GeometryType( &emTypeTest );

	// 比較対象がesriGeometryPointの時のみ、別途処理
	if( esriGeometryPoint == emTypeBase && esriGeometryPoint == emTypeTest )
	{
		return AheIsDBEqual( (IPointPtr)ipBaseGeom, (IPointPtr)ipTestGeom );
	}

	IRelationalOperatorPtr ipRelOpe(ipBaseGeom);
	if( SUCCEEDED( ipRelOpe->Equals( ipTestGeom, &vbRet ) ) )
	{
		if( vbRet )
		{
			// もしかして、構成点数が違っていても「等しい」くなるの？？
			// それっておかしくない？？ちなみに、未報告
			IPointCollectionPtr ipBaseCol( ipBaseGeom ), ipTestCol( ipTestGeom );
			if( ipBaseCol && ipTestCol )
			{
				LONG lBaseColCount = -1, lTestColCount = -1;
				ipBaseCol->get_PointCount( &lBaseColCount );
				ipTestCol->get_PointCount( &lTestColCount );
				if( lBaseColCount != lTestColCount )
					vbRet = VARIANT_FALSE;
			}
		}
	}

	return ( vbRet ) ? TRUE : FALSE;
}

// データベース内に格納した場合に同一点かどうかを検証する
BOOL AheIsDBEqual( IPointPtr ipP1, IPointPtr ipP2 )
{
	if( ! ( ipP1 && ipP2  ) )
	{
		_ASSERT_EXPR( ipP1 && ipP2, _T("渡されたポイントのどちらかがNULL") );
		return false;
	}

	ISpatialReferencePtr ipSpRef;
	ipP1->get_SpatialReference( &ipSpRef );
	// [Bug 7881]Yankモード時に右クリックキャンセルで「構成点の向きを正しく判断できませんでした」
	// のメッセージが出るバグ修正
	// sp1に空間参照ない場合に、sp2からとってみる
	if( ! ipSpRef )
		ipP2->get_SpatialReference( &ipSpRef );

	if( ! ipSpRef )
	{
		_ASSERT_EXPR( ipSpRef, _T("第一引数の空間参照がNULL") );
		return false;
	}

	WKSPoint p[2] = {0.0};
	ipP1->QueryCoords( &p[0].X, &p[0].Y );
	ipP2->QueryCoords( &p[1].X, &p[1].Y );

	// 計算に必要なものを取得
	double falseX = 0.0, falseY = 0.0, xyUnits = 0.0;
	ipSpRef->GetFalseOriginAndUnits( &falseX, &falseY, &xyUnits );

	// 丸め処理
	WKSPoint intp[2] = {0.0};
	intp[0].X = floor(((p[0].X - falseX) * xyUnits) + 0.5);
	intp[0].Y = floor(((p[0].Y - falseY) * xyUnits) + 0.5);
	intp[1].X = floor(((p[1].X - falseX) * xyUnits) + 0.5);
	intp[1].Y = floor(((p[1].Y - falseY) * xyUnits) + 0.5);

	return ( intp[0].X == intp[1].X && intp[0].Y == intp[1].Y );
}

BOOL AheIsExactEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr/* ipSpRel = NULL*/ )
{
	// IClone::IsEqualも1ユニットだと真と判断されるため、点列比較
	// を行う（bug 5620,5617）
#if 0
	// AheIsEqualはポイント比較で大量に使用される可能性もあるのでinlineに
	// また、Equal判定についてはIClone::IsEqualが使用できることが分かっているので、
	// それを使用する
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return FALSE;

	VARIANT_BOOL vbRet = VARIANT_FALSE;
	((IClonePtr)ipBaseGeom)->IsEqual( (IClonePtr)ipTestGeom, &vbRet );
	return ( vbRet ) ? TRUE : FALSE;
#else
	// 1. 点列比較を行う関係上同一座標点が＊連続して存在しない＊ことを
	//    前提にしている（連続していなければ別物なのでOK）
	// 2. ポリゴンはCloseされている必要がある

	// NULL判定
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return FALSE;

	// 空判定
	VARIANT_BOOL vbBaseEmpty = VARIANT_FALSE, vbTestEmpty = VARIANT_FALSE;
	ipBaseGeom->get_IsEmpty( &vbBaseEmpty );
	ipTestGeom->get_IsEmpty( &vbTestEmpty );
	// 通常ジオメトリが空であることを期待することはないと思われるので、
	// 両方空でもFALSEを返すことにする
	if( vbBaseEmpty || vbTestEmpty )
		return FALSE;

	// 型判定
	esriGeometryType emBase = esriGeometryAny, emTest = esriGeometryAny;
	ipBaseGeom->get_GeometryType( &emBase );
	ipTestGeom->get_GeometryType( &emTest );
	if( emBase != emTest )
		return FALSE;

	// 距離・エリア判定
	switch( emBase )
	{
		case esriGeometryPath:
		case esriGeometryLine:
		case esriGeometryPolyline:
		{
			double dBaseLen = 0, dTestLen = 0;
			((ICurvePtr)ipBaseGeom)->get_Length( &dBaseLen );
			((ICurvePtr)ipTestGeom)->get_Length( &dTestLen );
			if( dBaseLen != dTestLen )
				return FALSE;
			break;
		}
		case esriGeometryPolygon:
		case esriGeometryRing:
		{
			double dBaseArea = 0, dTestArea = 0;
			((IAreaPtr)ipBaseGeom)->get_Area( &dBaseArea );
			((IAreaPtr)ipTestGeom)->get_Area( &dTestArea );
			if( dBaseArea != dTestArea )
				return FALSE;
			break;
		}
		default: break;
	}

	// 点列判定
	switch( emBase )
	{
		case esriGeometryPoint:
		{
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			if ( AheIsEqual( ipBaseGeom, ipTestGeom ) )
#else
			double xBase = 0, yBase = 0, xTest = 0, yTest = 0;
			((IPointPtr)ipBaseGeom)->QueryCoords( &xBase, &yBase );
			((IPointPtr)ipTestGeom)->QueryCoords( &xTest, &yTest );
			if( xBase != xTest || yBase != yTest )
#endif
				return FALSE;
			break;
		}
		case esriGeometryEnvelope:
		{
			WKSEnvelope stBase = {0}, stTest = {0};
			((IEnvelopePtr)ipBaseGeom)->QueryWKSCoords( &stBase );
			((IEnvelopePtr)ipTestGeom)->QueryWKSCoords( &stTest );
			if( stBase.XMax != stTest.XMax || stBase.XMin != stTest.XMin ||
				stBase.YMax != stTest.YMax || stBase.YMin != stTest.YMin )
				return FALSE;
			break;
		}
		case esriGeometryPolygon:
		{
			// ポリゴンはリング単位で比較する
			IPolygonPtr ipBasePoly( ipBaseGeom );
			IPolygonPtr ipTestPoly( ipTestGeom );
			// Exteriorの比較
			long lBaseExCount = 0, lTestExCount = 0;
			ipBasePoly->get_ExteriorRingCount( &lBaseExCount );
			ipTestPoly->get_ExteriorRingCount( &lTestExCount );
			if( 0 == lBaseExCount || 0 == lTestExCount || lBaseExCount != lTestExCount )
				return FALSE; // SetWKSPointで作成したポリゴンはSimplifyPreserveFromToをしないと0になるみたい…なのでスルーしないようにここで阻止
			IRing** ppBaseExRing = new IRing*[lBaseExCount];
			IRing** ppTestExRing = new IRing*[lTestExCount];
			ipBasePoly->QueryExteriorRings( ppBaseExRing );
			ipTestPoly->QueryExteriorRings( ppTestExRing );
			for( int i = 0; i < lBaseExCount; ++i )
			{
				bool bIsEqual = false;
				int j = 0;
				for( ; j < lTestExCount; ++j )
				{
					if( AheIsEqual( ppBaseExRing[i], ppTestExRing[j] ) )
					{
						bIsEqual = true; // 一つでも同じものが見つかったらおしまい
						break;
					}
				}
				if( ! bIsEqual )
				{
					for( int n = 0; n < lBaseExCount; ++n )
					{
						ppBaseExRing[n]->Release();
						ppTestExRing[n]->Release();
					}
					delete [] ppBaseExRing;
					delete [] ppTestExRing;
					return FALSE;
				}
				else {
					// Exteriorが同じならその中のInterior同士で比較
					long lBaseInCount = 0, lTestInCount = 0;
					ipBasePoly->get_InteriorRingCount( ppBaseExRing[i], &lBaseInCount );
					ipTestPoly->get_InteriorRingCount( ppTestExRing[j], &lTestInCount );
					if( lBaseInCount != lTestInCount )
					{
						for( int n = 0; n < lBaseExCount; ++n )
						{
							ppBaseExRing[n]->Release();
							ppTestExRing[n]->Release();
						}
						delete [] ppBaseExRing;
						delete [] ppTestExRing;
						return FALSE;
					}
					IRing** ppBaseInRing = new IRing*[lBaseInCount];
					IRing** ppTestInRing = new IRing*[lTestInCount];
					ipBasePoly->QueryInteriorRings( ppBaseExRing[i], ppBaseInRing );
					ipTestPoly->QueryInteriorRings( ppTestExRing[j], ppTestInRing );
					for( int k = 0; k < lBaseInCount; ++k )
					{
						bool bIsEqualInterior = false;
						for( int l = 0; l < lTestInCount; ++l )
						{
							if( AheIsEqual( ppBaseInRing[i], ppTestInRing[j] ) )
							{
								bIsEqualInterior = true; // 一つでも同じものが見つかったらおしまい
								break;
							}
						}
						if( ! bIsEqualInterior )
						{
							for( int n = 0; n < lBaseExCount; ++n )
							{
								ppBaseExRing[n]->Release();
								ppTestExRing[n]->Release();
							}
							for( int n = 0; n < lBaseInCount; ++n )
							{
								ppBaseInRing[n]->Release();
								ppTestInRing[n]->Release();
							}
							delete [] ppBaseInRing;
							delete [] ppTestInRing;
							delete [] ppBaseExRing;
							delete [] ppTestExRing;
							return FALSE;
						}
					}
					for( int n = 0; n < lBaseInCount; ++n )
					{
						ppBaseInRing[n]->Release();
						ppTestInRing[n]->Release();
					}
					delete [] ppBaseInRing;
					delete [] ppTestInRing;
				}
			}
			for( int n = 0; n < lBaseExCount; ++n )
			{
				ppBaseExRing[n]->Release();
				ppTestExRing[n]->Release();
			}
			delete [] ppBaseExRing;
			delete [] ppTestExRing;
			break;
		}
		default:
		{
			IPointCollectionPtr ipBaseCol( ipBaseGeom );
			IPointCollectionPtr ipTestCol( ipTestGeom );
			if( ! ( ipBaseCol && ipTestCol ) )
				return FALSE;
			long lBaseCount = 0, lTestCount = 0;
			ipBaseCol->get_PointCount( &lBaseCount );
			ipTestCol->get_PointCount( &lTestCount );
			if( lBaseCount != lTestCount )
				return FALSE;
			// 点列確保
			// ただし、ポリゴンは最終点は開始点と重複するため除く
			// 関係で必ずCloseされている必要がある
			if( esriGeometryRing == emBase ){ --lBaseCount; --lTestCount; }
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			switch( emBase )
			{
				case esriGeometryPolyline:
					for( long i = 0; i < lBaseCount; ++i ) // 順方向
					{
						IPointPtr ipBasePoint;
						ipBaseCol->get_Point( i, &ipBasePoint );

						IPointPtr ipTestPoint;
						ipTestCol->get_Point( i, &ipTestPoint );

						if ( ! AheIsEqual( ipBasePoint, ipTestPoint ) )
						{
							for( long j = 0, k = lBaseCount - 1; j < lBaseCount; ++j, --k ) // 逆方向
							{
								ipBaseCol->get_Point( j, &ipBasePoint );
								ipTestCol->get_Point( k, &ipTestPoint );

								if ( ! AheIsEqual( ipBasePoint, ipTestPoint ) )
									return FALSE;
							}
						}
					}
					break;
				case esriGeometryRing:
				{
					// 同一点を見つける
					bool bFindSamePoint = false;
					long iStart = 0, jStart = 0;
					for( long i = 0; i < lBaseCount; ++i )
					{
						for( long j = 0; j < lTestCount; ++j )
						{
							IPointPtr ipBasePoint;
							ipBaseCol->get_Point( i, &ipBasePoint );

							IPointPtr ipTestPoint;
							ipTestCol->get_Point( j, &ipTestPoint );

							if ( AheIsEqual( ipBasePoint, ipTestPoint ) )
							{
								iStart = i;
								jStart = j;
								bFindSamePoint = true;
								break;
							}
						}
						if( bFindSamePoint )
							break;
					}
					if( ! bFindSamePoint )
						return FALSE;
					// 見つけた同一点から比較開始
					for( long i = iStart, j = jStart, iTotal = 0; iTotal  < lBaseCount; ++i, ++j, ++iTotal )
					{
						if( i == lBaseCount )
							i = 0;
						if( j == lTestCount )
							j = 0;

						IPointPtr ipBasePoint;
						ipBaseCol->get_Point( i, &ipBasePoint );

						IPointPtr ipTestPoint;
						ipTestCol->get_Point( j, &ipTestPoint );

						if ( ! AheIsEqual( ipBasePoint, ipTestPoint ) )
						{
							// 形状によっては隣り合わなくても同一点が存在する
							// 例：ドーナツポリゴンの内側と外側が一点で接している場合
							// しかし、現状のSiNDYではそのような形状の存在は困るはず
							// 従って、再検査は行わず不一致と判断する
							return FALSE;
						}
					}
					break;
				}
				default: 
					break;
			}
#else
			WKSPoint* pBase = new WKSPoint[lBaseCount];
			WKSPoint* pTest = new WKSPoint[lTestCount];
			ipBaseCol->QueryWKSPoints( 0, lBaseCount, pBase );
			ipTestCol->QueryWKSPoints( 0, lTestCount, pTest );
			switch( emBase )
			{
				case esriGeometryPolyline:
					for( long i = 0; i < lBaseCount; ++i ) // 順方向
					{
						if( pBase[i].X != pTest[i].X ||
							pBase[i].Y != pTest[i].Y )
						{
							for( long j = 0, k = lBaseCount - 1; j < lBaseCount; ++j, --k ) // 逆方向
							{
								if( pBase[j].X != pTest[k].X ||
									pBase[j].Y != pTest[k].Y )
								{
									delete [] pBase;
									delete [] pTest;
									return FALSE;
								}
							}
						}
					}
					break;
				case esriGeometryRing:
				{
					// 同一点を見つける
					bool bFindSamePoint = false;
					long iStart = 0, jStart = 0;
					for( long i = 0; i < lBaseCount; ++i )
					{
						for( long j = 0; j < lTestCount; ++j )
						{
							if( pBase[i].X == pTest[j].X &&
								pBase[i].Y == pTest[j].Y )
							{
								iStart = i;
								jStart = j;
								bFindSamePoint = true;
								break;
							}
						}
						if( bFindSamePoint )
							break;
					}
					if( ! bFindSamePoint )
					{
						delete [] pBase;
						delete [] pTest;
						return FALSE;
					}
					// 見つけた同一点から比較開始
					for( long i = iStart, j = jStart, iTotal = 0; iTotal  < lBaseCount; ++i, ++j, ++iTotal )
					{
						if( i == lBaseCount )
							i = 0;
						if( j == lTestCount )
							j = 0;
						if( pBase[i].X != pTest[j].X ||
							pBase[i].Y != pTest[j].Y )
						{
							// 形状によっては隣り合わなくても同一点が存在する
							// 例：ドーナツポリゴンの内側と外側が一点で接している場合
							// しかし、現状のSiNDYではそのような形状の存在は困るはず
							// 従って、再検査は行わず不一致と判断する
							delete [] pBase;
							delete [] pTest;
							return FALSE;
						}
					}
					break;
				}
				default: 
					break;
			}
			delete [] pBase;
			delete [] pTest;
#endif
			break;
		}
	}
	return TRUE;
#endif // if 0
}

/////////////////////////////////////////////////////////////////////////////
// TopologicalOperator系関数
/////////////////////////////////////////////////////////////////////////////
// ジオメトリの Union を取る
IGeometryPtr AheUnion( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;
	
	if( ipSpRef != NULL )
	{
		// 地理参照あわせ
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipAddGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
	ITopologicalOperatorPtr ipTopo2( ipAddGeomCopy );
	if( ipTopo1 == NULL || ipTopo2 == NULL ) return NULL;

	ipTopo1->Simplify();
	ipTopo2->Simplify();

	IGeometryPtr ipRetGeom;
	if( SUCCEEDED( ipTopo1->Union( ipAddGeomCopy, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	else
		AheTraceErrorMessage( _T("AheUnion") );

	return ipRetGeom;
}

// ポリライン Union を取る
IGeometryPtr AheUnionNetwork( IPolylinePtr ipBaseGeom, IPolylinePtr ipAddGeom, ISpatialReferencePtr ipSpRef )
{
	IPointCollectionPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IPointCollectionPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;
	
	// 端点同士の接触判定
	LONG lBasePointCount = -1, lAddPointCount = -1;
	ipBaseGeomCopy->get_PointCount( &lBasePointCount );
	ipAddGeomCopy->get_PointCount( &lAddPointCount );

	IPointPtr ipBaseFromPoint, ipBaseToPoint, ipAddFromPoint, ipAddToPoint;
	ipBaseGeomCopy->get_Point( 0, &ipBaseFromPoint );
	ipBaseGeomCopy->get_Point( lBasePointCount - 1, &ipBaseToPoint );
	ipAddGeomCopy->get_Point( 0, &ipAddFromPoint );
	ipAddGeomCopy->get_Point( lAddPointCount - 1, &ipAddToPoint );

	BOOL bBaseFromIsAddFrom = AheIsEqual( ipBaseFromPoint, ipAddFromPoint, ipSpRef );
	BOOL bBaseFromIsAddTo   = AheIsEqual( ipBaseFromPoint, ipAddToPoint,   ipSpRef );
	BOOL bBaseToIsAddFrom   = AheIsEqual( ipBaseToPoint,   ipAddFromPoint, ipSpRef );
	BOOL bBaseToIsAddTo     = AheIsEqual( ipBaseToPoint,   ipAddToPoint,   ipSpRef );

	// 端点同士が接していなければNULLを返す
	if( bBaseFromIsAddFrom == FALSE && bBaseFromIsAddTo == FALSE &&
		bBaseToIsAddFrom   == FALSE && bBaseToIsAddTo   == FALSE )
		return NULL;

	// 返すポリラインの点列作成
	// その場合、BASEの方向を遵守する
	WKSPoint* pPoints = new WKSPoint[lBasePointCount + lAddPointCount - 1];
	if( bBaseToIsAddFrom == TRUE || bBaseToIsAddTo == TRUE )
	{
		ipBaseGeomCopy->QueryWKSPoints( 0, lBasePointCount, pPoints );
		if( bBaseToIsAddTo == TRUE )
			((IPolylinePtr)ipAddGeomCopy)->ReverseOrientation();
		ipAddGeomCopy->QueryWKSPoints( 0, lAddPointCount, &(pPoints[lBasePointCount - 1]) );
	}
	else {
		if( bBaseFromIsAddFrom == TRUE )
			((IPolylinePtr)ipAddGeomCopy)->ReverseOrientation();
		ipAddGeomCopy->QueryWKSPoints( 0, lAddPointCount, pPoints );
		ipBaseGeomCopy->QueryWKSPoints( 0, lBasePointCount, &(pPoints[lAddPointCount - 1]) );
	}

	// 返すポリライン作成
	IPointCollectionPtr ipNewPolyline( CLSID_Polyline );
	((IGeometryPtr)ipNewPolyline)->putref_SpatialReference( ipSpRef ); // 空間参照付与
	ipNewPolyline->SetWKSPoints( lBasePointCount + lAddPointCount - 1, pPoints );
	((IGeometryPtr)ipNewPolyline)->SnapToSpatialReference(); // TODO: やらないほうがいいんじゃない？
	((IPolylinePtr)ipNewPolyline)->SimplifyNetwork();

#ifdef DEBUG
	LONG lNewPointCount = -1;
	ipNewPolyline->get_PointCount( &lNewPointCount );
	ATLASSERT( lNewPointCount == ( lBasePointCount + lAddPointCount - 1 ) );
#endif

	delete [] pPoints;

	return ipNewPolyline;
}

// ジオメトリ同士を合体する（マルチレンダラ用）
IGeometryPtr AheUnionSegments( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;
	
	if( ipSpRef != NULL )
	{
		// 地理参照あわせ
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipAddGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
	ITopologicalOperatorPtr ipTopo2( ipAddGeomCopy );
	if( ipTopo1 == NULL || ipTopo2 == NULL ) return NULL;

	ipTopo1->Simplify();
	ipTopo2->Simplify();

	// 元ジオメトリ・追加ジオメトリをそれぞれISegmentCollectionにして合体させる
	IPolylinePtr			ipBaseSegPolyline(ipBaseGeomCopy);
	IPolylinePtr			ipAddSegPolyline(ipAddGeomCopy);
	ISegmentCollectionPtr	ipBaseSegCol(ipBaseSegPolyline);
	ISegmentCollectionPtr	ipAddSegCol(ipAddSegPolyline);
	IPolylinePtr			ipBasePolyline2;
	IGeometryPtr			ipRetGeom;
	if (SUCCEEDED(ipBaseSegCol->AddSegmentCollection(ipAddSegCol))) {
		ipBasePolyline2 = ipBaseSegCol;
		ipRetGeom = ipBasePolyline2;
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	}
/*
	if( SUCCEEDED( ipTopo1->Union( ipAddGeomCopy, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	else
		AheTraceErrorMessage( _T("AheUnion") );
*/
	return ipRetGeom;
}

// 
IGeometryPtr AheDifference( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;

	if( ipSpRef != NULL )
	{
		// 地理参照あわせ
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipAddGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
	ITopologicalOperatorPtr ipTopo2( ipAddGeomCopy );
	if( ipTopo1 == NULL || ipTopo2 == NULL ) return NULL;

	ipTopo1->Simplify();
	ipTopo2->Simplify();

	IGeometryPtr ipRetGeom;
	if( SUCCEEDED( ipTopo1->Difference( ipAddGeomCopy, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	else
		AheTraceErrorMessage( _T("AheDiffrence") );

	return ipRetGeom;
}

// ジオメトリの Intersect を取る
IGeometryPtr AheIntersect( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, esriGeometryDimension enumDimension/* = esriGeometry1Dimension*/, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipTestGeomCopy( AheGetClone( ipTestGeom ) );

	if( ipBaseGeomCopy == NULL || ipTestGeomCopy == NULL ) return NULL;

	if( ipSpRef != NULL )
	{
		// 地理参照あわせ
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipTestGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
//	ITopologicalOperatorPtr ipTopo2( ipTestGeomCopy );
	if( ipTopo1 == NULL/* || ipTopo2 == NULL*/ ) 
	{
		ATLASSERT( ipTopo1 != NULL );
		return NULL;
	}

/*	ipTopo1->Simplify();
	ipTopo2->Simplify();*/

	// 判定
	IGeometryPtr ipRetGeom;

/*	if( SUCCEEDED( ipTopo1->Intersect( ipTestGeomCopy, enumDimension, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();*/
/*#ifdef DEBUG
	AheTraceGeometry( ipBaseGeomCopy, _T("AheIntersect : ipBaseGeometry") );
	AheTraceGeometry( ipTestGeomCopy, _T("AheIntersect : ipTestGeometry") );
#endif*/
	if( FAILED( ipTopo1->Intersect( ipTestGeomCopy, enumDimension, &ipRetGeom ) ) )
	{
		ATLASSERT( FALSE );
		AheTraceErrorMessage( _T("AheIntersect") );
		return NULL;
	}

	return ipRetGeom;
}

/////////////////////////////////////////////////////////////////////////////
// 複合演算関数
/////////////////////////////////////////////////////////////////////////////
// 交差あるいは接触しているポイントを取得する
IGeometryPtr AheGetCrossOrTouchPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode/* = 2*/, BOOL* bIsDisjoint/* = NULL*/ )
{
#ifdef DEBUG
	DWORD DWSTART = GetTickCount();
#endif
	if( ipBaseGeometry == NULL || ipTestGeometry == NULL ) return NULL;

	if( bIsDisjoint ) *bIsDisjoint = TRUE;
/*	AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipBaseGeometry, RGB( 255, 255, 0 ), 2, esriSLSSolid, TRUE, FALSE );
	AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipTestGeometry, RGB( 0, 255, 255 ), 2, esriSLSSolid, TRUE, FALSE );
	return NULL;*/
	//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipBaseGeometry, RGB(255,255,0), 8, esriSMSCircle );
	//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipTestGeometry, RGB(255,0,255), 8, esriSMSCircle );
	// iMode : 0=Cross 1=Touch 2=Both
	sindyeSpatialRel enumSpRel;
	switch( iMode )
	{
		case 0:	 enumSpRel = sindyeSpatialRelCross; break;												// Cross
		case 1:  enumSpRel = sindyeSpatialRelTouch; break;												// Touch
		case 2:  enumSpRel = (sindyeSpatialRel)(sindyeSpatialRelCross|sindyeSpatialRelTouch); break;	// Cross and Touch
		default: enumSpRel = sindyeSpatialRelNone;	break;
	}

	// 両方の空間参照の細かいほうからしきい値を割り出す
	ISpatialReferencePtr ipBaseSpRef, ipTestSpRef, ipMinimumSpRef;
	ipBaseGeometry->get_SpatialReference( &ipBaseSpRef );
	ipTestGeometry->get_SpatialReference( &ipTestSpRef );
	ipMinimumSpRef = AheGetMinimumSpatialReference( ipBaseSpRef, ipTestSpRef );
	DOUBLE dTorelance = AheGetXYDomain1UnitLength( ipMinimumSpRef );
	DOUBLE dTorelanceMin = -dTorelance, dTorelanceMax = 1.0 + dTorelance, dTorelanceMinMax = dTorelance, dTorelanceMaxMin = 1.0 - dTorelance;

	// 一度ジオメトリコレクションに落とす（）
	IGeometryCollectionPtr ipBaseGeomColCopy( AheGetClone( ipBaseGeometry ) );
	IGeometryCollectionPtr ipTestGeomColCopy( AheGetClone( ipTestGeometry ) );

	if( ipBaseGeomColCopy == NULL || ipTestGeomColCopy == NULL )
	{
		_ASSERTE( ipBaseGeomColCopy != NULL && ipTestGeomColCopy != NULL );
		return NULL;
	}

	// 交差ポイントを探す
	IPointCollectionPtr ipIntersectPoints( CLSID_Multipoint );	// 返値
	((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // ベースの空間参照（でいいの？）を付与

	LONG lBaseGeomCount = -1, lTestGeomCount = -1;
	IPointPtr ipBaseFromPoint( CLSID_Point ), ipBaseToPoint( CLSID_Point ), ipTestFromPoint( CLSID_Point ), ipTestToPoint( CLSID_Point );
	// 作ったポイントは空間参照をあわせておく
	ipBaseFromPoint->putref_SpatialReference( ipBaseSpRef );
	ipBaseToPoint->putref_SpatialReference( ipBaseSpRef );
	ipTestFromPoint->putref_SpatialReference( ipTestSpRef );
	ipTestToPoint->putref_SpatialReference( ipTestSpRef );

	// 使ってないので
	//IPolylinePtr ipBaseSegPolyline( CLSID_Polyline ), ipTestSegPolyline( CLSID_Polyline );
	//// 作ったラインは空間参照をあわせておく
	//ipBaseSegPolyline->putref_SpatialReference( ipBaseSpRef );
	//ipTestSegPolyline->putref_SpatialReference( ipTestSpRef );

	ipBaseGeomColCopy->get_GeometryCount( &lBaseGeomCount );
	ipTestGeomColCopy->get_GeometryCount( &lTestGeomCount );

	// Base のジオメトリごとのループ
	for( LONG lBaseGeomCounter = 0; lBaseGeomCounter != lBaseGeomCount; lBaseGeomCounter++ )
	{
		IGeometryPtr ipBaseGeomCopy;
		ipBaseGeomColCopy->get_Geometry( lBaseGeomCounter, &ipBaseGeomCopy );
		if( ipBaseGeomCopy == NULL )
		{
			_ASSERTE( ipBaseGeomCopy != NULL );
			continue;
		}
		//ISegmentCollectionPtr ipBaseSegColCopy( AheGetClone( ipBaseGeometry ) ); <- ipBaseGeometry だと全部でしょ？
		ISegmentCollectionPtr ipBaseSegColCopy( AheGetClone( ipBaseGeomCopy ) );
		if( ipBaseSegColCopy == NULL )
		{
			_ASSERTE( ipBaseSegColCopy != NULL );
			continue;
		}
		LONG lBaseSegColCount = -1;
		ipBaseSegColCopy->get_SegmentCount( &lBaseSegColCount );

		// Base のセグメントごとのループ
		for( LONG i = 0; i < lBaseSegColCount; i++ )
		{
			_ISegmentPtr ipBaseSegTmp, ipBaseSeg;
			DOUBLE dBaseFromX = 0.0f, dBaseFromY = 0.0f, dBaseToX = 0.0f, dBaseToY = 0.0f;

			ipBaseSegColCopy->get_Segment( i, &ipBaseSegTmp );
			ipBaseSeg = AheGetClone( (_ISegmentPtr)ipBaseSegTmp );
			if( ipBaseSeg == NULL ) continue;

			// ポイント初期化
			ipBaseFromPoint->PutCoords( 0.0f, 0.0f );
			ipBaseToPoint->PutCoords( 0.0f, 0.0f );

			if( FAILED( ipBaseSeg->QueryFromPoint( ipBaseFromPoint ) ) || FAILED( ipBaseSeg->QueryToPoint( ipBaseToPoint ) ) )
			{
				ATLASSERT( FALSE );
				AheTraceErrorMessage( _T("CEditHelper::GetCrossOrTouchPoints2()") );
				continue;
			}
			ipBaseFromPoint->QueryCoords( &dBaseFromX, &dBaseFromY );
			ipBaseToPoint->QueryCoords( &dBaseToX, &dBaseToY );

/*#ifdef DEBUG
			// セグメントハイライト消す
			if( ipBaseSegPolyline != NULL )
				AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipBaseSegPolyline, RGB( 0, 255, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
			// 使ってないので
			//// セグメント -> ポリライン変換
			//ipBaseSegPolyline->SetEmpty();
			//((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
			//((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipBaseToPoint ) );
/*#ifdef DEBUG
			// セグメントをハイライトしてみる
			AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipBaseSegPolyline, RGB( 0, 255, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
			DOUBLE Ax = (DOUBLE)( dBaseToX - dBaseFromX );
			DOUBLE Ay = (DOUBLE)( dBaseToY - dBaseFromY );

			// Test のジオメトリごとのループ
			for( LONG lTestGeomCounter = 0; lTestGeomCounter != lTestGeomCount; lTestGeomCounter++ )
			{
				IGeometryPtr ipTestGeomCopy;
				ipTestGeomColCopy->get_Geometry( lTestGeomCounter, &ipTestGeomCopy );
				if( ipTestGeomColCopy == NULL )
				{
					_ASSERTE( ipTestGeomCopy != NULL );
					continue;
				}
				//ISegmentCollectionPtr ipTestSegColCopy( AheGetClone( ipTestGeometry ) );	<- ipTestGeometry だと全部でしょ？
				ISegmentCollectionPtr ipTestSegColCopy( AheGetClone( ipTestGeomCopy ) );
				if( ipTestSegColCopy == NULL )
				{
					_ASSERTE( ipTestSegColCopy != NULL );
					continue;
				}
				LONG lTestSegColCount = -1;
				ipTestSegColCopy->get_SegmentCount( &lTestSegColCount );

				// Test のセグメントごとのループ
				for( LONG j = 0; j < lTestSegColCount; j++ )
				{
					_ISegmentPtr ipTestSegTmp, ipTestSeg;
					DOUBLE dTestFromX = 0.0f, dTestFromY = 0.0f, dTestToX = 0.0f, dTestToY = 0.0f;

					// コピーを取る
					ipTestSegColCopy->get_Segment( j, &ipTestSegTmp );
					ipTestSeg = AheGetClone( (_ISegmentPtr)ipTestSegTmp );
					if( ipTestSeg == NULL ) continue;

					// ポイント初期化
					ipTestFromPoint->PutCoords( 0.0f, 0.0f );
					ipTestToPoint->PutCoords( 0.0f, 0.0f );

					if( FAILED( ipTestSeg->QueryFromPoint( ipTestFromPoint ) ) || FAILED( ipTestSeg->QueryToPoint( ipTestToPoint ) ) )
					{
						ATLASSERT( FALSE );
						AheTraceErrorMessage( _T("CEditHelper::GetCrossOrTouchPoints2()") );
						continue;
					}
					ipTestFromPoint->QueryCoords( &dTestFromX, &dTestFromY );
					ipTestToPoint->QueryCoords( &dTestToX, &dTestToY );

/*#ifdef DEBUG
					// セグメントをハイライト消す
					if( ipTestSegPolyline != NULL )
						AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipTestSegPolyline, RGB( 255, 0, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
					// 使ってないので
					//ipTestSegPolyline->SetEmpty();
					//((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
					//((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestToPoint ) );
/*#ifdef DEBUG
					// セグメントをハイライトしてみる
					AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipTestSegPolyline, RGB( 255, 0, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
					// セグメント単位で平行、接触、又は完全に離れているかどうかのチェック
					// Base を Test の FromTo で、Test を Base の FromTo でスプリットしてみて、できたら平行、接触ではない又は離れていない
//					if( ipBaseSegPolyline != NULL && ipTestSegPolyline != NULL )
//					{
						// 端点同士が接しているかどうか
						BOOL bBaseFromIsTestFrom = AheIsEqual( ipBaseFromPoint, ipTestFromPoint );
						BOOL bBaseFromIsTestTo   = AheIsEqual( ipBaseFromPoint, ipTestToPoint );
						BOOL bBaseToIsTestFrom   = AheIsEqual( ipBaseToPoint, ipTestFromPoint );
						BOOL bBaseToIsTestTo     = AheIsEqual( ipBaseToPoint, ipTestToPoint );

						// 完全一致かどうか（完全一致の場合は Touch とみなす）
						if( bBaseFromIsTestFrom == TRUE && bBaseFromIsTestTo == TRUE && bBaseToIsTestFrom == TRUE && bBaseToIsTestTo == TRUE )
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							if( enumSpRel & sindyeSpatialRelTouch ) 
							{
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）一致：                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）一致：                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
							continue;	// 完全一致ならここで終わり
						}

						// 端点がセグメント上に存在しているかどうか
						VARIANT_BOOL vbBaseIsSplittedByTestFrom = ( AhePointIsOnSegment( ipBaseSeg, ipTestFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
						VARIANT_BOOL vbBaseIsSplittedByTestTo   = ( AhePointIsOnSegment( ipBaseSeg, ipTestToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
						VARIANT_BOOL vbTestIsSplittedByBaseFrom = ( AhePointIsOnSegment( ipTestSeg, ipBaseFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
						VARIANT_BOOL vbTestIsSplittedByBaseTo   = ( AhePointIsOnSegment( ipTestSeg, ipBaseToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

						// 平行かどうか（平行の場合は Touch とみなす）
						if( ( bBaseFromIsTestFrom == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
							( bBaseFromIsTestTo   == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
							( bBaseToIsTestFrom   == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
							( bBaseToIsTestTo     == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
							( bBaseFromIsTestFrom == FALSE && bBaseFromIsTestTo == FALSE && bBaseToIsTestFrom == FALSE && bBaseToIsTestTo == FALSE && 
							  ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE && vbBaseIsSplittedByTestTo == VARIANT_TRUE ) ||
							    ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE && vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ||
							    ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbBaseIsSplittedByTestTo == VARIANT_TRUE ) &&
							      ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ) ) ) )
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							if( enumSpRel & sindyeSpatialRelTouch ) 
							{
								// 平行の場合
								if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipTestFromPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）平行：BaseIsSplittedByTestFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestFromX, dTestFromY );
								}
								if( vbBaseIsSplittedByTestTo == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipTestToPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）平行：BaseIsSplittedByTestTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestToX, dTestToY );
								}
								if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipBaseFromPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）平行：TestIsSplittedByBaseFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
								if( vbTestIsSplittedByBaseTo == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipBaseFromPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）平行：TestIsSplittedByBaseTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;	// 平行ならここで終わり
						}
						
						// 一個前及び一個先の Base、Test のセグメントと From、To を取得
						_ISegmentPtr ipPrevBaseSeg, ipNextBaseSeg, ipPrevTestSeg, ipNextTestSeg;
						IPointPtr ipPrevBaseSegFromPoint, ipPrevBaseSegToPoint, ipNextBaseSegFromPoint, ipNextBaseSegToPoint;
						IPointPtr ipPrevTestSegFromPoint, ipPrevTestSegToPoint, ipNextTestSegFromPoint, ipNextTestSegToPoint;
						if( i > 0 )
						{ 
							ipBaseSegColCopy->get_Segment( i - 1, &ipPrevBaseSeg );
							if( ipPrevBaseSeg != NULL )
							{
								ipPrevBaseSeg->get_FromPoint( &ipPrevBaseSegFromPoint );
								ipPrevBaseSeg->get_ToPoint( &ipPrevBaseSegToPoint );
							}
						}
						if( i < lBaseSegColCount - 1 )
						{
							ipBaseSegColCopy->get_Segment( i + 1, &ipNextBaseSeg );
							if( ipNextBaseSeg != NULL )
							{
								ipNextBaseSeg->get_FromPoint( &ipNextBaseSegFromPoint );
								ipNextBaseSeg->get_ToPoint( &ipNextBaseSegToPoint );
							}
						}
						if( j > 0 )
						{
							ipTestSegColCopy->get_Segment( j - 1, &ipPrevTestSeg );
							if( ipPrevTestSeg != NULL )
							{
								ipPrevTestSeg->get_FromPoint( &ipPrevTestSegFromPoint );
								ipPrevTestSeg->get_ToPoint( &ipPrevTestSegToPoint );
							}
						}
						if( j < lTestSegColCount - 1 )
						{
							ipTestSegColCopy->get_Segment( j + 1, &ipNextTestSeg );
							if( ipNextTestSeg != NULL )
							{
								ipNextTestSeg->get_FromPoint( &ipNextTestSegFromPoint );
								ipNextTestSeg->get_ToPoint( &ipNextTestSegToPoint );
							}
						}

						// 端点同士が接しているが、一致でも平行でもない場合
						if( bBaseFromIsTestFrom == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base の From が Test の From と一致しているとき
							//
							// 1. Base の To           ... ipBaseToPoint
							// 2. Base の From         ... ipBaseFromPoint
							// 3. Base の一個前の From  ... ipPrevBaseSegFromPoint
							// 4. Test の To           ... ipTestToPoint
							// 5. Test の一個前の From  ... ipPrevTestSegFromPoint
							//
							// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
							if( ipPrevBaseSeg != NULL && ipPrevTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
								else
								{
									// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
							}
							// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
							}
							continue;
						}
						if( bBaseFromIsTestTo == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base の From が Test の To と一致しているとき
							//
							// 1. Base の To           ... ipBaseToPoint
							// 2. Base の From         ... ipBaseFromPoint
							// 3. Base の一個前の From  ... ipPrevBaseSegFromPoint
							// 4. Test の From         ... ipTestFromPoint
							// 5. Test の一個先の To    ... ipNextTestSegToPoint
							//
							// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
							if( ipPrevBaseSeg != NULL && ipNextTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestFromPoint        ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipNextTestSegToPoint   ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								} 
								else {
									// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
							}
							// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
							}
							continue;
						}
						if( bBaseToIsTestFrom == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base の To が Test の From と一致しているとき
							//
							// 1. Base の From         ... ipBaseFromPoint
							// 2. Base の To           ... ipBaseToPoint
							// 3. Base の一個先の To    ... ipNextBaseSegToPoint
							// 4. Test の To           ... ipTestToPoint
							// 5. Test の一個前の From  ... ipPrevTestSegFromPoint
							//
							// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
							if( ipNextBaseSeg != NULL && ipPrevTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
								else {
									// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
							}
							// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;
						}
						if( bBaseToIsTestTo == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base の To が Test の To と一致しているとき
							//
							// 1. Base の From         ... ipBaseFromPoint
							// 2. Base の To           ... ipBaseToPoint
							// 3. Base の一個先の To    ... ipNextBaseSegToPoint
							// 4. Test の From         ... ipTestFromPoint
							// 5. Test の一個先の To    ... ipNextTestSegToPoint
							//
							// 123 のなす角の間に 124、125 の角度が両方あるまたは両方ない場合は Touch、片方があってもう片方がない場合は Cross
							if( ipNextBaseSeg != NULL && ipNextTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestFromPoint      ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextTestSegToPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								} else {
									// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
							}
							// Base 又は Test が開始又は最後のセグメントを指していた場合は Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端端：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;
						}

						// 一致でも平行でもなく端点も接していない場合で、片方ののセグメント上にもう片方の From 又は To が存在するとき
						if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base が Test の From でスプリットされたとき
							//
							// 1. Base の From か To （どちらでも良い）... ipBaseFromPoint
							// 2. Test の From                        ... ipTestFromPoint
							// 3. Test の To                          ... ipTestToPoint
							// 4. Test の一個前の From                ... ipPrevTestSegFromPoint
							//
							// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
							// 一個前の Test を取得
							if( ipPrevTestSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipTestToPoint );
								DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipPrevTestSegFromPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
									}
								}
								else
								{
									// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
									}
								}
							}
							// Test が開始セグメントなので、 Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
								}
							}
							continue;
						}
						if( vbBaseIsSplittedByTestTo   == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base が Test の To でスプリットされたとき
							//
							// 1. Base の From か To （どちらでも良い）... ipBaseFromPoint
							// 2. Test の To                          ... ipTestToPoint
							// 3. Test の From                        ... ipTestFromPoint
							// 4. Test の一個先の To                  ... ipNextTestSegToPoint
							//
							// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
							if( ipNextTestSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipTestFromPoint );
								DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipNextTestSegToPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
									}
								} 
								else {
									// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
									}
								}
							}
							// Test の最後のセグメントの場合は Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
								}
							}
							continue;
						}
						if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Test が Base の From でスプリットされたとき
							//
							// 1. Test の From か To （どちらでも良い）... ipTestFromPoint
							// 2. Base の From                        ... ipBaseFromPoint
							// 3. Base の To                          ... ipBaseToPoint
							// 4. Base の一個前の From                ... ipPrevBaseSegFromPoint
							//
							// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
							// 一個前の Base を取得
							if( ipPrevBaseSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipBaseToPoint );
								DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
								else {
									// クロスしなかった場合は Touch とみなす（前のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
							}
							// j が 0 の場合は開始セグメントなので、 Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
							}
							continue;
						}
						if( vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Test が Base の To でスプリットされたとき
							//
							// 1. Test の From か To （どちらでも良い）... ipTestFromPoint
							// 2. Base の To                          ... ipBaseToPoint
							// 3. Base の From                        ... ipBaseFromPoint
							// 4. Base の一個先の To                  ... ipNextBaseSegToPoint
							//
							// 123 のなす角と 124 のなす角を取得し、どちらも正又は負の角度であれば Touch、どちらかが正でどちらかが負であれば Cross 
							// 一つ先の Base を取得
							if( ipNextBaseSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipBaseFromPoint );
								DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipNextBaseSegToPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// クロスした場合
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								} else {
									// クロスしなかった場合は Touch とみなす（先のセグメントが接していた場合も Touch となる）
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
							}
							// i が Base の最後のセグメントを指していた場合は Touch とする（問答無用）
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）端接：X=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;
						}
//					}

					DOUBLE Bx = (DOUBLE)( dTestToX - dTestFromX );
					DOUBLE By = (DOUBLE)( dTestToY - dTestFromY );
					DOUBLE DivideBase = (DOUBLE)( (DOUBLE)( Bx * Ay ) - (DOUBLE)( By * Ax ) );
					if( DivideBase == 0.0 ) 
					{
						ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 平行なので飛ばします。") );
						continue;	// 平行
					}

					DOUBLE t = (DOUBLE)( ( (DOUBLE)(Ax * (DOUBLE)( dTestFromY - dBaseFromY )) - (DOUBLE)(Ay * (DOUBLE)( dTestFromX - dBaseFromX )) ) / DivideBase );
//					if( dTorelanceMin <= t && t <= dTorelanceMax )
					if( 0.0 < t && t < 1.0 )
					{
						//DOUBLE s  = (DOUBLE)( (DOUBLE)(( dTestFromX + (DOUBLE)( t * Bx ) - dBaseFromX )) / Ax );	<- Base が垂直だと Ax が 0 になってとんでもないことに
						DOUBLE s = (DOUBLE)( ( (DOUBLE)(Bx * (DOUBLE)( dBaseFromY - dTestFromY )) - (DOUBLE)(By * (DOUBLE)( dBaseFromX - dTestFromX )) ) / (-DivideBase) );
						if( 0.0 < s && s < 1.0 )
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							BOOL bAdd = FALSE;
							DOUBLE Px = (DOUBLE)( dBaseFromX + (DOUBLE)(s * Ax) );
							DOUBLE Py = (DOUBLE)( dBaseFromY + (DOUBLE)(s * Ay) );
/*							if( ( enumSpRel & sindyeSpatialRelTouch ) && ( ( dTorelanceMin <= t && t <= dTorelanceMinMax ) || ( dTorelanceMaxMin <= t && t <= dTorelanceMax ) ) )
							{
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（TOUCH）発見：X=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
								bAdd = TRUE;
							} else if( enumSpRel & sindyeSpatialRelCross ) {
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）発見：X=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
								bAdd = TRUE;
							}*/
//							if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) {
							if( enumSpRel & sindyeSpatialRelCross ) {
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : 交点（CROSS）発見：X=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
								bAdd = TRUE;
							}
							if( bAdd == TRUE )
							{
								IPointPtr ipPoint( CLSID_Point );
								((IGeometryPtr)ipPoint)->putref_SpatialReference( ipBaseSpRef ); // ベースの空間参照（でいいの？）を付与
								ipPoint->PutCoords( Px, Py );
/*#ifdef DEBUG
								// インターセクト個所ハイライト
								AheDrawPoint( m_cArcHelper.GetScreenDisplay(FALSE), ipPoint, FALSE, 8, 100, 0, 50, esriSMSCircle, FALSE, TRUE );
								ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : インターセクト個所ハイライト（レンガ色）\n") );
								AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipPoint, RGB(100,0,50), 8, esriSMSCircle );
#endif*/
#ifdef DEBUG
/*								// ほんとにあるの？
								if( AheTestGeometryRelationship( ipSegPolyline,  ipPoint, (sindyeSpatialRel)(sindyeSpatialRelContain|sindyeSpatialRelTouch) ) == 0 ||
									AheTestGeometryRelationship( ipSegPolyline2, ipPoint, (sindyeSpatialRel)(sindyeSpatialRelContain|sindyeSpatialRelTouch) ) == 0 )
								{
									AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipSegPolyline, RGB( 10, 20, 255 ), 2, esriSLSSolid, TRUE, FALSE );
									AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipSegPolyline2, RGB( 10, 20, 0 ), 2, esriSLSSolid, TRUE, FALSE );
								}*/
/*								AheTraceGeometry( ipTestSegPolyline, _T("比較したライン（Base）") );
								AheTraceGeometry( ipBaseSegPolyline, _T("比較したライン（Test）") );
								AheTraceGeometry( ipPoint, _T("得られたポイント") );*/
/*								AheAddLineElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipSegPolyline, RGB(100,j + i * lTestSegColCount,50), 2, esriSLSSolid );
								AheAddLineElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipSegPolyline2, RGB(100,j + i * lTestSegColCount,50), 2, esriSLSSolid );
								AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipPoint, RGB(100,j + i * lTestSegColCount,50), 8, esriSMSCircle );*/
#endif
								ipIntersectPoints->AddPoint( ipPoint );
							}
						}
					}
				}
			}
		}
	}

	// 一つもなければ NULL を返す
	if( ipIntersectPoints != NULL )
	{
		LONG lIntersectCount = -1;
		ipIntersectPoints->get_PointCount( &lIntersectCount );
		if( lIntersectCount < 1 )
			ipIntersectPoints = NULL;
	}

/*#ifdef DEBUG
	AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipIntersectPoints, RGB(0,0,255), 8, esriSMSCircle );
#endif*/
#ifdef DEBUG
	ATLTRACE(_T("AheGetCrossOrTouchPoints() : 処理時間 %.3lf秒\n"), (GetTickCount() - DWSTART)/1000.0 );
#endif

	return ipIntersectPoints;
}

// ポイントがポリライン及びポリゴンフィーチャの From ポイントであるかどうかの判定
BOOL AheIsFromPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPointGeom && ipCurveFeature ) )
		return FALSE;

	IGeometryPtr ipGeom;
	ipCurveFeature->get_Shape( &ipGeom );

	return AheIsFromPoint( (IPointPtr)ipPointGeom, (ICurvePtr)ipGeom, ipSpRef );
}

// ポイントがポリライン及びポリゴンの From ポイントであるかどうかの判定
BOOL AheIsFromPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPoint && ipCurve ) )
		return FALSE;

	// From ポイント取得
	IPointPtr ipFromPoint;
	ipCurve->get_FromPoint( &ipFromPoint );

	return AheIsDBEqual( ipPoint, ipFromPoint );
}

// ポイントがポリライン及びポリゴンフィーチャの To ポイントであるかどうかの判定
BOOL AheIsToPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPointGeom && ipCurveFeature ) )
		return FALSE;

	IGeometryPtr ipGeom;
	ipCurveFeature->get_Shape( &ipGeom );

	return AheIsToPoint( (IPointPtr)ipPointGeom, (ICurvePtr)ipGeom, ipSpRef );
}

// ポイントがポリライン及びポリゴンの To ポイントであるかどうかの判定
BOOL AheIsToPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPoint && ipCurve ) )
		return FALSE;

	// To ポイント取得
	IPointPtr ipToPoint;
	ipCurve->get_ToPoint( &ipToPoint );

	return AheIsDBEqual( ipPoint, ipToPoint );
}

// ポイントがポリライン及びポリゴンフィーチャの端点であるかどうかの判定
BOOL AheIsEndPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPointGeom && ipCurveFeature ) )
		return FALSE;

	IGeometryPtr ipGeom;
	ipCurveFeature->get_Shape( &ipGeom );

	return AheIsEndPoint( (IPointPtr)ipPointGeom, (ICurvePtr)ipGeom, ipSpRef );
}

// ポイントがポリライン及びポリゴンの端点であるかどうかの判定
BOOL AheIsEndPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPoint && ipCurve ) )
		return FALSE;

#if 0
	return  AheIsFromPoint( ipPoint, ipCurve, ipSpRef ) || 
		AheIsToPoint( ipPoint, ipCurve, ipSpRef );
#else
	// get_FromPointなどすると、9.3.1では形状変更されたことになるようだ（bug 7860）
	// しょうがないので、HitTestを使うことで回避する
	IHitTestPtr ipHitTest( ipCurve );
	IPointPtr ipHitPoint( AheCreateClone(ipPoint) );
	double dLen = AheGetXYDomain1UnitLength(AheGetSpatialReference(ipCurve)) * 10.0, dDistance = 0.0;
	long lHitPart = 0, lHitSegment = 0;
	VARIANT_BOOL vbRightSide = VARIANT_FALSE, vbHit = VARIANT_FALSE;
	if( FAILED( ipHitTest->HitTest( ipPoint, dLen, esriGeometryPartEndpoint, ipHitPoint, &dDistance, &lHitPart, &lHitSegment, &vbRightSide, &vbHit ) ) ||
		( ! vbHit ) )
		return FALSE;

	return AheIsDBEqual( ipPoint, ipHitPoint );
#endif // if 0
}

// ジオメトリの端点同士が接触しているかどうか
BOOL AheIsTouchEndPoint( ICurvePtr ipBaseCurve, ICurvePtr ipTestCurve, ISpatialReferencePtr ipSpRef/* = NULL*/, IPoint** ipTouchPoint/* = NULL*/ )
{
	if( ipBaseCurve == NULL || ipTestCurve == NULL ) return FALSE;

	// From ポイント取得
	IPointPtr ipFromPoint, ipToPoint;

	if( ipSpRef == NULL )
		ipBaseCurve->get_SpatialReference( &ipSpRef );

	ipTestCurve->get_FromPoint( &ipFromPoint );
	ipTestCurve->get_ToPoint( &ipToPoint );

	BOOL bIsTouchFromPoint = AheIsEndPoint( ipFromPoint, ipBaseCurve, ipSpRef );
	BOOL bIsTouchToPoint   = AheIsEndPoint( ipToPoint,   ipBaseCurve, ipSpRef );

	if( ipTouchPoint != NULL )
	{
		if( bIsTouchFromPoint == TRUE )
			(*ipTouchPoint) = ipFromPoint;
		else if( bIsTouchToPoint == TRUE )
			(*ipTouchPoint) = ipToPoint;

		if( (*ipTouchPoint) != NULL )
			(*ipTouchPoint)->AddRef();
	}

	if( bIsTouchFromPoint == TRUE || bIsTouchToPoint == TRUE )
		return TRUE;
	else
		return FALSE;
}

// ポイントがジオメトリ上の構成点かどうかの判定
BOOL AheIsVertexPoint( IPointPtr ipPoint, IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef/* = NULL*/, LONG* pIndex/* = NULL*/ )
{
	// TODO: この関数はHitTestで閾値0でいいんジャマイカ？
	if( ipPoint == NULL || ipGeom == NULL ) return FALSE;
	if( pIndex != NULL ) *pIndex = -1;

	ISpatialReferencePtr ipGeomSpRef( AheGetSpatialReference( ipGeom ) );
	IPointCollectionPtr ipPointCol( ipGeom );
	if( ipPointCol == NULL )
	{
		IPointPtr ipGeomPoint( ipGeom );
		if( ipGeomPoint == NULL ) return FALSE;

		ipPointCol.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipPointCol)->putref_SpatialReference( ipGeomSpRef ); // 空間参照を付与
		ipPointCol->AddPoint( ipGeomPoint );
	}

	IPointPtr ipGeomPoint( CLSID_Point );
	ipGeomPoint->putref_SpatialReference( ipGeomSpRef ); // 空間参照付与
	LONG lPointCount = -1;
	ipPointCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		ipPointCol->QueryPoint( i, ipGeomPoint );
		if( AheIsEqual( ipGeomPoint, ipPoint, ipSpRef ) == TRUE )
		{
			if( pIndex != NULL ) *pIndex = i;
			return TRUE;
		}
	}
	return FALSE;
}

// ジオメトリが自己交差しているかどうかの判定
BOOL AheIsOneselfCross( IGeometryPtr ipGeometry, BOOL bIsNetworkFeature/* = TRUE*/ )
{
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeometry ) );
	if( ipGeomCopy == NULL ) return FALSE;

	// 空間参照
	ISpatialReferencePtr ipSpRef;
	ipGeomCopy->get_SpatialReference( &ipSpRef );

	// ジオメトリ -> セグメント分解
	std::list<SEGMENTITEM> listSegments;
	AheGeometry2SegmentItems( ipGeomCopy, listSegments );

	for( std::list<SEGMENTITEM>::iterator itBase = listSegments.begin(); itBase != listSegments.end(); itBase++ )
	{
		std::list<SEGMENTITEM>::iterator itTest = itBase;
		itTest++;
		for( ; itTest != listSegments.end(); itTest++ )
		{
			// 端点同士の接触判定
			BOOL bBaseFromIsTestFrom = AheIsEqual( itBase->ipFromPoint, itTest->ipFromPoint );
			BOOL bBaseFromIsTestTo   = AheIsEqual( itBase->ipFromPoint, itTest->ipToPoint );
			BOOL bBaseToIsTestFrom   = AheIsEqual( itBase->ipToPoint, itTest->ipFromPoint );
			BOOL bBaseToIsTestTo     = AheIsEqual( itBase->ipToPoint, itTest->ipToPoint );

			// 別パートの場合は、絶対に接触してはいけない
			if( itBase->lPartID != itTest->lPartID )
			{
				if( bBaseFromIsTestFrom == TRUE || bBaseFromIsTestTo == TRUE || bBaseToIsTestFrom == TRUE || bBaseToIsTestTo == TRUE ) return TRUE;
			}

			// 完全一致かどうか
			if( ( bBaseFromIsTestFrom == TRUE && bBaseToIsTestTo == TRUE ) || ( bBaseFromIsTestTo == TRUE && bBaseToIsTestFrom == TRUE ) ) return TRUE;
			
			// 端点がセグメント上に存在しているかどうか
			BOOL bTestFromIsOnBase = AhePointIsOnSegment( itBase->ipSegment, itTest->ipFromPoint, ipSpRef );
			BOOL bTestToIsOnBase   = AhePointIsOnSegment( itBase->ipSegment, itTest->ipToPoint,   ipSpRef );
			BOOL bBaseFromIsOnTest = AhePointIsOnSegment( itTest->ipSegment, itBase->ipFromPoint, ipSpRef );
			BOOL bBaseToIsOnTest   = AhePointIsOnSegment( itTest->ipSegment, itBase->ipToPoint,   ipSpRef );

			// 別パートの場合は、絶対に存在してはいけない
			if( itBase->lPartID != itTest->lPartID )
			{
				if( bTestFromIsOnBase == TRUE || bTestToIsOnBase == TRUE || bBaseFromIsOnTest == TRUE || bBaseToIsOnTest == TRUE ) return TRUE;
			}

			// 平行かどうか（平行の場合は Touch とみなす）
			if( ( bBaseFromIsTestFrom == TRUE && ( bTestToIsOnBase   == TRUE || bBaseToIsOnTest   == TRUE ) ) ||
				( bBaseFromIsTestTo   == TRUE && ( bTestFromIsOnBase == TRUE || bBaseToIsOnTest   == TRUE ) ) ||
				( bBaseToIsTestFrom   == TRUE && ( bTestToIsOnBase   == TRUE || bBaseFromIsOnTest == TRUE ) ) ||
				( bBaseToIsTestTo     == TRUE && ( bTestFromIsOnBase == TRUE || bBaseFromIsOnTest == TRUE ) ) ||
				( bBaseFromIsTestFrom == FALSE && bBaseFromIsTestTo == FALSE && bBaseToIsTestFrom == FALSE && bBaseToIsTestTo == FALSE && 
				  ( ( bTestFromIsOnBase == VARIANT_TRUE && bTestToIsOnBase == VARIANT_TRUE ) ||
					( bBaseFromIsOnTest == VARIANT_TRUE && bBaseToIsOnTest == VARIANT_TRUE ) ||
					( ( bTestFromIsOnBase == VARIANT_TRUE || bTestToIsOnBase == VARIANT_TRUE ) &&
					  ( bBaseFromIsOnTest == VARIANT_TRUE || bBaseToIsOnTest == VARIANT_TRUE ) ) ) ) ) return TRUE;

			// 隣同士のセグメントの場合はここで終わり
			if( itBase->lPartID == itTest->lPartID && 
				( itBase->lSegmentID == itTest->lSegmentID + 1 || itBase->lSegmentID == itTest->lSegmentID - 1 ) ) continue;

			// 一個前及び一個先を取得
			std::list<SEGMENTITEM>::iterator itBasePrev = itBase, itBaseNext = itBase, itTestPrev = itTest, itTestNext = itTest;
			if( itBase->bIsLastSegment  == TRUE ) itBaseNext = listSegments.end(); else itBaseNext++;
			if( itBase->bIsFirstSegment == TRUE ) itBasePrev = listSegments.end(); else itBasePrev--;
			if( itTest->bIsLastSegment  == TRUE ) itTestNext = listSegments.end(); else itTestNext++;
			if( itTest->bIsFirstSegment == TRUE ) itTestPrev = listSegments.end(); else itTestPrev--;

			// 道路系の場合のみ
			if( bIsNetworkFeature == TRUE )
			{
				// 端点同士が接しているが、一致でも平行でもない場合
				if( bBaseFromIsTestFrom == TRUE )
				{
					if( itBasePrev != listSegments.end() && itTestPrev != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itBasePrev->ipFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTest->ipToPoint       ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTestPrev->ipFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseFromIsTestTo == TRUE )
				{
					if( itBasePrev != listSegments.end() && itTestNext != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itBasePrev->ipFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTest->ipFromPoint     ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTestNext->ipToPoint   ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseToIsTestFrom == TRUE )
				{
					if( itBaseNext != listSegments.end() && itTestPrev != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itBaseNext->ipToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTest->ipToPoint       ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTestPrev->ipFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseToIsTestTo == TRUE )
				{
					if( itBaseNext != listSegments.end() && itTestNext != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itBaseNext->ipToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTest->ipFromPoint       ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTestNext->ipToPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}

				// 一箇所でも端点同士が接している場合はここで終わり
				if( bBaseFromIsTestFrom == TRUE || bBaseFromIsTestTo == TRUE || bBaseToIsTestFrom == TRUE || bBaseToIsTestTo == TRUE ) continue;

				// 一致でも平行でもなく端点も接していない場合で、片方ののセグメント上にもう片方の From 又は To が存在するとき
				if( bTestFromIsOnBase == TRUE )
				{
					if( itTestPrev != listSegments.end() )
					{
						DOUBLE dAngle1 = AheGetAngle( itBase->ipFromPoint, itTest->ipFromPoint, itTest->ipToPoint );
						DOUBLE dAngle2 = AheGetAngle( itBase->ipFromPoint, itTest->ipFromPoint, itTestPrev->ipFromPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bTestToIsOnBase == TRUE )
				{
					if( itTestNext != listSegments.end() )
					{
						DOUBLE dAngle1 = AheGetAngle( itBase->ipFromPoint, itTest->ipToPoint, itTest->ipFromPoint );
						DOUBLE dAngle2 = AheGetAngle( itBase->ipFromPoint, itTest->ipToPoint, itTestNext->ipToPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseFromIsOnTest == TRUE )
				{
					if( itBasePrev != listSegments.end() )
					{
						DOUBLE dAngle1 = AheGetAngle( itTest->ipFromPoint, itBase->ipFromPoint, itBase->ipToPoint );
						DOUBLE dAngle2 = AheGetAngle( itTest->ipFromPoint, itBase->ipFromPoint, itBasePrev->ipFromPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseToIsOnTest == TRUE )
				{
					if( itBaseNext != listSegments.end() ) 
					{
						DOUBLE dAngle1 = AheGetAngle( itTest->ipFromPoint, itBase->ipToPoint, itBase->ipFromPoint );
						DOUBLE dAngle2 = AheGetAngle( itTest->ipFromPoint, itBase->ipToPoint, itBaseNext->ipToPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
			}
			else
			{
				if( bBaseFromIsTestFrom == TRUE || bBaseFromIsTestTo == TRUE || bBaseToIsTestFrom == TRUE || bBaseToIsTestTo == TRUE ||
					bTestFromIsOnBase   == TRUE || bTestToIsOnBase   == TRUE || bBaseFromIsOnTest == TRUE || bBaseToIsOnTest == TRUE )
					return TRUE;
			}
			
			if( bIsNetworkFeature == FALSE )
			{
				// セグメント同士が交差しているかどうか
				DOUBLE Ax = (DOUBLE)( itBase->dToX - itBase->dFromX );
				DOUBLE Ay = (DOUBLE)( itBase->dToY - itBase->dFromY );
				DOUBLE Bx = (DOUBLE)( itTest->dToX - itTest->dFromX );
				DOUBLE By = (DOUBLE)( itTest->dToY - itTest->dFromY );
				DOUBLE DivideBase = (DOUBLE)( (DOUBLE)( By * Ax ) - (DOUBLE)( Bx * Ay ) );
				// コの字など、平行の場合もあるため
				if( DivideBase != 0.0 ) 
				{
					// tは分割点(内分点)の比
					DOUBLE t = (DOUBLE)( ( (DOUBLE)(Ay * (DOUBLE)( itTest->dFromX - itBase->dFromX )) - (DOUBLE)(Ax * (DOUBLE)( itTest->dFromY - itBase->dFromY )) ) / DivideBase );
					if( 0.0 < t && t < 1.0 )
					{
						// sは分割点の比
						DOUBLE s = (DOUBLE)( ( (DOUBLE)(By * (DOUBLE)( itTest->dFromX - itBase->dFromX )) - (DOUBLE)(Bx * (DOUBLE)( itTest->dFromY - itBase->dFromY )) ) / DivideBase );
						if( 0.0 < s && s < 1.0 )
							return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

// ジオメトリに微小セグメントが存在するかどうかの判定
BOOL AheHasMicroSegment( IGeometryPtr ipGeometry, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bFixGeometry/* = FALSE*/ )
{
	IGeometryCollectionPtr ipGeomCol( ipGeometry );
	LONG lGeomCount = -1;

	if( ipGeomCol == NULL ) return FALSE;

	// 最小距離の計算
	if( ipSpRef == NULL )
		ipGeometry->get_SpatialReference( &ipSpRef );
	DOUBLE dMicroLen = AheGetXYDomain1UnitLength( ipSpRef ) * 4.0;	// 斜めも考慮するので

	ipGeomCol->get_GeometryCount( &lGeomCount );
	for( LONG i = 0; i < lGeomCount; i++ )
	{
		IGeometryPtr ipPartGeom;
		ipGeomCol->get_Geometry( i, &ipPartGeom );

		ISegmentCollectionPtr ipSegCol( ipPartGeom );
		if( ipSegCol != NULL )
		{
			LONG lSegCount = -1;
			ipSegCol->get_SegmentCount( &lSegCount );
			for( LONG j = 0; j < lSegCount; j++ )
			{
				_ISegmentPtr ipSegment;
				ipSegCol->get_Segment( j, &ipSegment );
				if( ipSegment == NULL )
					return TRUE;

				DOUBLE dLen = 0.0;
				if( FAILED( ipSegment->get_Length( &dLen ) ) )
					_ASSERTE( FALSE );

				if( dLen < dMicroLen )
				{
					// 修復モードなら修復
					if( bFixGeometry == TRUE )
					{
						// セグメントインデックス+1の構成点を削除
						// ただし、最後のセグメントの場合には最終点が削除されてしまうため、
						// ポリゴンの場合はリングが欠け、ポリラインの場合は他のフィーチャとの
						// 接続性が崩れることになるのでセグメントインデックスの構成点を削除する
						IPointCollectionPtr ipCol( ipPartGeom );
						if( ipCol && SUCCEEDED( ipCol->RemovePoints( ( j == lSegCount - 1 ) ? j : j + 1, 1 ) ) )
						{
							// 修復できたらもう一度最初からチェック
							i = j = -1;
							break;
						}
						else
						{
							AheMessageBox( ERROR_FAIL_REPAIR, ERROR_GEO_IS_NOT_FIXED, MB_OK|MB_ICONWARNING );
							return TRUE;
						}
					}
					else
						return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

// ポイントがセグメント上に乗っているかどうか
BOOL AhePointIsOnSegment( _ISegmentPtr ipBaseSegment, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef, BOOL bExeptEndPoint/* = FALSE*/ )
{
	IPolylinePtr ipBaseSegmentCopy = ToPolyline( ipBaseSegment );
	IPointPtr ipTestPointCopy( AheGetClone( ipTestPoint ) );

	if( !ipBaseSegmentCopy || !ipTestPointCopy || !ipSpRef ) return FALSE;


	// 渡された空間参照での最小距離を求める（グリッドのポイント間の距離）
	double dTorelance = AheGetXYDomain1UnitLength(ipSpRef);

	if (dTorelance <= 0.0)
	{
		ATLASSERT(dTorelance > 0.0);
		return FALSE;
	}
	dTorelance *= 2.0;	// 斜め考慮

	// ポイントとセグメントの距離を計算して、空間参照の最小単位長 x 2（斜めのポイントも考慮するので）より長ければ FALSE
	double dPointToSegmentLen = -1.0; 

	// 空間参照あわせ（あわせないとエラーになる場合有り）
	ipTestPointCopy->putref_SpatialReference(ipSpRef);
	ipBaseSegmentCopy->putref_SpatialReference(ipSpRef);

	if (FAILED(IProximityOperatorPtr(ipBaseSegmentCopy)->ReturnDistance(ipTestPointCopy, &dPointToSegmentLen)))
	{
		AheTraceErrorMessage(_T("AhePointIsOnSegment() : IProximityOperator::ReturnDistance()"));
		ATLASSERT(false);
		return FALSE;
	}
	if (dPointToSegmentLen < 0.0)
	{
		ATLASSERT(dPointToSegmentLen >= 0.0);
		return FALSE;
	}
	// 対象のポイントとセグメント上の最近傍ポイントの距離が最小単位（の2倍）以上だったら乗ってないと判断
	else if (dTorelance < dPointToSegmentLen)
		return FALSE;


	// [bug 8140] 最小単位以内だった場合、セグメント上の最近傍点（ReturnNearestPoint）と一致するかチェックしていたが、
	// それだとビミョウにズレる場合があったので、実際に分割してしてできた構成点と一致するかチェックする
	VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
	long lNewPartIndex = -1, lNewSegmentIndex = -1;
	// ここで、ipTestPoint が ipBaseSegment の端点だった場合に vaIsSplitHappened はFALSE、lNewPartIndex は０または１
	// となり、hr は S_OK が返される。このあとの処理に支障はないため、特に何もケアしない。
	// SplitAtPointの第二引数:projectOntoをFALSEにすると分割位置がipTestPointCopyの位置に移動してしまうので、TRUEに変更
	HRESULT hr = ipBaseSegmentCopy->SplitAtPoint(ipTestPointCopy, VARIANT_TRUE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex);

	// 念のため、インデックス範囲もチェック。セグメントのため０～２
	if (FAILED(hr) ||
		lNewSegmentIndex < 0 ||
		lNewSegmentIndex > 2)
	{
		AheTraceErrorMessage(_T("AhePointIsOnSegment() : SplitAtPoint()"));
		ATLASSERT(false);
		return FALSE;
	}

	// 分割された構成点
	IPointPtr ipSplitedPoint;
	IPointCollectionPtr(ipBaseSegmentCopy)->get_Point(lNewSegmentIndex, &ipSplitedPoint);

	BOOL bRet = AheIsEqual(ipSplitedPoint, ipTestPointCopy);


	// 両端点を除く場合はチェック
	if( bRet && bExeptEndPoint )
	{
		ISpatialReferencePtr ipBaseSpRef;
		ipBaseSegmentCopy->get_SpatialReference( &ipBaseSpRef );
		IPointPtr ipEndPoint( CLSID_Point );

		ipEndPoint->putref_SpatialReference( ipBaseSpRef ); // 空間参照付与
		ipBaseSegmentCopy->QueryFromPoint( ipEndPoint );
		AheSnapToSpatialReference( ipEndPoint, ipSpRef );
		BOOL bEqualFrom = AheIsEqual( ipTestPointCopy, ipEndPoint );
		ipBaseSegmentCopy->QueryToPoint( ipEndPoint );
		AheSnapToSpatialReference( ipEndPoint, ipSpRef );
		BOOL bEqualTo = AheIsEqual( ipTestPointCopy, ipEndPoint );

		if( bEqualFrom == TRUE || bEqualTo == TRUE )
			return FALSE;
	}

	return bRet;
}

// ポイントがジオメトリ上に乗っているかどうか
BOOL AhePointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bExeptVertex/* = FALSE*/ )
{
	if( ipBaseGeometry == NULL || ipTestPoint == NULL ) return FALSE;

	if( ipSpRef == NULL )
		ipBaseGeometry->get_SpatialReference( &ipSpRef );

	esriGeometryType geomType;

	ipBaseGeometry->get_GeometryType( &geomType );
	switch( geomType )
	{
		case esriGeometryPoint:
			return AheIsEqual( ipBaseGeometry, ipTestPoint );
			break;
		default:
		{
			// HitTestで一番近いセグメント取得
			DOUBLE dTorelance = AheGetXYDomain1UnitLength( ipSpRef ) * (DOUBLE)2.0;	// しきい値は最小単位x2
			LONG lPartIndex = -1, lSegmentIndex = -1;
			if( AheTestGeometryHit( ipBaseGeometry, ipTestPoint, dTorelance, NULL, NULL, &lPartIndex, &lSegmentIndex, esriGeometryPartBoundary ) == esriGeometryPartBoundary )
			{
				_ISegmentPtr ipSegment;
				try {
					IGeometryPtr ipPartGeom;
					IGeometryCollectionPtr ipGeomCol( ipBaseGeometry );
					ipGeomCol->get_Geometry( lPartIndex, &ipPartGeom );
					((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lSegmentIndex, &ipSegment );


				}
				catch( _com_error e )
				{
					DEBUGPRINT( _T("AhePointIsOnGeometry() : 例外 : %s\n"), e.ErrorMessage() );
				}

				if (ipSegment)
					return AhePointIsOnSegment(ipSegment, ipTestPoint, ipSpRef, bExeptVertex);
			}
		}
		break;
	}

	return FALSE;
}

// ベースのジオメトリ上にのっているテストのジオメトリの構成点を返す
IPointCollectionPtr AheGetOnGeometryPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IPointCollectionPtr ipPointCol( ipTestGeometry ), ipRetPointCol;
	if( ipBaseGeometry == NULL || ipPointCol == NULL )
	{
		_ASSERTE( ipBaseGeometry != NULL && ipPointCol != NULL );
		return NULL;
	}

	IPointPtr ipPoint( CLSID_Point );
	((IGeometryPtr)ipPoint)->putref_SpatialReference( AheGetSpatialReference( ipTestGeometry ) ); // QueryPointでTestから取るので、Testの空間参照を付与
	LONG lPointCount = -1;

	ipPointCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		ipPointCol->QueryPoint( i, ipPoint );
		if( AhePointIsOnGeometry( ipBaseGeometry, ipPoint, ipSpRef ) )
		{
			if( ! ipRetPointCol )
			{
				ipRetPointCol.CreateInstance( CLSID_Multipoint );
				((IGeometryPtr)ipRetPointCol)->putref_SpatialReference( AheGetSpatialReference( ipTestGeometry ) ); // 同じ理由でTestの空間参照を付与
			}
			ipRetPointCol->AddPoint( (IPointPtr)AheGetClone( ipPoint ) );
		}
	}

	return ipRetPointCol;
}

// ベースのジオメトリ上に追加用構成点が乗っているのであれば、構成点を追加する
BOOL AheAddVertexIfOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipAddVertexPoint, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ipBaseGeometry == NULL || ipAddVertexPoint == NULL )
	{
		_ASSERTE( ipBaseGeometry != NULL && ipAddVertexPoint != NULL );
		return FALSE;
	}

	if( AhePointIsOnGeometry( ipBaseGeometry, ipAddVertexPoint, ipSpRef, TRUE ) == TRUE )
	{
		IPolycurvePtr ipPolycurve( ipBaseGeometry );
		if( ipPolycurve == NULL )
		{
			_ASSERTE( ipPolycurve != NULL );
			return FALSE;
		}

		VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
		LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
		if( FAILED( ipPolycurve->SplitAtPoint( ipAddVertexPoint, VARIANT_FALSE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex ) ) )
		{
			_ASSERTE( FALSE );
			AheTraceErrorMessage( _T("AheAddVertexIfOnGeometry() : SplitAtPoint 失敗") );
			return FALSE;
		}
		if( vaIsSplitHappened != VARIANT_TRUE )
		{
			_ASSERTE( vaIsSplitHappened == VARIANT_TRUE );	// AhePointIsOnGemetry() で TRUE が帰ってきているんだから、必ず成功しないとだめ
			return FALSE;
		}
		ATLTRACE(_T("AheAddVertexIfOnGeometry() : 構成点追加： SPLIT=%s NewPartIndex=%d NewSegmentIndex=%d\n"), (vaIsSplitHappened == VARIANT_TRUE ? _T("TRUE") : _T("FALSE")), lNewPartIndex, lNewSegmentIndex );

		return TRUE;
	}

	return FALSE;
}

// ベースのジオメトリ上に追加用構成点が乗っているのであれば、構成点を追加する
BOOL AheAddVertexesIfOnGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipAddVertexPoints, ISpatialReferencePtr ipSpRef/* = NULL */ )
{
	IPointCollectionPtr ipAddVertexPointCollection( ipAddVertexPoints );
	if( ! ipAddVertexPointCollection )
	{
		if( ipAddVertexPoints )
		{
			esriGeometryType type = esriGeometryAny;
			ipAddVertexPoints->get_GeometryType( &type );
			if( esriGeometryPoint == type )
			{
				ipAddVertexPointCollection.CreateInstance( CLSID_Multipoint );
				((IGeometryPtr)ipAddVertexPointCollection)->putref_SpatialReference( AheGetSpatialReference( ipAddVertexPoints ) ); // Point->Multipointなので、Pointの空間参照を付与
				ipAddVertexPointCollection->AddPoint( (IPointPtr)ipAddVertexPoints );
			}
		}
	}

	if( !ipBaseGeometry || !ipAddVertexPointCollection )
	{
		_ASSERTE( ipBaseGeometry && ipAddVertexPointCollection );
		return NULL;
	}

	// 従来のようにポイント1個ずつSplitAtPointする方法では1セグメントに複数のポイントが配置される場合に
	// 構成点が追加されない問題があるため、SplitAtPointsで処理する	[bug 9790]
	IPolycurve2Ptr ipPolycurve( ipBaseGeometry );
	if( !ipPolycurve )
	{
		_ASSERTE( ipPolycurve );
		return FALSE;
	}
	
	IEnumVertexPtr ipEnumVertex;
	ipAddVertexPointCollection->get_EnumVertices(&ipEnumVertex);
	IEnumSplitPointPtr ipEnumSplitPoint;
	
	// 閾値は空間参照の最小単位の2倍で(AhePointIfOnGeometryに準拠)
	DOUBLE cutOffDistance = AheGetXYDomain1UnitLength(AheGetSpatialReference(ipAddVertexPoints)) * (DOUBLE)2.0;

	if( FAILED( ipPolycurve->SplitAtPoints( ipEnumVertex, VARIANT_FALSE, VARIANT_FALSE, cutOffDistance, &ipEnumSplitPoint ) ) )
	{
		_ASSERTE( FALSE );
		AheTraceErrorMessage( _T("AheAddVertexesIfOnGeometry() : SplitAtPoints 失敗") );
		return FALSE;
	}

	VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
	ipEnumSplitPoint->get_SplitHappened(&vaIsSplitHappened);
	if( !vaIsSplitHappened )
	{
		return FALSE;
	}

#ifdef _DEBUG
	// 分割点のレポート
	ipEnumSplitPoint->Reset();
	IPointPtr ipPoint;
	LONG lNewPartIndex = -1, lNewVertextIndex = -1;
	while( SUCCEEDED( ipEnumSplitPoint->NextInSequence(&ipPoint, &lNewPartIndex, &lNewVertextIndex ) ) && ipPoint ){
		WKSPoint point = {};
		ipPoint->QueryCoords(&(point.X), &(point.Y));
		ATLTRACE(_T("AheAddVertexesIfOnGeometry() : 構成点追加：NewPartIndex=%d NewVertextIndex=%d (%.12f,%.12f)\n")
			, lNewPartIndex, lNewVertextIndex, point.X, point.Y );
	}
#endif

	return TRUE;
}

// ポイント移動時にラバーバンド候補となりうるセグメントを返す
ISegmentCollectionPtr AheGetRubberBandSegment( IGeometryPtr ipGeom, IPointPtr ipSplitPoint, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeom ) );
	ISegmentCollectionPtr ipSegmentCol( CLSID_Polyline );
	((IGeometryPtr)ipSegmentCol)->putref_SpatialReference( AheGetSpatialReference( ipGeom ) ); // ipGeomのセグメントを入れるので、ipSpRefでは無くipGeomの空間参照を付与
	esriGeometryType enumGeomType;

	if( ipGeomCopy == NULL || ipSplitPoint == NULL ) return ipSegmentCol;

	if( ipSpRef == NULL )
		ipGeom->get_SpatialReference( &ipSpRef );
	ipGeom->get_GeometryType( &enumGeomType );
	ipSplitPoint->putref_SpatialReference( ipSpRef );
	ipSplitPoint->SnapToSpatialReference();

	if( !( enumGeomType == esriGeometryPolyline || enumGeomType == esriGeometryPolygon ) ) return ipSegmentCol;

	// 各パートに分かれる
	IGeometryCollectionPtr ipGeomCol( ipGeomCopy );
	LONG lGeomCount = -1;
	ipGeomCol->get_GeometryCount( &lGeomCount );
	for( LONG i = 0; i < lGeomCount; ++i )
	{
		IGeometryPtr ipPartGeom;
		ipGeomCol->get_Geometry( i, &ipPartGeom );

		esriGeometryType geomType;
		LONG lPartPointCount = -1;
		LONG lIndex = -1;

		ipPartGeom->get_GeometryType( &geomType );
		((IPointCollectionPtr)ipPartGeom)->get_PointCount( &lPartPointCount );
		
		// 構成点かどうかを確認
		if( AheIsVertexPoint( ipSplitPoint, ipPartGeom, ipSpRef, &lIndex ) == TRUE )
		{
			_ISegmentPtr ipSegment;
			switch( geomType )
			{
			// Pathだったら最初の点を渡してセグメントを作成
			case esriGeometryPath:
				((ISegmentCollectionPtr)ipPartGeom)->get_Segment( 0, &ipSegment );
				ipSegmentCol->AddSegment( ipSegment );
				break;

			// Ringだったら構成点を掴んだとき２つのセグメントがラバーバンドになる
			case esriGeometryRing:
				if( 0 <= lIndex && lIndex < lPartPointCount )
				{
					// 1個めのセグメント
					((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lIndex, &ipSegment );
					ipSegmentCol->AddSegment( ipSegment );

					// もうひとつのセグメント
					// 開始点なら最後の点を渡して取得する
					long lOtherIndex = (lIndex == 0) ? lPartPointCount - 2 : lIndex - 1;
					((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lOtherIndex, &ipSegment );
					ipSegmentCol->AddSegment( ipSegment );
				}
				break;

			default:
				break;
			}

			// [Bug 4972] 2点以下で構成されているフィーチャを編集すると落ちる

			//if( 0 <= lIndex && lIndex < lPartPointCount )
			//{
			//	_ISegmentPtr ipSegment1;
			//	((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lIndex, &ipSegment1 );
			//	ipSegmentCol->AddSegment( ipSegment1 );
			//}

			//// 開始点の時は、 Ring の時だけ一番最後も入れる
			//LONG lOtherIndex = -1;
			//if( lIndex == 0 )
			//{
			//	if( geomType == esriGeometryRing )
			//		lOtherIndex = lPartPointCount - 2;
			//}
			//else
			//	lOtherIndex = lIndex - 1;

			//if( lOtherIndex > 0 )
			//{
			//	_ISegmentPtr ipSegment2;
			//	((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lOtherIndex, &ipSegment2 );
			//	ipSegmentCol->AddSegment( ipSegment2 );
			//}
		}
		else {
			// ポイントがジオメトリに載っている場合に限定
			if( AhePointIsOnGeometry( ipPartGeom, ipSplitPoint, ipSpRef ) == TRUE )
			{
				// IRing, IPath > IPolycurve 変換
				IPolycurvePtr ipPolycurve;
				switch( geomType )
				{
					case esriGeometryPath: ipPolycurve.CreateInstance( CLSID_Polyline ); break;
					case esriGeometryRing: ipPolycurve.CreateInstance( CLSID_Polygon  ); break;
					default: _ASSERTE(FALSE);	// 他の型が来るわけがない
				}
				ipPolycurve->putref_SpatialReference( ipSpRef ); // 空間参照付与
				WKSPoint* pPoints = NULL;
				IPointCollectionPtr ipPointColTmp( ipPartGeom );
				LONG lPointCount = -1;
				ipPointColTmp->get_PointCount( &lPointCount );
				pPoints = new WKSPoint[lPointCount];
				ipPointColTmp->QueryWKSPoints( 0, lPointCount, pPoints );
				((IPointCollectionPtr)ipPolycurve)->SetWKSPoints( lPointCount, pPoints );
				delete [] pPoints;
				switch( geomType )
				{
					case esriGeometryPath: ((IPolylinePtr)ipPolycurve)->SimplifyNetwork();        break;
					case esriGeometryRing: ((IPolygonPtr )ipPolycurve)->SimplifyPreserveFromTo(); break;
					default: _ASSERTE(FALSE);	// 他の型が来るわけがない
				}
				// 切ってみる
				VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
				LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
				ipPolycurve->SplitAtPoint( ipSplitPoint, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );
				
				if( vaSplitHappened == VARIANT_TRUE && lNewSegmentIndex > 0 )
				{
					_ISegmentPtr ipSegment1;
					((ISegmentCollectionPtr)ipPolycurve)->get_Segment( lNewSegmentIndex, &ipSegment1 );
					ipSegmentCol->AddSegment( ipSegment1 );

					_ISegmentPtr ipSegment2;
					((ISegmentCollectionPtr)ipPolycurve)->get_Segment( lNewSegmentIndex - 1, &ipSegment2 );
					ipSegmentCol->AddSegment( ipSegment2 );
				}
				else if( lNewSegmentIndex == 0 )
					ATLASSERT( FALSE );	// ないとおもう
			}
		}
	}

	return ipSegmentCol;
}

IGeometryPtr AheSetKnownSimple(IGeometryPtr ipGeometry)
{
	ITopologicalOperator2Ptr ipTopo2 = ipGeometry;

	if ( ipTopo2 ) {
		VARIANT_BOOL vbIsKnownSimple = VARIANT_FALSE;

		if ( SUCCEEDED( ipTopo2->get_IsKnownSimple( &vbIsKnownSimple ) ) ) {
			if ( !vbIsKnownSimple ) {
				if ( FAILED( ipTopo2->put_IsKnownSimple( VARIANT_TRUE ) ) )
					_ASSERTE(false); // put_IsKnownSimpleに失敗
			}
		} else
			_ASSERTE(false); // get_IsKnownSimpleに失敗
	}

	return ipGeometry;
}

IGeometryPtr AheRemoveAdjoiningPoint(IGeometryPtr ipGeometry)
{
	AheTraceGeometry( ipGeometry, _T("AheRemoveAdjoiningPoint前()") );

	IGeometryPtr ipRetGeometry = ipGeometry;

	if ( ipRetGeometry ) {
		esriGeometryType geomType;

		ipRetGeometry->get_GeometryType( &geomType );

		IGeometryCollectionPtr ipGeomColl( ipRetGeometry );

		if ( ipGeomColl && geomType != esriGeometryMultipoint ) {
			long lGeomCount = 0;

			ipGeomColl->get_GeometryCount( &lGeomCount );

			for ( long i = 0; i < lGeomCount; ++i ) {
				IGeometryPtr ipGeom;

				ipGeomColl->get_Geometry( i, &ipGeom );
				ipGeom = AheRemoveAdjoiningPointProc( ipGeom );
			}
		} else
			ipRetGeometry = AheRemoveAdjoiningPointProc( ipRetGeometry );
	}

	AheTraceGeometry( ipRetGeometry, _T("AheRemoveAdjoiningPoint後()") );

	return ipRetGeometry;
}

IGeometryPtr AheRemoveAdjoiningPointProc(IGeometryPtr ipGeometry)
{
	AheTraceGeometry( ipGeometry, _T("AheRemoveAdjoiningPointProc前()") );

	IGeometryPtr ipRetGeometry = ipGeometry;
	IPointCollectionPtr ipPointColl( ipRetGeometry );

	if ( ipPointColl ) {
		// ポイント数取得
		long lPointCount = 0;

		ipPointColl->get_PointCount( &lPointCount );

		// 複数ポイントがある時のみ
		if ( lPointCount > 1 ) {
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			esriGeometryType emType = esriGeometryAny;
			ipGeometry->get_GeometryType( &emType );

			CLSID clsid = CLSID_NULL;
			switch ( emType )
			{
				case esriGeometryRing: clsid = CLSID_Ring; break;
				case esriGeometryPath: clsid = CLSID__Path; break;
				case esriGeometryPolygon: clsid = CLSID_Polygon; break;
				case esriGeometryPolyline: clsid = CLSID_Polyline; break;
				case esriGeometryLine: clsid = CLSID_Line; break;
				case esriGeometryEnvelope: clsid = CLSID_Envelope; break;
				case esriGeometryMultipoint: clsid = CLSID_Multipoint; break;
				case esriGeometryBag: clsid = CLSID_GeometryBag; break;
				default: return ipRetGeometry;
			}

			long lNewPointCount = 0;
			IPointCollectionPtr ipNewPointColl( clsid );
			((IGeometryPtr)ipNewPointColl)->putref_SpatialReference( AheGetSpatialReference( ipGeometry ) );
			for ( long i = 0; i < lPointCount; ++i ) {
				IPointPtr ipPointSrc;
				ipPointColl->get_Point( i, &ipPointSrc );

				bool bIsNoSame = false;

				if ( i+1 >= lPointCount ) {
					// 最後の点は必ず追加
					bIsNoSame = true;
				} else {
					IPointPtr ipPointDest;
					ipPointColl->get_Point( i+1, &ipPointDest );

					// 隣り合っている点が等しくない？
					if ( ! AheIsEqual( ipPointSrc, ipPointDest ) )
						bIsNoSame = true;
				}

				if ( bIsNoSame ) {
					// 格納しようとしている点と新規ポイント群が等しくない点を新規ポイント群に設定。
					for ( long j = 0; j < lNewPointCount; ++j ) {
						IPointPtr ipPointDest;
						if ( j+1 >= lNewPointCount ) {
							// 最後の点は一つ前の点と比較
							ipPointColl->get_Point( j, &ipPointDest );
						} else {
							ipPointColl->get_Point( j+1, &ipPointDest );
						}

						// 隣り合っている点が等しい？
						if ( AheIsEqual( ipPointSrc, ipPointDest ) )
							bIsNoSame = false;
					}

					if ( bIsNoSame ) {
						// 新規ポイント群格納
						ipNewPointColl->AddPoint( ipPointSrc );
						++lNewPointCount;
					}
				}
			}

			// 0ってことは全部同じ点なはず
			if ( lNewPointCount == 0 ) {
				IPointPtr ipPointSrc;
				ipPointColl->get_Point( 0, &ipPointSrc );
				// 始点格納
				ipNewPointColl->AddPoint( ipPointSrc );
				++lNewPointCount;
			}

			ipPointColl->SetPointCollection( ipNewPointColl );
#else
			// 新規ポイント群領域確保
			WKSPoint* wksNew = new WKSPoint[lPointCount];

			// ポイント群取得
			WKSPoint* wks = new WKSPoint[lPointCount];

			ipPointColl->QueryWKSPoints( 0, lPointCount, wks );

			// 隣り合っている点が等しくない点を新規ポイント群に設定。
			long lNewPointCount = 0;

			for ( long i = 0; i < lPointCount; ++i ) {
				WKSPoint wksSrc={0}, wksDest={0};
				bool bIsNoSame = false;

				wksSrc = wks[i];

				if ( i+1 >= lPointCount ) {
					// 最後の点は必ず追加
					bIsNoSame = true;
				} else {
					wksDest = wks[i+1];

					// 隣り合っている点が等しくない？
					if ( wksSrc.X != wksDest.X || wksSrc.Y != wksDest.Y )
						bIsNoSame = true;
				}

				if ( bIsNoSame ) {
					// 格納しようとしている点と新規ポイント群が等しくない点を新規ポイント群に設定。
					for ( long j = 0; j < lNewPointCount; ++j ) {
						if ( j+1 >= lNewPointCount ) {
							// 最後の点は一つ前の点と比較
							wksDest = wksNew[j];
						} else {
							wksDest = wksNew[j+1];
						}

						// 隣り合っている点が等しい？
						if ( wksSrc.X == wksDest.X && wksSrc.Y == wksDest.Y )
							bIsNoSame = false;
					}

					if ( bIsNoSame ) {
						// 新規ポイント群格納
						wksNew[lNewPointCount] = wksSrc;
						++lNewPointCount;
					}
				}
			}

			// 0ってことは全部同じ点なはず
			if ( lNewPointCount == 0 ) {
				// 始点格納
				wksNew[lNewPointCount] = wks[0];
				++lNewPointCount;
			}

			// 新規ポイント群を格納
			ipPointColl->SetWKSPoints( lNewPointCount, wksNew );

			// ポイント群領域消去
			delete [] wksNew;
			delete [] wks;
#endif
		}
	}

	// なぜかVARIANT_FALSEになるので
	AheSetKnownSimple( ipRetGeometry );

	AheTraceGeometry( ipRetGeometry, _T("AheRemoveAdjoiningPointProc後()") );

	return ipRetGeometry;
}

/////////////////////////////////////////////////////////////////////////////
// ネットワーク系チェックで使用する関数
/////////////////////////////////////////////////////////////////////////////

// 構成点同士が接触していないかどうか調べる
bool AheIsTouchVertex( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom, IPointPtr& ipTouch )
{
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return false;

	bool ret = false; // 戻り値
	IEnumVertexPtr ipBaseEnum, ipTestEnum;
	ipBaseGeom->get_EnumVertices( &ipBaseEnum );
	ipTestGeom->get_EnumVertices( &ipTestEnum );
	ipBaseEnum->Reset();
	long baseCount = 0, testCount = 0;
	ipBaseGeom->get_PointCount( &baseCount );
	ipTestGeom->get_PointCount( &testCount );

	long basePart = 0, baseVert = 0, baseTotal = 0;
	IPointPtr ipBaseP;
	while( S_OK == ipBaseEnum->Next( &ipBaseP, &basePart, &baseVert ) )
	{
		if( ! ipBaseP )
			break;
		++baseTotal;

		ipTestEnum->Reset();
		long testPart = 0, testVert = 0, testTotal = 0;
		IPointPtr ipTestP;
		while( S_OK == ipTestEnum->Next( &ipTestP, &testPart, &testVert ) )
		{
			if( ! ipTestP )
				break;
			++testTotal;

			// ネットワーク系オブジェクト同士の構成点同士の接触判定に使用（bug 8463 ロードチェック2320）
			// 始点同士、終点同士は飛ばす
			// マルチパートの場合はこの関数の使用意義から外れるので、しらね
			if( 0 == basePart && 0 == baseVert && 0 == testPart && 0 == testVert || // 両方始点
				baseCount == baseTotal && testCount == testTotal ||                 // 両方終点
				0 == basePart && 0 == baseVert && testCount == testTotal ||         // base始点test終点
				0 == testPart && 0 == testVert && baseCount == baseTotal )          // base終点test始点
				continue;

			if( AheIsDBEqual( ipBaseP, ipTestP ) )
			{
				ipTouch = AheCreateClone( ipBaseP );
				return true;
			}
		}
	}
	return false;
}

// 端点が接触している形状の第一セグメントとの距離を返します。
double AheGetConnectSegmentCrack( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom )
{
	double ret = -1.0; // 戻り値

	if( ! ( ipBaseGeom && ipTestGeom ) )
		return ret;

	IPointPtr ipBaseFrom, ipBaseTo, ipTestFrom, ipTestTo, ipTestPoint;
	long baseCount = 0, testCount = 0;
	ipBaseGeom->get_PointCount( &baseCount );
	ipTestGeom->get_PointCount( &testCount );
	if( ! ( baseCount > 0 && testCount > 0 ) )
		return ret;

	ipBaseGeom->get_Point( 0, &ipBaseFrom );
	ipBaseGeom->get_Point( baseCount -1, &ipBaseTo );
	ipTestGeom->get_Point( 0, &ipTestFrom );
	ipTestGeom->get_Point( testCount -1, &ipTestTo );
	if( AheIsDBEqual( ipBaseFrom, ipTestFrom ) ||
		AheIsDBEqual( ipBaseTo,   ipTestFrom ) )
		ipTestGeom->get_Point( 1, &ipTestPoint );
	else if( AheIsDBEqual( ipBaseFrom, ipTestTo ) ||
		AheIsDBEqual( ipBaseTo, ipTestTo ) )
		ipTestGeom->get_Point( testCount -2, &ipTestPoint );
	else
		return ret; // 接触していない

	// チェック仕様では法線距離と書いてあるが、ここでは単純に
	// セグメント端点とラインの距離を出している。
	// これは、法線距離を出すのが面倒なのと、base と test を
	// 入れ替えることでもう一方のチェックでは必ず法線となるた
	// めである。
	IProximityOperatorPtr ipProx( ipBaseGeom );
	IPointPtr ipNearPoint;
	ipProx->ReturnNearestPoint( ipTestPoint, esriNoExtension, &ipNearPoint );

	double dx1 = 0.0, dy1 = 0.0, dx2 = 0.0, dy2 = 0.0;
	ipTestPoint->QueryCoords( &dx1, &dy1 );
	ipNearPoint->QueryCoords( &dx2, &dy2 );
	ret = AheGETDIST( dx1, dy1, dx2, dy2 );

	return ret*100.0;
}
