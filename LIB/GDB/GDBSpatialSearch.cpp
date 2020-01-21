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
 *	GDBSpatialSearch.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		V‹Kì¬
 */

#include "GDB.h"

HRESULT GDBCreateSpatialFilter(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, ISpatialFilter **ipSpatialFilter)
{
	HRESULT hr;
	const char *func = "GDBCreateSpatialFilter";

	*ipSpatialFilter = NULL;

	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	ATLASSERT(ipFilter != NULL);

	CComBSTR bstrName;
	if (FAILED(hr = ipFeatureClass->get_ShapeFieldName(&bstrName)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipFilter->put_GeometryField(bstrName)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipFilter->putref_Geometry(ipGeometry)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipFilter->put_SpatialRel(cSpatRel)))
		return GDBERROR(hr, func);

	*ipSpatialFilter = ipFilter;
	(*ipSpatialFilter)->AddRef();

	return S_OK;
}

HRESULT GDBSearchByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor, const std::string& strSubFields)
{
	HRESULT hr;
	const char *func = "GDBSearchByGeometry";

	*ipFeatureCursor = NULL;

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter(ipFeatureClass, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;

	if(!strSubFields.empty())
		ipSpatialFilter->put_SubFields(CComBSTR(strSubFields.c_str()));

	return GDBSearch(ipFeatureClass, ipSpatialFilter, ipFeatureCursor);
}
/*
HRESULT GDBSelectByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, ISelectionSet **ipSelectionSet)
{
	HRESULT hr;
	const char *func = "GDBSelectByGeometry";

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter(ipFeatureClass, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;

	return GDBSelect(ipFeatureClass, ipSpatialFilter, ipSelectionSet);
}
*/
HRESULT GDBCountByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, long *cCount)
{
	HRESULT hr;
	const char *func = "GDBCountByGeometry";

	*cCount = 0;

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter(ipFeatureClass, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;

	return GDBCount(ipFeatureClass, ipSpatialFilter, cCount);
}

HRESULT GDBSearchByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, IFeatureCursor **ipFeatureCursor)
{
	HRESULT hr;
	const char *func = "GDBSearchByGeometryAndWhereClause";

	*ipFeatureCursor = NULL;

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter(ipFeatureClass, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;
	if (cWhereClause.size())
	{
		if (FAILED(hr = ipSpatialFilter->put_WhereClause(CComBSTR(_T(cWhereClause.c_str())))))
			return GDBERROR(hr, func);
		if (FAILED(hr = ipSpatialFilter->put_SearchOrder(cSearchOrder)))
			return GDBERROR(hr, func);
	}

	return GDBSearch(ipFeatureClass, ipSpatialFilter, ipFeatureCursor);
}
/*
HRESULT GDBSelectByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, ISelectionSet **ipSelectionSet)
{
	HRESULT hr;
	const char *func = "GDBSelectByGeometryAndWhereClause";

	*ipSelectionSet = NULL;

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter(ipFeatureClass, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;
	if (cWhereClause.size())
	{
		if (FAILED(hr = ipSpatialFilter->put_WhereClause(CComBSTR(_T(cWhereClause.c_str())))))
			return GDBERROR(hr, func);
		if (FAILED(hr = ipSpatialFilter->put_SearchOrder(cSearchOrder)))
			return GDBERROR(hr, func);
	}

	return GDBSelect(ipFeatureClass, ipSpatialFilter, ipSelectionSet);
}
*/
HRESULT GDBCountByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, long *cCount)
{
	HRESULT hr;
	const char *func = "GDBSelectByGeometryAndWhereClause";

	*cCount = 0;

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter(ipFeatureClass, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;
	if (cWhereClause.size())
	{
		if (FAILED(hr = ipSpatialFilter->put_WhereClause(CComBSTR(_T(cWhereClause.c_str())))))
			return GDBERROR(hr, func);
		if (FAILED(hr = ipSpatialFilter->put_SearchOrder(cSearchOrder)))
			return GDBERROR(hr, func);
	}

	return GDBCount(ipFeatureClass, ipSpatialFilter, cCount);
}
