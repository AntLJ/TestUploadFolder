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

// SindyRoute.h: SindyRoute クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYROUTE_H__5613DFB9_A0FF_4B2D_A976_230EF90CD1B9__INCLUDED_)
#define AFX_SINDYROUTE_H__5613DFB9_A0FF_4B2D_A976_230EF90CD1B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyLinkQueue.h"

class SindyRoute : public SindyLinkQueue  
{
public:
	SindyRoute(SindyDBConnection *cDBConnection);
	virtual ~SindyRoute();

};

#endif // !defined(AFX_SINDYROUTE_H__5613DFB9_A0FF_4B2D_A976_230EF90CD1B9__INCLUDED_)
