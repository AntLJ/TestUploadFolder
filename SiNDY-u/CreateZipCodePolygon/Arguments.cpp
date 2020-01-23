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

namespace
{
	const char * const k_hyphen = "--";
	const char * const k_cityDb = "city_db";
	const char * const k_jusyoUser = "jusyo_user";
	const char * const k_jusyoDb = "jusyo_db";
	const char * const k_outDir = "out_dir";
	const char * const k_shpName = "shape_name";
	const char * const k_tblName = "table_name";
	const char * const k_runLog = "run_log";
	const char * const k_errLog = "err_log";

	const char * const k_shp = "shp";
	const char * const k_sde = "sde";
	const char * const k_outDb = "out_db";
};

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_shp, "[選択]shpae出力")
			(k_sde, "[選択]sde出力")
			(k_cityDb, tvalue<tstring>(&m_cityDb), "[必須]都市地図行政界データ接続先")
			(k_jusyoUser, tvalue<tstring>(&m_jusyoUser), "[必須]住所マスタユーザ名")
			(k_jusyoDb, tvalue<tstring>(&m_jusyoDb), "[必須]住所DB名")
			(k_outDir, tvalue<tstring>(&m_outputDir), "[SHP必須]郵便番号ポリゴン出力先")
			(k_shpName, tvalue<tstring>(&m_shapeName), "[SHP必須]郵便番号ポリゴンファイル名")
			(k_tblName, tvalue<tstring>(&m_tableName), "[SHP必須]郵便番号データテーブル名")
			(k_outDb, tvalue<tstring>(&m_outputDb), "[SDE必須]郵便番号ポリゴン出力先")
			(k_runLog, tvalue<tstring>(&m_runLog), "[必須]実行ログ出力先")
			(k_errLog, tvalue<tstring>(&m_errLog), "[必須]エラーログ出力先")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		
		if (0 < vm.count(k_shp))
		{
			if (0 < vm.count(k_sde))
			{
				cerr << "SDEモード、SHPモードが両方指定されています。" << endl;
				cerr << desc << endl;
				return false;
			}
			m_executeMode = ExecuteMode::shp;
		}
		else if (0 < vm.count(k_sde))
		{
			m_executeMode = ExecuteMode::sde;
		}

		if (ExecuteMode::none == m_executeMode)
		{
			cerr << "SDEモード、SHPモードどちらも指定されていません。" << endl;
			cerr << desc << endl;
			return false;
		}

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam(k_cityDb, m_cityDb);
		good &= CheckParam(k_jusyoUser, m_jusyoUser);
		good &= CheckParam(k_jusyoDb, m_jusyoDb);
		good &= CheckParam(k_runLog, m_runLog);
		good &= CheckParam(k_errLog, m_errLog);

		if( !good )
		{
			cerr << desc << endl;
			return false;
		}

		// shpなら必須
		if (ExecuteMode::shp == m_executeMode)
		{
			good &= CheckParam(k_shpName, m_shapeName);
			good &= CheckParam(k_tblName, m_tableName);
			good &= CheckParam(k_outDir, m_outputDir);
			if (!good)
			{
				cerr << desc << endl;
				return false;
			}

			// shp出力先チェック
			if( !uh::isDirectory(m_outputDir.c_str()) )
			{
				cerr << m_outputDir << " : 存在しないディレクトリです" << endl;
				return false;
			}

			tstring shpPath = m_outputDir + _T("\\") + m_shapeName + _T(".shp");
			if( uh::isFile(shpPath.c_str()) )
			{
				cerr << "既に " << m_shapeName << ".shp が存在します" << endl;
				return false;
			}

			tstring tblPath = m_outputDir + _T("\\") + m_tableName + _T(".dbf");
			if( uh::isFile(tblPath.c_str()) )
			{
				cerr << "既に " << m_tableName << ".dbf が存在します" << endl;
				return false;
			}
		}

		// sdeなら必須
		if (ExecuteMode::sde == m_executeMode)
		{
			if (!CheckParam(k_outDb, m_outputDb))
			{
				cerr << desc << endl;
				return false;
			}
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

bool Arguments::CheckParam(const char* const optName, const tstring& optValue)
{
	if( optValue.empty() )
	{
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}
	return true;
}


CString Arguments::GetAllOptionInfo()
{
	stringstream ss;
	if (ExecuteMode::shp == m_executeMode)
		ss << "実行モード : shp" << endl;
	else if (ExecuteMode::sde == m_executeMode)
		ss << "実行モード : sde" << endl;

	ss << k_hyphen << k_cityDb << " : " << m_cityDb << endl;
	ss << k_hyphen << k_jusyoUser << " : " << m_jusyoUser << endl;
	ss << k_hyphen << k_jusyoDb << " : " << m_jusyoDb << endl;
	ss << k_hyphen << k_outDir << " : " << m_outputDir << endl;
	ss << k_hyphen << k_outDb << " : " << m_outputDb << endl;
	ss << k_hyphen << k_shpName << " : " << m_shapeName << endl;
	ss << k_hyphen << k_tblName << " : " << m_tableName << endl;
	ss << k_hyphen << k_runLog << " : " << m_runLog << endl;
	ss << k_hyphen << k_errLog << " : " << m_errLog << endl;

	return ss.str().c_str();
}
