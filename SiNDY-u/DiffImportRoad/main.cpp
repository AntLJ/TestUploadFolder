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
#include <WinLib/arcobjects_import_highmethod.h>
#include <crd_cnv.h>
#include "main.h"
#include "Arguments.h"
#include "InputInfo.h"
#include "NWData.h"
#include "DataImport.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// コマンドライン引数処理
			Arguments args;
			CInputInfo cInputInfo;
			if( !args.parse( argc, argv, cInputInfo ) )
			{
				cerr << "コマンドライン引数に問題があります" << endl;
				return 1;
			}

			// 
			CImport cImport(cInputInfo);
			if( cImport.run() )
			{
				// 正常終了
				cerr << "正常終了" << endl;
			}
			else
			{
				// 異常終了
				cerr << "異常終了" << endl;
				return 1;
			}

			return 0;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 1;
	}
}
