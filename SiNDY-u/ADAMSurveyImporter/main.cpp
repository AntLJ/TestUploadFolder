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

#include <time.h>

#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <WinLib/VersionInfo.h>

#include "LogUtil.h"
#include "Arguments.h"
#include "ADAMSurveyImporter.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		clock_t start = clock(); 
		setlocale(LC_ALL,"japanese");

		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		LogUtil::printLine();
		LogUtil::print("   ADAM v2 インポートツール");
		CVersion cVer;
		LogUtil::print((uh::tformat(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s")) % cVer.GetOriginalFilename().GetString() % cVer.GetFileVersion().GetString() % cVer.GetProductVersion().GetString()).str());

		try
		{
			// コマンドライン引数処理
			Arguments args;
			if(! args.parse(argc, argv))
			{
				LogUtil::printError("コマンドライン引数に問題があります");
				return 1;
			}

			// 実処理
			int ret = 1;
			ADAMSurveyImporter app( args );
			if( app.execute() )
			{
				// 正常終了
				LogUtil::printLine();
				LogUtil::print("正常終了");
				ret = 0;
			}
			else
			{
				// 異常終了
				LogUtil::printLine();
				LogUtil::printError("異常終了\tログを確認してください");
				ret = 1;
			}
			clock_t end = clock();
			std::string msg = "[処理時間] " + std::to_string((double)(end - start) / CLOCKS_PER_SEC / 60) + "min.\n";
			LogUtil::print(msg);
			LogUtil::write();
			return ret;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		LogUtil::printLine();
		LogUtil::printError("異常終了しました");
		LogUtil::printError("例外検出\t" +  uh::toStr(e.what()));
		LogUtil::write();
		return 2;
	}
}
