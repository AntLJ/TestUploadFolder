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
#include <ArcHelperEx/GlobalFunctions.h>

// testGeomがbaseGeomに構成点一致で含まれるかどうか確認する
bool isContainAsSegment( IGeometryPtr baseGeom, IGeometryPtr testGeom )
{
	if( !( baseGeom && testGeom ) )
		return false;

	esriGeometryType baseType = esriGeometryAny;
	esriGeometryType testType = esriGeometryAny;
	baseGeom->get_GeometryType( &baseType );
	testGeom->get_GeometryType( &testType );

	// ポリゴンとポリラインの判定のみサポート
	if( ! ( esriGeometryPolygon == baseType && esriGeometryPolyline == testType ) )
		return false;

	// マルチパートの場合はそれぞれのパートで判断する
	long geomCount = 0;
	IGeometryCollectionPtr(testGeom)->get_GeometryCount(&geomCount);
	if( 0 >= geomCount )
		return false;

	for( long i = 0; i < geomCount; ++i )
	{
		IGeometryPtr testPartGeom;
		IGeometryCollectionPtr(testGeom)->get_Geometry(i, &testPartGeom);

		// testの構成点とbaseの構成点が順番に並んでいるかどうか確認する
		IHitTestPtr hittest( baseGeom );
		IPointCollectionPtr col(testPartGeom);
		long count = 0;
		col->get_PointCount(&count);
		if( 0 >= count )
			return false;

		IPointPtr hitPoint;
		long prevHitIndex = 0;
		for( long i = 0; i < count; ++i )
		{
			IPointPtr point;
			col->get_Point( i, &point );
			if( !hitPoint )
				hitPoint = AheCreateClone(point);
			double dist = 0;
			long partIndex = 0;
			long hitIndex = 0;
			VARIANT_BOOL isRightSide = VARIANT_FALSE;
			VARIANT_BOOL hit = VARIANT_FALSE;
			// 距離0でヒットするか検査
			hittest->HitTest( point, 0.0, esriGeometryPartVertex, hitPoint, &dist, &partIndex, &hitIndex, &isRightSide, &hit );
			if( !hit )
				return false;

			// ヒットしたパートの構成点数を取得
			IGeometryPtr partGeom;
			IGeometryCollectionPtr(baseGeom)->get_Geometry( partIndex, &partGeom );
			long partPointCount = 0;
			IPointCollectionPtr(partGeom)->get_PointCount( &partPointCount );

			// ヒットした構成点が、前回ヒットした構成点の直前直後かどうか確認
			if( 0 != i && 
				! ( 
				1 == abs( prevHitIndex - hitIndex )  || // ヒット位置が隣同士
				// ヒット位置が始終点を跨ぐ場合
				( 0 == hitIndex     && partPointCount -2 == prevHitIndex ) || // 始点と終点-1
				( 0 == prevHitIndex && partPointCount -2 == hitIndex     ) || // 始点と終点-1
				( 1 == hitIndex     && partPointCount -1 == prevHitIndex ) || // 終点と始点+1
				( 1 == prevHitIndex && partPointCount -1 == hitIndex     )    // 始点と終点+1
				))
				return false;

			prevHitIndex = hitIndex;
		}
	}
	return true;
}

// testGeomがbaseGeomに構成点一致で含まれるかどうか確認する
bool isContainAsVertex( IGeometryPtr baseGeom, IGeometryPtr testGeom )
{
	if( !( baseGeom && testGeom ) )
		return false;

	esriGeometryType baseType = esriGeometryAny;
	esriGeometryType testType = esriGeometryAny;
	baseGeom->get_GeometryType( &baseType );
	testGeom->get_GeometryType( &testType );

	// ポリラインとマルチポイントの判定のみサポート
	if( ! ( esriGeometryPolyline == baseType && esriGeometryMultipoint == testType ) )
		return false;

	// マルチパートの場合はそれぞれのパートで判断する
	long geomCount = 0;
	IGeometryCollectionPtr(testGeom)->get_GeometryCount(&geomCount);
	if( 0 >= geomCount )
		return false;

	for( long i = 0; i < geomCount; ++i )
	{
		IGeometryPtr testPartGeom;
		IGeometryCollectionPtr(testGeom)->get_Geometry(i, &testPartGeom);

		// testの構成点とbaseの構成点が順番に並んでいるかどうか確認する
		IHitTestPtr hittest( baseGeom );
		IPointPtr hitPoint;
		long prevHitIndex = 0;

		IPointPtr point(testPartGeom);
		if( !hitPoint )
			hitPoint = AheCreateClone(point);
		double dist = 0;
		long partIndex = 0;
		long hitIndex = 0;
		VARIANT_BOOL isRightSide = VARIANT_FALSE;
		VARIANT_BOOL hit = VARIANT_FALSE;
		// 距離0でヒットするか検査
		hittest->HitTest( point, 0.0, esriGeometryPartVertex, hitPoint, &dist, &partIndex, &hitIndex, &isRightSide, &hit );
		// 折り返し形状は想定しない（この処理以前に潰されているべき）
		// 単純に構成点にヒットしたらマージ対象とする
		if( hit )
			return true;
	}
	return false;
}
