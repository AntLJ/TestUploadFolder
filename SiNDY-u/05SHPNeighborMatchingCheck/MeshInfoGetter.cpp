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
#include "MeshInfoGetter.h"
#include "string.h"
#include "function.h"

CMeshInfoGetter::CMeshInfoGetter(std::string aLayerType)
{
	m_LayerType = aLayerType;
}

CMeshInfoGetter::~CMeshInfoGetter(void)
{
}

bool CMeshInfoGetter::GetMeshData(long aMeshCode, TKYMeshInfo *pTKYMeshInfo,int x,int y,const Parameters &rParameters)
{
	//�`�F�b�N�Ώۃ��b�V���̊m��
	pTKYMeshInfo->e_MeshCode	= Function::GetMeshCode(aMeshCode,x,y,rParameters.e_DataSet);

	std::map<long, TKYMeshData>::iterator Titr = m_TKYMeshInfo.find(pTKYMeshInfo->e_MeshCode);
	if( Titr == m_TKYMeshInfo.end() ){
		//SHP������Q�b�g�I�I
		bool aCheck = false;
		if(! ArcFunction::GetTKYInfo(rParameters,&pTKYMeshInfo->e_TKYMeshData,pTKYMeshInfo->e_MeshCode,m_LayerType,&m_OIDSet,&aCheck))
		{
			return aCheck;//���b�V����`���Ȃ��������̓f�[�^�����݂��Ȃ������ׁA��񂪎擾�ł��Ȃ������̂��A�����P�Ɏ��s�����̂��̔��f
		}
		//�`�F�b�N�Y�����b�V���A���̖k�܂��͓����b�V���������ꍇ�A����ۑ�
		if((x==0 && y==0)||(x==1 && y==0)||(x==0 && y==1)){
			if(m_TKYMeshInfo.size() >= 19){
				//���b�V������19�ȏ�ۑ����Ȃ��̂ŁA�v�f����19�ȏ�̏ꍇ�A�擪������
				m_TKYMeshInfo.erase(m_TKYMeshInfo.begin());
			}
				
			m_TKYMeshInfo.insert(std::pair<long,TKYMeshData>(pTKYMeshInfo->e_MeshCode,pTKYMeshInfo->e_TKYMeshData));

		}

	}
	else{
		//�����o�ϐ�(TKY�f�[�^�p�R���e�i)������Q�b�g�I�I
		pTKYMeshInfo->e_TKYMeshData = Titr->second;
	}

	return true;
}

bool CMeshInfoGetter::GetMeshData(long aMeshCode, JGDMeshInfo *pJGDMeshInfo,int x,int y,const Parameters &rParameters)
{
	//�`�F�b�N�Ώۃ��b�V���̊m��
	pJGDMeshInfo->e_MeshCode = Function::GetMeshCode(aMeshCode,x,y,rParameters.e_DataSet);

	std::map<long, JGDMeshData>::iterator Jitr = m_JGDMeshInfo.find(pJGDMeshInfo->e_MeshCode);
	if( Jitr == m_JGDMeshInfo.end() ){
		//SHP������Q�b�g�I�I
		bool aCheck = false;
		if(! ArcFunction::GetJGDInfo(rParameters,&pJGDMeshInfo->e_JGDMeshData,pJGDMeshInfo->e_MeshCode,m_ipFeatureClass,m_LayerType,m_OIDSet,&aCheck))
		{
			return aCheck;//���b�V����`���Ȃ��������̓f�[�^�����݂��Ȃ������ׁA��񂪃Q�b�g�ł��Ȃ������̂��A�����P�Ɏ��s�����̂��̔��f
		}
		//�`�F�b�N�Y�����b�V���A���̖k�܂��͓����b�V���������ꍇ�A����ۑ�
		if((x==0 && y==0)||(x==1 && y==0)||(x==0 && y==1)){
			if(m_JGDMeshInfo.size() >= 19){
				//���b�V������19�ȏ�ۑ����Ȃ��̂ŁA�v�f����19�ȏ�̏ꍇ�A�擪������
				m_JGDMeshInfo.erase(m_JGDMeshInfo.begin());
			}
				
			m_JGDMeshInfo.insert(std::pair<long,JGDMeshData>(pJGDMeshInfo->e_MeshCode,pJGDMeshInfo->e_JGDMeshData));

		}

	}
	else{
		//�����o�ϐ�(JGD�f�[�^�p�R���e�i)������Q�b�g�I�I
		pJGDMeshInfo->e_JGDMeshData = Jitr->second;
	}

	return true;
}


