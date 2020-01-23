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

namespace
{
/**
 * @brief  必須オプションが存在するか確認
 * @param  optName [in] オプション名
 * @param  optVal  [in] オプション値
 * @retval true  必須オプションが設定されている
 * @retval false 必須オプションが設定されていない
 */
inline bool checkParam( const char* const optName, const tstring& optValue)
{
	if(optValue.empty()){
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}
	return true;
}

} // namespace

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_db_road      = "db_road";
	const char * const k_db_road_past = "db_road_past";
	const char * const k_db_stop      = "db_stop";
	const char * const k_db_mesh      = "db_mesh";
	const char * const k_output       = "output";
	const char * const k_meshlist     = "meshlist";
	const char * const k_extract      = "extract";
	const char * const k_check        = "check";

	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_db_road,      tvalue<tstring>(&m_db_road),      "[必須]道路DB(チェックモードでは経年変化前の道路DB)")
			(k_db_road_past, tvalue<tstring>(&m_db_road_past), "[チェックモードで必須]経年変化前の道路DB")
			(k_db_stop,      tvalue<tstring>(&m_db_stop),      "[必須]一時停止DB")
			(k_db_mesh,      tvalue<tstring>(&m_db_mesh),      "[必須]メッシュDB")
			(k_output,       tvalue<tstring>(&m_output),       "[必須]ログファイル")
			(k_meshlist,     tvalue<tstring>(&m_meshlist),     "[必須]メッシュリスト")
			(k_extract,      "抽出モード")
			(k_check,        "チェックモード")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;

		good &= checkParam(k_db_road,  m_db_road);
		good &= checkParam(k_db_stop,  m_db_stop);
		good &= checkParam(k_db_mesh,  m_db_mesh);
		good &= checkParam(k_output,   m_output);
		if( checkParam(k_meshlist, m_meshlist) )
		{
			if(! isFile(m_meshlist.c_str())){
				cerr << toStr(m_meshlist) << "はファイルではありません" << endl;
				good = false;
			}
		}
		else
			good = false;

		// チェックモードの場合は経年変化前の道路DBも必要
		if( m_check )
			good &= checkParam(k_db_road_past, m_db_road_past);

		// 任意パラメータ
		if( vm.count(k_extract) != 0 ) { m_extract = true; }
		if( vm.count(k_check)   != 0 ) { m_check   = true; }

		if( !m_extract && !m_check ){
			cerr << k_extract << " or " << k_check << "オプションは必須です" << endl;
			good = false;
		}

		if(! good)
		{
			cerr << desc << endl;
			return false;
		}

		// 使用しないオプションが指定されている場合は警告を表示し、
		// オプションが指定されていない扱いとして、処理を続ける
		if( m_extract && !m_db_road_past.empty() )
		{
			cerr << k_db_road_past << "オブションは抽出モードで指定する必要はありません" << endl;
			cerr << k_db_road_past << "オプションの指定を無効にし、処理を続行します" << endl;
			m_db_road_past.clear();
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
