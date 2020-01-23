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
 * @file MovableLayers.h
 * @brief <b>CMovableLayersクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _MOVABLELAYERS_H_
#define _MOVABLELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbleLayers.h"
#include "RuleCommunity.h"

namespace sindy {
namespace movablelayers {
/**
 * @class CMovableLayers
 * @brief <b>移動可能レイヤ管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.MOVABLELAYERSテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CMovableLayers : public CAbleLayers  
{
	ROWCLASS_CONSTRUCTOR( CMovableLayers, movablelayers )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>作業目的IDを取得する</b>\n
	 * @retval 0以上 作業目的ID
	 * @retval -1 エラー又はNULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::movablelayers::kPurposeID, -1 );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::movablelayers::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>属性制限1を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict1( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict1, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限2を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict2( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict2, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限3を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict3( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict3, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限4を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict4( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict4, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限5を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict5( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict5, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限6を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict6( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict6, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限7を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict7( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict7, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限8を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict8( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict8, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限9を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict9( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict9, nLen, lpszRestrict );
	}

	/**
	 * @brief <b>属性制限10を取得する</b>\n
	 * @param nLen			[in]	lpszRestrictの収納可能文字列数
	 * @param lpszRestrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetRistrict10( int nLen, LPTSTR lpszRestrict ) const {
		return GetStringValueByFieldName( schema::movablelayers::kRistrict10, nLen, lpszRestrict );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>作業目的IDを設定する</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::movablelayers::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lLayerNameID ) {
		return SetLongValueByFieldName( schema::movablelayers::kLayerNameID, lLayerNameID );
	}

	/**
	 * @brief <b>属性制限1を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict1( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict1, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限2を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict2( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict2, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限3を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict3( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict3, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限4を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict4( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict4, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限5を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict5( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict5, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限6を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict6( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict6, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限7を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict7( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict7, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限8を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict8( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict8, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限9を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict9( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict9, lpcszRestrict );
	}

	/**
	 * @brief <b>属性制限10を設定する</b>\n
	 * @param lpcszRestrict	[in]	属性制限文字列
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRistrict10( LPCTSTR lpcszRestrict ) {
		return SetStringValueByFieldName( schema::movablelayers::kRistrict10, lpcszRestrict );
	}
	//@}
};

} // movablelayers

} // sindy

#endif // _MOVABLELAYERS_H_
