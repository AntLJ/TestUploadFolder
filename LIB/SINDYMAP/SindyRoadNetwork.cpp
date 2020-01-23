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
 *	SindyRoadNetwork.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/30		êVãKçÏê¨
 */

#include "SindyRoadNetwork.h"

long sindy::rnw::ROADDATA = sindy::rnw::data_type::road_link      |
                            sindy::rnw::data_type::road_node      |
							sindy::rnw::data_type::segment_attr   |
							sindy::rnw::data_type::grad           |
							sindy::rnw::data_type::turnreg        |
							sindy::rnw::data_type::uturn          |
							sindy::rnw::data_type::vics           |
							sindy::rnw::data_type::inter_navi     |
							sindy::rnw::data_type::byway          |
							sindy::rnw::data_type::route          |
							sindy::rnw::data_type::lane           |
							sindy::rnw::data_type::guide          |
							sindy::rnw::data_type::dirguide       |
							sindy::rnw::data_type::time_nopassage |
							sindy::rnw::data_type::time_oneway    |
							sindy::rnw::data_type::dupli_link     |
							sindy::rnw::data_type::road_code_list |
							sindy::rnw::data_type::parking_shape  |
							sindy::rnw::data_type::intersection;

long sindy::rnw::BASEROAD = sindy::rnw::data_type::road_link      |
                            sindy::rnw::data_type::road_node      |
							sindy::rnw::data_type::segment_attr   |
							sindy::rnw::data_type::grad           |
							sindy::rnw::data_type::turnreg        |
							sindy::rnw::data_type::vics           |
							sindy::rnw::data_type::lane           |
							sindy::rnw::data_type::guide          |
							sindy::rnw::data_type::dirguide       |
							sindy::rnw::data_type::time_nopassage |
							sindy::rnw::data_type::time_oneway	  |
							sindy::rnw::data_type::intersection;

long sindy::rnw::HWYDATA  = sindy::rnw::data_type::facil_info_point |
                            sindy::rnw::data_type::access_point     |
							sindy::rnw::data_type::highway_node     |
							sindy::rnw::data_type::highway_text;
