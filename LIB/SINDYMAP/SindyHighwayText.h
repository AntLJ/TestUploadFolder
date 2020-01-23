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

// SindyHighwayText.h: SindyHighwayText クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYHIGHWAYTEXT_H__E8E7ACF1_CD7C_4882_BACA_0AC1887488B5__INCLUDED_)
#define AFX_SINDYHIGHWAYTEXT_H__E8E7ACF1_CD7C_4882_BACA_0AC1887488B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"
#include "SindyHighwayFacil.h"

class SindyHighwayText : public SindyTable  
{
public:
	SindyHighwayText(SindyDBConnection *cDBConnection);
	virtual ~SindyHighwayText();

	virtual HRESULT Load();
	virtual HRESULT SearchByFacilCode(long cRoadCode, long cRoadSeq, GDBHWYFACILRANGE *cFacilRange);

protected:
	GDBHWYFACILLIST mHwyFacilList;
};

#endif // !defined(AFX_SINDYHIGHWAYTEXT_H__E8E7ACF1_CD7C_4882_BACA_0AC1887488B5__INCLUDED_)
