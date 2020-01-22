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
#include "ReadMeshList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <atl2/io.h>

CReadMeshList::CReadMeshList(std::wstring & rFilePath,std::wstring & rDataSetName) 
{
	m_FilePath = rFilePath;
	m_DataSetName = rDataSetName;
}

CReadMeshList::~CReadMeshList(void)
{
}

bool CReadMeshList::ReadList(std::set<long>& rListSet)
{

	//メッシュパス作成
	//std::wostringstream aRoadPath;
	//aRoadPath << m_FilePath << "\\" << m_DataSetName << "\\data_spec\\meshlist.txt";
	//std::wstring aRoadMeshPath = aRoadPath.str();
	
	//メッシュリストファイルを開く
	std::ifstream ifs(m_FilePath.c_str(),std::ios::in);

	if(! ifs.is_open()){
		std::cerr << m_FilePath << "の読み込みに失敗しました。" << std::endl;
		return false;
	}
	while(!ifs.eof())
	{
		//メッシュ読み込み用文字列 
		char aBuffer[9];
		ifs.getline(aBuffer,sizeof(aBuffer),'\n');		
		long aMeshCode = std::atoi(aBuffer);
		if(! CheckMesh(aMeshCode)) continue ;
		//メッシュコードをコンテナに
		rListSet.insert(aMeshCode);

	}

	return true;


}

bool CReadMeshList::CheckMesh(long aMesh)
{
	//読み込んだ文字列がメッシュコードであるかチェック
	if ( _tcscmp(m_DataSetName.c_str(), _T("Citymap")) == 0){
		long a1stY = aMesh/1000000;
		long a1stX = (aMesh/10000)%100;
		long a2ndY = (aMesh/1000)%10;
		long a2ndX = (aMesh/100)%10;
		long a3rdY = (aMesh/10)%10;
		long a3rdX = aMesh%10;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;
		if( (a2ndY < 0 ) || (a2ndY > 7 ) ) return false;
		if( (a2ndX < 0 ) || (a2ndX > 7 ) ) return false;
		if( (a3rdY < 0 ) || (a3rdY > 7 ) ) return false;
		if( (a3rdX < 0 ) || (a3rdX > 7 ) ) return false;

		return true;
	}
	else if ( _tcscmp(m_DataSetName.c_str(), _T("Basemap"))==0 || _tcscmp(m_DataSetName.c_str(), _T("Road"))==0)
	{
		long a1stY = aMesh/10000;
		long a1stX = (aMesh/100)%100;
		long a2ndY = (aMesh/10)%10;
		long a2ndX = aMesh%10;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;
		if( (a2ndY < 0 ) || (a2ndY > 7 ) ) return false;
		if( (a2ndX < 0 ) || (a2ndX > 7 ) ) return false;

		return true;
	}
	else if (_tcscmp(m_DataSetName.c_str(), _T("Middlemap"))==0)
	{
		long a1stY = aMesh/100;
		long a1stX = aMesh%100;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;

		return true;
	}
	else 
	{
		//std::cerr << "データセット" << m_DataSetName.c_str() << "はこのツールに対応していません。\n" << std::endl;
		return false;
	}

}		
