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

// LinkBranchTo.h: LinkBranchTo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "Link.h"

#define COST_MAX 0

namespace sindy
{
class LinkBranch;

class LinkBranchTo  
{
public:
// 構築/消滅
	LinkBranchTo(long nNodeId, const LinkBranch* pLink, double dCost);
	virtual ~LinkBranchTo();

// オペレータ
	bool operator < (const LinkBranchTo& rLinkBranchTo) const;

// プロパティ
	long m_nNodeId; ///< 分岐ノード
	const LinkBranch* m_pLinkBranch; ///< 分岐先リンク
	double m_dCost; ///< 分岐コスト
};

}
