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

// SindyHighwayNode.h: SindyHighwayNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYHIGHWAYNODE_H__3FCED17F_BEEF_4467_8A43_A2A9D415653B__INCLUDED_)
#define AFX_SINDYHIGHWAYNODE_H__3FCED17F_BEEF_4467_8A43_A2A9D415653B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayPoint.h"

class SindyHighwayNode : public SindyHighwayPoint  
{
public:
	SindyHighwayNode(SindyDBConnection *cDBConnection);
	virtual ~SindyHighwayNode();

};

#endif // !defined(AFX_SINDYHIGHWAYNODE_H__3FCED17F_BEEF_4467_8A43_A2A9D415653B__INCLUDED_)
