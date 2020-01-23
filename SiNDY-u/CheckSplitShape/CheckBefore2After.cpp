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
	// 相対高さデータ
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

		// 変換後データに存在しない場合
		if(cAfterRWLAll.m_mapDivGroup.cend() == itAfterRWL){
			if(check_mode::kWalkNode == m_CheckMode){
				// 歩行者リンクマージ処理の場合、マージされたか確認する
				for(const auto group : groupinfo.second){
					for(const auto div : group.eVecDivID){
						CRWLDiv cBeforeRWLDiv = cBeforeRWLAll.m_mapData.at(UNIQID(groupinfo.first, div));
						std::map<UNIQID,CRWLDiv> mapAfterRWLDiv;
						// 変換後側にマージ対象リンクが見つからない場合はエラー
						if(!GetAfterMergeRWL(cBeforeRWLDiv, cAfterRWLAll, mapAfterRWLDiv)){
							CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLMergeData, cBeforeRWLAll.m_strTableName, groupinfo.first, div);
							bReturn = false;
						} else {
							// 変換前データのOIDが1億以下、もしくは歩行者リンク種別が「人車共用」出ない場合はマージ対象外
							if(WALK_ID > cBeforeRWLDiv.eObjectID.eID || 
								sindy::schema::walk_link::walk_class::kWalkableRoad != cBeforeRWLDiv.eVecAttr.at(cBeforeRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link_a::kWalkClass_c)).lVal){
									CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLMergeData2, cBeforeRWLAll.m_strTableName, groupinfo.first, div);
									bReturn = false;
							}
						}
					}
				}
			} else {
				// 歩行者リンクマージ処理後以外はエラー
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
		// 変換後データに同一形状のデータが存在しない
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

	// 再構築した高さノードデータで比較
	for(const auto rebuildRH : mapRebuildHN){
		const auto beforeHNLonLat2OID = cBeforeHNodeAll.m_mapLonLat2OID.at(rebuildRH.first);
		const auto beforeHN = cBeforeHNodeAll.m_mapData.at(beforeHNLonLat2OID);
		bool bCheck = true;
		for(const auto reLink : rebuildRH.second){
			const auto afterRH = cAfterRelHeightAll.m_mapData.find(reLink);
			if(cAfterRelHeightAll.m_mapData.cend() == afterRH){
				// 高さ情報も出力させる
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

		// 変換後再構築高さノードの属性取得
		// ⇒このデータはそれなりに整合が取れているものなので、ここではチェックしない
		const auto afterHN1 = cAfterRelHeightAll.m_mapData.at(rebuildRH.second.at(0));
		const auto afterHN3 = cAfterRelHeightAll.m_mapData.at(rebuildRH.second.at(2));

		// 変換前高さノードの属性取得
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
			// 位置が一致していてもリンクIDが一致しない場合はエラー
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
			// 位置が一致していてもリンクIDが一致しない場合はエラー
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgHNSameLinkIdAsAfter, cBeforeHNodeAll.m_strTableName, linkid2, -1);
			bReturn = false;
		}
	}
	
	return bReturn;
}

bool CCheckBefore2After::GetAfterMergeRWL(const CRWLDiv& cBeforeRWLDiv, const CRWLAll& cAfterRWLAll, std::map<UNIQID,CRWLDiv>& mapAfterMergeRWLDiv)
{
	// 変換前リンクのセグメントが、変換後リンクの「人車共用リンクのセグメントリスト」内に存在するかチェック
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
