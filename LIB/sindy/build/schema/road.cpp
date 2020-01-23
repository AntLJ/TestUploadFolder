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
#include <sindy/schema/road.h>

namespace sindy {

namespace schema {

const _TCHAR kObjectID[] = _T("OBJECTID");

namespace road_link {
	const _TCHAR kTableName[] = _T("ROAD_LINK");

	const _TCHAR kNaviClass[] = _T("NAVICLASS_C");
	const _TCHAR kMainLinkClass[] = _T("MAIN_LINKCLASS_C");
	const _TCHAR kSubLinkClass[] = _T("SUB_LINKCLASS_C");
	const _TCHAR kAdminCode[] = _T("ADMIN_CODE");
	const _TCHAR kRoadManager[] = _T("ROADMANAGER_C");
	const _TCHAR kRoadWidth[] = _T("ROADWIDTH_C");
	const _TCHAR kBypass[] = _T("BYPASS_F");
	const _TCHAR kCarOnly[] = _T("CARONLY_F");
	const _TCHAR kWideArea[] = _T("WIDEAREA_F");
	const _TCHAR kIsland[] = _T("ISLAND_F");
	const _TCHAR kUpDownClass[] = _T("UPDOWNCLASS_C");
	const _TCHAR kKubun[] = _T("KUBUN_C");
	const _TCHAR kTripSpeed[] = _T("TRIP_SPEED");
	const _TCHAR kLimitSpeed[] = _T("LIMITSPEED_C");
	const _TCHAR kLaneCount[] = _T("LANE_COUNT");
	const _TCHAR kLaneValid[] = _T("LANEVALID_F");
	const _TCHAR kRightChange[] = _T("RIGHT_CHANGE");
	const _TCHAR kLeftChange[] = _T("LEFT_CHANGE");
	const _TCHAR kAlleyMainte[] = _T("ALLEYMAINTE_C");

	const _TCHAR kDupliLinkRF[] = _T("DUPLINK_RF");

	const _TCHAR kTurnRegLQRF[] = _T("TURNREG_LQRF");
	const _TCHAR kDirGuideLQRF[] = _T("DIRGUIDE_LQRF");
	const _TCHAR kGuideLQRF[] = _T("GUIDE_LQRF");
	const _TCHAR kLaneLQRF[] = _T("LANE_LQRF");
	const _TCHAR kVICSLQRF[] = _T("VICS_LQRF");
	const _TCHAR kBywayLQRF[] = _T("BYWAY_LQRF");
	const _TCHAR kRouteLQRF[] = _T("ROUTE_LQRF");
	const _TCHAR kUturnLQRF[] = _T("UTURN_LQRF");
	const _TCHAR kInternaviLQRF[] = _T("INTERNAVI_LQRF");
	const _TCHAR kIpcVicsLQRF[] = _T("IPCVICS_LQRF");
	const _TCHAR kIntersectionLQRF[] = _T("INTERSECTION_LQRF");
	const _TCHAR kVehicleRegRF[] = _T("VEHICLE_REG_RF");    
	const _TCHAR kVehicleTurnRegRF[] = _T("VEHICLE_TURNREG_LQRF"); 
	const _TCHAR kWalkable[] = _T("WALKABLE_C");
	const _TCHAR kPedestrianZone[] = _T("PEDESTRIAN_ZONE_F");
	const _TCHAR kWheelChair[] = _T("WHEELCHAIR_C");

	const _TCHAR kParkingLinkClass[] = _T("PARKINGLINKCLASS_C");
	const _TCHAR kFloorLevel[] = _T("FLOOR_LEVEL");
	const _TCHAR kMiddleFloorLevel[] = _T("MIDDLEFLOOR_LEVEL");

	const _TCHAR kLegalSpeed[] = _T("LEGALSPEED_C");
}

namespace road_node {
	const _TCHAR kTableName[] = _T("ROAD_NODE");

	const _TCHAR kNodeClass[] = _T("NODECLASS_C");
	const _TCHAR kSignal[] = _T("SIGNAL_C");
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");
	const _TCHAR kHeightCode[] = _T("HEIGHT_C");
	const _TCHAR kHeight[] = _T("HEIGHT");
	const _TCHAR kParkingID[] = _T("PARKING_ID");
}

namespace time_nopassage {
	const _TCHAR kTableName[] = _T("TIME_NOPASSAGE");

	const _TCHAR kSeasonReg[] = _T("SEASONREG_F");
}

const _TCHAR time_oneway::kTableName[] = _T("TIME_ONEWAY");

namespace segment_attr {
	const _TCHAR kTableName[] = _T("SEGMENT_ATTR");

	const _TCHAR kLinkAttr[] = _T("LINKATTR_C");
	const _TCHAR kUnderOver[] = _T("UNDER_OVER");
	const _TCHAR kName[] = _T("NAME");
}

namespace walk_link{
	const _TCHAR kTableName[]       = _T("WALK_LINK");

	const _TCHAR kWalkClass[]       = _T("WALKCLASS_C");
	const _TCHAR kFloorLevel[]      = _T("FLOOR_LEVEL");
	const _TCHAR kFloorMoveClass[]  = _T("FLOOR_MOVE_C");
	const _TCHAR kAutoWalkFlag[]    = _T("AUTOWALK_F");
	const _TCHAR kStairFlag[]       = _T("STAIR_F");
	const _TCHAR kSlopeFlag[]       = _T("SLOPE_F");
	const _TCHAR kEscalatorFlag[]   = _T("ESCALATOR_F");
	const _TCHAR kArcadeFlag[]      = _T("ARCADE_F");
	const _TCHAR kOpenAreaFlag[]    = _T("OPEN_AREA_F");
	const _TCHAR kTunnelFlag[]      = _T("TUNNEL_F");
	const _TCHAR kRoadSurface[]     = _T("ROAD_SURFACE_C");
	const _TCHAR kLinkRelationRF[]  = _T("LINK_RELATION_RF");
	const _TCHAR kWalkCode[]        = _T("WALK_CODE");
	const _TCHAR kWheelChair[]      = _T("WHEELCHAIR_C");
	const _TCHAR kPlatformNumber[] = _T("PLATFORM_NUMBER");
	const _TCHAR kScrambleFlag[] = _T("SCRAMBLE_F");
}

namespace walk_node{
	const _TCHAR kTableName[]              = _T("WALK_NODE");

	const _TCHAR kNodeClass[]              = _T("NODECLASS_C");
	const _TCHAR kRoadNodeID[]             = _T("ROAD_NODE_ID");
	const _TCHAR kStationGatePointID[]     = _T("STATION_GATE_POINT_ID");
	const _TCHAR kUndergroundGatePointID[] = _T("UNDERGROUND_GATE_POINT_ID");
}

namespace grad {
	const _TCHAR kTableName[] = _T("GRAD");

	const _TCHAR kGradValue[] = _T("GRAD_VALUE");
}

namespace simple_grad {
	const _TCHAR kTableName[] = _T("SIMPLE_GRAD");

	const _TCHAR kNodeID[] = _T("NODE_ID");
	const _TCHAR kGrad[] = _T("GRAD_C");
	const _TCHAR kLinkType[] = _T("LINKTYPE_C");
}

namespace parking_shape {
	const _TCHAR kTableName[] = _T("PARKING_SHAPE");

	const _TCHAR kParkingOrgID[] = _T("PARKING_ORGID");
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");
}

namespace edit_area {
	const _TCHAR kTableName[] = _T("EDIT_AREA");

	const _TCHAR kAreaName[]	=	_T("AREA_NAME");
	const _TCHAR kCreateYear[]	=	_T("CREATE_YEAR");
	const _TCHAR kPriority[]	=	_T("PRIORITY");
}

const _TCHAR walk_nopassage::kTableName[] = _T("WALK_NOPASSAGE");

const _TCHAR walk_oneway::kTableName[] = _T("WALK_ONEWAY");

namespace walk_type {
	const _TCHAR kTableName[] = _T("WALK_TYPE");
	const _TCHAR kWalkTypeClass[] = _T("WALKTYPE_C");
}

namespace inf_turnreg {
	const _TCHAR kTableName[] = _T("INF_TURNREG");
	const int kMaxNumber = 4;

	const _TCHAR kStartMonth0[] = _T("START_MONTH");
	const _TCHAR kStartDay0[] = _T("START_DAY");
	const _TCHAR kStartHour0[] = _T("START_HOUR");
	const _TCHAR kStartMin0[] = _T("START_MIN");
	const _TCHAR kEndMonth0[] = _T("END_MONTH");
	const _TCHAR kEndDay0[] = _T("END_DAY");
	const _TCHAR kEndHour0[] = _T("END_HOUR");
	const _TCHAR kEndMin0[] = _T("END_MIN");
	const _TCHAR kDayOfWeek0[] = _T("DAYOFWEEK_C");

#define DECL(z, n, data)                                      \
	const _TCHAR kStartMonth##n[] = _T("START_MONTH") _T(#n); \
	const _TCHAR kStartDay##n[] = _T("START_DAY") _T(#n);     \
	const _TCHAR kStartHour##n[] = _T("START_HOUR") _T(#n);   \
	const _TCHAR kStartMin##n[] = _T("START_MIN") _T(#n);     \
	const _TCHAR kEndMonth##n[] = _T("END_MONTH") _T(#n);     \
	const _TCHAR kEndDay##n[] = _T("END_DAY") _T(#n);         \
	const _TCHAR kEndHour##n[] = _T("END_HOUR") _T(#n);        \
	const _TCHAR kEndMin##n[] = _T("END_MIN") _T(#n);         \
	const _TCHAR kDayOfWeek##n[] = _T("DAYOFWEEK") _T(#n) _T("_C");
	BOOST_PP_REPEAT_FROM_TO(1, 5, DECL, _)
#undef DECL

	const LPCTSTR kStartMonth[5] = { BOOST_PP_ENUM_PARAMS(5, kStartMonth) };
	const LPCTSTR kStartDay[5] = { BOOST_PP_ENUM_PARAMS(5, kStartDay) };
	const LPCTSTR kStartHour[5] = { BOOST_PP_ENUM_PARAMS(5, kStartHour) };
	const LPCTSTR kStartMin[5] = { BOOST_PP_ENUM_PARAMS(5, kStartMin) };
	const LPCTSTR kEndMonth[5] = { BOOST_PP_ENUM_PARAMS(5, kEndMonth) };
	const LPCTSTR kEndDay[5] = { BOOST_PP_ENUM_PARAMS(5, kEndDay) };
	const LPCTSTR kEndHour[5] = { BOOST_PP_ENUM_PARAMS(5, kEndHour) };
	const LPCTSTR kEndMin[5] = { BOOST_PP_ENUM_PARAMS(5, kEndMin) };
	const LPCTSTR kDayOfWeek[5] = { BOOST_PP_ENUM_PARAMS(5, kDayOfWeek) };
}

namespace inf_guide {
	const _TCHAR kTableName[] = _T("INF_GUIDE");

	const _TCHAR kGuideType[] = _T("GUIDETYPE_C");
	const _TCHAR kDelete[] = _T("DELETE_F");
}

namespace inf_dirguide {
	const _TCHAR kTableName[] = _T("INF_DIRGUIDE");
	const int    kMaxNumber = 10;
	const _TCHAR kGuidePattern[] = _T("GUIDEPATTERN_C");
	const _TCHAR kSequence[] = _T("SEQUENCE");

	const _TCHAR kGuideClass0[] = _T("GUIDECLASS_C");
	const _TCHAR kNameKanji0[] = _T("NAME_KANJI");
	const _TCHAR kNameYomi0[] = _T("NAME_YOMI");

#define DECL(z, n, data)                                              \
	const _TCHAR kGuideClass##n[] = _T("GUIDECLASS") _T(#n) _T("_C"); \
	const _TCHAR kNameKanji##n[] = _T("NAME_KANJI") _T(#n);           \
	const _TCHAR kNameYomi##n[] = _T("NAME_YOMI") _T(#n);
	BOOST_PP_REPEAT_FROM_TO(1, 11, DECL, _)
#undef DECL

	const LPCTSTR kGuideClass[11] = { BOOST_PP_ENUM_PARAMS(11, kGuideClass) };
	const LPCTSTR kNameKanji[11] = { BOOST_PP_ENUM_PARAMS(11, kNameKanji) };
	const LPCTSTR kNameYomi[11] = { BOOST_PP_ENUM_PARAMS(11, kNameYomi) };
}

namespace inf_lane {
	const _TCHAR kTableName[] = _T("INF_LANE");

	const _TCHAR kLaneDir[] = _T("LANEDIR_C");
	const _TCHAR kApproachLane[] = _T("APPROACH_LANE");
	const _TCHAR kRightChange[] = _T("RIGHT_CHANGE");
	const _TCHAR kLeftChange[] = _T("LEFT_CHANGE");
	const _TCHAR kFlowLane[] = _T("FLOW_LANE");
	const _TCHAR kLanePassable[] = _T("LANE_PASSABLE");
}

const _TCHAR inf_vics::kTableName[] = _T("INF_VICS");
const _TCHAR inf_internavi::kTableName[] = _T("INF_INTERNAVI");
const _TCHAR inf_ipcvics::kTableName[] = _T("INF_IPCVICS");

// iPC-VICSのID管理テーブル
namespace ipcvics_idtable {
	const _TCHAR kTableName[] = _T("IPCVICS_IDTABLE"); ///< テーブル名
	const _TCHAR kMeshCode[] = _T("MESHCODE");	///< メッシュコード
	const _TCHAR kMainNumber[] = _T("MAIN_NUMBER");	///< 優先使用済みID番号
	const _TCHAR kSubNumber[] = _T("SUB_NUMBER");	///< 非優先使用済みID番号
	const _TCHAR kSubVicsClass[] = _T("SUB_VICSCLASS_C");	///< 非優先VICS区分
}

namespace inf_byway {
	const _TCHAR kTableName[] = _T("INF_BYWAY");

	const _TCHAR kByway[] = _T("BYWAY_F");
	const _TCHAR kBackRoad[] = _T("BACKROAD_F");
	const _TCHAR kSourceName[] = _T("SOURCE_NAME");
}

namespace inf_route {
	const _TCHAR kTableName[] = _T("INF_ROUTE");

	const _TCHAR kRouteClass[] = _T("ROUTECLASS_C");
	const _TCHAR kSourceName[] = _T("SOURCE_NAME");

	const _TCHAR kRankMeshCode[] = _T("RANK_MESHCODE");
	const _TCHAR kRankLinkID[] = _T("RANK_LINK_ID");
	const _TCHAR kDriveMapRank[] = _T("DRIVEMAP_RANK");
}

namespace inf_intersection {
	const _TCHAR kTableName[] = _T("INF_INTERSECTION");

	const _TCHAR kNameKanji[] = _T("NAME_KANJI"); ///< 交差点名称
	const _TCHAR kNameYomi[] = _T("NAME_YOMI"); ///< ヨミ
}

namespace inf_uturn {
	const _TCHAR kTableName[] = _T("INF_UTURN");
	const _TCHAR kSourceName[] = _T("SOURCE_NAME");
}

const _TCHAR lq_turnreg::kTableName[] = _T("LQ_TURNREG");
const _TCHAR lq_guide::kTableName[] = _T("LQ_GUIDE");
const _TCHAR lq_dirguide::kTableName[] = _T("LQ_DIRGUIDE");
const _TCHAR lq_lane::kTableName[] = _T("LQ_LANE");
const _TCHAR lq_vics::kTableName[] = _T("LQ_VICS");
const _TCHAR lq_internavi::kTableName[] = _T("LQ_INTERNAVI");
const _TCHAR lq_ipcvics::kTableName[] = _T("LQ_IPCVICS");
const _TCHAR lq_intersection::kTableName[] = _T("LQ_INTERSECTION");
const _TCHAR lq_byway::kTableName[] = _T("LQ_BYWAY");
const _TCHAR lq_route::kTableName[] = _T("LQ_ROUTE");
const _TCHAR lq_uturn::kTableName[] = _T("LQ_UTURN");

namespace road_code_list {
	const _TCHAR kTableName[] = _T("ROAD_CODE_LIST");

	const _TCHAR kLineClass[] = _T("LINECLASS_C");
	const _TCHAR kLineDirType[] = _T("LINEDIRTYPE_C");
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");	
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");	
	const _TCHAR kWorkKanji[] = _T("WORK_KANJI");	
	const _TCHAR kWorkYomi[] = _T("WORK_YOMI");	
	const _TCHAR kDisplayKanji[] = _T("DISPLAY_KANJI");	
	const _TCHAR kDisplayYomi[] = _T("DISPLAY_YOMI");	
	const _TCHAR kShieldNoStr[] = _T("SHIELD_NO_STRING"); // 標識番号・記号 [bug12036]
}

// 都道府県道リスト [bug 12036]
namespace pref_road_list {
	const _TCHAR kTableName[] = _T("PREF_ROAD_LIST"); // テーブル名

	const _TCHAR kNameKanji[] = _T("NAME_KANJI");	// 路線漢字名称
	const _TCHAR kNameYomi[]  = _T("NAME_YOMI");	// 路線ヨミ名称
}

// 区間別称コードリスト [bug 12036]
namespace section_code_list {
	const _TCHAR kTableName[] = _T("SECTION_CODE_LIST"); // テーブル名

	const _TCHAR kNameKanji[] = _T("NAME_KANJI");		// 路線漢字名称
	const _TCHAR kNameYomi[]  = _T("NAME_YOMI");		// 路線ヨミ名称
}

namespace facil_info_point {
	const _TCHAR kTableName[] = _T("FACIL_INFO_POINT");

	const _TCHAR kNameKanji[] = _T("NAME_KANJI");
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");
	const _TCHAR kDummyName[] = _T("DUMMYNAME_F");
	const _TCHAR kFacilClass[] = _T("FACILCLASS_C");
	const _TCHAR kAutoPosition[] = _T("AUTOPOSITION_F");
	const _TCHAR kSearchClass[] = _T("SEARCHCLASS_C");
	const _TCHAR kDummyFacil[] = _T("DUMMYFACIL_F");	// [Bug9140]で追加
}

namespace access_point {
	const _TCHAR kTableName[] = _T("ACCESS_POINT");

	const _TCHAR kHwyMode[] = _T("HWYMODE_F");
	const _TCHAR kDirectionKanji[] = _T("DIRECTION_KANJI");
	const _TCHAR kDirectionYomi[] = _T("DIRECTION_YOMI");
	const _TCHAR kGS[] = _T("GS_C");
	const _TCHAR kToilet[] = _T("TOILET_F");
	const _TCHAR kHandicap[] = _T("HANDICAP_F");
	const _TCHAR kRestaurant[] = _T("RESTAURANT_F");
	const _TCHAR kCafeteria[] = _T("CAFETERIA_F");
	const _TCHAR kSouvenir[] = _T("SOUVENIR_F");
	const _TCHAR kResting[] = _T("RESTING_F");
	const _TCHAR kNap[] = _T("NAP_F");
	const _TCHAR kInfoDesk[] = _T("INFODESK_F");
	const _TCHAR kInformation[] = _T("INFORMATION_F");
	const _TCHAR kShower[] = _T("SHOWER_F");
	const _TCHAR kLaundry[] = _T("LAUNDRY_F");
	const _TCHAR kSpa[] = _T("SPA_F");
	const _TCHAR kFax[] = _T("FAX_F");
	const _TCHAR kMailbox[] = _T("MAILBOX_F");
	const _TCHAR kCashService[] = _T("CASHSERVICE_F");
	const _TCHAR kHwyOasis[] = _T("HWYOASIS_F");
	const _TCHAR kCarWash[] = _T("CARWASH_F");
	const _TCHAR kDogRun[]  = _T("DOGRUN_F");
}

// ハイウェイノード
namespace highway_node {
	const _TCHAR kTableName[] = _T("HIGHWAY_NODE");

	const _TCHAR kHwyMode[] = _T("HWYMODE_F");
	const _TCHAR kTollClass[] = _T("TOLLCLASS_C");

	const _TCHAR kNoEtcCar[] = _T("NOETC_CAR_F");
	const _TCHAR kTollFunc[] = _T("TOLLFUNC_C");
	const _TCHAR kDummyToll[] = _T("DUMMYTOLL_F");

	const _TCHAR kEtcFacil[] = _T("ETC_FACIL_C");
	const _TCHAR kEtcTollFunc[] = _T("ETC_TOLLFUNC_C");
	const _TCHAR kEtcNoGuideToll[] = _T("ETC_NOGUIDETOLL_F");
	const _TCHAR kTollgateID[] = _T( "TOLLGATE_ID" );

	const _TCHAR kGuide[] = _T("GUIDE_F");
	const _TCHAR kRoadSeq2[] = _T("ROAD_SEQ2");
	const _TCHAR kApproachNodeID[] = _T("APPROACHNODE_ID");
	const _TCHAR kFlowNodeID[] = _T("FLOWNODE_ID");

	const _TCHAR kTollGateLane[] = _T("TOLLGATE_LANE");
	const _TCHAR kHWYGuideNumber[] = _T("HWYGUIDE_NUMBER");
}

namespace highway_text {
	const _TCHAR kTableName[] = _T("HIGHWAY_TEXT");

	const _TCHAR kPointID[] = _T("POINT_ID");
	const _TCHAR kOrderSeq[] = _T("ORDER_SEQ");
	const _TCHAR kUpDown[] = _T("UPDOWN_C");
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");
	const _TCHAR kPrevID[] = _T("PREV_ID");
	const _TCHAR kNextID[] = _T("NEXT_ID");
	const _TCHAR kParentID[] = _T("PARENT_ID");
	const _TCHAR kForeCount[] = _T("FORE_COUNT");
	const _TCHAR kForeID0[] = _T("FORE0_ID");
	const _TCHAR kForeID1[] = _T("FORE1_ID");
	const _TCHAR kForeID2[] = _T("FORE2_ID");
	const _TCHAR kForeID3[] = _T("FORE3_ID");
	const _TCHAR kForeID4[] = _T("FORE4_ID");
	const _TCHAR kForeDir1[] = _T("FORE1_DIR_C");
	const _TCHAR kForeDir2[] = _T("FORE2_DIR_C");
	const _TCHAR kForeDir3[] = _T("FORE3_DIR_C");
	const _TCHAR kForeDir4[] = _T("FORE4_DIR_C");
	const _TCHAR kBackCount[] = _T("BACK_COUNT");
	const _TCHAR kBackID0[] = _T("BACK0_ID");
	const _TCHAR kBackID1[] = _T("BACK1_ID");
	const _TCHAR kBackID2[] = _T("BACK2_ID");
	const _TCHAR kBackID3[] = _T("BACK3_ID");
	const _TCHAR kBackID4[] = _T("BACK4_ID");
	const _TCHAR kBackDir1[] = _T("BACK1_DIR_C");
	const _TCHAR kBackDir2[] = _T("BACK2_DIR_C");
	const _TCHAR kBackDir3[] = _T("BACK3_DIR_C");
	const _TCHAR kBackDir4[] = _T("BACK4_DIR_C");

	const LPCTSTR kForeID[5] = { BOOST_PP_ENUM_PARAMS(5, kForeID) };
	const LPCTSTR kBackID[5] = { BOOST_PP_ENUM_PARAMS(5, kBackID) };
}

namespace distance_post_point {
	const _TCHAR kTableName[] = _T("DISTANCE_POST_POINT");

	const _TCHAR kDistance[] = _T("DISTANCE");
	const _TCHAR kDistanceMark[] = _T("DISTANCE_MARK");
}

namespace access_store {
	const _TCHAR kTableName[] = _T("ACCESS_STORE");

	const _TCHAR kAccessPointID[] = _T("ACCESSPOINT_ID");
	const _TCHAR kMultipleStore[] = _T("MULTIPLESTORE_C");
	const _TCHAR kStartHour[] = _T("START_HOUR");
	const _TCHAR kStartMin[] = _T("START_MIN");
	const _TCHAR kEndHour[] = _T("END_HOUR");
	const _TCHAR kEndMin[] = _T("END_MIN");
	const _TCHAR kSunday[] = _T("SUNDAY_F");
	const _TCHAR kMonday[] = _T("MONDAY_F");
	const _TCHAR kTuesday[] = _T("TUESDAY_F");
	const _TCHAR kWednesday[] = _T("WEDNESDAY_F");
	const _TCHAR kThursday[] = _T("THURSDAY_F");
	const _TCHAR kFriday[] = _T("FRIDAY_F");
	const _TCHAR kSaturday[] = _T("SATURDAY_F");
	const _TCHAR kBon[] = _T("BON_F");
	const _TCHAR kYearEnd[] = _T("YEAREND_F");
	const _TCHAR kNewYear[] = _T("NEWYEAR_F");
	const _TCHAR kGoldenWeek[] = _T("GOLDENWEEK_F");
	const _TCHAR kHoliday[] = _T("HOLIDAY_F");
	const _TCHAR kStoreType[] = _T("STORETYPE_C");
}

// IPC料金テーブル

namespace ipc_toll_table {
	const _TCHAR kTableName[] = _T("TOLL_OTHER_TABLE");		

	const _TCHAR kRoadCode1[] = _T("ROAD_CODE1");
	const _TCHAR kRoadSeq1[] = _T("ROAD_SEQ1");
	const _TCHAR kRoadCode2[] = _T("ROAD_CODE2");
	const _TCHAR kRoadSeq2[] = _T("ROAD_SEQ2");
	const _TCHAR kKToll[] = _T("K_TOLL");
	const _TCHAR kSToll[] = _T("S_TOLL");
	const _TCHAR kMToll[] = _T("M_TOLL");
	const _TCHAR kLToll[] = _T("L_TOLL");
	const _TCHAR kVLToll[] = _T("VL_TOLL");
	const _TCHAR kNormal[] = _T("NORMAL_F");
}
// 単独料金テーブル

namespace toll_alone_table {
	const _TCHAR kTableName[] = _T("TOLL_ALONE_TABLE");

	const _TCHAR kLineClass[] = _T("LINECLASS_C");
	const _TCHAR kKTollUp[] = _T("K_TOLL_UP");
	const _TCHAR kSTollUp[] = _T("S_TOLL_UP");
	const _TCHAR kMTollUp[] = _T("M_TOLL_UP");
	const _TCHAR kLTollUp[] = _T("L_TOLL_UP");
	const _TCHAR kVLTollUp[] = _T("VL_TOLL_UP");
	const _TCHAR kKTollDown[] = _T("K_TOLL_DOWN");
	const _TCHAR kSTollDown[] = _T("S_TOLL_DOWN");
	const _TCHAR kMTollDown[] = _T("M_TOLL_DOWN");
	const _TCHAR kLTollDown[] = _T("L_TOLL_DOWN");
	const _TCHAR kVLTollDown[] = _T("VL_TOLL_DOWN");
	const _TCHAR kNormal[] = _T("NORMAL_F");
}

//一時停止ポイント
namespace stop_point {
	const _TCHAR kTableName[]	= _T("STOP_POINT");

	const _TCHAR kLinkID[]		= _T("LINK_ID");
	const _TCHAR kNodeID[]		= _T("NODE_ID");
}

// 車種別規制マスタ
namespace vehicle_reg_master {
	const _TCHAR kTableName[] = _T("VEHICLE_REG_MASTER");

	const _TCHAR kVehicleRegCode[] = _T("VEHICLE_REG_CODE");
	const _TCHAR kVehicleRegName[] = _T("VEHICLE_REG_NAME");
	const _TCHAR kOgataJoyoF[] = _T("OGATA_JOYO_F");
	const _TCHAR kTokuteiChuJoF[] = _T("TOKUTEI_CHU_JO_F");
	const _TCHAR kChugataJoyoF[] = _T("CHUGATA_JOYO_F");
	const _TCHAR kFutsuJoyoF[] = _T("FUTSU_JOYO_F");
	const _TCHAR kFutsuJoyoKeiF[] = _T("FUTSU_JOYO_KEI_F");
	const _TCHAR kOgataKamotsuF[] = _T("OGATA_KAMOTSU_F");
	const _TCHAR kTokuteiChuKaF[] = _T("TOKUTEI_CHU_KA_F");
	const _TCHAR kChugataKamotsuF[] = _T("CHUGATA_KAMOTSU_F");
	const _TCHAR kFutsuKamotsuF[] = _T("FUTSU_KAMOTSU_F");
	const _TCHAR kFutsuKamotsuKeiF[] = _T("FUTSU_KAMOTSU_KEI_F");
	const _TCHAR kOgataTokushuF[] = _T("OGATA_TOKUSHU_F");
	const _TCHAR kKogataTokushuF[] = _T("KOGATA_TOKUSHU_F");
	const _TCHAR kOgataBikeF[] = _T("OGATA_BIKE_F");
	const _TCHAR kFutsuBikeF[] = _T("FUTSU_BIKE_F");
	const _TCHAR kKogataBikeF[] = _T("KOGATA_BIKE_F");
	const _TCHAR kTaxiF[] = _T("TAXI_F");
	const _TCHAR kBusF[] = _T("BUS_F");
	const _TCHAR kMiniBikeF[] = _T("MINI_BIKE_F");
	const _TCHAR kTrolleyF[] = _T("TROLLEY_F");
	const _TCHAR kBicycleF[] = _T("BICYCLE_F");
	const _TCHAR kExceptBicycleF[] = _T("EXCEPT_BICYCLE_F");
	const _TCHAR kSignageIllustFile[] = _T("SIGNAGE_ILLUST_FILE");
}

// 車種別規制テーブル
namespace vehicle_reg {
	const _TCHAR kTableName[] = _T("VEHICLE_REG"); 
	
	const _TCHAR kLinkID[] = _T("LINK_ID");
	const _TCHAR kRegTypeC[] = _T("REGTYPE_C");
	const _TCHAR kHeight[] = _T("HEIGHT");
	const _TCHAR kWidth[] = _T("WIDTH");
	const _TCHAR kWeight[] = _T("WEIGHT");
	const _TCHAR kLength[] = _T("LENGTH");
	const _TCHAR kLoadage[] = _T("LOADAGE");
	const _TCHAR kDangerF[] = _T("DANGER_F");
}

// 車種テーブル
namespace vehicle_type{
	extern const _TCHAR kTableName[] = _T("VEHICLE_TYPE");

	extern const _TCHAR kRegID[] = _T("REG_ID");
	extern const _TCHAR kVehicleRegCode[] = _T("VEHICLE_REG_CODE");
	extern const _TCHAR kExcludeF[] = _T("EXCLUDE_F");
}

// 重なりリンクの高さノード
namespace height_node
{
	const _TCHAR kTableName[] = _T("HEIGHT_NODE");

	const _TCHAR kLinkID1[] = _T("LINK_ID1");
	const _TCHAR kLinkID1Height[] = _T("LINK_ID1_HEIGHT");
	const _TCHAR kLinkID1Table[] = _T("LINK_ID1_TABLE_C");
	const _TCHAR kLinkID2[] = _T("LINK_ID2");
	const _TCHAR kLinkID2Height[] = _T("LINK_ID2_HEIGHT");
	const _TCHAR kLinkID2Table[] = _T("LINK_ID2_TABLE_C");

} // namespace height_node

// 歩行者・道路リンク紐づけテーブル
namespace link_relation
{
	const _TCHAR kTableName[] = _T("LINK_RELATION");

	const _TCHAR kBaseLinkID[] = _T("BASE_LINK_ID");
	const _TCHAR kRelatedLinkID[] = _T("RELATED_LINK_ID");
	const _TCHAR kLinkSide[] = _T("LINK_SIDE_C");

} // namespace link_relation

// 歩行者路線コードリスト
namespace walk_code_list {

	const _TCHAR kTableName[] = _T("WALK_CODE_LIST");

	const _TCHAR kOperator[] = _T("OPERATOR");
	const _TCHAR kModifyDate[] = _T("MODIFYDATE");
	const _TCHAR kWalkCode[] = _T("WALK_CODE");
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");

} // namespace walk_code_list

/// 警戒標識
namespace warning_sign {
	const _TCHAR kTableName[] = _T("WARNING_SIGN");

	const _TCHAR kLinkID[] = _T("LINK_ID");
	const _TCHAR kWarningSign[] = _T("WARNING_SIGN_C");
	const _TCHAR kPicturePath[] = _T("PICTURE_PATH");

} // warning_sign

  /// ETC料金所コード
namespace etc_tollgate {
	const _TCHAR kTableName[]			= _T( "ETC_TOLLGATE" );

	const _TCHAR kTollgateID[]			= _T("TOLLGATE_ID");
	const _TCHAR kTollgateParentID[]	= _T("TOLLGATE_PARENT_ID");
	const _TCHAR kTollgateOfficeID[]	= _T("TOLLGATE_OFFICE_ID");
	const _TCHAR kTollgateOfficeName[]	= _T("TOLLGATE_OFFICE_NAME");
	const _TCHAR kTollgateRoadNumber[]	= _T("TOLLGATE_ROAD_NUMBER");
	const _TCHAR kTollgateRoadName[]	= _T("TOLLGATE_ROAD_NAME");
	const _TCHAR kTollgateICNumber[]	= _T("TOLLGATE_IC_NUMBER");
	const _TCHAR kTollgateICName[]		= _T("TOLLGATE_IC_NAME");
	const _TCHAR kTollgateICNameYomi[]	= _T("TOLLGATE_IC_NAME_YOMI");
	const _TCHAR kTollgateFlag[]		= _T("TOLLGATE_FLAG");
	const _TCHAR kTollgateOther[]		= _T("TOLLGATE_OTHER");

} // etc_tollgate


} // namespace schema

} // namespace sindy
