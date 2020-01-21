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

// DRMAAllRoadLink2.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_ALLROADLINK_H__
#define __DRMA_ALLROADLINK_H__

#include "DRMAAllRoadLink.h"

class DRMABaseRoadLink2;
class DRMAAllRoadLinkAttr2;
class DRMAAllRoadLink2 : public DRMAAllRoadLink  
{
public:
	DRMAAllRoadLink2(const Format& cOriginalRecord) :
	DRMAAllRoadLink(cOriginalRecord),
	m_pAllRoadLinkAttr(0),
	m_pBaseRoadLink(0)
	{
	}

// ê›íË/éÊìæ
	void setAllRoadLinkAttr(const DRMAAllRoadLinkAttr2* pAttr) { m_pAllRoadLinkAttr = pAttr; }
	void setBaseRoadLink(const DRMABaseRoadLink2* pLink) { m_pBaseRoadLink = pLink; }

	const DRMAAllRoadLinkAttr2* allRoadLinkAttr() const { return m_pAllRoadLinkAttr; }
	const DRMABaseRoadLink2* baseRoadLink() const { return m_pBaseRoadLink; }

private:
	const DRMAAllRoadLinkAttr2* m_pAllRoadLinkAttr;
	const DRMABaseRoadLink2* m_pBaseRoadLink;
};

#endif // __DRMA_ALLROADLINK_H__
