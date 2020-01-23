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

// SindyFacilInfoPoint.cpp: SindyFacilInfoPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyFacilInfoPoint.h"
#include "SindyRoadNetwork.h"
#include <boost/utility.hpp>	// for next

SindyFacilInfoPoint::SindyFacilInfoPoint(SindyDBConnection *cDBConnection) :
SindyHighwayPoint(cDBConnection, sindy::layer::FACIL_INFO_POINT)
{
}

SindyFacilInfoPoint::~SindyFacilInfoPoint()
{
}

_IRow* SindyFacilInfoPoint::SearchByFacilCode(long cRoadCode, long cRoadSeq)
{
	GDBHWYFACILRANGE aFacilRange;
	SindyHighwayPoint::SearchByFacilCode(cRoadCode, cRoadSeq, &aFacilRange);

	// 見つからない場合はNULLを返す
	if (aFacilRange.first == aFacilRange.second)
		{ return NULL; }

	// 見つかった場合は１つであると断言
	ATLASSERT(boost::next(aFacilRange.first) == aFacilRange.second);

	return aFacilRange.first->second;
}

