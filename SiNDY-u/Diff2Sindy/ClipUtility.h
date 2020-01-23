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

#include "Workspace.h"

namespace sindy {
	class CRowBase;
	class CTableContainer;
} // sindy

ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk ); // AheGlobals.h

/**
 * @brief ジオメトリのラベルポイントを取得する
 * @param geom [in] IGeometryPtr
 * @return ラベルポイント
 */
IPointPtr GetLabelPoint( IGeometryPtr geom );

/**
 * @brief ジオメトリをたす
 * 
 * 2つのジオメトリをGeometryBagとして返します。
 * 与えられたものが既にGeometryBagの場合は、ばらして入れ直します。
 * @param geom1 [in] 追加対象ジオメトリ1
 * @param geom2 [in] 追加対象ジオメトリ2
 * @retval GeometryBag
 */
IGeometryPtr AddGeometry( IGeometryPtr geom1, IGeometryPtr geom2 );

/**
 * @brief ジオメトリが空かどうか判定する
 * @param geom [in] 判定対象ジオメトリ
 * @retval true NULL、空である
 * @retval false 空ではない
 */
bool IsEmpty( IGeometryPtr geom );

/**
 * @brief マルチパート形状を別フィーチャとして分離する
 * @param row [in] マルチパート形状を保持しているかもしれないフィーチャ
 * @param table [in] 新規に作成する場合の対象コンテナ
 */
void MultiGeometry2Geometries( sindy::CRowBase& row, sindy::CTableContainer& table );

/**
 * @brief クリップ処理をする
 *
 * クリップ形状を跨ぐフィーチャを分割します。\n
 * 分割された形状は新規フィーチャとして作成されます。
 * 
 * @param con [in,out] クリップ処理対象フィーチャが格納されているコンテナ
 * @param row [in,out] クリップ処理対象フィーチャ
 * @param ipClip [in] クリップ形状
 */
void Clip( sindy::CTableContainer& con, sindy::CRowBase& row, IGeometryPtr ipClip );
