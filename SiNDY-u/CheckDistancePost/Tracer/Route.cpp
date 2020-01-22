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

// Route.cpp: Route �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "Route.h"
#include "math2.h"

using namespace sindy::schema;

namespace sindy
{
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Route::Route()
{
	clear();
}

/**
 * @brief ���[�g�̎n�܂���쐬����
 *
 * @param pLinkBranch [in] �n�܂�̃����N
 * @param nInNodeId	[in] �n�܂�̃����N�̎n�_���ifromNode�ł͂Ȃ��j�m�[�h��ID
 */
Route::Route(const LinkBranch* pLinkBranch, long nInNodeId)
{
	ATLASSERT(pLinkBranch);
	pLinkBranch->m_pLink->m_nFromNodeId == nInNodeId || pLinkBranch->m_pLink->m_nToNodeId == nInNodeId;

	clear();

	// ���[�g�̊J�n�ݒ�
	m_pPrevRoute = nullptr;
	m_pLinkBranch = pLinkBranch;
	m_nNodeId = nInNodeId;
	m_dCost= 1.0; //< �R�X�g�����l��1.0
}

/**
 * @brief �w�肵�����B�n�_����A�w�肵���}��ʂ����ꍇ��Route���쐬����
 *
 * @note �쐬���ꂽRoute�������N��K���Ɉ������������ꍇ�A�����͍ő�l�ƂȂ�
 *
 * @param pRoute [in] ���O�̃��[�g
 * @param rBranch [in] ���O�̃��[�g����̕���
 */
Route::Route(const Route *pRoute, const LinkBranchTo &rBranch)
{
	ATLASSERT(pRoute);
	
	clear();

	// ���O�̃��[�g�ݒ�
	m_pPrevRoute = pRoute;
	m_pLinkBranch = rBranch.m_pLinkBranch;
	m_nNodeId = rBranch.m_nNodeId;
	m_dCost = m_pPrevRoute->m_dCost + rBranch.m_dCost;
}

Route::~Route()
{

}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

void Route::clear()
{
	m_dCost = COST_MAX;
	m_nNodeId = 0;
	m_pLinkBranch = nullptr;
	m_pLeftLink = nullptr;
	m_pPrevRoute = nullptr;
	m_bDepartured = false;
}

//////////////////////////////////////////////////////////////////////
// �擾����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵���m�[�h�Ɣ��Α��̕���ꗗ���擾����
 *
 * @param nNodeId [in] �Ώۃm�[�hID
 * @return ����ꗗ
 * @exception std::runtime_error isValid()�̖߂�l��false������
 */
const LinkBranch::LinkBranchToSet& Route::oppositeBranch() const
{
	isValid();

	return m_pLinkBranch->oppositeBranch(m_nNodeId);	
}

/**
 * @brief ���̃��[�g���擾����
 *
 * @note isValid()��false��Ԃ�Route���u���̃��[�g�v�Ƃ��ēo�^����邱�Ƃ͂Ȃ�
 *
 * @param rRouteMap [out] ���[�g���L�^����}���`�}�b�v
 * @exception std::runtime_erro isValid()�̖߂�l��false������
 */
void Route::getNextRoutesTo(CostKeyRouteMap& rRouteMap) const
{
	isValid();

	const LinkBranch::LinkBranchToSet& rLinkBranchSet = oppositeBranch();

	for(LinkBranch::LinkBranchToSet::const_iterator itr = rLinkBranchSet.begin(); itr != rLinkBranchSet.end(); ++itr) {
		const LinkBranchTo& rLinkBranchTo = *itr;
		Route cRoute(this, rLinkBranchTo);
		if(cRoute.isValid()) {
			rRouteMap.insert(CostKeyRouteMap::value_type(cRoute.m_dCost, cRoute));
		}
	}
}

}
