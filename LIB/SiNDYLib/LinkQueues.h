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

// LinkQueues.h: CLinkQueues クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LINKQUEUES_H_)
#define _LINKQUEUES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkQueue.h"

namespace sindy {

class SINDYLIB_API CLinkQueues
{
public:
	CLinkQueues();
	virtual ~CLinkQueues();

	/**
	 * @brief 道路に属しているリンク列を取得する
	 * @param	lValue	[in]		リンク列を検索したい道路の OBJECTID
	 * @param	eCode	[in]		検索したいリンク列の種別
	 * @retval	sindyErr_NoErr		成功：メンバ m_mapData に取得したリンク列の情報を格納
	 * @retval	それ以外のエラー	失敗：規約に違反した種別
	*/
	errorcode::sindyErrCode SelectByRoadID(long lValue, sindyTableType::ECode emLinkQueue );

private:
	std::map<long, CLinkQueue*>		m_mapLQs;
};

}

#endif // !defined(_LINKQUEUES_H_)
