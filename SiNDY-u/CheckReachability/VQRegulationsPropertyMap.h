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

#include <boost/range/iterator_range.hpp>
#if BOOST_VERSION == 103400
#include <boost/property_map.hpp>
#elif BOOST_VERSION <= 105100
#include <boost/property_map/property_map.hpp>
#endif
#include "RegionGraphData.h"

typedef boost::iterator_range<VertexQueue::const_iterator> VertexQueueIteratorRange;

class VQRegulationsPropertyMap
{
public:
	typedef boost::graph_traits<Graph>::vertex_descriptor key_type;
	typedef std::vector<VertexQueueIteratorRange> value_type;
	typedef value_type reference;
	typedef boost::readable_property_map_tag category;

	typedef const VertexQueue& (BidirectionalVQRegulation::*Selection)() const;

	VQRegulationsPropertyMap(const VertexBidirVQRegRefsMap& rVertexBidirVQRegRefsMap, Selection fpSelection);

	value_type operator[](key_type v);

private:
	const VertexBidirVQRegRefsMap& m_rVertexBidirVQRegRefsMap;
	const Selection m_fpSelection;
};
