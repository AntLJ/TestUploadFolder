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
#include "Pyramid.h"
using namespace std;

namespace{
	const long PATH_LEN = 1024;
}

// コンストラクタ
CPyramid::CPyramid(void)
	: m_bBatch(false), m_lDir(0), m_lExtension(0), m_lMode(1){
		char szCurrent[PATH_LEN] = {};
		::GetCurrentDirectory( PATH_LEN, szCurrent );
		m_strCurrent.Format(_T("%s"), szCurrent);
		m_strIniPath.Format(_T("%s\\Common.ini"), szCurrent);
		m_strMeshlist.Format(_T("meshlist.txt"));
}

// メッシュリスト指定で処理する
void CPyramid::TargetMeshList( bool bOption )
{
	char szDir[PATH_LEN] = {};
	char szMeshPath[PATH_LEN] = {};
	char szExtension[16] = {};
	CString strExtension;
	cout<<"\n拡張子を指定してください。（例：「jpg」「png」）\n\t-> ";
	if( bOption )
	{
		switch( m_lExtension )
		{
		case 0:
			strExtension.Format(_T("jpg"));
			break;
		case 1:
			strExtension.Format(_T("png"));
			break;
		case 2:
			strExtension.Format(_T("bmp"));
			break;
		default:
			strExtension.Format(_T("jpg"));
			break;
		}
		cout<<strExtension<<endl;
	}
	else
	{
		cin>>szExtension;
		strExtension.Format(_T("%s"), szExtension);
	}

	cout<<"メッシュリストファイルを指定してください。\n\t->";
	char* pMeshPath = m_strCurrent.GetBuffer( m_strCurrent.GetLength() + m_strMeshlist.GetLength() + 1 );
	m_strCurrent.ReleaseBuffer();
	CString strMeshPath;
	if( bOption )
	{
		// 絶対パスか相対パスか
		if( strMeshPath.Left(3).Find(_T(":\\")) || strMeshPath.Left(3).Find(_T("\\\\")) || strMeshPath.Left(3).Find(_T(":/")) || strMeshPath.Left(3).Find(_T("//")) )
		{
			strMeshPath = m_strMeshlist;
		}
		else
		{
			PathAppend( pMeshPath, m_strMeshlist );
			strMeshPath.Format(_T("%s"), pMeshPath);
		}
		cout<<m_strMeshlist<<endl;
	}
	else
	{
		cin>>szMeshPath;
		strMeshPath.Format(_T("%s"), szMeshPath);
		// 絶対パスか相対パスか
		if( strMeshPath.Left(3).Find(_T(":\\")) || strMeshPath.Left(3).Find(_T("\\\\")) || strMeshPath.Left(3).Find(_T(":/")) || strMeshPath.Left(3).Find(_T("//")) )
		{
		}
		else
		{
			PathAppend( pMeshPath, szMeshPath );
			strMeshPath.Format(_T("%s"), pMeshPath);
		}
	}

	if( !PathFileExists( strMeshPath ) )
	{
		cout<<strMeshPath<<" はありません。"<<endl;
		return;
	}

	char szDirStruct[PATH_LEN] = {0};
	CString strDirStruct;
	cout<<"ディレクトリ構造"<<endl;
	cout<<"  [ 1 ]「./4桁/8桁.jpg」構造 or 「./4桁/6桁.jpg」"<<endl;
	cout<<"  [ 2 ]「./8桁.jpg」構造 or 「./6桁.jpg」構造"<<endl;
	cout<<"  [ 3 ]「./6桁/10桁.jpg」構造"<<endl;
	cout<<"\t 1 or 2 or 3 ??  : ";
	if( bOption )
	{
		if(m_lDir ==  1)
			cout<<"2"<<endl;
		else if(m_lDir == 0)
			cout<<"1"<<endl;
		else if(m_lDir == 2)
			cout<<"3"<<endl;
	}
	else
	{
		cin>>szDirStruct;
		strDirStruct.Format(_T("%s"), szDirStruct);
		if( strDirStruct.Left(1) == _T("1") )
			m_lDir = 0;
		else if( strDirStruct.Left(1) == _T("2") )
			m_lDir = 1;
		else if( strDirStruct.Left(1) == _T("3") )
			m_lDir = 2;
	}

	if( !PathFileExists( m_strIniPath ) )
	{
		cout<<"設定ファイル:"<<m_strIniPath<<"はありません。"<<endl;
		return;
	}

	switch( m_lDir ){
		case 0:
			::GetPrivateProfileString( _T("1"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
			break;
		case 1:
			::GetPrivateProfileString( _T("3"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
			break;
		case 2:
			::GetPrivateProfileString( _T("4"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
			break;
		default:
			break;
	}

	FILE *fin;
	fopen_s( &fin, strMeshPath, "r" );
	if( fin == NULL )
	{
		cout<<"\n!! メッシュリストファイルは存在しますが、オープンすることが出来ませんでした。"<<endl;
		return;
	}
	char szMesh[PATH_LEN] = {};
	std::list<CString> listMesh;
	while( fgets( szMesh, PATH_LEN, fin ) != NULL )
	{
		unsigned int iMesh = _atoi64(szMesh);
		CString strMesh;
		strMesh.Format(_T("%u"), iMesh);
		listMesh.push_back(strMesh);
	}
	fclose(fin);

	PathAddBackslash( szDir );

	cout<<"　◎ピラミッドファイル作成中"<<endl;

	IWorkspaceFactoryPtr ipWF;
	if( SUCCEEDED( ipWF.CreateInstance( CLSID_RasterWorkspaceFactory)))
	{
		// ラスタファイルのあるフォルダを指定
		IWorkspacePtr ipWS;
		if( SUCCEEDED( ipWF->OpenFromFile( CComBSTR( szDir ), NULL, &ipWS)))
		{
			// ラスタファイルの読み込み
			IRasterWorkspacePtr ipRasterWS(ipWS);
			IRasterDatasetPtr ipRasterDataset;

			int i = 0;

			for( std::list<CString>::iterator it = listMesh.begin(); it != listMesh.end(); it++ )
			{
				CString strMesh, strBase;
				strMesh = *it;
				if( strMesh.IsEmpty() )
					continue;

				if(m_lDir==2)
					strBase = strMesh.Left(6);
				else
					strBase = strMesh.Left(4);
				i++;
				list<CString> listFindFile;
				CString strTmp;
				CString strFindFile;
				switch( m_lDir ){
					case 0:
						strFindFile.Format(_T("%s\\%s.%s"), strBase, strMesh, strExtension);
						listFindFile.push_back( strFindFile );
						break;
					case 1:
						strFindFile.Format(_T("%s.%s"), strMesh, strExtension);
						listFindFile.push_back( strFindFile );
						break;
					case 2:
						for( int j = 0; j != 4; ++j ){
							strFindFile.Format(_T("%s\\%s_%ld.%s"), strBase, strMesh, j+1, strExtension);
							listFindFile.push_back( strFindFile );
						}
						break;
					default:
						break;
				}
				m_strRasterDir = szDir;

				for( list< CString >::iterator it = listFindFile.begin(); it != listFindFile.end(); ++it )
					CreatePyramidFile( ipRasterWS, *it, *it, i );
			}
		}
		else
			cout<<"\t"<<szDir<<" はありません。"<<endl;
	}
	cout<<"　◎ピラミッドファイル作成終了\n\n";
}

// ディレクトリ指定で処理する
void CPyramid::TargetPathList( bool bOption )
{
	char szDir[PATH_LEN] = {};
	char szExtension[16] = {};
	CString strExtension;
	cout<<"\n拡張子を指定してください。（例：「jpg」「png」）\n\t-> ";
	if( bOption )
	{
		switch( m_lExtension )
		{
		case 0:
			strExtension.Format(_T("jpg"));
			break;
		case 1:
			strExtension.Format(_T("png"));
			break;
		case 2:
			strExtension.Format(_T("bmp"));
			break;
		default:
			strExtension.Format(_T("jpg"));
			break;
		}
		cout<<strExtension<<endl;
	}
	else
	{
		cin>>szExtension;
		strExtension.Format(_T("%s"), szExtension);
	}

	::GetPrivateProfileString( _T("2"), _T("path"), _T(""), szDir, PATH_LEN, m_strIniPath );
	if( !PathFileExists( szDir ) )
	{
		cout<<szDir<<" はありません。"<<endl;
		return;
	}
	PathAddBackslash( szDir );
	m_strRasterDir = szDir;

	cout<<"　◎ピラミッドファイル作成中"<<endl;
	int i = 0;

	IWorkspaceFactoryPtr ipWF;
	if( SUCCEEDED( ipWF.CreateInstance( CLSID_RasterWorkspaceFactory ) ) )
	{
		// ラスタファイルのあるフォルダを指定
		IWorkspacePtr ipWS;
		if( SUCCEEDED( ipWF->OpenFromFile( CComBSTR( szDir ), NULL, &ipWS)))
		{
			// ラスタファイルの読み込み
			IRasterWorkspacePtr ipRasterWS(ipWS);
			IRasterDatasetPtr ipRasterDataset;

			CString strFindFile;
			strFindFile.Format(_T("%s*.%s"), szDir, strExtension);

			WIN32_FIND_DATA wfd = {0};
			HANDLE hFind = FindFirstFile( strFindFile, &wfd );
			if( hFind != INVALID_HANDLE_VALUE )
			{
				do{
					i++;
					CreatePyramidFile( ipRasterWS, wfd.cFileName, wfd.cFileName, i );
				} while( FindNextFile( hFind, &wfd ) );
			}
			FindClose( hFind );
		}
		else
			cout<<"\t× "<<szDir<<" はありません。"<<endl;
	}

	cout<<"　◎ピラミッドファイル作成終了\n\n";
}

// ピラミッドファイルを作成する
void CPyramid::CreatePyramidFile( IRasterWorkspacePtr ipRasterWS, LPCSTR szFildFile, LPCSTR szMesh, int i )
{
	cout<<"\t["<<i<<"]\t"<<szMesh;

	IRasterDatasetPtr ipRasterDataset;
	if( SUCCEEDED( ipRasterWS->OpenRasterDataset( CComBSTR( szFildFile ), &ipRasterDataset)))
	{
		IRasterLayerPtr ipRasterLayer;
		if( SUCCEEDED( ipRasterLayer.CreateInstance( CLSID_RasterLayer )))
		{
			if( SUCCEEDED( ipRasterLayer->CreateFromDataset( ipRasterDataset )))
			{
				// Arc10.3.1でIRasterLayer->get_PyramidPresentの結果が10.1と異なる
				// 動作を合わせるためにラスタファイルと同じファイル名のピラミッドファイル(*.ovr)の存在確認をする

				// 検索対象のピラミッドファイル名生成
				CString strFindPyramid(szFildFile);
				strFindPyramid = m_strRasterDir + strFindPyramid + _T(".ovr");
				// ピラミッドファイルを検索
				bool existsPyramid = false;
				WIN32_FIND_DATA wfd = {0};
				HANDLE hFind = FindFirstFile(strFindPyramid, &wfd );
				// 見つかったらtrue
				if( hFind != INVALID_HANDLE_VALUE )
					existsPyramid = true;
				FindClose( hFind );

				try{
					// ピラミッドファイルがない場合の処理
					if(!existsPyramid)
					{
						IRasterPyramidPtr ipRasterPyramid( ipRasterDataset );
						ipRasterPyramid->Create();
						VARIANT_BOOL vaBool = VARIANT_FALSE;
						ipRasterPyramid->get_Present(&vaBool);
						cout<<"\t作成完了!"<<endl;
					}
					// ピラミッドファイルがある場合の処理
					else
						cout<<"\t作成済!"<<endl;
				}
				catch(...){
					cout<<"\t作成失敗!!!"<<endl;
				}
			}
		}
		else
			cout<<"\t指定メッシュはありません。"<<endl;
	}
	else
		cout<<"\t指定メッシュはありません。"<<endl;
}

// 開始処理(処理モード受け付け)
int CPyramid::StartFunction( bool bOption )
{
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"ピラミッドファイル自動作成ツール"<<endl;
	cout<<"【 1 】リストからピラミッドファイルを作成"<<endl;
	cout<<"【 2 】ディレクトリを指定し、ピラミッドファイルを作成"<<endl;
	char szMode[PATH_LEN] = {};
	cout<<"\t 1 or 2 ??  : ";

	if( bOption )
	{
		cout<<m_lMode<<endl;

		return m_lMode;
	}
	else
	{
		cin>>szMode;

		CString strMode;
		strMode.Format(_T("%s"), szMode);
		strMode.Left(1);

		return atoi(strMode);
	}
}

// 終了処理
int CPyramid::EndFunction()
{
	char szYesNo[PATH_LEN] = {0};
	cout<<"続けて処理 [y] / 終了[n]\n\t-> ";
	cin>>szYesNo;

	CString strYesNo;
	strYesNo.Format(_T("%s"), szYesNo);

	if( strYesNo.Left(1) == _T("y") || strYesNo.Left(1) == _T("Y") )
		return 1;
	else if( strYesNo.Left(1) == _T("n") || strYesNo.Left(1) == _T("N") )
		return 2;
	else
		return 0;
}

// オプションを解析して設定する
bool CPyramid::InitOption( std::list<CString> listOption )
{
	long lePriority = 0;	//!< 優先度パラメータ
	long llPriority = 0;	//!< 優先度パラメータ
	for( std::list<CString>::iterator it = listOption.begin(); it != listOption.end(); it++ )
	{
		CString strTmp;
		strTmp = *it;

		// 標準入力かどうか
		if( strTmp == _T("/c") )
			m_bBatch = true;

		// 拡張子設定
		if( strTmp == _T("/j") && lePriority < 10 ){//jpg
			m_lExtension = 0;
			lePriority = 10;
		}else if( strTmp == _T("/p") && lePriority < 5 ){//png
			m_lExtension = 1;
			lePriority = 5;
		}else if( strTmp == _T("/b") && lePriority < 1 ){//bmp
			m_lExtension = 2;
			lePriority = 1;
		}

		// メッシュリストかディレクトリか
		if( strTmp == _T("/m") && llPriority < 5 ){//メッシュリスト
			m_lMode = 1;
			llPriority = 5;
		}else if( strTmp == _T("/d") && llPriority < 1 ){//ディレクトリ
			m_lMode = 2;
			llPriority = 1;
		}

		// 読込みリストディレクトリ変更
		if( strTmp == _T("/l")  )	//ディレクトリ
			m_lDir = 1;
		if( strTmp == _T("/s")  )	//ディレクトリ
			m_lDir = 2;

		// メッシュリストファイル
		if( strTmp.Find(_T(".txt")) >= 0 )
			m_strMeshlist = strTmp;

		if( strTmp.Find(_T(".exe")) >= 0 )
		{
			char cFullPath[PATH_LEN] = {};
			PathSearchAndQualify( strTmp, cFullPath, PATH_LEN );
			PathRemoveFileSpec( cFullPath );
			m_strCurrent.Format(_T("%s"), cFullPath);
			m_strIniPath.Format(_T("%s\\Common.ini"), cFullPath);
		}
	}

	return m_bBatch;
}
