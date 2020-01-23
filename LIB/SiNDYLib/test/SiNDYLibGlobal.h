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

// SiNDYLibGlobal.h: CSiNDYLibGlobal クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLIBGLOBAL_H__B16A12D2_B283_44F8_9782_3193B0A3D930__INCLUDED_)
#define AFX_SINDYLIBGLOBAL_H__B16A12D2_B283_44F8_9782_3193B0A3D930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnnotationCommunity.h"
#include "AnnoGlobals.h"
using namespace sindy;
using namespace errorcode;

class CSiNDYLibGlobal  
{
public:
	CSiNDYLibGlobal(){}
	virtual ~CSiNDYLibGlobal(){}

	sindyErrCode CheckOrFixAnnotationString( LPCTSTR lpcszString, long lStringLen, schema::annotation::annotation_code::ECode emAnnoCode, bool bIsCity = true, bool bIsString3 = false, bool bFix = false, CString* pstrFixString = NULL )
	{
		return AheCheckOrFixAnnotationString( lpcszString, lStringLen, emAnnoCode, bIsCity, bIsString3, bFix, pstrFixString );
	}
};

#endif // !defined(AFX_SINDYLIBGLOBAL_H__B16A12D2_B283_44F8_9782_3193B0A3D930__INCLUDED_)
