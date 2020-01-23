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
#include "Reflect.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/text_util.h>
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;
using namespace std;
using namespace uh;

CArguments::CArguments(void)
{
	m_testMode = false;
	m_forceMode = false;
	m_tdcMode = false;
	m_exceptSdeMode = false;
	m_geospaceMode = false;
	m_onlyImport = false;
	m_emptyDBMode = false;
}


CArguments::~CArguments(void)
{
}


/** 
 * @brief 使用法表示 （ちょっとだけ詳しく）
 */ 
void PrintHelp()
{
	cerr << "編集前・編集後のPGDBの差分をSiNDYに反映させるプログラムです。" << endl;
	cerr << "メッシュ境界にかかるものは全て差し替え対象になります。" << endl;
	cerr << "設定ファイルの書き方は、DiffImportPGDB.txtをご覧下さい。" << endl;
}


void OutputOption( const uh::tstring& option )
{
	cout << "#" << uh::toStr(option) << endl;
	cerr << "#" << uh::toStr(option) << endl;
}

void OutputOption( const uh::tstring& option, const uh::tstring& value )
{
	cout << "#" << uh::toStr(option) << ":" << uh::toStr(value) << endl;
	cerr << "#" << uh::toStr(option) << ":" << uh::toStr(value) << endl;
}

bool CArguments::parse( int argc, _TCHAR* argv[] )
{
	const char * const old_pgdb =     "old,O";
	const char * const new_pgdb =     "new,N";
	const char * const mesh =         "mesh,M";
	const char * const suffix =       "suffix,S";
	const char * const test_mode =    "test,T";
	const char * const force_mode =   "force,F";
	const char * const tdc_priority = "tdc_priority,J";
	const char * const except_sde =   "except_sde,E";
	const char * const geospace =     "geospace,G";
	const char * const availdate =    "availdate,D";
	const char * const logdir =       "logdir,L";
	const char * const only_import =  "only_import,I";
	const char * const empty_db =     "empty_db,P";

	// "hoge,h"の形だとvariables_map.count()でうまく判定できないのでチェック用に別途定義
	const char * const test_mode_check =    "test";
	const char * const force_mode_check =   "force";
	const char * const tdc_priority_check = "tdc_priority";
	const char * const except_sde_check =   "except_sde";
	const char * const geospace_check =     "geospace";
	const char * const only_import_check =  "only_import";
	const char * const empty_db_check =     "empty_db";

	po::options_description desc("how to use");
	try
	{
		desc.add_options()
			(old_pgdb,     tvalue<tstring>(&m_oldPath),       "[必須]編集前PGDB")
			(new_pgdb,     tvalue<tstring>(&m_newPath),       "[必須]編集後PGDB")
			(mesh,         tvalue<tstring>(&m_meshPath),      "[必須]メッシュリスト")
			(suffix,       tvalue<tstring>(&m_suffix),        "[必須]Suffix")
			(logdir,       tvalue<tstring>(&m_logPath),       "[必須]ログ出力ディレクトリ")
			(test_mode,                                       "[任意]テストモード")
			(force_mode,                                      "[任意](バージョン非対応時)強制インポートモード")
			(tdc_priority,                                    "[任意]国内優先モード")
			(except_sde,                                      "[任意]PGDB・FGDBインポートモード")
			(geospace,     tvalue<tstring>(&m_layerListPath), "[任意]GEOSPACEインポートモード")
			(only_import,                                     "[任意]マージなしモード")
			(empty_db,                                        "[任意]空DBインポートモード")
			(availdate,    tvalue<tstring>(&m_availDate),     "[poiのとき必須]有効日付 YYYY/MM/DD")
			;
	
		po::variables_map vm;
		po::store( po::parse_command_line(argc, argv, desc), vm );
		po::notify( vm );

		// 必須パラメータのチェック
		bool good = true;
		if( m_oldPath.empty() )
		{
			cerr << old_pgdb << "オプションは必須です" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("編集前PGDB"), m_oldPath );
		}
		
		if( m_newPath.empty() )
		{
			cerr << new_pgdb << "オプションは必須です" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("編集後PGDB"), m_newPath );
		}
		
		if( m_meshPath.empty() )
		{
			cerr << mesh << "オプションは必須です" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("メッシュリスト"), m_meshPath );
		}

		if( m_suffix.empty() )
		{
			cerr << suffix << "オプションは必須です" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("Suffix"), m_suffix );
			if( m_suffix == _T("poi") )
			{
				if( m_availDate.empty() )
				{
					cerr << suffix << "がpoiのとき" << availdate << "オプションは必須です" << endl;
					good = false;
				}
				else
					OutputOption( _T("有効日付"), m_availDate );
			}
		}

		if( m_logPath.empty() )
		{
			cerr << logdir << "オプションは必須です" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("ログ出力先"), m_logPath );
			CLogFile logFile( m_logPath.c_str() );
			m_logFile = logFile;
		}

		// 任意オプション確認
		if( vm.count(test_mode_check) )
		{
			OutputOption( _T("テストモードで実行") );
			m_testMode = true;
		}
		
		if( vm.count(force_mode_check) )
		{
			OutputOption( _T("強制インポートモードで実行") );
			m_forceMode = true;
		}

		if( vm.count(tdc_priority_check) )
		{
			OutputOption( _T("国内優先モードで実行") );
			m_tdcMode = true;
		}

		if( vm.count(except_sde_check) )
		{
			OutputOption( _T("PGDB・FGDBインポートモードで実行") );
			m_exceptSdeMode = true;
		}

		if( vm.count(geospace_check) )
		{
			OutputOption( _T("GEOSPACEインポートモードで実行") );
			
			if( m_layerListPath.empty() )
			{
				cerr << geospace << "差し替えレイヤリストが指定されていません" << endl;
				good = false;
			}
			else
			{
				OutputOption( _T("差し替えレイヤリスト"), m_layerListPath );
				list<tstring> layerList = text_util::ToTStrList(m_layerListPath);
				for( auto& layer : layerList )
				{
					m_layerList.push_back( ((CString)layer.c_str()).MakeUpper() );
				}

				m_geospaceMode = true;
			}
		}

		if( vm.count(only_import_check) )
		{
			OutputOption( _T("マージなしモードで実行") );
			m_onlyImport = true;
		}

		if( vm.count(empty_db_check) )
		{
			OutputOption( _T("空DBインポートモードで実行") );
			m_emptyDBMode = true;
		}

		if( !good )
		{
			cerr << desc << endl;
			PrintHelp();
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
