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
 * @file WKSFunctions.cpp
 * @brief <b>WKSPointを使用した演算関数実装ファイル</b>\n
 * このファイルは、WKSPointを使用した演算のための関数が実装されています。
 * IGeometryを始めとするCOMインターフェースでの演算に比べ、高速に動作
 * する事ができます。
 * WKSPointを取得するには、IPointCollection::QueryWKSPointを使用します。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "AheWKSFunctions.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 二点の距離を求める
double WKS2PLength( const WKSPoint& p1, const WKSPoint& p2 )
{
	double dx = p2.X-p1.X;
	double dy = p2.Y-p1.Y;
	return sqrt(dx*dx+dy*dy);
}

// 三点の内積を求める
double WKS3PInnerproduct( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 )
{
	return (p1.X-p2.X)*(p3.X-p2.X)+(p1.Y-p2.Y)*(p3.Y-p2.Y);
}

// 三点のなす角を求める
double WKS3PAngle( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 )
{
	// 内積 / ( 辺１長さ x 辺２長さ )がとる値は -1 ～ 1であり、これが -180° ～ 180° の範囲に相当する
	// つまり、1°は 0.00555…なので、小数点以下5桁もあればいいことになる
	// そこで、計算誤差により -1 ～ 1 の範囲を出ないようにするために一度丸めるために float に落とす
	// これにより、ほぼ 180°や360°の場合に計算誤差で acos が失敗することはなくなるはず
	return acos((double)(float)(WKS3PInnerproduct(p1,p2,p3)/(WKS2PLength(p1,p2)*WKS2PLength(p2,p3))));
}

// ベクトル同士の交差判定を行う
bool WKSIsCross( const WKSPoint& pV1From, const WKSPoint& pV1To, const WKSPoint& pV2From, const WKSPoint& pV2To, bool bTouchIsCross/* = true*/, bool* bParallel/* = NULL*/, double* dV1t/* = NULL*/, double* dV2t/* = NULL*/ )
{
	bool bRet = false;	// 返り値
	if( bParallel ) *bParallel = false;

	double Ax = pV1To.X - pV1From.X;
	double Ay = pV1To.Y - pV1From.Y;
	double Bx = pV2To.X - pV2From.X;
	double By = pV2To.Y - pV2From.Y;
	double DivideBase = Bx*Ay - By*Ax;
	
	// 平行判定:本来ならしきい値を設けるべきだが、面倒なので0で判定
	if( 0.0f != DivideBase )
	{
		double t = (Ax*(pV2From.Y-pV1From.Y)-Ay*(pV2From.X-pV1From.X))/DivideBase;
		double s = (Bx*(pV2From.Y-pV1From.Y)-By*(pV2From.X-pV1From.X))/DivideBase;

		if( dV1t ) *dV1t = t;
		if( dV2t ) *dV2t = s;

		// 交差又は接触
		if( 0.0f <= t && t <= 1.0f && 
			0.0f <= s && s <= 1.0f )
		{
			if( bTouchIsCross )
				bRet = true;
			else {
				// 接触を交差としたくない場合
				if( 0.0f < t && t < 1.0f &&
					0.0f < s && s < 1.0f )
					bRet = true;
			}
		}
	}
	// 平行
	else {
		if( bParallel ) *bParallel = true;
	}

	return bRet;
}

// 矩形同士の交差判定を行う
bool WKSIsCross( const WKSEnvelope& rect1, const WKSEnvelope& rect2 )
{
	return ( ( rect1.XMin > rect2.XMax ) ||
			 ( rect1.XMax < rect2.XMin ) ||
			 ( rect1.YMin > rect2.YMax ) ||
			 ( rect1.YMax < rect2.YMin ) ) ? false : true;
}
