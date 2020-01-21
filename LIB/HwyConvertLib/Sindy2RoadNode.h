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

// Sindy2RoadNode.h: Sindy2RoadNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2ROADNODE_H__A2F31FF9_8826_442F_8CA3_77734296F453__INCLUDED_)
#define AFX_SINDY2ROADNODE_H__A2F31FF9_8826_442F_8CA3_77734296F453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyRoadNode.h"

class Sindy2RoadNode : public SindyRoadNode  
{
public:
	Sindy2RoadNode(SindyDBConnection *cDBConnection);
	virtual ~Sindy2RoadNode();

	virtual HRESULT LoadByPoints(SindyPointClass *cPointClass);
};

#endif // !defined(AFX_SINDY2ROADNODE_H__A2F31FF9_8826_442F_8CA3_77734296F453__INCLUDED_)
