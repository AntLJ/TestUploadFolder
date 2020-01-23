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

// SindyBaseAnnotation.h: SindyBaseAnnotation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYBASEANNOTATION_H__C01D8E81_4933_427C_A766_62AEB6F19F9D__INCLUDED_)
#define AFX_SINDYBASEANNOTATION_H__C01D8E81_4933_427C_A766_62AEB6F19F9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyAnnotation.h"

class SindyBaseAnnotation : public SindyAnnotation  
{
public:
	SindyBaseAnnotation(SindyDBConnection *cDBConnection);
	virtual ~SindyBaseAnnotation();

};

#endif // !defined(AFX_SINDYBASEANNOTATION_H__C01D8E81_4933_427C_A766_62AEB6F19F9D__INCLUDED_)
