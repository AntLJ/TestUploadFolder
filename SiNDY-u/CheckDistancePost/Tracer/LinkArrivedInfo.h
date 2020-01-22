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

// LinkArrivedInfo.h: LinkArrivedInfo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Link.h"
#include "ArrivedInfo.h"

namespace sindy
{
class LinkArrivedInfo  
{
public:
	LinkArrivedInfo();
	LinkArrivedInfo(const LinkBranch* pLinkBranch);
	virtual ~LinkArrivedInfo();

// �ݒ�
	Route* setRoute(const Route& rRoute);

// �擾
	const Link* link() const { return m_pLinkBranch ? m_pLinkBranch->m_pLink : 0; }

// �擾����
	Route getStartRouteBy(int nNodeId) const;
	const Route* bestRouteOfAll() const;

// �v���p�e�B
	const LinkBranch* m_pLinkBranch; ///< �Ή����郊���N
	ArrivedInfo m_cFromSideArrivedInfo; ///< ���������B���
	ArrivedInfo m_cToSideArrivedInfo; ///< �t�������B���
};

}
