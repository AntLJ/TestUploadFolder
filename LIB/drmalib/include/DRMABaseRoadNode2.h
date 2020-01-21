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

// DRMABaseRoadNode2.h: 
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_BASEROADNODE_H__
#define __DRMA_BASEROADNODE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMABaseRoadNode.h"

class DRMAAllRoadNode2;
class DRMABaseRoadNode2 : public DRMABaseRoadNode  
{
public:
	DRMABaseRoadNode2(const Format& cOriginalRecord) :
	DRMABaseRoadNode(cOriginalRecord),
	m_pAllRoadNode(0)
	{
	}

// ê›íË/éÊìæ
	void setAllRoadNode(const DRMAAllRoadNode2* pNode) { m_pAllRoadNode = pNode; }

	const DRMAAllRoadNode2* allRoadNode() const { return m_pAllRoadNode; }

private:
	const DRMAAllRoadNode2* m_pAllRoadNode;
};

#endif // __DRMA_BASEROADNODE_H__
