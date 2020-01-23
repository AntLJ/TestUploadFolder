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

// LinkArrivedInfo.cpp: LinkArrivedInfo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkArrivedInfo.h"

namespace sindy
{

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

LinkArrivedInfo::LinkArrivedInfo() :
m_pLinkBranch(0)
{

}

LinkArrivedInfo::LinkArrivedInfo(const LinkBranch* pLinkBranch) :
m_pLinkBranch(pLinkBranch)
{
	ATLASSERT(pLinkBranch);
}

LinkArrivedInfo::~LinkArrivedInfo()
{

}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���[�g��ݒ肷��
 *
 * @param rRoute [in] �ݒ肷�郋�[�g
 * @return �ݒ肵�����[�g�B�w�肵�����[�g�����m�̃��[�g�������I�ł͂Ȃ����ߐݒ肵�Ȃ������ꍇNULL�B
 * @excetpion std::runtime_error �w�肵�����[�g�����̃����N�ɐݒ肷�邱�Ƃ͏o���Ȃ�
 */
Route* LinkArrivedInfo::setRoute(const Route& rRoute)
{
	ATLASSERT(m_pLinkBranch);

	if(rRoute.m_nNodeId == link()->m_nFromNodeId)
		return m_cFromSideArrivedInfo.setRoute(rRoute);
	else if(rRoute.m_nNodeId == link()->m_nToNodeId)
		return m_cToSideArrivedInfo.setRoute(rRoute);
}

//////////////////////////////////////////////////////////////////////
// �擾
//////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵���m�[�h���n�_�Ƃ����A�ǐՊJ�n���[�g���쐬����
 *
 * @param nNodeId [in] �n�_�m�[�h
 * @return �ǐՊJ�n���[�g
 */
Route LinkArrivedInfo::getStartRouteBy(int nNodeId) const
{
	ATLASSERT(m_pLinkBranch);
	return Route(m_pLinkBranch, nNodeId);
}

/**
 * @brief ���̃����N�ɓ��B���邷�ׂẴ��[�g���ŁA�œK�Ȃ��̂�I������
 *
 * @return �œK�ȃ��[�g�B���̃����N�ɓ��B���郋�[�g�����݂��Ȃ��ꍇNULL�B
 */
const Route* LinkArrivedInfo::bestRouteOfAll() const
{
	const Route* pRoute1 = m_cFromSideArrivedInfo.bestRouteOfAll();
	const Route* pRoute2 = m_cToSideArrivedInfo.bestRouteOfAll();

	ATLASSERT((! pRoute1 || pRoute1->isValid()) || (! pRoute2 || pRoute2->isValid()));

	if(! (pRoute1 || pRoute2))
		return 0;
	else if(pRoute1 && ! pRoute2)
		return pRoute1;
	else if(! pRoute1 && pRoute2)
		return pRoute2;

	return pRoute1->m_dCost <= pRoute2->m_dCost ? pRoute1 : pRoute2;
}

}
