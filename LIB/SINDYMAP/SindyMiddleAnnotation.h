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

// SindyMiddleAnnotation.h: SindyMiddleAnnotation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYMIDDLEANNOTATION_H__85A1223A_178A_4469_A475_5671BED9EE6A__INCLUDED_)
#define AFX_SINDYMIDDLEANNOTATION_H__85A1223A_178A_4469_A475_5671BED9EE6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyAnnotation.h"

class SindyMiddleAnnotation : public SindyAnnotation  
{
public:
	SindyMiddleAnnotation(SindyDBConnection *cDBConnection);
	virtual ~SindyMiddleAnnotation();

};

#endif // !defined(AFX_SINDYMIDDLEANNOTATION_H__85A1223A_178A_4469_A475_5671BED9EE6A__INCLUDED_)
