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

#pragma once
#include<map>
#include<set>


//Mesh�f�[�^�ۑ��p�\����
struct MeshInfo
{
	long e_OID;
	long e_X;
	long e_Y;
	long e_PNum;
    std::vector<IPointPtr> e_PointSet;
};

//�Y���{������k���b�V���f�[�^�\����
struct FiveMeshInfo
{
	MeshInfo e_MainMesh;
	MeshInfo e_NorthMesh;
	MeshInfo e_SouthMesh;
	MeshInfo e_EastMesh;
	MeshInfo e_WestMesh;
};

class CMeshData2List
{
public:
	CMeshData2List(std::string aLayerType);
	~CMeshData2List(void);
	bool Data2List(long aMeshCode, CheckList *pCheckList,  std::string aGeoType);
    

private:
	std::set<long> m_OIDSet;
	std::map<long,MeshInfo> m_TKYMeshData;
	std::map<long,MeshInfo> m_JGDMeshData;
	std::string m_LayerType;
	IGeometryPtr ipGeometry;					//���b�V����`��SHP


	bool GetMeshInfo(MeshInfo * pMeshInfo,long aMeshcode,std::string aGeoType);
	bool MakeList(FiveMeshInfo & r5MeshInfo,CheckList *pCheckList);

};
