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

#include "adam2_format_checker.h"

struct ImportFileList {
	const wchar_t*  file_name;
	const wchar_t*  alias_name;
	std::shared_ptr<CDataChecker> checker;
	bool            target;
};

enum filelist_feature_index {
	index_lane_link,
	index_lane_point,
	index_compartment_link,
	index_compartment_point,
	index_border_link,
	index_border_point,
	index_signal_point,
	index_roadsign_point,
	//index_crosswalk_line,
	index_covering_area,

	index_feature_count,
};
enum filelist_row_index {
	index_rel_lane_link_compartment_link,
	index_rel_lane_link_border_link,
	index_rel_lane_point_signal_point,
	index_rel_lane_point_roadsign_point,
	//index_rel_lane_point_crosswalk_line,
	index_version,

	index_row_count,
};

static const ImportFileList kFeatureFileList[] = {
	{L"lane_link.csv"                                   , L"�Ԑ������N"                         , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CLaneLinkChecker     ()) , true },
	{L"lane_point.csv"                                  , L"�Ԑ��|�C���g"                       , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CLaneNodeChecker     ()) , true },
	{L"compartment_link.csv"                            , L"���������N"                       , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CCompartLinkChecker  ()) , true },
	{L"compartment_point.csv"                           , L"�����|�C���g"                     , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CCompartNodeChecker  ()) , true },
	{L"border_link.csv"                                 , L"���E�������N"                       , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CBorderLinkChecker   ()) , true },
	{L"border_point.csv"                                , L"���E���|�C���g"                     , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CBorderNodeChecker   ()) , true },
	{L"signal_point.csv"                                , L"�M���@�|�C���g"                     , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CSignalPointChecker  ()) , true },
	{L"roadsign_point.csv"                              , L"���H�W���|�C���g"                   , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CRoadSignPointChecker()) , true },
	//{L"crosswalk_line.csv"                              , L"���f�������C��"                     , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CCrossWalkLineChecker()) , true },
	{L"covering_area.csv"                               , L"�������G���A"                       , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CCoveringAreaChecker ()) , true },
};

static const ImportFileList kRelFileList[] = {
	{L"rel_lane_link_compartment_link.csv"              , L"�Ԑ������N-���������N�֘A"        , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CRelCLChecker())  , true },
	{L"rel_border_link.csv"                             , L"�Ԑ������N-���E�������N�֘A"        , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CRelBLChecker())  , true },
	{L"rel_lane_point_signal_point.csv"                 , L"�Ԑ��m�[�h-�M���@�|�C���g�֘A"      , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CRelSPChecker())  , true },
	{L"rel_lane_point_roadsign_point.csv"               , L"�Ԑ��m�[�h-���H�W���|�C���g�֘A"    , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CRelRSChecker())  , true },
	//{L"rel_lane_point_crosswalk_line.csv"               , L"�Ԑ��m�[�h-���f�������C���֘A"      , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CRelCWChecker())  , true },

	// ���[�ɓ���Ă���
	{L"version.csv"                                     , L"�o�[�W�������"                     , std::shared_ptr<CDataChecker>(new sindy::adam2::format::CVersionChecker())}
};

// ��ԎQ�Ƃ͖ڎ��ŏ\���m�F�ł��邽�ߑΉ����Ȃ�!
