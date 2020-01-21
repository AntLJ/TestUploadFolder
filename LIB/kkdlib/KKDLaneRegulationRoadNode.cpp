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

// KKDLaneRegulationRoadNode.cpp: KKDLaneRegulationRoadNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDLaneRegulationRoadNode.h"
#include "KKDCodeCommunity.h"

const int KKDLaneRegulationRoadNode::sRecordId = kkd::kLaneRegulationRoadNodeId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationRoadNode::KKDLaneRegulationRoadNode(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(ePrefectureCode));
	VALID(SET_VALUE(eStationCode));
	VALID(SET_VALUE(eCrossId));

	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(eId));

	VALID(SET_VALUE(eAppertainNodeCount));
	appendAppertainNodes(cOriginalRecord);

	VALID(SET_VALUE(ePoint.eX));
	VALID(SET_VALUE(ePoint.eY));

	VALID(SET_VALUE(eClass));

	VALID(SET_VALUE(eConnectLinkCount));
	appendConnectLinks(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void KKDLaneRegulationRoadNode::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendAppertainNodes(cOriginalRecord);
	appendConnectLinks(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void KKDLaneRegulationRoadNode::appendAppertainNodes(const Format& cOriginalRecord)
{
	APPEND_LOOP_FROM(cOriginalRecord, eAppertainNodes, eAppertainNodeCount, 4);
}

void KKDLaneRegulationRoadNode::appendConnectLinks(const Format& cOriginalRecord)
{
	if(! eConnectLinks.empty()) {
		ConnectLink& rLink = *eConnectLinks.rbegin();
		if(rLink.append(cOriginalRecord.eConnectLinks[0]))
			return;
	}

	APPEND_LOOP_FROM(cOriginalRecord, eConnectLinks, eConnectLinkCount, 1);
}

// KKDLaneRegulationRoadNode::ConnectLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationRoadNode::ConnectLink::ConnectLink(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eConnectNodeMeshDirection));
	VALID(SET_VALUE(eConnectNodeId));
	VALID(SET_VALUE(eConnectIntersectionId.eMeshCode));
	VALID(SET_VALUE(eConnectIntersectionId.ePrefectureCode));
	VALID(SET_VALUE(eConnectIntersectionId.eStationCode));
	VALID(SET_VALUE(eConnectIntersectionId.eCrossId));

	VALID(SET_VALUE(eUTurnRegulation.eClass));
	VALID(SET_VALUE(eUTurnRegulation.eCarType));
	VALID(SET_VALUE(eUTurnRegulation.eWeekday));
	VALID(SET_VALUE(eUTurnRegulation.eTime));
	VALID(SET_VALUE(eUTurnRegulation.eDate));

	VALID(SET_VALUE(eRegulationCount));
	VALID(SET_VALUE(eLaneCount));
	VALID(SET_VALUE(eApproachLaneCount));

	appendIntersectionRegulations(cOriginal);
}

bool KKDLaneRegulationRoadNode::ConnectLink::append(const Format& cOriginal)
{
	return appendIntersectionRegulations(cOriginal);
}

bool KKDLaneRegulationRoadNode::ConnectLink::appendIntersectionRegulations(const Format& cOriginal)
{
	bool aResult = false;
	if((int)eIntersectionRegulations.size() < eRegulationCount) {
		APPEND_LOOP_FROM(cOriginal, eIntersectionRegulations, eRegulationCount, 2);
		aResult =  true;
	}
	return aResult;
}

// KKDLaneRegulationRoadNode::AppertainNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationRoadNode::AppertainNode::AppertainNode(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(ePrefectureCode));
	VALID(SET_VALUE(eStationCode));
	VALID(SET_VALUE(eCrossId));
	VALID(SET_VALUE(eNodeMeshDirection));
	VALID(SET_VALUE(eNodeId));
}

bool KKDLaneRegulationRoadNode::AppertainNode::operator < (const AppertainNode& rNode) const
{
	if(eNodeMeshDirection < rNode.eNodeMeshDirection)
		return true;
	else if(eNodeMeshDirection > rNode.eNodeMeshDirection)
		return false;

	if(ePrefectureCode < rNode.ePrefectureCode)
		return true;
	else if(ePrefectureCode > rNode.ePrefectureCode)
		return false;

	if(eStationCode < rNode.eStationCode)
		return true;
	else if(eStationCode > rNode.eStationCode)
		return false;

	if(eCrossId < rNode.eCrossId)
		return true;
	else if(eCrossId > rNode.eCrossId)
		return false;

	return eNodeId < rNode.eNodeId;
}

// KKDLaneRegulationRoadNode::IntersectionRegulation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationRoadNode::IntersectionRegulation::IntersectionRegulation(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eOppositeNodeMeshDirection));
	VALID(SET_VALUE(eOppositeNodeId));
	VALID(SET_VALUE(eOppositeIntersectionId.eMeshCode));
	VALID(SET_VALUE(eOppositeIntersectionId.ePrefectureCode));
	VALID(SET_VALUE(eOppositeIntersectionId.eStationCode));
	VALID(SET_VALUE(eOppositeIntersectionId.eCrossId));
	VALID(SET_VALUE(eRegulationLane));
	VALID(SET_VALUE(ePassage));
	VALID(SET_VALUE(eCarType));
	VALID(SET_VALUE(eWeekday));
	VALID(SET_VALUE(eTime));
	VALID(SET_VALUE(eDate));
}
