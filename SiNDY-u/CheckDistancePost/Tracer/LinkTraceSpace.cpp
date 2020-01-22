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

// LinkTraceSpace.cpp: LinkTraceSpace クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "LinkTraceSpace.h"

using namespace sindy::schema;

namespace sindy
{
typedef std::map<long, LinkBranch::LinkBranchPtrSet> LinkBranchPtrSetMap;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LinkTraceSpace::LinkTraceSpace()
{

}

LinkTraceSpace::~LinkTraceSpace()
{

}

//////////////////////////////////////////////////////////////////////
// クリア
//////////////////////////////////////////////////////////////////////

void LinkTraceSpace::clear()
{
	m_cLinkBranchMap.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @param rLinks [in] 登録するリンク
 * @param ipEnvelope [in] リンク追跡空間矩形
 */
void LinkTraceSpace::setLinks(const LinksCache& rLinks)
{
	using namespace road_link;

	// リンク空間をクリア
	clear();

	const LinkMap& rLinkMap = rLinks.m_cLinkMap;

	// ノードごとの流出可能リンクリストを作成する
	LinkBranchPtrSetMap cOutableLinkBranchPtrSetMap;
	for ( const auto& itr : rLinkMap)
	{
		const Link& rLink = itr.second;

		// リンク空間にリンクを追加
		LinkBranch& rBranch = m_cLinkBranchMap[itr.first] = LinkBranch(&rLink);

		// 正方向一方通行リンクでなければ、Toノード側から当該リンクに流入可能
		cOutableLinkBranchPtrSetMap[rLink.m_nToNodeId].insert(&rBranch);

		// 逆方向一方通行リンクでなければ、Fromノード側から当該リンクに流入可能
		cOutableLinkBranchPtrSetMap[rLink.m_nFromNodeId].insert(&rBranch);
	}

	// リンクごとの分岐リストを作成する
	for(LinkBranchMap::iterator itr = m_cLinkBranchMap.begin(); itr != m_cLinkBranchMap.end(); ++itr) {
		LinkBranch& rBranch = itr->second;
		
		// Fromノード側から流出する分岐を登録する
		LinkBranchPtrSetMap::const_iterator itr2 = cOutableLinkBranchPtrSetMap.find(rBranch.m_pLink->m_nFromNodeId);
		if(itr2 != cOutableLinkBranchPtrSetMap.end()) {
			rBranch.insertBranch(rBranch.m_pLink->m_nFromNodeId, itr2->second);
		}

		// Toノード側から流出する分岐を登録する
		itr2 = cOutableLinkBranchPtrSetMap.find(rBranch.m_pLink->m_nToNodeId);
		if(itr2 != cOutableLinkBranchPtrSetMap.end()) {
			rBranch.insertBranch(rBranch.m_pLink->m_nToNodeId, itr2->second);
		}
	}
}

}
