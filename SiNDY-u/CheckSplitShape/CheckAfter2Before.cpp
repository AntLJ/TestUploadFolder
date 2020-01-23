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
#include "CheckAfter2Before.h"

bool CCheckAfter2Before::run(const CRWLAll& cBeforeRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll,
							const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, const std::set<UNIQID>& setRebuildLinkId)
{
	bool bReturn  = true;

	// RWL
	if(!CheckRWLAfter2Before(cBeforeRWLAll, cAfterRWLAll, cBeforeRWNAll, cBeforeRSAAll, cBeforeHNodeAll))
		bReturn = false;
	// RWN
	if(!CheckRWNAfter2Before(cBeforeRWNAll, cAfterRWNAll, cBeforeRSAAll, cBeforeHNodeAll))
		bReturn = false;
	// ���΍����f�[�^
	if(check_mode::kHeightNode == m_CheckMode){
		if(!CheckHNAfter2Before(cBeforeHNodeAll, cAfterRelHeightAll, mapRebuildHN, setRebuildLinkId))
			bReturn = false;
	}

	return bReturn;
}

bool CCheckAfter2Before::CheckRWLAfter2Before(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll)
{
	bool bReturn = true;

	// �O���[�v�P�ʂŃ`�F�b�N
	for(const auto aftergrpinfo : cAfterRWLAll.m_mapDivGroup){
		const auto itBeforeGroup = cBeforeRWLAll.m_mapDivGroup.find(aftergrpinfo.first);
		// �ϊ��O�f�[�^�ɑ��݂��Ȃ�
		if(cBeforeRWLAll.m_mapDivGroup.cend() == itBeforeGroup){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistInBefore, cAfterRWLAll.m_strTableName, aftergrpinfo.first, -1);
			bReturn = false;
			continue;
		}

		// �O���[�v�����݂��Ă���΃O���[�v���͈�v���Ă���
		for(long l = 0; l < (long)aftergrpinfo.second.size(); ++l){
			RWLGROUP cBeforeRWLGroup = itBeforeGroup->second.at(l);
			RWLGROUP cAfterRWLGroup = aftergrpinfo.second.at(l);
			CRWLDiv cBeforeRWLDivBase = cBeforeRWLAll.m_mapData.at(UNIQID(itBeforeGroup->first, cBeforeRWLGroup.eVecDivID.at(0)));
			CRWLDiv cAfterRWLDivBase = cAfterRWLAll.m_mapData.at(UNIQID(aftergrpinfo.first, cAfterRWLGroup.eVecDivID.at(0)));
			for(const auto divid : cAfterRWLGroup.eVecDivID){
				CRWLDiv cAfterRWLDiv = cAfterRWLAll.m_mapData.at(UNIQID(aftergrpinfo.first, divid));
				// �ϊ���̑�����񂪈�v���邩
				for(const auto name2index : cAfterRWLAll.m_mapAttrName2Index){
					// �ϊ��O�ɑ����t�B�[���h�����݂��Ȃ��ꍇ
					auto beforeAttrIndex = cBeforeRWLAll.m_mapAttrName2Index.find(name2index.first);
					bool bAttrCheck = true;
					if(cBeforeRWLAll.m_mapAttrName2Index.cend() == beforeAttrIndex){
						// �����N������������RoadAttr�̏ꍇ
						if(check_mode::kSegmentAttr == m_CheckMode){
							// �����N�������t�B�[���h��
							if(0 == name2index.first.CompareNoCase(shp05::schema::road::road_walk_link_a::kRoadAttr_c)){
								// �ϊ���f�[�^�̑������A�Ή����郊���N�������ƃ}�b�`���Ă��邩
								long lBeforeRWLRoadAttr = 0;
								long lAfterRWLRoadAttr = cAfterRWLDiv.eVecAttr.at(name2index.second).lVal;
								if(!IsMatchSegmentAttr(cAfterRWLDiv,lAfterRWLRoadAttr, cBeforeRSAAll, lBeforeRWLRoadAttr)){
									CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameRoadAttrAsSplitData, cAfterRWLAll.m_strTableName, cAfterRWLDiv.eObjectID.eID, cAfterRWLDiv.eObjectID.eDivID, std::to_string(lBeforeRWLRoadAttr).c_str(), std::to_string(lAfterRWLRoadAttr).c_str());
									bReturn = false;
								}
							} else 
								bAttrCheck = false;
						} else
							bAttrCheck = false;
					} else {
						// ������v���邩
						CComVariant vaBeforeAttr = cBeforeRWLDivBase.eVecAttr.at(beforeAttrIndex->second);
						CComVariant vaAfterAttr = cAfterRWLDiv.eVecAttr.at(name2index.second);
						if(vaBeforeAttr != vaAfterAttr){
							// ��v���Ȃ��ꍇ�A���s�҃m�[�h������
							if(check_mode::kWalkNode == m_CheckMode){
								// Walkable�t�B�[���h��
								if(0 == name2index.first.CompareNoCase(shp05::schema::road::road_walk_link_a::kWalkable_c)){
									if(IsMatchMergeLink(cAfterRWLDiv, cBeforeRWLAll)){
										// �ϊ��O��Walkable�͕��s�֎~���ϊ����Walkable�͐l�ԋ��p�̏ꍇ�̂݋��e
										if(shp05::schema::road::walkable::kNotWalk != vaBeforeAttr.lVal || shp05::schema::road::walkable::kHumanCar != vaAfterAttr.lVal){
											CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameWalkableAsSplitData, cAfterRWLAll.m_strTableName, cAfterRWLDiv.eObjectID.eID, cAfterRWLDiv.eObjectID.eDivID);
											bReturn = false;
										}
									} else
										bAttrCheck = false;
								} else
									bAttrCheck = false;
							} else
								bAttrCheck = false;
						}
					}
					if(!bAttrCheck){
						CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameAttrAsBefore, cAfterRWLAll.m_strTableName, cAfterRWLDiv.eObjectID.eID, cAfterRWLDiv.eObjectID.eDivID, name2index.first);
						bReturn = false;
					}

					// �O���[�v��������v�`�F�b�N
					if(!CheckRWLGroupAttr(cAfterRWLDivBase, cAfterRWLDiv, cAfterRWLAll.m_strTableName, name2index.first, name2index.second)){
						bReturn = false;
					}
				}
			}
			// �O���[�v�`�󂪈�v���邩�m�F
			if(!IsMatchGroupGeom(cAfterRWLAll.m_strTableName, aftergrpinfo.first, -1, cBeforeRWLGroup.eVecLonLat, cAfterRWLGroup.eVecLonLat, cBeforeRWNAll, cBeforeRSAAll, cBeforeHNodeAll)){
				bReturn = false;
			}
		}
	}

	return bReturn;
}

bool CCheckAfter2Before::CheckRWNAfter2Before(const CRWNAll& cBeforeRWNAll, const CRWNAll& cAfterRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll)
{
	bool bReturn = true;

	for(const auto afterLonLat2oid : cAfterRWNAll.m_mapLonLat2OID){
		CRWNDiv cBeforeRWNDiv;
		CRWNDiv cAfterRWNDiv = cAfterRWNAll.m_mapData.at(afterLonLat2oid.second);

		long lCount = cBeforeRWNAll.m_mapLonLat2OID.count(afterLonLat2oid.first);
		// �ϊ��O�f�[�^�ɑ��݂��Ȃ��ꍇ
		if(0 == lCount){
			if(!CheckRWNExist(cAfterRWNAll, cBeforeRSAAll, cBeforeHNodeAll, cAfterRWNDiv))
				bReturn = false;
		} else {
			const auto rangeBeforeLonLat = cBeforeRWNAll.m_mapLonLat2OID.equal_range(afterLonLat2oid.first);
			
			bool bMatch = false, bObjectid = false;
			for(auto it = rangeBeforeLonLat.first; it != rangeBeforeLonLat.second; ++it){
				// �ʒu��ID����v���Ă���ꍇ�͑����`�F�b�N
				if(it->second == cAfterRWNDiv.eObjectID){
					// �ϊ��O�f�[�^�ɑ��݂���ꍇ�͑����l�`�F�b�N
					cBeforeRWNDiv = cBeforeRWNAll.m_mapData.at(it->second);
					for(const auto name2index : cAfterRWNAll.m_mapAttrName2Index){
						// �ϊ��O�����ƕs��v
						if(cBeforeRWNDiv.eVecAttr.at(cBeforeRWNAll.m_mapAttrName2Index.at(name2index.first)) != cAfterRWNDiv.eVecAttr.at(name2index.second)){
							CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
							bReturn = false;
						}
					}
					bMatch = true;
				}
				// OBJECTID������v���Ă���P�[�X���`�F�b�N
				if(it->second.eID == cAfterRWNDiv.eObjectID.eID)
					bObjectid = true;
			}
			// OBJECTID����v���Ă��Ȃ��P�[�X�̓m�[�h��v���Ă��Ȃ��ꍇ�Ɠ����`�F�b�N�����{
			if(!bObjectid){
				if(!CheckRWNExist(cAfterRWNAll, cBeforeRSAAll, cBeforeHNodeAll, cAfterRWNDiv))
					bReturn = false;
			}
		}
	}

	return bReturn;
}


bool CCheckAfter2Before::CheckHNAfter2Before(const CHeightNodeAll& cBeforeHNodeAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, const std::set<UNIQID>& setRebuildLinkId)
{
	bool bReturn = true;
	
	// �ϊ���f�[�^���ϊ���č\�z�f�[�^�ɑ��݂��邩
	for(const auto afterRH : cAfterRelHeightAll.m_mapData){
		const auto rebuildLinkId = setRebuildLinkId.find(afterRH.first);
		if(setRebuildLinkId.cend() == rebuildLinkId){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNExistInBefore, cAfterRelHeightAll.m_strTableName, afterRH.first.eID, afterRH.first.eDivID);
			bReturn = false;
		}
	}

	return bReturn;
}

bool CCheckAfter2Before::IsMatchGroupGeom(const CString& strTableName, long lOID, long lDivID, const std::vector<LONLAT>& vecBeforeLonLat, const std::vector<LONLAT>& vecAfterLonLat, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll)
{
	bool bReturn = true;
	long m = 0;
	for(long l = 0; l < (long)vecAfterLonLat.size()-1; ++l){
		LONLAT cAfterLonLat = vecAfterLonLat.at(l);
		LONLAT cBeforeLonLat = vecBeforeLonLat.at(m);

		if(cAfterLonLat != cBeforeLonLat){
			if(0 == cAfterLonLat.eNodeID.eID){
				// �n�_�I�_�ł͂Ȃ��A�r���̍\���_����v���Ȃ��ꍇ�̓G���[
				bReturn = false;
				break;
			}

			if(check_mode::kSegmentAttr == m_CheckMode){
				// �����N�����������̏ꍇ�̓G���[
				bReturn = false;
				break;
			} else if(check_mode::kWalkNode == m_CheckMode){
				// ���s�҃m�[�h�����̏ꍇ�A�����ɕ��s�҃m�[�h�����݂��邩
				std::map<UNIQID,CRWNDiv>::const_iterator itBeforeRWNDiv = cBeforeRWNAll.m_mapData.find(cAfterLonLat.eNodeID);
				// ���݂��Ȃ��ꍇ�̓G���[
				if(cBeforeRWNAll.m_mapData.cend() == itBeforeRWNDiv){
					bReturn = false;
					break;
				}
				// ���݂��Ă��|�C���g�ʒu����v���Ă��Ȃ��ꍇ�̓G���[
				if(itBeforeRWNDiv->second.eVecLonLat.at(0) != cAfterLonLat){
					bReturn = false;
					break;
				}
				// ���݂��Ă�ID����v���Ă��Ȃ��ꍇ�̓G���[
				if(itBeforeRWNDiv->second.eObjectID != cAfterLonLat.eNodeID){
					bReturn = false;
					break;
				}
			} else if(check_mode::kHeightNode == m_CheckMode){
				// �����m�[�h�����̏ꍇ
				std::map<LONLAT,UNIQID>::const_iterator itBeforeHNode = cBeforeHNodeAll.m_mapLonLat2OID.find(cAfterLonLat);
				// ���̂ɍ����m�[�h�����݂��Ȃ��ꍇ�̓G���[
				if(cBeforeHNodeAll.m_mapLonLat2OID.cend() == itBeforeHNode){
					bReturn = false;
					break;
				}
			}
		} else {
			if(cAfterLonLat.eNodeID != cBeforeLonLat.eNodeID){
				if(check_mode::kSegmentAttr == m_CheckMode && 0 == cBeforeLonLat.eNodeID.eID){
					// �����N�����������̏ꍇ
					std::set<LONLAT>::const_iterator itRSA = cBeforeRSAAll.m_setFromToLonLat.find(cAfterLonLat);
					// �����Ƀ����N�������̎n�_�I�_�����݂��Ȃ��ꍇ�̓G���[
					if(cBeforeRSAAll.m_setFromToLonLat.cend() == itRSA){
						bReturn = false;
						break;
					}
				} else if(check_mode::kWalkNode == m_CheckMode){
					// ���s�҃m�[�h�����̏ꍇ
					std::map<UNIQID,CRWNDiv>::const_iterator itBeforeRWNDiv = cBeforeRWNAll.m_mapData.find(cAfterLonLat.eNodeID);
					// �����ɕ��s�҃m�[�h�����݂��Ȃ��ꍇ�̓G���[
					if(cBeforeRWNAll.m_mapData.cend() == itBeforeRWNDiv){
						bReturn = false;
						break;
					}
					// ���݂��Ă�ID����v���Ă��Ȃ��ꍇ�̓G���[
					if(itBeforeRWNDiv->second.eObjectID != cAfterLonLat.eNodeID){
						bReturn = false;
						break;
					}
				} else if(check_mode::kHeightNode == m_CheckMode){
					// �����m�[�h�����̏ꍇ
					std::map<LONLAT,UNIQID>::const_iterator itBeforeHNode = cBeforeHNodeAll.m_mapLonLat2OID.find(cAfterLonLat);
					// ���̂ɍ����m�[�h�����݂��Ȃ��ꍇ�A�����̓��H�A���s�҃m�[�h�����݂��Ȃ��ꍇ�̓G���[
					if(cBeforeHNodeAll.m_mapLonLat2OID.cend() == itBeforeHNode){
						if(cBeforeRWNAll.m_mapLonLat2OID.cend() == cBeforeRWNAll.m_mapLonLat2OID.find(cAfterLonLat)){
							bReturn = false;
							break;
						}
					}
				}
			}
			m++;
		}
	}
	if(!bReturn)
		CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameShapeAsBefore, strTableName, lOID, lDivID);

	return bReturn;
}

bool CCheckAfter2Before::IsMatchSegmentAttr(const CRWLDiv& cAfterRWLDiv, long lAfterRWLRoadAttr, const CRSAAll& cBeforeRSAAll, long& lBeforeRWLRoadAttr)
{
	// �����N�ɑΉ����郊���N�����������N���擾
	std::map<long,std::set<long>>::const_iterator itRSAIDs = cBeforeRSAAll.m_mapLink2Segattr.find(cAfterRWLDiv.eObjectID.eID);
	if(cBeforeRSAAll.m_mapLink2Segattr.cend() == itRSAIDs){
		// �����N���������ݒ肳��Ă��Ȃ��ꍇ�̓X���[
		if(0 == lAfterRWLRoadAttr)
			return true;
		else
			return false;
	}

	for(const auto rsaid : itRSAIDs->second){
		CRSA cRSA = cBeforeRSAAll.m_mapData.at(UNIQID(rsaid, 0));
		// RWL�`��RSA�`��ɓ����Ă��邩�m�F
		//  ��RWL�̍\���_��RSA�̍\���_�ɑ��݂��邩�m�F
		//    �����я��͑��̃`�F�b�N�ōs���Ă���̂ł����ł͌��Ȃ�
		bool bFind = true;
		for(const auto rwllonlat : cAfterRWLDiv.eVecLonLat){
			bool bSearch = false;
			for(const auto rsalonlat : cRSA.eVecLonLat){
				if(rwllonlat == rsalonlat){
					bSearch = true;
					break;
				}
			}
			if(!bSearch){
				bFind = false;
				break;
			}
		}
		// �R�t�������N��������������΃X���[
		if(!bFind)
			continue;

		// �����N�������̑����l���擾
		long lLinkAttr = cRSA.eVecAttr.at(cBeforeRSAAll.m_mapAttrName2Index.at(shp05::schema::road::segment_attr::kLinkAttr_c)).lVal;
		// ���H�E���s�҃����N�X�L�[�}�ɍ��킹�������l��ݒ�
		switch(lLinkAttr){
		case shp05::schema::road::segment_attr::link_attr::kBridge:
			lBeforeRWLRoadAttr += shp05::schema::road::road_walk_link_a::road_attr::kBridge;
			break;
		case shp05::schema::road::segment_attr::link_attr::kTunnel:
			lBeforeRWLRoadAttr += shp05::schema::road::road_walk_link_a::road_attr::kTunnel;
			break;
		case shp05::schema::road::segment_attr::link_attr::kDoumon:
			lBeforeRWLRoadAttr += shp05::schema::road::road_walk_link_a::road_attr::kDoumon;
			break;
		case shp05::schema::road::segment_attr::link_attr::kRailCross:
			lBeforeRWLRoadAttr += shp05::schema::road::road_walk_link_a::road_attr::kRailCross;
			break;
		case shp05::schema::road::segment_attr::link_attr::kUnderPass:
			lBeforeRWLRoadAttr += shp05::schema::road::road_walk_link_a::road_attr::kUnderPass;
			break;
		case shp05::schema::road::segment_attr::link_attr::kDirtRoad:
			lBeforeRWLRoadAttr += shp05::schema::road::road_walk_link_a::road_attr::kDirtRoad;
			break;
		default:
			break;
		}
	}
	// �ϊ��O��ő����l���قȂ�ꍇ��NG
	if(lBeforeRWLRoadAttr != lAfterRWLRoadAttr)
		return false;

	return true;
}

bool CCheckAfter2Before::IsMatchMergeLink(const CRWLDiv& cAfterRWLDiv, const CRWLAll& cBeforeRWLAll)
{
	// �ϊ��ナ���N�̃Z�O�����g���ϊ��O�����N�́u�l�ԋ��p���s�҃����N�̃Z�O�����g���X�g�v���ɑ��݂��邩�`�F�b�N
	//   ���S�Z�O�����g�̊m�F������΃}�[�W�Ώۃ����N�ƈ�v���Ă��邱�Ɠ��`
	for(long l = 0; l < (long)cAfterRWLDiv.eVecLonLat.size()-1; ++l){
		std::set<LONLAT> setLONLAT;
		setLONLAT.insert(cAfterRWLDiv.eVecLonLat.at(l));
		setLONLAT.insert(cAfterRWLDiv.eVecLonLat.at(l+1));
		const auto beforeSegment = cBeforeRWLAll.m_mapWalkHCSegment.find(setLONLAT);
		if(cBeforeRWLAll.m_mapWalkHCSegment.cend() == beforeSegment){
			return false;
		}
	}

	return true;
}

bool CCheckAfter2Before::CheckRWNExist(const CRWNAll& cAfterRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll, const CRWNDiv& cAfterRWNDiv)
{
	bool bReturn = true;

	if(check_mode::kSegmentAttr == m_CheckMode){
		// �����N�����������̏ꍇ
		std::set<LONLAT>::const_iterator itRSA = cBeforeRSAAll.m_setFromToLonLat.find(cAfterRWNDiv.eVecLonLat.at(0));
		// �����Ƀ����N�������̎n�_�I�_�����݂��Ȃ��ꍇ�̓G���[
		if(cBeforeRSAAll.m_setFromToLonLat.cend() == itRSA){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, _T("�����N����������"));
			return false;
		}
		// �����͌Œ�l�ɂȂ��Ă��邩
		for(const auto name2index : cAfterRWNAll.m_mapAttrName2Index){
			CComVariant vaValue = cAfterRWNDiv.eVecAttr.at(name2index.second);
			if(VT_I4 == vaValue.vt){
				if(0 != vaValue.lVal){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
					bReturn =  false;
					continue;
				}
			}
			if(VT_BSTR == vaValue.vt){
				if(0 != CString(vaValue.bstrVal).Compare(_T(" "))){ // ���p�X�y�[�X
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
					bReturn =  false;
					continue;
				}
			}
		}
	} else if(check_mode::kWalkNode == m_CheckMode){
		// ���s�҃m�[�h�����̏ꍇ�͂������������ɗ���̂͂�������
		CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, _T("���s�҃m�[�h����"));
		return false;
	} else if(check_mode::kHeightNode == m_CheckMode){
		// �����m�[�h�����̏ꍇ
		std::map<LONLAT,UNIQID>::const_iterator itBeforeHNode = cBeforeHNodeAll.m_mapLonLat2OID.find(cAfterRWNDiv.eVecLonLat.at(0));
		// �����ɍ����m�[�h�����݂��Ȃ��ꍇ�̓G���[
		if(cBeforeHNodeAll.m_mapLonLat2OID.cend() == itBeforeHNode){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, _T("�����m�[�h����"));
			return false;
		}
		// �����͌Œ�l�ɂȂ��Ă��邩
		for(const auto name2index : cAfterRWNAll.m_mapAttrName2Index){
			CComVariant vaValue = cAfterRWNDiv.eVecAttr.at(name2index.second);
			if(VT_I4 == vaValue.vt){
				if(0 != vaValue.lVal){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
					bReturn =  false;
					continue;
				}
			}
			if(VT_BSTR == vaValue.vt){
				if(0 != CString(vaValue.bstrVal).Compare(_T(" "))){ // ���p�X�y�[�X
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
					bReturn =  false;
					continue;
				}
			}
		}
	}

	return bReturn;
}

bool CCheckAfter2Before::CheckRWLGroupAttr(const CRWLDiv& cAfterRWLDivBase, const CRWLDiv& cAfterRWLDiv, const CString& strAfterRWLTableName, const CString& strFieldName, const long lIndex)
{
	// �����N�����������̏ꍇ�ARoadAttr�t�B�[���h�̓X���[
	if(check_mode::kSegmentAttr == m_CheckMode &&
		0 == strFieldName.CompareNoCase(shp05::schema::road::road_walk_link_a::kRoadAttr_c))
		return true;

	// ���s�҃m�[�h�����E���s�҃����N�}�[�W�̏ꍇ�AWalkable�t�B�[���h�̓X���[
	if(check_mode::kWalkNode == m_CheckMode &&
		0 == strFieldName.CompareNoCase(shp05::schema::road::road_walk_link_a::kWalkable_c))
		return true;

	// �O���[�v���ő�������v���Ă��Ȃ�
	if(cAfterRWLDivBase.eVecAttr.at(lIndex) != cAfterRWLDiv.eVecAttr.at(lIndex)){
		CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameAttrAsSplitData, strAfterRWLTableName, cAfterRWLDiv.eObjectID.eID, cAfterRWLDiv.eObjectID.eDivID, strFieldName);
		return false;
	}

	return true;
}
