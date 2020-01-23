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
#include "ShapeFileManager.h"

using namespace sindy;

namespace {
	CComBSTR kShape = "shape";
}


bool ShapeFileManager::OpenWorkspace(const CString& workspaceName)
{
	m_outWorkspace = create_workspace(workspaceName);
	return m_outWorkspace ? true : false;
}

IFeatureClassPtr ShapeFileManager::CreateFeatureClass(
	const CString& fileName, IFieldsEditPtr& fields)
{
	IFeatureClassPtr featureClass;
	HRESULT hr = ((IFeatureWorkspacePtr)m_outWorkspace)->CreateFeatureClass(
		CComBSTR(fileName), fields, 0, 0, esriFTSimple, kShape,
		CComBSTR(_T("")), &featureClass);

	if( FAILED(hr) )
		return nullptr;

	return featureClass;
}

ITablePtr ShapeFileManager::CreateTable(
	const CString& fileName, IFieldsEditPtr& fields)
{
	ITablePtr table;
	HRESULT hr = ((IFeatureWorkspacePtr)m_outWorkspace)->CreateTable(
		CComBSTR(fileName), fields, 0, 0, CComBSTR(_T("")), &table );

	if( FAILED(hr) )
		return nullptr;

	return table;
}

bool ShapeFileManager::CreateShapeField(
	IFieldsEditPtr& fields, const ISpatialReferencePtr& spatialRef)
{
	// 形状フィールドの作成
	IFieldEditPtr shapeField(CLSID_Field);
	if( FAILED(shapeField->put_Name(kShape)) )
		return false;

	if( FAILED(shapeField->put_Type(esriFieldTypeGeometry)) )
		return false;

	IGeometryDefEditPtr geometryDefEdit(CLSID_GeometryDef);
	if( FAILED(geometryDefEdit->putref_SpatialReference(spatialRef)) )
		return false;

	if( FAILED(geometryDefEdit->put_GeometryType(esriGeometryPolygon)) )
		return false;

	if( FAILED(shapeField->putref_GeometryDef(geometryDefEdit)) )
		return false;

	if( FAILED(fields->AddField(shapeField)) )
		return false;

	return true;
}
