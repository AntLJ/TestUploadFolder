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
#include "GetInfo.h"
#include "GetPointInfo.h"
#include "GetPolyLineInfo.h"
#include "GetPolygonInfo.h"
#include <sstream>
#include <atl2/io.h>


CGetInfo::CGetInfo(void)
{
}


CGetInfo::~CGetInfo(void)
{
}

bool CGetInfo::GetInformation(std::multimap<Key,Info> & rInfoMap, std::wstring & rDirName, std::wstring & rLayerName, long aMeshCode,long & rFeatureType)
{
	
	//HRESULT hr;

	//�f�B���N�g���̑��݊m�F
	if(! PathIsDirectory(rDirName.c_str()))
	{
		std::cerr <<  rDirName << "\\" << aMeshCode << "_" << rLayerName << "�͑��݂��܂���B" << std::endl;
		return true;
	}

	//�ǂݎ��t�@�C�����̍쐬
	std::wostringstream aFile;
	aFile << aMeshCode << _T("_") << rLayerName;

	std::wstring aFileName = aFile.str();

	//�ǂݎ��t�@�C���̃t���p�X�쐬
	std::wostringstream aPath;
	aPath << rDirName << "\\"<<aFileName<<".shp";

	std::wstring aFileFullPath = aPath.str();

	//�t�@�C���̑��݊m�F
	if(! PathFileExists(aFileFullPath.c_str()))
	{
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "�͑��݂��܂���B"<<std::endl;
		return true;
	}

	
	
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_ShapefileWorkspaceFactory );

	//���[�N�X�y�[�X�擾
	IWorkspacePtr ipWorkspace;
	if( FAILED(ipWorkspaceFactory->OpenFromFile( T2BSTR( rDirName.c_str()), 0 , &ipWorkspace ) )){
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "�̃��[�N�X�y�[�X�擾�Ɏ��s���܂����B" << std::endl;
		return false;
	}

	

	//�t�B�[�`���N���X�̎擾
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(T2BSTR(aFileName.c_str()), &ipFeatureClass))){
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName  <<"�t�B�[�`���[�N���X�擾�Ɏ��s���܂���" << std::endl;
		return false;
	}

	long lOBJECTIDIndex;

	if(rLayerName == _T("DCL")){
		if(FAILED( ipFeatureClass->FindField(_T("BLDID"),&lOBJECTIDIndex) ) ){
			std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "��BLDID�C���f�b�N�X�̌����Ɏ��s���܂���"<<std::endl;
			return false;
		}
	}
	else if(( rLayerName == _T("XDA")) || (rLayerName == _T("XDM")) ){
		if(FAILED( ipFeatureClass->FindField(_T("ANNOID"),&lOBJECTIDIndex) ) ){
			std::cerr <<  rDirName << "\\" << aMeshCode << "_" << rLayerName << "��ANNOID�C���f�b�N�X�̌����Ɏ��s���܂���"<<std::endl;
			return false;
		}
	}
	else if( rLayerName == _T("MLA")){
		if(FAILED( ipFeatureClass->FindField(_T("IPC_ID"),&lOBJECTIDIndex) ) ){
			std::cerr << rDirName << "\\" << aMeshCode << "_" << rLayerName << "��IPC_ID�C���f�b�N�X�̌����Ɏ��s���܂���" << std::endl;
			return false;
		}
	}
	else{
		//OBJECTID�̃t�B�[���h��̃C���f�b�N�X�擾
		if(FAILED( ipFeatureClass->FindField(_T("OBJECTID"),&lOBJECTIDIndex) ) ){
			std::cerr <<  rDirName << "\\" << aMeshCode << "_" << rLayerName << "��OID�C���f�b�N�X�̌����Ɏ��s���܂���"<<std::endl;
			return false;
	
		}
	}

	//�t�B�[�`���J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED( ipFeatureClass->Search(NULL, VARIANT_FALSE, & ipFeatureCursor))){
		
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "�J�[�\���̎擾�Ɏ��s���܂���" << std::endl;
		return false;

	}

	IFeaturePtr ipFeature;
	//�t�B�[�`���J�[�\�����s����܂Ńt�B�[�`��������
	while( ipFeatureCursor->NextFeature(&ipFeature)==S_OK && (ipFeature != NULL))
	{
		//FID�擾
		long lOID = ipFeature->GetOID();


		CComVariant ObjectID;						//ObjectI
		if(FAILED(ipFeature->get_Value(lOBJECTIDIndex, &ObjectID))){
			std::cout << rDirName << "\\" << aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:" <<lOID<<"��OBJECTID�̎擾�Ɏ��s" << std::endl;
			continue;
		}

		Key aKey;                       //�L�[�i�I�u�W�F�N�gID,X,Y���W�j�i�[�p�@�\����
	    Info aInfo;						//���b�V���R�[�h�A��ԓ_�ԍ��i�[�p�@�\����

		aInfo.e_MeshCode = aMeshCode;
		//aKey.e_OID = ObjectID.lVal;
		
		
		
		//�`��̎擾
		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy( &ipGeometry ))){
			std::cout << rDirName << "\\" << aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:" <<lOID << "�`��̎擾�Ɏ��s" << std::endl;
			continue;
		}

		//�`��̎�ނ̎擾
		esriGeometryType aShapeType;
		if(FAILED( ipGeometry->get_GeometryType( &aShapeType ))){
			std::cout<< rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:"<< lOID << "�`��̃^�C�v�̎擾�Ɏ��s" << std::endl;
			continue;
		}
		rFeatureType = aShapeType;
		//�|�C���g���擾
		if (aShapeType == esriGeometryPoint){
			aKey.e_OID = 0;
			aInfo.e_OID = ObjectID.lVal;

			CGetPointInfo aGetPointInfo;
			if(! aGetPointInfo.GetPoint( ipGeometry, rInfoMap, & aKey, & aInfo)){
				std::cout << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:"<< lOID << "�|�C���g���擾�Ɏ��s" << std::endl;
			}
		}
		else if (aShapeType == 3){
			aKey.e_OID = 0;
			aInfo.e_OID = ObjectID.lVal;
			
			CGetPolyLineInfo aGetPolyLineInfo;
			if(! aGetPolyLineInfo.GetPolyline( ipGeometry, rInfoMap, & aKey, & aInfo)){
				std::cout << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:"<< lOID << "�|�C���g���擾�Ɏ��s" << std::endl;
			}
		}
		else if (aShapeType == 4){
			aKey.e_OID = ObjectID.lVal;
			aInfo.e_OID = 0;
			CGetPolygonInfo aGetPolygonInfo;
			if(! aGetPolygonInfo.GetPolygon( ipGeometry, rInfoMap, & aKey, & aInfo)){
				std::cout << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:"<< lOID << "�|�C���g���擾�Ɏ��s" << std::endl;
			}
		}
		else{
			std::cout << "���̃c�[����" << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "�t�B�[�`��ID:"<< lOID << "�̃^�C�v�ɑΉ����Ă��܂���" << std::endl;
		}



	}

return true;

}
