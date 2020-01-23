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

// SindyHighwayPoint.h: SindyHighwayPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYHIGHWAYPOINT_H__191999A8_8227_43D9_968E_D02E4D5D0D85__INCLUDED_)
#define AFX_SINDYHIGHWAYPOINT_H__191999A8_8227_43D9_968E_D02E4D5D0D85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyPointClass.h"
#include "SindyHighwayFacil.h"

class SindyHighwayPoint : public SindyPointClass  
{
public:
	SindyHighwayPoint(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyHighwayPoint();

	virtual HRESULT Load(const std::string &cWhereCause = "");
	virtual HRESULT SearchByFacilCode(long cRoadCode, long cRoadSeq, GDBHWYFACILRANGE *cFacilRange);

protected:
	GDBHWYFACILLIST mFacilList;
};

#endif // !defined(AFX_SINDYHIGHWAYPOINT_H__191999A8_8227_43D9_968E_D02E4D5D0D85__INCLUDED_)
