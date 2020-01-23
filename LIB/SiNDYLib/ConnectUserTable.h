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

/**
 * @file ConnectUserTable.h
 * @brief <b>CConnectUserTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _CONNECTUSERTABLE_H_
#define _CONNECTUSERTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "ConnectUser.h"

namespace sindy {
namespace connectuser {
/**
 * @class CConectUserTable
 * @brief <b>接続ユーザ名管理テーブルクラス</b>\n
 * このクラスはREFERENCE.CONNECTUSERテーブルのラップクラスです。
 */
class CConnectUserTable : public CTable  
{
public:
	CConnectUserTable() : m_bCached(false) {}
	virtual ~CConnectUserTable(){}

	CConnectUserTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CConnectUserTable& operator=(const CConnectUserTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>テーブルを全てキャッシュする</b>\n
	 * テーブルの内容を全てキャッシュします。
	 * bDetatchをtrueにすると、読み込んだレコード及び
	 * テーブルのDBへの接続を切り離します。
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>接続ユーザ名に対応するIDを返す</b>\n
	 * @param lpcszConnUser	[in]	接続ユーザ名
	 * @retval 0以上 接続ユーザ名ID
	 * @retval -1 登録されていない
	 */
	long GetConnectUserID( LPCTSTR lpcszConnUser ) const;

	/**
	 * @brief <b>既にキャッシュされているかどうか</b>\n
	 * @return キャッシュされていればtrue
	 */
	bool IsCached() const { return m_bCached; }
	/**
	 * @brief <b>キャッシュをクリアする</b>\n
	 */
	void ClearTableCache();
private:
	std::list<CConnectUser>		m_listCache;	//!< キャッシュ用リスト
	bool						m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};

} // workpurpose

} // sindy

#endif // _CONNECTUSERTABLE_H_
