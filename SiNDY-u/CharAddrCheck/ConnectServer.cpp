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
#include "ConnectServer.h"
#include "CheckConvert.h"
#include "OutputCharAddr.h"

#include <sindy/workspace.h> 
#include <sindy/schema.h>

CConnectServer::CConnectServer()
{
	m_bInfoAddr = false;
	OStartup();
}

CConnectServer::~CConnectServer()
{
	m_eOraDB.Close();
	OShutdown();
}

bool CConnectServer::run( LPCTSTR lpcszConf, int nCommandLine, LPTSTR lpcszCommandLine[] )
{
	ReadConf( lpcszConf, nCommandLine, lpcszCommandLine );

	// 設定チェック
	if( m_strSDEServer.GetLength() == 0 )
	{
		std::cout << "#ERROR\t接続するサーバ設定が記入されていません。\n";
		return false;
	}
	
	if( m_strPurposServer.IsEmpty() )
	{
		std::cout << "#ERROR\t接続する作業目的サーバ設定が記入されていません。\n";
		return false;
	}

	if( !PathFileExists( m_strConvertTable ) )
	{
		std::cout << "#ERROR\t変換テーブルが存在しません。\n";
		return false;
	}

	if( m_strAddrTable.IsEmpty() )
	{
		std::cout << "#ERROR\t住所テーブルがNULLです。\n";
		return false;
	}

	if( !PathIsDirectory( m_strOutput ) )
	{
		std::cout << "#ERROR\t出力先ディレクトリが存在しません：" << CT2CA(m_strOutput) << std::endl;
		return false;
	}

	if( m_bInfoAddr )
	{
		if( !CString(m_strGouPointText).IsEmpty() )
		{
			if( !PathFileExists( m_strGouPointText ) )
			{
				std::cout << "#ERROR\t号ポイントチェックリストが存在しません：" << CT2CA(m_strGouPointText) << std::endl;
				return false;
			}
		}
		if( !CString(m_strCityAdminText).IsEmpty() )
		{
			if( !PathFileExists( m_strCityAdminText ) )
			{
				std::cout << "#ERROR\t都市地図行政界チェックリストが存在しません：" << CT2CA(m_strCityAdminText) << std::endl;
				return false;
			}
		}
	}

	using namespace sindy;
	using namespace sindy::schema;
	std::cerr << "住所系サーバ：" << CT2A(m_strSDEServer) << "に接続中...\n";
	IWorkspacePtr ipAddrWorkspace = create_workspace(m_strSDEServer);
	if( ipAddrWorkspace )
		std::cerr << "接続しました。\n";
	else
	{
		std::cout << "#ERROR\t" << CT2A(m_strSDEServer) << "\tサーバ接続失敗。\n";
		return false;
	}
	std::cerr << "作業目的サーバ："<< CT2A(m_strPurposServer) <<"に接続中...\n";
	IWorkspacePtr ipWorkWorkspace = create_workspace(m_strPurposServer);
	if( ipWorkWorkspace )
		std::cerr << "接続しました。\n";
	else {
		std::cout << "#ERROR\t" << CT2A(m_strPurposServer) << "\tサーバ接続失敗。\n";
		return false;
	}

	IFeatureWorkspacePtr ipAddrFWorkspace(ipAddrWorkspace);
	IFeatureClassPtr ipGouPointClass, ipCityAdminClass;
	ITablePtr ipWorkPorposeTable;
	ipAddrFWorkspace->OpenFeatureClass(CComBSTR(schema::gou_point::kTableName), &ipGouPointClass);
	ipAddrFWorkspace->OpenFeatureClass(CComBSTR(schema::city_admin::kTableName), &ipCityAdminClass);
	IFeatureWorkspacePtr(ipWorkWorkspace)->OpenTable(CComBSTR(schema::workpurpose::kTableName), &ipWorkPorposeTable);

	if( !ipGouPointClass )
	{
		std::cout << "#Error\tフィーチャクラスオープン失敗\t号ポイント\n";
		return false;
	}
	if( !ipCityAdminClass )
	{
		std::cout << "#Error\tフィーチャクラスオープン失敗\t行政界\n";
		return false;
	}
	if( !ipWorkPorposeTable )
	{
		std::cout << "#Error\tテーブルオープン失敗\t作業目的テーブル\n";
		return false;
	}

	// 処理方法の選択
	bool bReturn = false;
	if( m_bInfoAddr )
	{
		std::cerr << "住所テーブル：" << CT2A(m_strAddrTable) << "に接続中...\n";
		if( OraDBConnect( m_strAddrTable ) )
		{
			COutputCharAddr cOutputCharAddr;
			cOutputCharAddr.Init( ipGouPointClass, ipCityAdminClass, m_eOraDB, m_strOutput, ipWorkPorposeTable );
			bReturn = cOutputCharAddr.run( m_strConvertTable, m_strGouPointText, m_strCityAdminText );
		}
		else
			std::cout << "#ERROR\t住所DBに接続できません: " << CT2A(m_strAddrTable) << std::endl;
	}
	else
	{
		CCheckConvert cCheckConvert;
		cCheckConvert.Init( ipGouPointClass, ipCityAdminClass, m_strConvertTable, m_strOutput );
		bReturn = cCheckConvert.run();
		if(bReturn){
			// NG項目があったらリスト作成まで行う
			if(cCheckConvert.get_GouCityAdminNG(m_strGouPointText, m_strCityAdminText)){
				std::cerr << "\n--NG出力があるので単語リスト出力モード再実行--\n";
				std::cerr << "住所テーブル：" << CT2A(m_strAddrTable) << "に接続中...\n";
				if( OraDBConnect( m_strAddrTable ) ){
					COutputCharAddr cOutputCharAddr;
					cOutputCharAddr.Init( ipGouPointClass, ipCityAdminClass, m_eOraDB, m_strOutput, ipWorkPorposeTable );
					bReturn = cOutputCharAddr.run( m_strConvertTable, m_strGouPointText, m_strCityAdminText );
				} else
					std::cout << "#ERROR\t住所DBに接続できません: " << CT2A(m_strAddrTable) << std::endl;
			} else {
				std::cerr << "\n--NG出力がないので終了します--\n";
			}
		}
	}

	return bReturn;
}


///////////////////////////////////////////////////////////
// config.txtの読み込み
///////////////////////////////////////////////////////////
void CConnectServer::ReadConf( LPCTSTR lpcszConf, int nCommandLine, LPTSTR lpszCommandLine[] )
{
	std::ifstream ifs(static_cast<CT2CW>(lpcszConf));
	char rStrLine[1024] = {0};
	while( ifs.getline( rStrLine, sizeof(rStrLine) ) )
	{
		CString strLine = rStrLine;
		if( strLine[0] == '#' )
			continue;

		if( strLine.Find(_T("SDESERVER") ) >= 0 )
		{
			strLine.Delete(0,strLine.Find(_T("="))+1);
			strLine.Remove(_T(' '));
			strLine.Remove(_T('	'));
			m_strSDEServer = strLine;
			continue;
		}

		if( strLine.Find(_T("CONVERTTABLE") ) >= 0 )
		{
			strLine.Delete(0,strLine.Find(_T("="))+1);
			strLine.Remove(_T(' '));
			strLine.Remove(_T('\t'));
			m_strConvertTable = strLine;
			continue;
		}

		if( strLine.Find(_T("ADDRTABLE") ) >= 0 )
		{
			strLine.Delete(0,strLine.Find(_T("="))+1);
			strLine.Remove(_T(' '));
			strLine.Remove(_T('\t'));
			m_strAddrTable = strLine;
			continue;
		}

		if( strLine.Find(_T("OUTPUT") ) >= 0 )
		{
			strLine.Delete(0,strLine.Find(_T("="))+1);
			strLine.Remove(_T(' '));
			strLine.Remove(_T('\t'));
			m_strOutput = strLine;
			continue;
		}
		
		if( strLine.Find(_T("WORKPURPOSSERVER") ) >= 0 )
		{
			strLine.Delete(0,strLine.Find(_T("="))+1);
			strLine.Remove(_T(' '));
			strLine.Remove(_T('\t'));
			m_strPurposServer = strLine;
			continue;
		}
	}

	for( int i = 2; i < nCommandLine; ++i )
	{
		CString strLine = lpszCommandLine[i];
		if( _tcscmp( strLine, _T("-g") ) == 0 || _tcscmp( strLine, _T("--goupoint.txt") ) == 0 )
			m_strGouPointText = lpszCommandLine[i+1];
		else if( _tcscmp( strLine, _T("-c") ) == 0 || _tcscmp( strLine, _T("--cityadmin.txt") ) == 0 )
			m_strCityAdminText = lpszCommandLine[i+1];
		else if( _tcscmp( strLine, _T("-i") ) == 0 || _tcscmp( strLine, _T("--infoaddr") ) == 0 )
			m_bInfoAddr = true;
	}
}

bool CConnectServer::OraDBConnect( LPCTSTR lpcszConnect )
{
	USES_CONVERSION;

	bool bRet = false, bCheck = false;
	TCHAR aUser[100], aPass[100], aService[100];
	TCHAR *aToken, aConTmp[CON_STR_SIZE];

	if( !CString(lpcszConnect).IsEmpty() )
	{
		_tcscpy( aConTmp, lpcszConnect );
		aToken = _tcstok( aConTmp, _T("/@") );
		if( aToken != NULL )
		{
			_tcscpy(aUser, aToken);
			aToken = _tcstok( NULL, _T("/@") );
			if( aToken != NULL )
			{
				_tcscpy( aPass, aToken );
				aToken = _tcstok( NULL, _T("/@") );
				if( aToken != NULL )
				{
					_tcscpy( aService, aToken );
					bCheck = true;
				}
			}
		}
	}

	if( bCheck && m_eOraDB.Open( CT2A(aService), CT2A(aUser), CT2A(aPass) ) == OSUCCESS )
		bRet = true;

	return bRet;
}
