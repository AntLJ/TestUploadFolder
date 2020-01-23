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

// LinkBranchTo.cpp: LinkBranchTo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkBranchTo.h"
#include "LinkBranch.h"

namespace sindy
{
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LinkBranchTo::LinkBranchTo(long nNodeId, const LinkBranch* pLinkBranch, double dCost) :
m_nNodeId(nNodeId),
m_pLinkBranch(pLinkBranch),
m_dCost(dCost)
{
	ATLASSERT(pLinkBranch);
}

LinkBranchTo::~LinkBranchTo()
{

}

//////////////////////////////////////////////////////////////////////
// オペレータ
//////////////////////////////////////////////////////////////////////

bool LinkBranchTo::operator < (const LinkBranchTo& rLinkBranchTo) const
{
	return
		(m_nNodeId < rLinkBranchTo.m_nNodeId) ||
		(m_nNodeId == rLinkBranchTo.m_nNodeId && m_pLinkBranch < rLinkBranchTo.m_pLinkBranch);
}

}
