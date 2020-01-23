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
#include "BidirectionalVQRegulation.h"

/**
 * @brief 順方向に頂点を追加する。
 *
 * @param v [in] 頂点記述子。
 */
void BidirectionalVQRegulation::add(const Graph::vertex_descriptor v)
{
	m_ForeVQ.push_back(v);
	m_ReverseVQ.push_front(v);
}

/**
 * @brief 頂点列の長さを求める。
 *
 * @return 頂点列の長さ。
 */
VertexQueue::size_type BidirectionalVQRegulation::length() const
{
	_ASSERT(m_ForeVQ.size() == m_ReverseVQ.size());
	return m_ForeVQ.size();
}

/**
 * @brief 順方向の頂点列を求める。
 */
const VertexQueue& BidirectionalVQRegulation::fore() const
{
	return m_ForeVQ;
}

/**
 * @brief 逆方向の頂点列を求める。
 */
const VertexQueue& BidirectionalVQRegulation::reverse() const
{
	return m_ReverseVQ;
}

bool BidirectionalVQRegulation::operator==(const BidirectionalVQRegulation& rhs) const
{
	const bool bEquals = m_ForeVQ == rhs.m_ForeVQ;
	_ASSERT(bEquals == (m_ReverseVQ == rhs.m_ReverseVQ));
	return bEquals;
}

bool BidirectionalVQRegulation::operator<(const BidirectionalVQRegulation& rhs) const
{
	return m_ForeVQ < rhs.m_ForeVQ;
}
