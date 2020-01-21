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
 * @file AheGeometryConvert.h
 * @brief <b>ジオメトリコンバート系定義ファイル</b>\n
 * @author コンテンツ本部技術開発部制作部開発グループ 古川貴宏
 * この関数群は、SiNDYLibのGeometryConvert.cppを参考に作りました。
 * @version $Id$
 */
#pragma once

/**
 * @brief <b>ジオメトリから与えられた次元のジオメトリを生成する</b>\n
 * 下記変換以外で次元が同じ場合はコピーを生成します。\n
 * サポートしている変換は次の通り：\n
 * @li Ring     -> Polygon
 * @li Envelope -> Polygon
 * @li Polygon  -> Polyline
 * @li Ring     -> Polyline
 * @li Envelope -> Polyline
 * @li Path     -> Polyline
 * @li Polygon  -> Multipoint
 * @li Ring     -> Multipoint
 * @li Envelope -> Multipoint
 * @li Polyline -> Multipoint
 * @li Path     -> Multipoint
 * @li Point    -> Multipoint
 * @param ipGeom	[in]	ジオメトリ
 * @param emGeomDim	[in]	次元
 * @retval NULL		エラー
 * @retval NULL以外	変換後のジオメトリ
 */
IGeometryPtr Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim );

/**
 * @brief <b>ジオメトリからマルチポイントを生成する</b>\n
 * 与えられたジオメトリがマルチポイントの場合はコピーを生成します。\n
 * サポートしているジオメトリタイプは以下のとおり：\n
 * @li Polygon
 * @li Ring
 * @li Envelope
 * @li Polyline
 * @li Path
 * @li Point
 * @param ipGeom	[in]	ジオメトリ
 * @retval NULL		エラー
 * @retval NULL以外	変換後のジオメトリ
 */
IGeometryPtr ToMultipoint( IGeometry* ipGeom );

/**
 * @brief <b>ジオメトリからポリラインを生成する</b>\n
 * 与えられたジオメトリがポリラインの場合はコピーを生成します。\n
 * サポートしているジオメトリタイプは以下のとおり：\n
 * @li Polygon
 * @li Ring
 * @li Envelope
 * @param ipGeom	[in]	ジオメトリ
 * @retval NULL		エラー
 * @retval NULL以外	変換後のジオメトリ
 */
IGeometryPtr ToPolyline( IGeometry* ipGeom );

/**
 * @brief <b>ジオメトリからパスを生成する</b>\n
 * 与えられたジオメトリがパスの場合はコピーを生成します。\n
 * サポートしているジオメトリタイプは以下のとおり：\n
 * @li Ring
 * @li Envelope
 * @param ipGeom	[in]	ジオメトリ
 * @retval NULL		エラー
 * @retval NULL以外	変換後のジオメトリ
 */
IGeometryPtr ToPath( IGeometry* ipGeom );

/**
 * @brief <b>ジオメトリからポリゴンを生成する</b>\n
 * 与えられたジオメトリがポリゴンの場合はコピーを生成します。\n
 * サポートしているジオメトリタイプは以下のとおり：\n
 * @li Ring
 * @li Envelope
 * @param ipGeom	[in]	ジオメトリ
 * @retval NULL		エラー
 * @retval NULL以外	変換後のジオメトリ
 */
IGeometryPtr ToPolygon( IGeometry* ipGeom );

/**
 * @brief <b>ジオメトリからリングを生成する</b>\n
 * 与えられたジオメトリがリングの場合はコピーを生成します。\n
 * サポートしているジオメトリタイプは以下のとおり：\n
 * @li Envelope
 * @param ipGeom	[in]	ジオメトリ
 * @retval NULL		エラー
 * @retval NULL以外	変換後のジオメトリ
 */
IGeometryPtr ToRing( IGeometry* ipGeom );
