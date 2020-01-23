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
#include "NodelistCreator.h"
#include "Defs.h"

// useful_headers
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>


namespace po = boost::program_options;


// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input_db      = "input_db";
	const char * const k_name_rule_ini = "name_rule_ini";
	const char * const k_output_ini    = "output_ini";
	const char * const k_output_dir    = "output_dir";
	const char * const k_runlog        = "runlog";
	const char * const k_errlog        = "errlog";
	const char * const k_latlon        = "latlon";
	const char * const k_road_db       = "road_db";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_input_db,       uh::tvalue<uh::tstring>(&m_input_db),        "[必須] 接続先DB")
			(k_name_rule_ini,  uh::tvalue<uh::tstring>(&m_name_rule_ini),   "[必須] イラスト名ルール設定ファイルパス")
			(k_output_ini,     uh::tvalue<uh::tstring>(&m_output_ini),      "[必須] イラスト出力情報設定ファイルパス")
			(k_output_dir,     uh::tvalue<uh::tstring>(&m_output_dir),      "[必須] 出力フォルダパス")
			(k_runlog,         uh::tvalue<uh::tstring>(&m_runlog),          "[必須] 処理ログ")
			(k_errlog,         uh::tvalue<uh::tstring>(&m_errlog),          "[必須] エラーログ")
			(k_latlon,         uh::tvalue<uh::tstring>(&m_latlon),          "[任意] 緯度経度出力指定")
			(k_road_db,        uh::tvalue<uh::tstring>(&m_road_db),         "[任意] 接続先DB(道路)")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// パラメータのチェック
		bool good = true;
		good &= CheckParam( k_input_db,      m_input_db );
		good &= CheckParam( k_name_rule_ini, m_name_rule_ini, kIsFile );
		good &= CheckParam( k_output_ini,    m_output_ini   , kIsFile );
		good &= CheckParam( k_output_dir,    m_output_dir   , kIsFolder );
		good &= CheckParam( k_runlog,        m_runlog );
		good &= CheckParam( k_errlog,        m_errlog );
		good &= CheckLatLonParam( m_latlon );

		if( ! good ){
			std::cerr << desc << std::endl;
			return false;
		}
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}


// コマンドライン引数の汎用チェック
bool Arguments::CheckParam( const char* const optName, const uh::tstring& optValue, ChkMode eChkMode /*= kNone */ ) const
{
	if( optValue.empty() ){	
		std::cerr << optName << "オプションは必須です" << std::endl;
		return false;
	}

	if( eChkMode ==  kIsFile ){
		if( ! uh::isFile( optValue.c_str() )){
			std::cerr << uh::toStr( optValue ) << "はファイルではありません" << std::endl;
			return false;
		}
	}

	if( eChkMode == kIsFolder ){
		if( ! uh::isDirectory( optValue.c_str() )){
			std::cerr << uh::toStr( optValue ) << "はフォルダではありません" << std::endl;
			return false;
		}
	}

	return true;
}


// latlonオプションのチェック
bool Arguments::CheckLatLonParam( const uh::tstring& strOptValue ) const
{
	if( strOptValue.empty() )
		return true;

	if( strOptValue != latlon::DMS && strOptValue != latlon::DEG )
	{
		std::cerr << "latlonオプションの指定が不正です : " << uh::toStr( strOptValue ) << std::endl;
		return false;
	}
	return true;
}
