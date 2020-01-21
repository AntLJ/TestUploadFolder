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
 *	GDBGeometry.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		新規作成
 */

#ifndef	___GDB_GEOMETRY_H___
#define	___GDB_GEOMETRY_H___

/**
 * @brief ２次メッシュ内正規化座標の取得
 * @param[in]		ipPoint		対象ポイント
 * @param[in,out]	cMesh		２次メッシュコード
 * 								0を指定した場合は[out]で、ZMeshPosによる変換で得られたメッシュコードをそのまま返す
 * 								非0を指定した場合は[in]で、cX, cYを指定したメッシュコードでの正規化座標に修正する
 * @param[out]		cX			２次メッシュ内正規化X座標
 * @param[out]		cY			２次メッシュ内正規化Y座標
 * @param[in]		ipSpatRef	ipPointにセットする空間参照（指定しない場合はセットしない）
 * @return	エラー情報
 */
HRESULT GDBGetBaseMeshNormalizeCoord(IPointPtr ipPoint, long *cMesh, long *cX, long *cY, ISpatialReferencePtr ipSpatRef = 0);

HRESULT GDBGetDistance(IPointPtr ipPoint1, IPointPtr ipPoint2, double *cDistance);	// 単位はｍ

/**
 * @brief ジオメトリとポリゴンの関係チェック
 * @note	ipGeometryの構成点全てが、「ipPolygonの構成点上にある(Contains)か、または辺上にある(Touches)」か否かを判定する
 * @param[in]	ipGeometry	対象ジオメトリ（Point, Polyline, Polygonのいずれか）
 * @param[in]	ipPolygon	対象ポリゴン
 * @param[out]	cContains	判定結果
 * @return	エラー情報
 */
HRESULT GDBCheckRelation(IGeometryPtr ipGeometry, IPolygonPtr ipPolygon, bool *cContains);

HRESULT GDBGetPolygonEnvelope(IPolygonPtr ipPolygon, IEnvelope **ipEnvelope);

#endif	//___GDB_GEOMETRY_H___
