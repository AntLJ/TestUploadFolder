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

typedef std::vector<BidirectionalVQRegulation> BidirVQRegs; ///< 頂点列規制集合
typedef std::set<const BidirectionalVQRegulation*> BidirVQRegRefs; ///< 頂点列規制ポインタ集合
typedef std::map<Graph::vertex_descriptor, BidirVQRegRefs> VertexBidirVQRegRefsMap; ///< 頂点に関連する頂点列規制一覧

/**
 * @brief リージョン内道路グラフ情報
 */
struct RegionGraphData
{
	BasicGraphDataRef m_pBasicGraphData; ///< 基本道路グラフ情報。
	std::auto_ptr<BidirVQRegs> m_pBidirVQRegs; ///< 頂点列規制一覧。
	std::auto_ptr<VertexBidirVQRegRefsMap> m_pVertexBidirVQRegRefsMap; ///< 頂点に関連する頂点列規制一覧。
};

std::auto_ptr<RegionGraphData>
create_region_graph_data(BasicGraphDataRef pBasicGraphData, const RegionData& rRegionData, const SharingData& rSharingData);
