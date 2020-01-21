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

// DRMALoad2.cpp: DRMALoad2 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMALoad2.h"

//////////////////////////////////////////////////////////////////////
// ���z�֐��̃I�[�o�[���C�h
//////////////////////////////////////////////////////////////////////

bool DRMALoad2::load(std::istream& cStream)
{
	if(! _DRMALoad2::load(cStream))
		return false;

	// �C���f�b�N�X����
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

	// �m�[�h�֌W����
	for(DRMABaseRoadNodeLayer2::iterator it = mBaseRoadNodeLayer.begin(); it != mBaseRoadNodeLayer.end(); ++it) {
		DRMABaseRoadNode2& rNode = *it;

		DRMAAllRoadNode2* pAllNode = mAllRoadNodeLayer.search(rNode.eId);
		if(pAllNode) {
			rNode.setAllRoadNode(pAllNode);
			pAllNode->setBaseRoadNode(&rNode);
		}
	}

	// ��{���H�����N�֌W����
	for(DRMABaseRoadLinkLayer2::iterator it = mBaseRoadLinkLayer.begin(); it != mBaseRoadLinkLayer.end(); ++it) {
		DRMABaseRoadLink2& rLink = *it;

		if(rLink.eAttr == drma::codes::y::kTrue) {
			// ��{���H�����N�ƃ����N�������̊֘A�t��
			DRMABaseRoadLinkAttr2* pAttr = mBaseRoadLinkAttrLayer.search(rLink.eId);
			rLink.setBaseRoadLinkAttr(pAttr);
			if(pAttr) {
				pAttr->setBaseRoadLink(&rLink);
			}
		}

		// ��{���H�����N�ƑS���H�����N�̊֘A�t��
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

	// �S���H�����N�֌W����
	for(DRMAAllRoadLinkLayer2::iterator it = mAllRoadLinkLayer.begin(); it != mAllRoadLinkLayer.end(); ++it) {
		DRMAAllRoadLink2& rLink = *it;

		if(rLink.eAttr == drma::codes::y::kTrue) {
			// �S���H�����N�ƃ����N�������̊֘A�t��
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

	// �C���f�b�N�X�j��
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
