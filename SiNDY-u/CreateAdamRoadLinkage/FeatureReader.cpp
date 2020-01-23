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
#include "FeatureReader.h"
#include "Util.h"

// ArcHelperEx
#include <GlobalFunctions.h>

// useful_headers
#include <TDC/useful_headers/str_util.h>

using namespace Util;

bool FeatureReader::read( const CString& where_clause)
{
	IFeatureCursorPtr cur;
	if(FAILED( m_FeatureClass->Search( AheInitQueryFilter(nullptr, nullptr, where_clause), VARIANT_FALSE, &cur)))
		return false;

	m_Features.clear();

	IFeaturePtr feature;
	while( cur->NextFeature(&feature) == S_OK && feature ) {
		m_Features.push_back(feature);
	}

	return true;
}

bool FeatureReader::read(long basemesh_code)
{
	using namespace uh;
	using namespace sindy::schema::category::mesh_code_table;

	if( ! m_BaseMeshClass )
		return false;

	// 指定ベースメッシュのジオメトリを取得
	IGeometryPtr meshGeo = getBasemeshShape(basemesh_code);
	if(! meshGeo)
		return false;

	// 指定ベースメッシュ内のフィーチャを取得
	IFeatureCursorPtr cur;
	if(FAILED( m_FeatureClass->Search( AheInitSpatialFilter( nullptr, meshGeo ), VARIANT_FALSE, &cur )))
		return false;

	m_Features.clear();
	
	IFeaturePtr feature;
	while( cur->NextFeature(&feature) == S_OK && feature ) {
		m_Features.push_back(feature);
	}

	return true;
}

bool FeatureReader::read(const IGeometryPtr& geo, const CString& where_clause)
{
	ISpatialFilterPtr sp_filter = AheInitSpatialFilter( nullptr, geo );
	sp_filter->PutWhereClause(_bstr_t(where_clause));

	IFeatureCursorPtr cur;
	if(FAILED( m_FeatureClass->Search( sp_filter, VARIANT_FALSE, &cur )))
		return false;

	m_Features.clear();
	
	IFeaturePtr feature;
	while( cur->NextFeature(&feature) == S_OK && feature ) {
		m_Features.push_back(feature);
	}

	return true;
}

bool FeatureReader::readAround(const IGeometryPtr& geo, double meter, const CString& where_clause)
{
	IGeometryPtr searchSrcGeo = getBuffer(geo, meter);
	return read(searchSrcGeo, where_clause);
}

bool FeatureReader::read(long basemesh_code, const CString& where_clause)
{
	using namespace uh;
	using namespace sindy::schema::category::mesh_code_table;

	if( ! m_BaseMeshClass )
		return false;

	IGeometryPtr meshGeo = getBasemeshShape(basemesh_code);
	if(! meshGeo)
		return false;

	return read(meshGeo, where_clause);
}

IGeometryPtr FeatureReader::getBasemeshShape( long basemesh_code ) const
{
	using namespace uh;
	using namespace sindy::schema::category::mesh_code_table;

	// 指定ベースメッシュのジオメトリを取得
	CString whereClause = str_util::format( _T("%s=%ld"), kMeshCode, basemesh_code);
	IFeatureCursorPtr mesh_cur;
	if(FAILED( m_BaseMeshClass->Search( AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &mesh_cur)))
		return nullptr;

	IFeaturePtr meshFeature;
	IGeometryPtr meshGeo;
	if( mesh_cur->NextFeature(&meshFeature) == S_OK && meshFeature )
		meshGeo = meshFeature->GetShapeCopy();

	return meshGeo;
}
