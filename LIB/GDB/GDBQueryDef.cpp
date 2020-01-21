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
 *	GDBQueryDef.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/10/09		V‹Kì¬
 */

#include "GDB.h"

HRESULT GDBQueryDef(IWorkspacePtr ipWorkspace, const std::string &cTablesName, const std::string &cWhereClause, const std::string &cSubFields, _ICursor **ipCursor)
{
	HRESULT hr;
	const char *func = "GDBQueryDef";

	*ipCursor = NULL;

	IQueryDefPtr ipQueryDef = NULL;
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	ATLASSERT(ipFeatureWorkspace != NULL);

	if (FAILED(hr = ipFeatureWorkspace->CreateQueryDef(&ipQueryDef))) return GDBERROR(hr, func);
	if (FAILED(hr = ipQueryDef->put_Tables(CComBSTR(_T(cTablesName.c_str()))))) return GDBERROR(hr, func);
	if (FAILED(hr = ipQueryDef->put_SubFields(CComBSTR(_T(cSubFields.c_str()))))) return GDBERROR(hr, func);
	if (FAILED(hr = ipQueryDef->put_WhereClause(CComBSTR(_T(cWhereClause.c_str()))))) return GDBERROR(hr, func);
	if (FAILED(hr = ipQueryDef->Evaluate(ipCursor))) return hr;

	return S_OK;
}
