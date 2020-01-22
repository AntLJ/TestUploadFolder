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

#include "stdafx.h"
#include "AutoSnap.h"

#include <atl2/coinitializer.h>
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>

#include "Config.h"
#include "AutoSnap.h"

using namespace std;

//Show the usage information when the command line parameter is not inputed.
void Usage(const wchar_t* strApp)
{
	wcerr << L"コマンドライン引数に問題があります" << std::endl;
	wcerr << strApp << L" <配置ファイル>" << std::endl;
}

//Force the entry point to UNICODE version, Only for VC compiler.
int wmain(int argc, wchar_t* argv[])
{
	// ロケール関連の不具合防止
	uh::setJapaneseCharacterTypeFacet();

	if(argc <=1)
	{
		Usage(argv[0]);
		return 1;
	}
	try
	{
		const arctl::coinitializer aCoInitializer;
		try
		{
			AutoSnapConfig cfg;
			if(!cfg.Load(argv[1]))
			{
				wcerr << L"配置ファイルエラー " << argv[1] << endl;
				return 1;
			}

			// 実処理
			CAutoSnap app(cfg)/*( args )*/;
			if( app.execute() ){
				// 正常終了
				wcerr << L"正常終了" << endl;
				return 0;
			}
			else{
				// 異常終了
				wcerr << L"異常終了" << endl;
				return 1;
			}
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		wcerr << L"異常終了" << endl;
		return 2;
	}
}
