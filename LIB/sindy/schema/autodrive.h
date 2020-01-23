#ifndef SINDY_SCHEMA_AUTODRIVE_H_
#define SINDY_SCHEMA_AUTODRIVE_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>
#include <sindy/libschema.h>

namespace sindy {

namespace schema {

/**
 * 自動運転支援マップ(ADAM v2)
 * @version	2.1.6
 */ 
namespace adam_v2 {
	
	extern const _TCHAR kObjectID[];		///< オブジェクトIDカラム名

// 車線リンク
namespace lane_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;
	using namespace ::sindy::schema::category::adam_v2_link_code;

	extern const _TCHAR kTableName[];			///< テーブル名
	
	extern const _TCHAR kLaneGroupID[];			///< 車線グループOID
	extern const _TCHAR kLaneNumber[];			///< 車線番号
	extern const _TCHAR kLaneCount[];			///< 車線数
	extern const _TCHAR kMaxLegalSpeedC[];		///< 制限速度コード（最大）
	extern const _TCHAR kMinLegalSpeedC[];		///< 制限速度コード（最小）
	extern const _TCHAR kTravelDirectionC[];	///< 進行方向種別コード
	extern const _TCHAR kRoadStructureC[];		///< 道路構造種別コード
	extern const _TCHAR kOneWayC[];				///< 一方通行種別コード
	extern const _TCHAR kLeftChangeF[];			///< 左車線変更可能フラグ
	extern const _TCHAR kRightChangeF[];		///< 右車線変更可能フラグ
	extern const _TCHAR kLeftProtrusionF[];		///< 左はみ出し可能フラグ
	extern const _TCHAR kRightProtrusionF[];	///< 右はみ出し可能フラグ
	extern const _TCHAR kRubbingF[];			///< 擦り付けフラグ
	extern const _TCHAR kTunnelF[];				///< トンネルフラグ
}

/// 車線ノード
namespace lane_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kPositionC[];			///< 位置種別コード
	extern const _TCHAR kStopPointF[];			///< 停止点フラグ
	extern const _TCHAR kCoveringF[];			///< 覆いものフラグ
}

// 上下線リンク
namespace updown_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;
	using namespace ::sindy::schema::category::adam_v2_link_code;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kUpdoenGroupID[];		///< 上下線グループOID
	extern const _TCHAR kLaneCount[];			///< 車線数
	extern const _TCHAR kRightChange[];			///< 右側増減車線数
	extern const _TCHAR kLeftChange[];			///< 左側増減車線数
	extern const _TCHAR kMaxLegalSpeedC[];		///< 制限速度コード（最大）
	extern const _TCHAR kMinLegalSpeedC[];		///< 制限速度コード（最小）
	extern const _TCHAR kTravelDirectionC[];	///< 進行方向種別コード
	extern const _TCHAR kRoadStructureC[];		///< 道路構造種別コード
	extern const _TCHAR kOneWayC[];				///< 一方通行種別コード
	extern const _TCHAR kRubbingF[];			///< 擦り付けフラグ
	extern const _TCHAR kTunnelF[];				///< トンネルフラグ
}

/// 上下線ノード
namespace updown_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kPositionC[];			///< 位置種別コード
	extern const _TCHAR kStopPointF[];			///< 停止点フラグ
	extern const _TCHAR kCoveringF[];			///< 覆いものフラグ
}

// 区画線リンク
namespace compart_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kCompartLineC[];		///< 線種別コード
	extern const _TCHAR kCompartStyleC[];		///< 区画線設置様式コード
	extern const _TCHAR kSlowdownLabelC[];		///< 減速標示コード
	extern const _TCHAR kInstallationF[];		///< 設置物フラグ

	/// ADAM区画線種別コード
	namespace compart_line_code {
		enum ECode {
			kUnset			= -1,											///< 未設定
			kVirtual		= 0,											///< 仮想線
			kWhiteDashed	= 1,											///< 白破線
			kWhiteSolid		= 2,											///< 白実線
			kYellowSolid	= 3,											///< 黄実線
			kWhiteDashedAndYellowSolid = 13,								///< 白破線(左) + 黄実線(右)　 (2重線)
			kYellowSolidAndWhiteDashed = 31,								///< 黄実線(左) + 白破線(右)　 (2重線)
			kWhiteDashedAndWhiteSolid = 12,									///< 白破線(左) + 白実線(右)　 (2重線)
			kWhiteSolidAndWhiteDashed = 21,									///< 白実線(左) + 白破線(右)　 (2重線)
			kWhiteSolidAndWhiteSolid = 22,									///< 白実線(左) + 白実線(右)　 (2重線)
			kWhiteSolidAndYellowSolid = 23,									///< 白実線(左) + 黄実線(右)　 (2重線)
			kYellowSolidAndWhiteSolid = 32,									///< 黄実線(左) + 白実線(右)　 (2重線)
			kYellowSolidAndYellowSolid = 33,								///< 黄実線(左) + 黄実線(右)　 (2重線)
			kWhiteSolidAndYellowSolidAndWhiteSolid = 232,					///< 白実線(左) + 黄実線(中) + 白実線(右)　 (3重線)
			kYellowSolidAndWhiteDashedAndYellowSolid = 313,					///< 黄実線(左) + 白破線(中) + 黄実線(右)　 (3重線)
			kYellowSolidAndWhiteSolidAndYellowSolid = 323,					///< 黄実線(左) + 白実線(中) + 黄実線(右)　 (3重線)
			kYellowSolidAndWhiteSolidAndWhiteSolidAndYellowSolid = 3223,	///< 黄実線(左) + 白実線(中左) + 白実線(中右) + 黄実線(右)　 (4重線)
		};
	}

	/// ADAM区画線設置様式コード
	namespace compart_style_code {
		enum ECode {
			kUnset			= -1,	///< 未設定
			kLeftOutside	= 1,	///< 左外側線
			kRightOutside	= 2,	///< 右外側線
			kLaneMarking	= 3,	///< 車線区分線
			kCenterLane 	= 4,	///< 中央線
		};
	}

	namespace slow_down_code {
		enum ECode {
			kUnset		= -1,	///< 未設定
			kNone		= 0,	///< 減速標示なし
			kLeft		= 1,	///< 左に減速標示あり
			kRight		= 2,	///< 右に減速標示あり
			kBoth		= 3,	///< 左右に減速標示あり
		};
	}
}

/// 区画線ノード
namespace compart_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< テーブル名
}

// 道路境界線リンク
namespace border_link {
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::adam_v2_from_to_node_relational_table_gid;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kImaginaryBorderF[];	///< 仮想線フラグ
}

/// 道路境界線ノード
namespace border_node{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;
	using namespace ::sindy::schema::category::adam_v2_node;

	extern const _TCHAR kTableName[];			///< テーブル名
}

/// 信号機ポイント
namespace signal_point{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kHorizontalAngle[];		///< 水平角
	extern const _TCHAR kGroundHeight[];		///< 地上高
	extern const _TCHAR kHeight[];				///< 縦幅
	extern const _TCHAR kWidth[];				///< 横幅
	extern const _TCHAR kSignalMoldC[];			///< 信号機形状コード
	extern const _TCHAR kLampCount[];			///< 信号灯数
	extern const _TCHAR kArrowF[];				///< 矢印信号フラグ
	extern const _TCHAR kWarningF[];			///< 予告信号フラグ
	extern const _TCHAR kIndependentF[];		///< 独立信号フラグ

	/// ADAM信号機形状コード
	namespace signal_mold_code {
		enum ECode {
			kUnset			= -1,		///< 未設定
			kVertical		= 1,		///< 縦型
			kHorizontal		= 2,		///< 横型
		};
	}
}

/// 横断歩道ライン
namespace crosswalk_line{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kWidth[];				///< 幅
}

/// 道路標識ポイント
namespace roadsign_point{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kHorizontalAngle[];		///< 水平角
	extern const _TCHAR kGroundHeight[];		///< 地上高
	extern const _TCHAR kHeight[];				///< 縦幅
	extern const _TCHAR kWidth[];				///< 横幅
	extern const _TCHAR kRoadSignC[];			///< 道路標識コード
	extern const _TCHAR kVariableF[];			///< 可変フラグ

	/// ADAM道路標識コード
	namespace road_sign_code {
		extern const _TCHAR kUnset[];			///< 未設定
		extern const _TCHAR k116_4[];			///< (116の4)非常電話
		extern const _TCHAR k116_6[];			///< (116の6)非常駐車帯
		extern const _TCHAR k119_D[];			///< (119-D)道路の通称名
		extern const _TCHAR k201_A[];			///< (201-A)十形道路交差点あり
		extern const _TCHAR k201_B[];			///< (201-B)ト形道路交差点あり
		extern const _TCHAR k201_C[];			///< (201-C)Ｔ形道路交差点あり
		extern const _TCHAR k201_D[];			///< (201-D)Ｙ形道路交差点あり
		extern const _TCHAR k201_2[];			///< (201の2)ロータリーあり
		extern const _TCHAR k202[];				///< (202)右（又は左）方屈曲あり
		extern const _TCHAR k203[];				///< (203)右（又は左）方屈折あり
		extern const _TCHAR k204[];				///< (204)右（又は左）方背向屈曲あり
		extern const _TCHAR k205[];				///< (205)右（又は左）方背向屈折あり
		extern const _TCHAR k206[];				///< (206)右（又は左）つづら折りあり
		extern const _TCHAR k207_A[];			///< (207-A)踏切あり
		extern const _TCHAR k207_B[];			///< (207-B)踏切あり
		extern const _TCHAR k208[];				///< (208)学校、幼稚園、保育所等あり
		extern const _TCHAR k208_2[];			///< (208の2)信号機あり
		extern const _TCHAR k209[];				///< (209)すべりやすい
		extern const _TCHAR k209_2[];			///< (209の2)落石のおそれあり
		extern const _TCHAR k209_3[];			///< (209の3)路面凹凸あり
		extern const _TCHAR k210[];				///< (210)合流交通あり
		extern const _TCHAR k211[];				///< (211)車線数減少
		extern const _TCHAR k212[];				///< (212)幅員減少
		extern const _TCHAR k212_2[];			///< (212の2)二方向交通
		extern const _TCHAR k212_3[];			///< (212の3)上り急勾配あり
		extern const _TCHAR k212_4[];			///< (212の4)下り急勾配あり
		extern const _TCHAR k213[];				///< (213)道路工事中
		extern const _TCHAR k214[];				///< (214)横風注意
		extern const _TCHAR k214_2[];			///< (214の2)動物が飛び出すおそれあり
		extern const _TCHAR k215[];				///< (215)その他の危険
		extern const _TCHAR k301[];				///< (301)通行止め
		extern const _TCHAR k302[];				///< (302)車両通行止め
		extern const _TCHAR k303[];				///< (303)車両進入禁止
		extern const _TCHAR k304[];				///< (304)二輪の自動車以外の自動車通行止め
		extern const _TCHAR k305[];				///< (305)大型貨物自動車等通行止め
		extern const _TCHAR k305_2[];			///< (305の2)特定の最大積載量以上の貨物自動車等通行止め
		extern const _TCHAR k306[];				///< (306)大型乗用自動車等通行止め
		extern const _TCHAR k307[];				///< (307)二輪の自動車・原動機付自転車通行止め
		extern const _TCHAR k308[];				///< (308)自転車以外の軽車両通行止め
		extern const _TCHAR k309[];				///< (309)自転車通行止め
		extern const _TCHAR k310[];				///< (310)車両（組合せ）通行止め
		extern const _TCHAR k310_2[];			///< (310の2)大型自動二輪車及び普通自動二輪車二人乗り通行禁止
		extern const _TCHAR k311_A[];			///< (311-A)指定方向外進行禁止
		extern const _TCHAR k311_B[];			///< (311-B)指定方向外進行禁止
		extern const _TCHAR k311_C[];			///< (311-C)指定方向外進行禁止
		extern const _TCHAR k311_D[];			///< (311-D)指定方向外進行禁止
		extern const _TCHAR k311_E[];			///< (311-E)指定方向外進行禁止
		extern const _TCHAR k311_F[];			///< (311-F)指定方向外進行禁止
		extern const _TCHAR k312[];				///< (312)車両横断禁止
		extern const _TCHAR k313[];				///< (313)転回禁止
		extern const _TCHAR k314[];				///< (314)追越しのための右側部分はみ出し通行禁止
		extern const _TCHAR k314_2[];			///< (314の2)追越し禁止
		extern const _TCHAR k315[];				///< (315)駐停車禁止
		extern const _TCHAR k316[];				///< (316)駐車禁止
		extern const _TCHAR k317[];				///< (317)駐車余地
		extern const _TCHAR k318[];				///< (318)時間制限駐車区間
		extern const _TCHAR k319[];				///< (319)危険物積載車両通行止め
		extern const _TCHAR k320[];				///< (320)重量制限
		extern const _TCHAR k321[];				///< (321)高さ制限
		extern const _TCHAR k322[];				///< (322)最大幅
		extern const _TCHAR k323[];				///< (323)最高速度
		extern const _TCHAR k323_2[];			///< (323の2)特定の種類の車両の最高速度
		extern const _TCHAR k324[];				///< (324)最低速度
		extern const _TCHAR k325[];				///< (325)自動車専用
		extern const _TCHAR k327_8[];			///< (327の8)原動機付自転車の右折方法（二段階）
		extern const _TCHAR k327_9[];			///< (327の9)原動機付自転車の右折方法（小回り）
		extern const _TCHAR k327_10[];			///< (327の10)環状の交差点における右回り通行
		extern const _TCHAR k327_11[];			///< (327の11)平行駐車
		extern const _TCHAR k327_12[];			///< (327の12)直角駐車
		extern const _TCHAR k327_13[];			///< (327の13)斜め駐車
		extern const _TCHAR k328[];				///< (328)警笛鳴らせ
		extern const _TCHAR k328_2[];			///< (328の2)警笛区間
		extern const _TCHAR k329[];				///< (329)徐行
		extern const _TCHAR k329_2[];			///< (329の2)前方優先道路
		extern const _TCHAR k330[];				///< (330)一時停止
		extern const _TCHAR k331[];				///< (331)歩行者通行止め
		extern const _TCHAR k332[];				///< (332)歩行者横断禁止
	}
}

/// 整備済みエリア
namespace maintenance_area{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名
}

/// 道路標示エリア
namespace roadmark_area{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kRoadMarkC[];			///< 道路標示コード
	extern const _TCHAR kDescription[];			///< 標示内容（文字）

	/// ADAM道路標示コード
	namespace roadmark_code {
		extern const _TCHAR kUnset[];			///< 未設定
		extern const _TCHAR k210[];				///< (210)横断歩道又は時点者横断帯あり
		extern const _TCHAR k105_30[];			///< (105)最高速度30km/h
		extern const _TCHAR k105_40[];			///< (105)最高速度40km/h
		extern const _TCHAR k105_50[];			///< (105)最高速度50km/h
	}
}

/// 仮想線ライン
namespace virtual_compart_line{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名
}

/// 覆い物エリア
namespace covering_area{
	using namespace ::sindy::schema::adam_v2_adam_ipc_feature;

	extern const _TCHAR kTableName[];			///< テーブル名
}

/// 道路リンク－上下線リンク関連
namespace rel_road_link_updown_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kRoadLinkID[];			///< 道路リンクOID
	extern const _TCHAR kUpdownLinkGID[];		///< 上下線リンクGID
	extern const _TCHAR kRoadLinkDirectionC[];	///< 道路リンク方向コード
}

/// 上下線ペアリンク
namespace pair_updown_link{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kUpdownLinkGID1[];		///< 上下線リンクGID1
	extern const _TCHAR kUpdownLinkGID2[];		///< 上下線リンクGID2
}

/// 上下線リンクグループ
namespace updown_link_group{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_group;

	extern const _TCHAR kTableName[];			///< テーブル名
}

/// 上下線リンク－車線リンク関連
namespace rel_updown_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kUpdownLinkGID[];		///< 上下線リンクGID
	extern const _TCHAR kLaneLinkGID[];			///< 車線リンクGID
}

/// 道路リンク－車線リンク関連
namespace rel_road_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;

	extern const _TCHAR kTableName[];			///< テーブル名

	extern const _TCHAR kRoadLinkID[];			///< 道路リンクOID
	extern const _TCHAR kLaneLinkGID[];			///< 車線リンクGID
	extern const _TCHAR kRoadLinkDirectionC[];	///< 道路リンク方向コード
}

/// 車線リンクグループ
namespace lane_link_group{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_group;

	extern const _TCHAR kTableName[];			///< テーブル名
	extern const _TCHAR kUpDownLinkGroupID[];	///< 上下線リンクグループOID
}

/// 区画線リンク－車線リンク関連
namespace rel_compart_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;
	using namespace ::sindy::schema::category::adam_v2_lr_code;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kCompartLinkGID[];			///< 区画線リンクGID
	extern const _TCHAR kLaneLinkGID[];				///< 車線リンクGID
	extern const _TCHAR kCompartLinkDirectionC[];	///< 区画線リンク方向コード
	extern const _TCHAR kLRC[];						///< 左右判定コード
	extern const _TCHAR kSequence[];				///< 順番
}

/// 道路境界線リンク－車線リンク関連
namespace rel_border_link_lane_link{
	using namespace ::sindy::schema::adam_v2_table;
	using namespace ::sindy::schema::category::adam_v2_link_dir_code;
	using namespace ::sindy::schema::category::adam_v2_lr_code;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kBorderLinkGID[];			///< 道路境界線リンクGID
	extern const _TCHAR kLaneLinkGID[];				///< 車線リンクGID
	extern const _TCHAR kBorderLinkDirectionC[];	///< 道路境界線リンク方向コード
	extern const _TCHAR kLRC[];						///< 左右判定コード
	extern const _TCHAR kSequence[];				///< 順番
}

/// 上下線ノード－信号機ポイント関連
namespace rel_updown_node_signal_point{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kUpdownNodeGID[];			///< 上下線ノードGID
	extern const _TCHAR kSignalPointGID[];			///< 信号機ポイントGID
}

/// 車線ノード－信号機ポイント関連
namespace rel_lane_node_signal_point{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kLaneNodeGID[];				///< 車線ノードGID
	extern const _TCHAR kSignalPointGID[];			///< 信号機ポイントGID
}

/// 車線ノード－横断歩道ライン関連
namespace rel_lane_node_crosswalk_line{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kLaneNodeGID[];				///< 車線ノードGID
	extern const _TCHAR kCrosswalkLineGID[];		///< 横断歩道ラインGID
}

/// 車線ノード－道路標識ポイント関連
namespace rel_lane_node_roadsign_point{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kLaneNodeGID[];				///< 車線ノードGID
	extern const _TCHAR kRoadsignPointGID[];		///< 道路標識ポイントGID
}

/// 道路標示エリア－車線リンク関連
namespace rel_roadmark_area_lane_link{
	using namespace ::sindy::schema::adam_v2_table;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kRoadMarkAreaGID[];			///< 道路標示エリアGID
	extern const _TCHAR kLaneLinkGID[];				///< 車線リンクGID
}

/// 上下線ノード列
namespace nq_updown_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kSourceID[];				///< 提供元ID
	extern const _TCHAR kUpdownLinkGID[];			///< 上下線リンクGID
	extern const _TCHAR kUpdownNodeGID[];			///< 上下線ノードGID
	extern const _TCHAR kWidth[];					///< 幅員
}

/// 車線ノード列
namespace nq_lane_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kSourceID[];				///< 提供元ID
	extern const _TCHAR kLaneLinkGID[];				///< 車線リンクGID
	extern const _TCHAR kLaneNodeGID[];				///< 車線ノードGID
	extern const _TCHAR kLinearElement[];			///< 線形要素属性コード
	extern const _TCHAR kWidth[];					///< 幅員
	extern const _TCHAR kCurvature[];				///< 曲率
	extern const _TCHAR kCurvatureRadius[];			///< 曲率半径
	extern const _TCHAR kCurvatureChangeRate[];		///< 曲率変化率
	extern const _TCHAR kClothoidParameter[];		///< クロソイドパラメータ
	extern const _TCHAR kLongitudinalSlope[];		///< 縦断勾配
	extern const _TCHAR kCrossSlope[];				///< 横断勾配

	/// ADAM線形要素属性コード
	namespace linear_element_code {
		enum ECode {
			kUnset = -1,		///< 未設定
			kStraight = 1,		///< 直線
			kCurve = 2,			///< 単カーブ
			kClothoid = 3,		///< クロソイド
		};
	}
}

/// 区画線ノード列
namespace nq_compart_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kSourceID[];				///< 提供元ID
	extern const _TCHAR kCompartLinkGID[];			///< 区画線リンクGID
	extern const _TCHAR kCompartNodeGID[];			///< 区画線ノードGID
	extern const _TCHAR kWidth[];					///< 幅員
}

/// 道路境界線ノード列
namespace nq_border_node{
	using namespace ::sindy::schema::adam_v2_node_queue;

	extern const _TCHAR kTableName[];				///< テーブル名

	extern const _TCHAR kSourceID[];				///< 提供元ID
	extern const _TCHAR kBorderLinkGID[];			///< 道路境界線リンクGID
	extern const _TCHAR kBorderNodeGID[];			///< 道路境界線ノードGID
}

/// ADAMバージョン
/// @NOTE	安定期には、削除されるので、各種ツール側で本テーブルが存在する前提での機能実装はしないこと
namespace adam_version{
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];				///< テーブル名
}

} // namespace adam_v2

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_AUTODRIVE_H_