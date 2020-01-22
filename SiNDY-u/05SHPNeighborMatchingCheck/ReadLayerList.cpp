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
#include "ReadLayerList.h"
#include <sstream>
#include <atl2/io.h>

 


CReadLayerList::CReadLayerList(std::wstring aFilePath)
{
	m_FilePath = aFilePath;
}

CReadLayerList::~CReadLayerList(void)
{
}

bool CReadLayerList::ReadList(std::vector<std::wstring> &rLayerList)
{

	USES_CONVERSION;
	//ファイルまでのパスの作成
	//std::wostringstream aPath;
	//aPath << m_DirPath << "\\" << aListFileName ;
	//std::wstring aFilePath = aPath.str();

	//レイヤリストを開く
	std::wifstream  ifs(m_FilePath.c_str(),std::ios::in);
	if(! ifs.is_open()){
		std::cerr << m_FilePath << "の読み込みに失敗しました" << std::endl;
		return false;
	}
	while(!ifs.eof())
	{
		
		//レイヤ読み込み用文字列
		TCHAR aBuffer[7];
		ifs.getline(aBuffer,sizeof(aBuffer),'\n');
		if(_tcscmp(aBuffer,_T(""))==0) continue;
		//レイヤをコンテナに
		std::wstring aLayerName( aBuffer); 
		rLayerList.push_back(aLayerName);

	}
		

	return true;

}
