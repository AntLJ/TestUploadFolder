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

// KKDRegulationRoadLink.cpp: KKDRegulationRoadLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDRegulationRoadLink.h"
#include "KKDCodeCommunity.h"

const int KKDRegulationRoadLink::sRecordId = kkd::kRegulationRoadLinkId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadLink::KKDRegulationRoadLink(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(eId.eIntersection1Id.eMeshCode));
	VALID(SET_VALUE(eId.eIntersection1Id.ePrefectureCode));
	VALID(SET_VALUE(eId.eIntersection1Id.eStationCode));
	VALID(SET_VALUE(eId.eIntersection1Id.eCrossId));

	VALID(SET_VALUE(eId.eIntersection2Id.eMeshCode));
	VALID(SET_VALUE(eId.eIntersection2Id.ePrefectureCode));
	VALID(SET_VALUE(eId.eIntersection2Id.eStationCode));
	VALID(SET_VALUE(eId.eIntersection2Id.eCrossId));

	VALID(SET_VALUE(eLinkId.eNode1Id));
	VALID(SET_VALUE(eLinkId.eNode2Id));

	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(eIllNavigation));

	VALID(SET_VALUE(eConditionCount));
	appendConditions(cOriginalRecord);

	VALID(SET_VALUE(eNodeCount));
	VALID(SET_VALUE(eMeshCount));
	appendNodesInMeshs(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void KKDRegulationRoadLink::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendConditions(cOriginalRecord);
	appendNodesInMeshs(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void KKDRegulationRoadLink::appendConditions(const Format& cOriginalRecord)
{
	APPEND_LOOP_FROM(cOriginalRecord, eConditions, eConditionCount, 3);
}

void KKDRegulationRoadLink::appendNodesInMeshs(const Format& cOriginalRecord)
{
	if(! eNodesInMeshs.empty()) {
		NodesInMesh& rNodesInMesh = *eNodesInMeshs.rbegin();
		if(rNodesInMesh.append(cOriginalRecord.eNodesInMeshs[0]))
			return;
	}

	APPEND_LOOP_FROM(cOriginalRecord, eNodesInMeshs, eMeshCount, 1);
}

// KKDRegulationRoadLink::NodesInMesh クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadLink::NodesInMesh::NodesInMesh(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eMeshCode));
	VALID(SET_VALUE(eNodeCount));
	
	appendNodeIds(cOriginal);
}

bool KKDRegulationRoadLink::NodesInMesh::append(const Format& cOriginal)
{
	return appendNodeIds(cOriginal);
}

bool KKDRegulationRoadLink::NodesInMesh::appendNodeIds(const Format& cOriginal)
{
	bool aResult = false;
	if((int)eNodeIds.size() < eNodeCount) {
		for(int i = 0; i < 20 && (int)eNodeIds.size() < eNodeCount; i++) {
			NodeId aNodeId;
			VALID(SET_VALUE_FROM_TO(cOriginal.eNodeIds[i], aNodeId));
			eNodeIds.push_back(aNodeId);
		}
		aResult = true;
	}
	return aResult;
}

// KKDRegulationRoadLink::Condition クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDRegulationRoadLink::Condition::Condition(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eCarType));
	VALID(SET_VALUE(eWeekday));
	VALID(SET_VALUE(eTime));
	VALID(SET_VALUE(eDate));
	VALID(SET_VALUE(eCarWeight));
	VALID(SET_VALUE(eCarWidth));
	VALID(SET_VALUE(eCarHeight));
}
