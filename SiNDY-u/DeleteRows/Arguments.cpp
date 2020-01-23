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

#include "StdAfx.h"
#include "Arguments.h"
#include <iostream>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input  = "input";
	const char * const k_file   = "file";
	const char * const k_fcname = "fcname";
	const char * const k_log    = "log";

	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_input,  tvalue<tstring>(&input),  "[必須]削除対象DB")
			(k_file,   tvalue<tstring>(&file),   "[必須]削除ObjectIDリスト")
			(k_fcname, tvalue<tstring>(&fcname), "[必須]削除対象フィーチャクラス名")
			(k_log,    tvalue<tstring>(&log),    "[必須]ログ")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		if(input.empty()){
			cerr << k_input << "オプションは必須です" << endl;
			good = false;
		}

		if(file.empty()){
			cerr << k_file << "オプションは必須です" << endl;
			good = false;
		}

		if(fcname.empty()){
			cerr << k_fcname << "オプションは必須です" << endl;
			good = false;
		}

		if(! good)
		{
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}
