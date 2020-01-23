#include "stdafx.h"
#include <boost/preprocessor.hpp>
#include <sindy/schema/autodrive.h>

namespace sindy {

namespace schema {

namespace adam_v2 {
	const _TCHAR kObjectID[] = _T("OBJECTID");	///< オブジェクトIDカラム名

	// 車線リンク
namespace lane_link {
	const _TCHAR kTableName[]			= _T("LANE_LINK");				///< テーブル名
	
	const _TCHAR kLaneGroupID[]			= _T("LANE_GROUP_ID");			///< 車線グループOID
	const _TCHAR kLaneNumber[]			= _T("LANE_NUMBER");			///< 車線番号
	const _TCHAR kLaneCount[]			= _T("LANE_COUNT");				///< 車線数
	const _TCHAR kMaxLegalSpeedC[]		= _T("MAX_LEGAL_SPEED_C");		///< 制限速度コード（最大）
	const _TCHAR kMinLegalSpeedC[]		= _T("MIN_LEGAL_SPEED_C");		///< 制限速度コード（最小）
	const _TCHAR kTravelDirectionC[]	= _T("TRAVEL_DIRECTION_C");		///< 進行方向種別コード
	const _TCHAR kRoadStructureC[]		= _T("ROAD_STRUCTURE_C");		///< 道路構造種別コード
	const _TCHAR kOneWayC[]				= _T("ONEWAY_C");				///< 一方通行種別コード
	const _TCHAR kLeftChangeF[]			= _T("LEFT_CHANGE_F");			///< 左車線変更可能フラグ
	const _TCHAR kRightChangeF[]		= _T("RIGHT_CHANGE_F");			///< 右車線変更可能フラグ
	const _TCHAR kLeftProtrusionF[]		= _T("LEFT_PROTRUSION_F");		///< 左はみ出し可能フラグ
	const _TCHAR kRightProtrusionF[]	= _T("RIGHT_PROTRUSION_F");		///< 右はみ出し可能フラグ
	const _TCHAR kRubbingF[]			= _T("RUBBING_F");				///< 擦り付けフラグ
	const _TCHAR kTunnelF[]				= _T("TUNNEL_F");				///< トンネルフラグ
}

/// 車線ノード
namespace lane_node{
	const _TCHAR kTableName[]			= _T("LANE_NODE");				///< テーブル名

	const _TCHAR kPositionC[]			= _T("POSITION_C");				///< 位置種別コード
	const _TCHAR kStopPointF[]			= _T("STOP_POINT_F");			///< 停止点フラグ
	const _TCHAR kCoveringF[]			= _T("COVERING_F");				///< 覆いものフラグ
}

// 上下線リンク
namespace updown_link {
	const _TCHAR kTableName[]			= _T("UPDOWN_LINK");			///< テーブル名

	const _TCHAR kUpdoenGroupID[]		= _T("UPDOWN_GROUP_ID");		///< 上下線グループOID
	const _TCHAR kLaneCount[]			= _T("LANE_COUNT");				///< 車線数
	const _TCHAR kRightChange[]			= _T("RIGHT_CHANGE");			///< 右側増減車線数
	const _TCHAR kLeftChange[]			= _T("LEFT_CHANGE");			///< 左側増減車線数
	const _TCHAR kMaxLegalSpeedC[]		= _T("MAX_LEGAL_SPEED_C");		///< 制限速度コード（最大）
	const _TCHAR kMinLegalSpeedC[]		= _T("MIN_LEGAL_SPEED_C");		///< 制限速度コード（最小）
	const _TCHAR kTravelDirectionC[]	= _T("TRAVEL_DIRECTION_C");		///< 進行方向種別コード
	const _TCHAR kRoadStructureC[]		= _T("ROAD_STRUCTURE_C");		///< 道路構造種別コード
	const _TCHAR kOneWayC[]				= _T("ONEWAY_C");				///< 一方通行種別コード
	const _TCHAR kRubbingF[]			= _T("RUBBING_F");				///< 擦り付けフラグ
	const _TCHAR kTunnelF[]				= _T("TUNNEL_F");				///< トンネルフラグ
}

/// 上下線ノード
namespace updown_node{
	const _TCHAR kTableName[]			= _T("UPDOWN_NODE");			///< テーブル名

	const _TCHAR kPositionC[]			= _T("POSITION_C");				///< 位置種別コード
	const _TCHAR kStopPointF[]			= _T("STOP_POINT_F");			///< 停止点フラグ
	const _TCHAR kCoveringF[]			= _T("COVERING_F");				///< 覆いものフラグ
}

// 区画線リンク
namespace compart_link {
	const _TCHAR kTableName[]			= _T("COMPART_LINK");			///< テーブル名

	const _TCHAR kCompartLineC[]		= _T("COMPART_LINE_C");			///< 線種別コード
	const _TCHAR kCompartStyleC[]		= _T("COMPART_STYLE_C");		///< 区画線設置様式コード
	const _TCHAR kSlowdownLabelC[]		= _T("SLOWDOWN_LABEL_C");		///< 減速標示コード
	const _TCHAR kInstallationF[]		= _T("INSTALLATION_F");			///< 設置物フラグ
}

/// 区画線ノード
namespace compart_node{
	const _TCHAR kTableName[]			= _T("COMPART_NODE");			///< テーブル名
}

// 道路境界線リンク
namespace border_link {
	const _TCHAR kTableName[]			= _T("BORDER_LINK");			///< テーブル名

	const _TCHAR kImaginaryBorderF[]	= _T("IMAGINARY_BODER_F");		///< 仮想線フラグ
}

/// 道路境界線ノード
namespace border_node{
	const _TCHAR kTableName[]			= _T("BORDER_NODE");			///< テーブル名
}

/// 信号機ポイント
namespace signal_point{
	const _TCHAR kTableName[]			= _T("SIGNAL_POINT");			///< テーブル名

	const _TCHAR kHorizontalAngle[]		= _T("HORIZONTAL_ANGLE");		///< 水平角
	const _TCHAR kGroundHeight[]		= _T("GROUND_HEIGHT");			///< 地上高
	const _TCHAR kHeight[]				= _T("HEIGHT");					///< 縦幅
	const _TCHAR kWidth[]				= _T("WIDTH");					///< 横幅
	const _TCHAR kSignalMoldC[]			= _T("SIGNAL_MOLD_C");			///< 信号機形状コード
	const _TCHAR kLampCount[]			= _T("LAMP_COUNT");				///< 信号灯数
	const _TCHAR kArrowF[]				= _T("ARROW_F");				///< 矢印信号フラグ
	const _TCHAR kWarningF[]			= _T("WARNING_F");				///< 予告信号フラグ
	const _TCHAR kIndependentF[]		= _T("INDEPENDENT_F");			///< 独立信号フラグ
}

/// 横断歩道ライン
namespace crosswalk_line{
	const _TCHAR kTableName[]			= _T("CROSSWALK_LINE");			///< テーブル名

	const _TCHAR kWidth[]				= _T("WIDTH");					///< 幅
}

/// 道路標識ポイント
namespace roadsign_point{
	const _TCHAR kTableName[]			= _T("ROADSIGN_POINT");			///< テーブル名

	const _TCHAR kHorizontalAngle[]		= _T("HORIZONTAL_ANGLE");		///< 水平角
	const _TCHAR kGroundHeight[]		= _T("GROUND_HEIGHT");			///< 地上高
	const _TCHAR kHeight[]				= _T("HEIGHT");					///< 縦幅
	const _TCHAR kWidth[]				= _T("WIDTH");					///< 横幅
	const _TCHAR kRoadSignC[]			= _T("ROADSIGN_C");				///< 道路標識コード
	const _TCHAR kVariableF[]			= _T("VARIABLE_F");				///< 可変フラグ

	/// ADAM道路標識コード
	namespace road_sign_code {
		const _TCHAR kUnset[]		= _T("-1");			///< 未設定
		const _TCHAR k116_4[]		= _T("116の4");		///< (116の4)非常電話
		const _TCHAR k116_6[]		= _T("116の6");		///< (116の6)非常駐車帯
		const _TCHAR k119_D[]		= _T("119-D");		///< (119-D)道路の通称名
		const _TCHAR k201_A[]		= _T("201-A");		///< (201-A)十形道路交差点あり
		const _TCHAR k201_B[]		= _T("201-B");		///< (201-B)ト形道路交差点あり
		const _TCHAR k201_C[]		= _T("201-C");		///< (201-C)Ｔ形道路交差点あり
		const _TCHAR k201_D[]		= _T("201-D");		///< (201-D)Ｙ形道路交差点あり
		const _TCHAR k201_2[]		= _T("201の2");		///< (201の2)ロータリーあり
		const _TCHAR k202[]			= _T("202");		///< (202)右（又は左）方屈曲あり
		const _TCHAR k203[]			= _T("203");		///< (203)右（又は左）方屈折あり
		const _TCHAR k204[]			= _T("204");		///< (204)右（又は左）方背向屈曲あり
		const _TCHAR k205[]			= _T("205");		///< (205)右（又は左）方背向屈折あり
		const _TCHAR k206[]			= _T("206");		///< (206)右（又は左）つづら折りあり
		const _TCHAR k207_A[]		= _T("207-A");		///< (207-A)踏切あり
		const _TCHAR k207_B[]		= _T("207-B");		///< (207-B)踏切あり
		const _TCHAR k208[]			= _T("208");		///< (208)学校、幼稚園、保育所等あり
		const _TCHAR k208_2[]		= _T("208の2");		///< (208の2)信号機あり
		const _TCHAR k209[]			= _T("209");		///< (209)すべりやすい
		const _TCHAR k209_2[]		= _T("209の2");		///< (209の2)落石のおそれあり
		const _TCHAR k209_3[]		= _T("209の3");		///< (209の3)路面凹凸あり
		const _TCHAR k210[]			= _T("210");		///< (210)合流交通あり
		const _TCHAR k211[]			= _T("211");		///< (211)車線数減少
		const _TCHAR k212[]			= _T("212");		///< (212)幅員減少
		const _TCHAR k212_2[]		= _T("212の2");		///< (212の2)二方向交通
		const _TCHAR k212_3[]		= _T("212の3");		///< (212の3)上り急勾配あり
		const _TCHAR k212_4[]		= _T("212の4");		///< (212の4)下り急勾配あり
		const _TCHAR k213[]			= _T("213");		///< (213)道路工事中
		const _TCHAR k214[]			= _T("214");		///< (214)横風注意
		const _TCHAR k214_2[]		= _T("214の2");		///< (214の2)動物が飛び出すおそれあり
		const _TCHAR k215[]			= _T("215");		///< (215)その他の危険
		const _TCHAR k301[]			= _T("301");		///< (301)通行止め
		const _TCHAR k302[]			= _T("302");		///< (302)車両通行止め
		const _TCHAR k303[]			= _T("303");		///< (303)車両進入禁止
		const _TCHAR k304[]			= _T("304");		///< (304)二輪の自動車以外の自動車通行止め
		const _TCHAR k305[]			= _T("305");		///< (305)大型貨物自動車等通行止め
		const _TCHAR k305_2[]		= _T("305の2");		///< (305の2)特定の最大積載量以上の貨物自動車等通行止め
		const _TCHAR k306[]			= _T("306");		///< (306)大型乗用自動車等通行止め
		const _TCHAR k307[]			= _T("307");		///< (307)二輪の自動車・原動機付自転車通行止め
		const _TCHAR k308[]			= _T("308");		///< (308)自転車以外の軽車両通行止め
		const _TCHAR k309[]			= _T("309");		///< (309)自転車通行止め
		const _TCHAR k310[]			= _T("310");		///< (310)車両（組合せ）通行止め
		const _TCHAR k310_2[]		= _T("310の2");		///< (310の2)大型自動二輪車及び普通自動二輪車二人乗り通行禁止
		const _TCHAR k311_A[]		= _T("311-A");		///< (311-A)指定方向外進行禁止
		const _TCHAR k311_B[]		= _T("311-B");		///< (311-B)指定方向外進行禁止
		const _TCHAR k311_C[]		= _T("311-C");		///< (311-C)指定方向外進行禁止
		const _TCHAR k311_D[]		= _T("311-D");		///< (311-D)指定方向外進行禁止
		const _TCHAR k311_E[]		= _T("311-E");		///< (311-E)指定方向外進行禁止
		const _TCHAR k311_F[]		= _T("311-F");		///< (311-F)指定方向外進行禁止
		const _TCHAR k312[]			= _T("312");		///< (312)車両横断禁止
		const _TCHAR k313[]			= _T("313");		///< (313)転回禁止
		const _TCHAR k314[]			= _T("314");		///< (314)追越しのための右側部分はみ出し通行禁止
		const _TCHAR k314_2[]		= _T("314の2");		///< (314の2)追越し禁止
		const _TCHAR k315[]			= _T("315");		///< (315)駐停車禁止
		const _TCHAR k316[]			= _T("316");		///< (316)駐車禁止
		const _TCHAR k317[]			= _T("317");		///< (317)駐車余地
		const _TCHAR k318[]			= _T("318");		///< (318)時間制限駐車区間
		const _TCHAR k319[]			= _T("319");		///< (319)危険物積載車両通行止め
		const _TCHAR k320[]			= _T("320");		///< (320)重量制限
		const _TCHAR k321[]			= _T("321");		///< (321)高さ制限
		const _TCHAR k322[]			= _T("322");		///< (322)最大幅
		const _TCHAR k323[]			= _T("323");		///< (323)最高速度
		const _TCHAR k323_2[]		= _T("323の2");		///< (323の2)特定の種類の車両の最高速度
		const _TCHAR k324[]			= _T("324");		///< (324)最低速度
		const _TCHAR k325[]			= _T("325");		///< (325)自動車専用
		const _TCHAR k327_8[]		= _T("327の8");		///< (327の8)原動機付自転車の右折方法（二段階）
		const _TCHAR k327_9[]		= _T("327の9");		///< (327の9)原動機付自転車の右折方法（小回り）
		const _TCHAR k327_10[]		= _T("327の10");	///< (327の10)環状の交差点における右回り通行
		const _TCHAR k327_11[]		= _T("327の11");	///< (327の11)平行駐車
		const _TCHAR k327_12[]		= _T("327の12");	///< (327の12)直角駐車
		const _TCHAR k327_13[]		= _T("327の13");	///< (327の13)斜め駐車
		const _TCHAR k328[]			= _T("328");		///< (328)警笛鳴らせ
		const _TCHAR k328_2[]		= _T("328の2");		///< (328の2)警笛区間
		const _TCHAR k329[]			= _T("329");		///< (329)徐行
		const _TCHAR k329_2[]		= _T("329の2");		///< (329の2)前方優先道路
		const _TCHAR k330[]			= _T("330");		///< (330)一時停止
		const _TCHAR k331[]			= _T("331");		///< (331)歩行者通行止め
		const _TCHAR k332[]			= _T("332");		///< (332)歩行者横断禁止
	}
}

/// 整備済みエリア
namespace maintenance_area{
	const _TCHAR kTableName[]				= _T("MAINTENANCE_AREA");				///< テーブル名
}

/// 道路標示エリア
namespace roadmark_area{
	const _TCHAR kTableName[]				= _T("ROADMARK_AREA");				///< テーブル名

	const _TCHAR kRoadMarkC[]				= _T("ROADMARK_C");					///< 道路標示コード
	const _TCHAR kDescription[]				= _T("DESCRIPTION");				///< 標示内容（文字）

	/// ADAM道路標示コード
	namespace roadmark_code {
		const _TCHAR kUnset[]				= _T("-1");							///< 未設定
		const _TCHAR k210[]					= _T("210");						///< (210)横断歩道又は時点者横断帯あり
		const _TCHAR k105_30[]				= _T("105(30)");					///< (105)最高速度30km/h
		const _TCHAR k105_40[]				= _T("105(40)");					///< (105)最高速度40km/h
		const _TCHAR k105_50[]				= _T("105(50)");					///< (105)最高速度50km/h
	}
}

/// 仮想線ライン
namespace virtual_compart_line{
	const _TCHAR kTableName[]				= _T("VIRTUAL_COMPART_LINE");				///< テーブル名
}

/// 覆い物エリア
namespace covering_area{
	const _TCHAR kTableName[]				= _T("COVERING_AREA");				///< テーブル名
}

/// 道路リンク－上下線リンク関連
namespace rel_road_link_updown_link{
	const _TCHAR kTableName[]				= _T("REL_ROAD_LINK_UPDOWN_LINK");		///< テーブル名

	const _TCHAR kRoadLinkID[]				= _T("ROAD_LINK_ID");					///< 道路リンクOID
	const _TCHAR kUpdownLinkGID[]			= _T("UPDOWN_LINK_GID");				///< 上下線リンクGID
	const _TCHAR kRoadLinkDirectionC[]		= _T("ROAD_LINK_DIRECTION_C");			///< 道路リンク方向コード
}

/// 上下線ペアリンク
namespace pair_updown_link{
	const _TCHAR kTableName[]				= _T("PAIR_UPDOWN_LINK");				///< テーブル名

	const _TCHAR kUpdownLinkGID1[]			= _T("UPDOWN_LINK_GID1");				///< 上下線リンクGID1
	const _TCHAR kUpdownLinkGID2[]			= _T("UPDOWN_LINK_GID2");				///< 上下線リンクGID2
}

/// 上下線リンクグループ
namespace updown_link_group{
	const _TCHAR kTableName[]				= _T("UPDOWN_LINK_GROUP");				///< テーブル名
}

/// 上下線リンク－車線リンク関連
namespace rel_updown_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_UPDOWN_LINK_LANE_LINK");		///< テーブル名

	const _TCHAR kUpdownLinkGID[]			= _T("UPDOWN_LINK_GID");				///< 上下線リンクGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< 車線リンクGID
}

/// 道路リンク－車線リンク関連
namespace rel_road_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_ROAD_LINK_LANE_LINK");		///< テーブル名

	const _TCHAR kRoadLinkID[]				= _T("ROAD_LINK_ID");					///< 道路リンクOID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< 車線リンクGID
	const _TCHAR kRoadLinkDirectionC[]		= _T("ROAD_LINK_DIRECTION_C");			///< 道路リンク方向コード
}

/// 車線リンクグループ
namespace lane_link_group{
	const _TCHAR kTableName[]				= _T("LANE_LINK_GROUP");				///< テーブル名
	const _TCHAR kUpDownLinkGroupID[]		= _T("UPDOWN_LINK_GROUP_ID");			///< 上下線リンクグループOID
}

/// 区画線リンク－車線リンク関連
namespace rel_compart_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_COMPART_LINK_LANE_LINK");		///< テーブル名

	const _TCHAR kCompartLinkGID[]			= _T("COMPART_LINK_GID");				///< 区画線リンクGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< 車線リンクGID
	const _TCHAR kCompartLinkDirectionC[]	= _T("COMPART_LINK_DIRECTION_C");		///< 区画線リンク方向コード
	const _TCHAR kLRC[]						= _T("LR_C");							///< 左右判定コード
	const _TCHAR kSequence[]				= _T("SEQUENCE");						///< 順番
}

/// 道路境界線リンク－車線リンク関連
namespace rel_border_link_lane_link{
	const _TCHAR kTableName[]				= _T("REL_BORDER_LINK_LANE_LINK");		///< テーブル名

	const _TCHAR kBorderLinkGID[]			= _T("BORDER_LINK_GID");				///< 道路境界線リンクGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< 車線リンクGID
	const _TCHAR kBorderLinkDirectionC[]	= _T("BORDER_LINK_DIRECTION_C");		///< 道路境界線リンク方向コード
	const _TCHAR kLRC[]						= _T("LR_C");							///< 左右判定コード
	const _TCHAR kSequence[]				= _T("SEQUENCE");						///< 順番
}

/// 上下線ノード－信号機ポイント関連
namespace rel_updown_node_signal_point{
	const _TCHAR kTableName[]				= _T("REL_UPDOWN_NODE_SIGNAL_POINT");	///< テーブル名

	const _TCHAR kUpdownNodeGID[]			= _T("UPDOWN_NODE_GID");				///< 上下線ノードGID
	const _TCHAR kSignalPointGID[]			= _T("SIGNAL_POINT_GID");				///< 信号機ポイントGID
}

/// 車線ノード－信号機ポイント関連
namespace rel_lane_node_signal_point{
	const _TCHAR kTableName[]				= _T("REL_LANE_NODE_SIGNAL_POINT");		///< テーブル名

	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< 車線ノードGID
	const _TCHAR kSignalPointGID[]			= _T("SIGNAL_POINT_GID");				///< 信号機ポイントGID
}

/// 車線ノード－横断歩道ライン関連
namespace rel_lane_node_crosswalk_line{
	const _TCHAR kTableName[]				= _T("REL_LANE_NODE_CROSSWALK_LINE");	///< テーブル名

	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< 車線ノードGID
	const _TCHAR kCrosswalkLineGID[]		= _T("CROSSWALK_LINE_GID");				///< 横断歩道ラインGID
}

/// 車線ノード－道路標識ポイント関連
namespace rel_lane_node_roadsign_point{
	const _TCHAR kTableName[]				= _T("REL_LANE_NODE_ROADSIGN_POINT");	///< テーブル名

	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< 車線ノードGID
	const _TCHAR kRoadsignPointGID[]		= _T("ROADSIGN_POINT_GID");				///< 道路標識ポイントGID
}

/// 道路標示エリア－車線リンク関連
namespace rel_roadmark_area_lane_link{
	const _TCHAR kTableName[]				= _T("REL_ROADMARK_AREA_LANE_LINK");	///< テーブル名

	const _TCHAR kRoadMarkAreaGID[]			= _T("ROADMARK_AREA_GID");				///< 道路標示エリアGID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< 車線リンクGID
}

/// 上下線ノード列
namespace nq_updown_node{
	const _TCHAR kTableName[]				= _T("NQ_UPDOWN_NODE");					///< テーブル名

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< 提供元ID
	const _TCHAR kUpdownLinkGID[]			= _T("UPDOWN_LINK_GID");				///< 上下線リンクGID
	const _TCHAR kUpdownNodeGID[]			= _T("UPDOWN_NODE_GID");				///< 上下線ノードGID
	const _TCHAR kWidth[]					= _T("WIDTH");							///< 幅員
}

/// 車線ノード列
namespace nq_lane_node{
	const _TCHAR kTableName[]				= _T("NQ_LANE_NODE");					///< テーブル名

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< 提供元ID
	const _TCHAR kLaneLinkGID[]				= _T("LANE_LINK_GID");					///< 車線リンクGID
	const _TCHAR kLaneNodeGID[]				= _T("LANE_NODE_GID");					///< 車線ノードGID
	const _TCHAR kLinearElement[]			= _T("LINEAR_ELEMENT_C");				///< 線形要素属性コード
	const _TCHAR kWidth[]					= _T("WIDTH");							///< 幅員
	const _TCHAR kCurvature[]				= _T("CURVATURE");						///< 曲率
	const _TCHAR kCurvatureRadius[]			= _T("CURVATURE_RADIUS");				///< 曲率半径
	const _TCHAR kCurvatureChangeRate[]		= _T("CURVATURE_CHANGE_RATE");			///< 曲率変化率
	const _TCHAR kClothoidParameter[]		= _T("CLOTHOID_PARAMETER");				///< クロソイドパラメータ
	const _TCHAR kLongitudinalSlope[]		= _T("LONGITUDINAL_SLOPE");				///< 縦断勾配
	const _TCHAR kCrossSlope[]				= _T("CROSS_SLOPE");					///< 横断勾配
}

/// 区画線ノード列
namespace nq_compart_node{
	const _TCHAR kTableName[]				= _T("NQ_COMPART_NODE");				///< テーブル名

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< 提供元ID
	const _TCHAR kCompartLinkGID[]			= _T("COMPART_LINK_GID");				///< 区画線リンクGID
	const _TCHAR kCompartNodeGID[]			= _T("COMPART_NODE_GID");				///< 区画線ノードGID
	const _TCHAR kWidth[]					= _T("WIDTH");							///< 幅員
}

/// 道路境界線ノード列
namespace nq_border_node{
	const _TCHAR kTableName[]				= _T("NQ_BORDER_NODE");					///< テーブル名

	const _TCHAR kSourceID[]				= _T("SOURCEID");						///< 提供元ID
	const _TCHAR kBorderLinkGID[]			= _T("BORDER_LINK_GID");				///< 道路境界線リンクGID
	const _TCHAR kBorderNodeGID[]			= _T("BORDER_NODE_GID");				///< 道路境界線ノードGID
}

/// ADAMバージョン
namespace adam_version{
	const _TCHAR kTableName[]				= _T("ADAM_VERSION_216");					///< テーブル名
}

} //namespace adam_v2

} // namespace schema

} // namespace sindy