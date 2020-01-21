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

// INROADINFO.h: INROADINFO クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INROADINFO_H__BDC4B713_D6DC_4648_B9F3_120B998C2A77__INCLUDED_)
#define AFX_INROADINFO_H__BDC4B713_D6DC_4648_B9F3_120B998C2A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Z2LinkLoad.h"
#include "Z2NodeLoad.h"
//#include "Z2RestLoad.h"
//#include "Z2FacilLoad.h"
#include "Z2RegLoad.h"
//#include "ZGuideLoad.h"
#include "ZNInfoLoad.h"
#include "ZLaneLoad.h"
#include "ZLExtLoad.h"

class INROADINFO  
{
public:
	INROADINFO();
	virtual ~INROADINFO();

	virtual bool is_open() { return mOpened; }
	virtual bool Load(const char *cDirectory, long cMesh);
	virtual bool GetFromNode(IN2Link *cLink, IN2Node **cNode);
	virtual bool GetToNode(IN2Link *cLink, IN2Node **cNode);
	virtual bool GetLink(IN2Node *cNode1, IN2Node *cNode2, IN2Link **cLink);
	virtual bool GetLink(IN2Node *cNode, std::vector<IN2Link*> *cLinks);
	virtual bool GetCommonNode(IN2Link *cLink1, IN2Link *cLink2, IN2Node **cNode);
	virtual bool TurnCorner(IN2Link *cLink1, IN2Link *cLink2, bool *cTurnCorner);
	virtual bool Passable(IN2Node *cNode1, IN2Node *cNode2, bool *cPassable);

public:
	Z2LinkLoad		eLink;
	Z2NodeLoad		eNode;
	//Z2RestLoad		eRest;
	//Z2FacilLoad		eFacil;
	Z2RegLoad		eReg;
	//ZGuideLoad		eGuide;
	ZNInfoLoad		eNInfo;
	ZLaneLoad		eLane;
	ZLExtLoad		eLext;
	long			eMesh;
	std::string		eDirectory;

private:
	bool			mOpened;
};

#endif // !defined(AFX_INROADINFO_H__BDC4B713_D6DC_4648_B9F3_120B998C2A77__INCLUDED_)
