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
#include "RWL.h"

bool CRWLDiv::SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index)
{
	// �p�����֐��Ń��R�[�h�����Z�b�g�A����Ȃ����͂��̌��
	if(!CRecodeBase::SetData(strTableName, ipFeature, mapFieldIndex2Name, mapFieldName2Index, mapAttrName2Index))
		return false;

	// FromNodeID��ݒ�
	CComVariant vaFromNodeID;
	if(!GetValue(strTableName, shp05::schema::road::road_walk_link_a::kFromNodeId, ipFeature, mapFieldName2Index.at(shp05::schema::road::road_walk_link_a::kFromNodeId), vaFromNodeID))
		return false;
	eFromNodeID.eID = vaFromNodeID.lVal;

	if(mapFieldName2Index.cend() != mapFieldName2Index.find(shp05::schema::road::road_link_a::kFromDivId)){
		CComVariant vaFromDivID;
		if(!GetValue(strTableName, shp05::schema::road::road_link_a::kFromDivId, ipFeature, mapFieldName2Index.at(shp05::schema::road::road_link_a::kFromDivId), vaFromDivID))
			return false;
		eFromNodeID.eDivID = vaFromDivID.lVal;
	}

	// ToNodeID��ݒ�
	CComVariant vaToNodeID;
	if(!GetValue(strTableName, shp05::schema::road::road_walk_link_a::kToNodeId, ipFeature, mapFieldName2Index.at(shp05::schema::road::road_walk_link_a::kToNodeId), vaToNodeID))
		return false;
	eToNodeID.eID = vaToNodeID.lVal;

	if(mapFieldName2Index.cend() != mapFieldName2Index.find(shp05::schema::road::road_link_a::kToDivId)){
		CComVariant vaToDivID;
		if(!GetValue(strTableName, shp05::schema::road::road_link_a::kToDivId, ipFeature, mapFieldName2Index.at(shp05::schema::road::road_link_a::kToDivId), vaToDivID))
			return false;
		eToNodeID.eDivID = vaToDivID.lVal;
	}

	// �`��f�[�^��FromTo�ݒ�
	eVecLonLat[0].eNodeID.eID = eFromNodeID.eID;
	eVecLonLat[0].eNodeID.eDivID = eFromNodeID.eDivID;
	eVecLonLat[eVecLonLat.size()-1].eNodeID.eID = eToNodeID.eID;
	eVecLonLat[eVecLonLat.size()-1].eNodeID.eDivID = eToNodeID.eDivID;

	return true;
}


bool CRWLAll::LoadData()
{
	bool bReturn = true;

	// �t�B�[�`���J�[�\���擾
	IFeatureCursorPtr ipFeatureCursor;
	if(!GetFeatureCursor(m_strTableName, m_ipFeatureClass, NULL, ipFeatureCursor))
		return false;

#ifdef _DEBUG
	// �t�B�[�`�����擾
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
			std::cout << "   RWL : " << n << " / " << lFeatureCount << "\r" << std::flush;
#endif
		// �������擾
		CRWLDiv cRWLDiv;
		if(!cRWLDiv.SetData(m_strTableName, ipFeature, m_mapFieldIndex2Name, m_mapFieldName2Index, m_mapAttrName2Index)){
			bReturn = false;
			continue;
		}

		// �f�[�^�d���m�F
		std::pair<std::map<UNIQID,CRWLDiv>::const_iterator,bool> itData = m_mapData.insert(std::make_pair(cRWLDiv.eObjectID, cRWLDiv));
		if(!itData.second){
			// �f�[�^�d���G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgRWLDuplicateData, m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
			bReturn = false;
		}
		// �m�[�h�ƃ����N�̃}�b�v
		m_mapNode2Link[cRWLDiv.eFromNodeID].insert(cRWLDiv.eObjectID);
		m_mapNode2Link[cRWLDiv.eToNodeID].insert(cRWLDiv.eObjectID);

		// OBJECTID�ɏ�������DIVID�Q���m��
		m_mapDivOID[cRWLDiv.eObjectID.eID].insert(cRWLDiv.eObjectID.eDivID);

		// ���s�҃����N�ŕ��s�҃����N��ʂ��l�ԋ��p�̏ꍇ
		if(WALK_ID < cRWLDiv.eObjectID.eID && sindy::schema::walk_link::walk_class::kWalkableRoad == cRWLDiv.eVecAttr.at(m_mapAttrName2Index.at(shp05::schema::road::road_walk_link_a::kWalkClass_c)).lVal){
				// �����N�̃Z�O�����g�����m�ۂ��Ă���
				for(long l = 0; l < (long)cRWLDiv.eVecLonLat.size()-1; ++l){
					std::set<LONLAT> setLONLAT;
					setLONLAT.insert(cRWLDiv.eVecLonLat.at(l));
					setLONLAT.insert(cRWLDiv.eVecLonLat.at(l+1));
					m_mapWalkHCSegment.insert(std::make_pair(setLONLAT,cRWLDiv.eObjectID));
				}
		}

		// ���s�҃����N�ŕ��s�҃����N��ʂ��l�ԋ��p�̏ꍇ�A�������́Awalkable_c���l�ԋ��p�̏ꍇ
		if((WALK_ID < cRWLDiv.eObjectID.eID && sindy::schema::walk_link::walk_class::kWalkableRoad == cRWLDiv.eVecAttr.at(m_mapAttrName2Index.at(shp05::schema::road::road_walk_link_a::kWalkClass_c)).lVal) ||
			shp05::schema::road::walkable::kHumanCar == cRWLDiv.eVecAttr.at(m_mapAttrName2Index.at(shp05::schema::road::road_walk_link_a::kWalkable_c)).lVal){
				// �����N�̃Z�O�����g�����m�ۂ��Ă���
				for(long l = 0; l < (long)cRWLDiv.eVecLonLat.size()-1; ++l){
					std::set<LONLAT> setLONLAT;
					setLONLAT.insert(cRWLDiv.eVecLonLat.at(l));
					setLONLAT.insert(cRWLDiv.eVecLonLat.at(l+1));
					m_mapRoadWalkHCSegment.insert(std::make_pair(setLONLAT,cRWLDiv.eObjectID));
				}
		}
	}
#ifdef _DEBUG
	std::cout << "   RWL : " << n << " / " << lFeatureCount << std::endl;
#endif

	return bReturn;
}

bool CRWLAll::ConstractGroupBefore()
{
	// ���̊֐��œ�����O���[�v���̎擾�C���[�W
	//     [objectid=12 [groupid=1 [divid=1]]]
	//     [objectid=12 [groupid=2 [divid=2]]]
	//          :           :          :
	//     [objectid=12 [groupid=8 [divid=8]]]

	// �����O�f�[�^��DIV�P�ʂ��O���[�v�ɂȂ�
	for(const auto divData : m_mapData){
		RWLGROUP cRWLGroup;
		cRWLGroup.eGroupID = 0 == divData.first.eDivID ? 0 : divData.first.eDivID - 1;
		cRWLGroup.eVecDivID.push_back(divData.first.eDivID);
		cRWLGroup.eVecLonLat = divData.second.eVecLonLat;

		m_mapDivGroup[divData.first.eID].push_back(cRWLGroup);
	}
	return true;
}

bool CRWLAll::ConstractGroupAfter(const std::map<long,std::vector<RWLGROUP>>& mapBeforeDivGroup)
{
	// ���̊֐��œ�����O���[�v���̎擾�C���[�W
	//     [objectid=12 [groupid=1 [divid=1]]]
	//     [objectid=12 [groupid=2 [divid=2, divid=3]]]
	//     [objectid=12 [groupid=3 [divid=4]]]
	//     [objectid=12 [groupid=4 [divid=5, divid=6, divid=7]]]
	//          :           :          :
	//     [objectid=12 [groupid=8 [divid=11]]]

	bool bReturn = true;

	// OBJECTID�P�ʂŏ�������
	for(const auto afterDivOID : m_mapDivOID){
		// �����O�O���[�v�����݂��邩
		std::map<long,std::vector<RWLGROUP>>::const_iterator itBeforeGroup = mapBeforeDivGroup.find(afterDivOID.first);
		if(mapBeforeDivGroup.cend() == itBeforeGroup){
			// �����O�f�[�^�������ꍇ��Div�P�ʂŃO���[�v����
			long lGroupID = 0;
			for(const auto afterDiv : afterDivOID.second){
				lGroupID++;
				RWLGROUP cRWLGroup;
				cRWLGroup.eGroupID = lGroupID;
				cRWLGroup.eVecDivID.push_back(afterDiv);
				cRWLGroup.eVecLonLat = m_mapData.at(UNIQID(afterDivOID.first, afterDiv)).eVecLonLat;

				m_mapDivGroup[afterDivOID.first].push_back(cRWLGroup);
			}
		} else {
			// �����O�O���[�v�̌`������ɕ�����̃O���[�v��ݒ肷��
			auto beforeGroup = itBeforeGroup->second.begin();
			long lBeforeGroupID = 0;
			bool bStart = false, bEnd = false;
			RWLGROUP cAfterRWLGroup;
			for(const auto afterDiv : afterDivOID.second){
				std::vector<LONLAT> afterLonLat = m_mapData.at(UNIQID(afterDivOID.first, afterDiv)).eVecLonLat;

				if(beforeGroup->eVecLonLat.at(0) == afterLonLat.at(0)){
					bStart = true;
				}
				if(beforeGroup->eVecLonLat.at(beforeGroup->eVecLonLat.size()-1) == afterLonLat.at(afterLonLat.size()-1)){
					bEnd = true;
				}
				cAfterRWLGroup.eGroupID = beforeGroup->eGroupID;
				cAfterRWLGroup.eVecDivID.push_back(afterDiv);

				if(bStart && bEnd){
					// �O���[�v�`��\�z
					ConstractGroupLonLat(afterDivOID.first, cAfterRWLGroup.eVecDivID, cAfterRWLGroup.eVecLonLat);

					m_mapDivGroup[afterDivOID.first].push_back(cAfterRWLGroup);

					// ���̃O���[�v��
					beforeGroup++;
					if(itBeforeGroup->second.cend() == beforeGroup)
						break;
					
					// ���̍\���͎̂g���܂킷�̂ŏ�����
					cAfterRWLGroup.eGroupID = 0;
					cAfterRWLGroup.eVecDivID.clear();
					cAfterRWLGroup.eVecLonLat.clear();

					bStart = false;
					bEnd = false;
				}
			}
			if(!bStart || !bEnd){
				// �����ł��܂���ł����G���[
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::fatal, err_code::NgRWLGroupLink, m_strTableName, afterDivOID.first, -1);
				bReturn = false;
			}
		}
	}
	return bReturn;
}

bool CRWLAll::ConstractGroupLonLat(long lOID, const std::vector<long>& vecDivID, std::vector<LONLAT>& vecGroupLonLat)
{
	// �`��A��
	long lStartDivID = vecDivID.at(0);
	vecGroupLonLat = m_mapData.at(UNIQID(lOID,lStartDivID)).eVecLonLat;
	for(const auto div : vecDivID){
		if(div == lStartDivID)
			continue;
		std::vector<LONLAT> vecTmpLonLat = m_mapData.at(UNIQID(lOID,div)).eVecLonLat;
		for(long l = 1; l < (long)vecTmpLonLat.size(); ++l){
			vecGroupLonLat.push_back(vecTmpLonLat.at(l));
		}
	}
	return true;
}
