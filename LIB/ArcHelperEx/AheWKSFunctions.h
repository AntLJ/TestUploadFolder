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
 * @file WKSFunctions.h
 * @brief <b>WKSPointを使用した演算関数定義ファイル</b>\n
 * このファイルは、WKSPointを使用した演算のための関数が定義されています。
 * IGeometryを始めとするCOMインターフェースでの演算に比べ、高速に動作
 * する事ができます。
 * WKSPointを取得するには、IPointCollection::QueryWKSPointを使用します。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _WKSFUNCTIONS_H_
#define _WKSFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>二点の距離を求める</b>
 * @return 距離
 */
double WKS2PLength( const WKSPoint& p1, const WKSPoint& p2 );

/**
 * @brief <b>三点の内積を求める</b>
 * @return 内積
 */
double WKS3PInnerproduct( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 );

/**
 * @brief <b>三点のなす角を求める</b>
 * @param p1		[in]			ポイント1
 * @param p2		[in]			ポイント2
 * @param p3		[in]			ポイント3
 * @return 角度（ラジアン）
 */
double WKS3PAngle( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 );

/**
 * @brief <b>ベクトル同士の交差判定を行う</b>\n
 * 交差判定後に交点を求めたい場合は、dV1t、dV2tを使用してください。\n
 * なお、平行の場合にはdV1t、dV2tは不定となるので、使用する際は
 * 必ずbParallelをチェックしてください。\n
 * 端点同士が接している場合は、交差と判定しますが、dV1t、dV2tの値で
 * 接触か交差かを判断することができます。交差と判断したくない場合は、
 * bTouchIsCrossにfalseをセットしてください。
 * @param pV1From		[in]			ベクトル1のFROMポイント
 * @param pV1To			[in]			ベクトル1のTOポイント
 * @param pV2From		[in]			ベクトル2のFROMポイント
 * @param pV2To			[in]			ベクトル2のTOポイント
 * @param bTouchIsCross	[in,optional]	接している場合をtrueで返す場合はtrue（デフォルト：true）
 * @param bParallel		[out,optional]	平行の場合はtrueがかえる（デフォルト：NULL）
 * @param dV1t			[out,optional]	ベクトル1の分割比（デフォルト：NULL）
 * @param dV2t			[out,optional]	ベクトル2の分割比（デフォルト：NULL）
 * @retval true 交差している
 * @retval false 交差しない
 */
bool WKSIsCross( const WKSPoint& pV1From, const WKSPoint& pV1To, const WKSPoint& pV2From, const WKSPoint& pV2To, bool bTouchIsCross = true, bool* bParallel = NULL, double* dV1t = NULL, double* dV2t = NULL );

/**
 * @brief <b>矩形同士の交差判定を行う</b>\n
 * 片方が片方に内包・外包される場合や、接している場合も交差と判断します。
 * @param rect1			[in]			判定矩形１
 * @param rect2			[in]			判定矩形２
 * @retval true 交差している
 * @retval false 交差していない
 */
bool WKSIsCross( const WKSEnvelope& rect1, const WKSEnvelope& rect2 );

#endif // _WKSFUNCTIONS_H_
