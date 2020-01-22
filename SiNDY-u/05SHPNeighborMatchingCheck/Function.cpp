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
#include "Function.h"
#include <shlwapi.h>
#include <sindy/workspace.h>
#include <sstream>
#include<boost/lexical_cast.hpp>
#include <atlstr.h>
#include <cstring>


namespace Function{

	std::wstring filestr(const char* str)
	{
		//�K�v�ȕ��������擾
		int buflen = MultiByteToWideChar(CP_ACP,0,str,-1,0,0);
		wchar_t* wbuf = new wchar_t[buflen];
		//UNICODE�ɕϊ�
		MultiByteToWideChar(CP_ACP,0,str,-1,wbuf,buflen);
		std::wstring ret = wbuf;
		delete[] wbuf;
		return ret;
	}

	long GetMeshCode(long aMeshCode,long x, long y, std::string aDataSet)
	{
		if( aDataSet == "top"){
			return ( aMeshCode + x + (y*10) );
		}else if( aDataSet == "middle" ){
			return (aMeshCode + x*200 + y*2);
		}else
		{
			crd_cnv aCrd_cnv;
			return aCrd_cnv.GetMeshCode(aMeshCode,x,y);
		}
	}	

}

namespace ArcFunction{
	
	bool GetFeatureClass(IFeatureClassPtr &ipFeatureClass,std::string aPGDBPath,std::string aDataSet)
	{
		bool aCheck = true;



		//���[�N�X�y�[�X�̎擾
		IWorkspacePtr ipWorkspace(INamePtr(sindy::create_workspace_name(Function::filestr(aPGDBPath.c_str()).c_str()))->_Open());

		//�t�B�[�`�����[�N�X�y�[�X�̎擾
		IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);

		//�t�B�[�`���[�N���X�擾

		if(aDataSet == "top"){
			if(FAILED (ipFeatureWorkspace->OpenFeatureClass(CComBSTR("TopMesh"), &ipFeatureClass)))
				aCheck = false;
		}
		else if(aDataSet == "middle" ){
			if(FAILED (ipFeatureWorkspace->OpenFeatureClass(CComBSTR("MiddleMesh"), &ipFeatureClass)))
				aCheck = false;

		}
		else if(aDataSet == "base" ){
			if(FAILED (ipFeatureWorkspace->OpenFeatureClass(CComBSTR("BaseMesh"), &ipFeatureClass)))
				aCheck = false;

		}
		else if(aDataSet == "city" ){
			if(FAILED (ipFeatureWorkspace->OpenFeatureClass(CComBSTR("CityMesh"), &ipFeatureClass)))
				aCheck = false;

		}

		return aCheck;
	}



	bool GetTKYInfo(const Parameters &rParameters, TKYMeshData *pTKYMeshData, long aMeshCode,std::string aLayerType,std::map<long,FID_Set> *pOIDSet, bool *pCheck)
	{
	
		//�Y�����C��&���b�V���̃��[�N�X�y�[�X�̎擾�`�t�B�[�`���N���X�擾+INDEX�̎擾�܂�
		IFeatureClassPtr ipFeatureClass;
		IDs aIDs;
		if( ! GetSHPFeatureClass( ipFeatureClass, &aIDs, rParameters.e_TKYDataPath, aLayerType, rParameters.e_DataSet,aMeshCode, "TKY",pCheck, rParameters.e_WalkFlag)){
			return false;
		}

		//�t�B���^�̐ݒ�
		//�t�B�[���h�̐ݒ�(DIVID�̖��O���ݒ肳��Ă��鎞��DIVID���擾)
		std::ostringstream aFields;
		if( aIDs.e_DIVID.e_Name.empty() )
			aFields << aIDs.e_ID.e_Name << ",SHAPE";
		else
			aFields << aIDs.e_ID.e_Name << "," << aIDs.e_DIVID.e_Name << ",SHAPE";
		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		ipQueryFilter->PutSubFields((Function::filestr(aFields.str().c_str()).c_str() ));
		//�J�[�\���̎擾
		IFeatureCursorPtr ipCursor = ipFeatureClass->_Search(ipQueryFilter,VARIANT_FALSE);
		//�I�u�W�F�N�g�̎擾
		if( ! GetObjectInfo(pTKYMeshData,ipQueryFilter,ipFeatureClass, aLayerType,aMeshCode,"TKY",pOIDSet,aIDs))
		{
			return false;
		}

		return true;

	}

	bool GetJGDInfo(const Parameters &rParameters, JGDMeshData *pJGDMeshData, long aMeshCode,IFeatureClassPtr &ipMeshClass,std::string aLayerType,std::map<long,FID_Set> &rOIDSet,bool *pCheck)
	{
		
		//�Y�����C��&���b�V���̃��[�N�X�y�[�X�̎擾�`�t�B�[�`���N���X�擾+INDEX�̎擾�܂�
		IFeatureClassPtr ipFeatureClass;
		IDs aIDs;
		if( ! GetSHPFeatureClass( ipFeatureClass, &aIDs, rParameters.e_JGDDataPath, aLayerType, rParameters.e_DataSet,aMeshCode, "JGD",pCheck, rParameters.e_WalkFlag))
			return false;

		//�t�B���^�Ɏg�p����Where���ݒ�
		std::set<std::string> aWhereSet;
		//OID�R���e�i����Y�����b�V���w��
		std::map<long,FID_Set>::iterator OIDSitr =  rOIDSet.find(aMeshCode);
		//OID�R���e�i��������Ȃ�������error
		if( OIDSitr == rOIDSet.end())
		{
			std::cerr << "error �Ȃ񂩂�������" << std::endl;
			return false;
		}
		//�Y���R���e�i�̃C�e���[�^�擾
		std::set<long>::iterator OIDitr = OIDSitr->second.begin();
		if(OIDitr == OIDSitr->second.end())
			return true;

		//500����IN
		for(OIDitr;OIDitr != OIDSitr->second.end();OIDitr++){
			std::ostringstream aWhere;
			aWhere << "FID IN (";
			aWhere << *OIDitr;
			for(int i=0; i<499; i++){
				OIDitr++;
				if(OIDitr == OIDSitr->second.end())
					break;
				aWhere << "," << *OIDitr;
			}
			aWhere << ")";
			aWhereSet.insert(aWhere.str());
			if(OIDitr ==OIDSitr->second.end())
				break;
		}

		//aWhereSet�̗v�f�����t�B���^���쐬���AObject���擾(DIVID�̖��O���ݒ肳��Ă��鎞��DIVID���擾)
		std::ostringstream aFields;
		if( aIDs.e_DIVID.e_Name.empty() )
			aFields << aIDs.e_ID.e_Name << ",SHAPE";
		else
			aFields << aIDs.e_ID.e_Name << "," << aIDs.e_DIVID.e_Name << ",SHAPE";

		std::set<std::string>::iterator Whereitr = aWhereSet.begin();
		for(Whereitr;Whereitr != aWhereSet.end();Whereitr++)
		{
			IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));

			ipQueryFilter->PutSubFields((Function::filestr(aFields.str().c_str()).c_str()));
			ipQueryFilter->put_WhereClause(CComBSTR( Function::filestr(Whereitr->c_str()).c_str()));
			GetObjectInfo(pJGDMeshData,ipQueryFilter,ipFeatureClass,aLayerType,aMeshCode,"JGD",&rOIDSet, aIDs);
		}
		return true;
	}


	bool GetSHPFeatureClass(IFeatureClassPtr &ipFeatureClass,IDs *pIDs, std::string aOrgPath,std::string aLayerType,std::string aDataSet,long aMeshCode,std::string aGeoType, bool *pCheck, bool aWalkFlag)
	{

		//SHP�p�X�̍쐬
		std::string aSHPPath;
		if( ! MakeSHPPath(aOrgPath, aDataSet, aMeshCode, &aSHPPath))
			return false;

		//�ǂݎ��t�@�C���̍쐬
		std::ostringstream aSHPFile;
		if(aDataSet == "top" ){
			CString cstMeshCode;
			cstMeshCode.Format(_T("%02d"),aMeshCode);
			aSHPFile << CT2CA(cstMeshCode) << "_" << aLayerType;
		}else{	
			aSHPFile << aMeshCode << "_" << aLayerType;
		}
		//shp�t�@�C���̑��݊m�F
		//�ǂݎ��t�@�C���̃t���p�X�쐬
		std::ostringstream aFullSHPFile;
		aFullSHPFile << aSHPPath << "\\" << aSHPFile.str() << ".shp";
		//�t�@�C���`�F�b�N
		//�Y�����b�V���������͈͊O�ł���ꍇ�i�Y���͈̗͂אڃ��b�V���������͈͂̒[)SHP�������ꍇ�̓G���[�ł͂Ȃ��̂�acheck��true�ɂ��邱�ƂŃG���[�ƂȂ邱�Ƃ��������B
		//�������A�{���K�v�ł���̂ɑ��݂��Ȃ������Ƃ����ꍇ���l�����邽�߁A�G���[���b�Z�[�W���c���Ă����B
		if( ! PathFileExists(Function::filestr(aFullSHPFile.str().c_str()).c_str())){
			std::cout << aSHPFile.str() << "�����݂��܂���(" << aGeoType << ")\t�Y�����b�V���������͈͊O�ɂ�����ꍇ�͖�肠��܂���"<< std::endl; 
			*pCheck = true;
			return false;
		}

		::CoInitialize(0);
		
		IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_ShapefileWorkspaceFactory );

		//���[�N�X�y�[�X�擾
		IWorkspacePtr ipWorkspace;
		if( FAILED(ipWorkspaceFactory->OpenFromFile( T2BSTR(Function::filestr(aSHPPath.c_str()).c_str()),0, &ipWorkspace))){
			std::cerr << aSHPFile.str() << "�̃��[�N�X�y�[�X�擾�Ɏ��s���܂���(" << aGeoType << ")"<< std::endl;
			return false;
		}

		//�t�B�[�`���N���X�̎擾
		if(FAILED(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass( T2BSTR(Function::filestr(aSHPFile.str().c_str()).c_str()),&ipFeatureClass))){
			std::cerr << aSHPFile.str() << "�̃t�B�[�`���N���X�擾�Ɏ��s���܂���(" << aGeoType << ")" << std::endl;
			return false;
		}

		//OBJECTID�������́A���ꑊ����ID�̃C���f�b�N�X�擾
		if(aLayerType == "DCL"){
			pIDs->e_ID.e_Name = "BLDID";
			if(FAILED( ipFeatureClass->FindField(_T("BLDID"), &pIDs->e_ID.e_Index))){
				std::cerr << aSHPFile.str() << "��BLDID�̃C���f�b�N�X�����Ɏ��s���܂���(" << aGeoType << ")" << std::endl;
				return false;
			}
		}
		else if(aLayerType == "XDA" || aLayerType == "XDM"){
			pIDs->e_ID.e_Name = "ANNOID";
			if(FAILED( ipFeatureClass->FindField(_T("ANNOID"),&pIDs->e_ID.e_Index))){
				std::cerr << aSHPFile.str() << "��ANNOID�̃C���f�b�N�X�����Ɏ��s���܂���(" << aGeoType << ")" << std::endl;
				return false;
			}
		}
		else if(aLayerType == "MLA"){
			pIDs->e_ID.e_Name = "IPC_ID";
			if(FAILED( ipFeatureClass->FindField(_T("IPC_ID"),&pIDs->e_ID.e_Index))){
				std::cerr << aSHPFile.str() << "��IPC_ID�̃C���f�b�N�X�����Ɏ��s���܂���(" << aGeoType << ")" << std::endl;
				return false;
			}
		}
		else{
			pIDs->e_ID.e_Name = "OBJECTID";
			if(FAILED( ipFeatureClass->FindField(_T("OBJECTID"),&pIDs->e_ID.e_Index))){
				std::cerr << aSHPFile.str() << "��OBJECTID�̃C���f�b�N�X�����Ɏ��s���܂���(" << aGeoType << ")" << std::endl;
				return false;
			}
			if(aWalkFlag){
				pIDs->e_DIVID.e_Name = "DIVID";
				if(FAILED( ipFeatureClass->FindField(_T("DIVID"),&pIDs->e_DIVID.e_Index))){
					std::cerr << aSHPFile.str() << "��DIVID�̃C���f�b�N�X�����Ɏ��s���܂���(" << aGeoType << ")" << std::endl;
					return false;
				}
			}

		}

		return true;
	}

	bool MakeSHPPath(std::string aOrgPath,std::string aDataSet,long aMeshCode,std::string *pSHPPath)
	{
		//�f�[�^�Z�b�g�ɂ���ďꍇ�ʂ����A�f�B���N�g���쐬
		std::ostringstream aSHPDir;

		if(aDataSet == "top" ){
			CString cstMeshCode;
			cstMeshCode.Format(_T("%02d"),aMeshCode);
			aSHPDir << aOrgPath << "\\" << CT2CA(cstMeshCode);
			*pSHPPath = aSHPDir.str();
			return true;
		}
		if(aDataSet == "middle"){
			aSHPDir << aOrgPath << "\\" << aMeshCode;
			*pSHPPath = aSHPDir.str();
			return true;
		}
		else if(aDataSet == "base" ){
			long a1stMeshCode = aMeshCode/100;	//1�����b�V���쐬
			aSHPDir << aOrgPath << "\\" << a1stMeshCode << "\\" << aMeshCode;
			*pSHPPath = aSHPDir.str();
			return true;
		}
		else if(aDataSet == "city"){
			long a1stMeshCode = aMeshCode/10000;
			long a2ndMeshCode = aMeshCode/100;
			aSHPDir << aOrgPath << "\\" << a1stMeshCode << "\\" << a2ndMeshCode << "\\" << aMeshCode;
			*pSHPPath = aSHPDir.str();
			return true;
		}
		else {
			std::cerr << "�v���O�������s���ł��B�J���҂ɖ⍇���ĉ�����(aDataSet�֘A)" << std::endl;
			return false;
		}

	}

	bool Insert(std::multimap<long,coord> *pPointInfo, std::multimap<coord,long> *pCoordInfo, long aNum,coord aCoord)
	{
		pPointInfo->insert(std::pair<long,coord>(aNum,aCoord));
		pCoordInfo->insert(std::pair<coord,long>(aCoord,aNum));
		return true;
	};


}
