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

// INRoadLinkTraceCondition.cpp: INRoadLinkTraceCondition クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "INRoadLinkTraceCondition.h"
#include "ZMap.h"

INRoadLinkTraceCondition::INRoadLinkTraceCondition() : ePassedLink(), eLinkList()
{

}

INRoadLinkTraceCondition::~INRoadLinkTraceCondition()
{

}

bool INRoadLinkTraceCondition::Passable(INROADINFO *cRoadInfo, IN2Link *cLink, IN2Node *cNode)
{
	// 反対側のノードの取得
	IN2Node *aNode = NULL;
	if (cNode->eID == cLink->eID1)
	{
		if (! cRoadInfo->GetToNode(cLink, &aNode)) { std::cerr << "ノードの取得に失敗しました" << std::endl; return false; }
	}
	else
	{
		if (! cRoadInfo->GetFromNode(cLink, &aNode)) { std::cerr << "ノードの取得に失敗しました" << std::endl; return false; }
	}

	bool passable = true;
	if (! cRoadInfo->Passable(cNode, aNode, &passable)) { std::cerr << "通行状態のチェックに失敗しました" << std::endl; return false; }

	return passable;
}

bool INRoadLinkTraceCondition::TurnCorner(INROADINFO *cRoadInfo, IN2Link *cLink)
{
	unsigned long l;
	if ((l = eLinkList.size()) >= 1 && eLinkList[l-1].eMesh == cRoadInfo->eMesh)
	{
		IN2Link *link = cRoadInfo->eLink.search(eLinkList[l-1].eFrom, eLinkList[l-1].eTo);
		if (link == NULL) { std::cerr << "リンクの取得に失敗しました" << std::endl; return false; }
		bool turn_corner = true;
		if (! cRoadInfo->TurnCorner(link, cLink, &turn_corner)) { std::cerr << "右左折禁止のチェックに失敗しました" << std::endl; return false; }
		return turn_corner;
	}

	return true;
}

bool road_trace(INROADINFO *cRoadInfo, IN2Node *cNode, INRoadLinkTraceCondition *cTraceCondition, INRoadInfoSet *cRoadInfoSet)
{
	// 到達できたかどうかのチェック
	if (cTraceCondition->exit(cRoadInfo, cNode))
		return true;

	INROADINFO *apRoadInfo = NULL;
	IN2Node *apNode = NULL;
	if (cNode->eX == 0 || cNode->eY == 0 || cNode->eX == 1000000 || cNode->eY == 1000000)
	{

		int xoff = 0, yoff = 0;
		if (cNode->eX == 0)			xoff = -1;
		if (cNode->eX == 1000000)	xoff =  1;
		if (cNode->eY == 0)			yoff = -1;
		if (cNode->eY == 1000000)	yoff =  1;

		int aMesh = ZMap::GetMovedMesh(cRoadInfo->eMesh, xoff, yoff);
		if (! cRoadInfoSet->Open(aMesh, &apRoadInfo)) { std::cerr << aMesh << "をオープンできません" << std::endl; return false; }

//		if (! cRoadInfoSet->Open(cNode->eAdjMesh, &apRoadInfo)) { std::cerr << cNode->eAdjMesh << "をオープンできません" << std::endl; return false; }
		if ((apNode = apRoadInfo->eNode.search(cNode->eAdjNode)) == NULL) { std::cerr << "ノードの取得に失敗しました" << std::endl; return false; }
	}
	else
	{
		apRoadInfo = cRoadInfo;
		apNode     = cNode;
	}

	// 接続リンクを求める
	std::vector<IN2Link *> aLinks;
	if (! apRoadInfo->GetLink(apNode, &aLinks))
	{
		std::cerr << "接続リンクの取得に失敗しました" << std::endl; return false;
	}

	for (unsigned long i = 0; i < aLinks.size(); i++)
	{
		// 既に通過済みのリンクであれば、通らない
		INRoadLinkTraceCondition::LinkID aKey(apRoadInfo->eMesh, aLinks[i]->eID1, aLinks[i]->eID2);
		std::set<INRoadLinkTraceCondition::LinkID,INRoadLinkTraceCondition::lessLinkID>::iterator aIter = cTraceCondition->ePassedLink.find(aKey);
		if (aIter != cTraceCondition->ePassedLink.end()) continue;

		if (! cTraceCondition->trace(apRoadInfo, aLinks[i], apNode)) continue;

		// 反対側のノードの取得
		IN2Node *apNode2 = NULL;
		if (apNode->eID == aLinks[i]->eID1)
		{
			if (! apRoadInfo->GetToNode(aLinks[i], &apNode2)) { std::cerr << "ノードの取得に失敗しました" << std::endl; return false; }
		}
		else
		{
			if (! apRoadInfo->GetFromNode(aLinks[i], &apNode2)) { std::cerr << "ノードの取得に失敗しました" << std::endl; return false; }
		}

		// 通過済みリンクの登録
		cTraceCondition->push_back(apRoadInfo, aLinks[i]);

		if (road_trace(apRoadInfo, apNode2, cTraceCondition, cRoadInfoSet))
			return true;
		cTraceCondition->pop_back();
	}

	return false;
}
