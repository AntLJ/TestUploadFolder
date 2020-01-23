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

#include <vector>
#include <set>
#include <map>
#include <memory>
#include "SharingData.h"
#include "RegionData.h"
#include "BasicGraphData.h"
#include "BidirectionalVQRegulation.h"

typedef boost::shared_ptr<const BasicGraphData> BasicGraphDataRef;

typedef std::vector<BidirectionalVQRegulation> BidirVQRegs; ///< ���_��K���W��
typedef std::set<const BidirectionalVQRegulation*> BidirVQRegRefs; ///< ���_��K���|�C���^�W��
typedef std::map<Graph::vertex_descriptor, BidirVQRegRefs> VertexBidirVQRegRefsMap; ///< ���_�Ɋ֘A���钸�_��K���ꗗ

/**
 * @brief ���[�W���������H�O���t���
 */
struct RegionGraphData
{
	BasicGraphDataRef m_pBasicGraphData; ///< ��{���H�O���t���B
	std::auto_ptr<BidirVQRegs> m_pBidirVQRegs; ///< ���_��K���ꗗ�B
	std::auto_ptr<VertexBidirVQRegRefsMap> m_pVertexBidirVQRegRefsMap; ///< ���_�Ɋ֘A���钸�_��K���ꗗ�B
};

std::auto_ptr<RegionGraphData>
create_region_graph_data(BasicGraphDataRef pBasicGraphData, const RegionData& rRegionData, const SharingData& rSharingData);
