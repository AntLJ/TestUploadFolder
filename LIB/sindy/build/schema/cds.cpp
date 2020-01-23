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
#include <sindy/schema/cds.h>

namespace sindy {

namespace schema {

namespace city_annotation_gs {
	const _TCHAR kTableName[]		= _T("CITY_ANNOTATION_GS");		//!< テーブル名

	const _TCHAR kNameString1[]		= _T("NAME_STRING1");			//!< 注記文字列1
	const _TCHAR kNameString2[]		= _T("NAME_STRING2");			//!< 注記文字列2
	const _TCHAR kStringNum1[]		= _T("STRING_NUM1");			//!< 注記1文字数
	const _TCHAR kStringNum2[]		= _T("STRING_NUM2");			//!< 注記2文字数
	const _TCHAR kAnnoClass[]		= _T("ANNOCLASS_C");			//!< 注記種別
	const _TCHAR kNameYomi[]		= _T("NAME_YOMI");				//!< 注記読み
	const _TCHAR kEngDefault[]		= _T("ENG_DEFAULT");			//!< デフォルト英語注記
	const _TCHAR kEngName[]			= _T("ENG_NAME");				//!< 個別英語注記
	const _TCHAR kEngOut[]			= _T("ENGOUT_F");				//!< 英語注記出力フラグ
	const _TCHAR kDispType[]		= _T("DISPTYPE_C");				//!< 表示タイプ
	const _TCHAR kHimawari[]		= _T("HIMAWARI_F");				//!< ひまわりフラグ
	const _TCHAR kContentsCode[]	= _T("CONTENTS_CODE");			//!< コンテンツコード
	const _TCHAR kContentsSeq[]		= _T("CONTENTS_SEQ");			//!< コンテンツシーケンス
	const _TCHAR kAnnoDisp[]		= _T("ANNO_DISP_C");			//!< 注記表示コード
} // city_annotation_gs

namespace building_gs {
	const _TCHAR kTableName[]		= _T("BUILDING_GS");		//!< テーブル名

	const _TCHAR kBldClass[]		= _T("BLDCLASS_C");			//!< 建物種別
	const _TCHAR kCorridor[]		= _T("CORRIDOR_F");			//!< 渡り廊下フラグ
	const _TCHAR kNoWall[]			= _T("NOWALL_F");			//!< 無壁フラグ
	const _TCHAR kBldName[]			= _T("BLDNAME");			//!< 建物名称
	const _TCHAR kBldAddr[]			= _T("BLDADDR");			//!< 建物住所
	const _TCHAR kGeospaceID[]		= _T("GEOSPACE_ID");		//!< GEOSPACE家形ID
} // building_gs

namespace city_line_gs {
	const _TCHAR kTableName[]		= _T("CITY_LINE_GS");		//!< テーブル名

	const _TCHAR kBgClass[]			= _T("BGCLASS_C");			//!< 背景ライン種別
} // city_line_gs

namespace city_railway_gs {
	const _TCHAR kTableName[]		= _T("CITY_RAILWAY_GS");	//!< テーブル名

	const _TCHAR kRailwayClass[]	= _T("RAILWAYCLASS_C");		//!< 鉄道種別
	const _TCHAR kUnderground[]		= _T("UNDERGROUND_F");		//!< 地下フラグ
} // city_railway_gs

namespace city_site_gs {
	const _TCHAR kTableName[]		= _T("CITY_SITE_GS");		//!< テーブル名

	const _TCHAR kBgClass[]			= _T("BGCLASS_C");			//!< 背景種別
} // city_site_gs

namespace land_cover_gs {
	const _TCHAR kTableName[]		= _T("LAND_COVER_GS");		//!< テーブル名
} // land_cover_gs

namespace underground_area_gs {
	const _TCHAR kTableName[]		= _T("UNDERGROUND_AREA_GS");	//!< テーブル名
} // underground_area_gs

namespace bld_tenant_gs {
	const _TCHAR kTableName[]		= _T("BLD_TENANT_GS");		//!< テーブル名

	const _TCHAR kGeospaceID[]		= _T("GEOSPACE_ID");		//!< 家屋形状地物ID
	const _TCHAR kLClass[]			= _T("L_CLASS");			//!< 大分類コード
	const _TCHAR kMClass[]			= _T("M_CLASS");			//!< 中分類コード
	const _TCHAR kSClass[]			= _T("S_CLASS");			//!< 小分類コード
	const _TCHAR kPopularName[]		= _T("POPULAR_NAME");		//!< 通称名称
	const _TCHAR kSignageName[]		= _T("SIGNAGE_NAME");		//!< 表記名称
	const _TCHAR kNameKana[]		= _T("NAME_KANA");			//!< カナ名称
	const _TCHAR kIconClass[]		= _T("ICON_CLASS");			//!< アイコン種別
	const _TCHAR kReserved[]		= _T("RESERVED");			//!< reserved
	const _TCHAR kAddrCode11[]		= _T("ADDR_CODE11");		//!< 住所コード11桁
	const _TCHAR kPrefKanji[]		= _T("PREF_KANJI");			//!< 漢字県名
	const _TCHAR kCityKanji[]		= _T("CITY_KANJI");			//!< 漢字市町村名
	const _TCHAR kOazaKanji[]		= _T("OAZA_KANJI");			//!< 漢字大字名
	const _TCHAR kAzaKanji[]		= _T("AZA_KANJI");			//!< 漢字字・丁目
	const _TCHAR kBlockKanji[]		= _T("BLOCK_KANJI");		//!< 番地
	const _TCHAR kGoKanji[]			= _T("GO_KANJI");			//!< 号
	const _TCHAR kBranchNum[]		= _T("BRANCH_NUM");			//!< 枝番
	const _TCHAR kPrefKana[]		= _T("PREF_KANA");			//!< 半角カナ県名
	const _TCHAR kCityKana[]		= _T("CITY_KANA");			//!< 半角カナ市町村名
	const _TCHAR kOazaKana[]		= _T("OAZA_KANA");			//!< 半角カナ大字名
	const _TCHAR kAzaKana[]			= _T("AZA_KANA");			//!< 半角カナ字・丁目
	const _TCHAR kPostNum[]			= _T("POST_NUM");			//!< 郵便番号
} // bld_tenant_gs

namespace city_station_gs {
	const TCHAR kTableName[]			= _T("CITY_STATION_GS");			//!< テーブル名

	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kStationClass[]			= _T("STATIONCLASS_C");			//!< 鉄道駅種別コード
} // city_station_gs

} // schema

} // sindy
