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

using namespace std;
using namespace uh;

namespace option{
	const std::string kTargetDb       = "target_db";
	const std::string kTargetTable    = "target_tbl";
	const std::string kBuildingDB     = "map_db";
	const std::string kAddrDB         = "addr_db";
	const std::string kXyUser         = "xy_user";
	const std::string kReleaseUser    = "release_user";  
	const std::string kTargetList     = "target_src_list";
	const std::string kRunLog         = "run_log";
	const std::string kErrLog         = "err_log";
} // namespace arg

CArgInfo::CArgInfo( const std::string& arg_, const std::string& desc_, bool required_/*= false*/, bool flag_/*= false*/, const std::string& default_ /*=""*/ ):
	arg( arg_ ),
	desc( desc_ ),
	required( required_ ),
	flag( flag_ ),
	def_val( default_)
{
}

void CArgInfos::add(const CArgInfo& arg) 
{
	m_argInfos.push_back(arg); 
}

bool CArgInfos::parse( int argc, _TCHAR* argv[] )
{
	namespace po = boost::program_options;

	po::options_description desc("How to use");
	try{
		for( const auto& arg : m_argInfos )
		{
			if( arg.flag )
			{
				desc.add_options()
					( arg.arg.c_str(), arg.desc.c_str());
			}
			else
			{
				if( arg.def_val.empty() )
				{
					desc.add_options()
						( arg.arg.c_str(), po::value<string>(), arg.desc.c_str());
				}
				else
				{
					desc.add_options()
						( arg.arg.c_str(), po::value<string>()->default_value( arg.def_val ), arg.desc.c_str());
				}
			}
		}

		po::store(po::parse_command_line(argc, argv, desc), m_args);
		po::notify(m_args);
	}
	catch(const std::exception& e)
	{
		// 定義されていないオプション（--xxx）が指定された
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}

	return true;
}

CString CArgInfos::getValue( const std::string& arg ) const
{
	CString value;

	if( !hasOption(arg) )
		return value;

	try{
		value = m_args[arg].as<string>().c_str();
	}
	catch(const std::exception& e){
		// m_argsに格納されていない
		cerr << "bad argument: " << arg << endl;
		throw;
	}

	return value;
}

bool CArgInfos::hasOption( const std::string& arg ) const
{
	return m_args.count( arg ) > 0;
}

bool CArgInfos::chekArg() const 
{
	bool chk = true;
	// [必須]チェック
	for( const auto& arg : m_argInfos )
	{
		if( arg.required && m_args.count(arg.arg.c_str()) == 0 )
		{
			cerr << "required option: --" << arg.arg << endl;
			chk = false;
		}
	}
	return chk;
}

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	//m_args.add( CArgInfo( option::kTargetDb, "[必須]更新先DB", true, false, "BLDGNAME201510/BLDGNAME201510/ETC_BLDNGNAME_AA_TEST/5151/coral2" ) );
	m_args.add( CArgInfo( option::kTargetTable, "[必須]更新先DB", true, false, "BUILDINGNAME_SRC_POINT" ) );
	m_args.add( CArgInfo( option::kTargetDb, "[必須]更新先DB", true, false, "C:\\PGDB\\BldName.mdb" ) );
	m_args.add( CArgInfo( option::kBuildingDB, "[必須]参照用家形DB", true, false, "TRIAL/TRIAL/SDE.DEFAULT/5151/coral2" ) );
	m_args.add( CArgInfo( option::kAddrDB, "[必須]住所DB（参照のみ）", true, false, "proteus" ) );
	m_args.add( CArgInfo( option::kXyUser, "[必須]住所DBのXY付与用ユーザ", true, false, "xy1510_all" ) );
	m_args.add( CArgInfo( option::kReleaseUser, "[必須]住所DBのリリース作業用ユーザ", true, false, "y1511" ) );
	//m_args.add( CArgInfo( option::kTargetList, "[任意]処理対象条件指定リスト", true, false, "\\\\win\\tdc\\ced-ref\\verification\\SiNDY-u\\map\\BldNameAddrAnalyzer\\SiNDY_u_10324_住所解析失敗\\condition.csv" ) );
	m_args.add( CArgInfo( option::kTargetList, "[任意]処理対象条件指定リスト", false, false, "c:\\tmp\\condition02.csv" ) );
	m_args.add( CArgInfo( option::kRunLog, "[必須]実行ログファイル", true, false, "C:\\tmp\\run.log" ) );
	m_args.add( CArgInfo( option::kErrLog, "[必須]エラーログファイル", true, false, "C:\\tmp\\err.log" ) );
	// TODO: デバッグでフラグオプションを指定する場合はコマンド引数で指定すること
	//m_args.add( CArgInfo( arg::kTestMode, "[任意]テストモード", false, true ) );
#else
	m_args.add( CArgInfo( option::kTargetDb, "[必須]更新先DB", true, false ) );
	m_args.add( CArgInfo( option::kTargetTable, "[必須]更新対象テーブル", true, false ) );
	m_args.add( CArgInfo( option::kBuildingDB, "[必須]参照用家形DB", true, false ) );
	m_args.add( CArgInfo( option::kAddrDB, "[必須]住所DB（参照のみ）", true, false ) );
	m_args.add( CArgInfo( option::kXyUser, "[必須]住所DBのXY付与用ユーザ", true, false ) );
	m_args.add( CArgInfo( option::kReleaseUser, "[必須]住所DBのリリース作業用ユーザ", true, false ) );
	m_args.add( CArgInfo( option::kTargetList, "[任意]処理対象条件指定リスト", true, false ) );
	m_args.add( CArgInfo( option::kRunLog, "[必須]実行ログファイル", true, false ) );
	m_args.add( CArgInfo( option::kErrLog, "[必須]エラーログファイル", true, false ) );
#endif

	if( !m_args.parse( argc, argv ) )
		return false;
	
	if( !m_args.chekArg() )
		return false;

	return true;
}


CString Arguments::description() const
{
	CString desc = _T("# Options:\n");
	for( const auto& arg : m_args )
	{
		// 指定されていなければ出さない
		if( !hasOption( arg.arg ) )
			continue;

		desc.AppendFormat( _T("#  --%s: %s\n"), CString(arg.arg.c_str()), getValue( arg.arg ) );
	}
	desc += _T("\n");

	return desc;
}
