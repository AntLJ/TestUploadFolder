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
#include "CreateIllustNameTable.h"
#include "common.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/sindylib_core/Workspace.h>
#include <sindy/schema/illust.h>
#include <TDC/illustlib/IllustLinkTable.h>
#include <TDC/illustlib/IllustNameCreator.h>
#include <TDC/illustlib/IllustOutputInfo.h>

using namespace sindy;
using namespace schema;
namespace po = boost::program_options;

CreateIllustNameTable::CreateIllustNameTable() : m_nErrCounter( 0 )
{
}

CreateIllustNameTable::~CreateIllustNameTable()
{
}

bool CreateIllustNameTable::init( int argc, _TCHAR* argv[] )
{
	// コマンド引数のチェック
	if( !checkArg( argc, argv ) ) return false;

	// イラストファイル名対応表ファイルのオープン
	if( !m_cIllustNameTable.open( m_strOutFile, m_varMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("イラストファイル名対応表ファイルをオープンできません : ") + m_strOutFile );
		return false;
	}

	// DBのオープン
	IWorkspacePtr ipWorkspace;
	if( !common_proc::getWorkspace( m_strDBConnect, ipWorkspace ) )
	{
		LogMgr::getInstance().writeRunLog( _T("DBに接続できません : ") + m_strDBConnect );
		return false;
	}

	// ILLUST_LINK フィーチャクラスの取得
	ITablePtr ipIllustLinkTable;
	if( !common_proc::getTable( illust_link::kTableName, ipWorkspace, ipIllustLinkTable ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + CString( illust_link::kTableName ) );
		return false;
	}
	// テーブルファインダに追加
	m_ipTableFinder.InitCollection( ipIllustLinkTable, CModel() );

	// 処理情報設定ファイルの読み込み
	if( !m_cIllustSettings.load( m_strIniFile, ipIllustLinkTable ) )
	{
		LogMgr::getInstance().writeRunLog( _T("処理情報設定ファイルを正常に読み込めません : ") + m_strIniFile );
		return false;
	}

	return true;
}

bool CreateIllustNameTable::execute()
{
	// 処理情報設定ファイルレコードのループ
	for( auto rec : m_cIllustSettings )
	{
		const auto& table = CAST_ILLUSTLINKTABLE( m_ipTableFinder.FindTable( illust_link::kTableName ) );
		if( !table ) return false;

		// ILLUST_LINK からイラスト種別コード＋仕向先コードが一致するレコード群を取得
		CString strWhereClause;
		strWhereClause.Format( _T("%s = %d and %s = %d"), illust_link::kIllustClass, rec.first.first, illust_link::kCustomerCode, rec.first.second );
		table->_Select( AheInitQueryFilter( nullptr, nullptr, strWhereClause ), true );
		table->CreateCache();

		// イラストファイル名生成クラスの初期化
		CIllustNameCreator cCreator( rec.second.m_strPrefix, _T(""), rec.second.m_vecNamingRule );

		int nIllustLinks = 0;				// イラストリンク数カウント用
		std::set<CString> setIllustFiles;	// イラストファイル数カウント用

		// ILLUST_LINK レコードのループ
		for( const auto& row : *table )
		{
			const auto& illustRow = CAST_ILLUSTLINKROW( row );

			// 出力除外条件に合致する場合はスキップ
			if( CIllustOutputInfo::IsException( illustRow, rec.second.m_cOutputInfo ) )
				continue;

			++nIllustLinks;

			// 標準出力に進捗を出力
			std::cout << "[" << rec.first.first << "] [" << rec.first.second << "] : " << std::setw(5) << nIllustLinks << " / " << std::setw(5) << table->size() << "\r";

			// イラストファイル名（整備用）取得
			CString strFileName = cCreator.GetIllustName( illustRow );
			if( strFileName.IsEmpty() )
			{
				outputErrLog( illustRow, LogMgr::EErrLevel::kError, _T(""), _T("イラストファイル名を取得できません") );
				continue;
			}
			setIllustFiles.insert( strFileName );

			// イラストファイルの存在チェック
			CString strFilePath = rec.second.m_strIllustDir + _T("\\") + strFileName + rec.second.m_strExtension;
			if( !checkFileExistence( strFilePath ) )
			{
				outputErrLog( illustRow, LogMgr::EErrLevel::kError, _T(""), _T("イラストファイルが存在しません"), strFilePath );
				continue;
			}

			// イラストファイル名対応表にレコードを追加
			m_cIllustNameTable.insert( IllustNameRec( rec.first.first, rec.first.second, illustRow->GetOID(), strFileName ) );
		}

		// キャッシュをクリア
		table->DeleteCache();
		table->clear();

		// 実行ログに処理情報を出力
		CString strLog;
		strLog.Format( _T("[%d] [%d] : %5d link(s), %5d file(s)"), rec.first.first, rec.first.second, nIllustLinks, setIllustFiles.size() );
		LogMgr::getInstance().writeRunLog( strLog );
	}

	// イラストファイル名対応表ファイルを出力
	m_cIllustNameTable.write();

	// エラーがある場合は false を戻す
	return ( 0 == m_nErrCounter );
}

bool CreateIllustNameTable::checkArg( int argc, _TCHAR* argv[] )
{
	po::options_description option("オプション");

	option.add_options()
		( "db,d",				po::value<std::string>(),	"DB接続文字列" )
		( "illust_settings,i",	po::value<std::string>(),	"処理情報設定ファイル名（入力）" )
		( "output_list,o",		po::value<std::string>(),	"イラストファイル名対応表ファイル名（出力）" )
		( "runlog,r",			po::value<std::string>(),	"実行ログファイル名" )
		( "errlog,e",			po::value<std::string>(),	"エラーログファイル名" )
		( "help,?",											"ヘルプを表示" );
	po::store( po::parse_command_line( argc, argv, option ), m_varMap );
	po::notify( m_varMap );

	// ヘルプ表示
	if( m_varMap.count( "help" ) || 0 == m_varMap.size() )
	{
		std::cout << option << std::endl;
		return false;
	}

	// 実行ログファイル名
	if( !getArgParam( "runlog", m_strRunLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("実行ログファイル名が指定されていません") );
		return false;
	}
	if( !LogMgr::getInstance().setRunLog( m_strRunLog, &m_varMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("実行ログファイルをオープンできません : ") + m_strRunLog );
		return false;
	}

	// エラーログファイル名
	if( !getArgParam( "errlog", m_strErrLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("エラーログファイル名が指定されていません") );
		return false;
	}
	if( !LogMgr::getInstance().setErrLog( m_strErrLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("エラーログファイルをオープンできません : ") + m_strErrLog );
		return false;
	}

	// DB接続文字列
	if( !getArgParam( "db", m_strDBConnect ) )
	{
		LogMgr::getInstance().writeRunLog( _T("DB接続文字列が指定されていません") );
		return false;
	}

	// イラストファイル名対応表ファイル名
	if( !getArgParam( "output_list", m_strOutFile ) )
	{
		LogMgr::getInstance().writeRunLog( _T("イラストファイル名対応表ファイル名が指定されていません") );
		return false;
	}

	// 処理情報設定ファイル名
	if( !getArgParam( "illust_settings", m_strIniFile ) )
	{
		LogMgr::getInstance().writeRunLog( _T("処理情報設定ファイル名が指定されていません") );
		return false;
	}

	return true;
}

bool CreateIllustNameTable::getArgParam( const std::string& strName, CString& strParam )
{
	if( 0 == m_varMap.count( strName ) ) return false;
	strParam = CString( m_varMap[strName].as<std::string>().c_str() );
	if( strParam.IsEmpty() ) return false;
	return true;
}

bool CreateIllustNameTable::checkFileExistence( const CString& strFileName )
{
	std::ifstream ifs( strFileName, std::ios::binary | std::ios::in );
	if( !ifs.is_open() ) return false;
	ifs.close();
	return true;
}

void CreateIllustNameTable::outputErrLog( const sindy::CSPIllustLinkRow& illustLinkRow, LogMgr::EErrLevel errLv, const CString& strErrCode, const CString& strErrMsg, const CString& strOption /* = _T("") */ )
{
	// 最初の1件目の場合、ログタイプを出力
	if( 0 == m_nErrCounter )
	{
		LogMgr::getInstance().writeErrLog( _T("# SINDYSTDLOG") );
	}
	++m_nErrCounter;

	// 標準ログフォーマットで出力
	CString strOut;
	strOut.Format( _T("0\t%s\t%ld\t\t\t%s\t%s\t%s\t%s"), sindy::schema::illust_link::kTableName, illustLinkRow->GetOID(), LogMgr::getErrLvString( errLv ), strErrCode, strErrMsg, strOption );
	LogMgr::getInstance().writeErrLog( strOut );
}
