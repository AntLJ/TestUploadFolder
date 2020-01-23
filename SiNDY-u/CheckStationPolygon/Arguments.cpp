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

// useful headers
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;


// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_db       = "db";
	const char * const k_log_dir  = "log_dir";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_db,       tvalue<tstring>(&m_db),      "[必須]DB接続情報")
			(k_log_dir,  tvalue<tstring>(&m_log_dir), "[必須]ログ出力先フォルダ")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( k_db,  m_db, false );
		good &= CheckParam( k_log_dir, m_log_dir, true );

		if( ! good ){
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e){
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}

	return true;
}

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bDir /* = false */ )
{
	if( optValue.empty() ){
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	if( bDir ){
		if( ! isDirectory( optValue.c_str() )){
			cerr << toStr( optValue ) << "はフォルダではありません" << endl;
			return false;
		}
	}

	return true;
}
