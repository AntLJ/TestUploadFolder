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

// SindyRoadNode.h: SindyRoadNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYROADNODE_H__0EEB7216_CA0B_4832_BF30_3F854E3AEE37__INCLUDED_)
#define AFX_SINDYROADNODE_H__0EEB7216_CA0B_4832_BF30_3F854E3AEE37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyPointClass.h"

class SindyRoadNode : public SindyPointClass  
{
public:
	SindyRoadNode(SindyDBConnection *cDBConnection);
	virtual ~SindyRoadNode();

};

#endif // !defined(AFX_SINDYROADNODE_H__0EEB7216_CA0B_4832_BF30_3F854E3AEE37__INCLUDED_)
