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
	const char * const k_rail_nw_db         = "rail_nw_db";
	const char * const k_rail_poi_db        = "rail_poi_db";
	const char * const k_mesh_db            = "mesh_db";
	const char * const k_station_db         = "station_db";
	const char * const k_output             = "output";
//	const char * const k_output_mesh        = "output_mesh";
//	const char * const k_output_query       = "output_query";
	const char * const k_meshlist           = "meshlist";
	const char * const k_dist_threshold     = "dist_threshold";
	const char * const k_neighbor_threshold = "neighbor_threshold";
//	const char * const k_mesh_mode          = "mesh";
//	const char * const k_query_mode         = "query";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_rail_nw_db,         tvalue<tstring>(&m_strRailNwDB),        "[必須]鉄道NW DB接続先")
			(k_rail_poi_db,        tvalue<tstring>(&m_strRailPoiDB),       "[必須]鉄道POI DB接続先")
			(k_mesh_db,            tvalue<tstring>(&m_strMeshDB),          "[必須]メッシュDB接続先")
			(k_station_db,         tvalue<tstring>(&m_strStationDB),       "[必須]中縮駅舎DB接続先")
			(k_output,             tvalue<tstring>(&m_strLog),             "[必須]ログファイル")
//			(k_output_mesh,        tvalue<tstring>(&m_strMeshLog),         "[メッシュモード必須]メッシュモードログファイル")
			(k_meshlist,           tvalue<tstring>(&m_strMeshList),        "[必須]メッシュリスト")
			(k_dist_threshold,     tvalue<tstring>(&m_dist_threshold),     "[必須]距離閾値(m)")
			(k_neighbor_threshold, tvalue<tstring>(&m_neighbor_threshold), "[必須]近傍閾値(m)")
//			(k_output_query,       tvalue<tstring>(&m_strQueryLog),        "[クエリモード必須]クエリモードログファイル")
//			(k_mesh_mode,                                                  "[任意]メッシュモード")
//			(k_query_mode,                                                 "[任意]クエリモード")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		bool good = true;
		
//		// モードセット
//		good != SetMode( vm, k_mesh_mode, k_query_mode);	

		// パラメータのチェック
		good &= CheckParam( k_rail_nw_db,  m_strRailNwDB );
		good &= CheckParam( k_rail_poi_db, m_strRailPoiDB );
		good &= CheckParam( k_mesh_db,     m_strMeshDB );
		good &= CheckParam( k_station_db,  m_strStationDB );
		good &= CheckParam( k_output,      m_strLog );
		good &= CheckParam( k_meshlist,    m_strMeshList );
		good &= CheckParam( k_dist_threshold,     m_dist_threshold );
		good &= CheckParam( k_neighbor_threshold, m_neighbor_threshold );

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

//bool Arguments::SetMode( const po::variables_map& vm, const char* const meshMode, const char* const queryMode )
//{
//	if( vm.count( meshMode ) != 0 )
//		m_meshMode  = true;
//
//	if( vm.count( queryMode ) != 0 )
//		m_queryMode = true;
//
//	if( ! m_meshMode && ! m_queryMode ){
//		cerr << "[" << meshMode << "/" << queryMode << "]オプションはいずれか必須です" << endl;
//		return false;
//	}
//	return true;
//}
