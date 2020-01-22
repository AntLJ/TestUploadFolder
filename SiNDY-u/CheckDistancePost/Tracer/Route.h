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

// Route.h: Route クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "LinkBranch.h"

namespace sindy
{
/**
 * @brief ルートクラス
 */
class Route  
{
public:
	typedef std::multimap<double, Route> CostKeyRouteMap;

// 構築/消滅
	Route();
	Route(const LinkBranch* pLinkBranch, long nInNodeId);
	Route(const Route* pRoute, const LinkBranchTo &rBranch);
	virtual ~Route();

// クリア
	void clear();

// 設定
	void setDepartured() { m_bDepartured = true; }

// 取得
	const Link* link() const { return m_pLinkBranch->m_pLink; }

// 取得処理
	const LinkBranch::LinkBranchToSet& oppositeBranch() const;
	void getNextRoutesTo(CostKeyRouteMap& rRouteMap) const;

// 判定処理
	bool isEmpty() const { return ! (m_pLinkBranch && m_nNodeId); }
	bool isTerminal() const { return ! isEmpty() && m_pPrevRoute && m_dCost == COST_MAX; }
	bool isValid() const { return ! (isEmpty() || isTerminal()); }
	bool isNoRegulations() const { return ! m_pLeftLink; }

private:
// 設定補佐

public:
// プロパティ
	const LinkBranch* m_pLinkBranch; ///< 現リンク
	const LinkBranch* m_pLeftLink; ///< Ｕターンの基点になりそうなリンク
	long m_nNodeId; ///< 直前のルートから現リンクへ繋がるノード
	const Route* m_pPrevRoute; ///< 直前のルート
	double m_dCost; ///< 現リンクまでのコスト
	bool m_bDepartured; ///< このルートから出発したことがあるか？
};

}
