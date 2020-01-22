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
#include "CityPolygonCheck.h"

// 初期化
bool CityPolygonCheck::initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster )
{
	printProgress( getMsg( iAC_CheckInit ), getLayerCheckName());

	m_textAddressMaster.setDirPath(textAddressMaster);

	m_errorLog = &errorLog;	
	m_ipCityPolygon =  tableFinder.findTable( DB_ADDRESS, city_polygon::kTableName);
	m_cityPolygonFieldsMap = gf::createFieldMap( m_ipCityPolygon );
	return true;
}

// チエック
void CityPolygonCheck::check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID )
{
	m_curMeshCode = ulMeshCode;
	IFeatureCursorPtr ipCityCursor;
	long geoCount = AheSelectByShapeAndGetCount( ipMeshGeometry, m_ipCityPolygon, &ipCityCursor );
	if(!ipCityCursor || geoCount == 0)
	{
		if( m_errorIdSet.find( ErrorIdInfo( m_curMeshCode, kAC_CityPolygonNoPolygonInMesh ) ) != m_errorIdSet.end() )	return;
		// Error code:0x02004004のチエック(チェック対象の都市地図メッシュにかかる住所ポリゴンを全てマージし、都市地図メッシュ図郭でClipした形状が都市地図メッシュ図郭と異なる場合エラー)
		m_errorLog->write( kAC_CityPolygonNoPolygonInMesh, basemesh::kTableName, meshID, NULL, _T("%d"), m_curMeshCode );
		m_errorIdSet.insert( ErrorIdInfo( m_curMeshCode, kAC_CityPolygonNoPolygonInMesh ) );
		return;
	}

	// 同一属性のポリゴンコンテナ( error code:02003001のチエック )
	map<CString, map<long, IGeometryPtr>>sameCityPolygonMap;

	// Ready for error code:02004003のチエック
	IGeometryCollectionPtr ipGeometryCollection( CLSID_GeometryBag );
	ISpatialReferencePtr	ipSpatialReference;

	IFeaturePtr ipCityFeature;
	// オブジェクトごとの処理
	while( S_OK == ipCityCursor->NextFeature(&ipCityFeature) && ipCityFeature )
	{
		// ジオメトリの取得
		IGeometryPtr ipCityGeometry; 
		if( ( S_OK != ipCityFeature->get_Shape( &ipCityGeometry ) ) || !ipCityGeometry )
		{
			printProgress( getMsg( eFeailGetShape ), ulMeshCode );
			continue;
		}

		// 空間参照の取得
		if( !ipSpatialReference )
		{
			ipCityGeometry->get_SpatialReference( &ipSpatialReference );
			IGeometryPtr(ipGeometryCollection)->putref_SpatialReference( ipSpatialReference );
		}
		ipGeometryCollection->AddGeometry( ipCityGeometry );

		// オブジェクトID
		long oID(0L);
		ipCityFeature->get_OID(&oID);
		// 市区町村コード
		CComVariant cityCode = gf::getValue( ipCityFeature, m_cityPolygonFieldsMap[city_polygon::kCityCode] );
		CString cityCodeStr = ToString( cityCode );

		// 同一属性のポリゴンが隣接しているのチエック( error code:02003001のチエック )
		checkSameCityPolygon( oID, ipCityGeometry, cityCodeStr, sameCityPolygonMap );

		// マスタ情報を通じてのチエック
		checkWithMasterInfo( oID, cityCodeStr );
	}
	// 市区町村ポリゴンに抜けがあるのチエック(Error code:0x02004003のチエック)
	if( !m_relationCheck.checkOutArea(ipMeshGeometry, ipGeometryCollection, ipSpatialReference,  ErrorIdInfo( m_curMeshCode, kAC_CityPolygonCityMissing), this, ErrorIdInfo( m_curMeshCode, kAC_CityPolygonNoPolygonInMesh ), basemesh::kTableName, meshID) ) printProgress( getMsg( eFailMissing ) );

}

// 同一属性のポリゴンが隣接しているのチエック
void CityPolygonCheck::checkSameCityPolygon( long oID, const IGeometryPtr& geo, const CString& cityCodeStr, map<CString, map<long, IGeometryPtr>>& sameCityPolygonMap )
{
	// Error code:0x02003001のチエック(同一市区町村コードのポリゴンが隣接している)
	auto itr = sameCityPolygonMap.find(cityCodeStr);
	if( itr == sameCityPolygonMap.end() )
	{
		map<long, IGeometryPtr>geoMap;
		geoMap[oID] = geo;
		sameCityPolygonMap[cityCodeStr] = geoMap;
	}else
	{
		for( const auto& itrCity:itr->second )
		{
			if( !gf::isTouches( geo, itrCity.second ) ) continue;
			if( m_errorIdSet.find( ErrorIdInfo( itrCity.first, kAC_CityPolygonSameCityAdjoin ) ) != m_errorIdSet.end() ) continue;

			m_errorLog->write( kAC_CityPolygonSameCityAdjoin, getName(), oID, NULL, _T("%d\t%d"), m_curMeshCode, itrCity.first );
			m_errorIdSet.insert( ErrorIdInfo( itrCity.first, kAC_CityPolygonSameCityAdjoin ) );
		}
		itr->second[oID] = geo;
	}
	return;
}

// マスタ情報を通じてのチエック
void CityPolygonCheck::checkWithMasterInfo( long oID, const CString& cityCodeStr )
{
	// 住所マースデータを読み込む
	m_textAddressMaster.getTextAddressMaster( cityCodeStr );
	// Error code:0x02004001のチエック(市区町村コードが住所マスタ上廃止(ステータス=2)になっている)
	if( m_errorIdSet.find( ErrorIdInfo( oID, kAC_CityPolygonAbolishedCode ) ) == m_errorIdSet.end() )
	{
		if( m_textAddressMaster.searchMaster(cityCodeStr, 2) )
		{
			m_errorLog->write( kAC_CityPolygonAbolishedCode, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_CityPolygonAbolishedCode ) );
		}
	}

	// Error code:0x02004002のチエック(市区町村コードが住所マスタ上に存在しない)
	if( m_errorIdSet.find( ErrorIdInfo( oID, kAC_CityPolygonCityNotInMaster ) ) == m_errorIdSet.end() )
	{
		if( !m_textAddressMaster.searchMaster(cityCodeStr) )
		{
			m_errorLog->write( kAC_CityPolygonCityNotInMaster, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_CityPolygonCityNotInMaster ) );
		}
	}
	return;
}

