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
 * @file SharedDeleteLayersTable.h
 * @brief <b>CSharedDeleteLayersTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _SHAREDDELETELAYERSTABLE_H_
#define _SHAREDDELETELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "SharedDeleteLayers.h"
#include <list>

class CListLong;
namespace sindy {
namespace shareddeletelayers {
/**
 * @class CSharedDeleteLayersTable
 * @brief <b>共有削除管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.SHAREDDELETELAYERSテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CSharedDeleteLayersTable : public CTable
{
public:
	CSharedDeleteLayersTable() : m_bCached(false) {}
	virtual ~CSharedDeleteLayersTable() {}

	CSharedDeleteLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CSharedDeleteLayersTable& operator=(const CSharedDeleteLayersTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>作業目的番号とレイヤ名に対応するレコードを返す</b>
	 * キャッシュされているものから検索を行います。
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @return 該当レコードがない場合はNULLを返す
	 */
//	const CSharedDeleteLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>渡されたレイヤ名IDに対応する共有削除対象レイヤ名IDを返す</b>\n
	 * @param lLayerNameID		[in]	レイヤ名ID
	 * @param listLayerNameID	[out]	レイヤ名IDを格納するリスト
	 * @retval 0以上 共有削除対象レイヤ数
	 * @retval -1 エラー
	 */
	long GetSharedDeleteLayers( long lLayerNameID, CListLong& listLayerNameID ) const;

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
	std::list<CSharedDeleteLayers>	m_listCache;	//!< キャッシュ用リスト
	bool							m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // shareddeletelayers

} // sindy

#endif // _SHAREDDELETELAYERSTABLE_H_
