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
 * @file OperatorTable.h
 * @brief <b>COperatorTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _OPERATORTABLE_H_
#define _OPERATORTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "Operator.h"
#include <list>

namespace sindy {
namespace soperator {
/**
 * @class COperatorTable
 * @brief <b>作業者管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.OPERATORテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API COperatorTable : public CTable  
{
public:
	COperatorTable() : m_bCached(false) {}
	virtual ~COperatorTable() {}

	COperatorTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	COperatorTable& operator=(const COperatorTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>作業者名から作業者名IDを取得する</b>\n
	 * キャッシュされているものから検索を行います。
	 * @param lpcszOperatorName	[in]	作業者名
	 * @retval 0以上 作業者名ID
	 * @retval -1 エラー又は該当するものがない
	 */
	long COperatorTable::GetOperatorID( LPCTSTR lpcszOperatorName ) const;

	/**
	 * @brief <b>テーブルを全てキャッシュする</b>\n
	 * テーブルの内容を全てキャッシュします。
	 * bDetatchをtrueにすると、読み込んだレコード及び
	 * テーブルのDBへの接続を切り離します。
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>キャッシュをクリアする</b>\n
	 */
	void ClearTableCache();

	/**
	 * @brief <b>既にキャッシュされているかどうか</b>\n
	 * @return キャッシュされていればtrue
	 */
	bool IsCached() const { return m_bCached; }
private:
	std::list<COperator>		m_listCache;	//!< キャッシュ用リスト
	bool						m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // soperator

} // sindy

#endif // _OPERATORTABLE_H_
