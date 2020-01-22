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

// ������
bool CityPolygonCheck::initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster )
{
	printProgress( getMsg( iAC_CheckInit ), getLayerCheckName());

	m_textAddressMaster.setDirPath(textAddressMaster);

	m_errorLog = &errorLog;	
	m_ipCityPolygon =  tableFinder.findTable( DB_ADDRESS, city_polygon::kTableName);
	m_cityPolygonFieldsMap = gf::createFieldMap( m_ipCityPolygon );
	return true;
}

// �`�G�b�N
void CityPolygonCheck::check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID )
{
	m_curMeshCode = ulMeshCode;
	IFeatureCursorPtr ipCityCursor;
	long geoCount = AheSelectByShapeAndGetCount( ipMeshGeometry, m_ipCityPolygon, &ipCityCursor );
	if(!ipCityCursor || geoCount == 0)
	{
		if( m_errorIdSet.find( ErrorIdInfo( m_curMeshCode, kAC_CityPolygonNoPolygonInMesh ) ) != m_errorIdSet.end() )	return;
		// Error code:0x02004004�̃`�G�b�N(�`�F�b�N�Ώۂ̓s�s�n�}���b�V���ɂ�����Z���|���S����S�ă}�[�W���A�s�s�n�}���b�V���}�s��Clip�����`�󂪓s�s�n�}���b�V���}�s�ƈقȂ�ꍇ�G���[)
		m_errorLog->write( kAC_CityPolygonNoPolygonInMesh, basemesh::kTableName, meshID, NULL, _T("%d"), m_curMeshCode );
		m_errorIdSet.insert( ErrorIdInfo( m_curMeshCode, kAC_CityPolygonNoPolygonInMesh ) );
		return;
	}

	// ���ꑮ���̃|���S���R���e�i( error code:02003001�̃`�G�b�N )
	map<CString, map<long, IGeometryPtr>>sameCityPolygonMap;

	// Ready for error code:02004003�̃`�G�b�N
	IGeometryCollectionPtr ipGeometryCollection( CLSID_GeometryBag );
	ISpatialReferencePtr	ipSpatialReference;

	IFeaturePtr ipCityFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while( S_OK == ipCityCursor->NextFeature(&ipCityFeature) && ipCityFeature )
	{
		// �W�I���g���̎擾
		IGeometryPtr ipCityGeometry; 
		if( ( S_OK != ipCityFeature->get_Shape( &ipCityGeometry ) ) || !ipCityGeometry )
		{
			printProgress( getMsg( eFeailGetShape ), ulMeshCode );
			continue;
		}

		// ��ԎQ�Ƃ̎擾
		if( !ipSpatialReference )
		{
			ipCityGeometry->get_SpatialReference( &ipSpatialReference );
			IGeometryPtr(ipGeometryCollection)->putref_SpatialReference( ipSpatialReference );
		}
		ipGeometryCollection->AddGeometry( ipCityGeometry );

		// �I�u�W�F�N�gID
		long oID(0L);
		ipCityFeature->get_OID(&oID);
		// �s�撬���R�[�h
		CComVariant cityCode = gf::getValue( ipCityFeature, m_cityPolygonFieldsMap[city_polygon::kCityCode] );
		CString cityCodeStr = ToString( cityCode );

		// ���ꑮ���̃|���S�����אڂ��Ă���̃`�G�b�N( error code:02003001�̃`�G�b�N )
		checkSameCityPolygon( oID, ipCityGeometry, cityCodeStr, sameCityPolygonMap );

		// �}�X�^����ʂ��Ẵ`�G�b�N
		checkWithMasterInfo( oID, cityCodeStr );
	}
	// �s�撬���|���S���ɔ���������̃`�G�b�N(Error code:0x02004003�̃`�G�b�N)
	if( !m_relationCheck.checkOutArea(ipMeshGeometry, ipGeometryCollection, ipSpatialReference,  ErrorIdInfo( m_curMeshCode, kAC_CityPolygonCityMissing), this, ErrorIdInfo( m_curMeshCode, kAC_CityPolygonNoPolygonInMesh ), basemesh::kTableName, meshID) ) printProgress( getMsg( eFailMissing ) );

}

// ���ꑮ���̃|���S�����אڂ��Ă���̃`�G�b�N
void CityPolygonCheck::checkSameCityPolygon( long oID, const IGeometryPtr& geo, const CString& cityCodeStr, map<CString, map<long, IGeometryPtr>>& sameCityPolygonMap )
{
	// Error code:0x02003001�̃`�G�b�N(����s�撬���R�[�h�̃|���S�����אڂ��Ă���)
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

// �}�X�^����ʂ��Ẵ`�G�b�N
void CityPolygonCheck::checkWithMasterInfo( long oID, const CString& cityCodeStr )
{
	// �Z���}�[�X�f�[�^��ǂݍ���
	m_textAddressMaster.getTextAddressMaster( cityCodeStr );
	// Error code:0x02004001�̃`�G�b�N(�s�撬���R�[�h���Z���}�X�^��p�~(�X�e�[�^�X=2)�ɂȂ��Ă���)
	if( m_errorIdSet.find( ErrorIdInfo( oID, kAC_CityPolygonAbolishedCode ) ) == m_errorIdSet.end() )
	{
		if( m_textAddressMaster.searchMaster(cityCodeStr, 2) )
		{
			m_errorLog->write( kAC_CityPolygonAbolishedCode, getName(), oID, NULL, _T("%d\t%s"), m_curMeshCode, cityCodeStr );
			m_errorIdSet.insert( ErrorIdInfo( oID, kAC_CityPolygonAbolishedCode ) );
		}
	}

	// Error code:0x02004002�̃`�G�b�N(�s�撬���R�[�h���Z���}�X�^��ɑ��݂��Ȃ�)
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

