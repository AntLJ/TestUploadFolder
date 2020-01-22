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


#pragma once

#include "data_checker.h"

namespace sindy {
namespace adam2 {
namespace format {

#define TARGET_FORMATDEF "1.0.3" // 高精度データ_納入フォーマット仕様書_verA.B.C.xlsx

//#define DOMAIN_
#define DOMAIN_INVALID_LANE     {0}
#define DOMAIN_LANE_GROUP       {0, 1, 2, 3, 4, 5}
#define DOMAIN_LEGAL_SPEED      {20,30,40,50,60,70,80,90,100,120,1001,1002,2020,2030,2040,2050,2060,2070,2080,2090,2100,2120}
#define DOMAIN_TRAVEL_DIRECTION {1,2,3,4,5,6,7,8,9}
#define DOMAIN_ROAD_STRUCTURE   {1,2,3,4,5,6,7,8,9,10,11,12}
#define DOMAIN_POINT_CLASS      {1, 2}
#define DOMAIN_CURVATURE_LINE   {0.00000}
#define DOMAIN_CURVRADIUS_LINE  {90000000000.00000}
#define DOMAIN_CLOTHOID_BEGIN   {0.00000}
#define DOMAIN_CLOTHOID_PARAM   {0.00000}
#define DOMAIN_LINEAR_ELEMENT   {1,2,3}
#define DOMAIN_COMPART_LINE     {0,1,2,3,13,31,12,21,22,23,32,33,232,313,323,3223}
#define DOMAIN_COMPART_STYLE    {1,2,3,4}
#define DOMAIN_SLOWDOWN_LABEL   {0,1,2,3}
#define DOMAIN_LINE_DIRECTION   {1,2}
#define DOMAIN_NO_RELATION_ID   {-1}
#define DOMAIN_ROADSIGN         {"116の4","116の6","119-D","201-A","201-B","201-C","201-D","201の2","202","203","204","205","206","207-A","207-B","208","208の2","209","209の2","209の3","210","211","212","212の2","212の3","212の4","213","214","214の2","215","301","302","303","304","305","305の2","306","307","308","309","310","310の2","311-A","311-B","311-C","311-D","311-E","311-F","312","313","314","314の2","315","316","317","318","319","320","321","322","323","323の2","324","325","327の8","327の9","327の10","327の11","327の12","327の13","328","328の2","329","329の2","330","331","332"}
#define DOMAIN_UPDATE           {0,1,2,3}
#define DOMAIN_SRID             {6668,4612,4301}
#define DOMAIN_SRNAME           {"世界測地系(JGD2011)","世界測地系(JGD2000)","日本測地系(TOKYO)"}

//// 共通フィールド値を利用する場合専用!!!
class CLinkChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		FROM_NODE_ID,
		TO_NODE_ID,
	};
};
class CNodeChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		NODE_ID,
		POINT_CLASS_C,
		LINK_ID,
		SEQUENCE,
		LAST_NODE_F,
	};

	enum point_class {
		POINT_NODE = 1,
		POINT_VIA  = 2,
	};
};
class CRelLinkChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_ID,
		LR_F,
		LINK_ID,
		LINK_DIRECTION_C,
		SEQUENCE,
	};
};

class CLaneLinkChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		FROM_NODE_ID,
		TO_NODE_ID,
		LANE_GROUP_ID,
		LANE_GROUP_C,
		LANE_NO,
		LANE_COUNT,
		MAX_LEGAL_SPEED_C,
		MIN_LEGAL_SPEED_C,
		TRAVEL_DIRECTION_C,
		ROAD_STRUCTURE_C,
		BOTH_LINK_DIRECTION_F,
		LEFT_CHANGE_F,
		RIGHT_CHANGE_F,
		LEFT_PROTRUSION_F,
		RIGHT_PROTRUSION_F,
		RUBBING_F,
		TUNNEL_F,
		UPDATE_C,
	};

public:
	~CLaneLinkChecker() {}
	CLaneLinkChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "LANE_LINK";

		// フォーマットチェック用
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  5 FROM_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  6 TO_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0,  true)));  //  7 LANE_GROUP_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  8 LANE_GROUP_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 2, 0,  true)));  //  9 LANE_NO
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 2, 0,  true)));  // 10 LANE_COUNT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  // 11 MAX_LEGAL_SPEED_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  // 12 MIN_LEGAL_SPEED_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  // 13 TRAVEL_DIRECTION_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  // 14 ROAD_STRUCTURE_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 15 BOTH_LINK_DIRECTION_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 16 LEFT_CHANGE_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 17 RIGHT_CHANGE_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 18 LEFT_PROTRUSION_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 19 RIGHT_PROTRUSION_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 20 RUBBING_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 21 TUNNEL_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  // 22 UPDATE_C

		// 属性チェック用
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));                   //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));                   //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));                //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));               //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));               //  5 FROM_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));               //  6 TO_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));               //  7 LANE_GROUP_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LANE_GROUP)));                //  8 LANE_GROUP_C
		attr.push_back(std::shared_ptr<IChecker>(new CSpRngChecker<long>  (1, LONG_MAX, DOMAIN_INVALID_LANE))); //  9 LANE_NO
		attr.push_back(std::shared_ptr<IChecker>(new CSpRngChecker<long>  (1, LONG_MAX, DOMAIN_INVALID_LANE))); // 10 LANE_COUNT
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LEGAL_SPEED)));               // 11 MAX_LEGAL_SPEED_C
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LEGAL_SPEED)));               // 12 MIN_LEGAL_SPEED_C
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_TRAVEL_DIRECTION)));          // 13 TRAVEL_DIRECTION_C
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_ROAD_STRUCTURE)));            // 14 ROAD_STRUCTURE_C
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 15 BOTH_LINK_DIRECTION_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 16 LEFT_CHANGE_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 17 RIGHT_CHANGE_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 18 LEFT_PROTRUSION_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 19 RIGHT_PROTRUSION_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 20 RUBBING_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                          // 21 TUNNEL_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));                    // 22 UPDATE_C 
	}
};

class CLaneNodeChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		NODE_ID,
		POINT_CLASS_C,
		LANE_LINK_ID,
		SEQUENCE,
		LAST_NODE_F,
		STOP_POINT_F,
		WIDTH,
		LINEAR_ELEMENT_C,
		CURVATURE,
		CURVATURE_RADIUS,
		CURVATURE_CHANGE_RATE,
		CLOTHOID_PARAMETER,
		LONGITUDINAL_SLOPE,
		CROSS_SLOPE,
		COVER_F,
		UPDATE_C,
	};

public:
	~CLaneNodeChecker() {}
	CLaneNodeChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "LANE_NODE";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  5 NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  6 POINT_CLASS_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  7 LANE_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  8 SEQUENCE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0, false)));  //  9 LAST_NODE_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 10 STOP_POINT_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 3,  true)));  // 11 WIDTH
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  // 12 LINEAR_ELEMENT_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(12, 5,  true)));  // 13 CURVATURE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(12, 5,  true)));  // 14 CURVATURE_RADIUS
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(12, 5,  true)));  // 15 CURVATURE_CHANGE_RATE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(12, 5,  true)));  // 16 CLOTHOID_PARAMETER
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 3,  true)));  // 17 LONGITUDINAL_SLOPE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 3,  true)));  // 18 CROSS_SLOPE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 19 COVER_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  // 20 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));                               //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));                               //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));                            //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));                           //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));                           //  5 NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_POINT_CLASS)));                           //  6 POINT_CLASS_C
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));                           //  7 LANE_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (0,        LONG_MAX)));                           //  8 SEQUENCE
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                                      //  9 LAST_NODE_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                                      // 10 STOP_POINT_F
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(0,        DBL_MAX)));                            // 11 WIDTH
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LINEAR_ELEMENT)));                        // 12 LINEAR_ELEMENT_C
		attr.push_back(std::shared_ptr<IChecker>(new CSpRngChecker<double>(-1.0, 1.0, DOMAIN_CURVATURE_LINE)));             // 13 CURVATURE
		attr.push_back(std::shared_ptr<IChecker>(new CSpRngChecker<double>(-DBL_MAX, DBL_MAX, DOMAIN_CURVRADIUS_LINE)));    // 14 CURVATURE_RADIUS
		attr.push_back(std::shared_ptr<IChecker>(new CSpRngChecker<double>(-DBL_MAX, DBL_MAX, DOMAIN_CLOTHOID_BEGIN)));     // 15 CURVATURE_CHANGE_RATE
		attr.push_back(std::shared_ptr<IChecker>(new CSpRngChecker<double>(-DBL_MAX, DBL_MAX, DOMAIN_CLOTHOID_PARAM)));     // 16 CLOTHOID_PARAMETER
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-999.999, 999.999)));                            // 17 LONGITUDINAL_SLOPE
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-999.999, 999.999)));                            // 18 CROSS_SLOPE
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                                      // 19 COVER_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));                                // 20 UPDATE_C 
	}
};

class CCompartLinkChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		FROM_NODE_ID,
		TO_NODE_ID,
		COMPART_LINE_C,
		COMPART_STYLE_C,
		SLOWDOWN_LABEL_C,
		INSTALLATION_F,
		UPDATE_C,
	};

public:
	~CCompartLinkChecker() {}
	CCompartLinkChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "COMPART_LINK";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  5 FROM_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  6 TO_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  7 COMPART_LINE_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  8 COMPART_STYLE_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  9 SLOWDOWN_LABEL_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  // 10 INSTALLATION_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  // 11 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));           //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));           //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));        //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));       //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));       //  5 FROM_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));       //  6 TO_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_COMPART_LINE)));      //  7 COMPART_LINE_C
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_COMPART_STYLE)));     //  8 COMPART_STYLE_C
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_SLOWDOWN_LABEL)));    //  9 SLOWDOWN_LABEL_C
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                  // 10 INSTALLATION_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));            // 11 UPDATE_C 
	}
};

class CCompartNodeChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		NODE_ID,
		POINT_CLASS_C,
		COMPART_LINK_ID,
		SEQUENCE,
		LAST_NODE_F,
		WIDTH,
		UPDATE_C,
	};

public:
	~CCompartNodeChecker() {}
	CCompartNodeChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "COMPART_NODE";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  5 NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  6 POINT_CLASS_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  7 COMPART_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  8 SEQUENCE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0, false)));  //  9 LAST_NODE_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 3,  true)));  // 10 WIDTH
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  // 11 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  5 NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_POINT_CLASS)));   //  6 POINT_CLASS_C
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  7 COMPART_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (0,        LONG_MAX)));   //  8 SEQUENCE
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              //  9 LAST_NODE_F
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(0,        DBL_MAX)));    // 10 WIDTH
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        // 11 UPDATE_C 
	}
};

class CBorderLinkChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		FROM_NODE_ID,
		TO_NODE_ID,
		IMAGINARY_BODER_F,
		UPDATE_C,
	};

public:
	~CBorderLinkChecker() {}
	CBorderLinkChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "BORDER_LINK";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  5 FROM_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  6 TO_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  //  7 IMAGINARY_BODER_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  8 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  5 FROM_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  6 TO_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              //  7 IMAGINARY_BODER_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  8 UPDATE_C 
	}
};

class CBorderNodeChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		NODE_ID,
		POINT_CLASS_C,
		BORDER_LINK_ID,
		SEQUENCE,
		LAST_NODE_F,
		UPDATE_C,
	};

public:
	~CBorderNodeChecker() {}
	CBorderNodeChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "BORDER_NODE";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  5 NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  6 POINT_CLASS_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  7 BORDER_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  8 SEQUENCE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0, false)));  //  9 LAST_NODE_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  // 10 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  5 NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_POINT_CLASS)));   //  6 POINT_CLASS_C
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  7 BORDER_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (0,        LONG_MAX)));   //  8 SEQUENCE
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              //  9 LAST_NODE_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        // 10 UPDATE_C 
	}
};

class CSignalPointChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		HORIZONTAL_ANGLE,
		SIGNAL_MOLD_C,
		LAMP_COUNT,
		ARROW_F,
		WARNING_F,
		INDEPENDENT_F,
		UPDATE_C,
	};

public:
	~CSignalPointChecker() {}
	CSignalPointChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "SIGNAL_POINT";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 3,  true)));  //  5 HORIZONTAL_ANGLE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  6 SIGNAL_MOLD_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 2, 0,  true)));  //  7 LAMP_COUNT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  //  8 ARROW_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  //  9 WARNING_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  // 10 INDEPENDENT_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  // 11 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(0,     359.999)));       //  5 HORIZONTAL_ANGLE
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  ({1, 2})));               //  6 SIGNAL_MOLD_C
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  7 LAMP_COUNT
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              //  8 ARROW_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              //  9 WARNING_F
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              // 10 INDEPENDENT_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        // 11 UPDATE_C 
	}
};

class CRoadSignPointChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		HORIZONTAL_ANGLE,
		ROADSIGN_C,
		VARIABLE_F,
		UPDATE_C,
	};

public:
	~CRoadSignPointChecker() {}
	CRoadSignPointChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "ROADSIGN_POINT";

		using std::string;
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 3,  true)));  //  5 HORIZONTAL_ANGLE
		frmt.push_back(std::shared_ptr<IChecker>(new CStringFormatChecker(32,     true)));  //  6 ROADSIGN_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  //  7 VARIABLE_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  8 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(0,     359.999)));       //  5 HORIZONTAL_ANGLE
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <string>(DOMAIN_ROADSIGN)));      //  6 ROADSIGN_C
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                              //  7 VARIABLE_F
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  8 UPDATE_C 
	}
};

class CCrossWalkLineChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		WIDTH,
		UPDATE_C,
	};

public:
	~CCrossWalkLineChecker() {}
	CCrossWalkLineChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "CROSSWALK_LINE";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 3,  true)));  //  5 WIDTH
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  6 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(0,     999.999)));       //  5 WIDTH
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  6 UPDATE_C 
	}
};

class CCoveringAreaChecker : public CDataChecker {
public:
	enum field_index {
		LON,
		LAT,
		HEIGHT,
		OBJECTID,
		UPDATE_C,
	};

public:
	~CCoveringAreaChecker() {}
	CCoveringAreaChecker () { init(); }

protected:
	void init() {
		has_pos  = true;
		chk_name = "COVERING_AREA";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  1 LON
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 3, 8, false)));  //  2 LAT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 8, 3, false)));  //  3 HEIGHT
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  5 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  1 LON
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-180.,    180.)));       //  2 LAT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<double>(-DBL_MAX, DBL_MAX)));    //  3 HEIGHT
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,        LONG_MAX)));   //  4 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  5 UPDATE_C 
	}
};


//////// ココから関連テーブルのチェッククラス

class SpatialReferenceChecker : public CDataChecker {
public:
	enum field_index {
		SRID,
		NAME,
	};

public:
	~SpatialReferenceChecker() {}
	SpatialReferenceChecker () { init(); }

protected:
	void init() {
		has_pos = false;
		chk_name = "SPATIAL_REF";

		using std::string;
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 SRID
		frmt.push_back(std::shared_ptr<IChecker>(new CStringFormatChecker(128,   false)));  //  2 NAME

		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_SRID)));              //  1 SRID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <string>(DOMAIN_SRNAME)));            //  2 NAME
	}
};

class CVersionChecker : public CDataChecker {
public:
	enum field_index {
		DATA_DEF,
		FORMAT_DEF,
	};

public:
	~CVersionChecker() {}
	CVersionChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "VERSION";

		frmt.push_back(std::shared_ptr<IChecker>(new CStringFormatChecker(128,    true)));  //  1 DATA_DEF
		frmt.push_back(std::shared_ptr<IChecker>(new CStringFormatChecker(128,    true)));  //  2 FORMAT_DEF

		attr.push_back(std::shared_ptr<IChecker>(new CNoneChecker()));                          //  1 DATA_DEF
		attr.push_back(std::shared_ptr<IChecker>(new CFixedFormatChecker(TARGET_FORMATDEF)));   //  2 FORMAT_DEF
	}
};

class CRelRoadLinkLaneLinkChecker : public CDataChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_LINK_ID,
		ROAD_LINK_ID,
		ROAD_DIR_C,
		UPDATE_C,
	};

public:
	~CRelRoadLinkLaneLinkChecker() {}
	CRelRoadLinkLaneLinkChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "REL_ROAD_LINK_LANE_LINK";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  2 LANE_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  3 ROAD_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  4 ROAD_DIR_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  5 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  1 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  2 LANE_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  3 ROAD_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LINE_DIRECTION)));    //  4 ROAD_DIR_C
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));            //  5 UPDATE_C 
	}
};

class CRelCompartLinkLaneLinkChecker : public CDataChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_LINK_ID,
		LR_F,
		COMPART_LINK_ID,
		COMPART_LINK_DIRECTION_C,
		SEQUENCE,
		UPDATE_C,
	};

public:
	~CRelCompartLinkLaneLinkChecker() {}
	CRelCompartLinkLaneLinkChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "REL_COMPART_LINK_LANE_LINK";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  2 LANE_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  //  3 LR_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 COMPART_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  5 COMPART_LINK_DIRECTION_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0,  true)));  //  6 SEQUENCE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  7 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  1 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  2 LANE_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                  //  3 LR_F
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  4 COMPART_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LINE_DIRECTION)));    //  5 COMPART_LINK_DIRECTION_C
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (0,       LONG_MAX)));        //  6 SEQUENCE
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));            //  7 UPDATE_C 
	}
};

class CRelBorderLinkLaneLinkChecker : public CDataChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_LINK_ID,
		LR_F,
		BORDER_LINK_ID,
		BORDER_LINK_DIRECTION_C,
		SEQUENCE,
		UPDATE_C,
	};

public:
	~CRelBorderLinkLaneLinkChecker() {}
	CRelBorderLinkLaneLinkChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "REL_BORDER_LINK_LANE_LINK";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  2 LANE_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 1, 0,  true)));  //  3 LR_F
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  4 BORDER_LINK_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0,  true)));  //  5 BORDER_LINK_DIRECTION_C
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0,  true)));  //  6 SEQUENCE
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  7 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  1 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  2 LANE_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CFlagChecker()));                                  //  3 LR_F
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));        //  4 BORDER_LINK_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_LINE_DIRECTION)));    //  5 BORDER_LINK_DIRECTION_C
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (0,       LONG_MAX)));        //  6 SEQUENCE
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));            //  7 UPDATE_C 
	}
};

class CRelSignalPointLaneNodeChecker : public CDataChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_NODE_ID,
		SIGNAL_POINT_ID,
		UPDATE_C,
	};

public:
	~CRelSignalPointLaneNodeChecker() {}
	CRelSignalPointLaneNodeChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "REL_LANE_NODE_SIGNAL_POINT";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  2 LANE_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  3 SIGNAL_POINT_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false))); //  4 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  1 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  2 LANE_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  3 SIGNAL_POINT_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  4 UPDATE_C 
	}
};

class CRelRoadSignPointLaneNodeChecker : public CDataChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_NODE_ID,
		ROADSIGN_POINT_ID,
		UPDATE_C,
	};

public:
	~CRelRoadSignPointLaneNodeChecker() {}
	CRelRoadSignPointLaneNodeChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "REL_LANE_NODE_ROADSIGN_POINT";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  2 LANE_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  3 ROADSIGN_POINT_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false))); //  4 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  1 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  2 LANE_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  3 ROADSIGN_POINT_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  4 UPDATE_C 
	}
};

class CRelCrossWalkLineLaneNodeChecker : public CDataChecker {
public:
	enum field_index {
		OBJECTID,
		LANE_NODE_ID,
		CROSSWALK_LINE_ID,
		UPDATE_C,
	};

public:
	~CRelCrossWalkLineLaneNodeChecker() {}
	CRelCrossWalkLineLaneNodeChecker () { init(); }

protected:
	void init() {
		has_pos  = false;
		chk_name = "REL_LANE_NODE_CROSSWALK_LINE";

		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  1 OBJECTID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  2 LANE_NODE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker(10, 0, false)));  //  3 CROSSWALK_LINE_ID
		frmt.push_back(std::shared_ptr<IChecker>(new CNumberFormatChecker( 5, 0, false)));  //  4 UPDATE_C

		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  1 OBJECTID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  2 LANE_NODE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CRangeChecker<long>  (1,       LONG_MAX)));    //  3 CROSSWALK_LINE_ID
		attr.push_back(std::shared_ptr<IChecker>(new CListChecker <long>  (DOMAIN_UPDATE)));        //  4 UPDATE_C 
	}
};


typedef CRelRoadLinkLaneLinkChecker             CRelRLChecker;
typedef CRelCompartLinkLaneLinkChecker          CRelCLChecker;
typedef CRelBorderLinkLaneLinkChecker           CRelBLChecker;
typedef CRelSignalPointLaneNodeChecker          CRelSPChecker;
typedef CRelRoadSignPointLaneNodeChecker        CRelRSChecker;
typedef CRelCrossWalkLineLaneNodeChecker        CRelCWChecker;

}
}
}
