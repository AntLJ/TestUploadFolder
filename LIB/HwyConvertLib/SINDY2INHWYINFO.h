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

// SINDY2INHWYINFO.h: SINDY2INHWYINFO クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2INHWYINFO_H__17E2DBBF_205C_48CC_8619_7B85060B8166__INCLUDED_)
#define AFX_SINDY2INHWYINFO_H__17E2DBBF_205C_48CC_8619_7B85060B8166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayInfo.h"
#include "SindyErrorInfo.h"

class SINDY2INHWYINFO : public SindyHighwayInfo  
{
public:
	SINDY2INHWYINFO(SindyDBConnection *cDBConnection);
	virtual ~SINDY2INHWYINFO();

	virtual HRESULT Load(bool cRoadInfoLoad = false, const std::string &cHwyNodeWhereCause = "");
};

void PrintError(_IRowPtr ipRow, const char *cMessage);

#endif // !defined(AFX_SINDY2INHWYINFO_H__17E2DBBF_205C_48CC_8619_7B85060B8166__INCLUDED_)
