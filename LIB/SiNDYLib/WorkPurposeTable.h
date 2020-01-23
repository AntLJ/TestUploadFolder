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
 * @file WorkPurposeTable.h
 * @brief <b>CWorkPurposeTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _WORKPURPOSETABLE_H_
#define _WORKPURPOSETABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "WorkPurpose.h"

namespace sindy {
namespace workpurpose {
/**
 * @class CWorkPurposeTable
 * @brief <b>作業目的管理テーブルクラス</b>\n
 * このクラスはREFERENCE.WORKPURPOSEテーブルのラップクラスです。
 */
class SINDYLIB_API CWorkPurposeTable : public CTable  
{
public:
	CWorkPurposeTable() : m_bCached(false) {}
	virtual ~CWorkPurposeTable() {}

	CWorkPurposeTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CWorkPurposeTable& operator=(const CWorkPurposeTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>作業目的番号に対応するレコードを返す</b>\n
	 * キャッシュされているものから検索を行います。
	 * @param lID	[in]	作業目的番号
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const CWorkPurpose* Find( long lID ) const;

	/**
	 * @brief <b>テーブルを全てキャッシュする</b>\n
	 * テーブルの内容を全てキャッシュします。
	 * bDetatchをtrueにすると、読み込んだレコード及び
	 * テーブルのDBへの接続を切り離します。
	 *
	 * @note 特定の作業目的だけキャッシュしたい場合はlPurposeIDに作業目的IDをセットします。
	 * デフォルトの状態では全作業目的をキャッシュします。
	 * 
	 * @param bDetatch [in,optional] キャッシュした後接続を切り離す（デフォルト：false）
	 * @param lPurposeID [in,optional] 作業目的を限定してキャッシュを作成する（デフォルト：-1）
	 */
	long CreateTableCache( bool bDetatch = false, long lPurposeID = -1 );

	/**
	 * @brief <b>キャッシュをクリアする</b>\n
	 */
	void ClearTableCache();

	/**
	 * @brief <b>既にキャッシュされているかどうか</b>\n
	 * @return キャッシュされていればtrue
	 */
	bool IsCached() const { return m_bCached; }

	//@{ @name イテレータ
	typedef std::map<long,CWorkPurpose>::iterator iterator;
	typedef std::map<long,CWorkPurpose>::const_iterator const_iterator;
	iterator begin() { return m_mapCache.begin(); }
	iterator end() { return m_mapCache.end(); }
	const_iterator begin() const { return m_mapCache.begin(); }
	const_iterator end() const { return m_mapCache.end(); }
	//@}
private:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::map<long,CWorkPurpose>	m_mapCache;		//!< キャッシュ用リスト（キーは背景種別）
#pragma warning(pop)
	bool						m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};

} // workpurpose

} // sindy

#endif // _WORKPURPOSETABLE_H_
