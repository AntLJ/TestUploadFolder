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
#pragma warning (disable : 4290)
#pragma warning (disable : 4786)
#include "SiNDYTraits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

#if(0)
#define ACCESSOR_GET_0(i, d, f) HRESULT (__stdcall i::*(traits<i>::accessor<d>::get))(d*) = i::f;
#define ACCESSOR_GET_1(i, a1, d, f) HRESULT (__stdcall i::*(traits<i>::accessor<d>::get))(a1, d*) = i::f;
#define ACCESSOR_GET_2(i, a1, a2, d, f) HRESULT (__stdcall i::*(traits<i>::accessor<d>::get))(a1, a2, d*) = i::f;

#define INDEX_CONTAINER(i, o, cf, gf) \
	HRESULT (__stdcall i::*(traits<i>::accessor<o>::count))(long*) = i::cf; \
	HRESULT (__stdcall i::*(traits<i>::accessor<o>::get))(long, o*) = i::gf;


// IFeatureアクセッサポインタ

ACCESSOR_GET_0(IFeature, IGeometry*, get_Shape);

// ITableアクセッサポインタ

ACCESSOR_GET_0(ITable, IFields*, get_Fields);
ACCESSOR_GET_0(ITable, IRowBuffer*, CreateRowBuffer);
ACCESSOR_GET_0(ITable, IIndexes*, get_Indexes);

// IFeatureClassアクセッサポインタ

ACCESSOR_GET_0(IFeatureClass, esriGeometryType, get_ShapeType);
ACCESSOR_GET_0(IFeatureClass, IFields*, get_Fields);
ACCESSOR_GET_0(IFeatureClass, IFeatureBuffer*, CreateFeatureBuffer);
ACCESSOR_GET_0(IFeatureClass, IIndexes*, get_Indexes);

// ICursorアクセッサポインタ

ACCESSOR_GET_0(ICursor, _IRow*, NextRow);
HRESULT (__stdcall ICursor::*(traits<ICursor>::insert))(IRowBuffer*, VARIANT*) = ICursor::InsertRow;

// IFeatureCursorアクセッサポインタ

ACCESSOR_GET_0(IFeatureCursor, IFeature*, NextFeature);
HRESULT (__stdcall IFeatureCursor::*(traits<IFeatureCursor>::insert))(IFeatureBuffer*, VARIANT*) = IFeatureCursor::InsertFeature;

// IDatasetアクセッサポインタ

ACCESSOR_GET_0(IDataset, IWorkspace*, get_Workspace);

// IEnumDatasetアクセッサポインタ

ACCESSOR_GET_0(IEnumDataset, IDataset*, Next);

// IEnumDatasetNameアクセッサポインタ

ACCESSOR_GET_0(IEnumDatasetName, IDatasetName*, Next);

// IFieldアクセッサポインタ

ACCESSOR_GET_0(IField, esriFieldType, get_Type);
ACCESSOR_GET_0(IField, IGeometryDef*, get_GeometryDef);
ACCESSOR_GET_0(IField, IDomain*, get_Domain);

// IFieldsアクセッサポインタ

INDEX_CONTAINER(IFields, IField*, get_FieldCount, get_Field)

// ICodedValueDomainアクセッサポインタ

INDEX_CONTAINER(ICodedValueDomain, VARIANT, get_CodeCount, get_Value)

// IGeometryアクセッサポインタ

ACCESSOR_GET_0(IGeometry, esriGeometryType, get_GeometryType);
ACCESSOR_GET_0(IGeometry, IEnvelope*, get_Envelope);

// IGeometryCollectionアクセッサポインタ

INDEX_CONTAINER(IGeometryCollection, IGeometry*, get_GeometryCount, get_Geometry);

// IPointアクセッサポインタ

ACCESSOR_GET_0(IPoint, ISpatialReference*, get_SpatialReference)

// IPointCollectionアクセッサポインタ

INDEX_CONTAINER(IPointCollection, IPoint*, get_PointCount, get_Point)

// IWorkspaceアクセッサポインタ

ACCESSOR_GET_1(IWorkspace, esriDatasetType, IEnumDataset*, get_Datasets);

// IFeatureWorkspaceアクセッサポインタ

ACCESSOR_GET_1(IFeatureWorkspace, BSTR, ITable*, OpenTable);
ACCESSOR_GET_1(IFeatureWorkspace, BSTR, IFeatureClass*, OpenFeatureClass);

// IWorkspaceDomainsアクセッサポインタ

ACCESSOR_GET_0(IWorkspaceDomains, IEnumDomain*, get_Domains)

// IEnumDomainアクセッサポインタ

ACCESSOR_GET_0(IEnumDomain, IDomain*, Next);

// IMapアクセッサポインタ

INDEX_CONTAINER(IMap, ILayer*, get_LayerCount, get_Layer)
ACCESSOR_GET_2(IMap, IUID*, VARIANT_BOOL, IEnumLayer*, get_Layers);

// ICompositeLayerアクセッサポインタ

INDEX_CONTAINER(ICompositeLayer, ILayer*, get_Count, get_Layer)

// IFeatureLayerアクセッサポインタ

ACCESSOR_GET_0(IFeatureLayer, IFeatureClass*, get_FeatureClass);

// IEnumLayerアクセッサポインタ

ACCESSOR_GET_0(IEnumLayer, ILayer*, Next);

// IIndexesアクセッサポインタ

INDEX_CONTAINER(IIndexes, IIndex*, get_IndexCount, get_Index)

// ICloneアクセッサポインタ

ACCESSOR_GET_0(IClone, IClone*, Clone);
#endif
