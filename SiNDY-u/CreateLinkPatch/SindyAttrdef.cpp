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
 *	SindyAttrdef.cpp : 属性定義初期値
 */

#include "StdAfx.h"
#include "SindyAttrdef.h"

std::string sindy::layer::ROAD_LINK = "road_link";
std::string sindy::layer::ROAD_NODE = "road_node";
std::string sindy::layer::SEGMENT_ATTR = "segment_attr";
std::string sindy::layer::GRAD = "grad";
std::string sindy::layer::PARKING_SHAPE = "parking_shape";
std::string sindy::layer::ACCESS_POINT = "access_point";
std::string sindy::layer::FACIL_INFO_POINT = "facil_info_point";
std::string sindy::layer::HIGHWAY_NODE = "highway_node";
std::string sindy::layer::HIGHWAY_TEXT = "highway_text";
std::string sindy::layer::TIME_NOPASSAGE = "time_nopassage";
std::string sindy::layer::TIME_ONEWAY = "time_oneway";
std::string sindy::layer::INF_BYWAY = "inf_byway";
std::string sindy::layer::INF_DIRGUIDE = "inf_dirguide";
std::string sindy::layer::INF_GUIDE = "inf_guide";
std::string sindy::layer::INF_LANE = "inf_lane";
std::string sindy::layer::INF_INTERNAVI = "inf_internavi";
std::string sindy::layer::INF_ROUTE = "inf_route";
std::string sindy::layer::INF_VICS = "inf_vics";
std::string sindy::layer::INF_TURNREG = "inf_turnreg";
std::string sindy::layer::INF_UTURN = "inf_uturn";
std::string sindy::layer::LQ_BYWAY = "lq_byway";
std::string sindy::layer::LQ_DIRGUIDE = "lq_dirguide";
std::string sindy::layer::LQ_GUIDE = "lq_guide";
std::string sindy::layer::LQ_LANE = "lq_lane";
std::string sindy::layer::LQ_INTERNAVI = "lq_internavi";
std::string sindy::layer::LQ_ROUTE = "lq_route";
std::string sindy::layer::LQ_VICS = "lq_vics";
std::string sindy::layer::LQ_TURNREG = "lq_turnreg";
std::string sindy::layer::LQ_UTURN = "lq_uturn";
std::string sindy::layer::BASE_ADMIN = "base_admin";
std::string sindy::layer::BASE_MESH = "reference.basemesh";
std::string sindy::layer::MIDDLE_MESH = "reference.middlemesh";
std::string sindy::layer::CITY_MESH = "reference.citymesh";
std::string sindy::layer::ROAD_CODE_LIST = "road_code_list";
std::string sindy::layer::DUPLI_LINK = "dupli_link";
std::string sindy::layer::MIDDLE_ANNOTATION = "middle_annotation";
std::string sindy::layer::BASE_ANNOTATION = "base_annotation";
std::string sindy::layer::CITY_ANNOTATION = "city_annotation";

std::string sindy::object::OBJECTID = "objectid";
std::string sindy::feature::SHAPE = "shape";
std::string sindy::ipc_table::OPERATOR = "operator";
std::string sindy::ipc_table::PURPOSE_C = "purpose_c";
std::string sindy::ipc_table::MODIFYDATE = "modifydate";
std::string sindy::ipc_table::UPDATE_TYPE = "update_type";
std::string sindy::ipc_table::PROGMODIFYDATE = "progmodifydate";
std::string sindy::ipc_table::MODIFYPROGNAME = "modifyprogname";
std::string sindy::ipc_table::USERCLAIM_F = "userclaim_f";
std::string sindy::ipc_table::SOURCE = "source";
std::string sindy::ipc_table::TMP_MESHCODE = "tmp_meshcode";
std::string sindy::ipc_table::TMP_PRIMID = "tmp_primid";
std::string sindy::meshcode_cls::MESHCODE = "meshcode";
std::string sindy::field_survey::FIELDSURVEY_F = "fieldsurvey_f";
std::string sindy::field_survey::WORK_YEAR = "work_year";

std::string sindy::road_link::TMPFROM_NODE = "tmpfrom_node";
std::string sindy::road_link::TMPTO_NODE = "tmpto_node";
std::string sindy::road_link::FROM_NODE_ID = "from_node_id";
std::string sindy::road_link::TO_NODE_ID = "to_node_id";
std::string sindy::road_link::ROADCLASS_C = "roadclass_c";
std::string sindy::road_link::NAVICLASS_C = "naviclass_c";
std::string sindy::road_link::MAIN_LINKCLASS_C = "main_linkclass_c";
std::string sindy::road_link::SUB_LINKCLASS_C = "sub_linkclass_c";
std::string sindy::road_link::ADMIN_CODE = "admin_code";
std::string sindy::road_link::ROADMANAGER_C = "roadmanager_c";
std::string sindy::road_link::ROADWIDTH_C = "roadwidth_c";
std::string sindy::road_link::WIDEAREA_C = "widearea_c";
std::string sindy::road_link::UPDOWNCLASS_C = "updownclass_c";
std::string sindy::road_link::BYPASS_F = "bypass_f";
std::string sindy::road_link::CARONLY_F = "caronly_f";
std::string sindy::road_link::ISLAND_F = "island_f";
std::string sindy::road_link::ROAD_NO = "road_no";
std::string sindy::road_link::ROAD_CODE = "road_code";
std::string sindy::road_link::NOPASSAGE_C = "nopassage_c";
std::string sindy::road_link::NOPASSAGE_RF = "nopassage_rf";
std::string sindy::road_link::ONEWAY_C = "oneway_c";
std::string sindy::road_link::ONEWAY_RF = "oneway_rf";
std::string sindy::road_link::TRIP_SPEED = "trip_speed";
std::string sindy::road_link::LIMITSPEED_C = "limitspeed_c";
std::string sindy::road_link::KUBUN_C = "kubun_c";
std::string sindy::road_link::LANE_COUNT = "lane_count";
std::string sindy::road_link::RIGHT_CHANGE = "right_change";
std::string sindy::road_link::LEFT_CHANGE = "left_change";
std::string sindy::road_link::LANEVALID_F = "lanevalid_f";
std::string sindy::road_link::DUPLINK_RF = "duplink_rf";
std::string sindy::road_link::TURNREG_LQRF = "turnreg_lqrf";
std::string sindy::road_link::DIRGUIDE_LQRF = "dirguide_lqrf";
std::string sindy::road_link::GUIDE_LQRF = "guide_lqrf";
std::string sindy::road_link::LANE_LQRF = "lane_lqrf";
std::string sindy::road_link::VICS_LQRF = "vics_lqrf";
std::string sindy::road_link::BYWAY_LQRF = "byway_lqrf";
std::string sindy::road_link::ROUTE_LQRF = "route_lqrf";
std::string sindy::road_link::UTURN_LQRF = "uturn_lqrf";
std::string sindy::road_link::INTERNAVI_LQRF = "internavi_lqrf";
std::string sindy::road_link::PARKINGCLASS_C = "parkingclass_c";
std::string sindy::road_link::PARKINGLINKCLASS_C = "parkinglinkclass_c";
std::string sindy::road_link::FLOOR_LEVEL = "floor_level";
std::string sindy::road_link::MIDDLEFLOOR_LEVEL = "middlefloor_level";
std::string sindy::road_link::ALLEYMAINTE_C = "alleymainte_c";
std::string sindy::road_link::ATTR[] = {
	sindy::road_link::ROADCLASS_C,
	sindy::road_link::NAVICLASS_C,
	sindy::road_link::MAIN_LINKCLASS_C,
	sindy::road_link::SUB_LINKCLASS_C,
	sindy::road_link::ROADMANAGER_C,
	sindy::road_link::ROADWIDTH_C,
	sindy::road_link::WIDEAREA_C,
	sindy::road_link::UPDOWNCLASS_C,
	sindy::road_link::BYPASS_F,
	sindy::road_link::CARONLY_F,
	sindy::road_link::ISLAND_F,
	sindy::road_link::ROAD_NO,
	sindy::road_link::ROAD_CODE,
	sindy::road_link::NOPASSAGE_C,
//	sindy::road_link::ONEWAY_C,			一方通行は比較には不要
	sindy::road_link::TRIP_SPEED,
	sindy::road_link::LIMITSPEED_C,
	sindy::road_link::KUBUN_C,
	sindy::road_link::LANE_COUNT,
	sindy::road_link::RIGHT_CHANGE,
	sindy::road_link::LEFT_CHANGE,
	sindy::road_link::LANEVALID_F,
	sindy::road_link::PARKINGCLASS_C,
	sindy::road_link::PARKINGLINKCLASS_C,
	sindy::road_link::FLOOR_LEVEL,
	sindy::road_link::MIDDLEFLOOR_LEVEL,
	sindy::road_link::ALLEYMAINTE_C,
	""
};

std::string sindy::road_node::NODECLASS_C = "nodeclass_c";
std::string sindy::road_node::SIGNAL_C = "signal_c";
std::string sindy::road_node::NAME_KANJI = "name_kanji";
std::string sindy::road_node::NAME_YOMI = "name_yomi";
std::string sindy::road_node::HEIGHT_C = "height_c";
std::string sindy::road_node::HEIGHT = "height";
std::string sindy::road_node::PARKING_ID = "parking_id";
std::string sindy::road_node::ATTR[] = {
	sindy::road_node::SIGNAL_C,
	sindy::road_node::NAME_KANJI,
	sindy::road_node::NAME_YOMI,
	sindy::road_node::HEIGHT_C,
	sindy::road_node::HEIGHT,
	""
};

std::string sindy::link_info::LINK_ID = "link_id";

std::string sindy::segment_attr::LINKATTR_C = "linkattr_c";
std::string sindy::segment_attr::UNDER_OVER = "under_over";
std::string sindy::segment_attr::ATTR[] = {
	sindy::segment_attr::LINKATTR_C,
	sindy::segment_attr::UNDER_OVER,
	""
};

std::string sindy::grad::GRAD_VALUE = "grad_value";
std::string sindy::grad::ATTR[] = {
	sindy::grad::GRAD_VALUE,
	""
};

std::string sindy::parking_shape::PARKING_ORGID = "parking_orgid";
std::string sindy::parking_shape::PARKINGCLASS_C = "parkingclass_c";
std::string sindy::parking_shape::NAME_KANJI = "kanji_name";
std::string sindy::parking_shape::ATTR[] = {
	sindy::parking_shape::PARKING_ORGID,
	sindy::parking_shape::PARKINGCLASS_C,
	sindy::parking_shape::NAME_KANJI,
	""
};

std::string sindy::hwy_facil_code::ROAD_CODE = "road_code";
std::string sindy::hwy_facil_code::ROAD_SEQ = "road_seq";

std::string sindy::hwy_mode::HWYMODE_F = "hwymode_f";

std::string sindy::inout::INOUT_C = "inout_c";

std::string sindy::direction::DIRECTION_C = "direction_c";

std::string sindy::access_point::DIRECTION_KANJI = "direction_kanji";
std::string sindy::access_point::DIRECTION_YOMI = "direction_yomi";
std::string sindy::access_point::GS_C = "gs_c";
std::string sindy::access_point::TOILET_F = "toilet_f";
std::string sindy::access_point::HANDICAP_F = "handicap_f";
std::string sindy::access_point::RESTAURANT_F = "restaurant_f";
std::string sindy::access_point::CAFETERIA_F = "cafeteria_f";
std::string sindy::access_point::SOUVENIR_F = "souvenir_f";
std::string sindy::access_point::RESTING_F = "resting_f";
std::string sindy::access_point::NAP_F = "nap_f";
std::string sindy::access_point::INFODESK_F = "infodesk_f";
std::string sindy::access_point::INFORMATION_F = "infomation_f";
std::string sindy::access_point::SHOWER_F = "shower_f";
std::string sindy::access_point::LAUNDRY_F = "laundry_f";
std::string sindy::access_point::SPA_F = "spa_f";
std::string sindy::access_point::FAX_F = "fax_f";
std::string sindy::access_point::MAILBOX_F = "mailbox_f";
std::string sindy::access_point::CASHSERVICE_F = "cashservice_f";
std::string sindy::access_point::HWYOASIS_F = "hwyoasis_f";
std::string sindy::access_point::CARWASH_F = "carwash_f";
std::string sindy::access_point::ATTR[] = {
	sindy::access_point::ROAD_CODE,
	sindy::access_point::ROAD_SEQ,
	sindy::access_point::HWYMODE_F,
	sindy::access_point::DIRECTION_C,
	sindy::access_point::INOUT_C,
	sindy::access_point::DIRECTION_KANJI,
	sindy::access_point::DIRECTION_YOMI,
	sindy::access_point::GS_C,
	sindy::access_point::TOILET_F,
	sindy::access_point::HANDICAP_F,
	sindy::access_point::RESTAURANT_F,
	sindy::access_point::CAFETERIA_F,
	sindy::access_point::SOUVENIR_F,
	sindy::access_point::RESTING_F,
	sindy::access_point::NAP_F,
	sindy::access_point::INFODESK_F,
	sindy::access_point::INFORMATION_F,
	sindy::access_point::SHOWER_F,
	sindy::access_point::LAUNDRY_F,
	sindy::access_point::SPA_F,
	sindy::access_point::FAX_F,
	sindy::access_point::MAILBOX_F,
	sindy::access_point::CASHSERVICE_F,
	sindy::access_point::HWYOASIS_F,
	sindy::access_point::CARWASH_F,
	""
};
std::string	sindy::access_point::SAPAINFO[] = {
	sindy::access_point::GS_C,
	sindy::access_point::HANDICAP_F,
	sindy::access_point::RESTAURANT_F,
	sindy::access_point::SOUVENIR_F,
	sindy::access_point::RESTING_F,
	sindy::access_point::NAP_F,
	sindy::access_point::INFORMATION_F,
	sindy::access_point::SHOWER_F,
	sindy::access_point::LAUNDRY_F,
	sindy::access_point::SPA_F,
	sindy::access_point::FAX_F,
	sindy::access_point::MAILBOX_F,
	sindy::access_point::CASHSERVICE_F,
	""
};
std::string sindy::access_point::NEWSAPAINFO[] = {
	sindy::access_point::GS_C,
	sindy::access_point::TOILET_F,
	sindy::access_point::HANDICAP_F,
	sindy::access_point::RESTAURANT_F,
	sindy::access_point::CAFETERIA_F,
	sindy::access_point::SOUVENIR_F,
	sindy::access_point::RESTING_F,
	sindy::access_point::NAP_F,
	sindy::access_point::INFODESK_F,
	sindy::access_point::INFORMATION_F,
	sindy::access_point::SHOWER_F,
	sindy::access_point::LAUNDRY_F,
	sindy::access_point::SPA_F,
	sindy::access_point::FAX_F,
	sindy::access_point::MAILBOX_F,
	sindy::access_point::CASHSERVICE_F,
	sindy::access_point::HWYOASIS_F,
	sindy::access_point::CARWASH_F,
	""
};

std::string sindy::facil_info_point::NAME_KANJI = "name_kanji";
std::string sindy::facil_info_point::NAME_YOMI = "name_yomi";
std::string sindy::facil_info_point::DUMMYNAME_F = "dummyname_f";
std::string sindy::facil_info_point::FACILCLASS_C = "facilclass_c";
std::string sindy::facil_info_point::AUTOPOSITION_F = "autoposition_f";
std::string sindy::facil_info_point::ATTR[] = {
	sindy::facil_info_point::ROAD_CODE,
	sindy::facil_info_point::ROAD_SEQ,
	sindy::facil_info_point::NAME_KANJI,
	sindy::facil_info_point::NAME_YOMI,
	sindy::facil_info_point::DUMMYNAME_F,
	sindy::facil_info_point::FACILCLASS_C,
	sindy::facil_info_point::AUTOPOSITION_F,
	""
};

std::string sindy::highway_node::TOLLCLASS_C = "tollclass_c";
std::string sindy::highway_node::TOLLFUNC_C = "tollfunc_c";
std::string sindy::highway_node::DUMMYTOLL_F = "dummytoll_f";
std::string sindy::highway_node::GUIDE_F = "guide_f";
std::string sindy::highway_node::ROAD_SEQ2 = "road_seq2";
std::string sindy::highway_node::APPROACHNODE_ID = "approachnode_id";
std::string sindy::highway_node::FLOWNODE_ID = "flownode_id";
std::string sindy::highway_node::ATTR[] = {
	sindy::highway_node::ROAD_CODE,
	sindy::highway_node::ROAD_SEQ,
	sindy::highway_node::HWYMODE_F,
	sindy::highway_node::INOUT_C,
	sindy::highway_node::DIRECTION_C,
	sindy::highway_node::TOLLCLASS_C,
	sindy::highway_node::TOLLFUNC_C,
	sindy::highway_node::DUMMYTOLL_F,
	sindy::highway_node::GUIDE_F,
	sindy::highway_node::ROAD_SEQ2,
	""
};

std::string sindy::highway_text::POINT_ID = "point_id";
std::string sindy::highway_text::ORDER_SEQ = "order_seq";
std::string sindy::highway_text::UPDOWN_C = "updown_c";
std::string sindy::highway_text::NAME_KANJI = "name_kanji";
std::string sindy::highway_text::PREV_ID = "prev_id";
std::string sindy::highway_text::NEXT_ID = "next_id";
std::string sindy::highway_text::PARENT_ID = "parent_id";
std::string sindy::highway_text::FORE_COUNT = "fore_count";
std::string sindy::highway_text::FORE1_ID = "fore1_id";
std::string sindy::highway_text::FORE2_ID = "fore2_id";
std::string sindy::highway_text::FORE3_ID = "fore3_id";
std::string sindy::highway_text::FORE4_ID = "fore4_id";
std::string sindy::highway_text::BACK_COUNT = "back_count";
std::string sindy::highway_text::BACK1_ID = "back1_id";
std::string sindy::highway_text::BACK2_ID = "back2_id";
std::string sindy::highway_text::BACK3_ID = "back3_id";
std::string sindy::highway_text::BACK4_ID = "back4_id";
std::string sindy::highway_text::ATTR[] = {
	sindy::highway_text::ROAD_CODE,
	sindy::highway_text::ROAD_SEQ,
	sindy::highway_text::INOUT_C,
	sindy::highway_text::UPDOWN_C,
	sindy::highway_text::NAME_KANJI,
	""
};
std::string sindy::highway_text::FOREID[] = {
	sindy::highway_text::FORE1_ID,
	sindy::highway_text::FORE2_ID,
	sindy::highway_text::FORE3_ID,
	sindy::highway_text::FORE4_ID,
	""
};
std::string sindy::highway_text::BACKID[] = {
	sindy::highway_text::BACK1_ID,
	sindy::highway_text::BACK2_ID,
	sindy::highway_text::BACK3_ID,
	sindy::highway_text::BACK4_ID,
	""
};

std::string sindy::base_admin::CITYCODE = "citycode";
std::string sindy::base_admin::ATTR[] = {
	sindy::base_admin::CITYCODE,
	""
};

std::string sindy::base_mesh::ATTR[] = {
	sindy::base_mesh::MESHCODE,
	""
};

std::string sindy::middle_mesh::MIDDLEMESHCODE = "middlemeshcode";
std::string sindy::city_mesh::CITYMESHCODE = "citymeshcode";
std::string sindy::city_mesh::CITYNAME = "cityname";
std::string sindy::city_mesh::CREATE_YEAR = "create_year";
std::string sindy::city_mesh::UPDATE_YEAR = "update_year";
std::string sindy::city_mesh::COMPLETE_F = "complete_f";
std::string sindy::city_mesh::SEA_F = "sea_f";
std::string sindy::city_mesh::ZUKA_F = "zuka_f";
std::string sindy::city_mesh::RASTER = "raster";
std::string sindy::city_mesh::CREATE_RASTER_YEAR = "create_raster_year";
std::string sindy::city_mesh::CURRENT_ADJ_YEAR = "current_adj_year";
std::string sindy::city_mesh::PHOTO_SHOT_DATE = "photo_shot_date";
std::string sindy::city_mesh::FIELD_RESEARCH_YEAR = "field_research_year";
std::string sindy::city_mesh::SHA_BGWORK_YEAR = "sha_bgwork_year";
std::string sindy::city_mesh::SHA_ANNOWORK_YEAR = "sha_annowork_year";
std::string sindy::city_mesh::CREATE_ANNO_MS_DATE = "create_anno_ms_date";
std::string sindy::city_mesh::ADDR_MAINTE_YEAR = "addr_mainte_year";
std::string sindy::city_mesh::JMAP_PUBLICATION_YEAR = "jmap_publication_year";
std::string sindy::city_mesh::MAPCUBE_WORK_YEAR = "mapcube_work_year";
std::string sindy::city_mesh::PDM_USE_YEAR = "pdm_use_year";

std::string sindy::dupli_link::KUBUN_C = "kubun_c";
std::string sindy::dupli_link::DUPCLASS_C = "dupclass_c";
std::string sindy::dupli_link::ROAD_NO = "road_no";
std::string sindy::dupli_link::ROAD_CODE = "road_code";
std::string sindy::dupli_link::ATTR[] = {
	sindy::dupli_link::KUBUN_C,
	sindy::dupli_link::DUPCLASS_C,
	sindy::dupli_link::ROAD_NO,
	sindy::dupli_link::ROAD_CODE,
	""
};

std::string sindy::linkdir::LINKDIR_C = "linkdir_c";

std::string sindy::link_queue::TMP_LINK_ID = "tmp_link_id";
std::string sindy::link_queue::SEQUENCE = "sequence";
std::string sindy::link_queue::LASTLINK_F = "lastlink_f";
std::string sindy::link_queue::INF_ID = "inf_id";

std::string sindy::inf_byway::BYWAY_F = "byway_f";
std::string sindy::inf_byway::BACKROAD_F = "backroad_f";
std::string sindy::inf_byway::SOURCE_NAME = "source_name";
std::string sindy::inf_byway::ATTR[] = {
	sindy::inf_byway::BYWAY_F,
	sindy::inf_byway::BACKROAD_F,
	""
};

std::string sindy::inf_route::ROUTECLASS_C = "routeclass_c";
std::string sindy::inf_route::SOURCE_NAME = "source_name";
std::string sindy::inf_route::ATTR[] = {
	sindy::inf_route::ROUTECLASS_C,
	""
};

std::string sindy::inf_dirguide::GUIDECLASS1_C = "guideclass1_c";
std::string sindy::inf_dirguide::NAME_KANJI1 = "name_kanji1";
std::string sindy::inf_dirguide::NAME_YOMI1 = "name_yomi1";
std::string sindy::inf_dirguide::GUIDECLASS2_C = "guideclass2_c";
std::string sindy::inf_dirguide::NAME_KANJI2 = "name_kanji2";
std::string sindy::inf_dirguide::NAME_YOMI2 = "name_yomi2";
std::string sindy::inf_dirguide::GUIDECLASS3_C = "guideclass3_c";
std::string sindy::inf_dirguide::NAME_KANJI3 = "name_kanji3";
std::string sindy::inf_dirguide::NAME_YOMI3 = "name_yomi3";
std::string sindy::inf_dirguide::GUIDECLASS4_C = "guideclass4_c";
std::string sindy::inf_dirguide::NAME_KANJI4 = "name_kanji4";
std::string sindy::inf_dirguide::NAME_YOMI4 = "name_yomi4";
std::string sindy::inf_dirguide::GUIDECLASS5_C = "guideclass5_c";
std::string sindy::inf_dirguide::NAME_KANJI5 = "name_kanji5";
std::string sindy::inf_dirguide::NAME_YOMI5 = "name_yomi5";
std::string sindy::inf_dirguide::GUIDECLASS6_C = "guideclass6_c";
std::string sindy::inf_dirguide::NAME_KANJI6 = "name_kanji6";
std::string sindy::inf_dirguide::NAME_YOMI6 = "name_yomi6";
std::string sindy::inf_dirguide::GUIDECLASS7_C = "guideclass7_c";
std::string sindy::inf_dirguide::NAME_KANJI7 = "name_kanji7";
std::string sindy::inf_dirguide::NAME_YOMI7 = "name_yomi7";
std::string sindy::inf_dirguide::GUIDECLASS8_C = "guideclass8_c";
std::string sindy::inf_dirguide::NAME_KANJI8 = "name_kanji8";
std::string sindy::inf_dirguide::NAME_YOMI8 = "name_yomi8";
std::string sindy::inf_dirguide::GUIDECLASS9_C = "guideclass9_c";
std::string sindy::inf_dirguide::NAME_KANJI9 = "name_kanji9";
std::string sindy::inf_dirguide::NAME_YOMI9 = "name_yomi9";
std::string sindy::inf_dirguide::GUIDECLASS10_C = "guideclass10_c";
std::string sindy::inf_dirguide::NAME_KANJI10 = "name_kanji10";
std::string sindy::inf_dirguide::NAME_YOMI10 = "name_yomi10";
std::string sindy::inf_dirguide::ATTR[] = {
	sindy::inf_dirguide::GUIDECLASS1_C,
	sindy::inf_dirguide::NAME_KANJI1,
	sindy::inf_dirguide::NAME_YOMI1,
	sindy::inf_dirguide::GUIDECLASS2_C,
	sindy::inf_dirguide::NAME_KANJI2,
	sindy::inf_dirguide::NAME_YOMI2,
	sindy::inf_dirguide::GUIDECLASS3_C,
	sindy::inf_dirguide::NAME_KANJI3,
	sindy::inf_dirguide::NAME_YOMI3,
	sindy::inf_dirguide::GUIDECLASS4_C,
	sindy::inf_dirguide::NAME_KANJI4,
	sindy::inf_dirguide::NAME_YOMI4,
	sindy::inf_dirguide::GUIDECLASS5_C,
	sindy::inf_dirguide::NAME_KANJI5,
	sindy::inf_dirguide::NAME_YOMI5,
	sindy::inf_dirguide::GUIDECLASS6_C,
	sindy::inf_dirguide::NAME_KANJI6,
	sindy::inf_dirguide::NAME_YOMI6,
	sindy::inf_dirguide::GUIDECLASS7_C,
	sindy::inf_dirguide::NAME_KANJI7,
	sindy::inf_dirguide::NAME_YOMI7,
	sindy::inf_dirguide::GUIDECLASS8_C,
	sindy::inf_dirguide::NAME_KANJI8,
	sindy::inf_dirguide::NAME_YOMI8,
	sindy::inf_dirguide::GUIDECLASS9_C,
	sindy::inf_dirguide::NAME_KANJI9,
	sindy::inf_dirguide::NAME_YOMI9,
	sindy::inf_dirguide::GUIDECLASS10_C,
	sindy::inf_dirguide::NAME_KANJI10,
	sindy::inf_dirguide::NAME_YOMI10,
	""
};

std::string sindy::inf_guide::GUIDETYPE_C = "guidetype_c";
std::string sindy::inf_guide::ATTR[] = {
	sindy::inf_guide::GUIDETYPE_C,
	""
};

std::string sindy::inf_lane::LANEDIR_C = "lanedir_c";
std::string sindy::inf_lane::APPROACH_LANE = "approach_lane";
std::string sindy::inf_lane::RIGHT_CHANGE = "right_change";
std::string sindy::inf_lane::LEFT_CHANGE = "left_change";
std::string sindy::inf_lane::FLOW_LANE = "flow_lane";
std::string sindy::inf_lane::LANE_PASSABLE = "lane_passable";
std::string sindy::inf_lane::ATTR[] = {
	sindy::inf_lane::LANEDIR_C,
	sindy::inf_lane::APPROACH_LANE,
	sindy::inf_lane::RIGHT_CHANGE,
	sindy::inf_lane::LEFT_CHANGE,
	sindy::inf_lane::FLOW_LANE,
	sindy::inf_lane::LANE_PASSABLE,
	""
};

std::string sindy::inf_vics::VICS_MESHCODE = "vics_meshcode";
std::string sindy::inf_vics::VICSCLASS_C = "vicsclass_c";
std::string sindy::inf_vics::VICS_LINK_ID = "vics_link_id";
std::string sindy::inf_vics::ATTR[] = {
	sindy::inf_vics::VICS_MESHCODE,
	sindy::inf_vics::VICSCLASS_C,
	sindy::inf_vics::VICS_LINK_ID,
	""
};

std::string sindy::inf_turnreg::START_MONTH1 = "start_month1";
std::string sindy::inf_turnreg::START_DAY1 = "start_day1";
std::string sindy::inf_turnreg::START_HOUR1 = "start_hour1";
std::string sindy::inf_turnreg::START_MIN1 = "start_min1";
std::string sindy::inf_turnreg::END_MONTH1 = "end_month1";
std::string sindy::inf_turnreg::END_DAY1 = "end_day1";
std::string sindy::inf_turnreg::END_HOUR1 = "end_hour1";
std::string sindy::inf_turnreg::END_MIN1 = "end_min1";
std::string sindy::inf_turnreg::DAYOFWEEK1_C = "dayofweek1_c";
std::string sindy::inf_turnreg::START_MONTH2 = "start_month2";
std::string sindy::inf_turnreg::START_DAY2 = "start_day2";
std::string sindy::inf_turnreg::START_HOUR2 = "start_hour2";
std::string sindy::inf_turnreg::START_MIN2 = "start_min2";
std::string sindy::inf_turnreg::END_MONTH2 = "end_month2";
std::string sindy::inf_turnreg::END_DAY2 = "end_day2";
std::string sindy::inf_turnreg::END_HOUR2 = "end_hour2";
std::string sindy::inf_turnreg::END_MIN2 = "end_min2";
std::string sindy::inf_turnreg::DAYOFWEEK2_C = "dayofweek2_c";
std::string sindy::inf_turnreg::START_MONTH3 = "start_month3";
std::string sindy::inf_turnreg::START_DAY3 = "start_day3";
std::string sindy::inf_turnreg::START_HOUR3 = "start_hour3";
std::string sindy::inf_turnreg::START_MIN3 = "start_min3";
std::string sindy::inf_turnreg::END_MONTH3 = "end_month3";
std::string sindy::inf_turnreg::END_DAY3 = "end_day3";
std::string sindy::inf_turnreg::END_HOUR3 = "end_hour3";
std::string sindy::inf_turnreg::END_MIN3 = "end_min3";
std::string sindy::inf_turnreg::DAYOFWEEK3_C = "dayofweek3_c";
std::string sindy::inf_turnreg::START_MONTH4 = "start_month4";
std::string sindy::inf_turnreg::START_DAY4 = "start_day4";
std::string sindy::inf_turnreg::START_HOUR4 = "start_hour4";
std::string sindy::inf_turnreg::START_MIN4 = "start_min4";
std::string sindy::inf_turnreg::END_MONTH4 = "end_month4";
std::string sindy::inf_turnreg::END_DAY4 = "end_day4";
std::string sindy::inf_turnreg::END_HOUR4 = "end_hour4";
std::string sindy::inf_turnreg::END_MIN4 = "end_min4";
std::string sindy::inf_turnreg::DAYOFWEEK4_C = "dayofweek4_c";
std::string sindy::inf_turnreg::ATTR[] = {
	sindy::inf_turnreg::START_MONTH1,
	sindy::inf_turnreg::START_DAY1,
	sindy::inf_turnreg::START_HOUR1,
	sindy::inf_turnreg::START_MIN1,
	sindy::inf_turnreg::END_MONTH1,
	sindy::inf_turnreg::END_DAY1,
	sindy::inf_turnreg::END_HOUR1,
	sindy::inf_turnreg::END_MIN1,
	sindy::inf_turnreg::DAYOFWEEK1_C,
	sindy::inf_turnreg::START_MONTH2,
	sindy::inf_turnreg::START_DAY2,
	sindy::inf_turnreg::START_HOUR2,
	sindy::inf_turnreg::START_MIN2,
	sindy::inf_turnreg::END_MONTH2,
	sindy::inf_turnreg::END_DAY2,
	sindy::inf_turnreg::END_HOUR2,
	sindy::inf_turnreg::END_MIN2,
	sindy::inf_turnreg::DAYOFWEEK2_C,
	sindy::inf_turnreg::START_MONTH3,
	sindy::inf_turnreg::START_DAY3,
	sindy::inf_turnreg::START_HOUR3,
	sindy::inf_turnreg::START_MIN3,
	sindy::inf_turnreg::END_MONTH3,
	sindy::inf_turnreg::END_DAY3,
	sindy::inf_turnreg::END_HOUR3,
	sindy::inf_turnreg::END_MIN3,
	sindy::inf_turnreg::DAYOFWEEK3_C,
	sindy::inf_turnreg::START_MONTH4,
	sindy::inf_turnreg::START_DAY4,
	sindy::inf_turnreg::START_HOUR4,
	sindy::inf_turnreg::START_MIN4,
	sindy::inf_turnreg::END_MONTH4,
	sindy::inf_turnreg::END_DAY4,
	sindy::inf_turnreg::END_HOUR4,
	sindy::inf_turnreg::END_MIN4,
	sindy::inf_turnreg::DAYOFWEEK4_C,
	""
};

std::string sindy::inf_uturn::SOURCE_NAME = "source_name";

std::string sindy::road_code_list::ROAD_CODE = "road_code";
std::string sindy::road_code_list::LINECLASS_C = "lineclass_c";
std::string sindy::road_code_list::LINEDIRTYPE_C = "linedirtype_c";
std::string sindy::road_code_list::LANDCLASS_C = "landclass_c";
std::string sindy::road_code_list::NAME_KANJI = "name_kanji";
std::string sindy::road_code_list::NAME_YOMI = "name_yomi";
std::string sindy::road_code_list::WORK_KANJI = "work_kanji";
std::string sindy::road_code_list::WORK_YOMI = "work_yomi";
std::string sindy::road_code_list::ATTR[] = {
	sindy::road_code_list::ROAD_CODE,
	sindy::road_code_list::LINECLASS_C,
	sindy::road_code_list::LINEDIRTYPE_C,
	sindy::road_code_list::LANDCLASS_C,
	sindy::road_code_list::NAME_KANJI,
	sindy::road_code_list::NAME_YOMI,
	sindy::road_code_list::WORK_KANJI,
	sindy::road_code_list::WORK_YOMI,
	""
};

std::string sindy::link_rest::MESHCODE = "meshcode";
std::string sindy::link_rest::START_MONTH = "start_month";
std::string sindy::link_rest::START_DAY = "start_day";
std::string sindy::link_rest::START_HOUR = "start_hour";
std::string sindy::link_rest::START_MIN = "start_min";
std::string sindy::link_rest::END_MONTH = "end_month";
std::string sindy::link_rest::END_DAY = "end_day";
std::string sindy::link_rest::END_HOUR = "end_hour";
std::string sindy::link_rest::END_MIN = "end_min";
std::string sindy::link_rest::DAYOFWEEK_C = "dayofweek_c";
std::string sindy::link_rest::ATTR[] = {
	sindy::link_rest::START_MONTH,
	sindy::link_rest::START_DAY,
	sindy::link_rest::START_HOUR,
	sindy::link_rest::START_MIN,
	sindy::link_rest::END_MONTH,
	sindy::link_rest::END_DAY,
	sindy::link_rest::END_HOUR,
	sindy::link_rest::END_MIN,
	sindy::link_rest::DAYOFWEEK_C,
	""
};


std::string sindy::annotation::NAME_STRING1 = "name_string1";
std::string sindy::annotation::NAME_STRING2 = "name_string2";
std::string sindy::annotation::NAME_STRING3 = "name_string3";
std::string sindy::annotation::STRING_NUM1 = "string_num1";
std::string sindy::annotation::STRING_NUM2 = "string_num2";
std::string sindy::annotation::STRING_NUM3 = "string_num3";
std::string sindy::annotation::ANNOCLASS_C = "annoclass_c";
std::string sindy::annotation::TMP_ANNO_CLASS = "tmp_anno_class";

std::string sindy::middle_annotation::SC1DISPTYPE_C = "sc1disptype_c";
std::string sindy::middle_annotation::SC2DISPTYPE_C = "sc2disptype_c";
std::string sindy::middle_annotation::SC3DISPTYPE_C = "sc3disptype_c";
std::string sindy::middle_annotation::SC4DISPTYPE_C = "sc4disptype_c";

std::string sindy::base_annotation::SC1DISPTYPE_C = "sc1disptype_c";
std::string sindy::base_annotation::SC2DISPTYPE_C = "sc2disptype_c";
std::string sindy::base_annotation::SC3DISPTYPE_C = "sc3disptype_c";
std::string sindy::base_annotation::SC4DISPTYPE_C = "sc4disptype_c";

std::string sindy::city_annotation::SCDISPTYPE_C = "scdisptype_c";
