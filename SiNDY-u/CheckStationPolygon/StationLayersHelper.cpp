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

#include "stdafx.h"

#include "StationLayersHelper.h"

// boost
#include <boost/assign.hpp>

// WinLib
#include <type_convert.h>

// ArcHelperEx
#include <AheLayerFunctions.h>
#include <GlobalFunctions.h>

// useful_header
#include <TDC/useful_headers/str_util.h>

using namespace sindy;
using namespace sindy::schema::sj;

namespace station_layer_helper
{
	// レイヤ名でのレイヤ番号検索用
	std::map<CString, long> LAYER_NOS = boost::assign::map_list_of
							( station_detail::kTableName,   rel_station_site::layerno_class::kStationDetail )
							( midzoom_platform::kTableName, rel_station_site::layerno_class::kMidzoomPlatform )
							( highzoom_station::kTableName, rel_station_site::layerno_class::kHighzoomStation )
							( midzoom_station::kTableName,  rel_station_site::layerno_class::kMidzoomStation )
							( lowzoom_platform::kTableName, rel_station_site::layerno_class::kLowzoomPlatform )
							;

	// レイヤ番号でのレイヤ名検索用
	std::map<long, CString> STATION_LAYERS = boost::assign::map_list_of
							( rel_station_site::layerno_class::kStationDetail,   station_detail::kTableName )
							( rel_station_site::layerno_class::kMidzoomPlatform, midzoom_platform::kTableName )
							( rel_station_site::layerno_class::kHighzoomStation, highzoom_station::kTableName )
							( rel_station_site::layerno_class::kMidzoomStation,  midzoom_station::kTableName )
							( rel_station_site::layerno_class::kLowzoomPlatform, lowzoom_platform::kTableName )
							;

	// 指定した駅ポリゴンレイヤのみが整備されているかチェックする (bug12350)
	bool checkRelationalLayer(const sindy::CRowContainer& row, sindy::CTableContainer& table, std::bitset<6> okLayerBits )
	{
		// チェック対象の駅IDを取得
		long stationId = getStationId(row, table);
		// 駅に紐付いていない
		if (stationId == -1)
			return false;

		// 駅ポリゴン紐付きテーブルからチェック対象の駅IDに一致するレコードを検索
		CString where = uh::str_util::format(_T("%s=%d"), rel_station_site::kGroupID, stationId);
		
		table.clear();
		table._Select(AheInitQueryFilter(nullptr, nullptr, where), true);
		table.CreateCache();

		// テーブルに紐付いていない
		if (table.empty())
			return false;

		// 検索したレコードのレイヤ番号群を取得
		std::set<long> layerNumbers;
		for (const auto& target_row : table) {
			long layerNo = CAST_ROWC(target_row)->GetValue(rel_station_site::kLayerNo_C, -1L);
			if (layerNo != -1L)
				layerNumbers.insert(layerNo);
		}

		// 検索したレコードのレイヤ種別をbitsetに格納
		std::bitset<6> layerBits;
		for (const auto& layerNo : layerNumbers) {
			layerBits.set(layerNo);
		}

		return okLayerBits == layerBits ? true : false;
	}

	// 対象フィーチャの駅IDを取得
	long getStationId(const sindy::CRowContainer& row, sindy::CTableContainer& table)
	{
		using namespace sindy::schema::sj;
		using namespace station_layer_helper;

		// 当該フィーチャのOIDとレイヤIDを取得
		long layerNo = getLayerNo(row.GetTableName());
		long layerOid = row.GetOID();

		// 駅ポリゴン紐付きテーブルを検索
		CString where = uh::str_util::format(_T("%s=%d and %s=%d"),
			rel_station_site::kLayerNo_C, layerNo,
			rel_station_site::kLayerOID, layerOid);

		table.clear();
		table._Select(AheInitQueryFilter(nullptr, nullptr, where), true);
		table.CreateCache();
		if (table.empty())
			return -1;

		// グループID を取得
		const auto& rowc = CAST_ROWC(*table.begin());
		long stationId = rowc->GetValue(rel_station_site::kGroupID, 0L);

		// 検索結果クリア
		table.clear();

		return stationId;
	}
}
