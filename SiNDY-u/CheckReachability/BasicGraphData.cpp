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
#include <boost/iterator/counting_iterator.hpp>
#include <rns/iterator/second_iterator.h>
#include <rns/functional.h>
#include "BasicGraphData.h"

/**
 * @brief 基本道路グラフを作成する
 *
 * @param rRegionData [in] グラフを作成するリージョンのオブジェクト情報。
 */
std::auto_ptr<BasicGraphData> create_basic_graph_data(const RegionData& rRegionData)
{
	using namespace boost;
	using namespace sindy::schema::road_link;

	using arctl::field::get;

	const RoadLinks& rRoadLinks = *rRegionData.m_pRoadLinks;

	std::auto_ptr<NodeVertexMap> pNodeVertexMap(new NodeVertexMap);

	// 道路ノード頂点記述子一覧を準備する。
	for(RoadLinks::const_iterator it(rRoadLinks.begin()), it_end(rRoadLinks.end()); it != it_end; ++it) {
		(*pNodeVertexMap)[get<kFromNodeID>(*it)];
		(*pNodeVertexMap)[get<kToNodeID>(*it)];
	}

	// 道路ノードIDに頂点記述子を関連付ける。
	std::copy(
		counting_iterator<size_t>(0),
		counting_iterator<size_t>(pNodeVertexMap->size()),
		rns::make_second_iterator(pNodeVertexMap->begin())
	);

	std::auto_ptr<Graph> pGraph(new Graph(pNodeVertexMap->size()));

	for(NodeVertexMap::const_iterator it(pNodeVertexMap->begin()), it_end(pNodeVertexMap->end()); it != it_end; ++it) {
		put(vertex_index1, *pGraph, it->second, it->first);
	}

	std::auto_ptr<LinkEdgeMap> pLinkEdgeMap(new LinkEdgeMap);

	// グラフと、道路リンク辺記述子一覧を作成する。
	for(RoadLinks::const_iterator it(rRoadLinks.begin()), it_end(rRoadLinks.end()); it != it_end; ++it) {
		using namespace sindy::schema::road_link;

		pLinkEdgeMap->insert(
			LinkEdgeMap::value_type(
				it->oid(),
				add_edge(
					(*pNodeVertexMap)[get<kFromNodeID>(*it)],
					(*pNodeVertexMap)[get<kToNodeID>(*it)],
					EdgeBundle(&*it),
					*pGraph
				).first
			)
		);
	}

	std::auto_ptr<BasicGraphData> pBasicGraphData(new BasicGraphData);
	pBasicGraphData->m_pGraph = pGraph;
	pBasicGraphData->m_pNodeVertexMap = pNodeVertexMap;
	pBasicGraphData->m_pLinkEdgeMap = pLinkEdgeMap ;

	return pBasicGraphData;
}
