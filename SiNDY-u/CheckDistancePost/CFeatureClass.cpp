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
#include "CFeatureClass.h"
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/AheGlobals.h>
#include <sindy/workspace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy
{
	// 空間参照取得
	ISpatialReferencePtr CFeatureClass::getSpRef()
	{
		ISpatialReferencePtr ipSpRef;
		IGeoDatasetPtr ipGeoDataset = m_ipFeatureClass;

		if ( ipGeoDataset )
			atl2::valid( ipGeoDataset->get_SpatialReference(&ipSpRef) );

		return ipSpRef;
	}

	// 形状フィールド名取得
	CString CFeatureClass::getShapeFieldName() const
	{
		CComBSTR bstrShape;

		if ( m_ipFeatureClass )
			atl2::valid( m_ipFeatureClass->get_ShapeFieldName(&bstrShape) );

		return (CString)bstrShape;
	}

	// OBJECTIDフィールド名取得
	CString CFeatureClass::getOIDFieldName() const
	{
		CComBSTR bstrOID;

		if ( m_ipFeatureClass )
			atl2::valid( m_ipFeatureClass->get_OIDFieldName(&bstrOID) );

		return (CString)bstrOID;
	}

	// フィーチャ検索
	CFeatureClass::Container CFeatureClass::search(const IQueryFilterPtr& ipFilter)
	{
		if ( !m_ipFeatureClass )
			return m_mapContainer;

		IFeatureCursorPtr ipCursor;
		atl2::valid(m_ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor));

		if ( ! ipCursor )
			return m_mapContainer;

		IFeaturePtr ipFeature;
		while(S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature )
		{
			long nOID = 0;
			atl2::valid(ipFeature->get_OID(&nOID));

			m_mapContainer[nOID].reset( new CRowBase(ipFeature) );
		}

		return m_mapContainer;
	}

	// フィーチャ検索
	CFeatureClass::Container CFeatureClass::searchNoContain(const IQueryFilterPtr& ipFilter)
	{
		CFeatureClass::Container mapContainer;

		if ( ! m_ipFeatureClass )
			return mapContainer;

		IFeatureCursorPtr ipCursor;
		atl2::valid(m_ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor));

		if ( ! ipCursor )
			return mapContainer;

		IFeaturePtr ipFeature;
		while(S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature )
		{
			long nOID = 0;
			atl2::valid(ipFeature->get_OID(&nOID));

			mapContainer[nOID].reset( new CRowBase(ipFeature) );
		}

		return mapContainer;
	}

	// フィーチャ取得
	CSPRowBase CFeatureClass::GetFeature(long nOID)
	{
		if ( ! m_ipFeatureClass )
			return CSPRowBase();

		if ( m_mapContainer.end() == m_mapContainer.find( nOID ) )
		{
			IFeaturePtr ipFeature;
			atl2::valid( m_ipFeatureClass->GetFeature(nOID, &ipFeature) );

			if ( ipFeature )
				m_mapContainer[nOID].reset( new CRowBase(ipFeature) );
		}

		return m_mapContainer[nOID];
	}

	// フィールド情報作成
	void CFeatureClass::CreateFieldMap()
	{
		if (!m_ipFeatureClass || m_spFieldMap )
			return;

		IFieldsPtr ipFields;

		atl2::valid( m_ipFeatureClass->get_Fields( &ipFields ) );
		m_spFieldMap.reset( new CFieldMap( ipFields ) );
	}

	// フィールド情報取得
	CSPFieldMap CFeatureClass::GetFieldMap() const
	{
		return m_spFieldMap;
	}

	// コンテナクリア
	void CFeatureClass::Clear()
	{
		m_mapContainer.clear();
	}
}
