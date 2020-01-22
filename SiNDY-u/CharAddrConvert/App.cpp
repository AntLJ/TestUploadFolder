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
#include "App.h"
#include <io.h>
#include <conio.h>
#include "DataOutputter.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

CCharAddrConvert g_cYomiHelper;

CApp::CApp(void) : 
	m_emListMode(kAll)
{
}

CApp::~CApp(void)
{
}

// 使用法
void CApp::PrintUsage()
{
	std::cerr << "[文字付き住所データ変換ツール]\n"
			  << "CharAddrConvert.exe [各種オプション]\n"
			  << "・オプションについて\n"
			  << "  -i [設定ファイル] ... 設定ファイルを指定(必須)\n"
			  << "  (実行エリア指定オプション) ... デフォルトは、全国)\n"
			  << "  -ca ... 全件\n"
			  << "  -cr [開始市区町村コード-終了市区町村コード] ... エリア指定\n"
			  << "  -cl [市区町村リストファイル] ... リスト指定\n"
			  << "  -cs [市区町村コード] ... 単発\n"
			  << "※ログは、標準エラー出力から取得して下さい" << std::endl;
}

// 初期化
bool CApp::Init( int argc, _TCHAR* argv[] )
{
	// コマンドライン解析
	if( argc < 3 )
		return progress_out::ProgressError<bool>( false, kError, "引数の数が不正です" );

	CString strInitFile;
	for( int i = 1; i < argc - 1; ++i )
	{
		if( _tcscmp(argv[i], _T("-ca")) == 0 )
		{
			m_emListMode = kAll;
		}
		else if( _tcscmp(argv[i], _T("-cr")) == 0 )
		{
			m_emListMode = kRange;
			m_strListInfo = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-cl")) == 0 )
		{
			m_emListMode = kList;
			m_strListInfo = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-cs")) == 0 )
		{
			m_emListMode = kSingle;
			m_strListInfo = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-i")) == 0 )
		{
			strInitFile = argv[++i];
		}
		else
			return progress_out::ProgressError<bool>( false, kError, "不正なオプション : %s", argv[i] );
	}

	return LoadInitFile( strInitFile );	// 最後に設定ファイル読み込み
}

// 実行
void CApp::Run()
{
	progress_out::ProgressTime( _T("処理開始") );
	object_out::LogHeader();	// ログヘッダー出力

	// 市区町村ごとにデータ変換開始
	for( std::set<CString>::const_iterator itr = m_setCode.begin(); itr != m_setCode.end(); ++itr )
	{
		object_out::g_strCurrentCityCode = *itr;
		progress_out::Progress( _T("%s : 変換開始"), *itr );

		// 指定5桁以下の都市地図行政界取得
		CString strWhere;
		strWhere.Format( _T("%s='%s'"), city_admin::kCityCode, *itr );
		long lAdminCount = 0;
		IFeatureCursorPtr ipFeatureCursor = gf::SelectByAttr( m_ipCityAdminFC, &lAdminCount, strWhere );
		if( ipFeatureCursor )
		{
			PNTMAP PntMap;	// Pntレコード格納コンテナ
			GOUMAP GouMap;	// Gouレコード格納コンテナ

			long lIndex = 1;
			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
			{
				CAdminInfo cAdminInfo( ipFeature, m_cAdminFID );

				// 11桁有効化チェック
				if( m_cMakeAddrRec.IsValidAddrCode(cAdminInfo.m_strAddrCode) )
				{
					if( !cAdminInfo.m_strExtGaiku.IsEmpty() )
					{
						// ヨミが取得できていないなら、当該行政界内の号ポイントをデータ化しない
						if( cAdminInfo.m_strExtGaikuYomi.IsEmpty() )
						{
							object_out::ObjectError( m_cParam.m_strCityAdminFCName.c_str(), cAdminInfo.GetOID(), "拡張街区符号があるが、ヨミの取得に失敗したのでデータ化できない\t%s", cAdminInfo.m_strExtGaiku );
							++lIndex;
							continue;
						}

						// 住所整備種別取得
						switch( cAdminInfo.m_lAddrClass_C )
						{
						case city_admin::addr_class::kResidenceIndicationMaintenance:	// 住居表示整備済み
							m_cMakeAddrRec.MakePntRecordByAdmin( cAdminInfo, PntMap );	// 行政界から代表点を生成
							break;
						case city_admin::addr_class::kAzaUndeveloped:
						case city_admin::addr_class::kChibanMaintenance:
							object_out::ObjectError( m_cParam.m_strCityAdminFCName.c_str(), cAdminInfo.GetOID(), "拡張街区符号があって、住所整備種別が[町丁目界未整備|地番整備済み]はNGです" );
							++lIndex;
							continue;
						default:
							break;
						}
					}

					// 行政界以下にある号ポイントを20桁化(文字付きに限り)
					IFeatureCursorPtr ipGouCursor = gf::SelectByShape( cAdminInfo.GetGeometry(), m_ipGouPointFC, NULL, esriSpatialRelIntersects );
					if( ipGouCursor )
					{
						IFeaturePtr ipGouFeature;					
						while( ipGouCursor->NextFeature(&ipGouFeature) == S_OK )
						{
							m_cMakeAddrRec.MakeGouRecordByDB( cAdminInfo, CGouInfo(ipGouFeature, m_cGouFID), GouMap );
						}
					}
				}
				else
					object_out::ObjectError( m_cParam.m_strCityAdminFCName.c_str(), cAdminInfo.GetOID(), "11桁コードが住所マスタに存在しない or 廃止コードです\t%s", cAdminInfo.m_strAddrCode );

				std::cerr << lIndex++ << " / " << lAdminCount << " 件 行政界内の変換終了\r";
			}

			std::cerr << std::endl;

			// 地番データ生成
			m_cMakeAddrRec.MakePntRecordByGou( GouMap, PntMap );

			// 号データ生成(同一20桁から代表点を生成する)
			GOUMAP RepGouMap;
			m_cMakeAddrRec.SelectRepGouRecord( GouMap, RepGouMap );

			// データ書き出し
			if( !WriteData(*itr, PntMap, RepGouMap) )
				return;
		}

		progress_out::Progress( _T("%s : 変換終了"), *itr );

		// 大文字[Q]押されていたら途中中断
		if( _kbhit() && 'Q' == _getch() )
		{
			progress_out::Progress( _T("[Q]が押されました。変換を中断します。") );
			break;
		}
	}

	progress_out::ProgressTime( _T("処理終了") );
}

/////////////////////////////////////////
// 内部関数
/////////////////////////////////////////

// 設定ファイル読み込み
bool CApp::LoadInitFile( LPCTSTR lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
		return progress_out::ProgressError<bool>( false, kError, "設定ファイルをオープンできませんでした : %s", lpcszFile );

	po::options_description config_file( "設定ファイル" );
	config_file.add_options()
		("CONV_MODE",        po::value<int>()->default_value(kNormal),                  "データ出力モード")
		("DB_ADDRESS",       po::value<std::string>(&m_cParam.m_strAddrDBProp),         "住所系SDE接続プロパティ")
		("DB_MAP",           po::value<std::string>(&m_cParam.m_strMapDBProp),          "地図系SDE接続プロパティ")
		("FC_CITY_ADMIN",    po::value<std::string>(&m_cParam.m_strCityAdminFCName),    "都市地図行政界フィーチャクラス名")
		("FC_GOU_POINT",     po::value<std::string>(&m_cParam.m_strGouPointFCName),     "号ポイントフィーチャクラス名")
		("FC_CITY_SITE",     po::value<std::string>(&m_cParam.m_strCitySiteFCName),     "都市地図背景フィーチャクラス名")
		("FC_BUILDING",      po::value<std::string>(&m_cParam.m_strBuildingFCName),     "都市地図建物フィーチャクラス名")
		("FC_BUILDING_ROOF", po::value<std::string>(&m_cParam.m_strBuildingRoofFCName), "都市地図屋上ポリゴンフィーチャクラス名")
		("FC_CITYMESH",      po::value<std::string>(&m_cParam.m_strCityMeshFCName),     "都市地図メッシュフィーチャクラス名")
		("DB_CODECONV",      po::value<std::string>(&m_cParam.m_strCodeConvDB),         "住所コード変換DB")
		("DB_YOMICONV",      po::value<std::string>(&m_cParam.m_strYomiConvDB),         "住所コード変換DB")
		("JDB_CONNECT",      po::value<std::string>(&m_cParam.m_strJusyoDB),            "住所DB接続プロパティ")
		("PNT_DATA_DIR",     po::value<std::string>(&m_cParam.m_strPntDir),             "pntexデータ出力先")
		("GOU_DATA_DIR",     po::value<std::string>(&m_cParam.m_strGouDir),             "gouexデータ出力先");

	po::variables_map vm;
	try
	{
		po::store( po::parse_config_file(ifs, config_file), vm );
	}
	catch( std::exception& e )
	{
		return progress_out::ProgressError<bool>( false, kError, "設定ファイルの解析に失敗 [%s]", e.what() );
	}

	po::notify( vm );

	// 変換モード(Bug7599)
	if( ! (0 < vm["CONV_MODE"].as<int>() && vm["CONV_MODE"].as<int>() < 3) )
		return progress_out::ProgressError<bool>( false, kError, "変換モードが許容外の値 : %d", vm["CONV_MODE"].as<int>() );
	m_cParam.m_emConvMode = static_cast<emConvMode>(vm["CONV_MODE"].as<int>());

	progress_out::Progress( "変換モード : %s", (kNormalConv == m_cParam.m_emConvMode? "ナビ向け" : "外部向け") );

	// DB接続・フィーチャクラス取得
	m_ipAddrWorkspace = gf::GetWorkspace( m_cParam.m_strAddrDBProp.c_str() );
	if( NULL == m_ipAddrWorkspace )
		return progress_out::ProgressError<bool>( false, kError, "住所系SDEへの接続に失敗 : %s", m_cParam.m_strAddrDBProp.c_str() );

	progress_out::Progress( "住所系SDEへ接続 : %s", m_cParam.m_strAddrDBProp.c_str() );

	// フィーチャクラス取得
	((IFeatureWorkspacePtr)m_ipAddrWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strCityAdminFCName.c_str()), &m_ipCityAdminFC );
	if( m_ipCityAdminFC )
		m_cAdminFID.CreateFieldMap( (ITablePtr)m_ipCityAdminFC );
	else
		return progress_out::ProgressError<bool>( false, kError, "都市地図行政界フィーチャクラスの取得に失敗 : %s", m_cParam.m_strCityAdminFCName.c_str() );

	((IFeatureWorkspacePtr)m_ipAddrWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strGouPointFCName.c_str()), &m_ipGouPointFC );
	if( m_ipGouPointFC )
		m_cGouFID.CreateFieldMap( (ITablePtr)m_ipGouPointFC );
	else
		return progress_out::ProgressError<bool>( false, kError, "号ポイントフィーチャクラスの取得に失敗 : %s", m_cParam.m_strGouPointFCName.c_str() );

	m_ipMapWorkspace = gf::GetWorkspace( m_cParam.m_strMapDBProp.c_str() );
	if( !m_ipMapWorkspace )
		return progress_out::ProgressError<bool>( false, kError, "地図系SDEへの接続に失敗 : %s", m_cParam.m_strMapDBProp.c_str() );

	progress_out::Progress( "地図系SDEへ接続 : %s", m_cParam.m_strMapDBProp.c_str() );

	// フィーチャクラス取得
	IFeatureClassPtr ipCitySiteFC, ipBuildingFC, ipBuildingRoofFC, ipCityMeshFC;
	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strCitySiteFCName.c_str()), &ipCitySiteFC );
	if( !ipCitySiteFC )
		return progress_out::ProgressError<bool>( false, kError, "都市地図背景ポリゴンフィーチャクラスの取得に失敗 : %s", m_cParam.m_strCitySiteFCName.c_str() );

	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strBuildingFCName.c_str()), &ipBuildingFC );
	if( !ipBuildingFC )
		return progress_out::ProgressError<bool>( false, kError, "都市地図建物ポリゴンフィーチャクラスの取得に失敗 : %s", m_cParam.m_strBuildingFCName.c_str() );
	
	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strBuildingRoofFCName.c_str()), &ipBuildingRoofFC );
	if( !ipBuildingRoofFC )
		return progress_out::ProgressError<bool>( false, kError, "都市地図屋上ポリゴンフィーチャクラスの取得に失敗 : %s", m_cParam.m_strBuildingRoofFCName.c_str() );

	((IFeatureWorkspacePtr)m_ipMapWorkspace)->OpenFeatureClass( CComBSTR(m_cParam.m_strCityMeshFCName.c_str()), &ipCityMeshFC );
	if( !ipCityMeshFC )
		return progress_out::ProgressError<bool>( false, kError, "都市地図メッシュポリゴンフィーチャクラスの取得に失敗 : %s", m_cParam.m_strCityMeshFCName.c_str() );

	// コード変換クラス初期化
	if( !m_cMakeAddrRec.Init(m_ipCityAdminFC, ipCitySiteFC, ipBuildingFC, ipBuildingRoofFC, ipCityMeshFC, m_cParam.m_strCodeConvDB.c_str(), m_cParam.m_strJusyoDB.c_str()) )
		return false;

	// ヨミ変換クラス初期化
	std::multimap<long, CString> mapError = g_cYomiHelper.Init( m_cParam.m_strYomiConvDB.c_str() );
	if( !mapError.empty() )
	{
		// エラー有り
		for( std::multimap<long, CString>::const_iterator itr = mapError.begin(); itr != mapError.end(); ++itr )
			progress_out::ProgressError<int>( 0, kError, "%s : %s ", itr->second, m_cParam.m_strYomiConvDB.c_str() );

		return false;
	}

	// 市区町村コードリスト生成
	if( !MakeCityCodeList() )
		return false;

	// pntexデータ出力先の存在確認(なければ作成)
	if( _access_s(m_cParam.m_strPntDir.c_str(), 0) != 0 )
	{
		if( !CreateDirectory(m_cParam.m_strPntDir.c_str(), NULL) )
			return progress_out::ProgressError<bool>( false, kError, "pntex出力ディレクトリが作成できません : %s", m_cParam.m_strPntDir.c_str() );	
	}

	// gouexデータ出力先の存在確認(なければ作成)
	if( _access_s(m_cParam.m_strGouDir.c_str(), 0) != 0 )
	{
		if( !CreateDirectory(m_cParam.m_strGouDir.c_str(), NULL) )
			return progress_out::ProgressError<bool>( false, kError, "gouex出力ディレクトリが作成できません : %s", m_cParam.m_strGouDir.c_str() );
	}
	
	return true;
}

// 市区町村コードリスト作成
bool CApp::MakeCityCodeList()
{
	switch( m_emListMode )
	{
	case kAll:
	case kRange:
		{
			CString strWhere;
			if( kAll == m_emListMode )
				strWhere.Format( _T("%s != '00000'"), city_admin::kCityCode );
			else
			{
				// 範囲を取得
				int nPos = m_strListInfo.Find('-');
				if( -1 != nPos )
					strWhere.Format( _T("%s between '%s' and '%s'"), city_admin::kCityCode, m_strListInfo.Left(nPos), m_strListInfo.Mid(nPos+1) );
				else
					return progress_out::ProgressError<bool>( false, kError, "市区町村範囲指定方法がおかしい(<code>-<code>) : %s", m_strListInfo );
			}

			// 行政界検索
			IQueryDefPtr	ipQueryDef;
			IFeatureWorkspacePtr(m_ipAddrWorkspace)->CreateQueryDef( &ipQueryDef );
			ipQueryDef->put_Tables( _bstr_t(_T(m_cParam.m_strCityAdminFCName.c_str())) );
			ipQueryDef->put_WhereClause( _bstr_t(strWhere) );
			CString strSubFields;
			strSubFields.Format( _T("distinct(%s)"), city_admin::kCityCode );
			ipQueryDef->put_SubFields( CComBSTR(strSubFields) );

			_ICursorPtr ipCursor;
			if( SUCCEEDED(ipQueryDef->Evaluate(&ipCursor)) && ipCursor )
			{
				_IRowPtr ipRow;
				while( ipCursor->NextRow(&ipRow) == S_OK )
				{
					CComVariant vaValue;
					ipRow->get_Value( 0, &vaValue );
					if( VT_BSTR == vaValue.vt )
						m_setCode.insert( CString(vaValue.bstrVal) );
				}
			}
		}
		break;
	case kList:
		{
			// リストから市区町村コード取得
			std::ifstream ifs( static_cast<CT2CW>(m_strListInfo) );
			if( ifs )
			{
				std::string strBuff;
				while( std::getline(ifs, strBuff) ){ m_setCode.insert( strBuff.c_str() ); }
			}
			else
				return progress_out::ProgressError<bool>( false, kError, "市区町村リストファイルの読み込みに失敗 : %s", m_strListInfo );
		}
		break;
	case kSingle:
		m_setCode.insert( m_strListInfo );
		break;
	}
	return true;
}

// データ書き出し
bool CApp::WriteData( LPCTSTR lpcszCityCode, const PNTMAP& rPntList, const GOUMAP& rGouList )
{
	// pntex
	CString strFile;
	if( !rPntList.empty() )
	{
		strFile.Format( _T("%s\\pntex%s.txt"), m_cParam.m_strPntDir.c_str(), lpcszCityCode );
		std::ofstream ofspnt( static_cast<CT2CW>(strFile), std::ios::out | std::ios::binary );
		if( ofspnt )
		{
			// 変換モードで場合わけ(Bug7599)
			if( kNormalConv == m_cParam.m_emConvMode )
				data_output::OutPntFile( ofspnt, rPntList );
			else
				data_output::OutPntFile_Outside( ofspnt, rPntList );
		}
		else
			return progress_out::ProgressError<bool>( false, kError, "出力用pntexファイルのオープンに失敗 : %s", strFile );
	}

	// gouex
	if( !rGouList.empty() )
	{
		strFile.Format( _T("%s\\gouex%s.txt"), m_cParam.m_strGouDir.c_str(), lpcszCityCode );
		std::ofstream ofsgou( static_cast<CT2CW>(strFile), std::ios::out | std::ios::binary );
		if( ofsgou )
		{
			// 変換モードで場合わけ(Bug7599)
			if( kNormalConv == m_cParam.m_emConvMode )
				data_output::OutGouFile( ofsgou, rGouList );
			else
				data_output::OutGouFile_Outside( ofsgou, rGouList );
		}
		else
			return progress_out::ProgressError<bool>( false, kError, "出力用gouexファイルのオープンに失敗 : %s", strFile );
	}
	return true;
}
