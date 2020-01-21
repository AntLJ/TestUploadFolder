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

// DRMAValues.cpp
//
//////////////////////////////////////////////////////////////////////

#include "DRMAValues.h"

DRMABaseLinkId::DRMABaseLinkId()
{
}

DRMABaseLinkId::DRMABaseLinkId(int nNode1Id, int nNode2Id) :
eNode1Id(nNode1Id),
eNode2Id(nNode2Id)
{
}

bool operator < (const DRMABaseLinkId& rId1, const DRMABaseLinkId& rId2)
{
	if(rId1.eNode1Id < rId2.eNode1Id)
		return true;

	if(rId1.eNode1Id > rId2.eNode1Id)
		return false;

	return rId1.eNode2Id < rId2.eNode2Id;
}
