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

// SindyAnnotation.h: SindyAnnotation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYANNOTATION_H__6BF0AF50_8F83_411A_8AB7_D7C7B65E5762__INCLUDED_)
#define AFX_SINDYANNOTATION_H__6BF0AF50_8F83_411A_8AB7_D7C7B65E5762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyPointClass.h"

class SindyAnnotation : public SindyPointClass  
{
public:
	SindyAnnotation(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyAnnotation();

};

#endif // !defined(AFX_SINDYANNOTATION_H__6BF0AF50_8F83_411A_8AB7_D7C7B65E5762__INCLUDED_)
