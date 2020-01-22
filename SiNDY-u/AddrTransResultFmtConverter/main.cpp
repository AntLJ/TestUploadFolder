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

// AddrTransResultFmtConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "AddrTransResultFmtConverter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcerr.imbue(std::locale(""));
	std::wcout.imbue(std::locale(""));
	try
	{
		AddrTransResultFmtConverter addrTransResultFmtConverter;
		// 初期化 && チェック開始
		if(addrTransResultFmtConverter.init(argc, argv) && addrTransResultFmtConverter.run())
		{
			// 正常終了
			wcout << L"正常終了" << endl;
			return 0;
		}
	}
	catch(const std::exception& e)
	{
		wcerr << e.what() << endl;
		wcerr << L"異常終了" << endl;
		return 1;
	}
	return 0;
}