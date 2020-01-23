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
 * @file LayerNamesTable.h
 * @brief <b>CLayerNamesTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LAYERNAMESTABLE_H_
#define _LAYERNAMESTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "LayerNames.h"
#include <list>

namespace sindy {
namespace layernames {
/**
 * @class CLayerNamesTable
 * @brief <b>レイヤ名称管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.LAYERNAMESテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CLayerNamesTable : public CTable  
{
public:
	CLayerNamesTable() : m_bCached(false) {}
	virtual ~CLayerNamesTable() {}

	CLayerNamesTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CLayerNamesTable& operator=(const CLayerNamesTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>レイヤ名IDからレイヤ名を取得する</b>\n
	 * キャッシュされているものから検索を行います。
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param nLen			[in]	バッファ収納可能文字数
	 * @param lpszName		[out]	格納バッファ
	 * @retval LPCTSTR lpszName
	 * @retval NULL エラー又は対応するものが存在しない
	 */
	LPCTSTR Find( long lLayerNameID, int nLen, LPTSTR lpszName ) const;

	/**
	 * @brief <b>レイヤ名に対応するレイヤ名IDを取得する</b>
	 * キャッシュされているものから検索を行います。
	 * @param lpcszLayerName	[in]	レイヤ名
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 登録されていない
	 */
	long Find( LPCTSTR lpcszLayerName ) const;

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
	std::list<CLayerNames>	m_listCache;	//!< キャッシュ用リスト
	bool					m_bCached;		//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // layernames

} // sindy

#endif // _LAYERNAMESTABLE_H_
