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
 * @file CreatableLayersTable.h
 * @brief <b>CCreatableLayersTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _CREATABLELAYERSTABLE_H_
#define _CREATABLELAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "CreatableLayers.h"
//#include "RuleDef.h"

class CListString;
namespace sindy {
namespace creatablelayers {
/**
 * @class CCreatableLayersTable
 * @brief <b>作成可能レイヤ管理テーブルクラス</b>\n
 * このクラスはREFERNCE.CREATABLELAYERSテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CCreatableLayersTable : public CTable  
{
public:
	CCreatableLayersTable() : m_bCached(false) {}
	virtual ~CCreatableLayersTable() {}

	CCreatableLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CCreatableLayersTable& operator=(const CCreatableLayersTable& lp )
	{
		AtlComPtrAssign((IUnknown**)&p, (ITable*)lp );
		return *this;
	}
	/**
	 * @brief <b>作業目的番号とレイヤ名に対応するレコードをキャッシュから探して返す</b>
	 * キャッシュされているものから検索を行いますので、
	 * キャッシュが作成されていることが前提です。
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @return 該当レコードがない場合はNULLを返す
	 */
	const CCreatableLayers* Find( long lPurposeID, long lLayerNameID ) const;
	CCreatableLayers* Find( long lPurposeID, long lLayerNameID );

	/**
	 * @brief <b>作業目的番号とレイヤ名に対応するレコードをデータベースから探して返す</b>\n
	 * キャッシュがあってもキャッシュを使用せずにデータベースに
	 * 直接問い合わせを行います。\n
	 * 複数存在する場合でも常に最初に見つかったもののみ返します。
	 * その際、トレースログにその旨表示されます。
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @return 該当レコードがない場合はNULLを返す
	 */
	_IRowPtr CCreatableLayersTable::GetRow( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>作成可能レイヤの一覧を返す</b>\n
	 * 格納コンテナはクリアしませんので、既存のアイテムがある場合、そのまま
	 * 追加されます。また、重複も考慮しませんので、考慮する場合は sort、unique
	 * してください。
	 * @param lPurposeID	[in]		作業目的ID
	 * @param listLayerName	[out]		作成可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetCreatableLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>作成可能レイヤかどうかチェックする</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	チェック対象のレイヤID
	 * @retval true 作成可能
	 * @retval false 作成不可能
	 */
	bool IsCreatableLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>渡されたオブジェクトが作成可能かどうかをチェックする</b>
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsCreatable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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
	 * @brief <b>既にキャッシュされているかどうか</b>\n
	 * @return キャッシュされていればtrue
	 */
	bool IsCached() const { return m_bCached; }
	/**
	 * @brief <b>キャッシュをクリアする</b>\n
	 */
	void ClearTableCache();

	//@{ @name 編集系関数
	/**
	 * @brief <b>レイヤを追加する</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤID
	 * @return 該当レコードが既にある場合又は失敗の場合はNULLを、無い場合は新規作成してそのレコードを返す
	 */
	CCreatableLayers* AddLayer( long lPurposeID, long lLayerNameID );

	/**
	 * @brief <b>レイヤを削除する</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤID
	 * sindyErrCode
	 */
	errorcode::sindyErrCode DeleteLayer( long lPurposeID, long lLayerNameID );
	//@}

	//@{ @name コンテナ操作系関数
	typedef std::map<long, CCreatableLayers> CCreatableLayersList;
	typedef std::map<long, CCreatableLayersList> CCreatableLayersLists;
	CCreatableLayersList::const_iterator by_purpose_begin(long lPurposeID) const {
		CCreatableLayersLists::const_iterator it = m_mapCache.find( lPurposeID );
		if( it != m_mapCache.end() )
			return it->second.begin();
		else
			return CCreatableLayersList::const_iterator();
	}
	CCreatableLayersList::const_iterator by_purpose_end(long lPurposeID) const {
		CCreatableLayersLists::const_iterator it = m_mapCache.find( lPurposeID );
		if( it != m_mapCache.end() )
			return it->second.end();
		else
			return CCreatableLayersList::const_iterator();
	}
	//@}
private:
	CCreatableLayersLists	m_mapCache;	//!< キャッシュ用リスト
	bool					m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // creatablelayers

} // sindy

#endif // _CREATABLELAYERSTABLE_H_
