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
#include <boost/preprocessor.hpp>
#include <sindy/schema/global.h>

namespace sindy {

namespace schema {

namespace global{

namespace middlemesh {
	const _TCHAR kTableName[]       = _T("PRIMARYMESH");  //!< テーブル名
	const _TCHAR kObjectID[]        = _T("OBJECTID");      //!< ObjectID
	const _TCHAR kMeshCode[]        = _T("MESHCODE");      //!< メッシュコード
	const _TCHAR kShape[]           = _T("SHAPE");
} // middlemesh

namespace basemesh {
	const _TCHAR kTableName[]       = _T("SECONDARYMESH");  //!< テーブル名
	const _TCHAR kObjectID[]        = _T("OBJECTID");      //!< ObjectID
	const _TCHAR kMeshCode[]        = _T("MESHCODE");      //!< メッシュコード
	const _TCHAR kShape[]           = _T("SHAPE");
} // basemesh

namespace citymesh {
	const _TCHAR kTableName[]       = _T("TERTIARYMESH");  //!< テーブル名
	const _TCHAR kObjectID[]        = _T("OBJECTID");      //!< ObjectID
	const _TCHAR kMeshCode[]        = _T("MESHCODE");      //!< メッシュコード
	const _TCHAR kCreateYear[]      = _T("CREATE_YEAR");   //!< 作成年度
	const _TCHAR kModifyYear[]      = _T("MODIFY_YEAR");   //!< 更新年度
	const _TCHAR kCompleteF[]       = _T("COMPLETE_F");    //!< 完全化フラグ
	const _TCHAR kShape[]           = _T("SHAPE");
} // citymesh

const _TCHAR object::kObjectID[]    = _T("OBJECTID");

namespace common_table {
	const _TCHAR kOperator[]        = _T("OPERATOR");
	const _TCHAR kPurpose[]         = _T("PURPOSE_C");
	const _TCHAR kModifyDate[]      = _T("MODIFYDATE");
	const _TCHAR kUpdateType[]      = _T("UPDATETYPE_C");
	const _TCHAR kProgModifyDate[]  = _T("PROGMODIFYDATE");
	const _TCHAR kModifyProgName[]  = _T("MODIFYPROGNAME");
	const _TCHAR kUserClaim[]       = _T("USERCLAIM_F");
	const _TCHAR kSource[]          = _T("SOURCE");
} // common_table

namespace country_table {
	const _TCHAR kCountryCode[]     = _T("COUNTRY_C"); //!< 国コード
} // country_table

namespace common_feature {
	const _TCHAR kShape[]           = _T("SHAPE");
} // common_feature

namespace country_feature {
	const _TCHAR kShape[]           = _T("SHAPE");
} // country_feature

namespace scaledisp_flag {
	const _TCHAR kSC1Disp[]         = _T("DISPSCALE1_F"); //!< スケール1表示フラグ
	const _TCHAR kSC2Disp[]         = _T("DISPSCALE2_F"); //!< スケール2表示フラグ
	const _TCHAR kSC3Disp[]         = _T("DISPSCALE3_F"); //!< スケール3表示フラグ
	const _TCHAR kSC4Disp[]         = _T("DISPSCALE4_F"); //!< スケール4表示フラグ
} // scaledisp_flag

namespace scale_class {
	const _TCHAR kSC1BgClass[]      = _T("SC1BGCLASS_C"); //!< スケール1背景種別コード
	const _TCHAR kSC2BgClass[]      = _T("SC2BGCLASS_C"); //!< スケール2背景種別コード
	const _TCHAR kSC3BgClass[]      = _T("SC3BGCLASS_C"); //!< スケール3背景種別コード
	const _TCHAR kSC4BgClass[]      = _T("SC4BGCLASS_C"); //!< スケール4背景種別コード
} // scale_class

namespace name_class {
	const _TCHAR kNameID[]          = _T("NAME_ID");      //!< 名称ID
} // name_class

namespace building {
	const _TCHAR kTableName[]       = _T("BUILDING");     //!< テーブル名
	const _TCHAR kBldClass[]        = _T("BLDCLASS_C");   //!< 建物種別コード
	const _TCHAR kCorridor[]        = _T("CORRIDOR_F");   //!< 渡り廊下フラグ
	const _TCHAR kNoWall[]          = _T("NOWALL_F");     //!< 無壁舎フラグ
	const _TCHAR kGeospaceID[]      = _T("GEOSPACE_ID");  //!< GEOSPACE_ID
} // building

namespace building_step {
	const _TCHAR kTableName[]       = _T("BUILDING_STEP"); //!< テーブル名
	const _TCHAR kFloors[]          = _T("FLOORS");        //!< 階数
	const _TCHAR kCheck[]           = _T("CHECK_F");       //!< 階数情報確認フラグ
	const _TCHAR kBuildingID[]      = _T("BUILDING_ID");   //!< 建物ID
} // building_step

namespace building_roof {
	const _TCHAR kTableName[]		= _T("BUILDING_ROOF");	//!< テーブル名
	const _TCHAR kRfClassC[]		= _T("RFCLASS_C");		//!< 建物屋上施設種別コード
} // building_roof

namespace building_line{
	const _TCHAR kTableName[]       = _T("BUILDING_LINE"); //!< テーブル名
	const _TCHAR kBldClass[]        = _T("BLDCLASS_C");    //!< 建物種別コード
} // building_line

namespace city_site {
	const _TCHAR kTableName[]       = _T("CITY_SITE");	    //!< テーブル名
	const _TCHAR kBgClass[]         = _T("BGCLASS_C");      //!< 背景ポリゴン種別コード
} // city_site

namespace city_line {
	const _TCHAR kTableName[]       = _T("CITY_LINE");	    //!< テーブル名
	const _TCHAR kBgClass[]         = _T("BGCLASS_C");      //!< 背景ライン種別コード
} // city_line

namespace city_tunnel {
	const _TCHAR kTableName[]       = _T("CITY_TUNNEL");	//!< テーブル名
	const _TCHAR kTunnelClass[]     = _T("TUNNELCLASS_C");  //!< トンネル種別コード
} // city_tunnel

namespace city_railway {
	const _TCHAR kTableName[]       = _T("CITY_RAILWAY");	//!< テーブル名
	const _TCHAR kRailWayClassC[]   = _T("RAILWAYCLASS_C"); //!< 鉄道種別コード
} // city_railway

namespace city_station {
	const _TCHAR kTableName[]       = _T("CITY_STATION");   //!< テーブル名
	const _TCHAR kUnderGround[]     = _T("UNDERGROUND_F");  //!< 地下フラグ
	const _TCHAR kStationClass[]    = _T("STATIONCLASS_C"); //!< 鉄道駅種別コード
} // city_station

namespace city_divider {
	const _TCHAR kTableName[]       = _T("CITY_DIVIDER");	//!< テーブル名
	const _TCHAR kDividerClassC[]   = _T("DIVIDERCLASS_C"); //!< 料金所分離帯種別コード
} // city_divider

namespace base_line {
	const _TCHAR kTableName[]       = _T("BASE_LINE");      //!< テーブル名

	const _TCHAR kUnderGround[]     = _T("UNDERGROUND_F");  //!< 地下フラグ
	const _TCHAR kBgClass[]         = _T("BGCLASS_C");      //!< 背景ライン種別コード
} // base_line

namespace base_railway {
	const _TCHAR kTableName[]       = _T("BASE_RAILWAY");   //!< テーブル名
	const _TCHAR kUnderGround[]     = _T("UNDERGROUND_F");  //!< 地下フラグ
	const _TCHAR kRailwayClass[]    = _T("RAILWAYCLASS_C"); //!< 鉄道種別コード
} // base_railway

namespace land_use {
	const _TCHAR kTableName[]       = _T("LAND_USE");       //!< テーブル名
} // land_use

namespace base_station {
	const _TCHAR kTableName[]       = _T("BASE_STATION");   //!< テーブル名

	const _TCHAR kUnderGround[]     = _T("UNDERGROUND_F");  //!< 地下フラグ
	const _TCHAR kStationClass[]    = _T("STATIONCLASS_C"); //!< 鉄道駅種別コード
} // base_station

namespace water_way {
	const _TCHAR kTableName[]       = _T("WATERWAY");       //!< テーブル名
} // water_way

namespace waterway_line {
	const _TCHAR kTableName[]       = _T("WATERWAY_LINE");       //!< テーブル名
	
	const _TCHAR kBgClass[]         = _T("BGCLASS_C");      //!< 背景種別
} // waterway_line

namespace land_cover {
	const _TCHAR kTableName[]       = _T("LAND_COVER");     //!< テーブル名
} // land_cover

namespace island {
	const _TCHAR kTableName[]       = _T("ISLAND");         //!< テーブル名
} // island

namespace from_to_node_relational_table {
	const _TCHAR kFromNodeID[]      = _T("FROM_NODE_ID");   //!< 始点側ノードのOBJECTID
	const _TCHAR kToNodeID[]        = _T("TO_NODE_ID");     //!< 終点側ノードのOBJECTID
} // from_to_node_relational_table

namespace highway_road_relational_table {
	const _TCHAR kRoadCode[]        = _T("ROAD_CODE");      //!< 路線コード
} // highway_road_relational_table

namespace fieldsurvey_table {
	const _TCHAR kFieldSurvey[]     = _T("FIELDSURVEY_F");  //!< 現地調査フラグ
	const _TCHAR kWorkYear[]        = _T("WORK_YEAR");      //!< 現地調査年度
} // fieldsurvey_table

const _TCHAR mesh_code_table::kMeshCode[]                  = _T("MESHCODE");
const _TCHAR link_relational_table::kLinkID[]              = _T("LINK_ID");
const _TCHAR directional_link_relational_table::kLinkDir[] = _T("LINKDIR_C");

namespace link_queue {
	const _TCHAR kInfID[]           = _T("INF_ID");
	const _TCHAR kSequence[]        = _T("SEQUENCE");
	const _TCHAR kLastLink[]        = _T("LASTLINK_F");
}

namespace time_condition_table {
	const _TCHAR kStartMonth[]      = _T("START_MONTH");
	const _TCHAR kStartDay[]        = _T("START_DAY");
	const _TCHAR kStartHour[]       = _T("START_HOUR");
	const _TCHAR kStartMin[]        = _T("START_MIN");
	const _TCHAR kEndMonth[]        = _T("END_MONTH");
	const _TCHAR kEndDay[]          = _T("END_DAY");
	const _TCHAR kEndHour[]         = _T("END_HOUR");
	const _TCHAR kEndMin[]          = _T("END_MIN");
	const _TCHAR kDayOfWeek[]       = _T("DAYOFWEEK_C");
}

namespace road_link {
	const _TCHAR kTableName[]       = _T("ROAD_LINK");

	const _TCHAR kRoadClass[]       = _T("ROADCLASS_C");
	const _TCHAR kNaviClass[]       = _T("NAVICLASS_C");
	const _TCHAR kMainLinkClass[]   = _T("MAIN_LINKCLASS_C");
	const _TCHAR kSubLinkClass[]    = _T("SUB_LINKCLASS_C");
	const _TCHAR kAdminCode[]       = _T("ADMIN_CODE");
	const _TCHAR kRoadManager[]     = _T("ROADMANAGER_C");
	const _TCHAR kRoadWidth[]       = _T("ROADWIDTH_C");
	const _TCHAR kBypass[]          = _T("BYPASS_F");
	const _TCHAR kCarOnly[]         = _T("CARONLY_F");
	const _TCHAR kWideArea[]        = _T("WIDEAREA_F");
	const _TCHAR kIsland[]          = _T("ISLAND_F");
	const _TCHAR kRoadNo[]          = _T("ROAD_NO");
	const _TCHAR kRoadNoString[]    = _T("ROAD_NO_STRING");
	const _TCHAR kRoadNoString2[]   = _T("ROAD_NO_STRING2");
	const _TCHAR kUpDownClass[]     = _T("UPDOWNCLASS_C");
	const _TCHAR kNoPassage[]       = _T("NOPASSAGE_C");
	const _TCHAR kOneway[]          = _T("ONEWAY_C");
	const _TCHAR kKubun[]           = _T("KUBUN_C");
	const _TCHAR kTripSpeed[]       = _T("TRIP_SPEED");
	const _TCHAR kLimitSpeed[]      = _T("LIMITSPEED_C");
	const _TCHAR kLaneCount[]       = _T("LANE_COUNT");
	const _TCHAR kLaneValid[]       = _T("LANEVALID_F");
	const _TCHAR kRightChange[]     = _T("RIGHT_CHANGE");
	const _TCHAR kLeftChange[]      = _T("LEFT_CHANGE");
	const _TCHAR kAlleyMainte[]     = _T("ALLEYMAINTE_C");

	const _TCHAR kNoPassageRF[]     = _T("NOPASSAGE_RF");
	const _TCHAR kOnewayRF[]        = _T("ONEWAY_RF");
	const _TCHAR kDupliLinkRF[]     = _T("DUPLINK_RF");

	const _TCHAR kTurnRegLQRF[]     = _T("TURNREG_LQRF");
	const _TCHAR kDirGuideLQRF[]    = _T("DIRGUIDE_LQRF");
	const _TCHAR kGuideLQRF[]       = _T("GUIDE_LQRF");
	const _TCHAR kLaneLQRF[]        = _T("LANE_LQRF");
	const _TCHAR kVICSLQRF[]        = _T("VICS_LQRF");
	const _TCHAR kBywayLQRF[]       = _T("BYWAY_LQRF");
	const _TCHAR kRouteLQRF[]       = _T("ROUTE_LQRF");
	const _TCHAR kUturnLQRF[]       = _T("UTURN_LQRF");
	const _TCHAR kInternaviLQRF[]   = _T("INTERNAVI_LQRF");
	const _TCHAR kIpcVicsLQRF[]     = _T("IPCVICS_LQRF");
	const _TCHAR kIntersectionLQRF[] = _T("INTERSECTION_LQRF");

	const _TCHAR kParkingLinkClass[] = _T("PARKINGLINKCLASS_C");
	const _TCHAR kFloorLevel[]       = _T("FLOOR_LEVEL");
	const _TCHAR kMiddleFloorLevel[] = _T("MIDDLEFLOOR_LEVEL");

	const _TCHAR kLegalSpeed[]      = _T("LEGALSPEED_C");
	const _TCHAR kLegalSpeedVerified[] = _T("LEGALSPEED_VERIFIED_C");

	const _TCHAR kVehicleTurnRegLQRF[] = _T("VEHICLE_TURNREG_LQRF");
	const _TCHAR kVehicleRegRF[]       = _T("VEHICLE_REG_RF");

	const _TCHAR kFromHNLeft[]    = _T("FROM_HN_LEFT");
	const _TCHAR kToHNLeft[]      = _T("TO_HN_LEFT");
	const _TCHAR kFromHNRight[]   = _T("FROM_HN_RIGHT");
	const _TCHAR kToHNRight[]     = _T("TO_HN_RIGHT");
	const _TCHAR kHNSchemeLeft[]  = _T("HN_SCHEME_LEFT");
	const _TCHAR kHNSchemeRight[] = _T("HN_SCHEME_RIGHT");
	const _TCHAR kHNType[]        = _T("HN_TYPE");
}

namespace road_node {
	const _TCHAR kTableName[]       = _T("ROAD_NODE");

	const _TCHAR kNodeClass[]       = _T("NODECLASS_C");
	const _TCHAR kSignal[]          = _T("SIGNAL_C");
	const _TCHAR kNameKanji[]       = _T("NAME_KANJI");
	const _TCHAR kNameYomi[]        = _T("NAME_YOMI");
	const _TCHAR kHeightCode[]      = _T("HEIGHT_C");
	const _TCHAR kHeight[]          = _T("HEIGHT");
	const _TCHAR kParkingID[]       = _T("PARKING_ID");
}

namespace segment_attr {
	const _TCHAR kTableName[]       = _T("SEGMENT_ATTR");

	const _TCHAR kLinkAttr[]        = _T("LINKATTR_C");
	const _TCHAR kUnderOver[]       = _T("UNDER_OVER");
}

namespace inf_turnreg {
	const _TCHAR kTableName[]       = _T("INF_TURNREG");

	const _TCHAR kStartMonth0[]     = _T("START_MONTH");
	const _TCHAR kStartDay0[]       = _T("START_DAY");
	const _TCHAR kStartHour0[]      = _T("START_HOUR");
	const _TCHAR kStartMin0[]       = _T("START_MIN");
	const _TCHAR kEndMonth0[]       = _T("END_MONTH");
	const _TCHAR kEndDay0[]         = _T("END_DAY");
	const _TCHAR kEndHour0[]        = _T("END_HOUR");
	const _TCHAR kEndMin0[]         = _T("END_MIN");
	const _TCHAR kDayOfWeek0[]      = _T("DAYOFWEEK_C");

#define DECL(z, n, data)                                        \
	const _TCHAR kStartMonth##n[]   = _T("START_MONTH") _T(#n); \
	const _TCHAR kStartDay##n[]     = _T("START_DAY")   _T(#n); \
	const _TCHAR kStartHour##n[]    = _T("START_HOUR")  _T(#n); \
	const _TCHAR kStartMin##n[]     = _T("START_MIN")   _T(#n); \
	const _TCHAR kEndMonth##n[]     = _T("END_MONTH")   _T(#n); \
	const _TCHAR kEndDay##n[]       = _T("END_DAY")     _T(#n); \
	const _TCHAR kEndHour##n[]      = _T("END_HOUR")    _T(#n); \
	const _TCHAR kEndMin##n[]       = _T("END_MIN")     _T(#n); \
	const _TCHAR kDayOfWeek##n[]    = _T("DAYOFWEEK")   _T(#n) _T("_C");
	BOOST_PP_REPEAT_FROM_TO(1, 5, DECL, _)
#undef DECL

	const LPCTSTR kStartMonth[5]    = { BOOST_PP_ENUM_PARAMS(5, kStartMonth) };
	const LPCTSTR kStartDay[5]      = { BOOST_PP_ENUM_PARAMS(5, kStartDay) };
	const LPCTSTR kStartHour[5]     = { BOOST_PP_ENUM_PARAMS(5, kStartHour) };
	const LPCTSTR kStartMin[5]      = { BOOST_PP_ENUM_PARAMS(5, kStartMin) };
	const LPCTSTR kEndMonth[5]      = { BOOST_PP_ENUM_PARAMS(5, kEndMonth) };
	const LPCTSTR kEndDay[5]        = { BOOST_PP_ENUM_PARAMS(5, kEndDay) };
	const LPCTSTR kEndHour[5]       = { BOOST_PP_ENUM_PARAMS(5, kEndHour) };
	const LPCTSTR kEndMin[5]        = { BOOST_PP_ENUM_PARAMS(5, kEndMin) };
	const LPCTSTR kDayOfWeek[5]     = { BOOST_PP_ENUM_PARAMS(5, kDayOfWeek) };
}

namespace inf_dirguide {
	const _TCHAR kTableName[]    = _T("INF_DIRGUIDE");
	const int    kMaxNumber      = 10;

	const _TCHAR kGuideClass0[]  = _T("GUIDECLASS_C");
	const _TCHAR kNameKanji0[]   = _T("NAME_KANJI");
	const _TCHAR kNameYomi0[]    = _T("NAME_YOMI");

#define DECL(z, n, data)                                              \
	const _TCHAR kGuideClass##n[] = _T("GUIDECLASS") _T(#n) _T("_C"); \
	const _TCHAR kNameKanji##n[]  = _T("NAME_KANJI") _T(#n);           \
	const _TCHAR kNameYomi##n[]   = _T("NAME_YOMI") _T(#n);
	BOOST_PP_REPEAT_FROM_TO(1, 11, DECL, _)
#undef DECL

	const LPCTSTR kGuideClass[11] = { BOOST_PP_ENUM_PARAMS(11, kGuideClass) };
	const LPCTSTR kNameKanji[11]  = { BOOST_PP_ENUM_PARAMS(11, kNameKanji) };
	const LPCTSTR kNameYomi[11]   = { BOOST_PP_ENUM_PARAMS(11, kNameYomi) };

	const _TCHAR kGuidePattern[] = _T("GUIDEPATTERN_C");
	const _TCHAR kSequence[]     = _T("SEQUENCE");
}

namespace inf_guide {
	const _TCHAR kTableName[] = _T("INF_GUIDE");

	const _TCHAR kGuideType[] = _T("GUIDETYPE_C");
	const _TCHAR kDelete[]    = _T("DELETE_F");
}

namespace inf_lane {
	const _TCHAR kTableName[]    = _T("INF_LANE");

	const _TCHAR kLaneDir[]      = _T("LANEDIR_C");
	const _TCHAR kApproachLane[] = _T("APPROACH_LANE");
	const _TCHAR kRightChange[]  = _T("RIGHT_CHANGE");
	const _TCHAR kLeftChange[]   = _T("LEFT_CHANGE");
	const _TCHAR kFlowLane[]     = _T("FLOW_LANE");
	const _TCHAR kLanePassable[] = _T("LANE_PASSABLE");
}

const _TCHAR lq_turnreg::kTableName[]  = _T("LQ_TURNREG");

const _TCHAR lq_dirguide::kTableName[] = _T("LQ_DIRGUIDE");

const _TCHAR lq_guide::kTableName[]    = _T("LQ_GUIDE");

const _TCHAR lq_lane::kTableName[]     = _T("LQ_LANE");

const _TCHAR inf_vehicle_turnreg::kTableName[] = _T("INF_VEHICLE_TURNREG");

const _TCHAR lq_vehicle_turnreg::kTableName[]  = _T("LQ_VEHICLE_TURNREG");

namespace vehicle_turnreg {
	const _TCHAR kTableName[]       = _T("VEHICLE_TURNREG");

	const _TCHAR kInfID[]           = _T("INF_ID");
	const _TCHAR kHeight[]          = _T("HEIGHT");
	const _TCHAR kWidth[]           = _T("WIDTH");
	const _TCHAR kWeight[]          = _T("WEIGHT");
	const _TCHAR kLength[]          = _T("LENGTH");
	const _TCHAR kLoadAge[]         = _T("LOADAGE");
	const _TCHAR kDangerF[]         = _T("DANGER_F");

	const _TCHAR kStartMonth[]      = _T("START_MONTH");
	const _TCHAR kStartDay[]        = _T("START_DAY");
	const _TCHAR kStartHour[]       = _T("START_HOUR");
	const _TCHAR kStartMin[]        = _T("START_MIN");
	const _TCHAR kEndMonth[]        = _T("END_MONTH");
	const _TCHAR kEndDay[]          = _T("END_DAY");
	const _TCHAR kEndHour[]         = _T("END_HOUR");
	const _TCHAR kEndMin[]          = _T("END_MIN");
	const _TCHAR kDayOfWeek[]       = _T("DAYOFWEEK_C");
} // vehicle_turnreg

const _TCHAR time_oneway::kTableName[] = _T("TIME_ONEWAY");

namespace time_nopassage {
	const _TCHAR kTableName[]       = _T("TIME_NOPASSAGE");

	const _TCHAR kSeasonReg[]       = _T("SEASONREG_F");
}

namespace vehicle_reg {
	const _TCHAR kTableName[]       = _T("VEHICLE_REG");

	const _TCHAR kLinkID[]          = _T("LINK_ID");
	const _TCHAR kRegTypeClass[]    = _T("REGTYPE_C");
	const _TCHAR kHeight[]          = _T("HEIGHT");
	const _TCHAR kWidth[]           = _T("WIDTH");
	const _TCHAR kWeight[]          = _T("WEIGHT");
	const _TCHAR kLength[]          = _T("LENGTH");
	const _TCHAR kLoadAge[]         = _T("LOADAGE");
	const _TCHAR kDangerF[]         = _T("DANGER_F");

	const _TCHAR kStartMonth[]      = _T("START_MONTH");
	const _TCHAR kStartDay[]        = _T("START_DAY");
	const _TCHAR kStartHour[]       = _T("START_HOUR");
	const _TCHAR kStartMin[]        = _T("START_MIN");
	const _TCHAR kEndMonth[]        = _T("END_MONTH");
	const _TCHAR kEndDay[]          = _T("END_DAY");
	const _TCHAR kEndHour[]         = _T("END_HOUR");
	const _TCHAR kEndMin[]          = _T("END_MIN");
	const _TCHAR kDayOfWeek[]       = _T("DAYOFWEEK_C");
} // vehicle_reg

namespace vehicle_type {
	extern const _TCHAR kTableName[]      = _T("VEHICLE_TYPE");

	extern const _TCHAR kRegID[]          = _T("REG_ID");
	extern const _TCHAR kVehicleRegCode[] = _T("VEHICLE_REG_CODE");
	extern const _TCHAR kExcludeF[]       = _T("EXCLUDE_F");
} // vehicle_type

namespace vehicle_reg_master {
	extern const _TCHAR kTableName[]       = _T("VEHICLE_REG_MASTER");

	extern const _TCHAR kVehicleRegCode[]  = _T("VEHICLE_REG_CODE");
	extern const _TCHAR kVehicleRegName[]  = _T("VEHICLE_REG_NAME");
	extern const _TCHAR kPassengerF[]      = _T("PASSENGER_F");
	extern const _TCHAR kTaxiF[]           = _T("TAXI_F");
	extern const _TCHAR kPublicBusF[]      = _T("PUBLIC_BUS_F"); 
	extern const _TCHAR kPrivateBusF[]     = _T("PRIVATE_BUS_F"); 
	extern const _TCHAR kTransportTruckF[] = _T("TRANSPORT_TRUCK_F"); 
	extern const _TCHAR kSignageIllustFile[] = _T("SIGNAGE_ILLUST_FILE");
} // vehicle_reg_master

namespace road_code_list {
	const _TCHAR kTableName[] = _T("ROAD_CODE_LIST");

	const _TCHAR kOperator[]     = _T("OPERATOR");
	const _TCHAR kModifyDate[]   = _T("MODIFYDATE");
	const _TCHAR kRoadCode[]     = _T("ROAD_CODE");
	const _TCHAR kNameKanji[]    = _T("NAME_KANJI");
	const _TCHAR kNameYomi[]     = _T("NAME_YOMI");
	const _TCHAR kWorkKanji[]    = _T("WORK_KANJI");
	const _TCHAR kWorkYomi[]     = _T("WORK_YOMI");
	const _TCHAR kLineClass[]    = _T("LINECLASS_C");
	const _TCHAR kLineDirType[]  = _T("LINEDIRTYPE_C");
	const _TCHAR kLandClass[]    = _T("LANDCLASS_C");
}

namespace admin_class {
	const _TCHAR kAdminCode[]       = _T("ADMIN_CODE");
	const _TCHAR kPopulation[]      = _T("POPULATION");
}

namespace built_up_area {
	const _TCHAR kTableName[]		= _T("BUILT_UP_AREA");

	const _TCHAR kAdminCode[]       = _T("ADMIN_CODE");
	const _TCHAR kPopulation[]      = _T("POPULATION");
}

namespace country {
	const _TCHAR kTableName[]       = _T("COUNTRY");       

	const _TCHAR kAdminCode[]       = _T("ADMIN_CODE");      
	const _TCHAR kLangC[]           = _T("LANGUAGE_C");    
	const _TCHAR kDriveSide[]       = _T("DRIVE_SIDE");     
	const _TCHAR kSpeedLimitUnit[]  = _T("SPEED_LIMIT_UNIT");  
	const _TCHAR kTimeZone[]        = _T("TIME_ZONE");       
	const _TCHAR kPhoneCountryC[]   = _T("PHONE_COUNTRY_CODE");   
	const _TCHAR kPhonePrefix[]     = _T("PHONE_PREFIX");   
	const _TCHAR kPOIAddrDispType[] = _T("POI_ADDRESS_DISP_TYPE"); 
}

namespace province {
	const _TCHAR kTableName[]       = _T("PROVINCE");
}

namespace district {
	const _TCHAR kTableName[]       = _T("DISTRICT");
}

namespace sub_district {
	const _TCHAR kTableName[]       = _T("SUBDISTRICT");
}

namespace village {
	const _TCHAR kTableName[]       = _T("VILLAGE");
}

namespace named_area {
	const _TCHAR kTableName[]       = _T("NAMEDAREA");
}

namespace postal_district {
	const _TCHAR kTableName[]       = _T("POSTALDISTRICT");
	const _TCHAR kPostalCode[]      = _T("POSTAL_CODE");
}

namespace hnp {
	const _TCHAR kTableName[]       = _T("HNP");
	const _TCHAR kHn[]              = _T("HN");
	const _TCHAR kHnType[]          = _T("HN_TYPE");
	const _TCHAR kLinkID[]          = _T("LINK_ID");
    const _TCHAR kRoadNameID[]      = _T("ROADNAMEID");
	const _TCHAR kHnRangeLineID[]   = _T("HN_RANGELINE_ID");
}

namespace hnp_entry_point {
	const _TCHAR kTableName[]       = _T("HNP_ENTRYPOINT");
	const _TCHAR kHnpPointID[]      = _T("HNP_POINT_ID");
	const _TCHAR kAccuracyCode[]    = _T("ACCURACY_C");
}

namespace postal_point{
 	const _TCHAR kTableName[]       = _T("POSTALPOINT");
 
 	const _TCHAR kPostalCode[]      = _T("POSTAL_CODE");
 	const _TCHAR kRoadLinkID[]      = _T("LINK_ID");
	const _TCHAR kRoadNameID[]      = _T( "ROADNAMEID" );
 }

namespace postal_entry_point {
	const _TCHAR kTableName[]       = _T("POSTALENTRYPOINT");
	const _TCHAR kPostalPointID[]   = _T("POSTAL_POINT_ID");
	const _TCHAR kAccuracyCode[]    = _T("ACCURACY_C");
}

namespace hnr {
	const _TCHAR kTableName[]       = _T("HNR");
	const _TCHAR kFromHnLeft[]      = _T("FROM_HN_LEFT");
	const _TCHAR kToHnLeft[]        = _T("TO_HN_LEFT"); 
	const _TCHAR kFromHnRight[]     = _T("FROM_HN_RIGHT");
	const _TCHAR kToHnRight[]       = _T("TO_HN_RIGHT");
	const _TCHAR kHnSchemeLeft[]    = _T("HN_SCHEME_LEFT");
	const _TCHAR kHnSchemeRight[]   = _T("HN_SCHEME_RIGHT");
	const _TCHAR kHnType[]          = _T("HN_TYPE");
	const _TCHAR kLinkID[]          = _T("LINK_ID");
}

namespace poi_info {
	const _TCHAR kTableName[]           = _T("POI_INFO");
	const _TCHAR kUpdAvailDate[]        = _T("UPDAVAILDATE");
	const _TCHAR kAccuracyLevel[]       = _T("ACCURACY_C");
	const _TCHAR kAdminCode[]           = _T("ADMINCODE");
	const _TCHAR kRoadNameID[]          = _T("ROADNAMEID");
	const _TCHAR kProductType[]         = _T("PRODUCT_C");
	const _TCHAR kHouseNumber[]         = _T("HOUSENUMBER");
	const _TCHAR kLanguageCode[]        = _T("LANGUAGE_C");
	const _TCHAR kActualAddress[]       = _T("ACTUALADDRESS");
	const _TCHAR kLanguageTransCode[]   = _T("LANGUAGE_TRANS_C");
	const _TCHAR kActualAddressTrans[]  = _T("ACTUALADDRESS_TRANS");
	const _TCHAR kCategoryCode[]        = _T("CATEGORY_C");
	const _TCHAR kSubCategoryCode[]     = _T("SUBCATEGORY_C");
	const _TCHAR kChainMasterCode[]     = _T("CHAIN_MASTER_CODE");
	const _TCHAR kDetailInfo[]          = _T("DETAILINFO");
	const _TCHAR kDeletionCode[]        = _T("DELETION_C");
	const _TCHAR kFuelType[]            = _T("FUELTYPE");
}

namespace poi_chain_master {
	const _TCHAR kTableName[]		= _T("POI_CHAIN_MASTER");
	const _TCHAR kChainCode[]		= _T("CHAIN_CODE");
	const _TCHAR kNameLatin[]		= _T("NAME_LATIN");
	const _TCHAR kNameLocal[]		= _T("NAME_LOCAL");
	const _TCHAR kLocalLanguageC[]	= _T("LOCAL_LANGUAGE_C");
}

namespace poi_entry_point {
	const _TCHAR kTableName[]         = _T("POI_ENTRYPOINT");
	const _TCHAR kPoiInfoID[]         = _T("POIINFOID");
	const _TCHAR kPriorityF[]         = _T("PRIORITY_F");
	const _TCHAR kEntryPointCode[]    = _T("ENTRYPOINT_C");
	const _TCHAR kAccuracyCode[]      = _T("ACCURACY_C");
}

namespace poi_phone {
	const _TCHAR kTableName[]         = _T("POI_PHONE");
	const _TCHAR kPoiInfoID[]        = _T("POIINFOID");
	const _TCHAR kPhonePriorityF[]    = _T("PRIORITY_F");
	const _TCHAR kPhoneType[]         = _T("PHONE_C");
	const _TCHAR kPhoneNumber[]       = _T("PHONENUMBER");
}

namespace poi_association {
	const _TCHAR kTableName[]         = _T("POI_ASSOCIATION");
	const _TCHAR kParentID[]          = _T("PARENTID");
	const _TCHAR kChildID[]           = _T("CHILDID");
}

namespace top_annotation{
	const _TCHAR kTableName[]         = _T("TOP_ANNOTATION");
	const _TCHAR kAnnoClassC[]        = _T("ANNOCLASS_C");
}

namespace top_country{
	const _TCHAR kTableName[]         = _T("TOP_COUNTRY");
	const _TCHAR kCountryC[]          = _T("COUNTRY_C");
}

namespace top_province{
	const _TCHAR kTableName[]         = _T("TOP_PROVINCE");
}

namespace top_waterway_line{
	const _TCHAR kTableName[]         = _T("TOP_WATERWAY_LINE");
	const _TCHAR kBgClassC[]          = _T("BGCLASS_C");
}

namespace top_waterway_polygon{
	const _TCHAR kTableName[]         = _T("TOP_WATERWAY_POLYGON");
	const _TCHAR kBgClassC[]          = _T("BGCLASS_C");
}

namespace auto_annotation{
	const _TCHAR kTableName[]         = _T("AUTO_ANNOTATION");
	const _TCHAR kOrgID[]             = _T("ORG_ID");
	const _TCHAR kMainName[]          = _T("MAIN_NAME");
	const _TCHAR kMainLanguageC[]     = _T("MAIN_LANGUAGE_C");
	const _TCHAR kSubName[]           = _T("SUB_NAME");
	const _TCHAR kSubLanguageC[]      = _T("SUB_LANGUAGE_C");
	const _TCHAR kLayerC[]            = _T("LAYER_C");
	const _TCHAR kAnnoCode[]          = _T("ANNOCODE");
	const _TCHAR kAnnoAngle[]         = _T("ANNO_ANGLE");
}

namespace anno_common{
	const _TCHAR kOrgID[]             = _T("ORG_ID");
	const _TCHAR kRoadNoString[]      = _T("ROAD_NO_STRING");
	const _TCHAR kLayerC[]            = _T("LAYER_C");
	const _TCHAR kAnnoCode[]          = _T("ANNOCODE");
}

namespace anno_base_railway{
	const _TCHAR kTableName[]         = _T("ANNO_BASE_RAILWAY");
}

namespace anno_road_name{
	const _TCHAR kTableName[]         = _T("ANNO_ROAD_NAME");
}

namespace anno_road_no{
	const _TCHAR kTableName[]         = _T("ANNO_ROAD_NO");
}

namespace official_name {
	const _TCHAR kTableName[]       = _T("OFFICIAL_NAME");  //!< テーブル名
	const _TCHAR kLayerCode[]       = _T("LAYER_C");        //!< レイヤコード
	const _TCHAR kName[]            = _T("NAME");           //!< 名称
	const _TCHAR kLanguageCode[]    = _T("LANGUAGE_C");     //!< 言語コード
	const _TCHAR kDisplayF[]        = _T("DISPLAY_F");        //!< DisplayFlag
} // official_name

namespace alternative_name {
	const _TCHAR kTableName[]       = _T("ALTERNATIVE_NAME"); //!< テーブル名
	const _TCHAR kOfficialID[]      = _T("OFFICIAL_ID");      //!< オフィシャルID
	const _TCHAR kName[]            = _T("NAME");             //!< 名称
	const _TCHAR kLanguageCode[]    = _T("LANGUAGE_C");       //!< 言語コード
	const _TCHAR kSequence[]        = _T("SEQUENCE");         //!< シーケンス
	const _TCHAR kDisplayF[]        = _T("DISPLAY_F");        //!< DisplayFlag
} // alternative_name

namespace translation {
	const _TCHAR kTableName[]       = _T("TRANSLATION");    //!< テーブル名
	const _TCHAR kTypeF[]           = _T("TYPE_F");         //!< 0:Official, 1:Alternative
	const _TCHAR kID[]              = _T("TRANS_ID");       //!< [Official or Alternative] ObjectID
	const _TCHAR kName[]            = _T("NAME");           //!< 名称
	const _TCHAR kLanguageCode[]    = _T("LANGUAGE_C");     //!< 言語コード
	const _TCHAR kDisplayF[]        = _T("DISPLAY_F");        //!< DisplayFlag
} // translation

namespace base_border_line {
	const _TCHAR kTableName[]			= _T("BASE_BORDER_LINE");	//!< テーブル名
	const _TCHAR kCountryType[]			= _T("COUNTRY_TYPE");		//!< CountryType
	const _TCHAR kOceanF[]				= _T("OCEAN_F");			//!< OceanFlag
} // base_border_line

namespace border_point {
	const _TCHAR kTableName[]			= _T("BORDER_POINT");		//!< テーブル名
	const _TCHAR kBorderPointCode[]		= _T("BORDER_POINT_C");		//!< 境界コード
	const _TCHAR kCountryCode1[]		= _T("COUNTRY1_C");			//!< 国番号1
	const _TCHAR kOriginalObjectID1[]	= _T("OBJECTID1_ORG");		//!< オブジェクトID1
	const _TCHAR kCountryCode2[]		= _T("COUNTRY2_C");			//!< 国番号2
	const _TCHAR kOriginalObjectID2[]	= _T("OBJECTID2_ORG");		//!< オブジェクトID2

} // border_point

namespace release_country_zone {
	const _TCHAR kTableName[]       = _T("RELEASE_COUNTRY_ZONE");   //!< テーブル名
	const _TCHAR kCountryCode[]     = _T("COUNTRY_C");              //!< 国番号
} // release_country_zone

namespace sea_polygon {
	const _TCHAR kTableName[]       = _T("SEA_POLYGON");   //!< テーブル名
} // sea_polygon

namespace sea_polygon_name {
	const _TCHAR kTableName[]       = _T("SEA_POLYGON_NAME");   //!< テーブル名
	const _TCHAR kUnitF[]			= _T("UNIT_F");			   //!< 海名称ポリゴンユニットフラグ
} // sea_polygon_name

namespace land_complement {
	const _TCHAR kTableName[]       = _T("LAND_COMPLEMENT");   //!< テーブル名
} // sea_polygon_name

namespace top_border_line {
	const _TCHAR kTableName[]			= _T("TOP_BORDER_LINE");	//!< テーブル名
	const _TCHAR kCountryType[]			= _T("COUNTRY_TYPE");		//!< CountryType
	const _TCHAR kOceanF[]				= _T("OCEAN_F");			//!< OceanFlag
} // top_border_line

namespace ipc_diff_feature {
	const _TCHAR kCountry[]         = _T("COUNTRY_C");        //!< countryコード
	const _TCHAR kOrgId[]           = _T("OBJECTID_ORG");     //!< オブジェクトID
	const _TCHAR kChangeType[]      = _T("CHANGE_TYPE");      //!< changeコード
	const _TCHAR kCountryAlias[]    = _T("CountryCode");      //!< countryコード別名
	const _TCHAR kObjectIdAlias[]   = _T("OriginalObjectID"); //!< オブジェクトID別名
	const _TCHAR kChangeTypeAlias[] = _T("ChangeType");       //!< changeコード別名
}// diffattribute

namespace illust_link {
	const _TCHAR kTableName[] = _T("ILLUST_LINK");

	const _TCHAR kIllustClass[]           = _T("ILLUST_CLASS_C");
	const _TCHAR kCustomerCode[]          = _T("CUSTOMER_C");
	const _TCHAR kIllustFileName[]        = _T("ILLUST_FILENAME");
	const _TCHAR kGuideSeq[]              = _T("GUIDE_SEQ");
	const _TCHAR kDirectionCode[]         = _T("DIRECTION_C");
	const _TCHAR kUTurnFlag[]             = _T("UTURN_F");
	const _TCHAR kRoundaboutFlag[]        = _T("ROUNDABOUT_F");

	const int    kMaxNodeNumber = 20;
	const _TCHAR kNode0[]  = _T("ROAD_NODE_00");
	const _TCHAR kNode1[]  = _T("ROAD_NODE_01");
	const _TCHAR kNode2[]  = _T("ROAD_NODE_02");
	const _TCHAR kNode3[]  = _T("ROAD_NODE_03");
	const _TCHAR kNode4[]  = _T("ROAD_NODE_04");
	const _TCHAR kNode5[]  = _T("ROAD_NODE_05");
	const _TCHAR kNode6[]  = _T("ROAD_NODE_06");
	const _TCHAR kNode7[]  = _T("ROAD_NODE_07");
	const _TCHAR kNode8[]  = _T("ROAD_NODE_08");
	const _TCHAR kNode9[]  = _T("ROAD_NODE_09");
	const _TCHAR kNode10[] = _T("ROAD_NODE_10");
	const _TCHAR kNode11[] = _T("ROAD_NODE_11");
	const _TCHAR kNode12[] = _T("ROAD_NODE_12");
	const _TCHAR kNode13[] = _T("ROAD_NODE_13");
	const _TCHAR kNode14[] = _T("ROAD_NODE_14");
	const _TCHAR kNode15[] = _T("ROAD_NODE_15");
	const _TCHAR kNode16[] = _T("ROAD_NODE_16");
	const _TCHAR kNode17[] = _T("ROAD_NODE_17");
	const _TCHAR kNode18[] = _T("ROAD_NODE_18");
	const _TCHAR kNode19[] = _T("ROAD_NODE_19");
	const _TCHAR kNode20[] = _T("ROAD_NODE_20");

	const LPCTSTR kNode[21] = { BOOST_PP_ENUM_PARAMS(21, kNode) };
}// illust_link

namespace grad {
	const _TCHAR kTableName[] = _T("GRAD");

	const _TCHAR kGradValue[] = _T("GRAD_VALUE");
}// grad

} // global

} // schema

} // sindy
