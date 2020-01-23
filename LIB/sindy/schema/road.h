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

#ifndef SINDY_SCHEMA_ROAD_H_
#define SINDY_SCHEMA_ROAD_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

extern const _TCHAR kObjectID[];		///< オブジェクトIDカラム名

/// 道路リンクフィーチャクラス
namespace road_link {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::time_regulation_relational_table;
	using namespace ::sindy::schema::category::parking_table;
	using namespace ::sindy::schema::category::highway_road_relational_table;
	using namespace ::sindy::schema::category::section_code_relational_table; ///< [bug 12036]
	using namespace ::sindy::schema::category::pref_road_relational_table;    ///< [bug 12036]

	extern const _TCHAR kTableName[];

	extern const _TCHAR kNaviClass[]; ///< 道路経路用種別
	extern const _TCHAR kMainLinkClass[]; ///< リンク種別（主）
	extern const _TCHAR kSubLinkClass[]; ///< リンク種別（副）
	extern const _TCHAR kAdminCode[]; ///< 行政区域コード
	extern const _TCHAR kRoadManager[]; ///< 管理者コード
	extern const _TCHAR kRoadWidth[]; ///< 道路幅員区分
	extern const _TCHAR kBypass[]; ///< バイパスフラグ
	extern const _TCHAR kCarOnly[]; ///< 自動車専用道路フラグ
	extern const _TCHAR kWideArea[]; ///< 広域存在フラグ
	extern const _TCHAR kIsland[]; ///< 離島フラグ
	extern const _TCHAR kUpDownClass[]; ///< 上下線種別コード
	extern const _TCHAR kKubun[]; ///< 主従区分コード
	extern const _TCHAR kTripSpeed[]; ///< 旅行速度
	extern const _TCHAR kLimitSpeed[]; ///< 規制速度コード
	extern const _TCHAR kLaneCount[]; ///< 車線数
	extern const _TCHAR kLaneValid[]; ///< 片側車線数有効フラグ
	extern const _TCHAR kRightChange[]; ///< 車線数右増減
	extern const _TCHAR kLeftChange[]; ///< 車線数左増減
	extern const _TCHAR kAlleyMainte[]; ///< 超細街路作業年度情報ID
	extern const _TCHAR kDupliLinkRF[]; ///< 重用路線情報関係フラグ
	extern const _TCHAR kTurnRegLQRF[]; ///< 右左折禁止リンク列関係フラグ
	extern const _TCHAR kDirGuideLQRF[]; ///< 方面案内リンク列関係フラグ
	extern const _TCHAR kGuideLQRF[]; ///< 強制誘導リンク列関係フラグ
	extern const _TCHAR kLaneLQRF[]; ///< レーン情報リンク列関係フラグ
	extern const _TCHAR kVICSLQRF[]; ///< VICSリンク列関係フラグ
	extern const _TCHAR kBywayLQRF[]; ///< 抜け道情報リンク列関係フラグ
	extern const _TCHAR kRouteLQRF[]; ///< ルート情報リンク列関係フラグ
	extern const _TCHAR kUturnLQRF[]; ///< Uターン可能箇所情報リンク列関係フラグ
	extern const _TCHAR kInternaviLQRF[]; ///< インターナビVICSリンク列関係フラグ
	extern const _TCHAR kIpcVicsLQRF[]; ///< iPC-VICSリンク列関係フラグ
	extern const _TCHAR kIntersectionLQRF[]; ///< 進入方向別交差点リンク列関係フラグ
	extern const _TCHAR kVehicleRegRF[];     //!< 車種別規制関連フラグ
	extern const _TCHAR kVehicleTurnRegRF[]; //!< 車種別右左折禁止リンク列関連フラグ
	extern const _TCHAR kWalkable[]; //!< 歩行可能種別コード
	extern const _TCHAR kPedestrianZone[]; //!< 歩行者天国フラグ
	extern const _TCHAR kWheelChair[]; //!< 車椅子通行コード

	extern const _TCHAR kParkingLinkClass[]; ///< 駐車場リンク種別
	extern const _TCHAR kFloorLevel[]; ///< 駐車場階層レベル
	extern const _TCHAR kMiddleFloorLevel[]; ///< 駐車場中階層レベル

	extern const _TCHAR kLegalSpeed[]; ///< 法定速度コード(本流には未適用 2010.2.25)

	/// 道路経路用種別コード
	namespace navi_class {
		enum ECode {
			kNoNavi               =   0, ///< 経路外道路
			kCountry              =   1, ///< 一般国道
			kPrefectureMain       =   2, ///< 主要地方道（都府県）
			kCityMain             =   3, ///< 主要地方道（指定市）
			kPrefectureNormal     =   4, ///< 一般都道府県道
			kCityNormal           =   5, ///< 指定市の一般市道
			kOther                =   6, ///< その他道路
			kHighway              = 101, ///< 高速自動車道路
			kCityHighway          = 102, ///< 都市高速道路
			kTollCountry          = 103, ///< 一般国道（有料）
			kTollPrefectureMain   = 104, ///< 主要地方道（都府県）（有料）
			kTollCityMain         = 105, ///< 主要地方道（指定市）（有料）
			kTollPrefectureNormal = 106, ///< 一般都道府県道（有料）
			kTollCityNormal       = 107, ///< 指定市の一般市道（有料）
			kTollOther            = 108, ///< その他道路（有料）
			kFerry                = 201, ///< フェリー航路
			kLinkCountry          = 403, ///< 接続国道
		};
	}

	/// 道路幅員区分コード
	namespace road_width {
		enum ECode {
			kUnsurveyed      = 0, ///< 未調査
			kOver130         = 1, ///< 幅員13.0m以上
			kBetween55And130 = 2, ///< 幅員5.5m以上13.0m未満
			kBetween30And55  = 3, ///< 幅員3.0m以上5.5m未満
			kUnder30         = 4, ///< 幅員3.0m未満
		};
	}

	/// リンク種別コード
	namespace link_class {
		enum ECode {
			kUnsurveyed  = 0,   //!< 未調査・作業中
			kMain        = 1,   //!< 本線（上下線非分離）リンク
			kMainUpDown  = 2,   //!< 本線（上下分離）リンク
			kConnect     = 3,   //!< 連結路（本線間の渡り線）
			kCross       = 4,   //!< 交差点内リンク
			kRamp        = 5,   //!< 連結路（ランプ）リンク
			kByway       = 6,   //!< 本線と同一路線の側道リンク
			kSA          = 7,   //!< ＳＡなど側線リンク
			kBicycle     = 8,   //!< 自転車道等
			kBywayToMain = 9,   //!< 本線側道接続リンク
			kRoundAbout  = 101, //!< ラウンドアバウト
		};
	}

	/// 主リンク種別コード
	namespace main_link_class {
		using namespace ::sindy::schema::road_link::link_class;
	}

	/// 副リンク種別コード
	namespace sub_link_class {
		using namespace ::sindy::schema::road_link::link_class;
	}

	/// 管理者コード
	namespace road_manager {
		enum ECode {
			kUnsurveyed = 0,
			kJH         = 1, ///< 日本道路公団
			kPublic3    = 2, ///< 道路３公団
			kPublic     = 3, ///< 道路公社
			kCountry    = 4, ///< 国
			kPrefecture = 5, ///< 都道府県
			kCity       = 6, ///< 指定市
			kOtherTown  = 7, ///< 他の市町村等（含東京23区）
			kOther      = 8, ///< その他の管理者
		};
	}

	/// 上下線種別コード（ハイウェイ路線コード）
	namespace up_down_class {
		enum ECode {
			kNoHighway   = 0, ///< 対象外
			kUndirected  = 1, ///< HWY(上下なし)
			kUpInWest    = 2, ///< HWY上り(内・西)
			kDownOutEast = 3, ///< HWY下り(外・東)
			kCommon      = 4, ///< HWY上下線共通
			kEtcRamp	 = 5, ///< ETC専用ランプリンク
			kEtcAccess	 = 6, ///< ETC接続一般道リンク
		};
	}

	/// 通行禁止種別コード
	namespace no_passage {
		enum ECode {
			kPassable          = 0, ///< 通行禁止なし
			kNoPassage         = 1, ///< 条件なし通行禁止
			kNoPassageWithReg  = 2, ///< 条件付き通行禁止
			kInpassable        = 3, ///< 通行不可
			kRestricted        = 4, ///< 条件なし通行禁止(関係者以外)
			kRestrictedWithReg = 5, ///< 条件付き通行禁止(関係者以外)
		};
	}

	/// 一方通行種別コード
	namespace oneway {
		enum ECode {
			kNoOneway           = 0, ///< 一方通行なし
			kFore               = 1, ///< 条件なし一方通行(正方向)
			kReverse            = 2, ///< 条件なし一方通行(逆方向)
			kForeWithReg        = 3, ///< 条件付き一方通行(正方向)
			kReverseWithReg     = 4, ///< 条件付き一方通行(逆方向)
			kForeReverseWithReg = 5, ///< 条件付き一方通行(正逆)
		};
	}

	/// 主従道路区分コード
	namespace kubun {
		enum ECode {
			kUnsurveyed = 0, ///< 未調査
			kMain       = 1, ///< 主道路
			kSub        = 2, ///< 従道路
		};
	}

	/// 駐車場リンク種別コード
	namespace parking_link_class {
		enum ECode {
			kUnsurveyed   = 0, ///< 未調査
			kEntrance     = 1, ///< 入口リンク
			kExit         = 2, ///< 出口リンク
			kExitEntrance = 3, ///< 出入口リンク
			kParkingRoad  = 4, ///< 構内道路リンク
			kLevelRamp    = 5, ///< レベル間の渡りリンク
		};
	}

	/// DRMA規制速度コード
	namespace limit_speed {
		enum ECode {
			kUnInvestigated = 0, ///< 未調査
			kLess30         = 1, ///< ３０km/h未満
			k40             = 2, ///< ４０km/h
			k50             = 3, ///< ５０km/h
			k60             = 4, ///< ６０km/h
			k70             = 5, ///< ７０km/h
			k80             = 6, ///< ８０km/h
			k100            = 7, ///< １００km/h
		};
	}

	/// 細街路作業年度コード
	namespace alley_mainte {
		enum ECode {
			kNoData     =  0, ///< 該当なし
			k1998       =  1, ///< 1998年度
			k1999       =  2, ///< 1999年度
			k2000Spring =  3, ///< 2000年度（春）
			k2000Autumn =  4, ///< 2000年度（秋）
			k2001       =  5, ///< 2001年度
			k2002       =  6, ///< 2002年度
			k2003       =  7, ///< 2003年度
			k2004       =  8, ///< 2004年度
			k2005       =  9, ///< 2005年度
			k2006       = 10, ///< 2006年度
		};
	}

	namespace updown_class {
		enum ECode
		{
			kNonHwy		= 0,	///< 対象外
			kNoDir		= 1,	///< HWY（上下なし）
			kUp			= 2,	///< HWY上り（内・西）
			kDown		= 3,	///< HWY下り（外・東）
			kUpDown		= 4,	///< HWY上下線共通
			kEtcRamp	= 5,	///< ETC専用ランプリンク
			kEtcAccess	= 6,	///< ETC接続一般道リンク
		};
	}

	namespace walkable_class {
		enum ECode
		{
			kNotWalk  = 0,  ///< 歩道なし（歩行禁止）
			kWalkable = 1,  ///< 歩道なし（人車共用）
			kWalkOnly = 2,  ///< 歩行者専用
		};
	} // walkable_class

	/**
	 * @brief 最高速度コード
	 * @note	[Bug9294]対応時に[Bug9293]版（[Bug8924]を含む）のスキーマに合わせて変更。修正前は[Bug7486]の暫定スキーマの通りになっていた。
	 *			修正前のこれを使ったコードをビルドしようとした時にビルドできてしまうとバグの元になりそうなので、
	 *			ビルドが失敗してここの変化に気付けるように、各コード値に対応する列挙型要素名を全て変えておく（例：「k40」→「k40km」）。
	 */
	namespace legal_speed {
		enum ECode
		{
			kUnInvestigated = 0,	///< 未調査
			k30kmOrLess		= 1,	///< 30km/h以下
			k20km			= 2,	///< 20km/h
			k30km			= 3,	///< 30km/h
			k40km			= 4,	///< 40km/h
			k50km			= 5,	///< 50km/h
			k60km			= 6,	///< 60km/h
			k70km			= 7,	///< 70km/h
			k80km			= 8,	///< 80km/h
			k90km			= 9,	///< 90km/h
			k100km			= 10,	///< 100km/h
			k110km			= 11,	///< 110km/h [bug12460]
			k120km			= 12,	///< 120km/h [bug12460]
			kZone30km		= 30,	///< ゾーン30
			kNone			= 99,	///< なし
		};
	}
}

/// 道路ノードテーブル
namespace road_node {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kNodeClass[];///< ノード種別
	extern const _TCHAR kSignal[]; ///< 信号機コード
	extern const _TCHAR kNameKanji[]; ///< 交差点漢字名称
	extern const _TCHAR kNameYomi[]; ///< 交差点カナ名称
	extern const _TCHAR kHeightCode[]; ///< 高さコード 
	extern const _TCHAR kHeight[]; ///< 高さ情報
	extern const _TCHAR kParkingID[]; ///< 駐車場ＩＤ

	/// ノード種別コード
	namespace node_class {
		enum ECode {
			kNoFunction =  0, ///< 機能なしノード
			kCross      =  1, ///< 交差点ノード
			kTerminal   =  2, ///< 端点ノード
			kAlter      =  3, ///< 属性変化点ノード
			kAttr       =  4, ///< 属性付きノード
			kParking    =  5, ///< 駐車場リンク接続ノード
			kLQBase     =  6, ///< ノード列基点ノード
			kTollbooth  =  7, ///< 料金所ノード
			kHighway    =  8, ///< ハイウェイ関係ノード
			kEdge       =  9, ///< ２次メッシュの区切り上のノード
			kEvasion    = 10, ///< 同一リンクID防止ノード
			kPec        = 11, ///< PECノード
			kPecEdge    = 12, ///< ２次メッシュ区画辺上PECノード
			kDummyFacil = 13, ///< ダミー施設ノード
		};
	}

	/// 信号機コード
	namespace signal {
		enum ECode {
			kSignal     = 1, ///< 信号機あり
			kNoSignal   = 0, ///< 信号機なし
			kUnsurveyed = 2, ///< 未調査
		};
	}

	/// 高さコード
	namespace height_code {
		enum ECode {
			kInvalid = 0, ///< 相対高さ無し
			kValid   = 1, ///< 相対高さ有効
			kAuto    = 2, ///< 高さ自動計算
		};
	}
}

/// 歩道リンクテーブル
namespace walk_link {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::time_regulation_relational_table;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kWalkClass[];			///< 歩行者種別
	extern const _TCHAR kFloorLevel[];			///< 階層レベル
	extern const _TCHAR kFloorMoveClass[];		///< 階層移動種別
	extern const _TCHAR kAutoWalkFlag[];		///< オートウォークフラグ
	extern const _TCHAR kStairFlag[];			///< 階段フラグ
	extern const _TCHAR kSlopeFlag[];			///< スロープフラグ
	extern const _TCHAR kEscalatorFlag[];		///< エスカレータフラグ
	extern const _TCHAR kArcadeFlag[];			///< 屋根付きフラグ
	extern const _TCHAR kOpenAreaFlag[];		///< 広場（Open Traffic Area）フラグ（bug 11187）
	extern const _TCHAR kTunnelFlag[];			///< トンネルフラグ
	extern const _TCHAR kRoadSurface[];			///< 路面素材コード
	extern const _TCHAR kLinkRelationRF[];		///< リンク紐付テーブル関係フラグ
	extern const _TCHAR kWalkCode[];			///< 歩行者路線コード
	extern const _TCHAR kWheelChair[];			///< 車椅子通行コード
	extern const _TCHAR kPlatformNumber[];		///< のりば番号
	extern const _TCHAR kScrambleFlag[];		///< スクランブルフラグ

	/// 歩行者表示種別コード
	namespace walk_class {
		enum ECode {
			kNone                 = 0,    ///< 設定なし
			kWalkRoad             = 1,    ///< 歩道
			kInferredCrossRoad    = 2,    ///< みなし横断歩道
			kCrossRoad            = 3,    ///< 横断歩道
			kOtherWalkRoad        = 99,   ///< その他歩道
			kWalkableRoad         = 100,  ///< 人車共用リンク
			kConnect              = 101,  ///< 接続リンク
			kWalkBridge           = 200,  ///< 歩道橋
			kBridge               = 201,  ///< 橋
			kRailCrossing         = 202,  ///< 踏切
			kOutdoor              = 300,  ///< 施設敷地内（屋外）
			kIndoor               = 301,  ///< 施設敷地内（屋内）
			kInternalTicketGate   = 302,  ///< 地上改札内
			kPlatForm             = 303,  ///< 地上プラットフォーム
			kUnderGround          = 400,  ///< 地下通路
			kUnderGroundPath      = 401,  ///< 横断地下道（bug 11187）
			kInternalTicketGateUG = 402,  ///< 地下改札内
			kPlatFormUG           = 403,  ///< 地下プラットフォーム
			kWalkFerry            = 500,  ///< 歩行者専用フェリー
			
		};
	} // walk_class

	/// 階層移動種別コード
	namespace floor_move_class {
		enum ECode {
			kNone      = 0,  ///< 上下無し
			kForeUp    = 1,  ///< 上り(正方向)
			kReverseUp = 2,  ///< 上り(逆方向)
		};
	} // floor_move_class

	/// 一方通行種別コード
	namespace oneway {
		using namespace ::sindy::schema::road_link::oneway;
	} // oneway

	/// 通行禁止種別コード
	namespace nopassage {
		enum ECode {
			kPassable          = 0, ///< 通行禁止なし
			kNoPassageWithReg  = 2, ///< 条件付き通行禁止
		};
	} // nopassage

	/// 路面素材コード
	namespace road_surface_class {
		enum ECode {
			kPaved   = 1,  ///< 舗装
			kUnpaved = 99, ///< 未舗装
		};
	} // road_surface_class

	/// 車椅子通行コード
	namespace wheel_chair {
		enum ECode {
			kUnInvestigated       = 0,    ///< 未調査
			kPassable             = 1,    ///< 通行可能
			kNoPassable           = 2,    ///< 通行不可能
		};
	} // walk_class
} // walk_link

/// 歩道ノードテーブル
namespace walk_node {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];              ///< テーブル名

	extern const _TCHAR kNodeClass[];              ///< 歩行者ノード種別
	extern const _TCHAR kRoadNodeID[];             ///< 道路ノード対応ID
	extern const _TCHAR kStationGatePointID[];     ///< 駅出入口ポイントID
	extern const _TCHAR kUndergroundGatePointID[]; ///< 地下街出入口ポイントID

	/// 歩行者ノード種別
	namespace node_class {
		enum ECode {
			kPublic           = 0,  ///< 一般ノード
			kTerminal         = 1,  ///< 端点ノード
			kAlter            = 2,  ///< 属性遷移点ノード
			kRoadNW           = 3,  ///< 道路ノード接続ノード
			kGate             = 4,  ///< 駅出入口ノード
			kUndergroundGate  = 5,  ///< 地下街出入口ノード
			kElevator         = 6,  ///< エレベータノード
			kKaisatsu         = 7,  ///< 改札ノード
			kCarPratform      = 8,  ///< 一般車乗降場ノード
			kBusPratform      = 9,  ///< バス乗降場ノード
			kTaxiPratform     = 10, ///< タクシー乗降場ノード
			kParking          = 11, ///< 駐車場出入口ノード
			kDepartureGate    = 12, ///< 空港出発ゲートノード
			kArrivalGate      = 13, ///< 空港到着ゲートノード
			kTicketGate       = 14, ///< 駅改札ノード
			kOutline          = 99, ///< 図郭上ノード
		};
	}
} // walk_node

/// 曜日指定コードドメイン（walk_nopassage, walk_onewayでも使用するので、inf_turnregから分離）
namespace day_code {
	enum ECode {
		kHoliday                =  1, ///< 日・休日
		kExceptHolidayAndSunday =  2, ///< 日・休日をのぞく
		kHolidayExceptSunday    =  3, ///< 休日(日曜はのぞく)
		kExceptHoliday          =  4, ///< 休日をのぞく
		kOrdinaryDays           =  5, ///< 平日
		kSunday                 =  6, ///< 日曜
		kMonday                 =  7, ///< 月
		kTuesday                =  8, ///< 火
		kWednesday              =  9, ///< 水
		kThursday               = 10, ///< 木
		kFriday                 = 11, ///< 金
		kSaturday               = 12, ///< 土
		kWeekday                = 13, ///< 土日・休日
		kExceptWeekday          = 14, ///< 土日・休日をのぞく
		kAlldays                = 15, ///< 全曜日
		kExceptSunday           = 16, ///< 日曜日をのぞく
		kBeforeHoliday          = 17, ///< 日・休日の前日
	};
}


/// 条件付き通行禁止テーブル
namespace time_nopassage {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kSeasonReg[]; ///< 季節規制フラグ

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 条件付き一方通行テーブルフィールド
namespace time_oneway {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::directional_link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/**
 * @defgroup sindy_schema_road_link_segment_sharing_feature_classes
 * @brief 道路リンク線分共有フィーチャクラス
 */
//@{
/// リンク内属性フィーチャクラス
namespace segment_attr {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kUnderOver[]; ///< 上下関係
	extern const _TCHAR kLinkAttr[]; ///< リンク内属性種別コード
	extern const _TCHAR kName[]; ///< 名称

	/// リンク内属性コード
	namespace link_attr {
		enum ECode {
			kBridge            = 1, ///< 橋・高架
			kTunnel            = 2, ///< トンネル
			kDoumon            = 3, ///< 洞門
			kRailCross         = 4, ///< 踏切
			kUnderPass         = 5, ///< アンダーパス
			kUnderOver         = 6, ///< 上下関係
			kDirtRoad          = 7, ///< 未舗装道路
			kFlooding          = 8, ///< 冠水注意地点
			kTunnelSquare      = 100, ///< トンネル（四角）
			kTunnelCircle      = 101, ///< トンネル（丸）
			kRailCrossKamaboko = 102, ///< 踏切（かまぼこ型）
		};
	}
}

/// 傾斜フィーチャクラス
namespace grad {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::link_relational_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kGradValue[]; ///< 傾斜値

}

/// 簡易傾斜フィーチャクラス
namespace simple_grad {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::link_relational_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kNodeID[]; ///< 分岐ノードID
	extern const _TCHAR kGrad[]; ///< 簡易傾斜コード
	extern const _TCHAR kLinkType[]; ///< リンクタイプコード

	/// 簡易傾斜コード
	namespace grad {
		enum ECode {
			kUnknown   = 0, ///< UNKNOWN
			kFlat      = 1, ///< FLAT
			kUp        = 2, ///< UP
			kDown      = 3, ///< Down
		};
	}
	/// リンクタイプコード
	namespace link_type {
		enum ECode {
			kUnknown   = 0, ///< 未定義
			kFromLink  = 1, ///< Fromリンク
			kToLink    = 2, ///< Toリンク
		};
	}
}

//@}

/// 駐車場外形フィーチャクラス
namespace parking_shape {
	using namespace ::sindy::schema::category::parking_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kParkingOrgID[]; ///< 駐車場ID
	extern const _TCHAR kNameKanji[]; ///< 駐車場名称
}

/// 歩行者ナビエリアポリゴン
namespace edit_area {
	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kAreaName[];	///< 整備エリア
	extern const _TCHAR kCreateYear[];	///< 作成年度
	extern const _TCHAR kPriority[];	///< 優先度
}

/// 歩行者用条件付き通行禁止テーブル
namespace walk_nopassage {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // walk_nopassage

/// 歩行者用条件付き一方通行テーブルフィールド
namespace walk_oneway {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::directional_link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
} // walk_oneway

/// 歩行者タイプテーブル
namespace walk_type{
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];  ///< テーブル名

	extern const _TCHAR kWalkTypeClass[];  ///< 歩行者種別コード

	namespace type_class {
		enum ECode {
			kGeneral     = 1, ///< 歩道あり（汎用版）
			kDownConvert = 2, ///< 歩道なし
		};
	} // type_class
} // walk_type

/**
 * @defgroup sindy_schema_inf
 * @brief リンク列Infテーブルグループ
 */
//@{
/// 右左折禁止リンク列Infテーブル
namespace inf_turnreg {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
	extern const int    kMaxNumber; ///< 最大右左折禁止数

	extern const _TCHAR kStartMonth0[]; ///< 規制条件:開始月
	extern const _TCHAR kStartDay0[]; ///< 規制条件:開始日
	extern const _TCHAR kStartHour0[]; ///< 規制条件:開始時間
	extern const _TCHAR kStartMin0[]; ///< 規制条件:開始分
	extern const _TCHAR kEndMonth0[]; ///< 規制条件;終了月
	extern const _TCHAR kEndDay0[]; ///< 規制条件:終了日
	extern const _TCHAR kEndHour0[]; ///< 規制条件:終了時間
	extern const _TCHAR kEndMin0[]; ///< 規制条件:終了分
	extern const _TCHAR kDayOfWeek0[]; ///< 規制条件:曜日指定

	extern const _TCHAR kStartMonth1[]; ///< 規制条件1:開始月
	extern const _TCHAR kStartDay1[]; ///< 規制条件1:開始日
	extern const _TCHAR kStartHour1[]; ///< 規制条件1:開始時間
	extern const _TCHAR kStartMin1[]; ///< 規制条件1:開始分
	extern const _TCHAR kEndMonth1[]; ///< 規制条件1;終了月
	extern const _TCHAR kEndDay1[]; ///< 規制条件1:終了日
	extern const _TCHAR kEndHour1[]; ///< 規制条件1:終了時間
	extern const _TCHAR kEndMin1[]; ///< 規制条件1:終了分
	extern const _TCHAR kDayOfWeek1[]; ///< 規制条件1:曜日指定

	extern const _TCHAR kStartMonth2[]; ///< 規制条件2:開始月
	extern const _TCHAR kStartDay2[]; ///< 規制条件2:開始日
	extern const _TCHAR kStartHour2[]; ///< 規制条件2:開始時間
	extern const _TCHAR kStartMin2[]; ///< 規制条件2:開始分
	extern const _TCHAR kEndMonth2[]; ///< 規制条件2;終了月
	extern const _TCHAR kEndDay2[]; ///< 規制条件2:終了日
	extern const _TCHAR kEndHour2[]; ///< 規制条件2:終了時間
	extern const _TCHAR kEndMin2[]; ///< 規制条件2:終了分
	extern const _TCHAR kDayOfWeek2[]; ///< 規制条件2:曜日指定

	extern const _TCHAR kStartMonth3[]; ///< 規制条件3:開始月
	extern const _TCHAR kStartDay3[]; ///< 規制条件3:開始日
	extern const _TCHAR kStartHour3[]; ///< 規制条件3:開始時間
	extern const _TCHAR kStartMin3[]; ///< 規制条件3:開始分
	extern const _TCHAR kEndMonth3[]; ///< 規制条件3;終了月
	extern const _TCHAR kEndDay3[]; ///< 規制条件3:終了日
	extern const _TCHAR kEndHour3[]; ///< 規制条件3:終了時間
	extern const _TCHAR kEndMin3[]; ///< 規制条件3:終了分
	extern const _TCHAR kDayOfWeek3[]; ///< 規制条件3:曜日指定

	extern const _TCHAR kStartMonth4[]; ///< 規制条件4:開始月
	extern const _TCHAR kStartDay4[]; ///< 規制条件4:開始日
	extern const _TCHAR kStartHour4[]; ///< 規制条件4:開始時間
	extern const _TCHAR kStartMin4[]; ///< 規制条件4:開始分
	extern const _TCHAR kEndMonth4[]; ///< 規制条件4;終了月
	extern const _TCHAR kEndDay4[]; ///< 規制条件4:終了日
	extern const _TCHAR kEndHour4[]; ///< 規制条件4:終了時間
	extern const _TCHAR kEndMin4[]; ///< 規制条件4:終了分
	extern const _TCHAR kDayOfWeek4[]; ///< 規制条件4:曜日指定

	extern const LPCTSTR kStartMonth[5]; ///< 規制条件:開始月一覧
	extern const LPCTSTR kStartDay[5]; ///< 規制条件:開始日一覧
	extern const LPCTSTR kStartHour[5]; ///< 規制条件:開始時間一覧
	extern const LPCTSTR kStartMin[5]; ///< 規制条件:開始分一覧
	extern const LPCTSTR kEndMonth[5]; ///< 規制条件;終了月一覧
	extern const LPCTSTR kEndDay[5]; ///< 規制条件:終了日一覧
	extern const LPCTSTR kEndHour[5]; ///< 規制条件:終了時間一覧
	extern const LPCTSTR kEndMin[5]; ///< 規制条件:終了分一覧
	extern const LPCTSTR kDayOfWeek[5]; ///< 規制条件:曜日指定一覧

	using namespace ::sindy::schema::day_code;

	struct where_all_year_all_days {};

	template <typename CharT>
	inline std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<CharT>& os, where_all_year_all_days)
	{
		using namespace atl2::io::compatible;
		return os << kStartMonth1 << L" IS NULL";
	}
}

/// 強制誘導リンク列Infテーブル
namespace inf_guide {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
	
	extern const _TCHAR kGuideType[]; ///< 強制誘導案内コード
	extern const _TCHAR kDelete[]; ///< 強制誘導削除フラグ

	/// 強制誘導案内コード
	namespace guide_type {
		enum ECode {
			kUnsurveyed =  0, ///< 未設定
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
	}
}

/// 方面案内リンク列Infテーブル
namespace inf_dirguide {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const int kMaxNumber; ///< 最大方面案内数

	extern const _TCHAR kGuidePattern[]; ///< 方面看板パターン番号
	extern const _TCHAR kSequence[]; ///< 分岐シーケンス番号

	extern const _TCHAR kGuideClass0[]; ///< 方面案内:地先種別
	extern const _TCHAR kNameKanji0[]; ///< 方面案内:漢字名称
	extern const _TCHAR kNameYomi0[]; ///< 方面案内:カナ名称

	extern const _TCHAR kGuideClass1[]; ///< 方面案内1:地先種別
	extern const _TCHAR kNameKanji1[]; ///< 方面案内1:漢字名称
	extern const _TCHAR kNameYomi1[]; ///< 方面案内1:カナ名称

	extern const _TCHAR kGuideClass2[]; ///< 方面案内2:地先種別
	extern const _TCHAR kNameKanji2[]; ///< 方面案内2:漢字名称
	extern const _TCHAR kNameYomi2[]; ///< 方面案内2:カナ名称

	extern const _TCHAR kGuideClass3[]; ///< 方面案内3:地先種別
	extern const _TCHAR kNameKanji3[]; ///< 方面案内3:漢字名称
	extern const _TCHAR kNameYomi3[]; ///< 方面案内3:カナ名称

	extern const _TCHAR kGuideClass4[]; ///< 方面案内4:地先種別
	extern const _TCHAR kNameKanji4[]; ///< 方面案内4:漢字名称
	extern const _TCHAR kNameYomi4[]; ///< 方面案内4:カナ名称

	extern const _TCHAR kGuideClass5[]; ///< 方面案内5:地先種別
	extern const _TCHAR kNameKanji5[]; ///< 方面案内5:漢字名称
	extern const _TCHAR kNameYomi5[]; ///< 方面案内5:カナ名称

	extern const _TCHAR kGuideClass6[]; ///< 方面案内6:地先種別
	extern const _TCHAR kNameKanji6[]; ///< 方面案内6:漢字名称
	extern const _TCHAR kNameYomi6[]; ///< 方面案内6:カナ名称

	extern const _TCHAR kGuideClass7[]; ///< 方面案内7:地先種別
	extern const _TCHAR kNameKanji7[]; ///< 方面案内7:漢字名称
	extern const _TCHAR kNameYomi7[]; ///< 方面案内7:カナ名称

	extern const _TCHAR kGuideClass8[]; ///< 方面案内8:地先種別
	extern const _TCHAR kNameKanji8[]; ///< 方面案内8:漢字名称
	extern const _TCHAR kNameYomi8[]; ///< 方面案内8:カナ名称

	extern const _TCHAR kGuideClass9[]; ///< 方面案内9:地先種別
	extern const _TCHAR kNameKanji9[]; ///< 方面案内9:漢字名称
	extern const _TCHAR kNameYomi9[]; ///< 方面案内9:カナ名称

	extern const _TCHAR kGuideClass10[]; ///< 方面案内10:地先種別
	extern const _TCHAR kNameKanji10[]; ///< 方面案内10:漢字名称
	extern const _TCHAR kNameYomi10[]; ///< 方面案内10:カナ名称

	extern const LPCTSTR kGuideClass[11]; ///< 方面案内:地先種別一覧
	extern const LPCTSTR kNameKanji[11]; ///< 方面案内:漢字名称一覧
	extern const LPCTSTR kNameYomi[11]; ///< 方面案内:カナ名称一覧

	/// 方面看板パターン番号
	namespace guide_pattern {
		enum ECode {
			kNoPattern  =  0, ///< パターン無し
			kOverpass   =  1, ///< 立体Ａ（オーバーパス）
			kUnderpass  =  2, ///< 立体Ｂ（アンダーパス）
			kCrossroads =  3, ///< 十字路
			kTJunctionL =  4, ///< Ｔ字Ａ（右折なし）
			kTJunctionR =  5, ///< Ｔ字Ｂ（左折なし）
			kYJunction  =  6, ///< Ｙ字
			kTJunction  =  7, ///< Ｔ字
			kInitial    = 99, ///< 未設定
		};
	};

	/// 方面案内種別コード2
	namespace guide_class {
		enum ECode {
			kNormal            =  1, ///< 地先（一般）
			kToll              =  2, ///< 地先（有料）
			kCountry           =  3, ///< 国道
			kPrefecture        =  4, ///< 県道
			kOther             =  5, ///< その他
			kTollString        = 12, ///< 有料（文字列）
			kTollNationalWayNo = 13, ///< 有料（国道番号）
			kTollPrefWayNo     = 14, ///< 有料（県道番号）
		};
	}
}

/// レーンリンク列Infテーブル
namespace inf_lane {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kLaneDir[]; ///< レーン方向コード
	extern const _TCHAR kApproachLane[]; ///< 交差点直近車線数
	extern const _TCHAR kRightChange[]; ///< 右側増減車線数
	extern const _TCHAR kLeftChange[]; ///< 左側増減車線数
	extern const _TCHAR kFlowLane[]; ///< 流出車線数
	extern const _TCHAR kLanePassable[]; ///< 車線通行状態

	/// レーン方向コード
	namespace lane_dir {
		enum ECode {
			kUnsurveyed = 0, ///< 未調査・作業中
			k0   = 1, ///< 前方向(0)
			k45  = 2, ///< 右斜め前方向(45)
			k90  = 3, ///< 右方向(90)
			k135 = 4, ///< 右斜め後ろ方向(135)
			k180 = 5, ///< 後ろ方向(180)
			k225 = 6, ///< 左斜め後ろ方向(225)
			k270 = 7, ///< 左方向(270)
			k315 = 8, ///< 左斜め前方向(315)
		};
	}
}

/// VICSリンク列Infテーブル
namespace inf_vics {
	using namespace ::sindy::schema::fieldsurvey_table;
	using namespace ::sindy::schema::category::vics_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// インターナビVICSリンク列Infテーブル
namespace inf_internavi {
	using namespace ::sindy::schema::fieldsurvey_table;
	using namespace ::sindy::schema::category::vics_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// iPC-VICSリンク列Infテーブル
namespace inf_ipcvics {
	using namespace ::sindy::schema::fieldsurvey_table;
	using namespace ::sindy::schema::category::vics_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// iPC-VICSのID管理テーブル(最大値保持)
namespace ipcvics_idtable {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[]; ///< テーブル名
	extern const _TCHAR kMeshCode[];	///< メッシュコード
	extern const _TCHAR kMainNumber[];	///< 優先使用済みID番号
	extern const _TCHAR kSubNumber[];	///< 非優先使用済みID番号
	extern const _TCHAR kSubVicsClass[];	///< 非優先VICS区分
}

/// 進入方向別交差点リンク列Infテーブル
namespace inf_intersection {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kNameKanji[]; ///< 交差点名称
	extern const _TCHAR kNameYomi[]; ///< ヨミ
}

/// 抜け道リンク列Infテーブル
namespace inf_byway {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kByway[]; ///< 抜け道フラグ
	extern const _TCHAR kBackRoad[]; ///< 裏道フラグ
	extern const _TCHAR kSourceName[]; ///< 出典
}


/// ルートリンク列Infテーブル
namespace inf_route {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kRouteClass[]; ///< ルート種別コード
	extern const _TCHAR kSourceName[]; ///< 出典
	extern const _TCHAR kRankMeshCode[]; ///< 走りやすさメッシュ
	extern const _TCHAR kRankLinkID[]; ///< 走りやすさリンク番号
	extern const _TCHAR kDriveMapRank[]; ///< 走りやすさ評価

	/// ルート種別コード
	namespace route_class {
		enum ECode {
			kAccessRoad       = 1,  //!< アクセス道路	
			kConnectRoad      = 2,  //!< 乗り継ぎ道
			kInfomation       = 3,  //!< 観光案内
			kHighAccidentArea = 4,  //!< 事故多発地点
		};
	}

	/// 評価ランク種別コード
	namespace rank_class {
		enum ECode {
			kSRank = 1,
			kARank = 2,
			kBRank = 3,
			kCRank = 4,
			kDRank = 5,
			kMRank = 11,
			kYRAnk = 12,
			kRRAnk = 13,
		};
	}
}

/// Ｕターン可能箇所リンク列Infテーブル
namespace inf_uturn {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kSourceName[]; ///< 出典
}
//@}

/**
 * @defgroup sindy_schema_link_queue
 * @brief リンク列LQテーブルグループ
 */
//@{
/// 右左折禁止リンク列LQテーブル
namespace lq_turnreg {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 強制誘導リンク列LQテーブル
namespace lq_guide {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 方面案内リンク列LQテーブル
namespace lq_dirguide {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// レーンリンク列LQテーブル
namespace lq_lane {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// VICSリンク列LQテーブル
namespace lq_vics {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// インターナビVICSリンク列LQテーブル
namespace lq_internavi {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// iPC-VICSリンク列LQテーブル
namespace lq_ipcvics {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 進入方向別交差点リンク列LQテーブル
namespace lq_intersection {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// 抜け道リンク列LQテーブル
namespace lq_byway {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}

/// ルートリンク列LQテーブル
namespace lq_route {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}
/// Ｕターン可能箇所リンク列LQテーブル
namespace lq_uturn {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< テーブル名
}
//@}

/// 施設情報ポイントフィーチャクラス
namespace facil_info_point {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_facil_relational_table;

	extern const _TCHAR kTableName[];		///< テーブル（レイヤ）名

	extern const _TCHAR kNameKanji[];		///< 施設漢字名称
	extern const _TCHAR kNameYomi[];		///< 施設カナ名称
	extern const _TCHAR kDummyName[];		///< 仮称フラグ
	extern const _TCHAR kFacilClass[];		///< 施設種別コード
	extern const _TCHAR kAutoPosition[];	///< 代表座標自動計算フラグ
	extern const _TCHAR kSearchClass[];		///< 検索用施設種別コード
	extern const _TCHAR kDummyFacil[];		///< ダミー施設フラグ（[Bug9140]で追加）

	/// 施設種別コード定義
	namespace facil_class {
		enum ECode {
			kSA			= 1, ///< ＳＡ
			kPA			= 2, ///< ＰＡ
			kJCT		= 3, ///< ＪＣＴ
			kRamp		= 4, ///< ランプ
			kIC			= 5, ///< ＩＣ
			kTollbooth	= 6, ///< 料金所
			kSmartIC	= 7, ///< スマートＩＣ
		};
	}

	/// 検索用施設種別コード定義
	namespace search_class {
		enum ECode {
			kNotForSearch	= 0, ///< 対象外
			kSA				= 1, ///< ＳＡ
			kPA				= 2, ///< ＰＡ
			kJCT			= 3, ///< ＪＣＴ
			kRamp			= 4, ///< ランプ
			kIC				= 5, ///< ＩＣ
			kTollbooth		= 6, ///< 料金所
			kSmartIC		= 7, ///< スマートＩＣ
		};
	}
}

/// アクセスポイントフィーチャクラス
namespace access_point {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;
	using namespace ::sindy::schema::category::highway_in_out_table;

	extern const _TCHAR kTableName[]; ///< テーブル（レイヤ）名

	extern const _TCHAR kHwyMode[]; ///< ハイウェイモード対応フラグ
	extern const _TCHAR kDirectionKanji[]; ///< 方面漢字名称
	extern const _TCHAR kDirectionYomi[]; ///< 方面カナ名称
	extern const _TCHAR kGS[]; ///< ガソリンスタンド識別コード
	extern const _TCHAR kToilet[]; ///< トイレフラグ
	extern const _TCHAR kHandicap[]; ///< 身障者トイレフラグ
	extern const _TCHAR kRestaurant[]; ///< レストランフラグ
	extern const _TCHAR kCafeteria[]; ///< 軽食フラグ
	extern const _TCHAR kSouvenir[]; ///< 売店フラグ
	extern const _TCHAR kResting[]; ///< 休憩所フラグ
	extern const _TCHAR kNap[]; ///< 仮眠休憩施設フラグ
	extern const _TCHAR kInfoDesk[]; ///< 案内所(対人)フラグ
	extern const _TCHAR kInformation[]; ///< インフォメーションフラグ
	extern const _TCHAR kShower[]; ///< シャワールームフラグ
	extern const _TCHAR kLaundry[]; ///< コインランドリーフラグ
	extern const _TCHAR kSpa[]; ///< お風呂フラグ
	extern const _TCHAR kFax[]; ///< FAXフラグ
	extern const _TCHAR kMailbox[]; ///< 郵便ポストフラグ
	extern const _TCHAR kCashService[]; ///< キャッシュサービスフラグ
	extern const _TCHAR kHwyOasis[]; ///< ハイウェイオアシスフラグ
	extern const _TCHAR kCarWash[]; ///< コイン洗車場フラグ
	extern const _TCHAR kDogRun[];	//!< ドッグランフラグ

	namespace GS {
		enum ECode {
			kNoGS = 0,						//!< GS無し
			kOther = 1,					//!< その他・未定
			kEsso = 2,						//!< エッソ
			kMobil = 3,					//!< モービル
			kKygnus = 4,					//!< キグナス
			kCosmo = 5,					//!< コスモ
			kShell = 6,					//!< シェル
			kJomo = 7,						//!< ジョモ
			kGeneral = 8,					//!< ゼネラル
			kStork = 9,					//!< 九州
			kIdemitsu = 10,					//!< 出光
			kTaiyo = 11,					//!< 太陽
			kEneos = 12,					//!< ENEOS
		};
	}
};

/// ハイウェイノードフィーチャクラス
namespace highway_node {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;
	using namespace ::sindy::schema::category::highway_in_out_table;

	extern const _TCHAR kTableName[]; ///< テーブル（レイヤ）名

	extern const _TCHAR kHwyMode[];			///< ハイウェイモード対応フラグ
	extern const _TCHAR kTollClass[];		///< 料金所種別コード

	// 一般車両（＝ETC非搭載車両）用
	extern const _TCHAR kNoEtcCar[];		///< 一般車両対応フラグ
	extern const _TCHAR kTollFunc[];		///< 料金所機能コード
	extern const _TCHAR kDummyToll[];		///< 非誘導料金所フラグ

	// ETC搭載車両用
	extern const _TCHAR kEtcFacil[];		///< ETC施設コード
	extern const _TCHAR kEtcTollFunc[];		///< 料金所機能コード(ETC)
	extern const _TCHAR kEtcNoGuideToll[];	///< 非誘導料金所フラグ(ETC)
	extern const _TCHAR kTollgateID[];		///< 料金所ID
	
	extern const _TCHAR kGuide[];			///< 案内料金所フラグ
	extern const _TCHAR kRoadSeq2[];		///< tollnode用路線内シーケンス
	extern const _TCHAR kApproachNodeID[];	///< 流入ノードＩＤ
	extern const _TCHAR kFlowNodeID[];		///< 流出ノードＩＤ

	extern const _TCHAR kTollGateLane[];	///< 料金所レーン情報

	extern const _TCHAR kHWYGuideNumber[];  //!< 高速施設案内番号（bug 10205）

	/// 料金所種別コード定義
	namespace toll_class {
		enum ECode {
			kNoTollbooth   = 0, ///< 分岐合流ノード
			kMainTollbooth = 1, ///< 本線上料金所
			kRampTollbooth = 2, ///< ランプ上料金所
			kBarrier       = 3, ///< バリア
			kGateway       = 4, ///< 出入口ノード
		};
	}

	/// 料金所機能コード定義
	namespace toll_func {
		enum ECode {
			kNoFunc             = 0, ///< 機能無し
			kTicket             = 1, ///< 発券機能
			kAdjust             = 2, ///< 精算機能
			kAlone              = 3, ///< 単独料金所
			kAloneTicket        = 4, ///< 単独料金所＋発券機能
			kAloneAdjust        = 5, ///< 単独料金所＋精算機能
			kCheckTicket        = 6, ///< Uターン用検札機能
			kAloneInvalidTicket = 7, ///< 単独料金無効発券機能
			kAdjustTicket		= 8, ///< 精算機能＋発券機能
		};
	}

	/// ETC施設コード定義
	namespace etc_facil {
		enum ECode {
			kNoEtcFacil	= 0,	///< ETC非対応
			kGate		= 1,	///< ゲート
			kAntenna	= 2,	///< アンテナ
		};
	}

	/// 料金所レーン情報
	namespace tollgate_lane {
		enum ECode {
			kUnusable		= 0,	///< 使用不可
			kGeneralLane	= 1,	///< 一般レーン
			kEtcLane		= 2,	///< ETC専用レーン
			kMixedLane		= 3,	///< 混在レーン
			kSwitchingLane	= 4,	///< 切り替えレーン
			kUnknownLane	= 5,	///< 不明レーン
		};
	}
}

/// ハイウェイテキストテーブル
namespace highway_text {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_in_out_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kPointID[]; ///< ハイウェイポイントＩＤ
	extern const _TCHAR kOrderSeq[]; ///< 出現順シーケンス
	extern const _TCHAR kUpDown[]; ///< 方向コード
	extern const _TCHAR kNameKanji[]; ///< ポイント名称
	extern const _TCHAR kPrevID[]; ///< 前の施設へのポイントＩＤ
	extern const _TCHAR kNextID[]; ///< 次の施設へのポイントＩＤ
	extern const _TCHAR kParentID[]; ///< 併設の親施設のポイントＩＤ
	extern const _TCHAR kForeCount[]; ///< 前方分岐数
	extern const _TCHAR kForeID0[];
	extern const _TCHAR kForeID1[]; ///< 前方分岐ＩＤ１
	extern const _TCHAR kForeID2[]; ///< 前方分岐ＩＤ２
	extern const _TCHAR kForeID3[]; ///< 前方分岐ＩＤ３
	extern const _TCHAR kForeID4[]; ///< 前方分岐ＩＤ４
	extern const _TCHAR kForeDir1[]; ///< 前方分岐方向コード１
	extern const _TCHAR kForeDir2[]; ///< 前方分岐方向コード２
	extern const _TCHAR kForeDir3[]; ///< 前方分岐方向コード３
	extern const _TCHAR kForeDir4[]; ///< 前方分岐方向コード４
	extern const _TCHAR kBackCount[]; ///< 後方分岐数
	extern const _TCHAR kBackID0[];
	extern const _TCHAR kBackID1[]; ///< 後方分岐ＩＤ１
	extern const _TCHAR kBackID2[]; ///< 後方分岐ＩＤ２
	extern const _TCHAR kBackID3[]; ///< 後方分岐ＩＤ３
	extern const _TCHAR kBackID4[]; ///< 後方分岐ＩＤ４
	extern const _TCHAR kBackDir1[]; ///< 後方分岐方向コード１
	extern const _TCHAR kBackDir2[]; ///< 後方分岐方向コード２
	extern const _TCHAR kBackDir3[]; ///< 後方分岐方向コード３
	extern const _TCHAR kBackDir4[]; ///< 後方分岐方向コード４

	extern const LPCTSTR kForeID[5]; ///< 前方分岐ＩＤ一覧
	extern const LPCTSTR kBackID[5]; ///< 後方分岐ＩＤ一覧

	/// 方向コード２コード定義
	namespace up_down {
		enum ECode {
			kUp   = 1, ///< 上り・内回り・西行き
			kDown = 2, ///< 下り・外回り・東行き
		};
	}

	/// 分岐方向コード定義
	namespace dircode {
		enum ECode {
			kStraight = 0, //!< 直進方向
			kRight    = 1, //!< 右方向
			kLeft     = 2, //!< 左方向
			kUnknown  = 3, //!< 不定
		};
	}
}

/// 路線コードリストテーブル
namespace road_code_list {
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::highway_road_relational_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kLineClass[]; ///< 路線種別コード
	extern const _TCHAR kLineDirType[]; ///< 路線方向タイプコード
	extern const _TCHAR kNameKanji[]; ///< 路線名称漢字
	extern const _TCHAR kNameYomi[]; ///< 路線名称読み
	extern const _TCHAR kWorkKanji[]; ///< 路線漢字名称(Hwyモード用)
	extern const _TCHAR kWorkYomi[]; ///< 路線カナ名称(Hwyモード用)
	extern const _TCHAR kDisplayKanji[]; ///< 路線名称漢字(表示用)
	extern const _TCHAR kDisplayYomi[]; ///< 路線名称読み(表示用)
	extern const _TCHAR kShieldNoStr[]; ///< 標識番号・記号 [bug12036]

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
	}

	/// 路線方向タイプコード定義
	namespace line_dir_type {
		enum ECode {
			kNoType		= 0,	///< タイプなし
			kUpDown		= 1,	///< 上り・下り
			kInOut		= 2,	///< 内回り・外回り
			kEastWest	= 3,	///< 西行き・東行き
			kOther		= 4,	///< その他
		};
	}
}

/// 都道府県道リスト [bug 12036]
namespace pref_road_list {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::pref_road_relational_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kNameKanji[];	///< 路線漢字名称
	extern const _TCHAR kNameYomi[];	///< 路線ヨミ名称
} // namespace pref_road_list

/// 区間別称コードリスト [bug 12036]
namespace section_code_list {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::section_code_relational_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kNameKanji[];	///< 路線漢字名称
	extern const _TCHAR kNameYomi[];	///< 路線ヨミ名称
} // namespace section_code_list

/// 距離標ポイント
namespace distance_post_point {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_road_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kDistance[];		///< 距離標値
	extern const _TCHAR kDistanceMark[];	///< 距離標記号
}

/// HWY店舗情報テーブル
namespace access_store {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kAccessPointID[];	///< 対応するアクセスポイントのオブジェクトID
	extern const _TCHAR kMultipleStore[];	///< チェーン店マスターコード
	extern const _TCHAR kStartHour[];		///< 営業開始時間
	extern const _TCHAR kStartMin[];		///< 営業開始分
	extern const _TCHAR kEndHour[];			///< 営業終了時間
	extern const _TCHAR kEndMin[];			///< 営業終了分
	extern const _TCHAR kSunday[];			///< 日曜日営業フラグ
	extern const _TCHAR kMonday[];			///< 月曜日営業フラグ
	extern const _TCHAR kTuesday[];			///< 火曜日営業フラグ
	extern const _TCHAR kWednesday[];		///< 水曜日営業フラグ
	extern const _TCHAR kThursday[];		///< 木曜日営業フラグ
	extern const _TCHAR kFriday[];			///< 金曜日営業フラグ
	extern const _TCHAR kSaturday[];		///< 土曜日営業フラグ
	extern const _TCHAR kBon[];				///< お盆営業フラグ
	extern const _TCHAR kYearEnd[];			///< 年末営業フラグ
	extern const _TCHAR kNewYear[];			///< 年始営業フラグ
	extern const _TCHAR kGoldenWeek[];		///< ゴールデンウィーク営業フラグ
	extern const _TCHAR kHoliday[];			///< 祝祭日営業フラグ
	extern const _TCHAR kStoreType[];		///< チェーン店種別コード

	namespace store_type {
		enum ECode {
			kGS			= 0,	///< ガソリンスタンド
			kChainStore	= 1,	///< チェーン店
		};
	}
}

/// iPC料金テーブル
namespace ipc_toll_table {

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kRoadCode1[]; ///< 路線コード
	extern const _TCHAR kRoadSeq1[]; ///< 路線内シーケンス
	extern const _TCHAR kRoadCode2[]; ///< 路線コード
	extern const _TCHAR kRoadSeq2[]; ///< 路線内シーケンス
	extern const _TCHAR kKToll[]; ///< 二輪・軽料金
	extern const _TCHAR kSToll[]; ///< 普通車料金
	extern const _TCHAR kMToll[]; ///< 中型車料金
	extern const _TCHAR kLToll[]; ///< 大型車料金
	extern const _TCHAR kVLToll[]; ///< 特大車料金
	extern const _TCHAR kNormal[]; ///< 一般有料道料金フラグ
}

/// ハイウェイ非対応路線料金テーブル
namespace toll_other_table {
	using namespace ipc_toll_table;
}

/// 単独料金テーブル
namespace toll_alone_table {
	using namespace ::sindy::schema::category::highway_facil_relational_table;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kLineClass[]; ///< 路線種別コード
	extern const _TCHAR kKTollUp[]; ///< 上り:二輪・軽料金
	extern const _TCHAR kSTollUp[]; ///< 上り:普通車料金
	extern const _TCHAR kMTollUp[]; ///< 上り:中型車料金
	extern const _TCHAR kLTollUp[]; ///< 上り:大型車料金
	extern const _TCHAR kVLTollUp[]; ///< 上り:特大車料金
	extern const _TCHAR kKTollDown[]; ///< 下り:二輪・軽料金
	extern const _TCHAR kSTollDown[]; ///< 下り:普通車料金
	extern const _TCHAR kMTollDown[]; ///< 下り:中型車料金
	extern const _TCHAR kLTollDown[]; ///< 下り:大型車料金
	extern const _TCHAR kVLTollDown[]; ///< 下り:特大車料金
	extern const _TCHAR kNormal[]; ///< 一般有料道料金フラグ

	/// 路線種別コード
	namespace line_class {
		/// 路線種別コード一覧
		typedef ::sindy::schema::road_code_list::line_class::ECode ECode;
	}
}

//一時停止ポイント(本流には未適用 2010.2.25)
namespace stop_point {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; ///< テーブル名

	extern const _TCHAR kLinkID[]; ///< リンクID
	extern const _TCHAR kNodeID[]; ///< ノードID
}

/// 車種別規制マスタ
namespace vehicle_reg_master {
	extern const _TCHAR kTableName[];  //!< テーブル（レイヤ）名

	extern const _TCHAR kVehicleRegCode[];    //!< 車種別規制コード
	extern const _TCHAR kVehicleRegName[];    //!< 規制の種類
	extern const _TCHAR kOgataJoyoF[];        //!< 大乗
	extern const _TCHAR kTokuteiChuJoF[];     //!< 特定中乗
	extern const _TCHAR kChugataJoyoF[];      //!< 中乗
	extern const _TCHAR kFutsuJoyoF[];        //!< 普乗
	extern const _TCHAR kFutsuJoyoKeiF[];     //!< 普乗（軽）
	extern const _TCHAR kOgataKamotsuF[];     //!< 大貨
	extern const _TCHAR kTokuteiChuKaF[];     //!< 特定中貨
	extern const _TCHAR kChugataKamotsuF[];   //!< 中貨
	extern const _TCHAR kFutsuKamotsuF[];     //!< 普貨
	extern const _TCHAR kFutsuKamotsuKeiF[];  //!< 普貨（軽）
	extern const _TCHAR kOgataTokushuF[];     //!< 大特
	extern const _TCHAR kKogataTokushuF[];    //!< 小特
	extern const _TCHAR kOgataBikeF[];        //!< 大型二輪
	extern const _TCHAR kFutsuBikeF[];        //!< 普通二輪
	extern const _TCHAR kKogataBikeF[];       //!< 小型二輪
	extern const _TCHAR kTaxiF[];             //!< タクシー
	extern const _TCHAR kBusF[];              //!< 路線バス
	extern const _TCHAR kMiniBikeF[];         //!< 原動機付自転車
	extern const _TCHAR kTrolleyF[];          //!< トロリー
	extern const _TCHAR kBicycleF[];          //!< 自転車
	extern const _TCHAR kExceptBicycleF[];    //!< 自転車以外の軽車両
	extern const _TCHAR kSignageIllustFile[]; //!< 規制画像名
}

/// 車種別規制テーブル
namespace vehicle_reg {
	extern const _TCHAR kTableName[];      //!< テーブル名
	
	extern const _TCHAR kLinkID[];         //!< 道路リンクのOID

	using namespace category::time_condition_table;

	extern const _TCHAR kRegTypeC[];       //!< 規制タイプ
	extern const _TCHAR kHeight[];         //!< 車高
	extern const _TCHAR kWidth[];          //!< 車幅
	extern const _TCHAR kWeight[];         //!< 車重
	extern const _TCHAR kLength[];         //!< 車長
	extern const _TCHAR kLoadage[];        //!< 積載量
	extern const _TCHAR kDangerF[];        //!< 危険物フラグ

	/// 規制タイプコード定義
	namespace reg_type {
		enum ECode {
			kOneway    = 0,            //!< 一方通行（正方向）
			kROneway   = 1,            //!< 一方通行（逆方向）
			kNoPassage = 2,            //!< 通行禁止
		};
	}
	
	/// 曜日コード
	using namespace ::sindy::schema::day_code;
}

/// 車種テーブル
namespace vehicle_type{
	extern const _TCHAR kTableName[];      //!< テーブル名

	extern const _TCHAR kRegID[];           //!< VEHICLE_REGのOID
	extern const _TCHAR kVehicleRegCode[];  //!< 車種別規制コード
	extern const _TCHAR kExcludeF[];        //!< 除外フラグ
}

/// 重なりリンクの高さノード
namespace height_node
{
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];      ///< テーブル名

	extern const _TCHAR kLinkID1[];        ///< リンク1のリンクID
	extern const _TCHAR kLinkID1Height[];  ///< リンク1の相対高さ
	extern const _TCHAR kLinkID1Table[];   ///< リンク1のリンクテーブル
	extern const _TCHAR kLinkID2[];        ///< リンク2のリンクID
	extern const _TCHAR kLinkID2Height[];  ///< リンク2の相対高さ
	extern const _TCHAR kLinkID2Table[];   ///< リンク2のリンクテーブル

	namespace link_table{
		enum ECode {
			kNone     = 0,    ///< 未整備
			kWalkLink = 1,    ///< 歩行者リンク
			kRoadLink = 2,    ///< 道路リンク
		};
	}

} // namespace height_node

/// 歩行者・道路リンク紐づけテーブル
namespace link_relation
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];       ///< テーブル名

	extern const _TCHAR kBaseLinkID[];      ///< 基準リンクID
	extern const _TCHAR kRelatedLinkID[];   ///< 関連リンクID
	extern const _TCHAR kLinkSide[];   ///< リンク相対位置

	namespace link_side_class{
		enum ECode{
			kRight    = 1,   ///< 右
			kLeft     = 2,   ///< 左
		};
	}

} // namespace link_relation

/// 歩行者路線コードリスト
namespace walk_code_list {
	extern const _TCHAR kTableName[];   ///< テーブル名

	extern const _TCHAR kOperator[];    ///< 最終更新者
	extern const _TCHAR kModifyDate[];  ///< 最終更新日時
	extern const _TCHAR kWalkCode[];    ///< 歩行者路線コード
	extern const _TCHAR kNameKanji[];   ///< 歩行者路線漢字
	extern const _TCHAR kNameYomi[];    ///< 歩行者路線読み

} // namespace walk_code_list

/// 警戒標識
namespace warning_sign {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kLinkID[];      //!< リンクID
	extern const _TCHAR kWarningSign[]; //!< 警戒標識種別
	extern const _TCHAR kPicturePath[]; //!< 走行画像ファイルパス

	/// 警戒標識種別コード
	namespace warning_sign_class {
		enum ECode {
			kNone                =   0, //!< 未設定
			kFallingLock         =   1, //!< 落石の恐れあり
			kSlippy              =   2  //!< すべりやすい
		};
	}
} // warning_sign

	
  /// ETC料金所
namespace etc_tollgate {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kTollgateID[];			//!< 料金所ID
	extern const _TCHAR kTollgateParentID[];	//!< 料金所重複管理ID
	extern const _TCHAR kTollgateOfficeID[];	//!< 事業所ID
	extern const _TCHAR kTollgateOfficeName[];	//!< 事業所名
	extern const _TCHAR kTollgateRoadNumber[];	//!< 路線番号
	extern const _TCHAR kTollgateRoadName[];	//!< 路線名
	extern const _TCHAR kTollgateICNumber[];	//!< IC番号
	extern const _TCHAR kTollgateICName[];		//!< IC名
	extern const _TCHAR kTollgateICNameYomi[];	//!< IC名ヨミ
	extern const _TCHAR kTollgateFlag[];		//!< 料金所フラグ
	extern const _TCHAR kTollgateOther[];		//!< その他

} // etc_tollgate


} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_ROAD_H_
