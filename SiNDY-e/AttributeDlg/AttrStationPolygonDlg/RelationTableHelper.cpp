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
#include "RelationTableHelper.h"
#include "Map.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindy_lib_base
#include <TDC/sindylib_base/TableContainer.h>

using namespace sindy::schema;
using namespace sindy::schema::sj;
using namespace uh;


// ������
bool CRelationTableHelper::init( const _IRowPtr& ipRow )
{
	if( !ipRow )
		return false;

	// OID�擾
	if( FAILED( ipRow->get_OID( &m_Oid )) )
		return false;

	// ���C���ԍ��擾
	m_LayerNo = getLayerNo( ipRow );

	// �t�B�[�`���[����e�[�u�����擾
	ITablePtr ipCurrentTable;
	ipRow->get_Table(&ipCurrentTable);
	if( !ipCurrentTable )
		return false;

	// �e�[�u���擾
	m_ipRelStationSite = AheOpenSameOwnerTable( ipCurrentTable, rel_station_site::kTableName );
	m_ipGroupIdMaster  = AheOpenSameOwnerTable( ipCurrentTable, groupid_master::kTableName );
	if( ! m_ipRelStationSite || ! m_ipGroupIdMaster )
		return false;

	// �t�B�[���h�}�b�v�̍쐬
	IFieldsPtr ipFields;
	//   �w�|���S���R�t���e�[�u��
	m_ipRelStationSite->get_Fields( &ipFields );
	m_ipRelStationSiteFM.CreateFieldMap( ipFields );
	//   �O���[�vID�}�X�^
	m_ipGroupIdMaster->get_Fields( &ipFields );
	m_ipGroupIdMasterFM.CreateFieldMap( ipFields );

	return true;
}


// �w�|���S���R�t���e�[�u���̓ǂݍ���
bool CRelationTableHelper::readRelStationSite(CRelStationSiteData& cData )
{
	cData.clear();

	// ����
	CString strWhereClause = str_util::format( _T("%s=%ld AND %s=%ld"), 
	                                           rel_station_site::kLayerNo_C, m_LayerNo,
											   rel_station_site::kLayerOID,  m_Oid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRelStationSite->Search( AheInitQueryFilter(nullptr, nullptr, strWhereClause),  
				VARIANT_TRUE,
				&ipCursor)) )
	{
		return false;
	}

	// �f�[�^���i�[
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelStationSiteData::RelStationSiteRecord record;
		record.lGroupId  = AheGetAttribute( ipRow, rel_station_site::kGroupID ).lVal;
		record.lLayerNoC = AheGetAttribute( ipRow, rel_station_site::kLayerNo_C ).lVal;;
		record.lLayerOid = AheGetAttribute( ipRow, rel_station_site::kLayerOID ).lVal;
		
		cData.add(record);
	}

	return true;
}



// �w������w�|���S���R�t���e�[�u����ǂݍ���
bool CRelationTableHelper::readRelStationSiteByStationName(const CString& strStationName, CRelStationSiteData& cData)
{
	// LIKE�����p�Ɍ���������𐮌`
	CString strSearchWord = _T("%") + strStationName + _T("%");
	CString strWhereClause = str_util::format( _T("%s LIKE '%s'"), groupid_master::kStationName, strSearchWord );

	// Where����"%"���܂ނ� AheInitQueryFilter �͎g���Ȃ��̂ŁA����IQueryFilter�����
	const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipGroupIdMaster->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor )) )
		return false;

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK && ipRow )
	{
		long lGroupId(0L);
		ipRow->get_OID( &lGroupId );

		CRelStationSiteData::RelStationSiteRecord record;
		record.lGroupId  = lGroupId;
		record.lLayerNoC = m_LayerNo; 
		record.lLayerOid = m_Oid;

		cData.add( record );
	}
	return true;
}


// �w�|���S���R�t���e�[�u���փf�[�^�ǉ�
bool CRelationTableHelper::insertData( const CRelStationSiteData& cData ) const
{
	if( cData.empty() )
		return true;

	for( const auto& rec : cData )
	{
		bool bSucceeded = true;

		_IRowPtr ipRow;
		if(  FAILED( m_ipRelStationSite->CreateRow(&ipRow)) || ! ipRow )
			return false;

		// GROUP_ID, ���C���[�ԍ�, ���C���[��OID
		if( ! AheSetAttribute(ipRow, rel_station_site::kGroupID,   CComVariant(rec.lGroupId)) )
			bSucceeded = false;
		if( ! AheSetAttribute(ipRow, rel_station_site::kLayerNo_C, CComVariant(rec.lLayerNoC)) )
			bSucceeded = false;
		if( ! AheSetAttribute(ipRow, rel_station_site::kLayerOID,  CComVariant(rec.lLayerOid)) )
			bSucceeded = false;

		// �C�����X�V
		if( bSucceeded && ! AheSetModifyData(ipRow, sindyUpdateTypeCreate) )
			return false;

		if( ! bSucceeded )
			return false;

		if( FAILED(ipRow->Store()) )
			return false;
	}

	return true;
}


// �S�w�|���S���R�t���e�[�u���̃f�[�^�폜
bool CRelationTableHelper::deleteData( const CRelStationSiteData& cData ) const
{
	if( cData.empty() )
		return true;

	CString strWhereClause = getDeleteWhereClause( cData );

	if( FAILED(m_ipRelStationSite->DeleteSearchedRows( AheInitQueryFilter( nullptr, nullptr, strWhereClause ) )) )
		return false;

	return true;
}


// �O���[�vID�}�X�^����w�����擾����
CString CRelationTableHelper::getStationName( long lGroupId ) const
{
	_IRowPtr ipRow;
	m_ipGroupIdMaster->GetRow( lGroupId, & ipRow );
	return uh::str_util::ToString( AheGetAttribute( ipRow, groupid_master::kStationName ));
}


// �w�|���S���R�t���e�[�u���̍폜�������쐬
CString CRelationTableHelper::getDeleteWhereClause( const CRelStationSiteData& cData ) const
{
	if( cData.empty() )
		return false;

	// IN��̐���
	if( cData.size() > 1000 )
		return false;

	CString strWhereClause;
	// ����������: GROUPID in (*)
	strWhereClause.AppendFormat( _T("%s=%ld and %s in ("), rel_station_site::kLayerNo_C, m_LayerNo, rel_station_site::kGroupID );

	// IN���p�̕����񐶐�
	std::list<long> group_ids;
	for( const auto& rec : cData )
		group_ids.push_back( rec.lGroupId );

	std::list<CString> strInWhereClause;
	sindy::CTableBase::IDs2Str(group_ids, strInWhereClause);
	strWhereClause.AppendFormat( _T("%s"), strInWhereClause.front() );
	strWhereClause.Append(_T(")"));

	return strWhereClause;
}


// �w�|���S���R�t���e�[�u���̃f�[�^���wID����擾����
bool CRelationTableHelper::readRelStationSiteByStationId( long lStationId, CRelStationSiteData& cData )
{
	// ����
	CString strWhereClause = str_util::format( _T("%s=%ld"), rel_station_site::kGroupID, lStationId );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRelStationSite->Search(AheInitQueryFilter(nullptr, nullptr, strWhereClause),
				VARIANT_TRUE, &ipCursor)) )
	{
		return false;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelStationSiteData::RelStationSiteRecord record;
		record.lGroupId  = lStationId;
		record.lLayerNoC = AheGetAttribute( ipRow, rel_station_site::kLayerNo_C ).lVal;
		record.lLayerOid = AheGetAttribute( ipRow, rel_station_site::kLayerOID ).lVal;

		cData.add(record);
	}
	return true;
}


// ���C���ԍ����擾����
int CRelationTableHelper::getLayerNo( const _IRowPtr& ipRow )
{
	ITablePtr ipTable;
	ipRow->get_Table( &ipTable );
	if( ! ipTable )
		return -1;

	// ���C�����擾
	CString strLayerName = AheGetFeatureClassName( ipTable );

	return getLayerNo( strLayerName );
}


// ���C���ԍ����擾����
int CRelationTableHelper::getLayerNo( const CString& strLayerName )
{
	if( strLayerName.CompareNoCase( station_detail::kTableName ) == 0 ){
		return rel_station_site::layerno_class::kStationDetail;
	}
	if( strLayerName.CompareNoCase( midzoom_platform::kTableName ) == 0 ){
		return rel_station_site::layerno_class::kMidzoomPlatform;
	}
	if( strLayerName.CompareNoCase( highzoom_station::kTableName ) == 0 ){
		return rel_station_site::layerno_class::kHighzoomStation;
	}
	if( strLayerName.CompareNoCase( midzoom_station::kTableName ) == 0 ){
		return rel_station_site::layerno_class::kMidzoomStation;
	}
	if( strLayerName.CompareNoCase( lowzoom_platform::kTableName ) == 0 ){
		return rel_station_site::layerno_class::kLowzoomPlatform;
	}
	return -1;
}

