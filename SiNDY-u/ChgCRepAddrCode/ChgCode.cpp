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

// ChgCode.cpp: ChgCode �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChgCode.h"

#define BUF_MAX (256)

#define ADDR_CODE_NAME "AddrCode"
#define MODIFY_PROG_NAME "MODIFYPROGNAME"
#define MODIFY_DATE_NAME "PROGMODIFYDATE"
#define PROG_NAME "ChgCRepAddrCode"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ChgCode::ChgCode()
{

}

ChgCode::~ChgCode()
{

}

bool ChgCode::init(IFeatureClassPtr ipFeatureClass, LPCTSTR cFile, LPCTSTR cLayerName)
{
	FILE *pFile;
	char aBuf[BUF_MAX];
	CString aSrcCode, aTgtCode;
	char *pPointer;

	if((pFile = fopen(cFile, "rt")) != NULL){
		while(fgets(aBuf, BUF_MAX, pFile) != NULL){
			pPointer = strchr(aBuf, '\t');
			if(pPointer != NULL){
				*pPointer = '\0';
				aSrcCode = aBuf;
				pPointer++;
				aTgtCode = CString(pPointer).Left(11);
				if(aSrcCode != aTgtCode){
					m_AdrsMap.insert(ADRSMAP::value_type(aSrcCode, aTgtCode));
				}else{
					fprintf(stderr, "#�Ή��R�[�h���ꏏ�B:%s\n", aSrcCode);
				}
			}else{
				fprintf(stderr, "#�Ή����X�g�̃t�H�[�}�b�g���Ⴄ�B:%s", aBuf);
			}
		}
		fclose(pFile);
	}else{
		fprintf(stderr, "#�t�@�C�����J���Ȃ��B:%s\n", cFile);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(ADDR_CODE_NAME)), &m_AddrCodeIndex) != S_OK){
		fprintf(stderr, "#�Z���R�[�h�t�B�[���h��������Ȃ��B[%s]\n", ADDR_CODE_NAME);
		return false;
	}
	if(m_AddrCodeIndex < 0){
		fprintf(stderr, "#�Z���R�[�h�t�B�[���h��������Ȃ��B[%s]\n", ADDR_CODE_NAME);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(MODIFY_PROG_NAME)), &m_ProgNameIndex) != S_OK){
		fprintf(stderr, "#�ŏI�X�V�v���O�������t�B�[���h��������Ȃ��B[%s]\n", MODIFY_PROG_NAME);
		return false;
	}
	if(m_ProgNameIndex < 0){
		fprintf(stderr, "#�ŏI�X�V�v���O�������t�B�[���h��������Ȃ��B[%s]\n", MODIFY_PROG_NAME);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(MODIFY_DATE_NAME)), &m_ModifyDateIndex) != S_OK){
		fprintf(stderr, "#�ŏI�X�V���t�t�B�[���h��������Ȃ��B[%s]\n", MODIFY_DATE_NAME);
		return false;
	}
	if(m_ModifyDateIndex < 0){
		fprintf(stderr, "#�ŏI�X�V���t�t�B�[���h��������Ȃ��B[%s]\n", MODIFY_DATE_NAME);
		return false;
	}
	m_ipCityRep = ipFeatureClass;
	m_LayerName = cLayerName;
	return true;
}

bool ChgCode::execChgCRepAddrCode(bool cChangeMode)
{
	ADRSMAP::iterator aAddr;

	for(aAddr = m_AdrsMap.begin(); aAddr != m_AdrsMap.end(); aAddr++){
		fnChangeCode(aAddr->first, aAddr->second, cChangeMode);
		if(_kbhit()){
			int aKey = getch();
			if(aKey == 'Q'){
				printf("�����͒��~����܂����B\n");
				fprintf(stderr, "�����͒��~����܂����B\n");
				break;
			}
		}
	}
	return true;
}

bool ChgCode::fnChangeCode(const CString &cSrc, const CString &cTgt, bool cChangeMode)
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format("%s ='%s'", ADDR_CODE_NAME, cSrc);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
//	CString aSubFeld;
//	aSubFeld.Format("OBJECTID,%s,%s,%s", ADDR_CODE_NAME, MODIFY_PROG_NAME, MODIFY_DATE_NAME);
//	ipFilter->put_SubFields(CComBSTR(aSubFeld));
	IFeatureCursorPtr ipCursor;

	if(cChangeMode){
		if(FAILED(m_ipCityRep->Update(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error �������s�B,%s\n", cSrc);
			return false;
		}
	}else{
		if(FAILED(m_ipCityRep->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error �������s�B,%s\n", cSrc);
			return false;
		}
	}
	//���삳��̃p�N��
	CString aStr = _T("");
	SYSTEMTIME stSystemTime;
	//���삳��̃p�N���@�����܂�

	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		long aObjID;
		ipFeature->get_OID(&aObjID);
		printf("%s->%s\n", cSrc, cTgt);
		
		fprintf(stderr, "0\t%s\t%d\t\t\tInfo\t0\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
		if(!cChangeMode){
			continue;
		}
		if(FAILED(ipFeature->put_Value(m_AddrCodeIndex, CComVariant(cTgt)))){
			fprintf(stderr, "0\t%s\t%d\t\t\tERROR\t0\t�R�[�h�ύX���s\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
			continue;
		}
		if(FAILED(ipFeature->put_Value(m_ProgNameIndex, CComVariant(PROG_NAME)))){
			fprintf(stderr, "0\t%s\t%d\t\t\tERROR\t0\t�v���O�������Z�b�g���s\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
			continue;
		}


		//���삳��̃p�N���i������Ƃł����m�Ȏ��Ԃ��Ƃ邽�߂ɂ����Ɉړ��j
		::GetLocalTime( &stSystemTime);
		aStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
				stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
				stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
		//���삳��̃p�N���@�����܂�
		ipFeature->put_Value(m_ModifyDateIndex, CComVariant(aStr));
		if(FAILED(ipCursor->UpdateFeature(ipFeature))){
			fprintf(stderr, "0\t%s\t%d\t\t\tERROR\t0\t�X�V���s\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
		}
	}
	ipCursor->Flush();
	return true;
}
