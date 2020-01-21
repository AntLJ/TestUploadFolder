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

// DRMARoadLinkRelation.cpp: DRMARoadLinkRelation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMARoadLinkRelation.h"
#include "DRMACodeCommunity.h"

const int DRMARoadLinkRelation::sRecordId = drma::kRoadLinkRelationId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMARoadLinkRelation::DRMARoadLinkRelation(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId.eNode1Id));
	VALID(SET_VALUE(eId.eNode2Id));
	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(eAllLinkCount));
	appendAllLinks(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void DRMARoadLinkRelation::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendAllLinks(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void DRMARoadLinkRelation::appendAllLinks(const Format& cOriginalRecord)
{
	for(int i = 0; i < 24 && (int)eAllLinks.size() < eAllLinkCount; i++) {
		eAllLinks.push_back(AllLink(cOriginalRecord.eAllLinks[i]));
	}
}

// DRMARoadLinkRelation::AllLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMARoadLinkRelation::AllLink::AllLink(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eNode1Id));
	VALID(SET_VALUE(eNode2Id));
}
