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

// SindyCityAnnotation.h: SindyCityAnnotation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYCITYANNOTATION_H__D0020C29_BC17_4847_A8C1_6902468988A0__INCLUDED_)
#define AFX_SINDYCITYANNOTATION_H__D0020C29_BC17_4847_A8C1_6902468988A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyAnnotation.h"

class SindyCityAnnotation : public SindyAnnotation  
{
public:
	SindyCityAnnotation(SindyDBConnection *cDBConnection);
	virtual ~SindyCityAnnotation();

};

#endif // !defined(AFX_SINDYCITYANNOTATION_H__D0020C29_BC17_4847_A8C1_6902468988A0__INCLUDED_)
