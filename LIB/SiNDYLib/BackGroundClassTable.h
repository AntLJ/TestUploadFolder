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
 * @file BackGroundClassTable.h
 * @brief <b>CBackGroundClassTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _BACKGROUNDCLASSTABLE_H_
#define _BACKGROUNDCLASSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "BackgroundClass.h"
#include <list>

namespace sindy {
namespace background_class {
/**
 * @class CBackGroundClassTable
 * @brief <b>背景種別管理テーブルクラス</b>\n
 * このクラスはREFERENCE.BACKGROUNDCLASSテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CBackGroundClassTable : public CTable  
{
public:
	CBackGroundClassTable() : m_bCached(false) {}
	virtual ~CBackGroundClassTable() {}

	CBackGroundClassTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CBackGroundClassTable& operator=(const CBackGroundClassTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>注記種別で検索を行う</b>
	 * キャッシュされているものから検索を行います。
	 * @param emAnnoLayer	[in]	真位置レイヤタイプ
	 * @param emCode		[in]	注記種別
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const CBackGroundClass* GetRowByAnnoCode( /*sindyAnnotationLayer emAnnoLayer, annotation_code::ECode emCode*/ ) const;

	/**
	 * @brief <b>テーブルを全てキャッシュする</b>\n
	 * テーブルの内容を全てキャッシュします。
	 * bDetatchをtrueにすると、読み込んだレコード及び
	 * テーブルのDBへの接続を切り離します。
	 */
	long CreateTableCache( bool bDetatch = false );

	/**
	 * @brief <b>既にキャッシュされているかどうか</b>\n
	 * @return キャッシュされていればtrue
	 */
	bool IsCached() const { return m_bCached; }
	/**
	 * @brief <b>キャッシュをクリアする</b>\n
	 */
	void ClearTableCache();
	//@{ @name キャッシュコンテナ参照系定義
	typedef std::list<CBackGroundClass>::const_iterator const_iterator;
	const_iterator begin() const { return m_listCache.begin(); }
	const_iterator end() const { return m_listCache.end(); }
	//@}

private:
//	std::map<annotation_code::ECode,CBackGroundClass>	m_mapCache;		//!< キャッシュ用リスト（キーは背景種別）
	std::list<CBackGroundClass>	m_listCache;	//!< キャッシュ用リスト
	bool												m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};

} // background_class

} // sindy

#endif // _BACKGROUNDCLASSTABLE_H_
