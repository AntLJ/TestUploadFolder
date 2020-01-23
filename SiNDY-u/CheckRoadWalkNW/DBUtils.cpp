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

#include "DBUtils.h"

bool CDBUtils::createFieldIndex(const ITablePtr ipTable, FIELD_INDEX& fieldIndex)
{
	IFieldsPtr ipFields;
	if(FAILED(ipTable->get_Fields(&ipFields)))
		return false;
	
	long count;
	if(FAILED(ipFields->get_FieldCount(&count)) || count == 0)
		return false;
	
	for(long i = 0 ; i < count ; ++i)
	{
		IFieldPtr ipField;
		if(FAILED( ipFields->get_Field(i, &ipField)))
			return false;
		CComBSTR fieldName;
		if(FAILED( ipField->get_Name(&fieldName)))
			return false;
		fieldIndex.insert(std::pair<CString, long>(CString(fieldName).MakeUpper(), i));
	}
	return true;
}

bool CDBUtils::createFeatureMap(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, const ISpatialFilterPtr ipSpatialFilter, MAP_FEATURE& features)
{
	IFeatureCursorPtr ipFeatureCursor;
	if(FAILED(ipFeatureClass->Search(ipSpatialFilter, VARIANT_FALSE, &ipFeatureCursor)))
		return false;

	IFeaturePtr ipFeature;
	while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
	{
		long obejctID = 0;
		CComVariant objectIDValue;
		if( FAILED(ipFeature->get_Value(fieldIndex.find(_T("OBJECTID"))->second, &objectIDValue)))
			return false;
		objectIDValue.ChangeType(VT_I4);
		long objectid = objectIDValue.lVal;

		features.insert(std::pair<long, CAdapt<IFeaturePtr>>(objectid, ipFeature));
	}
	return true;
}

bool CDBUtils::createRowMap(const ITablePtr ipTable, FIELD_INDEX fieldIndex, const IQueryFilterPtr ipQueryFilter, MAP_ROW& rows)
{
	_ICursorPtr ipCursor;
	if (FAILED(ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor)))
		return false;

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK && ipRow )
	{
		long obejctID = 0;
		CComVariant objectIDValue;
		if( FAILED(ipRow->get_Value(fieldIndex.find(_T("OBJECTID"))->second, &objectIDValue)))
			return false;
		objectIDValue.ChangeType(VT_I4);
		long objectid = objectIDValue.lVal;

		rows.insert(std::pair<long, CAdapt<_IRowPtr>>(objectid, ipRow));
	}
	return true;
}
