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

// SindyHighwayPoint.cpp: SindyHighwayPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyHighwayPoint.h"
#include "SindyRoadNetwork.h"

SindyHighwayPoint::SindyHighwayPoint(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyPointClass(cDBConnection, cName),
mFacilList()
{

}

SindyHighwayPoint::~SindyHighwayPoint()
{

}

HRESULT SindyHighwayPoint::Load(const std::string &cWhereCause)
{
	HRESULT hr;
	const char *func = "SindyHighwayPoint::Load";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((IFeatureClassPtr)ipDataset, cWhereCause, &ipFeatureCursor)))
		return hr;

	IFeaturePtr ipFeature = NULL;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
	{
		long aOID;
		if (FAILED(hr = ipFeature->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipFeature));

		SindyHighwayFacil aFacil;
		if (FAILED(hr = aFacil.SetFacil(ipFeature)))
			return hr;
		mFacilList.insert(GDBHWYFACILSET(aFacil, ipFeature));

		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry)))
			return GDBERROR(hr, func);
		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);
		mPointList.insert(GDBPNTSET(ipPoint, ipFeature));
	}

	return S_OK;
}

HRESULT SindyHighwayPoint::SearchByFacilCode(long cRoadCode, long cRoadSeq, GDBHWYFACILRANGE *cFacilRange)
{
	SindyHighwayFacil aKey(cRoadCode, cRoadSeq);
	*cFacilRange = mFacilList.equal_range(aKey);
	return S_OK;
}
