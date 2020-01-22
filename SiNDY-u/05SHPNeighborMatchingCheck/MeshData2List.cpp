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
#include "MeshData2List.h"
#include "crd_cnv.h"
#include <string>
#include "crd_cnv.h"

CMeshData2List::CMeshData2List(std::string aLayerType)
{
	m_LayerType = aLayerType;
}

CMeshData2List::~CMeshData2List(void)
{
}

bool CMeshData2List::Data2List(long aMeshCode, CheckList *pCheckList, std::string aGeoType)
{
	FiveMeshInfo a5MeshInfo;
	crd_cnv aCrd_cnv;

	//該当メッシュの情報取得
	GetMeshInfo(&a5MeshInfo.e_MainMesh,aMeshCode,aGeoType);

	//該当メッシュ北のメッシュ情報取得
	long aNorthMesh =  aCrd_cnv.GetMeshCode(aMeshCode,0,1);
	GetMeshInfo(&a5MeshInfo.e_NorthMesh,aNorthMesh,aGeoType);

	//該当メッシュ南のメッシュ情報取得
	long aSouthMesh = aCrd_cnv.GetMeshCode(aMeshCode,0,-1);
	GetMeshInfo(&a5MeshInfo.e_SouthMesh,aSouthMesh,aGeoType);

	//該当メッシュ東のメッシュ情報取得
	long aEastMesh = aCrd_cnv.GetMeshCode(aMeshCode,1,0);
	GetMeshInfo(&a5MeshInfo.e_EastMesh,aEastMesh,aGeoType);

	//該当メッシュ西のメッシュ情報取得
	long aWestMesh = aCrd_cnv.GetMeshCode(aMeshCode,-1,0);
	GetMeshInfo(&a5MeshInfo.e_WestMesh,aWestMesh,aGeoType);

	
	//該当メッシュと東西南北メッシュの情報でリスト作成


	return true;

}


bool CMeshData2List::GetMeshInfo(MeshInfo *pMeshInfo,long aMeshCode ,std::string aGeoType)
{
	if(aGeoType == "TKY"){
		//メッシュ情報がメンバコンテナにないか調べる
		std::map<long,MeshInfo>::iterator TKYitr = m_TKYMeshData.find(aMeshCode);
		if(TKYitr != m_TKYMeshData.end()){
			*pMeshInfo = TKYitr->second;
		}
		else{
		//無い場合は情報取得

		}
	}
	else if(aGeoType == "JGD"){
		//メッシュ情報がコンテナにないか調べる
		std::map<long,MeshInfo>::iterator JGDitr = m_JGDMeshData.find(aMeshCode);
		if(JGDitr != m_JGDMeshData.end()){
			*pMeshInfo = JGDitr->second;
		}
		else{
		//無い場合は情報取得
	
		}
	}

	return true;
	
}


bool CMeshData2List::MakeList(FiveMeshInfo &r5MeshInfo, CheckList *pCheckList)
{



return true;

}
