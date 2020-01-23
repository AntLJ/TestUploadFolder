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

#pragma once

#include <vector>
#include <set>

struct EtcLink
{
	typedef	std::vector<long>	NodeList;

	enum EType
	{
		type_ramp	= 1,	// ETC専用ランプリンク
		type_access	= 2		// ETC接続一般道リンク
	};

	EType		eType;				// 分類コード
	long		eMeshCode;			// メッシュコード
	long		eFromNodeID;		// 始点ノードID
	NodeList	eNodeList;			// 中点ノードID列
	long		eToNodeID;			// 終点ノードID

	EtcLink(void) : eMeshCode(0), eFromNodeID(0), eToNodeID(0) {}

	bool operator< (const EtcLink& rhs) const
	{
		return
			eMeshCode   != rhs.eMeshCode   ? eMeshCode   < rhs.eMeshCode	:
			eFromNodeID != rhs.eFromNodeID ? eFromNodeID < rhs.eFromNodeID	:
			eToNodeID   <  rhs.eToNodeID;
	}
};


typedef std::multiset<EtcLink> EtcLinkList;

