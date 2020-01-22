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

// Route.cpp: Route クラスのインプリメンテーション
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
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Route::Route()
{
	clear();
}

/**
 * @brief ルートの始まりを作成する
 *
 * @param pLinkBranch [in] 始まりのリンク
 * @param nInNodeId	[in] 始まりのリンクの始点側（fromNodeではない）ノードのID
 */
Route::Route(const LinkBranch* pLinkBranch, long nInNodeId)
{
	ATLASSERT(pLinkBranch);
	pLinkBranch->m_pLink->m_nFromNodeId == nInNodeId || pLinkBranch->m_pLink->m_nToNodeId == nInNodeId;

	clear();

	// ルートの開始設定
	m_pPrevRoute = nullptr;
	m_pLinkBranch = pLinkBranch;
	m_nNodeId = nInNodeId;
	m_dCost= 1.0; //< コスト初期値は1.0
}

/**
 * @brief 指定した到達地点から、指定した枝を通った場合のRouteを作成する
 *
 * @note 作成されたRouteがリンク列規制に引っかかった場合、距離は最大値となる
 *
 * @param pRoute [in] 直前のルート
 * @param rBranch [in] 直前のルートからの分岐
 */
Route::Route(const Route *pRoute, const LinkBranchTo &rBranch)
{
	ATLASSERT(pRoute);
	
	clear();

	// 直前のルート設定
	m_pPrevRoute = pRoute;
	m_pLinkBranch = rBranch.m_pLinkBranch;
	m_nNodeId = rBranch.m_nNodeId;
	m_dCost = m_pPrevRoute->m_dCost + rBranch.m_dCost;
}

Route::~Route()
{

}

//////////////////////////////////////////////////////////////////////
// 設定
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
// 取得処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定したノードと反対側の分岐一覧を取得する
 *
 * @param nNodeId [in] 対象ノードID
 * @return 分岐一覧
 * @exception std::runtime_error isValid()の戻り値がfalseだった
 */
const LinkBranch::LinkBranchToSet& Route::oppositeBranch() const
{
	isValid();

	return m_pLinkBranch->oppositeBranch(m_nNodeId);	
}

/**
 * @brief 次のルートを取得する
 *
 * @note isValid()がfalseを返すRouteが「次のルート」として登録されることはない
 *
 * @param rRouteMap [out] ルートを記録するマルチマップ
 * @exception std::runtime_erro isValid()の戻り値がfalseだった
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
