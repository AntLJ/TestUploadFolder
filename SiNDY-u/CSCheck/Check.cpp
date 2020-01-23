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

// Check.cpp: CCheck �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Check.h"
#include <AddrLib/Initializer.h>

namespace {
	LPCTSTR CITY_SITE_NAME = _T("BGCLASS_C");
	LPCTSTR BASE_SITE_NAME = _T("SC4BGCLASS_C");

	LPCTSTR PURPOSE_ID_NAME = _T("PURPOSE_ID");
	LPCTSTR PNAME_NAME = _T("PURPOSE");

	using namespace sindy::schema;
	const LONG gCityInvalidAttr[] ={
		city_site::bg_class::kSands,                    //���n
		city_site::bg_class::kMarsh,                    //���n
		city_site::bg_class::kRiver,                    //�͐�
		city_site::bg_class::kLake,                     //�r�E��
		city_site::bg_class::kPool,                     //�v�[��
		city_site::bg_class::kInterCityTollExpressway,  //�s�s�ԍ������i�L���j
		city_site::bg_class::kUrbanTollExpressway,      //�s�s�����i�L���j
		city_site::bg_class::kTollRoad,                 //�L�����H
		city_site::bg_class::kNationalHighway,          //����
		city_site::bg_class::kPrincipalPrefecturalRoad, //��v�n����
		city_site::bg_class::kPrefectureNormalRoad,     //��ʓs���{����
		city_site::bg_class::kOtherArterialRoad,        //���̑��������H
		city_site::bg_class::kSideway,                  //����
		city_site::bg_class::kGardenPath,               //�뉀�H
		city_site::bg_class::kCenterDivider,            //����������
		city_site::bg_class::kDamGuideNotExist,         //�_���i�֗��L�ڂȂ��j
		city_site::bg_class::kDamGuideExist,            //�_���i�֗��L�ڂ���j
		city_site::bg_class::kRunway,                   //�����H
		city_site::bg_class::kPublicRoadStairs,         //�����K�i
		city_site::bg_class::kInterCityFreeExpressway,  //�s�s�ԍ������i�����j
		city_site::bg_class::kUrbanFreeExpressway,      //�s�s�����i�����j
		city_site::bg_class::kUnderRepairRoad,          //�H�������H
		city_site::bg_class::kTollGateLane,             //���������[��
		-1
	};

	const LONG gBaseInvalidAttr[] ={
		base_site::bg_class::kSea,          //�C
		base_site::bg_class::kRiver,        //�͐�E��E�J
		base_site::bg_class::kLake,         //�΁E�r�E��
		base_site::bg_class::kSands,        //���n
		base_site::bg_class::kMarsh,        //���n
		base_site::bg_class::kSwimmingPool, //�v�[��
		base_site::bg_class::kDam,          //�_��
		-1
	};
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CCheck::CCheck()
{

}

CCheck::~CCheck()
{

}

bool CCheck::init(IFeatureClassPtr ipAdmClass, IFeatureClassPtr ipGouClass, IFeatureClassPtr ipCSClass, IFeatureClassPtr ipCityClass, IFeatureClassPtr ipCityDividerClass, IFeatureClassPtr ipBaseClass, ITablePtr ipRule, LPCTSTR cJdbConnect)
{
	m_FcAdmin = CFeatureClass(ipAdmClass);
	m_FcGou = CFeatureClass(ipGouClass);
	m_FcCsAddr = CFeatureClass(ipCSClass);
	m_ipCity = ipCityClass;
	m_ipBase = ipBaseClass;
	m_ipCityDivider = ipCityDividerClass;
	m_AdmFld = CSPFieldMap( m_FcAdmin.GetFieldMap() );
	m_AdmName = CSPTableNameString( m_FcAdmin.GetNameString() );
	m_GouFld = CSPFieldMap( m_FcGou.GetFieldMap() );
	m_GouName = CSPTableNameString( m_FcGou.GetNameString() );
	m_CspFld = CSPFieldMap( m_FcCsAddr.GetFieldMap() );
	m_CspName = CSPTableNameString( m_FcCsAddr.GetNameString() );
	m_pGous  = new CGouPoints( m_FcAdmin );
	m_pGous->SetGouPointTable((ITable*)m_FcGou);

	if(!fnInitRule(ipRule)){
		return false;
	}
	m_ipCity->FindField(CComBSTR(CITY_SITE_NAME), &m_CitySiteIndex);
	if(m_CitySiteIndex < 0){
		fprintf(stderr, "#Error,�s�s�n�}�w�i�����t�B�[���h��������Ȃ��B\n");
		return false;
	}

	m_ipBase->FindField(CComBSTR(BASE_SITE_NAME), &m_BaseSiteIndex);
	if(m_BaseSiteIndex < 0){
		fprintf(stderr, "#Error,���k�w�i�����t�B�[���h��������Ȃ��B\n");
		return false;
	}

	m_LayerName = m_FcCsAddr.GetOwnerTableName();
	m_GouLayerName = m_FcGou.GetOwnerTableName();
	if(!m_Helper.Connect( cJdbConnect )){
		_ftprintf(stderr, _T("#Error,�Z��DB�̏������Ɏ��s�B\n"));
		return false;
	}
	for(int i = 0; gCityInvalidAttr[i] > 0; i++){
		m_CityAttr.insert(gCityInvalidAttr[i]);
	}
	for(int i = 0; gBaseInvalidAttr[i] > 0; i++){
		m_BaseAttr.insert(gBaseInvalidAttr[i]);
	}
	return true;
}

bool CCheck::execCheck(LPCTSTR cListFile)
{
	bool aIsList;
	//8�����X�g�A�b�v
	if(cListFile == NULL){
		//�S��
		aIsList = false;
		if(!fnCheckPriorGPAll()){
			return false;
		}
		if(!fnSearch8Code()){
			return false;
		}
	}else{
		//�t�@�C������
		aIsList = true;
		if(!fnLoadList(cListFile)){
			return false;	
		}
	}
	//8���P�ʂł̏���
	CODE_SET::const_iterator aCode;
	CString aCityCode(_T("00000"));
	for(aCode = m_CodeSet.begin(); aCode != m_CodeSet.end(); aCode++){
		_tprintf(_T("Now %s\n"), static_cast<LPCTSTR>(*aCode));

		// ���|�C���g�̎擾��5���P�ʂŏ���
		CString aTmpCode(*aCode);
		if(aCityCode != aTmpCode.Left(5)) {
			aCityCode = aTmpCode.Left(5);
			m_GouSet.clear();
			//���|�C���g�̏����擾
			if(!fnGetGouInfo(aCityCode, aIsList)){
				continue;
			}
			//CheckPriorGP����
			if(aIsList){
				//�S�����̎��͈ꊇ�ł�邩��K�v�Ȃ�
				fnCheckPriorGP(m_GouSet);
			}
		}

		//�]���̃`�F�b�N
		fnCheckAt8Code(*aCode, aIsList);
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				_tprintf(_T("�����͒��~����܂����B\n"));
				_ftprintf(stderr, _T("�����͒��~����܂����B\n"));
				break;
			}
		}
	}
	m_GouSet.clear();
	return true;
}

bool CCheck::fnPointCheck(CCSAddrPoint &cCsp)
{
	LONG aObjID	=cCsp.GetOID();
	ERR_INFO aErrInfo;
	aErrInfo.m_Purpose = fnGetPurpose(cCsp);
//	printf("��ƖړI = %s\n", aPurpose);
	aErrInfo.m_AddrCode = cCsp.GetAddrCode();
//	printf("�Z���R�[�h = %s\n", a20Code);

	TCHAR aInfo[256];
	aErrInfo.m_Source = cCsp.GetStringValueByFieldName(schema::ipc_table::kSource, 256, aInfo);
	if(aErrInfo.m_AddrCode.GetLength() != 20){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�Z���R�[�h��20���łȂ�"), &aErrInfo);
	}
	if(aErrInfo.m_AddrCode.Mid(11, 5) == _T("00000")) {
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�n�ԃR�[�h���u00000�v"), &aErrInfo);
	}
	if(fnSameCodeCheck(aObjID, &aErrInfo)){
//		_ftprintf(stderr, _T("0\t%s\t%d\t\t\tError\t0\t����20���R�[�h�̃|�C���g������B\t%s\t%s\t%s\n"), static_cast<LPCTSTR>(m_LayerName), aObjID, static_cast<LPCTSTR>(aPurpose), static_cast<LPCTSTR>(a20Code), static_cast<LPCTSTR>(aSource));
	}
	GOU_INFO_SET::const_iterator aSame = m_GouSet.find(GOU_INFO(aErrInfo.m_AddrCode));
	for(;aSame  != m_GouSet.end() && *aSame == aErrInfo.m_AddrCode; aSame++){
		CString aMsg;
		int aErrLv = (aSame->m_PriFlag && !aSame->m_Oaza) ? 0 : 1;
		aMsg.Format(_T("�yCSP�z����20���R�[�h�̍��|�C���g�i%s�j������%s"), aSame->m_PriFlag ? _T("�D��") : _T("�ʏ�"), aSame->m_Oaza ? _T("�iCODE2�j") : _T(""));
		fnErrOut(m_LayerName, aObjID, m_GouLayerName, aSame->m_ObjID, aErrLv, aMsg, &aErrInfo);
	}
	CString a11Code = aErrInfo.m_AddrCode.Left(11);
	addrCodeStatus aStatus =  m_Helper.GetCodeStatus(a11Code);
	if(aStatus != addr::kShiko && aStatus != addr::kTmpShiko){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�Z���R�[�h������"), &aErrInfo);
	}
	if(aErrInfo.m_Source == _T("")){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("�y�v�C���z�yCSP�z���\�[�X�ɋL�q���Ȃ�"), &aErrInfo);
	}
	bool aIsCityArea = false;

	IGeometryPtr ipGeom = cCsp.GetShapeCopy();
	const CCityAdmin *pAdm =  cCsp.GetCityAdmin(m_FcAdmin);
	if(pAdm != NULL){
		CString aAddrCode = pAdm->GetAddrCode();
		city_admin::area_class::ECode aAreaF = pAdm->GetAreaClass();
#ifdef KAJO
		CString aAddr2 = pAdm->GetOazaAzaCode2();
#else
		city_admin::conv_type::ECode aType= pAdm->GetConvType();
#endif
		long aFugoCode = pAdm->GetGaikuFugo();
		if(aAreaF == city_admin::area_class::kInArea){
			aIsCityArea = true;
		}
		if(aFugoCode > 0){
			CString aComp16 = aAddrCode;
#if 1	//�b��Ή�
			if(aComp16.Left(11) == aErrInfo.m_AddrCode.Left(11)){
				fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�s���E�i�����E�j�ƏZ���R�[�h11������v"), &aErrInfo);
#else
			if(aComp16 == aErrInfo.m_AddrCode.Left(16)){
				fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("16���������B"), &aErrInfo);
#endif
			}else{
				CString aComp8 = aComp16.Left(8);
				if(aComp8 != a11Code.Left(8)){
					if(aComp16.Left(5) != a11Code.Left(5)){
						fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�yCSP�z�s���E�ƏZ���R�[�h5�����s��v"), &aErrInfo);
					}else{
						fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("�yCSP�z�s���E�ƏZ���R�[�h8�����s��v"), &aErrInfo);
					}
					//[bug 8248] �Ή��@�����ł͖{���͒ʂ薼�t���O�Ŕ��f���Ȃ���΂Ȃ�Ȃ����A�R�[�h�őΉ�
					if(aComp8.Left(3) == _T("261")){
						if(m_Helper.GetAddrKanji(aComp8+ _T("000")) == m_Helper.GetAddrKanji(a11Code.Left(8)+ _T("000"))){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�s���E�ƏZ���R�[�h8���͕s��v�����厚���̂���v�i���s�s�j"), &aErrInfo);
						}
					}
					//[bug 8248] �Ή��@�����ł͖{���͒ʂ薼�t���O�Ŕ��f���Ȃ���΂Ȃ�Ȃ����A�R�[�h�őΉ��@�����܂�
				}else{
#ifdef KAJO
					if(aAddr2 != _T("000000")){
						aComp16 = aAddrCode.Left(5) + aAddr2 + aAddrCode.Mid(11,5);
						if(aComp16 == aErrInfo.m_AddrCode.Left(16)){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("�yCSP�z�s���E�iCODE2�j�ƏZ���R�[�h16������v"), &aErrInfo);
						}
					}
#else
					if(aType == city_admin::conv_type::kOazaGeneration){
						aComp16 = aAddrCode.Left(8) + _T("000") + aAddrCode.Mid(11,5);
						if(aComp16 == aErrInfo.m_AddrCode.Left(16)){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("16��������(�厚������)�B"), &aErrInfo);
						}
					}
#endif
				}
			}
		}else{
			CString aComp11 = aAddrCode;
			if(aComp11 == a11Code){
				fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�s���E�ƏZ���R�[�h11������v"), &aErrInfo);
			}else{
				CString aComp8 = aComp11.Left(8);
				if(aComp8 != a11Code.Left(8)){
					if(aComp11.Left(5) != a11Code.Left(5)){
						fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�yCSP�z�s���E�ƏZ���R�[�h5�����s��v"), &aErrInfo);
					}else{
						fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("�yCSP�z�s���E�ƏZ���R�[�h8�����s��v"), &aErrInfo);
					}
					//[bug 8248] �Ή��@�����ł͖{���͒ʂ薼�t���O�Ŕ��f���Ȃ���΂Ȃ�Ȃ����A�R�[�h�őΉ�
					if(aComp8.Left(3) == _T("261")){
#ifndef _DEBUG
						CString aCmpName = m_Helper.GetAddrKanji(aComp8+ _T("000"));
						CString aOrgName = m_Helper.GetAddrKanji(a11Code.Left(8) + _T("000"));
						if(aCmpName == aOrgName){
#else
						if(m_Helper.GetAddrKanji(aComp8+ _T("000")) == m_Helper.GetAddrKanji(a11Code.Left(8)+ _T("000"))){
#endif
							fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�s���E�ƏZ���R�[�h8���͕s��v�����厚���̂���v�i���s�s�j"), &aErrInfo);
						}
					}
					//[bug 8248] �Ή��@�����ł͖{���͒ʂ薼�t���O�Ŕ��f���Ȃ���΂Ȃ�Ȃ����A�R�[�h�őΉ��@�����܂�
				}else{
#ifdef KAJO
					if(aAddr2 != _T("000000")){
						aComp11 = aAddrCode.Left(5) + aAddr2;
						if(aComp11 == a11Code){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("�yCSP�z�s���E�iCODE2�j�ƏZ���R�[�h11������v"), &aErrInfo);
						}
					}
#else
					if(aType == city_admin::conv_type::kOazaGeneration){
						aComp11 = aAddrCode.Left(8) + _T("000");
						if(aComp11 == a11Code){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("11��������(�厚������)�B"), &aErrInfo);
						}
					}
#endif
				}
			}
		}
	}else{
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�y�v�C���z�yCSP�z�s���E�|���S���̎擾�Ɏ��s"), &aErrInfo);
	}
	if(aIsCityArea && fnCitySiteCheck(ipGeom, aObjID)){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�yCSP�z�s���Ȏ�ʂ̓s�s�n�}�w�i�|���S����ɑ���"), &aErrInfo);
	}
	if(aIsCityArea && !fnCityDividerCheck(ipGeom, aObjID)){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�yCSP�z�s�s�����������у|���S����ɑ���"), &aErrInfo);
	}
	if(fnBaseSiteCheck(ipGeom, aObjID)){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("�yCSP�z�s���Ȏ�ʂ̒��k�w�i�|���S����ɑ���"), &aErrInfo);
	}
	return true;
}

CString CCheck::fnGetPurpose(CCSAddrPoint &cCsp)
{
	CString aName;
	long aPID = cCsp.GetLongValueByFieldName(schema::ipc_table::kPurpose, 0);
	CString aWhere;
	if(aPID != 0){
		PURPOSE_MAP::iterator aPurp = m_PurposeMap.find(aPID);
		if(aPurp != m_PurposeMap.end()){
			aName = aPurp->second;
		}else{
			fprintf(stderr, "#Error,�s���ȍ�ƖړI,%d\n", aPID);
		}
	}else{
		TCHAR aBuf[32];
		aName = cCsp.GetStringValueByFieldName(schema::ipc_table::kModifyProgName, 32, aBuf);
	}
	return aName;
}

bool CCheck::fnSameCodeCheck(LONG cObjID, const ERR_INFO* cpErrInfo)
{
	CS_CODE_MAP::const_iterator it = m_CsMap20.find(cpErrInfo->m_AddrCode);
	for(; it != m_CsMap20.end() && it->first == cpErrInfo->m_AddrCode; it++){
		if(it->second != cObjID){
			fnErrOut(m_LayerName, cObjID, m_LayerName, it->second, 0, _T("�y�v�C���z�yCSP�z����20���R�[�h��CS�|�C���g������"), cpErrInfo);
		}
	}
	m_CsMap20.erase(cpErrInfo->m_AddrCode);
	return true;
}

bool CCheck::fnCitySiteCheck(IGeometryPtr ipGeom, LONG cObjID)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipCity->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipCursor;
	if(m_ipCity->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnErrOut(m_LayerName, cObjID, NULL, -1, 0, _T("�yCSP�z�s�s�n�}�w�i�|���S���̎擾�Ɏ��s"), NULL);
		return false;
	}
	bool aIsInvalid = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aAttr;
		ipFeature->get_Value(m_CitySiteIndex, &aAttr);
		if(fnIsInvalidCityAttr(aAttr.lVal)){
			return true;
		}
		aIsInvalid = false;
	}
	return aIsInvalid;
}

bool CCheck::fnCityDividerCheck(IGeometryPtr ipGeom, LONG cObjID)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipCityDivider->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	long count = 0;
	if(m_ipCityDivider->FeatureCount(ipFilter, &count) != S_OK){
		fnErrOut(m_LayerName, cObjID, NULL, -1, 0, _T("�yCSP�z�s�s�����������у|���S���̎擾�Ɏ��s"), NULL);
		return true; // �G���[���e���Ⴄ���߁A���s���Ă��邪true��Ԃ�
	}
	// CSPoint��CITY_DIVIDER��ɂ���ꍇ�̓G���[
	if(count > 0)
		return false;

	return true;
}

bool CCheck::fnBaseSiteCheck(IGeometryPtr ipGeom, LONG cObjID)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipBase->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipCursor;
	if(m_ipBase->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnErrOut(m_LayerName, cObjID, NULL, -1, 0, _T("�yCSP�z���k�w�i�|���S���̎擾�Ɏ��s"), NULL);
		return false;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aAttr;
		ipFeature->get_Value(m_BaseSiteIndex, &aAttr);
		if(fnIsInvalidBaseAttr(aAttr.lVal)){
			return true;
		}
	}
	return false;
}

bool CCheck::fnIsInvalidCityAttr(LONG cAttr)
{
#if 0
	for(int i = 0; gCityInvalidAttr[i] >= 0; i++){
		if(gCityInvalidAttr[i] == cAttr){
			return true;
		}
	}
	return false;
#else
	if(m_CityAttr.find(cAttr) != m_CityAttr.end()){
		return true;
	}
	return false;
#endif
}

bool CCheck::fnIsInvalidBaseAttr(LONG cAttr)
{
#if 0
	for(int i = 0; gBaseInvalidAttr[i] >= 0; i++){
		if(gBaseInvalidAttr[i] == cAttr){
			return true;
		}
	}
	return false;
#else
	if(m_BaseAttr.find(cAttr) != m_BaseAttr.end()){
		return true;
	}
	return false;
#endif
}

bool CCheck::fnLoadList(LPCTSTR cListFile)
{
	FILE *pFile;
	if((pFile = _tfopen(cListFile, _T("rt"))) != NULL){
		TCHAR aBuf[32];
		while(_fgetts(aBuf, 32, pFile) != NULL){
			if(aBuf[0] != '#'){
				CString aCode = aBuf;
				aCode.Replace(_T("\n"), _T(""));
				m_CodeSet.insert(aCode);
			}
		}
		fclose(pFile);
	}else{
		_ftprintf(stderr, _T("#Error,���X�g�t�@�C�����J���܂���B,%s\n"), cListFile);
		return false;
	}
	return true;
}

bool CCheck::fnGetGouInfo(LPCTSTR cCityCode, bool cIsList)
{
	// �w��Z���R�[�h�����s���E������
	m_pGous->SelectByAddrCode( cCityCode );
	// �e�s���E���ɂ��鍆�|�C���g�擾�E�Z���R�[�h�o��
	for( CCityAdmin::const_rows_iterator itAdmin = m_pGous->begin(m_pGous->GetTableType()); itAdmin != m_pGous->end(m_pGous->GetTableType()); ++itAdmin )
	{
		// �s���E���ɂ��鍆�|�C���g�擾
		const CSPCityAdmin pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
#ifdef KAJO
		CString aAddr2 = pAdmin->GetOazaAzaCode2();
#else
		city_admin::conv_type::ECode aType= pAdmin->GetConvType();
#endif
		for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
		{
			const CSPGouPoint pGou = boost::static_pointer_cast<CGouPoint>(*itGou);
			GOU_INFO aInfo;
			aInfo.m_20Code = pGou->GetAddrCode();
			aInfo.m_ObjID = pGou->GetOID();
			aInfo.m_PriFlag = pGou->IsPrior();
			if(aInfo.m_20Code == _T("")){
				if(cIsList && aInfo.m_PriFlag){
					fnErrOut(m_GouLayerName, aInfo.m_ObjID, NULL, -1, 0, _T("�y�v�C���z�yGOU�z���|�C���g�i�D��j��20�����Ɏ��s"), NULL);
				}
				continue;
			}
#ifdef _DEBUG
			if(aInfo.m_20Code == _T("01101002009000030012")){
				int a = 0;
			}
#endif
			aInfo.m_Oaza = false;
			m_GouSet.insert(aInfo);
#ifdef KAJO
			if(aAddr2 != _T("000000")){
				CString aTmpCpde = aInfo.m_20Code;
				aInfo.m_20Code = aTmpCpde.Left(5) + aAddr2 + aTmpCpde.Mid(11, 9);
				aInfo.m_Oaza = true;
				m_GouSet.insert(aInfo);
			}
#else
			if(aType == city_admin::conv_type::kOazaGeneration){
				CString aTmpCpde = aInfo.m_20Code;
				aInfo.m_20Code = aTmpCpde.Left(8) + _T("000") + aTmpCpde.Mid(11, 9);
				aInfo.m_Oaza = true;
				m_GouSet.insert(aInfo);
			}
#endif
		}
	}
	return true;
}

bool CCheck::fnCheckAt8Code(LPCTSTR cAddrCode, bool cNeedLoad)
{
	if(cNeedLoad){
		//���X�g����ǂݍ���
		IQueryFilterPtr ipFilter(CLSID_QueryFilter);
		CString aWhere;
		aWhere.Format(_T("%s LIKE '%s%%'"), cs_addr_point::kAddrCode, cAddrCode);
		ipFilter->put_WhereClause(CComBSTR(aWhere));
		_ICursorPtr ipCursor = m_FcCsAddr.Search(ipFilter, VARIANT_FALSE);
		if(ipCursor == NULL){
			return false;
		}
		_IRowPtr ipRow;
		m_CsMap.clear();
		m_CsMap20.clear();
		while(ipCursor->NextRow(&ipRow) == S_OK){
			CCSAddrPoint aCsp(ipRow, sindyTableType::cs_addr_point, false, m_CspFld, m_CspName);
			CString aCode = aCsp.GetAddrCode();
			m_CsMap.insert(CS_MAP::value_type(aCode.Left(8), aCsp));
			m_CsMap20.insert(CS_CODE_MAP::value_type(aCode, aCsp.GetOID()));
		}
	}
	CS_MAP::iterator aRec = m_CsMap.find(cAddrCode);
	for(; aRec != m_CsMap.end() && aRec->first == cAddrCode; aRec++){
		fnPointCheck(aRec->second);
	}
	return true;
}

bool CCheck::fnSearch8Code(void)
{
	_tprintf(_T("CS�|�C���g�S������8���R�[�h���擾���Ă��܂��B�E�E�E"));
	//CS����S������8���擾
	_ICursorPtr ipCursor = m_FcCsAddr.Search(NULL, VARIANT_FALSE);
	if(ipCursor == NULL){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CCSAddrPoint aCsp(ipRow, sindyTableType::cs_addr_point, false, m_CspFld, m_CspName);
		CString aCode = aCsp.GetAddrCode();
		if(aCode == _T("")){
			fnErrOut(m_LayerName, aCsp.GetOID(), NULL, -1, 0, _T("�y�v�C���z�yCSP�z�Z���R�[�h���ݒ�"), NULL);
			continue;
		}
		m_CodeSet.insert(aCode.Left(8));
		m_CsMap.insert(CS_MAP::value_type(aCode.Left(8), aCsp));
		m_CsMap20.insert(CS_CODE_MAP::value_type(aCode, aCsp.GetOID()));
	}
	_tprintf(_T("�I��\n"));
	return true;
}

bool CCheck::fnCheckPriorGP(GOU_INFO_SET &cGouSet)
{
	CString aCurrentCode = _T("");
//	long aCurrentID = 0;
	GOU_INFO_SET::const_iterator aOrg = cGouSet.begin();
	for(GOU_INFO_SET::const_iterator aCmp = cGouSet.begin(); aCmp != cGouSet.end(); aCmp++){
		if(aCmp->m_PriFlag){
			if(aCurrentCode == aCmp->m_20Code){
				ERR_INFO aErrInfo;
				aErrInfo.m_AddrCode = aCurrentCode;
				if(aOrg->m_Oaza || aCmp->m_Oaza){
#ifdef KAJO
					fnErrOut(m_GouLayerName, aOrg->m_ObjID, m_GouLayerName, aCmp->m_ObjID, 0, _T("�yGOU�z����20���R�[�h�̍��|�C���g�i�D��j�����݁iCODE2�j"), &aErrInfo);
#else
					fnErrOut(m_GouLayerName, aOrg->m_ObjID, m_GouLayerName, aCmp->m_ObjID, 0, _T("����20���R�[�h�̗D�捆�|�C���g������(�厚������)�B"), &aErrInfo);
#endif
				}else{
					fnErrOut(m_GouLayerName, aOrg->m_ObjID, m_GouLayerName, aCmp->m_ObjID, 0, _T("�y�v�C���z�yGOU�z����20���R�[�h�̍��|�C���g�i�D��j������"), &aErrInfo);
				}
			}else{
				aCurrentCode = aCmp->m_20Code;
//				aCurrentID = aCmp->m_ObjID;
				aOrg = aCmp;
			}
		}
	}
	return true;
}

bool CCheck::fnInitRule(ITablePtr ipRule)
{
	long aPNameIndex, aPIDIndex;
	ipRule->FindField(CComBSTR(PNAME_NAME), &aPNameIndex);
	if(aPNameIndex < 0){
		fprintf(stderr, "#Error,���[���̍�ƖړI���̃t�B�[���h��������Ȃ��B\n");
		return false;
	}
	
	ipRule->FindField(CComBSTR(PURPOSE_ID_NAME), &aPIDIndex);
	if(aPIDIndex < 0){
		fprintf(stderr, "#Error,���[���̍�ƖړIID�t�B�[���h��������Ȃ��B\n");
		return false;
	}
	_ICursorPtr ipCursor;
	if(ipRule->Search(NULL, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "#Error,��ƖړI�������s\n");
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CComVariant aPName, aPID;
		ipRow->get_Value(aPNameIndex, &aPName);
		ipRow->get_Value(aPIDIndex, &aPID);
		m_PurposeMap.insert(PURPOSE_MAP::value_type(aPID.lVal, CString(aPName.bstrVal)));
	}
	return true;
}

bool CCheck::fnCheckPriorGPAll(void)
{
	_tprintf(_T("�D��t���O�t���̍����������Ă��܂��B�E�E�E"));
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format(_T("%s = 1"), schema::gou_point::kPriority);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	_ICursorPtr ipCursor = m_FcGou.Search(ipFilter, VARIANT_FALSE);
	if(ipCursor == NULL){
		return false;
	}
	GOU_INFO_SET aPriSet;
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CGouPoint cGou(ipRow,sindyTableType::gou_point,false, m_GouFld, m_GouName );
		const CCityAdmin *pAdm = cGou.GetCityAdmin(m_FcAdmin);
#ifdef KAJO
		CString aAddr2 = pAdm->GetOazaAzaCode2();
#else
		city_admin::conv_type::ECode aType= pAdm->GetConvType();
#endif
		GOU_INFO aInfo;
		aInfo.m_20Code = cGou.GetAddrCode();
		if(aInfo.m_20Code == _T("")){
			fnErrOut(m_GouLayerName, cGou.GetOID(), NULL, -1, 0, _T("�y�v�C���z�yGOU�z���|�C���g�i�D��j��20�����Ɏ��s"), NULL);
			continue;
		}
		aInfo.m_ObjID = cGou.GetOID();
		aInfo.m_PriFlag = true;
		aInfo.m_Oaza = false;
		aPriSet.insert(aInfo);
#ifdef KAJO
		if(aAddr2 != _T("000000")){
			CString aTmpCpde = aInfo.m_20Code;
			aInfo.m_20Code = aTmpCpde.Left(5) + aAddr2 + aTmpCpde.Mid(11, 9);
			aInfo.m_Oaza = true;
			aPriSet.insert(aInfo);
		}
#else
		if(aType == city_admin::conv_type::kOazaGeneration){
			CString aTmpCpde = aInfo.m_20Code;
			aInfo.m_20Code = aTmpCpde.Left(8) + _T("000") + aTmpCpde.Mid(11, 9);
			aInfo.m_Oaza = true;
			aPriSet.insert(aInfo);
		}
#endif
#ifdef _DEBUG
	_tprintf(_T("�T�C�Y=%d\n"), aPriSet.size());
#endif
	}
#ifdef _DEBUG
	_tprintf(_T("�T�C�Y=%d\n"), aPriSet.size());
#endif
	bool aResult = fnCheckPriorGP(aPriSet);
	_tprintf(_T("�I��\n"));
	return aResult;
}

void CCheck::fnErrOut(LPCTSTR cLayer1, long cObjID1, LPCTSTR cLayer2, long cObjID2, int cLevel, LPCTSTR cErrMsg, const ERR_INFO* cpErrInfo)
{
	static LPCTSTR aMsg[] = {_T("ERROR"), _T("WARNING")};
	_ftprintf(stderr, _T("0\t%s\t%d\t"), cLayer1, cObjID1);
	if(cLayer2 != NULL){
		_ftprintf(stderr, _T("%s\t%d\t"), cLayer2, cObjID2);
	}else{
		_ftprintf(stderr, _T("\t\t"));
	}
	_ftprintf(stderr, _T("%s\t%s\t"), aMsg[cLevel], cErrMsg);
	if(cpErrInfo != NULL){
		_ftprintf(stderr, _T("%s\t%s\t%s\t\n"), static_cast<LPCTSTR>(cpErrInfo->m_Purpose), static_cast<LPCTSTR>(cpErrInfo->m_AddrCode), static_cast<LPCTSTR>(cpErrInfo->m_Source));
	}else{
		_ftprintf(stderr, _T("\t\t\t\n"));
	}
}
