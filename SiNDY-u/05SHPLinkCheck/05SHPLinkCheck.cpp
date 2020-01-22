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

// 05SHPLinkCheck.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include<map>
#include"RND.h"
#include"RLK.h"
#include"CHECK.h"
#include"READMESHLIST.h"
#include <sstream>
#include <atl2/io.h>
#include"time.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start = clock();

	int iRET = 0; //戻り値の値を表す変数
	int iErrorFlag=0; //戻り値が1となる場合のフラグ
	
	try {
		//argv[1]メッシュリストまでのフルパス argv[2]チェック対象である全ての05Shapedateが格納されているディレクトリまでのパス
		if(argc < 3 || 4 < argc ) {
			std::cerr << "05Shapeのリンクが所属する都市地図メッシュコード取得ツール" << std::endl;
			std::cerr << "usage : 05SHPLinkCheck <メッシュリストまでのフルパス> <チェック対象05Shapeファイル格納ディレクトリ> [-w]" << std::endl;
			std::cerr << "option :\n"
					<< "\t\tRND, RLKを読み込む\n"
					<< "-w\tRWN, RWLを読み込む\n" << std::endl;
			return 1;
		}

		//メッシュリストの存在確認
		if(! PathFileExists(argv[1])) {
			std::cerr << "メッシュリストが存在しません。" << std::endl;
			return 1;
		}
		//Roadディレクトリの存在確認
		if(! PathIsDirectory(argv[2])) {
			std::cerr << "チェック対象05Shapeファイル格納ディレクトリが存在しません。" << std::endl;
			return 1;
		}

		//メッシュリスト格納用のコンテナ
		std::vector<long> rMeshList;

		//コンテナにメッシュリストを格納
		CREADMESHLIST RML(argv[1]);
		if (! RML.ReadMeshList((std::vector<long> & ) rMeshList)) {
			//メッシュリストの読み込みに失敗した場合
			return 1;
		}

		// COMの初期化&ライセンス認証
		//::CoInitialize(0);
		arctl::coinitializer init;

		//全てのメッシュに対して、リンクエラーチェック
		std::vector<long>::iterator Meshitr = rMeshList.begin();
		for(;Meshitr != rMeshList.end();Meshitr++) {	
			//チェックを行っているメッシュの出力
			std::cerr << *Meshitr << "\t" ;

			long aSecondMeshCode = *Meshitr;
			long aFirstMeshCode = (aSecondMeshCode/100);

			//データ存在するディレクトリまでのパスの作成
			std::wostringstream aDirPath;
			aDirPath <<argv[2]<<_T("\\")<<aFirstMeshCode << _T("\\") << aSecondMeshCode;
			std::wstring aDirName;
			aDirName = aDirPath.str();

			//RNDファイル名、RLKファイル名(-w時はRWN,RWLファイル名)の作成
			std::wostringstream aNodeFile,aLinkFile;
			bool bWalkOpt = false;
			if(argc == 3)
			{
				aNodeFile << aSecondMeshCode << _T("_RND");
				aLinkFile << aSecondMeshCode << _T("_RLK");
			}
			else if(wcscmp(argv[3] ,_T("-w")) == 0)
			{
				aNodeFile << aSecondMeshCode << _T("_RWN");
				aLinkFile << aSecondMeshCode << _T("_RWL");
				bWalkOpt = true;
			}
			std::wstring aNodeFileName = aNodeFile.str();
			std::wstring aLinkFileName = aLinkFile.str();

			//RNDファイル、RLKファイル(-w時はRWN,RWLファイル)のフルパス作成
			std::wostringstream aFullRND, aFullRLK;
			aFullRND << aDirName << _T("\\") << aNodeFileName << _T(".shp");
			aFullRLK << aDirName << _T("\\") << aLinkFileName << _T(".shp");

			std::wstring aFullRNDPath = aFullRND.str();
			std::wstring aFullRLKPath = aFullRLK.str();

			//RNDファイル(-w時はRWNファイル)の存在確認
			if(! PathFileExists(aFullRNDPath.c_str())) {
				std::cerr << aNodeFileName <<"が存在しません。リンクチェックできません" << std::endl;
				std::cout << aSecondMeshCode << "：リンクチェックできません" <<std::endl;
				iErrorFlag=1;
				continue;
			}

			//RLKファイル(-w時はRWLファイル)の存在確認
			if(! PathFileExists(aFullRLKPath.c_str())) {
				std::cerr << aLinkFileName << "が存在しません。リンクチェックできません" << std::endl;
				std::cout << aSecondMeshCode << "：リンクチェックできません" << std::endl;
				iErrorFlag=1;
				continue;
			}

			IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_ShapefileWorkspaceFactory );

			// ワークスペース取得
			IWorkspacePtr ipWorkspace;
			if( FAILED( ipWorkspaceFactory->OpenFromFile( T2BSTR(aDirName.c_str()), 0, &ipWorkspace) ) ) {
				std::cerr << aDirName << "\tのワークスペース取得に失敗しました。\n" << std::endl;
				return 1;
			}

			// RND(-w時はRWN)のフィーチャクラス取得
			IFeatureClassPtr ipFeatureClass;
			if( FAILED( IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass( T2BSTR(aNodeFileName.c_str()), &ipFeatureClass) ) ) {
				std::cerr <<aNodeFileName << "\tのフィーチャクラス取得に失敗しました。\n" << std::endl;
				return 1;
			}

			//キーをオブジェクトID・DIVID、要素を座標とするmapコンテナ
			std::map<std::pair<long, long>, WKSPoint> rNodeSet;

			//ノードのOID、DIVID、座標をmapに格納
			CRND NodeSHP(ipFeatureClass);
			if(! NodeSHP.Point2Map((std::map<std::pair<long, long>, WKSPoint> &)rNodeSet, aNodeFileName, bWalkOpt) ) {
				std::cerr<< aNodeFileName <<"コンテナへの格納が失敗しました。\n"<<std::endl;
				return 1;
			}

			// RLK(-w時はRWL)のフィーチャクラス取得
			if( FAILED( IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass( T2BSTR(aLinkFileName.c_str()), &ipFeatureClass) ) ) {
				std::cerr << aLinkFileName << "\tのフィーチャクラス取得に失敗しました。\n" << std::endl;
				return 1;
			}

			//RLKファイル(-w時はRWLファイル)のリンク情報を格納するコンテナ
			std::vector<LINK> rLinkSet;

			//CRLKｸﾗｽのインスタンスを作成し、関数GetInfoで、リンク情報（OID,始点ノードID,始点DivID,終点ノードID,終点DivID,始点座標,終点座標）をvectorに格納
			CRLK LinkSHP(ipFeatureClass);
			if( LinkSHP.GetInfo((std::vector<LINK> & )rLinkSet, aLinkFileName, bWalkOpt) ) {
			}
			else{
				std::cerr << aLinkFileName << "コンテナへの格納が失敗しました。\n"<<std::endl;
				return 1;
			}

			//リンク情報とノード情報を用いて、リンクに異常がないかチェックする。
			CCHECK cCheck;

			if(cCheck.CheckLinkSHP((std::map<std::pair<long, long>, WKSPoint> &) rNodeSet,(std::vector<LINK> &)rLinkSet, aSecondMeshCode, bWalkOpt)) {
				std::cout<< aSecondMeshCode <<"のデータは正常です"<<std::endl;
			}
			else {
				std::cout<< aSecondMeshCode <<"のデータが異常です"<<std::endl;
				iRET=3;
			}
			std::cerr << "チェック終了" <<std::endl;
		}

		if (iErrorFlag==1) {
			iRET=1;
		}
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
		iRET = 2;
	}

	clock_t end = clock();
	std::cerr << (double)(end-start)/CLOCKS_PER_SEC << "secかかりました" << std::endl;

	return iRET;

}

