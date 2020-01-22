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
#include "Road.h"
#include <sstream>
#include "GetLayerInfo.h"
#include "ReadLayerList.h"
#include <set>
#include "CheckList.h"





CRoad::CRoad(long aMeshCode, std::wstring aLayerListPath, std ::wstring aAllow)
{
	m_SecondMeshCode = aMeshCode;
	m_LayerListPath = aLayerListPath;
	m_Allow = aAllow;
}

CRoad::~CRoad(void)
{
}

bool CRoad::Main(std::wstring aTKYPath, std::wstring aJGDPath,std::wstring aDataSetName)
{

	//COMの初期化
	::CoInitialize(0);


	//1次メッシュコードの取得
	long aFirstMeshCode = (m_SecondMeshCode/100);


	//東京測地系のデータの存在するディレクトリまでのパス作成
	std::wostringstream aTKYDirPath;
	aTKYDirPath << aTKYPath << _T("\\") << aDataSetName ; 
	std::wstring  aTKYRoadPath;
	aTKYRoadPath = aTKYDirPath.str();

	//世界測地系のデータが存在するディレクトリまでのパス作成
	std::wostringstream aJGDDirPath;
	aJGDDirPath << aJGDPath << _T("\\") << aDataSetName ;
	std::wstring aJGDRoadPath;
	aJGDRoadPath = aJGDDirPath.str();

	//レイヤリストをコンテナに読み込む
	std::vector<std::wstring> aLayerList;
	CReadLayerList aLayer(m_LayerListPath);
	//std::wostringstream aListFile;
	//aListFile << aDataSetName <<"layerlist.txt";
	//std::wstring aListFileName;
	//aListFileName = aListFile.str();
	aLayer.ReadList(aLayerList);

	std::vector<std::wstring>::iterator itr =aLayerList.begin();

	bool aCheck=true;

	//イテレータが終わるまで回す
	while(itr != aLayerList.end())
	{
		std::wstring aLayerName = *itr;
		//東京測地系、世界測地系それぞれ5メッシュの情報を取り出し、
		//境界線上の情報を一つのリストにまとめ、東京測地系のリストと世界測地系のリストを比較する	
		std::set<CheckKey> aTKYMergeSet, aJGDMergeSet;
		
		CGetLayerInfo aTKYGetLayerInfo(m_SecondMeshCode,m_Allow);
		aTKYGetLayerInfo.Merge5MeshInfo(aTKYRoadPath,aLayerName ,aTKYMergeSet);
		
		CGetLayerInfo aJGDGetLayerInfo(m_SecondMeshCode,m_Allow);
		aJGDGetLayerInfo.Merge5MeshInfo(aJGDRoadPath,aLayerName ,aJGDMergeSet);

		CCheckList aChekList;
		if(! aChekList.Check(aTKYMergeSet,aJGDMergeSet,aLayerName)){
			aCheck= false;
		}

		itr++;

	}

	return aCheck;
	
}



