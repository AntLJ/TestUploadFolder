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

// SindyAccessPoint.h: SindyAccessPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYACCESSPOINT_H__3D0241DE_3F3D_48CD_82F9_44EE6B517F60__INCLUDED_)
#define AFX_SINDYACCESSPOINT_H__3D0241DE_3F3D_48CD_82F9_44EE6B517F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayPoint.h"

class SindyAccessPoint : public SindyHighwayPoint  
{
public:
	SindyAccessPoint(SindyDBConnection *cDBConnection);
	virtual ~SindyAccessPoint();

};

#endif // !defined(AFX_SINDYACCESSPOINT_H__3D0241DE_3F3D_48CD_82F9_44EE6B517F60__INCLUDED_)
