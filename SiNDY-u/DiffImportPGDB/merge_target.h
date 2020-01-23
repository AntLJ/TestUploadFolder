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

#include <sindy/schema/map.h>

using namespace sindy::schema;

namespace{
	// 背景のマージ対象種別（マージ用の設定ファイルより転記）
	// GEOSPACEで追加されたもので対象のコードを追加、↑のうち使われていないコードは削除
	static const long CS_TARGET_LIST[] = {
		city_site::bg_class::kBlock,                   //!< 街区
		city_site::bg_class::kSands,                   //!< 砂地
		city_site::bg_class::kMarsh,                   //!< 湿地
		city_site::bg_class::kLawnOther,               //!< 芝生等その他緑地
		city_site::bg_class::kPark,                    //!< 植栽地
		city_site::bg_class::kLevee,                   //!< 堤外地
		city_site::bg_class::kPool,                    //!< プール
		city_site::bg_class::kSideway,                 //!< 歩道（その他道）
		city_site::bg_class::kCenterDivider,           //!< 中央分離帯（標準）
		city_site::bg_class::kPublicRoadStairs,        //!< 公道階段
		city_site::bg_class::kHospital,                //!< 病院
		city_site::bg_class::kSchool,                  //!< 学校
		city_site::bg_class::kPlant,                   //!< 工場
		city_site::bg_class::kDamGuideExist,           //!< ダム（便覧記載あり）
		city_site::bg_class::kDamGuideNotExist,        //!< ダム（便覧記載なし）
		city_site::bg_class::kStationSite,             //!< 鉄道敷地
		city_site::bg_class::kOtherSite,               //!< 舗装地その他敷地
		city_site::bg_class::kBreakwater,              //!< 防波堤
		city_site::bg_class::kForce,                   //!< 自衛隊・米軍演習場（駐屯地ほか）
		city_site::bg_class::kForceManeuveringGround,  //!< 自衛隊・米軍演習場（演習場）
		city_site::bg_class::kCenterDividerPark,       //!< 中央分離帯（植栽地）
	};

	// CityLineのマージ対象(bug 10519で追加)
	static const long CL_TARGET_LIST[] = {
		city_line::bg_class::kTollbooth,         //!< 料金所
		city_line::bg_class::kWatergate,         //!< 水門
		city_line::bg_class::kErosionControlDam, //!< 砂防ダム
		city_line::bg_class::kStairsDecoration,  //!< 階段装飾線
		city_line::bg_class::kParent,            //!< 親線
		city_line::bg_class::kStairsTread,       //!< 階段踏面ライン（公道階段）
	};

	// エリアポリゴンのマージ対象種別 (未調査・作業中以外)
	static const long AP_TARGET_LIST[] = {
		city_area::area_class::kPark,   //!< 公園
	};
			
	// 行政界のマージ対象種別 (海以外)
	static const long ADM_TARGET_LIST[] = {
		city_admin::area_class::kOutArea, //!< エリア外
		city_admin::area_class::kInArea,  //!< エリア内
	};
}
