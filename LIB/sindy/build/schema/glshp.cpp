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

#include "stdafx.h"
#include <sindy/schema/glshp.h>

namespace glshp
{
namespace schema
{
namespace objectid
{
	const _TCHAR kObjectId[] = _T("OBJECTID");			//!< OBJECTID
}
namespace base_table
{
	const _TCHAR kCountry_C[] = _T("COUNTRY_C");		//!< 国コード
}
namespace base_feature
{
	const _TCHAR kShape[]     = _T("SHAPE");			//!< 形状フィールド名
}
namespace scale_class
{
	const _TCHAR kSc1Class_C[] = _T("SC1CLASS_C");		//!< スケール1背景種別
	const _TCHAR kSc2Class_C[] = _T("SC2CLASS_C");		//!< スケール2背景種別
	const _TCHAR kSc3Class_C[] = _T("SC3CLASS_C");		//!< スケール3背景種別
	const _TCHAR kSc4Class_C[] = _T("SC4CLASS_C");		//!< スケール4背景種別
}
namespace disp_scale
{
	const _TCHAR kDispScale1_F[] = _T("SC1DISP_F");		//!< スケール1表示フラグ
	const _TCHAR kDispScale2_F[] = _T("SC2DISP_F");		//!< スケール2表示フラグ
	const _TCHAR kDispScale3_F[] = _T("SC3DISP_F");		//!< スケール3表示フラグ
	const _TCHAR kDispScale4_F[] = _T("SC4DISP_F");		//!< スケール4表示フラグ
}
namespace lang_type
{
	const _TCHAR kOnJpn[] = _T("ON_JPN");				//!< メイン名称(日本語)
	const _TCHAR kAnJpn[] = _T("AN_JPN");				//!< サブ名称(日本語)
	const _TCHAR kOnEng[] = _T("ON_ENG");				//!< メイン名称(英語)
	const _TCHAR kAnEng[] = _T("AN_ENG");				//!< サブ名称(英語)
	const _TCHAR kOnTha[] = _T("ON_THA");				//!< メイン名称(タイ語)
	const _TCHAR kAnTha[] = _T("AN_THA");				//!< サブ名称(タイ語)
	const _TCHAR kOnInd[] = _T("ON_IND");				//!< メイン名称(インドネシア語)
	const _TCHAR kAnInd[] = _T("AN_IND");				//!< サブ名称(インドネシア語)
	const _TCHAR kOnMay[] = _T("ON_MAY");				//!< メイン名称(マレー語)
	const _TCHAR kAnMay[] = _T("AN_MAY");				//!< サブ名称(マレー語)
	const _TCHAR kOnVie[] = _T("ON_VIE");				//!< メイン名称(ベトナム語)
	const _TCHAR kAnVie[] = _T("AN_VIE");				//!< サブ名称(ベトナム語)
}
namespace under_f
{
	const _TCHAR kUnder_F[] = _T("UNDER_F");			//!< 地下フラグ
}
namespace infid
{
	const _TCHAR kInfId[] = _T("INFID");				//!< InfID
}
///// 地図系 /////
	// LAND_USE
	namespace land_use
	{
		const _TCHAR kSuffix[] = _T("BLU");				//!< 識別子(ランドユース)
	}
	// LAND_COVER
	namespace land_cover
	{
		const _TCHAR kSuffix[] = _T("BLC");				//!< 識別子(ランドカバー)
	}
	// WATERWAY
	namespace waterway
	{
		const _TCHAR kSuffix[] = _T("BWW");				//!< 識別子(水系)
	}
	// ISLAND
	namespace island
	{
		const _TCHAR kSuffix[] = _T("BIL");				//!< 識別子(島)
	}
	// 背景ライン
	namespace base_line
	{
		const _TCHAR kSuffix[] = _T("BGL");				//!< 識別子(背景ライン)

		const _TCHAR kBgClass_C[] = _T("BGCLS_C");		//!< 背景ライン種別
	}
	// PROVINCE
	namespace province
	{
		const _TCHAR kSuffix[] = _T("APR");				//!< 識別子(行政界(州・地方))

		const _TCHAR kAdminCode[] = _T("ADMIN_CODE");	//!< 行政界コード
	}
	// DISTRICT
	namespace district
	{
		const _TCHAR kSuffix[] = _T("ADR");				//!< 識別子(行政界(地区))

		const _TCHAR kAdminCode[] = _T("ADMIN_CODE");	//!< 行政界コード
	}
	// SUB DISTRICT
	namespace sub_district
	{
		const _TCHAR kSuffix[] = _T("ASD");				//!< 識別子(行政界(分区))

		const _TCHAR kAdminCode[] = _T("ADMIN_CODE");	//!< 行政界コード
	}
	// ZIP_CODE
	namespace postal_district
	{
		const _TCHAR kSuffix[] = _T("PSD");				//!< 識別子(郵便番号)

		const _TCHAR kPostCode[] = _T("POST_CODE");		//!< 郵便番号コード
	}
	// BASE_RAILWAY
	namespace base_railway
	{
		const _TCHAR kSuffix[] = _T("RAL");				//!< 識別子(鉄道路線)

		const _TCHAR kRalCls_C[] = _T("RALCLS_C");		//!< 鉄道ライン種別
	}
	// BASE_STATION
	namespace base_station
	{
		const _TCHAR kSuffix[] = _T("STA");				//!< 識別子(駅舎)

		const _TCHAR kStaCls_C[] = _T("STACLS_C");		//!< 駅舎種別
	}
	// BUILDING
	namespace building
	{
		const _TCHAR kSuffix[] = _T("BLD");				//!< 識別子(建物)

		const _TCHAR kBldCls_C[] = _T("BLDCLS_C");		//!< 建物種別
		const _TCHAR kFloors[] = _T("FLOORS");			//!< 階数情報
		const _TCHAR kCorridor_F[] = _T("CORRIDOR_F");	//!< 渡り廊下フラグ
	}
	// CITY_STATION
	namespace city_station
	{
		const _TCHAR kSuffix[] = _T("USP");				//!< 識別子(地下駅舎)

		const _TCHAR kStaCls_C[] = _T("STACLS_C");		//!< 地下駅舎種別
	}
///// 道路系 /////
namespace linkid
{
	const _TCHAR kLinkId[] = _T("LINKID");				//!< 道路リンクID
}
namespace link_queue
{
	const _TCHAR kSequence[] = _T("SEQUENCE");			//!< シーケンス番号
}
namespace reg_condition
{
	const _TCHAR kFromMonth[] = _T("FROM_MONTH");		//!< 開始月
	const _TCHAR kFromDay[] = _T("FROM_DAY");			//!< 開始日
	const _TCHAR kToMonth[] = _T("TO_MONTH");			//!< 終了月
	const _TCHAR kToDay[] = _T("TO_DAY");				//!< 終了日
	const _TCHAR kFromHour[] = _T("FROM_HOUR");			//!< 開始時
	const _TCHAR kFromMin[] = _T("FROM_MIN");			//!< 開始分
	const _TCHAR kToHour[] = _T("TO_HOUR");				//!< 終了時
	const _TCHAR kToMin[] = _T("TO_MIN");				//!< 終了分
	const _TCHAR kDayOfWeek[] = _T("DAYOFWEEK");		//!< 曜日指定
}

	// 道路リンク
	namespace road_link
	{
		const _TCHAR kSuffix[] = _T("RLK");				//!< 識別子(道路リンク)

		const _TCHAR kFromNodeId[] = _T("FROMNODEID");	//!< 始点ノードID
		const _TCHAR kToNodeId[] = _T("TONODEID");		//!< 終点ノードID
		const _TCHAR kRoadCls_C[] = _T("ROADCLS_C");	//!< 道路種別
		const _TCHAR kNaviCls_C[] = _T("NAVICLS_C");	//!< 経路種別
		const _TCHAR kLinkCls_C[] = _T("LINKCLS_C");	//!< リンク種別
		const _TCHAR kWidth_C[] = _T("WIDTH_C");		//!< 幅員区分
		const _TCHAR kIsland_F[] = _T("ISLAND_F");		//!< 離島フラグ
		const _TCHAR kRoadNo[] = _T("ROADNO");			//!< 道路番号
		const _TCHAR kLaneCount[] = _T("LANECOUNT");	//!< レーン数
		const _TCHAR kNoPassage_C[] = _T("NOPASS_C");	//!< 通行禁止コード
		const _TCHAR kOneway_C[] = _T("ONEWAY_C");		//!< 一方通行コード
		const _TCHAR kVclReg_RF[] = _T("VCLREG_RF");	//!< 車種別規制関連フラグ
		const _TCHAR kLegalSpd_C[] = _T("LEGALSPD_C");	//!< 法定速度コード
	}
	// 道路ノード
	namespace road_node
	{
		const _TCHAR kSuffix[] = _T("RND");				//!< 識別子(道路ノード)

		const _TCHAR kObjectId2[]  = _T("OBJECTID_2");	//!< OBJECTID2
		const _TCHAR kCountry_2C[] = _T("COUNTRY_2C");	//!< 国コード2
		const _TCHAR kNodeCls_C[]  = _T("NODECLS_C");	//!< ノード種別
	}
	// リンク内属性
	namespace segment_attr
	{
		const _TCHAR kSuffix[] = _T("RSA");				//!< 識別子(リンク内属性)

		const _TCHAR kLinkAttr_C[] = _T("LINKATTR_C");	//!< リンク内属性種別
	}
	// 通行禁止
	namespace nopassage_condition
	{
		const _TCHAR kSuffix[] = _T("NPC");				//!< 識別子(通行禁止)
	}
	// 一方通行
	namespace oneway_condition
	{
		const _TCHAR kSuffix[] = _T("OWC");				//!< 識別子(一方通行)

		const _TCHAR kLinkDir[] = _T("LINKDIR");		//!< リンク方向
	}
	// 車種別規制
	namespace vehicle_reg
	{
		const _TCHAR kSuffix[] = _T("VRI");				//!< 識別子(車種別規制)

		const _TCHAR kRegType_C[] = _T("REGTYPE_C");	//!< 車種別規制状態
		const _TCHAR kHeight[] = _T("HEIGHT");			//!< 高さ制限
		const _TCHAR kWidth[] = _T("WIDTH");			//!< 最大幅制限
		const _TCHAR kWeight[] = _T("WEIGHT");			//!< 重量制限
		const _TCHAR kLength[] = _T("LENGTH");			//!< 長さ制限
		const _TCHAR kLoadage[] = _T("LOADAGE");		//!< 積載重量制限
		const _TCHAR kDanger_F[] = _T("DANGER_F");		//!< 危険貨物規制
	}
	// 車種別規制タイプ
	namespace vehicle_type
	{
		const _TCHAR kSuffix[] = _T("VRT");				//!< 識別子(車種別規制タイプ)

		const _TCHAR kRegId[] = _T("REGID");			//!< 車種別規制情報のID
		const _TCHAR kExclude_F[] = _T("EXCLUDE_F");	//!< 除外条件フラグ
		const _TCHAR kRegName[] = _T("REG_NAME");		//!< 規制条件名称
		const _TCHAR kPsgr_F[] = _T("PSGR_F");			//!< 乗用車フラグ
		const _TCHAR kTaxi_F[] = _T("TAXI_F");			//!< タクシーフラグ
		const _TCHAR kPbBus_F[] = _T("PBBUS_F");		//!< 公共バスフラグ
		const _TCHAR kPrBus_F[] = _T("PRBUS_F");		//!< プライベートバスフラグ
		const _TCHAR kTruck_F[] = _T("TRUCK_F");		//!< 輸送トラックフラグ
	}
	// リンク列規制
	namespace turnreg_inf
	{
		const _TCHAR kSuffix[] = _T("TRI");				//!< 識別子(リンク列規制情報)
	}
	namespace turnreg_lq
	{
		const _TCHAR kSuffix[] = _T("TRL");				//!< 識別子(リンク列)
	}
	// 方面案内
	namespace dirguide_inf
	{
		const _TCHAR kSuffix[] = _T("DRI");				//!< 識別子(方面案内情報)

		const _TCHAR kGuideCls_C[] = _T("GUIDECLASS");	//!< 案内種別
		const _TCHAR kName1[] = _T("NAME_1");			//!< 方面案内名称(第一言語)
		const _TCHAR kName2[] = _T("NAME_2");			//!< 方面案内名称(第二言語)
		const _TCHAR kNameSeq[] = _T("NAME_SEQ");		//!< 名称シーケンス
		const _TCHAR kBoardPtn_C[] = _T("BOARDPTN_C");	//!< 看板色
	}
	namespace dirguide_lq
	{
		const _TCHAR kSuffix[] = _T("DRL");				//!< 識別子(リンク列)
	}
	// レーン
	namespace lane_inf
	{
		const _TCHAR kSuffix[] = _T("LNI");				//!< 識別子(レーン情報)

		const _TCHAR kLaneDir[] = _T("LANEDIR");		//!< レーン方向
		const _TCHAR kAppLane[] = _T("APPLANE");		//!< 直近車線数
		const _TCHAR kRightChg[] = _T("RIGHTCHG");		//!< 右車線変化量
		const _TCHAR kLeftChg[] = _T("LEFTCHG");		//!< 左車線変化量
		const _TCHAR kPassable[] = _T("PASSABLE");		//!< 通行可・不可情報
	}
	namespace lane_lq
	{
		const _TCHAR kSuffix[] = _T("LNL");				//!< 識別子(リンク列)
	}
	// 強制誘導
	namespace guide_inf
	{
		const _TCHAR kSuffix[] = _T("GDI");				//!< 識別子(強制誘導情報)

		const _TCHAR kGuideType_C[] = _T("GUIDETYPE");	//!< 案内コード
	}
	namespace guide_lq
	{
		const _TCHAR kSuffix[] = _T("GDL");				//!< 識別子(リンク列)
	}
///// POI /////
	// POI
	namespace poi
	{
		const _TCHAR kSuffix[] = _T("POI");				//!< 識別子(POI)

		const _TCHAR kCat_C[]      = _T("CAT_C");		//!< カテゴリコード
		const _TCHAR kSubCat_C[]   = _T("SUBCAT_C");	//!< サブカテゴリコード
		const _TCHAR kChainMst_C[] = _T("CHAINMST_C");	//!< チェーンマスターコード
		const _TCHAR kAdmin_C[]    = _T("ADMIN_C");		//!< 行政界コード
		const _TCHAR kHouseNum[]   = _T("HOUSENUM");	//!< ハウスナンバー
		const _TCHAR kActAddr[]    = _T("ACTADDR");		//!< Actual Address
		const _TCHAR kTelNum[]     = _T("TELNUM");		//!< 代表電話番号

		const _TCHAR kRName_Jpn[] = _T("RNAME_JPN");	//!< 道路名称(日本語)
		const _TCHAR kRName_Eng[] = _T("RNAME_ENG");	//!< 道路名称(英語)
		const _TCHAR kRName_Tha[] = _T("RNAME_THA");	//!< 道路名称(タイ語)
		const _TCHAR kRName_Ind[] = _T("RNAME_IND");	//!< 道路名称(インドネシア語)
		const _TCHAR kRName_May[] = _T("RNAME_MAY");	//!< 道路名称(マレー語)
		const _TCHAR kRName_Vie[] = _T("RNAME_VIE");	//!< 道路名称(ベトナム語)
	}
	// HNP
	namespace hnp
	{
		const _TCHAR kSuffix[] = _T("HNP");				//!< 識別子(住所)

		const _TCHAR kType_C[] = _T("TYPE_C");			//!< ハウスナンバー種別コード
		const _TCHAR kNumber[] = _T("NUMBER");			//!< ハウスナンバー
		const _TCHAR kAdmin_C[] = _T("ADMIN_C");		//!< 行政界コード
		const _TCHAR kLinkID[] = _T("LINKID");			//!< 道路リンクID
	}
}	// schema
}	// glshp
