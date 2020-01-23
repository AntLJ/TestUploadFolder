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
 * @file LayerNames.h
 * @brief <b>CLayerNamesクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LAYERNAMES_H_
#define _LAYERNAMES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace layernames {
/**
 * @class CLayerNames
 * @brief <b>レイヤ名管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.LAYERNAMESテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CLayerNames : public CRow
{
	ROWCLASS_CONSTRUCTOR( CLayerNames, layernames )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>レイヤ名を取得する</b>\n
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetLayerName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::layernames::kLayerName, nLen, lpszName );
	}
	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又は登録されていない
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::layernames::kLayerNameID, -1 );
	}
	/**
	 * @brief <b>レイヤエイリアス名を取得する</b>\n
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetLayerAliasName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::layernames::kLayerAliasName, nLen, lpszName );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>レイヤ名を設定する</b>\n
	 * @param lpcszName	[in]	設定するレイヤ名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::layernames::kLayerName, lpcszName );
	}
	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lID		[in]	設定するレイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::layernames::kLayerNameID, lID );
	}
	/**
	 * @brief <b>レイヤエイリアス名を設定する</b>\n
	 * @param lpcszName	[in]	設定するレイヤエイリアス名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerAliasName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::layernames::kLayerAliasName, lpcszName );
	}
	//@}
};

} // layernames 

} // sindy

#endif // _LAYERNAMES_H_
