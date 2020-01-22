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

// LinkBranch.h: LinkBranch �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <set>
#include "Link.h"
#include "LinkBranchTo.h"

namespace sindy
{
class LinkBranch  
{
public:
// ��`
	typedef std::set<const LinkBranch*> LinkBranchPtrSet;
	typedef std::set<LinkBranchTo> LinkBranchToSet;
	typedef std::set<LinkBranchTo*> LinkBranchToPtrSet;

// �\�z/����
	LinkBranch();
	LinkBranch(const Link* pLink);
	virtual ~LinkBranch();

// �ݒ�
	bool insertBranch(long nNodeId, const LinkBranchPtrSet& rLinkBranchPtrSet);

// �擾����
	const LinkBranchToSet& oppositeBranch(int nNodeId) const;

// �v���p�e�B
	const Link* m_pLink;
	LinkBranchToSet m_cFromSideLinkBranchSet;
	LinkBranchToSet m_cToSideLinkBranchSet;

	double m_dCost;

private:
// �������⍲
	void setCost();

// �ݒ�
	bool insertBranch(long nNodeId, const LinkBranch* pLinkBranch);
};

}
