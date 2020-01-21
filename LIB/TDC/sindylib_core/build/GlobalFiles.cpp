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
 * @file グローバル変数実体ファイル
 * @brief DLL内でグローバルに使用する変数の実体を定義します。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "GlobalFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
	// エラーログファイル用
	TCHAR g_strErrlog[_MAX_PATH] = _T("C:\\Temp\\error.log");	//!< エラーログファイル名
	FILE* g_Errlog = NULL;										//!< エラーログファイルハンドル

	// トレースログ用
	TCHAR g_strTracelog[_MAX_PATH] = _T("C:\\Temp\\trace.log");	//!< トレースログファイル名
	FILE* g_Tracelog = NULL;									//!< トレースログファイルハンドル
} // sindy
