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

#define MIN_DIST 0.01 // 微小セグメント長（0.01[m] = 1[cm]）

bool CCheckNWIntegrity::run(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll)
{
	bool bReturn = true;

	// RWLに対するチェック
	if(!CheckNWRWLIntegrity(cAfterRWLAll, cAfterRWNAll))
		bReturn = false;

	// RWNに対するチェック
	if(!CheckNWRWNIntegrity(cAfterRWLAll, cAfterRWNAll))
		bReturn = false;

	// rel_heightに対するチェック
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

	// RWLに対するチェック
	for(const auto divoid : cRWLAll.m_mapDivOID){
		CRWLDiv cRWLDivTmp = cRWLAll.m_mapData.at(UNIQID(divoid.first,*divoid.second.begin()));
		CRWLDiv cRWLDivLast = cRWLAll.m_mapData.at(UNIQID(divoid.first,*divoid.second.rbegin()));

		// 最初のDIVIDを持つリンクのFROMDIVIDが0でない
		if(0 != cRWLDivTmp.eFromNodeID.eDivID){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLFirstDivLinkFromDivIDNot0, cRWLAll.m_strTableName, cRWLDivTmp.eObjectID.eID, cRWLDivTmp.eObjectID.eDivID);
			bReturn = false;
		}
		// 最後のDIVIDを持つリンクのTODIVIDが0でない
		if(0 != cRWLDivLast.eToNodeID.eDivID){
			CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLLastDivLinkToDivIDNot0, cRWLAll.m_strTableName, cRWLDivLast.eObjectID.eID, cRWLDivLast.eObjectID.eDivID);
			bReturn = false;
		}

		long counter = 0; // Div用カウンタ
		for(const auto div : divoid.second){
			CRWLDiv cRWLDiv = cRWLAll.m_mapData.at(UNIQID(divoid.first,div));

			const auto rwn_fromnode = cRWNAll.m_mapData.find(cRWLDiv.eFromNodeID);
			if(cRWNAll.m_mapData.cend() == rwn_fromnode){
				// FROMNODEがRWNに存在しない
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistFromNodeInRWN, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eFromNodeID.eID).c_str(), std::to_string(cRWLDiv.eFromNodeID.eDivID).c_str());
				bReturn = false;
			} else {
				if(cRWLDiv.eVecLonLat.at(0) != rwn_fromnode->second.eVecLonLat.at(0)){
					// FROMNODEIDのRWNと始点の形状が不一致
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLFromNodeAndRWNGeomSame, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eFromNodeID.eID).c_str(), std::to_string(cRWLDiv.eFromNodeID.eDivID).c_str());
					bReturn = false;
				}
			}

			const auto rwn_tonode = cRWNAll.m_mapData.find(cRWLDiv.eToNodeID);
			if(cRWNAll.m_mapData.cend() == rwn_tonode){
				// TONODEがRWNに存在しない
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLExistToNodeInRWN, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, std::to_string(cRWLDiv.eToNodeID.eID).c_str(), std::to_string(cRWLDiv.eToNodeID.eDivID).c_str());
				bReturn = false;
			} else {
				if(cRWLDiv.eVecLonLat.at(cRWLDiv.eVecLonLat.size()-1) != rwn_tonode->second.eVecLonLat.at(0)){
					// TONODEIDのRWNと始点の形状が不一致
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
				// 微小セグメントチェック
				if(dDist < m_dMinSegment)
					bSmallSegment = false;

				// 全データ内のセグメント重複チェック
				std::set<LONLAT> setSegment;
				setSegment.insert(cRWLDiv.eVecLonLat.at(l));
				setSegment.insert(cRWLDiv.eVecLonLat.at(l+1));
				std::pair<std::map<std::set<LONLAT>,UNIQID>::const_iterator,bool> itSegment = setDuplicateSegment.insert(std::make_pair(setSegment, cRWLDiv.eObjectID));
				if(!itSegment.second){
					// リンク内属性処理で
					if(check_mode::kSegmentAttr == m_CheckMode){
						// 歩行者リンク種別が人車共用リンクでない場合はエラー
						if(sindy::schema::walk_link::walk_class::kWalkableRoad != cRWLAll.m_mapData.at(itSegment.first->second).eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal &&
							sindy::schema::walk_link::walk_class::kWalkableRoad != cRWLDiv.eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal){
								bDuplicateSegment = false;
						}
					} else // その他は強制的にエラー
						bDuplicateSegment = false;
				}
			}
			// セグメント重複エラーは1リンク1回でOK
			if(!bDuplicateSegment){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLDuplicateSegment, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
				bReturn = false;
			}
			// 微小セグメントエラーは1リンク1回
			if(!bSmallSegment){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSmallSegment, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, CString(std::to_string(m_dMinSegment).c_str())+_T("[m]以下"));
				bReturn = false;
			}

			// 構成点重複チェック
			long lLonLatSize = cRWLDiv.eVecLonLat.size();
			std::set<LONLAT> setCheckVertex;
			// リンク内属性分割時は歩行者リンク種別が人車共用の場合は処理スルーさせるためのフラグ設定
			bool bCheckSegHumanCar = false;
			if(check_mode::kSegmentAttr == m_CheckMode){
				if(sindy::schema::walk_link::walk_class::kWalkableRoad == cRWLDiv.eVecAttr.at(cRWLAll.m_mapAttrName2Index.at(shp05::schema::road::road_walk_link::kWalkClass_c)).lVal)
					bCheckSegHumanCar = true;
			}
			bool bVertex = true, bVertexAll = true;
			for(long l = 0; l < lLonLatSize; ++l){
				LONLAT cVertex = cRWLDiv.eVecLonLat.at(l);

				std::pair<std::set<LONLAT>::const_iterator,bool> itVertex = setCheckVertex.insert(cVertex);
				// 構成点重複チェック
				if(!itVertex.second)
					bVertex = false;

				// リンク内属性分割時で人車共用リンクは、全データ構成点チェックはスルー
				if(!bCheckSegHumanCar){
					// 全データでの構成点重複チェック（チェック自体はFromTo含めた全構成点で行う）
					const auto vertexAll = mapVertexAll.find(cVertex);
					if(mapVertexAll.cend() != vertexAll){
						bool bCheck = true;
						if(0 == l || lLonLatSize-1 == l){
							// 端点同士の場合はエラーとしない
							if(vertexAll->second)
								bCheck = false;
						} else
							bCheck = false;

						// リンク内属性分割処理で、この構成点にRWNが存在し、紐付くリンクに人車共用リンクがある場合はチェックスルーできる
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
						// 端点の場合はフラグOFFで新規追加、既存のデータがある場合は既存のフラグを使用する
						mapVertexAll.insert(std::make_pair(cVertex,false));
					} else {
						// 端点でない場合はフラグをONにする
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

			// ループリンク存在チェック
			std::set<UNIQID> setLinkNode;
			setLinkNode.insert(cRWLDiv.eFromNodeID);
			setLinkNode.insert(cRWLDiv.eToNodeID);
			std::pair<std::map<std::set<UNIQID>,UNIQID>::const_iterator,bool> itLoopLink = setLoopLink.insert(std::make_pair(setLinkNode, cRWLDiv.eObjectID));
			if(!itLoopLink.second){
				bool bLoop = false;
				// リンク内属性処理で
				if(check_mode::kSegmentAttr == m_CheckMode){
					// 歩行者リンク種別が人車共用リンクの場合はエラーではない
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

			// 同一OBJECTID群の属性が不一致
			for(long l = 0; l < (long)cRWLDiv.eVecAttr.size(); ++l){
				// リンク内属性、歩行可能コードはスルー
				if(0 == cRWLAll.m_mapAttrIndex2Name.at(l).CompareNoCase(shp05::schema::road::road_walk_link_a::kRoadAttr_c) ||
					0 == cRWLAll.m_mapAttrIndex2Name.at(l).CompareNoCase(shp05::schema::road::road_walk_link_a::kWalkable_c)) 
					continue;

				// ひとつ前のDIVIDと属性比較
				if(cRWLDivTmp.eVecAttr.at(l) != cRWLDiv.eVecAttr.at(l)){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSameAttribute, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID, cRWLAll.m_mapAttrIndex2Name.at(l));
					bReturn = false;
				}
			}
			// 同一OBJECTID群のDIVIDが不連続
			//  →DivIDが「0」のデータは分割されていないため下記処理はスルー
			if(0 < counter){
				// DivIDが連番になっているか
				// DivIDに抜けが無いか
				// ひとつ前のDIVIDの終点と現在DIVIDの始点が一致しているか
				if(cRWLDivTmp.eObjectID.eDivID != counter ||
					cRWLDivTmp.eObjectID.eDivID != cRWLDiv.eObjectID.eDivID - 1 ||
					cRWLDivTmp.eVecLonLat.at(cRWLDivTmp.eVecLonLat.size()-1) != cRWLDiv.eVecLonLat.at(0)){
						CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWLSequenceDivID, cRWLAll.m_strTableName, cRWLDiv.eObjectID.eID, cRWLDiv.eObjectID.eDivID);
						bReturn = false;
				}
			}
			// DivIDカウンタインクリメント
			counter++;

			// ひとつ前のRWLDivとするため更新
			cRWLDivTmp = cRWLDiv;
		}
	}
	return bReturn;
}

bool CCheckNWIntegrity::CheckNWRWNIntegrity(const CRWLAll& cRWLAll, const CRWNAll& cRWNAll)
{
	bool bReturn = true;

	// RWNに対するチェック
	for(const auto lonlat2oid : cRWNAll.m_mapLonLat2OID){
		if(IsRWNDuplicateGeom(lonlat2oid.first, cRWNAll)){
			// 形状重複エラー
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
			// RWLのFROMTONODEに存在しない
			std::map<UNIQID,std::set<UNIQID>>::const_iterator itNode2Link = cRWLAll.m_mapNode2Link.find(UNIQID(divoid.first, div));
			if(cRWLAll.m_mapNode2Link.cend() == itNode2Link){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNExistInRWL, cRWNAll.m_strTableName, divoid.first, div);
				bReturn = false;
			}

			if(0 == div)
				continue;

			lCounter++;
			CRWNDiv cRWNDiv= cRWNAll.m_mapData.at(UNIQID(divoid.first, div));

			// DIVIDを持つノードOBJECTIDが接続リンクの最小OBJECTIDになっていない
			if(cRWLAll.m_mapNode2Link.cend() != itNode2Link){
				if(divoid.first != itNode2Link->second.begin()->eID){
					CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNDivOIDisMinimumRWLOID, cRWNAll.m_strTableName, divoid.first, div);
					bReturn = false;
				}
			}

			// DIVIDを持つ同一OBJECTIDのノードがリンクの向きにDIVIDが連続していない
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
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSequenceDivID, cRWNAll.m_strTableName, divoid.first, div, _T("形状不一致"));
				bReturn = false;
			}
			if(lCounter != div){
				CLog::GetInstance().PrintLog(false, false, false, true, err_type::error, err_code::NgRWNSequenceDivID, cRWNAll.m_strTableName, divoid.first, div, _T("DIV不正"));
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
		// 対応するリンクが存在しない
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
			
			// 接続点が連続していない場合
			if(preHeight.first+1 != height.first){
				lCounter++;
				preHeight = height;
				continue;
			}
			// 同一LINKIDIDのリンクで接続点の高さが不一致
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
		// リンク内属性分割の場合
		if(check_mode::kSegmentAttr == m_CheckMode){
			// 重複しているのは2件で「DIVIDなし歩行者ノード＋DIVIDあり道路ノードのペア」しか認めない
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
