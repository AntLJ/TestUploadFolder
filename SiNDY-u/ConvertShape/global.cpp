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

#pragma once

#include "global.h"

#include <WinLib/VersionInfo.h>

FILE* g_pFile_Runlog = NULL;
FILE* g_pFile_Errlog = NULL;

bool CreateMeshList(IFeatureClassPtr ipMesh, MESHLIST& setMeshlist)
{
	if( !ipMesh )
		return false;

	long lidx_mesh = 0;
	ipMesh->FindField( CComBSTR( sindy::schema::basemesh::kMeshCode ), &lidx_mesh );

	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	CString strWhere;
	strWhere.Format( _T("%s<>0 ORDER BY %s"), sindy::schema::basemesh::kMeshCode, sindy::schema::basemesh::kMeshCode );
	ipFilter->put_WhereClause( CComBSTR( strWhere ) );

	IFeatureCursorPtr ipCursor;
	if( FAILED( ipMesh->Search( ipFilter, VARIANT_FALSE, &ipCursor ) ) )
	{
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetCursor, _T(""), ipMesh);
		return false;
	}
	IFeaturePtr ipFeature;
	while( SUCCEEDED( ipCursor->NextFeature( &ipFeature ) ) && ipFeature )
	{
		CComVariant cVal_mesh;
		if(S_OK != ipFeature->get_Value(lidx_mesh, &cVal_mesh))
		{
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetAttr, _T(""), ipFeature);
			return false;
		}
		if( cVal_mesh.vt != VT_NULL )
		{
			setMeshlist.insert( cVal_mesh.lVal );
		}
	}
	return true;
}

bool GetFieldIndex(ITablePtr ipTable, std::map<long,CString>& mapFieldIndex)
{
	mapFieldIndex.clear();

	IFieldsPtr ipFields;
	ipTable->get_Fields(&ipFields);
	if(!ipFields)
		return false;

	long lCount = -1;
	ipFields->get_FieldCount(&lCount);
	if(lCount < 0)
		return false;

	for(long l = 0; l < lCount; ++l){
		IFieldPtr ipField;
		ipFields->get_Field(l, &ipField);
		if(!ipField)
			return false;

		CComBSTR bstrName;
		ipField->get_Name(&bstrName);

		mapFieldIndex.insert(std::make_pair(l,CString(bstrName)));
	}
	return true;
}

bool GetFieldNameIndex(ITablePtr ipTable, std::map<CString,long>& mapFieldNameIndex)
{
	mapFieldNameIndex.clear();

	IFieldsPtr ipFields;
	ipTable->get_Fields(&ipFields);
	if(!ipFields)
		return false;

	long lCount = -1;
	ipFields->get_FieldCount(&lCount);
	if(lCount < 0)
		return false;

	for(long l = 0; l < lCount; ++l){
		IFieldPtr ipField;
		ipFields->get_Field(l, &ipField);
		if(!ipField)
			return false;

		CComBSTR bstrName;
		ipField->get_Name(&bstrName);

		mapFieldNameIndex.insert(std::make_pair(CString(bstrName), l));
	}
	return true;
}

bool GetFieldTypes(ITablePtr ipTable, std::map<CString,esriFieldType>& mapFieldType)
{
	mapFieldType.clear();

	IFieldsPtr ipFields;
	ipTable->get_Fields(&ipFields);
	if(!ipFields)
		return false;

	long lCount = -1;
	ipFields->get_FieldCount(&lCount);
	if(lCount < 0)
		return false;

	for(long l = 0; l < lCount; ++l){
		IFieldPtr ipField;
		ipFields->get_Field(l, &ipField);
		if(!ipField)
			return false;

		CComBSTR bstrName;
		ipField->get_Name(&bstrName);

		esriFieldType eType;
		ipField->get_Type(&eType);

		mapFieldType.insert(std::make_pair(CString(bstrName), eType));
	}
	return true;
}

bool IsMatch(const CString& strFilter, const CString& strCheck)
{
	CString strChk = strCheck.SpanIncluding(strFilter);
	if(0 == strChk.Compare(strCheck))
		return true;
	else
		return false;
}

bool IsNumeric(const CString& str)
{
	if(!IsMatch(_T("0123456789."), str))
		return false;

	CStringTokenizer cToken(str, _T("."), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();
	if(1 != lTokenCount && 2 != lTokenCount)
		return false;
	
	return true;
}
bool IsInteger(const CString& str)
{
	if(!IsMatch(_T("0123456789"), str))
		return false;

	CStringTokenizer cToken(str, _T("."), TOKEN_RET_EMPTY_ALL);
	size_t lTokenCount = cToken.CountTokens();
	if(1 != lTokenCount && 2 != lTokenCount)
		return false;
	
	return true;
}
