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
 * @file DispNaviRelationTable.h
 * @brief <b>CDispNaviRelationTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _DISPNAVIRELATIONTABLE_H_
#define _DISPNAVIRELATIONTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "DispNaviRelation.h"

namespace sindy {
namespace dispnavirelation {
/**
 * @class CDispNaviRelationTable
 * @brief <b>道路リンクの道路種別と経路種別の関連管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.DISPNAVIRELATIONテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CDispNaviRelationTable : public CTable  
{
public:
	CDispNaviRelationTable() : m_bCached(false) {}
	virtual ~CDispNaviRelationTable() {}

	CDispNaviRelationTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CDispNaviRelationTable& operator=(const CDispNaviRelationTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>道路リンクの道路種別名IDと道路リンクの経路種別IDに対応するレコードを返す</b>
	 * キャッシュされているものから検索を行います。
	 * @param lDispClassID	[in]	道路リンクの道路種別名ID
	 * @param lNaviClassID	[in]	道路リンクの経路種別ID
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const CDispNaviRelation* Find( long lDispClassID, long lNaviClassID ) const;

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
	std::map<long, std::map<long, CDispNaviRelation> >	m_mapCache;	//!< キャッシュ用リスト
	bool												m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // deletablelayers

} // sindy

#endif // _DISPNAVIRELATIONTABLE_H_
