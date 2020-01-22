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

// CharAddrCheck.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ConnectServer.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		arctl::coinitializer aoinit;
#ifdef _UNICODE
		std::wcerr.imbue(std::locale("japanese"));
#else
		//std::cerr.imbue(std::locale("japanese"));
#endif
		if( argc < 2 )
		{
			std::cerr << "Usage: CharAddrCheck.exe <arg1config.txt> [-option]\n";
			std::cerr << "<引数>\n";
			std::cerr << " <arg1>                  : config.txtへのフルパス\n";
			std::cerr << "[オプション]\n";
			std::cerr << "  [-i (--infoaddr)]      : 単語リスト出力モード（このオプションがなければ読み変換モード）\n";
			std::cerr << "  [-g (--goupoint.txt)]  : [-i]オプションを指定した場合に有効。読み変換正誤リストから単語リストを作成する。\n";
			std::cerr << "                            goupoint.txtへのフルパス\n";
			std::cerr << "  [-c (--cityadmin.txt)] : [-i]オプションを指定した場合に有効。読み変換正誤リストから単語リストを作成する。\n";
			std::cerr << "                            cityadmin.txtへのフルパス\n";
			return 0;
		}
		CString strConf = argv[1];

		CConnectServer aConnect;

		if( !aConnect.run( strConf, argc, argv ) )
			std::cerr << "異常終了致しました。" << std::endl;
		else
			std::cerr << "正常終了致しました。" << std::endl;
	}
	// 例外処理
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::cout << e.ErrorMessage() << std::endl;
	}

	return 0;
}
