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
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/text_util.h>
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;
using namespace std;
using namespace uh;

CArguments::CArguments(void)
{
}


CArguments::~CArguments(void)
{
}

bool checkNecessaryOption( const tstring& argument, const char* option )
{
	if( argument.empty() )
	{
		cerr << option << "オプションは必須です。" << endl;
		return false;
	}
	return true;
}

bool CArguments::parse( int argc, _TCHAR* argv[] )
{
	const char * const poi_db =          "poi_db";
	const char * const addr_db =         "addr_db";
	const char * const map_db =          "map_db";
	const char * const poi_fc =          "poi_fc";
	const char * const poi_sub_fc =      "poi_sub_fc";
	const char * const admin_fc =        "admin_fc";
	const char * const building_fc =     "building_fc";
	const char * const contents_master = "contents_master";
	const char * const list =            "list";
	const char * const out =             "out";
	const char * const out_sub =         "out_sub";
	const char * const rsp_idn =         "rsp_idn";

	po::options_description desc("[電話帳マッチング用POIデータ出力ツール]");
	try
	{
		desc.add_options()
			(poi_db,             tvalue<tstring>(&m_poiDB),        "[必須]POIデータSDE接続先")
			(addr_db,            tvalue<tstring>(&m_addrDB),       "[必須]住所データSDE接続先")
			(map_db,             tvalue<tstring>(&m_mapDB),        "[必須]地図データSDE接続先")
			(poi_fc,             tvalue<tstring>(&m_poiPoint),     "[必須]POI_POINTフィーチャクラス名")
			(poi_sub_fc,         tvalue<tstring>(&m_poiSubPoint),  "[必須]POI_SUB_POINTフィーチャクラス名")
			(admin_fc,           tvalue<tstring>(&m_cityAdmin),    "[必須]CITY_ADMINフィーチャクラス名")
			(building_fc,        tvalue<tstring>(&m_building),     "[必須]BUILDINGフィーチャクラス名")
			(contents_master,    tvalue<tstring>(&m_contents),     "[必須]POI_CONTENTS_MASTERテーブル名")
			(list,               tvalue<tstring>(&m_listPath),     "[必須]出力対象種別表リスト")
			(out,                tvalue<tstring>(&m_outFile),      "[必須]出力ファイル（POIポイントファイル）名")
			(out_sub,            tvalue<tstring>(&m_outFileSub),   "[必須]出力ファイル（ルート探索用ポイントファイル）名")
			(rsp_idn,            tvalue<tstring>(&m_idnCode),      "[必須]対象IDNCODE")
			;
	
		po::variables_map vm;
		po::store( po::parse_command_line(argc, argv, desc), vm );
		po::notify( vm );

		// 必須パラメータのチェック
		bool good = true;
		good &= checkNecessaryOption( m_poiDB,     poi_db );
		good &= checkNecessaryOption( m_addrDB,    addr_db );
		good &= checkNecessaryOption( m_mapDB,     map_db );
		good &= checkNecessaryOption( m_poiPoint,  poi_fc );
		good &= checkNecessaryOption( m_poiSubPoint, poi_sub_fc );
		good &= checkNecessaryOption( m_cityAdmin, admin_fc );
		good &= checkNecessaryOption( m_building,  building_fc );
		good &= checkNecessaryOption( m_contents,  contents_master );
		good &= checkNecessaryOption( m_listPath,  list );
		good &= checkNecessaryOption( m_outFile,   out );
		good &= checkNecessaryOption( m_outFileSub,   out_sub );
		good &= checkNecessaryOption( m_idnCode,   rsp_idn );

		if( !good )
		{
			cerr << desc << endl;
			cerr << "※標準出力に実行ログが出力されます。" << endl;
			return false;
		}
	}
	catch( const std::exception& e )
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	
	return true;
}
