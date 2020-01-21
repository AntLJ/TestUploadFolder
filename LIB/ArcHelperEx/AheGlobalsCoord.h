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
 * @file
 * @note AheGlobals.h/cppからcrd_cnvのリンクが必要になる関数群を分離したもの。
 *       経緯は下記レビューを参照の事。
 *       ArcHelperExのリファクタリング（crd_cnv関係） http://orias.mr.ipc.pioneer.co.jp/reviewboard/r/2424/
 *       但し、ビルドが通った時点でやめたので、実はcrd_cnvを必要としないものが含まれている可能性あり。
 *       その場合はAheGlobals.h/cppかAheGlobalsMisc.h/cppに移動するといいかも知れない。
 */
#pragma once

//////////////////////////////////////////////////////////////////////
// URI 解析用関数・座標変換など
//////////////////////////////////////////////////////////////////////
/// URI 解析関数用構造体
struct LonLat{
	double Longitude;
	double Latitude;
	double Scale;
};

struct MeshXY{
	unsigned int MeshCode;
	int X;
	int Y;
	double Scale;
};

struct AdrsScale{
	CComBSTR Adrs;
	double Scale;
};

/**
* @brief メッシュコード座標インクリメント
*
* @note メッシュコードをXY方向にインクリメントする際の８進インクリメント
*/
#define INC_MESHXY(x)	( 7 == ( x % 10)) ? ( x + 3) : ( x + 1)
/**
* @brief メッシュコード座標デクリメント
*
* @note メッシュコードをXY方向にデクリメントする際の８進デクリメント
*/
#define DEC_MESHXY(x)	( 0 == ( x % 10)) ? ( x - 3) : ( x - 1)

/**
 * @brief メッシュコードから、メッシュXYコードへ変換
 *
 * @param nX				[out]			X
 * @param nY				[out]			Y
 * @param nMeshCode			[in]			メッシュコード
 */
void AheMeshcode2MeshXY( int& nX, int& nY, unsigned int nMeshCode);

/**
 * @brief	メッシュXYコードから、メッシュコードへ変換
 *
 * @note	指定されたポイントと長さから矩形を作成します。
 *			フィーチャの選択などに多く使用されています。
 *
 * @param nX				[in]  X
 * @param nY				[in]  Y
 *
 * @return メッシュコード
 */
unsigned int AheMeshXY2Meshcode( int nX, int nY);
/**
 * @brief 度分秒の緯度、経度から Real Degree 方式の緯度、経度を取得する
 *
 * @param iDeg			[in]			度
 * @param iMin			[in]			分
 * @param dSec			[in]			秒
 *
 * @return Real Degree での数値
 */
DOUBLE AheDegMinSec2Realdegree( INT iDeg, INT iMin, DOUBLE dSec );

/**
 * @brief 入力された文字列から度分秒形式の緯度、経度を取得する
 *
 * @param str			[in]			対象となる文字列
 * @param iDeg			[out]			取得した度
 * @param iMin			[out]			取得した分
 * @param dSec			[out]			取得した秒
 *
 * @retval TRUE 適合する座標を取得できた
 * @retval FALSE 適合する座標を取得できなかった
 */
BOOL AheStr2DegMinSec( CString& , INT& , INT& , DOUBLE& );

/**
 * URI ( MapFanV 用 ) mf://mapfan/map/pos? で始まるアドレスを LonLat 構造体に変換して返す
 *
 * @param CString		[in]  URI 文字列
 * @param LonLat		[out] LonLat 型の結果
 *
 * @retval TURE 正常なURI
 * @retval FALSE 解析できないURI
 */
BOOL AheURIMFV2LonLat(CString, LonLat&);

BOOL AheURIMFV2MeshXY(CString, MeshXY&);

BOOL AheChkLonLat(CString);

/**
 * URI ( MapFan Web 用 ) http://kokomail.mapfan.com/receive.cgi? で始まるアドレスを LonLat 構造体に変換して返す
 *
 * @param CString		[in]  URI 文字列
 * @param LonLat		[out] LonLat 型の結果
 *
 * @retval TURE 正常なURI
 * @retval FALSE 解析できないURI
 */
BOOL AheURIMFW2LonLat(CString, LonLat&);

BOOL AheURIMFW2MeshXY(CString, MeshXY&);

/**
 * URI ( MapFan Web 用 ) http://mapserv.mapfan.com/cgi-bin/map/mapserv.cgi? で始まるアドレスを LonLat 構造体に変換して返す
 *
 * @param CString		[in]  URI 文字列
 * @param LonLat		[out] LonLat 型の結果
 *
 * @retval TURE 正常なURI
 * @retval FALSE 解析できないURI
 */
BOOL AheURIMFP2LonLat(CString, LonLat&);

/**
 * URI ( SiNDY メッシュ XY用 ) sindy://sindy/map/pos? で始まるアドレスを LonLat 構造体に変換して返す
 *
 * @param CString		[in]  URI 文字列
 * @param LonLat		[out] LonLat 型の結果
 *
 * @retval TURE 正常なURI
 * @retval FALSE 解析できないURI
 */
BOOL AheURISiNDYMesh2LonLat(CString, LonLat&);

/**
 * URI ( SiNDY メッシュ XY用 ) sindy://sindy/map/pos? で始まるアドレスを MeshXY 構造体に変換して返す
 *
 * @param CString		[in]  URI 文字列
 * @param LonLat		[out] MeshXY 型の結果
 *
 * @retval TURE 正常なURI
 * @retval FALSE 解析できないURI
 */
BOOL AheURISiNDYMesh2MeshXY(CString, MeshXY&);

/**
 * URI ( SiNDY 住所用 ) sindy://sindy/map/pos? で始まるアドレスを LonLat 構造体に変換して返す
 *
 * @param CString		[in]  URI 文字列
 * @param LonLat		[out] LonLat 型の結果
 *
 * @retval TURE 正常なURI
 * @retval FALSE 解析できないURI
 */
BOOL AheURISiNDYAdrs2LonLat(CString, LonLat&);

BOOL AheURISiNDYAdrs2Adrs(CString, AdrsScale&);

/**
 * @brief 地理参照を取得する
 *
 * @param ipUnk	[in]	AheGetFeatureClass でフィーチャクラスを取得できるオブジェクト
 *
 * return ISpatialReferencePtr
 */
ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk );
