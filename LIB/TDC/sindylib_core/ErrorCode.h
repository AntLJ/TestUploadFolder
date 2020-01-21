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
 * @file ErrorCode.h
 * @brief SiNDYで使用されるエラーコードユーティリティ関数定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _ERRORCODE_H_
#define _ERRORCODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "WinLib/macro_util.h"

namespace sindy {

	enum sindyErrLevel {
		sindyErrLevel_INFO,			//!< 情報
		sindyErrLevel_WARN,			//!< 警告
		sindyErrLevel_ERR,			//!< エラー
	};

	namespace errorcode {
		/**
		 * @enum sindyErrCode
		 * @brief SiNDYエラーコード定義
		 */
		enum sindyErrCode {
#define ERROR_CODE(id,...) id,
#include "ErrorCodeBase.h"
#undef ERROR_CODE
		};

		/**
		 * @brief <b>sindyErrCodeを文字列に変換する</b>\n
		 * @param code	[in]	sindyErrCode
		 * @return エラー文字列
		 */
		LPCTSTR SINDYLIB_API SINDYERRCODE2STRING( sindyErrCode code );

		/**
		 * @brief <b>sindyErrCodeをエラーログに出力する</b>\n
		 * sindyErr_NoErr以外に判定されたときにこの関数を呼んで下さい。\n
		 * エラー出力専用のエラーログファイル（g_Errlog）に出力します。
		 * g_Errlogは、デフォルトでは./error.logとなります。
		 * 出力先を変えたい場合は、プログラムの先頭でg_Errlogのハンドル
		 * を自分でオープンする必要があります。
		 * \n\n
		 * エラーログ出力例：\n
		 * 
		 * 
		 * @param code			[in]	エラーコード
		 * @param lpcszFormat	[in]	出力文字列フォーマット
		 */
		void SINDYLIB_API _ERRORLOG( sindyErrCode code, LPCTSTR lpcszFormat, ...);

		/**
		 * @brief <b>sindyErrCodeとHRESULT値をエラーログに出力する</b>\n
		 * この関数はsindyErrCodeの他にHRESULT値もサポートします。\n\n
		 * sindyErr_NoErr以外に判定されたときにこの関数を呼んで下さい。\n
		 * エラー出力専用のエラーログファイル（g_Errlog）に出力します。
		 * g_Errlogは、デフォルトでは./error.logとなります。
		 * 出力先を変えたい場合は、プログラムの先頭でg_Errlogのハンドル
		 * を自分でオープンする必要があります。
		 * \n\n
		 * エラーログ出力例：\n
		 * 
		 * 
		 * @param code			[in]	エラーコード
		 * @param hr			[in]	HRESULT値
		 * @param lpcszFormat	[in]	出力文字列フォーマット
		 */
		void SINDYLIB_API _ERRORHRLOG( sindyErrCode code, HRESULT hr, LPCTSTR lpcszFormat, ...);

		/**
		 * @brief <b>sindyErrCodeとHRESULT値をエラーログに出力する</b>\n
		 * この関数は、sindyErrCodeとHRESULTをサポートします。\n
		 * \n\n
		 * エラーログ出力例：\n
		 * 
		 * 
		 * @param code			[in]			エラーコード
		 * @param lpcszFormat	[in]			出力文字列フォーマット
		 * @param args			[in,optional]	可変長引数リスト（デフォルト：NULL）
		 * @param out			[in,optional]	ファイルハンドル（デフォルト：NULL）\nNULLの場合はファイル出力しません。
		 * @param hr			[in,optional]	HRESULT値（デフォルト：S_OK）
		 */
		void SINDYLIB_API ErrorlogV( sindyErrCode code, LPCTSTR lpcszFormat, va_list args = NULL, FILE* out = NULL, HRESULT hr = S_OK );

		#define ERRORLOG(x) _ERRORLOG(x,_T("%s%s S%d: "),__FILE__LINE__,__func__,x)
		#define ERRORHRLOG(x,hr) _ERRORLOG(x,_T("%s%s S%d: HRESULT[%ld] : "),__FILE__LINE__,__func__,x,hr)
#ifdef _UNICODE
		#define ERRORLOG2(x,mes) _ERRORLOG(x,_T("%s%s S%d（%s）: "),__FILE__LINE__,__func__,x,L#mes)
		#define ERRORHRLOG2(x,hr,mes) _ERRORLOG(x,_T("%s%s S%d（%s）: HRESULT[%ld] : "),__FILE__LINE__,__func__,x,L#mes,hr)
#else
		#define ERRORLOG2(x,mes) _ERRORLOG(x,_T("%s%s S%d（%s）: "),__FILE__LINE__,__func__,x,#mes)
		#define ERRORHRLOG2(x,hr,mes) _ERRORLOG(x,_T("%s%s S%d（%s）: HRESULT[%ld] : "),__FILE__LINE__,__func__,x,#mes,hr)
#endif // ifdef _UNICODE

	} // errorcode
} // sindy

#endif // _ERRORCODE_H_
