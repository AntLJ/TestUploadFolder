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
 *	GDBQuery.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		V‹Kì¬
 */

#include "GDB.h"

static HRESULT GDBCreateQueryFilter(const std::string &cWhereClause, IQueryFilter **ipQueryFilter)
{
	HRESULT hr;
	const char *func = "GDBCreateQueryFilter";

	*ipQueryFilter = NULL;

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	ATLASSERT(ipFilter != NULL);

	if (FAILED(hr = ipFilter->put_WhereClause(CComBSTR(_T(cWhereClause.c_str())))))
		return GDBERROR(hr, func);

	*ipQueryFilter = ipFilter;
	(*ipQueryFilter)->AddRef();

	return S_OK;
}

HRESULT GDBSearch(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter, IFeatureCursor **ipFeatureCursor)
{
	HRESULT hr;
	const char *func = "GDBSearch";

	*ipFeatureCursor = NULL;

	if (FAILED(hr = ipFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, ipFeatureCursor)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBSearch(ITablePtr ipTable, IQueryFilterPtr ipQueryFilter, _ICursor **ipCursor)
{
	HRESULT hr;
	const char *func = "GDBSearch";

	*ipCursor = NULL;

	if (FAILED(hr = ipTable->Search(ipQueryFilter, VARIANT_FALSE, ipCursor)))
		return GDBERROR(hr, func);

	return S_OK;
}
/*
HRESULT GDBSelect(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter, ISelectionSet **ipSelectionSet)
{
	HRESULT hr;
	const char *func = "GDBSelect";

	*ipSelectionSet = NULL;

	IScratchWorkspaceFactoryPtr ipScratchWorkspaceFactory(CLSID_ScratchWorkspaceFactory);
	ATLASSERT(ipScratchWorkspaceFactory != NULL);
	IWorkspacePtr ipWorkspace = NULL;
	if (FAILED(hr = ipScratchWorkspaceFactory->get_DefaultScratchWorkspace(&ipWorkspace))) return GDBERROR(hr);

	if (FAILED(hr = ipFeatureClass->Select(ipQueryFilter, esriSelectionTypeSnapshot, esriSelectionOptionNormal, ipWorkspace, ipSelectionSet)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBSelect(ITablePtr ipTable, IQueryFilterPtr ipQueryFilter, ISelectionSet **ipSelectionSet)
{
	HRESULT hr;
	const char *func = "GDBSelect";

	*ipSelectionSet = NULL;

	IScratchWorkspaceFactoryPtr ipScratchWorkspaceFactory(CLSID_ScratchWorkspaceFactory);
	ATLASSERT(ipScratchWorkspaceFactory != NULL);
	IWorkspacePtr ipWorkspace = NULL;
	if (FAILED(hr = ipScratchWorkspaceFactory->get_DefaultScratchWorkspace(&ipWorkspace))) return GDBERROR(hr);

	if (FAILED(hr = ipTable->Select(ipQueryFilter, esriSelectionTypeSnapshot, esriSelectionOptionNormal, ipWorkspace, ipSelectionSet)))
		return GDBERROR(hr, func);

	return S_OK;
}
*/

HRESULT GDBCount(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter, long *cCount)
{
	HRESULT hr;
	const char *func = "GDBCount";

	*cCount = 0;

	if (FAILED(hr = ipFeatureClass->FeatureCount(ipQueryFilter, cCount)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBCount(ITablePtr ipTable, IQueryFilterPtr ipQueryFilter, long *cCount)
{
	HRESULT hr;
	const char *func = "GDBCount";

	*cCount = 0;

	if (FAILED(hr = ipTable->RowCount(ipQueryFilter, cCount)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBSearchByWhereClause(IFeatureClassPtr ipFeatureClass, const std::string &cWhereClause, IFeatureCursor **ipFeatureCursor, const std::string& strSubFields)
{
	HRESULT hr;
	const char *func = "GDBSearchByWhereClause";

	*ipFeatureCursor = NULL;

	IQueryFilterPtr ipQueryFilter = NULL;
	if (FAILED(hr = GDBCreateQueryFilter(cWhereClause, &ipQueryFilter)))
		return hr;

	if(!strSubFields.empty())
		ipQueryFilter->put_SubFields(CComBSTR(strSubFields.c_str()));
	return GDBSearch(ipFeatureClass, ipQueryFilter, ipFeatureCursor);

}

HRESULT GDBSearchByWhereClause(ITablePtr ipTable, const std::string &cWhereClause, _ICursor **ipCursor)
{
	HRESULT hr;
	const char *func = "GDBSearchByWhereClause";

	*ipCursor = NULL;

	IQueryFilterPtr ipQueryFilter = NULL;
	if (FAILED(hr = GDBCreateQueryFilter(cWhereClause, &ipQueryFilter)))
		return hr;

	return GDBSearch(ipTable, ipQueryFilter, ipCursor);
}
/*
HRESULT GDBSelectByWhereClause(IFeatureClassPtr ipFeatureClass, const std::string &cWhereClause, ISelectionSet **ipSelectionSet)
{
	HRESULT hr;
	const char *func = "GDBSelectByWhereClause";

	*ipSelectionSet = NULL;

	IQueryFilterPtr ipQueryFilter = NULL;
	if (FAILED(hr = GDBCreateQueryFilter(cWhereClause, &ipQueryFilter)))
		return hr;

	return GDBSelect(ipFeatureClass, ipQueryFilter, ipSelectionSet);
}

HRESULT GDBSelectByWhereClause(ITablePtr ipTable, const std::string &cWhereClause, ISelectionSet **ipSelectionSet)
{
	HRESULT hr;
	const char *func = "GDBSelectByWhereClause";

	*ipSelectionSet = NULL;

	IQueryFilterPtr ipQueryFilter = NULL;
	if (FAILED(hr = GDBCreateQueryFilter(cWhereClause, &ipQueryFilter)))
		return hr;

	return GDBSelect(ipTable, ipQueryFilter, ipSelectionSet);
}
*/

HRESULT GDBCountByWhereClause(IFeatureClassPtr ipFeatureClass, const std::string &cWhereClause, long *cCount)
{
	HRESULT hr;
	const char *func = "GDBCountByWhereClause";

	*cCount = 0;

	IQueryFilterPtr ipQueryFilter = NULL;
	if (FAILED(hr = GDBCreateQueryFilter(cWhereClause, &ipQueryFilter)))
		return hr;

	return GDBCount(ipFeatureClass, ipQueryFilter, cCount);
}

HRESULT GDBCountByWhereClause(ITablePtr ipTable, const std::string &cWhereClause, long *cCount)
{
	HRESULT hr;
	const char *func = "GDBCountByWhereClause";

	*cCount = 0;

	IQueryFilterPtr ipQueryFilter = NULL;
	if (FAILED(hr = GDBCreateQueryFilter(cWhereClause, &ipQueryFilter)))
		return hr;

	return GDBCount(ipTable, ipQueryFilter, cCount);
}
