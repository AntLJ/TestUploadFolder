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

// LinkArrivedInfo.cpp: LinkArrivedInfo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkArrivedInfo.h"

namespace sindy
{

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief ルートを設定する
 *
 * @param rRoute [in] 設定するルート
 * @return 設定したルート。指定したルートが既知のルートより効率的ではないため設定しなかった場合NULL。
 * @excetpion std::runtime_error 指定したルートをこのリンクに設定することは出来ない
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
// 取得
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定したノードを始点とした、追跡開始ルートを作成する
 *
 * @param nNodeId [in] 始点ノード
 * @return 追跡開始ルート
 */
Route LinkArrivedInfo::getStartRouteBy(int nNodeId) const
{
	ATLASSERT(m_pLinkBranch);
	return Route(m_pLinkBranch, nNodeId);
}

/**
 * @brief このリンクに到達するすべてのルート中で、最適なものを選択する
 *
 * @return 最適なルート。このリンクに到達するルートが存在しない場合NULL。
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
