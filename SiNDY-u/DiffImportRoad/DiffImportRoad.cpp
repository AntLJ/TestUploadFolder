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

// DiffImportRoad.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/boost_program_options.h>
#include "Reflect.h"
#include "TimeLog.h"

namespace {

	CString gConnect[2];	///< サーバ接続文字列

	CString gMeshList;		///< メッシュリスト
	CString gOldPath;		///< 編集前PGDBベースパス
	CString gNewPath;		///< 編集後PGDBベース
	CString gWorkVersion;	///< 作業用子バージョン
	long gMode = 0;			///< 動作モード（更新の有無・優先）
}

using namespace std;
namespace po = boost::program_options;

/** 
 * @brief 使用法表示 
 * @param[in]	App	実行時コマンドラインの一つ目 
 */ 
void PrintUsage(LPCTSTR App, po::options_description& opt)
{
	_TCHAR AppName[64]= {0};
	_tsplitpath(App, NULL, NULL, AppName, NULL);

	cout << "道路用PGDB差分取り込みプログラム\n"; 
	cout <<	"Usage:" << AppName << " [オプション]\n";
	cout << opt << endl;
}

/** 
 * @brief 使用法表示 （ちょっとだけ詳しく）
 * @param[in]	App	実行時コマンドラインの一つ目 
 */ 
void PrintHelp(LPCTSTR App, po::options_description& opt)
{
	PrintUsage(App, opt);
	_tprintf(_T("編集前・編集後のPGDBの差分をSiNDYに反映させるプログラムです。\n")
			 _T("設定ファイルの書き方は、DiffImportRoad.txtをご覧下さい。\n")
			);
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
		("old-path,o", uh::tvalue<uh::tstring>(),    "編集前PGDBベースディレクトリ（必須）")
		("new-path,n", uh::tvalue<uh::tstring>(),    "編集後PGDBベースディレクトリ（必須）")
		("mesh-list,m", uh::tvalue<uh::tstring>(),   "メッシュリスト（必須）")
		("test-mode,t",                              "テストモード（変更なし）")
		("japan-mode,j",                             "国内作業優先モード")
		("help,?",                                   "ヘルプを表示")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	//ヘルプ表示
	if( vm.count("help")){
		PrintHelp(argv[0], desc);
		return false;
	}

	//必須オプション判定
	if(!vm.count("old-path") || !vm.count("new-path") || !vm.count("mesh-list")){
		PrintUsage(argv[0], desc);
		return false;
	}

	gOldPath = vm["old-path"].as<uh::tstring>().c_str();
	gNewPath = vm["new-path"].as<uh::tstring>().c_str();
	gMeshList = vm["mesh-list"].as<uh::tstring>().c_str();
	if(vm.count("test-mode")){
		gMode |= Reflect::NO_CHG_MODE;
	}
	if(vm.count("japan-mode")){
		gMode |= Reflect::TDC_MODE;
	}
	return true;
}

/** 
 * @brief 環境変数から接続情報の取得と作業用子バージョン名を決める
 *
 */ 
bool GetConnectInfo(void)
{
#ifdef _DEBUG
	_putenv("DB_ROAD=SINDY2013/SINDY2013/SDE.ETC_IMPORTROAD_TEST/5151/onyx");
//	_putenv("DB_ROAD=TRIAL/TRIAL/TRIAL.ETC_IMPORTROAD_TEST/5151/coral2");
	_putenv("OPERATOR=test_130805");
	_putenv("WORK_VERSION=ETC_TEST_IMPORT_130829_test");
	_putenv("FIX_USER=FIX201307");
#endif

	LPCTSTR aEnv = _tgetenv(_T("DB_ROAD"));
	if(aEnv != NULL){
		gConnect[CLayerManage::DB_ROAD] = aEnv;
	}else{
		_ftprintf(stderr, _T("環境変数「DB_ROAD」が設定されていません\n"));
		return false;
	}
#ifdef STOP_POINT
	aEnv = _tgetenv(_T("DB_STOP"));
	if(aEnv != NULL){
		gConnect[CLayerManage::DB_STOP] = aEnv;
	}else{
		_ftprintf(stderr, _T("環境変数「DB_STOP」が設定されていません\n"));
		return false;
	}
#endif
	aEnv = _tgetenv(_T("WORK_VERSION"));
	if(aEnv != NULL){
		gWorkVersion = aEnv;
	}else{
		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);
		//メンテで消えるようにしようとしたが、それでは中断機能が無駄になるので「_for_post」追加
		gWorkVersion.Format(_T("DiffImportRoad_%04d/%02d/%02d %02d:%02d:%02d_for_post"), 
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	}
	return true;
}

/** 
 * @brief 作業用子バージョンの作成又は使いまわし後、実際のバージョン名を返す
 *
 * @retval 作業用の子バージョン名（そのまま接続文字列として使用可能）
 */ 
CString CreateWorkVersion(void)
{
	// 接続
#ifdef STOP_POINT
	IVersionPtr ipCurrentVersion = CLayerManage::connectSDE( gConnect[CLayerManage::DB_STOP] );
#else
	IVersionPtr ipCurrentVersion = CLayerManage::connectSDE( gConnect[CLayerManage::DB_ROAD] );
#endif
	if(ipCurrentVersion){
		IVersionPtr ipChildVersion;
		((IVersionedWorkspacePtr)ipCurrentVersion)->FindVersion(CComBSTR(gWorkVersion), &ipChildVersion);
		if(ipChildVersion){
			_ftprintf(stderr, _T("#作業用子バージョンは既存のものを使います,%s\n"), gWorkVersion);
		}else{
			HRESULT hr = ipCurrentVersion->CreateVersion(CComBSTR(gWorkVersion),&ipChildVersion);
			if(ipChildVersion == NULL){
				_ftprintf(stderr, _T("#作業用子バージョン作成失敗,%s(hr=%d)\n"), gWorkVersion, hr);
				return _T("");
			}
		}
		return sindy::workspace_address((IWorkspacePtr)ipChildVersion).c_str();
	}
	return _T("");
}

/** 
 * @brief 実行部本体
 *
 */ 
void Execute()
{
	//子バージョンを作成(作成したら一旦切断)
	CString aWorkVersion[2];	//配列の0番目は道路データ一般用、1は一時停止用
	//最終的には接続1つで良くなる可能性があるので、とりあえずいろいろハードコーディング
#ifdef STOP_POINT
	aWorkVersion[CLayerManage::DB_ROAD] = gConnect[CLayerManage::DB_ROAD];
	aWorkVersion[1] = CreateWorkVersion();
#else
	aWorkVersion[CLayerManage::DB_ROAD] = CreateWorkVersion();
	aWorkVersion[CLayerManage::DB_STOP] = aWorkVersion[CLayerManage::DB_ROAD];	// 使わないないけど、下のエラーチェックもあるし同じものを突っ込む
#endif
	if(!aWorkVersion[CLayerManage::DB_ROAD].IsEmpty() && !aWorkVersion[CLayerManage::DB_STOP].IsEmpty()){
#ifdef STOP_POINT
		_tprintf(_T("作業バージョンは[%s]です\n"), aWorkVersion[CLayerManage::DB_STOP]);
		_ftprintf(stderr, _T("#作業バージョンは[%s]です\n"), aWorkVersion[CLayerManage::DB_STOP]);
#else
		_tprintf(_T("作業バージョンは[%s]です\n"), aWorkVersion[CLayerManage::DB_ROAD]);
		_ftprintf(stderr, _T("#作業バージョンは[%s]です\n"), aWorkVersion[CLayerManage::DB_ROAD]);
#endif
		bool aContinue = true;
		CReflect aReflect;
		if(aReflect.init_once(aWorkVersion, gOldPath, gNewPath, gMeshList, gMode)){
			while(aContinue){
				//中断できるようにループ
				if(!aReflect.init()){
					return;
				}
				aContinue = aReflect.execReflect();
				while(aContinue){
					//大文字の「Q」又は「S」以外は受け付けない（CTRL+Cは効いてしまう）
					_tprintf(_T("「S」キーで再開します。「Q」キーで終了します。\n"));
					int aKey = _getch();
					if(aKey == 'Q'){
						return; 
					}
					if(aKey == 'S'){
						break;
					}
				}
			}
		}
	}
}


/**
 * @brief メイン関数
 *
 * @param argc	[in]	コマンドラインのトークン数（？）
 * @param argv	[in]	コマンドラインのトークン列（？）
 *
 * @retval 0		正常終了（実はきっちり守られてはいない）
 * @retval 0以外	異常終了
 */
int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	const arctl::coinitializer coinit;

	//実行情報出力
	CTimeLog aTimeLog;

	try {
		if(AnalyseArg(argc, argv) && GetConnectInfo()){	//この呼ぶ順番は重要。必要な環境変数が定義されていないとUsageも表示できなくなるので
			Execute();
		}
	}
	catch(const std::exception& e) {
		printf("ERROR,%s\n", e.what());
		fprintf(stderr, "#ERROR,%s\n", e.what());
		return 1;
	}
	return 0;
}
