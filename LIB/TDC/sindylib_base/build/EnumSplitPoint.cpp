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

/**
 * @file EnumSplitPoint.cpp
 * CEnumSplitPointクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "EnumSplitPoint.h"
#include "sindymacroutil.h"
#include "sindycomutil.h"
#include "type_convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;
	
double CEnumSplitPoint::GetSplitDistance() const
{
	double dRet = -1.0; // 返り値
	QUERYINVOKEMETHOD( IEnumSplitPoint, IID_IEnumSplitPoint, get_SplitDistance( &dRet ) );
	return dRet;
}

bool CEnumSplitPoint::SplitHappened() const
{
	VARIANT_BOOL vb = VARIANT_FALSE; // 返り値
	QUERYINVOKEMETHOD( IEnumSplitPoint, IID_IEnumSplitPoint, get_SplitHappened( &vb ) );
	return VB2bool(vb);
}

IPointPtr CEnumSplitPoint::NextInSequence( long* Part/* = NULL*/, long* Vertex/* = NULL*/ ) const
{
	IPointPtr ipPoint; // 返り値
	long lPart = -1, lIndex = -1;
	QUERYINVOKEMETHOD( IEnumSplitPoint, IID_IEnumSplitPoint, NextInSequence( &ipPoint, &lPart, &lIndex ) );
	if( Part ) *Part = lPart;
	if( Vertex ) *Vertex = lIndex;

	return ipPoint;
}

IPointPtr CEnumSplitPoint::PreviousInSequence( long* Part/* = NULL*/, long* Vertex/* = NULL*/ ) const
{
	IPointPtr ipPoint; // 返り値
	long lPart = -1, lIndex = -1;
	QUERYINVOKEMETHOD( IEnumSplitPoint, IID_IEnumSplitPoint, PreviousInSequence( &ipPoint, &lPart, &lIndex ) );
	if( Part ) *Part = lPart;
	if( Vertex ) *Vertex = lIndex;

	return ipPoint;
}

} // sindy
