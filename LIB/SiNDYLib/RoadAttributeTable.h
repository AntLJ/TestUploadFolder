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
 * @file RoadAttributeTable.h
 * @brief <b>CRoadAttributeTableクラス定義ファイル</b>
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ROADATTRIBUTETABLE_H_
#define _ROADATTRIBUTETABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "RoadAttribute.h"
#include <list>

namespace sindy {
namespace roadattribute {
/**
 * @class CRoadAttributeTable
 * @brief <b>道路リンク属性名管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.ROADATTRIBUTEテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CRoadAttributeTable : public CTable
{
public:
	CRoadAttributeTable() : m_bCached(false) {}
	virtual ~CRoadAttributeTable() {}

	CRoadAttributeTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CRoadAttributeTable& operator=(const CRoadAttributeTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>フィールド名に対応するIDを返す</b>
	 * キャッシュされているものから検索を行います。
	 * @param lpcszFieldName	[in]	フィールド名
	 * @retval 0以上 フィールド名ID
	 * @retval -1 エラー又は対応するIDがない
	 */
	long GetFieldNameID( LPCTSTR lpcszFieldName ) const;

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
	std::list<CRoadAttribute>		m_listCache;	//!< キャッシュ用リスト
	bool							m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // roadattribute

} // sindy

#endif // _ROADATTRIBUTETABLE_H_
