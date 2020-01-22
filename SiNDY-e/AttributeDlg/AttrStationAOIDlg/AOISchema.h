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

#include <tchar.h>
#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

namespace sj{

// TODO: 駅AOIの整備継続が決まったなら、下記はsj.hのsindy::schma::sj内に移行する

/// 駅AOIポリゴン
namespace station_aoi {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; //!< テーブル名
}

/// 駅プラットフォームポリゴン
namespace station_platform {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; //!< テーブル名
	extern const _TCHAR kAOI_ID[];    //!< AOIポリゴンID
	extern const _TCHAR kZLevel[];    //!< Zレベル
}

/// 駅POI紐付きテーブル名
namespace aoi_rel_station_poi {
	extern const _TCHAR kTableName[];    //!< テーブル名
	extern const _TCHAR kAOI_ID[];       //!< AOIポリゴンID
	extern const _TCHAR kJoinLineInfo[]; //!< 乗り入れ路線のOID
} // namespace aoi_rel_station_poi

/// 駅POI紐付きテーブル
namespace rel_station_poi {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::sj::aoi_rel_station_poi;
} // namespace rel_station_poi

// TODO:移行するのはこの上の行まで

} // namespace sj

} // namespace schema

} // namespace sindy
