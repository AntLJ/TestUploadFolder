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

#include <TDC/useful_headers/str_util.h>
#include <TDC/sindylib_base/TableContainer.h>

using namespace sindy::schema;
using namespace sindy::schema::sj;
using namespace uh;

// ������
bool CRelationTableHelper::init( _IRowPtr& ipRow, const CApplication& cApp )
{
	if( !ipRow )
		return false;

	if( FAILED( ipRow->get_OID( &m_Oid )) )
		return false;

	// �t�B�[�`���[����e�[�u�����擾
	ITablePtr ipCurrentTable;
	ipRow->get_Table(&ipCurrentTable);
	if( !ipCurrentTable )
		return false;

	// �Q�ƃe�[�u����TOC����擾
	CMap cMap( cApp.GetFocusMap() );
	m_ipStationPoint = AheGetFeatureClass( cMap.FindLayer( nullptr, nullptr, station_point::kTableName ) );

	// �e�[�u���擾
	m_ipRelRailwayLink = AheOpenSameOwnerTable( ipCurrentTable, rel_railway_link::kTableName );
	m_ipRailwayLink    = AheOpenSameOwnerTable( ipCurrentTable, railway_link::kTableName );
	m_ipJoinLineInfo   = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, join_line_info::kTableName );
	m_ipRailwayLine    = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, railway_line::kTableName );
	m_ipRailwayCorp    = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, railway_corp::kTableName );
	m_ipStationInfo    = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, station_info::kTableName );
	if( !m_ipRelRailwayLink || !m_ipRailwayLink  || !m_ipJoinLineInfo || !m_ipRailwayLine 
				|| !m_ipRailwayCorp || !m_ipStationPoint || !m_ipStationInfo )
	{
		return false;
	}

	// �t�B�[���h�}�b�v�̍쐬
	IFieldsPtr ipFields;
	// �S�������N�R�t���e�[�u��
	m_ipRelRailwayLink->get_Fields( &ipFields );
	m_RelRailwayLinkFM.CreateFieldMap( ipFields );
	// �S�������N�e�[�u��
	m_ipRailwayLink->get_Fields( &ipFields );
	m_RailwayLinkFM.CreateFieldMap( ipFields );
	// ������H���e�[�u�� 
	m_ipJoinLineInfo->get_Fields( &ipFields );
	m_JoinLineInfoFM.CreateFieldMap( ipFields);
	// �S���H���e�[�u��
	m_ipRailwayLine->get_Fields( &ipFields );
	m_RailwayLineFM.CreateFieldMap( ipFields);
	// �S����Ѓe�[�u��
	m_ipRailwayCorp->get_Fields( &ipFields );
	m_RailwayCorpFM.CreateFieldMap( ipFields);
	// �w���}�X�^�[
	m_ipStationInfo->get_Fields( &ipFields );
	m_StationInfoFM.CreateFieldMap( ipFields);
	// �w�|�C���g
	m_ipStationPoint->get_Fields( &ipFields );
	m_StationPointFM.CreateFieldMap( ipFields);

	return true;
}


// �S�������N�R�t���e�[�u���̓ǂݍ���
bool CRelationTableHelper::readRelRailwayLink(CRelTableData& cData )
{
	cData.clear();

	// ����
	CString strSubFields   = str_util::format(_T("%s,%s"), rel_railway_link::kRwLinkID,
															  rel_railway_link::kRailwayLineID );
	CString strWhereClause = str_util::format( _T("%s=%ld"), rel_railway_link::kRwLinkID, m_Oid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRelRailwayLink->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
				VARIANT_TRUE, &ipCursor)) )
	{
		return false;
	}

	// �f�[�^���i�[
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::RailwayLineRecord cRecord;
		CComVariant va;
		
		// RAILWAY_LINE_ID
		long lId = AheGetAttribute( ipRow, rel_railway_link::kRailwayLineID ).lVal;
		// RAILWAY_LINE �e�[�u������l�擾
		if( ! readRailwayLineRecord(lId, cRecord))
			continue;
		
		cData.add(cRecord);
	}

	return true;
}


// �w�|�C���gID��OID����S���H���f�[�^��ǂݍ���
bool CRelationTableHelper::readRailLineFromStaPtOid(long lStaPtOid, CRelTableData& cData)
{
	// ����
	CString strSubFields   = str_util::format(_T("%s,%s"), join_line_info::kLineID,
		join_line_info::kStationID );
	CString strWhereClause = str_util::format( _T("%s=%ld"), join_line_info::kStationID, lStaPtOid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipJoinLineInfo->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		VARIANT_TRUE, &ipCursor)) )
	{
		return false;
	}

	// �擾�������R�[�h���i�[
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::RailwayLineRecord cRecord;
		CComVariant vaOid;

		// LINE_ID�擾
		long lLineId = AheGetAttribute( ipRow, join_line_info::kLineID ).lVal;
		// RAILWAY_LINE�e�[�u������l�擾
		if( ! readRailwayLineRecord( lLineId, cRecord ))
			continue;

		// �擾�ς݂̃��R�[�h�łȂ���Ίi�[
		if( cData.find(lLineId) == cData.end() )
			cData.add(cRecord);
	}

	return true;
}


// �w������S���H���f�[�^��ǂݍ���   
bool CRelationTableHelper::readRailwayLineFromStaName(const CString& strStationName, CRelTableData& cData)
{
	CString strSubFields   = str_util::format(_T("%s,%s"), station_info::kObjectID, station_info::kName );

	// LIKE�����p�Ɍ����l���`
	CString strSearchValue = _T("%") + strStationName + _T("%");
	CString strWhereClause = str_util::format( _T("%s LIKE '%s'"), station_info::kName, strSearchValue );

	// Where����"%"���܂ނ� AheInitQueryFilter �͎g���Ȃ��̂ŁA����IQueryFilter�����
	const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipStationInfo->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor )) )
		return false;

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK && ipRow )
	{
		long lOid = -1;
		ipRow->get_OID( &lOid );

		// �S���H���e�[�u�����w���ID���猟������
		if( ! readRailwayLineFromStaInfoId( lOid, cData ) )
			continue;
	}
	return true;
}


// �S�������N�R�t���e�[�u���փ��R�[�h�ǉ�
bool CRelationTableHelper::addData( const CRelTableData& cData ) const
{
	if( cData.empty() )
		return true;

	for( const auto& rec : cData )
	{
		bool bSucceeded = true;

		_IRowPtr ipRow;
		if(  FAILED( m_ipRelRailwayLink->CreateRow(&ipRow)) || ! ipRow )
			return false;

		// RW_LINK_ID
		if( ! AheSetAttribute(ipRow, rel_railway_link::kRwLinkID, CComVariant(m_Oid)) )
			bSucceeded = false;

		// �S���H��OID
		if( ! AheSetAttribute(ipRow, rel_railway_link::kRailwayLineID, CComVariant( rec.lRailwayLineOid )) )
			bSucceeded = false;

		// �C�����X�V
		if( bSucceeded && ! AheSetModifyData(ipRow, sindyUpdateTypeCreate) )
			bSucceeded = false;

		if( ! bSucceeded )
			return false;

		if( FAILED(ipRow->Store()) )
			return false;
	}

	return true;
}


// �S�������N�R�t���e�[�u���̃f�[�^�폜
bool CRelationTableHelper::deleteData(const std::vector<long>& IDs) const
{
	if( IDs.empty() )
		return true;

	CString strSubFields   = str_util::format(_T("%s,%s"), rel_railway_link::kRwLinkID,
		rel_railway_link::kRailwayLineID );
	CString strWhereClause = getDeleteWhereClause( IDs );

	if( FAILED(m_ipRelRailwayLink->DeleteSearchedRows( AheInitQueryFilter( nullptr, strSubFields, strWhereClause ) )) )
		return false;

	return true;
}


// �S���H����OID���w�肵�ĊY������S�������N�̃t�B�[�`���[�Q���擾����
std::vector<CAdapt<IFeaturePtr>> CRelationTableHelper::getRailwayLinkFeatures(long lRwLineId)
{
	std::vector<CAdapt<IFeaturePtr>> RWLinkFeatures;	

	// ����
	CString strSubFields = str_util::format(_T("%s,%s"), rel_railway_link::kRailwayLineID,
		rel_railway_link::kRwLinkID);
	CString strWhereClause = str_util::format( _T("%s=%ld"), rel_railway_link::kRailwayLineID, lRwLineId );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRelRailwayLink->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),
		VARIANT_TRUE, &ipCursor )) )
	{
		return RWLinkFeatures;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK && ipRow )
	{
		long lRwOid = AheGetAttribute(ipRow, sj::rel_railway_link::kRwLinkID).lVal;

		// RAILWAY_LINK�̃t�B�[�`�����擾���Ċi�[
		_IRowPtr ipRWLinkRow;
		if( m_ipRailwayLink->GetRow(lRwOid, &ipRWLinkRow) == S_OK && ipRWLinkRow )
			RWLinkFeatures.push_back(IFeaturePtr(ipRWLinkRow));
	}

	return RWLinkFeatures;
}




// �S�������N�R�t���e�[�u���̍폜�������쐬
CString CRelationTableHelper::getDeleteWhereClause(const std::vector<long>& IDs) const
{
	// IN��̐���
	if( IDs.size() > 1000 )
		return false;

	CString strWhereClause;
	// ����������: OBJECTID=*
	strWhereClause.Format( _T("%s=%ld"), rel_railway_link::kRwLinkID, m_Oid );
	// ����������: RAILWAY_LINE_ID in (*)
	strWhereClause.AppendFormat( _T(" and %s in ("), rel_railway_link::kRailwayLineID );

	// IN���p�̕����񐶐�
	std::list<long> ids(IDs.begin(), IDs.end());
	std::list<CString> inWhereClaus;
	sindy::CTableBase::IDs2Str(ids, inWhereClaus);

	for( const auto& id : inWhereClaus )
	{
		strWhereClause.AppendFormat( _T("%s"), id );
	}
	strWhereClause.Append(_T(")"));

	return strWhereClause;
}


// �S���H���e�[�u����OID����f�[�^���擾����
bool CRelationTableHelper::readRailwayLineRecord( long lOid, CRelTableData::RailwayLineRecord& record )
{
	// �S���H���e�[�u����1���R�[�h�擾
	_IRowPtr ipRow;
	m_ipRailwayLine->GetRow(lOid, &ipRow);

	// �e�t�B�[���h��ǂݍ���
	CComVariant va;
	// OID
	record.lRailwayLineOid = lOid;
	// �H����
	record.strRailwayName = str_util::ToString( AheGetAttribute(ipRow, railway_line::kName));
	// �S�����ID->��Ж�
	record.strCorpName = getCorpName( AheGetAttribute(ipRow, railway_line::kCorpID).lVal );

	return true;
}


// �S���H���e�[�u���̃f�[�^���w���ID����擾����
bool CRelationTableHelper::readRailwayLineFromStaInfoId(long lStaInfoId, CRelTableData& cData)
{
	// ����
	CString strSubFields   = str_util::format(_T("%s,%s,%s"), join_line_info::kObjectID,
		                                                      join_line_info::kLineID,
		                                                      join_line_info::kInfoID );
	CString strWhereClause = str_util::format( _T("%s=%ld"), join_line_info::kInfoID, lStaInfoId );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipJoinLineInfo->Search(AheInitQueryFilter(nullptr, strSubFields, strWhereClause),
				VARIANT_TRUE, &ipCursor)) )
	{
		return false;
	}

	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::RailwayLineRecord cRecord;
		CComVariant va;

		// LINE ID�擾
		long lLineId = AheGetAttribute( ipRow, join_line_info::kLineID ).lVal;

		if( cData.find(lLineId) != cData.end())
			continue;

		if( ! readRailwayLineRecord( lLineId, cRecord ) )
			continue;

		cData.add(cRecord);
	}
	return true;
}


// �S�����ID�����Ж����擾����
CString CRelationTableHelper::getCorpName(long lCorpId)
{
	// ����
	CString strSubFields = str_util::format(_T("%s,%s"), railway_corp::kObjectID,
	                                                     railway_corp::kName);
	CString strWhereClause = str_util::format( _T("%s=%ld"), railway_corp::kObjectID, lCorpId );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRailwayCorp->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		        VARIANT_TRUE, &ipCursor)) )
	{
		return _T("");
	}

	_IRowPtr ipRow;
	if( ipCursor->NextRow(&ipRow) != S_OK  || !ipRow )
		return _T("");

	// ��Ж��擾���ĕԂ�
	return str_util::ToString( AheGetAttribute(ipRow, railway_corp::kName));
}
