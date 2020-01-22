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

#include <boost/assign.hpp>

#include "SurveyCSV.h"

using namespace adam_v2;

// フォーマット仕様書バージョン
const uh::tstring	SurveyCSV::CONST_FMT_VERSION						=	_T("1.0.3")	;	///< フォーマット仕様書バージョン
const std::vector<uh::tstring>	SurveyCSV::CONST_ALLOW_FMT_VERSION_LIST	=	boost::assign::list_of(_T("1.0.0"));	///< 許容するフォーマット仕様書バージョンリスト

const long			SurveyCSV::GEOMETRY_ROW_COUNT			=	3			;	///< ジオメトリ定義列数

// デリミタ
const uh::tstring	SurveyCSV::DELIMITER					=	_T(",")	;

const std::string	SurveyCSV::HEADER_PREFIX				=	"#"			;	///< ヘッダ開始文字
const long			SurveyCSV::HEADER_COUNT					=	1			;	///< ヘッダ行数

//// 文字コード
//const char*			SurveyCSV::FILE_MODE					=	"r, ccs=UTF-8";

// ファイル名
const uh::tstring	SurveyCSV::LaneLink::FILE_NAME			= _T("lane_link.csv");
const int			SurveyCSV::LaneLink::ROW_COUNT			= 22;
const uh::tstring	SurveyCSV::LanePoint::FILE_NAME			= _T("lane_point.csv");
const int			SurveyCSV::LanePoint::ROW_COUNT			= 20;
const uh::tstring	SurveyCSV::RelLaneLink::FILE_NAME		= _T("rel_road_link_lane_link.csv");
const int			SurveyCSV::RelLaneLink::ROW_COUNT		= 5;
const uh::tstring	SurveyCSV::CompartLink::FILE_NAME		= _T("compartment_link.csv");
const int			SurveyCSV::CompartLink::ROW_COUNT		= 11;
const uh::tstring	SurveyCSV::CompartPoint::FILE_NAME		= _T("compartment_point.csv");
const int			SurveyCSV::CompartPoint::ROW_COUNT		= 11;
const uh::tstring	SurveyCSV::RelCompartLink::FILE_NAME	= _T("rel_lane_link_compartment_link.csv");
const int			SurveyCSV::RelCompartLink::ROW_COUNT	= 7;
const uh::tstring	SurveyCSV::BorderLink::FILE_NAME		= _T("border_link.csv");
const int			SurveyCSV::BorderLink::ROW_COUNT		= 8;
const uh::tstring	SurveyCSV::BorderPoint::FILE_NAME		= _T("border_point.csv");
const int			SurveyCSV::BorderPoint::ROW_COUNT		= 10;
const uh::tstring	SurveyCSV::RelBorderLink::FILE_NAME		= _T("rel_border_link.csv");
const int			SurveyCSV::RelBorderLink::ROW_COUNT		= 7;
const uh::tstring	SurveyCSV::SignalPoint::FILE_NAME		= _T("signal_point.csv");
const int			SurveyCSV::SignalPoint::ROW_COUNT		= 11;
const uh::tstring	SurveyCSV::RelSignalPoint::FILE_NAME	= _T("rel_lane_point_signal_point.csv");
const int			SurveyCSV::RelSignalPoint::ROW_COUNT	= 4;
const uh::tstring	SurveyCSV::RoadSignPoint::FILE_NAME		= _T("roadsign_point.csv");
const int			SurveyCSV::RoadSignPoint::ROW_COUNT		= 8;
const uh::tstring	SurveyCSV::RelRoadSignPoint::FILE_NAME	= _T("rel_lane_point_roadsign_point.csv");
const int			SurveyCSV::RelRoadSignPoint::ROW_COUNT	= 4;
const uh::tstring	SurveyCSV::CrossWalkLine::FILE_NAME		= _T("crosswalk_line.csv");
const int			SurveyCSV::CrossWalkLine::ROW_COUNT		= 6;
const uh::tstring	SurveyCSV::RelCrossWalkLine::FILE_NAME	= _T("rel_lane_point_crosswalk_line.csv");
const int			SurveyCSV::RelCrossWalkLine::ROW_COUNT	= 4;
const uh::tstring	SurveyCSV::VirtualCompartLink::FILE_NAME	= _T("virtual_compartment_link.csv");
const int			SurveyCSV::VirtualCompartLink::ROW_COUNT	= 10;
const uh::tstring	SurveyCSV::VirtualCompartPoint::FILE_NAME	= _T("virtual_compartment_point.csv");
const int			SurveyCSV::VirtualCompartPoint::ROW_COUNT	= 10;
const uh::tstring	SurveyCSV::RelVirtualCompartLink::FILE_NAME	= _T("rel_lane_link_virtual_compartment_link.csv");
const int			SurveyCSV::RelVirtualCompartLink::ROW_COUNT	= 6;
const uh::tstring	SurveyCSV::CoveringArea::FILE_NAME			= _T("covering_area.csv");
const int			SurveyCSV::CoveringArea::ROW_COUNT			= 5;
const uh::tstring	SurveyCSV::SpatialRef::FILE_NAME			= _T("spatial_reference.csv");
const int			SurveyCSV::SpatialRef::ROW_COUNT			= 2;
const uh::tstring	SurveyCSV::Version::FILE_NAME				= _T("version.csv");
const int			SurveyCSV::Version::ROW_COUNT				= 2;

// コード値ドメイン
const std::vector<int>			SurveyCSV::DOMAIN_LANE_GROUP_C				=	boost::assign::list_of(1)(2)(3)(4)(5)(6)										;	///< 車線グループ種別コード
const std::vector<int>			SurveyCSV::DOMAIN_MAX_LEGAL_SPEED_C			=	boost::assign::list_of(20)(30)(40)(50)(60)(70)(80)(90)(100)(120)(1001)(1002)(2020)(2030)(2040)(2050)(2060)(2070)(2080)(2090)(2100)(2120)	;	///< 制限速度コード（最大）
const std::vector<int>			SurveyCSV::DOMAIN_MIN_LEGAL_SPEED_C			=	boost::assign::list_of(20)(30)(40)(50)(60)(70)(80)(90)(100)(120)(1001)(1002)(2020)(2030)(2040)(2050)(2060)(2070)(2080)(2090)(2100)(2120)	;	///< 制限速度コード（最小）
const std::vector<int>			SurveyCSV::DOMAIN_TRAVEL_DIRECTION_C		=	boost::assign::list_of(1)(2)(3)(4)(5)(6)(7)(8)(9)								;	///< 進行方向種別コード
const std::vector<int>			SurveyCSV::DOMAIN_ROAD_STRUCTURE_C			=	boost::assign::list_of(1)(2)(3)(4)(5)(6)(7)(8)(9)(10)(11)(12)					;	///< 道路構造種別コード

const std::vector<int>			SurveyCSV::DOMAIN_POINT_CLASS_C				=	boost::assign::list_of(1)(2)													;	///< ポイント種別コード
const std::vector<int>			SurveyCSV::DOMAIN_LINEAR_ELEMENT_C			=	boost::assign::list_of(1)(2)(3)													;	///< 線形要素属性コード

const std::vector<int>			SurveyCSV::DOMAIN_COMPART_LINE_C			=	boost::assign::list_of(0)(1)(2)(3)(13)(31)(12)(21)(22)(23)(32)(33)(232)(313)(323)(3223);	///< 線種別コード
const std::vector<int>			SurveyCSV::DOMAIN_COMPART_STYLE_C			=	boost::assign::list_of(1)(2)(3)(4)													;		///< 区画線設置様式コード
const std::vector<int>			SurveyCSV::DOMAIN_COMPART_SLOW_C			=	boost::assign::list_of(0)(1)(2)(3)													;		///< 減速表示コード

const std::vector<int>			SurveyCSV::DOMAIN_LINK_DIRECTION_C			=	boost::assign::list_of(1)(2)													;	///< 区画線リンク方向コード

const std::vector<int>			SurveyCSV::DOMAIN_SIGNAL_MOLD_C				=	boost::assign::list_of(1)(2)													;	///< 信号機形状コード

const std::vector<uh::tstring>	SurveyCSV::DOMAIN_ROADSIGN_C				=	boost::assign::list_of(_T("116の4"))(_T("116の6"))(_T("119-D"))(_T("201-A"))(_T("201-B"))(_T("201-C"))(_T("201-D"))(_T("201の2"))(_T("202"))(_T("203"))(_T("204"))(_T("205"))(_T("206"))(_T("207-A"))(_T("207-B"))(_T("208"))(_T("208の2"))(_T("209"))(_T("209の2"))(_T("209の3"))(_T("210"))(_T("211"))(_T("212"))(_T("212の2"))(_T("212の3"))(_T("212の4"))(_T("213"))(_T("214"))(_T("214の2"))(_T("215"))(_T("301"))(_T("302"))(_T("303"))(_T("304"))(_T("305"))(_T("305の2"))(_T("306"))(_T("307"))(_T("308"))(_T("309"))(_T("310"))(_T("310の2"))(_T("311-A"))(_T("311-B"))(_T("311-C"))(_T("311-D"))(_T("311-E"))(_T("311-F"))(_T("312"))(_T("313"))(_T("314"))(_T("314の2"))(_T("315"))(_T("316"))(_T("317"))(_T("318"))(_T("319"))(_T("320"))(_T("321"))(_T("322"))(_T("323"))(_T("323の2"))(_T("324"))(_T("325"))(_T("327の8"))(_T("327の9"))(_T("327の10"))(_T("327の11"))(_T("327の12"))(_T("327の13"))(_T("328"))(_T("328の2"))(_T("329"))(_T("329の2"))(_T("330"))(_T("331"))(_T("332"));	///< 道路標識コード

const std::vector<long long>	SurveyCSV::DOMAIN_SRID						=	boost::assign::list_of(6668LL)(4612LL)(4301LL)										;	///< 空間参照ID

const std::vector<int>			SurveyCSV::DOMAIN_UPDATE_C					=	boost::assign::list_of(0)(1)(2)(3)												;	///< 更新コード

// フラグ
const std::vector<int>			SurveyCSV::DOMAIN_FLAG					=	boost::assign::list_of(0)(1)	;	///< フラグ値

// 固定値
const double					SurveyCSV::CONST_CURV_STRAIGHT			=	0.0					;	///< 曲率:直線
const double					SurveyCSV::CONST_CURV_RAD_STRAIGHT		=	90000000000.0		;	///< 曲率半径:直線
const double					SurveyCSV::CONST_CURV_CH_RA_STRT_PNT	=	0.0					;	///< 曲率変化率:起点
const double					SurveyCSV::CONST_CLOT_PARAM_STRAIGHT	=	0.0					;	///< クロソイドパラメータ:直線

SurveyCSV::SurveyCSV(void)
{
}


SurveyCSV::~SurveyCSV(void)
{
}
