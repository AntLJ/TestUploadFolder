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

// MatchingCheck.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include"ReadBMList.h"
#include"ReadMeshList.h"
#include <iostream>
#include "Road.h"
#include "time.h"


int _tmain(int argc, _TCHAR* argv[])
{

	clock_t start = clock();
	//argv[1]���b�V�����X�g�܂ł̃t���p�X
	//argv[2]���C�����X�g�܂ł̃t���p�X�@
	//argv[3]�������n�n�̑S�Ẵf�[�^�Z�b�g���i�[����Ă���f�B���N�g���܂ł̃p�X�@
	//argv[4]���E���n�n�̑S�ăf�[�^�Z�b�g���i�[����Ă���f�B���N�g���܂ł̃p�X
	//argv[5]�`�F�b�N���s���f�[�^�Z�b�g�̐ݒ� r :Road�̂݁@m :Middle�̂� b :base�̂� c :City�̂� a :�S�Ẵf�[�^�Z�b�g
	//argv[6]�����_��10�ʈȉ��̌덷�����e���邩�̐ݒ� Yes:���e����@No:���e���Ȃ�
	if(argc != 7){
		std::cerr << "�����̐�������������܂���B" << std::endl;
		return 0;
	}

	std::wstring aMeshListPath = argv[1];
	std::wstring aLayerListPath = argv[2];
	std::wstring aTKYPath = argv[3];
	std::wstring aJGDPath = argv[4];
	std::wstring aSetting = argv[5];
	std::wstring aAllow = argv[6];

	if(_tcscmp(aAllow.c_str(),_T("No"))!=0 && _tcscmp(aAllow.c_str(),_T("Yes"))!=0){
		std::cerr << "��6�������������ݒ肳��Ă��܂���" << std::endl;
		return 0;
	}

	//Road�̃`�F�b�N
	if(_tcscmp(aSetting.c_str(), _T("r"))==0){

		std::cout << "�f�[�^�Z�b�g:Road �`�F�b�N�J�n\n"<< std::endl;
		std::cerr << "�f�[�^�Z�b�g:Road �`�F�b�N�J�n\n"<< std::endl;
	
		//���b�V�����X�g���R���e�i�Ɋi�[
		std::set<long> aRoadMeshList;
		std::wstring aRoadSet(_T("Road"));
		CReadMeshList ReadRoadMeshList(aMeshListPath, aRoadSet);
		if(! ReadRoadMeshList.ReadList((std::set<long> &)aRoadMeshList))
		{
			std::cerr << "���b�V�����X�g�̎擾�Ɏ��s���܂���\n" << std::endl;
			std::cout << "�f�[�^�Z�b�g:Road�`�F�b�N���s\n" << std::endl;
			std::cerr << "�f�[�^�Z�b�g:Road�`�F�b�N���s\n" << std::endl;
		}
		else{

			//�e���b�V���R�[�h�ɑ΂��A�אڐ����`�F�b�N���s���B
			std::set<long>::iterator Ritr = aRoadMeshList.begin();
			for(;Ritr != aRoadMeshList.end();Ritr++){
				CRoad aRoad(*Ritr,aLayerListPath,aAllow);
				if( aRoad.Main(aTKYPath,aJGDPath,aRoadSet))
				{
					std::cout <<"���b�V��:"<< *Ritr << "�͐���ł�" << std::endl;
				}
			}
	
			std::cout << "\n�f�[�^�Z�b�g:Road �`�F�b�N�I��\n"<< std::endl;
			std::cerr << "\n�f�[�^�Z�b�g:Road �`�F�b�N�I��\n"<< std::endl;
		}
	}

	//Base�̃`�F�b�N
	if(_tcscmp(aSetting.c_str(), _T("b"))==0){
		std::cout << "�f�[�^�Z�b�g:Base �`�F�b�N�J�n\n"<< std::endl;
		std::cerr << "�f�[�^�Z�b�g:Base �`�F�b�N�J�n\n"<< std::endl;
	
		//Base�̃f�[�^�Z�b�g�̃��b�V�����X�g���R���e�i�Ɋi�[
		std::set<long> aBaseMeshList;
		std::wstring aBaseSet(_T("Basemap"));
		CReadMeshList ReadBaseMeshList(aMeshListPath, aBaseSet);
		if(! ReadBaseMeshList.ReadList((std::set<long> &)aBaseMeshList))
		{
			std::cerr << "���b�V�����X�g�̎擾�Ɏ��s���܂���\n" << std::endl;
			std::cout << "�f�[�^�Z�b�g:Base�`�F�b�N���s\n" << std::endl;
			std::cerr << "�f�[�^�Z�b�g:Base�`�F�b�N���s\n" << std::endl;
		}
		else{

			//�e���b�V���R�[�h�ɑ΂��A�אڐ����`�F�b�N���s���B
			std::set<long>::iterator Bitr = aBaseMeshList.begin();
			for(; Bitr != aBaseMeshList.end(); Bitr++){
				CRoad aBase(*Bitr, aLayerListPath,aAllow );
				if( aBase.Main(aTKYPath,aJGDPath,aBaseSet))
				{
					std::cout  <<"���b�V��:"<< *Bitr << "�͐���ł�"<< std::endl; 
				}
			}

			std::cout << "\n�f�[�^�Z�b�g:Base �`�F�b�N�I��\n"<< std::endl;
			std::cerr << "\n�f�[�^�Z�b�g:Base �`�F�b�N�I��\n"<< std::endl;
		}
	}

	//Middle�̃`�F�b�N
	if(_tcscmp(aSetting.c_str(), _T("m"))==0){

		std::cout << "�f�[�^�Z�b�g:Middle �`�F�b�N�J�n\n" << std::endl;
		std::cerr << "�f�[�^�Z�b�g:Middle �`�F�b�N�J�n\n" << std::endl;
	
		//Middle�̃f�[�^�Z�b�g�̃��b�V�����X�g���R���e�i�Ɋi�[
		std::set<long> aMiddleMeshList;
		std::wstring aMiddleSet(_T("Middlemap"));
		CReadMeshList ReadMiddleMeshList(aMeshListPath, aMiddleSet);
		if(! ReadMiddleMeshList.ReadList((std::set<long> &)aMiddleMeshList))
		{
			std::cerr << "���b�V�����X�g�̎擾�Ɏ��s���܂���\n" << std::endl;
			std::cout << "�f�[�^�Z�b�g:Middle�`�F�b�N���s\n" << std::endl;
			std::cerr << "�f�[�^�Z�b�g:Middle�`�F�b�N���s\n" << std::endl;
		}
		else{

			//�e���b�V���R�[�h�ɑ΂��A�אڐ����`�F�b�N���s���B
			std::set<long>::iterator Mitr = aMiddleMeshList.begin();
			for(; Mitr != aMiddleMeshList.end(); Mitr++){
				CRoad aMiddle(*Mitr, aLayerListPath,aAllow );
				if( aMiddle.Main(aTKYPath,aJGDPath,aMiddleSet) )
				{
					std::cout  <<"���b�V��:"<< *Mitr << "�͐���ł�"<< std::endl;
				}
			}	

			std::cout << "\n�f�[�^�Z�b�g:Middle �`�F�b�N�I��\n" << std::endl;
			std::cerr << "\n�f�[�^�Z�b�g:Middle �`�F�b�N�I��\n" << std::endl;
		}
	}

	//City�̃`�F�b�N
	if(_tcscmp(aSetting.c_str(),_T("c"))==0 ){

		std::cout << "�f�[�^�Z�b�g:City�`�F�b�N�J�n\n" << std::endl;
		std::cerr << "�f�[�^�Z�b�g:City�`�F�b�N�J�n\n" << std::endl;
	
		//City�̃f�[�^�Z�b�g�̃��b�V�����X�g���R���e�i�Ɋi�[
		std::set<long> aCityMeshList;
		std::wstring aCitySet(_T("Citymap"));
		CReadMeshList ReadCityMeshList(aMeshListPath, aCitySet);
		if(! ReadCityMeshList.ReadList((std::set<long> &)aCityMeshList))
		{
			std::cerr << "���b�V�����X�g�̎擾�Ɏ��s���܂���\n" << std::endl;
			std::cout << "�f�[�^�Z�b�g:City�`�F�b�N���s\n" << std::endl;
			std::cerr << "�f�[�^�Z�b�g:City�`�F�b�N���s\n" << std::endl;
		}
		else{
	
			//�e���b�V���R�[�h�ɑ΂��A�אڐ����`�F�b�N���s���B
			std::set<long>::iterator Citr = aCityMeshList.begin();
			for(Citr; Citr != aCityMeshList.end(); Citr++){
				CRoad aCity(*Citr, aLayerListPath,aAllow );
				if( aCity.Main(aTKYPath,aJGDPath,aCitySet))
				{
					std::cout << "���b�V��:" << *Citr << "�͐���ł�" << std::endl;
				}
			}	

			std::cout << "\n�f�[�^�Z�b�g:City�`�F�b�N�I��\n" << std::endl;
			std::cerr << "\n�f�[�^�Z�b�g:City�`�F�b�N�I��\n" << std::endl;
		}
	}

	clock_t end = clock();
	std::cerr << (double)(end-start)/CLOCKS_PER_SEC << "sec������܂���" << std::endl;
	return 0;

}
