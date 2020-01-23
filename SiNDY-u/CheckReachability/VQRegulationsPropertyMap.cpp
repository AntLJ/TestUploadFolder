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
#include "VQRegulationsPropertyMap.h"

VQRegulationsPropertyMap::VQRegulationsPropertyMap(const VertexBidirVQRegRefsMap& rVertexBidirVQRegRefsMap, Selection fpSelection) :
m_rVertexBidirVQRegRefsMap(rVertexBidirVQRegRefsMap),
m_fpSelection(fpSelection)
{
}

VQRegulationsPropertyMap::value_type VQRegulationsPropertyMap::operator[](key_type v)
{
	value_type aVQItRanges;

	const VertexBidirVQRegRefsMap::const_iterator it1(m_rVertexBidirVQRegRefsMap.find(v));
	if(it1 != m_rVertexBidirVQRegRefsMap.end()) {
		const BidirVQRegRefs& rBidirVQRegRefs = it1->second;
		for(BidirVQRegRefs::const_iterator it2(rBidirVQRegRefs.begin()), it2_end(rBidirVQRegRefs.end()); it2 != it2_end; ++it2) {
			const VertexQueue& rVertexQueue = ((*it2)->*m_fpSelection)();
			aVQItRanges.push_back(VertexQueueIteratorRange(rVertexQueue.begin(), rVertexQueue.end()));
		}
	}

	return aVQItRanges;
}
