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

// SindyDirguide.h: SindyDirguide クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYDIRGUIDE_H__EE2FA78C_302B_40C5_8460_FB5DE209E639__INCLUDED_)
#define AFX_SINDYDIRGUIDE_H__EE2FA78C_302B_40C5_8460_FB5DE209E639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyLinkQueue.h"

class SindyDirguide : public SindyLinkQueue  
{
public:
	SindyDirguide(SindyDBConnection *cDBConnection);
	virtual ~SindyDirguide();

};

#endif // !defined(AFX_SINDYDIRGUIDE_H__EE2FA78C_302B_40C5_8460_FB5DE209E639__INCLUDED_)
