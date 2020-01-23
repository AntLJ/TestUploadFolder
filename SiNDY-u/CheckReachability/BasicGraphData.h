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

#include <set>
#include <map>
#include <memory>
#include <boost/graph/adjacency_list.hpp>
#include "RegionData.h"

struct EdgeBundle
{
	EdgeBundle() :
	m_pRoadLink(0)
	{
	}

	EdgeBundle(const RoadLink* pRoadLink) :
	m_pRoadLink(pRoadLink)
	{
	}

	const RoadLink* m_pRoadLink; ///< 関連付けられる道路リンク。
};

typedef
	boost::adjacency_list<
		boost::vecS, boost::vecS, boost::undirectedS,
		boost::property<boost::vertex_index1_t, long>,
		EdgeBundle
	> Graph;

typedef std::map<long, Graph::vertex_descriptor> NodeVertexMap; ///< 道路ノード頂点記述子一覧
typedef std::map<long, Graph::edge_descriptor> LinkEdgeMap; ///< 道路リンク辺記述子一覧

/**
 * @brief 基本道路グラフ情報
 */
struct BasicGraphData
{
	std::auto_ptr<Graph> m_pGraph; ///< 道路グラフ。
	std::auto_ptr<NodeVertexMap> m_pNodeVertexMap; ///<  道路ノード頂点記述子一覧。
	std::auto_ptr<LinkEdgeMap> m_pLinkEdgeMap; ///< 道路リンク辺記述子一覧。
};

std::auto_ptr<BasicGraphData> create_basic_graph_data(const RegionData& rRegionData);
