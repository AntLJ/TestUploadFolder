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
#include "RSA.h"

bool CRSAAll::LoadData()
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
			std::cout << "   RSA : " << n << " / " << lFeatureCount << "\r" << std::flush;
#endif

		// �����N���������擾
		CRSA cRSA;
		if(!cRSA.SetData(m_strTableName, ipFeature, m_mapFieldIndex2Name, m_mapFieldName2Index, m_mapAttrName2Index))
			return false;

		// �����N����������OBJECTID��DIVID�Ń��j�[�N��
		std::pair<std::map<UNIQID,CRSA>::const_iterator,bool> itInsert = m_mapData.insert(std::make_pair(cRSA.eObjectID, cRSA));
		if(!itInsert.second){
			// �f�[�^�d���G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgRSADuplicateData, m_strTableName, cRSA.eObjectID.eID, cRSA.eObjectID.eDivID);
			bReturn = false;
		}
		// �����N�ɕR�t�������N�������Q�̃}�b�v��ێ�
		m_mapLink2Segattr[cRSA.eVecAttr.at(m_mapAttrName2Index.at(shp05::schema::road::segment_attr::kLinkId)).lVal].insert(cRSA.eObjectID.eID);

		// �����N��������FromTo�m�[�h�ʒu���X�g��ێ�
		m_setFromToLonLat.insert(cRSA.eVecLonLat.at(0));
		m_setFromToLonLat.insert(cRSA.eVecLonLat.at(cRSA.eVecLonLat.size()-1));

	}
#ifdef _DEBUG
	std::cout << "   RSA : " << n << " / " << lFeatureCount << std::endl;
#endif

	return bReturn;
}
