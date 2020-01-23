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
 * @file AnnotationClassTable.h
 * @brief CAnnotationClassTableクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ANNOTATIONCLASSTABLE_H_
#define _ANNOTATIONCLASSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "AnnotationClass.h"
#include <list>

namespace sindy {
namespace annotation_class {

extern double g_dGetRowByAnnoCodeTime;	//!< GetRowByAnnoCodeにかかる総時間

/**
 * @class CAnnotationClassTable
 * @brief <b>注記ルールテーブルクラス</b>\n
 * 注記ルールテーブル（ANNOTATION_CLASS）のラッパークラスです。
 * 編集時における注記ルールの適用、及びルール編集のサポートをします。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CAnnotationClassTable : public CTable  
{
public:
	CAnnotationClassTable() : m_bCached(false) {}
	virtual ~CAnnotationClassTable() {}

	CAnnotationClassTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CAnnotationClassTable& operator=(const CAnnotationClassTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>注記種別で検索を行う</b>
	 * キャッシュされているものから検索を行います。
	 * @param emTableType	[in]	テーブルタイプ
	 * @param emCode		[in]	注記種別
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const CAnnotationClass* GetRowByAnnoCode( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const;

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
	 * @brief <b>接続しているテーブルを切り離す</b>\n
	 * キャッシュを作成した後にルールDBへの接続を切り
	 * 離す場合に使用してください。
	 */
	//void DetatchTable(){ Release(); }

	/**
	 * @brief <b>既にキャッシュされているかどうか</b>\n
	 * @return キャッシュされていればtrue
	 */
	bool IsCached() const { return m_bCached; }

	//@{ @name キャッシュコンテナ参照系定義
	typedef std::list<CAnnotationClass>::const_iterator const_iterator;
	const_iterator begin() const { return m_listCache.begin(); }
	const_iterator end() const { return m_listCache.end(); }
	//@}
private:
	std::list<CAnnotationClass>	m_listCache;	//!< キャッシュ用リスト
	bool						m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)

} // annotation_class

} // sindy

#endif // _ANNOTATIONCLASSTABLE_H_
