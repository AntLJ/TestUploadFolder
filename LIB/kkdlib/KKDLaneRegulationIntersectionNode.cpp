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

// KKDLaneRegulationIntersectionNode.cpp: KKDLaneRegulationIntersectionNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDLaneRegulationIntersectionNode.h"
#include "KKDCodeCommunity.h"

const int KKDLaneRegulationIntersectionNode::sRecordId = kkd::kLaneRegulationIntersectionNodeId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationIntersectionNode::KKDLaneRegulationIntersectionNode(const Format& cOriginalRecord)
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

	VALID(SET_VALUE(eConnectIntersectionCount));
	appendConnectIntersections(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void KKDLaneRegulationIntersectionNode::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendAppertainNodes(cOriginalRecord);
	appendConnectIntersections(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void KKDLaneRegulationIntersectionNode::appendAppertainNodes(const Format& cOriginalRecord)
{
	APPEND_LOOP_FROM(cOriginalRecord, eAppertainNodes, eAppertainNodeCount, 5);
}

void KKDLaneRegulationIntersectionNode::appendConnectIntersections(const Format& cOriginalRecord)
{
	APPEND_LOOP_FROM(cOriginalRecord, eConnectIntersections, eConnectIntersectionCount, 5);
}

// KKDLaneRegulationIntersectionNode::AppertainNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationIntersectionNode::AppertainNode::AppertainNode(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(ePrefectureCode));
	VALID(SET_VALUE(eStationCode));
	VALID(SET_VALUE(eCrossId));
	VALID(SET_VALUE(eNodeMeshDirection));
	VALID(SET_VALUE(eNodeId));
}

// KKDLaneRegulationIntersectionNode::ConnectIntersection クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

KKDLaneRegulationIntersectionNode::ConnectIntersection::ConnectIntersection(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eMeshCode));
	VALID(SET_VALUE(eConnectIntersectionNodeId));
	VALID(SET_VALUE(ePrefectureCode));
	VALID(SET_VALUE(eStationCode));
	VALID(SET_VALUE(eCrossId));
}
