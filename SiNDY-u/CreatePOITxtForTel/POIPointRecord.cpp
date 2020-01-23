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

#include "stdafx.h"
#include "POIPointRecord.h"

int POIPointRecord::GetPriorValueFromAcc() const
{
	if( 0 == m_strAccCode.CompareNoCase(_T("1S")) )
		return 0;
	else if( 0 == m_strAccCode.CompareNoCase(_T("3A")) )
		return 1;
	else if( 0 == m_strAccCode.CompareNoCase(_T("1A")) )
		return 2;
	else if( 0 == m_strAccCode.CompareNoCase(_T("UK")) )
		return 3;
	else
		return 4;
}
