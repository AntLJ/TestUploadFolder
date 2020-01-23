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

// CreateNoParkingRoadList.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ConnectServer.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		arctl::coinitializer aoinit;
		if( argc != 2 )
		{
			std::cerr << "Usage    　：CreateNoParkingRoadList.exe [config.txt]" << std::endl;
			std::cerr << "[config.txt] ：設定ファイル" << std::endl;
			return 0;
		}

		CConnectServer aConnect;
		CString aConf = argv[1];
		if( !aConnect.run(aConf) )
			std::cerr << "異常終了致しました。" << std::endl;
		else
			std::cerr << "正常終了致しました。" << std::endl;
	}
	// 例外処理
#ifdef _UNICODE
	catch(const std::exception& e) {
		std::wcout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::wcout << e.ErrorMessage() << std::endl;
	}
#else
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::cout << e.ErrorMessage() << std::endl;
	}
#endif

	std::cout << std::flush;
	std::cerr << std::flush;

	return 0;
}

