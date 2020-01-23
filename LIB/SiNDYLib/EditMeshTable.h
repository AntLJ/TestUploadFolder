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
 * @file EditMeshTable.h
 * @brief <b>CEditMeshTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _EDITMESHTABLE_H_
#define _EDITMESHTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "EditMesh.h"
#include "Geometry.h"
#include <list>

namespace sindy {
class CGeometry;
namespace editmesh {
/**
 * @class CEditMeshTable
 * @brief <b>編集可能メッシュ管理テーブルクラス（ルール用）</b>\n
 * このクラスは、REFERNCE.EDITMESHテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CEditMeshTable : public CTable  
{
public:
	CEditMeshTable() : m_bCached(false) {}
	virtual ~CEditMeshTable() {}

	CEditMeshTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CEditMeshTable& operator=(const CEditMeshTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>編集可能なエリアかどうかをチェックする</b>\n
	 * キャッシュされているものから検索を行います。
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param lOperatorID	[in]	作業者名ID
	 * @param ipGeom		[in]	チェックエリア
	 * @retval true 編集可能
	 * @retval false 編集不可
	 */
	bool IsEditable( long lPurposeID, long lLayerNameID, long lOperatorID, IGeometry* ipGeom ) const;

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

	/**
	 * @brief <b>作業可能エリアを設定する</b>\n
	 * @param ipGeom [in] セットするエリア
	 * @note NULLをセットすると空になります
	 */
	void SetGlobalWorkArea( IGeometry* ipGeom );

	/**
	 * @brief <b>作業可能エリアを取得する</b>\n
	 * @return 作業可能エリア
	 */
	const CGeometry& GetGlobalWorkArea() const;

private:
	CGeometry m_cGlobalWorkArea; //!< 全てのレイヤ・作業目的に課せられる作業エリア（キャッシュ対象ではない、空なら制限なし）
	std::map<long, std::map<long, std::map<long, std::pair<CGeometry, std::list<CEditMesh> > > > >	m_mapCache;	//!< キャッシュ用リスト
	bool m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // editmesh

} // sindy

#endif // _EDITMESHTABLE_H_
