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

#pragma once

/**
 * @file	LogOutputter.h
 * @brief	進捗系の出力関数郡定義
 */

/**
 * @enum	ERROR_LEVEL
 * @brief	エラーレベル
 */
enum ERROR_LEVEL
{
	kInfo,		//!< 情報
	kWarning,	//!< 警告
	kError,		//!< エラー
	kFatal,		//!< 致命的エラー
};

namespace progress_out
{
	/**
	 * @brief	メッセージ整形
	 * @param	kErrorLevel	[in]	メッセージレベル
	 * @param	lpcszFormat	[in]	フォーマット
	 * @param	vaList		[in]	引数
	 * @return	CString
	 */
	CString FormatMsg( ERROR_LEVEL kErrorLevel, LPCTSTR lpcszFormat, va_list vaList );	

	/**
	 * @brief	進捗メッセージ出力
	 * @note	標準出力・標準エラーに出力
	 * @param	lpcszFormat	[in]	メッセージ
	 * @param	...			[in]	必要に応じて
	 */
	void Progress( LPCTSTR lpcszFormat, ... );

	/**
	 * @brief	時間表示
	 * @param	lpcszMsg	[in]	必要であれば
	 */
	void ProgressTime( LPCTSTR lpcszMsg = NULL );

	/**
	 * @brief	進捗系エラーメッセージ出力(戻り値あり)
	 * @param	ret			[in]	戻り値
	 * @param	kErrorLevel	[in]	エラーレベル	 
	 * @param	lpcszFormat	[in]	メッセージ
	 * @param	...			[in]	必要に応じて
	 */
	template <class T> inline T ProgressError( T ret, ERROR_LEVEL kErrorLevel, LPCSTR lpcszFormat, ... )
	{
		va_list vaList;
		if( lpcszFormat )
			va_start( vaList, lpcszFormat );

		std::cerr << FormatMsg(kErrorLevel, lpcszFormat, vaList) << std::endl;
		std::cout << FormatMsg(kErrorLevel, lpcszFormat, vaList) << std::endl;


		return ret;
	}
}

namespace object_out
{
	extern CString g_strCurrentCityCode;	//!< 市区町村コード

	/**
	 * @brief	ヘッダー出力
	 */
	void LogHeader();

	/**
	 * @brief	メッセージ整形
	 * @param	lpcszFormat	[in]	フォーマット
	 * @param	vaList		[in]	引数
	 * @return	CString
	 */
	inline CString FormatMsg( LPCTSTR lpcszFormat, va_list vaList )
	{
		CString strMsg;
		if( lpcszFormat )
			strMsg.AppendFormatV( lpcszFormat, vaList );
		else
			strMsg.Append( _T("メッセージなし") );
		return strMsg;
	}

	/**
	 * @brief	エラー出力
	 * @param	lpcszLayer	[in]	レイヤ名
	 * @param	lOID		[in]	OBJECTID
	 * @param	lpcszFormat	[in]	メッセージ
	 * @param	...			[in]	必要に応じて
	 */
	void ObjectError( LPCTSTR lpcszLayer, long lOID, LPCTSTR lpcszFormat, ... );

	/**
	 * @brief	エラー出力(IRow指定版)
	 * @param	ipRow		[in]	IRow
	 * @param	lpcszFormat	[in]	メッセージ
	 * @param	...			[in]	必要に応じて
	 */
	void ObjectError( _IRowPtr ipRow, LPCTSTR lpcszFormat, ... );
}
