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

#include "stdafx.h"
#include "RWN.h"

bool CRWNDiv::SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index)
{
	if(!CRecodeBase::SetData(strTableName, ipFeature, mapFieldIndex2Name, mapFieldName2Index, mapAttrName2Index))
		return false;

	return true;
}

bool CRWNAll::LoadData()
{
	bool bReturn = true;

	IFeatureCursorPtr ipFeatureCursor;
	if(!GetFeatureCursor(m_strTableName, m_ipFeatureClass, NULL, ipFeatureCursor))
		return false;

#ifdef _DEBUG
	long lFeatureCount = 0;
	if(!GetFeatureCount(m_strTableName, m_ipFeatureClass, NULL, lFeatureCount))
		return false;
	long n = 0;
#endif
	IFeaturePtr ipFeature;
	while(S_OK == ipFeatureCursor->NextFeature(&ipFeature) && ipFeature){
#ifdef _DEBUG
		n++;
		if(0 == n % 100)
			std::cout << "   RWN : " << n << " / " << lFeatureCount << "\r" << std::flush;
#endif
		// �m�[�h���擾
		CRWNDiv cRWNDiv;
		if(!cRWNDiv.SetData(m_strTableName, ipFeature, m_mapFieldIndex2Name, m_mapFieldName2Index, m_mapAttrName2Index)){
			bReturn = false;
			continue;
		}

		// �m�[�h����OBJECTID��DIVID�Ń��j�[�N��
		std::pair<std::map<UNIQID,CRWNDiv>::const_iterator,bool> itData = m_mapData.insert(std::make_pair(cRWNDiv.eObjectID, cRWNDiv));
		if(!itData.second){
			// �f�[�^�d���G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgRWNDuplicateData, m_strTableName, cRWNDiv.eObjectID.eID, cRWNDiv.eObjectID.eDivID);
			bReturn = false;
		}

		// �ܓx�o�x��OID�̃}�b�v��ێ��i�m�[�h�}�[�W�����O�͏d������P�[�X������̂Ń}���`�}�b�v�ŕێ����Ă���j
		m_mapLonLat2OID.insert(std::make_pair(cRWNDiv.eVecLonLat.at(0), cRWNDiv.eObjectID));

		// OBJECTID�ɏ�������DIVID�Q���m��
		m_mapDivOID[cRWNDiv.eObjectID.eID].insert(cRWNDiv.eObjectID.eDivID);
	}
#ifdef _DEBUG
	std::cout << "   RWN : " << n << " / " << lFeatureCount << std::endl;
#endif

	return bReturn;
}
