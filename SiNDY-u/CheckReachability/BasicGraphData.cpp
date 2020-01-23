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
 * @brief ��{���H�O���t���쐬����
 *
 * @param rRegionData [in] �O���t���쐬���郊�[�W�����̃I�u�W�F�N�g���B
 */
std::auto_ptr<BasicGraphData> create_basic_graph_data(const RegionData& rRegionData)
{
	using namespace boost;
	using namespace sindy::schema::road_link;

	using arctl::field::get;

	const RoadLinks& rRoadLinks = *rRegionData.m_pRoadLinks;

	std::auto_ptr<NodeVertexMap> pNodeVertexMap(new NodeVertexMap);

	// ���H�m�[�h���_�L�q�q�ꗗ����������B
	for(RoadLinks::const_iterator it(rRoadLinks.begin()), it_end(rRoadLinks.end()); it != it_end; ++it) {
		(*pNodeVertexMap)[get<kFromNodeID>(*it)];
		(*pNodeVertexMap)[get<kToNodeID>(*it)];
	}

	// ���H�m�[�hID�ɒ��_�L�q�q���֘A�t����B
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

	// �O���t�ƁA���H�����N�ӋL�q�q�ꗗ���쐬����B
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
