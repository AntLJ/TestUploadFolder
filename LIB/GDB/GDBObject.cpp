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
 *	GDBObject.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		V‹Kì¬
 */

#include "GDB.h"

HRESULT GDBGetValue(_IRowPtr ipRow, const std::string &cColumn, VARIANT *vaVal)
{
	HRESULT hr;
	const char *func = "GDBGetValue";

	IFieldsPtr ipFields = NULL;
	if (FAILED(hr = ipRow->get_Fields(&ipFields)))
		return GDBERROR(hr, func);
	long aIndex;
	if (FAILED(hr = ipFields->FindField(CComBSTR(_T(cColumn.c_str())), &aIndex)))
		return GDBERROR(hr, func);
	if (aIndex < 0)
		return GDBERROR2(E_NOINTERFACE, func);
	if (FAILED(hr = ipRow->get_Value(aIndex, vaVal)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBGetValue(IFeaturePtr ipFeature, const std::string &cColumn, VARIANT *vaVal)
{
	return GDBGetValue((_IRowPtr)ipFeature, cColumn, vaVal);
}

HRESULT GDBPutValue(_IRowPtr ipRow, const std::string &cColumn, VARIANT vaVal)
{
	HRESULT hr;
	const char *func = "GDBPutValue";

	IFieldsPtr ipFields = NULL;
	if (FAILED(hr = ipRow->get_Fields(&ipFields)))
		return GDBERROR(hr, func);
	long aIndex;
	if (FAILED(hr = ipFields->FindField(CComBSTR(_T(cColumn.c_str())), &aIndex)))
		return GDBERROR(hr, func);
	if (aIndex < 0)
		return GDBERROR2(E_NOINTERFACE, func);
	if (FAILED(hr = ipRow->put_Value(aIndex, vaVal)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipRow->Store()))
		return GDBERROR(hr, func);
	return S_OK;
}

HRESULT GDBPutValue(IFeaturePtr ipFeature, const std::string &cColumn, VARIANT vaVal)
{
	return GDBPutValue((_IRowPtr)ipFeature, cColumn, vaVal);
}

HRESULT GDBGetTableName(_IRowPtr ipRow, std::string *cName)
{
	HRESULT hr;
	const char *func = "GDBGetTableName";

	ITablePtr ipTable = NULL;
	if (FAILED(hr = ipRow->get_Table(&ipTable)))
		return GDBERROR(hr, func);

	IObjectClassPtr ipObjectClass(ipTable);
	ATLASSERT(ipObjectClass != NULL);

	CComBSTR bstrName;
	if (FAILED(hr = ipObjectClass->get_AliasName(&bstrName)))
		return GDBERROR(hr, func);

	USES_CONVERSION;
	*cName = OLE2T(bstrName);

	return S_OK;
}

HRESULT GDBGetTableName(IFeaturePtr ipFeature, std::string *cName)
{
	return GDBGetTableName((_IRowPtr)ipFeature, cName);
}
