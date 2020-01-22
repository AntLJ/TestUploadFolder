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

	//�Y�����b�V���̏��擾
	GetMeshInfo(&a5MeshInfo.e_MainMesh,aMeshCode,aGeoType);

	//�Y�����b�V���k�̃��b�V�����擾
	long aNorthMesh =  aCrd_cnv.GetMeshCode(aMeshCode,0,1);
	GetMeshInfo(&a5MeshInfo.e_NorthMesh,aNorthMesh,aGeoType);

	//�Y�����b�V����̃��b�V�����擾
	long aSouthMesh = aCrd_cnv.GetMeshCode(aMeshCode,0,-1);
	GetMeshInfo(&a5MeshInfo.e_SouthMesh,aSouthMesh,aGeoType);

	//�Y�����b�V�����̃��b�V�����擾
	long aEastMesh = aCrd_cnv.GetMeshCode(aMeshCode,1,0);
	GetMeshInfo(&a5MeshInfo.e_EastMesh,aEastMesh,aGeoType);

	//�Y�����b�V�����̃��b�V�����擾
	long aWestMesh = aCrd_cnv.GetMeshCode(aMeshCode,-1,0);
	GetMeshInfo(&a5MeshInfo.e_WestMesh,aWestMesh,aGeoType);

	
	//�Y�����b�V���Ɠ�����k���b�V���̏��Ń��X�g�쐬


	return true;

}


bool CMeshData2List::GetMeshInfo(MeshInfo *pMeshInfo,long aMeshCode ,std::string aGeoType)
{
	if(aGeoType == "TKY"){
		//���b�V����񂪃����o�R���e�i�ɂȂ������ׂ�
		std::map<long,MeshInfo>::iterator TKYitr = m_TKYMeshData.find(aMeshCode);
		if(TKYitr != m_TKYMeshData.end()){
			*pMeshInfo = TKYitr->second;
		}
		else{
		//�����ꍇ�͏��擾

		}
	}
	else if(aGeoType == "JGD"){
		//���b�V����񂪃R���e�i�ɂȂ������ׂ�
		std::map<long,MeshInfo>::iterator JGDitr = m_JGDMeshData.find(aMeshCode);
		if(JGDitr != m_JGDMeshData.end()){
			*pMeshInfo = JGDitr->second;
		}
		else{
		//�����ꍇ�͏��擾
	
		}
	}

	return true;
	
}


bool CMeshData2List::MakeList(FiveMeshInfo &r5MeshInfo, CheckList *pCheckList)
{



return true;

}
