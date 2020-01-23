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
 * @file ConnectUser.h
 * @brief <b>CConnectUserクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _CONNECTUSER_H_
#define _CONNECTUSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace connectuser {

/**
 * @class CConnectUser
 * @brief <b>接続ユーザレコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.CONNECTUSERテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CConnectUser : public CRow  
{
public:
	ROWCLASS_CONSTRUCTOR( CConnectUser, connectuser )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>接続ユーザ名IDを取得する</b>\n
	 * @retval 0以上 接続ユーザ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetConnectUserID() const {
		return GetLongValueByFieldName( schema::connectuser::kConnectUserID, -1 );
	}

	/**
	 * @brief <b>接続ユーザ名を取得する</b>\n
	 * @param nLen		[in]	バッファ格納可能文字列数
	 * @param lpszName	[out]	格納バッファ
	 * @retval LPCTSTR 接続ユーザ名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::connectuser::kName, nLen, lpszName );
	}
	//@]
	//@{ @name 属性設定関数
	/**
	 * @brief <b>接続ユーザ名IDを設定する</b>\n
	 * @param nID		[in]	接続ユーザ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetConnectUserID( long nID ) {
		return SetLongValueByFieldName( schema::connectuser::kConnectUserID, nID );
	}

	/**
	 * @brief <b>接続ユーザ名を設定する</b>\n
	 * @param lpcszName	[in]	接続ユーザ名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::connectuser::kName, lpcszName );
	}
	//@}
};

} // connectuser

} // sindy

#endif // _CONNECTUSER_H_
