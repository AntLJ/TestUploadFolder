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

// KKDValues.cpp
//
//////////////////////////////////////////////////////////////////////

#include "KKDValues.h"

bool operator < (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2)
{
	if(rId1.eMeshCode < rId2.eMeshCode)
		return true;
	else if(rId1.eMeshCode > rId2.eMeshCode)
		return false;

	if(rId1.ePrefectureCode < rId2.ePrefectureCode)
		return true;
	else if(rId1.ePrefectureCode > rId2.ePrefectureCode)
		return false;

	if(rId1.eStationCode < rId2.eStationCode)
		return true;
	else if(rId1.eStationCode > rId2.eStationCode)
		return false;

	return rId1.eCrossId < rId2.eCrossId;
}

bool operator > (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2)
{
	if(rId1.eMeshCode > rId2.eMeshCode)
		return true;
	else if(rId1.eMeshCode < rId2.eMeshCode)
		return false;

	if(rId1.ePrefectureCode > rId2.ePrefectureCode)
		return true;
	else if(rId1.ePrefectureCode < rId2.ePrefectureCode)
		return false;

	if(rId1.eStationCode > rId2.eStationCode)
		return true;
	else if(rId1.eStationCode < rId2.eStationCode)
		return false;

	return rId1.eCrossId > rId2.eCrossId;
}

bool operator == (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2)
{
	return 
		rId1.eMeshCode == rId2.eMeshCode && 
		rId1.ePrefectureCode == rId2.ePrefectureCode &&
		rId1.eStationCode == rId2.eStationCode &&
		rId1.eCrossId == rId2.eCrossId;
}

bool operator < (const KKDIntersectionIdPair& rPair1, const KKDIntersectionIdPair& rPair2)
{
	if(rPair1.eIntersection1Id < rPair2.eIntersection1Id)
		return true;
	
	if(rPair1.eIntersection1Id > rPair2.eIntersection1Id)
		return false;

	return rPair1.eIntersection2Id < rPair2.eIntersection2Id;
}
