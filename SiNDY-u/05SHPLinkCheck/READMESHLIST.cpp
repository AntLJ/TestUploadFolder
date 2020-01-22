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
#include "READMESHLIST.h"
#include <iostream>
#include <fstream>
#include <atl2/io.h>

CREADMESHLIST::CREADMESHLIST(_TCHAR* pFilePath)
{
	//���b�V�����X�g�t�@�C���̃t���p�X���Z�b�g
	m_FilePath = pFilePath;   

}

CREADMESHLIST::~CREADMESHLIST(void)
{
}

bool CREADMESHLIST::ReadMeshList(std::vector<long>& rMeshList)
{

	//���b�V�����X�g�t�@�C�����J��
	std::ifstream ifs(m_FilePath.c_str(), std::ios::in);

	if( !ifs.is_open() ){
		std::cerr << m_FilePath <<"�̓ǂݍ��݂Ɏ��s���܂���"<<std::endl;
		return false;
	}
	while(!ifs.eof()){
		//7�o�C�g�̓ǂݍ��ݗp������
		char aBuffer[7];
		//7�o�C�g�ǂށB���ꂪ���b�V���R�[�h
		ifs.getline(aBuffer,7);
		long aMeshCode = std::atoi(aBuffer);
		//���b�V���R�[�h�̃`�F�b�N
		if( aMeshCode == 0) continue;
		if(! MeshCheck(aMeshCode) ){
			std::cerr << aMeshCode << "�͕s���ȃ��b�V���R�[�h�ł��B�������I�����܂��B" << std::endl;
			return false;
		}
		//���b�V���R�[�h���R���e�i��
		rMeshList.push_back(aMeshCode);
	}
	return true;
}

bool CREADMESHLIST::MeshCheck(long aMeshCode)
{
	//�ǂݍ��񂾃��b�V���R�[�h�����������`�F�b�N

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
