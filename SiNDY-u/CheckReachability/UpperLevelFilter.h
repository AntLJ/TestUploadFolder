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

#include "BasicGraphData.h"

/**
 * @brief ��w�O���t�ɂ����݂���ӂ��Ԉ����N���X
 *
 * ��w�O���t�ɂ����݂��铹�H�����N�̌o�H�T���͏�w�O���t�ɔC����B
 */
class UpperLevelFilter
{
public:
	UpperLevelFilter() :
	m_pGraph(0),
	m_pUpperLinkEdgeMap(0)
	{
	}

	UpperLevelFilter(const Graph& rCurrentGraph, const LinkEdgeMap* pUpperLinkEdgeMap) :
	m_pGraph(&rCurrentGraph),
	m_pUpperLinkEdgeMap(pUpperLinkEdgeMap)
	{
	}

	bool operator()(Graph::edge_descriptor e) const
	{
		if(! m_pUpperLinkEdgeMap)
			return true;

		using namespace boost;

		return m_pUpperLinkEdgeMap->count(get(edge_bundle, *m_pGraph, e).m_pRoadLink->oid()) == 0;
	}

private:
	const Graph* m_pGraph; ///< ���̃O���t�B
	const LinkEdgeMap* m_pUpperLinkEdgeMap; ///< ��w�O���t�̕ӈꗗ�B
};
