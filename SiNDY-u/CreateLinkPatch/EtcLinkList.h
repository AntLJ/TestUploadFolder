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

// EtcLinkList.h: EtcLinkList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETCLINKLIST_H__7BFD1718_1AC5_4FBA_9544_339E15D40A61__INCLUDED_)
#define AFX_ETCLINKLIST_H__7BFD1718_1AC5_4FBA_9544_339E15D40A61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class EtcLinkList  
{
public:
	enum EType
	{
		type_ramp	= 1,	// ETC専用ランプリンク
		type_access	= 2		// ETC接続一般道リンク
	};

	struct EtcLink
	{
		typedef	std::vector<long>	NodeList;

		EType		eType;				// 分類コード
		long		eMeshCode;			// メッシュコード
		long		eFromNodeID;		// 始点ノードID
		NodeList	eNodeList;			// 中点ノードID列
		long		eToNodeID;			// 終点ノードID

		EtcLink(void) : eMeshCode(0), eFromNodeID(0), eToNodeID(0) {}
	};

	typedef std::vector<EtcLink> Data;

	EtcLinkList();
	virtual ~EtcLinkList();

	void	append(EtcLink cEtcLink) { mData.push_back(cEtcLink); }
	void	write(std::ostream& crStream);
	void	write(const char* cFilePath, bool cUseEUC);

private:
	Data	mData;

};

#endif
