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

// DRMACodeCommunity.h: DRMACodeCommunity クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

/**
 * @file DRMACodeCommunity.h
 * @brief DRMAコード定義ヘッダファイル
 */
#if !defined(AFX_DRMACODECOMMUNITY_H__0157285E_1EEA_488E_A218_EB21BB39E5AC__INCLUDED_)
#define AFX_DRMACODECOMMUNITY_H__0157285E_1EEA_488E_A218_EB21BB39E5AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// DRMAコード名前空間
namespace drma {
	/// DRMAコード一覧
	namespace codes {
		/// レコードIDコード
		namespace a {
			/// レコードIDコード一覧
			enum ECode {
				kAdmin1Id              = 11, ///< 管理データ（その1）
				kAdmin2Id              = 12, ///< 管理データ（その2）
				kBaseRoadNodeId        = 21, ///< 基本道路ノードデータ
				kBaseRoadLinkId        = 22, ///< 基本道路リンクデータ
				kBaseRoadLinkAttrId    = 23, ///< 基本道路リンク内属性データ
				kRoadLinkRelationId    = 24, ///< 基本道路リンク・全道路リンク対応データ
				kBeaconLocationId      = 29, ///< ビーコン位置データ
				kAllRoadNodeId         = 31, ///< 全道路ノードデータ
				kAllRoadLinkId         = 32, ///< 全道路リンクデータ
				kSiteId                = 41, ///< 水系データ
				kAdminLocationId       = 42, ///< 行政界位置データ
				kRailwayLocationId     = 43, ///< 鉄道位置データ
				kFacilLocationId       = 44, ///< 施設等位置データ
				kFacilShapeId          = 45, ///< 施設等形状データ
				kPlaceNameLocationId   = 46, ///< 地名等表示位置データ
				kAllRoadLinkAttrId     = 93  ///< 全道路リンク内属性データ
			};
		}

		/// ノード種別コード
		namespace b {
			/// ノード種別コード一覧
			enum ECode {
				kCross     = 1, ///< 交差点ノード
				kTerminal  = 2, ///< 行き止まりノード
				kDummy     = 3, ///< ダミー点ノード
				kMeshJoint = 4, ///< 区画辺交点ノード
				kAlter     = 5, ///< 属性変化点ノード
				kEvasion   = 6, ///< 交通管制上必要なノード
			};

			const char* kDomains[];
		}

		/// 交差点通行コード
		namespace c {
			/// 交差点通行コード一覧
			enum ECode {
				kUnknown          = 0, ///< 未調査
				kPassable         = 1, ///< 通行可
				kNoPassage        = 2, ///< 通行不可
				kTypePassable     = 3, ///< 条件付（車種）通行可
				kTimePassable     = 4, ///< 条件付（時刻、曜日）通行可
				kTypeTimePassable = 5, ///< 条件付（時刻、曜日、車種）通行可
			};

			const char* kDomains[];
		}

		/// 管理者コード
		namespace d {
			/// 管理者コード一覧
			enum ECode {
				kUnknown    = 0, ///< 未調査
				kJH         = 1, ///< 日本道路公団
				kPublic3    = 2, ///< 道路３公団
				kPublic     = 3, ///< 道路公社
				kCountry    = 4, ///< 国
				kPrefecture = 5, ///< 都道府県
				kCity       = 6, ///< 指定市
				kOtherTown  = 7, ///< 他の市町村等（含東京23区）
				kOther      = 8, ///< その他の管理者
			};

			const char* kDomains[];
		}

		/// 道路種別コード
		namespace e {
			/// 道路種別コード一覧
			enum ECode {
				kUnknown          = 0, ///< 未調査
				kHighway          = 1, ///< 高速自動車道路
				kCityHighway      = 2, ///< 都市高速道路（含指定都市高速道路）
				kCountry          = 3, ///< 一般国道
				kPrefectureMain   = 4, ///< 主要地方道（都道府県道）
				kCityMain         = 5, ///< 主要地方道（指定市道）
				kPrefectureNormal = 6, ///< 一般都道府県道
				kCityNormal       = 7, ///< 指定市の一般市道
				kOther            = 9, ///< その他道路
			};

			const char* kDomains[];
		}

		/// 主・従道路区分コード
		namespace f {
			/// 主・従道路区分コード一覧
			enum ECode {
				kUnknown = 0, ///< 未調査
				kMain    = 1, ///< 主道路
				kSub     = 2, ///< 従道路
			};

			const char* kDomains[];
		}

		/// リンク種別コード
		namespace h {
			/// リンク種別コード一覧
			enum ECode {
				kUnknown       = 0, ///< 未調査
				kMain          = 1, ///< 本線（上下線非分離）リンク
				kMainUpDown    = 2, ///< 本線（上下分離）リンク
				kConnect       = 3, ///< 連結路（本線間の渡り線）リンク
				kCross         = 4, ///< 交差点内リンク
				kRamp          = 5, ///< 連結路（ランプ）リンク
				kFontage       = 6, ///< 本線と同一路線の側道リンク
				kSA	           = 7, ///< ＳＡ等側線リンク
				kBicycle       = 8, ///< 自転車道等リンク
				kFontageToMain = 9, ///< 本線側道接続リンク
			};

			const char* kDomains[];
		}

		/// リンク通行可・自動車不可コード
		namespace i {
			/// リンク通行可・自動車不可コード一覧
			enum ECode {
				kUnknown      = 0, ///< 未調査
				kCarPassable  = 1, ///< 自動車通行可
				kCarNoPassage = 2, ///< 自動車通行不可
				kPreService   = 3, ///< 未供用（含工事中）
				kPlan         = 4, ///< 計画決定
			};

			const char* kDomains[];
		}

		/// 異常気象時通行規制区間種別コード
		namespace j {
			/// 異常気象時通行規制区間種別コード一覧
			enum ECode {
				kUnknown      = 0, ///< 未調査
				kNoRegulation = 1, ///< 規制無
				kRain         = 2, ///< 雨量規制有
				kSnow         = 3, ///< 雪規制有
				kOther        = 4, ///< その他規制有
			};

			const char* kDomains[];
		}

		/// 道路幅員区分コード
		namespace k {
			/// 道路幅員区分コード一覧
			enum ECode {
				kUnknown         = 0, ///< 未調査
				kOver130         = 1, ///< 幅員13.0m以上
				kBetween55And130 = 2, ///< 幅員5.5m以上～13.0m未満
				kBetween30And55  = 3, ///< 幅員3.0m以上～5.5m未満
				kUnder30         = 4, ///< 幅員3.0m未満
			};

			const char* kDomains[];
		}

		/// 車線数コード
		namespace l {
			/// 車線数コード一覧
			enum ECode {
				kUnknown = 0, ///< 未調査
				k1       = 1, ///< 1車線
				k2       = 2, ///< 2車線
				k3       = 3, ///< 3車線
				k4       = 4, ///< 4車線
				k5       = 5, ///< 5車線
				kOver6   = 6, ///< 6車線異常
			};

			const char* kDomains[];
		}

		/// 交通規制種別コード
		namespace m {
			/// 交通規制種別コード一覧
			enum ECode {
				kUnknown             = 0, ///< 未調査
				kNoRegulation        = 1, ///< 規制無し
				kNoPassage           = 2, ///< 通行禁止（条件無）
				kNoPassageWithCond   = 3, ///< 通行禁止（条件付）
				kFore                = 4, ///< 一方通行(正方向、条件無)
				kReverse             = 5, ///< 一方通行(逆方向、条件無)
				kForeWithCond        = 6, ///< 一方通行(正方向、条件付)
				kReverseWithCond     = 7, ///< 一方通行(逆方向、条件付)
				kForeReverseWithCond = 8, ///< 一方通行(正逆切替え有り)
			};

			const char* kDomains[];
		}

		/// 交通規制条件種別コード
		namespace n {
			/// 交通規制条件種別コード一覧
			enum ECode {
				kUnknown     = 0, ///< 未調査
				kType        = 1, ///< 車種のみ
				kTime        = 2, ///< 時刻のみ
				kWeekday     = 3, ///< 曜日のみ
				kTypeTime    = 4, ///< 車種及び時刻
				kTypeWeekday = 5, ///< 車種及び曜日
				kWeekdayTime = 6, ///< 曜日及び時刻
				kAll         = 7, ///< 車種、時刻及び曜日
				kOther       = 8, ///< その他の条件
			};

			const char* kDomains[];
		}

		/// 規制速度コード
		namespace o {
			/// 規制速度コード一覧
			enum ECode {
				kUnknown = 0, ///< 未調査
				kUnder30 = 1, ///< 30km/h未満
				k40      = 2, ///< 40km/h
				k50      = 3, ///< 50km/h
				k60      = 4, ///< 60km/h
				k70      = 5, ///< 70km/h
				k80      = 6, ///< 80km/h
				k100     = 7, ///< 100km/h
			};

			const char* kDomains[];
		}

		/// 形状データ取得資料コード
		namespace o10 {
			/// 形状データ取得資料コード一覧
			enum ECode {
				kUnknown    = 0, ///< 未調査
				kAfter1991  = 1, ///< 91年度以降道路管理者等資料
				kTopography = 2, ///< 縮尺１/2.5万地形図
				kBefore1990 = 3, ///< 90年度以前道路管理者等資料
				kTopo10000  = 4, ///< 縮尺1/1万以上の地形図
				k5          = 5, ///< 土地区画整理事業者等資料
				k6          = 6, ///< 市町村道整理事業者資料
				k7          = 7, ///< 農道・林道・港湾整備事業者資料
				k8          = 8, ///< 現地調査
			};

			const char* kDomains[];
		}

		/// 属性種別コード
		namespace p {
			/// 属性種別コード一覧
			enum ECode {
				kBridge       = 1, ///< 橋・高架
				kTunnel       = 2, ///< トンネル
				kDoumon       = 3, ///< 洞門等
				kRailCross    = 4, ///< 踏切
				kUnderPass    = 5, ///< 他の施設をアンダーパス
				kPedestrian   = 6, ///< 歩道橋
				kTollbooth    = 7, ///< 料金所（ETC無し）
				kName         = 8, ///< 道路通称名
				kEtcTollbooth = 9, ///< 料金所（ETC有り）
				kEtcTollOnly  = 10,///< 料金所（ETC専用）
				kBypass       = 11,///< バイパス道路名
			};

			const char* kDomains[];
		}

		/// 水系種別コード
		namespace r {
			/// 水系種別コード一覧
			enum ECode {
				kPolygon =  1, ///< 面表示水系
				kPolyline = 2, ///< 線表示水系
			};

			const char* kDomains[];
		}

		/// 線種コード
		namespace s {
			/// 線種コード一覧
			enum ECode {
				kEmptiness = 0, ///< 意味なし
				kLine      = 1, ///< 実境界線
				kBoundray  = 2, ///< 区画辺線
				kVirtual   = 3, ///< 仮想線
			};

			const char* kDomains[];
		}

		/// 鉄道種別コード
		namespace u {
			/// 鉄道種別コード一覧
			enum ECode {
				kUnknown      = 0, ///< 未調査
				kShinkansen   = 1, ///< 新幹線
				kJR           = 2, ///< JR線
				kNormal       = 3, ///< JR線以外（普通鉄道）
				kSurfaceTrack = 4, ///< JR線以外（路面鉄道
				kMonorail     = 5, ///< JR線以外（モノレール）
				kNewTraffic   = 6, ///< JR線以外（新交通）
				kOther        = 7, ///< その他
			};

			const char* kDomains[];
		}

		/// 該当有無コード
		namespace y {
			/// 該当有無コード一覧
			enum ECode {
				kUnknown = 0, ///< 未調査・意味なし
				kTrue    = 1, ///< 該当あり
				kFlase   = 2, ///< 該当無し
			};

			const char* kDomains[];
		}
	}

	using namespace codes::a;
	typedef ECode ERecordId;

	/// 管理データ（その１）データコード空間
	namespace admin1 {
		extern const int kRecordId;
	}

	/// 管理データ（その２）データコード空間
	namespace admin2 {
		extern const int kRecordId;
	}

	/// 基本道路ノードデータコード空間
	namespace base_road_node {
		extern const int kRecordId;

		namespace node_class {
			using namespace ::drma::codes::b;
		};

		namespace passage {
			using namespace ::drma::codes::c;
		};
	}

	/// 基本道路リンクデータコード空間
	namespace base_road_link {
		extern const int kRecordId;

		namespace manager {
			using namespace ::drma::codes::d;
		};

		namespace road_class {
			using namespace ::drma::codes::e;
		}

		namespace kubun {
			using namespace ::drma::codes::f;
		}

		namespace link_class {
			using namespace ::drma::codes::h;
		}

		namespace beacon {
			using namespace ::drma::codes::y;
		}

		namespace car_only {
			using namespace ::drma::codes::y;
		}

		namespace toll {
			using namespace ::drma::codes::y;
		}

		namespace passage {
			using namespace ::drma::codes::i;
		}

		namespace weather_regulation {
			using namespace ::drma::codes::j;
		}

		namespace car_weight {
			using namespace ::drma::codes::y;
		}

		namespace car_height {
			using namespace ::drma::codes::y;
		}

		namespace car_width {
			using namespace ::drma::codes::y;
		}

		namespace width {
			using namespace ::drma::codes::k;
		}

		namespace lane {
			using namespace ::drma::codes::l;
		}

		namespace regulation {
			using namespace ::drma::codes::m;
		}

		namespace regulation_cond {
			using namespace ::drma::codes::n;
		}

		namespace no_passing {
			using namespace ::drma::codes::y;
		}

		namespace speed_regulation {
			using namespace ::drma::codes::o;
		}

		namespace spetial_vehicle {
			using namespace ::drma::codes::y;
		}

		namespace attr {
			using namespace ::drma::codes::y;
		}

		namespace shape_source {
			using namespace ::drma::codes::o10;
		}

		namespace designate {
			using namespace ::drma::codes::y;
		}
	}

	/// 基本道路リンク内属性データコード空間
	namespace base_road_link_attr {
		extern const int kRecordId;

		namespace attr_class {
			using namespace ::drma::codes::p;
		}

		namespace pre_begin {
			using namespace ::drma::codes::y;
		}

		namespace epi_begin {
			using namespace ::drma::codes::y;
		}

		namespace heavy_no_passage {
			using namespace ::drma::codes::y;
		}
	}

	/// 基本道路リンク・全道路リンク対応データコード空間
	namespace road_link_relation {
		extern const int kRecordId;
	}

	/// ビーコン位置データコード空間
	namespace beacon_location {
		extern const int kRecordId;
	}

	/// 全道路ノードデータコード空間
	namespace all_road_node {
		extern const int kRecordId;

		namespace node_class {
			using namespace ::drma::codes::b;
		};
	}

	/// 全道路リンクデータコード空間
	namespace all_road_link {
		extern const int kRecordId;

		namespace manager {
			using namespace ::drma::codes::d;
		};

		namespace road_class {
			using namespace ::drma::codes::e;
		}

		namespace width {
			using namespace ::drma::codes::k;
		}

		namespace lane {
			using namespace ::drma::codes::l;
		}

		namespace regulation {
			using namespace ::drma::codes::m;
		}

		namespace regulation_cond {
			using namespace ::drma::codes::n;
		}

		namespace attr {
			using namespace ::drma::codes::y;
		}

	}

	/// 水系データコード空間
	namespace site {
		extern const int kRecordId;

		namespace site_class {
			using namespace ::drma::codes::r;
		}

		namespace line_class {
			using namespace ::drma::codes::s;
		}
	}

	/// 行政界位置データコード空間
	namespace admin_location {
		extern const int kRecordId;
	}

	/// 鉄道位置データコード空間
	namespace railway_location {
		extern const int kRecordId;

		namespace railway_class {
			using namespace ::drma::codes::u;
		}

		namespace underground {
			using namespace ::drma::codes::y;
		}
	}

	/// 施設等位置データコード空間
	namespace facil_location {
		extern const int kRecordId;
	}

	/// 施設等形状データコード空間
	namespace facil_shape {
		extern const int kRecordId;
	}

	/// 地名等表示位置データコード空間
	namespace place_name_location {
		extern const int kRecordId;
	}

	/// 全道路リンク内属性データコード空間
	namespace all_road_link_attr {
		extern const int kRecordId;
	}
}

#endif // !defined(AFX_DRMACODECOMMUNITY_H__0157285E_1EEA_488E_A218_EB21BB39E5AC__INCLUDED_)
