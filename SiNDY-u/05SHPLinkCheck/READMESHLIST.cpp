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
#include "READMESHLIST.h"
#include <iostream>
#include <fstream>
#include <atl2/io.h>

CREADMESHLIST::CREADMESHLIST(_TCHAR* pFilePath)
{
	//メッシュリストファイルのフルパスをセット
	m_FilePath = pFilePath;   

}

CREADMESHLIST::~CREADMESHLIST(void)
{
}

bool CREADMESHLIST::ReadMeshList(std::vector<long>& rMeshList)
{

	//メッシュリストファイルを開く
	std::ifstream ifs(m_FilePath.c_str(), std::ios::in);

	if( !ifs.is_open() ){
		std::cerr << m_FilePath <<"の読み込みに失敗しました"<<std::endl;
		return false;
	}
	while(!ifs.eof()){
		//7バイトの読み込み用文字列
		char aBuffer[7];
		//7バイト読む。これがメッシュコード
		ifs.getline(aBuffer,7);
		long aMeshCode = std::atoi(aBuffer);
		//メッシュコードのチェック
		if( aMeshCode == 0) continue;
		if(! MeshCheck(aMeshCode) ){
			std::cerr << aMeshCode << "は不正なメッシュコードです。処理を終了します。" << std::endl;
			return false;
		}
		//メッシュコードをコンテナに
		rMeshList.push_back(aMeshCode);
	}
	return true;
}

bool CREADMESHLIST::MeshCheck(long aMeshCode)
{
	//読み込んだメッシュコードが正しいかチェック

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
