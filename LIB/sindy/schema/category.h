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

#ifndef SINDY_SCHEMA_CATEGORY_H_
#define SINDY_SCHEMA_CATEGORY_H_

namespace sindy {

namespace schema {

namespace category {

/// メッシュにひもづくテーブル
namespace mesh_code_table {
	extern const _TCHAR kMeshCode[]; ///< メッシュコード
}

/// リンクにひもづくテーブル
namespace link_relational_table {
	extern const _TCHAR kLinkID[]; ///< リンクID
}

/// リンクとその向きにひもづくテーブル
namespace directional_link_relational_table {
	using namespace ::sindy::schema::category::link_relational_table;

	extern const _TCHAR kLinkDir[]; ///< リンク方向コード

	/// リンク方向コード
	namespace link_dir {
		enum ECode {
			kFore    = 1, ///< 順方向
			kReverse = 2, ///< 逆方向
		};
	}
}

/// 始終点がノードとひもづくテーブル
namespace from_to_node_relational_table {
	extern const _TCHAR kFromNodeID[]; ///< 始点側ノードのOBJECTID
	extern const _TCHAR kToNodeID[]; ///< 終点側ノードのOBJECTID
}

/// 時間規制にひもづくテーブル
namespace time_regulation_relational_table {
	extern const _TCHAR kOneway[];      ///< 一方通行種別コード
	extern const _TCHAR kOnewayRF[];    ///< 条件付一方通行関係フラグ
	extern const _TCHAR kNoPassage[];   ///< 通行禁止種別コード
	extern const _TCHAR kNoPassageRF[]; ///< 条件付通行禁止関係フラグ
} // time_regulation_relational_table
/// ADAM v2用
/// 始終点ノードGID
namespace adam_v2_from_to_node_relational_table_gid {
	extern const _TCHAR kFromNodeGID[]; ///< 始点側ノードのGID
	extern const _TCHAR kToNodeGID[]; ///< 終点側ノードのGID
}
/// ノード
namespace adam_v2_node {
	extern const _TCHAR kNodeClassC[];			///< ノード種別コード
	/// ADAMノード種別コード
	namespace node_class_code {
		enum ECode {
			kUnset					= -1,		///< 未設定
			kNode					= 1,	///< ノード
			kInterpolationPoint		= 2,	///< 補間点
		};
	}
}
/// リンクのコード値ドメイン
namespace adam_v2_link_code {
	/// ADAM制限速度コード
	namespace speed_code {
		enum ECode {
			kUnset				= -1,	///< 未設定
			k20km				= 20,	///< 20km/h
			k30km				= 30,	///< 30km/h
			k40km				= 40,	///< 40km/h
			k50km				= 50,	///< 50km/h
			k60km				= 60,	///< 60km/h
			k70km				= 70,	///< 70km/h
			k80km				= 80,	///< 80km/h
			k90km				= 90,	///< 90km/h
			k100km				= 100,	///< 100km/h
			k110km				= 110,	///< 110km/h
			k120km				= 120,	///< 120km/h
			kNoLabel			= 1001,	///< 標識/表示なし
			kVariable			= 1002,	///< 可変式
			kVariable20km		= 2020,	///< 可変式、20km/h
			kVariable30km		= 2030,	///< 可変式、30km/h
			kVariable40km		= 2040,	///< 可変式、40km/h
			kVariable50km		= 2050,	///< 可変式、50km/h
			kVariable60km		= 2060,	///< 可変式、60km/h
			kVariable70km		= 2070,	///< 可変式、70km/h
			kVariable80km		= 2080,	///< 可変式、80km/h
			kVariable90km		= 2090,	///< 可変式、90km/h
			kVariable100km		= 2100,	///< 可変式、100km/h
			kVariable110km		= 2110,	///< 可変式、110km/h
			kVariable120km		= 2120,	///< 可変式、120km/h
		};
	}

	/// ADAM進行方向種別コード
	namespace travel_dir {
		enum ECode {
			kUnset				= -1,	///< 未設定
			kStraight			= 1,	///< 直進
			kRight				= 2,	///< 右折
			kLeft				= 3,	///< 左折
			kRightBranch		= 4,	///< 右分岐
			kLeftBranch			= 5,	///< 左分岐
			kRightConfluence	= 6,	///< 右合流
			kLeftConfluence		= 7,	///< 左合流
			kRightLaneChange	= 8,	///< 右レーンチェンジ
			kLeftLaneChange		= 9,	///< 左レーンチェンジ
		};
	}

	/// ADAM道路構造種別コード
	namespace road_structure_code {
		enum ECode {
			kUnset				= -1,	///< 未設定
			kRoad				= 1,	///< 単路
			kBranchConfluence	= 2,	///< 分岐合流車線
			kClimbing			= 3,	///< 登坂車線
			kSapa				= 4,	///< SAPA
			kBusOnly			= 5,	///< バス専用
			kNoLaneSection		= 6,	///< 無車線区間
			kTollGeneral		= 7,	///< 料金所（一般）
			kTollETC			= 8,	///< 料金所（ETC）
			kTollETCGeneral		= 9,	///< 料金所（ETC／一般）
			kTollUnknown		= 10,	///< 料金所（不明）
			kTollDisabled		= 11,	///< 料金所（使用不可）
			kTollSwitch			= 12,	///< 料金所（切り替え）
		};
	}

	/// ADAM一方通行種別コード
	namespace oneway_code {
		enum ECode {
			kUnset					= -1,	///< 未設定
			kNoOneway				= 0,	///< 一方通行なし
			kFore					= 1,	///< 条件なし一方通行（正方向）
			kReverse				= 2,	///< 条件なし一方通行（逆方向）
			kForeWithReg			= 3,	///< 条件付き一方通行（正方向）
			kReverseWithReg			= 4,	///< 条件付き一方通行（逆方向）
			kForeReverseWithReg		= 5,	///< 条件付き一方通行（正逆あり）
		};
	}
}

/// ノードのコード値ドメイン
namespace adam_v2_node_code {

	/// ADAM位置種別コード
	namespace position_code {
		enum ECode {
			kUnset = -1,	///< 未設定
			kNone = 0,		///< 位置情報なし
			kToll = 1,		///< 料金所（ブース開始位置）
		};
	}
}

namespace adam_v2_group{

	extern const _TCHAR kGroupC[];				///< グループ種別コード

	/// ADAMグループ種別コード
	namespace group_code {
		enum ECode {
			kUnset				= -1,		///< 未設定
			kRoad				= 1,		///< 車線
			kCross				= 2,		///< 交差点
			kBranchConfluence	= 3,		///< 分合流
			kToll				= 4,		///< 料金所
			kSAPA				= 5,		///< SAPA
			kSmartIC			= 6,		///< スマートIC
		};
	}
}

namespace adam_v2_link_dir_code{
	/// ADAMリンク方向コード
	namespace roadlink_dir_code {
		enum ECode {
			kUnset				= -1,		///< 未設定
			kForward			= 1,		///< 順方向
			kReverse			= 2,		///< 逆方向
		};
	}
}

namespace adam_v2_lr_code{
	/// ADAM左右判定コード
	namespace lr_code {
		enum ECode {
			kUnset				= -1,		///< 未設定
			kLeft				= 0,		///< 左
			kRight				= 1,		///< 右
		};
	}
}
/* ============================================================================== */

/// 時間規制条件テーブル
namespace time_condition_table {
	extern const _TCHAR kStartMonth[]; ///< 規制条件:開始月
	extern const _TCHAR kStartDay[]; ///< 規制条件:開始日
	extern const _TCHAR kStartHour[]; ///< 規制条件:開始時間
	extern const _TCHAR kStartMin[]; ///< 規制条件:開始分
	extern const _TCHAR kEndMonth[]; ///< 規制条件;終了月
	extern const _TCHAR kEndDay[]; ///< 規制条件:終了日
	extern const _TCHAR kEndHour[]; ///< 規制条件:終了時間
	extern const _TCHAR kEndMin[]; ///< 規制条件:終了分
	extern const _TCHAR kDayOfWeek[]; ///< 規制条件:曜日指定

	namespace dayofweek {
		enum ECode {
			kSunHoliday          = 1,  ///< 日・休日
			kExceptSunHoliday    = 2,  ///< 日・休日をのぞく
			kHoliday             = 3,  ///< 休日(日曜はのぞく)
			kExceptHoliday       = 4,  ///< 休日をのぞく)
			kWeekday             = 5,  ///< 平日
			kSunday              = 6,  ///< 日曜
			kMonday              = 7,  ///< 月
			kTuesday             = 8,  ///< 火
			kWednesday           = 9,  ///< 水
			kThursday            = 10, ///< 木
			kFriday              = 11, ///< 金
			kSaturday            = 12, ///< 土
			kSatSunHoliday       = 13, ///< 土日・休日
			kExceptSatSunHoliday = 14, ///< 土日・休日をのぞく
			kAllDay              = 15, ///< 全曜日
			kExceptSunday        = 16, ///< 日曜日をのぞく
			kBeforeSunHoliday    = 17, ///< 日・休日の前日
		};
	}
}

/// VICSテーブル
namespace vics_table {
	extern const _TCHAR kVICSMeshCode[]; ///< VICSメッシュコード
	extern const _TCHAR kVICSClass[]; ///< VICSリンク区分
	extern const _TCHAR kVICSLinkID[]; ///< VICSリンク番号

	/// VICSリンク区分
	namespace vics_class {
		enum ECode {
			kHighway     = 1, ///< 高速道路
			kCityHighway = 2, ///< 都市高速道路
			kNormal      = 3, ///< 一般道
			kOther       = 4, ///< その他の道路
		};
	}
	/// IPC-VICSリンク区分
	namespace ipcvics_class {
		enum ECode {
			kCode1		= 1, ///< 区分1
			kCode2		= 2, ///< 区分2
			kCode3		= 3, ///< 区分3
			kCode4		= 4, ///< 区分4
		};
	}
}

/// 元VICSテーブル
namespace original_vics_table {
	extern const _TCHAR kMeshCode[]; ///< VICSメッシュコード
	extern const _TCHAR kVICSClass[]; ///< VICSリンク区分
	extern const _TCHAR kVICSLinkID[]; ///< VICSリンク番号
}

namespace drive_map_table {
	extern const _TCHAR kMeshCode[]; ///< 走りやすさマップメッシュコード
	extern const _TCHAR kDriveMapRank[]; ///< 走りやすさマップ評価
	extern const _TCHAR kRankLinkID[]; ///< 走りやすさマップリンク番号
}

/// 駐車場関係テーブル
namespace parking_table {
	extern const _TCHAR kParkingClass[]; ///< 駐車場種別コード

	/// 駐車場種別コード
	namespace parking_class {
		enum ECode {
			kUnsurveyed           = 0, ///< 未調査
			kStructure            = 1, ///< 立体駐車場
			kUnderGround          = 2, ///< 地下駐車場
			kUnderGroundStructure = 3, ///< 立体かつ地下駐車場
			kTower                = 4, ///< タワーパーキング
			kRoll                 = 5, ///< 自動回転タワーパーキング
			kFlat                 = 6, ///< 平面駐車場
		};
	}
}

/// ハイウェイの路線に関連付くテーブル
namespace highway_road_relational_table {
	extern const _TCHAR kRoadCode[]; ///< 路線コード
}

/// ハイウェイの施設に関連付くテーブル
namespace highway_facil_relational_table {
	using namespace ::sindy::schema::category::highway_road_relational_table;

	extern const _TCHAR kRoadSeq[]; ///< 路線内シーケンス
}

/// ハイウェイ方向テーブル
namespace highway_direction_table {
	extern const _TCHAR kDirection[]; ///< ハイウェイ方向コード

	/// 方向コード定義
	namespace direction {
		enum ECode {
			kNoDir  = 0, ///< 方向なし
			kUp     = 1, ///< 方向１（上り・内回り・西行き）
			kDown   = 2, ///< 方向２（下り・外回り・東行き）
			kUpDown = 3, ///< 双方向共通
		};
	}
}

/// 入口・出口属性コードを含むテーブル
namespace highway_in_out_table {
	extern const _TCHAR kInOut[]; ///< 入口・出口属性

	/// 入口（合流）・出口（分岐）コード定義
	namespace in_out {
		enum ECode {
			kInOut = 0, ///< 区別なし
			kIn    = 1, ///< 入口・合流
			kOut   = 2, ///< 出口・分岐
		};
	}
}

/// SiNDY-cに関するテーブル
namespace sindyc_table {
	extern const _TCHAR kOrgOperator[];    ///< ORG_OPERATOR
	extern const _TCHAR kOrgModifyDate[];  ///< ORG_MODIFY_DATE
	extern const _TCHAR kOrgOBJID[];       ///< ORG_OBJID
	extern const _TCHAR kUpdateC[];        ///< UPDATE_C
	extern const _TCHAR kCheckC[];         ///< CHECK_C

	/// 更新コード（重ねあわせ可）
	namespace update_code {
		enum ECode {
			kDefault = 0,      ///< 更新なし
			kShapeUpdated = 1, ///< 形状変更
			kAttrUpdated = 2,  ///< 属性変更
			kDeleted = 4,      ///< 削除
			kCreated = 8,      ///< 新規作成
		};
	}
}

/// シリアルIDを含むテーブル
namespace serial_id_table
{
	extern const _TCHAR kSerialID[];	//!< シリアルID
}

/// コンテンツ内シーケンスを含むテーブル
namespace contents_seq_table
{
	extern const _TCHAR kContentsSeq[];	//!< コンテンツ内シーケンス
}

/// データ分類コードを含むテーブル
namespace group_code_table
{
	extern const _TCHAR kGroupCode[];	//!< データ分類コード
}

/// 店舗コードを含むテーブル
namespace chain_code_table
{
	extern const _TCHAR kChainCode[];	//!< 店舗コード
}

/// コンテンツコードを含むテーブル
namespace contents_code_table
{
	extern const _TCHAR kContentsCode[];	//!< コンテンツコード
}

/// 区間別称に関連付くテーブル [bug 12036]
namespace section_code_relational_table {
	extern const _TCHAR kSectionCode[];	//!< 区間別称コード
}

/// 都道府県道名称に関連付くテーブル [bug 12036]
namespace pref_road_relational_table {
	extern const _TCHAR kRoadClass[];	//!< 道路表示種別
	extern const _TCHAR kPrefCode[];	//!< 都道府県コード
	extern const _TCHAR kRoadNo[];		//!< 県道番号

		/// 道路表示用種別コード
	namespace road_class {
		enum ECode {
			kNone	              =   0, ///< 表示種別なし（[Bug8941]で追加）
			kCountry              =   1, ///< 一般国道
			kPrefectureMain       =   2, ///< 主要地方道（都府県）
			kCityMain             =   3, ///< 主要地方道（指定市）
			kPrefectureNormal     =   4, ///< 一般都道府県道
			kCityNormal           =   5, ///< 指定市の一般市道
			kOther                =   6, ///< その他道路
			kOtherNoDisp          =   7, ///< その他道路（非表示）
			kNarrowL1             =   8, ///< 細街路L1
			kNarrowL2             =   9, ///< 細街路L2
			kNarrowL3             =  10, ///< 細街路L3
			kFreeHighway		  =	 11, ///< 高速自動車道路
			kHighway              = 101, ///< 高速自動車道路（有料）
			kCityHighway          = 102, ///< 都市高速道路（有料）
			kTollCountry          = 103, ///< 一般国道（有料）
			kTollPrefectureMain   = 104, ///< 主要地方道（都府県）（有料）
			kTollCityMain         = 105, ///< 主要地方道（指定市）（有料）
			kTollPrefectureNormal = 106, ///< 一般都道府県道（有料）
			kTollCityNormal       = 107, ///< 指定市の一般市道（有料）
			kTollOther            = 108, ///< その他道路（有料）
			kFerryNoDisp          = 201, ///< フェリー航路（非表示部）
			kFerryS2              = 202, ///< フェリー航路（S2破線表示部）
			kFerryS3              = 203, ///< フェリー航路（S3破線表示部）
			kFerryS4              = 204, ///< フェリー航路（S4破線表示部）
			kVirtual              = 301, ///< 仮想リンク
			kGarden               = 302, ///< 庭園路
			kBridge               = 303, ///< ブリッジリンク
			kFacilityEntrance     = 304, ///< 施設出入口リンク
			kParkingEntrance      = 305, ///< 施設内リンク
			kParking              = 401, ///< 駐車場構内道路リンク(PEC)
			kMatching             = 402, ///< マッチング用リンク(PEC)
			kMatchingBT			  = 501, ///< マッチング用リンク(BT)
		};
	}
}

} // namespace category

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_CATEGORY_H_
