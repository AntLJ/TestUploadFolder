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
 * @file RoadNaviClassTable.h
 * @brief <b>CRoadNaviClassTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ROADNAVICLASSTABLE_H_
#define _ROADNAVICLASSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "RoadNaviClass.h"

namespace sindy {
namespace roadnaviclass {
/**
 * @class CRoadNaviClassTable
 * @brief <b>道路リンクの経路種別管理テーブルクラス（ルール用）</b>\n
 * このクラスは、REFERENCE.ROADNAVICLASSテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CRoadNaviClassTable : public CTable  
{
public:
	CRoadNaviClassTable() : m_bCached(false) {}
	virtual ~CRoadNaviClassTable() {}

	CRoadNaviClassTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CRoadNaviClassTable& operator=(const CRoadNaviClassTable& lp )
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
//	const CRoadNaviClass* Find( long lPurposeID, long lLayerNameID ) const;

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
	std::map<CString, CRoadNaviClass>	m_mapCache;	//!< キャッシュ用リスト
	bool								m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // roadnaviclass

} // sindy

#endif // !defined(AFX_ROADNAVICLASSTABLE_H__0B7D9707_0C0A_4074_972E_5F3D25DE151D__INCLUDED_)
