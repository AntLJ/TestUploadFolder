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

// LinkArrivedInfo.h: LinkArrivedInfo クラスのインターフェイス
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

// 設定
	Route* setRoute(const Route& rRoute);

// 取得
	const Link* link() const { return m_pLinkBranch ? m_pLinkBranch->m_pLink : 0; }

// 取得処理
	Route getStartRouteBy(int nNodeId) const;
	const Route* bestRouteOfAll() const;

// プロパティ
	const LinkBranch* m_pLinkBranch; ///< 対応するリンク
	ArrivedInfo m_cFromSideArrivedInfo; ///< 正方向到達情報
	ArrivedInfo m_cToSideArrivedInfo; ///< 逆方向到達情報
};

}
