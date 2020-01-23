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
	const char * const k_road_db           = "road_db";
	const char * const k_adam_db           = "adam_db";
	const char * const k_diff_db           = "diff_db";
	const char * const k_target_road_where = "target_road_where";
	const char * const k_mesh_list         = "mesh_list";
	const char * const k_log_dir           = "log_dir";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_road_db,           tvalue<tstring>(&m_RoadDB),           "[必須]接続先DB(道路DB)")
			(k_adam_db,           tvalue<tstring>(&m_AdamDB),           "[必須]接続先DB(ADAM DB)")
			(k_diff_db,           tvalue<tstring>(&m_DiffDB),           "[任意]道路リンクまたは車線リンクの形状差分FGDB")
			(k_target_road_where, tvalue<tstring>(&m_TargetRoadWhere),  "[任意]紐付け対象道路リンク検索条件")
			(k_mesh_list,         tvalue<tstring>(&m_MeshListStr),      "[必須]メッシュコードまたはメッシュリストファイルパス")
			(k_log_dir,           tvalue<tstring>(&m_LogDir),           "[必須]ログ出力フォルダパス")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( k_road_db,   m_RoadDB );
		good &= CheckParam( k_adam_db,   m_AdamDB );
		good &= CheckParam( k_mesh_list, m_MeshListStr );
		good &= CheckParam( k_log_dir,   m_LogDir );

		// メッシュコード読み込み
		bool isMeshErr(false);
		for( long meshcode : uh::text_util::ToLongList( m_MeshListStr ) ){
			if( meshcode < 100000  || 1000000 <= meshcode ) {
				std::cerr << "メッシュコードの指定が不正です(ベースメッシュのみ指定可) : " << std::to_string(meshcode) << std::endl;
				isMeshErr = true;
				continue;
			}
			m_MeshList.push_back( meshcode );
		}
		if( isMeshErr )
			return false;

		if ( m_MeshList.empty() ){
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

bool Arguments::CheckParam( const char* const opt_name, const uh::tstring& opt_value )
{
	if( opt_value.empty() ){	
		cerr << opt_name << "オプションは必須です" << endl;
		return false;
	}
	return true;
}
