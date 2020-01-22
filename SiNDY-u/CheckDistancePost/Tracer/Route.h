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

// Route.h: Route �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "LinkBranch.h"

namespace sindy
{
/**
 * @brief ���[�g�N���X
 */
class Route  
{
public:
	typedef std::multimap<double, Route> CostKeyRouteMap;

// �\�z/����
	Route();
	Route(const LinkBranch* pLinkBranch, long nInNodeId);
	Route(const Route* pRoute, const LinkBranchTo &rBranch);
	virtual ~Route();

// �N���A
	void clear();

// �ݒ�
	void setDepartured() { m_bDepartured = true; }

// �擾
	const Link* link() const { return m_pLinkBranch->m_pLink; }

// �擾����
	const LinkBranch::LinkBranchToSet& oppositeBranch() const;
	void getNextRoutesTo(CostKeyRouteMap& rRouteMap) const;

// ���菈��
	bool isEmpty() const { return ! (m_pLinkBranch && m_nNodeId); }
	bool isTerminal() const { return ! isEmpty() && m_pPrevRoute && m_dCost == COST_MAX; }
	bool isValid() const { return ! (isEmpty() || isTerminal()); }
	bool isNoRegulations() const { return ! m_pLeftLink; }

private:
// �ݒ�⍲

public:
// �v���p�e�B
	const LinkBranch* m_pLinkBranch; ///< �������N
	const LinkBranch* m_pLeftLink; ///< �t�^�[���̊�_�ɂȂ肻���ȃ����N
	long m_nNodeId; ///< ���O�̃��[�g���猻�����N�֌q����m�[�h
	const Route* m_pPrevRoute; ///< ���O�̃��[�g
	double m_dCost; ///< �������N�܂ł̃R�X�g
	bool m_bDepartured; ///< ���̃��[�g����o���������Ƃ����邩�H
};

}
