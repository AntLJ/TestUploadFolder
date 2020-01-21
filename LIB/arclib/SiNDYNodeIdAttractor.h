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

#ifndef __SINDY_NODE_ID_ATTRACTOR_H__
#define __SINDY_NODE_ID_ATTRACTOR_H__

#include "VertexAttractor.h"

namespace sindy {

template <typename TVertexAttractor>
class NodeIdAttractorT : public TVertexAttractor
{
public:
	NodeIdAttractorT(
		TVertexAttractor::GroupCollection& rGroupCollection,
		TVertexAttractor::GroupIndex& rGroupIndex,
		long nFromNodeIdField,
		long nToNodeIdField) :
	TVertexAttractor(rGroupCollection, rGroupIndex),
	m_nFromNodeIdField(nFromNodeIdField),
	m_nToNodeIdField(nToNodeIdField)
	{
	}

	bool attract(IFeature* ipRoadLink)
	{
		if(! ipRoadLink)
			return false;

		_variant_t va;
		ipRoadLink->get_Value(m_nFromNodeIdField, &va);
		long nFromNodeId = va;

		ipRoadLink->get_Value(m_nToNodeIdField, &va);
		long nToNodeId = va;

		TVertexAttractor::attract(nFromNodeId, nToNodeId);

		return true;
	}

	inline bool operator () (IFeature* ipRoadLink)
	{
		return attract(ipRoadLink);
	}

	long m_nFromNodeIdField;
	long m_nToNodeIdField;
};

typedef NodeIdAttractorT< VertexAttractorT<long> > NodeIdAttractor;

/**
 * @warning コンストラクタ引数が異なるため、VertexGroupCollectionT::make_Attractor()は使用できない。
 */
template<typename TNodeIdAttractor>
class NodeIdGroupCollectionT : public VertexGroupCollectionT<TNodeIdAttractor>
{
public:
	inline VertexAttractor make_Attractor(long nFromNodeIdField, long nToNodeIdField)
	{
		return VertexAttractor(m_GroupCollection, m_GroupIndex, nFromNodeIdField, nToNodeIdField);
	}
};
} // namesapce sindy

#endif // __SINDY_NODE_ID_ATTRACTOR_H__
