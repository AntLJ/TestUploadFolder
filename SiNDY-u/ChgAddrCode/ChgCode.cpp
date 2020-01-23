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

namespace {
const int BUF_MAX =256;

LPCSTR CITY_CODE_NAME = "CityCode";
LPCSTR ADDR_CODE_NAME = "AddrCode";
LPCSTR ADDR_CODE_NAME2 = "AddrCode2";	// [bug 8793] �Ή��B
LPCSTR MODIFY_PROG_NAME = "MODIFYPROGNAME";
LPCSTR MODIFY_DATE_NAME = "PROGMODIFYDATE";
LPCSTR PROG_NAME = "ChgAddrCode";

#ifdef KAJO_CODE
LPCSTR KAJO_CODE_NAME = "KajoCode";
#endif

}
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ChgCode::ChgCode()
{

}

ChgCode::~ChgCode()
{
	m_AdrsMap.clear();
	m_CityList.clear();
}

bool ChgCode::init(IFeatureClassPtr ipFeatureClass, LPCTSTR cFile, LPCTSTR cLayerName)
{
	FILE *pFile;
	char aBuf[BUF_MAX];
	CString aSrcCode, aTgtCode;
	CString aCityCode;
	char *pPointer;

	if((pFile = fopen(cFile, "rt")) != NULL){
		while(fgets(aBuf, BUF_MAX, pFile) != NULL){
			pPointer = strchr(aBuf, '\t');
			if(pPointer != NULL){
				*pPointer = '\0';
				aSrcCode = aBuf;
				pPointer++;
				aTgtCode = CString(pPointer).Left(11);
				aCityCode = aSrcCode.Left(5);
				// [bug 8793]�Ή����ǉ� �ύX�O�R�[�h�̉�6����'000000'�͑ΏۊO�Ƃ���
				if( aSrcCode.Right(6) == "000000" ) {
					fprintf(stderr, "# �z��O�̃��X�g�Ȃ̂ŏ������X�g����폜���܂� :%s\t%s\n", static_cast<LPCSTR>(aSrcCode), static_cast<LPCSTR>(aTgtCode));
					continue;
				}
				m_CityList.insert(aCityCode);
				m_AdrsMap.insert(ADRSMAP::value_type(aSrcCode, aTgtCode));
			}else{
				fprintf(stderr, "�Ή����X�g�̃t�H�[�}�b�g���Ⴄ�B:%s", aBuf);
			}
		}
		fclose(pFile);
	}else{
		fprintf(stderr, "�t�@�C�����J���Ȃ��B:%s\n", cFile);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(CITY_CODE_NAME)), &m_CityCodeIndex) != S_OK){
		fprintf(stderr, "�s�撬���R�[�h�t�B�[���h��������Ȃ��B[%s]\n", CITY_CODE_NAME);
		return false;
	}
	if(m_CityCodeIndex < 0){
		fprintf(stderr, "�s�撬���R�[�h�t�B�[���h��������Ȃ��B[%s]\n", CITY_CODE_NAME);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(ADDR_CODE_NAME)), &m_AddrCodeIndex) != S_OK){
		fprintf(stderr, "�Z���R�[�h�t�B�[���h��������Ȃ��B[%s]\n", ADDR_CODE_NAME);
		return false;
	}
	if(m_AddrCodeIndex < 0){
		fprintf(stderr, "�Z���R�[�h�t�B�[���h��������Ȃ��B[%s]\n", ADDR_CODE_NAME);
		return false;
	}
	// [bug 8793] �Ή�
	if(ipFeatureClass->FindField(CComBSTR(_T(ADDR_CODE_NAME2)), &m_AddrCodeIndex2) != S_OK){
		fprintf(stderr, "�Z���R�[�h�t�B�[���h2��������Ȃ��B[%s]\n", ADDR_CODE_NAME2);
		return false;
	}
	if(m_AddrCodeIndex2 < 0){
		fprintf(stderr, "�Z���R�[�h�t�B�[���h2��������Ȃ��B[%s]\n", ADDR_CODE_NAME2);
		return false;
	}
#ifdef KAJO_CODE
	if(ipFeatureClass->FindField(CComBSTR(_T(KAJO_CODE_NAME)), &m_KajoCodeIndex) != S_OK){
		fprintf(stderr, "�����R�[�h�t�B�[���h��������Ȃ��B[%s]\n", KAJO_CODE_NAME);
		return false;
	}
	if(m_KajoCodeIndex < 0){
		fprintf(stderr, "�����R�[�h�t�B�[���h��������Ȃ��B[%s]\n", KAJO_CODE_NAME);
		return false;
	}
#endif
	if(ipFeatureClass->FindField(CComBSTR(_T(MODIFY_PROG_NAME)), &m_ProgNameIndex) != S_OK){
		fprintf(stderr, "�ŏI�X�V�v���O�������t�B�[���h��������Ȃ��B[%s]\n", MODIFY_PROG_NAME);
		return false;
	}
	if(m_ProgNameIndex < 0){
		fprintf(stderr, "�ŏI�X�V�v���O�������t�B�[���h��������Ȃ��B[%s]\n", MODIFY_PROG_NAME);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(MODIFY_DATE_NAME)), &m_ModifyDateIndex) != S_OK){
		fprintf(stderr, "�ŏI�X�V���t�t�B�[���h��������Ȃ��B[%s]\n", MODIFY_DATE_NAME);
		return false;
	}
	if(m_ModifyDateIndex < 0){
		fprintf(stderr, "�ŏI�X�V���t�t�B�[���h��������Ȃ��B[%s]\n", MODIFY_DATE_NAME);
		return false;
	}
	m_ipAdmin = ipFeatureClass;
	m_LayerName = cLayerName;
	return true;
}

bool ChgCode::execChgAddrCode(bool cChangeMode)
{
	CITYLIST::iterator aCity;

	for(aCity = m_CityList.begin(); aCity != m_CityList.end(); aCity++){
		fnChangeCode(*aCity, cChangeMode);
	}
	return true;
}

bool ChgCode::fnChangeCode(const CString &cCityCode, bool cChangeMode)
{
	printf("Now %s\n", static_cast<LPCSTR>(cCityCode));
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format("%s = '%s'", CITY_CODE_NAME, cCityCode);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	CString aSubFeld;
#ifdef KAJO_CODE
	aSubFeld.Format("OBJECTID,%s,%s,%s,%s", KAJO_CODE_NAME, ADDR_CODE_NAME, MODIFY_PROG_NAME, MODIFY_DATE_NAME);
#else
//	aSubFeld.Format("OBJECTID,%s,%s,%s,%s", CITY_CODE_NAME, ADDR_CODE_NAME, MODIFY_PROG_NAME, MODIFY_DATE_NAME);
	aSubFeld.Format("OBJECTID,%s,%s,%s,%s,%s", CITY_CODE_NAME, ADDR_CODE_NAME, ADDR_CODE_NAME2, MODIFY_PROG_NAME, MODIFY_DATE_NAME);	// [bug 8793] �Ή�
#endif
	ipFilter->put_SubFields(CComBSTR(aSubFeld));
	IFeatureCursorPtr ipCursor;

	if(cChangeMode){
		if(FAILED(m_ipAdmin->Update(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error �������s�B,%s\n", static_cast<LPCSTR>(cCityCode));
			return false;
		}
	}else{
		if(FAILED(m_ipAdmin->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error �������s�B,%s\n", static_cast<LPCSTR>(cCityCode));
			return false;
		}
	}
	//���삳��̃p�N��
	CString cStr = _T("");
	SYSTEMTIME stSystemTime;
	//���삳��̃p�N���@�����܂�

	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		// [bug 8793]�Ή��Œǉ�
		CComVariant aAddrCode, aAddrCode2;
		ipFeature->get_Value(m_AddrCodeIndex, &aAddrCode);
		ipFeature->get_Value(m_AddrCodeIndex2, &aAddrCode2);
		CString aAddr = aAddrCode.bstrVal;
		CString aAddr2 = aAddrCode2.bstrVal;
		CString aAdrsCode = cCityCode + aAddr;
		CString aAdrsCode2 = cCityCode + aAddr2;
		ADRSMAP::iterator aTgt = m_AdrsMap.find(aAdrsCode);
		ADRSMAP::iterator aTgt2 = m_AdrsMap.find(aAdrsCode2);
		if(aTgt != m_AdrsMap.end() || aTgt2 != m_AdrsMap.end()){
			long aObjID;
			ipFeature->get_OID(&aObjID);
			if( aTgt != m_AdrsMap.end() ) {
	//			fprintf(stderr, "%I64d->%I64d\n", aAdrsCode, aTgt->second);
				fprintf(stderr, "0\t%s\t%d\t\t\tInfo\t0\t%s->%s\n", static_cast<LPCSTR>(m_LayerName), aObjID, static_cast<LPCSTR>(aAdrsCode), static_cast<LPCSTR>(aTgt->second));
				if(cChangeMode){
#ifdef KAJO_CODE
					long aTgtAddrCode = long(aTgt->second % 1000000);
					CString aTgtStr;
					aTgtStr.Format("%06d", aTgtAddrCode);
					CComVariant aTgtVal = aTgtStr;
					if(FAILED(ipFeature->put_Value(m_KajoCodeIndex, aTgtVal))){
						fprintf(stderr, "�l�Z�b�g���s�B\n");
						continue;
					}
#else
					CComVariant aCityCode;
					ipFeature->get_Value(m_CityCodeIndex, &aCityCode);
					CString aCity = CString(aCityCode.bstrVal);
					CString aTgtCityCode = aTgt->second.Left(5);
					if(aCity != aTgtCityCode){
						CComVariant aTgtVal = aTgtCityCode;
						ipFeature->put_Value(m_CityCodeIndex, aTgtVal);
					}
					CString aTgtAddrCode = aTgt->second.Right(6);
					if(aAddr != aTgtAddrCode){
						CComVariant aTgtVal = aTgtAddrCode;
						ipFeature->put_Value(m_AddrCodeIndex, aTgtVal);
					}
				}
#endif
			}
			// [bug 8793]�Ή��Œǉ�
			if( aTgt2 != m_AdrsMap.end() ) {
				fprintf(stderr, "0\t%s\t%d\t\t\tInfo2\t0\t%s->%s\n", static_cast<LPCSTR>(m_LayerName), aObjID, static_cast<LPCSTR>(aAdrsCode2), static_cast<LPCSTR>(aTgt2->second));
				if(cChangeMode){
					CComVariant aCityCode;
					ipFeature->get_Value(m_CityCodeIndex, &aCityCode);
					CString aCity = CString(aCityCode.bstrVal);
					CString aTgtCityCode = aTgt2->second.Left(5);
					if(aCity != aTgtCityCode){
						CComVariant aTgtVal = aTgtCityCode;
						ipFeature->put_Value(m_CityCodeIndex, aTgtVal);
					}
					CString aTgtAddrCode = aTgt2->second.Right(6);
					if(aAddr != aTgtAddrCode){
						CComVariant aTgtVal = aTgtAddrCode;
						ipFeature->put_Value(m_AddrCodeIndex2, aTgtVal);
					}
				}
			}

			if(cChangeMode){
				if(FAILED(ipFeature->put_Value(m_ProgNameIndex, CComVariant(PROG_NAME)))){
					fprintf(stderr, "�v���O�������Z�b�g���s�B\n");
					continue;
				}
				//���삳��̃p�N���i������Ƃł����m�Ȏ��Ԃ��Ƃ邽�߂ɂ����Ɉړ��j
				::GetLocalTime( &stSystemTime);
				cStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
						stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
						stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
				//���삳��̃p�N���@�����܂�
				ipFeature->put_Value(m_ModifyDateIndex, CComVariant(cStr));
				if(FAILED(ipCursor->UpdateFeature(ipFeature))){
					fprintf(stderr, "�X�V���s\n");
				}
			}
		}
	}
	ipCursor->Flush();
	return true;
}
