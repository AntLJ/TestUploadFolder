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

// LinkBranchTo.h: LinkBranchTo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "Link.h"

#define COST_MAX 0

namespace sindy
{
class LinkBranch;

class LinkBranchTo  
{
public:
// �\�z/����
	LinkBranchTo(long nNodeId, const LinkBranch* pLink, double dCost);
	virtual ~LinkBranchTo();

// �I�y���[�^
	bool operator < (const LinkBranchTo& rLinkBranchTo) const;

// �v���p�e�B
	long m_nNodeId; ///< ����m�[�h
	const LinkBranch* m_pLinkBranch; ///< ����惊���N
	double m_dCost; ///< ����R�X�g
};

}
