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

// DRMAAllRoadNode2.h: 
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_ALLROADNODE_H__
#define __DRMA_ALLROADNODE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMAAllRoadNode.h"

class DRMABaseRoadNode2;
class DRMAAllRoadNode2 : public DRMAAllRoadNode  
{
public:
	DRMAAllRoadNode2(const Format& cOriginalRecord) :
	DRMAAllRoadNode(cOriginalRecord),
	m_pBaseRoadNode(0)
	{
	}

// ê›íË/éÊìæ
	void setBaseRoadNode(const DRMABaseRoadNode2* pNode) { m_pBaseRoadNode = pNode; }

	const DRMABaseRoadNode2* baseRoadNode() const { return m_pBaseRoadNode; }

private:
	const DRMABaseRoadNode2* m_pBaseRoadNode;
};

#endif // __DRMA_ALLROADNODE_H__
