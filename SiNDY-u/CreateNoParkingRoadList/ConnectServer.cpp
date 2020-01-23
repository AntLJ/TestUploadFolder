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
#include "MakeList.h"

extern "C"{
#include <wkf.h>
}

using namespace sindy;
using namespace sindy::schema;

CConnectServer::CConnectServer(void)
{
}

CConnectServer::~CConnectServer(void)
{
}

//////////////////////////////////////////////////////////////////////
// 指定サーバへの接続
//////////////////////////////////////////////////////////////////////
bool CConnectServer::run(CString &rConf)
{
	ConfData aConfData;
	ReadConf(rConf, aConfData);

	if( aConfData.eSDEServer.IsEmpty() )
	{
		std::cout << "# ERROR\t接続するサーバ設定が記入されていません。\n";
		return false;
	}

	if( !aConfData.eOutputShape.IsEmpty() )
	{
		if( !PathIsDirectory( aConfData.eOutputShape ) )
		{
			std::cout << "# ERROR\t出力shapeディレクトリのパスが不正です。\n";
			return false;
		}
	}

	if( !aConfData.eOutputLink.IsEmpty() )
	{
		if( !PathIsDirectory( aConfData.eOutputShape ) )
		{
			std::cout << "# ERROR\t駐車禁止取締区間のラインの出力shapeディレクトリのパスが不正です。\n";
			return false;
		}
	}

	// 道路サーバに接続
	const IWorkspacePtr ipSDEWorkspace = GetWorkspace( aConfData.eSDEServer, _T("道路用サーバ") );
	if( !ipSDEWorkspace )
		return false;

	// ※※※修正点※※※
	// shapeの場合は都道府県別読込まないといけないのでここのディレクトリでは判別できない
	// そのためこのディレクトリに都道府県のディレクトリを追記する必要がある、と思う(2009/3/13)
	// PGDBの場合とshapeの場合で処理を分ける（というよりIFeatureClassの取得方法を若干変える）
	bool bShape = true;		// 読込み対象がshapeかどうか
	if( !PathIsDirectory( aConfData.eNoParkingPath ) )
	{
		if( !PathFileExists( aConfData.eNoParkingPath ) )
		{
			std::cout << "# ERROR\t駐禁取締路線データ設定が不正です。\n";
			return false;
		}
		else
			bShape = false;
	}

	std::set<CString> setPerfecturePath;
	if( bShape )
	{
		// shapeの場合は都道府県ディレクトリを取得する
		if( !GetPerfecturePath( aConfData.eNoParkingPath, setPerfecturePath ) )
			return false;
	}
	else
		// PGDBの場合はそのまま追加
		setPerfecturePath.insert( aConfData.eNoParkingPath );

	// 駐禁取締路線データに接続し、駐禁取締路線フィーチャクラスを取得
	std::vector<CAdapt<IFeatureClassPtr>> vecFeatureClass;
	for( std::set<CString>::const_iterator it = setPerfecturePath.begin(); it != setPerfecturePath.end(); ++it )
	{
		// IWorkspace取得
		const IWorkspacePtr ipNoParkingWorkspace = GetWorkspace( *it, _T("駐禁路線") );
		if( ipNoParkingWorkspace )
		{
			IFeatureWorkspacePtr ipNoParkingFeatureWorkspace(ipNoParkingWorkspace);
			if( ipNoParkingFeatureWorkspace )
			{
				IFeatureClassPtr ipNoParkingRoadClass;
				ipNoParkingFeatureWorkspace->OpenFeatureClass(CComBSTR("NoParking_Road"), &ipNoParkingRoadClass);
			}
			else
			{
				std::cout << "# ERROR\tIFeatureWorkspaceの取得に失敗\n";
				return false;
			}
			// IFeatureClassの取得はshapeかPGDBかで処理を分ける
			if( bShape )
			{
				// IFeatureClassを取得し、
				IFeatureClassPtr ipNoParkingRoadClass1, ipNoParkingRoadClass2;
				ipNoParkingRoadClass1 = GetFeatureClass( ipNoParkingWorkspace, _T("Reg_R_Imp") );
				ipNoParkingRoadClass2 = GetFeatureClass( ipNoParkingWorkspace, _T("Reg_R") );
				// リストへ突っ込む
				if( ipNoParkingRoadClass1 && ipNoParkingRoadClass2 )
				{
					vecFeatureClass.push_back( ipNoParkingRoadClass1 );
					vecFeatureClass.push_back( ipNoParkingRoadClass2 );
				}
				else
				{
					if( !ipNoParkingRoadClass1 )
						std::cout << "# ERROR\t" << CT2A(*it) << "\tReg_R_Imp.shp がありまんせん。\n";
					if( !ipNoParkingRoadClass2 )
						std::cout << "# ERROR\t" << CT2A(*it) << "\tReg_R.shp がありまんせん。\n";
					return false;
				}
			}
			else
			{
				// IFeatureClassを取得し、リストへ突っ込む
				IFeatureClassPtr ipNoParkingRoadClass;
				ipNoParkingRoadClass = GetFeatureClass( ipNoParkingWorkspace, _T("NoParking_Road") );
				if( ipNoParkingRoadClass )
					vecFeatureClass.push_back( ipNoParkingRoadClass );
				else
				{
					std::cout << "# ERROR\t" << CT2A(*it) <<"フィーチャクラス(NoParking_Road) がありまんせん。\n";
					return false;
				}
			}
		}
		else
			return false;
	}

	// 道路リンククラスを取得
	IFeatureClassPtr ipLinkClass = GetFeatureClass( ipSDEWorkspace, _T("Road_Link") );
	if( !ipLinkClass )
		return false;

	// 出力先ログの作成
	std::ofstream ofp(static_cast<CT2A>(aConfData.eOutput));
	if( ofp.fail() )
	{
		std::cout << "# ERROR\tエラーログファイル「" << CT2A(aConfData.eOutput);
		std::cout << "」作成失敗" << std::endl;
		return false;
	}
	CMakeList aMakeList;
	aMakeList.create( ipLinkClass, vecFeatureClass, bShape, ofp, aConfData.eOutputShape, aConfData.eOutputLink );

	ofp.close();

	// SJIS→EUC文字コード変換
	char szOutputTmpFile[L_tmpnam];
	::tmpnam(szOutputTmpFile);
	FILE *ifp = fopen(CT2A(aConfData.eOutput), "r");
	FILE *ofp2 = fopen(szOutputTmpFile, "wb");
	wkfConvertKanjiCodeOfFile(KC_SJIS, ifp, KC_EUC, ofp2);
    fclose(ifp);
    fclose(ofp2);
	remove( CT2A(aConfData.eOutput) );
	rename( szOutputTmpFile, CT2A(aConfData.eOutput) );

	return true;
}

//////////////////////////////////////////////////////////////////////
// 設定ファイルの読み込み
//////////////////////////////////////////////////////////////////////
void CConnectServer::ReadConf(CString &rConf, ConfData &rConfData)
{
	std::ifstream ifs(rConf);
	char rStrLine[1024] ={0};
	while( ifs.getline( rStrLine, sizeof(rStrLine) ) )
	{
		CString csLine = rStrLine;
		if( csLine[0] == '#' )
			continue;

		if( csLine.Find(_T("SDESERVER") ) >= 0 )
		{
			csLine.Delete(0,9);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eSDEServer = csLine;
		}

		if( csLine.Find(_T("NOPARKINGDATA") ) >= 0 )
		{
			csLine.Delete(0,13);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eNoParkingPath = csLine;
		}

		if( csLine.Find(_T("OUTPUTFILE") ) >= 0 )
		{
			csLine.Delete(0,10);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eOutput = csLine;
		}

		if( csLine.Find(_T("OUTPUTSHAPE") ) >= 0 )
		{
			csLine.Delete(0,11);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eOutputShape = csLine;
		}
		if( csLine.Find(_T("OUTPUTBASE") ) >= 0 )
		{
			csLine.Delete(0,10);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			rConfData.eOutputLink = csLine;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// shapeの都道府県ディレクトリを取得
//////////////////////////////////////////////////////////////////////
bool CConnectServer::GetPerfecturePath( LPCTSTR lpcszNoParkingPath, std::set<CString>& setPerfecturePath )
{
	setPerfecturePath.clear();

	WIN32_FIND_DATA fd;
	CString strFindDir = CString(lpcszNoParkingPath) + _T("\\*");
	HANDLE hFind = FindFirstFile( strFindDir, &fd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		// ルートパス以下のディレクトリ探索
		do
		{
			if( fd.cFileName[0] == '.' )
				continue;
			// NOPARKINGDIRで指定したディレクトリ以下には都道府県別のディレクトリしかないはずだけど
			// 「.shp」があるかも検索して判別するようにした
			CString strPerfecturePath = CString(lpcszNoParkingPath) + _T("\\") + fd.cFileName;
			if( PathIsDirectory( strPerfecturePath ) )
			{
				WIN32_FIND_DATA fd2;
				CString strFindDir2 = strPerfecturePath + _T("\\*");
				HANDLE hFind2 = FindFirstFile( strFindDir2, &fd2 );
				if( hFind2 != INVALID_HANDLE_VALUE )
				{
					// ルートパス以下のディレクトリ探索
					do
					{
						if( fd2.cFileName[0] == '.' )
							continue;
						// 検索ディレクトリ内に「.shp」があれば都道府県ディレクトリと見なす
						if( _tcsicmp( CString(fd2.cFileName).Right(4), _T(".shp") ) == 0 )
						{
							setPerfecturePath.insert( strPerfecturePath );
							break;
						}
					} while( FindNextFile( hFind2, &fd2 ) );
					FindClose(hFind2);
				}
				else
				{
					// ここに来ることはないと思うけど、一応
					std::cout << "# ERROR\t駐禁取締路線データ検索失敗2\n";
					FindClose(hFind);
					return false;
				}
			}
		} while( FindNextFile( hFind, &fd ) );
		FindClose(hFind);
	}
	else
	{
		std::cout << "# ERROR\t駐禁取締路線データ検索失敗\n";
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// IWorkspaceを取得
//////////////////////////////////////////////////////////////////////
IWorkspacePtr CConnectServer::GetWorkspace( LPCTSTR lpcszDBPath, LPCTSTR lpcszMessage )
{
	IWorkspacePtr ipWorkspace(INamePtr(create_workspace_name(lpcszDBPath))->_Open());
	if( ipWorkspace )
		std::cerr << CT2A(lpcszMessage) << "：" << sindy::workspace_address(ipWorkspace) << "に接続\n";
	else
		std::cout << "# ERROR\t" << CT2A(lpcszDBPath) << "\tIWorkspace取得失敗\n";

	return ipWorkspace;
}

//////////////////////////////////////////////////////////////////////
// IFeatureClassを取得
//////////////////////////////////////////////////////////////////////
IFeatureClassPtr CConnectServer::GetFeatureClass( IWorkspace* ipWorkspace, LPCTSTR lpcszFeatureClassName )
{
	IFeatureClassPtr ipFeatureClass;
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	if( ipFeatureWorkspace )
	{
		ipFeatureWorkspace->OpenFeatureClass( CComBSTR(lpcszFeatureClassName), &ipFeatureClass );
		if( !ipFeatureClass )
			std::cout << "# ERROR\tIFeatureClassの取得に失敗\n";
	}
	else

		std::cout << "# ERROR\tIFeatureWorkspaceの取得に失敗\n";

	return ipFeatureClass;
}
