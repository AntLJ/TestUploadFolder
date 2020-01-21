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
#ifndef _AHE_MESHUTIL_H_
#define _AHE_MESHUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef AHE_UNUSE_ARCOBJECTS
	#include "AheMeshUtilArcObjects.h"
#endif

#if 0
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
#endif // if 0

/**
 * @brief 世界メッシュ編集モードかどうかを調べる
 * @return 世界メッシュ編集モードならtrue
 */
bool AheIsWorldMeshMode();

/**
 * @brief メッシュコード＋正規化XYから緯度経度に変換する
 * @see crd_cnv::MeshToLL
 * @param lMeshCode	[in]	メッシュコード
 * @param x			[in]	正規化座標X
 * @param y			[in]	正規化座標Y
 * @param lon		[out]	経度
 * @param lat		[out]	緯度
 */
void AheMESH2LL( unsigned int lMeshCode, int x, int y, double& lon, double& lat );

/**
 * @brief メッシュコードから緯度経度に変換する
 * @see MESH2LL
 * @param lMeshCode	[in]	メッシュコード
 * @param dMinLon	[out]	最小経度
 * @param dMinLat	[out]	最小緯度
 * @param dMaxLon	[out]	最大経度
 * @param dMaxLat	[out]	最大緯度
 */
void AheMESH2ENV( unsigned int lMeshCode, double& dMinLon, double& dMinLat, double& dMaxLon, double& dMaxLat );


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
unsigned int AheLL2MESH( const double& dLon, const double& dLat, int level, int& x, int& y, int no_mill = 0 );

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
unsigned int AheLL2MESH1st( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

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
unsigned int AheLL2MESH2nd( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

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
unsigned int AheLL2MESHcity( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );


/**
 * @brief 渡されたメッシュコードから（x,y）離れたメッシュコードを求める
 * @param code  [in] メッシュコード
 * @param xdist [in] X方向へ何メッシュ離れているか
 * @param ydist [in] Y方向へ何メッシュ離れているか
 * @retval -1 失敗
 * @retval -1以外 メッシュコード
 */
unsigned int AheGETMESHCODE( unsigned int code, int xdist, int ydist );

/**
 * @brief <b>渡されたメッシュコードの右隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 右隣のメッシュコード
 */
unsigned int AheINCMESH_X( unsigned int code );

/**
 * @brief <b>渡されたメッシュコードの上隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 上隣のメッシュコード
 */
unsigned int AheINCMESH_Y( unsigned int code );

/**
 * @brief <b>渡されたメッシュコードの左隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 左隣のメッシュコード
 */
unsigned int AheDECMESH_X( unsigned int code );

/**
 * @brief <b>渡されたメッシュコードの下隣のメッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @return 下隣のメッシュコード
 */
unsigned int AheDECMESH_Y( unsigned int code );

/**
 * @brief <b>2次メッシュ、都市地図メッシュから1次メッシュコードを返す</b>\n
 * @param code	[in]	メッシュコード
 * @retval -1		エラー
 * @retval -1以外	1次メッシュコード
 */
unsigned int AheMESH2MESH1ST( unsigned int code );

/**
 * @brief 2点間（緯度経度）の距離（m）を求める
 * @param lon1 [in] 地点1 X
 * @param lat1 [in] 地点1 Y
 * @param lon2 [in] 地点2 X
 * @param lat2 [in] 地点2 Y
 * @retval 距離（m）
 */
double AheGETDIST( const double& lon1, const double& lat1, const double& lon2, const double& lat2 );

/**
 * @brief 2地点の経緯度から、2点間の距離 + 距離(X,Y)mを求める
 * @param lon1  [in]  地点1 X
 * @param lat1  [in]  地点1 Y
 * @param lon2  [in]  地点2 X
 * @param lat2  [in]  地点2 Y
 * @param distX [out] 距離 X
 * @param distY [out] 距離 Y
 * @retval 距離（m）
 */
double AheGETDISTXY( const double& lon1, const double& lat1, const double& lon2, const double& lat2, double& distX, double& distY);

/**
 * @brief 経緯度とそこから経度方向にX(m),緯度方向にY(m)離れたところの経緯度を求める
 * @param lon1  [in]  地点1 X
 * @param lat1  [in]  地点1 Y
 * @param distX [in]  距離 X
 * @param distY [in]  距離 Y
 * @param lon2  [out] 地点2 X
 * @param lat2  [out] 地点2 Y
 * @retval 収束までの回数
 */
int AheGETLLPT( const double& lon1, const double& lat1, const double& distX, const double& distY, double& lon2, double& lat2);

/**
 * @brief 2点間（メッシュXY）の差分を求める
 * @param mesh1 [in] メッシュコード１
 * @param x1    [in] 正規化X座標1
 * @param y1    [in] 正規化Y座標1
 * @param mesh2 [in] メッシュコード２
 * @param x2    [in] 正規化X座標2
 * @param y2    [in] 正規化Y座標2
 * @param dx    [out] X差分
 * @param dy    [out] Y差分
 * @retval  0 成功
 * @retval -1 失敗
 */
int AheGETDIFF( unsigned int mesh1, int x1, int y1, unsigned int mesh2, int x2, int y2, double& dx, double& dy );

struct coord19;

/**
 * @brief 経緯度から最も近い１９座標系の系とＸＹを返す
 * @param lon [in] X
 * @param lat [in] Y
 * @param sys_xy [out] 系＋XY
 */
void AheLL2XY19( double lon, double lat, coord19 *sys_xy );

/**
 * @brief 経緯度からUTM座標系のゾーンをを返す
 * @param lon [in] X
 * @param lat [in] Y
 * @param zone [out] XY
 */
int AheLL2UTM( double lon, double lat, double *dX, double *dY ); 

/**
 * @brief 緯度経度系からUTM座標系の座標を返す
 * @param nZone [in] UTMゾーン
 * @param lon [in] X
 * @param lat [in] Y
 * @param sys_xy [out] 系＋XY
 */
void AheLLtoUTM_WGS84( int nZone,  double lon, double lat, double *dX, double *dY );

struct d_lonlat;

/**
 * @brief １９座標系のＸＹから経緯度へ変換
 * @param axis [in] 該当する座標系の番号
 * @param x [in] 19座標系のX(緯度方向)
 * @param y [in] 19座標系のY(経度方向)
 * param d_lonlat [out] 10進で表した経度緯度(ex:130.23432342....)
 */
void AheXY192LL( int axis, double x, double y, d_lonlat *lonlat );
/**
 * @brief UTM座標系から、緯度経度を返す
 * @param nZone [in] UTMゾーン
 * @param lon [in] X
 * @param lat [in] Y
 * @param sys_xy [out] 系＋XY
 */
void AheUTM2LL_WGS84( int nZone, double x, double y, double *dX, double *dY );

#endif // _AHE_MESHUTIL_H_
