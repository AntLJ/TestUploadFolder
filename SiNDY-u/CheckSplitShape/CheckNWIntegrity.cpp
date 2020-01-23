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
#include "CheckNWIntegrity.h"

#define MIN_DIST 0.01 // �����Z�O�����g���i0.01[m] = 1[cm]�j

bool CCheckNWIntegrity::run(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll)
{
	bool bReturn = true;

	// RWL�ɑ΂���`�F�b�N
	if(!CheckNWRWLIntegrity(cAfterRWLAll, cAfterRWNAll))
		bReturn = false;

	// RWN�ɑ΂���`�F�b�N
	if(!CheckNWRWNIntegrity(cAfterRWLAll, cAfterRWNAll))
		bReturn = false;

	// rel_height�ɑ΂���`�F�b�N
	if(check_mode::kHeightNode == m_CheckMode){
		if(!CheckNWRelHeightIntegrity(cAfterRWLAll, cAfterRelHeightAll))
			bReturn = false;
	}

	return bReturn;
}

bool CCheckNWIntegrity::CheckNWRWLIntegrity(const CRWLAll& cRWLAll, const CRWNAll& cRWNAll)
{
	bool bReturn = true;

	std::map<std::set<UNIQID>,UNIQID> setLoopLink;
	std::map<std::set<LONLAT>,UNIQID> setDuplicateSegment;
	std::set<LONLAT> setVertexAll;
	std::map<LONLAT,bool> mapVertexAll;

	// RWL�ɑ΂���`�F�b�N
	for(const auto divoid : cRWLAll.m_mapDivOID){
		CRWLDiv cRWLDivTmp = cRWLAll.m_mapData.at(UNIQID(divoid.first,*divoid.second.begin()));
		CRWLDiv cRWLDivLast = cRWLAll.m_mapData.at(UNIQID(divoid.first,*divoid.second.rbegin()));

		// �ŏ���DIVID���������N��FROMDIVID��0�łȂ�
		if(0 != cRWLDivTmp.eFromNodeID.eDivID){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLFirstDivLinkFromDivIDNot0, cRWLAll.m_strTableName, cRWLDivTmp.eObjectID.eID, cRWLDivTmp.eObjectID.eDivID);
			bReturn = false;
		}
		// �Ō��DIVID���������N��TODIVID��0�łȂ�
		if(0 != cRWLDivLast.eToNodeID.eDivID){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLLastDivLinkToDivIDNot0, cRWLAll.m_strTableName, cRWLDivLast.eObjectID.eID, cRWLDivLast.eObjectID.eDivID);
			bReturn = false;
		}

		long counter = 0; // Div�p�J�E���^
		for(const auto div : divoid.second){
			CRWLDiv cRWLDiv = cRWLAll.m_mapData.at(UNIQID(divoid.first,div));

			const auto rwn_fromnode = cRWNAll.m_mapData.find(cRWLDiv.eFromNodeID);
			if(cRWNAll.m_mapData.cend() == rwn_fromnode){
				// FROMNODE��RWN�ɑ��݂��Ȃ�
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistFromNodeInRWN, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eFromNodeID.eID).c_str(), std::to_string(cRWLDiv.eFromNodeID.eDivID).c_str());
				bReturn = false;
			} else {
				if(cRWLDiv.eVecLonLat.at(0) != rwn_fromnode->second.eVecLonLat.at(0)){
					// FROMNODEID��RWN�Ǝn�_�̌`�󂪕s��v
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLFromNodeAndRWNGeomSame, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eFromNodeID.eID).c_str(), std::to_string(cRWLDiv.eFromNodeID.eDivID).c_str());
					bReturn = false;
				}
			}

			const auto rwn_tonode = cRWNAll.m_mapData.find(cRWLDiv.eToNodeID);
			if(cRWNAll.m_mapData.cend() == rwn_tonode){
				// TONODE��RWN�ɑ��݂��Ȃ�
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistToNodeInRWN, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eToNodeID.eID).c_str(), std::to_string(cRWLDiv.eToNodeID.eDivID).c_str());
				bReturn = false;
			} else {
				if(cRWLDiv.eVecLonLat.at(cRWLDiv.eVecLonLat.size()-1) != rwn_tonode->second.eVecLonLat.at(0)){
					// TONODEID��RWN�Ǝn�_�̌`�󂪕s��v
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLToNodeAndRWNGeomSame, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eToNodeID.eID).c_str(), std::to_string(cRWLDiv.eToNodeID.eDivID).c_str());
					bReturn = false;
				}
			}

			bool bDuplicateSegment = true, bSmallSegment = true;
			long lCount = cRWLDiv.eVecLonLat.size() - 1;
			for(long l = 0; l < lCount; ++l){
				double dDist = 0.0;
				if(m_bJGD2000)
					dDist = g_cnv.GetDist_JGD2000(cRWLDiv.eVecLonLat.at(l).eLon, cRWLDiv.eVecLonLat.at(l).eLat, cRWLDiv.eVecLonLat.at(l+1).eLon, cRWLDiv.eVecLonLat.at(l+1).eLat);
				else
					dDist = g_cnv.GetDist(cRWLDiv.eVecLonLat.at(l).eLon, cRWLDiv.eVecLonLat.at(l).eLat, cRWLDiv.eVecLonLat.at(l+1).eLon, cRWLDiv.eVecLonLat.at(l+1).eLat);
				// �����Z�O�����g�`�F�b�N
				if(dDist < m_dMinSegment)
					bSmallSegment = false;

				// �S�f�[�^���̃Z�O�����g�d���`�F�b�N
				std::set<LONLAT> setSegment;
				setSegment.insert(cRWLDiv.eVecLonLat.at(l));
				setSegment.insert(cRWLDiv.eVecLonLat.at(l+1));
				std::pair<std::map<std::set<LONLAT>,UNIQID>::const_iterator,bool> itSegment = setDuplicateSegment.insert(std::make_pair(setSegment, cRWLDiv.eObjectID));
				if(!itSegment.second){
					// �����N������������
					if(check_mode::kSegmentAttr == m_CheckMode){
						// ���s�҃����N��ʂ��l�ԋ��p�����N�łȂ��ꍇ�̓G���[
						if(sindy::schema::walk_link::walk_class::kWalkableRoad != cRWLAll.m_mapData.at(itSegment.first->second).eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal &&
							sindy::schema::walk_link::walk_class::kWalkableRoad != cRWLDiv.eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal){
								bDuplicateSegment = false;
						}
					} else // ���̑��͋����I�ɃG���[
						bDuplicateSegment = false;
				}
			}
			// �Z�O�����g�d���G���[��1�����N1���OK
			if(!bDuplicateSegment){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLDuplicateSegment, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
				bReturn = false;
			}
			// �����Z�O�����g�G���[��1�����N1��
			if(!bSmallSegment){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSmallSegment, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, CString(std::to_string(m_dMinSegment).c_str())+_T("[m]�ȉ�"));
				bReturn = false;
			}

			// �\���_�d���`�F�b�N
			long lLonLatSize = cRWLDiv.eVecLonLat.size();
			std::set<LONLAT> setCheckVertex;
			// �����N�������������͕��s�҃����N��ʂ��l�ԋ��p�̏ꍇ�͏����X���[�����邽�߂̃t���O�ݒ�
			bool bCheckSegHumanCar = false;
			if(check_mode::kSegmentAttr == m_CheckMode){
				if(sindy::schema::walk_link::walk_class::kWalkableRoad == cRWLDiv.eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal)
					bCheckSegHumanCar = true;
			}
			bool bVertex = true, bVertexAll = true;
			for(long l = 0; l < lLonLatSize; ++l){
				LONLAT cVertex = cRWLDiv.eVecLonLat.at(l);

				std::pair<std::set<LONLAT>::const_iterator,bool> itVertex = setCheckVertex.insert(cVertex);
				// �\���_�d���`�F�b�N
				if(!itVertex.second)
					bVertex = false;

				// �����N�������������Ől�ԋ��p�����N�́A�S�f�[�^�\���_�`�F�b�N�̓X���[
				if(!bCheckSegHumanCar){
					// �S�f�[�^�ł̍\���_�d���`�F�b�N�i�`�F�b�N���̂�FromTo�܂߂��S�\���_�ōs���j
					const auto vertexAll = mapVertexAll.find(cVertex);
					if(mapVertexAll.cend() != vertexAll){
						bool bCheck = true;
						if(0 == l || lLonLatSize-1 == l){
							// �[�_���m�̏ꍇ�̓G���[�Ƃ��Ȃ�
							if(vertexAll->second)
								bCheck = false;
						} else
							bCheck = false;

						// �����N���������������ŁA���̍\���_��RWN�����݂��A�R�t�������N�ɐl�ԋ��p�����N������ꍇ�̓`�F�b�N�X���[�ł���
						if(!bCheck){
							if(check_mode::kSegmentAttr == m_CheckMode){
								bool bIgnor = false;
								const auto vertexRWN = cRWNAll.m_mapLonLat2OID.find(cVertex);
								if(cRWNAll.m_mapLonLat2OID.cend() != vertexRWN){
									const auto vertexRWL = cRWLAll.m_mapNode2Link.find(vertexRWN->second);
									if(cRWLAll.m_mapNode2Link.cend() != vertexRWL){
										for(const auto rwloid : vertexRWL->second){
											const auto rwldata = cRWLAll.m_mapData.find(rwloid);
											if(cRWLAll.m_mapData.cend() != rwldata){
												if(sindy::schema::walk_link::walk_class::kWalkableRoad == rwldata->second.eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal){
													bIgnor =  true;
													break;
												}
											}
										}
									}
								}
								if(!bIgnor)
									bVertexAll = false;
							} else
								bVertexAll = false;
						}
					}
					if(0 == l || lLonLatSize-1 == l){
						// �[�_�̏ꍇ�̓t���OOFF�ŐV�K�ǉ��A�����̃f�[�^������ꍇ�͊����̃t���O���g�p����
						mapVertexAll.insert(std::make_pair(cVertex,false));
					} else {
						// �[�_�łȂ��ꍇ�̓t���O��ON�ɂ���
						mapVertexAll[cVertex] = true;
					}
				}
			}
			if(!bVertex){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLDuplicateVertex, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
				bReturn = false;
			}
			if(!bVertexAll){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLDuplicateVertexAllData, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
				bReturn = false;
			}

			// ���[�v�����N���݃`�F�b�N
			std::set<UNIQID> setLinkNode;
			setLinkNode.insert(cRWLDiv.eFromNodeID);
			setLinkNode.insert(cRWLDiv.eToNodeID);
			std::pair<std::map<std::set<UNIQID>,UNIQID>::const_iterator,bool> itLoopLink = setLoopLink.insert(std::make_pair(setLinkNode, cRWLDiv.eObjectID));
			if(!itLoopLink.second){
				bool bLoop = false;
				// �����N������������
				if(check_mode::kSegmentAttr == m_CheckMode){
					// ���s�҃����N��ʂ��l�ԋ��p�����N�̏ꍇ�̓G���[�ł͂Ȃ�
					if(sindy::schema::walk_link::walk_class::kWalkableRoad == cRWLAll.m_mapData.at(itLoopLink.first->second).eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal ||
						sindy::schema::walk_link::walk_class::kWalkableRoad == cRWLDiv.eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal){
							bLoop = true;
					}
				}
				if(!bLoop){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::warning, err_code::NgRWLLoopLink, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
					bReturn = false;
				}
			}

			// ����OBJECTID�Q�̑������s��v
			for(long l = 0; l < (long)cRWLDiv.eVecAttr.size(); ++l){
				// �����N�������A���s�\�R�[�h�̓X���[
				if(0 == cRWLAll.m_mapAttrIndex2Name.at(l).CompareNoCase(shp05::schema::road::road_walk_link_a::kRoadAttr_c) ||
					0 == cRWLAll.m_mapAttrIndex2Name.at(l).CompareNoCase(shp05::schema::road::road_walk_link_a::kWalkable_c)) 
					continue;

				// �ЂƂO��DIVID�Ƒ�����r
				if(cRWLDivTmp.eVecAttr.at(l) != cRWLDiv.eVecAttr.at(l)){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameAttribute, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, cRWLAll.m_mapAttrIndex2Name.at(l));
					bReturn = false;
				}
			}
			// ����OBJECTID�Q��DIVID���s�A��
			//  ��DivID���u0�v�̃f�[�^�͕�������Ă��Ȃ����߉��L�����̓X���[
			if(0 < counter){
				// DivID���A�ԂɂȂ��Ă��邩
				// DivID�ɔ�����������
				// �ЂƂO��DIVID�̏I�_�ƌ���DIVID�̎n�_����v���Ă��邩
				if(cRWLDivTmp.eObjectID.eDivID != counter ||
					cRWLDivTmp.eObjectID.eDivID != cRWLDiv.eObjectID.eDivID - 1 ||
					cRWLDivTmp.eVecLonLat.at(cRWLDivTmp.eVecLonLat.size()-1) != cRWLDiv.eVecLonLat.at(0)){
						CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSequenceDivID, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
						bReturn = false;
				}
			}
			// DivID�J�E���^�C���N�������g
			counter++;

			// �ЂƂO��RWLDiv�Ƃ��邽�ߍX�V
			cRWLDivTmp = cRWLDiv;
		}
	}
	return bReturn;
}

bool CCheckNWIntegrity::CheckNWRWNIntegrity(const CRWLAll& cRWLAll, const CRWNAll& cRWNAll)
{
	bool bReturn = true;

	// RWN�ɑ΂���`�F�b�N
	for(const auto lonlat2oid : cRWNAll.m_mapLonLat2OID){
		if(IsRWNDuplicateGeom(lonlat2oid.first, cRWNAll)){
			// �`��d���G���[
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNDuplicateGeom, cRWNAll.m_strTableName, lonlat2oid.second.eID, lonlat2oid.second.eDivID);
			bReturn = false;
		}
	}
	for(const auto divoid : cRWNAll.m_mapDivOID){
		std::set<long> setRWLDiv;
		if(cRWLAll.m_mapDivOID.cend() != cRWLAll.m_mapDivOID.find(divoid.first))
			setRWLDiv = cRWLAll.m_mapDivOID.at(divoid.first);
		long lSequence = 0;
		long lCounter = 0;
		for(const auto div : divoid.second){
			// RWL��FROMTONODE�ɑ��݂��Ȃ�
			std::map<UNIQID,std::set<UNIQID>>::const_iterator itNode2Link = cRWLAll.m_mapNode2Link.find(UNIQID(divoid.first, div));
			if(cRWLAll.m_mapNode2Link.cend() == itNode2Link){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInRWL, cRWNAll.m_strTableName, divoid.first, div);
				bReturn = false;
			}

			if(0 == div)
				continue;

			lCounter++;
			CRWNDiv cRWNDiv= cRWNAll.m_mapData.at(UNIQID(divoid.first, div));

			// DIVID�����m�[�hOBJECTID���ڑ������N�̍ŏ�OBJECTID�ɂȂ��Ă��Ȃ�
			if(cRWLAll.m_mapNode2Link.cend() != itNode2Link){
				if(divoid.first != itNode2Link->second.begin()->eID){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNDivOIDisMinimumRWLOID, cRWNAll.m_strTableName, divoid.first, div);
					bReturn = false;
				}
			}

			// DIVID��������OBJECTID�̃m�[�h�������N�̌�����DIVID���A�����Ă��Ȃ�
			bool bMatch = false;
			long lPos = 0;
			for(const auto rwldivid : setRWLDiv){
				lPos++;
				if(cRWNDiv.eVecLonLat.at(0) == cRWLAll.m_mapData.at(UNIQID(divoid.first, rwldivid)).eVecLonLat.at(0)){
					if(lSequence > rwldivid){
						CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSequenceDivID, cRWNAll.m_strTableName, divoid.first, div);
						bReturn = false;
					}
					lSequence = lPos;
					bMatch = true;
					break;
				}
			}
			if(!bMatch){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSequenceDivID, cRWNAll.m_strTableName, divoid.first, div, _T("�`��s��v"));
				bReturn = false;
			}
			if(lCounter != div){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSequenceDivID, cRWNAll.m_strTableName, divoid.first, div, _T("DIV�s��"));
				bReturn = false;
			}
		}
	}

	return bReturn;
}

bool CCheckNWIntegrity::CheckNWRelHeightIntegrity(const CRWLAll& cAfterRWLAll,  const CRelHeightAll& cAfterRelHeightAll)
{
	bool bReturn = true;

	for(const auto rel_height : cAfterRelHeightAll.m_mapData){
		// �Ή����郊���N�����݂��Ȃ�
		std::map<UNIQID,CRWLDiv>::const_iterator itRWLDiv = cAfterRWLAll.m_mapData.find(rel_height.first);
		if(cAfterRWLAll.m_mapData.cend() == itRWLDiv){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNExistInRWL, cAfterRelHeightAll.m_strTableName, rel_height.first.eID, rel_height.first.eDivID);
			bReturn = false;
		}
	}

	for(const auto heights : cAfterRelHeightAll.m_mapHeight){
		long lCounter = 0;
		std::pair<long,std::pair<long,long>> preHeight;
		for(const auto height : heights.second){
			if(0 == lCounter){
				lCounter++;
				preHeight = height;
				continue;
			}
			
			// �ڑ��_���A�����Ă��Ȃ��ꍇ
			if(preHeight.first+1 != height.first){
				lCounter++;
				preHeight = height;
				continue;
			}
			// ����LINKIDID�̃����N�Őڑ��_�̍������s��v
			if(preHeight.second.second != height.second.first){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameHeight, cAfterRelHeightAll.m_strTableName, heights.first, height.first);
				bReturn = false;
			}

			lCounter++;
			preHeight = height;
		}
	}

	return bReturn;
}

bool CCheckNWIntegrity::IsRWNDuplicateGeom(const LONLAT& cLonLat, const CRWNAll& cRWNAll)
{
	bool bReturn = false;
	long lCount = cRWNAll.m_mapLonLat2OID.count(cLonLat);
	if(1 < lCount){
		bReturn = true;
		// �����N�����������̏ꍇ
		if(check_mode::kSegmentAttr == m_CheckMode){
			// �d�����Ă���̂�2���ŁuDIVID�Ȃ����s�҃m�[�h�{DIVID���蓹�H�m�[�h�̃y�A�v�����F�߂Ȃ�
			if(2 == lCount){
				auto lonlat2oid = cRWNAll.m_mapLonLat2OID.find(cLonLat);
				const auto cRWN1 = cRWNAll.m_mapData.at(lonlat2oid->second);
				lonlat2oid++;
				const auto cRWN2 = cRWNAll.m_mapData.at(lonlat2oid->second);
				if(WALK_ID > cRWN1.eObjectID.eID && 0 < cRWN1.eObjectID.eDivID &&
					WALK_ID < cRWN2.eObjectID.eID && 0 == cRWN2.eObjectID.eDivID){
						bReturn = false;
				}
			}
		}
	}
	return bReturn;
}
