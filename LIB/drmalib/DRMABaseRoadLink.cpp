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

// DRMABaseRoadLink.cpp: DRMABaseRoadLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMABaseRoadLink.h"
#include "DRMACodeCommunity.h"

const int DRMABaseRoadLink::sRecordId = drma::kBaseRoadLinkId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMABaseRoadLink::DRMABaseRoadLink(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId.eNode1Id));
	VALID(SET_VALUE(eId.eNode2Id));
	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(eManager));
	VALID(SET_VALUE(eMain.eRoadClass));
	VALID(SET_VALUE(eMain.eRoadNo));
	VALID(SET_VALUE(eMain.eKubun));

	VALID(SET_VALUE(eDuplicateCount));
	appendDuplicates(cOriginalRecord);

	VALID(SET_VALUE(eAdminCode)); 
	VALID(SET_VALUE(eLength));
	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eBeacon));

	VALID(SET_VALUE(eCarOnly));
	VALID(SET_VALUE(eToll));
	VALID(SET_VALUE(ePassage));
	VALID(SET_VALUE(eWeatherRegulation));
	VALID(SET_VALUE(eCarWeight));
	VALID(SET_VALUE(eCarHeight));
	VALID(SET_VALUE(eCarWidth));
	
	VALID(SET_VALUE(eWidth));
	VALID(SET_VALUE(eLane));
	VALID(SET_VALUE(eRoadWidth));
	VALID(SET_VALUE(eMinRoadWidth));
	VALID(SET_VALUE(eMedianWidth));
	VALID(SET_VALUE(eMedianLength));
	
	VALID(SET_VALUE(eTrafic));
	VALID(SET_VALUE(eTripSpeed));
	
	VALID(SET_VALUE(eRegulation));
	VALID(SET_VALUE(eRegulationCond));
	VALID(SET_VALUE(eNoPassing));
	VALID(SET_VALUE(eSpeedRegulation));
	
	VALID(SET_VALUE(eSpetialVehicle));
	VALID(SET_VALUE(eAttr));
	VALID(SET_VALUE(ePointCount));
	appendPoints(cOriginalRecord);

	VALID(SET_VALUE(eShapeSource));
	VALID(SET_VALUE(eDesignate));

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void DRMABaseRoadLink::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendDuplicates(cOriginalRecord);
	appendPoints(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void DRMABaseRoadLink::appendDuplicates(const Format& cOriginalRecord)
{
	for(int i = 0; i < 3 && (int)eDuplicates.size() < eDuplicateCount; i++) {
		eDuplicates.push_back(Road(cOriginalRecord.eDuplicates[i]));
	}
}

void DRMABaseRoadLink::appendPoints(const Format& cOriginalRecord)
{
	for(int i = 0; i < 16 && (int)ePoints.size() < ePointCount; i++) {
		ePoints.push_back(Point(cOriginalRecord.ePoints[i]));
	}
}

// DRMABaseRoadLink::ConnectLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMABaseRoadLink::Road::Road() :
eRoadClass(0),
eRoadNo(0),
eKubun(0)
{
}

DRMABaseRoadLink::Road::Road(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eRoadClass));
	VALID(SET_VALUE(eRoadNo));
	VALID(SET_VALUE(eKubun));
}

// DRMABaseRoadLink::Point クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMABaseRoadLink::Point::Point(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eX));
	VALID(SET_VALUE(eY));
}
