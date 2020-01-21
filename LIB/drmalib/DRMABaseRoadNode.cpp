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

// DRMABaseRoadNode.cpp: DRMABaseRoadNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMABaseRoadNode.h"
#include "DRMACodeCommunity.h"

const int DRMABaseRoadNode::sRecordId = drma::kBaseRoadNodeId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMABaseRoadNode::DRMABaseRoadNode(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId));
	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(ePoint.eX));
	VALID(SET_VALUE(ePoint.eY));

	VALID(SET_VALUE(eAltitude));
	VALID(SET_VALUE(eClass));

	VALID(SET_VALUE(eJointMeshCode));
	VALID(SET_VALUE(eJointNodeId));

	VALID(SET_VALUE(eConnectLinkCount));
	appendConnectLinks(cOriginalRecord);

	VALID(SET_VALUE(eName.eKanjiLength));
	VALID(SET_VALUE(eName.eKanji));
	VALID(SET_VALUE(eName.eKanaLength));
	VALID(SET_VALUE(eName.eKana));

	VALID(SET_VALUE(eConnectFerryCount));
	appendConnectFerries(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void DRMABaseRoadNode::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendConnectLinks(cOriginalRecord);
	appendConnectFerries(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void DRMABaseRoadNode::appendConnectLinks(const Format& cOriginalRecord)
{
	for(int i = 0; i < 8 && (int)eConnectLinks.size() < eConnectLinkCount; i++) {
		eConnectLinks.push_back(ConnectLink(cOriginalRecord.eConnectLinks[i]));
	}
}

void DRMABaseRoadNode::appendConnectFerries(const Format& cOriginalRecord)
{
	for(int i = 0; i < 5 && (int)eConnectFerries.size() < eConnectFerryCount; i++) {
		eConnectFerries.push_back(ConnectFerry(cOriginalRecord.eConnectFerries[i]));
	}
}

// DRMABaseRoadNode::ConnectLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMABaseRoadNode::ConnectLink::ConnectLink(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eNodeId));
	VALID(SET_VALUE(ePassageCode));
	VALID(SET_VALUE(eAngle));
}

// DRMABaseRoadNode::ConnectFerry クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMABaseRoadNode::ConnectFerry::ConnectFerry(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eJointMeshCode));
	VALID(SET_VALUE(eJointNodeId));
}
