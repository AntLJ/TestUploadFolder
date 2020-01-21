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

// DRMAAllRoadLink.cpp: DRMAAllRoadLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMAAllRoadLink.h"
#include "DRMACodeCommunity.h"

const int DRMAAllRoadLink::sRecordId = drma::kAllRoadLinkId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMAAllRoadLink::DRMAAllRoadLink(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId.eNode1Id));
	VALID(SET_VALUE(eId.eNode2Id));
	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(eManager));
	VALID(SET_VALUE(eRoadClass));
	VALID(SET_VALUE(eAdminCode)); 
	VALID(SET_VALUE(eLength));
	VALID(SET_VALUE(eWidth));
	VALID(SET_VALUE(eLane));
	VALID(SET_VALUE(eRegulation));
	VALID(SET_VALUE(eRegulationCond));

	VALID(SET_VALUE(eBaseLinkId.eNode1Id));
	VALID(SET_VALUE(eBaseLinkId.eNode2Id));

	VALID(SET_VALUE(ePointCount));
	appendPoints(cOriginalRecord);

	VALID(SET_VALUE(eAttr));

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void DRMAAllRoadLink::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendPoints(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}


void DRMAAllRoadLink::appendPoints(const Format& cOriginalRecord)
{
	for(int i = 0; i < 21 && (int)ePoints.size() < ePointCount; i++) {
		ePoints.push_back(Point(cOriginalRecord.ePoints[i]));
	}
}


// DRMABaseRoadLink::Point クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMAAllRoadLink::Point::Point(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eX));
	VALID(SET_VALUE(eY));
}
