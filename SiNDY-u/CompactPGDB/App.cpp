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

#include <algorithm>
#include <iterator>

CApp::CApp(void)
{
}

CApp::~CApp(void)
{
}

// 使用法
bool CApp::printUsage()
{
	std::wcerr
		<< L"【SiNDY-c用PGDB 最適化ツール" << TOOL_VERSION << L"】\n"
		<< L"(Usage)CompactPGDB.exe [オプション] (> 進捗ログ)\n"
		<< L"・オプションについて\n"
		<< L"  -f PGDBファイル名\n"
		<< L"  -d PGDBベースディレクトリ\n"
		<< L"  -l メッシュリスト(-b オプション指定時必須)\n" << std::endl;

	return false;
}

// 初期化
bool CApp::init( int argc, TCHAR* argv[] )
{
	if( !checkArg(argc, argv) )
		return false;

	// 最適化ファイルリストを作成する
	if( !m_strBaseDir.IsEmpty() ){
		
		for( std::vector<long>::const_iterator itr=m_vecMesh.begin();itr!=m_vecMesh.end();++itr )
		{
			TCHAR szPGDBFile[_MAX_PATH] = {0};
			_stprintf( szPGDBFile, _T("%s\\%d\\%d.mdb"), m_strBaseDir, (*itr)/10000, *itr );
			m_vecFile.push_back( szPGDBFile );
		}
	}

	return true;
}

// 実行
bool CApp::execute()
{
	for( std::vector<CString>::const_iterator itr=m_vecFile.begin();itr!=m_vecFile.end();++itr )
	{
		compact(*itr);
	}

	return true;
}

// コンパクト化
bool CApp::compact( LPCTSTR lpszFile )
{
	std::wcout << lpszFile << L" : 最適化中・・・・";

	// PGDBへ接続
	IWorkspacePtr ipWorkspace;
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	if( FAILED(ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszFile), NULL, &ipWorkspace)) ){
		std::wcout << L"PGDBへの接続に失敗" << std::endl;
		return false;
	}

	IDatabaseCompactPtr ipDataBaseCompact( ipWorkspace );
	if( NULL == ipDataBaseCompact ){
		std::wcout << L"失敗(ipDataBaseCompact==NULL)" << std::endl;
		return false;
	}

	VARIANT_BOOL vb;
	ipDataBaseCompact->CanCompact( &vb );
	if( VARIANT_TRUE == vb ){
		ipDataBaseCompact->Compact();
		std::wcout << L"完了" << std::endl;
	}
	else
		std::wcout << L"最適化できません" << std::endl;

	return true;
}

//////////////////////////////////////
//	非公開関数
//////////////////////////////////////

// 引数チェック
bool CApp::checkArg(int argc, TCHAR *argv[])
{
	if( argc < 3 )
		return false;

	for( int i=1;i<argc-1;++i )
	{
		if( _tcscmp(argv[i], _T("-f")) == 0 ){
			m_vecFile.push_back( argv[++i] );
		}
		else if( _tcscmp(argv[i], _T("-d")) == 0 ){
			m_strBaseDir = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-l")) == 0 ){
			
			if( !loadMeshList(argv[++i]) ){
				std::wcerr << L"#ERROR メッシュリストの読み込みに失敗 : " << argv[i] << std::endl;
				return false;
			}

		}
	}

	return true;
}

// メッシュリスト読み込み
bool CApp::loadMeshList( LPCTSTR lpszFile )
{
	std::ifstream ifs( lpszFile );
	if( !ifs )
		return false;
	copy(
		std::istream_iterator<long>(ifs),
		std::istream_iterator<long>(),
		std::back_inserter(m_vecMesh)
		);

	return true;
}
