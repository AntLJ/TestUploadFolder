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

// DRMABaseRoadLink2.h: DRMABaseRoadLink2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADLINK2_H__9FCE8D63_B0C8_4FBE_BB9F_0FD905F474B1__INCLUDED_)
#define AFX_DRMABASEROADLINK2_H__9FCE8D63_B0C8_4FBE_BB9F_0FD905F474B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMABaseRoadLink.h"

class DRMABaseRoadLinkAttr2;
class DRMABaseRoadLink2 : public DRMABaseRoadLink  
{
public:
	DRMABaseRoadLink2(const Format& cOriginalRecord) :
	DRMABaseRoadLink(cOriginalRecord),
	m_pBaseRoadLinkAttr(0)
	{
	}

// 設定/取得
	void setBaseRoadLinkAttr(const DRMABaseRoadLinkAttr2* pAttr) { m_pBaseRoadLinkAttr = pAttr; }

	const DRMABaseRoadLinkAttr2* baseRoadLinkAttr() const { return m_pBaseRoadLinkAttr; }

private:
	const DRMABaseRoadLinkAttr2* m_pBaseRoadLinkAttr;
};

#endif // !defined(AFX_DRMABASEROADLINK2_H__9FCE8D63_B0C8_4FBE_BB9F_0FD905F474B1__INCLUDED_)
