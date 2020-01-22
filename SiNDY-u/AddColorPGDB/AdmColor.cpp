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

// AdmColor.cpp: AdmColor �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdmColor.h"

namespace {
LPCSTR CITY_CODE_NAME = "CITYCODE";
LPCSTR ADDR_CODE_NAME = "ADDRCODE";
LPCSTR COLOR_CODE_NAME = "COLORCODE";
LPCSTR AREACLASS_NAME = "AREACLASS_C";
LPCSTR UPDATE_NAME = "UPDATE_C";

}
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

AdmColor::AdmColor()
{

}

AdmColor::~AdmColor()
{

}

bool AdmColor::init(LPCTSTR cBaseDir, LPCTSTR cAdmLayer, LPCTSTR cTableFile, bool cChgMode)
{
	m_BaseDir = cBaseDir;
	m_AdmLayer = cAdmLayer;
	m_ChgMode = cChgMode;
	return fnLoadColorTable(cTableFile);
}

bool AdmColor::addColor(LONG cMeshCode)
{
	m_MeshCode = cMeshCode;
	IWorkspacePtr ipWorkspace = fnOpenLocalDatabase(cMeshCode);
	if(ipWorkspace == NULL){
		return false;
	}
	IFeatureClassPtr ipAdm = fnOpenLocalFeatureClass(ipWorkspace);
	if(ipAdm == NULL){
		return false;
	}
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	ipFilter->put_WhereClause(CComBSTR("AREACLASS_C = 1"));
	IFeatureCursorPtr ipCursor;
	ipAdm->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(ipCursor == NULL){
		return false;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		fnAddColor(ipFeature);
	}
	return true;
}

bool AdmColor::fnLoadColorTable(LPCTSTR cTableFile)
{
	FILE *pFile;
	bool aResult = true;
	if((pFile = fopen(cTableFile, "rt")) != NULL){
		char aBuf[64];
		while(fgets(aBuf, 64, pFile) != NULL){
			COLOR_INFO aColor;
			char *p = strchr(aBuf, ',');
			if(p == NULL){
				printf("Error,�e�[�u���t�@�C���̃t�H�[�}�b�g���Ⴄ\n");
				fprintf(stderr, "Error,�e�[�u���t�@�C���̃t�H�[�}�b�g���Ⴄ\n");
				aResult = false;
				break;
			}
			*p = '\0';
			aColor.m_CityCode = aBuf;
			p++;
			char *p2 = strchr(p, ',');
			if(p2 == NULL){
				printf("Error,�e�[�u���t�@�C���̃t�H�[�}�b�g���Ⴄ\n");
				fprintf(stderr, "Error,�e�[�u���t�@�C���̃t�H�[�}�b�g���Ⴄ\n");
				aResult = false;
				break;
			}
			*p2 = '\0';
			aColor.m_AddrCode = p;
			p2++;
			aColor.m_Color = atol(p2);
			m_ColorTable.insert(aColor);
		}
		fclose(pFile);
	}else{
		printf("Error,�e�[�u���t�@�C�����J���܂���B,%s\n", cTableFile);
		fprintf(stderr, "Error,�e�[�u���t�@�C�����J���܂���B,%s\n", cTableFile);
		aResult = false;
	}
	return aResult;
}

IWorkspacePtr AdmColor::fnOpenLocalDatabase(LONG cMeshCode)
{
	CString strDatabase;
	strDatabase.Format("%s\\%d\\%d.mdb", m_BaseDir, cMeshCode/10000, cMeshCode);

	// �p�[�\�i���W�IDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;

	// �I�[�v�����Ă݂�
	ipWorkspaceFactory->OpenFromFile( CComBSTR(strDatabase), 0, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		fprintf( stderr, "#Error,%s ���J���܂���B\n", static_cast<LPCSTR>(strDatabase));
	}
	return ipWorkspace;
}

IFeatureClassPtr AdmColor::fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace)
{
	IFeatureClassPtr ipAdm;
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(m_AdmLayer), &ipAdm);

	if(ipAdm->FindField(CComBSTR(_T(CITY_CODE_NAME)), &m_CityIndex) == S_OK){
		if(m_CityIndex < 0){
			fprintf(stderr, "�s���{���R�[�h�t�B�[���h��������܂���B,[%s]\n", CITY_CODE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "�s���{���R�[�h�t�B�[���h��������܂���B,[%s]\n", CITY_CODE_NAME);
		return NULL;
	}
	if(ipAdm->FindField(CComBSTR(_T(ADDR_CODE_NAME)), &m_AddrIndex) == S_OK){
		if(m_AddrIndex < 0){
			fprintf(stderr, "�Z���R�[�h�t�B�[���h��������܂���B,[%s]\n", ADDR_CODE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "�Z���R�[�h�t�B�[���h��������܂���B,[%s]\n", ADDR_CODE_NAME);
		return NULL;
	}
	if(ipAdm->FindField(CComBSTR(_T(COLOR_CODE_NAME)), &m_ColorIndex) == S_OK){
		if(m_ColorIndex < 0){
			fprintf(stderr, "�J���[�R�[�h�t�B�[���h��������܂���B,[%s]\n", COLOR_CODE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "�J���[�R�[�h�t�B�[���h��������܂���B,[%s]\n", COLOR_CODE_NAME);
		return NULL;
	}
	if(ipAdm->FindField(CComBSTR(_T(UPDATE_NAME)), &m_UpdateIndex) == S_OK){
		if(m_UpdateIndex < 0){
			fprintf(stderr, "�X�V�t���O�t�B�[���h��������܂���B,[%s]\n", UPDATE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "�X�V�t���O�t�B�[���h��������܂���B,[%s]\n", UPDATE_NAME);
		return NULL;
	}

	return ipAdm;
}

bool AdmColor::fnAddColor(IFeaturePtr ipFeature)
{
	CComVariant aCityCode, aAddrCode;

	ipFeature->get_Value(m_CityIndex, &aCityCode);
	ipFeature->get_Value(m_AddrIndex, &aAddrCode);
	COLOR_INFO aCmp;
	aCmp.m_CityCode = aCityCode.bstrVal;
	aCmp.m_AddrCode = aAddrCode.bstrVal;
	COLOR_TABLE::iterator aColor = m_ColorTable.find(aCmp);
	if(aColor != m_ColorTable.end()){
		LONG aObjID;
		ipFeature->get_OID(&aObjID);
		if(aColor->m_Color > 0 && aColor->m_Color < 7){
			CComVariant aColorCode;
			ipFeature->get_Value(m_ColorIndex, &aColorCode);
			if(aColorCode.lVal != aColor->m_Color){
				if(m_ChgMode){
					ipFeature->put_Value(m_ColorIndex, CComVariant(aColor->m_Color));
					CComVariant aUpdate;
					ipFeature->get_Value(m_UpdateIndex, &aUpdate);
					aUpdate.lVal |= 2;	//�����ύX
					ipFeature->put_Value(m_UpdateIndex, aUpdate);
					ipFeature->Store();
					fprintf(stderr, "�F�ύX�B,%d,%d,%s,%s,%d->%d\n", m_MeshCode, aObjID, static_cast<LPCSTR>(aCmp.m_CityCode), static_cast<LPCSTR>(aCmp.m_AddrCode), aColorCode.lVal, aColor->m_Color);
				}else{
					fprintf(stderr, "�F�ύX���K�v�B,%d,%d,%s,%s,%d->%d\n", m_MeshCode, aObjID, static_cast<LPCSTR>(aCmp.m_CityCode), static_cast<LPCSTR>(aCmp.m_AddrCode), aColorCode.lVal, aColor->m_Color);
				}
			}else{
				fprintf(stderr, "�ύX�̕K�v���Ȃ��B,%d,%d\n", m_MeshCode, aObjID);
			}
		}else if(aColor->m_Color == 0){
			fprintf(stderr, "�ύX�ΏۊO�B,%d,%d\n", m_MeshCode, aObjID);
		}else{
			fprintf(stderr, "�F������������܂���B,%d,%d\n", m_MeshCode, aObjID);
		}
	}else{
		fprintf(stderr, "Error,�Ή���������Ȃ�,%s,%s\n", static_cast<LPCSTR>(aCmp.m_CityCode), static_cast<LPCSTR>(aCmp.m_AddrCode));
		return false;
	}
	return true;
}
