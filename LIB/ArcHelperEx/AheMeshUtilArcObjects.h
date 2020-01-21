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


#pragma once


/**
 * @brief メッシュコードから矩形に変換する
 * @see MESH2LL
 * @param lMeshCode	[in]	メッシュコード
 * @param ipSpRef	[in]	出力矩形に適用する空間参照
 * @return IEnvelopePtr
 */
IEnvelopePtr AheMESH2ENV( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief メッシュコードからポリゴンに変換する
 * @see MESH2LL
 * @param lMeshCode	[in]	メッシュコード
 * @param ipSpRef	[in]	出力矩形に適用する空間参照
 * @return IGeometryPtr
 */
IGeometryPtr AheMESH2POLY( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief Pointからメッシュコード＋正規化XYに変換する
 * @see LL2MESH
 * @param Point	[in]	ポイント
 * @param x		[out]	正規化座標X
 * @param y		[out]	正規化座標Y
 * @param no_mill	[in,optional]	正規化座標1000000を許容する場合は0（デフォルト：0）
 * @return メッシュコード
 */
unsigned int AhePoint2MESH( IPoint* ipPoint, int level, int& x, int& y, int no_mill = 0 );
