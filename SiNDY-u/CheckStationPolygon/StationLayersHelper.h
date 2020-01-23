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

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

namespace station_layer_helper
{
	/// レイヤ名とレイヤ番号の対応表(レイヤ番号の検索用)
	extern std::map<CString, long> LAYER_NOS;

	/// レイヤ番号とレイヤ名の対応表(レイヤ名の検索用)
	extern std::map<long, CString> STATION_LAYERS;

	/**
	 * @brief レイヤ名指定してレイヤ番号を取得する
	 * @param layer_name [in] レイヤ名
	 * @return レイヤ番号
	 */
	inline long getLayerNo( const CString& layer_name ) { return LAYER_NOS[layer_name]; }

	/**
	 * @brief レイヤ番号指定してレイヤ名を取得する
	 * @param layer_no [in] レイヤ番号
	 * @return レイヤ名
	 */
	inline CString getLayerName( long layer_no ){ return STATION_LAYERS[layer_no]; }

	/**
	 * @brief レイヤ名とレイヤ番号の対応表を返す
	 * @return レイヤ名とレイヤ番号の対応表
	 */
	inline std::map<CString, long> getStationLayerInfos(){  return LAYER_NOS; }

	/**
	 * @brief 駅ポリゴンのレイヤか
	 * @param layer_name [in] レイヤ名
	 * @retval true  駅ポリゴンレイヤである
	 * @retval false 駅ポリゴンレイヤではない
	 */
	inline bool isStationPolygonLayer( const CString& layer_name ){ return LAYER_NOS.count( layer_name ) != 0; }

	/**
	* @brief  指定した駅ポリゴンレイヤのみが整備されているかチェックする (bug12350)
	* @note   フラグの意味については "define_enum.h"を参照
	* @param  row         [in] 対象フィーチャ
	* @param  table       [in] 駅ポリゴン紐付きテーブル
	* @param  okLayerBits [in] 整備されていて良いレイヤを表すbitset
	* @retval true  問題なし
	* @retval false 問題あり
	*/
	bool checkRelationalLayer(const sindy::CRowContainer& row, sindy::CTableContainer& table, std::bitset<6> okLayerBits );

	/**
	* @brief  対象フィーチャの駅IDを取得
	* @param  row   [in] 対象フィーチャ
	* @param  table [in] 駅ポリゴン紐付きテーブル
	* @return 駅ID
	* @retval -1 ID取得失敗
	*/
	inline long getStationId(const sindy::CRowContainer& row, sindy::CTableContainer& table);
}
