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

#ifndef SINDY_SCHEMA_GLOBAL_H_
#define SINDY_SCHEMA_GLOBAL_H_

#include <iostream>

namespace sindy {

namespace schema {

namespace global{

namespace middlemesh {
	extern const _TCHAR kTableName[];      //!< テーブル名
	extern const _TCHAR kObjectID[];       //!< ObjectID
	extern const _TCHAR kMeshCode[];       //!< メッシュコード
	extern const _TCHAR kShape[];
} // middlemesh

namespace basemesh {
	extern const _TCHAR kTableName[];      //!< テーブル名
	extern const _TCHAR kObjectID[];       //!< ObjectID
	extern const _TCHAR kMeshCode[];       //!< メッシュコード
	extern const _TCHAR kShape[];
} // basemesh

namespace citymesh {
	extern const _TCHAR kTableName[];      //!< テーブル名
	extern const _TCHAR kObjectID[];       //!< ObjectID
	extern const _TCHAR kMeshCode[];       //!< メッシュコード
	extern const _TCHAR kCreateYear[];     //!< 作成年度
	extern const _TCHAR kModifyYear[];     //!< 更新年度
	extern const _TCHAR kCompleteF[];      //!< 完全化フラグ
	extern const _TCHAR kShape[];
} // citymesh

namespace object {
	extern const _TCHAR kObjectID[]; ///< オブジェクトID
} // object

namespace common_table {
	using namespace ::sindy::schema::global::object;
	extern const _TCHAR kOperator[];       //!< 最終更新者
	extern const _TCHAR kPurpose[];        //!< 作業目的
	extern const _TCHAR kModifyDate[];     //!< 最終更新日時
	extern const _TCHAR kUpdateType[];     //!< 最終更新内容
	extern const _TCHAR kProgModifyDate[]; //!< 最終プログラム更新日時
	extern const _TCHAR kModifyProgName[]; //!< 最終更新プログラム名
	extern const _TCHAR kUserClaim[];      //!< ユーザクレームフラグ
	extern const _TCHAR kSource[];         //!< 情報ソース

	namespace gl_chg_code {
		enum ECode {
			kExistData   = 0,              //!< 既存
			kNew         = 1,              //!< 新規作成
			kDelete      = 2,              //!< 削除
			kShapeModify = 3,              //!< 形状変更
			kAttrModify  = 4,              //!< 属性変更
			kWorkImport  = 5,              //!< 作業インポート
			kDataImport  = 6,              //!< データインポート
		};
	} // gl_chg_code

	namespace gl_layer_code{
		enum ECode{
			kBuilding			= 0,
			kCityStation		= 1,
			kBaseLine			= 2,
			kBaseRailway		= 3,
			kLandUse			= 4,
			kBaseStation		= 5,
			kWaterway			= 6,
			kLandCover			= 7,
			kIsland				= 8,
			kRoadLink			= 9,
			kCountry			= 10,
			kProvince			= 11,
			kDistrict			= 12,
			kSubDistrict		= 13,
			kNamedArea			= 14,
			kPoiInfo			= 15,
			kWaterwayLine		= 16,
			kRoadNode			= 17,
			kSegmentAttr		= 18,
			kBuiltUpArea		= 19,
			kTopCountry			= 20,
			kTopProvince		= 21,
			kTopWaterwayLine	= 22,
			kTopWaterwayPolygon	= 23,
			kTopAnnotation		= 24,
			kVillage            = 25,
			kSeaPolygon			= 26,
			kBuildingRoof		= 27,
			kCityDivider		= 28,
			kCityLine			= 29,
			kCityRailWay		= 30,
			kCitySite			= 31,
			kCityTunnel			= 32,
			kPoiInfoRoadNameID	= 33,
			kHnpRoadNameID		= 34,
			kSeaPolygonName		= 35,
			kPoiEntryPoint		= 36,
			kAnnoRoadName		= 1000,
			kAnnoRoadNo			= 1001,
		};
	} // gl_layer_code

	namespace gl_country_code {
		enum ECode {
			kUndefined						= 0,
			kAustralia						= 36,
			kBangladesh						= 50,
			kBhutan							= 64,
			kBruneiDarussalam				= 96,
			kMyanmar						= 104,
			kCambodia						= 116,
			kChina							= 156,
			kTaiwan							= 158,
			kHongKong						= 344,
			kIndia							= 356,
			kIndonesia						= 360,
			kJapan							= 392,
			kLaos							= 418,
			kMacao							= 446,
			kMalaysia						= 458,
			kFederatedStatesofMicronesia	= 583,
			kPalau							= 585,
			kPapuaNewGuinea					= 598,
			kPhilippines					= 608,
			kTimorLeste						= 626,
			kSingapore						= 702,
			kVietnam						= 704,
			kThailand						= 764,
		};
	} // gl_country_code

} // common_table

namespace country_table {
	using namespace ::sindy::schema::global::common_table;

	extern const _TCHAR kCountryCode[];	//!< 国コード（統合DB専用）
} // contry_table

namespace common_feature {
	using namespace ::sindy::schema::global::common_table;

	extern const _TCHAR kShape[];        //!< SHAPE
} // common_feature

namespace country_feature {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kShape[];        //!< SHAPE
} // country_feature

namespace scaledisp_flag {
	extern const _TCHAR kSC1Disp[];      //!< スケール1表示フラグ
	extern const _TCHAR kSC2Disp[];      //!< スケール2表示フラグ
	extern const _TCHAR kSC3Disp[];      //!< スケール3表示フラグ
	extern const _TCHAR kSC4Disp[];      //!< スケール4表示フラグ
} // scaledisp_flag

namespace scale_class {
	extern const _TCHAR kSC1BgClass[];   //!< スケール1背景種別コード
	extern const _TCHAR kSC2BgClass[];   //!< スケール2背景種別コード
	extern const _TCHAR kSC3BgClass[];   //!< スケール3背景種別コード
	extern const _TCHAR kSC4BgClass[];   //!< スケール4背景種別コード
} // scale_class

namespace name_class {
	extern const _TCHAR kNameID[];       //!< 名称ID
} // name_class

// 建物ポリゴンテーブル
namespace building {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kBldClass[];     //!< 建物種別コード
	extern const _TCHAR kCorridor[];     //!< 渡り廊下フラグ
	extern const _TCHAR kNoWall[];       //!< 無壁舎フラグ
	extern const _TCHAR kGeospaceID[];   //!< GEOSPACE_ID

	namespace gl_bld_class {
		enum ECode {
			kUndefined					= 0,     //!< 未定義
			kTrain						= 1,     //!< 地下鉄以外駅（地上）
			kSubway						= 2,     //!< 地下鉄駅（地上）
			kGovernmentPublic			= 5,     //!< 官公庁
			kHospital					= 6,     //!< 病院
			kSchool						= 7,     //!< 学校
			kResidential				= 8,     //!< 住宅
			kOtherFacility				= 9,     //!< その他施設
			kHotel						= 11,    //!< ホテル
			kPedestrianDeck				= 15,    //!< 歩行者用デッキ
			kIndustrialBusiness			= 17,    //!< 工場
			kTramwayStation				= 19,    //!< 路面鉄道駅(地上)
			kFootbridge					= 20,    //!< 歩道橋
			kStairsPedestrianDeck		= 23,    //!< ぺデストリアンデッキ（階段・エスカレータ）
			kStairsInsideThePremises	= 25,    //!< 敷地階段
			kTollGate					= 27,    //!< 料金所
			kNewTransportSystem			= 28,    //!< 新交通駅（地上）
			kMonorailStation			= 29,    //!< モノレール駅（地上）
			kCablecarStation			= 30,    //!< ケーブルカー駅（地上）
			kTransportation				= 100,   //!< 運送
			kCulturalLeisure			= 101,   //!< 文化レジャー
			kCommercial					= 102,   //!< 商業
			kReligious					= 103,   //!< 宗教
			kBank						= 104,   //!< 銀行
		};
	} // bld_class
} // building

// 建物段差ポリゴンテーブル
namespace building_step {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kFloors[];       //!< 階数
	extern const _TCHAR kCheck[];        //!< 階数情報確認フラグ
	extern const _TCHAR kBuildingID[];   //!< 建物ID
} // building_step

// 建物屋上施設ポリゴンテーブル
namespace building_roof {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kRfClassC[];     //!< 建物屋上施設種別コード

	namespace gl_bldroof_class {
		enum ECode {
			kUndefined		= 0,	//!< 未定義
			kPravedArea		= 1,	//!< 舗装地その他敷地（屋上施設）
			kGreenUrbanArea = 2,	//!< 植栽地（屋上施設）
			kPool			= 3,	//!< プール（屋上施設）
		};
	} // gl_bldroof_class

} // building_roof

  // 建物ラインテーブル
namespace building_line {
	using namespace sindy::schema::global::common_feature;
	using namespace sindy::schema::global::building::gl_bld_class;
	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kBldClass[];     //!< 建物種別コード
} // building_line


// 都市地図背景ポリゴンテーブル
namespace city_site {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kBgClass[];      //!< 背景ポリゴン種別コード

	namespace gl_bgpolygon_class{
		enum ECode {
			kUndefined				= 0,	//!< 未定義
			kUnspecified			= 1,	//!< 街区
			kSandySoil				= 10,	//!< 砂地
			kSwamp					= 11,	//!< 湿地
			kForest					= 12,	//!< 山林
			kGrass					= 13,	//!< 芝生等その他緑地
			kGreenUrbanArea			= 14,	//!< 植栽地
			kRiver					= 20,	//!< 河川
			kRiverside				= 21,	//!< 堤外地
			kPool					= 22,	//!< プール
			kOcean					= 23,	//!< 海
			kPondOrLake				= 24,	//!< 池・湖
			kExpressway				= 30,	//!< 都市間高速道路（有料）
			kMainRoad				= 31,	//!< 最重要道路
			kSidewalk				= 34,	//!< 歩道
			kPathway				= 35,	//!< 一般重要道路
			kDividerNomal			= 37,	//!< 中央分離帯（標準）
			kStairs					= 39,	//!< 公道階段
			kHospital				= 40,	//!< 病院
			kSchool					= 41,	//!< 学校
			kFactory				= 42,	//!< 工場
			kDam					= 45,	//!< ダム
			kRailwayGround			= 50,	//!< 鉄道敷地
			kPavedArea				= 51,	//!< 舗装地その他敷地
			kBreakwater				= 52,	//!< 防波堤
			kMilitaryGround			= 55,	//!< 軍用地
			kIncompleteArea			= 60,	//!< 不完全エリア
			kDividerGreen			= 75,	//!< 中央分離帯（植栽地）
			kRunway					= 76,	//!< 滑走路
			kUnderConstructionRoad	= 77,	//!< 工事中道路
			kOrdinaryRoad			= 79,	//!< 細街路
		};
	} // gl_bgpolygon_class
} // city_site

// 都市地図背景ラインテーブル
namespace city_line {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBgClass[];				//!< 背景ライン種別コード

	namespace gl_bgline_class {
		enum ECode {
			kUndefined					=        0, //!< 未調査・作業中
			kDecorative					=        1, //!< 装飾線
			kTunnel		 				=        2, //!< トンネル
			kTollGate					=        3, //!< 料金所
			kFloodGate 					=        4, //!< 水門
			kFerryRoute 				=       10, //!< フェリー航路
			kOtherPassengerBoatRoute	=       11, //!< その他旅客船航路
			kStairsDecorative			=		15,	//!< 階段装飾線
			kInitial					=		16, //!< 親線
			kStairsTread				=		17, //!< 階段踏面ライン（公道階段）
			kDivider					=		20,	//!< 中央分離帯（ライン）
			kRoadBackground				=		21,	//!< 道路背景
			kExpressway					=		22,	//!< 高速道路
			kRoadOverpass				=		23,	//!< 道路高架
			kRailwayOverpass			=		24,	//!< 鉄道高架
			kVehicleStopBarrier			=		25, //!< 車止めライン
		};
	} // gl_bgline_class
} // city_line

// 都市トンネル坑口
namespace city_tunnel {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kTunnelClass[];			//!< トンネル種別コード
} // city_tunnel

// 都市鉄道ラインテーブル
namespace city_railway {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kRailWayClassC[];		//!< 鉄道種別

	namespace gl_ug_railway_class {
		enum ECode {
			kUndefined				=        0, //!< 未調査・作業中
			kTrain					=        1, //!< 普通鉄道
			kMetroSubway			=        2, //!< 地下鉄
			kTramway				=        3, //!< 路面鉄道
			kRollercoaster 			=        4, //!< ジェットコースター等
			kNewTransportSystem		=        5, //!< 新交通システム
			kMonorail				=        6, //!< モノレール
			Cablecar				=        7, //!< ケーブルカー
		};
	} // gl_ug_railway_class
} // city_railway

// 都市料金所分離帯ポリゴンテーブル
namespace city_divider {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kDividerClassC[];		//!< 料金所分離帯種別コード

} // city_divider

// 都市地図駅舎ポリゴンテーブル
namespace city_station {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kUnderGround[];  //!< 地下フラグ
	extern const _TCHAR kStationClass[]; //!< 鉄道駅種別コード

	namespace gl_ug_station_class {
		enum ECode {
			kUndefined   = 0,            //!< 未定義
			kNotSubwayUG = 1,            //!< 地下鉄以外駅
			kSubwayUG    = 2,            //!< 地下鉄駅
		};
	} // gl_ug_station_class
} // city_station

/// 中縮背景ラインテーブル
namespace base_line {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scaledisp_flag;
	using namespace ::sindy::schema::global::name_class;
	
	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kUnderGround[];  //!< 地下フラグ
	extern const _TCHAR kBgClass[];      //!< 背景ライン種別コード

	namespace gl_bgline_class {
		enum ECode {
			kUndefined    = 0,           //!< 未定義
			kNoDisplay    = 1,           //!< 非表示
			kRiver        = 2,           //!< 河川
			kOcean        = 3,           //!< 海
			kCanal        = 4,           //!< 水路
			kLake         = 5,           //!< 池
			kWaterUnknown = 9,           //!< 不明水系
		};
	} // gl_bgline_class
} // base_line

/// 中縮背景鉄道ラインテーブル
namespace base_railway {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scaledisp_flag;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kUnderGround[];  //!< 地下フラグ
	extern const _TCHAR kRailwayClass[]; //!< 鉄道種別コード

	namespace gl_railway_class {
		enum ECode {
			kUndefined          = 0,     //!< 未定義
			kTrain              = 1,     //!< 普通鉄道
			kMetroSubway        = 2,     //!< 地下鉄
			kNewTransportSystem = 3,     //!< 新交通
		};
	} // gl_railway_class
} // base_railway

/// LAND USEポリゴンテーブル
namespace land_use {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< テーブル名

	namespace gl_use_class {
		enum ECode {
			kUndefined          = 0,      //!< 未定義
			kNoDisplay          = 1,      //!< 非表示
			kLC7110             = 100,    //!< LC7110(MPA専用)
			kPavedArea          = 201001, //!< 舗装路
			kRailwayGround      = 202001, //!< 鉄道敷地
			kAirportGround      = 203002, //!< 空港敷地
			kAirportRunway      = 203006, //!< 滑走路
			kMilitaryGround     = 303002, //!< 軍事施設
			kPublicFacility     = 304003, //!< 公共施設
			kSchoolGround       = 401010, //!< 学校
			kHospitalGround     = 402001, //!< 病院
			kPark               = 501001, //!< 公園
			kRaceCourse         = 501013, //!< レース場
			kLeisureFacility    = 501019, //!< 娯楽施設
			kCommercialFacility = 502005, //!< 商業施設
			kFootballStadium    = 503001, //!< サッカー場
			kGolfCourseGround   = 503007, //!< ゴルフコース
			kGolfDrivingRange   = 503011, //!< ゴルフ練習場
			kSportsFacility     = 503021, //!< その他スポーツ施設
			kFarm               = 504005, //!< 農場
			kIndustrialBusiness = 601001, //!< 工場
			kCemetery           = 603004, //!< 墓地
			kReligiousFacility  = 603005, //!< 宗教施設
			kOtherFacility      = 604004, //!< その他施設
			kHotel              = 701001, //!< ホテル
			kResidential        = 701101, //!< 住居
		};
	} // gl_use_class
} // land_use

/// 中縮背景駅舎ポリゴンテーブル
namespace base_station {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scaledisp_flag;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kUnderGround[];   //!< 地下フラグ
	extern const _TCHAR kStationClass[];  //!< 鉄道駅種別コード

	namespace gl_station_class {
		enum ECode {
			kUndefined          = 0,      //!< 未定義
			kTrain              = 1,      //!< 普通鉄道
			kMetroSubway        = 2,      //!< 地下鉄
			kNewTransportSystem = 3,      //!< 新交通
		};
	} // gl_station_class
} // base_station

/// WATER WAYポリゴンテーブル
namespace water_way {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< テーブル名

	namespace gl_water_class {
		enum ECode {
			kUndefined  = 0,              //!< 未定義
			kNoDisplay  = 1,              //!< 非表示
			kOcean		= 101001,		  //!< 海洋
			kRiver      = 101002,         //!< 河川
			kLake       = 101003,         //!< 池
			kCanal      = 102003,         //!< 水路
			kHarborPort = 203005,         //!< 港
 		};
	} // gl_water_class
} // water_way

/// WATER WAYラインテーブル
namespace waterway_line {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< テーブル名
	
	extern const _TCHAR kBgClass[];       //!< 背景種別
	namespace gl_bgline_class{
		using namespace ::sindy::schema::global::base_line::gl_bgline_class;
	}
} // waterway_line

/// LAND COVERポリゴンテーブル
namespace land_cover {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< テーブル名

	namespace gl_cover_class {
		enum ECode {
			kUndefined       = 0,         //!< 未定義
			kNoDisplay       = 1,         //!< 非表示
			kSandySoil       = 102001,    //!< 砂地
			kWetland         = 102002,    //!< 湿地
			kGround          = 102005,    //!< グラウンド
			kForest          = 102101,    //!< 森林
			kNationalPark    = 102102,    //!< 国立公園
			kForestPark      = 102103,    //!< 森林公園
			kWildLifeReserve = 102104,    //!< 自然保護区
			kNoHuntingArea   = 102105,    //!< 禁猟区
		};
	} // gl_cover_class
} // land_cover

/// ISLANDポリゴンテーブル
namespace island {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< テーブル名

	namespace gl_island_class {
		enum ECode {
			kUndefined = 0,               //!< 未定義
			kNoDisplay = 1,               //!< 非表示
			kIsland    = 102201,          //!< 島
		};
	} // gl_island_class
} // island

/// メッシュにひもづくテーブル
namespace mesh_code_table {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kMeshCode[];      //!< メッシュコード
} // mesh_code_table

// 始終点がノードとひもづくテーブル
namespace from_to_node_relational_table {
	extern const _TCHAR kFromNodeID[];    //!< 始点側ノードのOBJECTID
	extern const _TCHAR kToNodeID[];      //!< 終点側ノードのOBJECTID
} // from_to_node_relational_table

// ハイウェイの路線に関連付くテーブル
namespace highway_road_relational_table {
	extern const _TCHAR kRoadCode[];      //!< 路線コード
} // highway_road_relational_table

// 調査
namespace fieldsurvey_table {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kFieldSurvey[];   //!< 現地調査フラグ
	extern const _TCHAR kWorkYear[];      //!< 現地調査年度
} // fieldsurvey_table

// リンクにひもづくテーブル
namespace link_relational_table {
	extern const _TCHAR kLinkID[];        //!< リンクID
} // link_relational_table

/// リンクとその向きにひもづくテーブル
namespace directional_link_relational_table {
	using namespace ::sindy::schema::global::link_relational_table;

	extern const _TCHAR kLinkDir[];       //!< リンク方向コード

	/// リンク方向コード
	namespace gl_link_dir {
		enum ECode {
			kPositive = 1, //!< 順方向
			kNegative = 2, //!< 逆方向
		};
	}
} // directional_link_relational_table

// リンク列
namespace link_queue {
	using namespace ::sindy::schema::global::mesh_code_table;
	using namespace ::sindy::schema::global::directional_link_relational_table;

	extern const _TCHAR kInfID[];      //!< リンク列ID
	extern const _TCHAR kSequence[];   //!< リンク列順番
	extern const _TCHAR kLastLink[];   //!< 終端リンクフラグ
} // link_queue

/// 時間規制条件テーブル
namespace time_condition_table {
	extern const _TCHAR kStartMonth[]; //!< 規制条件:開始月
	extern const _TCHAR kStartDay[];   //!< 規制条件:開始日
	extern const _TCHAR kStartHour[];  //!< 規制条件:開始時間
	extern const _TCHAR kStartMin[];   //!< 規制条件:開始分
	extern const _TCHAR kEndMonth[];   //!< 規制条件;終了月
	extern const _TCHAR kEndDay[];     //!< 規制条件:終了日
	extern const _TCHAR kEndHour[];    //!< 規制条件:終了時間
	extern const _TCHAR kEndMin[];     //!< 規制条件:終了分
	extern const _TCHAR kDayOfWeek[];  //!< 規制条件:曜日指定
} // time_condition_table

// 道路リンクフィーチャクラス
namespace road_link {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::from_to_node_relational_table;
	using namespace ::sindy::schema::global::highway_road_relational_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];
	
	extern const _TCHAR kRoadClass[];     //!< 道路表示用種別
	extern const _TCHAR kNaviClass[];     //!< 道路経路用種別
	extern const _TCHAR kMainLinkClass[]; //!< リンク種別（主）
	extern const _TCHAR kAdminCode[];     //!< 行政区域コード
	extern const _TCHAR kRoadManager[];   //!< 管理者コード
	extern const _TCHAR kRoadWidth[];     //!< 道路幅員区分
	extern const _TCHAR kBypass[];        //!< バイパスフラグ
	extern const _TCHAR kCarOnly[];       //!< 自動車専用道路フラグ
	extern const _TCHAR kWideArea[];      //!< 広域存在フラグ
	extern const _TCHAR kIsland[];        //!< 離島フラグ
	extern const _TCHAR kRoadNo[];        //!< 道路番号(国内用)
	extern const _TCHAR kRoadNoString[];  //!< 道路番号(海外用)
	extern const _TCHAR kRoadNoString2[]; //!< 道路番号2(海外用)
	extern const _TCHAR kUpDownClass[];   //!< 上下線種別コード
	extern const _TCHAR kNoPassage[];     //!< 通行禁止種別コード
	extern const _TCHAR kOneway[];        //!< 一方通行種別コード
	extern const _TCHAR kKubun[];         //!< 主従区分コード
	extern const _TCHAR kTripSpeed[];     //!< 旅行速度
	extern const _TCHAR kLimitSpeed[];    //!< 規制速度コード
	extern const _TCHAR kLaneCount[];     //!< 車線数
	extern const _TCHAR kLaneValid[];     //!< 片側車線数有効フラグ
	extern const _TCHAR kRightChange[];   //!< 車線数右増減
	extern const _TCHAR kLeftChange[];    //!< 車線数左増減
	extern const _TCHAR kAlleyMainte[];   //!< 超細街路作業年度情報ID
	extern const _TCHAR kNoPassageRF[];   //!< 条件付通行禁止関係フラグ
	extern const _TCHAR kOnewayRF[];      //!< 条件付一方通行関係フラグ
	extern const _TCHAR kDupliLinkRF[];   //!< 重用路線情報関係フラグ
	extern const _TCHAR kTurnRegLQRF[];   //!< 右左折禁止リンク列関係フラグ
	extern const _TCHAR kDirGuideLQRF[];  //!< 方面案内リンク列関係フラグ
	extern const _TCHAR kGuideLQRF[];     //!< 強制誘導リンク列関係フラグ
	extern const _TCHAR kLaneLQRF[];      //!< レーン情報リンク列関係フラグ
	extern const _TCHAR kVICSLQRF[];      //!< VICSリンク列関係フラグ
	extern const _TCHAR kBywayLQRF[];     //!< 抜け道情報リンク列関係フラグ
	extern const _TCHAR kRouteLQRF[];     //!< ルート情報リンク列関係フラグ
	extern const _TCHAR kUturnLQRF[];     //!< Uターン可能箇所情報リンク列関係フラグ
	extern const _TCHAR kInternaviLQRF[]; //!< インターナビVICSリンク列関係フラグ
	extern const _TCHAR kIpcVicsLQRF[];   //!< iPC-VICSリンク列関係フラグ
	extern const _TCHAR kIntersectionLQRF[]; //!< 進入方向別交差点リンク列関係フラグ

	extern const _TCHAR kParkingLinkClass[]; //!< 駐車場リンク種別
	extern const _TCHAR kFloorLevel[];       //!< 駐車場階層レベル
	extern const _TCHAR kMiddleFloorLevel[]; //!< 駐車場中階層レベル

	extern const _TCHAR kLegalSpeed[];    //!< 最高速度コード
	extern const _TCHAR kLegalSpeedVerified[]; //!< 最高速度確認コード

	extern const _TCHAR kVehicleTurnRegLQRF[]; //!< 乗り物別右左折禁止リンク列関係フラグ
	extern const _TCHAR kVehicleRegRF[];       //!< 乗り物別規制フラグ

	extern const _TCHAR kFromHNLeft[];    //!< LeftFromのハウスナンバー
	extern const _TCHAR kToHNLeft[];      //!< LeftToのハウスナンバー
	extern const _TCHAR kFromHNRight[];   //!< RightFromのハウスナンバー
	extern const _TCHAR kToHNRight[];     //!< RightToのハウスナンバー
	extern const _TCHAR kHNSchemeLeft[];  //!< Leftのハウスナンバースキーム
	extern const _TCHAR kHNSchemeRight[]; //!< Rightのハウスナンバースキーム
	extern const _TCHAR kHNType[];        //!< ハウスナンバー種別

	// 道路表示用種別コード
	namespace gl_disp_class {
		enum ECode {
			kHighImportanceMajorRoads = 1,   //!< 最重要道路
			kOtherMajorRoads          = 2,   //!< その他重要道路
			kSecondaryRoads           = 4,   //!< サブ道路
			kLocalConnectingRoads     = 5,   //!< その他接続道路
			kLocalRoads               = 6,   //!< その他道路
			kLocalRoadsMinor          = 7,   //!< その他格下道路
			kNarrowRoads1             = 8,   //!< 細街路L1
			kNarrowRoads2             = 9,   //!< 細街路L2
			kMotorways                = 11,  //!< 高速道路
			kTollMotorways            = 101, //!< 高速道路（有料）
			kFerry                    = 202, //!< フェリー
			kFerryNonRoutable         = 205, //!< フェリー（経路外）
			kPrivateRoads             = 302, //!< 私道
			kInSiteRoads              = 305, //!< 施設内道路
			kWalkway                  = 601, //!< 歩道

		};
	} // gl_disp_class

	// 道路経路用種別コード
	namespace gl_navi_class {
		enum ECode {
			kNonNavigableRoads        = 0,   //!< 経路外道路（L1、私道等）
			kHighImportanceMajorRoads = 1,   //!< 最重要メイン道路
			kOtherMajorRoads          = 2,   //!< その他メイン道路
			kSecondaryRoads           = 4,   //!< サブ道路
			kLocalConnectingRoads     = 5,   //!< その他接続道路
			kLocalRoads               = 6,   //!< その他道路
			kNonRoutableRoads         = 99,  //!< 経路外道路（L2、歩道）
			kMotorways                = 101, //!< 高速
		};
	} // gl_navi_class

	// リンク種別コード
	namespace gl_link_class {
		enum ECode {
			kUndefined          = 0,   //!< 未定義
			kMainSingleRoadLink = 1,   //!< 本線（上下線非分離）リンク
			kMainMultiRoadLink  = 2,   //!< 本線（上下分離）リンク
			kJunctionLink       = 3,   //!< ジャンクションリンク
			kIntersectionLink   = 4,   //!< 交差点内リンク
			kRampLink           = 5,   //!< 連結路（ランプ）リンク
			kFrontageRoadLink   = 6,   //!< 側道リンク
			kSAetcFrontageLink  = 7,   //!< ＳＡなど側線リンク
			kConnectionLink     = 9,   //!< 接続リンク
			kRoundAbout         = 101, //!< ラウンドアバウト
			kEntranceRoad       = 102, //!< 入口リンク
			kPartOfParking      = 103, //!< 駐車場リンク
			kPartOfWalkway      = 104, //!< 歩道リンク
		};
	} // gl_link_class

	// 道路幅員区分コード
	namespace gl_road_width {
		enum ECode {
			kUndefined       = 0, //!< 未調査
			kOver130         = 1, //!< 幅員13.0m以上
			kBetween55And130 = 2, //!< 幅員5.5m以上13.0m未満
			kBetween30And55  = 3, //!< 幅員3.0m以上5.5m未満
			kUnder30         = 4, //!< 幅員3.0m未満
		};
	} // gl_road_width

	// 通行禁止種別コード
	namespace gl_no_passage {
		enum ECode {
			kNotclosed                   = 0, //!< 通行禁止なし
			kClosedNoCond                = 1, //!< 条件なし通行禁止
			kClosedWithCond              = 2, //!< 条件付き通行禁止
			kClosedNoCondExcAuthorized   = 4, //!< 条件なし通行禁止(関係者以外)
			kClosedWithCondExcAuthorized = 5, //!< 条件付き通行禁止(関係者以外)
		};
	} // gl_no_passage

	// 一方通行種別コード
	namespace gl_oneway {
		enum ECode {
			kNoOneway                   = 0, //!< 一方通行なし
			kOnewayNoCondOpenPositive   = 1, //!< 条件なし一方通行(正方向)
			kOnewayNoCondOpenNegative   = 2, //!< 条件なし一方通行(逆方向)
			kOnewayWithCondOpenPositive = 3, //!< 条件付き一方通行(正方向)
			kOnewayWithCondOpenNegative = 4, //!< 条件付き一方通行(逆方向)
			kOnewayWithCondDependOnCond = 5, //!< 条件付き一方通行(正逆)
		};
	} // gl_oneway

	// 最高速度コード
	namespace gl_max_speed {
		enum ECode
		{
			kUndefined = 0,
			k5         = 5,
			k10        = 10,
			k15        = 15,
			k20        = 20,
			k25        = 25,
			k30        = 30,
			k35        = 35,
			k40        = 40,
			k45        = 45,
			k50        = 50,
			k55        = 55,
			k60        = 60,
			k65        = 65,
			k70        = 70,
			k75        = 75,
			k80        = 80,
			k85        = 85,
			k90        = 90,
			k95        = 95,
			k100       = 100,
			k105       = 105,
			k110       = 110,
			k115       = 115,
			k120       = 120,
			kNon       = 999,
		};
	} // gl_max_speed

	// 最高速度確認コード
	namespace gl_speed_verified {
		enum ECode
		{
			kUnverified = 0, // 最高速度標識　未確認
			kVerified   = 1, // 最高速度標識　有り確認
			kLegal      = 2, // 最高速度標識　無し確認(法定速度整備)
		};
	} // gl_verified

	// ハウスナンバーレンジスキーマ
	namespace hnr_scheme
	{
		enum ECode
		{
			kUndefined = 0,  //!< Undefined
			kEven      = 1,  //!< Even
			kOdd       = 2,  //!< Odd
			kMixed     = 3,  //!< Mixed
		};
	}

	// ハンスナンバータイプ
	namespace hn_type
	{
		enum ECode
		{
			kBase = 0,  //!< Base
			kOld  = 1,  //!< Old
		};
	}

} // road_link

// 道路ノードテーブル
namespace road_node {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];  //!< テーブル名

	extern const _TCHAR kNodeClass[];  ///< ノード種別
	extern const _TCHAR kSignal[];     //!< 信号機コード
	extern const _TCHAR kNameKanji[];  //!< 交差点漢字名称
	extern const _TCHAR kNameYomi[];   //!< 交差点カナ名称
	extern const _TCHAR kHeightCode[]; //!< 高さコード 
	extern const _TCHAR kHeight[];     //!< 高さ情報
	extern const _TCHAR kParkingID[];  //!< 駐車場ＩＤ

	/// ノード種別コード
	namespace gl_node_class {
		enum ECode {
			kNoFunction           = 0,  //!< 機能無しノード
			kCross                = 1,  //!< 交差点ノード
			kEdge                 = 2,  //!< 端点ノード
			kAttributeChangePoint = 3,  //!< 属性変化点ノード
			kAttributed           = 4,  //!< 属性付きノード
			kOnSecondaryMesh      = 9,  //!< 2次メッシュ区画上ノード
			kAvoidDuplicateLink   = 10, //!< ループリンク回避ノード
			KBorderPoint          = 999,//!< 国境ノード
		};
	} // gl_node_class

	/// 信号機コード
	namespace gl_signal_class {
		enum ECode {
			kNotPresent = 0,  //!< 信号機なし
			kPresent    = 1,  //!< 信号機あり
		};
	} // gl_signal_class

} // road_node

// リンク内属性フィーチャクラス
namespace segment_attr {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::link_relational_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kUnderOver[]; ///< 上下関係
	extern const _TCHAR kLinkAttr[]; ///< リンク内属性種別コード

	// リンク内属性コード
	namespace gl_link_attr {
		enum ECode {
			kUndefined         = 0, //!< 未定義
			kBridge            = 1, //!< 橋・高架
			kTunnel            = 2, //!< トンネル
			kCave              = 3, //!< 洞穴道
			kRailroadCrossings = 4, //!< 踏切
			kUnderpass         = 5, //!< アンダーパス
			kOverpass          = 6, //!< オーバーパス
			kUnpaved           = 7, //!< 未舗装
		};
	} // gl_link_attr
} // segment_attr

// 曜日指定コードドメイン
namespace day_code {
	enum ECode {
		kSunHolidays         =  1, //!< 日・休日
		kNotSunHolidays      =  2, //!< 日・休日をのぞく
		kHolidaysExcSun      =  3, //!< 休日(日曜はのぞく)
		kNotHolidays         =  4, //!< 休日をのぞく
		kWeekday             =  5, //!< 平日
		kSun                 =  6, //!< 日
		kMon                 =  7, //!< 月
		kTue                 =  8, //!< 火
		kWed                 =  9, //!< 水
		kThu                 = 10, //!< 木
		kFri                 = 11, //!< 金
		kSat                 = 12, //!< 土
		kSatandSun           = 13, //!< 土日
		kNotSatSunHoliday    = 14, //!< 土日・休日をのぞく
		kAllDays             = 15, //!< 全曜日
		kNotSun              = 16, //!< 日曜日をのぞく
		kDaybeforeSunHoliday = 17, //!< 日・休日の前日
	};
} // day_code

// 右左折禁止リンク列Infテーブル
namespace inf_turnreg {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kStartMonth0[];  //!< 規制条件:開始月
	extern const _TCHAR kStartDay0[];    //!< 規制条件:開始日
	extern const _TCHAR kStartHour0[];   //!< 規制条件:開始時間
	extern const _TCHAR kStartMin0[];    //!< 規制条件:開始分
	extern const _TCHAR kEndMonth0[];    //!< 規制条件;終了月
	extern const _TCHAR kEndDay0[];      //!< 規制条件:終了日
	extern const _TCHAR kEndHour0[];     //!< 規制条件:終了時間
	extern const _TCHAR kEndMin0[];      //!< 規制条件:終了分
	extern const _TCHAR kDayOfWeek0[];   //!< 規制条件:曜日指定

	extern const _TCHAR kStartMonth1[];  //!< 規制条件1:開始月
	extern const _TCHAR kStartDay1[];    //!< 規制条件1:開始日
	extern const _TCHAR kStartHour1[];   //!< 規制条件1:開始時間
	extern const _TCHAR kStartMin1[];    //!< 規制条件1:開始分
	extern const _TCHAR kEndMonth1[];    //!< 規制条件1;終了月
	extern const _TCHAR kEndDay1[];      //!< 規制条件1:終了日
	extern const _TCHAR kEndHour1[];     //!< 規制条件1:終了時間
	extern const _TCHAR kEndMin1[];      //!< 規制条件1:終了分
	extern const _TCHAR kDayOfWeek1[];   //!< 規制条件1:曜日指定

	extern const _TCHAR kStartMonth2[];  //!< 規制条件2:開始月
	extern const _TCHAR kStartDay2[];    //!< 規制条件2:開始日
	extern const _TCHAR kStartHour2[];   //!< 規制条件2:開始時間
	extern const _TCHAR kStartMin2[];    //!< 規制条件2:開始分
	extern const _TCHAR kEndMonth2[];    //!< 規制条件2;終了月
	extern const _TCHAR kEndDay2[];      //!< 規制条件2:終了日
	extern const _TCHAR kEndHour2[];     //!< 規制条件2:終了時間
	extern const _TCHAR kEndMin2[];      //!< 規制条件2:終了分
	extern const _TCHAR kDayOfWeek2[];   //!< 規制条件2:曜日指定

	extern const _TCHAR kStartMonth3[];  //!< 規制条件3:開始月
	extern const _TCHAR kStartDay3[];    //!< 規制条件3:開始日
	extern const _TCHAR kStartHour3[];   //!< 規制条件3:開始時間
	extern const _TCHAR kStartMin3[];    //!< 規制条件3:開始分
	extern const _TCHAR kEndMonth3[];    //!< 規制条件3;終了月
	extern const _TCHAR kEndDay3[];      //!< 規制条件3:終了日
	extern const _TCHAR kEndHour3[];     //!< 規制条件3:終了時間
	extern const _TCHAR kEndMin3[];      //!< 規制条件3:終了分
	extern const _TCHAR kDayOfWeek3[];   //!< 規制条件3:曜日指定

	extern const _TCHAR kStartMonth4[];  //!< 規制条件4:開始月
	extern const _TCHAR kStartDay4[];    //!< 規制条件4:開始日
	extern const _TCHAR kStartHour4[];   //!< 規制条件4:開始時間
	extern const _TCHAR kStartMin4[];    //!< 規制条件4:開始分
	extern const _TCHAR kEndMonth4[];    //!< 規制条件4;終了月
	extern const _TCHAR kEndDay4[];      //!< 規制条件4:終了日
	extern const _TCHAR kEndHour4[];     //!< 規制条件4:終了時間
	extern const _TCHAR kEndMin4[];      //!< 規制条件4:終了分
	extern const _TCHAR kDayOfWeek4[];   //!< 規制条件4:曜日指定

	extern const LPCTSTR kStartMonth[5]; //!< 規制条件:開始月一覧
	extern const LPCTSTR kStartDay[5];   //!< 規制条件:開始日一覧
	extern const LPCTSTR kStartHour[5];  //!< 規制条件:開始時間一覧
	extern const LPCTSTR kStartMin[5];   //!< 規制条件:開始分一覧
	extern const LPCTSTR kEndMonth[5];   //!< 規制条件;終了月一覧
	extern const LPCTSTR kEndDay[5];     //!< 規制条件:終了日一覧
	extern const LPCTSTR kEndHour[5];    //!< 規制条件:終了時間一覧
	extern const LPCTSTR kEndMin[5];     //!< 規制条件:終了分一覧
	extern const LPCTSTR kDayOfWeek[5];  //!< 規制条件:曜日指定一覧

	using namespace ::sindy::schema::global::day_code;

	struct where_all_year_all_days {};

	template <typename CharT>
	inline std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<CharT>& os, where_all_year_all_days)
	{
		using namespace atl2::io::compatible;
		return os << kStartMonth1 << L" IS NULL";
	}
} // inf_turnreg

// 右左折禁止リンク列LQテーブル
namespace lq_turnreg {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // lq_turnreg

// 車種別右左折禁止リンク列Infテーブル
namespace inf_vehicle_turnreg {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];    //!< テーブル名
}; // inf_vehicle_turnreg

// 車種別右左折禁止リンク列LQテーブル
namespace lq_vehicle_turnreg {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // lq_vehicle_turnreg

// 車種別右左折禁止リンク列規制テーブル
namespace vehicle_turnreg {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kInfID[];        //!< 車種別右左折禁止ID
	extern const _TCHAR kHeight[];       //!< 高さ制限
	extern const _TCHAR kWidth[];        //!< 最大幅
	extern const _TCHAR kWeight[];       //!< 重量制限
	extern const _TCHAR kLength[];       //!< 車長
	extern const _TCHAR kLoadAge[];      //!< 積載量
	extern const _TCHAR kDangerF[];      //!< 危険物フラグ

	extern const _TCHAR kStartMonth[];   //!< 規制条件:開始月
	extern const _TCHAR kStartDay[];     //!< 規制条件:開始日
	extern const _TCHAR kStartHour[];    //!< 規制条件:開始時間
	extern const _TCHAR kStartMin[];     //!< 規制条件:開始分
	extern const _TCHAR kEndMonth[];     //!< 規制条件;終了月
	extern const _TCHAR kEndDay[];       //!< 規制条件:終了日
	extern const _TCHAR kEndHour[];      //!< 規制条件:終了時間
	extern const _TCHAR kEndMin[];       //!< 規制条件:終了分
	extern const _TCHAR kDayOfWeek[];    //!< 規制条件:曜日指定
} // vehicle_turnreg

// 条件付き通行禁止テーブル
namespace time_nopassage {
	using namespace ::sindy::schema::global::mesh_code_table;
	using namespace ::sindy::schema::global::link_relational_table;
	using namespace ::sindy::schema::global::time_condition_table;

	extern const _TCHAR kSeasonReg[]; ///< 季節規制フラグ

	extern const _TCHAR kTableName[]; ///< テーブル名
} // time_nopassage

// 条件付き一方通行テーブルフィールド
namespace time_oneway {
	using namespace ::sindy::schema::global::mesh_code_table;
	using namespace ::sindy::schema::global::directional_link_relational_table;
	using namespace ::sindy::schema::global::time_condition_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // time_oneway

// 車種別規制テーブル
namespace vehicle_reg {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kLinkID[];       //!< RoadLinkID
	extern const _TCHAR kRegTypeClass[]; //!< 規制タイプコード
	extern const _TCHAR kHeight[];       //!< 高さ制限
	extern const _TCHAR kWidth[];        //!< 最大幅
	extern const _TCHAR kWeight[];       //!< 重量制限
	extern const _TCHAR kLength[];       //!< 車長
	extern const _TCHAR kLoadAge[];      //!< 積載量
	extern const _TCHAR kDangerF[];      //!< 危険物フラグ

	extern const _TCHAR kStartMonth[];   //!< 規制条件:開始月
	extern const _TCHAR kStartDay[];     //!< 規制条件:開始日
	extern const _TCHAR kStartHour[];    //!< 規制条件:開始時間
	extern const _TCHAR kStartMin[];     //!< 規制条件:開始分
	extern const _TCHAR kEndMonth[];     //!< 規制条件;終了月
	extern const _TCHAR kEndDay[];       //!< 規制条件:終了日
	extern const _TCHAR kEndHour[];      //!< 規制条件:終了時間
	extern const _TCHAR kEndMin[];       //!< 規制条件:終了分
	extern const _TCHAR kDayOfWeek[];    //!< 規制条件:曜日指定

	// 規制タイプコード
	namespace gl_reg_type_class {
		enum ECode {
			kOnewayOpenPositive = 0,     //!< 一方通行（正方向）
			kOnewayOpenNegative = 1,     //!< 一方通行（逆方向）
			kClosed             = 2,     //!< 通行禁止
		};
	} // gl_reg_type_class
} // vehicle_reg

namespace vehicle_type {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];      //!< テーブル名

	extern const _TCHAR kRegID[];          //!< VehicleRegID
	extern const _TCHAR kVehicleRegCode[]; //!< 車種別規制コード
	extern const _TCHAR kExcludeF[];       //!< 除外フラグ
} // vehicle_type

namespace vehicle_reg_master {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];         //!< テーブル名

	extern const _TCHAR kVehicleRegCode[];    //!< 規制コード
	extern const _TCHAR kVehicleRegName[];    //!< 規制名称
	extern const _TCHAR kPassengerF[];        //!< 乗用車フラグ
	extern const _TCHAR kTaxiF[];             //!< タクシーフラグ
	extern const _TCHAR kPublicBusF[];        //!< 公共バス
	extern const _TCHAR kPrivateBusF[];       //!< 私用バス
	extern const _TCHAR kTransportTruckF[];   //!< トラック
	extern const _TCHAR kSignageIllustFile[]; //!< 規制画像名
} // vehicle_reg_master


/// 方面案内リンク列Infテーブル
namespace inf_dirguide {
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];   //!< テーブル名

	extern const int kMaxNumber;        //!< 最大方面案内数

	extern const _TCHAR kGuideClass0[]; //!< 方面種別
	extern const _TCHAR kNameKanji0[];  //!< 方面名称
	extern const _TCHAR kNameYomi0[];   //!< 方面ヨミ

	extern const _TCHAR kGuideClass1[]; //!< 方面種別1
	extern const _TCHAR kNameKanji1[];  //!< 方面名称1
	extern const _TCHAR kNameYomi1[];   //!< 方面ヨミ1

	extern const _TCHAR kGuideClass2[]; //!< 方面種別2
	extern const _TCHAR kNameKanji2[];  //!< 方面名称2
	extern const _TCHAR kNameYomi2[];   //!< 方面ヨミ2

	extern const _TCHAR kGuideClass3[]; //!< 方面種別3
	extern const _TCHAR kNameKanji3[];  //!< 方面名称3
	extern const _TCHAR kNameYomi3[];   //!< 方面ヨミ3

	extern const _TCHAR kGuideClass4[]; //!< 方面種別4
	extern const _TCHAR kNameKanji4[];  //!< 方面名称4
	extern const _TCHAR kNameYomi4[];   //!< 方面ヨミ4

	extern const _TCHAR kGuideClass5[]; //!< 方面種別5
	extern const _TCHAR kNameKanji5[];  //!< 方面名称5
	extern const _TCHAR kNameYomi5[];   //!< 方面ヨミ5

	extern const _TCHAR kGuideClass6[]; //!< 方面種別6
	extern const _TCHAR kNameKanji6[];  //!< 方面名称6
	extern const _TCHAR kNameYomi6[];   //!< 方面ヨミ6

	extern const _TCHAR kGuideClass7[]; //!< 方面種別7
	extern const _TCHAR kNameKanji7[];  //!< 方面名称7
	extern const _TCHAR kNameYomi7[];   //!< 方面ヨミ7

	extern const _TCHAR kGuideClass8[]; //!< 方面種別8
	extern const _TCHAR kNameKanji8[];  //!< 方面名称8
	extern const _TCHAR kNameYomi8[];   //!< 方面ヨミ8

	extern const _TCHAR kGuideClass9[]; //!< 方面種別9
	extern const _TCHAR kNameKanji9[];  //!< 方面名称9
	extern const _TCHAR kNameYomi9[];   //!< 方面ヨミ9

	extern const _TCHAR kGuideClass10[]; //!< 方面種別10
	extern const _TCHAR kNameKanji10[];  //!< 方面名称10
	extern const _TCHAR kNameYomi10[];   //!< 方面ヨミ10

	extern const LPCTSTR kGuideClass[11]; //!< 方面種別一覧
	extern const LPCTSTR kNameKanji[11];  //!< 方面名称一覧
	extern const LPCTSTR kNameYomi[11];   //!< 方面ヨミ名称一覧

	extern const _TCHAR kGuidePattern[]; //!< 方面案内パターン
	extern const _TCHAR kSequence[];     //!< シーケンス

	/// 方面案内種別コード
	namespace guide_class {
		enum ECode {
			kGuide     = 51, //!< Guide
			kRouteNum  = 52, //!< RouteNumber
			kExitNum   = 53, //!< ExitNumber
			kPictogram = 54, //!< Pictogram
		};
	}; // guide_class

	/// 方面案内パターンコード
	namespace guide_pattern {
		enum ECode {
			kNoPattern  =  0, //!< パターン無し
			kGreen      = 51, //!< Green
			kBlue       = 52, //!< Blue
			kWhite      = 53, //!< White
			kOrange     = 54, //!< Orange
			kUndefined  = 99, //!< 未調査
		};
	}; // guide_pattern

	/// 方面種別コード
	namespace destination_type {
		enum ECode {
			kGuideLocal                   = 1,  //!< Guide(Local)
			kGuideToll                    = 2,  //!< Guide(Toll)
			kNationalRoadRouteNumber      = 3,  //!< NationalRoadRouteNumber 
			kPrefecturalHighwayNumber     = 4,  //!< PrefecturalHighwayNumber
			kOther                        = 5,  //!< Other
			kTollCharacterString          = 12, //!< Toll(Character String)
			kTollNationalRoadNumber       = 13, //!< Toll(National Road Number)
			kTollPrefecturalHighwayNumber = 14, //!< Toll(Prefectural Highway Number)
		};
	} // destination_type

} // inf_dirguide

/// 強制誘導リンク列Infテーブル
namespace inf_guide {
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kGuideType[]; //!< 強制誘導案内コード
	extern const _TCHAR kDelete[];    //!< 強制誘導削除フラグ

	/// 強制誘導案内コード
	namespace guide_type {
		enum ECode {
			kNotSet     =  0, ///< 未設定
			kNoGuide    =  1, ///< 道なり（案内なし）
			k12         =  2, ///< 直進（１２時方向）
			k1          =  3, ///< 右前方向（１時方向）
			k2          =  4, ///< 右手前方向（２時方向）
			k3          =  5, ///< 右折（３時方向）
			k4          =  6, ///< 右斜め後ろ方向（４時方向）
			k5          =  7, ///< 右戻る方向（５時方向）
			k6          =  8, ///< 戻る方向（６時方向）
			k7          =  9, ///< 左戻る方向（７時方向）
			k8          = 10, ///< 左斜め後ろ方向（８時方向）
			k9          = 11, ///< 左折（９時方向）
			k10         = 12, ///< 左手前方向（１０時方向）
			k11         = 13, ///< 左前方向（１１時方向）
		};
	} // guide_type

} // inf_guide

/// レーンリンク列Infテーブル
namespace inf_lane {
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kLaneDir[];      //!< レーン方向コード
	extern const _TCHAR kApproachLane[]; //!< 交差点直近車線数
	extern const _TCHAR kRightChange[];  //!< 右側増減車線数
	extern const _TCHAR kLeftChange[];   //!< 左側増減車線数
	extern const _TCHAR kFlowLane[];     //!< 流出車線数
	extern const _TCHAR kLanePassable[]; //!< 車線通行状態

	/// レーン方向コード
	namespace lane_direction {
		enum ECode {
			kUndefined = 0, //!< 未設定
			k0         = 1, //!< 前方向(0)
			k45        = 2, //!< 右斜め前方向(45)
			k90        = 3, //!< 右方向(90)
			k135       = 4, //!< 右斜め後ろ方向(135)
			k180       = 5, //!< 後ろ方向(180)
			k225       = 6, //!< 左斜め後ろ方向(225)
			k270       = 7, //!< 左方向(270)
			k315       = 8, //!< 左斜め前方向(315)
		};
	} // lane_direction

} // inf_lane

/// 強制誘導リンク列LQテーブル
namespace lq_guide {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // lq_guide

/// 方面案内リンク列LQテーブル
namespace lq_dirguide {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // lq_dirguide

/// レーンリンク列LQテーブル
namespace lq_lane {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // lq_lane

/// 路線コードリストテーブル
namespace road_code_list {
	using namespace ::sindy::schema::global::object;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kOperator[];    //!< 最終更新者
	extern const _TCHAR kModifyDate[];  //!< 最終更新日時
	extern const _TCHAR kRoadCode[];    //!< 路線名称ID
	extern const _TCHAR kNameKanji[];   //!< 路線漢字名称
	extern const _TCHAR kNameYomi[];    //!< 路線ヨミ名称
	extern const _TCHAR kWorkKanji[];   //!< 路線漢字名称(Hwyモード用)
	extern const _TCHAR kWorkYomi[];    //!< 路線ヨミ名称(Hwyモード用)
	extern const _TCHAR kLineClass[];   //!< 路線種別コード
	extern const _TCHAR kLineDirType[]; //!< 路線方向タイプコード
	extern const _TCHAR kLandClass[];   //!< 地域種別コード

	/// 路線種別コード定義
	namespace line_class {
		enum ECode {
			kHighway     =  1, ///< 高速道路
			kToll        =  2, ///< 有料道路
			kCityHighway =  3, ///< 首都高速道路
			kHanshin     =  4, ///< 阪神高速道路
			kNagoya      =  5, ///< 名古屋都市高速道路
			kFukuoka     =  6, ///< 福岡都市高速道路
			kKitakyu     =  7, ///< 北九州都市高速道路
			kHiroshima   =  8, ///< 広島高速道路
			kFerry       =  9, ///< フェリー航路
			kNormal      = 10, ///< 一般街道名称
			kTF          = 11, ///< TF料金対応路線
			kKyoto		 = 12, ///< 京都高速道路
		};
	} // line_class

	/// 路線方向タイプコード定義
	namespace line_dir_type {
		enum ECode {
			kNoType		= 0,	///< タイプなし
			kUpDown		= 1,	///< 上り・下り
			kInOut		= 2,	///< 内回り・外回り
			kEastWest	= 3,	///< 西行き・東行き
			kOther		= 4,	///< その他
		};
	} // line_dir_type

} // road_code_list

// 行政界のベース
namespace admin_class {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kAdminCode[];       //!< 行政界コード
	extern const _TCHAR kPopulation[];      //!< 人口値
} // admin_class

// built up areaポリゴン
namespace built_up_area {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kAdminCode[];       //!< 行政界コード
	extern const _TCHAR kPopulation[];      //!< 人口値
} // built_up_area

namespace common_lang_type{
	namespace gl_lang_type {
		enum ECode {
			kUndefined = 0,                 //!< 未定義
			kENG = 1,                       //!< 英語
			kTHA = 2,                       //!< タイ語
			kIND = 3,                       //!< インドネシア語
			kMAY = 4,                       //!< マレーシア語
			kVIE = 5,                       //!< ベトナム語
			kLAO = 6,						//!< ラオス語
			kKHM = 7,                       //!< カンボジア語
			kJPN = 99,                      //!< 日本語
		};
	} // gl_lang_type
} // common_lang_type

// 国ポリゴン
namespace country {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::common_lang_type;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kAdminCode[];       //!< 行政界コード
	extern const _TCHAR kLangC[];           //!< 公用語
	extern const _TCHAR kDriveSide[];       //!< 進行車線指定
	extern const _TCHAR kSpeedLimitUnit[];  //!< 最高速度単位
	extern const _TCHAR kTimeZone[];        //!< タイムゾーン
	extern const _TCHAR kPhoneCountryC[];   //!< 国電話番号
	extern const _TCHAR kPhonePrefix[];     //!< 電話番号プレフィックス
	extern const _TCHAR kPOIAddrDispType[]; //!< 表示タイプ

	namespace gl_drive_side {
		enum ECode {
			kLeftSide  = 0,                 //!< 左車線
			kRightSide = 1,                 //!< 右車線
		};
	} // gl_drive_side

	// 最高速度単位
	namespace gl_speed_limit {
		enum ECode {
			kKPH = 0,                       //!< km/h
			kMPH = 1,                       //!< mile/h
		};
	} // gl_speed_limit

} // country

// PROVINCEポリゴン
namespace province {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
} // province

// DISTRICTポリゴン
namespace district {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
} // district

// SUBDISTRICTポリゴン
namespace sub_district {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
} // subdistrict

// VILLAGEポリゴン
namespace village {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
} // Village

// NAMEDAREAポリゴン
namespace named_area {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
} // namedarea

// POSTALDISTRICT
namespace postal_district {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kPostalCode[];      //!< 郵便番号
} // postal_district

// タイプコード
namespace hn_common_domain{
	namespace gl_hn_type {
		enum ECode {
			kBase = 0,                          //!< 標準
			kOld  = 1,                          //!< 旧標準
		};
	} // gl_hn_type
} // hn_common_domain

// HNP
namespace hnp {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::hn_common_domain;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kHn[];              //!< ハウスナンバー
	extern const _TCHAR kHnType[];          //!< ハウスナンバータイプ
	extern const _TCHAR kLinkID[];          //!< 道路リンクID
	extern const _TCHAR kRoadNameID[];      //!< 道路名ID
	// TODO 2015年の4中には不要になる見込みなので、不要になったら消す
	extern const _TCHAR kHnRangeLineID[];   //!< ハウスナンバーレンジラインID
} // hnp

// GL_POIAccuracy
namespace gl_poi_accuracy{
	enum ECode {
		kUndefined  ,                   //!< 未定義
		kVerified   ,                   //!< 検証済
		kSpecialized,                   //!< Specialized
		kAuto       ,                   //!< 自動
	};
} // gl_poi_accuracy

// HNP_ENTRYPOINT
namespace hnp_entry_point {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::gl_poi_accuracy;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kHnpPointID[];      //!< HNPのID
	extern const _TCHAR kAccuracyCode[];      //!< 精度コード

} // hnp_entry_point

// POSTALPOINT
 namespace postal_point{
 	using namespace ::sindy::schema::global::country_feature;
 
 	extern const _TCHAR kTableName[];       //!< テーブル名
 
 	extern const _TCHAR kPostalCode[];		//!< POSTALPOINT(郵便番号ポイント)はシンガポールのみで使用。シンガポールは6桁の郵便番号と決まっている
 	extern const _TCHAR kRoadLinkID[];		//!< 道路リンクID
	extern const _TCHAR kRoadNameID[];		//!< 道路名ID
 }// postal_point

// POSTALENTRYPOINT
namespace postal_entry_point {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::gl_poi_accuracy;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kPostalPointID[];   //!< POSTALPOINTのID
	extern const _TCHAR kAccuracyCode[];    //!< 精度コード

} // postal_entry_point

 // HNR
namespace hnr {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::hn_common_domain;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kFromHnLeft[];      //!< 左側の開始ハウスナンバー
	extern const _TCHAR kToHnLeft[];        //!< 左側の終了ハウスナンバー
	extern const _TCHAR kFromHnRight[];     //!< 右側の開始ハウスナンバー
	extern const _TCHAR kToHnRight[];       //!< 右側の終了ハウスナンバー
	extern const _TCHAR kHnSchemeLeft[];    //!< 左側のハウスナンバー定義
	extern const _TCHAR kHnSchemeRight[];   //!< 右側のハウスナンバー定義
	extern const _TCHAR kHnType[];          //!< ハウスナンバータイプ
	extern const _TCHAR kLinkID[];          //!< リンクID

	// 定義コード
	namespace gl_hnr_scheme {
		enum ECode {
			kEven  = 0,                     //!< 偶数
			kOdd   = 1,                     //!< 奇数
			kMixed = 2,                     //!< 混合
		};
	} // gl_hnr_scheme
} // hnr

// POI_INFO
namespace poi_info {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::common_lang_type;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];         //!< テーブル名

	extern const _TCHAR kUpdAvailDate[];      //!< 基準日
	extern const _TCHAR kAccuracyLevel[];     //!< 精度正確レベル
	extern const _TCHAR kAdminCode[];         //!< 住所コード
	extern const _TCHAR kRoadNameID[];        //!< 道路の名称テーブルID
	extern const _TCHAR kProductType[];       //!< 製品種別
	extern const _TCHAR kHouseNumber[];       //!< ハウスナンバー
	extern const _TCHAR kLanguageCode[];      //!< Actual Addressの言語コード
	extern const _TCHAR kActualAddress[];     //!< Actual Address
	extern const _TCHAR kLanguageTransCode[]; //!< Actual Address Translationの言語コード
	extern const _TCHAR kActualAddressTrans[];//!< Actual Address Translation
	extern const _TCHAR kCategoryCode[];      //!< カテゴリーコード
	extern const _TCHAR kSubCategoryCode[];   //!< サブカテゴリーコード
	extern const _TCHAR kChainMasterCode[];   //!< チェーンコード
	extern const _TCHAR kDetailInfo[];        //!< 詳細情報
	extern const _TCHAR kDeletionCode[];      //!< 削除コード
	extern const _TCHAR kFuelType[];          //!< タイプコード

	namespace gl_poi_accuracy {
		enum ECode {
			kUndefined, // =0             //!< 未定義
			kVerified,                    //!< 検証済
			kSpecialized,                 //!< Specialized
			kAuto                         //!< 自動
		};
	} // gl_poi_accuracy

	namespace gl_product_type {
		enum ECode {
			kOriginal	= 0,              //!< オリジナル
			k3rdParty	= 1,              //!< サードパーティ
			kOutsource	= 2,              //!< アウトソース
		};
	} // gl_product_type
	
	namespace gl_poi_deletion {
		enum ECode {
			kInUse            = 0,              
			TemporaryClosure  = 1,              
			kSuveyImpossible  = 2,
			kDependOnList     = 3,
			kDeletion         = 99
		};
	} // gl_product_type

	// TODO:使用されていないので、調査して削除
	namespace gl_phon_type {
		enum ECode {
			kPhoneNumber     = 0,         //!< 通常
			kTollFreeNumber  = 1,         //!< フリーダイアル
			kMobilePhone     = 2,         //!< 携帯電話
			kEtc             = 3          //!< その他
		};
	} // gl_phon_type

	namespace gl_category {					
		enum ECode {
			kFood                     = 10, //!< 飲食
			kBank_ATM                 = 11, //!< 銀行
			kBusiness                 = 12, //!< 企業
			kEducation                = 13, //!< 教育
			kReligion                 = 14, //!< 宗教
			kHospital_Healthcare      = 15, //!< 病院
			kConference_Community     = 16, //!< 連盟
			kAutomobile               = 17, //!< 自動車
			kTransportation           = 18, //!< 運送
			kCityCenter               = 22, //!< シティセンター
			kService                  = 19, //!< サービス
			kHotel                    = 20, //!< ホテル
			kInfrastructure_Hydro     = 21, //!< インフラ・水道
			kResidence                = 30, //!< 住宅
			kShops_Store              = 40, //!< ショップ
			kTourism                  = 50, //!< 観光
			kArt_Culture              = 60, //!< 芸術
			kSport                    = 70, //!< スポーツ
			kEntertainment            = 80, //!< エンターテイメント
			kGovernment               = 90, //!< 行政
			kUndefined                = 0   //!< 未定義
		};				
	} // gl_category
	
	// TODO: コメント記載
	namespace gl_sub_category {
		enum ECode {
			kThai                            = 1001,
			kChinese                         = 1002,
			kJapanese                        = 1003,
			kKorean                          = 1004,
			kVietnam                         = 1005,
			kMuslim_Indian                   = 1006,
			kItalian                         = 1007,
			kFrench                          = 1008,
			kGerman                          = 1009,
			kWesternOthers                   = 1010,
			kSeafood                         = 1011,
			kFoodCenter                      = 1012,
			kSuki                            = 1013,
			kFastFood                        = 1014,
			kSteak                           = 1015,
			kBarbecue                        = 1016,
			kCoffeeShop                      = 1017,
			kBakery_IceCream                 = 1018,
			kFood_Others                     = 1099,
			kBanks_FinancialServices         = 1101,
			kCurrencyExchange                = 1102,
			kATM                             = 1103,
			kBank_Others                     = 1199,
			kOfficeBuilding                  = 1201,
			kBuisness_Others                 = 1299,
			kUniversity_College              = 1301,
			kSchool_Kindergarten_Vocation    = 1302,
			kTutoring_Computer_LanguageIns   = 1303,
			kLibrary                         = 1304,
			kNon_formalEducationCenter       = 1305,
			kEducation_Others                = 1399,
			kBuddhistTemple                  = 1401,
			kChurch                          = 1402,
			kMosque                          = 1403,
			kCemetery                        = 1404,
			kBureauOfMonks                   = 1405,
			kShrine                          = 1406,
			kReligion_Others                 = 1499,
			kHospital                        = 1501,
			kPolyClinic                      = 1502,
			kPublicHealthCenter              = 1503,
			kDentalClinic                    = 1504,
			kEyeEarNoseThroatClinic          = 1505,
			kSkin_SurgeryClinic              = 1506,
			kPharmacy                        = 1507,
			kSpas_Massage_BodyCare           = 1508,
			kBone_ArticulationClinic         = 1509,
			kChildClinic                     = 1510,
			kChildrenDevelopmentCenter       = 1511,
			kClinicOthers                    = 1512,
			kCommunityHealthCenter           = 1513,
			kFamilyPlanningCenter            = 1514,
			kRehabilitateMedicalClinic       = 1515,
			kRehabilitationClinic            = 1516,
			kSinusClinic                     = 1517,
			kThaiMassageClinic               = 1518,
			kX_rayLab_LabClinic              = 1519,
			kHospital_Others                 = 1599,
			kExhibition_Conference           = 1601,
			kCommunityCentre                 = 1602,
			kMeetingHall_AssocClubHose       = 1603,
			kConference_Others               = 1699,
			kCarDealership                   = 1701,
			kCarRental                       = 1702,
			kCarRepair                       = 1703,
			kAutoParts_AutoAccessories       = 1704,
			kCarCare                         = 1705,
			kPetrolStations                  = 1706,
			kNGV_LPGStation                  = 1707,
			kParkingGarage                   = 1708,
			kParkingLot                      = 1709,
			kRestArea                        = 1710,
			kTankFarm                        = 1711,
			kAutomobile_Others               = 1799,
			kAirport                         = 1801,
			kAirportPassengerTerminal        = 1802,
			kBusStation                      = 1803,
			kBTS_MRTStation                  = 1804,
			kFerryTerminal                   = 1805,
			kPier                            = 1806,
			kMarina                          = 1807,
			kAirlines                        = 1808,
			kRailwayStation                  = 1809,
			kTransportCompany                = 1810,
			kTollPlaza                       = 1811,
			kPort                            = 1820,
			kTransport_Others                = 1899,
			kAir_conditioning                = 1902,
			kAntiques                        = 1903,
			kBeautySalon_Barber              = 1904,
			kBeautyServiceCenter_Cosmetic    = 1905,
			kBuddha_ReligiousArticleFoundry  = 1906,
			kBicycle                         = 1907,
			kBugExterminator                 = 1909,
			kCargoCentre                     = 1911,
			kCarpet_Curtain_Blinds_Flooring  = 1912,
			kCircle_ClockTower               = 1915,
			kClothesSewingShop               = 1917,
			kClothes_Leather_HandBag_Shoe    = 1918,
			kConstruction_LandFill           = 1920,
			kCookingGas                      = 1921,
			kCooperativeSociety              = 1922,
			kDrinkingWaterBottler_IceFactory = 1923,
			kDryGoods                        = 1924,
			kDryRice_AnimalFeed              = 1925,
			kDutyFreeShop                    = 1926,
			kElectricalAppliance             = 1927,
			kElectricalRepair                = 1928,
			kFactory_Industry                = 1929,
			kFlorist_FlowerNursery           = 1932,
			kFoundation                      = 1933,
			kFreePort                        = 1934,
			kFrontierCrossing                = 1935,
			kGift_Souvenir                   = 1936,
			kGlassware                       = 1937,
			kGun_Ammunition                  = 1938,
			kHandcraftHousing                = 1939,
			kHardware_ConstructionMaterial   = 1940,
			kHornSculpture                   = 1941,
			kHousePet_Fish_Aquarium          = 1942,
			kInsurance                       = 1943,
			kInteriorDesign                  = 1944,
			kInternetService_InternetCafe    = 1945,
			kJewelryShop                     = 1946,
			kKitchenware                     = 1947,
			kLaundry                         = 1949,
			kLeatherHandBagMfg               = 1950,
			kLocksmith                       = 1951,
			kLuggage                         = 1952,
			kModelingAgency                  = 1954,
			kMusicalInstruments              = 1955,
			kMusical_Arts_DanceLesson        = 1956,
			kOffice_CompanyOffice            = 1958,
			kPawnShop                        = 1959,
			kPaymentService                  = 1960,
			kPC_VideoGames                   = 1961,
			kPetsClinic                      = 1962,
			kPhotoLab_Camera_Film            = 1963,
			kPictureFrame                    = 1964,
			kPlasticGoodsMfg                 = 1965,
			kPoliticalPartyOffice            = 1967,
			kPrintingShop                    = 1968,
			kRubberStamp_Chops               = 1973,
			kShoeRepair                      = 1974,
			kSignBoard                       = 1976,
			kTape_VDO_VCD                    = 1979,
			kTelevision_RadioStation         = 1980,
			kToys                            = 1982,
			kTVAntenna_SatelliteDish         = 1984,
			kCableTVAgency                   = 1985,
			kUpholstery                      = 1986,
			kWarehouse                       = 1987,
			kWatches_Glasses_Optics          = 1988,
			kWeddingService                  = 1989,
			kWindowFrame_Aluminum_Screen_Alloy = 1990,
			kWine_Beer_AlcoholicBeverage     = 1991,
			kWinery                          = 1992,
			kService_Others                  = 1999,
			kHotel_Motel                     = 2001,
			kResort                          = 2002,
			kBungalow                        = 2003,
			kGuestHouse                      = 2004,
			kHotel_Others                    = 2099,
			kJunction_Bridge                 = 2101,
			kHydro                           = 2103,
			kInfra_Others                    = 2199,
			kCountry                         = 2201,
			kCapital                         = 2202,
			kProvince                        = 2203,
			kDistrict                        = 2204,
			kSubDistrict                     = 2205,
			kCityCenter_Others               = 2299,
			kCondominium                     = 3001,
			kApartment                       = 3002,
			kFlat                            = 3003,
			kDormitory                       = 3004,
			kHousing                         = 3005,
			kResidence_Others                = 3099,
			kShoppingCentre_Bazaar           = 4001,
			kDepartmentStore                 = 4002,
			kSupermarket                     = 4003,
			kMarket                          = 4004,
			kBooks_OfficeEquipment           = 4005,
			kFurniture_HomeRepair            = 4006,
			kComputer_Software               = 4007,
			kSportingGoods                   = 4008,
			kMobileShop                      = 4009,
			kMinimart_ConvenienceStore       = 4010,
			kShops_Others                    = 4099,
			kNationalPark_ForestPark         = 5001,
			kMountain                        = 5002,
			kBeach                           = 5003,
			kIsland                          = 5004,
			kWaterfall                       = 5005,
			kReservoir_Dam                   = 5006,
			kCave                            = 5007,
			kHotSpring                       = 5008,
			kVantagePoint                    = 5009,
			kTourismSite                     = 5010,
			kFarm_Orchard_Winery             = 5011,
			kCityPark                        = 5012,
			kZoo                             = 5013,
			kElephantRide                    = 5014,
			kRafting                         = 5015,
			kCamping                         = 5016,
			kTouristOffice                   = 5017,
			kTravelAgency                    = 5018,
			kTourism_Others                  = 5099,
			kPalace                          = 6001,
			kHistoricalMonument              = 6002,
			kCulturalCentre                  = 6003,
			kMuseum                          = 6004,
			kArtGallery                      = 6005,
			kArt_Others                      = 6099,
			kStadium                         = 7001,
			kGolfCourse                      = 7002,
			kGolfDrivingRange                = 7003,
			kHorseracing_Racecourses         = 7004,
			kSports_Recreation               = 7005,
			kFitnessCenter                   = 7006,
			kSwimmingPool                    = 7007,
			kBowling                         = 7008,
			kSpot_Others                     = 7099,
			kAmusementPark_ThemePark         = 8001,
			kMovieTheater                    = 8002,
			kTheaters                        = 8003,
			kAssociation_Club                = 8004,
			kPub_Bar                         = 8005,
			kDiscotheque_NightClub           = 8006,
			kCafe                            = 8007,
			kKaraoke                         = 8008,
			kSnookerClub                     = 8009,
			kMassageParlor                   = 8010,
			kEntertainment_Others            = 8099,
			kGovernmentOffices_Military      = 9001,
			kEmbassy_Consulate               = 9002,
			kCityHall                        = 9003,
			kCourtHouse                      = 9004,
			kPoliceDepartment                = 9005,
			kPostOffice                      = 9006,
			kFireStation                     = 9007,
			kLandOffice                      = 9008,
			kPrison                          = 9009,
			kRevenueDepartmentOffice         = 9010,
			kGovernmentOther                 = 9011,
			kGovernment_Others               = 9099,
			kUndefined                       = 0   ,
		};				
	} // gl_sub_category

} // poi_position

// POI_CHAIN_MASTER
namespace poi_chain_master {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kChainCode[];		//!< チェーンコード
	extern const _TCHAR kNameLatin[];		//!< 英語名（ラテン名）
	extern const _TCHAR kNameLocal[];		//!< 現地語名
	extern const _TCHAR kLocalLanguageC[];	//!< 現地語コード

} // poi_chain_master

// POI_ENTRYPOINT
namespace poi_entry_point {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];         //!< テーブル名

	extern const _TCHAR kPoiInfoID[];         //!< POI_POSITIONのID
	extern const _TCHAR kPriorityF[];         //!< 優先度フラグ(0:Main, 1:Sub)
	extern const _TCHAR kEntryPointCode[];    //!< 誘導ポイントの種別（駐車場入口/施設入口）
	extern const _TCHAR kAccuracyCode[];      //!< 精度コード

	// 誘導ポイント種別コード
	namespace gl_entrypoint_code{
		enum ECode {
			kUndefined            = 0,        //!< 未定義
			kEntrance_of_parking  = 1,        //!< 駐車場入口
			kEntrance_of_facility = 2,        //!< 施設入口
			kEntrance_for_pedestrian = 3,     //!< 歩行者
		};
	} // gl_entrypoint_code

	// 精度コード
	namespace gl_coordinate_code {
		enum ECode {
			kUndefined   = 0,                 //!< 未定義
			kVerified    = 1,                 //!< 検証済
			kSpecialized = 2,                 //!< Specialized
			kAuto        = 3,                 //!< 自動
		};
	} // gl_coordinate_code
} // poi_entry_point

// POI_PHONE
namespace poi_phone {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];         //!< テーブル名

	extern const _TCHAR kPoiInfoID[];         //!< POI_POSITIONのID
	extern const _TCHAR kPhonePriorityF[];    //!< 優先度フラグ(1: Main, 0: Sub)
	extern const _TCHAR kPhoneType[];         //!< 種別タイプ
	extern const _TCHAR kPhoneNumber[];       //!< 電話番号

	// 種別タイプコード
	namespace gl_phone_type {
		enum ECode {
			kPhoneNumber    = 0,              //!< 通常
			kTollFreeNumber = 1,              //!< フリーダイヤル
			kMobilePhone    = 2,              //!< 携帯電話
			kEtc            = 3,              //!< その他
		};
	} // gl_phone_type
} // poi_phone

// POI_ASSOCIATION
namespace poi_association {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];         //!< テーブル名

	extern const _TCHAR kParentID[];          //!< 親ID(POI_POSITIONのID)
	extern const _TCHAR kChildID[];           //!< 子ID(POI_POSITIONのID)
} // poi_association

// TOP_ANNOTATION
namespace top_annotation{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kAnnoClassC[];		//!< 注記種別コード
}

// TOP_COUNTRY
namespace top_country{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kCountryC[];		//!< 国コード
}

// TOP_PROVINCE
namespace top_province{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
}

// TOP_WATERWAY_LINE
namespace top_waterway_line{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kBgClassC[];		//!< 背景種別コード
}

// TOP_WATERWAY_POLYGON
namespace top_waterway_polygon{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kBgClassC[];		///!< 背景種別コード
}

// TOP_PROVINCE
namespace top_province{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< テーブル名
}

// AUTO_ANNOTATION
namespace auto_annotation{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];       //!< テーブル名

	extern const _TCHAR kOrgID[];			//!< OriginalObjectID
	extern const _TCHAR kMainName[];		//!< MainName
	extern const _TCHAR kMainLanguageC[];	//!< MainLanguageCode
	extern const _TCHAR kSubName[];			//!< SubName
	extern const _TCHAR kSubLanguageC[];	//!< SubLanguageCode
	extern const _TCHAR kLayerC[];			//!< CorrespondingLayerCode
	extern const _TCHAR kAnnoCode[];		//!< OriginalAnnotationCode
	extern const _TCHAR kAnnoAngle[];       //!< AnnotationAngle

}

// Annotation common object
namespace anno_common{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kOrgID[];			//!< OriginalObjectID			
	extern const _TCHAR kRoadNoString[];	//!< RoadNoString
	extern const _TCHAR kLayerC[];			//!< CorrespondingLayerCode
	extern const _TCHAR kAnnoCode[];		//!< OriginalAnnotationCode
}

// ANNO_BASE_RAILWAY
namespace anno_base_railway{
	using namespace anno_common;

	extern const _TCHAR kTableName[];       //!< テーブル名
}

// ANNO_ROAD_NAME
namespace anno_road_name{
	using namespace anno_common;

	extern const _TCHAR kTableName[];       //!< テーブル名
}

// ANNO_ROAD_NO
namespace anno_road_no{
	using namespace anno_common;

	extern const _TCHAR kTableName[];       //!< テーブル名
}

/// OFFICIAL_NAMEテーブル
namespace official_name {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kLayerCode[];     //!< レイヤコード
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kLanguageCode[];  //!< 言語コード
	extern const _TCHAR kDisplayF[];      //!< DisplayFlag

} // official_name

/// ALTERNATIVE_NAMEテーブル
namespace alternative_name {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kOfficialID[];    //!< オフィシャルID
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kLanguageCode[];  //!< 言語コード
	extern const _TCHAR kSequence[];      //!< シーケンス
	extern const _TCHAR kDisplayF[];      //!< DisplayFlag

} // alternative_name

/// TRANSLATIONテーブル
namespace translation {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kTypeF[];         //!< 0:Official, 1:Alternative
	extern const _TCHAR kID[];            //!< [Official or Alternative] ObjectID
	extern const _TCHAR kName[];          //!< 名称
	extern const _TCHAR kLanguageCode[];  //!< 言語コード
	extern const _TCHAR kDisplayF[];      //!< DisplayFlag

} // translation

namespace base_border_line {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[];       //!< テーブル名
	extern const _TCHAR kCountryType[];		//!< CountryType
	extern const _TCHAR kOceanF[];			//!< OceanFlag
} // base_border_line

namespace border_point {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[]    ;       //!< テーブル名

	extern const _TCHAR kBorderPointCode[];		//!< BORDER_POINTが国境か、フェリー航路のみなし国境かを示す種別
	extern const _TCHAR kCountryCode1[] ;       //!< 国番号1
	extern const _TCHAR kOriginalObjectID1[] ;  //!< オブジェクトID1
	extern const _TCHAR kCountryCode2[] ;       //!< 国番号2
	extern const _TCHAR kOriginalObjectID2[] ;  //!< オブジェクトID2

	namespace gl_borderpoint_code {
		enum ECode {
			kUndefined		= 0,             //!< Undefined
			kOfficial		= 1,             //!< Official
			kNonOfficial	= 2,             //!< NonOfficial
		};
	} // gl_borderpoint_code

} // border_point

namespace release_country_zone {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;
	using namespace ::sindy::schema::global::border_point::gl_country_code;

	extern const _TCHAR kTableName[];            //!< テーブル名

	extern const _TCHAR kCountryCode[];          //!< 国番号
} // release_country_zone

namespace sea_polygon {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];            //!< テーブル名
} // sea_polygon

namespace sea_polygon_name {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];            //!< テーブル名
	extern const _TCHAR kUnitF[];				 //!< 海名称ポリゴンユニットフラグ
} // sea_polygon_name

namespace land_complement {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[];            //!< テーブル名
} // land_complement

namespace top_border_line {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[];       //!< テーブル名
	extern const _TCHAR kCountryType[];		//!< CountryType
	extern const _TCHAR kOceanF[];			//!< OceanFlag
} // top_border_line

/// diffattribute
namespace ipc_diff_feature{
    using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kCountry[];           //!< countryコード
	extern const _TCHAR kOrgId[];             //!< オブジェクトID
	extern const _TCHAR kChangeType[];        //!< changeコード
	extern const _TCHAR kCountryAlias[];      //!< countryコード別名
	extern const _TCHAR kObjectIdAlias[];     //!< オブジェクトID別名
	extern const _TCHAR kChangeTypeAlias[];   //!< changeコード別名
}// diffattribute


/// イラストリンクフィーチャクラス
namespace illust_link {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kIllustClass[];           //!< イラスト種別
	extern const _TCHAR kCustomerCode[];          //!< 仕向け先コード
	extern const _TCHAR kIllustFileName[];        //!< 画像ファイル名
	extern const _TCHAR kGuideSeq[];              //!< 案内シーケンス
	extern const _TCHAR kDirectionCode[];         //!< 案内方向コード
	extern const _TCHAR kUTurnFlag[];             //!< Uターンフラグ
	extern const _TCHAR kRoundaboutFlag[];        //!< ラウンドアバウト

	extern const int    kMaxNodeNumber;// 最大ノード数
	extern const _TCHAR kNode0[];   //!< 道路ノード0(使わないが便宜上)
	extern const _TCHAR kNode1[];   //!< 道路ノード1
	extern const _TCHAR kNode2[];   //!< 道路ノード2
	extern const _TCHAR kNode3[];   //!< 道路ノード3
	extern const _TCHAR kNode4[];   //!< 道路ノード4
	extern const _TCHAR kNode5[];   //!< 道路ノード5
	extern const _TCHAR kNode6[];   //!< 道路ノード6
	extern const _TCHAR kNode7[];   //!< 道路ノード7
	extern const _TCHAR kNode8[];   //!< 道路ノード8
	extern const _TCHAR kNode9[];   //!< 道路ノード9
	extern const _TCHAR kNode10[];  //!< 道路ノード10
	extern const _TCHAR kNode11[];  //!< 道路ノード11
	extern const _TCHAR kNode12[];  //!< 道路ノード12
	extern const _TCHAR kNode13[];  //!< 道路ノード13
	extern const _TCHAR kNode14[];  //!< 道路ノード14
	extern const _TCHAR kNode15[];  //!< 道路ノード15
	extern const _TCHAR kNode16[];  //!< 道路ノード16
	extern const _TCHAR kNode17[];  //!< 道路ノード17
	extern const _TCHAR kNode18[];  //!< 道路ノード18
	extern const _TCHAR kNode19[];  //!< 道路ノード19
	extern const _TCHAR kNode20[];  //!< 道路ノード20

	extern const LPCTSTR kNode[21]; //!< 道路ノード一覧

	// イラスト仕向け先コード
	namespace customer {
		enum ECode {
			kUndefined     = 0,   //!< 未設定
			kCommon        = 1,   //!< 共通
		};
	} // illust_customer

	// イラスト種別コード
	namespace illust_class {
		enum ECode {
			kUndefined         = 0,   //!< 未設定
			kJunction          = 1,   //!< (高速)ジャンクション分岐
			kExitFromMain      = 2,   //!< (高速)本線出口分岐
			kBranchOnRamp      = 3,   //!< (高速)入口・出口分岐
			kEntrance          = 4,   //!< (高速)入口
			kBranchFront       = 5,   //!< 側道分岐
			kIntersection      = 6,   //!< 一般道交差点
			kCmplIntersection  = 7,   //!< 複雑交差点
		};
	} // illust_code
	
	// イラスト案内方向コード
	namespace direction_code {
		enum ECode {
			kUndefined     = 0,   //!< 未設定
			kA             = 1,   //!< A
			kB             = 2,   //!< B
			kC             = 3,   //!< C
			kD             = 4,   //!< D
			kE             = 5,   //!< E
			kF             = 6,   //!< F
			kG             = 7,   //!< G
			kH             = 8,   //!< H
			kI             = 9,   //!< I
			kJ             = 10,  //!< J
			kK             = 11,  //!< K
			kL             = 12,  //!< L
			kM             = 13,  //!< M
			kN             = 14,  //!< N
			kO             = 15,  //!< O
		};
	} // direction_code
} // illust_link

/// 傾斜フィーチャクラス
namespace grad{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::link_relational_table;

	extern const _TCHAR kTableName[]; //!< テーブル名
	extern const _TCHAR kGradValue[]; //!< 傾斜値
}


} // global

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_SJ_H_
