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
#include "PointRotate.h"

// 点列を回転させて一番X,Yが小さいところにする
IGeometryPtr	PointRotate::
ExecRotate	( IGeometryPtr&	c_ipGeo )
{
	IGeometryPtr	a_ipRetGeo;	// 返却Geometry

	a_ipRetGeo	= c_ipGeo;

	try {
		// ジオメトリタイプの判別
		esriGeometryType	a_eGeoType;
		a_ipRetGeo->get_GeometryType ( &a_eGeoType );
		if( a_eGeoType != esriGeometryPolyline && a_eGeoType != esriGeometryPolygon ) {
			return	( NULL );
		}
		// 
		IGeometryCollectionPtr	a_ipGeoColl;
		a_ipGeoColl	= a_ipRetGeo;

		long	a_lGeoColl = 0;
		a_ipGeoColl->get_GeometryCount ( &a_lGeoColl );
		
		int i = 0;
		for( i = 0; i < a_lGeoColl; i++ )
		{
			IGeometryPtr	a_ipCurGeo;
			a_ipGeoColl->get_Geometry (i, &a_ipCurGeo );

			IPointCollectionPtr	a_ipPtsColl;
			a_ipPtsColl	= a_ipCurGeo;

			long	a_lPtsCount = 0;
			a_ipPtsColl->get_PointCount ( &a_lPtsCount );

			int j = 0;
			std::list<PtNum>	a_vpPtNum;
			PtNum				a_vpMinPoint;	// 最小XYの位置(X,Yが同値の場合は並びの早いほう
			for( j = 0; j < a_lPtsCount; j++ )
			{
				IPointPtr	a_ipCurPt;
				PtNum		a_pTmp;
				a_ipPtsColl->get_Point ( j, &a_ipCurPt );
				// 並べ替え用の情報を取得
				a_pTmp.m_lNum	= j;
				a_ipCurPt->get_X ( &a_pTmp.m_dX );
				a_ipCurPt->get_Y ( &a_pTmp.m_dY );
				if( j == 0 ) {
					a_vpMinPoint	= a_pTmp;
				}
				if( a_pTmp < a_vpMinPoint ) {
					a_vpMinPoint	= a_pTmp;
				}
				a_vpPtNum.push_back ( a_pTmp );
			}
			if( a_eGeoType == esriGeometryPolygon ) {
				// ジオメトリタイプがPolygonの場合、最後の点を一度削除
				a_vpPtNum.pop_back ();

				// 並べ替え処理
				for( j = 0; j < a_vpMinPoint.m_lNum; j++ )
				{
					a_vpPtNum.push_back ( *a_vpPtNum.begin() );	// 始点を追加
					a_vpPtNum.erase ( a_vpPtNum.begin() );		// 要素の最後に追加した始点は削除
				}
				// ジオメトリタイプがPolygonの場合、最後の点に始点と同じ点を追加(PtNumの点列の番号はこのあとでは意味がないので無視）
				a_vpPtNum.push_back (a_vpMinPoint);
			}else {
				// 始点と終点の位置を比較
				if( a_vpPtNum.back() < *a_vpPtNum.begin() ) {
					// 点列を逆にする
					a_vpPtNum.reverse();
				}
			}
			// 点列を更新する
			std::list<PtNum>::iterator	a_iIndx;
			for( j = 0, a_iIndx = a_vpPtNum.begin(); j < a_lPtsCount; j++, a_iIndx++ )
			{
				IPointPtr	a_ipCurPt;
				PtNum		a_pTmp;
				a_ipPtsColl->get_Point ( j, &a_ipCurPt );
				// 並べ替え用の情報を取得
				a_pTmp.m_lNum	= j;
				a_ipCurPt->put_X ( (*a_iIndx).m_dX );
				a_ipCurPt->put_Y ( (*a_iIndx).m_dY );
				a_ipPtsColl->UpdatePoint ( j, a_ipCurPt );	// 更新
			}
		}
	}
	catch (...) {
		fprintf	( stderr, "例外発生　PointRotate::ExecRotate" );
		return	( NULL );
	}
	return	( a_ipRetGeo );
}
