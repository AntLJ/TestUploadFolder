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

#include <deque>
#include "BasicGraphData.h"

typedef std::deque<Graph::vertex_descriptor> VertexQueue; ///< 頂点列

/**
 * @brief 双方向頂点列規制クラス
 */
class BidirectionalVQRegulation
{
public:
	void add(const Graph::vertex_descriptor v);

	VertexQueue::size_type length() const;
	const VertexQueue& fore() const;
	const VertexQueue& reverse() const;

	bool operator==(const BidirectionalVQRegulation& rhs) const;
	bool operator<(const BidirectionalVQRegulation& rhs) const;

private:
	VertexQueue m_ForeVQ; ///< 順方向頂点列。
	VertexQueue m_ReverseVQ; ///< 逆方向頂点列。
};
