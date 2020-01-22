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

// Dijkstra.cpp: Dijkstra クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dijkstra.h"

namespace sindy
{
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Dijkstra::Dijkstra()
{

}

Dijkstra::~Dijkstra()
{

}

//////////////////////////////////////////////////////////////////////
// クリア
//////////////////////////////////////////////////////////////////////

void Dijkstra::clear()
{
	m_cLinkArrivedInfoMap.clear();
	m_itrEndLinkArrivedInfo = m_cLinkArrivedInfoMap.end();
	m_cCostKeyRoutePtrMap.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief リンク追跡空間を登録する
 *
 * @param rSpace [in] リンク追跡空間
 * @param nStartLinkId [in] 追跡開始リンクID
 * @retval true 登録成功
 * @retval false 登録失敗
 */
bool Dijkstra::setLinks(const LinkTraceSpace& rSpace, long nStartLinkId)
{
	clear();

	const LinkTraceSpace::LinkBranchMap& rMap = rSpace.m_cLinkBranchMap;
	for(LinkTraceSpace::LinkBranchMap::const_iterator itr = rMap.begin(); itr != rMap.end(); ++itr) {
		m_cLinkArrivedInfoMap[itr->first] = LinkArrivedInfo(&itr->second);
	}

	LinkArrivedInfoMap::iterator itr = m_cLinkArrivedInfoMap.find(nStartLinkId);
	if(itr == m_cLinkArrivedInfoMap.end())
		return false;

	LinkArrivedInfo& rLinkArrivedInfo = itr->second;
	m_itrStartLinkArrivedInfo = itr;

	setRoute(rLinkArrivedInfo.getStartRouteBy(rLinkArrivedInfo.link()->m_nFromNodeId));
	setRoute(rLinkArrivedInfo.getStartRouteBy(rLinkArrivedInfo.link()->m_nToNodeId));

	return true;
}

/**
 * @brief 追跡終端リンクを登録する
 *
 * @param nLinkId [in] 終端リンクID。登録を解除したい場合は0。
 * @retval true 登録成功
 * @retval false 登録失敗
 */
bool Dijkstra::setEndLinkId(long nLinkId)
{
	if(nLinkId == 0) {
		m_itrEndLinkArrivedInfo = m_cLinkArrivedInfoMap.end();
		return true;
	}

	m_itrEndLinkArrivedInfo = m_cLinkArrivedInfoMap.find(nLinkId);
	if(m_itrEndLinkArrivedInfo == m_cLinkArrivedInfoMap.end())
		return false;

	return true;
}

/**
 * @brief 追跡を１ステップ進行する
 *
 * @retval true 追跡続行中
 * @retval false 追跡終了
 */
bool Dijkstra::trace()
{
	// 定義されている終端リンクまで辿り着いたら終了
	if(getRouteToEndLink())
		return false;

	// 最短ルート取得
	CostKeyRoutePtrMap::iterator itrCurrentRoute = m_cCostKeyRoutePtrMap.begin();
	if(itrCurrentRoute == m_cCostKeyRoutePtrMap.end())
		return false;
	Route* pRoute = itrCurrentRoute->second;
	m_cCostKeyRoutePtrMap.erase(itrCurrentRoute);

	// 次のルート取得
	Route::CostKeyRouteMap cCostKeyRouteMap;
	pRoute->getNextRoutesTo(cCostKeyRouteMap);

	// 次のルートを登録
	setRoutes(cCostKeyRouteMap);

	return true;
}

//////////////////////////////////////////////////////////////////////
// 取得処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief 定義された終端リンクまでのルートを返す
 *
 * @return 定義された終端リンクまでのルート。まだ結果が出ていない場合はNULL。終端ルートが定義されていない場合はNULL。
 */
const Route* Dijkstra::getRouteToEndLink()
{
	if(m_itrEndLinkArrivedInfo == m_cLinkArrivedInfoMap.end())
		return nullptr;

	const LinkArrivedInfo& rLinkArrivedInfo = m_itrEndLinkArrivedInfo->second;

	return rLinkArrivedInfo.bestRouteOfAll();
}

const Route* Dijkstra::getNextTraceRoute()
{
	const Route* pRoute = nullptr;

	CostKeyRoutePtrMap::iterator itr = m_cCostKeyRoutePtrMap.begin();
	if(itr == m_cCostKeyRoutePtrMap.end())
		return nullptr;

	return itr->second;
}

//////////////////////////////////////////////////////////////////////
// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief ルートを登録する
 *
 * @param rCostKeyRouteMap [in] 登録するルート一覧
 */
void Dijkstra::setRoutes(Route::CostKeyRouteMap& rCostKeyRouteMap)
{
	for(Route::CostKeyRouteMap::iterator itr = rCostKeyRouteMap.begin(); itr != rCostKeyRouteMap.end(); ++itr) {
		Route& rRoute = itr->second;
		setRoute(rRoute);
	}
}

/**
 * @brief ルートを登録する
 *
 * @param rRoute [in] 登録するルート
 * @retval true 登録成功
 * @retval false 登録失敗、あるいは登録する価値のないルート
 * @exception std::runtime_error rRouteが無効、あるいはrRouteが示すリンクがリンク空間内に存在しない
 */
bool Dijkstra::setRoute(Route& rRoute)
{
	rRoute.isValid();

	LinkArrivedInfoMap::iterator itr = m_cLinkArrivedInfoMap.find(rRoute.link()->m_nObjectId);
	itr != m_cLinkArrivedInfoMap.end();

	LinkArrivedInfo& rLinkArrivedInfo = itr->second;
	Route* pRoute = rLinkArrivedInfo.setRoute(rRoute);
	if(pRoute) {
		m_cCostKeyRoutePtrMap.insert(CostKeyRoutePtrMap::value_type(rRoute.m_dCost, pRoute));

/*
#ifdef _DEBUG
		// ループルートチェック
		std::set<const Route*> cRoutePtrSet;
		for(const Route* pTestRoute = pRoute; pTestRoute; pTestRoute = pTestRoute->m_pPrevRoute) {
			if(cRoutePtrSet.find(pTestRoute) != cRoutePtrSet.end()) {
				ATLASSERT(0);
				break;
			}
			else {
				cRoutePtrSet.insert(pTestRoute);
			}
		}
#endif
*/
		return true;
	}	
	else
		return false;
}

}
