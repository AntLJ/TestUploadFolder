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

// LinkBranch.cpp: LinkBranch �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "LinkCostList.h"
#include "LinkBranch.h"

using namespace sindy::schema;

namespace sindy
{

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

LinkBranch::LinkBranch() :
m_pLink(nullptr),
m_dCost(0)
{

}

LinkBranch::LinkBranch(const Link* pLink) :
m_pLink(pLink),
m_dCost(0)
{
	setCost();
}

LinkBranch::~LinkBranch()
{

}

//////////////////////////////////////////////////////////////////////
// �������⍲
//////////////////////////////////////////////////////////////////////

void LinkBranch::setCost()
{
	using namespace road_link;

	ATLASSERT(m_pLink);

	double dRoadCost = 1.0, dLinkCost = 1.0, dCostLen = 1.0;
	IPolylinePtr(m_pLink->m_ipGeometry)->get_Length(&dCostLen);

	{
		using namespace road_class;

		LinkCostList::RoadClassCostMap::const_iterator itrRoad = LinkCostList::m_cRoadClassCostMap.find(m_pLink->m_nRoadClass);
		if(itrRoad != LinkCostList::m_cRoadClassCostMap.end()) {
			dRoadCost = itrRoad->second;
		}
	}

/*
	{
		using namespace navi_class;

		LinkCostList::NaviClassCostMap::const_iterator itrNavi = LinkCostList::m_cNaviClassCostMap.find(m_pLink->m_nNaviClass);
		if(itrNavi != LinkCostList::m_cNaviClassCostMap.end()) {
			dCost *= itrNavi->second;
		}
	}

	{
		using namespace road_width;

		LinkCostList::RoadWidthCostMap::const_iterator itrLink = LinkCostList::m_cRoadWidthCostMap.find(m_pLink->m_nRoadWidth);
		if(itrLink != LinkCostList::m_cRoadWidthCostMap.end()) {
			dCost *= itrLink->second;
		}
	}
*/

	{
		using namespace link_class;

		LinkCostList::LinkClassCostMap::const_iterator itrLink = LinkCostList::m_cLinkClassCostMap.find(m_pLink->m_nLinkClass);
		if(itrLink != LinkCostList::m_cLinkClassCostMap.end()) {
			dLinkCost = itrLink->second;
		}
	}

	m_dCost = (dCostLen / dRoadCost) * dLinkCost;
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief �����N������ǉ�����
 *
 * @param nNodeId [in] �ʉ߂���m�[�hID
 * @param rLinkBranchPtrSet [in] ����惊���N�ꗗ
 * @retval true �ǉ�����
 * @retval false �ǉ����s
 */
bool LinkBranch::insertBranch(long nNodeId, const LinkBranchPtrSet& rLinkBranchPtrSet)
{
	ATLASSERT(m_pLink);

	for(LinkBranchPtrSet::const_iterator itr = rLinkBranchPtrSet.begin(); itr != rLinkBranchPtrSet.end(); ++itr) {
		const LinkBranch* pLinkBranch = *itr;

		// ���������N�̓p�X
		if(pLinkBranch->m_pLink->m_nObjectId == m_pLink->m_nObjectId)
			continue;

		insertBranch(nNodeId, pLinkBranch);
	}

	return true;
}

/**
 * @brief �����N������ǉ�����
 *
 * @param nNodeId [in] �ʉ߂���m�[�hID
 * @param pLinkBranch [in] ����惊���N
 * @retval true �ǉ�����
 * @retval false �ǉ����s�A���邢�͒ǉ��̕K�v���Ȃ�
 */
bool LinkBranch::insertBranch(long nNodeId, const LinkBranch* pLinkBranch)
{
	ATLASSERT(m_pLink && pLinkBranch);

	if(pLinkBranch->m_dCost == COST_MAX)
		return false;

	double dCost = (m_dCost + pLinkBranch->m_dCost) / 2.0;

	if(nNodeId == m_pLink->m_nFromNodeId) {
		m_cFromSideLinkBranchSet.insert(LinkBranchTo(nNodeId, pLinkBranch, dCost));
	}
	else if(nNodeId == m_pLink->m_nToNodeId) {
		m_cToSideLinkBranchSet.insert(LinkBranchTo(nNodeId, pLinkBranch, dCost));
	}
	else
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// �擾����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵���m�[�h�Ɣ��Α��̕���ꗗ���擾����
 *
 * @param nNodeId [in] �Ώۃm�[�hID
 * @return ����ꗗ
 * @exception std::runtime_error nNodeId�����Y�����N�ɐڑ�����m�[�hID�ƈ�v���Ȃ�����
 */
const LinkBranch::LinkBranchToSet& LinkBranch::oppositeBranch(int nNodeId) const
{
	ATLASSERT(m_pLink);

	if(nNodeId == m_pLink->m_nFromNodeId)
		return m_cToSideLinkBranchSet;
	else if(nNodeId == m_pLink->m_nToNodeId)
		return m_cFromSideLinkBranchSet;
}

}
