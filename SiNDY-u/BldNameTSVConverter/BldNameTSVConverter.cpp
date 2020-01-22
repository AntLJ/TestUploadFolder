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
#include "BldNameTSVConverter.h"
#include "TsvWriter.h"
#include "MultiTranslateListWriter.h"
#include <crd_cnv/crd_cnv.h>
#include <boost/assign.hpp>
#include <TDC/useful_headers/str_util.h>
#include <regex>

crd_cnv gCrdCnv;

namespace po = boost::program_options;

LPCSTR g_strKey_INI			= "ini";
LPCSTR g_strKey_RUN_LOG		= "run_log";
LPCSTR g_strKey_ERR_LOG		= "err_log";
LPCSTR g_strKey_HELP		= "help";

LPCSTR g_strKey_BASE_DB		= "BASE_DB";
LPCSTR g_strKey_MAP_DB		= "MAP_DB";
LPCSTR g_strKey_ADDR_DB		= "ADDR_DB";
LPCSTR g_strKey_AM_DB		= "AM_DB";
LPCSTR g_strKey_GRP_CODE	= "GRP_CODE";
LPCSTR g_strKey_OUTPUT_DIR	= "OUTPUT_DIR";
LPCSTR g_strKey_TRANSLATE   = "TRANSLATE";
LPCSTR g_strKey_ADDCOLUMN   = "ADD_COLUMN";
LPCSTR g_strKey_MAX_REC_NUM = "MAX_REC_NUM";

LPCSTR g_strFile_BLD001		= "BLD001.tsv.bld";
LPCSTR g_strFile_MSTBLA		= "MSTBLA.tsv.bld";

const long g_lClassNameMaxBytes	= 40;
const long g_lBldgNameMaxBytes	= 80;
const long g_lBldgYomiMaxBytes	= 160;
const long g_lBldgNameSepMaxBytes	= 120;
const long g_lBldgYomiSepMaxBytes	= 200;

const long g_lBldgOIDRange		= 500000;

BldNameTSVConverter::BldNameTSVConverter() : 
	m_conAddrDB( __uuidof(ADODB::Connection) ), m_mode(ExecuteMode::ReleaseTsv)
{
}

BldNameTSVConverter::~BldNameTSVConverter()
{
}

bool BldNameTSVConverter::init( int argc, _TCHAR* argv[] )
{
	// コマンド引数のチェック
	if( !checkArg( argc, argv ) ) return false;

	// 設定ファイルの読み込み
	if( !loadIniFile() ) return false;

	// 実行モード問わず、両方で必要な初期化をまず実施

	// DBのオープン
	IWorkspacePtr ipWSBase, ipWSMap, ipWSAddr;
	if( !common_proc::getWorkspace( m_strDBBase, ipWSBase ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + m_strDBBase );
		return false;
	}

	// フィーチャクラスの取得
	if( !common_proc::getFeatureClass( sindy::schema::sj::buildingname_point::kTableName, ipWSBase, m_ipFCBldgNamePoint ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + CString( sindy::schema::sj::buildingname_point::kTableName ) );
		return false;
	}

	// フィールドインデックスマップの取得
	if( !common_proc::getFieldIndexes( m_ipFCBldgNamePoint, m_mapIndexesBldgNamePoint ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィールドインデックスマップを取得できません : ") + CString( sindy::schema::sj::buildingname_point::kTableName ) );
		return false;
	}

	// 「多言語翻訳リスト作成」モード時の初期化
	if( ExecuteMode::TranslateList == m_mode )
	{
		// 多言語翻訳リスト追加カラム設定ファイル読込み
		std::ifstream ifs( m_strAddColumn );
		if( !ifs.is_open() )
		{
			LogMgr::getInstance().writeRunLog( _T("多言語翻訳リスト追加カラム設定ファイルをオープンできません : ") + m_strAddColumn );
			return false;
		}

		std::string line;
		while(std::getline(ifs, line))
		{
			// 空行とコメント行は取得対象外
			if( line.empty() || line.at(0) == _T('#') )
				continue;

			m_addcolumnList.push_back( CString(line.c_str()) );
		}

		return true;
	}

	// 「検索リリースデータ作成」モード時のみで必要な初期化を実施

	// DBのオープン
	if( !common_proc::getWorkspace( m_strDBMap, ipWSMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + m_strDBMap );
		return false;
	}
	if( !common_proc::getWorkspace( m_strDBAddr, ipWSAddr ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + m_strDBAddr );
		return false;
	}

	// フィーチャクラスの取得
	if( !common_proc::getFeatureClass( sindy::schema::building::kTableName, ipWSMap, m_ipFCBuilding ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + CString( sindy::schema::building::kTableName ) );
		return false;
	}
	if( !common_proc::getFeatureClass( sindy::schema::city_admin::kTableName, ipWSAddr, m_ipFCCityAdmin ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィーチャクラスをオープンできません : ") + CString( sindy::schema::city_admin::kTableName ) );
		return false;
	}

	// フィールドインデックスマップの取得
	if( !common_proc::getFieldIndexes( m_ipFCCityAdmin, m_mapIndexesCityAdmin ) )
	{
		LogMgr::getInstance().writeRunLog( _T("フィールドインデックスマップを取得できません : ") + CString( sindy::schema::city_admin::kTableName ) );
		return false;
	}

	// 住所中間テーブル格納DBのオープン
	if( !openAddrDB() ) return false;

	return true;
}

bool BldNameTSVConverter::execCreateReleaseTsv()
{
	// 建物種別コードマスタの生成
	if( !createBldgClassMaster() ) return false;

	// 検索リリースデータ（TSV）の生成
	if( !createBldgNameTSV() ) return false;

	return true;
}

bool BldNameTSVConverter::execCreateMultiTranslateList()
{
	// 多言語翻訳リストの生成
	return createMultiTranslateList();
}

bool BldNameTSVConverter::exec()
{
	// 実行モードに応じた処理
	switch(m_mode)
	{
	case ExecuteMode::ReleaseTsv : // 検索リリースデータ作成
		return execCreateReleaseTsv();
		break;
	case ExecuteMode::TranslateList: // 多言語翻訳リスト作成
		return execCreateMultiTranslateList();
		break;
	}
	return true;
}

bool BldNameTSVConverter::checkArg( int argc, _TCHAR* argv[] )
{
	po::options_description descOption( "オプション" );
	descOption.add_options()
		( g_strKey_INI,	          po::value<std::string>(),	"設定ファイル名" )
		( g_strKey_RUN_LOG,	      po::value<std::string>(),	"実行ログファイル名" )
		( g_strKey_ERR_LOG,	      po::value<std::string>(),	"エラーログファイル名" )
		( g_strKey_HELP,                                    "ヘルプを表示" );
	po::variables_map varMap;
	po::store( po::parse_command_line( argc, argv, descOption ), varMap );
	po::notify( varMap );

	// ヘルプ表示
	if( varMap.count( g_strKey_HELP ) || 0 == varMap.size() )
	{
		std::cout << descOption << std::endl;
		return false;
	}

	// 必須カラム一覧（実行モードに関係なく共通のものだけ定義）
	std::vector<std::pair<LPCSTR, CString*>> paramList = boost::assign::pair_list_of
		(g_strKey_RUN_LOG, &m_strRunLog)
		(g_strKey_ERR_LOG, &m_strErrLog)
		(g_strKey_INI, &m_strIniFile)
		;

	// 必須オプション検査
	for(const auto& param : paramList)
	{
		if( !getArgParam( varMap, param.first, *(param.second) ) )
		{
			LogMgr::getInstance().writeRunLog( uh::str_util::format(_T("%sオプションが指定されていません"), (CString)param.first) );
			return false;
		}
	}

	// 実行ログファイル名
	if( !LogMgr::getInstance().setRunLog( m_strRunLog, &varMap ) )
	{
		LogMgr::getInstance().writeRunLog( _T("実行ログファイルをオープンできません : ") + m_strRunLog );
		return false;
	}

	// エラーログファイル名
	if( !LogMgr::getInstance().setErrLog( m_strErrLog ) )
	{
		LogMgr::getInstance().writeRunLog( _T("エラーログファイルをオープンできません : ") + m_strErrLog );
		return false;
	}
	LogMgr::getInstance().writeErrLog( _T("# FREESTYLELOG") );
	LogMgr::getInstance().writeErrLog( _T("FLAG\tERR_LV\tLAYER\tOBJECTID\tERR_MSG") );

	return true;
}

bool BldNameTSVConverter::getArgParam( const boost::program_options::variables_map& varMap, const std::string& strName, CString& strParam )
{
	if( 0 == varMap.count( strName ) ) return false;
	strParam = CString( varMap[strName].as<std::string>().c_str() );
	if( strParam.IsEmpty() ) return false;
	return true;
}

bool BldNameTSVConverter::loadIniFile()
{
	po::options_description descIniFile( "設定ファイル" );
	descIniFile.add_options()
		( g_strKey_BASE_DB,		po::value<std::string>(), "ベースDB" )
		( g_strKey_MAP_DB,		po::value<std::string>(), "地図DB" )
		( g_strKey_ADDR_DB,		po::value<std::string>(), "住所DB" )
		( g_strKey_AM_DB,		po::value<std::string>(), "住所中間テーブル格納DB" )
		( g_strKey_GRP_CODE,	po::value<std::string>(), "データ分類コード" )
		( g_strKey_OUTPUT_DIR,	po::value<std::string>(), "出力ディレクトリ" )
		( g_strKey_TRANSLATE, 	                          "多言語翻訳リスト作成モード" )
		( g_strKey_ADDCOLUMN,   po::value<std::string>(), "多言語翻訳リスト追加カラム設定ファイル" )
		( g_strKey_MAX_REC_NUM, po::value<std::string>(), "多言語翻訳リスト内最大レコード数" );


	std::ifstream ifs( m_strIniFile );
	if( !ifs.is_open() )
	{
		LogMgr::getInstance().writeRunLog( _T("設定ファイルをオープンできません") );
		return false;
	}
	po::variables_map varMap;
	po::store( po::parse_config_file( ifs, descIniFile ), varMap );
	po::notify( varMap );

	// 「TRANSLATE」項目があり、値が「true」の場合、実行モードを「多言語翻訳リスト作成」に変更
	if( 0 != varMap.count( g_strKey_TRANSLATE ) && 
			0 == CString( varMap[ g_strKey_TRANSLATE ].as<std::string>().c_str() ).CompareNoCase( _T("true") ) )
			m_mode = ExecuteMode::TranslateList;

	// 必須設定項目一覧（検索リリースデータ生成時）
	const std::vector<std::pair<LPCSTR, CString*>> paramReleaseTsvList = boost::assign::pair_list_of
		( g_strKey_BASE_DB, &m_strDBBase )
		( g_strKey_MAP_DB, &m_strDBMap )
		( g_strKey_ADDR_DB, &m_strDBAddr )
		( g_strKey_AM_DB, &m_strDBAM )
		( g_strKey_GRP_CODE, &m_strGrpCode )
		( g_strKey_OUTPUT_DIR, &m_strOutDir )
		;

	// 必須設定項目一覧（多言語翻訳リスト作成時）
	const std::vector<std::pair<LPCSTR, CString*>> paramTranslateList = boost::assign::pair_list_of
		( g_strKey_BASE_DB, &m_strDBBase )
		( g_strKey_GRP_CODE, &m_strGrpCode )
		( g_strKey_OUTPUT_DIR, &m_strOutDir )
		( g_strKey_ADDCOLUMN, &m_strAddColumn )	
		( g_strKey_MAX_REC_NUM, &m_maxRecNum )	
		;

	auto paramList = ( m_mode == ExecuteMode::ReleaseTsv ) ?
							paramReleaseTsvList : paramTranslateList;

	LogMgr::getInstance().writeParamInfos( varMap );

	// 必須設定項目検査
	for(const auto& param : paramList)
	{
		if( !getArgParam( varMap, param.first, *(param.second) ) )
		{
			LogMgr::getInstance().writeRunLog( _T("設定項目 ") + CString( param.first ) + _T(" が設定されていません") );
			return false;
		}
	}

	if( ExecuteMode::TranslateList == m_mode )
	{
		// 多言語翻訳リストレコード最大出力件数値の検査（1以上の整数であるか）
		std::wregex maxRecNumRegex(L"^[1-9]{1}[0-9]{0,}$");
		if( 0 != varMap.count(g_strKey_MAX_REC_NUM) && !std::regex_match( CStringW(m_maxRecNum).GetString(), maxRecNumRegex ) )
		{
			LogMgr::getInstance().writeRunLog( _T("多言語翻訳リストレコード最大出力件数が1以上の整数ではありません : ") + m_maxRecNum );
			return false;
		}
	}

	ifs.close();

	return true;
}

bool BldNameTSVConverter::openAddrDB()
{
	// 接続文字列からユーザ・サービス名を取得
	int nPos = m_strDBAM.Find( _T('@') );
	if( 0 >= nPos )
	{
		LogMgr::getInstance().writeRunLog( _T("住所中間テーブル格納DBへの接続文字列が無効です : ") + m_strDBAM );
		return false;
	}
	CString strDBUser = m_strDBAM.Left( nPos );
	CString strDBSrvc = m_strDBAM.Mid( nPos + 1 );
	if( strDBUser.IsEmpty() || strDBSrvc.IsEmpty() )
	{
		LogMgr::getInstance().writeRunLog( _T("住所中間テーブル格納DBへの接続文字列が無効です : ") + m_strDBAM );
		return false;
	}

	// DBに接続
	CString strConnect;
	strConnect.Format( _T(" Provider=OraOLEDB.Oracle; Data Source=%s; User ID=%s; Password=%s; "), strDBSrvc, strDBUser, strDBUser );
	m_conAddrDB->ConnectionString = _bstr_t( strConnect );
	try
	{
		if( FAILED( m_conAddrDB->Open( "", "", "", ADODB::adConnectUnspecified ) ) )
		{
			LogMgr::getInstance().writeRunLog( _T("住所中間テーブル格納DBに接続できません : ") + m_strDBAM );
			return false;
		}
	}
	catch( const _com_error& e )
	{
		LogMgr::getInstance().writeRunLog( _T("住所中間テーブル格納DB接続時に例外発生 : ") + m_strDBAM );
		throw e;
	}

	return true;
}

void BldNameTSVConverter::outputErrLog( LogMgr::EErrLevel errLv, const CString& strLayer, long lOID, const CString& strMsg )
{
	CString strTmp;
	strTmp.Format( _T("0\t%s\t%s\t%ld\t%s"), LogMgr::getErrLvString( errLv ), strLayer, lOID, strMsg );
	LogMgr::getInstance().writeErrLog( strTmp );
}

bool BldNameTSVConverter::createBldgClassMaster()
{
	std::cout << "建物種別コードマスタ出力開始" << std::endl;

	// マスタファイルを開く（改行コードがLFとなるようバイナリモードで開く）
	TsvWriter writer;
	CString file = uh::str_util::format( _T("%s\\%s"), m_strOutDir, (CString)g_strFile_MSTBLA );
	if( !writer.open( file, Encoding::Euc, LineFeed::Lf ) )
	{
		LogMgr::getInstance().writeRunLog( _T("建物種別コードマスタファイルをオープンできません : ") + file );
		return false;
	}

	// コード値ドメイン取得
	std::map< long, CString > mapValues;
	if( !common_proc::getCodedValues( m_ipFCBldgNamePoint, sindy::schema::sj::buildingname_point::kBldgClass1, mapValues ) )
	{
		LogMgr::getInstance().writeRunLog( _T("建物種別コードのコード値ドメインを取得できません : ") + CString( sindy::schema::sj::buildingname_point::kBldgClass1 ) );
		return false;
	}

	// マスタファイルにコード値ドメインの一覧を出力
	for( const auto& rec : mapValues )
	{
		// 説明文字列のバイト数をチェック
		if( common_proc::convSJIS2EUC( std::string( CT2A( rec.second ) ) ).size() > g_lClassNameMaxBytes )
		{
			CString strTmp;
			strTmp.Format( _T("建物種別コードの説明文字列が %ld バイトを超えている : [%ld] %s"), g_lClassNameMaxBytes, rec.first, rec.second );
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, 0, strTmp );
		}

		std::vector<CString> valueList = boost::assign::list_of
			( uh::str_util::ToString(rec.first) )
			( rec.second )
			;

		CString joinStr = uh::str_util::join(valueList, _T("\t"));
		std::string writeStr = CT2A( joinStr + LINE_FEED_CODE_LF );

		// EUCで出力
		if( !writer.writeRecord( valueList ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, 0, _T("建物種別コードマスタファイルに書き込めない") );
			continue;
		}
	}

	// マスタファイルを閉じる
	writer.close();

	std::cout << "建物種別コードマスタ出力完了" << std::endl;

	return true;
}

bool BldNameTSVConverter::searchBldNamePoint(IFeatureCursorPtr& ipFeatureCur, long& lCount)
{
	// 建物ビル名称ポイントのカーソルを取得
	CString strWhere( CString( sindy::schema::sj::buildingname_point::kYomiSeparated ) + _T(" IS NOT NULL") );
	CString strPostfix( _T("ORDER BY ") + CString( sindy::schema::sj::buildingname_point::kBuildingOID ) );
	if( !common_proc::searchByWhereClause( m_ipFCBldgNamePoint, ipFeatureCur, lCount, strWhere, strPostfix ) )
	{
		LogMgr::getInstance().writeRunLog( _T("建物ビル名称ポイントの検索に失敗しました : ") + strWhere );
		return false;
	}
	return true;
}

bool BldNameTSVConverter::getCommonAttrValue(const IFeaturePtr& ipFeature, BldNamePointRec& rec )
{
	// 名称ポイントのOIDを取得
	ipFeature->get_OID( &rec.bldNamePointOID );

	// 建物ポリゴンOIDを取得
	CComVariant varBldgOID;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kBuildingOID, m_mapIndexesBldgNamePoint, varBldgOID ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("建物ポリゴンOIDを取得できない") );
		return false;
	}
	rec.bldgOID = varBldgOID.lVal;

	// 名称を取得
	CComVariant varName;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kName, m_mapIndexesBldgNamePoint, varName ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("名称を取得できない") );
		return false;
	}
	rec.name = ( VT_BSTR == varName.vt ) ? varName.bstrVal : _T("");

	// 名称のバイト数をチェック
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.name ) ) ).size() > g_lBldgNameMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("名称のサイズが %ld バイトを超えている : %s"), g_lBldgNameMaxBytes, rec.name );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	// 名称（分割）を取得
	CComVariant varNameSep;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kNameSeparated, m_mapIndexesBldgNamePoint, varNameSep ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("名称（分割）を取得できない") );
		return false;
	}
	rec.nameSep = ( VT_BSTR == varNameSep.vt ) ? varNameSep.bstrVal : _T("");

	// 名称（分割）のバイト数をチェック
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.nameSep ) ) ).size() > g_lBldgNameSepMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("名称（分割）のサイズが %ld バイトを超えている : %s"), g_lBldgNameSepMaxBytes, rec.nameSep );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	// 読み（分割）を取得
	CComVariant varYomiSep;
	if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kYomiSeparated, m_mapIndexesBldgNamePoint, varYomiSep ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("ヨミ（分割）を取得できない") );
		return false;
	}
	rec.yomiSep = ( VT_BSTR == varYomiSep.vt ) ? varYomiSep.bstrVal : _T("");

	// 読みのバイト数をチェック
	// （読み（分割）の値から分割文字を除去したもの）
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.getRemovedSepStrYomi() ) ) ).size() > g_lBldgYomiMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("読みのサイズが %ld バイトを超えている : %s"), g_lBldgYomiMaxBytes, rec.getRemovedSepStrYomi() );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	// 読み（分割）のバイト数をチェック
	if( common_proc::convSJIS2EUC( std::string( CT2A( rec.yomiSep ) ) ).size() > g_lBldgYomiSepMaxBytes )
	{
		CString strTmp;
		strTmp.Format( _T("読み（分割）のサイズが %ld バイトを超えている : %s"), g_lBldgYomiSepMaxBytes, rec.yomiSep );
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, strTmp );
	}

	return true;
}

bool BldNameTSVConverter::createBldgNameTSV()
{
	std::cout << "建物ビル名称検索リリースデータ出力開始" << std::endl;

	// 検索リリースデータファイルを開く
	TsvWriter writer;
	CString file = uh::str_util::format( _T("%s\\%s"), m_strOutDir, (CString)g_strFile_BLD001 );
	if( !writer.open( file, Encoding::Euc, LineFeed::Lf ) )
	{
		LogMgr::getInstance().writeRunLog( _T("検索リリースデータファイルをオープンできません : ") + file );
		return false;
	}
	
	// 建物ビル名称ポイントを検索
	long lCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( !searchBldNamePoint( ipFeatureCursor, lCount ) )
		return false;

	std::cout << "出力対象検索リリースデータ件数 : " << std::setw( 7 ) << lCount << " 件" << std::endl;

	// 建物ビル名称ポイントのループ
	IFeaturePtr ipFeature;
	long i = 0;
	while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) && ipFeature )
	{
		++i;
		if( 0 == i % 100 || i == lCount )
		{
			std::cout << "検索リリースデータ出力中 [ " << std::setw( 7 ) << i << " / " << std::setw( 7 ) << lCount << " ]\r";
		}

		BldNamePointRec rec;

		// 共通属性値取得
		if( !getCommonAttrValue( ipFeature, rec ) ) 
			continue;

		// 建物種別コード1を取得
		CComVariant varBlgdClass1;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kBldgClass1, m_mapIndexesBldgNamePoint, varBlgdClass1 ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("建物種別コード1を取得できない") );
			continue;
		}
		rec.bldgClass1 = ( VT_I4 == varBlgdClass1.vt ) ? CString( std::to_string( varBlgdClass1.lVal ).c_str() ) : _T("");

		// 階数を取得
		CComVariant varFloors;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kFloors, m_mapIndexesBldgNamePoint, varFloors ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("階数を取得できない") );
			continue;
		}
		rec.floors = ( VT_I4 == varFloors.vt ) ? CString( std::to_string( varFloors.lVal ).c_str() ) : _T("");

		// コンテンツコードを取得
		CComVariant varContentsCode;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kContentsCode, m_mapIndexesBldgNamePoint, varContentsCode ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("注記POIコンテンツコードを取得できない") );
			continue;
		}
		rec.contentsCode = ( VT_I4 == varContentsCode.vt ) ? varContentsCode.lVal : 0L;

		// ビル名称妥当フラグを取得
		CComVariant varSuitable;
		if( !common_proc::getValue( ipFeature, sindy::schema::sj::buildingname_point::kSuitable, m_mapIndexesBldgNamePoint, varSuitable ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T( "ビル名称妥当フラグを取得できない" ) );
			continue;
		}
		rec.suitable = ( VT_I4 == varSuitable.vt ) ? CString( std::to_string( varSuitable.lVal ).c_str() ) : _T( "0" );

		// 住所コードを設定
		if( !getAddrCode( rec.bldNamePointOID, rec.bldgOID, ipFeature, rec.addrcode ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, rec.bldNamePointOID, _T("住所コードを取得できない") );
			continue;
		}

		// 代表点を設定
		IPointPtr ipPoint;
		if( !getBldgRepPoint( rec.bldNamePointOID, rec.bldgOID, ipFeature, ipPoint ) ) continue;

		// 座標値を2次メッシュXYに変換
		double dLon = 0.0, dLat = 0.0;
		if( FAILED( ipPoint->QueryCoords( &dLon, &dLat ) ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, rec.bldgOID, _T("建物ポリゴンの代表点座標を取得できない") );
			continue;
		}
		if( 0 != gCrdCnv.LLtoMesh( dLon, dLat, 2, &rec.meshcode, &rec.x, &rec.y, 1 ) )
		{
			outputErrLog(
					LogMgr::kError,
					sindy::schema::sj::buildingname_point::kTableName,
					rec.bldgOID,
					_T("建物ポリゴンの代表点座標を2次メッシュXYに変換できない") );
			continue;
		}

		// 出力ファイルに出力する値を格納
		std::vector<CString> valueList = boost::assign::list_of
			( m_strGrpCode )
			( uh::str_util::ToString(rec.bldgOID) )
			( uh::str_util::ToString(rec.meshcode) )
			( uh::str_util::ToString(rec.x) )
			( uh::str_util::ToString(rec.y) )
			( rec.addrcode.Mid(  0, 2 ) )
			( rec.addrcode.Mid(  2, 3 ) )
			( rec.addrcode.Mid(  5, 3 ) )
			( rec.addrcode.Mid(  8, 3 ) )
			( rec.addrcode.Mid( 11, 5 ) )
			( rec.addrcode.Mid( 16, 4 ) )
			( rec.name )
			( rec.getRemovedSepStrYomi() )
			( rec.nameSep )
			( rec.yomiSep )
			( rec.bldgClass1 )
			( rec.floors )
			( rec.getGrpCode() )
			( rec.getChainCode() )
			( rec.suitable )
			;

		// EUCで出力
		if( !writer.writeRecord( valueList ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, rec.bldgOID, _T("検索リリースデータファイルに書き込めない") );
			continue;
		}
	}

	// 検索リリースデータファイルを閉じる
	writer.close();

	std::cout << std::endl << "建物ビル名称検索リリースデータ出力完了" << std::endl;

	return true;
}

bool BldNameTSVConverter::createMultiTranslateList()
{
	std::cout << "多言語翻訳リスト出力開始" << std::endl;

	// 検索リリースデータファイルを開く（UTF-8(BOM付き））
	MultiTranslateListWriter writer( m_strOutDir, _tstol(m_maxRecNum), m_strGrpCode, m_addcolumnList );
	if( !writer.open() )
		return false;

	// 建物ビル名称ポイントを検索
	long lCount = 0;
	IFeatureCursorPtr ipFeatureCursor;
	if( !searchBldNamePoint( ipFeatureCursor, lCount ) )
		return false;

	std::cout << "多言語翻訳リスト出力対象データ件数 : " << std::setw( 7 ) << lCount << " 件" << std::endl;

	// 建物ビル名称ポイントのループ
	IFeaturePtr ipFeature;
	long i = 0;
	while( SUCCEEDED( ipFeatureCursor->NextFeature( &ipFeature ) ) && ipFeature )
	{
		++i;
		if( 0 == i % 100 || i == lCount )
		{
			std::cout << "多言語翻訳リストデータ出力中 [ " << std::setw( 7 ) << i << " / " << std::setw( 7 ) << lCount << " ]\r";
		}

		BldNamePointRec rec;

		// 共通属性値取得
		if( !getCommonAttrValue(ipFeature, rec) ) 
			continue;

		// 出力ファイルに出力する値を格納
		std::vector<CString> valueList = boost::assign::list_of
			( m_strGrpCode )
			( uh::str_util::ToString(rec.bldgOID) )
			( rec.name )
			( rec.getRemovedSepStrYomi() )
			( rec.getReplacedSepName() )
			( rec.getReplacedSepYomi() )
			;

		// 多言語翻訳リスト追加カラム設定ファイルに記載されたヘッダ数分、空列を追加
		for( const auto& column : m_addcolumnList )
			valueList.push_back( _T("") );

		writer.writeRecord( valueList );
	}

	// 検索リリースデータファイルを閉じる
	writer.close();

	std::cout << std::endl << "多言語翻訳リスト出力完了" << std::endl;

	return true;
}

bool BldNameTSVConverter::getAddrCode( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, CString& strAddrCode )
{
	strAddrCode.Empty();

	// 建物ポリゴンOIDをキーに住所中間テーブルより検索
	if( !getAddrCodeFromIntermediate( lBldgOID, strAddrCode )  ) return false;
	if( !strAddrCode.IsEmpty() ) return true;

	// 名称ポイントのジオメトリを取得
	IGeometryPtr ipGeometry;
	if( FAILED( ipFeature->get_Shape( &ipGeometry ) ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("ジオメトリの取得に失敗") );
		return false;
	}

	// 当該ポイントを内包する都市地図行政界ポリゴンより住所コードを取得
	if( !getAddrCodeFromAdmin( lOID, ipGeometry, strAddrCode ) ) return false;

	return true;
}

bool BldNameTSVConverter::getAddrCodeFromIntermediate( long lBldgOID, CString& strAddrCode )
{
	// 建物ポリゴンOID→住所コード対応表を作成
	if( !createMapBldgOID2AddrCode( lBldgOID ) ) return false;

	// 対応表を検索し、OIDがあれば住所コードを返す
	const auto itr = m_mapBldgOID2AddrCode.find( lBldgOID );
	if( m_mapBldgOID2AddrCode.end() != itr ) strAddrCode = itr->second;

	return true;
}

bool BldNameTSVConverter::createMapBldgOID2AddrCode( long lBldgOID )
{
	// 対応表のOID範囲
	static long lOIDMin = -1, lOIDMax = -1;

	// 対応表のOID範囲に当該OIDが含まれていたらそのまま抜ける
	if( lOIDMin <= lBldgOID && lBldgOID <= lOIDMax ) return true;

	// 当該OIDを含むOID範囲を設定
	// g_lBldgOIDRange = 500000 の場合 : [ 0, 499999 ] → [ 500000, 999999 ] → [ 1000000, 1499999 ] → … と推移するはず
	lOIDMin = static_cast<long>( lBldgOID / g_lBldgOIDRange ) * g_lBldgOIDRange;
	lOIDMax = lOIDMin + g_lBldgOIDRange - 1;

	// 対応表のクリア
	m_mapBldgOID2AddrCode.clear();

	// クエリ設定
	// rep_flag の立っている polygon_id は gp_view と cs_view で被らないため、単純なUNIONで問題ない
	CString strSQL;
	strSQL.Format(
		_T( "SELECT polygon_id, addr_code FROM gp_view WHERE rep_flag = '1' AND polygon_id BETWEEN %ld AND %ld UNION " ) \
		_T( "SELECT polygon_id, addr_code FROM cs_view WHERE rep_flag = '1' AND polygon_id BETWEEN %ld AND %ld" ),
		lOIDMin, lOIDMax, lOIDMin, lOIDMax );

	try
	{
		// クエリのオープン
		ADODB::_RecordsetPtr pRecs( __uuidof( ADODB::Recordset ) );
		pRecs->PutRefActiveConnection( m_conAddrDB );
		if( FAILED( pRecs->Open( _bstr_t( strSQL ), vtMissing, ADODB::adOpenKeyset, ADODB::adLockReadOnly, ADODB::adCmdText ) ) )
		{
			outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T( "建物ポリゴンOID→住所コード対応表の生成に失敗" ) );
			return false;
		}
		// レコード群を対応表に格納
		while( !pRecs->adoEOF )
		{
			m_mapBldgOID2AddrCode.insert( std::make_pair( pRecs->Fields->GetItem( 0L )->Value.lVal, pRecs->Fields->GetItem( 1L )->Value.bstrVal ) );
			pRecs->MoveNext();
		}
	}
	catch( const _com_error& e )
	{
		outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T( "建物ポリゴンOID→住所コード対応表の生成時に例外発生 : " ) + CString( e.ErrorMessage() ) );
		return false;
	}

	return true;
}

bool BldNameTSVConverter::getAddrCodeFromAdmin( long lOID, const IGeometryPtr& ipGeometry, CString& strAddrCode )
{
	// 住所コードを空に
	strAddrCode.Empty();

	// 当該ポイントを内包する都市地図行政界ポリゴンを空間検索
	// （OID昇順で結果を取得し、複数返った場合は最初の1フィーチャを選択）
	IFeatureCursorPtr ipFCurCityAdmin;
	CString strPostfix( _T("ORDER BY ") + CString( sindy::schema::city_admin::kObjectID ) );
	long lCount = 0;
	if( !common_proc::searchByGeometry( m_ipFCCityAdmin, ipFCurCityAdmin, lCount, ipGeometry, esriSpatialRelIntersects, strPostfix ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("行政界ポリゴンの空間検索に失敗") );
		return false;
	}

	IFeaturePtr ipFCityAdmin;
	if( FAILED( ipFCurCityAdmin->NextFeature( &ipFCityAdmin ) ) || !ipFCityAdmin )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("行政界ポリゴンフィーチャの取得に失敗") );
		return false;
	}

	// 都市地図行政界ポリゴンのOID取得
	long lOIDCityAdmin = 0;
	ipFCityAdmin->get_OID( &lOIDCityAdmin );

	// 都市地図行政界ポリゴンの各種属性値取得
	CComVariant varCityCode, varAddrCode, varGaikuFugo;
	if( !common_proc::getValue( ipFCityAdmin, sindy::schema::city_admin::kCityCode, m_mapIndexesCityAdmin, varCityCode ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::city_admin::kTableName, lOIDCityAdmin, _T("行政界ポリゴンフィーチャの市区町村コードの取得に失敗") );
		return false;
	}
	if( !common_proc::getValue( ipFCityAdmin, sindy::schema::city_admin::kAddrCode, m_mapIndexesCityAdmin, varAddrCode ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::city_admin::kTableName, lOIDCityAdmin, _T("行政界ポリゴンフィーチャの住所コードの取得に失敗") );
		return false;
	}
	if( !common_proc::getValue( ipFCityAdmin, sindy::schema::city_admin::kGaikuFugo, m_mapIndexesCityAdmin, varGaikuFugo ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::city_admin::kTableName, lOIDCityAdmin, _T("行政界ポリゴンフィーチャの街区符号の取得に失敗") );
		return false;
	}
	CString strCityCodeTmp( ( VT_BSTR == varCityCode.vt ) ? varCityCode.bstrVal : _T("") );
	CString strAddrCodeTmp( ( VT_BSTR == varAddrCode.vt ) ? varAddrCode.bstrVal : _T("") );
	long lGaikuFugo( ( VT_I4 == varGaikuFugo.vt ) ? varGaikuFugo.lVal : -1 );

	// 住所コードが不正
	if( strCityCodeTmp.IsEmpty() || _T("00000") == strCityCodeTmp || strAddrCodeTmp.IsEmpty() || _T("000000") == strAddrCodeTmp )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("当該ポイントを内包する行政界ポリゴンの住所コードが不正です : ") + strCityCodeTmp + strAddrCodeTmp );
		return false;
	}

	// まず5桁を格納
	strAddrCode = strCityCodeTmp;

	// 大字・通称～字・丁目コードが「999999」でないなら
	if( _T("999999") != strAddrCodeTmp )
	{
		// 11桁まで格納
		strAddrCode += strAddrCodeTmp;
		// 街区符号が5桁までの正の整数なら16桁まで格納
		if( 0 < lGaikuFugo && lGaikuFugo < 100000 ) strAddrCode.AppendFormat( _T("%05ld"), lGaikuFugo );
	}

	return true;
}

bool BldNameTSVConverter::getBldgRepPoint( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, IPointPtr& ipPoint )
{
	// 建物ポリゴンフィーチャを取得
	IFeaturePtr ipFBuilding;
	if( FAILED( m_ipFCBuilding->GetFeature( lBldgOID, &ipFBuilding ) ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::sj::buildingname_point::kTableName, lOID, _T("建物ポリゴンフィーチャを取得できない : ") + CString( std::to_string( lBldgOID ).c_str() ) );
		return false;
	}

	// 建物ポリゴン形状を取得
	IGeometryPtr ipGBuilding;
	if( FAILED( ipFBuilding->get_Shape( &ipGBuilding ) ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T("建物ポリゴン形状を取得できない") );
		return false;
	}

	// 代表ポイントを取得
	if( !getRepPoint( ipGBuilding, ipPoint ) )
	{
		outputErrLog( LogMgr::kError, sindy::schema::building::kTableName, lBldgOID, _T("代表ポイントを取得できない") );
		return false;
	}

	return true;
}

bool BldNameTSVConverter::getRepPoint( const IPolygon4Ptr& ipPolygon, IPointPtr& ipPoint )
{
	// 空間参照の取得
	ISpatialReferencePtr ipSpRef;
	if( FAILED( ipPolygon->get_SpatialReference( &ipSpRef ) ) ) return false;

	// エクステリアリングの検査（1つだけかどうか）
	long lExteriorRings = 0;
	if( FAILED( ipPolygon->get_ExteriorRingCount( &lExteriorRings ) ) ) return false;
	if( 1 != lExteriorRings ) return false;

	// エクステリアリングの取得
	IRingPtr ipRingExterior;
	if( FAILED( ipPolygon->QueryExteriorRingsEx( 1, &ipRingExterior ) ) ) return false;

	// インテリアリング数の取得
	long lInteriorRings = 0;
	if( FAILED( ipPolygon->get_InteriorRingCount( ipRingExterior, &lInteriorRings ) ) ) return false;

	// エクステリアリング構成点数の取得
	long lExteriorPoints = 0;
	if( FAILED( ( (IPointCollectionPtr)ipRingExterior )->get_PointCount( &lExteriorPoints ) ) ) return false;

	// （インテリアリングを持たない）三角形の場合
	if( 0 == lInteriorRings && 4 == lExteriorPoints )
	{
		// 重心を取得して終了
		if( FAILED( ( (IAreaPtr)ipRingExterior )->get_Centroid( &ipPoint ) ) ) return false;
		return true;
	}

	// 対角線群の取得（長さの降順でコンテナに格納）
	DiagonalLenMap mapDiagonals;

	// リングのループ
	long lParts = 0;
	if( FAILED( ( (IGeometryCollectionPtr)ipPolygon )->get_GeometryCount( &lParts ) ) ) return false;
	for( long lRingIndex1 = 0; lRingIndex1 < lParts; ++lRingIndex1 )
	{
		// リング①取得
		IGeometryPtr ipGRing1;
		if( FAILED( ( (IGeometryCollectionPtr)ipPolygon )->get_Geometry( lRingIndex1, &ipGRing1 ) ) ) return false;

		// リング①の構成点数取得
		long lPoints1 = 0;
		if( FAILED( ( (IPointCollectionPtr)ipGRing1 )->get_PointCount( &lPoints1 ) ) ) return false;

		// エクステリアリングの場合、まずは自リング内での対角線を抽出
		VARIANT_BOOL vbIsExterior = VARIANT_FALSE;
		if( FAILED( ( (IRingPtr)ipGRing1 )->get_IsExterior( &vbIsExterior ) ) ) return false;
		if( vbIsExterior )
		{
			// 始点のループ
			for( long i = 0; i < lPoints1 - 2; ++i )
			{
				// 終点のループ
				for( long j = i + 2; j < lPoints1 - 1; ++j )
				{
					// 隣り合う構成点同士の場合はスキップ
					if( 0 == i && lPoints1 - 2 == j ) continue;

					// 対角線の生成とコンテナへの追加
					if( !createDiagonals( ipPolygon, ipGRing1, i, ipGRing1, j, ipSpRef, mapDiagonals ) ) return false;
				}
			}
		}

		// 他リングとの対角線も抽出
		for( long lRingIndex2 = lRingIndex1 + 1; lRingIndex2 < lParts; ++lRingIndex2 )
		{
			// リング②取得
			IGeometryPtr ipGRing2;
			if( FAILED( ( (IGeometryCollectionPtr)ipPolygon )->get_Geometry( lRingIndex2, &ipGRing2 ) ) ) return false;

			// リング②の構成点数取得
			long lPoints2 = 0;
			if( FAILED( ( (IPointCollectionPtr)ipGRing2 )->get_PointCount( &lPoints2 ) ) ) return false;

			// 始点のループ
			for( long i = 0; i < lPoints1 - 1; ++i )
			{
				// 終点のループ
				for( long j = 0; j < lPoints2 - 1; ++j )
				{
					// 対角線の生成とコンテナへの追加
					if( !createDiagonals( ipPolygon, ipGRing1, i, ipGRing2, j, ipSpRef, mapDiagonals ) ) return false;
				}
			}
		}
	}

	// 対角線群格納コンテナが空の場合は異常終了
	if( 0 == mapDiagonals.size() ) return false;

	// 対角線の長いほうから順に中点を取得、その点からある距離（閾値）内にポリゴンの境界線が存在しなければそれを代表点として返す
	// （代表点がポリゴンの境界付近に設定されることを防ぐ）
	IGeometryPtr ipGBoundary;	// ポリゴン境界線
	if( FAILED( ( (ITopologicalOperatorPtr)ipPolygon )->get_Boundary( &ipGBoundary ) ) ) return false;
	double dArea = 0.0;			// ポリゴン面積
	if( FAILED( ( (IAreaPtr)ipPolygon )->get_Area( &dArea ) ) ) return false;
	double dThreshold = sqrt( dArea ) / 20.0;	// 閾値: ポリゴン面積の平方根の5%
	IPointPtr ipPointMid( CLSID_Point ), ipPointOnEdge( CLSID_Point );
	for( const auto& rec : mapDiagonals )
	{
		if( FAILED( rec.second->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPointMid ) ) ) return false;
		double dDistAlongEdge = 0.0, dDistFromEdge = 0.0;
		VARIANT_BOOL vbIsRight = VARIANT_FALSE;
		if( FAILED( ( ( (IPolylinePtr)ipGBoundary )->QueryPointAndDistance( esriNoExtension, ipPointMid, VARIANT_TRUE, ipPointOnEdge, &dDistAlongEdge, &dDistFromEdge, &vbIsRight ) ) ) ) return false;
		if( dDistFromEdge > dThreshold )
		{
			ipPoint = ipPointMid;
			return true;
		}
	}

	// （ここまでで代表点を決定できない場合）最長の対角線より中点を選択
	const auto itr = mapDiagonals.begin();
	if( mapDiagonals.end() == itr ) return false;
	ipPoint = IPointPtr( CLSID_Point );
	if( FAILED( (*itr).second->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPoint ) ) ) return false;

	return true;
}

bool BldNameTSVConverter::createDiagonals( const IPolygon4Ptr& ipPolygon, const IRingPtr& ipRing1, long lIndex1, const IRingPtr& ipRing2, long lIndex2, const ISpatialReferencePtr& ipSpRef, DiagonalLenMap& mapDiagonals )
{
	// 構成点を取得
	IPointPtr ipPt1, ipPt2;
	if( FAILED( ( (IPointCollectionPtr)ipRing1 )->get_Point( lIndex1, &ipPt1 ) ) ) return false;
	if( FAILED( ( (IPointCollectionPtr)ipRing2 )->get_Point( lIndex2, &ipPt2 ) ) ) return false;

	// 対角線生成
	ILinePtr ipLine( CLSID_Line );
	if( FAILED( ipLine->putref_SpatialReference( ipSpRef ) ) ) return false;
	if( FAILED( ipLine->PutCoords( ipPt1, ipPt2 ) ) ) return false;

	// 対角線の長さを取得
	double dLength = 0.0;
	if( FAILED( ipLine->get_Length( &dLength ) ) ) return false;

	// ポリラインとして生成（ラインのままでは IRelationalOperator が利用できないので）
	IPolylinePtr ipPolyline( CLSID_Polyline );
	if( FAILED( ipPolyline->putref_SpatialReference( ipSpRef ) ) ) return false;
	if( FAILED( ( (IPointCollectionPtr)ipPolyline )->AddPoint( ipPt1 ) ) ) return false;
	if( FAILED( ( (IPointCollectionPtr)ipPolyline )->AddPoint( ipPt2 ) ) ) return false;

	// ポリゴンとの空間的関係をチェック
	VARIANT_BOOL vbContains;
	if( FAILED( ( (IRelationalOperatorPtr)ipPolygon )->Contains( ipPolyline, &vbContains ) ) ) return false;

	// 対角線がポリゴンに含まれている場合
	if( vbContains )
	{
		// 対角線をコンテナに登録
		mapDiagonals.insert( DiagonalLenMap::value_type( dLength, ipLine ) );
	}

	return true;
}
