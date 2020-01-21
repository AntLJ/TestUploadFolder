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

// KKDRegulationRoadNode.cpp: KKDRegulationRoadNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDRegulationRoadNode.h"
#include "KKDCodeCommunity.h"

const int KKDRegulationRoadNode::sRecordId = kkd::kRegulationRoadNodeId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadNode::KKDRegulationRoadNode(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(ePrefectureCode));
	VALID(SET_VALUE(eStationCode));
	VALID(SET_VALUE(eCrossId));

	VALID(SET_VALUE(eSignal));
	
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

void KKDRegulationRoadNode::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendAppertainNodes(cOriginalRecord);
	appendConnectLinks(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void KKDRegulationRoadNode::appendAppertainNodes(const Format& cOriginalRecord)
{
	APPEND_LOOP_FROM(cOriginalRecord, eAppertainNodes, eAppertainNodeCount, 5);
}

void KKDRegulationRoadNode::appendConnectLinks(const Format& cOriginalRecord)
{
	if(! eConnectLinks.empty()) {
		ConnectLink& rLink = *eConnectLinks.rbegin();
		if(rLink.append(cOriginalRecord.eConnectLinks[0]))
			return;
	}

	APPEND_LOOP_FROM(cOriginalRecord, eConnectLinks, eConnectLinkCount, 1);
}

// KKDRegulationRoadNode::ConnectLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadNode::ConnectLink::ConnectLink(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eConnectNodeMeshDirection));
	VALID(SET_VALUE(eConnectNodeId));
	VALID(SET_VALUE(eConnectIntersectionId.eMeshCode));
	VALID(SET_VALUE(eConnectIntersectionId.ePrefectureCode));
	VALID(SET_VALUE(eConnectIntersectionId.eStationCode));
	VALID(SET_VALUE(eConnectIntersectionId.eCrossId));
	VALID(SET_VALUE(eIntersectionName.eKanjiLength));
	VALID(SET_VALUE(eIntersectionName.eKanji));
	VALID(SET_VALUE(eIntersectionName.eKanaLength));
	VALID(SET_VALUE(eIntersectionName.eKana));
	VALID(SET_VALUE(eRegulationCount));

#if KKD_VERSION >= 101
	SET_VALUE(eIntersectionNameKanjiResearchLevel);
	SET_VALUE(eIntersectionNameKanaResearchLevel);
	SET_VALUE(eIntersectionResearchDate);
#endif // KKD_VERSION >= 101
	
	appendIntersectionRegulations(cOriginal);
}

bool KKDRegulationRoadNode::ConnectLink::append(const Format& cOriginal)
{
	return appendIntersectionRegulations(cOriginal);
}

bool KKDRegulationRoadNode::ConnectLink::appendIntersectionRegulations(const Format& cOriginal)
{
	bool aResult = false;
	if((int)eIntersectionRegulations.size() < eRegulationCount) {
		APPEND_LOOP_FROM(cOriginal, eIntersectionRegulations, eRegulationCount, 2);
		aResult =  true;
	}
	return aResult;
}

// KKDRegulationRoadNode::AppertainNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadNode::AppertainNode::AppertainNode(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eMeshDirection));
	VALID(SET_VALUE(eId));
}

KKDRegulationRoadNode::AppertainNode::AppertainNode(int nMeshDirection, int nId) :
eMeshDirection(nMeshDirection),
eId(nId)
{
}

bool KKDRegulationRoadNode::AppertainNode::operator < (const AppertainNode& rNode) const
{
	if(eMeshDirection < rNode.eMeshDirection)
		return true;
	else if(eMeshDirection > rNode.eMeshDirection)
		return false;
	else
		return eId < rNode.eId;
}

// KKDRegulationRoadNode::IntersectionRegulation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadNode::IntersectionRegulation::IntersectionRegulation(const Format& cOriginal)
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
