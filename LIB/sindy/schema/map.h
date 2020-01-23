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

#ifndef SINDY_SCHEMA_MAP_H_
#define SINDY_SCHEMA_MAP_H_

#include <sindy/schema/common.h>
#include <sindy/schema/reference.h>

namespace sindy {

namespace schema {

namespace scaledisp_flag {
	extern const _TCHAR kSC1Disp[];			//!< スケール1表示フラグ
	extern const _TCHAR kSC2Disp[];			//!< スケール2表示フラグ
	extern const _TCHAR kSC3Disp[];			//!< スケール3表示フラグ
	extern const _TCHAR kSC4Disp[];			//!< スケール4表示フラグ
} // scaledisp_flag

namespace scale_class {
	extern const _TCHAR kSC1BgClass[];		//!< スケール1背景種別コード
	extern const _TCHAR kSC2BgClass[];		//!< スケール2背景種別コード
	extern const _TCHAR kSC3BgClass[];		//!< スケール3背景種別コード
	extern const _TCHAR kSC4BgClass[];		//!< スケール4背景種別コード
} // scale_class

// クレームポリゴン
namespace claimpoly {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kClaimType[];		//!< クレームタイプコード
	extern const _TCHAR kClaimNo[];			//!< クレーム番号
	extern const _TCHAR kComFirm[];			//!< 常時確認フラグ
	extern const _TCHAR kUseLess[];			//!< 不要フラグ
	extern const _TCHAR kRemarks[];			//!< コメント、備考

	namespace claim_type {
		enum ECode {
			kBackward			=        1, //!< 旧過去トラ
			kCSDB				=        2, //!< CSDB
			kCQ					=        3, //!< CQ
			kTopic				=        4, //!< トピック
			kTDC				=        5, //!< TDC掲示板
			kOther				=        9, //!< その他
		};
	} // claim_type
} // claimpoly

/// 建物ポリゴンテーブル
namespace building {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];  //!< テーブル名

	extern const _TCHAR kBldClass[];   //!< 建物種別コード
	extern const _TCHAR kCorridor[];   //!< 渡り廊下フラグ
	extern const _TCHAR kNoWall[];     //!< 無壁舎フラグ
	extern const _TCHAR kGeospaceID[]; //!< GEOSPACE_ID

	namespace bld_class {
		enum ECode {
			kNone							=        0, //!< 未調査・作業中
			kOtherStation					=        1, //!< 地下鉄以外駅（地上）
			kSubwayStation					=        2, //!< 地下鉄駅（地上）
			kOtherStationHome				=        3, //!< 地下鉄以外駅ホーム（地上）
			kSubwayStationHome				=        4, //!< 地下鉄駅ホーム（地上）
			kGovmentOffice					=        5, //!< 官公庁
			kHospital						=        6, //!< 病院
			kSchool							=        7, //!< 学校
			KHighRiseComplexHousing			=        8, //!< 高層集合住宅
			kPlant							=        9, //!< 工場・銀行・ビル
			kTank							=       10, //!< タンク
			kHotel							=       11, //!< ホテル
			kDepartmentStroe				=       12, //!< 娯楽・デパート
			kRailroadBuilding 				=       13, //!< 駅ビル
			kPedestrianDeck 				=       15, //!< ペデストリアンデッキ（平面）
			kMarket							=		16, //!< 市場
			kFactory						=		17,	//!< 工場
			kStoreHouse						=		18, //!< 倉庫
			kSurfaceRailwayStation			=		19,	//!< 路面鉄道駅(地上)
			kFootBridge						=		20,	//!< 歩道橋
			kPedestrianDeckStairsEscalator	=		23,	//!< ぺデストリアンデッキ（階段・エスカレータ）
			kSiteStairs						=		25,	//!< 敷地階段
			kArcade							=		26,	//!< アーケード
			kTollGate						=		27,	//!< 料金所
			kUniversalTrafficStation		=		28,	//!< 新交通駅（地上）
			kMonorailStation				=		29,	//!< モノレール駅（地上）
			kCableCarStation				=		30,	//!< ケーブルカー駅（地上）
			kForce							=		31,	//!< 自衛隊・米軍
			kGasStation						=		32, //!< ガソリンスタンド
		};
	} // bld_class
} // building

/// 建物ラインテーブル
namespace building_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBldClass[];			//!< 建物ライン種別コード

	namespace bld_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kDecoration				=        1, //!< 装飾線
			kStairsDecoration		=		 2,	//!< 階段装飾線
			kParent					=		 3, //!< 親線
			kStairsTread			=		 4, //!< 階段踏面ライン（敷地階段・ペデ階段）
		};
	} // bld_class
} // building_line

/// 建物段差ポリゴンテーブル
namespace building_step {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kFloors[];				//!< 階数
	extern const _TCHAR kCheck[];				//!< 階数情報確認フラグ
	extern const _TCHAR kBuildingID[];          //!< 建物ID
} // building_step

/// 都市地図背景ポリゴンテーブル
namespace city_site {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBgClass[];				//!< 背景種別コード

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kBlock						=        1, //!< 街区
			kSands						=       10, //!< 砂地
			kMarsh						=       11, //!< 湿地
			kForest						=       12, //!< 山林
			kLawnOther					=       13, //!< 芝生等その他緑地
			kPark						=       14, //!< 植栽地
			kRiver						=		20,	//!< 河川
			kLevee						=       21, //!< 堤外地
			kPool						=       22, //!< プール
			kSea						=		23, //!< 海
			kLake						=		24,	//!< 池・湖
			kInterCityTollExpressway	=		30,	//!< 都市間高速道（有料）
			kNationalHighway			=       31, //!< 国道
			kPrincipalPrefecturalRoad 	=       32, //!< 主要地方道
			kPrefectureNormalRoad		=		33,	//!< 一般都道府県道 
			kSideway					=       34, //!< 歩道
			kGardenPath					=       35, //!< 庭園路
			kCenterDivider				=       37, //!< 中央分離帯（標準）
			kInterCityFreeExpressway	=		38,	//!< 都市間高速道（無料）
			kPublicRoadStairs			=		39,	//!< 公道階段
			kHospital					=       40, //!< 病院
			kSchool						=       41, //!< 学校
			kPlant						=       42, //!< 工場
			kDamGuideNotExist			=       45, //!< ダム（便覧記載なし）
			kDamGuideExist				=       46, //!< ダム（便覧記載あり）
			kStationSite				=       50, //!< 鉄道敷地
			kOtherSite					=       51, //!< 舗装地その他敷地
			kBreakwater					=       52, //!< 防波堤
			kForceManeuveringGround		=		55,	//!< 自衛隊・米軍（演習場）
			kForce						=		56,	//!< 自衛隊・米軍（駐屯地ほか）
			kIncompleteArea 			=       60, //!< 不完全エリア
			kUrbanTollExpressway		=		70,	//!< 都市高速（有料）
			kUrbanFreeExpressway		=		71,	//!< 都市高速（無料）
			kTollRoad					=		72,	//!< 有料道路
			kOtherArterialRoad			=		73,	//!< その他幹線道路
			kCenterDividerPark			=       75, //!< 中央分離帯（植栽地）
			kRunway						=		76,	//!< 滑走路
			kUnderRepairRoad			=		77, //!< 工事中道路
			kTollGateLane				=		78, //!< 料金所レーン
			kNarrow						=		79, //!< 細街路
		};
	} // bg_class
} // city_site

/// 都市地図背景ラインテーブル
namespace city_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBgClass[];				//!< 背景ライン種別コード

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kSolid						=        1, //!< 実線表示
			kDashe		 				=        2, //!< 破線表示
			kTollbooth					=        3, //!< 料金所
			kWatergate 					=        4, //!< 水門
			kErosionControlDam			=		 5,	//!< 砂防ダム
			kFerrySeaway 				=       10, //!< フェリー航路
			kPassengerShipSeaway		=       11, //!< その他旅客船航路
			kStairsDecoration			=		15,	//!< 階段装飾線
			kParent						=		16, //!< 親線
			kStairsTread				=		17, //!< 階段踏面ライン（公道階段）
			kDivider					=		20,	//!< 中央分離帯（ライン）
			kRoadSite					=		21,	//!< 道路背景
			kExpressway					=		22,	//!< 高速道路
			kOverheadRoad				=		23,	//!< 道路高架
			kOverheadRailway			=		24,	//!< 鉄道高架
			kChockLine					=		25, //!< 車止めライン
		};
	} // bg_class
} // city_line

/// 都市地図駅舎ポリゴンテーブル
namespace city_station {
	using namespace ::sindy::schema::ipc_table;

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
} // city_station

/// 都市地図鉄道ラインテーブル
namespace city_railway {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kRailwayClass[];		//!< 鉄道種別コード

	namespace railway_class {
		extern LPCTSTR kFieldName; //!< フィールド名
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kNormalRailway			=		 1,	//!< 普通鉄道
			kSubway					=        2, //!< 地下鉄
			kSurfaceRailway			=		 3, //!< 路面鉄道
			kRollerCoster			=		 4,	//!< ジェットコースター等
			kUniversalTraffic		=		 5,	//!< 新交通システム
			kMonorail				=		 6,	//!< モノレール
			kCableCar				=		 7,	//!< ケーブルカー
		};
	} // railway_class
} // city_railway

/// 地下ポリゴン(歩行者ナビ向け)
namespace underground_site {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kUgClass[];			//!< 地下背景種別

	namespace ug_class {
		enum ECode {
			kUnder			=	1,			//!< 地下
			kStairs			=	2			//!< 地下階段
		};
	} // ug_class

} // underground

/// 地下ライン(歩行者ナビ向け)
namespace underground_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kUgClass[];			//!< 地下背景ライン種別

	namespace ug_class {
		enum ECode {
			kStairsDecoration		=		 2,	//!< 階段装飾線
			kParent					=		 3	//!< 親線
		};
	} // ug_class

} // underground_line

/// 都市地図行政界ポリゴンテーブル
namespace city_admin {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityCode[];			//!< 市区町村コード
	extern const _TCHAR kAddrCode[];			//!< 住所コード
	extern const _TCHAR kAddrCode2[];			//!< 住所コード2
	extern const _TCHAR kTmpCode[];				//!< 仮住所コードフラグ
	extern const _TCHAR kGaikuFugo[];			//!< 街区符号
	extern const _TCHAR kExtGaikuFugo[];		//!< 拡張街区符号
	extern const _TCHAR kAddrClass[];			//!< 住所整備種別
	extern const _TCHAR kAreaClass[];			//!< エリア判別種別
	extern const _TCHAR kManage[];				//!< 管理フラグ
	extern const _TCHAR kDetach[];				//!< 飛地フラグ
	extern const _TCHAR kColorCode[];			//!< 街区色付与用色属性
	extern const _TCHAR kWorkColor[];			//!< 街区色付与作業用
	extern const _TCHAR kConvType[];			//!< 住所変換モード

	namespace addr_class {
		enum ECode {
			kAzaUndeveloped								=        0, //!< 町丁目界未整備
			kResidenceIndicationMaintenance 		 	=        1, //!< 住居表示整備済み
			kChibanMaintenance							=        2, //!< 地番整備済み
			kResidenceIndicationAndChibanUndeveloped	=        3, //!< 住居表示・地番未整備
		};
	} // addr_class

	namespace area_class {
		enum ECode {
			kOutArea				=        0, //!< エリア外
			kInArea					=        1, //!< エリア内
			kSea					=        2, //!< 海
		};
	} // area_class

	namespace conv_type {
		enum ECode {
			kNormal					=        0, //!< 通常
			kOazaGeneration			=        1, //!< 大字も生成
			kAzaGeneration			=        2, //!< 字も生成
		};
	} // conv_type
} // city_admin

namespace city_rep_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kChibanNo[];			//!< 地番
	extern const _TCHAR kAnno[];				//!< 地番注記フラグ
	extern const _TCHAR kPointClass[];			//!< ポイント種別
	extern const _TCHAR kAddrCode[];			//!< 住所１１桁コード
	extern const _TCHAR kRepType[];				//!< 代表点／地番止まりフラグ

	namespace point_class {
		enum ECode {
			kChiban						=        0, //!< 地番（16桁）
			kAza						=        1, //!< 町丁目・字（11桁）
			kOaza						=        2, //!< 大字（8桁）
		};
	} // point_class

	namespace rep_type {
		enum ECode {
			kIncertitude 				=        0, //!< 不定（大字・町長目代表点の時のみ）
			kArea_OFF					=        1, //!< エリア/OFF
			kOFF_ON						=        2, //!< OFF/ON
			kOFF_OFF					=        3, //!< OFF/OFF
			kMultiple_OFF				=        4, //!< 複数/OFF
			kMultiple_ON				=        5, //!< 複数/ON
		};
	} // rep_type
} // city_rep_point

namespace cityplanmap {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kMapID[];				//!< 図葉ID
	extern const _TCHAR kCityCode[];			//!< 行政区域コード
	extern const _TCHAR kCityName[];			//!< 行政名
	extern const _TCHAR kMapName[];				//!< 図葉名
	extern const _TCHAR kMapNo[];				//!< 図葉NO
	extern const _TCHAR kPubDate[];				//!< 作成年度
} // cityplanmap

/// 中縮背景ポリゴンテーブル
namespace base_site {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scale_class;

	extern const _TCHAR kTableName[];			//!< テーブル名

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kNoDisp						=        1, //!< 非表示
			kSea						=   101001, //!< 海
			kRiver						=   101002, //!< 河川・沢・谷
			kLake						=   101003, //!< 湖・池・沼
			kSands						=   102001, //!< 砂地
			kMarsh						=   102002, //!< 湿地
			kLevee						=   102003, //!< 堤外地
			kGround						=   102005, //!< 地面（ポリゴン内地面）
			kParking					=   201001, //!< 駐車場
			kRoadStation				=   201002, //!< 道の駅
			kTrafficHome				=   201003, //!< その他交通関連施設
			kRailwayHome				=   202001, //!< その他の鉄道施設
			kFerryTerminal				=   203001, //!< フェリーターミナルの敷地
			kAirport					=   203002, //!< 空港の敷地
			kPrivateAirport				=   203003, //!< 民間飛行場の敷地
			kHeliport					=   203004, //!< ヘリポートの敷地
			kMarina						=   203005, //!< ヨットハーバー・マリーナ
			kRunway						=	203006,	//!< 滑走路
			kPref						=   301001, //!< 都道府県庁
			kCityHall					=   301002, //!< 市役所
			kWardOffice					=   301003, //!< 区役所
			kTownOffice					=   301004, //!< 町村役場
			kBranchOffice				=   301005, //!< 市町村役場出張所
			kGovmentOffice				=   302001, //!< 官公署
			kCourt						=   302002, //!< 裁判所
			kPrison						=   302003, //!< 刑務所
			kHealth						=   302004, //!< 保健所
			kPoliceOffice				=   302005, //!< 警察署
			kFireOffice					=   302006, //!< 消防署
			kTaxOffice					=   302007, //!< 税務署
			kPostOffice					=   302008, //!< 郵便局
			kForestManagedSite			=   302009, //!< 森林管理所の敷地
			kSelfDifenceForce			=   303001, //!< 自衛隊
			kUnitedStatesArmy			=   303002, //!< 米軍
			kPublicHall					=   304001, //!< 公民館
			kImperialFamily				=   304002, //!< 皇室施設
			kPublicInstitution			=   304003, //!< その他の公共施設
			kElementarySchool			=   401001, //!< 小学校
			kJuniorHighSchool			=   401002, //!< 中学校
			kSeniorHighSchool			=   401003, //!< 高校
			kSpecialHighSchool			=   401004, //!< 高専
			kJuniorCollege				=   401005, //!< 短期大学
			kUniversityCollege			=   401006, //!< 大学・大学校
			kSpecialSchool				=   401007, //!< 専修学校・各種学校
			kNurseSchool				=   401008, //!< 盲・聾・養護学校
			kDriversSchool				=   401009, //!< 自動車学校
			kSchool						=   401010, //!< その他の学校
			kHospital					=   402001, //!< 病院
			kPark						=   501001, //!< 公園・庭園・緑地・植栽地
			kAmusementPark				=   501002, //!< 遊園地
			kThemePark					=   501003, //!< テーマパーク
			kResort						=   501004, //!< 総合リゾート
			kZoo						=   501005, //!< 動物園
			kAquarium					=   501006, //!< 水族館
			kBotanicalGarden			=   501007, //!< 植物園
			kArtMuseum					=   501008, //!< 美術館
			kMuseum						=   501009, //!< 博物館・文学館・科学館・資料館
			kLibrary					=   501010, //!< 公立図書館
			kStudyMuseum				=   501011, //!< 体験・学習館
			kPlayhouse					=   501012, //!< ホール・劇場・文化会館
			kRaceTrack					=   501013, //!< 競馬場
			kBicycleRaceTrack			=   501014, //!< 競輪場
			kMotorbortRaceTrack			=   501015, //!< 競艇場
			kMotorcycleRaceTrack		=   501016, //!< オートレース場
			kSwimmingPool				=   501017, //!< プール
			kSpaHouse					=   501018, //!< 健康ランド
			kAmusementFacilities		=   501019, //!< その他の娯楽施設
			kFamilyRestaurant			=   502001, //!< ファミリーレストラン
			kFastFood					=   502002, //!< ファーストフード
			kRestaurant					=   502003, //!< 飲食店・レストラン
			kDepartmentStore			=   502004, //!< 百貨店
			kShoppingCenter				=   502005, //!< ショッピングセンター
			kSupermarket				=   502006, //!< スーパー
			kCommerseFacillities		=   502007, //!< 複合商業施設
			kDIY						=   502008, //!< ＤＩＹ
			kCarDealer					=   502009, //!< カーディーラー
			kCarShop					=   502010, //!< 自動車用品店
			kSpatialiryStore			=   502011, //!< 専門店
			kTouristMarket				=   502012, //!< 物産館・観光市場
			kMarket						=   502013, //!< 市場
			kWeddingHall				=   502014, //!< 結婚式場
			kAthleticGround				=   503001, //!< 陸上競技場
			kGym						=   503002, //!< 体育館
			kBaseballGround				=   503003, //!< 野球場
			kSkiingGround				=   503004, //!< スキー場
			kSoccerGround				=   503005, //!< サッカー場
			kRugbyGround				=   503006, //!< ラグビー場
			kPublicGolfLinks			=   503007, //!< パブリックゴルフ場
			kSemipublicGolfLinks		=   503008, //!< セミパブリックゴルフ場
			kMembersGolfLinks			=   503009, //!< メンバーズゴルフ場
			kGolfLinks					=   503010, //!< その他のゴルフ場
			kDrivingRange				=   503011, //!< ゴルフ練習場
			kTennisCourt				=   503012, //!< テニスコート
			kPlayPark					=   503013, //!< 総合スポーツ公園
			kMotorSportsPark			=   503014, //!< モータースポーツ施設
			kCyclingPark				=   503015, //!< サイクルスポーツセンター
			kRidingTrack				=   503016, //!< 乗馬
			kAutoCampingSite			=   503017, //!< オートキャンプ場
			kCampingSite				=   503018, //!< 通常キャンプ場
			kBowlingAlley				=   503019, //!< ボウリング場
			kShootingGallery			=   503020, //!< 射撃場
			kOtherSportsPark			=   503021, //!< その他のスポーツ施設
			kCastle						=   504001, //!< 城・城跡
			kHistoricSite				=   504002, //!< 史跡・名勝・天然記念物
			kObservatory				=   504003, //!< 展望台
			kSpa						=   504004, //!< 温泉
			kRanch						=   504005, //!< 牧場
			kTuristRanch				=   504006, //!< 観光牧場
			kTuristFarm					=   504007, //!< 観光農園
			kTuristOrchard				=   504008, //!< 観光果樹園
			kPleasureBoat				=   504009, //!< 観光遊覧船乗場
			kPlant						=   601001, //!< 工場
			kLaboratory					=   601002, //!< 研究所
			kPublicLabo					=   601003, //!< 公設の研究所
			kDispositionPlant			=   601004, //!< 浄水場・終末処理場
			kPowerPlant					=   601005, //!< 発電所・変電所
			kStoreHouse					=   601006, //!< 倉庫
			kBank						=   602001, //!< 銀行
			kNTT						=   602002, //!< ＮＴＴ
			kOtherPrincipalCorporate	=   602003, //!< その他の重要企業施設
			kShintoShrine				=   603001, //!< 神社
			kBuddhistTemple				=   603002, //!< 寺院
			kChurch						=   603003, //!< キリスト教会・天主堂
			kCemetery					=   603004, //!< 墓地
			kReligionSite				=   603005, //!< その他の宗教施設
			kYouthHouse					=   604001, //!< 青年の家
			kMine						=   604002, //!< 鉱山
			kFuneralHall				=   604003, //!< 斎場
			kInstitution				=   604004, //!< その他施設
			kHotel						=   701001, //!< ホテル
			kPublicInn					=   701002, //!< 公共宿泊施設
			kDam						=   801001, //!< ダム
			kMausoleum					=   801002, //!< 陵墓
			kBuilding					=   900001, //!< 建物（ポリゴン内施設）
		};
	} // bg_class
} // base_site

/// 中縮背景ラインテーブル
namespace base_line {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;


	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kBgClass[];				//!< 背景ライン種別コード

	namespace bg_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kRiver					=        1, //!< 河川・沢・谷
			kBreakwater				=        2, //!< 防波堤
			kPrefecturesBorder		=       11, //!< 都道府県界
			kMunicipalityBorder		=       12, //!< 市区町村界
		};
	} // bg_class
} // base_line

/// 中縮背景駅舎ポリゴンテーブル
namespace base_station {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kStationClass[];		//!< 鉄道駅種別コード

	namespace station_class {
		enum ECode {
			kNone							=        0, //!< 未調査・作業中
			kBulletTrainStation 			=        1, //!< 新幹線駅
			kJRStation						=        2, //!< ＪＲ駅
			kThirdSectorCorporationStation	=        3, //!< 第３セクター駅
			kPrivateRailwayStation			=        4, //!< 私鉄駅
			kSurfaceRailwayStation			=        5, //!< 路面鉄道駅
			kSubwayStation					=        6, //!< 地下鉄駅
			kMonorailStation				=        7, //!< モノレール駅
			kUniversalTrafficStation		=        8, //!< 新交通駅
			kCableCarStation				=       10, //!< ケーブルカー駅
			kGoodsLineStation				=       11, //!< 貨物線駅
		};
	} // station_class
} // base_station

/// 中縮背景鉄道ラインテーブル
namespace base_railway {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kRailwayClass[];		//!< 鉄道種別コード

	namespace railway_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kBulletTrain			=        1, //!< 新幹線
			kJR						=        2, //!< ＪＲ
			kThirdSectorCorporation	=        3, //!< 第３セクター
			kPrivateRailway			=        4, //!< 私鉄
			kSurfaceRailway			=        5, //!< 路面鉄道
			kSubway					=        6, //!< 地下鉄
			kMonorail				=        7, //!< モノレール
			kUniversalTraffic		=        8, //!< 新交通
			kAerialCableway			=        9, //!< ロープウェイ
			kCableCar				=       10, //!< ケーブルカー
			kGoodsLine				=       11, //!< 貨物線
			kRailwaySideTrack		=       20, //!< 鉄道側線部
		};
	} // railway_class
} // base_railway

/// 中縮背景地面テーブル
namespace base_land {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
} // base_land

/// 中縮行政界ポリゴンテーブル
namespace base_admin {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityCode[];			//!< 市区町村コード
} // base_admin

namespace base_rep_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kAddrCode[];			//!< 住所１１桁コード
	extern const _TCHAR kAdminName[];			//!< 代表点名称
	extern const _TCHAR kZipCode[];				//!< 郵便番号
} // base_rep_point

/// 中縮段彩ポリゴンテーブル
namespace base_contour {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCntClass[];			//!< 段彩種別コード

	namespace cnt_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			k500_1000					=        2, //!< 段彩500m以上1000m未満
			k1000_1500					=        3, //!< 段彩1000m以上1500m未満
			k1500_2000					=        4, //!< 段彩1500m以上2000m未満
			k2000_2500					=        5, //!< 段彩2000m以上2500m未満
			k2500_3000					=        6, //!< 段彩2500m以上3000m未満
			k3000_3500					=        7, //!< 段彩3000m以上3500m未満
			k3500_4000					=        8, //!< 段彩3500m以上4000m未満
		};
	} // cnt_class
} // base_contour

/// ミドル背景ポリゴンテーブル
namespace middle_site {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scale_class;

	extern const _TCHAR kTableName[];			//!< テーブル名

	namespace bg_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kNoDisp					=        1, //!< 非表示
			kSea					=   101001, //!< 海
			kRiver					=   101002, //!< 河川・沢・谷
			kLake					=   101003, //!< 湖・池・沼
			kAirport				=   203002, //!< 空港の敷地
			kRunway					=	203006,	//!< 滑走路
			kUrbanArea				=   300000, //!< 市街地
		};
	} // bg_class
} // middle_site

/// ミドル背景ラインテーブル
namespace middle_line {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kBgClass[];				//!< 背景ライン種別コード

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kRiver						=        1, //!< 河川・沢・谷
		};
	} // bg_class
} // middle_line

/// ミドル段彩テーブル
namespace middle_contour {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCntClass[];			//!< 段彩種別コード

	namespace cnt_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			k0_500						=        1, //!< 段彩0ｍ以上500m未満
			k500_1000					=        2, //!< 段彩500m以上1000m未満
			k1000_1500					=        3, //!< 段彩1000m以上1500m未満
			k1500_2000					=        4, //!< 段彩1500m以上2000m未満
			k2000_2500					=        5, //!< 段彩2000m以上2500m未満
			k2500_3000					=        6, //!< 段彩2500m以上3000m未満
			k3000_3500					=        7, //!< 段彩3000m以上3500m未満
			k3500_4000					=        8, //!< 段彩3500m以上4000m未満
		};
	} // cnt_class
} // middle_contour

/// トップ背景ポリゴンテーブル
namespace top_site {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBgClass[];				//!< トップ背景種別コード

	namespace bg_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kSea					=   101001, //!< 海
			kRiver					=   101002, //!< 河川・沢・谷
			kLake					=   101003, //!< 湖・池・沼
			kSands					=   102001, //!< 砂地
		};
	} // bg_class
} // top_site

/// トップ背景ラインテーブル
namespace top_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kBgClass[];				//!< 背景ライン種別コード

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kRiver						=        1, //!< 河川・沢・谷
			kNationalBorderLine			=       10, //!< 国境線
			kPrefecturesBorder			=       11, //!< 都道府県界
		};
	} // bg_class
} // top_line

/// トップ段彩テーブル
namespace top_contour {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCntClass[];			//!< 段彩種別コード

	namespace cnt_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			k0_500						=        1, //!< 段彩0ｍ以上500m未満
			k500_1000					=        2, //!< 段彩500m以上1000m未満
			k1000_1500					=        3, //!< 段彩1000m以上1500m未満
			k1500_2000					=        4, //!< 段彩1500m以上2000m未満
			k2000_2500					=        5, //!< 段彩2000m以上2500m未満
			k2500_3000					=        6, //!< 段彩2500m以上3000m未満
			k3000_3500					=        7, //!< 段彩3000m以上3500m未満
			k3500_4000					=        8, //!< 段彩3500m以上4000m未満
			k4000_4500					=        9, //!< 段彩4000m以上4500m未満
		};
	} // cnt_class
} /// top_contour

/// オルソエリアテーブル
namespace ortho_area {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kTileName[];			//!< 
} // orth_area

namespace adminarea {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityCode[];			//!< 市区町村コード
	extern const _TCHAR kCityName[];			//!< 市区町村名
} // adminarea

namespace aerial_photo {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kMeshCode[];			//!< 都市地図メッシュコード
	extern const _TCHAR kBinderName[];			//!< バインダー名
	extern const _TCHAR kCourse[];				//!< 撮影コース
	extern const _TCHAR kPhotoNo[];				//!< 写真番号
	extern const _TCHAR kShootYear[];			//!< 写真撮影年
	extern const _TCHAR kShootDate[];			//!< 写真撮影年月日
	extern const _TCHAR kOwnerCode[];			//!< 所在地

	namespace owner {
		enum ECode {
			kMorioka						=        1, //!< 盛岡
			kShanghai						=        2, //!< 上海
		};
	} // owner
}

namespace daikei_point {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kAddrCode[];			//!< 住所コード
	extern const _TCHAR kAddrStr[];				//!< 住所文字列
	extern const _TCHAR kSrcType[];				//!< データソースタイプ
	extern const _TCHAR kDupType[];				//!< 重複タイプ
} // daikei_point

namespace parea {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kAddrCode[];			//!< PAREA住所コード
	extern const _TCHAR kRepPointCode[];		//!< 代表点コード
	extern const _TCHAR kUpdateCode[];			//!< 更新コード
	extern const _TCHAR kTmpCode[];				//!< 仮コードフラグ

	namespace update {
		enum ECode {
			kMasterCode						=        0, //!< マスターコード
			kTmpCode						=        1, //!< 仮コード
			kFixCode						=        2, //!< 修正コード
		};
	} // update

	namespace rep_oint {
		enum ECode {
			kOazaRepPoint				=        0, //!< 大字代表点
			kTmpAzaRepPoint				=        1, //!< 仮町丁目代表点
		};
	} // rep_point
} // parea

/// 号テーブル
namespace gou_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kGouNo[];				//!< 号番号
	extern const _TCHAR kAddrCode[];			//!< 20桁住所コード
	extern const _TCHAR kBuildRF[];				//!< 建物関連フラグ
	extern const _TCHAR kBuildID[];				//!< 対応建物ID
	extern const _TCHAR kExpGouNo[];			//!< 拡張番号フラグ
	extern const _TCHAR kTmpCheck[];			//!< 確認フラグ
	extern const _TCHAR kTmpFloors[];			//!< 階数情報
	extern const _TCHAR kGouType[];				//!< 号タイプ
	extern const _TCHAR kPriority[];			//!< 優先フラグ
	extern const _TCHAR kPrecisionCode[];		//!< 住所一致桁数
	extern const _TCHAR kConfirm[];				//!< 住所確認ステータスコード
	extern const _TCHAR kHimawari[];			//!< ひまわりフラグ

	namespace confirm {
		enum ECode {
			kNone					=        0, //!< 未確認
			kUndevelopedAreaNG		=        1, //!< 未整備エリアNG
			kProgramNG				=		 2,	//!< プログラム確認NG
			kWatchingNG				=		 3,	//!< 目視確認確認NG
			kNeedPublicChart		=		 4,	//!< 要公図確認
			kProgramOK16			=		 5,	//!< プログラム確認OK（16桁）
			kProgramOK				=		 6,	//!< プログラム確認OK
			kWatchingOK				=		 7,	//!< 目視確認OK
			kResearchReferenceOK	=		 8,	//!< 調査資料OK
			kPublicReferenceOK		=		 9,	//!< 公的資料OK
		};
	} // confirm

} // gou_point

// 家形リンクテーブル
namespace klink_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kAddrCodeOrg[];			//!< 初期22桁コード
	extern const _TCHAR kAddrCode[];			//!< 最新22桁コード
	extern const _TCHAR kKatagakiName[];		//!< タウンページ方書名称
	extern const _TCHAR kKeisaiName[];			//!< タウンページ掲載名称
	extern const _TCHAR kInfoSrc[];				//!< 情報提供元コード
	extern const _TCHAR kInputSeido[];			//!< 座標精度種別コード
	extern const _TCHAR kReleaseSeido[];		//!< 座標精度種別コード
	extern const _TCHAR kBuildName[];			//!< ビル名称
	extern const _TCHAR kBuildInfoSrc[];		//!< ビル情報提供元コード
	extern const _TCHAR kMaxFloors[];			//!< 最上階数
	extern const _TCHAR kFloor[];				//!< 階数
	extern const _TCHAR kNequal[];				//!< 11桁不一致フラグ

	namespace info_src {
		enum ECode {
			kUnlooked				=        0, //!< 未調査（座標無し）
			kWallLinkData			=        1, //!< 家形リンクデータ
			kGouMaintenanceData 	=        2, //!< 号整備データ
			kWorking				=       10, //!< 作業中
		};
	} // info_src

	namespace input_seido {
		enum ECode {
			kNone						=        0, //!< 未調査
			kNoVerification 			=        1, //!< 未検証
			kMultipleWallCounterData	=        2, //!< 対応家形複数（データ）
			kNotWallCounterData			=        3, //!< 対応家形なし（データ）
			kAddrClericalMistake 		=        4, //!< 住所名誤記
			kUnmatchedName				=        5, //!< 名称不一致
			kUnmatchedShape				=        6, //!< 形状不一致
			kMultipleWallCounterSrc		=        7, //!< 対応家形複数（資料）
			kNotWallCounterSrc			=        8, //!< 対応家形なし（資料）
			kUnmatchedBlock				=        9, //!< 街区不一致
			kOther						=       10, //!< その他
		};
	} // input_seido

	namespace release_seido {
		enum ECode {
			kNone					=        0, //!< 未調査
			kMunicipalityBorder		=        1, //!< 市区郡町村界
			kOazaBorder				=        2, //!< 町・大字界
			kAzaBorder				=        3, //!< 丁目・字（大字）界
			kBlock					=        4, //!< 街区（地番）
			kPinpoint 				=        5, //!< ピンポイント
			kWall					=        6, //!< 家形
			kUnmatchedBlock			=        9, //!< 街区不一致
			kOther					=       10, //!< その他
		};
	} // release_seido

	namespace buildinfo_src {
		enum ECode {
			kUnknown				=        0, //!< 名称不明
			kAutoGeneration 		=        1, //!< 自動生成
			kPhoneSurvey 			=        2, //!< 電話調査
			kReference				=        3, //!< 参考資料
		};
	} // buildinfo_src
} // klink_point



// 住所クレームポリゴンテーブル
namespace claimpoly_addr {
	using namespace ::sindy::schema::claimpoly;

	extern const _TCHAR kTableName[];			//!< テーブル名
} // claimpoly_addr

// 地図クレームポリゴンテーブル
namespace claimpoly_map {
	using namespace ::sindy::schema::claimpoly;

	extern const _TCHAR kTableName[];			//!< テーブル名
} // claimpoly_map

// 住所クレームポイントテーブル
namespace cs_addr_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kAddrCode[];			//!< ２０桁住所コード
} // cs_addr_point

namespace city_admin_c {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityCode[];			//!< 市区町村コード
} // city_admin_c

namespace city_admin_pref {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kCityCode[];			//!< 市区町村コード
} // city_admin_pref

// ExTPポイントテーブル
namespace extp_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR	kAddrCode[];			//!< 20桁住所コード
	extern const _TCHAR	kAddrName[];			//!< 住所名
	extern const _TCHAR	kKatagakiName[];		//!< タウンページ方書名称
	extern const _TCHAR	kKeisaiName[];			//!< タウンページ掲載名称
	extern const _TCHAR	kAddPoint[];			//!< 住所検索ポイント化フラグ
	extern const _TCHAR	kInfoSrc[];				//!< 初期座標提供元コード
	extern const _TCHAR kPointStat[];			//!< 調査結果コード

	namespace info_src {
		enum ECode {
			kiPC	= 1,	//!< iPC
			kAlps	= 2,	//!< アルプス
			kiPC_4	= 3,	//!< iPC(精度4)
		};
	} // info_src

	namespace point_stat {
		enum ECode {
			kNone					= 0,	//!< 未調査
			kUnknownPos				= 1,	//!< 位置不明
			kGou					= 2,	//!< GOU整備
			kGouEx					= 3,	//!< GOU拡張整備
			kTPG					= 4,	//!< TPG整備
			kUndergroundNo			= 5,	//!< 地下街番号
			kReferenceNequal		= 6,	//!< 資料番号と不一致
			kAdminNequal			= 7,	//!< 行政界不一致
			kNoNotKeisai			= 8,	//!< 番号未掲載
			kInstitutionNotKeisai	= 9,	//!< 施設未掲載
		};
	} // point_stat

} // extp_point

// TelTPポイントテーブル
namespace teltp_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR	kTelNum[];				//!< 電話番号
	extern const _TCHAR	kKatagakiName[];		//!< タウンページ方書名称
	extern const _TCHAR	kKeisaiName[];			//!< タウンページ掲載名称
	extern const _TCHAR	kAddrCode[];			//!< 11桁住所コード
	extern const _TCHAR	kAddrName[];			//!< 住所名
	extern const _TCHAR kPointStat[];			//!< 調査結果コード
	extern const _TCHAR kSequence[];			//!< シーケンス番号
	extern const _TCHAR kAddXY[];				//!< 座標付与種別

	namespace point_stat {
		enum ECode {
			kNone					= 0,	//!< 未調査
			kUnknownPos				= 1,	//!< 位置不明
			kInstitutionOK			= 2,	//!< 該当施設あり
			kReserved				= 3,	//!< 保留
		};
	} // point_stat

	namespace add_xy {
		enum ECode {
			kNormal					= 0,	//!< 通常
			kClaim					= 1,	//!< クレーム
			kSameAddr				= 2,	//!< 同一住所
			kNormalRPoint			= 3,	//!< 品証誘導ポイント(一般道路)
			kHighwayRPoint			= 4,	//!< 品証誘導ポイント(高速道路)
			kTWNPinPoint			= 5,	//!< TWNピンポイント落ち回避(加除移行)
			kHLPPinPoint			= 6,	//!< HLPピンポイント落ち回避(加除移行)
		};
	} // add_xy

} // teltp_point

// 都市建物屋上施設
namespace building_roof {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kRfClass[];			//!< 屋上施設種別

	namespace rf_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kOtherSite				=        1, //!< 舗装地その他敷地（屋上施設）
			kPlanting				=		 2,	//!< 植栽地（屋上施設）
			kPool					=		 3, //!< プール（屋上施設）
		};
	} // rf_class
} // building_roof

// 都市エリア
namespace city_area {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kAreaClass[];			//!< エリア種別

	namespace area_class {
		enum ECode {
			kNone					=        0, //!< 未調査・作業中
			kPark					=        1, //!< 公園
		};
	} // area_class
} // city_area

// 都市トンネル坑口
namespace city_tunnel {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kTunnelClass[];			//!< トンネル種別
} // city_tunnel

// 都市道路ポリゴン
namespace city_road {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kRoadClass[];			//!< 道路ポリゴン種別
	
	namespace road_class {
		enum ECode {
			kNone						=        0, //!< 未調査・作業中
			kInterCityTollExpressway	=		 1,	//!< 都市間高速道（有料）
			kInterCityFreeExpressway	=		 2,	//!< 都市間高速道（無料）
			kUrbanTollExpressway		=		 3,	//!< 都市高速（有料）
			kUrbanFreeExpressway		=		 4,	//!< 都市高速（無料）
			kNationalHighway			=        5, //!< 国道
			kPrincipalPrefecturalRoad 	=        6, //!< 主要地方道
			kPrefectureNormalRoad		=		 7,	//!< 一般都道府県道
			kOtherArterialRoad			=		 8,	//!< その他幹線道路
			kOtherRoad					=		 9,	//!< その他道
			kTollRoad					=		10,	//!< 有料道路
		};
	} // road_class
} // city_road

// 都市料金所分離帯ポリゴン
namespace city_divider {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];           //!< テーブル名
	extern const _TCHAR kDividerClass[];        //!< 料金所分離帯種別

} // city_divider

// 高架下背景ライン
namespace city_line_under {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kBgClass[];				//!< 道路ポリゴン種別

} // city_line_under

// 高架下背景ポリゴン
namespace city_site_under {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< テーブル名
	extern const _TCHAR kBgClass[];				//!< 道路ポリゴン種別

} // city_site_under

// 地下通路・地下街ポリゴン
namespace underground_area
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];   //!< テーブル名

} // underground_area

// 郵便番号ポリゴン
namespace zipcode_area
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[]; //!< テーブル名
	extern const _TCHAR kZipCode1[]; //!< 郵便番号1
	extern const _TCHAR kZipCode2[]; //!< 郵便番号2
}

// 郵便番号ポリゴン住所紐付けテーブル
namespace rel_addr
{
	extern const _TCHAR kTableName20[]; //!< 20桁コード版テーブル名
	extern const _TCHAR kTableName23[]; //!< 23桁コード版テーブル名
	extern const _TCHAR kZipCode[]; //!< 郵便番号
	extern const _TCHAR kAddrCode[]; //!< 住所コード
	extern const _TCHAR kModifyStatus[]; //!< 整備状態

	namespace ModifyStatusCode {
		enum ECode {
			None = 0, //!< 反映前
			Used11digit = 1, //!< 使用中（11桁以降）
			UnUsed = 2, //!< 未使用
			Obsolete = 3, //!< 廃止
			Used5digit = 4, //!< 使用中（5桁）
		};
	}
}


} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_MAP_H_
