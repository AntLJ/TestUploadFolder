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
#include "AOISchema.h"

namespace sindy {

namespace schema {

namespace sj{

// TODO: 駅AOIの整備継続が決まったなら、下記はsj.cppのsindy::schma::sj内に移行する

/// 駅AOIポリゴン
namespace station_aoi {
	const _TCHAR kTableName[]         = _T("STATION_AOI"); //!< テーブル名
}// namespace station_aoi

/// 駅プラットフォームポリゴン
namespace station_platform {
	const _TCHAR kTableName[]         = _T("STATION_PLATFORM"); //!< テーブル名

	const _TCHAR kAOI_ID[]            = _T("AOI_ID");           //!< AOIポリゴンID
	const _TCHAR kZLevel[]            = _T("ZLEVEL");           //!< Zレベル
}// namespace station_station_poi

namespace aoi_rel_station_poi {
	const _TCHAR kTableName[]    = _T("REL_STATION_POI");   //!< テーブル名

	const _TCHAR kAOI_ID[]       = _T("AOI_ID");            //!< AOIポリゴンID
	const _TCHAR kJoinLineInfo[] = _T("JOIN_LINE_INFO_ID"); //!< 乗り入れ路線のOID
} // namespace aoi_rel_station_poi

// TODO:移行するのはこの上の行まで

}

}

}
