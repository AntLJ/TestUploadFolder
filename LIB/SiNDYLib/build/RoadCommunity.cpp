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
 * @file RoadCommunity.cpp
 * @brief 道路テーブルフィールド・ドメイン定義
 * @author 
 * $Id$
 */
#include "stdafx.h"
#include "RoadCommunity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
	namespace access_point {
		const TCHAR kHwyMode_F[]		= _T("HWYMODE_F");
		const TCHAR kRoadCode[]			= _T("ROAD_CODE");
		const TCHAR kRoadSeq[]			= _T("ROAD_SEQ");
		const TCHAR kDirection[]		= _T("DIRECTION_C");
		const TCHAR kInput[]			= _T("INOUT_C");
		const TCHAR kDirectionKanji[]	= _T("DIRECTION_KANJI");
		const TCHAR kDirectionYomi[]	= _T("DIRECTION_YOMI");
		const TCHAR kGS[]				= _T("GS_C");
		const TCHAR kToilet_F[]			= _T("TOILET_F");
		const TCHAR kHandicap_F[]		= _T("HANDICAP_F");
		const TCHAR kRestaurant_F[]		= _T("RESTAURANT_F");
		const TCHAR kCafeteria_F[]		= _T("CAFETERIA_F");
		const TCHAR kSouvenir_F[]		= _T("SOUVENIR_F");
		const TCHAR kResting_F[]		= _T("RESTING_F");
		const TCHAR kNap_F[]			= _T("NAP_F");
		const TCHAR kInfoDest_F[]		= _T("INFODESK_F");
		const TCHAR kInfomation_F[]		= _T("INFODESK_F");
		const TCHAR kShower_F[]			= _T("SHOWER_F");
		const TCHAR kLaundry_F[]		= _T("LAUNDRY_F");
		const TCHAR kSpa_F[]			= _T("SPA_F");
		const TCHAR kFax_F[]			= _T("FAX_F");
		const TCHAR kMailBox_F[]		= _T("MAILBOX_F");
		const TCHAR kCashService_F[]	= _T("CASHSERVICE_F");
		const TCHAR kHwyOasis_F[]		= _T("HWYOASIS_F");
		const TCHAR kCarWash_F[]		= _T("CARWASH_F");
	} /// access_point
	namespace facil_info_point {
		const TCHAR kRoadCode[]			= _T("ROAD_CODE");
		const TCHAR kRoadSeq[]			= _T("ROAD_SEQ");
		const TCHAR kNameKanji[]		= _T("NAME_KANJI");
		const TCHAR kNameYomi[]			= _T("NAME_YOMI");
		const TCHAR kDummyName_F[]		= _T("DUMMYNAME_F");
		const TCHAR kFacilClass[]		= _T("FACILCLASS_C");
		const TCHAR kAutoPosition_F[]	= _T("AUTOPOSITION_F");
	} /// facil_info_point
	namespace grad {
		const TCHAR kFieldSurvey_F[]	= _T("FIELDSURVEY_F");
		const TCHAR kWorkYear[]			= _T("WORK_YEAR");
		const TCHAR kLinkID[]			= _T("LINK_ID");
		const TCHAR kGradValue[]		= _T("GRAD_VALUE");
	} /// grad
	namespace highway_node {
		const TCHAR kHwyMode_F[]		= _T("HWYMODE_F");
		const TCHAR kRoadCode[]			= _T("ROAD_CODE");
		const TCHAR kRoadSeq[]			= _T("ROAD_SEQ");
		const TCHAR kDirection[]		= _T("DIRECTION_C");
		const TCHAR kInput[]			= _T("INOUT_C");
		const TCHAR kApprochNode_ID[]	= _T("APPROACHNODE_ID");
		const TCHAR kFlowNode_ID[]		= _T("FLOWNODE_ID");
		const TCHAR kRoadSeq2[]			= _T("ROAD_SEQ2");
		const TCHAR kTollClass[]		= _T("TOLLCLASS_C");
		const TCHAR kTollFunc[]			= _T("TOLLFUNC_C");
		const TCHAR kDummyToll_F[]		= _T("DUMMYTOLL_F");
		const TCHAR kGuide_F[]			= _T("GUIDE_F");
	} /// highway_node
	namespace parking_shape {
		const TCHAR kParking_OrgID[]	= _T("PARKING_ORGID");
		const TCHAR kParkingClass[]		= _T("PARKINGCLASS_C");
		const TCHAR kKanjiName[]		= _T("KANJI_NAME");
	} /// parking_shape
	namespace road_link {
		const TCHAR kFieldSurvey_F[]	= _T("FIELDSURVEY_F");
		const TCHAR kWorkYear[]			= _T("WORK_YEAR");
		const TCHAR kTmpFrom_Node[]		= _T("TMPFROM_NODE");
		const TCHAR kTmpTo_Node[]		= _T("TMPTO_NODE");
		const TCHAR kFrom_Node_ID[]		= _T("FROM_NODE_ID");
		const TCHAR kTo_Node_ID[]		= _T("TO_NODE_ID");
		const TCHAR kRoadClass[]		= _T("ROADCLASS_C");
		const TCHAR kNaviClass[]		= _T("NAVICLASS_C");
		const TCHAR kMain_LinkClass[]	= _T("MAIN_LINKCLASS_C");
		const TCHAR kSub_LinkClass[]	= _T("SUB_LINKCLASS_C");
		const TCHAR kAdmin_Code[]		= _T("ADMIN_CODE");
		const TCHAR kRoadManager[]		= _T("ROADMANAGER_C");
		const TCHAR kRoadWidth[]		= _T("ROADWIDTH_C");
		const TCHAR kWideArea[]			= _T("WIDEAREA_C");
		const TCHAR kUpDownClass[]		= _T("UPDOWNCLASS_C");
		const TCHAR kBypass_F[]			= _T("BYPASS_F");
		const TCHAR kCarOnly_F[]		= _T("CARONLY_F");
		const TCHAR kIsland_F[]			= _T("ISLAND_F");
		const TCHAR kRoad_No[]			= _T("ROAD_NO");
		const TCHAR kRoad_Code[]		= _T("ROAD_CODE");
		const TCHAR kNoPassage[]		= _T("NOPASSAGE_C");
		const TCHAR kNoPassage_RF[]		= _T("NOPASSAGE_RF");
		const TCHAR kOneway[]			= _T("ONEWAY_C");
		const TCHAR kOneway_RF[]		= _T("ONEWAY_RF");
		const TCHAR kTrip_Speed[]		= _T("TRIP_SPEED");
		const TCHAR kLimitSpeed[]		= _T("LIMITSPEED_C");
		const TCHAR kKubun[]			= _T("KUBUN_C");
		const TCHAR kLane_Count[]		= _T("LANE_COUNT");
		const TCHAR kRight_Change[]		= _T("RIGHT_CHANGE");
		const TCHAR kLeft_Change[]		= _T("LEFT_CHANGE");
		const TCHAR kLaneValid_F[]		= _T("LANEVALID_F");
		const TCHAR kAlleyMainte[]		= _T("ALLEYMAINTE_C");
		const TCHAR kDuplink_RF[]		= _T("DUPLINK_RF");
		const TCHAR kTurnReg_LQRF[]		= _T("TURNREG_LQRF");
		const TCHAR kDirGuide_LQRF[]	= _T("DIRGUIDE_LQRF");
		const TCHAR kGuide_LQRF[]		= _T("GUIDE_LQRF");
		const TCHAR kLane_LQRF[]		= _T("LANE_LQRF");
		const TCHAR kVics_LQRF[]		= _T("VICS_LQRF");
		const TCHAR kByway_LQRF[]		= _T("BYWAY_LQRF");
		const TCHAR kRoute_LQRF[]		= _T("ROUTE_LQRF");
		const TCHAR kUTurn_LQRF[]		= _T("UTURN_LQRF");
		const TCHAR kInterNave_LQRF[]	= _T("INTERNAVI_LQRF");
		const TCHAR kParkingClass[]		= _T("PARKINGCLASS_C");
		const TCHAR kParkingLinkClass[]	= _T("PARKINGLINKCLASS_C");
		const TCHAR kFloor_Level[]		= _T("FLOOR_LEVEL");
		const TCHAR kMiddleFloor_Level[]= _T("MIDDLEFLOOR_LEVEL");
	} /// road_link
	namespace road_node {
		const TCHAR kFieldSurvey[]		= _T("FIELDSURVEY_F");
		const TCHAR kWork_Year[]		= _T("WORK_YEAR");
		const TCHAR kNodeClass[]		= _T("NODECLASS_C");
		const TCHAR kName_Kanji[]		= _T("NAME_KANJI");
		const TCHAR kName_Yomi[]		= _T("NAME_YOMI");
		const TCHAR kHeight_C[]			= _T("HEIGHT_C");
		const TCHAR kHeight[]			= _T("HEIGHT");
		const TCHAR kSignal[]			= _T("SIGNAL_C");
		const TCHAR kParking_ID[]		= _T("PARKING_ID");
	} /// road_node
	namespace segment_attr {
		const TCHAR kFieldSurvey_F[]	= _T("FIELDSURVEY_F");
 		const TCHAR kWork_Year[]		= _T("WORK_YEAR");
 		const TCHAR kLink_ID[]			= _T("LINK_ID");
 		const TCHAR kLinkAttr[]			= _T("LINKATTR_C");
 		const TCHAR kUnder_Over[]		= _T("UNDER_OVER");
	} /// segment_attr
