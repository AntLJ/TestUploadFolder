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

// diffshapeadd.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "diffshapeadd.h"
#include "diffadd.h"
#include "DiffAddSDE.h"

#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <sindy/schema.h>
#include <versioninfo.h>
#include <TDC/useful_headers/flush_scope_guard.h>

int _tmain(int argc, _TCHAR* argv[])
{

	uh::flush_scope_guard fsg;

	// ツールのバージョン出力
	CVersion aVersion;
	CString strFileVersion = aVersion.GetFileVersion();
	CString strProductVersion = aVersion.GetProductVersion();
	std::cout << "diffshapeadd.exe\tFILEVERSION : " << CT2A(strFileVersion) << " PRODUCTVERSION : " << CT2A(strProductVersion) << "\n";
	std::cout << "start : " << CT2A(nowtime()) << "\n";

	try
	{
		const arctl::coinitializer coinit;

		CInputData cInputData;
		// コマンドライン引数取得 =====================================================================================
		for(int i = 0; i < argc; ++i){
			CString strArg = argv[i];
			if(_tcscmp(strArg, _T("-dt")) == 0 || _tcscmp(strArg, _T("--do-type")) == 0){
				i++;
				cInputData.lDoType = _ttol(argv[i]);
			} else if(_tcscmp(strArg, _T("-bsd")) == 0 || _tcscmp(strArg, _T("--base-shape-dir")) == 0){
				i++;
				cInputData.strSShapeDir = argv[i];
			} else if(_tcscmp(strArg, _T("-dsd")) == 0 || _tcscmp(strArg, _T("--diff-shape-dir")) == 0){
				i++;
				cInputData.strDiffShapeDir = argv[i];
			} else if(_tcscmp(strArg, _T("-dal")) == 0 || _tcscmp(strArg, _T("--diff-add-list")) == 0){
				i++;
				cInputData.strDiffAddList = argv[i];
			} else if(_tcscmp(strArg, _T("-md")) == 0 || _tcscmp(strArg, _T("--m-division")) == 0){
				i++;
				cInputData.lMDivision = _ttol(argv[i]);
			} else if(_tcscmp(strArg, _T("-sc")) == 0 || _tcscmp(strArg, _T("--sde-connect")) == 0){
				i++;
				cInputData.strSDEConnect = argv[i];
			} else if(_tcscmp(strArg, _T("-ml")) == 0 || _tcscmp(strArg, _T("--mesh-list")) == 0){
				i++;
				cInputData.strMeshlist = argv[i];
			} 
		}
		// 接続情報表示
		if(cInputData.lDoType == 0){
			std::cout << "# BASESHAPEDIR=" << CT2A(cInputData.strSShapeDir) << "\n";
			std::cout << "# DIFFSHAPEDIR=" << CT2A(cInputData.strDiffShapeDir) << "\n";
			std::cout << "# DIFFADDLIST=" << CT2A(cInputData.strDiffAddList) << "\n";
			if(cInputData.lMDivision < 0)
				cInputData.lMDivision = 3;	// 値が指定されていなかったら「3」を初期値とする
			std::cout << "# MDIVISION=" << cInputData.lMDivision << "\n";
			std::cout << "\n";
		} else if(cInputData.lDoType == 1) {
			std::cout << "# SDECONNECT=" << CT2A(cInputData.strSDEConnect) << "\n";
			std::cout << "# DIFFSHAPEDIR=" << CT2A(cInputData.strDiffShapeDir) << "\n";
			std::cout << "# MESHLIST=" << CT2A(cInputData.strMeshlist) << "\n";
			std::cout << "\n";
		} else {
			std::cerr << "#ERROR\t[-dt]を指定してください。\n";
			std::cout << "end : " << CT2A(nowtime()) << "\n";
			return false;
		}

		// コマンドライン引数取得 ここまで============================================================================

		// コマンドライン引数の正誤判定 ===============================================================================
		if(cInputData.lDoType == 0){
			if(!PathIsDirectory(cInputData.strSShapeDir)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strSShapeDir) << "\tS社元データのディレクトリが存在しない\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
			if(!PathIsDirectory(cInputData.strDiffShapeDir)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strDiffShapeDir) << "\t差分データのディレクトリが存在しない\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
			if(!PathFileExists(cInputData.strDiffAddList)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strDiffAddList) << "\t差分追加リストが存在しない\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
		} else if(cInputData.lDoType == 1){
			if(!PathIsDirectory(cInputData.strDiffShapeDir)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strDiffShapeDir) << "\t差分データのディレクトリが存在しない\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
			if(!cInputData.strMeshlist.IsEmpty()){
				if(!PathFileExists(cInputData.strMeshlist)){
					std::cerr << "#ERROR\t" << CT2A(cInputData.strMeshlist) << "\t差分データ追加のメッシュリストが存在しない\n";
					std::cout << "end : " << CT2A(nowtime()) << "\n";
					return 0;
				}
				std::ifstream ifs;
				ifs.open(cInputData.strMeshlist);
				if(ifs.fail())
					return false;
				char cLine[256];
				CString strNum(_T("0123456789"));
				while(ifs.getline(cLine,256)){
					CString strLine(cLine);
					if(!strLine.IsEmpty()){
						long lIndex = StrSpn(strLine, strNum);
						if(strLine.GetLength() == 6 && strLine.GetLength() == lIndex)
							cInputData.setMeshlist.insert(strLine);
						else
							std::cerr << "#WARNING\t" << CT2A(strLine) << "\t2次メッシュじゃないのでスルーします\n";
					}
				}
			} else {
				std::cerr << "#NOTICE\tメッシュリストの指定が無いので差分データディレクトリにあるすべてのデータがインポート対象になります。\n";
			}

			// ワークスペース取得
			cInputData.ipSDEWork = sindy::create_workspace(cInputData.strSDEConnect);
			if(!cInputData.ipSDEWork){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strSDEConnect) << "\tSDEワークスペース接続失敗\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}

			// フィーチャクラス、テーブル類を取得しておく
			IFeatureWorkspacePtr(cInputData.ipSDEWork)->OpenFeatureClass(CComBSTR(sindy::schema::road_link::kTableName), &cInputData.ipLinkClass);
			IFeatureWorkspacePtr(cInputData.ipSDEWork)->OpenFeatureClass(CComBSTR(sindy::schema::road_node::kTableName), &cInputData.ipNodeClass);

			if(!cInputData.ipLinkClass || !cInputData.ipNodeClass){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strSDEConnect) << "\tフィーチャクラス、テーブル取得失敗\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
		}
		// コマンドライン引数の正誤判定 ここまで =====================================================================

		// 処理開始 ===============================================
		if(cInputData.lDoType == 0){
			// dotype=0の場合は差分SHPにデータを追加するモード
			CDiffAdd cDiffAdd(cInputData);
			if(cDiffAdd.run()){
				std::cout << "正常終了\n";
			} else {
				std::cout << "異常終了\n";
			}
		} else if(cInputData.lDoType == 1){
			// dotype=1の場合はSDEに差文データをインポートするモード
			CDiffAddSDE cDiffAddSDE(cInputData);
			if(cDiffAddSDE.run()){
				std::cout << "正常終了\n";
			} else {
				std::cout << "異常終了\n";
			}
		}
	}
	// 例外処理
	catch(const std::exception& e) {
		std::wcout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::wcout << e.ErrorMessage() << std::endl;
	}

	std::cout << "end : " << CT2A(nowtime()) << "\n";
	return 0;
}

CString nowtime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strNowTime;
	strNowTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return strNowTime;
}
