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
#include "HeightNode.h"

bool CHeightNodeAll::Init(IFeatureClassPtr ipFeatureClass, IFeatureClassPtr ipMeshClass, const CString& strMeshCode, bool bJGD2000)
{
	m_ipFeatureClass = ipFeatureClass;
	m_ipMeshClass = ipMeshClass;
	m_strMeshCode = strMeshCode;
	m_strTableName = sindy::schema::height_node::kTableName;
	m_bJGD2000 = bJGD2000;

	if(!SetFieldInfo(m_strTableName, m_ipFeatureClass))
		return false;

	return true;
}

bool CHeightNodeAll::LoadData()
{
	bool bReturn = true;

	// SDE����擾���邽�߃��b�V���`����擾
	IGeometryPtr ipMeshGeom;
	if(!GetMeshGeom(m_strMeshCode, m_ipMeshClass, ipMeshGeom))
		return false;

	// ��Ԍ����p�̃t�B���^�쐬
	ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
	if(!GetSpatialFilter(ipMeshGeom, ipSpFilter))
		return false;

	// �t�B�[�`���J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if(!GetFeatureCursor(m_strTableName, m_ipFeatureClass, ipSpFilter, ipFeatureCursor))
		return false;

#ifdef _DEBUG
	long lFeatureCount = 0;
	if(!GetFeatureCount(m_strTableName, m_ipFeatureClass, ipSpFilter, lFeatureCount))
		return false;
	long n = 0;
#endif
	IFeaturePtr ipFeature;
	while(S_OK == ipFeatureCursor->NextFeature(&ipFeature) && ipFeature){
#ifdef _DEBUG
		n++;
		if(0 == n % 100)
			std::cout << "   HeightNode : " << n << " / " << lFeatureCount << "\r" << std::flush;
#endif
		// �����m�[�h���擾
		CHeightNode cHeightNode;
		if(!cHeightNode.SetData(m_strTableName, ipFeature, m_mapFieldIndex2Name, m_mapFieldName2Index, m_mapAttrName2Index))
			return false;

		// �K�v�ł����JGD2000��
		if(m_bJGD2000){
			g_cnv.TOKYOtoJGD2000_RP(cHeightNode.eVecLonLat.at(0).eLon, cHeightNode.eVecLonLat.at(0).eLat, &cHeightNode.eVecLonLat[0].eLon, &cHeightNode.eVecLonLat[0].eLat, true);
		}

		// �����m�[�h����OBJECTID��DIVID�Ń��j�[�N��
		std::pair<std::map<UNIQID,CHeightNode>::const_iterator,bool> itInsert = m_mapData.insert(std::make_pair(cHeightNode.eObjectID, cHeightNode));

		if(!itInsert.second){
			// �f�[�^�d���G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgHNDuplicateData, m_strTableName, cHeightNode.eObjectID.eID, cHeightNode.eObjectID.eDivID);
			bReturn = false;
		}

		// �ܓx�o�x��OID�̃}�b�v��ێ�
		std::pair<std::map<LONLAT,UNIQID>::const_iterator,bool> itInsert2 = m_mapLonLat2OID.insert(std::make_pair(cHeightNode.eVecLonLat.at(0), cHeightNode.eObjectID));
		if(!itInsert2.second){
			// �f�[�^�d���G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgHNDuplicateData, m_strTableName, cHeightNode.eObjectID.eID, cHeightNode.eObjectID.eDivID);
			bReturn = false;
		}

		// �g�p�������H�����N���m��
		long lLinkID1Base = (cHeightNode.eVecAttr.at(m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1Table)).lVal == sindy::schema::height_node::link_table::kWalkLink ? WALK_ID : 0 );
		long lLinkID2Base = (cHeightNode.eVecAttr.at(m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2Table)).lVal == sindy::schema::height_node::link_table::kWalkLink ? WALK_ID : 0 );
		m_setLinkID.insert(cHeightNode.eVecAttr.at(m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1)).lVal + lLinkID1Base);
		m_setLinkID.insert(cHeightNode.eVecAttr.at(m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2)).lVal + lLinkID2Base);

	}
#ifdef _DEBUG
	std::cout << "   HeightNode : " << n << " / " << lFeatureCount << std::endl;
#endif
	return bReturn;
}
