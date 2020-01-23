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
#include "ChangeSmart.h"
#include "globals.h"

CChangeSmart::CChangeSmart(void) : m_dAllowOffset( 0.001 )
{
}

CChangeSmart::~CChangeSmart(void)
{
}

bool CChangeSmart::ExecSmart(IFeatureClassPtr ipFeatureClass)
{
	if( !ipFeatureClass )
	{
		std::cerr << "[ERROR] no FeatureClass" << std::endl;
		return false;
	}

	// ジオメトリタイプチェック
	esriGeometryType esriType = esriGeometryLine;
	ipFeatureClass->get_ShapeType( &esriType );
	if( esriGeometryPolygon != esriType )
	{
		std::cerr << "[ERROR] Only Polygon. GeometryType = " << esriType << std::endl;
		return false;
	}

	// 形状の検索
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL );
	if( !ipFilter )
	{
		std::cerr << "[ERROR] no QueryFilter" << std::endl;
		return false;
	}
	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
	{
		std::cerr << "[ERROR] fail feature search" << std::endl;
		return false;
	}

	IFeaturePtr ipFeature;
	std::map<long, long> mapFeatureChk;			// 処理済みか判断する
	while( ipCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
	{
		long lOID = 0;
		ipFeature->get_OID( &lOID );
		mapFeatureChk[ lOID ] = 0;
	}

	if( !ToSmart( ipFeatureClass, mapFeatureChk ) )
		return false;

	return true;
}

bool CChangeSmart::ToSmart(IFeatureClassPtr ipFeatureClass, std::map<long, long>& mapFeatureChk)
{
	long lTotalCnt = mapFeatureChk.size();
	long lCnt = 1;

	OutPutTime( "Polygon 間引きスタート" );

			IPathPtr ipPath1;
			IPathPtr ipPath2;
			long aa = 0;
			ISpatialReferencePtr ipSpRef;
	for( std::map<long, long>::iterator it = mapFeatureChk.begin(); it != mapFeatureChk.end(); ++it, ++lCnt )
	{
		printf( "%07d / %07d\r", lCnt, lTotalCnt );

		if( lCnt % 1000 == 0 )
		{
			CString cStr;
			cStr.Format( _T("Polygon 処理 : %d"), lCnt );
			std::string str = CT2CA( cStr );
			OutPutTime( str );
		}

		IFeaturePtr ipFeature;
		ipFeatureClass->GetFeature( it->first, &ipFeature );
		if( !ipFeature )
		{
			std::cerr << "[WARNING] fail GetFeature ( " << it->first << " )" << std::endl;
			continue;
		}
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		ipGeom->get_SpatialReference( &ipSpRef );

		// test
		{
			esriGeometryType esriType = esriGeometryLine;
			IGeometryCollectionPtr ipGeomCollection( ipGeom );
			long lGeomCnt = 0;
			ipGeomCollection->get_GeometryCount( &lGeomCnt );
			// アウターループとインナーループを取得する
			if( aa == 0 )
			{
				for( long l=0; l<lGeomCnt; ++l )
				{
					IGeometryPtr ipGeomLoop;
					ipGeomCollection->get_Geometry( l, &ipGeomLoop );
					if( !ipGeomLoop )
						continue;

					ipGeomLoop->get_GeometryType( &esriType );
					if( esriGeometryRing == esriType )
					{
					}
					IPointCollectionPtr ipCol( ipGeomLoop );
					long lCntP = 0;
					ipCol->get_PointCount( &lCntP );
					IPolylinePtr ipPoly( ipGeomLoop );
					if( ipPoly )
					{
						continue;
					}
					IPathPtr ipPath( ipGeomLoop );
					if( ipPath )
					{
						IPointCollectionPtr ipCol( ipPath );
						long lCntP = 0;
						ipCol->get_PointCount( &lCntP );
						if( l == 0 )
							ipPath1 = ipPath;
					}
					aa = 1;
					break;
				}
			}
			else
			{
				for( long l=0; l<lGeomCnt; ++l )
				{
					IGeometryPtr ipGeomLoop;
					ipGeomCollection->get_Geometry( l, &ipGeomLoop );
					if( !ipGeomLoop )
						continue;

					ipGeomLoop->get_GeometryType( &esriType );
					if( esriGeometryRing == esriType )
					{
					}
					IPointCollectionPtr ipCol( ipGeomLoop );
					long lCntP = 0;
					ipCol->get_PointCount( &lCntP );
					IPolylinePtr ipPoly( ipGeomLoop );
					if( ipPoly )
					{
						continue;
					}
					IPathPtr ipPath( ipGeomLoop );
					if( ipPath )
					{
						IPointCollectionPtr ipCol( ipPath );
						long lCntP = 0;
						ipCol->get_PointCount( &lCntP );
						if( l == 0 )
							ipPath2 = ipPath;
					}
				}
				break;
			}
			continue;
		}
		ISpatialFilterPtr ipSpFilter = AheGetSpatialFilter( ipGeom, ipFeatureClass, esriSpatialRelTouches, 0, 0 );

		IFeatureCursorPtr ipCursor;
		ipFeatureClass->Search( ipSpFilter, VARIANT_FALSE, &ipCursor );
		if( !ipCursor )
		{
			std::cerr << "[ERROR] fail search polygon ( " << it->first << " )" << std::endl;
			continue;
		}
		IFeaturePtr ipTouchFeature;
		while( S_OK == ipCursor->NextFeature( &ipTouchFeature ) && ipTouchFeature )
		{
			long lOID = 0;
			ipTouchFeature->get_OID( &lOID );
			if( lOID == it->first )
				continue;
			// 既にチェック済みであれば、スキップ
			if( mapFeatureChk[ lOID ] )
				continue;

			// 形状を取得し直す必要あり
			IGeometryPtr ipGeom;
			ipFeature->get_ShapeCopy( &ipGeom );

			IGeometryPtr ipTargetGeom;
			ipTouchFeature->get_ShapeCopy( &ipTargetGeom );

			// 基準ポリゴンと接触ポリゴンのインターセクト(共有部分)を取得する
			IGeometryPtr ipIntersect;
			((ITopologicalOperatorPtr)ipGeom)->Intersect( ipTargetGeom, esriGeometry1Dimension, &ipIntersect );
			if( !ipIntersect )
			{
				std::cerr << "[WARNING] no intersect " << it->first << " and " << lOID << std::endl;
				continue;
			}

			IGeometryCollectionPtr ipGeomCollection( ipIntersect );
			long lGeomCnt = 0;
			ipGeomCollection->get_GeometryCount( &lGeomCnt );

			IPolygonPtr ipBasePolygon = ipGeom;
			IPolygonPtr ipTouchPolygon = ipTargetGeom;
			for( long l = 0; l < lGeomCnt; ++l )
			{
				IGeometryPtr ipIntersectGeom;
				ipGeomCollection->get_Geometry( l, &ipIntersectGeom );
				IPathPtr ipPath( ipIntersectGeom );
				if( !ipPath )
					continue;

#ifdef _DEBUG
				long lBaseOID = it->first;
				long lCnt = 0;
				((IPointCollectionPtr)ipPath)->get_PointCount( &lCnt );
				std::cout << "ipPath count : " << lCnt << std::endl;
#endif
				// 共有部分のポリランをスムージングする
				if( FAILED( ipPath->Generalize( m_dAllowOffset ) ) )
					continue;

#ifdef _DEBUG
				lCnt = 0;
				((IPointCollectionPtr)ipPath)->get_PointCount( &lCnt );
				std::cout << "ipPath count : " << lCnt << std::endl;
#endif

				IPointPtr ipFromP;
				ipPath->get_FromPoint( &ipFromP );

				// 基準ポリゴンと接触ポリゴンでループの向きが違うため処理を分ける

				// 基準ポリゴン
				IClonePtr ipClone;
				((IClonePtr)ipPath)->Clone( &ipClone );
				IPathPtr ipBasePath = ipClone;

				bool bBaseOuter = false;
				LIST_LOOP listBaseInner;
				LIST_POINTS listBaseOuter;
				GetLoop( ipBasePolygon, ipFromP, listBaseOuter, listBaseInner, bBaseOuter );
				// インナーに共有部分がある場合はリバースする
				if( !bBaseOuter )
					ipBasePath->ReverseOrientation();

				if( listBaseOuter.empty() )
				{
					std::cerr << "[WARNING] no Base Outer loop : " << it->first << std::endl;
					continue;
				}

				ipBasePolygon = GetMergePolygon( listBaseOuter, listBaseInner, bBaseOuter, ipBasePath );

				// 接触ポリゴン
				IClonePtr ipTouchClone;
				((IClonePtr)ipPath)->Clone( &ipTouchClone );
				IPathPtr ipTouchPath = ipTouchClone;

				bool bTouchOuter = false;
				LIST_LOOP listTouchInner;
				LIST_POINTS listTouchOuter;
				GetLoop( ipTouchPolygon, ipFromP, listTouchOuter, listTouchInner, bTouchOuter );
				// 接触側はアウターに共有部分がある場合リバースする
				if( bTouchOuter )
					ipTouchPath->ReverseOrientation();

				if( listTouchOuter.empty() )
				{
					std::cerr << "[WARNING] no Touch Outer loop : " << lOID << std::endl;
					continue;
				}

				ipTouchPolygon = GetMergePolygon( listTouchOuter, listTouchInner, bTouchOuter, ipTouchPath );
			}

			if( ipBasePolygon )
			{
				ipFeature->putref_Shape( ipBasePolygon );
				ipFeature->Store();
			}

			if( ipTouchPolygon )
			{
				ipTouchFeature->putref_Shape( ipTouchPolygon );
				ipTouchFeature->Store();
			}
			// チェック済み
			mapFeatureChk[ it->first ] = 1;
		}
	}

	ISegmentCollectionPtr ipGeoCol( CLSID_Polyline );
	((IPolylinePtr)ipGeoCol)->putref_SpatialReference( ipSpRef );
	ISegmentCollectionPtr ipSegC( ipPath1 );
	long lseg;
	ipSegC->get_SegmentCount( &lseg );
	for( long l=0; l<lseg; ++l )
	{
		_ISegmentPtr ipSeg;
		ipSegC->get_Segment( l, &ipSeg );
		ipGeoCol->AddSegment( ipSeg );
	}
//	ipGeoCol->AddGeometry( ipPath1 );
	IPolylinePtr ipPoly1 = ipGeoCol;
//	IGeometryCollectionPtr ipGeoCol2( CLSID_Polyline );
//	ipGeoCol2->AddGeometry( ipPath2 );
	ISegmentCollectionPtr ipGeoCol2( CLSID_Polyline );
	((IPolylinePtr)ipGeoCol2)->putref_SpatialReference( ipSpRef );
	ISegmentCollectionPtr ipSegC2( ipPath2 );
	ipSegC2->get_SegmentCount( &lseg );
	for( long l=0; l<lseg; ++l )
	{
		_ISegmentPtr ipSeg;
		ipSegC->get_Segment( l, &ipSeg );
		ipGeoCol2->AddSegment( ipSeg );
	}
	IPolylinePtr ipPoly2 = ipGeoCol2;
	IGeometryPtr ipdiff;
	ITopologicalOperatorPtr ipTop( ipPoly1 );
	((ITopologicalOperatorPtr)ipTop)->Difference( ipPoly2, &ipdiff );
	IGeometryCollectionPtr ipDiff( ipdiff );
	esriGeometryType type;
	ipdiff->get_GeometryType( &type );
	long lCntaa;
	ipDiff->get_GeometryCount( &lCntaa );
	IPolylineArrayPtr ipArr( ipdiff );
//	ipArr->get_Count( &lCntaa );
	IPointCollectionPtr ipC( ipPoly1 );
	ipC->get_PointCount( &lCntaa );

	IPointCollectionPtr ipC2( ipPoly2 );
	ipC2->get_PointCount( &lCntaa );

	ISegmentCollectionPtr ipSegC3( ipdiff );
	ipSegC3->get_SegmentCount( &lseg );
	
	IPointCollectionPtr ipC4( ipdiff );
	ipC4->get_PointCount( &lCntaa );
	IPointPtr ipPPP;
	for(long l=0; l<lCntaa; ++l )
	{
		ipC4->get_Point( l, &ipPPP );
		double x;
		double y;
		ipPPP->get_X( &x );
		ipPPP->get_Y( &y );
		ATLTRACE( "%f,%f\n", x,y );
	}

	return true;
}

IPolygonPtr CChangeSmart::GetMergePolygon(LIST_POINTS& listOuter, LIST_LOOP& listInner, bool bOuter, IPathPtr ipPath)
{
	IPointPtr ipFromPoint;
	ipPath->get_FromPoint( &ipFromPoint );
	if( bOuter )
		ChangeLoop( listOuter, ipPath );
	else
	{
		for( LIST_LOOP::iterator it = listInner.begin(); it != listInner.end(); ++it )
			ChangeLoop( *it, ipPath );
	}

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipFromPoint );
	IPointCollectionPtr ipOuterCol( CLSID_Polygon );
	((IPolygonPtr)ipOuterCol)->putref_SpatialReference( ipSpRef );
	for( LIST_POINTS::iterator it = listOuter.begin(); it != listOuter.end(); ++it )
		ipOuterCol->AddPoint( it->m_T );

	ipOuterCol->AddPoint( ( listOuter.begin() )->m_T );		// 閉じるために最初のポイント追加
	IPolygonPtr ipOuterPolygon = ipOuterCol;

	// ポリゴンチェック
	IAreaPtr ipArea( ipOuterPolygon );
	double dArea = 0;
	ipArea->get_Area( &dArea );
	if( dArea <= 0 )
	{
		std::cerr << "[ERROR] Outer polygon(01) is 0" << std::endl;
		return NULL;
	}

	for( LIST_LOOP::iterator itLoop = listInner.begin(); itLoop != listInner.end(); ++itLoop )
	{
		IPointCollectionPtr ipInnerCol( CLSID_Polygon );
		((IPolygonPtr)ipInnerCol)->putref_SpatialReference( ipSpRef );

		for( LIST_POINTS::iterator it = itLoop->begin(); it != itLoop->end(); ++it )
			ipInnerCol->AddPoint( it->m_T );

		ipInnerCol->AddPoint( ( itLoop->begin() )->m_T );

		IPolygonPtr ipInnerPolygon = ipInnerCol;

#ifdef _DEBUG
#endif
		IGeometryPtr ipRetGeom;
		((ITopologicalOperatorPtr)ipOuterPolygon)->Difference( ipInnerPolygon, &ipRetGeom );
		ipOuterPolygon = ipRetGeom;
	}

	ipArea = ipOuterPolygon;
	ipArea->get_Area( &dArea );
	if( dArea <= 0 )
	{
		std::cerr << "[ERROR] Outer polygon(02) is 0" << std::endl;
		return NULL;
	}

	return ipOuterPolygon;

}

// ポリゴンをアウターとインナーループ群に分ける。インナーは順番をリバース済み
bool CChangeSmart::GetLoop(IPolygonPtr ipPolygon, IPointPtr ipFromPoint, LIST_POINTS& listOuter, LIST_LOOP& listInner, bool& bOuter)
{
	IPointCollectionPtr ipPointCol( ipPolygon );
	if( !ipPointCol )
		return NULL;

	// アウターとインナーのポイント列に分ける
	long lCount = 0;
	ipPointCol->get_PointCount( &lCount );
	bool bOuterChk = true;		// アウター取得(1順目)
	bool bFirst = true;		// 1点目かどうか
	IPointPtr ipFirstP;
	LIST_POINTS listPoints;
	for( long l = 0; l < lCount; ++l )
	{
		IPointPtr ipPoint;
		ipPointCol->get_Point( l, &ipPoint );

		// ipFromPointを同じポイントがアウター内に存在したかどうか
		if( AheIsEqual( ipFromPoint, ipPoint ) )
			bOuter = bOuterChk;

		if( bFirst )
		{
			// 1点目を保持する
			listPoints.clear();
			ipFirstP = ipPoint;
			listPoints.push_back( ipPoint );
			bFirst = false;

			continue;
		}
		// 1点目と同じポイントが出現したら、1つのループが完成
		// 最後に同一ポイントは追加しない
		if( AheIsEqual( ipFirstP, ipPoint ) )
		{
			if( bOuterChk )
			{
				listOuter = listPoints;
				bOuterChk = false;
			}
			else
			{
				listPoints.reverse();
				listInner.push_back( listPoints );
			}

			listPoints.clear();
			bFirst = true;
			continue;
		}

		// ポイント追加
		listPoints.push_back( ipPoint );
	}

	return true;
}

// ipPointがあった場合は、ipPointを先頭に並べ替え
bool CChangeSmart::SortTarget(LIST_POINTS& listPoints, IPointPtr ipPoint)
{
	bool bOK = false;
	LIST_POINTS tmpBackList;
	LIST_POINTS tmpFrontList;
	for( LIST_POINTS::iterator it = listPoints.begin(); it != listPoints.end(); ++it )
	{
		// ipPointが見つからない間
		if( !bOK )
		{
			if( AheIsEqual( ipPoint, it->m_T ) )
			{
				bOK = true;
				tmpFrontList.push_back( it->m_T );
			}
			else
				tmpBackList.push_back( it->m_T );
		}
		else		// ipPointが見つかった場合
			tmpFrontList.push_back( it->m_T );

	}

	if( bOK )
	{
		listPoints.clear();
		listPoints = tmpFrontList;
		for( LIST_POINTS::iterator it = tmpBackList.begin(); it != tmpBackList.end(); ++it )
			listPoints.push_back( it->m_T );
	}

	return bOK;
}

void CChangeSmart::ChangeLoop(LIST_POINTS& listPoints, IPathPtr ipPath)
{
	IPointCollectionPtr ipPointCol( ipPath );
	IPointPtr ipBegin;
	IPointPtr ipEnd;
	ipPath->get_FromPoint( &ipBegin );
	ipPath->get_ToPoint( &ipEnd );

	// listPointsに共有部分があれば並び替え
	if( !SortTarget( listPoints, ipBegin ) )
		return;

	bool bChange = false;
	LIST_POINTS tmpList;
	for( LIST_POINTS::iterator it = listPoints.begin(); it != listPoints.end(); ++it )
	{
		if( !bChange )
		{
			// 共有部分が見つかるまで
			if( AheIsEqual( ipBegin, it->m_T ) )
			{
				long lCnt = 0;
				ipPointCol->get_PointCount( &lCnt );
				for( long l = 0; l<lCnt; ++l )
				{
					IPointPtr ipPoint;
					ipPointCol->get_Point( l, &ipPoint );
					tmpList.push_back( ipPoint );
				}
				bChange = true;

				continue;
			}
			tmpList.push_back( it->m_T );
		}
		else
		{
			if( !AheIsEqual( ipEnd, it->m_T ) )
				continue;

			// 共有部を置き換えたら、またポイントを追加
			bChange = false;
		}
	}

	listPoints = tmpList;
}
