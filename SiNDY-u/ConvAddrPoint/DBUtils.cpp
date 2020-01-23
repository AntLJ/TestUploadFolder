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

#include "stdafx.h"
#include <sindy/workspace.h>
#include <sindy/libschema.h>
#include "DBUtils.h"

CDBUtils::CDBUtils(void)
{
}


CDBUtils::~CDBUtils(void)
{
}

bool CDBUtils::initDB(const CString& dbConnection, IWorkspacePtr& ipWorkspace)
{
	ipWorkspace = sindy::create_workspace(dbConnection);
	if (!ipWorkspace)
		return false;

	return true;
}

bool CDBUtils::openFeatureClass(const IWorkspacePtr& ipWorkspace, const CString& featureClassName, IFeatureClassPtr& ipFeatureClass, FIELD_INDEX& fieldIndex)
{
	if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(featureClassName), &ipFeatureClass)))
		return false;

	if (!createFieldIndex((ITablePtr)ipFeatureClass, fieldIndex))
		return false;

	return true;
}

bool CDBUtils::openTable(const IWorkspacePtr& ipWorkspace, const CString& tabelName, ITablePtr& ipTable, FIELD_INDEX& fieldIndex)
{
	if (FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenTable(CComBSTR(tabelName), &ipTable)))
		return false;

	if (!createFieldIndex(ipTable, fieldIndex))
		return false;

	return true;
}

bool CDBUtils::createFieldIndex(const ITablePtr& ipTable, FIELD_INDEX& fieldIndex)
{
	IFieldsPtr ipFields;
	if (FAILED(ipTable->get_Fields(&ipFields)))
		return false;

	long count = 0;
	if (FAILED(ipFields->get_FieldCount(&count)) || count == 0)
		return false;

	for (long i = 0; i < count; ++i)
	{
		IFieldPtr ipField;
		if (FAILED(ipFields->get_Field(i, &ipField)))
			return false;
		CComBSTR fieldName;
		if (FAILED(ipField->get_Name(&fieldName)))
			return false;
		fieldIndex.insert(std::pair<CString, long>(CString(fieldName).MakeUpper(), i));
	}

	return true;
}

bool CDBUtils::getRowCount(const ITablePtr& ipTable, const CString& strWhere, long& lCount)
{
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (!strWhere.IsEmpty()) {
		if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(strWhere))))
			return false;
	}

	if (FAILED(ipTable->RowCount(ipQueryFilter, &lCount))) {
		return false;
	}

	return true;
}

bool CDBUtils::getCursor(const ITablePtr& ipTable, const CString& strWhere, const CString& strSort, _ICursorPtr& ipCursor)
{
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if (!strWhere.IsEmpty()) {
		if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(strWhere))))
			return false;
	}
	
	_ICursorPtr ipCursorWork;
	if (FAILED(ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursorWork))) {
		return false;
	}

	if (!strSort.IsEmpty()) {
		ITableSortPtr ipTableSort(CLSID_TableSort);
		if (FAILED(ipTableSort->putref_Table(ipTable)))
			return false;
		if (FAILED(ipTableSort->putref_Cursor(ipCursorWork)))
			return false;
		if (FAILED(ipTableSort->put_Fields(CComBSTR(strSort))))
			return false;
		if (FAILED(ipTableSort->Sort(NULL)))
			return false;
		if (FAILED(ipTableSort->get_Rows(&ipCursor)))
			return false;
	}
	else
		ipCursor = ipCursorWork;

	return true;
}

bool CDBUtils::getFieldIndex(const FIELD_INDEX& fieldIndex, const CString& strFieldName, long& index)
{
	CString strFieldNameUpper(strFieldName);
	strFieldNameUpper.MakeUpper();
	FIELD_INDEX::const_iterator it = fieldIndex.find(strFieldNameUpper);
	if (it == fieldIndex.end())
		return false;

	index = it->second;
	return true;
}

bool CDBUtils::getValue(const _IRowPtr& ipRow, const CString& strFieldName, const long& index, CString& strVal)
{
	CComVariant vaVal;
	if (FAILED(ipRow->get_Value(index, &vaVal)))
		return false;

	if (vaVal.vt != VT_NULL) {
		vaVal.ChangeType(VT_BSTR);
		strVal = vaVal.bstrVal;
	}
	return true;
}
