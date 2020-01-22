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
#include "NeighborMatchingCheck.h"
#include <fstream>
#include <string>
#include "Function.h"
#include "MeshInfoGetter.h"
#include <sstream>
#include "ErrorCheck.h"

CNeighborMatchingCheck::CNeighborMatchingCheck(void)
{
}

CNeighborMatchingCheck::~CNeighborMatchingCheck(void)
{
}

/////////メインの処理/////////////////////////////////////

int CNeighborMatchingCheck::App(const Parameters &rParams)
{
	int iErrFlag=0;//エラーフラグ・・・エラーがあるが処理を止めない場合に、エラーであることを記録しておく変数
	int iRET = 0;

	//対象レイヤの読み込み
	std::vector<std::string> aLayerList;
	if(! ReadLayer(&aLayerList,rParams.e_LayerList)){
		return 1;
	}

	//メッシュリストの読み込み(チェック対象メッシュを決める)
	//メッシュリストに不正な値があった場合はエラーフラグに1を入れるが、処理は止めない。
	std::set<long> aTargetMesh;
	if(! GetTargetMesh(&aTargetMesh,rParams,&iErrFlag)){
		return 1;
	}

	//データ読み込み、整合チェックの開始
	//レイヤの数だけ繰り返す
	std::vector<std::string>::iterator Litr = aLayerList.begin();
	for(;Litr != aLayerList.end(); Litr++){
		CMeshInfoGetter aMeshInfoGetter(*Litr);

		std::cerr << *Litr << "レイヤのチェック開始" << std::endl;

		//エラー書き込み用テキスト作成
		std::ostringstream aErrorFilePath;
		aErrorFilePath << rParams.e_ErrorDir << "\\MatchingError_" << *Litr  << ".txt";

		std::ofstream aErrorFile(aErrorFilePath.str().c_str());
		if( ! aErrorFile.is_open())
		{
			std::cerr << "エラー出力用ファイルが確保できませんでした" << std::endl;
			return 1;
		}
		//ログハンドルツールで利用できるように設定を書き込む
		aErrorFile << "# FREESTYLELOG" << std::endl;
		aErrorFile << "FLAG\tLAYER\tOBJECTID\tエラーレベル\tエラーコード\tエラー内容\t該当MeshCode\t該当OBJECTID\t該当DIVID\t該当補間点番号\t隣接MeshCode\t隣接OBJECTID\t隣接DIVID\t隣接補間点番号\t角度①差\t角度②差\t角度③差\t角度④差" << std::endl;			
		//チェック対象メッシュの数だけ、繰り返す
		std::set<long>::iterator Mitr = aTargetMesh.begin();
		for(Mitr;Mitr != aTargetMesh.end();Mitr++){	
			std::cerr << *Mitr << "\t";
			//TKYデータの取得
			//TKY用5メッシュ情報格納コンテナ
			FiveMeshInfo<TKYMeshInfo> aTKY5MeshInfo;
			if(! aMeshInfoGetter.Get5MeshData(*Mitr,&aTKY5MeshInfo,"TKY",rParams))
			{
				iErrFlag = 1;
				std::cerr << "NG\t日本測地系データの取得に失敗しました\t" << std::endl;
				continue;
			}
			//JGDデータの取得
			FiveMeshInfo<JGDMeshInfo> aJGD5MeshInfo;
			if(! aMeshInfoGetter.Get5MeshData(*Mitr,&aJGD5MeshInfo,"JGD",rParams) )
			{
				iErrFlag = 1;
				std::cerr << "NG\t世界測地系データの取得に失敗しました\t" << std::endl;
				continue;
			}

			//整合チェック
			CErrorCheck aErrorCheck;
			//TKYリスト作成
			if (! aErrorCheck.MakeTKYList(aTKY5MeshInfo) )
			{
				std::cerr << "東京測地系データでエラーがありました" << std::endl;
				iRET = 3;
			}
			//JGDデータチェック
			if(! aErrorCheck.CheckJGDData(aJGD5MeshInfo,&aErrorFile,*Litr,rParams.e_Border))
			{
				std::cerr << "世界測地系のデータでエラーがありました" << std::endl;
				iRET=3;
				continue;
			}
			std::cerr << "OK\tデータは正常です" << std::endl;
		}
		std::cerr << *Litr << "レイヤのチェック終了" << std::endl;
	}
	if (iErrFlag == 1)
	{
		return 1;
	} 
	else
	{
		return iRET;
	}

}



/////////レイヤリスト読み取り用関数///////////////////////////////////////////////////////////////////

	bool CNeighborMatchingCheck::ReadLayer(std::vector<std::string> *pLayerList,const std::string &rListName )
{
	//レイヤリストを開く
	std::ifstream ifs(rListName.c_str());
	if(! ifs.is_open()){
		std::cerr << "レイヤリストの読み込みに失敗しました" << std::endl;
		return false;
	}

	while(!ifs.eof())
	{
		//レイヤ読み込み用文字列
		char aBuffer[128];
		ifs.getline(aBuffer,sizeof(aBuffer),'\n');
		if(strcmp(aBuffer,"")==0) continue;
		//レイヤ名をコンテナに
		std::string aLayerName(aBuffer);
		pLayerList->push_back(aLayerName);
	}

	return true;

}

/////////チェック対象メッシュ抽出用関数///////////////////////////////////////////////////////

bool CNeighborMatchingCheck::GetTargetMesh(std::set<long> *pTargetMesh, const Parameters &rParams, int *iErrFlag)
{	
	//日本語ファイル名に対応する為
    std::locale loc = std::locale();
   loc = std::locale(loc, "japanese", std::locale::ctype);
    std::locale::global(loc);
	//メッシュリストを開く
	std::ifstream ifs;
	//ifs.imbue(std::locale("japanese"));
	//std::locale::global( std::locale( "japanese" ) ); 
	ifs.open(rParams.e_MeshList.c_str());
	if(! ifs.is_open()){
		std::cerr << "メッシュリストの読み込みに失敗しました" << std::endl;
		return false;
	}

	while(!ifs.eof())
	{
		//メッシュ読み込み用文字列
		char aBuffer[9];
		ifs.getline(aBuffer,sizeof(aBuffer));
		if (aBuffer[0] == NULL) break;
		long aMeshCode = std::atoi(aBuffer);
		//読み込んだメッシュをチェック
		if(! MeshCheck(aMeshCode,rParams.e_DataSet)){
			std::cerr << aMeshCode << "は不正なメッシュです" << std::endl;
			*iErrFlag = 1;
			continue;
		}
		//メッシュコードをコンテナに格納
		pTargetMesh->insert(aMeshCode);
#ifdef _DEBUG
		std::cerr << aMeshCode << std::endl;
#endif
	}

	//チェック対象範囲が全国ではない場合(設定ファイルのCHECKAREAがpartの場合)メッシュを追加
	if(rParams.e_Area == "part")
	{
		//追加メッシュ用コンテナ
		std::set<long> SubMeshList;
		
		//読み込んだ各メッシュに対し、両隣(東と西)のメッシュが読み込まれていない場合は追加する
		std::set<long>::iterator Titr = pTargetMesh->begin();

		for(;Titr != pTargetMesh->end();Titr ++){
			
			//東メッシュを読みこんだメッシュリストから検索し、なければ追加メッシュ用コンテナに追加
			long aEastMeshCode = Function::GetMeshCode(*Titr,1,0,rParams.e_DataSet);

			if( pTargetMesh->find(aEastMeshCode) == pTargetMesh->end() && aEastMeshCode != -1  )
			{
				SubMeshList.insert(aEastMeshCode);
			}

			//西メッシュを読みこんだメッシュリストから検索し、なければ追加メッシュ用コンテナに追加
			long aWestMeshCode = Function::GetMeshCode(*Titr,-1,0,rParams.e_DataSet);
		
			if( pTargetMesh->find(aWestMeshCode) == pTargetMesh->end() && aWestMeshCode != -1 )
			{
				SubMeshList.insert(aWestMeshCode);
			}
		}

		//追加メッシュ用コンテナのデータをpTargetMeshに追加する。
		std::set<long>::iterator Sitr = SubMeshList.begin();
		for(;Sitr != SubMeshList.end();Sitr ++){
			pTargetMesh->insert(*Sitr);
		}
	}

	return true;

}

/////////メッシュの正当性チェック関数////////////////////////////////////

bool CNeighborMatchingCheck::MeshCheck(long aMeshCode, std::string aDataSet)
{
	//読み込んだ文字列がメッシュコードであるかチェック
	if ( aDataSet == "city")
	{
		long a1stY = aMeshCode/1000000;
		long a1stX = (aMeshCode/10000)%100;
		long a2ndY = (aMeshCode/1000)%10;
		long a2ndX = (aMeshCode/100)%10;
		long a3rdY = (aMeshCode/10)%10;
		long a3rdX = aMeshCode%10;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;
		if( (a2ndY < 0 ) || (a2ndY > 7 ) ) return false;
		if( (a2ndX < 0 ) || (a2ndX > 7 ) ) return false;
		if( (a3rdY < 0 ) || (a3rdY > 7 ) ) return false;
		if( (a3rdX < 0 ) || (a3rdX > 7 ) ) return false;

		return true;
	}
	else if (aDataSet == "base")
	{
		long a1stY = aMeshCode/10000;
		long a1stX = (aMeshCode/100)%100;
		long a2ndY = (aMeshCode/10)%10;
		long a2ndX = aMeshCode%10;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;
		if( (a2ndY < 0 ) || (a2ndY > 7 ) ) return false;
		if( (a2ndX < 0 ) || (a2ndX > 7 ) ) return false;

		return true;
	}
	else if (aDataSet == "middle")
	{
		long a1stY = aMeshCode/100;
		long a1stX = aMeshCode%100;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;

		return true;
	}
	else if(aDataSet == "top")
	{
		if( (aMeshCode != 0) && (aMeshCode != 1) && (aMeshCode != 10) && (aMeshCode != 11)) return false;

		return true;
	}

	return false;

}		




