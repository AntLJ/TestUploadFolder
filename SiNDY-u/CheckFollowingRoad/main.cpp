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
#include "main.h"
#include "Arguments.h"
#include "RunLogger.h"
#include "ErrLogger.h"
#include "CheckFollowingRoad.h"

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
			if(! args.parse(argc, argv))
			{
				wcerr << L"コマンドライン引数に問題があります" << endl;
				wcerr << L"異常終了" << endl;
				return 1;
			}

			// 実行ログをオープン
			CRunLogger runLog(args.m_Runlog);
			if( !runLog.open() )
			{
				wcerr << L"異常終了" << endl;
				return 1;
			}

			// エラーログをオープン
			CErrLogger errLog(args.m_Errlog);
			if( !errLog.open() )
			{
				wcerr << L"異常終了" << endl;
				return 1;
			}

			// 実行ログ・エラーログのヘッダーを出力
			runLog.header(args);
			errLog.header();

			// メッシュリストを解析
			vector<long> meshes;
			map<int, CString> mapInvalidRow;
			bool isValid = args.parseMeshes( meshes, mapInvalidRow );
			runLog.writeParseMeshResult( args.m_MeshType, mapInvalidRow );
			// メッシュリストに不正な文字が含まれていたり、メッシュが混在している場合は異常終了
			if( !isValid )
			{
				wcerr << L"メッシュリストが不正です" << endl;
				runLog.writeLogAndConsole( L"異常終了" );
				return 1;
			}

			// 処理実行
			CCheckFollowingRoad checker(args, runLog, errLog, meshes);
			runLog.writeExeTime(true);
			bool isSucceed = checker.execute();
			runLog.writeExeTime(false);
			const CString exeResult( isSucceed ? L"正常終了" : L"正常終了(エラーあり)" );
			runLog.writeLogAndConsole( exeResult );

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
		wcerr << L"異常終了" << endl;
		return 2;
	}
}
