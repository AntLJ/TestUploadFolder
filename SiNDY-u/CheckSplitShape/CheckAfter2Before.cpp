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
	// 相対高さデータ
	if(check_mode::kHeightNode == m_CheckMode){
		if(!CheckHNAfter2Before(cBeforeHNodeAll, cAfterRelHeightAll, mapRebuildHN, setRebuildLinkId))
			bReturn = false;
	}

	return bReturn;
}

bool CCheckAfter2Before::CheckRWLAfter2Before(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll)
{
	bool bReturn = true;

	// グループ単位でチェック
	for(const auto aftergrpinfo : cAfterRWLAll.m_mapDivGroup){
		const auto itBeforeGroup = cBeforeRWLAll.m_mapDivGroup.find(aftergrpinfo.first);
		// 変換前データに存在しない
		if(cBeforeRWLAll.m_mapDivGroup.cend() == itBeforeGroup){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistInBefore, cAfterRWLAll.m_strTableName, aftergrpinfo.first, -1);
			bReturn = false;
			continue;
		}

		// グループが存在していればグループ数は一致している
		for(long l = 0; l < (long)aftergrpinfo.second.size(); ++l){
			RWLGROUP cBeforeRWLGroup = itBeforeGroup->second.at(l);
			RWLGROUP cAfterRWLGroup = aftergrpinfo.second.at(l);
			CRWLDiv cBeforeRWLDivBase = cBeforeRWLAll.m_mapData.at(UNIQID(itBeforeGroup->first, cBeforeRWLGroup.eVecDivID.at(0)));
			CRWLDiv cAfterRWLDivBase = cAfterRWLAll.m_mapData.at(UNIQID(aftergrpinfo.first, cAfterRWLGroup.eVecDivID.at(0)));
			for(const auto divid : cAfterRWLGroup.eVecDivID){
				CRWLDiv cAfterRWLDiv = cAfterRWLAll.m_mapData.at(UNIQID(aftergrpinfo.first, divid));
				// 変換後の属性情報が一致するか
				for(const auto name2index : cAfterRWLAll.m_mapAttrName2Index){
					// 変換前に属性フィールドが存在しない場合
					auto beforeAttrIndex = cBeforeRWLAll.m_mapAttrName2Index.find(name2index.first);
					bool bAttrCheck = true;
					if(cBeforeRWLAll.m_mapAttrName2Index.cend() == beforeAttrIndex){
						// リンク内属性分割でRoadAttrの場合
						if(check_mode::kSegmentAttr == m_CheckMode){
							// リンク内属性フィールドか
							if(0 == name2index.first.CompareNoCase(shp05::schema::road::road_walk_link_a::kRoadAttr_c)){
								// 変換後データの属性が、対応するリンク内属性とマッチしているか
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
						// 属性一致するか
						CComVariant vaBeforeAttr = cBeforeRWLDivBase.eVecAttr.at(beforeAttrIndex->second);
						CComVariant vaAfterAttr = cAfterRWLDiv.eVecAttr.at(name2index.second);
						if(vaBeforeAttr != vaAfterAttr){
							// 一致しない場合、歩行者ノード分割か
							if(check_mode::kWalkNode == m_CheckMode){
								// Walkableフィールドか
								if(0 == name2index.first.CompareNoCase(shp05::schema::road::road_walk_link_a::kWalkable_c)){
									if(IsMatchMergeLink(cAfterRWLDiv, cBeforeRWLAll)){
										// 変換前のWalkableは歩行禁止且つ変換後のWalkableは人車共用の場合のみ許容
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

					// グループ内属性一致チェック
					if(!CheckRWLGroupAttr(cAfterRWLDivBase, cAfterRWLDiv, cAfterRWLAll.m_strTableName, name2index.first, name2index.second)){
						bReturn = false;
					}
				}
			}
			// グループ形状が一致するか確認
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
		// 変換前データに存在しない場合
		if(0 == lCount){
			if(!CheckRWNExist(cAfterRWNAll, cBeforeRSAAll, cBeforeHNodeAll, cAfterRWNDiv))
				bReturn = false;
		} else {
			const auto rangeBeforeLonLat = cBeforeRWNAll.m_mapLonLat2OID.equal_range(afterLonLat2oid.first);
			
			bool bMatch = false, bObjectid = false;
			for(auto it = rangeBeforeLonLat.first; it != rangeBeforeLonLat.second; ++it){
				// 位置もIDも一致している場合は属性チェック
				if(it->second == cAfterRWNDiv.eObjectID){
					// 変換前データに存在する場合は属性値チェック
					cBeforeRWNDiv = cBeforeRWNAll.m_mapData.at(it->second);
					for(const auto name2index : cAfterRWNAll.m_mapAttrName2Index){
						// 変換前属性と不一致
						if(cBeforeRWNDiv.eVecAttr.at(cBeforeRWNAll.m_mapAttrName2Index.at(name2index.first)) != cAfterRWNDiv.eVecAttr.at(name2index.second)){
							CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
							bReturn = false;
						}
					}
					bMatch = true;
				}
				// OBJECTIDだけ一致しているケースをチェック
				if(it->second.eID == cAfterRWNDiv.eObjectID.eID)
					bObjectid = true;
			}
			// OBJECTIDも一致していないケースはノード一致していない場合と同じチェックを実施
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
	
	// 変換後データが変換後再構築データに存在するか
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
				// 始点終点ではなく、途中の構成点が一致しない場合はエラー
				bReturn = false;
				break;
			}

			if(check_mode::kSegmentAttr == m_CheckMode){
				// リンク内属性分割の場合はエラー
				bReturn = false;
				break;
			} else if(check_mode::kWalkNode == m_CheckMode){
				// 歩行者ノード分割の場合、そこに歩行者ノードが存在するか
				std::map<UNIQID,CRWNDiv>::const_iterator itBeforeRWNDiv = cBeforeRWNAll.m_mapData.find(cAfterLonLat.eNodeID);
				// 存在しない場合はエラー
				if(cBeforeRWNAll.m_mapData.cend() == itBeforeRWNDiv){
					bReturn = false;
					break;
				}
				// 存在してもポイント位置が一致していない場合はエラー
				if(itBeforeRWNDiv->second.eVecLonLat.at(0) != cAfterLonLat){
					bReturn = false;
					break;
				}
				// 存在してもIDが一致していない場合はエラー
				if(itBeforeRWNDiv->second.eObjectID != cAfterLonLat.eNodeID){
					bReturn = false;
					break;
				}
			} else if(check_mode::kHeightNode == m_CheckMode){
				// 高さノード分割の場合
				std::map<LONLAT,UNIQID>::const_iterator itBeforeHNode = cBeforeHNodeAll.m_mapLonLat2OID.find(cAfterLonLat);
				// そのに高さノードが存在しない場合はエラー
				if(cBeforeHNodeAll.m_mapLonLat2OID.cend() == itBeforeHNode){
					bReturn = false;
					break;
				}
			}
		} else {
			if(cAfterLonLat.eNodeID != cBeforeLonLat.eNodeID){
				if(check_mode::kSegmentAttr == m_CheckMode && 0 == cBeforeLonLat.eNodeID.eID){
					// リンク内属性分割の場合
					std::set<LONLAT>::const_iterator itRSA = cBeforeRSAAll.m_setFromToLonLat.find(cAfterLonLat);
					// そこにリンク内属性の始点終点が存在しない場合はエラー
					if(cBeforeRSAAll.m_setFromToLonLat.cend() == itRSA){
						bReturn = false;
						break;
					}
				} else if(check_mode::kWalkNode == m_CheckMode){
					// 歩行者ノード分割の場合
					std::map<UNIQID,CRWNDiv>::const_iterator itBeforeRWNDiv = cBeforeRWNAll.m_mapData.find(cAfterLonLat.eNodeID);
					// そこに歩行者ノードが存在しない場合はエラー
					if(cBeforeRWNAll.m_mapData.cend() == itBeforeRWNDiv){
						bReturn = false;
						break;
					}
					// 存在してもIDが一致していない場合はエラー
					if(itBeforeRWNDiv->second.eObjectID != cAfterLonLat.eNodeID){
						bReturn = false;
						break;
					}
				} else if(check_mode::kHeightNode == m_CheckMode){
					// 高さノード分割の場合
					std::map<LONLAT,UNIQID>::const_iterator itBeforeHNode = cBeforeHNodeAll.m_mapLonLat2OID.find(cAfterLonLat);
					// そのに高さノードが存在しない場合、既存の道路、歩行者ノードが存在しない場合はエラー
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
	// リンクに対応するリンク内属性リンクを取得
	std::map<long,std::set<long>>::const_iterator itRSAIDs = cBeforeRSAAll.m_mapLink2Segattr.find(cAfterRWLDiv.eObjectID.eID);
	if(cBeforeRSAAll.m_mapLink2Segattr.cend() == itRSAIDs){
		// リンク内属性が設定されていない場合はスルー
		if(0 == lAfterRWLRoadAttr)
			return true;
		else
			return false;
	}

	for(const auto rsaid : itRSAIDs->second){
		CRSA cRSA = cBeforeRSAAll.m_mapData.at(UNIQID(rsaid, 0));
		// RWL形状がRSA形状に内包されているか確認
		//  →RWLの構成点がRSAの構成点に存在するか確認
		//    →並び順は他のチェックで行っているのでここでは見ない
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
		// 紐付くリンク内属性が無ければスルー
		if(!bFind)
			continue;

		// リンク内属性の属性値を取得
		long lLinkAttr = cRSA.eVecAttr.at(cBeforeRSAAll.m_mapAttrName2Index.at(shp05::schema::road::segment_attr::kLinkAttr_c)).lVal;
		// 道路・歩行者リンクスキーマに合わせた属性値を設定
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
	// 変換前後で属性値が異なる場合はNG
	if(lBeforeRWLRoadAttr != lAfterRWLRoadAttr)
		return false;

	return true;
}

bool CCheckAfter2Before::IsMatchMergeLink(const CRWLDiv& cAfterRWLDiv, const CRWLAll& cBeforeRWLAll)
{
	// 変換後リンクのセグメントが変換前リンクの「人車共用歩行者リンクのセグメントリスト」内に存在するかチェック
	//   →全セグメントの確認が取れればマージ対象リンクと一致していること同義
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
		// リンク内属性分割の場合
		std::set<LONLAT>::const_iterator itRSA = cBeforeRSAAll.m_setFromToLonLat.find(cAfterRWNDiv.eVecLonLat.at(0));
		// そこにリンク内属性の始点終点が存在しない場合はエラー
		if(cBeforeRSAAll.m_setFromToLonLat.cend() == itRSA){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, _T("リンク内属性分割"));
			return false;
		}
		// 属性は固定値になっているか
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
				if(0 != CString(vaValue.bstrVal).Compare(_T(" "))){ // 半角スペース
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSameAttrAsBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, name2index.first);
					bReturn =  false;
					continue;
				}
			}
		}
	} else if(check_mode::kWalkNode == m_CheckMode){
		// 歩行者ノード分割の場合はそもそもここに来るのはおかしい
		CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, _T("歩行者ノード分割"));
		return false;
	} else if(check_mode::kHeightNode == m_CheckMode){
		// 高さノード分割の場合
		std::map<LONLAT,UNIQID>::const_iterator itBeforeHNode = cBeforeHNodeAll.m_mapLonLat2OID.find(cAfterRWNDiv.eVecLonLat.at(0));
		// ここに高さノードが存在しない場合はエラー
		if(cBeforeHNodeAll.m_mapLonLat2OID.cend() == itBeforeHNode){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInBefore, cAfterRWNAll.m_strTableName, cAfterRWNDiv.eObjectID.eID, cAfterRWNDiv.eObjectID.eDivID, _T("高さノード分割"));
			return false;
		}
		// 属性は固定値になっているか
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
				if(0 != CString(vaValue.bstrVal).Compare(_T(" "))){ // 半角スペース
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
	// リンク内属性分割の場合、RoadAttrフィールドはスルー
	if(check_mode::kSegmentAttr == m_CheckMode &&
		0 == strFieldName.CompareNoCase(shp05::schema::road::road_walk_link_a::kRoadAttr_c))
		return true;

	// 歩行者ノード分割・歩行者リンクマージの場合、Walkableフィールドはスルー
	if(check_mode::kWalkNode == m_CheckMode &&
		0 == strFieldName.CompareNoCase(shp05::schema::road::road_walk_link_a::kWalkable_c))
		return true;

	// グループ内で属性が一致していない
	if(cAfterRWLDivBase.eVecAttr.at(lIndex) != cAfterRWLDiv.eVecAttr.at(lIndex)){
		CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameAttrAsSplitData, strAfterRWLTableName, cAfterRWLDiv.eObjectID.eID, cAfterRWLDiv.eObjectID.eDivID, strFieldName);
		return false;
	}

	return true;
}
