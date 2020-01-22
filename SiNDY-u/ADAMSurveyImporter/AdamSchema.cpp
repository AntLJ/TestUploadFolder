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
#include "AdamSchema.h"

using namespace adam_v2;

const uh::tstring	AdamSchema::UpdownLink::TableName				=	sindy::schema::adam_v2::updown_link::kTableName						;	///< テーブル名
const uh::tstring	AdamSchema::UpdownNode::TableName				=	sindy::schema::adam_v2::updown_node::kTableName						;	///< テーブル名
const uh::tstring	AdamSchema::LaneLink::TableName					=	sindy::schema::adam_v2::lane_link::kTableName						;	///< テーブル名
const uh::tstring	AdamSchema::LaneNode::TableName					=	sindy::schema::adam_v2::lane_node::kTableName						;	///< テーブル名
const uh::tstring	AdamSchema::CompartLink::TableName				=	sindy::schema::adam_v2::compart_link::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::CompartNode::TableName				=	sindy::schema::adam_v2::compart_node::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::BorderLink::TableName				=	sindy::schema::adam_v2::border_link::kTableName						;	///< テーブル名
const uh::tstring	AdamSchema::BorderNode::TableName				=	sindy::schema::adam_v2::border_node::kTableName						;	///< テーブル名
const uh::tstring	AdamSchema::SignalPoint::TableName				=	sindy::schema::adam_v2::signal_point::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::CrossWalkLine::TableName			=	sindy::schema::adam_v2::crosswalk_line::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::RoadSignPoint::TableName			=	sindy::schema::adam_v2::roadsign_point::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::MaintenanceArea::TableName			=	sindy::schema::adam_v2::maintenance_area::kTableName				;	///< テーブル名
const uh::tstring	AdamSchema::RoadMarkArea::TableName				=	sindy::schema::adam_v2::roadmark_area::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::VirtualCompartLine::TableName		=	sindy::schema::adam_v2::virtual_compart_line::kTableName			;	///< テーブル名
const uh::tstring	AdamSchema::CoveringArea::TableName				=	sindy::schema::adam_v2::covering_area::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::RelRoadLinkUpdownLink::TableName	=	sindy::schema::adam_v2::rel_road_link_updown_link::kTableName		;	///< テーブル名
const uh::tstring	AdamSchema::PairUpdownLink::TableName			=	sindy::schema::adam_v2::pair_updown_link::kTableName				;	///< テーブル名
const uh::tstring	AdamSchema::UpdownLinkGroup::TableName			=	sindy::schema::adam_v2::updown_link_group::kTableName				;	///< テーブル名
const uh::tstring	AdamSchema::RelUpdownLinkLaneLink::TableName	=	sindy::schema::adam_v2::rel_updown_link_lane_link::kTableName		;	///< テーブル名
const uh::tstring	AdamSchema::RelRoadLinkLaneLink::TableName		=	sindy::schema::adam_v2::rel_road_link_lane_link::kTableName			;	///< テーブル名
const uh::tstring	AdamSchema::LaneLinkGroup::TableName			=	sindy::schema::adam_v2::lane_link_group::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::RelCompartLinkLaneLink::TableName	=	sindy::schema::adam_v2::rel_compart_link_lane_link::kTableName		;	///< テーブル名
const uh::tstring	AdamSchema::RelBorderLinkLaneLink::TableName	=	sindy::schema::adam_v2::rel_border_link_lane_link::kTableName		;	///< テーブル名
const uh::tstring	AdamSchema::RelUpdoenNodeSignalPoint::TableName	=	sindy::schema::adam_v2::rel_updown_node_signal_point::kTableName	;	///< テーブル名
const uh::tstring	AdamSchema::RelLaneNodeSignalPoint::TableName	=	sindy::schema::adam_v2::rel_lane_node_signal_point::kTableName		;	///< テーブル名
const uh::tstring	AdamSchema::RelLaneNodeCrossWalkLine::TableName	=	sindy::schema::adam_v2::rel_lane_node_crosswalk_line::kTableName	;	///< テーブル名
const uh::tstring	AdamSchema::RelLaneNodeRoadSignPoint::TableName	=	sindy::schema::adam_v2::rel_lane_node_roadsign_point::kTableName	;	///< テーブル名
const uh::tstring	AdamSchema::RelRoadMarkAreaLaneLink::TableName	=	sindy::schema::adam_v2::rel_roadmark_area_lane_link::kTableName		;	///< テーブル名
const uh::tstring	AdamSchema::NQUpdownNode::TableName				=	sindy::schema::adam_v2::nq_updown_node::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::NQLaneNode::TableName				=	sindy::schema::adam_v2::nq_lane_node::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::NQCompartNode::TableName			=	sindy::schema::adam_v2::nq_compart_node::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::NQBorderNode::TableName				=	sindy::schema::adam_v2::nq_border_node::kTableName					;	///< テーブル名
const uh::tstring	AdamSchema::AdamVersion::TableName				=	sindy::schema::adam_v2::adam_version::kTableName					;	///< テーブル名

void AdamSchema::Clear()
{
	//インポート対象外はコメントアウトしておく
	//featureUpdownLink				= nullptr;
	//featureUpdownNode				= nullptr;
	featureLaneLink					= nullptr;
	featureLaneNode					= nullptr;
	featureCompartLink				= nullptr;
	featureCompartNode				= nullptr;
	featureBorderLink				= nullptr;
	featureBorderNode				= nullptr;
	featureSignalPoint				= nullptr;
	featureCrossWalkLine			= nullptr;
	featureRoadSignPoint			= nullptr;
	//featureMaintenanceArea			= nullptr;
	//featureRoadMarkArea				= nullptr;	
	featureVirtualCompartLine		= nullptr;
	featureCoveringArea				= nullptr;

	//tableRelRoadLinkUpdownLink		= nullptr;
	//tablePairUpdownLink				= nullptr;
	//tableUpdownLinkGroup			= nullptr;
	//tableRelUpdownLinkLaneLink		= nullptr;
	tableRelRoadLinkLaneLink		= nullptr;
	tableLaneLinkGroup				= nullptr;
	tableRelCompartLinkLaneLink		= nullptr;
	tableRelBorderLinkLaneLink		= nullptr;
	//tableRelUpdoenNodeSignalPoint	= nullptr;
	tableRelLaneNodeSignalPoint		= nullptr;
	tableRelLaneNodeCrossWalkLine	= nullptr;
	tableRelLaneNodeRoadSignPoint	= nullptr;
	//tableRelRoadMarkAreaLaneLink	= nullptr;
	//tableNQUpdownNode				= nullptr;
	tableNQLaneNode					= nullptr;
	tableNQCompartNode				= nullptr;
	tableNQBorderNode				= nullptr;
	//tableAdamVersion				= nullptr;
}