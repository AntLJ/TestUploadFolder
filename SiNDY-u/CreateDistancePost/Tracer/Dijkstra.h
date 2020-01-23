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

// Dijkstra.h: Dijkstra クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "LinkTraceSpace.h"
#include "LinkArrivedInfo.h"

namespace sindy
{
class Dijkstra  
{
public:
	typedef std::map<long, LinkArrivedInfo> LinkArrivedInfoMap;
	typedef std::multimap<double, Route*> CostKeyRoutePtrMap;

// 構築/消滅
	Dijkstra();
	virtual ~Dijkstra();

// 消去
	virtual void clear();

// 設定
	virtual bool setLinks(const LinkTraceSpace& rSpace, long nStartLinkId);
	virtual bool setEndLinkId(long nEndLinkId);

// 取得
	virtual long startLinkId() const { return m_itrStartLinkArrivedInfo != m_cLinkArrivedInfoMap.end() ? m_itrStartLinkArrivedInfo->first : 0; }

// 取得処理
	virtual const Route* getRouteToEndLink();
	virtual const Route* getNextTraceRoute();

// 処理
	virtual bool trace();

// プロパティ
	LinkArrivedInfoMap m_cLinkArrivedInfoMap; ///< リンク、及びその到達情報一覧

private:
// 処理
	void setRoutes(Route::CostKeyRouteMap& rCostKeyRouteMap);
	bool setRoute(Route& rRoute);

// プロパティ
	LinkArrivedInfoMap::const_iterator m_itrStartLinkArrivedInfo; ///< 追跡始端リンク
	LinkArrivedInfoMap::const_iterator m_itrEndLinkArrivedInfo; ///< 追跡終端リンク
	CostKeyRoutePtrMap m_cCostKeyRoutePtrMap; ///< コスト順ルートポインタ一覧
};

}

