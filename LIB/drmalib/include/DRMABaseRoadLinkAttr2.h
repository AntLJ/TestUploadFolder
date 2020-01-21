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

// DRMABaseRoadLinkAttr2.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_BASEROADLINKATTR_H__
#define __DRMA_BASEROADLINKATTR_H__

#include "DRMABaseRoadLinkAttr.h"
#include "DRMABaseRoadLink2.h"

class DRMABaseRoadLinkAttr2 : public DRMABaseRoadLinkAttr  
{
public:
	DRMABaseRoadLinkAttr2(const Format& cOriginalRecord) :
	DRMABaseRoadLinkAttr(cOriginalRecord),
	m_pBaseRoadLink(0)
	{
	}

// ê›íË/éÊìæ
	void setBaseRoadLink(const DRMABaseRoadLink2* pLink) { m_pBaseRoadLink = pLink; }

	const DRMABaseRoadLink2* baseRoadLink() const { return m_pBaseRoadLink; }

private:
	const DRMABaseRoadLink2* m_pBaseRoadLink;
};

#endif // __DRMA_BASEROADLINKATTR_H__
