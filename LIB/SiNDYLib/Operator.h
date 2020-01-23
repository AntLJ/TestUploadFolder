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
 * @file Operator.h
 * @brief <b>COperatorクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace soperator {
/**
 * @class COperator
 * @brief <b>作業者管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.OPERATORテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API COperator : public CRow  
{
	ROWCLASS_CONSTRUCTOR( COperator, soperator )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>オペレータ名IDを取得する</b>\n
	 * @retval 0以上 オペレータ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetOperatorID() const {
		return GetLongValueByFieldName( schema::soperator::kOperatorID, -1 );
	}

	/**
	 * @brief <b>オペレータ名を取得する</b>\n
	 * @param nLen		[in]	バッファ収納可能文字数
	 * @param lpszName	[out]	格納バッファ
	 * @retval LPCTSTR オペレータ名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::soperator::kName, nLen, lpszName );
	}

	/**
	 * @brief <b>オペレータのエイリアス名を取得する</b>\n
	 * @param nLen		[in]	バッファ収納可能文字数
	 * @param lpszName	[out]	格納バッファ
	 * @retval LPCTSTR エイリアス名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetAliasName( int nLen, LPTSTR lpszName ) const{
		return GetStringValueByFieldName( schema::soperator::kAliasName, nLen, lpszName );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>オペレータ名IDを設定する</b>\n
	 * @param lOperatorID	[in]	オペレータ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOperatorID( long lOperatorID ) {
		return SetLongValueByFieldName( schema::soperator::kOperatorID, lOperatorID );
	}

	/**
	 * @brief <b>オペレータ名を設定する</b>\n
	 * @param lpcszName		[in]	オペレータ名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::soperator::kName, lpcszName );
	}

	/**
	 * @brief <b>オペレータのエイリアス名を設定する</b>\n
	 * @param lpcszName		[in]	エイリアス名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAliasName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::soperator::kAliasName, lpcszName );
	}
	//@}
};

} // soperator

} // sindy

#endif // _OPERATOR_H_
