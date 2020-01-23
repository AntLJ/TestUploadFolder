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
 * @file EditHistory.h
 * @brief <b>CEditHistoryクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _EDITHISTORY_H_
#define _EDITHISTORY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace edithistory {
/**
 * @class CEditHistory
 * @brief <b>ルールの編集履歴保持レコードクラス（ルール用）</b>\n
 * このクラスは、REFERENCE.EDITHISTORYテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CEditHistory : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CEditHistory, edithistory )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>編集日時を取得する</b>\n
	 * @param nLen		[in]	バッファ格納可能文字数
	 * @param lpszDate	[out]	取得文字列格納用バッファ
	 * @retval LPCTSTR 編集日時
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetModifyDate( int nLen, LPTSTR lpszDate ) const {
		return GetStringValueByFieldName( schema::edithistory::kModifyDate, nLen, lpszDate );
	}

	/**
	 * @brief <b>編集テーブル名を取得する</b>\n
	 * @param nLen		[in]	バッファ格納可能文字数
	 * @param lpszName	[out]	取得文字列格納用バッファ
	 * @retval LPCTSTR 編集テーブル名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetModifyTable( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::edithistory::kModifyTable, nLen, lpszName );
	}

	/**
	 * @brief <b>編集レコードOBJECTIDを取得する</b>\n
	 * @retval 0以上 編集レコードOBJECTID
	 * @retval -1 エラー又はNULL
	 */
	long GetModifyOID() const {
		return GetLongValueByFieldName( schema::edithistory::kModifyOID, -1 );
	}

	/**
	 * @brief <b>編集コードを取得する</b>\n
	 * @retval 0以上 編集コード
	 * @retval -1 エラー又はNULL
	 */
	long GetUpdateType() const {
		return GetLongValueByFieldName( schema::edithistory::kUpdateType, -1 );
	}

	/**
	 * @brief <b>編集者名を取得する</b>\n
	 * @param nLen		[in]	バッファ格納可能文字数
	 * @param lpszName	[out]	取得文字列格納用バッファ
	 * @retval LPCTSTR 編集者名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetOperator( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::edithistory::kOperator, nLen, lpszName );
	}

	/**
	 * @brief <b>メッセージを取得する</b>\n
	 * @param nLen			[in]	バッファ格納可能文字数
	 * @param lpszMessage	[out]	取得文字列格納用バッファ
	 * @retval LPCTSTR メッセージ
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetMessage( int nLen, LPTSTR lpszMessage ) const {
		return GetStringValueByFieldName( schema::edithistory::kMessage, nLen, lpszMessage );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>編集日時を設定する</b>\n
	 * @param lpcszDate	[in]	編集日時
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetModifyDate( LPCTSTR lpcszDate ) {
		return SetStringValueByFieldName( schema::edithistory::kModifyDate, lpcszDate );
	}

	/**
	 * @brief <b>編集テーブル名を設定する</b>\n
	 * @param lpcszName	[in]	編集テーブル名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetModifyTable( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::edithistory::kModifyTable, lpcszName );
	}

	/**
	 * @brief <b>編集レコードOBJECTIDを設定する</b>\n
	 * @param lOID		[in]	編集レコードOBJECTID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetModifyOID( long lOID ) {
		return SetLongValueByFieldName( schema::edithistory::kModifyOID, lOID );
	}

	/**
	 * @brief <b>編集コードを設定する</b>\n
	 * @param lCode		[in]	編集コード
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetUpdateType( long lCode ) {
		return SetLongValueByFieldName( schema::edithistory::kUpdateType, lCode );
	}

	/**
	 * @brief <b>編集者名を設定する</b>\n
	 * @param lpcszName	[in]	編集者名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOperator( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::edithistory::kOperator, lpcszName );
	}

	/**
	 * @brief <b>メッセージを設定する</b>\n
	 * @param lpcszMessage	[in]	メッセージ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetMessage( LPCTSTR lpcszMessage ) {
		return SetStringValueByFieldName( schema::edithistory::kMessage, lpcszMessage );
	}
	//@}
};

} // edithistory

} // sindy

#endif // _EDITHISTORY_H_
