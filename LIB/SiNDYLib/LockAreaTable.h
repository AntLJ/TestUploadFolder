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
 * @file LockAreaTable.h
 * @brief <b>CLockAreaTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LOCKAREATABLE_H_
#define _LOCKAREATABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "LockArea.h"

namespace sindy {
namespace lockarea {
/**
 * @class CLockArea
 * @brief </b>編集ロックエリア管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.LOCKAREAテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CLockAreaTable : public CTable  
{
public:
	CLockAreaTable() : m_bCached(false) {}
	virtual ~CLockAreaTable() {}

	CLockAreaTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CLockAreaTable& operator=(const CLockAreaTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}

	/**
	 * @brief <b>ロックされているかどうかをチェックする</b>\n
	 * キャッシュされているものから検索を行います。
	 * @param lConnUserID	[in]	接続ユーザ名ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param ipGeom		[in]	チェックエリア
	 * @retval true ロックされている
	 * @retval false ロックされていない
	 */
	bool IsLocked( long lConnUserID, long lLayerNameID, IGeometry* ipGeom ) const;

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
	std::map<long, std::map<long, CLockArea> >			m_mapCache;	//!< キャッシュ用リスト
	bool												m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // lockarea

} // sindy

#endif // _LOCKAREATABLE_H_
