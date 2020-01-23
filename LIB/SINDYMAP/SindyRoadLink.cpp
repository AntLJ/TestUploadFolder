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

// SindyRoadLink.cpp: SindyRoadLink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyRoadLink.h"
#include "SindyRoadNetwork.h"

SindyRoadLink::SindyRoadLink(SindyDBConnection *cDBConnection) :
SindyFeatureClass(cDBConnection, sindy::layer::ROAD_LINK),
mFrom(), mTo()
{
}

SindyRoadLink::SindyRoadLink(SindyDBConnection *cDBConnection, const std::string& subFields) :
SindyFeatureClass(cDBConnection, sindy::layer::ROAD_LINK),
mFrom(), mTo(), mSubFields(subFields)
{
}

SindyRoadLink::~SindyRoadLink()
{

}

HRESULT SindyRoadLink::LoadByPolygon(IPolygonPtr ipPolygon)
{
	HRESULT hr;
	const char *func = "SindyRoadLink::LoadByPolygon";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	// Arc9.3.1対応
	// ISelectionSetは使用しないように修正
	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByGeometry(((IFeatureClassPtr)ipDataset), (IGeometryPtr)ipPolygon, esriSpatialRelIntersects, &ipFeatureCursor, mSubFields)))
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

		CComVariant vaFromID, vaToID;
		if (FAILED(hr = GDBGetValue(ipFeature, sindy::road_link::FROM_NODE_ID, &vaFromID)))
			return hr;
		if (FAILED(hr = GDBGetValue(ipFeature, sindy::road_link::TO_NODE_ID, &vaToID)))
			return hr;
		vaFromID.ChangeType(VT_I4); vaToID.ChangeType(VT_I4);

		mFrom.insert(GDBOBJSET(vaFromID.lVal, ipFeature));
		mTo.insert(GDBOBJSET(vaToID.lVal, ipFeature));
	}

	return S_OK;
}

static HRESULT FindLink(long cNodeID, GDB2OBJLIST *cList, GDBOBJLIST *cLinkList)
{
	HRESULT hr;
	const char *func = "FindLink";

	GDBOBJRANGE aRange = cList->equal_range(cNodeID);
	GDB2OBJITER aIter = aRange.first;
	for (; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipLink = (IFeaturePtr)aIter->second;
		long aOID;
		if (FAILED(hr = ipLink->get_OID(&aOID)))
			return GDBERROR(hr, func);
		cLinkList->insert(GDBOBJSET(aOID, ipLink));
	}
	return S_OK;
}

HRESULT SindyRoadLink::SearchByNodeID(long cNodeID, long cRelation, GDBOBJLIST *cLinkList)
{
	HRESULT hr;
	const char *func = "SindyRoadLink::SearchByNodeID";

	cLinkList->clear();

	if (cRelation & sindy::rnw::relation::from)
	{
		if (FAILED(hr = FindLink(cNodeID, &mFrom, cLinkList)))
			return hr;
	}
	if (cRelation & sindy::rnw::relation::to)
	{
		if (FAILED(hr = FindLink(cNodeID, &mTo, cLinkList)))
			return hr;
	}

	return S_OK;
}

HRESULT SindyRoadLink::SearchLink(long cFromNodeID, long cToNodeID, IFeature **ipLink)
{
	HRESULT hr;
	const char *func = "SindyRoadLink::SearchLink";

	*ipLink = NULL;

	GDBOBJLIST aFromList, aToList;
	if (FAILED(hr = SearchByNodeID(cFromNodeID, sindy::rnw::relation::from, &aFromList)))
		return hr;
	if (FAILED(hr = SearchByNodeID(cToNodeID, sindy::rnw::relation::to, &aToList)))
		return hr;
	long aCount = 0;
	for (GDBOBJITER aIter1 = aFromList.begin(); aIter1 != aFromList.end(); aIter1++)
	{
		IFeaturePtr ipLink1 = (IFeaturePtr)aIter1->second;
		for (GDBOBJITER aIter2 = aToList.begin(); aIter2 != aToList.end(); aIter2++)
		{
			IFeaturePtr ipLink2 = (IFeaturePtr)aIter2->second;
			if (ipLink1 == ipLink2)
			{
				(*ipLink) = ipLink1;
				(*ipLink)->AddRef();
				aCount++;
			}
		}
	}

	// ２本以上検出された場合はエラー
	if (aCount > 1)
		GDBERROR2(E_UNEXPECTED, func);

	return S_OK;
}

HRESULT SindyRoadLink::SearchByGeometry(IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor)
{
	HRESULT hr;
	const char *func = "SindyRoadLink::SearchByGeometry";

	*ipFeatureCursor = nullptr;
	ISpatialFilterPtr ipSpatialFilter;
	if (FAILED(hr = GDBCreateSpatialFilter((IFeatureClassPtr)ipDataset, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;

	if(!mSubFields.empty())
		ipSpatialFilter->put_SubFields(CComBSTR(mSubFields.c_str()));

	if (FAILED(hr = ((IFeatureClassPtr)ipDataset)->Search(ipSpatialFilter, VARIANT_FALSE, (IFeatureCursor **)ipFeatureCursor)))
		return GDBERROR(hr, func);

	return S_OK;
}
