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
#include "define.h"
/**
 * @file	GlobalFunc.h
 * @brief	グローバル関数定義
 */

namespace gf
{
	/**
	 * @brief	ワークスペース取得
	 * @note	SDE PGDB FGDB いずれもＯＫ
	 * @param	[in] lpcszDBProp		ＤＢ接続プロパティ
	 * @return	ワークスペース
	 */
	IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp );

	/**
	 * @brief	属性検索
	 * @note	内部エラー発生時は、件数に-1を設定します
	 * @param	ipTable			[in]	検索元のフィーチャクラス
	 * @param	pCount			[out]	検索レコード数件数取得しないなら、NULL指定)
	 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
	 * @return	フィーチャカーソル(検索レコード数 = 0なら、NULL返す)
	 */
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount = NULL, LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );
	
	/**
	 * @brief	マージ
	 * @param	[in] ipGeometryCollection ジオメトリコレクション
	 * @param	[in] ipSpatialReference   空間参照
	 * @return	IGeometryPtr
	 */
	IGeometryPtr constructUnion( const IGeometryPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference );
	
	/**
	 * @brief	ポリゴンの差分
	 * @param	[in] ipMeshGeometry    メッシュジオメトリ
	 * @param	[in] ipMergeGeometry   マージジオメトリ
	 * @return	IGeometryPtr
	 */
	IGeometryPtr difference( const IGeometryPtr& ipMeshGeometry, const IGeometryPtr& ipMergeGeometry );

	/**
	 * @brief   接触判定
	 * @param   [in] ipBaseGeometry   ベースジオメトリ
	 * @param   [in] ipOtherGeometry  ベースとの接触判定を行いたいジオメトリ
	 * @retval  true(接触している)、false(接触していない)
	 */
	bool isTouches( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry );

	/**
	 * @brief   含む判定
	 * @param   [in] ipBaseGeometry   ベースジオメトリ
	 * @param   [in] ipOtherGeometry  ベースとの含む判定を行いたいジオメトリ
	 * @retval  true(含むしている)、false(含むしていない)
	 */
	bool IsContains( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry );

	/**
	* @brief	フィールドマップ作成
	* @param	[in] ipTable    テーブル
	* @return  フィールドマップ値
	*/
	FIELDSMAP createFieldMap( const ITablePtr& ipTable );

	/**
	* @brief	属性値取得
	* @param	[in] ipFeature    フィチャ
	* @param	[in] fieldIdx     フィールド索引   
	* @return  CComVariant値
	*/
	CComVariant getValue( const IFeaturePtr &ipFeature, long fieldIdx) ;

} // gf
