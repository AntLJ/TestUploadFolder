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

// 初期化
bool CRelationTableHelper::init( _IRowPtr& ipRow, const CApplication& cApp )
{
	if( !ipRow )
		return false;

	if( FAILED( ipRow->get_OID( &m_Oid )) )
		return false;

	// フィーチャーからテーブルを取得
	ITablePtr ipCurrentTable;
	ipRow->get_Table(&ipCurrentTable);
	if( !ipCurrentTable )
		return false;

	// 参照テーブルをTOCから取得
	CMap cMap( cApp.GetFocusMap() );
	m_ipStationPoint = AheGetFeatureClass( cMap.FindLayer( nullptr, nullptr, station_point::kTableName ) );

	// テーブル取得
	m_ipRelRailwayNode = AheOpenSameOwnerTable( ipCurrentTable, rel_railway_node::kTableName );
	m_ipJoinLineInfo   = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, join_line_info::kTableName );
	m_ipRailwayLine    = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, railway_line::kTableName );
	m_ipStationInfo    = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, station_info::kTableName );
	m_ipStationPoint   = AheOpenSameOwnerTable( (ITablePtr)m_ipStationPoint, station_point::kTableName );
	if( !m_ipRelRailwayNode || !m_ipJoinLineInfo || !m_ipRailwayLine || !m_ipStationInfo || !m_ipStationPoint )
		return false;

	// フィールドマップの作成
	IFieldsPtr ipFields;
	// 鉄道ノード紐付けテーブル
	m_ipRelRailwayNode->get_Fields( &ipFields );
	m_RelRailwayNodeFM.CreateFieldMap( ipFields );
	// 乗り入れ路線テーブル 
	m_ipJoinLineInfo->get_Fields( &ipFields );
	m_JoinLineInfoFM.CreateFieldMap( ipFields);
	// 鉄道路線テーブル
	m_ipRailwayLine->get_Fields( &ipFields );
	m_RailwayLineFM.CreateFieldMap( ipFields);
	// 駅情報マスター
	m_ipStationInfo->get_Fields( &ipFields );
	m_StationInfoFM.CreateFieldMap( ipFields);
	// 駅ポイント
	m_ipStationPoint->get_Fields( &ipFields );
	m_StationPointFM.CreateFieldMap( ipFields);

	return true;
}


// 鉄道ノード紐付けテーブルの読み込み
bool CRelationTableHelper::read(CRelTableData& cData )
{
	// 検索
	CString strSubFields   = str_util::format(_T("%s,%s,%s"), rel_railway_node::kRwNodeID,
															  rel_railway_node::kJoinLineInfoID,
															  rel_railway_node::kStationNo );
	CString strWhereClause = str_util::format( _T("%s=%ld"), rel_railway_node::kRwNodeID, m_Oid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRelRailwayNode->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
				VARIANT_TRUE, &ipCursor)) || ! ipCursor )
	{
		return false;
	}

	// データを格納
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::LineInfoRecord cRecord;
		
		// JOIN_LINE_ID
		cRecord.nJoinLineId = AheGetAttribute( ipRow, rel_railway_node::kJoinLineInfoID ).lVal;
		// 駅番号
		cRecord.strStationNumber = str_util::ToString( AheGetAttribute( ipRow, rel_railway_node::kStationNo ));
		
		_IRowPtr ipRow;
		m_ipJoinLineInfo->GetRow(cRecord.nJoinLineId, &ipRow);

		// 路線ID->路線名
		cRecord.strRailwayName = getRailwayLineName( AheGetAttribute( ipRow, join_line_info::kLineID).lVal );
		// 駅情報ID->駅名
		cRecord.strStationName = getStaionName( AheGetAttribute( ipRow, join_line_info::kInfoID ).lVal );;

		cData.add(cRecord);
	}

	return true;
}


// 乗り入れ路線テーブルの読み込み
bool CRelationTableHelper::readJoinLineInfo(long lStaPtOid, CRelTableData& cData)
{
	// 検索
	CString strSubFields   = str_util::format(_T("%s,%s,%s,%s"), join_line_info::kObjectID,
		                                                         join_line_info::kStationID,
														         join_line_info::kLineID,
														         join_line_info::kInfoID );
	CString strWhereClause = str_util::format( _T("%s=%ld"), join_line_info::kStationID, lStaPtOid );
	
	_ICursorPtr ipCursor;
	if( FAILED( m_ipJoinLineInfo->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		        VARIANT_TRUE, &ipCursor) || ! ipCursor) )
	{
		return false;
	}

	// 取得したレコードを格納
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::LineInfoRecord cRecord;
		CComVariant vaOid;

		// JOIN_LINE_ID 取得
		ipRow->get_Value( m_JoinLineInfoFM.FindField( object::kObjectID ), &vaOid);
		cRecord.nJoinLineId = vaOid.lVal;
		// 路線ID->路線名
		cRecord.strRailwayName = getRailwayLineName( AheGetAttribute( ipRow, join_line_info::kLineID).lVal );
		// 駅情報ID->駅名
		cRecord.strStationName = getStaionName( AheGetAttribute( ipRow, join_line_info::kInfoID ).lVal );

		cData.add(cRecord);
	}

	return true;
}


// 鉄道ノード紐付けテーブルへレコード追加
bool CRelationTableHelper::addData( const CRelTableData& cData ) const
{
	if( cData.empty() )
		return true;

	for( auto& rec : cData )
	{
		bool bSucceeded = true;

		_IRowPtr ipRow;
		if(  FAILED( m_ipRelRailwayNode->CreateRow(&ipRow)) || ! ipRow )
			return false;

		// 鉄道ノードID
		if( ! AheSetAttribute( ipRow, m_RelRailwayNodeFM.FindField( rel_railway_node::kRwNodeID), CComVariant( m_Oid )) ) 
			bSucceeded =  false;

		// JOIN_LINE_INFO_ID 
		if( ! AheSetAttribute( ipRow, m_RelRailwayNodeFM.FindField( rel_railway_node::kJoinLineInfoID), CComVariant( rec.nJoinLineId )) ) 
			bSucceeded =  false;

		// 駅番号
		if( ! AheSetAttribute( ipRow, m_RelRailwayNodeFM.FindField( rel_railway_node::kStationNo), CComVariant( rec.strStationNumber )) ) 
			bSucceeded =  false;

		// 修正情報更新
		if( bSucceeded && ! AheSetModifyData(ipRow, sindyUpdateTypeCreate) )
			bSucceeded = false;

		if( ! bSucceeded )
			return false;

		if( FAILED(ipRow->Store()) )
			return false;
	}

	return true;
}


// 鉄道ノード紐付けテーブルのデータ削除
bool CRelationTableHelper::deleteData(const std::vector<long>& IDs) const
{
	if( IDs.empty() )
		return true;

	CString strWhereClause = getDeleteWhereClause( IDs );

	if( FAILED(m_ipRelRailwayNode->DeleteSearchedRows( AheInitQueryFilter( nullptr, nullptr, strWhereClause ) )) )
		return false;

	return true;
}


// 鉄道ノード紐付けテーブルの駅番号を更新
bool CRelationTableHelper::changeStationNumber( const CRelTableData& cData) const
{
	for( auto& rec : cData )
	{
		CString strWhereClause = uh::str_util::format( _T("%s=%ld and %s=%ld"), 
		                                                rel_railway_node::kRwNodeID, m_Oid, 
                                                        rel_railway_node::kJoinLineInfoID, rec.nJoinLineId );

		_ICursorPtr ipCursor;
		if( FAILED( m_ipRelRailwayNode->Search( AheInitQueryFilter(nullptr, nullptr, strWhereClause),  
					VARIANT_FALSE, &ipCursor)) || !ipCursor )
		{
			return false;
		}

		_IRowPtr iRow;
		if( ipCursor->NextRow( &iRow ) == S_FALSE || ! iRow ) 
		{
			return false;
		}
		// 修正情報更新
		if( ! AheSetModifyData( iRow, sindyUpdateTypeProperty ) )
		{
			return false;
		}
		// 駅番号更新
		CComVariant vaValue(rec.strStationNumber);

		if( ! AheSetAttribute( iRow, m_RelRailwayNodeFM.FindField( rel_railway_node::kStationNo), vaValue) ) 
		{
			return false;
		}
		iRow->Store();
	}
	return true;
}


// 路線IDから路線名を取得する
CString CRelationTableHelper::getRailwayLineName(long lId)
{
	// 検索
	CString strSubFields = str_util::format(_T("%s,%s"), railway_line::kObjectID,
	                                                     railway_line::kName);
	CString strWhereClause = str_util::format( _T("%s=%ld"), railway_line::kObjectID, lId );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRailwayLine->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		        VARIANT_TRUE, &ipCursor)) || ! ipCursor )
	{
		return _T("");
	}

	_IRowPtr ipRow;
	if( ipCursor->NextRow(&ipRow) != S_OK  || !ipRow )
		return _T("");

	return str_util::ToString( AheGetAttribute( ipRow, railway_line::kName ));
}


// 駅情報IDから駅名を取得する
CString CRelationTableHelper::getStaionName(long lInfoOId)
{
	// 検索
	CString strSubFields = str_util::format(_T("%s,%s"), railway_line::kObjectID,
	                                                     railway_line::kName);
	CString strWhereClause = str_util::format( _T("%s=%ld"), railway_line::kObjectID, lInfoOId );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipStationInfo->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		        VARIANT_TRUE, &ipCursor)) || ! ipCursor)
	{
		return _T("");
	}

	_IRowPtr ipRow;
	if( ipCursor->NextRow(&ipRow) != S_OK  || !ipRow )
		return _T("");

	return str_util::ToString( AheGetAttribute( ipRow, station_info::kName ));
}


// 鉄道ノード紐付けテーブルの削除条件分作成
CString CRelationTableHelper::getDeleteWhereClause(const std::vector<long>& IDs) const
{
	// IN句の制限
	if( IDs.size() > 1000 )
		return false;

	CString strWhereClause;
	// 条件文生成: OBJECTID=*
	strWhereClause.Format( _T("%s=%ld"), rel_railway_node::kRwNodeID, m_Oid);
	
	// 条件文生成: JOIN_LINE_INFO_ID in (*)
	strWhereClause.AppendFormat(_T(" and %s in ("), rel_railway_node::kJoinLineInfoID);

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


// 駅ポイントのフィーチャを取得
IFeaturePtr CRelationTableHelper::getStationPointFeature(long lOid)
{
	// 検索
	CString strSubFields = str_util::format(_T("%s,%s"), join_line_info::kObjectID,
	                                                     join_line_info::kStationID);
	CString strWhereClause = str_util::format( _T("%s=%ld"), join_line_info::kObjectID, lOid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipJoinLineInfo->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		        VARIANT_TRUE, &ipCursor)) || ! ipCursor)
	{
		return nullptr;
	}

	// 乗り入れ路線テーブルのレコード取得
	_IRowPtr ipRow;
	if( ipCursor->NextRow(&ipRow) != S_OK  || ! ipRow )
		return nullptr;

	// 駅ポイントテーブルからOIDで検索して返す
	long lStaPtOid = AheGetAttribute( ipRow, join_line_info::kStationID).lVal;
	ITablePtr(m_ipStationPoint)->GetRow( lStaPtOid, &ipRow );

	return IFeaturePtr(ipRow); 
}
