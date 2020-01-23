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
 * @file DispOtherRelationTable.h
 * @brief <b>CDispOtherRelationTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _DISPOTHERRELATIONTABLE_H_
#define _DISPOTHERRELATIONTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "DispOtherRelation.h"

namespace sindy {
namespace dispotherrelation {
/**
 * @class CDispOtherRelationTable
 * @brief <b>道路リンクの表示種別とその他の属性の対応管理テーブルクラス（ルール用）</b>\n
 * このクラスは、REFERENCE.DISPOTHERRELATIONテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CDispOtherRelationTable : public CTable  
{
public:
	CDispOtherRelationTable() : m_bCached(false) {}
	virtual ~CDispOtherRelationTable() {}

	CDispOtherRelationTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CDispOtherRelationTable& operator=(const CDispOtherRelationTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>道路リンクの道路種別名IDと道路リンクのその他の属性のIDに対応するレコードを返す</b>
	 * キャッシュされているものから検索を行います。
	 * @param lDispClassID	[in]	道路リンクの道路種別名ID
	 * @param lOtherAttrID	[in]	道路リンクのその他の属性のID
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const CDispOtherRelation* Find( long lDispClassID, long lOtherAttrID ) const;

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
	std::map<long, std::map<long, CDispOtherRelation> >	m_mapCache;	//!< キャッシュ用リスト
	bool												m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // dispotherrelation

} // sindy

#endif // _DISPOTHERRELATIONTABLE_H_
