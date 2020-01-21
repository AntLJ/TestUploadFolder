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

// DRMAAllRoadLinkAttr2.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_ALLROADLINKATTR_H__
#define __DRMA_ALLROADLINKATTR_H__

#include "DRMAAllRoadLinkAttr.h"
#include "DRMAAllRoadLink2.h"

class DRMAAllRoadLinkAttr2 : public DRMAAllRoadLinkAttr  
{
public:
	DRMAAllRoadLinkAttr2(const Format& cOriginalRecord) :
	DRMAAllRoadLinkAttr(cOriginalRecord),
	m_pAllRoadLink(0)
	{
	}

// ê›íË/éÊìæ
	void setAllRoadLink(const DRMAAllRoadLink2* pLink) { m_pAllRoadLink = pLink; }

	const DRMAAllRoadLink2* allRoadLink() const { return m_pAllRoadLink; }

private:
	const DRMAAllRoadLink2* m_pAllRoadLink;
};

#endif // __DRMA_ALLROADLINKATTR_H__
