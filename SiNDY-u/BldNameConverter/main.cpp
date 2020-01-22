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

// BldNameConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <comdef.h>
#include <shlwapi.h>

#include "stdafx.h"
#include "LogSys.h"
#include "common.h"

using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;

extern bool exec(const po::variables_map &vmContent, bool isTest);

bool option_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (vmMap.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "が不正です" << std::endl;
		return false;
	}
	return true;
}

bool file_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (! PathFileExists(vmMap[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "が不正です" << std::endl;
		return false;
	}
	return true;
}

bool dir_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (! PathIsDirectory(vmMap[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "が不正です" << std::endl;
		return false;
	}
	return true;
}

bool ini_chk(const po::variables_map &vmContent)
{
	bool ok = true;
	// DB関連は値が入っているかのチェック・リスト・ディレクトリ関連はファイルの存在チェック・
	ok &= option_exist_chk(vmContent, BASE_DB);
	ok &= option_exist_chk(vmContent, MAP_DB);
	ok &= option_exist_chk(vmContent, MESH_DB);
	ok &= file_exist_chk(vmContent, BLD_EXC_LIST);
	ok &= file_exist_chk(vmContent, MESH_LIST);
	ok &= dir_exist_chk(vmContent, OUTPUT_DIR);
	return ok;
}

bool write_usage()
{
	std::cout << "How to set ini_file" << std::endl;
	std::cout << "  " << BASE_DB << " = <ベースDB>" << std::endl;
	std::cout << "  " << MAP_DB << " = <地図DB>" << std::endl;
	std::cout << "  " << MESH_DB << " = <メッシュDB>" << std::endl;
	std::cout << "  " << BLD_EXC_LIST << " = <処理対象外建物種別リスト>" << std::endl;
	std::cout << "  " << MESH_LIST << " = <変換対象メッシュリスト>" << std::endl;
	std::cout << "  " << OUTPUT_DIR << " = <出力ディレクトリ>" << std::endl;
	return false;
}

bool arg_check(int argc, _TCHAR* argv[], po::variables_map *vmContent, bool *isTest)
{
	// ロケール関連の不具合防止	
	uh::setJapaneseCharacterTypeFacet();
	// 引数読み込み
	uh::tstring iniFile, procLog, errLog;
	po::options_description descPath("How to user");
	descPath.add_options()
		(INI, uh::tvalue<uh::tstring>(&iniFile), "設定ファイル(必須)")
		(PROCLOG, uh::tvalue<uh::tstring>(&procLog), "実行ログ(任意)")
		(ERRLOG, uh::tvalue<uh::tstring>(&errLog), "エラーログ(任意)")
		(TEST, "検証用モード(形状付きでの出力)");
	if (1 == argc) {
		std::cout << descPath << std::endl;
		return write_usage();
	}

	po::variables_map vmPath;
	po::store(po::parse_command_line(argc, argv, descPath), vmPath);
	po::notify(vmPath);

	// 設定ファイルパスチェック
	if (! PathFileExists(iniFile.c_str())) {
		std::cerr << "ERROR\t" << INI << "オプションが不正です" << std::endl;
		return false;
	}
	// 実行ログ・エラーログのチェック
	if (vmPath.count(PROCLOG) != 0) {
		if (! LogSys::GetInstance().SetProcLog(procLog.c_str())) {
			std::cerr << "ERROR\t" << PROCLOG << "オプションが不正です" << std::endl;
			return false;
		}
	}
	if (vmPath.count(ERRLOG) != 0) {
		if (! LogSys::GetInstance().SetErrLog(errLog.c_str())) {
			std::cerr << "ERROR\t" << ERRLOG << "オプションが不正です" << std::endl;
			return false;
		}
		// SiNDYFreeStyleLog用のヘッダ記入
		LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
		LogSys::GetInstance().WriteErrLog(false, false, _T("FLAG\tLAYER\tOBJECTID\tエラーレベル\tエラーメッセージ\n"));
	}
	// モードのチェック
	if (vmPath.count(TEST) != 0)
		*isTest = true;

	// 設定ファイル読み込み
	po::options_description descContent("How to set a ini file");
	descContent.add_options()
		(BASE_DB, uh::tvalue<uh::tstring>(), "ベースDB")
		(MAP_DB, uh::tvalue<uh::tstring>(), "地図DB")
		(MESH_DB, uh::tvalue<uh::tstring>(), "メッシュDB")
		(BLD_EXC_LIST, uh::tvalue<uh::tstring>(), "処理対象外建物種別リスト")
		(MESH_LIST, uh::tvalue<uh::tstring>(), "変換対象メッシュリスト")
		(OUTPUT_DIR, uh::tvalue<uh::tstring>(), "出力ディレクトリ");

	std::ifstream ifsIniFile(CT2CA(iniFile.c_str()));
	if (! ifsIniFile.is_open()) {
		std::cerr << "ERROR\t設定ファイルのオープンに失敗しました" << std::endl;
		return false;
	}
	po::store(po::parse_config_file(ifsIniFile, descContent), *vmContent);
	po::notify(*vmContent);

	if (! ini_chk(*vmContent))
		return write_usage();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;
		// 引数取得&チェック
		po::variables_map vmContent;
		bool isTest = false;
		if (! arg_check(argc, argv, &vmContent, &isTest)) {
			retval = 1;
		} else {
			// 処理実行
			LogSys::GetInstance().WriteProcLog(true, true, _T("処理開始\n"));
			if (! exec(vmContent, isTest))
				retval = 1;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
	}
	if (retval == 0) {
		LogSys::GetInstance().WriteProcLog(true, true, _T("正常終了\n"));
	} else {
		LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
	}

	std::cout << std::flush;
	std::cerr << std::flush;

	return retval;
}
