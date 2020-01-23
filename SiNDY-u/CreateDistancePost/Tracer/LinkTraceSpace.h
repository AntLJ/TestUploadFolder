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

// LinkTraceSpace.h: LinkTraceSpace �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "LinkBranch.h"
#include "LinksCache.h"

namespace sindy
{
class LinkTraceSpace  
{
public:
	typedef std::map<long, LinkBranch> LinkBranchMap;

// �\�z/����
	LinkTraceSpace();
	virtual ~LinkTraceSpace();

	void clear();

// �ݒ�/�擾
	void setLinks(const LinksCache& rLinks);

	bool empty() const { return m_cLinkBranchMap.empty(); }

// �v���p�e�B
	LinkBranchMap m_cLinkBranchMap;
};

}
