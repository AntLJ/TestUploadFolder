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
 * @file BackGroundClass.h
 * @brief <b>CBackGroundClassクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _BACKGROUNDCLASS_H_
#define _BACKGROUNDCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace background_class {
/**
 * @class CBackGroundClass
 * @brief <b>背景系ルールレコードクラス</b>\n
 */
class SINDYLIB_API CBackGroundClass : public CRow
{
	ROWCLASS_CONSTRUCTOR( CBackGroundClass, background_class )
	/**
	 * @brief <b>テーブル名を取得する</b>\n
	 * @return テーブル名
	 */
	 LPCTSTR GetTableName() const { return schema::background_class::kTableName; }

	//@{ @name 属性取得関数
	/**
	 * @brief <b>種別を取得する</b>\n
	 * @retval 0以上 種別
	 * @retval -1 エラー又はNULL
	 */
	long GetClass() const {
		return GetLongValueByFieldName( schema::background_class::kClass, -1 );
	}

	/**
	 * @brief <b>種別名称を取得する</b>\n
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	種別名称を格納するバッファ
	 * @retval LPCTSTR 種別名称
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetClassName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::background_class::kClassName, nLen, lpszName );
	}

	/**
	 * @brief <b>原稿番号を取得する</b>\n
	 * @param nLen		[in]	lpszCodeの収納可能文字列数
	 * @param lpszCode	[out]	原稿番号を格納するバッファ
	 * @retval LPCTSTR 原稿番号
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetMSNo( int nLen, LPTSTR lpszCode ) const {
		return GetStringValueByFieldName( schema::background_class::kMSNo, nLen, lpszCode );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerID() const {
		return GetLongValueByFieldName( schema::background_class::kLayerID, -1 );
	}

	/**
	 * @brief <b>地下フラグを取得する</b>\n
	 * @return bool
	 */
	bool GetUnderGround() const {
		return GetBoolValueByFieldName( schema::background_class::kUnderGround, false );
	}

	/**
	 * @brief <b>IN種別コードを取得する</b>\n
	 * @param nLen		[in]	lpszCodeの収納可能文字列数
	 * @param lpszCode	[out]	IN種別コードを格納するバッファ
	 * @retval LPCTSTR 原稿番号
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetINClass( int nLen, LPTSTR lpszCode ) const {
		return GetStringValueByFieldName( schema::background_class::kInClass, nLen, lpszCode );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>種別を設定する</b>\n
	 * @param lCode		[in]	種別コード
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetClass( long lCode ) {
		return SetLongValueByFieldName( schema::background_class::kClass, lCode );
	}

	/**
	 * @brief <b>種別名称を設定する</b>\n
	 * @param lpcszName	[in]	種別名称
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetClassName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::background_class::kClassName, lpcszName );
	}

	/**
	 * @brief <b>原稿番号を設定する</b>\n
	 * @param lpcszCode	[in]	原稿番号
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetMSNo( LPCTSTR lpcszCode ) {
		return SetStringValueByFieldName( schema::background_class::kMSNo, lpcszCode );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param nID		[in]	レイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerID( long nID ) {
		return SetLongValueByFieldName( schema::background_class::kLayerID, nID );
	}

	/**
	 * @brief <b>地下フラグを設定する</b>\n
	 * @param bFlag		[in]	地下フラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetUnderGround( bool bFlag ) {
		return SetBoolValueByFieldName( schema::background_class::kUnderGround, bFlag );
	}

	/**
	 * @brief <b>IN種別コードを設定する</b>\n
	 * @param lpcszCode	[out]	IN種別コード
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetINClass( LPCTSTR lpcszCode ) {
		return SetStringValueByFieldName( schema::background_class::kInClass, lpcszCode );
	}
	//@}
};

} // background_class

} // sindy

#endif // _BACKGROUNDCLASS_H_
