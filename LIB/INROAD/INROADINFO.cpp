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

// INROADINFO.cpp: INROADINFO クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "INROADINFO.h"


INROADINFO::INROADINFO() :
eLink(),
eNode(),
//eRest(),
//eFacil(),
eReg(),
//eGuide(),
eNInfo(),
eLane(),
eLext(),
eMesh(0),
eDirectory(""),
mOpened(false)
{

}

INROADINFO::~INROADINFO()
{

}

bool INROADINFO::Load(const char *cDirectory, long cMesh)
{
	eMesh = cMesh;
	eDirectory = cDirectory;

	if (! eLink.load(cDirectory, eMesh)) return false;
	if (! eNode.load(cDirectory, eMesh)) return false;
	//if (! eRest.load(cDirectory, eMesh)) return false;
	//if (! eFacil.load(cDirectory, eMesh)) return false;
	if (! eReg.load(cDirectory, eMesh)) return false;
	//if (! eGuide.load(cDirectory, eMesh)) return false;
	if (! eNInfo.load(cDirectory, eMesh)) return false;
	if (! eLane.load(cDirectory, eMesh)) return false;
	if (! eLext.load(cDirectory, eMesh)) return false;

	mOpened = true;

	return true;
}

bool INROADINFO::GetFromNode(IN2Link *cLink, IN2Node **cNode)
{
	*cNode = eNode.search(cLink->eID1);
	return true;
}

bool INROADINFO::GetToNode(IN2Link *cLink, IN2Node **cNode)
{
	*cNode = eNode.search(cLink->eID2);
	return true;
}

bool INROADINFO::GetLink(IN2Node *cNode1, IN2Node *cNode2, IN2Link **cLink)
{
	*cLink = eLink.search(cNode1->eID, cNode2->eID);
	return true;
}

bool INROADINFO::GetLink(IN2Node *cNode, std::vector<IN2Link*> *cLinks)
{
	cLinks->clear();

	for (long i = 0; i < cNode->eJointCount; i++)
	{
		IN2Link *link = eLink.search(cNode->eID, cNode->eJoint[i].eID);
		if (link == NULL)
		{
			if ((link = eLink.search(cNode->eJoint[i].eID, cNode->eID)) == NULL)
				return false;
		}
		cLinks->push_back(link);
	}
	return true;
}

bool INROADINFO::GetCommonNode(IN2Link *cLink1, IN2Link *cLink2, IN2Node **cNode)
{
	*cNode = NULL;

	if (cLink1->eID1 == cLink2->eID1 || cLink1->eID1 == cLink2->eID2) *cNode = eNode.search(cLink1->eID1);
	else if (cLink1->eID2 == cLink2->eID1 || cLink1->eID2 == cLink2->eID2) *cNode = eNode.search(cLink1->eID2);
	return true;
}

bool INROADINFO::TurnCorner(IN2Link *cLink1, IN2Link *cLink2, bool *cTurnCorner)
{
	*cTurnCorner = false;

	IN2Node *node =  NULL;
	if (! GetCommonNode(cLink1, cLink2, &node)) { *cTurnCorner = false; return true; }

	int n1 = (cLink1->eID1 == node->eID) ? cLink1->eID2 : cLink1->eID1;
	int n2 = node->eID;
	int n3 = (cLink2->eID1 == node->eID) ? cLink2->eID2 : cLink2->eID1;

	std::vector<INReg*> aRegList;
	eReg.searchByID(node->eID, &aRegList);
	if (! aRegList.size()) { *cTurnCorner = true; return true; }

	for (unsigned int i = 0; i < aRegList.size(); i++)
	{
		INReg *reg = aRegList[i];
		if (reg->eCond.eClass != 2) continue;
		if (reg->eID1 == n2 && reg->eID2 == n1 && reg->eID3 == n3)
		{
			if (reg->eCond.eCond == 1) { *cTurnCorner = false; return true; }
		}
	}

	*cTurnCorner = true;

	return true;
}

bool INROADINFO::Passable(IN2Node *cNode1, IN2Node *cNode2, bool *cPassable)
{
	*cPassable = false;

	IN2Link *link = eLink.search(cNode1->eID, cNode2->eID);
	if (link == NULL) return true;

	bool dir_fore = (link->eID1 == cNode1->eID) ? true : false;

	switch (link->eInfo.eRest)
	{
	case  2: *cPassable = false; break;
	case  4: *cPassable = dir_fore ? true : false; break;
	case  5: *cPassable = dir_fore ? false : true; break;
	default: *cPassable = true; break;
	}

	return true;
}
