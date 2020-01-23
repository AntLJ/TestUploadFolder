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

// CompactPGDB.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>

/**
 * @fn		checkFileAttr
 * @brief	ファイル属性調べる
 * @param	lpszFile	[in]	パス
 * @retval	1 : ファイル
 * @retval	2 : ディレクトリ
 * @retval	0 : 無効パス
 */
int checkFileAttr( LPCTSTR lpszFile )
{
	DWORD dwVal = GetFileAttributes( lpszFile );
	if( -1 == dwVal )
		return 0;

	if( FILE_ATTRIBUTE_DIRECTORY == dwVal )
		return 2;

	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcerr.imbue(std::locale("japanese"));
	std::wcout.imbue(std::locale("japanese"));

	if( 1 == argc )
		CApp::printUsage();

	try
	{
		arctl::coinitializer coinit;
		CApp cApp;

		// 引数が１つならドラッグアンドドロップと判断
		if( 2 == argc ){

			if( 1 == checkFileAttr(argv[1]) )
				cApp.compact(argv[1]);

		}
		else{
			if( !cApp.init(argc, argv) || !cApp.execute() )
				return 2;
		}
	}
	catch( std::exception& e )
	{
		std::wcerr << CA2W(e.what()) << std::endl;
	}

	std::wcerr << L"正常終了" << std::endl;

	return 0;
}

