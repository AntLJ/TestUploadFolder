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

// DRMALoad2.cpp: DRMALoad2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMALoad2.h"

//////////////////////////////////////////////////////////////////////
// 仮想関数のオーバーライド
//////////////////////////////////////////////////////////////////////

bool DRMALoad2::load(std::istream& cStream)
{
	if(! _DRMALoad2::load(cStream))
		return false;

	// インデックス整備
	mBaseRoadNodeLayer.setIndex();
	mBaseRoadLinkLayer.setIndex();
	mBaseRoadLinkAttrLayer.setIndex();
	mRoadLinkRelationLayer.setIndex();
	mBeaconLocationLayer.setIndex();
	mAllRoadNodeLayer.setIndex();
	mAllRoadLinkLayer.setIndex();
	mSiteLayer.setIndex();
	mAdminLocationLayer.setIndex();
	mRailwayLocationLayer.setIndex();
	mFacilLocationLayer.setIndex();
	mFacilShapeLayer.setIndex();
	mPlaceNameLocationLayer.setIndex();
	mAllRoadLinkAttrLayer.setIndex();

	// ノード関係整備
	for(DRMABaseRoadNodeLayer2::iterator it = mBaseRoadNodeLayer.begin(); it != mBaseRoadNodeLayer.end(); ++it) {
		DRMABaseRoadNode2& rNode = *it;

		DRMAAllRoadNode2* pAllNode = mAllRoadNodeLayer.search(rNode.eId);
		if(pAllNode) {
			rNode.setAllRoadNode(pAllNode);
			pAllNode->setBaseRoadNode(&rNode);
		}
	}

	// 基本道路リンク関係整備
	for(DRMABaseRoadLinkLayer2::iterator it = mBaseRoadLinkLayer.begin(); it != mBaseRoadLinkLayer.end(); ++it) {
		DRMABaseRoadLink2& rLink = *it;

		if(rLink.eAttr == drma::codes::y::kTrue) {
			// 基本道路リンクとリンク内属性の関連付け
			DRMABaseRoadLinkAttr2* pAttr = mBaseRoadLinkAttrLayer.search(rLink.eId);
			rLink.setBaseRoadLinkAttr(pAttr);
			if(pAttr) {
				pAttr->setBaseRoadLink(&rLink);
			}
		}

		// 基本道路リンクと全道路リンクの関連付け
		DRMARoadLinkRelation* pRelation = mRoadLinkRelationLayer.search(rLink.eId);
		if(pRelation) {
			for(int i = 0; i < pRelation->eAllLinkCount; i++) {
				DRMAAllRoadLink2* pAllLink = mAllRoadLinkLayer.search(pRelation->eAllLinks[i]);
				if(pAllLink) {
					pAllLink->setBaseRoadLink(&rLink);
				}
			}
		}
	}

	// 全道路リンク関係整備
	for(DRMAAllRoadLinkLayer2::iterator it = mAllRoadLinkLayer.begin(); it != mAllRoadLinkLayer.end(); ++it) {
		DRMAAllRoadLink2& rLink = *it;

		if(rLink.eAttr == drma::codes::y::kTrue) {
			// 全道路リンクとリンク内属性の関連付け
			DRMAAllRoadLinkAttr2* pAttr = mAllRoadLinkAttrLayer.search(rLink.eId);
			rLink.setAllRoadLinkAttr(pAttr);
			if(pAttr) {
				pAttr->setAllRoadLink(&rLink);
			}
		}
	}

	return true;
}

void DRMALoad2::clear()
{
	_DRMALoad2::clear();

	// インデックス破棄
	mBaseRoadNodeLayer.clearIndex();
	mBaseRoadLinkLayer.clearIndex();
	mBaseRoadLinkAttrLayer.clearIndex();
	mRoadLinkRelationLayer.clearIndex();
	mBeaconLocationLayer.clearIndex();
	mAllRoadNodeLayer.clearIndex();
	mAllRoadLinkLayer.clearIndex();
	mSiteLayer.clearIndex();
	mAdminLocationLayer.clearIndex();
	mRailwayLocationLayer.clearIndex();
	mFacilLocationLayer.clearIndex();
	mFacilShapeLayer.clearIndex();
	mPlaceNameLocationLayer.clearIndex();
	mAllRoadLinkAttrLayer.clearIndex();
}
