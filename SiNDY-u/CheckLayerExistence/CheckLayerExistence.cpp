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


#include <stdio.h>

#include "stdafx.h"
#include "CheckLayerExistence.h"
#include <iterator>

using namespace std;


CCheckLayerExistence::CCheckLayerExistence()
{
	m_MeshSuccessFlag = 0;
	m_ModeFlag = 0;	
	m_ExeModeFlag = 0;
}

CCheckLayerExistence::~CCheckLayerExistence()
{

}

bool CCheckLayerExistence::init(int argc, char *argv[])
{
	if( !checkArg(argc, argv) )					// 引数解析
		return false;

	// ログファイル
	if( !m_cError.open(argv[argc-1]) )
	{
		cerr << "#Error ログファイルのオープンに失敗 : " << argv[argc-1] << endl;
		return false;
	}

	CString strVersion;
	strVersion.Format( _T("%s %s"), PROGNAME, TOOL_VERSION );
	m_cError.printHeader( strVersion, m_FileProp );

	return true;
}



void CCheckLayerExistence::printUsage()
{
	cerr
		<< "【PGDB内存在レイヤチェックツール" << TOOL_VERSION << "】\n"
		<< "(Usage)CheckLayerExistence.exe [option] [logfile]\n"
		<< "・チェック対象オプション\n"
		<< " -m [meshcode]		メッシュコード指定\n"
		<< " -l [meshlist]		メッシュリスト指定\n"
		<< " -n [filenamelist]	ファイル名リスト指定\n"
		<< " -s [layername]		存在チェックするレイヤ名指定\n"
		<< " -f [filename]		存在チェックするレイヤリスト名指定\n"
		<< " -c []			レイヤ毎の○×のみの出力モード指定\n"
		<< " -d []			存在しなかったレイヤを出力モード指定\n"
		<< "\n"
		<< "・環境変数について\n"
		<< " DB_ANY			PGDBファイル格納フォルダのパス\n"
		<< endl;
}

void CCheckLayerExistence::execute()
{
	if( m_ExeModeFlag == 2 )
		Mode_Meshcode();

	else if( m_ExeModeFlag == 3 )
		Mode_Filename();

}



void CCheckLayerExistence::Mode_Meshcode()
{
	CString ErrorMsg;
	for( vector<string>::iterator itr=m_vecMesh.begin(); itr!=m_vecMesh.end() ; ++itr )
	{
		m_FileName = (*itr).c_str();						// string型のメッシュコードをCString型に変換
		unsigned long meshnum = strtoul(m_FileName, NULL, 10);			// メッシュ番号をunsigned long型に変換したものを保管
		m_FileName += ".mdb";								// [メッシュ番号].mdb

		// メッシュコードから地図の種類を判別する
		CString strMeshFName;
		if( meshnum > 1000 && meshnum < 10000 )
		{
			strMeshFName = _T("MIDDLEMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName;		// PGDBファイルの絶対パス作成
		}
		else if( meshnum > 100000 && meshnum < 1000000 )
		{
			strMeshFName = _T("BASEMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName.Left(4) + "\\" + m_FileName;	// PGDBファイルの絶対パス作成
		}
		else if( meshnum > 10000000 && meshnum < 100000000 )
		{
			strMeshFName = _T("CITYMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName.Left(4) + "\\" + m_FileName;	// PGDBファイルの絶対パス作成
		}
		else if( meshnum > 999999999 && meshnum < 4000000000 )
		{
			//国際都市地図対応
			strMeshFName = _T("TERTIARYMESH");
			m_FilePass = m_FileProp + "\\" + m_FileName.Left(6) + "\\" + m_FileName.Left(8) + "\\" + m_FileName;	// PGDBファイルの絶対パス作成
		}
		else
		{
			ErrorMsg = "メッシュコードが不正です！";
			cerr << "#Error " << ErrorMsg << endl;
			m_cError.printPGDBOpenError(m_FileName, ErrorMsg);
			continue;
		}

		// ファイル名から必要情報を取得
		if( !setInfoFromPGDB() )
		{
			ErrorMsg = "PGDBファイルが存在しないか、アドレスが間違っています！";
			cerr << "#Error " << ErrorMsg << endl;
			m_cError.printPGDBOpenError(m_FilePass, ErrorMsg);
			continue;
		}
		
		// 実際のレイヤ有無チェック部分
		LayerCheck();
	}
}

void CCheckLayerExistence::Mode_Filename()
{
	CString ErrorMsg;
	for( vector<string>::iterator itr=m_vecMesh.begin(); itr!=m_vecMesh.end() ; ++itr )
	{
		m_FileName = (*itr).c_str();						// string型のメッシュコードをCString型に変換

		m_FilePass = m_FileProp + "\\" + m_FileName;

		// ファイル名から必要情報を取得
		if( !setInfoFromPGDB() )
		{
			ErrorMsg = "PGDBファイルが存在しないか、アドレスが間違っています！";
			cerr << "#Error " << ErrorMsg << endl;
			m_cError.printPGDBOpenError(m_FilePass, ErrorMsg);
			continue;
		}
		
		// 実際のレイヤ有無チェック部分
		LayerCheck();
	}
}


bool CCheckLayerExistence::checkArg(int argc, char *argv[])
{
	for( int i=1 ; i<argc ; i++ )
	{
		if( strcmp(argv[i], "-m") == 0 )
		{
			m_ExeModeFlag += 2;
			string temp( argv[++i] );
			m_vecMesh.push_back( temp );
		}
		if( strcmp(argv[i], "-l") == 0 )
		{
			m_ExeModeFlag += 2;
			if( !loadMeshlist(argv[++i]) )
				return false;
		}
		if( strcmp(argv[i], "-n") == 0 )
		{
			m_ExeModeFlag += 3;
			if( !loadMeshlist(argv[++i]) )
				return false;
		}
		if( strcmp(argv[i], "-s") == 0 )
		{
			string temp( argv[++i] );
			m_vecLayerName.push_back( temp );
		}
		if( strcmp(argv[i], "-f") == 0 )
		{
			if( !loadLayerlist(argv[++i]) )
				return false;
		}
		if( strcmp(argv[i], "-c") == 0 )
		{
			m_ModeFlag = 1;
		}
		if( strcmp(argv[i], "-d") == 0 )
		{
			m_ModeFlag = 2;
		}

	}
	if( m_vecMesh.empty() )
	{
		cerr << "PGDBリストやファイルが指定されていません" << endl;
		return false;
	}
	if( m_vecLayerName.empty() )
	{
		cerr << "探索レイヤが指定されていません" << endl;
		return false;
	}
	if( m_ModeFlag != 1 && m_ModeFlag != 2 )
	{
		cerr << "出力モードが指定されていません" << endl;
		return false;
	}
	if( m_ExeModeFlag == 0 )
	{
		cerr << "実行モードが指定されていません" << endl;
		return false;
	}
	if( m_ExeModeFlag != 2 && m_ExeModeFlag != 3 )
	{
		cerr << "実行モード指定が不正です" << endl;
		return false;
	}

	return true;
}

bool CCheckLayerExistence::loadMeshlist(LPCTSTR lpszFile)
{
	m_MeshlistName = lpszFile;
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	
	// string型でメッシュコードを格納
	copy(
		std::istream_iterator<string>(ifs),
		std::istream_iterator<string>(),
		std::back_inserter(m_vecMesh)
		);

	return ifs.eof();
}

bool CCheckLayerExistence::loadLayerlist(LPCTSTR lpszFile)
{

	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if( !ifs )
		return false;
	
	copy(
		std::istream_iterator<string>(ifs),
		std::istream_iterator<string>(),
		std::back_inserter(m_vecLayerName)
		);
	
	return ifs.eof();

}

bool CCheckLayerExistence::getEnv()
{

#ifdef _DEBUG
	_tputenv( _T("DB_ANY=C:\\svn\\SiNDY-u\\CheckLayerExistence\\test") );
	
#endif

	CString strError(_T(""));
	if(!m_FileProp.GetEnvironmentVariable( _T("DB_ANY")) )
		strError += _T("#Error 環境変数 DB_ANY の取得に失敗\n");
	
	if( !strError.IsEmpty() )
	{
		std::cerr << strError;
		return false;
	}
	else
		return true;
}

bool CCheckLayerExistence::setInfoFromPGDB()
{
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );

	USES_CONVERSION;

	// 接続先の表示
	CComVariant vaPGDB( m_FilePass );
	cerr << "\t接続PGDB ： " << OLE2T( vaPGDB.bstrVal ) << endl;

	// 接続確認
	cerr << "\t接続中・・・";
	if( ipWorkspaceFactory->OpenFromFile( vaPGDB.bstrVal, NULL, &m_ipPGDBWorkspace ) != S_OK){
		cerr << "接続できませんでした" << endl;
		return false;
	}
	cerr << "接続しました" << endl;


	return true;
}

void CCheckLayerExistence::LayerCheck()
{
	for(vector<string>::iterator itr = m_vecLayerName.begin() ; itr != m_vecLayerName.end() ; ++itr)
	{
		// ユーザのレイヤ名大文字に変換する

		// 実際にレイヤオープンを試みることで、レイヤが存在するかしないかを判別する
		CString temp = (*itr).c_str();
		IFeatureWorkspacePtr(m_ipPGDBWorkspace)->OpenFeatureClass( CComBSTR(temp), &m_PGDBFC );  
		if( m_PGDBFC == NULL )
		{
			if(m_ModeFlag == 2)
				m_cError.printLog( m_FileName, *itr );
			if(m_ModeFlag == 1)
			{
				m_MeshSuccessFlag =1;
				break;
			}
		}
	}
	if(m_ModeFlag == 1)
		m_cError.printLog( m_FileName, m_MeshSuccessFlag );

	m_MeshSuccessFlag = 0;
}


