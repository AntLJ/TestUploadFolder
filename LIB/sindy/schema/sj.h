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

#ifndef SINDY_SCHEMA_SJ_H_
#define SINDY_SCHEMA_SJ_H_

#include <sindy/schema/common.h>
#include <sindy/schema/map.h>
#include <sindy/schema/annotation.h>

namespace sindy {

namespace schema {

namespace sj{


extern const _TCHAR kRoofTopTableName[];     //!< rooftopテーブル名

extern const _TCHAR kWalkwayTableName[];     //!< walkwayテーブル名

extern const _TCHAR kOtherLCTableName[];     //!< other_lcテーブル名

extern const _TCHAR kRailwayTableName[];     //!< railway_groundテーブル名

extern const _TCHAR kRoadTableName[];        //!< road_polygonテーブル名

extern const _TCHAR kPredominantTableName[]; //!< predominant_lcテーブル名

extern const _TCHAR kRoadF[];  //!< 道路フラグ
extern const _TCHAR kLCCLS[];  //!< Land Cover
extern const _TCHAR kLUCLS[];  //!< Land Use

namespace land_cover {
	enum ECode {
		kUndefined               = 0,
		kUnknown_Improved        = 10,
		kPaved                   = 11,
		kCobblestone             = 12,
		kBrick                   = 13,
		kMetal                   = 14,
		kWood                    = 15,
		kRailway_ground          = 16,
		kUnknown_Crushed_ground  = 20,
		kSand                    = 21,
		kClay                    = 22,
		kDirt                    = 23,
		kStone                   = 24,
		kGravel                  = 25,
		kUnknown_Vegetation      = 30,
		kGrass_Meadow            = 31,
		kCropland                = 32,
		kMarshland               = 33,
		kTrees_Forest_Coniferous = 34,
		kTrees_Forest_Deciduous  = 35,
		kTrees_Forest_Mixed      = 36,
		kTrees_Forest_Unknwon    = 37,
		kWater                   = 40,
		kUnknown                 = 50,
	};
} // land_cover

namespace land_use1 {
	enum ECode {
		kNotpplicable       = 0,
		kDiviver            = 11,
		kSidewalks          = 12,
		kOffroad_pathways   = 13,
		kParking            = 14,
		kWater_residential  = 21,
		kWater_recreation   = 22,
		kWater_natural      = 23,
		kWater_industrial   = 24,
		kWater_unknown      = 25,
		kIce                = 26,
		kBaseball           = 31,
		kBasketball         = 32,
		kFootball           = 33,
		kGolf               = 34,
		kSoccer             = 35,
		kTennis             = 36,
		kTrack              = 37,
		kSports_unknown     = 38,
		kUnder_construction = 41,
		kPier               = 51,
	};
} // land_use1

namespace land_use2 {
	enum ECode {
		kNotapplicable     = 0,
		kSidewalks         = 12,
		kOffroad_pathways  = 13,
	};
} // land_use2

namespace land_use3 {
	enum ECode {
		kNotapplicable  = 0,
		kDivider        = 11,
	};
} // land_use3


/// 地下鉄リンク
namespace subway_link {
	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kFromNodeID[];    //!< 始点ノードID
	extern const _TCHAR kToNodeID[];      //!< 終点ノードID
	extern const _TCHAR kRailwayLineID[]; //!< 鉄道路線ID
}

/// 地下鉄ノード
namespace subway_node {
	extern const _TCHAR kTableName[];     //!< テーブル名

	extern const _TCHAR kNodeClassC[];    //!< ノード種別
	extern const _TCHAR kJoinLineInfo[];  //!< 乗り入れ路線ID
	extern const _TCHAR kStationNumber[]; //!< 駅番号
	
	/// 地下鉄ノード種別
	namespace node_class {
		enum ECode {
			kPublic		= 0,	///< 一般ノード
			kTerminal	= 1,	///< 端点ノード
			kStation    = 2,	///< 駅POIノード
			kOutline	= 99,	///< 図郭上ノード
		};
	}
}

/// 国立・国定公園ポリゴン
namespace nationalpark_site {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kNparkID[];   //!< 公園関連テーブルID
}

///国立・国定公園関連テーブル
namespace nationalpark_table {
	extern const _TCHAR kTableName[];  //!< テーブル名

	extern const _TCHAR kParkTypeC[];  //!< 公園種別コード
	extern const _TCHAR kParkNumber[]; //!< 公園番号
	extern const _TCHAR kName[];       //!< 名称

	namespace park_class {
		enum ECode {
			kUnknown = 0,        ///<不明
			kNational = 1,       ///<国立公園
			kQuasiNational = 2,  ///<国定公園
		};
	}
}

// 都市地図注記背景ポリゴン
namespace city_anno_site {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];  //!< テーブル名

	extern const _TCHAR kCityAnnoID[]; //!< 都市注記ObjectID
	extern const _TCHAR kAnnoClassC[]; //!< 都市注記種別コード
	extern const _TCHAR kCitySiteID[]; //!< 都市背景ObjectID
	extern const _TCHAR kBGClassC[];   //!< 都市背景種別コード

	namespace bg_class {
		using namespace sindy::schema::city_site::bg_class;
	} // bg_class

	namespace annotation_code {
		using namespace sindy::schema::city_annotation::annotation_code;
	} // annotation_code

} // city_anno_site

// 中縮注記背景紐付きテーブル
namespace rel_base_anno_site {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];  //!< テーブル名

	extern const _TCHAR kBaseAnnoID[]; //!< 中縮注記ID
	extern const _TCHAR kAnnoClassC[]; //!< 中縮注記種別コード
	extern const _TCHAR kBaseSiteID[]; //!< 中縮背景ObjectID
	extern const _TCHAR kBGClassC[];   //!< 中縮背景種別コード

	namespace bg_class {
		using namespace sindy::schema::base_site::bg_class;
	} // bg_class

	namespace annotation_code {
		using namespace sindy::schema::base_annotation::annotation_code;
	} // annotation_code

} // rel_base_anno_site

// 建物ビル名称共通部分
namespace buildingname_common {
	extern const _TCHAR kName[];			//!< 名称
	extern const _TCHAR kSourceType[];		//!< 出典コード

	namespace source_type {
		enum ECode {
			kOwnSurvey		= 0,	//!< 独自調査
			kImmovables		= 1,	//!< 不動産情報
			kFirePreventive	= 2,	//!< 防火対象物
			kGeoSpace		= 3,	//!< GEOSPACE
			kKankosha		= 4,	//!< 刊広社
			kGSSurvey		= 5,	//!< GS現地調査
			kAnnoPOI		= 101,	//!< POI注記
			kCompAnnoPOI	= 102,	//!< 補完用POI注記
			kTownPage		= 201,	//!< タウンページ
			kExclusion		= 999,	//!< 除外建物
		};
	}
} // buildingname_common

// 建物ビル名称共通部分（付加情報）
namespace buildingname_common_addinfo {
	using namespace sindy::schema::sj::buildingname_common;
	extern const _TCHAR kNameSeparated[];	//!< 名称（分割）
	extern const _TCHAR kYomiSeparated[];	//!< ヨミ（分割）
	extern const _TCHAR kBldgClass1[];		//!< 建物種別コード1
	extern const _TCHAR kBldgClass2[];		//!< 建物種別コード2
	extern const _TCHAR kBldgClass3[];		//!< 建物種別コード3
	extern const _TCHAR kFloors[];			//!< 階数
	extern const _TCHAR kSuitable[];		//!< ビル名称妥当フラグ
	extern const _TCHAR kFixClass[];		//!< 種別確定コード

	namespace bldg_class {
		enum ECode {
			kMansion		= 101,		//!< マンション
			kApartment		= 102,		//!< アパート
			kHousingComplex	= 103,		//!< 団地
			kDormitory		= 104,		//!< 寮・社宅
			kMiscHousing	= 199,		//!< その他住宅
			kRestaurant		= 201,		//!< 飲食店
			kRetailStore	= 202,		//!< 小売店
			kFinancial		= 203,		//!< 金融機関・郵便局
			kAutomotive		= 204,		//!< 自動車関連店舗
			kMiscStore		= 299,		//!< サービス店・その他店舗
			kCommercial		= 301,		//!< 複合商業施設・商業ビル
			kOfficeBldg		= 302,		//!< オフィスビル・オフィス系建物
			kGovernmental	= 303,		//!< 官公庁・公的機関
			kWelfare		= 304,		//!< 医療施設・福祉施設
			kEducational	= 305,		//!< 教育・保育施設
			kLodging		= 306,		//!< 宿泊施設
			kReligious		= 307,		//!< 宗教関連施設
			kCultural		= 308,		//!< 文化施設・観光・イベント関連
			kAmusement		= 309,		//!< 娯楽・スポーツ関連
			kTransportation	= 310,		//!< 公共交通機関
			kIndustrial		= 311,		//!< 工場・倉庫・研究所
			kParkingGarage	= 312,		//!< 立体駐車場
			kMiscellaneous	= 999,		//!< その他施設・建物
		};
	}

	namespace fix_class {
		enum ECode {
			kUnconfirmed		= 0,		//!< 不明
			kFixedByKeyword		= 1,		//!< 種別確定（キーワード一致）
			kFixedByAgreement	= 2,		//!< 種別確定（完全2名一致（その他種別以外））
			kFixedByManualCheck	= 3,		//!< 種別確定（キーワード不一致・目視除外確認OK）
			kRejected			= 9,		//!< 不採用（棟番号のみのものなど）
		};
	}
} // buildingname_common_addinfo

// 建物ビル名称素材ポイント
namespace buildingname_src_point {
	using namespace sindy::schema::feature;
	using namespace sindy::schema::sj::buildingname_common;
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kOrgName1[];		//!< 加工前名称1
	extern const _TCHAR kOrgName2[];		//!< 加工前名称2
	extern const _TCHAR kKana[];			//!< カナ
	extern const _TCHAR kAddr[];			//!< 住所
	extern const _TCHAR kOrgAddr[];			//!< 整形前住所
	extern const _TCHAR kAddrCode[];		//!< 住所コード
	extern const _TCHAR kAddrLon[];			//!< 住所経度
	extern const _TCHAR kAddrLat[];			//!< 住所緯度
	extern const _TCHAR kRepType[];			//!< 地番代表点複合コード
	extern const _TCHAR kAddrLevel[];		//!< 住所レベルコード
	extern const _TCHAR kPinPoint[];		//!< ピンポイントフラグ
	extern const _TCHAR kMultiSameBldg[];	//!< 同一家形内複数住所存在フラグ
	extern const _TCHAR kSameAddr[];		//!< 同一iPC住所複数存在フラグ
	extern const _TCHAR kSameAddrCount[];	//!< 同一iPC住所数
	extern const _TCHAR kAddrVer[];			//!< 住所解析元XYDB名
	extern const _TCHAR kSourceName[];		//!< 出典元
	extern const _TCHAR kSourceDate[];		//!< 出典資料入手時期
	extern const _TCHAR kSourceID[];		//!< 出典資料ID
	extern const _TCHAR kBuilding1Div1[];	//!< 用途区分1
	extern const _TCHAR kBuilding1Div2[];	//!< 用途区分2
	extern const _TCHAR kBuilding2Div1[];	//!< 建物棟用途区分1
	extern const _TCHAR kBuilding2Div2[];	//!< 建物棟用途区分2
	extern const _TCHAR kHeight[];			//!< 建物高さ
	extern const _TCHAR kFloors[];			//!< 地上階数
	extern const _TCHAR kBasementFloors[];	//!< 地下階数
	extern const _TCHAR kBuildingArea[];	//!< 建物面積
	extern const _TCHAR kAllArea[];			//!< 述べ床面積
	extern const _TCHAR kSrcStartDate[];	//!< 素材使用開始年月日
	extern const _TCHAR kSrcUpdateDate[];	//!< 素材更新年月日
	extern const _TCHAR kAnyPlaceID[];		//!< AnyPlaceID
	extern const _TCHAR kUpdateDate[];		//!< 更新年月日
	extern const _TCHAR kAdoption[];		//!< 採用コード

	namespace adoption_code {
		enum ECode {
			kAdopt = 0,						//!< 採用
			kRejectNoSurvey = 11,			//!< 間引きにより不採用(未調査)
			kRejectUnnecessary = 12,		//!< 間引きにより不採用(不要)
			kRejectChain = 13,				//!< 間引きにより不採用(チェーン系)
			kRejectAddrAnalyze = 21,		//!< 住所解析レベル不十分により不採用
			kRejectClosed = 31,				//!< 現調により不採用(閉鎖・民家)
			kRejectSrcDeleted = 32,			//!< 素材削除により不採用
			kRejectUnkown = 33				//!< 現調により不採用(不明)
		};
	}
	
	namespace chiban_fukugou {
		enum ECode {
			kIndefinite = 0,				//!< 不定
			kAreaOff = 1,					//!< エリア/OFF
			kOffOn = 2,						//!< OFF/ON
			kOffOff = 3,					//!< OFF/OFF
			kMultiOff = 4,					//!< 複数/OFF
			kMultiOn = 5					//!< 複数/ON
		};
	}

	namespace addr_level {
		enum ECode {
			kUnconverted = 0,               //!< 未変換
			kPref        = 1,               //!< 都道府県レベル
			kCity        = 2,               //!< 市区町村レベル
			kChomoku     = 3,               //!< 丁目レベル
			kChiban      = 4,               //!< 地番レベル
			kGou         = 5,               //!< 号レベル
		};
	}
	
} // buildingname_src_point

// 建物ビル名称素材付加情報
namespace buildingname_src_addinfo {
	using namespace sindy::schema::object;
	using namespace sindy::schema::sj::buildingname_common_addinfo;
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kSourceName[];		//!< 出典元
	extern const _TCHAR kSourceID[];		//!< 出典資料ID
} // buildingname_src_addinfo

// 建物ビル名称ポイント
namespace buildingname_point {
	using namespace sindy::schema::ipc_feature;
	using namespace sindy::schema::sj::buildingname_common_addinfo;
	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kBuildingOID[];		//!< 建物ポリゴンOID
	extern const _TCHAR kSrcOID[];			//!< 参照素材OID
	extern const _TCHAR kSurveyDate[];		//!< 調査日
	extern const _TCHAR kAnyPlaceID[];		//!< AnyPlaceID
	extern const _TCHAR kPriority[];		//!< 信頼度コード
	extern const _TCHAR kMatchPer[];		//!< マッチング率
	extern const _TCHAR kContentsCode[];	//!< 注記POIコンテンツコード
	extern const _TCHAR kContentsSeq[];		//!< 注記POIコンテンツ内シーケンス

	namespace priority_code {
		enum ECode {
			kUnknown				= 0,		//!< 不明
			kEdit					= 1,		//!< 調査によって編集されたもの
			kAnnoPoi				= 2,		//!< 注記POIが採用された場合
			kPri3					= 3,		//!< 完全一致有り・一致個数が一番多いもの
			kPri4					= 4,		//!< 加工後完全一致有り・一致個数が一番多いもの
			kPri5					= 5,		//!< 完全一致あり・優先度が高いもの
			kPri6					= 6,		//!< 加工後一致あり・優先度が高いもの
			kPri7					= 7,		//!< 完全一致あり・文字数が多いもの
			kPri8					= 8,		//!< 加工後一致有り・文字数が多いもの
			kPri9					= 9,		//!< 完全一致あり・文字列をソートして若いもの
			kPri10					= 10,		//!< 加工後一致あり・文字列をソートして若いもの
			kPri11					= 11,		//!< 完全一致無し・マッチング率が高いもの
			kPri12					= 12,		//!< 完全一致無し・優先度が高いもの
			kPri13					= 13,		//!< 完全一致無し・文字数が多いもの
			kPri14					= 14,		//!< 完全一致無し・文字列をソートして若い順
			kOneFirePreventive		= 15,		//!< 建物上に防火対象情報がユニークに存在し、採用されたもの
			kOneImmovalbles			= 16,		//!< 建物上に不動産情報がユニークに存在し、採用されたもの
			kNPartFirePreventive	= 17,		//!< 建物上に防火対象情報のみ複数存在し、その中で一つ採用されたもの
			kNPartImmovalbles		= 18,		//!< 建物上に不動産情報のみ複数存在し、その中で一つ採用されたもの
			kCompAnnoPoi			= 19,		//!< 注記POI（補完用）が採用された場合
		};
	}
} // buildingname_point

// 建物ビル名称紐づきテーブル
namespace rel_buildingname {
	using namespace sindy::schema::object;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBuildingNameOID[];	//!< 建物ビル名称OID
	extern const _TCHAR kSrcOID[];
} // rel_buildingname

// 鉄道リンク
namespace railway_link {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kFromNodeOID[];			//!< 始点ノードのOID
	extern const _TCHAR kToNodeOID[];			//!< 終点ノードのOID
	extern const _TCHAR kRailwayClass_C[];		//!< 鉄道種別
	extern const _TCHAR kDispScale_C[];			//!< スケール表示種別
	extern const _TCHAR kUnderGround_F[];		//!< 地下フラグ

	// 鉄道リンク種別コード
	namespace link_class{
		enum ECode{
			kDivideByPoi = 0,		//!< 鉄道POI種別で分類
			kFreightLine = 1,		//!< 貨物線
			kSideTrack   = 2		//!< 鉄道側線部
		};
	}// node_code

	// スケール表示種別コード
	namespace disp_scale{
		enum ECode{
			kS1toS4 = 0,		//!< S1～S4で表示
			kS2toS4 = 1,		//!< S2～S4で表示
			kS3toS4 = 2,		//!< S3～S4で表示
			k4      = 3			//!< S4で表示
		};
	}// disp_scale

} // railway_link

// 鉄道リンク紐付きテーブル
namespace rel_railway_link {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kRwLinkID[];			//!< 鉄道リンクのOID
	extern const _TCHAR kRailwayLineID[];		//!< 鉄道路線のOID
} // rel_railway_link

// 鉄道ノード
namespace railway_node {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kNodeClass_C[];			//!< ノード種別

	// 鉄道ノード種別コード
	namespace node_class{
		enum ECode{
			kNone      = 0,		//!< 機能なしノード
			kTerminal  = 1,		//!< 端点ノード
			kStation   = 2,		//!< 駅POIノード
			kAlter     = 3,		//!< 属性変化点ノード
			kBranch    = 4,		//!< 分岐ノード
			kOutline   = 99		//!< 図郭上ノード
		};
	}// node_code

} // railway_node

// 鉄道ノード紐付きテーブル
namespace rel_railway_node {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kRwNodeID[];			//!< 鉄道ノードのOID
	extern const _TCHAR kJoinLineInfoID[];		//!< 乗り入れ路線のOID
	extern const _TCHAR kStationNo[];			//!< 駅番号
} // rel_railway_node


// 鉄道カラー
namespace railway_color {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kRailwayLineID[];		//!< 鉄道路線のOID
	extern const _TCHAR kColorName[];			//!< 色名称
	extern const _TCHAR kColorNameEng[];		//!< 色名称(英語) 
	extern const _TCHAR kColorCode[];			//!< カラーコード
} // railway_color


// 詳細ポリゴン
namespace station_detail {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kPolygonType_C[];		//!< ポリゴン種別

	// 駅ポリゴン種別コード
	namespace polygontype_class{
		enum ECode{
			kMainFacility       = 1,	//!< 駅主要施設
			kCommercialFacility = 2,	//!< 駅関連商業施設
			kPlatform           = 3		//!< 駅プラットフォーム
		};
	} // polygontype_class

} // station_detail


// プラットフォームポリゴン
namespace midzoom_platform {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名
} // midzoom_platform


// 駅詳細ポリゴン
namespace highzoom_station {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名
} // highzoom_station


// 駅簡易ポリゴン
namespace midzoom_station {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名
} // midzoom_station


// プラットフォーム簡易ポリゴン
namespace lowzoom_platform {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< テーブル名
} // lowzoom_platform


// 駅ポリゴン紐付きテーブル
namespace rel_station_site {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kGroupID[];			//!< 駅ID
	extern const _TCHAR kLayerNo_C[];		//!< レイヤー番号
	extern const _TCHAR kLayerOID[];		//!< レイヤー内OID

	// 駅ポリゴンレイヤー番号コード
	namespace layerno_class{
		enum ECode{
			kStationDetail    = 1,		//!< 詳細ポリゴン
			kMidzoomPlatform  = 2,		//!< プラットフォームポリゴン
			kHighzoomStation  = 3,		//!< 駅詳細ポリゴン
			kMidzoomStation   = 4,		//!< 駅簡易ポリゴン
			kLowzoomPlatform  = 5		//!< プラットフォーム全体ポリゴン
		}; // layerno_class
	}
} // rel_station_site


// グループIDマスター
namespace groupid_master {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kStationName[];			//!< 駅名称
} // groupid_master

// 道路名称テーブル
namespace road_name_list {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kNameClass[];	//!< 道路名称種別
	extern const _TCHAR kNameKanji[];	//!< 名称
	extern const _TCHAR kNameYomi[];	//!< 読み

	// 道路経路用種別コード
	namespace name_class {
		enum ECode {
			kNone = 0, //!< 未設定
			kShieldRoadNo = 1,		//!< 標識道路番号
			kShieldHwyNo = 2,		//!< 標識高速番号
			kNationalRoadNo = 3,	//!< 国道番号
			kPrefRoadNoName = 4,	//!< 県道番号・名称
			kPrefRoadNo = 5,		//!< 県道番号
			kPrefRoadNoAbbr = 6,	//!< 県道番号略称
			kPrefRoadName = 7,		//!< 県道名称
			kRoadName = 8,			//!< 路線名称
			kRoadHwyAbbr = 9,		//!< 高速道路略称
			kRoadCityHwyAbbr = 10,	//!< 都市高速略称
			kRoadCityHwyAbbr2 = 11,	//!< 都市高速略称2
			kSectionName = 12		//!< 区間名称
		};
	}
} // road_name_list

} // sj

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_SJ_H_
