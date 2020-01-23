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

// DeleteMarkedKakei.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <WinLib/VersionInfo.h>
#include <tstring.h>
#include <boost_program_options.h>
#include "Pgdb.h"

using namespace std;
namespace po = boost::program_options;

namespace {
	CString gTargetPath;	///< ターゲットデータフォルダ
	CString gMarkPath;		///< マーキングデータフォルダ
	CString gDelWord;		///< 削除条件ワード
	CString gDate1;			///< 条件日付１
	CString gDate2;			///< 条件日付２
	CString gLogFile;		///< ログファイル
	CString gMeshList;		///< メッシュリスト
}

/** 
 * @brief 使用法表示 
 * @param[in]	App	実行時コマンドラインの一つ目 
 */ 
void PrintUsage( LPCTSTR App, po::options_description& opt)
{
	_TCHAR AppName[64]= {0};
	_tsplitpath(App, NULL, NULL, AppName, NULL);

	cout << "マーク家形削除プログラム\n"; 
	cout <<	"Usage:" << AppName << " [オプション]\n";
	cout << opt << endl;
}

/**
 * @brief コマンドライン解析。
 *
 * @param argc	[in]	メイン関数の第一引数
 * @param argv	[in]	メイン関数の第二引数
 *
 * @retval true		成功
 * @retval false	失敗、エラー
 */
bool AnalyseArg(int argc, _TCHAR* argv[])
{
	// オプションの設計
	po::options_description desc("オプション");
	desc.add_options()
		("target-path,t", uh::tvalue<uh::tstring>(),   "ターゲットデータフォルダ")
		("marking-path,m", uh::tvalue<uh::tstring>(),  "マーキングデータフォルダ")
		("keyword,w", uh::tvalue<uh::tstring>(),       "削除条件ワード")
		("date1,1", uh::tvalue<uh::tstring>(),        "条件日付１")
		("date2,2", uh::tvalue<uh::tstring>(),        "条件日付２")
		("out-path,o", uh::tvalue<uh::tstring>(),      "実行時ログファイル")
		("mesh-list,l", uh::tvalue<uh::tstring>(),     "メッシュリスト")
		("help,?",                                     "ヘルプを表示")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	//必須オプション判定
	if( vm.count("help") || !vm.count("target-path") || !vm.count("marking-path") || !vm.count("keyword") || !vm.count("out-path") || !vm.count("mesh-list")){
		PrintUsage(argv[0], desc);
		return false;
	}

	gTargetPath = vm["target-path"].as<uh::tstring>().c_str();
	gMarkPath = vm["marking-path"].as<uh::tstring>().c_str();
	gDelWord = vm["keyword"].as<uh::tstring>().c_str();
	gLogFile = vm["out-path"].as<uh::tstring>().c_str();
	gMeshList = vm["mesh-list"].as<uh::tstring>().c_str();
	if(vm.count("date1")){
		gDate1 = vm["date1"].as<uh::tstring>().c_str();
	}
	if(vm.count("date2")){
		gDate2 = vm["date2"].as<uh::tstring>().c_str();
	}
	return true;
}

/**
 * @brief 実行部本体。
 *
 */
void Execute()
{
	FILE *pFile;
	if((pFile = _tfopen(gLogFile, _T("wt"))) == NULL){
		_ftprintf(stderr, _T("ERROR,ログファイルが開けません,%s\n"), static_cast<LPCTSTR>(gLogFile));
		return;
	}
	_ftprintf(pFile, _T("#MESHXYLOG\n"));
	// ツールバージョン
	time_t st_time;
	time(&st_time);
	CVersion cVer;
	_ftprintf(pFile, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
	SYSTEMTIME  aTime;
	GetLocalTime(&aTime);
	_ftprintf(pFile, _T("#開始時刻: %04d/%02d/%02d %02d:%02d:%02d\n"),
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
	CPgdb aPgdb;
	if(aPgdb.init(gTargetPath, gMarkPath, gDelWord, gDate1, gDate2, pFile)){
		aPgdb.execDeleteKakei(gMeshList);
	}
	time_t tm;
	time(&tm);
	GetLocalTime(&aTime);
	_ftprintf(pFile, _T("#終了時刻: %04d/%02d/%02d %02d:%02d:%02d\n"),
		aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
		aTime.wMinute, aTime.wSecond);
	_tprintf(_T(" %6.0f秒\n"), difftime( tm, st_time));
	_ftprintf(pFile, _T("#トータル時間%6.0f秒\n"), difftime( tm, st_time));
	_tprintf(_T("終了します\n"));
	fclose(pFile);
	return;
}

/**
 * @brief メイン関数。
 *
 * @param argc	[in]	コマンドラインのトークン数（？）
 * @param argv	[in]	コマンドラインのトークン列（？）
 *
 * @retval 0		正常終了（実はきっちり守られてはいない）
 * @retval 0以外	異常終了
 */
int _tmain(int argc, _TCHAR* argv[])
{
	::_tsetlocale(LC_ALL, _T("Japanese"));

	try {
		const arctl::coinitializer coinit;	//認証に失敗すると落ちるからtryの内側に移動
		if(AnalyseArg(argc, argv)){
			Execute();
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
		return 1;
	}
	return 0;
}
