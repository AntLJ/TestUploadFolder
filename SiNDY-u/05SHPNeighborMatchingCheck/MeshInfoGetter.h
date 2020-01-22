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
#include<set>


class CMeshInfoGetter
{
public:
	CMeshInfoGetter(std::string LayerType);
	~CMeshInfoGetter(void);
	template <class Type> bool Get5MeshData(long aMeshCode, FiveMeshInfo<Type> *pFiveMeshInfo,std::string aGeoType,const Parameters &rParameters)
	{

		bool aCheck = true;
		//�Y�����b�V���̏��擾
		if(! GetMeshData(aMeshCode,&pFiveMeshInfo->e_MainMesh,0,0,rParameters)){
			std::cerr << aMeshCode << "_" << m_LayerType << "(" << aGeoType << ")" << "�̏��擾�Ɏ��s���܂���\t";
			aCheck = false;
		}

		//�Y�����b�V���k�̏��擾
		if(! GetMeshData(aMeshCode,&pFiveMeshInfo->e_NorthMesh,0,1,rParameters)){
			std::cerr << aMeshCode << "_" << m_LayerType << "(" << aGeoType << ")" << "�̏��擾�Ɏ��s���܂���\t";
			aCheck = false;
		}

		//�Y�����b�V����̏��擾
		if(! GetMeshData(aMeshCode,&pFiveMeshInfo->e_SouthMesh,0,-1,rParameters)){
			std::cerr << aMeshCode << "_" << m_LayerType << "(" << aGeoType << ")" << "�̏��擾�Ɏ��s���܂���\t";
			aCheck = false;
		}

		//�Y�����b�V�����̏��擾
		if(! GetMeshData(aMeshCode,&pFiveMeshInfo->e_EastMesh,1,0,rParameters)){
			std::cerr << aMeshCode << "_" << m_LayerType << "(" << aGeoType << ")" << "�̏��擾�Ɏ��s���܂���\t";
			aCheck = false;
		}

		//�Y�����b�V�����̏��擾
		if(! GetMeshData(aMeshCode,&pFiveMeshInfo->e_WestMesh,-1,0,rParameters)){
			std::cerr << aMeshCode << "_" << m_LayerType << "(" << aGeoType << ")" << "�̏��擾�Ɏ��s���܂���\t";
			aCheck = false;
		}
	
		return aCheck;

	}


private:
	std::string m_LayerType;//�`�F�b�N�Ώۃ��C��
	IFeatureClassPtr m_ipFeatureClass; //���b�V����`�̃t�B�[�`���N���X
	std::map<long,FID_Set> m_OIDSet; //���b�V�����E�ڐG�I�u�W�F�N�g��OBJECTID�i�[�R���e�i long�FMeshCode 
	std::map<long, TKYMeshData> m_TKYMeshInfo; //���FMeshCode
	std::map<long, JGDMeshData> m_JGDMeshInfo; //���FMeshCode
	bool GetMeshData(long aMeshCode, TKYMeshInfo *pTKYMeshInfo,int x,int y,const Parameters &rParameters);
	bool GetMeshData(long aMeshCode, JGDMeshInfo *pJGDMeshInfo,int x,int y,const Parameters &rParameters);


};
