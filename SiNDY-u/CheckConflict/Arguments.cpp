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
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_tgtdb    = "tgt_db,t";
	const char * const k_diffdb   = "diff_db,d";
	const char * const k_config   = "config,c";
	const char * const k_runlog   = "runlog,r";
	const char * const k_errlog   = "errlog,e";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_tgtdb,    tvalue<tstring>(&m_tgtdb),  "[必須]チェック対象接続先")
			(k_diffdb,   tvalue<tstring>(&m_diffdb), "[必須]差分DB接続先")
			(k_config,   tvalue<tstring>(&m_config), "[必須]設定ファイル")
			(k_runlog,   tvalue<tstring>(&m_runlog), "[必須]実行ファイル")
			(k_errlog,   tvalue<tstring>(&m_errlog), "[必須]エラーファイル")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( k_tgtdb,  m_tgtdb );
		good &= CheckParam( k_diffdb, m_diffdb );
		good &= CheckParam( k_config, m_config, true );
		good &= CheckParam( k_runlog, m_runlog );
		good &= CheckParam( k_errlog, m_errlog );

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

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() )){
			cerr << toStr( optValue ) << "はファイルではありません" << endl;
			return false;
		}
	}

	return true;
}
