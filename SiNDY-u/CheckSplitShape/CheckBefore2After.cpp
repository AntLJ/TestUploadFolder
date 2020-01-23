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
#include "CheckBefore2After.h"

bool CCheckBefore2After::run(const CRWLAll& cBeforeRWLAll, const CRWNAll& cBeforeRWNAll, const CHeightNodeAll& cBeforeHNodeAll,
							const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll, std::map<LONLAT,std::vector<UNIQID>> mapRebuildHN)
{
	bool bReturn = true;

	// RWL
	if(!CheckRWLBefore2After(cBeforeRWLAll, cAfterRWLAll))
		bReturn = false;
	// RWN
	if(!CheckRWNBefore2After(cBeforeRWNAll, cAfterRWNAll))
		bReturn = false;
	// ���΍����f�[�^
	if(check_mode::kHeightNode == m_CheckMode){
		if(!CheckHNBefore2After(cBeforeHNodeAll, cAfterRelHeightAll, mapRebuildHN))
			bReturn = false;
	}

	return bReturn;
}

bool CCheckBefore2After::CheckRWLBefore2After(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll)
{
	bool bReturn = true;

	for(const auto groupinfo : cBeforeRWLAll.m_mapDivGroup){
		const auto itAfterRWL = cAfterRWLAll.m_mapDivGroup.find(groupinfo.first);

		// �ϊ���f�[�^�ɑ��݂��Ȃ��ꍇ
		if(cAfterRWLAll.m_mapDivGroup.cend() == itAfterRWL){
			if(check_mode::kWalkNode == m_CheckMode){
				// ���s�҃����N�}�[�W�����̏ꍇ�A�}�[�W���ꂽ���m�F����
				for(const auto group : groupinfo.second){
					for(const auto div : group.eVecDivID){
						CRWLDiv cBeforeRWLDiv = cBeforeRWLAll.m_mapData.at(UNIQID(groupinfo.first, div));
						std::map<UNIQID,CRWLDiv> mapAfterRWLDiv;
						// �ϊ��㑤�Ƀ}�[�W�Ώۃ����N��������Ȃ��ꍇ�̓G���[
						if(!GetAfterMergeRWL(cBeforeRWLDiv, cAfterRWLAll, mapAfterRWLDiv)){
							CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLMergeData, cBeforeRWLAll.m_strTableName, groupinfo.first, div);
							bReturn = false;
						} else {
							// �ϊ��O�f�[�^��OID��1���ȉ��A�������͕��s�҃����N��ʂ��u�l�ԋ��p�v�o�Ȃ��ꍇ�̓}�[�W�ΏۊO
							if(WALK_ID > cBeforeRWLDiv.eObjectID.eID || 
								sindy::schema::walk_link::walk_class::kWalkableRoad != cBeforeRWLDiv.eVecAttr.at(cBeforeRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link_a::kWalkClass_c)).lVal){
									CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLMergeData2, cBeforeRWLAll.m_strTableName, groupinfo.first, div);
									bReturn = false;
							}
						}
					}
				}
			} else {
				// ���s�҃����N�}�[�W������ȊO�̓G���[
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistInAfter, cBeforeRWLAll.m_strTableName, groupinfo.first, -1);
				bReturn = false;
				continue;
			}
		}
	}

	return bReturn;
}

bool CCheckBefore2After::CheckRWNBefore2After(const CRWNAll& cBeforeRWNAll, const CRWNAll& cAfterRWNAll)
{
	bool bReturn = true;

	for(auto beforelolat = cBeforeRWNAll.m_mapLonLat2OID.cbegin(); beforelolat != cBeforeRWNAll.m_mapLonLat2OID.cend(); ++beforelolat){
		const auto afterlonlat = cAfterRWNAll.m_mapLonLat2OID.find(beforelolat->first);
		// �ϊ���f�[�^�ɓ���`��̃f�[�^�����݂��Ȃ�
		if(cAfterRWNAll.m_mapLonLat2OID.cend() == afterlonlat){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameShape, cBeforeRWNAll.m_strTableName, beforelolat->second.eID, beforelolat->second.eDivID);
			bReturn = false;
		}
	}

	return bReturn;
}

bool CCheckBefore2After::CheckHNBefore2After(const CHeightNodeAll& cBeforeHNodeAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN)
{
	bool bReturn = true;

	// �č\�z���������m�[�h�f�[�^�Ŕ�r
	for(const auto rebuildRH : mapRebuildHN){
		const auto beforeHNLonLat2OID = cBeforeHNodeAll.m_mapLonLat2OID.at(rebuildRH.first);
		const auto beforeHN = cBeforeHNodeAll.m_mapData.at(beforeHNLonLat2OID);
		bool bCheck = true;
		for(const auto reLink : rebuildRH.second){
			const auto afterRH = cAfterRelHeightAll.m_mapData.find(reLink);
			if(cAfterRelHeightAll.m_mapData.cend() == afterRH){
				// ���������o�͂�����
				CComVariant vaBeforeH1 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1Height));
				CComVariant vaBeforeH2 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2Height));
				CString strOtherMsg;
				strOtherMsg.Format(_T("%ld:%ld"), vaBeforeH1.lVal, vaBeforeH2.lVal);
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNExistInAfter, cBeforeHNodeAll.m_strTableName, beforeHN.eObjectID.eID, beforeHN.eObjectID.eDivID, strOtherMsg);
				bReturn = false;
				bCheck = false;
				break;
			}
		}
		if(!bCheck)
			continue;

		// �ϊ���č\�z�����m�[�h�̑����擾
		// �˂��̃f�[�^�͂���Ȃ�ɐ��������Ă�����̂Ȃ̂ŁA�����ł̓`�F�b�N���Ȃ�
		const auto afterHN1 = cAfterRelHeightAll.m_mapData.at(rebuildRH.second.at(0));
		const auto afterHN3 = cAfterRelHeightAll.m_mapData.at(rebuildRH.second.at(2));

		// �ϊ��O�����m�[�h�̑����擾
		long linkid1 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1)).lVal;
		long tablename1 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1Table)).lVal;
		long relheight1 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID1Height)).lVal;
		long linkid2 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2)).lVal;
		long tablename2 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2Table)).lVal;
		long relheight2 = beforeHN.eVecAttr.at(cBeforeHNodeAll.m_mapAttrName2Index.at(sindy::schema::height_node::kLinkID2Height)).lVal;
		if(sindy::schema::height_node::link_table::kWalkLink == tablename1)
			linkid1 += WALK_ID;
		if(sindy::schema::height_node::link_table::kWalkLink == tablename2)
			linkid2 += WALK_ID;

		if(linkid1 == afterHN1.eObjectID.eID){
			if(relheight1 != afterHN1.eVecAttr.at(cAfterRelHeightAll.m_mapAttrName2Index.at(shp05::schema::road::rel_height::kToLevel)).lVal){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameAttrAsAfter, cBeforeHNodeAll.m_strTableName, linkid1, -1, sindy::schema::height_node::kLinkID1Height, std::to_string(relheight1).c_str());
				bReturn = false;
			}
		} else if(linkid1 == afterHN3.eObjectID.eID){
			if(relheight1 != afterHN3.eVecAttr.at(cAfterRelHeightAll.m_mapAttrName2Index.at(shp05::schema::road::rel_height::kToLevel)).lVal){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameAttrAsAfter, cBeforeHNodeAll.m_strTableName, linkid1, -1, sindy::schema::height_node::kLinkID1Height, std::to_string(relheight1).c_str());
				bReturn = false;
			}
		} else {
			// �ʒu����v���Ă��Ă������NID����v���Ȃ��ꍇ�̓G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameLinkIdAsAfter, cBeforeHNodeAll.m_strTableName, linkid1, -1);
			bReturn = false;
		}

		if(linkid2 == afterHN1.eObjectID.eID){
			if(relheight2 != afterHN1.eVecAttr.at(cAfterRelHeightAll.m_mapAttrName2Index.at(shp05::schema::road::rel_height::kToLevel)).lVal){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameAttrAsAfter, cBeforeHNodeAll.m_strTableName, linkid2, -1, sindy::schema::height_node::kLinkID2Height, std::to_string(relheight2).c_str());
				bReturn = false;
			}
		} else if(linkid2 == afterHN3.eObjectID.eID){
			if(relheight2 != afterHN3.eVecAttr.at(cAfterRelHeightAll.m_mapAttrName2Index.at(shp05::schema::road::rel_height::kToLevel)).lVal){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameAttrAsAfter, cBeforeHNodeAll.m_strTableName, linkid2, -1, sindy::schema::height_node::kLinkID2Height, std::to_string(relheight2).c_str());
				bReturn = false;
			}
		} else {
			// �ʒu����v���Ă��Ă������NID����v���Ȃ��ꍇ�̓G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameLinkIdAsAfter, cBeforeHNodeAll.m_strTableName, linkid2, -1);
			bReturn = false;
		}
	}
	
	return bReturn;
}

bool CCheckBefore2After::GetAfterMergeRWL(const CRWLDiv& cBeforeRWLDiv, const CRWLAll& cAfterRWLAll, std::map<UNIQID,CRWLDiv>& mapAfterMergeRWLDiv)
{
	// �ϊ��O�����N�̃Z�O�����g���A�ϊ��ナ���N�́u�l�ԋ��p�����N�̃Z�O�����g���X�g�v���ɑ��݂��邩�`�F�b�N
	for(long l = 0; l < (long)cBeforeRWLDiv.eVecLonLat.size()-1; ++l){
		std::set<LONLAT> setLONLAT;
		setLONLAT.insert(cBeforeRWLDiv.eVecLonLat.at(l));
		setLONLAT.insert(cBeforeRWLDiv.eVecLonLat.at(l+1));
		const auto afterSegment = cAfterRWLAll.m_mapRoadWalkHCSegment.find(setLONLAT);
		if(cAfterRWLAll.m_mapRoadWalkHCSegment.cend() == afterSegment){
			return false;
		}
		mapAfterMergeRWLDiv.insert(std::make_pair(afterSegment->second,cAfterRWLAll.m_mapData.at(afterSegment->second)));
	}

	return true;
}
