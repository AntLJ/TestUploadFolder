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

// AddressCheck.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/flush_scope_guard.h>
#include "CheckAddress.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// 強制フラッシュ
	uh::flush_scope_guard fsg;

	const arctl::coinitializer aCoInitializer;

	try
	{
		CheckAddress checkAddress;
		// 初期化 && チェック開始
		if( checkAddress.init(argc, argv) &&  checkAddress.run())
		{
			// 正常終了
			cerr << "正常終了" << endl;
			return 0;
		}	
	}

	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 1;
	}
}

