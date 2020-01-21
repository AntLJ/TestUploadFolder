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
 * @file GlobalFiles.h
 * @brief <b>グローバルファイル変数定義ファイル</b>\n
 * DLL内でグローバルに使用する変数を定義します。
 * データセグメントで定義しているわけではないのでここで
 * 定義された変数の内容は全てプロセス別になります。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _GLOBALFILES_H_
#define _GLOBALFILES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

namespace sindy {
	// エラーログ用
	extern SINDYLIB_API TCHAR	g_strErrlog[];		//!< エラーログファイル名
	extern SINDYLIB_API FILE*	g_Errlog;			//!< エラーログファイルハンドル
	// トレースログ用
	extern SINDYLIB_API TCHAR g_strTracelog[];		//!< トレースログファイル名
	extern SINDYLIB_API FILE* g_Tracelog;			//!< トレースログファイルハンドル
} // sindy

#endif // _GLOBALFILES_H_
