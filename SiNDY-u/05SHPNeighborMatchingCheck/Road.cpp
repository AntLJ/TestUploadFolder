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

	//COM�̏�����
	::CoInitialize(0);


	//1�����b�V���R�[�h�̎擾
	long aFirstMeshCode = (m_SecondMeshCode/100);


	//�������n�n�̃f�[�^�̑��݂���f�B���N�g���܂ł̃p�X�쐬
	std::wostringstream aTKYDirPath;
	aTKYDirPath << aTKYPath << _T("\\") << aDataSetName ; 
	std::wstring  aTKYRoadPath;
	aTKYRoadPath = aTKYDirPath.str();

	//���E���n�n�̃f�[�^�����݂���f�B���N�g���܂ł̃p�X�쐬
	std::wostringstream aJGDDirPath;
	aJGDDirPath << aJGDPath << _T("\\") << aDataSetName ;
	std::wstring aJGDRoadPath;
	aJGDRoadPath = aJGDDirPath.str();

	//���C�����X�g���R���e�i�ɓǂݍ���
	std::vector<std::wstring> aLayerList;
	CReadLayerList aLayer(m_LayerListPath);
	//std::wostringstream aListFile;
	//aListFile << aDataSetName <<"layerlist.txt";
	//std::wstring aListFileName;
	//aListFileName = aListFile.str();
	aLayer.ReadList(aLayerList);

	std::vector<std::wstring>::iterator itr =aLayerList.begin();

	bool aCheck=true;

	//�C�e���[�^���I���܂ŉ�
	while(itr != aLayerList.end())
	{
		std::wstring aLayerName = *itr;
		//�������n�n�A���E���n�n���ꂼ��5���b�V���̏������o���A
		//���E����̏�����̃��X�g�ɂ܂Ƃ߁A�������n�n�̃��X�g�Ɛ��E���n�n�̃��X�g���r����	
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



