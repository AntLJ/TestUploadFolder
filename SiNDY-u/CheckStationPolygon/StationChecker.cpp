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

#include "StationChecker.h"
#include "StationLayersHelper.h"

// WinLib
#include <type_convert.h>

// sindy::schema
#include <sindy/schema.h>

// ArcHelperEx,WinLib
#include <AheLayerFunctions.h>
#include <GlobalFunctions.h>

// useful_header
#include <TDC/useful_headers/str_util.h>


using namespace sindy;
using namespace sindy::schema::sj;


// ������
bool StationChecker::init( const CWorkspace& workspace )
{
	// �e�[�u���t�@�C���_�[������
	if( ! initTableFinder( workspace ) )
		return false;

	// �|���S����ʂ̃R�[�h�l�h���C���ǂݍ���
	if( ! readCodeValueDomain() )
		return false;

	// �w�|���S����OID�ǂݍ���
	readStationPolygonOid();

	// �wID�̓ǂݍ���
	readStationIds();

	return true;
}


// �w���R�t���`�F�b�N
bool StationChecker::checkLinkedStation( const CRowContainer& row, const CString& table_name ) const
{
	using namespace rel_station_site;
	using namespace station_layer_helper;

	_ASSERTE( isStationPolygonLayer( table_name ) );

	// �w�|���S���R�t���e�[�u���擾
	auto& table = *m_TableFinder.FindTable( rel_station_site::kTableName );
	table.clear();

	// ���R�[�h���� ( LAYERNO_C=* and LAYER_OID=*  )
	CString where = uh::str_util::format( _T("%s=%d and %s=%d"),
		                                  rel_station_site::kLayerNo_C, getLayerNo( table_name ),
										  rel_station_site::kLayerOID,  row.GetOID() );

	// �w�|���S���R�t���e�[�u���ɓ��Y�|���S���̃��R�[�h���Ȃ���΃G���[
	if( table.RowCount( AheInitQueryFilter( nullptr, nullptr, where ) ) == 0 )
		return false;

	return true;
}


// �|���S����ʃ`�F�b�N
bool StationChecker::checkPolygonC( const CRowContainer& row  ) const
{
	// �|���S����ʂ��擾
	long polygonC = row.GetValue( station_detail::kPolygonType_C, 0L );

	// �R�[�h�l�h���C���O�Ȃ�G���[
	if( m_CVDomains.count( polygonC ) == 0 )
		return false;

	return true;
}


// �|���S����`�F�b�N
bool StationChecker::checkContain( const sindy::CRowContainer& row, const CString& outer_fc_name ) const
{
	using namespace station_layer_helper;

	_ASSERTE( isStationPolygonLayer( outer_fc_name ) );

	// �wID���l�������ɕ�֌W�`�F�b�N
	if( ! checkContainedByFeatureClass( row, outer_fc_name, std::set<long>() ) )
		return false;

	return true;
}


// �|���S����`�F�b�N(�wID�l��)
bool StationChecker::checkContainedBySameStationPolygon( const sindy::CRowContainer& row, const CString& outer_fc_name ) const
{
	using namespace station_layer_helper;

	_ASSERTE( isStationPolygonLayer( outer_fc_name ) );

	// �P���ɕ����|���S�����Ȃ��ꍇ�͕ʂ̃`�F�b�N�ň���������̂�OK�Ƃ���B
	if( ! checkContain( row, outer_fc_name ) )
		return true;

	// �w�|���S���R�t���e�[�u���擾
	auto& table = *m_TableFinder.FindTable(rel_station_site::kTableName);

	// �`�F�b�N�Ώۂ̉wID���擾
	long stationId = getStationId( row, table );
	// �w�ɕR�t���Ă��Ȃ��ꍇ�͕ʂ̃`�F�b�N�ŃG���[�ƂȂ�̂�OK�Ƃ���
	if( stationId == -1 )
		return true;

	// �w�|���S���R�t���e�[�u������`�F�b�N�Ώۂ̉wID�Ɉ�v���郌�R�[�h������
	CString where = uh::str_util::format( _T("%s=%d and %s=%d"),
		                                  rel_station_site::kGroupID,   stationId,
		                                  rel_station_site::kLayerNo_C, getLayerNo( outer_fc_name ) );

	table.clear();
	table._Select( AheInitQueryFilter( nullptr, nullptr, where ), true );
	table.CreateCache();

	// �wID�Ɉ�v����|���S�����Ȃ��ꍇ�̓G���[
	if( table.empty() )
		return false;

	// �����������R�[�h���烌�C����OID���擾
	std::set<long> oids;
	for( const auto& target_row : table ) {
		long oid = CAST_ROWC( target_row )->GetValue( rel_station_site::kLayerOID, -1L );
		oids.insert( oid );
	}

	// �����|���S���͂��邪�A�wID����v���Ă��Ȃ��ꍇ�̓G���[
	if( ! checkContainedByFeatureClass( row, outer_fc_name, oids ) )
		return false;

	// �������ʂ��N���A
	table.clear();

	return true;
}


// �w�|���S���Ƃ̂̕R�t���������`�F�b�N
bool StationChecker::checkLinkedNotExistPolygon( const sindy::CRowContainer& row )
{
	// ���C���ԍ�,���C����OID���擾
	long layerNo  = row.GetValue( rel_station_site::kLayerNo_C, 0L );
	long layerOid = row.GetValue( rel_station_site::kLayerOID, 0L );

	// ���C���ԍ��AOID�̑g�ݍ��킹�����݂��邩�A���O�ɓǂݍ��񂾃��X�g������
	//   ���C���ԍ��Ō���
	auto result = m_OidList.equal_range( layerNo );
	if( result.first == result.second )
		return false;
	//   �����C���ԍ����œ��Y�I�u�W�F�N�g��OID�̂������OK
	for( auto iter = result.first; iter != result.second; ++iter ) {
		if( iter->second == layerOid )
			return true;
	}

	return false;
}


// �w�Ƃ̕R�t���������`�F�b�N
bool StationChecker::checkLinkedNotExistStation( const sindy::CRowContainer& row )
{
	// ���O�ɓǂݍ��񂾉wID���X�g�ɑ��݂��Ȃ����NG
	long stationId = row.GetValue( rel_station_site::kGroupID, 0L );
	if( m_StationIdList.count( stationId ) == 0 )
		return false;

	return true;
}


// �e�[�u���t�@�C���_������
bool StationChecker::initTableFinder( const sindy::CWorkspace& workspace )
{
	using namespace sindy::schema::sj;
	using namespace station_layer_helper;

	// �w���C����񃊃X�g( key:���C����, value: ���C���ԍ� )�Ń��[�v
	for( const auto& layer : getStationLayerInfos() )
	{
		ITablePtr table = workspace.OpenSameOwnersTable( layer.first );
		if( ! table ) {
			return false;
		}
		m_TableFinder.InitCollection( table , CModel() );
	}
	return true;
}


// SJ�w�|���S����ʃR�[�h�̃R�[�h�l�h���C���ǂݍ���
bool StationChecker::readCodeValueDomain()
{
	_ASSERTE( m_TableFinder.FindTable( station_detail::kTableName ) );

	auto& table = *m_TableFinder.FindTable( station_detail::kTableName );
	const auto& domainList = table.GetFieldMap()->GetDomain( station_detail::kPolygonType_C );

	for( const auto& rec : domainList )
		m_CVDomains.insert( rec.second );

	if( m_CVDomains.empty() )
		return false;

	return true;
}


// SJ�w�|���S����OID�Q��ǂݍ���
void StationChecker::readStationPolygonOid()
{
	using namespace sindy::schema::sj;
	using namespace station_layer_helper;

	// �w���C����񃊃X�g( key:���C����, value: ���C���ԍ� )�Ń��[�v
	for( const auto& layer : getStationLayerInfos() )
	{
		auto& table = *m_TableFinder.FindTable( layer.first );

		// �S����
		table._Select( IQueryFilterPtr(), true );
		table.CreateCache();

		// ���C����OID�����C���ԍ��Ƃ̃y�A�Ŋi�[
		for( const auto& row : table )
		{
			long oid = CAST_ROWC(row)->GetOID();
			m_OidList.insert( std::make_pair( layer.second, oid ) );
		}
	}
}


// �wID�̓ǂݍ���
void StationChecker::readStationIds()
{
	auto& table = *m_TableFinder.FindTable( groupid_master::kTableName );

	// �S����
	table._Select( IQueryFilterPtr(), true );
	table.CreateCache();

	// �wID( �O���[�vID�}�X�^��OID )���i�[
	for( const auto& row : table )
		m_StationIdList.insert( CAST_ROWC(row)->GetOID() );
}


// �w��̃t�B�[�`�����w��̃t�B�[�`���N���X�̃|���S���ɕ����Ă��邩���`�F�b�N����
bool StationChecker::checkContainedByFeatureClass( const sindy::CRowContainer& row, const CString& outer_fc_name, const std::set<long>& oids ) const
{
	// �`�F�b�N�Ώۂ̃W�I���g���擾
	IGeometryPtr srcGeo = row.CRowBase::GetShapeCopy();

	// �e�[�u���擾
	auto& featureTable = *m_TableFinder.FindTable( outer_fc_name );
	featureTable.clear();

	IFeatureCursorPtr cur = featureTable.Search( AheInitSpatialFilter( nullptr, srcGeo ), true );
	IFeaturePtr feature;
	while( cur->NextFeature( &feature ) == S_OK && feature )
	{
		// OID�w�肪����΁A�w��OID�t�B�[�`���ȊO�͕�֌W�������Ă�OK�Ƃ��Ȃ�
		if( ! oids.empty() && ! oids.count( feature->GetOID() ) )
			continue;

		// ��֌W�������OK�Ƃ���
		VARIANT_BOOL isOk = VARIANT_FALSE;
		HRESULT hr = IRelationalOperatorPtr( srcGeo )->Within( feature->GetShapeCopy(), &isOk );
		_ASSERTE( SUCCEEDED(hr) );
		if( FAILED( hr ) )
			return false;

		if( VB2bool( isOk ) )
			return true;
	}

	return false;
}