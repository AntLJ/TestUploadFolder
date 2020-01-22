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
#include "GetLayerInfo.h"
#include "GetInfo.h"
#include "Merge.h"
#include <iostream>
#include <sstream>
#include <atl2/io.h>


CGetLayerInfo::CGetLayerInfo(long aMeshCode,std::wstring aAllow)
{
	m_MeshCode = aMeshCode;
	m_Allow = aAllow;
}

CGetLayerInfo::~CGetLayerInfo(void)
{
}

bool CGetLayerInfo::Merge5MeshInfo(std::wstring &rDirName, std::wstring & rLayerName, std::set<CheckKey> &rMergeSet)
{

	long aShapeType;

	if( (m_MeshCode <= 6848)&&(m_MeshCode >= 2936)) //�~�h���}�b�v�̏ꍇ
	{
		 aShapeType = Merge5MiddleMesh(rDirName,rLayerName);
	}
	else if( (m_MeshCode <= 684827)&&(m_MeshCode >= 303640)) //�x�[�X�}�b�v�̏ꍇ
	{
		aShapeType = Merge5BaseMesh(rDirName,rLayerName);
	}
	else if( (m_MeshCode <= 68482777)&&(m_MeshCode >= 30364000))//�V�e�B�}�b�v�̏ꍇ
	{
		aShapeType = Merge5CityMesh(rDirName,rLayerName);
	}
	else
	{
		std::cerr << m_MeshCode <<"�͑ΏۊO�̃��b�V���ł�" <<std::endl;
		return false;
	}

	if(_tcscmp(m_Allow.c_str(),_T("No"))==0)
	{
		CMerge aMerge;
		bool aOverlap = false;
		if(aMerge.MakeList(m_MainMap, m_NorthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeList(m_MainMap, m_SouthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeList(m_MainMap, m_EastMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeList(m_MainMap, m_WestMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}

		return aOverlap;
	}
	else if(_tcscmp(m_Allow.c_str(),_T("Yes"))==0)
	{
		CMerge aMerge;
		bool aOverlap = false;
		if(aMerge.MakeListAllowError(m_MainMap, m_NorthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeListAllowError(m_MainMap, m_SouthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeListAllowError(m_MainMap, m_EastMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeListAllowError(m_MainMap, m_WestMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}

		return aOverlap;
	}
	else
	{
		std::cout<<"��5�������������ݒ肳��Ă��܂���"<<std::endl;

		return false;
	}

}

long CGetLayerInfo::Merge5MiddleMesh(std::wstring &rDirName, std::wstring &rLayerName)
{
	long aShapeType;
	
	CGetInfo aGetInfo;
	//���ڃ��b�V���̏����R���e�i��
	std::wostringstream aMainMeshDir;
	aMainMeshDir << rDirName  << _T("\\") << m_MeshCode;
	std::wstring aDirPath = aMainMeshDir.str();
	if(! aGetInfo.GetInformation( m_MainMap, aDirPath, rLayerName,m_MeshCode,aShapeType)){
		std::cerr << m_MeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	//���ڃ��b�V���̏�̃��b�V���̏����R���e�i��
	long aNorthMeshCode = m_MeshCode+200;	//��̃��b�V���̃��b�V���R�[�h
	std::wostringstream aNorthMeshDir;			
	aNorthMeshDir << rDirName << _T("\\") << aNorthMeshCode;
	aDirPath= aNorthMeshDir.str();

	if(! aGetInfo.GetInformation( m_NorthMap,aDirPath,rLayerName,aNorthMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}
	
	//���ڃ��b�V���̉��̃��b�V���̏����R���e�i��
	long aSouthMeshCode = m_MeshCode-200;	
	std::wostringstream aSouthMeshDir;
	aSouthMeshDir << rDirName << _T("\\") << aSouthMeshCode;
	aDirPath = aSouthMeshDir.str();
	if(! aGetInfo.GetInformation( m_SouthMap,aDirPath,rLayerName,aSouthMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" <<  rLayerName <<"�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}
	
	//���ڃ��b�V���̉E�̃��b�V���̏����R���e�i��
	long aEastMeshCode = m_MeshCode+2;
	std::wostringstream aEastMeshDir;
	aEastMeshDir << rDirName << _T("\\") << aEastMeshCode;
	aDirPath = aEastMeshDir.str();
	if(! aGetInfo.GetInformation( m_EastMap,aDirPath,rLayerName,aEastMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" <<  rLayerName <<"�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	//���ڃ��b�V���̍��̃��b�V���̏����R���e�i��
	long aWestMeshCode = m_MeshCode-2;
	std::wostringstream aWestMeshDir;
	aWestMeshDir << rDirName << _T("\\") << aWestMeshCode;
	aDirPath = aWestMeshDir.str();
	if(! aGetInfo.GetInformation( m_WestMap,aDirPath,rLayerName,aWestMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" <<  rLayerName <<"�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	return aShapeType;
}



long CGetLayerInfo::Merge5BaseMesh(std::wstring &rDirName, std::wstring &rLayerName)
{
	long aShapeType;

	CGetInfo aGetInfo;
	//���ڃ��b�V���̏����R���e�i��
	long aFirstMeshCode = (m_MeshCode/100);
	std::wostringstream aMainMeshDir;
	aMainMeshDir << rDirName << _T("\\") << aFirstMeshCode << _T("\\") << m_MeshCode;
	std::wstring aDirPath = aMainMeshDir.str();
	if(! aGetInfo.GetInformation( m_MainMap, aDirPath, rLayerName,m_MeshCode,aShapeType)){
		std::cerr << m_MeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	//���ڃ��b�V���Ə㉺���b�V���̋��E����1�����b�V���̋��E���ɂȂ��ĂȂ����̔���p�ϐ�
	long aNorthSouthCheck = (m_MeshCode%100)/10;

	//���ڃ��b�V���̏�̃��b�V���R�[�h�擾
	long aNorthMeshCode;			
	if( aNorthSouthCheck == 7 ){
	    aNorthMeshCode = m_MeshCode - 70 + 10000;	
	}
	else{
		aNorthMeshCode = m_MeshCode + 10;
	}

	//1�����b�V���R�[�h�擾
	long aNorthFirstMeshCode = (aNorthMeshCode/100);
	//�p�X�̍쐬
	std::wostringstream aNorthMeshDir;			
	aNorthMeshDir << rDirName << _T("\\") << aNorthFirstMeshCode << _T("\\") << aNorthMeshCode;
	aDirPath = aNorthMeshDir.str();
	if(! aGetInfo.GetInformation( m_NorthMap, aDirPath, rLayerName,aNorthMeshCode,aShapeType)){
		std::cerr << aNorthMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	//���ڃ��b�V���̉��̃��b�V���̃��b�V���R�[�h
	long aSouthMeshCode;

	//���̃��b�V���̏����R���e�i��
	if( aNorthSouthCheck == 0 ){
		aSouthMeshCode = m_MeshCode + 70 - 10000;
	}
	else{
		aSouthMeshCode = m_MeshCode - 10;
	}
	//1�����b�V���R�[�h�擾
	long aSouthFirstMeshCode = (aSouthMeshCode/100);
	//�p�X�̍쐬
	std::wostringstream aSouthMeshDir;
	aSouthMeshDir << rDirName << _T("\\") << aSouthFirstMeshCode << _T("\\")<< aSouthMeshCode;
	aDirPath = aSouthMeshDir.str();
	
	if(! aGetInfo.GetInformation( m_SouthMap, aDirPath, rLayerName,aSouthMeshCode,aShapeType)){
		std::cerr << aSouthMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}
		

	//���ڃ��b�V���ƍ��E���b�V���̋��E����1�����b�V���̋��E���ɂȂ��ĂȂ����̔���p�ϐ�
	long aEastWestCheck = m_MeshCode%10;
		
	//���ڃ��b�V���̉E�̃��b�V���̃��b�V���R�[�h
	long aEastMeshCode;

	//�E�̃��b�V���̏����R���e�i��
	if( aEastWestCheck == 7){
		aEastMeshCode = m_MeshCode - 7 + 100;
	}
	else{
		aEastMeshCode = m_MeshCode + 1;
	}
	//1�����b�V���R�[�h�擾
	long aEastFirstMeshCode = aEastMeshCode/100;
	std::wostringstream aEastMeshDir;
	aEastMeshDir << rDirName << _T("\\") << aEastFirstMeshCode << _T("\\") << aEastMeshCode;
	aDirPath = aEastMeshDir.str();
	if(! aGetInfo.GetInformation( m_EastMap, aDirPath, rLayerName,aEastMeshCode,aShapeType)){
		std::cerr << aEastMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	//���ڃ��b�V���̍��̃��b�V���̃��b�V���R�[�h�i
	long aWestMeshCode;

	//���̃��b�V���̏����R���e�i��
	if ( aEastWestCheck == 0){
		aWestMeshCode = m_MeshCode + 7 -100;
	}
	else{
		aWestMeshCode = m_MeshCode - 1;
	}
	//1�����b�V���R�[�h�擾
	long aWestFirstMeshCode = aWestMeshCode/100;
	std::wostringstream aWestMeshDir;
	aWestMeshDir << rDirName << _T("\\") << aWestFirstMeshCode << _T("\\") << aWestMeshCode;
	aDirPath = aWestMeshDir.str();
	if(! aGetInfo.GetInformation( m_WestMap, aDirPath, rLayerName,aWestMeshCode,aShapeType)){
		std::cerr << aWestMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}

	return aShapeType;


}


long CGetLayerInfo::Merge5CityMesh(std::wstring &rDirName, std::wstring &rLayerName)
{
	long aShapeType;

	CGetInfo aGetInfo;

	long aSecondMeshCode = (m_MeshCode/100);
	long aFirstMeshCode = (aSecondMeshCode/100);

	std::wostringstream aMainMeshDir;
	aMainMeshDir << rDirName << _T("\\") << aFirstMeshCode << _T("\\") << aSecondMeshCode << _T("\\") <<m_MeshCode;
	std::wstring aDirPath = aMainMeshDir.str();
	if(! aGetInfo.GetInformation( m_MainMap, aDirPath, rLayerName,m_MeshCode,aShapeType)){
		std::cerr << m_MeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B" << std::endl;
	}




	//���ڃ��b�V���Ə㉺���b�V���̋��E����2�����b�V���̋��E���ɂȂ��ĂȂ����̔���p�ϐ�
	long aFirstNorthSouthCheck = (m_MeshCode%100)/10;
	//���ڃ��b�V���Ə㉺���b�V���̋��E����1�����b�V���̋��E���ɂȂ��ĂȂ����̔���p�ϐ�
	long aSecondNorthSouthCheck = (m_MeshCode%10000)/1000;
	//���ڃ��b�V���̏�̃R�[�h
	long aNorthMeshCode;
	//���ڃ��b�V���̏�̃��b�V���R�[�h�擾
	if(aFirstNorthSouthCheck == 7){
		if( aSecondNorthSouthCheck == 7){
			aNorthMeshCode = m_MeshCode-70-7000+1000000;
		}
		else{
			aNorthMeshCode = m_MeshCode-70+1000;
		}
	}
	else{
		aNorthMeshCode = m_MeshCode+10;
	}
	//2���A1�����b�V���̎擾
	long aNorthSecondMeshCode = aNorthMeshCode/100;
	long aNorthFirstMeshCode = aNorthSecondMeshCode/100;
	//�p�X�擾
	std::wostringstream aNorthMeshDir;
	aNorthMeshDir << rDirName << "\\" << aNorthFirstMeshCode << "\\" << aNorthSecondMeshCode << "\\" << aNorthMeshCode;
	aDirPath = aNorthMeshDir.str();

	//���ڃ��b�V���̏�̃��b�V���̏��擾
	if(! aGetInfo.GetInformation( m_NorthMap, aDirPath, rLayerName, aNorthMeshCode,aShapeType)){
		std::cerr << aNorthMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B\n" <<std::endl;
	}

	//���ڃ��b�V���̉��̃R�[�h
	long aSouthMeshCode;
	//���ڃ��b�V���̉��̃��b�V���̃��b�V���R�[�h�擾
	if(aFirstNorthSouthCheck == 0){
		if(aSecondNorthSouthCheck == 0){
			aSouthMeshCode = m_MeshCode +70 +7000 -1000000;
		}
		else{
			aSouthMeshCode = m_MeshCode +70 -1000;
		}
	}
	else{
		aSouthMeshCode = m_MeshCode -10;
	}
	//2���A1�����b�V���̎擾
	long aSouthSecondMeshCode = aSouthMeshCode/100;
	long aSouthFirstMeshCode = aSouthSecondMeshCode/100;
	//�p�X�擾
	std::wostringstream aSouthMeshDir;
	aSouthMeshDir << rDirName << "\\" << aSouthFirstMeshCode << "\\" << aSouthSecondMeshCode << "\\" << aSouthMeshCode;
	aDirPath = aSouthMeshDir.str();

	//���ڃ��b�V���̉��̃��b�V���̏��擾
	if(! aGetInfo.GetInformation( m_SouthMap, aDirPath, rLayerName, aSouthMeshCode,aShapeType)){
		std::cerr << aSouthMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B\n" <<std::endl;
	}

	//���ڃ��b�V���ƍ��E���b�V���̋��E����2�����b�V���̋��E���ɂȂ��ĂȂ����̔���p�ϐ�
	long aFirstEastWestCheck = (m_MeshCode%100)/10;
	//���ڃ��b�V���ƍ��E���b�V���̋��E����1�����b�V���̋��E���ɂȂ��ĂȂ����̔���p�ϐ�
	long aSecondEastWestCheck = (m_MeshCode%10000)/1000;

	//���ڃ��b�V���̉E�̃��b�V���̃R�[�h�i�[�p
	long aEastMeshCode;
	//���ڃ��b�V���̉E�̃��b�V���R�[�h�擾
	if( aFirstEastWestCheck == 7){
		if( aSecondEastWestCheck == 7){
			aEastMeshCode = m_MeshCode -7 -700 +10000;
		}
		else{
			aEastMeshCode = m_MeshCode -7 +100;
		}
	}
	else{
		aEastMeshCode = m_MeshCode +1;
	}
	//2���A1�����b�V���̎擾
	long aEastSecondMeshCode = aEastMeshCode/100;
	long aEastFirstMeshCode = aEastSecondMeshCode/100;
	//�p�X�擾
	std::wostringstream aEastMeshDir;
	aEastMeshDir << rDirName << "\\" << aEastFirstMeshCode << "\\" << aEastSecondMeshCode << "\\" << aEastMeshCode;
	aDirPath = aEastMeshDir.str();

	//���ڃ��b�V���̉E�̃��b�V���̏��擾
	if(! aGetInfo.GetInformation( m_EastMap, aDirPath, rLayerName, aEastMeshCode,aShapeType)){
		std::cerr << aEastMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B\n" <<std::endl;
	}

	//���ڃ��b�V���̍��̃��b�V���̃R�[�h�i�[�p
	long aWestMeshCode;
	//���ڃ��b�V���̍��̃��b�V���̃R�[�h�擾
	if( aFirstEastWestCheck == 0){
		if( aSecondEastWestCheck == 0){
			aWestMeshCode = m_MeshCode +7+700 -10000;
		}
		else{
			aWestMeshCode = m_MeshCode +7-100;
		}
	}
	else{
		aWestMeshCode = m_MeshCode -1;
	}
	//2���A1�����b�V���̎擾
	long aWestSecondMeshCode = aWestMeshCode/100;
	long aWestFirstMeshCode = aWestSecondMeshCode/100;
	//�p�X�擾
	std::wostringstream aWestMeshDir;
	aWestMeshDir << rDirName << "\\" << aWestFirstMeshCode << "\\" << aWestSecondMeshCode << "\\" << aWestMeshCode;
	aDirPath = aWestMeshDir.str();

	//���ڃ��b�V���̍��̃��b�V���̏��擾
	if(! aGetInfo.GetInformation( m_WestMap, aDirPath, rLayerName, aWestMeshCode ,aShapeType)){
		std::cerr << aWestMeshCode << "_" << rLayerName << "�̏��̎擾�Ɏ��s���܂����B�f�[�^��\n" <<std::endl;
	}

	return aShapeType;

}












