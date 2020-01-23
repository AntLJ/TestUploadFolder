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

// SindyLineClass.cpp: SindyLineClass クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyLineClass.h"


SindyLineClass::SindyLineClass(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyFeatureClass(cDBConnection, cName),
mLineList()
{

}

SindyLineClass::~SindyLineClass()
{

}

HRESULT SindyLineClass::LoadByPolygon(IPolygonPtr ipPolygon)
{
	HRESULT hr;
	const char *func = "SindyLineClass::LoadByPolygon";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByGeometry((IFeatureClassPtr)ipDataset, (IGeometryPtr)ipPolygon, esriSpatialRelIntersects, &ipFeatureCursor)))
		return hr;

	IFeaturePtr ipFeature = NULL;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
	{
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry)))
			return GDBERROR(hr, func);

		bool rel;
		if (FAILED(hr = GDBCheckRelation(ipGeometry, ipPolygon, &rel)))
			return hr;
		if (! rel)
			continue;

		long aOID;
		if (FAILED(hr = ipFeature->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipFeature));

		IPointCollectionPtr ipPointCollection(ipGeometry);
		ATLASSERT(ipPointCollection != NULL);
		long aPointCount = 0;
		if (FAILED(hr = ipPointCollection->get_PointCount(&aPointCount)))
			return GDBERROR(hr, func);
		for (long i = 0; i < (aPointCount-1); i++)
		{
			IPointPtr ipFrom = NULL, ipTo = NULL;
			if (FAILED(hr = ipPointCollection->get_Point(i, &ipFrom)))
				return GDBERROR(hr, func);
			if (FAILED(hr = ipPointCollection->get_Point((i+1), &ipTo)))
				return GDBERROR(hr, func);
			SindyLine aLine(ipFrom, ipTo);
			mLineList.insert(GDBLINESET(aLine, ipFeature));
		}
	}

	return S_OK;
}

HRESULT SindyLineClass::SearchByLine(IPointPtr ipFrom, IPointPtr ipTo, GDBLINERANGE *cRange)
{
	SindyLine aKey(ipFrom, ipTo);
	*cRange = mLineList.equal_range(aKey);
	return S_OK;
}
