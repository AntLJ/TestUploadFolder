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
 * @file LockMeshTable.h
 * @brief <b>CLockMeshTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LOCKMESHTABLE_H_
#define _LOCKMESHTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "LockMesh.h"
#include <list>

namespace sindy {
namespace lockmesh {
/**
 * @class CLockMeshTable
 * @brief <b>編集ロックメッシュ管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.LOCKMESHテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CLockMeshTable : public CTable  
{
public:
	CLockMeshTable() : m_bCached(false) {}
	virtual ~CLockMeshTable() {}

	CLockMeshTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CLockMeshTable& operator=(const CLockMeshTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>ロックされているかどうかをチェックする</b>
	 * キャッシュされているものから検索を行います。
	 * @param lConnUserID	[in]	接続ユーザ名ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param lCode			[in]	メッシュコード
	 * @retval true ロックされている
	 * @retval false ロックされていない
	 */
	bool IsLocked( long lConnUserID, long lLayerNameID, long lCode ) const;

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
	std::map<long, std::map<long, std::list<CLockMesh> > >	m_mapCache;	//!< キャッシュ用リスト
	bool													m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // lockmesh

} // sindy

#endif // _LOCKMESHTABLE_H_
