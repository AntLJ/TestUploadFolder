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
 * @file DeletableLayersTable.h
 * @brief <b>CDeletableLayersTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _DELETABLELAYERSTABLE_H_
#define _DELETABLELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "DeletableLayers.h"

class CListString;
namespace sindy {
namespace deletablelayers {
/**
 * @class CDeletableLayersTable
 * @brief <b>削除可能レイヤ管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.DELETABLELAYERSテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CDeletableLayersTable : public CTable  
{
public:
	CDeletableLayersTable() : m_bCached(false) {}
	virtual ~CDeletableLayersTable(){}

	CDeletableLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CDeletableLayersTable& operator=(const CDeletableLayersTable& lp )
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
	const CDeletableLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>削除可能レイヤの一覧を返す</b>\n
	 * 格納コンテナはクリアしませんので、既存のアイテムがある場合、そのまま
	 * 追加されます。また、重複も考慮しませんので、考慮する場合は sort、unique
	 * してください。
	 * @param lPurposeID	[in]		作業目的ID
	 * @param listLayerName	[out]		削除可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetDeletableLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>作成可能レイヤかどうかチェックする</b>\n
	 * @warning この関数では、属性による制限はチェック対象外です。
	 * 通常はIsDeletableを使用してください。
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	チェック対象のレイヤID
	 * @retval true 削除可能
	 * @retval false 削除不可能
	 */
	bool IsDeletableLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>渡されたオブジェクトが削除可能かどうかをチェックする</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsDeletable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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
private:
	std::map<long, std::map<long, CDeletableLayers> >	m_mapCache;	//!< キャッシュ用リスト
	bool												m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // deletablelayers

} // sindy

#endif // _DELETABLELAYERSTABLE_H_
