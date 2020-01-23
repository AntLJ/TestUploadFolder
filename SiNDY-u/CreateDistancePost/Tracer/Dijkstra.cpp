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

// Dijkstra.cpp: Dijkstra �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dijkstra.h"

namespace sindy
{
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Dijkstra::Dijkstra()
{

}

Dijkstra::~Dijkstra()
{

}

//////////////////////////////////////////////////////////////////////
// �N���A
//////////////////////////////////////////////////////////////////////

void Dijkstra::clear()
{
	m_cLinkArrivedInfoMap.clear();
	m_itrEndLinkArrivedInfo = m_cLinkArrivedInfoMap.end();
	m_cCostKeyRoutePtrMap.clear();
}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief �����N�ǐՋ�Ԃ�o�^����
 *
 * @param rSpace [in] �����N�ǐՋ��
 * @param nStartLinkId [in] �ǐՊJ�n�����NID
 * @retval true �o�^����
 * @retval false �o�^���s
 */
bool Dijkstra::setLinks(const LinkTraceSpace& rSpace, long nStartLinkId)
{
	clear();

	const LinkTraceSpace::LinkBranchMap& rMap = rSpace.m_cLinkBranchMap;
	for(LinkTraceSpace::LinkBranchMap::const_iterator itr = rMap.begin(); itr != rMap.end(); ++itr) {
		m_cLinkArrivedInfoMap[itr->first] = LinkArrivedInfo(&itr->second);
	}

	LinkArrivedInfoMap::iterator itr = m_cLinkArrivedInfoMap.find(nStartLinkId);
	if(itr == m_cLinkArrivedInfoMap.end())
		return false;

	LinkArrivedInfo& rLinkArrivedInfo = itr->second;
	m_itrStartLinkArrivedInfo = itr;

	setRoute(rLinkArrivedInfo.getStartRouteBy(rLinkArrivedInfo.link()->m_nFromNodeId));
	setRoute(rLinkArrivedInfo.getStartRouteBy(rLinkArrivedInfo.link()->m_nToNodeId));

	return true;
}

/**
 * @brief �ǐՏI�[�����N��o�^����
 *
 * @param nLinkId [in] �I�[�����NID�B�o�^�������������ꍇ��0�B
 * @retval true �o�^����
 * @retval false �o�^���s
 */
bool Dijkstra::setEndLinkId(long nLinkId)
{
	if(nLinkId == 0) {
		m_itrEndLinkArrivedInfo = m_cLinkArrivedInfoMap.end();
		return true;
	}

	m_itrEndLinkArrivedInfo = m_cLinkArrivedInfoMap.find(nLinkId);
	if(m_itrEndLinkArrivedInfo == m_cLinkArrivedInfoMap.end())
		return false;

	return true;
}

/**
 * @brief �ǐՂ��P�X�e�b�v�i�s����
 *
 * @retval true �ǐՑ��s��
 * @retval false �ǐՏI��
 */
bool Dijkstra::trace()
{
	// ��`����Ă���I�[�����N�܂ŒH�蒅������I��
	if(getRouteToEndLink())
		return false;

	// �ŒZ���[�g�擾
	CostKeyRoutePtrMap::iterator itrCurrentRoute = m_cCostKeyRoutePtrMap.begin();
	if(itrCurrentRoute == m_cCostKeyRoutePtrMap.end())
		return false;
	Route* pRoute = itrCurrentRoute->second;
	m_cCostKeyRoutePtrMap.erase(itrCurrentRoute);

	// ���̃��[�g�擾
	Route::CostKeyRouteMap cCostKeyRouteMap;
	pRoute->getNextRoutesTo(cCostKeyRouteMap);

	// ���̃��[�g��o�^
	setRoutes(cCostKeyRouteMap);

	return true;
}

//////////////////////////////////////////////////////////////////////
// �擾����
//////////////////////////////////////////////////////////////////////

/**
 * @brief ��`���ꂽ�I�[�����N�܂ł̃��[�g��Ԃ�
 *
 * @return ��`���ꂽ�I�[�����N�܂ł̃��[�g�B�܂����ʂ��o�Ă��Ȃ��ꍇ��NULL�B�I�[���[�g����`����Ă��Ȃ��ꍇ��NULL�B
 */
const Route* Dijkstra::getRouteToEndLink()
{
	if(m_itrEndLinkArrivedInfo == m_cLinkArrivedInfoMap.end())
		return nullptr;

	const LinkArrivedInfo& rLinkArrivedInfo = m_itrEndLinkArrivedInfo->second;

	return rLinkArrivedInfo.bestRouteOfAll();
}

const Route* Dijkstra::getNextTraceRoute()
{
	const Route* pRoute = nullptr;

	CostKeyRoutePtrMap::iterator itr = m_cCostKeyRoutePtrMap.begin();
	if(itr == m_cCostKeyRoutePtrMap.end())
		return nullptr;

	return itr->second;
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���[�g��o�^����
 *
 * @param rCostKeyRouteMap [in] �o�^���郋�[�g�ꗗ
 */
void Dijkstra::setRoutes(Route::CostKeyRouteMap& rCostKeyRouteMap)
{
	for(Route::CostKeyRouteMap::iterator itr = rCostKeyRouteMap.begin(); itr != rCostKeyRouteMap.end(); ++itr) {
		Route& rRoute = itr->second;
		setRoute(rRoute);
	}
}

/**
 * @brief ���[�g��o�^����
 *
 * @param rRoute [in] �o�^���郋�[�g
 * @retval true �o�^����
 * @retval false �o�^���s�A���邢�͓o�^���鉿�l�̂Ȃ����[�g
 * @exception std::runtime_error rRoute�������A���邢��rRoute�����������N�������N��ԓ��ɑ��݂��Ȃ�
 */
bool Dijkstra::setRoute(Route& rRoute)
{
	rRoute.isValid();

	LinkArrivedInfoMap::iterator itr = m_cLinkArrivedInfoMap.find(rRoute.link()->m_nObjectId);
	itr != m_cLinkArrivedInfoMap.end();

	LinkArrivedInfo& rLinkArrivedInfo = itr->second;
	Route* pRoute = rLinkArrivedInfo.setRoute(rRoute);
	if(pRoute) {
		m_cCostKeyRoutePtrMap.insert(CostKeyRoutePtrMap::value_type(rRoute.m_dCost, pRoute));

/*
#ifdef _DEBUG
		// ���[�v���[�g�`�F�b�N
		std::set<const Route*> cRoutePtrSet;
		for(const Route* pTestRoute = pRoute; pTestRoute; pTestRoute = pTestRoute->m_pPrevRoute) {
			if(cRoutePtrSet.find(pTestRoute) != cRoutePtrSet.end()) {
				ATLASSERT(0);
				break;
			}
			else {
				cRoutePtrSet.insert(pTestRoute);
			}
		}
#endif
*/
		return true;
	}	
	else
		return false;
}

}
