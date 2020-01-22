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
#include "NeighborMatchingCheck.h"
#include <fstream>
#include <string>
#include "Function.h"
#include "MeshInfoGetter.h"
#include <sstream>
#include "ErrorCheck.h"

CNeighborMatchingCheck::CNeighborMatchingCheck(void)
{
}

CNeighborMatchingCheck::~CNeighborMatchingCheck(void)
{
}

/////////���C���̏���/////////////////////////////////////

int CNeighborMatchingCheck::App(const Parameters &rParams)
{
	int iErrFlag=0;//�G���[�t���O�E�E�E�G���[�����邪�������~�߂Ȃ��ꍇ�ɁA�G���[�ł��邱�Ƃ��L�^���Ă����ϐ�
	int iRET = 0;

	//�Ώۃ��C���̓ǂݍ���
	std::vector<std::string> aLayerList;
	if(! ReadLayer(&aLayerList,rParams.e_LayerList)){
		return 1;
	}

	//���b�V�����X�g�̓ǂݍ���(�`�F�b�N�Ώۃ��b�V�������߂�)
	//���b�V�����X�g�ɕs���Ȓl���������ꍇ�̓G���[�t���O��1�����邪�A�����͎~�߂Ȃ��B
	std::set<long> aTargetMesh;
	if(! GetTargetMesh(&aTargetMesh,rParams,&iErrFlag)){
		return 1;
	}

	//�f�[�^�ǂݍ��݁A�����`�F�b�N�̊J�n
	//���C���̐������J��Ԃ�
	std::vector<std::string>::iterator Litr = aLayerList.begin();
	for(;Litr != aLayerList.end(); Litr++){
		CMeshInfoGetter aMeshInfoGetter(*Litr);

		std::cerr << *Litr << "���C���̃`�F�b�N�J�n" << std::endl;

		//�G���[�������ݗp�e�L�X�g�쐬
		std::ostringstream aErrorFilePath;
		aErrorFilePath << rParams.e_ErrorDir << "\\MatchingError_" << *Litr  << ".txt";

		std::ofstream aErrorFile(aErrorFilePath.str().c_str());
		if( ! aErrorFile.is_open())
		{
			std::cerr << "�G���[�o�͗p�t�@�C�����m�ۂł��܂���ł���" << std::endl;
			return 1;
		}
		//���O�n���h���c�[���ŗ��p�ł���悤�ɐݒ����������
		aErrorFile << "# FREESTYLELOG" << std::endl;
		aErrorFile << "FLAG\tLAYER\tOBJECTID\t�G���[���x��\t�G���[�R�[�h\t�G���[���e\t�Y��MeshCode\t�Y��OBJECTID\t�Y��DIVID\t�Y����ԓ_�ԍ�\t�א�MeshCode\t�א�OBJECTID\t�א�DIVID\t�אڕ�ԓ_�ԍ�\t�p�x�@��\t�p�x�A��\t�p�x�B��\t�p�x�C��" << std::endl;			
		//�`�F�b�N�Ώۃ��b�V���̐������A�J��Ԃ�
		std::set<long>::iterator Mitr = aTargetMesh.begin();
		for(Mitr;Mitr != aTargetMesh.end();Mitr++){	
			std::cerr << *Mitr << "\t";
			//TKY�f�[�^�̎擾
			//TKY�p5���b�V�����i�[�R���e�i
			FiveMeshInfo<TKYMeshInfo> aTKY5MeshInfo;
			if(! aMeshInfoGetter.Get5MeshData(*Mitr,&aTKY5MeshInfo,"TKY",rParams))
			{
				iErrFlag = 1;
				std::cerr << "NG\t���{���n�n�f�[�^�̎擾�Ɏ��s���܂���\t" << std::endl;
				continue;
			}
			//JGD�f�[�^�̎擾
			FiveMeshInfo<JGDMeshInfo> aJGD5MeshInfo;
			if(! aMeshInfoGetter.Get5MeshData(*Mitr,&aJGD5MeshInfo,"JGD",rParams) )
			{
				iErrFlag = 1;
				std::cerr << "NG\t���E���n�n�f�[�^�̎擾�Ɏ��s���܂���\t" << std::endl;
				continue;
			}

			//�����`�F�b�N
			CErrorCheck aErrorCheck;
			//TKY���X�g�쐬
			if (! aErrorCheck.MakeTKYList(aTKY5MeshInfo) )
			{
				std::cerr << "�������n�n�f�[�^�ŃG���[������܂���" << std::endl;
				iRET = 3;
			}
			//JGD�f�[�^�`�F�b�N
			if(! aErrorCheck.CheckJGDData(aJGD5MeshInfo,&aErrorFile,*Litr,rParams.e_Border))
			{
				std::cerr << "���E���n�n�̃f�[�^�ŃG���[������܂���" << std::endl;
				iRET=3;
				continue;
			}
			std::cerr << "OK\t�f�[�^�͐���ł�" << std::endl;
		}
		std::cerr << *Litr << "���C���̃`�F�b�N�I��" << std::endl;
	}
	if (iErrFlag == 1)
	{
		return 1;
	} 
	else
	{
		return iRET;
	}

}



/////////���C�����X�g�ǂݎ��p�֐�///////////////////////////////////////////////////////////////////

	bool CNeighborMatchingCheck::ReadLayer(std::vector<std::string> *pLayerList,const std::string &rListName )
{
	//���C�����X�g���J��
	std::ifstream ifs(rListName.c_str());
	if(! ifs.is_open()){
		std::cerr << "���C�����X�g�̓ǂݍ��݂Ɏ��s���܂���" << std::endl;
		return false;
	}

	while(!ifs.eof())
	{
		//���C���ǂݍ��ݗp������
		char aBuffer[128];
		ifs.getline(aBuffer,sizeof(aBuffer),'\n');
		if(strcmp(aBuffer,"")==0) continue;
		//���C�������R���e�i��
		std::string aLayerName(aBuffer);
		pLayerList->push_back(aLayerName);
	}

	return true;

}

/////////�`�F�b�N�Ώۃ��b�V�����o�p�֐�///////////////////////////////////////////////////////

bool CNeighborMatchingCheck::GetTargetMesh(std::set<long> *pTargetMesh, const Parameters &rParams, int *iErrFlag)
{	
	//���{��t�@�C�����ɑΉ������
    std::locale loc = std::locale();
   loc = std::locale(loc, "japanese", std::locale::ctype);
    std::locale::global(loc);
	//���b�V�����X�g���J��
	std::ifstream ifs;
	//ifs.imbue(std::locale("japanese"));
	//std::locale::global( std::locale( "japanese" ) ); 
	ifs.open(rParams.e_MeshList.c_str());
	if(! ifs.is_open()){
		std::cerr << "���b�V�����X�g�̓ǂݍ��݂Ɏ��s���܂���" << std::endl;
		return false;
	}

	while(!ifs.eof())
	{
		//���b�V���ǂݍ��ݗp������
		char aBuffer[9];
		ifs.getline(aBuffer,sizeof(aBuffer));
		if (aBuffer[0] == NULL) break;
		long aMeshCode = std::atoi(aBuffer);
		//�ǂݍ��񂾃��b�V�����`�F�b�N
		if(! MeshCheck(aMeshCode,rParams.e_DataSet)){
			std::cerr << aMeshCode << "�͕s���ȃ��b�V���ł�" << std::endl;
			*iErrFlag = 1;
			continue;
		}
		//���b�V���R�[�h���R���e�i�Ɋi�[
		pTargetMesh->insert(aMeshCode);
#ifdef _DEBUG
		std::cerr << aMeshCode << std::endl;
#endif
	}

	//�`�F�b�N�Ώ۔͈͂��S���ł͂Ȃ��ꍇ(�ݒ�t�@�C����CHECKAREA��part�̏ꍇ)���b�V����ǉ�
	if(rParams.e_Area == "part")
	{
		//�ǉ����b�V���p�R���e�i
		std::set<long> SubMeshList;
		
		//�ǂݍ��񂾊e���b�V���ɑ΂��A����(���Ɛ�)�̃��b�V�����ǂݍ��܂�Ă��Ȃ��ꍇ�͒ǉ�����
		std::set<long>::iterator Titr = pTargetMesh->begin();

		for(;Titr != pTargetMesh->end();Titr ++){
			
			//�����b�V����ǂ݂��񂾃��b�V�����X�g���猟�����A�Ȃ���Βǉ����b�V���p�R���e�i�ɒǉ�
			long aEastMeshCode = Function::GetMeshCode(*Titr,1,0,rParams.e_DataSet);

			if( pTargetMesh->find(aEastMeshCode) == pTargetMesh->end() && aEastMeshCode != -1  )
			{
				SubMeshList.insert(aEastMeshCode);
			}

			//�����b�V����ǂ݂��񂾃��b�V�����X�g���猟�����A�Ȃ���Βǉ����b�V���p�R���e�i�ɒǉ�
			long aWestMeshCode = Function::GetMeshCode(*Titr,-1,0,rParams.e_DataSet);
		
			if( pTargetMesh->find(aWestMeshCode) == pTargetMesh->end() && aWestMeshCode != -1 )
			{
				SubMeshList.insert(aWestMeshCode);
			}
		}

		//�ǉ����b�V���p�R���e�i�̃f�[�^��pTargetMesh�ɒǉ�����B
		std::set<long>::iterator Sitr = SubMeshList.begin();
		for(;Sitr != SubMeshList.end();Sitr ++){
			pTargetMesh->insert(*Sitr);
		}
	}

	return true;

}

/////////���b�V���̐������`�F�b�N�֐�////////////////////////////////////

bool CNeighborMatchingCheck::MeshCheck(long aMeshCode, std::string aDataSet)
{
	//�ǂݍ��񂾕����񂪃��b�V���R�[�h�ł��邩�`�F�b�N
	if ( aDataSet == "city")
	{
		long a1stY = aMeshCode/1000000;
		long a1stX = (aMeshCode/10000)%100;
		long a2ndY = (aMeshCode/1000)%10;
		long a2ndX = (aMeshCode/100)%10;
		long a3rdY = (aMeshCode/10)%10;
		long a3rdX = aMeshCode%10;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;
		if( (a2ndY < 0 ) || (a2ndY > 7 ) ) return false;
		if( (a2ndX < 0 ) || (a2ndX > 7 ) ) return false;
		if( (a3rdY < 0 ) || (a3rdY > 7 ) ) return false;
		if( (a3rdX < 0 ) || (a3rdX > 7 ) ) return false;

		return true;
	}
	else if (aDataSet == "base")
	{
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
	else if (aDataSet == "middle")
	{
		long a1stY = aMeshCode/100;
		long a1stX = aMeshCode%100;

		if( (a1stY < 29) || (a1stY > 68) ) return false;
		if( (a1stX < 22) || (a1stX > 53) ) return false;

		return true;
	}
	else if(aDataSet == "top")
	{
		if( (aMeshCode != 0) && (aMeshCode != 1) && (aMeshCode != 10) && (aMeshCode != 11)) return false;

		return true;
	}

	return false;

}		




