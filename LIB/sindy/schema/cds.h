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

#ifndef SINDY_SCHEMA_CDS_H_
#define SINDY_SCHEMA_CDS_H_

#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

namespace city_annotation_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kNameString1[];		//!< 注記文字列1
	extern const _TCHAR kNameString2[];		//!< 注記文字列2
	extern const _TCHAR kStringNum1[];		//!< 注記1文字数
	extern const _TCHAR kStringNum2[];		//!< 注記2文字数
	extern const _TCHAR kAnnoClass[];		//!< 注記種別
	extern const _TCHAR kNameYomi[];		//!< 注記読み
	extern const _TCHAR kEngDefault[];		//!< デフォルト英語注記
	extern const _TCHAR kEngName[];			//!< 個別英語注記
	extern const _TCHAR kEngOut[];			//!< 英語注記出力フラグ
	extern const _TCHAR kDispType[];		//!< 表示タイプ
	extern const _TCHAR kHimawari[];		//!< ひまわりフラグ
	extern const _TCHAR kContentsCode[];	//!< コンテンツコード
	extern const _TCHAR kContentsSeq[];		//!< コンテンツシーケンス
	extern const _TCHAR kAnnoDisp[];		//!< 注記表示コード

	//// CDS注記種別コード
	//namespace annotation_code {
	//	enum ECode {
	//	};
	//} // annotation_code

	// CDS注記表示タイプ
	namespace disp_type {
		enum ECode {
			kStr	= 1,	//!< 文字列のみ
			kSymStr	= 2,	//!< 記号＋文字列
		};
	} // disp_type

} // city_annotation_gs

namespace building_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kBldClass[];		//!< 建物種別
	extern const _TCHAR kCorridor[];		//!< 渡り廊下フラグ
	extern const _TCHAR kNoWall[];			//!< 無壁フラグ
	extern const _TCHAR kBldName[];			//!< 建物名称
	extern const _TCHAR kBldAddr[];			//!< 建物住所
	extern const _TCHAR kGeospaceID[];		//!< GEOSPACE家形ID

	// CDS都市建物種別コード
	namespace bld_class {
		enum ECode {
			kNone							=        0, //!< 未調査・作業中
			kNormalRailwayStation			=        1, //!< 普通鉄道駅(地上)
			kSubwayStation					=        2, //!< 地下鉄駅(地上)
			kOtherStationHome				=        3, //!< 地下鉄以外駅ホーム（地上）
			kSubwayStationHome				=        4, //!< 地下鉄駅ホーム（地上）
			kGovmentOffice					=        5, //!< 官公庁
			kHospital						=        6, //!< 病院
			kSchool							=        7, //!< 学校
			KHighRiseComplexHousing			=        8, //!< 高層集合住宅
			kPlant							=        9, //!< 工場・銀行・ビル
			kHotel							=       11, //!< ホテル
			kDepartmentStore				=       12, //!< 娯楽・デパート
			kRailroadBuilding 				=       13, //!< 駅ビル
			kMarket							=		16, //!< 市場
			kFactory						=		17,	//!< 工場
			kStoreHouse						=		18, //!< 倉庫
			kSurfaceRailwayStation			=		19,	//!< 路面鉄道駅(地上)
			kArcade							=		26,	//!< アーケード(面)
			kTollGate						=		27,	//!< 料金所(面)
			kUniversalTrafficStation		=		28,	//!< 新交通駅(地上)
			kMonorailStation				=		29,	//!< モノレール駅(地上)
			kCableCarStation				=		30,	//!< ケーブルカー駅(地上)
			kForce							=		31,	//!< 自衛隊・米軍
			kGasStation						=		32, //!< ガソリンスタンド
			kConfirmationTenantAttr			=		99, //!< テナント属性確認
		};
	} // bld_class

} // building_gs

namespace city_line_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kBgClass[];			//!< 背景ライン種別

	// CDS都市背景ライン種別コード
	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kDashe		 				=        2, //!< 破線表示
			kWatergate 					=        4, //!< 水門
			kErosionControlDam			=		 5,	//!< 砂防ダム
			kStairsDecoration			=		15,	//!< 階段装飾線
			kDivider					=		20,	//!< 中央分離帯（ライン）
			kRoadSite					=		21,	//!< 道路背景
			kContour					=      101, //!< 等高線
			kPrefecturesBorder			=      111, //!< 都道府県界
			kMunicipalityBorder			=      112, //!< 市町村界
			kWardBorder					=      113, //!< 区界
			kOazaBorder					=      114, //!< 大字界
			kKoazaBorder				=      115, //!< 線小字界
			kRiver						=      121, //!< 河川
			kLake                       =      122, //!< 線池・湖
			kCoastline                  =      123, //!< 海岸線
			kLineRoad                   =      131, //!< 線道路
			kGardenPath                 =      132, //!< 庭園路
			kFootBridge                 =      133, //!< 歩道橋
			kTank                       =      141, //!< タンク
		};
	} // bg_class

} // city_line_gs

namespace city_railway_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kRailwayClass[];	//!< 鉄道種別
	extern const _TCHAR kUnderground[];		//!< 地下フラグ

	// CDS都市鉄道種別コード
	namespace railway_class {
		enum ECode {
			kNone						=         0, //!< 未調査・作業中
			kNormalRailwayStation		=         1, //!< 普通鉄道駅(地上)
			kSubway 					=         2, //!< 地下鉄
			kSurfaceRailway			    =		  3, //!< 路面鉄道
			kSpecialOrbit		    	=		101, //!< 特殊軌道
		};
	} // railway_class

} // city_railway_gs

namespace city_site_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kBgClass[];			//!< 背景種別

	// CDS都市背景ポリゴン種別コード
	namespace bg_class {
		enum ECode {
			kNone								=         0, //!< 未調査・作業中
			kPark								=        14, //!< 植栽地
			kRiver								=		 20, //!< 河川
			kSea								=		 23, //!< 海
			kLake								=		 24, //!< 池・湖
			kInterCityTollExpressway			=		 30, //!< 都市間高速(有料)
			kNationalHighway					=        31, //!< 国道
			kPrincipalPrefecturalRoad 			=        32, //!< 主要地方道
			kPrefectureNormalRoad				=		 33, //!< 一般都道府県道 
			kSideway							=        34, //!< 歩道
			kGardenPath							=        35, //!< 庭園路
			kCenterDivider						=        37, //!< 中央分離帯(標準)
			kInterCityFreeExpressway			=		 38, //!< 都市間高速(無料)
			kOtherSite							=        51, //!< 舗装地その他敷地
			kUrbanTollExpressway				=		 70, //!< 都市高速(有料)
			kUrbanFreeExpressway				=		 71, //!< 都市高速(無料)
			kTollRoadPolygon					=		 72, //!< 有料道路ポリゴン
			kOtherArterialRoad					=		 73, //!< その他幹線道路
			kOtherRoad							=		 74, //!< その他道
			kHighwayPolygon						=		101, //!< 高速道路ポリゴン（立体交差）
			kUrbanExpresswayPolygon				=		111, //!< 都市高速道路ポリゴン（立体交差）
			kNationalHighwayPolygon				=		121, //!< 国道ポリゴン（立体交差）
			kPrincipalPrefecturalRoadPolygon	=		131, //!< 主要地方道ポリゴン（立体交差）
			kPrefectureNormalRoadPolygon		=		141, //!< 一般都道府県道ポリゴン（立体交差）
			kSidewayPolygon						=		161, //!< 歩道ポリゴン（全道路）
			kTollRoadPolygonCross				=		171, //!< 有料道路ポリゴン（立体交差）
		};
	} // bg_class

} // city_site_gs

namespace land_cover_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

} // land_cover_gs

namespace underground_area_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< テーブル名

} // underground_area_gs

namespace bld_tenant_gs {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kGeospaceID[];		//!< 家屋形状地物ID
	extern const _TCHAR kLClass[];			//!< 大分類コード
	extern const _TCHAR kMClass[];			//!< 中分類コード
	extern const _TCHAR kSClass[];			//!< 小分類コード
	extern const _TCHAR kPopularName[];		//!< 通称名称
	extern const _TCHAR kSignageName[];		//!< 表記名称
	extern const _TCHAR kNameKana[];		//!< カナ名称
	extern const _TCHAR kIconClass[];		//!< アイコン種別
	extern const _TCHAR kReserved[];		//!< reserved
	extern const _TCHAR kAddrCode11[];		//!< 住所コード11桁
	extern const _TCHAR kPrefKanji[];		//!< 漢字県名
	extern const _TCHAR kCityKanji[];		//!< 漢字市町村名
	extern const _TCHAR kOazaKanji[];		//!< 漢字大字名
	extern const _TCHAR kAzaKanji[];		//!< 漢字字・丁目
	extern const _TCHAR kBlockKanji[];		//!< 番地
	extern const _TCHAR kGoKanji[];			//!< 号
	extern const _TCHAR kBranchNum[];		//!< 枝番
	extern const _TCHAR kPrefKana[];		//!< 半角カナ県名
	extern const _TCHAR kCityKana[];		//!< 半角カナ市町村名
	extern const _TCHAR kOazaKana[];		//!< 半角カナ大字名
	extern const _TCHAR kAzaKana[];			//!< 半角カナ字・丁目
	extern const _TCHAR kPostNum[];			//!< 郵便番号

} // bld_tenant_gs

/// 都市地図駅舎ポリゴンテーブル
namespace city_station_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kStationClass[];		//!< 鉄道駅種別コード

	namespace station_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kOtherStation				=        1, //!< 地下鉄以外駅（地下）
			kSubwayStation				=        2, //!< 地下鉄駅（地下）
			kUniversalTrafficStation	=		 3,	//!< 新交通システム（地下）
			kMonorailStation			=		 4,	//!< モノレール（地下）
			kCableCarStation			=		 5,	//!< ケーブルカー（地下）
		};
	} // station_class
} // city_station_gs

} // schema

} // sindy

#endif // SINDY_SCHEMA_CDS_H_
