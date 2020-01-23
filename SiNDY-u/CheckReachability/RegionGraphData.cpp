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
#include <boost/scoped_ptr.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <rns/functional.h>
#include "RegionGraphData.h"

using namespace arctl::field;
using namespace sindy::schema::road_link;

namespace {

template <typename T>
bool is_no_passage(const T& rT)
{
	switch(get<kNoPassage>(rT)) {
	case no_passage::kPassable:
	case no_passage::kNoPassageWithReg:
	case no_passage::kRestrictedWithReg:
		return false;
	default:
		return true;
	}
}

void assign_linkregs(const Graph& rGraph, BidirVQRegs& rBidirVQRegs)
{
	using namespace boost;
	using namespace arctl::field;

	std::auto_ptr<BidirVQRegs> pBidirVQRegs(new BidirVQRegs);

	property_map<Graph, edge_bundle_t>::const_type pEdgeBundleMap(get(edge_bundle, rGraph));

	Graph::edge_iterator e, e_end;
	for(boost::tie(e, e_end) = edges(rGraph); e != e_end; ++e) {
		const RoadLink* pRoadLink = get(pEdgeBundleMap, *e).m_pRoadLink;

		const long nOneway = get<kOneway>(*pRoadLink);
		bool bNoPassage = is_no_passage(*pRoadLink);

		// �u�����Ȃ��ʍs�֎~�v��u����������ʍs�v�̏ꍇ�B
		if(bNoPassage || nOneway == oneway::kFore) {
			BidirectionalVQRegulation aBidirVQReg;
			aBidirVQReg.add(target(*e, rGraph));
			aBidirVQReg.add(source(*e, rGraph));
			rBidirVQRegs.push_back(aBidirVQReg);
		}

		// �u�����Ȃ��ʍs�֎~�v��u�t��������ʍs�v�̏ꍇ�B
		if(bNoPassage || nOneway == oneway::kReverse) {
			BidirectionalVQRegulation aBidirVQReg;
			aBidirVQReg.add(source(*e, rGraph));
			aBidirVQReg.add(target(*e, rGraph));
			rBidirVQRegs.push_back(aBidirVQReg);
		}
	}
}

void assign_turnregs(
	const Graph& rGraph,
	const LQMap& rLQMap,
	const LinkEdgeMap& rLinkEdgeMap,
	BidirVQRegs& rBidirVQRegs
)
{
	using namespace boost;
	using namespace arctl::field;

	const rns::mapped_value_referer<const LinkEdgeMap> aLinkEdgeReferer(rLinkEdgeMap);

	// �����N��K�����璸�_��K�����쐬����B
	for(LQMap::const_iterator it1(rLQMap.begin()), it1_end(rLQMap.end()); it1 != it1_end; ++it1) {
		const LinkQueue& rLinkQueue = it1->second;
		boost::scoped_ptr<BidirectionalVQRegulation> pVQReg(new BidirectionalVQRegulation);

		for(LinkQueue::const_iterator it2(rLinkQueue.begin()), it2_end(rLinkQueue.end()); it2 != it2_end; ++it2) {
			using namespace sindy::schema::category::directional_link_relational_table;

			const sindy::lq_t& rLq = *it2;
			const Graph::edge_descriptor* pe = aLinkEdgeReferer(get<kLinkID>(rLq));
			if(pe) {
				const Graph::vertex_descriptor u(source(*pe, rGraph)), v(target(*pe, rGraph));

				switch(get<kLinkDir>(rLq)) {
				case link_dir::kFore:
					if(it2 == rLinkQueue.begin()) {
						pVQReg->add(u);
					}
					pVQReg->add(v);
					break;
				case link_dir::kReverse:
					if(it2 == rLinkQueue.begin()) {
						pVQReg->add(v);
					}
					pVQReg->add(u);
					break;
				default:
					throw std::logic_error("unexpected LINK_DIR.");
				}
			}
			else {
				pVQReg.reset();
				break;
			}
		}

		if(pVQReg && pVQReg->length() != 0) {
			rBidirVQRegs.push_back(*pVQReg);
		}
	}
}

template <typename Pred>
void assign_noderegs(const Graph& rGraph, BidirVQRegs& rBidirVQRegs, Pred pred)
{
	using namespace boost;

	Graph::vertex_iterator v, v_end;
	for(tie(v, v_end) = vertices(rGraph); v != v_end; ++v) {
		if(pred(get(vertex_index1, rGraph, *v))) {
			BidirectionalVQRegulation aBidirVQReg;
			aBidirVQReg.add(*v);
			rBidirVQRegs.push_back(aBidirVQReg);
		}
	}
}

void build_vqrefs_map(const VertexQueue& rVQ, VertexBidirVQRegRefsMap& rVertexBidirVQRegRefsMap, const BidirectionalVQRegulation* pVQReg)
{
	for(VertexQueue::const_iterator it(rVQ.begin()), it_end(rVQ.end()); it != it_end; ++it) {
		rVertexBidirVQRegRefsMap[*it].insert(pVQReg);
	}
}

std::auto_ptr<VertexBidirVQRegRefsMap> create_vqrefs_map(const BidirVQRegs& rBidirVQRegs)
{
	std::auto_ptr<VertexBidirVQRegRefsMap> pVertexVQRegRefsMap(new VertexBidirVQRegRefsMap);

	for(BidirVQRegs::const_iterator it(rBidirVQRegs.begin()), it_end(rBidirVQRegs.end()); it != it_end; ++it) {
		const BidirectionalVQRegulation& rVQReg = *it;
		build_vqrefs_map(rVQReg.fore(), *pVertexVQRegRefsMap, &rVQReg);
	}

	return pVertexVQRegRefsMap;
}

} // anonymous namespace

/**
 * @brief ���[�W���������H�O���t�����쐬����B
 *
 * @param pBasicGraphData [in,ref] ��{���H�O���t���B
 * @param rRegionData [in] ���[�W���������H�O���t�����쐬���郊�[�W�����̃I�u�W�F�N�g���B
 * @param rSharingData [in] �S���[�W�������p���B
 * @return ���[�W���������H�O���t���B
 */
std::auto_ptr<RegionGraphData>
create_region_graph_data(
	BasicGraphDataRef pBasicGraphData,
	const RegionData& rRegionData,
	const SharingData& rSharingData
)
{
	using namespace boost::lambda;

	// ���_��K�����쐬����B
	std::auto_ptr<BidirVQRegs> pBidirVQRegs(new BidirVQRegs);

	assign_linkregs(*pBasicGraphData->m_pGraph, *pBidirVQRegs);
	assign_turnregs(*pBasicGraphData->m_pGraph, *rSharingData.m_pLQMap, *pBasicGraphData->m_pLinkEdgeMap, *pBidirVQRegs);
	assign_noderegs(
		*pBasicGraphData->m_pGraph,
		*pBidirVQRegs,
		const_parameters(
			boost::lambda::bind(&RoadNodeIDSet::count, rRegionData.m_pInnerRoadNodeIDSet.get(), _1) == 0UL &&
			boost::lambda::bind(&RoadNodeIDSet::count, rRegionData.m_pOuterRoadNodeIDSet.get(), _1) == 0UL
		)
	);

	// ���_��K�������j�[�N�E�\�[�g����B
	std::sort(pBidirVQRegs->begin(), pBidirVQRegs->end());
	pBidirVQRegs->erase(std::unique(pBidirVQRegs->begin(), pBidirVQRegs->end()), pBidirVQRegs->end());

	// ���_��K�����}�b�s���O����B
	std::auto_ptr<VertexBidirVQRegRefsMap> pVertexBidirVQRegRefsMap(create_vqrefs_map(*pBidirVQRegs));

	// ���[�W���������H�O���t����Ԃ��B
	std::auto_ptr<RegionGraphData> pRegionGraphData(new RegionGraphData);
	pRegionGraphData->m_pBasicGraphData = pBasicGraphData;
	pRegionGraphData->m_pBidirVQRegs = pBidirVQRegs;
	pRegionGraphData->m_pVertexBidirVQRegRefsMap = pVertexBidirVQRegRefsMap;

	return pRegionGraphData;
}
