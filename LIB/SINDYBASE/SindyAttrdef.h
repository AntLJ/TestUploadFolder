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

/**
 *	SindyAttrdef.h : 属性定義
 */

#ifndef	___SINDY_ATTR_DEF_H___
#define	___SINDY_ATTR_DEF_H___

#include <string>
#include <atlbase.h>

namespace sindy
{
	namespace layer
	{
		extern std::string ROAD_LINK;
		extern std::string ROAD_NODE;
		extern std::string SEGMENT_ATTR;
		extern std::string GRAD;
		extern std::string PARKING_SHAPE;
		extern std::string ACCESS_POINT;
		extern std::string FACIL_INFO_POINT;
		extern std::string HIGHWAY_NODE;
		extern std::string HIGHWAY_TEXT;
		extern std::string TIME_NOPASSAGE;
		extern std::string TIME_ONEWAY;
		extern std::string INF_BYWAY;
		extern std::string INF_DIRGUIDE;
		extern std::string INF_GUIDE;
		extern std::string INF_LANE;
		extern std::string INF_INTERNAVI;
		extern std::string INF_IPCVICS;
		extern std::string INF_ROUTE;
		extern std::string INF_VICS;
		extern std::string INF_TURNREG;
		extern std::string INF_UTURN;
		extern std::string INF_INTERSECION;
		extern std::string LQ_BYWAY;
		extern std::string LQ_DIRGUIDE;
		extern std::string LQ_GUIDE;
		extern std::string LQ_LANE;
		extern std::string LQ_INTERNAVI;
		extern std::string LQ_IPCVICS;
		extern std::string LQ_ROUTE;
		extern std::string LQ_VICS;
		extern std::string LQ_TURNREG;
		extern std::string LQ_UTURN;
		extern std::string LQ_INTERSECTION;
		extern std::string BASE_ADMIN;
		extern std::string BASE_MESH;
		extern std::string MIDDLE_MESH;
		extern std::string CITY_MESH;
		extern std::string ROAD_CODE_LIST;
		extern std::string DUPLI_LINK;
		extern std::string MIDDLE_ANNOTATION;
		extern std::string BASE_ANNOTATION;
		extern std::string CITY_ANNOTATION;
		extern std::string ACCESS_STORE;
		extern std::string ETC_TOLLGATE;
	};
	namespace object
	{
		extern std::string OBJECTID;
	};
	namespace feature
	{
		extern std::string SHAPE;
	};
	namespace ipc_table
	{
		extern std::string OPERATOR;
		extern std::string PURPOSE_C;
		extern std::string MODIFYDATE;
		extern std::string UPDATE_TYPE;
		extern std::string PROGMODIFYDATE;
		extern std::string MODIFYPROGNAME;
		extern std::string USERCLAIM_F;
		extern std::string SOURCE;
		extern std::string TMP_MESHCODE;
		extern std::string TMP_PRIMID;
	};
	namespace ipc_feature
	{
		using namespace ipc_table;
		using namespace feature;
	};
	namespace meshcode_cls
	{
		extern std::string MESHCODE;
	};
	namespace field_survey
	{
		extern std::string FIELDSURVEY_F;
		extern std::string WORK_YEAR;
	};
	namespace road_link
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		extern std::string TMPFROM_NODE;
		extern std::string TMPTO_NODE;
		extern std::string FROM_NODE_ID;
		extern std::string TO_NODE_ID;
		extern std::string ROADCLASS_C;
		extern std::string NAVICLASS_C;
		extern std::string MAIN_LINKCLASS_C;
		extern std::string SUB_LINKCLASS_C;
		extern std::string ADMIN_CODE;
		extern std::string ROADMANAGER_C;
		extern std::string ROADWIDTH_C;
		extern std::string WIDEAREA_F;
		extern std::string UPDOWNCLASS_C;
		extern std::string BYPASS_F;
		extern std::string CARONLY_F;
		extern std::string ISLAND_F;
		extern std::string ROAD_NO;
		extern std::string ROAD_CODE;
		extern std::string NOPASSAGE_C;
		extern std::string NOPASSAGE_RF;
		extern std::string ONEWAY_C;
		extern std::string ONEWAY_RF;
		extern std::string TRIP_SPEED;
		extern std::string LIMITSPEED_C;
		extern std::string KUBUN_C;
		extern std::string LANE_COUNT;
		extern std::string RIGHT_CHANGE;
		extern std::string LEFT_CHANGE;
		extern std::string LANEVALID_F;
		extern std::string DUPLINK_RF;
		extern std::string TURNREG_LQRF;
		extern std::string DIRGUIDE_LQRF;
		extern std::string GUIDE_LQRF;
		extern std::string LANE_LQRF;
		extern std::string VICS_LQRF;
		extern std::string BYWAY_LQRF;
		extern std::string ROUTE_LQRF;
		extern std::string UTURN_LQRF;
		extern std::string INTERNAVI_LQRF;
		extern std::string INTERSECTION_LQRF;
		extern std::string IPCVICS_LQRF;
		extern std::string PARKINGCLASS_C;
		extern std::string PARKINGLINKCLASS_C;
		extern std::string FLOOR_LEVEL;
		extern std::string MIDDLEFLOOR_LEVEL;
		extern std::string ALLEYMAINTE_C;

		extern std::string ATTR[];

		/// 道路種別コード
		enum roadclass_c
		{
			roadclass_kokudo		= 1,		// 一般国道
			roadclass_prefmlocal	= 2,		// 主要地方道(都府県)
			roadclass_citymlocal	= 3,		// 主要地方道(指定市)
			roadclass_prefroad		= 4,		// 一般都道府県道
			roadclass_cityroad		= 5,		// 指定市の一般市道
			roadclass_otherroad		= 6,		// その他の道路
			roadclass_nodisproad	= 7,		// その他の道路(格下げその他道) --- 表示用
			roadclass_detail1		= 8,		// 細街路L1
			roadclass_detail2		= 9,		// 細街路L2
			roadclass_detail3		= 10,		// 細街路L3
			roadclass_highway2		= 11,		// 高速自動車道路
			roadclass_highway		= 101,		// 高速自動車道路(有料)
			roadclass_cityhighway	= 102,		// 都市高速道路(有料)
			roadclass_kokudo2		= 103,		// 一般国道(有料)
			roadclass_prefmlocal2	= 104,		// 主要地方道(都府県)(有料)
			roadclass_citymlocal2	= 105,		// 主要地方道(指定市)(有料)
			roadclass_prefroad2		= 106,		// 一般都道府県道(有料)
			roadclass_cityroad2		= 107,		// 指定市の一般市道(有料)
			roadclass_otherroad2	= 108,		// その他の道路(有料)
			roadclass_ferry			= 201,		// フェリー航路(非表示部)
			roadclass_ferry_s2		= 202,		// フェリー航路(S2破線表示部)
			roadclass_ferry_s3		= 203,		// フェリー航路(S3破線表示部)
			roadclass_ferry_s4		= 204,		// フェリー航路(S4破線表示部)
			roadclass_virtual		= 301,		// 仮想リンク
			roadclass_garden		= 302,		// 庭園路
			roadclass_bridge		= 303,		// ブリッジリンク
			roadclass_entrance		= 304,		// 施設出入口リンク
			roadclass_facil			= 305,		// 施設内リンク
			roadclass_parking		= 401,		// 駐車場構内道路
			roadclass_match			= 402,		// マッチング用リンク(PEC用)
			roadclass_match2		= 501		// マッチング用リンク(BT用)
		};

		/// 経路種別コード
		enum naviclass_c
		{
			naviclass_kokudo		= 1,		// 一般国道
			naviclass_prefmlocal	= 2,		// 主要地方道(都府県)
			naviclass_citymlocal	= 3,		// 主要地方道(指定市)
			naviclass_prefroad		= 4,		// 一般都道府県道
			naviclass_cityroad		= 5,		// 指定市の一般市道
			naviclass_otherroad		= 6,		// その他の道路
			naviclass_highway		= 101,		// 高速自動車道路
			naviclass_cityhighway	= 102,		// 都市高速道路
			naviclass_kokudo2		= 103,		// 一般国道(有料)
			naviclass_prefmlocal2	= 104,		// 主要地方道(都府県)(有料)
			naviclass_citymlocal2	= 105,		// 主要地方道(指定市)(有料)
			naviclass_prefroad2		= 106,		// 一般都道府県道(有料)
			naviclass_cityroad2		= 107,		// 指定市の一般市道(有料)
			naviclass_otherroad2	= 108,		// その他の道路(有料)
			naviclass_ferry			= 201,		// フェリー航路
			naviclass_rampway		= 403,		// 接続国道
			naviclass_notnavi		= 0			// 経路対象外
		};

		/// リンク種別
		enum linkclass_c
		{
			linkclass_unknown		= 0,		// 未調査・作業中
			linkclass_hibunri		= 1,		// 本線(上下非分離)リンク
			linkclass_bunri			= 2,		// 本線(上下分離)リンク
			linkclass_junction		= 3,		// 連結路(本線間の渡り線)
			linkclass_crossing		= 4,		// 交差点内リンク
			linkclass_ramp			= 5,		// 連結路(ランプ)リンク
			linkclass_sidelink		= 6,		// 本線と同一路線の側道リンク
			linkclass_salink		= 7,		// SA等側線リンク
			linkclass_bicycle		= 8,		// 自転車道
			linkclass_connect		= 9,		// 本線側道接続リンク
			linkclass_roundabout	= 101		// 環状交差点リンク
		};

		/// 道路管理者
		enum roadmanager_c
		{
			roadmanager_unknown		= 0,		// 未調査・作業中
			roadmanager_jh			= 1,		// 日本道路公団
			roadmanager_public3		= 2,		// 道路3公社
			roadmanager_public		= 3,		// 道路公社
			roadmanager_national	= 4,		// 国
			roadmanager_pref		= 5,		// 都道府県
			roadmanager_city		= 6,		// 指定市
			roadmanager_town		= 7,		// その他の市町村
			roadmanager_other		= 8			// その他の管理者
		};

		/// 道路幅員区分
		enum roadwidth_c
		{
			roadwidth_unknown		= 0,		// 未調査・作業中
			roadwidth_over13m		= 1,		// 幅員13.0m以上
			roadwidth_under13m		= 2,		// 幅員5.5m～13.0m未満
			roadwidth_over3m		= 3,		// 幅員3.0m～5.5m未満
			roadwidth_under3m		= 4			// 幅員3.0m未満
		};

		/// ハイウェイ路線コード（旧 上下線属性）
		enum updown_c
		{
			updownclass_nothwy		= 0,		// 対象外
			updownclass_nodir		= 1,		// HWY(上下なし)
			updownclass_up			= 2,		// HWY上り(内・西)
			updownclass_down		= 3,		// HWY下り(外・東)
			updownclass_common		= 4,		// HWY上下線共通
			updownclass_etcramp		= 5,		// ETC専用ランプリンク
			updownclass_etcaccess	= 6			// ETC接続一般道リンク
		};

		/// 通行禁止
		enum nopassage_c
		{
			nopassage_passable		= 0,		// 通行禁止なし
			nopassage_nocondition	= 1,		// 通行禁止(条件なし)
			nopassage_withcondition	= 2,		// 通行禁止(条件あり)
			badpassage				= 3,		// 通行不可
			relation_nocondition	= 4,		// 通行禁止(関係者以外)(条件なし)
			relation_withcondition	= 5			// 通行禁止(関係者以外)(条件あり)
		};

		/// 一方通行
		enum oneway_c
		{
			oneway_no				= 0,		// 一方通行なし
			oneway_order			= 1,		// 一方通行(正方向・条件なし)
			oneway_reverse			= 2,		// 一方通行(逆方向・条件なし)
			oneway_ordwithcond		= 3,		// 一方通行(正方向・条件あり)
			oneway_revwithcond		= 4,		// 一方通行(逆方向・条件あり)
			oneway_both				= 5			// 一方通行(正逆切り替えあり)
		};

		/// 規制速度
		enum limitspeed_c
		{
			limitspeed_unknown		= 0,		// 未調査
			limitspeed_under30km	= 1,		// 30km/h未満
			limitspeed_40km			= 2,		// 40km/h
			limitspeed_50km			= 3,		// 50km/h
			limitspeed_60km			= 4,		// 60km/h
			limitspeed_70km			= 5,		// 70km/h
			limitspeed_80km			= 6,		// 80km/h
			limitspeed_100km		= 7			// 100km/h
		};

		/// 主従区分
		enum kubun_c
		{
			kubun_unknown			= 0,		// 未調査
			kubun_main				= 1,		// 主道路
			kubun_sub				= 2			// 従道路
		};

		/// 駐車場種別
		enum parkingclass_c
		{
			parkingclass_unknown	= 0,		// 未調査
			parkingclass_solid		= 1,		// 立体駐車場
			parkingclass_under		= 2,		// 地下駐車場
			parkingclass_both		= 3,		// 立体かつ地下駐車場
			parkingclass_tower		= 4,		// タワーパーキング
			parkingclass_auto		= 5,		// 自動回転タワーパーキング
			parkingclass_flat		= 6			// 平面駐車場
		};

		/// 駐車場リンク種別
		enum parkinglinkclass_c
		{
			parkinglinkclass_unknown	= 0,	// 未調査
			parkinglinkclass_enter		= 1,	// 入口リンク
			parkinglinkclass_exit		= 2,	// 出口リンク
			parkinglinkclass_entrance	= 3,	// 出入口リンク
			parkinglinkclass_premises	= 4,	// 構内道路リンク
			parkinglinkclass_junction	= 5,	// レベル間の渡りリンク
		};

		/// 細街路作業年度フラグ
		enum alley_mainte
		{
			alleymainte_notavailable	= 0,		// 該当なし
			alleymainte_1998			= 1,		// 1998年度
			alleymainte_1999			= 2,		// 1999年度
			alleymainte_2000spring		= 3,		// 2000年度(春)
			alleymainte_2000autumn		= 4,		// 2000年度(秋)
			alleymainte_2001			= 5,		// 2001年度
			alleymainte_2002			= 6,		// 2002年度
			alleymainte_2003			= 7,		// 2003年度
			alleymainte_2004			= 8,		// 2004年度
			alleymainte_2005			= 9,		// 2005年度
			alleymainte_2006			= 10,		// 2006年度
			alleymainte_2007			= 11		// 2007年度
		};
	};
	namespace road_node
	{
		using namespace object;
		using namespace ipc_feature;
		extern std::string NODECLASS_C;
		extern std::string SIGNAL_C;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;
		extern std::string HEIGHT_C;
		extern std::string HEIGHT;
		extern std::string PARKING_ID;

		extern std::string ATTR[];

		/// ノード種別
		enum nodeclass_c
		{
			nodeclass_crossing		= 1,		// 交差点ノード
			nodeclass_term			= 2,		// 端点ノード
			nodeclass_attrchange	= 3,		// 属性変化点ノード
			nodeclass_attrnode		= 4,		// 属性付きノード
			nodeclass_peclink		= 5,		// PECリンク接続ノード
			nodeclass_nodearray		= 6,		// ノード列基点ノード
			nodeclass_tollgate		= 7,		// 料金所ノード
			nodeclass_highway		= 8,		// ハイウェイ関係ノード
			nodeclass_meshedge		= 9,		// 2次メッシュ区画辺上ノード
			nodeclass_notsamelink	= 10,		// 同一リンクID防止ノード
			nodeclass_pecnode		= 11,		// PECノード
			nodeclass_pecnode2		= 12,		// 2次メッシュ区画辺上PECノード
			nodeclass_dummyfacil	= 13		// ダミー施設ノード
		};

		/// 信号機有無
		enum signal_c
		{
			signal_no				= 0,		// 信号機なし
			signal_exist			= 1,		// 信号機あり
			signal_unknown			= 2			// 未調査
		};

		/// 高さフラグ
		enum height_c
		{
			height_no				= 0,		// 相対高さなし
			height_valid			= 1,		// 相対高さ有効
			height_auto				= 2			// 相対高さ自動付与
		};
	};
	namespace link_info
	{
		extern std::string LINK_ID;
	};
	namespace segment_attr
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		using namespace link_info;
		extern std::string LINKATTR_C;
		extern std::string UNDER_OVER;

		extern std::string ATTR[];

		/// リンク内属性コード
		enum linkattr_c
		{
			linkattr_tunnel		= 2,		// トンネル
			linkattr_underpass	= 5,		// アンダーパス
			linkattr_bridge		= 1,		// 橋・高架
			linkattr_crossing	= 4,		// 踏切
			linkattr_doumon		= 3,		// 洞門等
			linkattr_underover	= 6,		// 上下関係
			linkattr_dirtroad	= 7,		// 未舗装
			linkattr_flooding	= 8,		// 冠水注意地点
			linkattr_tunnel_square	= 100,	// トンネル（四角）
			linkattr_tunnel_circle	= 101,	// トンネル（丸）
			linkattr_crossing_kamaboko	= 102	// 踏切（かまぼこ型）
		};
	};
	namespace grad
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		using namespace link_info;
		extern std::string GRAD_VALUE;

		extern std::string ATTR[];
	};
	namespace parking_shape
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace field_survey;
		extern std::string PARKING_ORGID;
		extern std::string PARKINGCLASS_C;
		extern std::string NAME_KANJI;

		extern std::string ATTR[];

		/// 駐車場種別
		enum parkingclass_c
		{
			parkingclass_unknown	= 0,		// 未調査
			parkingclass_solid		= 1,		// 立体駐車場
			parkingclass_under		= 2,		// 地下駐車場
			parkingclass_both		= 3,		// 立体かつ地下駐車場
			parkingclass_tower		= 4,		// タワーパーキング
			parkingclass_auto		= 5,		// 自動回転タワーパーキング
			parkingclass_flat		= 6			// 平面駐車場
		};
	};
	namespace hwy_facil_code
	{
		extern std::string ROAD_CODE;
		extern std::string ROAD_SEQ;
	};
	namespace hwy_mode
	{
		extern std::string HWYMODE_F;
	};
	namespace inout
	{
		extern std::string INOUT_C;
		/// 出入口コード
		enum inout_c
		{
			inout_no			= 0,		// 区別なし
			inout_enter			= 1,		// 入口・合流
			inout_exit			= 2			// 出口・分岐
		};
	};
	namespace direction
	{
		extern std::string DIRECTION_C;

		/// 方向コード
		enum direction_c
		{
			direction_no		= 0,		// 方向なし
			direction_up		= 1,		// 上り・内回り・西行き
			direction_down		= 2,		// 下り・外回り・東行き
			direction_common	= 3			// 双方向共通
		};
	};
	namespace access_point
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace hwy_facil_code;
		using namespace hwy_mode;
		using namespace direction;
		using namespace inout;
		extern std::string DIRECTION_KANJI;
		extern std::string DIRECTION_YOMI;
		extern std::string GS_C;
		extern std::string TOILET_F;
		extern std::string HANDICAP_F;
		extern std::string RESTAURANT_F;
		extern std::string CAFETERIA_F;
		extern std::string SOUVENIR_F;
		extern std::string RESTING_F;
		extern std::string NAP_F;
		extern std::string INFODESK_F;
		extern std::string INFORMATION_F;
		extern std::string SHOWER_F;
		extern std::string LAUNDRY_F;
		extern std::string SPA_F;
		extern std::string FAX_F;
		extern std::string MAILBOX_F;
		extern std::string CASHSERVICE_F;
		extern std::string HWYOASIS_F;
		extern std::string CARWASH_F;
		extern std::string GS_F;
		extern std::string MULTIPLESTORE_F;
		extern std::string DOGRUN_F;

		extern std::string ATTR[];
		extern std::string SAPAINFO[];
		extern std::string NEWSAPAINFO[];
		extern std::string SAPAINFO3[];

		/// ガソリンスタンド
		enum gs_c
		{
			gs_no				= 0,		// ガソリンスタンドなし
			gs_other			= 1,		// その他・未定
			gs_esso				= 2,		// エッソ
			gs_mobil			= 3,		// モービル
			gs_kygnus			= 4,		// キグナス
			gs_cosmo			= 5,		// コスモ
			gs_shell			= 6,		// シェル
			gs_jomo				= 7,		// ジョモ
			gs_general			= 8,		// ゼネラル
			gs_kyushu			= 9,		// 九州
			gs_idemitsu			= 10,		// 出光
			gs_taiyo			= 11,		// 太陽
			gs_eneos			= 12		// ENEOS
		};
	};
	namespace facil_info_point
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace hwy_facil_code;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;
		extern std::string DUMMYNAME_F;
		extern std::string FACILCLASS_C;
		extern std::string SEARCHCLASS_C;
		extern std::string AUTOPOSITION_F;
		extern std::string DUMMYFACIL_F;

		extern std::string ATTR[];

		/// 施設種別コード
		enum facilclass_c
		{
			facilclass_sa		= 1,		// SA
			facilclass_pa		= 2,		// PA
			facilclass_jct		= 3,		// JCT
			facilclass_ramp		= 4,		// ランプ
			facilclass_ic		= 5,		// IC
			facilclass_tollgate	= 6,		// 料金所
			facilclass_smartic	= 7			// スマートIC
		};

		/// 検索用施設種別コード
		enum searchclass_c
		{
			searchclass_not_for_search	= 0,	// 対象外
			searchclass_sa				= 1,	// SA
			searchclass_pa				= 2,	// PA
			searchclass_jct				= 3,	// JCT
			searchclass_ramp			= 4,	// ランプ
			searchclass_ic				= 5,	// IC
			searchclass_tollgate		= 6,	// 料金所
			searchclass_smartic			= 7		// スマートIC
		};

	};
	namespace highway_node
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace hwy_facil_code;
		using namespace hwy_mode;
		using namespace inout;
		using namespace direction;
		extern std::string TOLLCLASS_C;

		extern std::string NOETC_CAR_F;
		extern std::string TOLLFUNC_C;
		extern std::string DUMMYTOLL_F;

		extern std::string ETC_FACIL_C;
		extern std::string ETC_TOLLFUNC_C;
		extern std::string ETC_NOGUIDETOLL_F;
		extern std::string TOLLGATE_ID;

		extern std::string GUIDE_F;
		extern std::string ROAD_SEQ2;
		extern std::string APPROACHNODE_ID;
		extern std::string FLOWNODE_ID;
		extern std::string TOLLGATE_LANE;

		extern std::string ATTR[];

		/// 料金所種別
		enum tollclass_c
		{
			tollclass_other		= 0,		// 分岐合流ノード（旧・料金所以外）
			tollclass_honsen	= 1,		// 本線上料金所
			tollclass_ramp		= 2,		// ランプ上料金所
			tollclass_barrier	= 3,		// バリア
			tollclass_inout		= 4			// 出入口ノード
		};

		/// 料金所機能
		enum tollfunc_c
		{
			tollfunc_no			= 0,		// 機能なし
			tollfunc_issue		= 1,		// 発券機能
			tollfunc_pay		= 2,		// 精算機能
			tollfunc_single		= 3,		// 単独料金所
			tollfunc_singleissue	= 4,	// 単独料金所＋発券機能
			tollfunc_singlepay	= 5,		// 単独料金所＋精算機能
			tollfunc_check		= 6,		// Uターン用検札機能
			tollfunc_issue2		= 7,		// 単独料金無効発券機能
			tollfunc_payissue	= 8			// 精算機能＋発券機能
		};

		/// ETC施設コード
		enum etc_facil_c
		{
			etc_facil_no_etc_facil	= 0,	// ETC非対応
			etc_facil_gate			= 1,	// ゲート
			etc_facil_antenna		= 2,	// アンテナ
		};

		/// 料金所レーン情報
		enum tollgate_lane
		{
			tollgate_lane_unusable			= 0,	///< 使用不可
			tollgate_lane_general_lane		= 1,	///< 一般レーン
			tollgate_lane_etc_lane			= 2,	///< ETC専用レーン
			tollgate_lane_mixed_lane		= 3,	///< 混在レーン
			tollgate_lane_switching_lane	= 4,	///< 切り替えレーン
			tollgate_lane_unknown_lane		= 5,	///< 不明レーン
		};
	};
	namespace highway_text
	{
		using namespace object;
		using namespace ipc_table;
		using namespace hwy_facil_code;
		using namespace inout;
		extern std::string POINT_ID;
		extern std::string ORDER_SEQ;
		extern std::string UPDOWN_C;
		extern std::string NAME_KANJI;
		extern std::string PREV_ID;
		extern std::string NEXT_ID;
		extern std::string PARENT_ID;
		extern std::string FORE_COUNT;
		extern std::string FORE1_ID;
		extern std::string FORE2_ID;
		extern std::string FORE3_ID;
		extern std::string FORE4_ID;
		extern std::string FORE1_DIR_C;
		extern std::string FORE2_DIR_C;
		extern std::string FORE3_DIR_C;
		extern std::string FORE4_DIR_C;
		extern std::string BACK_COUNT;
		extern std::string BACK1_ID;
		extern std::string BACK2_ID;
		extern std::string BACK3_ID;
		extern std::string BACK4_ID;
		extern std::string BACK1_DIR_C;
		extern std::string BACK2_DIR_C;
		extern std::string BACK3_DIR_C;
		extern std::string BACK4_DIR_C;

		extern std::string ATTR[];
		extern std::string FOREID[];
		extern std::string FORDIR[];
		extern std::string BACKID[];
		extern std::string BACKDIR[];

		/// 上下線属性
		enum updown_c
		{
			updown_up		= 1,		// 上り・内回り・西行き
			updown_down		= 2			// 下り・外回り・東行き
		};

		/// 分岐方向コード
		enum dir_c
		{
			direct_dir		= 0,		// 直進方向
			right_dir		= 1,		// 右方向
			left_dir		= 2,		// 左方向
			unknown			= 3			// 不定
		};
			

	};
	namespace base_admin
	{
		using namespace object;
		using namespace ipc_feature;
		extern std::string CITYCODE;

		extern std::string ATTR[];
	};
	namespace base_mesh
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace meshcode_cls;

		extern std::string ATTR[];
	};
	namespace middle_mesh
	{
		using namespace object;
		using namespace ipc_feature;
		extern std::string MIDDLEMESHCODE;
	};
	namespace city_mesh
	{
		using namespace object;
		extern std::string CITYMESHCODE;
		extern std::string CITYNAME;
		extern std::string CREATE_YEAR;
		extern std::string UPDATE_YEAR;
		extern std::string COMPLETE_F;
		extern std::string SEA_F;
		extern std::string ZUKA_F;
		extern std::string RASTER;
		extern std::string CREATE_RASTER_YEAR;
		extern std::string CURRENT_ADJ_YEAR;
		extern std::string PHOTO_SHOT_DATE;
		extern std::string FIELD_RESEARCH_YEAR;
		extern std::string SHA_BGWORK_YEAR;
		extern std::string SHA_ANNOWORK_YEAR;
		extern std::string CREATE_ANNO_MS_DATE;
		extern std::string ADDR_MAINTE_YEAR;
		extern std::string JMAP_PUBLICATION_YEAR;
		extern std::string MAPCUBE_WORK_YEAR;
		extern std::string PDM_USE_YEAR;
	};
	namespace dupli_link
	{
		using namespace object;
		using namespace ipc_table;
		using namespace meshcode_cls;
		using namespace link_info;
		extern std::string KUBUN_C;
		extern std::string DUPCLASS_C;
		extern std::string ROAD_NO;
		extern std::string ROAD_CODE;

		extern std::string ATTR[];

		/// 主従区分
		enum kubun_c
		{
			kubun_unknown			= 0,		// 未調査
			kubun_main				= 1,		// 主道路
			kubun_sub				= 2			// 従道路
		};

		/// 路線IDか道路番号
		enum dupclass_c
		{
			dupclass_kokudo			= 1,		// 国道番号
			dupclass_pref			= 2,		// 都道府県道番号
			dupclass_nameid			= 3			// 路線名称ID
		};
	};

	namespace linkdir
	{
		extern std::string LINKDIR_C;
		enum linkdir_c
		{
			linkdir_fore		= 1,		// 正方向
			linkdir_back		= 2			// 逆方向
		};
	};

	namespace link_queue
	{
		using namespace object;
		using namespace ipc_table;
		using namespace meshcode_cls;
		extern std::string TMP_LINK_ID;
		using namespace link_info;
		using namespace linkdir;
		extern std::string SEQUENCE;
		extern std::string LASTLINK_F;
		extern std::string INF_ID;
	};
	namespace lq_byway
	{
		using namespace link_queue;
	};
	namespace lq_route
	{
		using namespace link_queue;
	};
	namespace lq_guide
	{
		using namespace link_queue;
	};
	namespace lq_dirguide
	{
		using namespace link_queue;
	};
	namespace lq_lane
	{
		using namespace link_queue;
	};
	namespace lq_turnreg
	{
		using namespace link_queue;
	};
	namespace lq_uturn
	{
		using namespace link_queue;
	};
	namespace lq_vics
	{
		using namespace link_queue;
	};
	namespace lq_internavi
	{
		using namespace link_queue;
	};
	namespace lq_intersection
	{
		using namespace link_queue;
	};
	namespace inf_byway
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string BYWAY_F;
		extern std::string BACKROAD_F;
		extern std::string SOURCE_NAME;

		extern std::string ATTR[];
	};
	namespace inf_route
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string ROUTECLASS_C;
		extern std::string SOURCE_NAME;

		extern std::string ATTR[];

		/// ルートクラス
		enum routeclass_c
		{
			routeclass_access		= 1,		// アクセス道
			routeclass_connectway	= 2,		// 乗り継ぎ道
			routeclass_tourguide	= 3			// 観光案内
		};
	};
	namespace inf_dirguide
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string GUIDECLASS1_C;
		extern std::string NAME_KANJI1;
		extern std::string NAME_YOMI1;
		extern std::string GUIDECLASS2_C;
		extern std::string NAME_KANJI2;
		extern std::string NAME_YOMI2;
		extern std::string GUIDECLASS3_C;
		extern std::string NAME_KANJI3;
		extern std::string NAME_YOMI3;
		extern std::string GUIDECLASS4_C;
		extern std::string NAME_KANJI4;
		extern std::string NAME_YOMI4;
		extern std::string GUIDECLASS5_C;
		extern std::string NAME_KANJI5;
		extern std::string NAME_YOMI5;
		extern std::string GUIDECLASS6_C;
		extern std::string NAME_KANJI6;
		extern std::string NAME_YOMI6;
		extern std::string GUIDECLASS7_C;
		extern std::string NAME_KANJI7;
		extern std::string NAME_YOMI7;
		extern std::string GUIDECLASS8_C;
		extern std::string NAME_KANJI8;
		extern std::string NAME_YOMI8;
		extern std::string GUIDECLASS9_C;
		extern std::string NAME_KANJI9;
		extern std::string NAME_YOMI9;
		extern std::string GUIDECLASS10_C;
		extern std::string NAME_KANJI10;
		extern std::string NAME_YOMI10;

		extern std::string ATTR[];

		/// 地先種別
		enum guideclass_c
		{
			guideclass_normal		= 1,		// 地先(一般)
			guideclass_toll			= 2,		// 地先(有料)
			guideclass_kokudo		= 3,		// 国道番号
			guideclass_pref			= 4,		// 都道府県道番号
			guideclass_other		= 5,		// その他
			guideclass_tstring		= 12,		// 有料(文字列)
			guideclass_tkokudo		= 13,		// 有料(国道番号)
			guideclass_tpref		= 14		// 有料(県道番号)
		};
	};
	namespace inf_guide
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string GUIDETYPE_C;
		extern std::string DELETE_F;

		extern std::string ATTR[];

		/// 強制誘導コード
		enum guidetype_c
		{
			guidetype_unknown		= 0,		// 未設定
			guidetype_noguide		= 1,		// 道なり(案内なし)
			guidetype_straight		= 2,		// 直進(12時方向)
			guidetype_rightfront	= 3,		// 右前方向(1時方向)
			guidetype_rightahead	= 4,		// 右手前方向(2時方向)
			guidetype_right			= 5,		// 右折(3時方向)
			guidetype_rightbehind	= 6,		// 右斜め後ろ方向(4時方向)
			guidetype_rightback		= 7,		// 右戻る方向(5時方向)
			guidetype_back			= 8,		// 戻る方向(6時方向)
			guidetype_leftback		= 9,		// 左戻る方向(7時方向)
			guidetype_leftbehind	= 10,		// 左斜め後ろ方向(8時方向)
			guidetype_left			= 11,		// 左折(9時方向)
			guidetype_leftahead		= 12,		// 左手前方向(10時方向)
			guidetype_leftfront		= 13		// 左前方向(11時方向)
		};
	};
	namespace inf_lane
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string LANEDIR_C;
		extern std::string APPROACH_LANE;
		extern std::string RIGHT_CHANGE;
		extern std::string LEFT_CHANGE;
		extern std::string FLOW_LANE;
		extern std::string LANE_PASSABLE;

		extern std::string ATTR[];

		/// レーン方向コード
		enum lanedir_c
		{
			lanedir_front		= 1,		// 前方向(0)
			lanedir_rightfront	= 2,		// 右斜め前方向(45)
			lanedir_right		= 3,		// 右方向(90)
			lanedir_rightback	= 4,		// 右斜め後ろ方向(135)
			lanedir_back		= 5,		// 後ろ方向(180)
			lanedir_leftback	= 6,		// 左斜め後ろ方向(225)
			lanedir_left		= 7,		// 左方向(270)
			lanedir_leftfront	= 8			// 左斜め前方向(315)
		};

		/// レーン通行コード
		enum lane_passable
		{
			lanepass_nopassage	= 0,		// 通行不可
			lanepass_passable	= 1,		// 通行可
			lanepass_passage2	= 2			// 一般車両通行不可
		};
	};
	namespace inf_vics
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string VICS_MESHCODE;
		extern std::string VICSCLASS_C;
		extern std::string VICS_LINK_ID;

		extern std::string ATTR[];

		/// VICS区分
		enum vicsclass_c
		{
			vicsclass_highway		= 1,		// 高速道路
			vicsclass_cityhighway	= 2,		// 都市高速道路(含指定都市高速道路)
			vicsclass_street		= 3,		// 一般道路
			vicsclass_other			= 4			// その他
		};
	};
	namespace inf_internavi
	{
		using namespace inf_vics;
	};
	namespace dayofweek
	{
		/// 曜日指定
		enum dayofweek_c
		{
			dayofweek_holiday			= 1,		// 日・休日
			dayofweek_except_holiday	= 2,		// 日・休日を除く
			dayofweek_pubholiday		= 3,		// 休日(日曜は除く)
			dayofweek_except_pubholiday	= 4,		// 休日を除く
			dayofweek_weekday			= 5,		// 平日
			dayofweek_sunday			= 6,		// 日曜日
			dayofweek_monday			= 7,		// 月曜日
			dayofweek_tuesday			= 8,		// 火曜日
			dayofweek_wednesday			= 9,		// 水曜日
			dayofweek_thursday			= 10,		// 木曜日
			dayofweek_friday			= 11,		// 金曜日
			dayofweek_saturday			= 12,		// 土曜日
			dayofweek_weekend			= 13,		// 土日・休日
			dayofweek_except_weekend	= 14,		// 土日・休日を除く
			dayofweek_allday			= 15,		// 全曜日
			dayofweek_except_sunday		= 16,		// 日曜日を除く
			dayofweek_pre_holiday		= 17		// 日・休日の前日
		};
	};
	namespace inf_turnreg
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		using namespace dayofweek;
		extern std::string START_MONTH1;
		extern std::string START_DAY1;
		extern std::string START_HOUR1;
		extern std::string START_MIN1;
		extern std::string END_MONTH1;
		extern std::string END_DAY1;
		extern std::string END_HOUR1;
		extern std::string END_MIN1;
		extern std::string DAYOFWEEK1_C;
		extern std::string START_MONTH2;
		extern std::string START_DAY2;
		extern std::string START_HOUR2;
		extern std::string START_MIN2;
		extern std::string END_MONTH2;
		extern std::string END_DAY2;
		extern std::string END_HOUR2;
		extern std::string END_MIN2;
		extern std::string DAYOFWEEK2_C;
		extern std::string START_MONTH3;
		extern std::string START_DAY3;
		extern std::string START_HOUR3;
		extern std::string START_MIN3;
		extern std::string END_MONTH3;
		extern std::string END_DAY3;
		extern std::string END_HOUR3;
		extern std::string END_MIN3;
		extern std::string DAYOFWEEK3_C;
		extern std::string START_MONTH4;
		extern std::string START_DAY4;
		extern std::string START_HOUR4;
		extern std::string START_MIN4;
		extern std::string END_MONTH4;
		extern std::string END_DAY4;
		extern std::string END_HOUR4;
		extern std::string END_MIN4;
		extern std::string DAYOFWEEK4_C;

		extern std::string ATTR[];
	};
	namespace inf_uturn
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string SOURCE_NAME;

//		extern std::string ATTR[];
	};
	namespace inf_intersection
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;

		extern std::string ATTR[];
	};
	namespace road_code_list
	{
		using namespace object;
		using namespace ipc_table;
		extern std::string ROAD_CODE;
		extern std::string LINECLASS_C;
		extern std::string LINEDIRTYPE_C;
		extern std::string LANDCLASS_C;
		extern std::string NAME_KANJI;
		extern std::string NAME_YOMI;
		extern std::string WORK_KANJI;
		extern std::string WORK_YOMI;
		extern std::string DISPLAY_KANJI;
		extern std::string DISPLAY_YOMI;
		extern std::string SHIELD_NO_STRING;

		extern std::string ATTR[];

		/// 路線種別コード
		enum lineclass_c
		{
			lineclass_highway		= 1,		// 高速道路
			lineclass_tollroad		= 2,		// 有料道路
			lineclass_metropolis	= 3,		// 首都高速道路
			lineclass_hanshin		= 4,		// 阪神高速道路
			lineclass_nagoya		= 5,		// 名古屋都市高速道路
			lineclass_fukuoka		= 6,		// 福岡都市高速道路
			lineclass_kitakyushu	= 7,		// 北九州都市高速道路
			lineclass_hiroshima		= 8,		// 広島高速道路
			lineclass_ferry			= 9,		// フェリー航路
			lineclass_street		= 10,		// 一般街道名称
			lineclass_tfspecial		= 11,		// TF料金対応路線（一般有料道路）
			lineclass_kyouto		= 12		// 京都高速道路
		};

		/// 路線方向タイプ
		enum linedirtype_c
		{
			linedirtype_none		= 0,		// タイプなし
			linedirtype_updown		= 1,		// 上り・下り
			linedirtype_inout		= 2,		// 内回り・外回り
			linedirtype_eastwest	= 3,		// 西行き・東行き
			linedirtype_other		= 4			// その他
		};

		/// 地域種別
		enum landclass_c
		{
			landclass_unknown		= 0,		// 未調査
			landclass_hokkaido		= 1,		// 北海道
			landclass_honshu		= 2,		// 本州
			landclass_shikoku		= 3,		// 四国
			landclass_kyushu		= 4,		// 九州
			landclass_okinawa		= 5,		// 沖縄
			landclass_detached		= 6			// 独立路線
		};
	};
	namespace link_rest
	{
		using namespace object;
		using namespace field_survey;
		using namespace ipc_table;
		using namespace link_info;
		extern std::string MESHCODE;
		extern std::string START_MONTH;
		extern std::string START_DAY;
		extern std::string START_HOUR;
		extern std::string START_MIN;
		extern std::string END_MONTH;
		extern std::string END_DAY;
		extern std::string END_HOUR;
		extern std::string END_MIN;
		extern std::string DAYOFWEEK_C;
		using namespace dayofweek;

		extern std::string ATTR[];
	};
	namespace time_nopassage
	{
		using namespace link_rest;
		extern std::string SEASONREG_F;
	};
	namespace time_oneway
	{
		using namespace link_rest;
		using namespace linkdir;
	};

	namespace scale_disptype
	{
		enum scdisptype_c
		{
			scdisptype_nodisp	= 0,	// 表示なし
			scdisptype_stronly	= 1,	// 文字列のみ
			scdisptype_markstr	= 2,	// 記号＋注記
			scdisptype_pntstr	= 3,	// 指示点＋注記
			scdisptype_markonly	= 4		// 記号のみ
		};
	};

	namespace annotation
	{
		using namespace object;
		using namespace ipc_feature;
		using namespace scale_disptype;
		extern std::string NAME_STRING1;
		extern std::string NAME_STRING2;
		extern std::string NAME_STRING3;
		extern std::string STRING_NUM1;
		extern std::string STRING_NUM2;
		extern std::string STRING_NUM3;
		extern std::string ANNOCLASS_C;
		extern std::string TMP_ANNO_CLASS;
	};

	namespace middle_annotation
	{
		using namespace annotation;
		extern std::string SC1DISPTYPE_C;
		extern std::string SC2DISPTYPE_C;
		extern std::string SC3DISPTYPE_C;
		extern std::string SC4DISPTYPE_C;
	};

	namespace base_annotation
	{
		using namespace annotation;
		extern std::string SC1DISPTYPE_C;
		extern std::string SC2DISPTYPE_C;
		extern std::string SC3DISPTYPE_C;
		extern std::string SC4DISPTYPE_C;
	};

	namespace city_annotation
	{
		using namespace annotation;
		extern std::string SCDISPTYPE_C;
	};
	namespace access_store
	{
		using namespace object;
		using namespace ipc_table;

		extern std::string ACCESSPOINT_ID;
		extern std::string MULTIPLESTORE_C;
		extern std::string START_HOUR;
		extern std::string START_MIN;
		extern std::string END_HOUR;
		extern std::string END_MIN;
		extern std::string SUNDAY_F;
		extern std::string MONDAY_F;
		extern std::string TUESDAY_F;
		extern std::string WEDNESDAY_F;
		extern std::string THURSDAY_F;
		extern std::string FRIDAY_F;
		extern std::string SATURDAY_F;
		extern std::string BON_F;
		extern std::string YEAREND_F;
		extern std::string NEWYEAR_F;
		extern std::string GOLDENWEEK_F;
		extern std::string HOLIDAY_F;
		extern std::string STORETYPE_C;

		enum storetype_c
		{
			storetype_gs	= 0,	// ガソリンスタンド
			storetype_store	= 1		// チェーン店
		};
	}

	namespace etc_tollgate
	{
		using namespace object;
		using namespace ipc_table;

		
		extern std::string TOLLGATE_ID;
		extern std::string TOLLGATE_PARENT_ID; 
		extern std::string TOLLGATE_OFFICE_ID;
		extern std::string TOLLGATE_OFFICE_NAME;
		extern std::string TOLLGATE_ROAD_NUMBER;
		extern std::string TOLLGATE_ROAD_NAME;
		extern std::string TOLLGATE_IC_NUMBER;
		extern std::string TOLLGATE_IC_NAME;
		extern std::string TOLLGATE_IC_NAME_YOMI;
		extern std::string TOLLGATE_FLAG;
		extern std::string TOLLGATE_OTHER;
	}
};

#endif	//___SINDY_ATTR_DEF_H___
