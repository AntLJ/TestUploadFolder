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
 * @file EditAttrLayersTable.h
 * @brief <b>CEditAttrLayersTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _EDITATTRLAYERSTABLE_H_
#define _EDITATTRLAYERSTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Table.h"
#include "EditAttrLayers.h"
#include <map>

class CListString;
namespace sindy {
namespace editattrlayers {
/**
 * @class CEditAttrLayersTable
 * @brief <b>属性編集可能レイヤテーブルクラス（ルール用）</b>\n
 * このクラスは、REFERENCE.EDITATTRLAYERSテーブルのラップクラスです。
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CEditAttrLayersTable : public CTable  
{
public:
	CEditAttrLayersTable() : m_bCached(false) {}
	virtual ~CEditAttrLayersTable() {}

	CEditAttrLayersTable& operator=(ITable* lp)
	{
		AtlComPtrAssign((IUnknown**)&p, lp);
		return *this;
	}
	CEditAttrLayersTable& operator=(const CEditAttrLayersTable& lp )
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
	const CEditAttrLayers* Find( long lPurposeID, long lLayerNameID ) const;

	/**
	 * @brief <b>属性編集可能レイヤの一覧を返す</b>\n
	 * 格納コンテナはクリアしませんので、既存のアイテムがある場合、そのまま
	 * 追加されます。また、重複も考慮しませんので、考慮する場合は sort、unique
	 * してください。
	 * @param lPurposeID	[in]		作業目的ID
	 * @param listLayerName	[out]		属性編集可能レイヤリスト
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetEditAttrLayers( long lPurposeID, CListString& listLayerName ) const;

	/**
	 * @brief <b>属性編集可能レイヤかどうかチェックする</b>\n
	 * @warning この関数では、属性による制限はチェック対象外です。
	 * 通常はIsAttributeEditableを使用してください。
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	チェック対象のレイヤID
	 * @retval true 属性編集可能
	 * @retval false 属性編集不可能
	 */
	bool IsEditAttrLayer( long lPurposeID, long lLayerNameID ) const { return ( Find( lPurposeID, lLayerNameID ) ) ? true : false; }

	/**
	 * @brief <b>渡されたオブジェクトが属性編集可能かどうかをチェックする</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsAttributeEditable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

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
	std::map<long, std::map<long, CEditAttrLayers> >	m_mapCache;	//!< キャッシュ用リスト
	bool												m_bCached;	//!< 全レコードをキャッシュしたかどうかのフラグ
};
#pragma warning(pop)
} // editattrlayers

} // sindy

#endif // _EDITATTRLAYERSTABLE_H_
