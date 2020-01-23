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
#include <sindy/schema/shp05.h>

namespace shp05 {

namespace schema
{
	LPCTSTR kObjectId = _T("OBJECTID");
	LPCTSTR kShape = _T("SHAPE");

	namespace annotation
	{
		LPCTSTR kAnnoClass_c = _T("ANNOCLS_C");
		LPCTSTR kNameString = _T("NAMESTRING");
		LPCTSTR kDispScale = _T("SCALE");
		LPCTSTR kFontSize = _T("FONTSIZE");
		LPCTSTR kMark = _T("MARK");
	}

	namespace admin
	{
		LPCTSTR kSuffix = _T("ADM");

		LPCTSTR kCityCode = _T("CITYCODE");
	}

	namespace topmap
	{
		namespace back_poly
		{
			LPCTSTR kSuffix = _T("BGP");

			LPCTSTR kBgClass_c = _T("BGCLS_C");
		}

		namespace back_line
		{
			LPCTSTR kSuffix = _T("BGL");

			LPCTSTR kBgClass_c = _T("BGCLS_C");
		}

		namespace contour
		{
			LPCTSTR kSuffix = _T("CNT");

			LPCTSTR kCntClass_c = _T("CNTCLS_C");
		}
	}

	namespace middlemap
	{
		namespace back_poly
		{
			LPCTSTR kSuffix = _T("BGP");

			LPCTSTR kSC4BgClass_c = _T("SC4CLASS_C");
			LPCTSTR kSC3BgClass_c = _T("SC3CLASS_C");
			LPCTSTR kSC2BgClass_c = _T("SC2CLASS_C");
			LPCTSTR kSC1BgClass_c = _T("SC1CLASS_C");
		}

		namespace back_line
		{
			LPCTSTR kSuffix = _T("BGL");

			LPCTSTR kBgClass_c = _T("BGCLS_C");
			LPCTSTR kSC1Disp_f = _T("SC1DISP_F");
			LPCTSTR kSC2Disp_f = _T("SC2DISP_F");
			LPCTSTR kSC3Disp_f = _T("SC3DISP_F");
			LPCTSTR kSC4Disp_f = _T("SC4DISP_F");
			LPCTSTR kUnder_f = _T("UNDER_F");
		}

		namespace contour
		{
			LPCTSTR kSuffix = _T("CNT");

			LPCTSTR kCntClass_c = _T("CNTCLS_C");
		}

		using namespace annotation;
	}

	namespace basemap
	{
		namespace back_poly
		{
			LPCTSTR kSuffix = _T("BGP");

			LPCTSTR kSC4BgClass_c = _T("SC4CLASS_C");
			LPCTSTR kSC3BgClass_c = _T("SC3CLASS_C");
			LPCTSTR kSC2BgClass_c = _T("SC2CLASS_C");
			LPCTSTR kSC1BgClass_c = _T("SC1CLASS_C");
		}

		namespace back_line
		{
			LPCTSTR kSuffix = _T("BGL");

			LPCTSTR kBgClass_c = _T("BGCLS_C");
			LPCTSTR kSC1Disp_f = _T("SC1DISP_F");
			LPCTSTR kSC2Disp_f = _T("SC2DISP_F");
			LPCTSTR kSC3Disp_f = _T("SC3DISP_F");
			LPCTSTR kSC4Disp_f = _T("SC4DISP_F");
			LPCTSTR kUnder_f = _T("UNDER_F");
		}

		namespace contour
		{
			LPCTSTR kSuffix = _T("CNT");

			LPCTSTR kCntClass_c = _T("CNTCLS_C");
		}

		namespace railway
		{
			LPCTSTR kSuffix = _T("RAL");

			LPCTSTR kRalClass_c = _T("RALCLS_C");
			LPCTSTR kSC1Disp_f = _T("SC1DISP_F");
			LPCTSTR kSC2Disp_f = _T("SC2DISP_F");
			LPCTSTR kSC3Disp_f = _T("SC3DISP_F");
			LPCTSTR kSC4Disp_f = _T("SC4DISP_F");
			LPCTSTR kUnder_f = _T("UNDER_F");
		}

		namespace station
		{
			LPCTSTR kSuffix = _T("STA");

			LPCTSTR kStaClass_c = _T("STACLS_C");
			LPCTSTR kUnder_f = _T("UNDER_F");
		}

		namespace admin_table
		{
			LPCTSTR kCityCode = _T("CITYCODE");
			LPCTSTR kPrefName = _T("PREFNAME");
			LPCTSTR kCityName = _T("CITYNAME");
			LPCTSTR kCityYomi = _T("CITYYOMI");
		}
	}

	namespace citymap
	{
		namespace back_poly
		{
			LPCTSTR kSuffix = _T("BGP");

			LPCTSTR kBgClass_c = _T("BGCLS_C");
			LPCTSTR kColorCode = _T("COLORCODE");
		}

		namespace back_line
		{
			LPCTSTR kSuffix = _T("BGL");

			LPCTSTR kBgClass_c = _T("BGCLS_C");
		}

		namespace railway
		{
			LPCTSTR kSuffix = _T("RAL");

			LPCTSTR kRalClass_c = _T("RALCLS_C");
			LPCTSTR kUnder_f = _T("UNDER_F");
		}

		namespace building
		{
			LPCTSTR kSuffix = _T("BLD");

			LPCTSTR kBldClass_c = _T("BLDCLS_C");
			LPCTSTR kCorridor_f = _T("CORRIDOR_F");
		}

		namespace station
		{
			LPCTSTR kSuffix = _T("USP");

			LPCTSTR kStaClass_c = _T("STACLS_C");
		}

		namespace bld_line
		{
			LPCTSTR kSuffix = _T("DCL");

			LPCTSTR kBldId = _T("BLDID");
		}

		namespace bld_name
		{
			LPCTSTR kSuffix = _T("BDN");

			LPCTSTR kBldId = _T("BLDID");
			LPCTSTR kBldName  = _T("BLDNAME");
		}
	}

	namespace road
	{
		namespace link_id
		{
			LPCTSTR kLinkId = _T("LINKID");
		}

		namespace link_dir
		{
			LPCTSTR kLinkDir = _T("LINKDIR");
		}

		namespace kind
		{
			LPCTSTR kKind_c = _T("KIND_C");
		}

		namespace road_link
		{
			LPCTSTR kSuffix = _T("RLK");

			LPCTSTR kFromNodeId = _T("FROMNODEID");
			LPCTSTR kToNodeId = _T("TONODEID");
			LPCTSTR kRoadClass_c = _T("ROADCLS_C");
			LPCTSTR kNaviClass_c = _T("NAVICLS_C");
			LPCTSTR kLinkClass_c = _T("LINKCLS_C");
			LPCTSTR kManager_c = _T("MANAGER_C");
			LPCTSTR kWidth_c = _T("WIDTH_C");
			LPCTSTR kWideArea_f = _T("WIDEAREA_F");
			LPCTSTR kIsland_f = _T("ISLAND_F");
			LPCTSTR kBypass_f = _T("BYPASS_F");
			LPCTSTR kCaronly_f = _T("CARONLY_F");
			LPCTSTR kRoadNo = _T("ROADNO");
			LPCTSTR kRoadCode = _T("ROADCODE");
			LPCTSTR kLaneCount = _T("LANECOUNT");
			LPCTSTR kNoPassage_c = _T("NOPASS_C");
			LPCTSTR kOneway_c = _T("ONEWAY_C");
		}

		namespace road_link_a
		{
			LPCTSTR kSuffix = _T("RLA");

			LPCTSTR kDivId = _T("DIVID");
			LPCTSTR kFromDivId = _T("FROMDIVID");
			LPCTSTR kToDivId = _T("TODIVID");
			LPCTSTR kRoadAttr_c = _T("ROADATTR_C");
		}

		namespace road_node
		{
			LPCTSTR kSuffix = _T("RND");

			LPCTSTR kSignal_f = _T("SIGNAL_F");
			LPCTSTR kNameKanji = _T("NAME_KANJI");
			LPCTSTR kNameYomi = _T("NAME_YOMI");
		}

		namespace road_node_a
		{
			LPCTSTR kSuffix = _T("RNA");

			LPCTSTR kDivId = _T("DIVID");
		}

		namespace segment_attr
		{
			LPCTSTR kSuffix = _T("RSA");

			LPCTSTR kLinkAttr_c = _T("LINKATTR_C");
		}

		namespace segment_attr_vehicle
		{
			LPCTSTR kSuffix = _T("RSV");
		}

		namespace reg_condition
		{
			LPCTSTR kFromMonth = _T("FROM_MONTH");
			LPCTSTR kFromDay = _T("FROM_DAY");
			LPCTSTR kToMonth = _T("TO_MONTH");
			LPCTSTR kToDay = _T("TO_DAY");
			LPCTSTR kFromHour = _T("FROM_HOUR");
			LPCTSTR kFromMin = _T("FROM_MIN");
			LPCTSTR kToHour = _T("TO_HOUR");
			LPCTSTR kToMin = _T("TO_MIN");
			LPCTSTR kDayOfWeek = _T("DAYOFWEEK");
		}

		namespace nopassage_condition
		{
			LPCTSTR kSuffix = _T("NPC");
		}

		namespace oneway_condition
		{
			LPCTSTR kSuffix = _T("OWC");
		}

		namespace vehicle_reg
		{
			LPCTSTR kSuffix = _T("VRI");

			LPCTSTR kRegType_c = _T("REGTYPE_C");
			LPCTSTR kHeight = _T("HEIGHT");
			LPCTSTR kWidth = _T("WIDTH");
			LPCTSTR kWeight = _T("WEIGHT");
			LPCTSTR kLength = _T("LENGTH");
			LPCTSTR kLoadage = _T("LOADAGE");
			LPCTSTR kDanger_f = _T("DANGER_F");
		}

		namespace vehicle_type
		{
			LPCTSTR kSuffix = _T("VRT");

			LPCTSTR kRegId = _T("REG_ID");
			LPCTSTR kVehicleRegCode = _T("VR_CODE");
			LPCTSTR kExclude_f = _T("EXCLUDE_F");
		}

		namespace vehicle_reg_master
		{
			LPCTSTR kTableName = _T("vehicle_reg_master");

			LPCTSTR kVehicleRegCode = _T("VR_CODE");
			LPCTSTR kVehicleRegName = _T("VR_NAME");
			LPCTSTR kOgataJoyo_f = _T("O_JO_F");
			LPCTSTR kTokuteiChuJo_f = _T("T_C_JO_F");
			LPCTSTR kChugataJoyo_f = _T("C_JO_F");
			LPCTSTR kFutsuJoyo_f = _T("F_JO_F");
			LPCTSTR kFutsuJoyoKei_f = _T("F_JO_K_F");
			LPCTSTR kOgataKamotsu_f = _T("O_KA_F");
			LPCTSTR kTokuteiChuKa_f = _T("T_C_KA_F");
			LPCTSTR kChugataKamotsu_f = _T("C_KA_F");
			LPCTSTR kFutsuKamotsu_f = _T("F_KA_F");
			LPCTSTR kFutsuKamotsuKei_f = _T("F_KA_K_F");
			LPCTSTR kOgataTokushu_f = _T("O_TO_F");
			LPCTSTR kKogataTokushu_f = _T("K_TO_F");
			LPCTSTR kOgataBike_f = _T("O_BI_F");
			LPCTSTR kFutsuBike_f = _T("F_BI_F");
			LPCTSTR kKogataBike_f = _T("K_BI_F");
			LPCTSTR kTaxi_f = _T("TAXI_F");
			LPCTSTR kBus_f = _T("BUS_F");
			LPCTSTR kMiniBike_f = _T("M_BI_F");
			LPCTSTR kTrolley_f = _T("TROLLEY_F");
			LPCTSTR kBicycle_f = _T("BICY_F");
			LPCTSTR kExceptBicycle_f = _T("EX_BICY_F");
		}

		namespace link_queue
		{
			LPCTSTR kFromLinkId = _T("FROMLINKID");
			LPCTSTR kToLinkId = _T("TOLINKID");
			LPCTSTR kMidCount = _T("MIDCOUNT");
			LPCTSTR kMidLinkId = _T("MIDLINKID");
		}
		// リンク列規制
		namespace turnreg
		{
			LPCTSTR kSuffix = _T("TRN");
		}
		// 方面案内
		namespace dir_guide
		{
			LPCTSTR kSuffix = _T("DIR");

			LPCTSTR kGuideClass = _T("GUIDECLASS");
			LPCTSTR kNameKanji = _T("NAME_KANJI");
			LPCTSTR kNameYomi = _T("NAME_YOMI");
			LPCTSTR kBoardPtn = _T("BOARDPTN_C");
			LPCTSTR kBoardSeq = _T("BOARD_SEQ");
		}
		// レーン情報
		namespace lane
		{
			LPCTSTR kSuffix = _T("LAN");

			LPCTSTR kLaneDir = _T("LANEDIR");
			LPCTSTR kAppLane = _T("APPLANE");
			LPCTSTR kRightChange = _T("RIGHTCHG");
			LPCTSTR kLeftChange = _T("LEFTCHG");
			LPCTSTR kPassable = _T("PASSABLE");
		}
		// 強制誘導
		namespace guide
		{
			LPCTSTR kSuffix = _T("GID");

			LPCTSTR kGuideType = _T("GUIDETYPE");
		}
		// VICS
		namespace vics
		{
			LPCTSTR kSuffix = _T("VIC");

			LPCTSTR kVicsMesh = _T("MESHCODE");
			LPCTSTR kVicsId = _T("VICSID");
			LPCTSTR kVicsClass = _T("VICSCLASS");
			LPCTSTR kLinkDir = _T("LINKDIR");
			
		}
		// 進入方向別交差点
		namespace intersection
		{
			LPCTSTR kSuffix = _T("INT");

			LPCTSTR kNameKanji = _T("NAME_KANJI");
			LPCTSTR kNameYomi = _T("NAME_YOMI");
		}
		// 道路名称
		namespace road_code
		{
			LPCTSTR kTableName = _T("roadcode");

			LPCTSTR kRoadType = _T("ROADTYPE");
			LPCTSTR kRoadCode = _T("ROADCODE");
			LPCTSTR kRoadName = _T("ROADNAME");
			LPCTSTR kRoadYomi = _T("ROADYOMI");
		}

		// 道路・歩行者リンク
		namespace road_walk_link
		{
			LPCTSTR kSuffix = _T("RWL");

			LPCTSTR kWalkClass_c = _T("WALKCLS_C");
			LPCTSTR kFloorMove_c = _T("FLRMV_C");
			LPCTSTR kWalkAttr_c = _T("WALKATTR_C");
			LPCTSTR kWalkable_c = _T("WALKABLE_C");
			LPCTSTR kSurface_c = _T("SURFACE_C");
			LPCTSTR kWheelChair_c = _T("WCHR_C");
		
		}

		namespace road_walk_link_a
		{
			LPCTSTR kDivId = _T("DIVID");
			LPCTSTR kFromDivId = _T("FROMDIVID");
			LPCTSTR kToDivId = _T("TODIVID");
			LPCTSTR kRoadAttr_c = _T("ROADATTR_C");
		}

		// 道路・歩行者ノード
		namespace road_walk_node
		{
			LPCTSTR kSuffix = _T("RWN");

			LPCTSTR kWalkNodeClass_c = _T("WKNDCLS_C");
		}

		namespace road_walk_node_a
		{
			LPCTSTR kDivId = _T("DIVID");
		}

		namespace road_walk_nopassage_condition
		{
			LPCTSTR kSuffix = _T("NPW");
		}

		namespace road_walk_oneway_condition
		{
			LPCTSTR kSuffix = _T("OWW");
		}

		// 道路・歩行者ノード－駅出入口POI関連テーブル
		namespace rel_sgp
		{
			LPCTSTR kSuffix = _T("rel_sgp");

			LPCTSTR kNodeId = _T("NODE_ID");
			LPCTSTR kPoiId = _T("POI_ID"); 
		}

		// 道路・歩行者ノード－一般POI関連テーブル
		namespace rel_walk_node_general_poi
		{
			LPCTSTR kNodeId = _T("NODE_ID");
			LPCTSTR kPoiId = _T("POI_ID"); 
		}

		// 道路リンク横断可否テーブル
		namespace roadcrossing
		{
			LPCTSTR kTableName = _T("roadcrossing");

			LPCTSTR kLinkId = _T("LINK_ID");
			LPCTSTR kCrs_f = _T("CRS_F");
			LPCTSTR kPedeZone_f = _T("PEDEZONE_F");
		}

		// 道路・歩行者リンク関連付けテーブル
		namespace buddy
		{
			LPCTSTR kTableName = _T("buddy");

			LPCTSTR kBaseLinkId = _T("BASE_LNKID");
			LPCTSTR kBaseDivId = _T("BASE_DIVID");
			LPCTSTR kRelLinkId = _T("REL_LNKID");
			LPCTSTR kRelDivId = _T("REL_DIVID");
			LPCTSTR kSide_c = _T("SIDE_C");
		}

		// 道路・歩行者リンク相対高さテーブル
		namespace rel_height
		{
			LPCTSTR kTableName = _T("rel_height");

			LPCTSTR kLinkId = _T("LINK_ID");
			LPCTSTR kLinkDivId = _T("LINK_DIVID");
			LPCTSTR kFromLevel = _T("FROMLEVEL");
			LPCTSTR kToLevel = _T("TOLEVEL");
		}

		// 汎用向け道路・歩行者リンク相対高さレイヤ
		namespace height_node
		{
			LPCTSTR kLayerName = _T("height_node");
			
			LPCTSTR kLinkId1 = _T("LINK_ID1");
			LPCTSTR kDivId1 = _T("DIVID1");
			LPCTSTR kRelHeight1 = _T("RELHEIGHT1");
			LPCTSTR kLinkId2 = _T("LINK_ID2");
			LPCTSTR kDivId2 = _T("DIVID2");
			LPCTSTR kRelHeight2 = _T("RELHEIGHT2");
		}
	}
	// 道路標高
	namespace road_alt
	{
		LPCTSTR kSuffix = _T("RAP");

		LPCTSTR kLinkId = _T("LINK_ID");
		LPCTSTR kNodeId = _T("NODE_ID");
		LPCTSTR kSequence = _T("SEQUENCE");
		LPCTSTR kHeightF = _T("HEIGHT_F");
	}
}

}
