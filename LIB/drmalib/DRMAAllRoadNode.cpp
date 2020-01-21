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

// DRMAAllRoadNode.cpp: DRMAAllRoadNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMAAllRoadNode.h"
#include "DRMACodeCommunity.h"

const int DRMAAllRoadNode::sRecordId = drma::kAllRoadNodeId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMAAllRoadNode::DRMAAllRoadNode(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(eId));
	VALID(SET_VALUE(ePoint.eX));
	VALID(SET_VALUE(ePoint.eY));
	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eJointMeshCode));
	VALID(SET_VALUE(eJointNodeId));

	VALID(SET_VALUE(eConnectLinkCount));
	appendConnectLinks(cOriginalRecord);
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void DRMAAllRoadNode::appendConnectLinks(const Format& cOriginalRecord)
{
	for(int i = 0; i < 8 && (int)eConnectLinks.size() < eConnectLinkCount; i++) {
		eConnectLinks.push_back(ConnectLink(cOriginalRecord.eConnectLinks[i]));
	}
}

// DRMAAllRoadNode::ConnectLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMAAllRoadNode::ConnectLink::ConnectLink(const Format& cOriginal)
{
	VALID(SET_VALUE_FROM(cOriginal, eNodeId));
}
