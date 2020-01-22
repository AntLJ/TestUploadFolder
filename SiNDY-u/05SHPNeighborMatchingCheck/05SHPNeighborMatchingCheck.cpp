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

// MatchingCheck.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include"ReadBMList.h"
#include"ReadMeshList.h"
#include <iostream>
#include "Road.h"
#include "time.h"


int _tmain(int argc, _TCHAR* argv[])
{

	clock_t start = clock();
	//argv[1]メッシュリストまでのフルパス
	//argv[2]レイヤリストまでのフルパス　
	//argv[3]東京測地系の全てのデータセットが格納されているディレクトリまでのパス　
	//argv[4]世界測地系の全てデータセットが格納されているディレクトリまでのパス
	//argv[5]チェックを行うデータセットの設定 r :Roadのみ　m :Middleのみ b :baseのみ c :Cityのみ a :全てのデータセット
	//argv[6]小数点第10位以下の誤差を許容するかの設定 Yes:許容する　No:許容しない
	if(argc != 7){
		std::cerr << "引数の数が正しくありません。" << std::endl;
		return 0;
	}

	std::wstring aMeshListPath = argv[1];
	std::wstring aLayerListPath = argv[2];
	std::wstring aTKYPath = argv[3];
	std::wstring aJGDPath = argv[4];
	std::wstring aSetting = argv[5];
	std::wstring aAllow = argv[6];

	if(_tcscmp(aAllow.c_str(),_T("No"))!=0 && _tcscmp(aAllow.c_str(),_T("Yes"))!=0){
		std::cerr << "第6引数が正しく設定されていません" << std::endl;
		return 0;
	}

	//Roadのチェック
	if(_tcscmp(aSetting.c_str(), _T("r"))==0){

		std::cout << "データセット:Road チェック開始\n"<< std::endl;
		std::cerr << "データセット:Road チェック開始\n"<< std::endl;
	
		//メッシュリストをコンテナに格納
		std::set<long> aRoadMeshList;
		std::wstring aRoadSet(_T("Road"));
		CReadMeshList ReadRoadMeshList(aMeshListPath, aRoadSet);
		if(! ReadRoadMeshList.ReadList((std::set<long> &)aRoadMeshList))
		{
			std::cerr << "メッシュリストの取得に失敗しました\n" << std::endl;
			std::cout << "データセット:Roadチェック失敗\n" << std::endl;
			std::cerr << "データセット:Roadチェック失敗\n" << std::endl;
		}
		else{

			//各メッシュコードに対し、隣接整合チェックを行う。
			std::set<long>::iterator Ritr = aRoadMeshList.begin();
			for(;Ritr != aRoadMeshList.end();Ritr++){
				CRoad aRoad(*Ritr,aLayerListPath,aAllow);
				if( aRoad.Main(aTKYPath,aJGDPath,aRoadSet))
				{
					std::cout <<"メッシュ:"<< *Ritr << "は正常です" << std::endl;
				}
			}
	
			std::cout << "\nデータセット:Road チェック終了\n"<< std::endl;
			std::cerr << "\nデータセット:Road チェック終了\n"<< std::endl;
		}
	}

	//Baseのチェック
	if(_tcscmp(aSetting.c_str(), _T("b"))==0){
		std::cout << "データセット:Base チェック開始\n"<< std::endl;
		std::cerr << "データセット:Base チェック開始\n"<< std::endl;
	
		//Baseのデータセットのメッシュリストをコンテナに格納
		std::set<long> aBaseMeshList;
		std::wstring aBaseSet(_T("Basemap"));
		CReadMeshList ReadBaseMeshList(aMeshListPath, aBaseSet);
		if(! ReadBaseMeshList.ReadList((std::set<long> &)aBaseMeshList))
		{
			std::cerr << "メッシュリストの取得に失敗しました\n" << std::endl;
			std::cout << "データセット:Baseチェック失敗\n" << std::endl;
			std::cerr << "データセット:Baseチェック失敗\n" << std::endl;
		}
		else{

			//各メッシュコードに対し、隣接整合チェックを行う。
			std::set<long>::iterator Bitr = aBaseMeshList.begin();
			for(; Bitr != aBaseMeshList.end(); Bitr++){
				CRoad aBase(*Bitr, aLayerListPath,aAllow );
				if( aBase.Main(aTKYPath,aJGDPath,aBaseSet))
				{
					std::cout  <<"メッシュ:"<< *Bitr << "は正常です"<< std::endl; 
				}
			}

			std::cout << "\nデータセット:Base チェック終了\n"<< std::endl;
			std::cerr << "\nデータセット:Base チェック終了\n"<< std::endl;
		}
	}

	//Middleのチェック
	if(_tcscmp(aSetting.c_str(), _T("m"))==0){

		std::cout << "データセット:Middle チェック開始\n" << std::endl;
		std::cerr << "データセット:Middle チェック開始\n" << std::endl;
	
		//Middleのデータセットのメッシュリストをコンテナに格納
		std::set<long> aMiddleMeshList;
		std::wstring aMiddleSet(_T("Middlemap"));
		CReadMeshList ReadMiddleMeshList(aMeshListPath, aMiddleSet);
		if(! ReadMiddleMeshList.ReadList((std::set<long> &)aMiddleMeshList))
		{
			std::cerr << "メッシュリストの取得に失敗しました\n" << std::endl;
			std::cout << "データセット:Middleチェック失敗\n" << std::endl;
			std::cerr << "データセット:Middleチェック失敗\n" << std::endl;
		}
		else{

			//各メッシュコードに対し、隣接整合チェックを行う。
			std::set<long>::iterator Mitr = aMiddleMeshList.begin();
			for(; Mitr != aMiddleMeshList.end(); Mitr++){
				CRoad aMiddle(*Mitr, aLayerListPath,aAllow );
				if( aMiddle.Main(aTKYPath,aJGDPath,aMiddleSet) )
				{
					std::cout  <<"メッシュ:"<< *Mitr << "は正常です"<< std::endl;
				}
			}	

			std::cout << "\nデータセット:Middle チェック終了\n" << std::endl;
			std::cerr << "\nデータセット:Middle チェック終了\n" << std::endl;
		}
	}

	//Cityのチェック
	if(_tcscmp(aSetting.c_str(),_T("c"))==0 ){

		std::cout << "データセット:Cityチェック開始\n" << std::endl;
		std::cerr << "データセット:Cityチェック開始\n" << std::endl;
	
		//Cityのデータセットのメッシュリストをコンテナに格納
		std::set<long> aCityMeshList;
		std::wstring aCitySet(_T("Citymap"));
		CReadMeshList ReadCityMeshList(aMeshListPath, aCitySet);
		if(! ReadCityMeshList.ReadList((std::set<long> &)aCityMeshList))
		{
			std::cerr << "メッシュリストの取得に失敗しました\n" << std::endl;
			std::cout << "データセット:Cityチェック失敗\n" << std::endl;
			std::cerr << "データセット:Cityチェック失敗\n" << std::endl;
		}
		else{
	
			//各メッシュコードに対し、隣接整合チェックを行う。
			std::set<long>::iterator Citr = aCityMeshList.begin();
			for(Citr; Citr != aCityMeshList.end(); Citr++){
				CRoad aCity(*Citr, aLayerListPath,aAllow );
				if( aCity.Main(aTKYPath,aJGDPath,aCitySet))
				{
					std::cout << "メッシュ:" << *Citr << "は正常です" << std::endl;
				}
			}	

			std::cout << "\nデータセット:Cityチェック終了\n" << std::endl;
			std::cerr << "\nデータセット:Cityチェック終了\n" << std::endl;
		}
	}

	clock_t end = clock();
	std::cerr << (double)(end-start)/CLOCKS_PER_SEC << "secかかりました" << std::endl;
	return 0;

}
