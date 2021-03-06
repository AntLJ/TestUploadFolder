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
* @file WKSHelper.cpp
*
* @brief WKSPointを使用したジオメトリ系関数実装ファイル
*/
#include "stdafx.h"
#include "WKSHelper.h"
#include "../../LIB/WinLib/math2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// 3点の内積を求める
DOUBLE innerproduct( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 )
{
	return ( x1 - x2 ) * ( x3 - x2 ) + ( y1 - y2 ) * ( y3 - y2 );
}

/// 2点の長さを求める
DOUBLE length( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 )
{
	DOUBLE dx = x2 - x1;
	DOUBLE dy = y2 - y1;

	return sqrt( dx*dx + dy*dy );
}

/// 2点からなる線分の角度を求める
double angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 )
{
	DOUBLE dx,dy,t,a;
	INT    area;

	dx=x2-x1;
	dy=y2-y1;

	if(dx>=0.0)    {
		if(dy>=0.0)    { area=0;                         }  // 第１象限
		else           { area=3; t=dx; dx= -dy; dy=   t; }  // 第４象限
	}
	else{
		if(dy>=0.0)    { area=1; t=dx; dx=  dy; dy= - t; }  // 第２象限
		else           { area=2;       dx= -dx; dy= -dy; }  // 第３象限
	}

	if(dy>dx)a=M_PI/(DOUBLE)2.0-atan(dx/dy);		// ４５度以上
		else a=                       atan(dy/dx);		// ４５度以下

	return a+(DOUBLE)area*(M_PI/(DOUBLE)2.0);
}

/// 3点からなる線分の角度を求める
DOUBLE angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 )
{
	DOUBLE dInnerProduct = innerproduct( x1, y1, x2, y2, x3, y3 );
	DOUBLE d = length( x1, y1, x2, y2 ) * length( x2, y2, x3, y3 );

	return ( acos( dInnerProduct / d ) * (DOUBLE)180.0 ) / M_PI;
}

/// 点をある点を中心に回転させる
void rotation( DOUBLE xc, DOUBLE yc, DOUBLE rot, DOUBLE xsrc, DOUBLE ysrc, DOUBLE* xret, DOUBLE* yret )
{
	double cc,ss;

	cc=cos(-rot);
	ss=sin(-rot);

	xsrc-=xc;
	ysrc-=yc;

	*xret=  xsrc*cc+ysrc*ss+xc;
	*yret= -xsrc*ss+ysrc*cc+yc;
}

/*/// 角度（0〜360）からラジアンへ変換する
DOUBLE angle2rad( DOUBLE angle ){ return ( angle * M_PI ) / (DOUBLE)180.0; }

/// ラジアンから角度（0〜360）へ変換する
DOUBLE rad2angle( DOUBLE rad ){ return ( rad * (DOUBLE)180 ) / M_PI; }*/

/// 指定されたリングが時計回りかどうか調べる
BOOL IsClockwise( RING& ring, WKSPoint*& pPoints )
{
	LONG prev, curr = ring.XMIN, next;
	LONG count = ring.END - ring.START;

	prev = ( curr == ring.START   ? ring.END - 1 : curr - 1 );
	next = ( curr == ring.END - 1 ? ring.START   : curr + 1 );

	// prev の X座標と curr のX座標が同じなら、prev.Y < curr.Y なら右回り。next はその逆
	// prev、curr、next のX座標がすべて同じになることはアルゴリズム上ありえない
	if( pPoints[prev].X == pPoints[curr].X ) 
	{
		if( pPoints[prev].Y < pPoints[curr].Y )
		{
			//ring.IsClockwise = TRUE;
			return TRUE;
		}
		else {
			//ring.IsClockwise = FALSE;
			return FALSE;
		}
	}
	if( pPoints[next].X == pPoints[curr].X )
	{
		if( pPoints[next].Y > pPoints[curr].Y )
		{
			//ring.IsClockwise = TRUE;
			return TRUE;
		}
		else {
			//ring.IsClockwise = FALSE;
			return FALSE;
		}
	}

	// prev - curr - next の角度を求める
	DOUBLE dAngle1 = angle( pPoints[prev].X, pPoints[prev].Y, pPoints[curr].X, pPoints[curr].Y );
	DOUBLE dAngle2 = angle( pPoints[curr].X, pPoints[curr].Y, pPoints[next].X, pPoints[next].Y );
	DOUBLE dAngle3 = rad2angle( dAngle1 - dAngle2 );
	DOUBLE dAngle;
	
	if( -180 < dAngle3 && dAngle3 < 0 ) dAngle = dAngle3;
	else if( -360 < dAngle3 && dAngle3 < -180 ) dAngle = - ( dAngle3 + (DOUBLE)180.0 );
	else dAngle = (DOUBLE)180.0 - dAngle3;

	if( dAngle < 0 )
	{
		//ring.IsClockwise = FALSE;
		return FALSE;
	}
	else {
		//ring.IsClockwise = TRUE;
		return TRUE;
	}
}

/// 点 test は base のリングに内包されるか
sindyeChkInOut inside( LONG START, LONG END, const DOUBLE& tX, const DOUBLE& tY, const WKSPoint*& pPoints )
{
	LONG lCrossCount = 0;

	for( LONG i = START; i < END; i++ )
	{
		LONG nexti = i + 1;
		LONG previ = ( i == START ? END - 1 : i - 1 );

		// 同一点の場合は絶対にありえない
		if( pPoints[i].X == tX && pPoints[i].Y == tY )
		{
			// リング同士で同一の点がある
			return sindyeChkInOutSame;
		}

		// i 又は nexti のY座標が tY より上で、且つ i - nexti が tX を跨いでいるときのみ演算
		if( pPoints[i].Y >= tY || pPoints[nexti].Y >= tY )
		{
			// i と test のX座標が同じ場合
			if( pPoints[i].X == tX )
			{
				// previ、i、nexti、tX が全て同じ X座標ならカウントしない
				if( pPoints[previ].X == tX && pPoints[i].X == tX && pPoints[nexti].X == tX )
					continue;
				// パターン：1 previ、nexti が tX を跨いでいる時カウント
				else if( ( ( pPoints[previ].X < tX && tX <= pPoints[nexti].X ) || ( pPoints[nexti].X < tX && tX <= pPoints[previ].X ) ) && tY < pPoints[i].Y ) 
					lCrossCount++;
				// パターン：4 nexti、previ どちらの X座標も tX と違う場合はカウントしない
				else if( pPoints[nexti].X != tX && pPoints[previ].X != tX )
					continue;
			}
			// それ以外
			else if( ( pPoints[i].X < tX && tX < pPoints[nexti].X ) || ( pPoints[nexti].X < tX && tX < pPoints[i].X ) ) 
			{
				// 交点が tY より上ならカウント
				DOUBLE crossY = (( tX - pPoints[i].X ) * ( pPoints[nexti].Y - pPoints[i].Y ))/( pPoints[nexti].X - pPoints[i].X ) + pPoints[i].Y;
				if( crossY > tY )
					lCrossCount++;
			}
		}
	}
	// カウントが奇数なら内包
	return ( ( lCrossCount % 2 ) == 1 ? sindyeChkInOutIn : sindyeChkInOutOut );
}

/// 点 test は base のリングに内包されるか
sindyeChkInOut inside( const RING& base, const DOUBLE& tX, const DOUBLE& tY, const WKSPoint*& pPoints )
{
	return inside( base.START, base.END, tX, tY, pPoints );
}

/// 点 test は base のリングに内包されるか
sindyeChkInOut inside( const RING& base, LONG test, const WKSPoint*& pPoints )
{
	return inside( base, pPoints[test].X, pPoints[test].Y, pPoints );
}

/// baseとtestのリングに対してジオメトリの関係を調べる
sindyeSpatialRel TestRelationship( const RING& base, const RING& test, WKSPoint*& pPoints, WKSPoint& errPoint )
{
	errPoint.X = -1; errPoint.Y = -1;	// 初期化
	LONG lIn1 = 0, lOut1 = 0;
	LONG lIn2 = 0, lOut2 = 0;

	for( LONG i = base.START; i < base.END; i++ )
	{
		switch( inside( test, i, (const WKSPoint*&)pPoints ) )
		{
			case sindyeChkInOutIn:
				lIn1++;
				break;
			case sindyeChkInOutOut:
				lOut1++;
				break;
			case sindyeChkInOutSame:
				errPoint = pPoints[i];
				return sindyeSpatialRelCross;
				break;
			default:
				break;
		}
		if( lOut1 > 0 && lIn1 > 0 ) 
		{
			errPoint = pPoints[i];
			return sindyeSpatialRelCross;
		}
	}

	for( LONG i = test.START; i < test.END; i++ )
	{
		switch( inside( base, i, (const WKSPoint*&)pPoints ) )
		{
			case sindyeChkInOutIn:
				lIn2++;
				break;
			case sindyeChkInOutOut:
				lOut2++;
				break;
			case sindyeChkInOutSame:
				errPoint = pPoints[i];
				return sindyeSpatialRelCross;
				break;
			default:
				break;
		}
		if( lOut2 > 0 && lIn2 > 0 ) 
		{
			errPoint = pPoints[i];
			return sindyeSpatialRelCross;
		}
	}

	if( lIn1 > 0 && lOut1 == 0 && lIn2 == 0 && lOut2 > 0 ) 
		return sindyeSpatialRelWithin;
	else if( lIn1 == 0 && lOut1 > 0 && lIn2 > 0 && lOut2 == 0 ) 
		return sindyeSpatialRelContain;
	else 
		return sindyeSpatialRelNone;
}
/// WKSPoint -> RING コンバータ
BOOL InitRing( LONG lPointCount, WKSPoint*& pPoints, std::list<RING>& listRings )
{
	BOOL bRet = TRUE;
	if( lPointCount < 3 )
	{
		//ポイント数が3未満はポリゴンじゃない
		_ASSERTE( lPointCount > 2 );
		return	false;
	}

	// 各リングの開始・終了及び、Xが最大の時のインデックスを記憶
	std::list<RING> listInRing;
	RING stRing;
	LONG lCount = -1;

	stRing.START = stRing.END = stRing.XMIN = stRing.NUM = -1;	// 初期化
	for( LONG i = 0; i < lPointCount; i++ )
	{
		// 最大・最小を記憶
		if( stRing.START == -1 )
		{
			stRing.xmin = stRing.xmax = pPoints[i].X;
			stRing.ymin = stRing.ymax = pPoints[i].Y;
		}
		else {
			if(      pPoints[i].X < stRing.xmin ) stRing.xmin = pPoints[i].X;
			else if( pPoints[i].X > stRing.xmax ) stRing.xmax = pPoints[i].X;
			if(      pPoints[i].Y < stRing.ymin ) stRing.ymin = pPoints[i].Y;
			else if( pPoints[i].Y > stRing.ymax ) stRing.ymax = pPoints[i].Y;
		}

		if( stRing.START == -1 )
		{
			stRing.START = i;
			stRing.XMIN  = i;
			stRing.NUM   = (++lCount);
		}
		else if( pPoints[i].X == pPoints[stRing.START].X && pPoints[i].Y == pPoints[stRing.START].Y )
		{
			// リングの終わり

			stRing.END = i;
			// 時計回りならExterior、そうでなければInteriorとする
			stRing.IsClockwise = IsClockwise( stRing, pPoints );
			listRings.push_back( stRing );

			stRing.START = stRing.END = stRing.XMIN = stRing.NUM = -1;	// 初期化
		}
		// X座標が同一な場合はY座標が小さいほうを選択する
		else if( pPoints[stRing.XMIN].X > pPoints[i].X )
			stRing.XMIN = i;
		else if( pPoints[stRing.XMIN].X == pPoints[i].X && pPoints[stRing.XMIN].Y > pPoints[i].Y )
			stRing.XMIN = i;
	}
	// 2010.09.07 終点が見つからない場合は、falseを返して抜ける
	if( stRing.START != -1 ) {
		return	false;
	}
	return	true;
}

//	自己接触問題修正版InitRing add by kudo
BOOL InitRing2( LONG lPointCount, IGeometryPtr& ipGeom, WKSPoint*& pPoints, std::list<RING>& listRings )
{
	BOOL bRet = TRUE;
	if( lPointCount < 3 )
	{
		//ポイント数が3未満はポリゴンじゃない
		_ASSERTE( lPointCount > 2 );
		return	false;
	}

	if( ipGeom == NULL )
		return	false;

	// リングの開始位置、終了位置が座標一致だけでは限界があるので、IGeometryCollectionを使用する
	LONG	a_lGeomCount	= 0;
	IGeometryCollectionPtr	a_ipGeomColl(ipGeom);

	if( a_ipGeomColl == NULL && ipGeom != NULL ) {
		a_lGeomCount	= 1;
	}else {
		a_ipGeomColl->get_GeometryCount ( &a_lGeomCount );
	}

	// 各リングの開始・終了及び、Xが最大の時のインデックスを記憶
	std::list<RING> listInRing;
	RING stRing;
	LONG lCount = -1;

	LONG i = 0;
	for ( LONG roop = 0; roop < a_lGeomCount; roop++ )
	{
		IGeometryPtr		a_ipCurGeom;
		if( a_lGeomCount == 1 ) {
			// IGeometryCollectionに対応していないジオメトリタイプの場合を吸収
			a_ipCurGeom	= ipGeom;
		}else {
			a_ipGeomColl->get_Geometry ( roop, &a_ipCurGeom );
		}

		IPointCollectionPtr	a_ipCurPtColl(a_ipCurGeom);

		LONG	a_lPointCount	= 0;	// 補間点数
		a_ipCurPtColl->get_PointCount ( &a_lPointCount );

		stRing.START= i;
		stRing.END	= i+(a_lPointCount-1);
		stRing.XMIN	= i;
		stRing.NUM	= roop;
		for ( LONG indx = 0; indx < a_lPointCount; indx++, i++ )
		{
			if( pPoints[stRing.XMIN].X > pPoints[i].X )
				stRing.XMIN = i;
			else if( pPoints[stRing.XMIN].X == pPoints[i].X && pPoints[stRing.XMIN].Y > pPoints[i].Y )
				stRing.XMIN = i;
		}
		// 時計回りならExterior、そうでなければInteriorとする
		stRing.IsClockwise = IsClockwise( stRing, pPoints );
		listRings.push_back( stRing );
	}
	return	true;
}

/// リングの包含関係をチェックする
void CheckExteriorInterior( WKSPoint*& pPoints, std::list<RING>& listRings )
{
	// 全てのリングに対して、包含関係をチェック
	LONG	a_iIndx1	= 0;	// 2003.11.10
	for( std::list<RING>::iterator itRing1 = listRings.begin(); itRing1 != listRings.end(); itRing1++, a_iIndx1++ )
	{
		LONG	a_iIndx2	= a_iIndx1;
		WKSPoint errPoint;
		std::list<RING>::iterator itRing2 = itRing1;
		advance( itRing2, 1 );
		a_iIndx2++;
		for( ; itRing2 != listRings.end(); itRing2++, a_iIndx2++ )
		{
			sindyeSpatialRel rel = TestRelationship( *itRing1, *itRing2, pPoints, errPoint );
			SIMPLERING ring1, ring2;
			ring1.INDX	= a_iIndx1;	// 2003.11.10
			ring1.START = itRing1->START;
			ring1.END   = itRing1->END;
			ring1.ERRPT	= errPoint;	// 2004.01.13
			ring2.INDX	= a_iIndx2;	// 2003.11.10
			ring2.START = itRing2->START;
			ring2.END   = itRing2->END;
			ring2.ERRPT	= errPoint;	// 2004.01.13
			switch( rel )
			{
				case sindyeSpatialRelWithin:
					itRing1->Within.push_back( ring2 );
					itRing2->Contain.push_back( ring1 );
					break;
				case sindyeSpatialRelContain:
					itRing1->Contain.push_back( ring2 );
					itRing2->Within.push_back( ring1 );
					break;
				case sindyeSpatialRelCross:
					itRing1->Cross.push_back( ring2 );
					itRing2->Cross.push_back( ring1 );
					break;
				default: 
					itRing1->Disjoint.push_back( ring2 );
					itRing2->Disjoint.push_back( ring1 );
					break;
			}
		}
	}
#ifdef DEBUG
	// チェック
	for( std::list<RING>::iterator itRing1 = listRings.begin(); itRing1 != listRings.end(); itRing1++ )
	{
		// 他のリングを含んでいるのに他のリングに含まれてはいけない
		if( itRing1->Contain.size() > 0 && itRing1->Within.size() > 0 )
			_ASSERTE( ! ( itRing1->Contain.size() > 0 && itRing1->Within.size() > 0 ) );
		// 他のリングとクロスしてはいけない
		if( itRing1->Cross.size() > 0 )
			_ASSERTE( itRing1->Cross.size() < 1 );
		// 左回りなのに他のリングを含んではいけない
		if( itRing1->IsClockwise == FALSE && itRing1->Contain.size() > 0 )
			_ASSERTE( ! ( itRing1->IsClockwise == FALSE && itRing1->Contain.size() > 0 ) );
		// 右回りなのに他のリングに含まれてはいけない
		if( itRing1->IsClockwise == TRUE && itRing1->Within.size() > 0 )
			_ASSERTE( ! ( itRing1->IsClockwise == TRUE && itRing1->Within.size() > 0 ) );
		// 左回りなのにどのリングにも含まれていないのはいけない
		if( itRing1->IsClockwise == FALSE && itRing1->Within.size() < 1 )
			_ASSERTE( ! ( itRing1->IsClockwise == FALSE && itRing1->Within.size() < 1 ) );
	}
#endif
}
