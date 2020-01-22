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
#include <TDC/useful_headers/text_util.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_road_db   = "road_db";
	const char * const k_adam_db   = "adam_db";
	const char * const k_mesh_list = "mesh_list";
	const char * const k_log_dir   = "log_dir";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_road_db   , tvalue<tstring>(&m_roadDB)     , "[必須]接続先DB(道路DB)")
			(k_adam_db   , tvalue<tstring>(&m_adamDB)     , "[必須]接続先DB(ADAM DB)")
			(k_mesh_list , tvalue<tstring>(&m_meshListStr), "[必須]ベースメッシュコードまたはベースメッシュリストのファイルパス")
			(k_log_dir   , tvalue<tstring>(&m_logDir)     , "[必須]ログ出力フォルダパス")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);


		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam(k_road_db, m_roadDB);
		good &= CheckParam(k_adam_db, m_adamDB);
		good &= CheckParam(k_mesh_list, m_meshListStr);
		good &= CheckParam(k_log_dir, m_logDir);

		// メッシュコード読み込み
		bool isMeshErr(false);
		for (long meshcode : uh::text_util::ToLongList(m_meshListStr)) {
			if (meshcode < 100000 || 1000000 <= meshcode) {
				std::cerr << "メッシュコードの指定が不正です(ベースメッシュのみ指定可) : " << meshcode << std::endl;
				isMeshErr = true;
				continue;
			}
			m_meshList.push_back(meshcode);
		}
		if (isMeshErr)
			return false;

		if (m_meshList.empty()) {
			std::cerr << "mesh_list オプション指定が不正です。" << std::endl;
			return false;
		}

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

bool Arguments::CheckParam( const char* const optName, const tstring& optValue)
{
	if( optValue.empty() ){	
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	return true;
}
