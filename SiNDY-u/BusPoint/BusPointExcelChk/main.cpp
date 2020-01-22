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
#include <atl2/io.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "Util.h"
#include "BusPointExcelChk.h"

namespace po = boost::program_options;

/**
 * @brief 必要なオプション情報があるかをチェックする
 * @param vm [in] 引数のmap
 * @param option [in] オプション名称
 * @param desctiption [in] オプションの記述
 * @retval true 必要なオプションあり
 * @retval false 必要なオプションなし
 */
bool check_required_option(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption);


int _tmain(int argc, _TCHAR* argv[])
{
	try{
		::CoInitialize(NULL);

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		po::options_description desc("Allowed options");

		const char * const k_in_list		= "in_list";
		const char * const k_check_log		= "check_log";

		Arguments	Args;
		desc.add_options()
			(k_in_list,			uh::tvalue<uh::tstring>(&Args.in_list),			"[必須]調査結果ファイルリスト")
			(k_check_log,		uh::tvalue<uh::tstring>(&Args.check_log),		"[必須]チェックログファイル")
			;

		if(argc == 1) {
			std::cerr << desc << std::endl;
			return 1;
		}

		// argc, argv を解析して、結果をvmに格納
		po::variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		// 必須パラメータチェック
		bool	bMust = true;

		bMust &= check_required_option( vm, k_in_list,		"[必須]調査結果ファイルリスト");
		bMust &= check_required_option( vm, k_check_log,	"[必須]チェックログファイル");

		if( !bMust){
			std::cerr << "バスデータExcelチェックツール" << std::endl;
			std::cerr << desc << std::endl;
			return 1;
		}

		// 実処理
		bool bRet = false;

		CBusPointExcelChk app( Args );
		if( app.execute() ){
			bRet = true;
		}
		else{
			bRet = false;
		}

		::CoUninitialize();

		// 処理後の終了判定
		if( bRet ){
			// 正常終了
			std::cerr << "正常終了" << std::endl;
			return 0;
		}
		else{
			// 異常終了
			std::cerr << "異常終了" << std::endl;
			return 1;
		}
	}
	catch(const _com_error& e) {
		::CoUninitialize();
		std::cerr << atl2::stringize(e) << std::endl;
		std::cerr << "異常終了" << std::endl;
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "異常終了" << std::endl;
	}

	return 1;
}

// 必要なオプション情報があるかをチェックする
bool check_required_option(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption)
{
	if( vm.count(option) == 0 ){
		std::cerr << desctiption << "が未設定です。" << std::endl;
		return false;
	}
	return true;
}


