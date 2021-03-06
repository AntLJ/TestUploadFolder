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

// SINDY2INHWY.h: SINDY2INHWY クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2INHWY_H__EF25B0F3_A168_43D7_A732_BD1B8920CBD0__INCLUDED_)
#define AFX_SINDY2INHWY_H__EF25B0F3_A168_43D7_A732_BD1B8920CBD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWYINFO.h"

class SINDY2INHWY  
{
public:
	SINDY2INHWY(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2INHWY();

protected:
	SINDY2INHWYINFO *mpHwyInfo;
};

#ifdef	_DEBUG
static long		debug;
#endif

#endif // !defined(AFX_SINDY2INHWY_H__EF25B0F3_A168_43D7_A732_BD1B8920CBD0__INCLUDED_)
