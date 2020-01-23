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
 * @file meshutil.h
 * @brief メッシュ操作用外部関数定義ファイル\n
 * このファイルはcrd_cnvのラップ関数群の定義がかかれています。
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * $Id$
 */
#ifndef _MESHUTIL_H_
#define _MESHUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

/**
 * @namespace sindy::mesh_type
 * @brief メッシュタイプネームスペース
 */
namespace mesh_type { // TODO: sindyAnnotationGroupで代用できるのでいらないはず
	/**
	 * @enum sindy::mesh_type::ECode
	 * @brief メッシュタイプ
	 */
	enum ECode {
		kCity,			//!< 都市地図メッシュ
		kBase,			//!< ベースメッシュ
		kMiddle,		//!< ミドルメッシュ
		kTop,			//!< トップメッシュ
	};
} // mesh_type

/**
 * @brief メッシュコード＋正規化XYから緯度経度に変換する
 * @see crd_cnv::MeshToLL
 * @param lMeshCode	[in]	メッシュコード
 * @param x			[in]	正規化座標X
 * @param y			[in]	正規化座標Y
 * @param lon		[out]	経度
 * @param lat		[out]	緯度
 */
void SINDYLIB_API MESH2LL( unsigned int lMeshCode, int x, int y, double& lon, double& lat );

/**
 * @brief メッシュコードから緯度経度に変換する
 * @see MESH2LL
 * @param lMeshCode	[in]	メッシュコード
 * @param dMinLon	[out]	最小経度
 * @param dMinLat	[out]	最小緯度
 * @param dMaxLon	[out]	最大経度
 * @param dMaxLat	[out]	最大緯度
 */
void SINDYLIB_API MESH2ENV( unsigned int lMeshCode, double& dMinLon, double& dMinLat, double& dMaxLon, double& dMaxLat );

/**
 * @brief メッシュコードから矩形に変換する
 * @see MESH2LL
 * @param lMeshCode	[in]	メッシュコード
 * @param ipSpRef	[in]	出力矩形に適用する空間参照
 * @return IEnvelopePtr
 */
IEnvelopePtr SINDYLIB_API MESH2ENV( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief メッシュコードからポリゴンに変換する
 * @see MESH2LL
 * @param lMeshCode	[in]	メッシュコード
 * @param ipSpRef	[in]	出力矩形に適用する空間参照
 * @return IGeometryPtr
 */
IGeometryPtr SINDYLIB_API MESH2POLY( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief 緯度経度からメッシュコード＋正規化XYに変換する
 * @see crd_cnv::LLtoMesh
 * @param dLon	[in]	経度
 * @param dLat	[in]	緯度
 * @param level	[in]	1:一次メッシュ 2:二次メッシュ 64:都市地図メッシュ
 * @param x		[out]	正規化座標X
 * @param y		[out]	正規化座標Y
 * @param no_mill	[in,optional]	正規化座標1000000を許容する場合は0（デフォルト：0）
 * @return メッシュコード
 */
unsigned int SINDYLIB_API LL2MESH( const double& dLon, const double& dLat, int level, int& x, int& y, int no_mill = 0 );

/**
 * @brief 緯度経度から一次メッシュコード＋正規化XYに変換する
 * @see LL2MESH
 * @param dLon	[in]	経度
 * @param dLat	[in]	緯度
 * @param x		[out]	正規化座標X
 * @param y		[out]	正規化座標Y
 * @param no_mill	[in,optional]	正規化座標1000000を許容する場合は0（デフォルト：0）
 * @return メッシュコード
 */
unsigned int SINDYLIB_API LL2MESH1st( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

/**
 * @brief 緯度経度から二次メッシュコード＋正規化XYに変換する
 * @see LL2MESH
 * @param dLon	[in]	経度
 * @param dLat	[in]	緯度
 * @param x		[out]	正規化座標X
 * @param y		[out]	正規化座標Y
 * @param no_mill	[in,optional]	正規化座標1000000を許容する場合は0（デフォルト：0）
 * @return メッシュコード
 */
unsigned int SINDYLIB_API LL2MESH2nd( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

/**
 * @brief 緯度経度から都市地図メッシュコード＋正規化XYに変換する
 * @see LL2MESH
 * @param dLon	[in]	経度
 * @param dLat	[in]	緯度
 * @param x		[out]	正規化座標X
 * @param y		[out]	正規化座標Y
 * @param no_mill	[in,optional]	正規化座標1000000を許容する場合は0（デフォルト：0）
 * @return メッシュコード
 */
unsigned int SINDYLIB_API LL2MESHcity( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

/**
 * @brief Pointからメッシュコード＋正規化XYに変換する
 * @see LL2MESH
 * @param Point	[in]	ポイント
 * @param x		[out]	正規化座標X
 * @param y		[out]	正規化座標Y
 * @param no_mill	[in,optional]	正規化座標1000000を許容する場合は0（デフォルト：0）
 * @return メッシュコード
 */
unsigned int SINDYLIB_API Point2MESH( IPoint* ipPoint, int level, int& x, int& y, int no_mill = 0 );

/**
 * @brief <b>渡されたメッシュコードの右隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 右隣のメッシュコード
 */
unsigned int SINDYLIB_API INCMESH_X( unsigned int code );

/**
 * @brief <b>渡されたメッシュコードの上隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 上隣のメッシュコード
 */
unsigned int SINDYLIB_API INCMESH_Y( unsigned int code );

/**
 * @brief <b>渡されたメッシュコードの左隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 左隣のメッシュコード
 */
unsigned int SINDYLIB_API DECMESH_X( unsigned int code );

/**
 * @brief <b>渡されたメッシュコードの下隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 下隣のメッシュコード
 */
unsigned int SINDYLIB_API DECMESH_Y( unsigned int code );

/**
 * @brief <b>2次メッシュ、都市地図メッシュから1次メッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @retval -1		エラー
 * @retval -1以外	1次メッシュコード
 */
unsigned int SINDYLIB_API MESH2MESH1ST( unsigned int code );

#endif // _MESHUTIL_H_
