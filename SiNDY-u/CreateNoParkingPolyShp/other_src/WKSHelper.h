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
* @file WKSHelper.h
*
* @brief WKSPointを使用したジオメトリ系関数定義ファイル
*/
#if !defined( __WKSHELPER_H__)
#define __WKSHELPER_H__	//!< 重複インクルード防止
#include <stdarg.h>
//#include "define.h"
#include <list>
using namespace std;

// ヘッダでいろいろ問題が発生しているので、ローカルに持ってきたもの -------------------------------
const static DOUBLE Pai = 3.1415926535897932384626433832795;
// ジオメトリ比較演算用
enum sindyeSpatialRel
{
	sindyeSpatialRelNone		= 0,	//!< 関連性なし
	sindyeSpatialRelTouch		= 1,	//!< 接触
	sindyeSpatialRelOverlap		= 2,	//!< オーバーラップ
	sindyeSpatialRelCross		= 4,	//!< 交差
	sindyeSpatialRelWithin		= 8,	//!< 含まれる
	sindyeSpatialRelContain		= 16,	//!< 含んでいる
	sindyeSpatialRelDisjoint	= 32,	//!< 分離
	sindyeSpatialRelEqual		= 64,	//!< 等しい
};

// <ここまで> ---------------------------------------------------------------------------------


struct SIMPLERING {
	LONG INDX;	// 2003.10.02
	LONG START;
	LONG END;
};

struct RING {
	LONG NUM;
	LONG START;
	LONG END;
	LONG XMIN;
	BOOL IsClockwise;
	list<SIMPLERING> Contain;	// このリングが含んでいるリングの番号リスト
	list<SIMPLERING> Within;	// このリングが含まれているリングの番号のリスト
	list<SIMPLERING> Cross;		// このリングとクロスしているリングの番号のリスト
	list<SIMPLERING> Disjoint;	// このリングと関連のないリングの番号のリスト
};

struct SEGMENT {
	LONG p1;
	LONG p2;
	BOOL IsReverse;	//!< 点列の向きがp2->p1の場合はTRUE
};

enum sindyeChkInOut {
	sindyeChkInOutIn,
	sindyeChkInOutOut,
	sindyeChkInOutSame,
};

/**
 * @brief 3点の内積を求める
 *
 * @note 点の向きは 1 -> 2 -> 3
 *
 * @return 内積
 */
DOUBLE innerproduct( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 );

/**
 * @brief 2点の長さを求める
 */
DOUBLE length( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 );

/**
 * @brief 2点からなる線分の角度を求める
 *
 * @note http://www.inet-lab.org/ted/program/prog020.html#020.9 を参照
 *
 * @param x1	[in]	DOUBLE
 * @param y1	[in]	DOUBLE
 * @param x2	[in]	DOUBLE
 * @param y2	[in]	DOUBLE
 *
 * @retval 角度（0～360度）
 */
double angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2 );

/**
 * @brief 3点からなる線分の角度を求める
 *
 * @param x1	[in]	DOUBLE
 * @param y1	[in]	DOUBLE
 * @param x2	[in]	DOUBLE
 * @param y2	[in]	DOUBLE
 * @param x3	[in]	DOUBLE
 * @param y3	[in]	DOUBLE
 *
 * @retval 角度（-180～180度）
 */
DOUBLE angle( DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE x3, DOUBLE y3 );

/**
 * @brief 点をある点を中心に回転させる
 *
 * @note http://www.inet-lab.org/ted/program/prog016.html#016.1 を参照
 *
 * @param xc	[in]	回転の中心X座標
 * @param yc	[in]	回転の中心Y座標
 * @param rot	[in]	回転角（ラジアン）
 * @param xsrc	[in]	回転させる点のX座標
 * @param ysrc	[in]	回転させる点のY座標
 * @param xret	[out]	回転後の点のX座標
 * @param yret	[out]	回転後の点のY座標
 */
void rotation( DOUBLE xc, DOUBLE yc, DOUBLE rot, DOUBLE xsrc, DOUBLE ysrc, DOUBLE* xret, DOUBLE* yret );

/**
 * @brief 角度（0～360）からラジアンへ変換する
 *
 * @param angle		[in]	角度（0～360）
 *
 * @return ラジアン（DOUBLE）
 */
DOUBLE angle2rad( DOUBLE angle );

/**
 * @brief ラジアンから角度（0～360）へ変換する
 *
 * @param rad		[in]	ラジアン
 *
 * @return 角度（0～360）（DOUBLE）
 */
DOUBLE rad2angle( DOUBLE rad );

/**
 * @brief 指定されたリングが時計回りかどうか調べる
 *
 * @note http://www.inet-lab.org/ted/program/prog032.html を参照
 *
 * @param ring		[in,out]	リングの開始インデックス
 * @param lEnd		[in]		リングの終了インデックス
 * @param pPoints	[in]		ポリゴン点列
 *
 * @retval TRUE 時計回り
 * @retval FALSE 半時計回り
 */
BOOL IsClockwise( RING& ring, WKSPoint*& pPoints );

/**
 * @brief 点 test は base のリングに内包されるか
 *
 * @param base		[in]	RING
 * @param test		[in]	検査する点のインデックス
 * @param pPoints	[in]	点列
 *
 * @see http://www.inet-lab.org/ted/program/prog033.html
 *
 * @retval sindyuChkInOutIn 内包される
 * @retval sindyuChkInOutOut 内包されない
 * @retval sindyuChkInOutSame 同一点がある
 */
sindyeChkInOut inside( const RING& base, LONG test, const WKSPoint*& pPoints );

/**
 * @brief baseとtestのリングに対してジオメトリの関係を調べる
 *
 * @note within、containについて調べます
 * @note
 * @note baseがtestに含まれる場合はwithinを返します
 * @note baseがtestを含む場合はcontainを返します
 * @note どちらもどちらに含まれない場合はnoneを返します
 * @note 
 * @note スピード重視のため、overlapしていてもnoneとなりますが、
 * @note このプログラム上では問題ありません。他のアルゴリズムに適用
 * @note する場合は注意して下さい。
 *
 * @param base		[in]	RING
 * @param test		[in]	RING
 * @param pPoints	[in]	ポイント列
 *
 * @retval sindyeSpatialRelWithin baseがtestに含まれる
 * @retval sindyeSpatialRelContain baseがtestを含む
 * @retval sindyeSpatialRelNone baseとtestの関連性不明
 */
sindyeSpatialRel TestRelationship( const RING& base, const RING& test, WKSPoint*& pPoints, WKSPoint& errPoint );

/**
 * @brief WKSPoint -> RING コンバータ
 *
 * @param lPointCount	[in]	総ポイント数
 * @param pPoints		[in]	ポイント配列
 */
void InitRing( LONG lPointCount, WKSPoint*& pPoints, list<RING>& listRings );

/**
 * @brief リングの包含関係をチェックする
 *
 * @note InitRing を先に呼ぶ必要があります
 *
 * @param pPoints		[in]	ポイント配列
 * @param listRings		[in]	初期化済みのリングリスト
 */
void CheckExteriorInterior( WKSPoint*& pPoints, list<RING>& listRings );

#endif // if !defined( __WKSHELPER_H__)
