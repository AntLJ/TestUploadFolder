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
#include <sindy/schema/illust.h>
#include <sindy/schema/road.h>

namespace sindy {

namespace schema {

namespace illust_link {
	const _TCHAR kTableName[] = _T("ILLUST_LINK");

	const _TCHAR kIllustName[]            = _T("ILLUST_NAME");
	const _TCHAR kIllustClass[]           = _T("ILLUST_CLASS_C");
	const _TCHAR kCustomerCode[]          = _T("CUSTOMER_C");
	const _TCHAR kRoadCode[]              = _T("ROAD_CODE");
	const _TCHAR kRoadName[]              = _T("ROAD_NAME");
	const _TCHAR kRoadSubName[]           = _T("ROAD_SUBNAME");
	const _TCHAR kRoadSeq[]               = _T("ROAD_SEQ");
	const _TCHAR kFacilName[]             = _T("FACIL_NAME");
	const _TCHAR kFacilSubName[]          = _T("FACIL_SUBNAME");
	const _TCHAR kIdentificationName[]    = _T("IDENTIFICATION_NAME");
	const _TCHAR kGuideRoute[]            = _T("GUIDE_ROUTE");
	const _TCHAR kUpDownC[]               = _T("UPDOWN_C");
	const _TCHAR kGuideSeq[]              = _T("GUIDE_SEQ");
	const _TCHAR kSuccessiveCode[]        = _T("SUCCESSIVE_C");
	const _TCHAR kSingleBranchCode[]      = _T("SINGLE_BRANCH_C");
	const _TCHAR kIrregularCode[]         = _T("IRREGULAR_C");
	const _TCHAR kTunnelCode[]            = _T("TUNNEL_C");
	const _TCHAR kStraightCode[]          = _T("STRAIGHT_C");
	const _TCHAR kBoundaryCode[]          = _T("BOUNDARY_C");
	const _TCHAR kBranchDirectionCode[]   = _T("BRANCH_DIR_C");
	const _TCHAR kLaneBranchPatternCode[] = _T("LANE_BRANCH_PATTERN_C");
	const _TCHAR kDirguideExistCode[]     = _T("DIR_GUIDE_EXIST_C");
	const _TCHAR kDirectionCode[]         = _T("DIRECTION_C");

	const _TCHAR kBranch1PatternCode[]           = _T("BRANCH1_PATTERN_C");
	const _TCHAR kBranch1LaneCountMainBefore[]   = _T("BRANCH1_LANE_COUNT_MAIN_BFR");
	const _TCHAR kBranch1LaneCountMainAfter[]    = _T("BRANCH1_LANE_COUNT_MAIN_AFT");
	const _TCHAR kBranch1LaneCountLeftBefore[]   = _T("BRANCH1_LANE_COUNT_LEFT_BFR");
	const _TCHAR kBranch1LaneCountLeftAfter[]    = _T("BRANCH1_LANE_COUNT_LEFT_AFT");
	const _TCHAR kBranch1LaneCountCenterBefore[] = _T("BRANCH1_LANE_COUNT_CENTER_BFR");
	const _TCHAR kBranch1LaneCountCenterAfter[]  = _T("BRANCH1_LANE_COUNT_CENTER_AFT");
	const _TCHAR kBranch1LaneCountRightBefore[]  = _T("BRANCH1_LANE_COUNT_RIGHT_BFR");
	const _TCHAR kBranch1LaneCountRightAfter[]   = _T("BRANCH1_LANE_COUNT_RIGHT_AFT");
	const _TCHAR kBranch2PatternCode[]           = _T("BRANCH2_PATTERN_C");
	const _TCHAR kBranch2LaneCountMainBefore[]   = _T("BRANCH2_LANE_COUNT_MAIN_BFR");
	const _TCHAR kBranch2LaneCountMainAfter[]    = _T("BRANCH2_LANE_COUNT_MAIN_AFT");
	const _TCHAR kBranch2LaneCountLeftBefore[]   = _T("BRANCH2_LANE_COUNT_LEFT_BFR");
	const _TCHAR kBranch2LaneCountLeftAfter[]    = _T("BRANCH2_LANE_COUNT_LEFT_AFT");
	const _TCHAR kBranch2LaneCountCenterBefore[] = _T("BRANCH2_LANE_COUNT_CENTER_BFR");
	const _TCHAR kBranch2LaneCountCenterAfter[]  = _T("BRANCH2_LANE_COUNT_CENTER_AFT");
	const _TCHAR kBranch2LaneCountRightBefore[]  = _T("BRANCH2_LANE_COUNT_RIGHT_BFR");
	const _TCHAR kBranch2LaneCountRightAfter[]   = _T("BRANCH2_LANE_COUNT_RIGHT_AFT");
	const _TCHAR kBranch3PatternCode[]           = _T("BRANCH3_PATTERN_C");
	const _TCHAR kBranch3LaneCountMainBefore[]   = _T("BRANCH3_LANE_COUNT_MAIN_BFR");
	const _TCHAR kBranch3LaneCountMainAfter[]    = _T("BRANCH3_LANE_COUNT_MAIN_AFT");
	const _TCHAR kBranch3LaneCountLeftBefore[]   = _T("BRANCH3_LANE_COUNT_LEFT_BFR");
	const _TCHAR kBranch3LaneCountLeftAfter[]    = _T("BRANCH3_LANE_COUNT_LEFT_AFT");
	const _TCHAR kBranch3LaneCountCenterBefore[] = _T("BRANCH3_LANE_COUNT_CENTER_BFR");
	const _TCHAR kBranch3LaneCountCenterAfter[]  = _T("BRANCH3_LANE_COUNT_CENTER_AFT");
	const _TCHAR kBranch3LaneCountRightBefore[]  = _T("BRANCH3_LANE_COUNT_RIGHT_BFR");
	const _TCHAR kBranch3LaneCountRightAfter[]   = _T("BRANCH3_LANE_COUNT_RIGHT_AFT");
	const _TCHAR kBranch4PatternCode[]           = _T("BRANCH4_PATTERN_C");
	const _TCHAR kBranch4LaneCountMainBefore[]   = _T("BRANCH4_LANE_COUNT_MAIN_BFR");
	const _TCHAR kBranch4LaneCountMainAfter[]    = _T("BRANCH4_LANE_COUNT_MAIN_AFT");
	const _TCHAR kBranch4LaneCountLeftBefore[]   = _T("BRANCH4_LANE_COUNT_LEFT_BFR");
	const _TCHAR kBranch4LaneCountLeftAfter[]    = _T("BRANCH4_LANE_COUNT_LEFT_AFT");
	const _TCHAR kBranch4LaneCountCenterBefore[] = _T("BRANCH4_LANE_COUNT_CENTER_BFR");
	const _TCHAR kBranch4LaneCountCenterAfter[]  = _T("BRANCH4_LANE_COUNT_CENTER_AFT");
	const _TCHAR kBranch4LaneCountRightBefore[]  = _T("BRANCH4_LANE_COUNT_RIGHT_BFR");
	const _TCHAR kBranch4LaneCountRightAfter[]   = _T("BRANCH4_LANE_COUNT_RIGHT_AFT");

	const _TCHAR kCrossRoadFront_A[]  = _T("CROSS_ROAD_FRONT_A");
	const _TCHAR kCrossRoadFront_B[]  = _T("CROSS_ROAD_FRONT_B");
	const _TCHAR kCrossRoadFront_C[]  = _T("CROSS_ROAD_FRONT_C");
	const _TCHAR kCrossRoadFront_D[]  = _T("CROSS_ROAD_FRONT_D");
	const _TCHAR kCrossRoadFront_E[]  = _T("CROSS_ROAD_FRONT_E");
	const _TCHAR kCrossRoadFront_F[]  = _T("CROSS_ROAD_FRONT_F");
	const _TCHAR kCrossRoadMiddle_A[] = _T("CROSS_ROAD_MIDDLE_A");
	const _TCHAR kCrossRoadMiddle_B[] = _T("CROSS_ROAD_MIDDLE_B");
	const _TCHAR kCrossRoadMiddle_C[] = _T("CROSS_ROAD_MIDDLE_C");
	const _TCHAR kCrossRoadMiddle_D[] = _T("CROSS_ROAD_MIDDLE_D");
	const _TCHAR kCrossRoadMiddle_E[] = _T("CROSS_ROAD_MIDDLE_E");
	const _TCHAR kCrossRoadMiddle_F[] = _T("CROSS_ROAD_MIDDLE_F");
	const _TCHAR kCrossRoadAhead_A[]  = _T("CROSS_ROAD_AHEAD_A");
	const _TCHAR kCrossRoadAhead_B[]  = _T("CROSS_ROAD_AHEAD_B");
	const _TCHAR kCrossRoadAhead_C[]  = _T("CROSS_ROAD_AHEAD_C");
	const _TCHAR kCrossRoadAhead_D[]  = _T("CROSS_ROAD_AHEAD_D");
	const _TCHAR kCrossRoadAhead_E[]  = _T("CROSS_ROAD_AHEAD_E");
	const _TCHAR kCrossRoadAhead_F[]  = _T("CROSS_ROAD_AHEAD_F");
	const _TCHAR kCrossRoadAhead_G[]  = _T("CROSS_ROAD_AHEAD_G");

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

	const _TCHAR kProvisional[] = _T("PROVISIONAL_F");
}

} // namespace schema

} // namespace sindy
