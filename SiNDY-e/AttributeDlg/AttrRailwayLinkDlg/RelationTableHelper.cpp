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

	// フィールドマップの作成
	IFieldsPtr ipFields;
	// 鉄道リンク紐付けテーブル
	m_ipRelRailwayLink->get_Fields( &ipFields );
	m_RelRailwayLinkFM.CreateFieldMap( ipFields );
	// 鉄道リンクテーブル
	m_ipRailwayLink->get_Fields( &ipFields );
	m_RailwayLinkFM.CreateFieldMap( ipFields );
	// 乗り入れ路線テーブル 
	m_ipJoinLineInfo->get_Fields( &ipFields );
	m_JoinLineInfoFM.CreateFieldMap( ipFields);
	// 鉄道路線テーブル
	m_ipRailwayLine->get_Fields( &ipFields );
	m_RailwayLineFM.CreateFieldMap( ipFields);
	// 鉄道会社テーブル
	m_ipRailwayCorp->get_Fields( &ipFields );
	m_RailwayCorpFM.CreateFieldMap( ipFields);
	// 駅情報マスター
	m_ipStationInfo->get_Fields( &ipFields );
	m_StationInfoFM.CreateFieldMap( ipFields);
	// 駅ポイント
	m_ipStationPoint->get_Fields( &ipFields );
	m_StationPointFM.CreateFieldMap( ipFields);

	return true;
}


// 鉄道リンク紐付けテーブルの読み込み
bool CRelationTableHelper::readRelRailwayLink(CRelTableData& cData )
{
	cData.clear();

	// 検索
	CString strSubFields   = str_util::format(_T("%s,%s"), rel_railway_link::kRwLinkID,
															  rel_railway_link::kRailwayLineID );
	CString strWhereClause = str_util::format( _T("%s=%ld"), rel_railway_link::kRwLinkID, m_Oid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipRelRailwayLink->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
				VARIANT_TRUE, &ipCursor)) )
	{
		return false;
	}

	// データを格納
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::RailwayLineRecord cRecord;
		CComVariant va;
		
		// RAILWAY_LINE_ID
		long lId = AheGetAttribute( ipRow, rel_railway_link::kRailwayLineID ).lVal;
		// RAILWAY_LINE テーブルから値取得
		if( ! readRailwayLineRecord(lId, cRecord))
			continue;
		
		cData.add(cRecord);
	}

	return true;
}


// 駅ポイントIDのOIDから鉄道路線データを読み込む
bool CRelationTableHelper::readRailLineFromStaPtOid(long lStaPtOid, CRelTableData& cData)
{
	// 検索
	CString strSubFields   = str_util::format(_T("%s,%s"), join_line_info::kLineID,
		join_line_info::kStationID );
	CString strWhereClause = str_util::format( _T("%s=%ld"), join_line_info::kStationID, lStaPtOid );

	_ICursorPtr ipCursor;
	if( FAILED( m_ipJoinLineInfo->Search( AheInitQueryFilter(nullptr, strSubFields, strWhereClause),  
		VARIANT_TRUE, &ipCursor)) )
	{
		return false;
	}

	// 取得したレコードを格納
	_IRowPtr ipRow;
	while( ipCursor->NextRow(&ipRow) == S_OK  && ipRow )
	{
		CRelTableData::RailwayLineRecord cRecord;
		CComVariant vaOid;

		// LINE_ID取得
		long lLineId = AheGetAttribute( ipRow, join_line_info::kLineID ).lVal;
		// RAILWAY_LINEテーブルから値取得
		if( ! readRailwayLineRecord( lLineId, cRecord ))
			continue;

		// 取得済みのレコードでなければ格納
		if( cData.find(lLineId) == cData.end() )
			cData.add(cRecord);
	}

	return true;
}


// 駅名から鉄道路線データを読み込む   
bool CRelationTableHelper::readRailwayLineFromStaName(const CString& strStationName, CRelTableData& cData)
{
	CString strSubFields   = str_util::format(_T("%s,%s"), station_info::kObjectID, station_info::kName );

	// LIKE検索用に検索値整形
	CString strSearchValue = _T("%") + strStationName + _T("%");
	CString strWhereClause = str_util::format( _T("%s LIKE '%s'"), station_info::kName, strSearchValue );

	// Where文に"%"を含むと AheInitQueryFilter は使えないので、直にIQueryFilterを作る
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

		// 鉄道路線テーブルを駅情報IDから検索する
		if( ! readRailwayLineFromStaInfoId( lOid, cData ) )
			continue;
	}
	return true;
}


// 鉄道リンク紐付けテーブルへレコード追加
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

		// 鉄道路線OID
		if( ! AheSetAttribute(ipRow, rel_railway_link::kRailwayLineID, CComVariant( rec.lRailwayLineOid )) )
			bSucceeded = false;

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


// 鉄道リンク紐付けテーブルのデータ削除
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


// 鉄道路線のOIDを指定して該当する鉄道リンクのフィーチャー群を取得する
std::vector<CAdapt<IFeaturePtr>> CRelationTableHelper::getRailwayLinkFeatures(long lRwLineId)
{
	std::vector<CAdapt<IFeaturePtr>> RWLinkFeatures;	

	// 検索
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

		// RAILWAY_LINKのフィーチャを取得して格納
		_IRowPtr ipRWLinkRow;
		if( m_ipRailwayLink->GetRow(lRwOid, &ipRWLinkRow) == S_OK && ipRWLinkRow )
			RWLinkFeatures.push_back(IFeaturePtr(ipRWLinkRow));
	}

	return RWLinkFeatures;
}




// 鉄道リンク紐付けテーブルの削除条件分作成
CString CRelationTableHelper::getDeleteWhereClause(const std::vector<long>& IDs) const
{
	// IN句の制限
	if( IDs.size() > 1000 )
		return false;

	CString strWhereClause;
	// 条件文生成: OBJECTID=*
	strWhereClause.Format( _T("%s=%ld"), rel_railway_link::kRwLinkID, m_Oid );
	// 条件文生成: RAILWAY_LINE_ID in (*)
	strWhereClause.AppendFormat( _T(" and %s in ("), rel_railway_link::kRailwayLineID );

	// IN文用の文字列生成
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


// 鉄道路線テーブルのOIDからデータを取得する
bool CRelationTableHelper::readRailwayLineRecord( long lOid, CRelTableData::RailwayLineRecord& record )
{
	// 鉄道路線テーブルの1レコード取得
	_IRowPtr ipRow;
	m_ipRailwayLine->GetRow(lOid, &ipRow);

	// 各フィールドを読み込み
	CComVariant va;
	// OID
	record.lRailwayLineOid = lOid;
	// 路線名
	record.strRailwayName = str_util::ToString( AheGetAttribute(ipRow, railway_line::kName));
	// 鉄道会社ID->会社名
	record.strCorpName = getCorpName( AheGetAttribute(ipRow, railway_line::kCorpID).lVal );

	return true;
}


// 鉄道路線テーブルのデータを駅情報IDから取得する
bool CRelationTableHelper::readRailwayLineFromStaInfoId(long lStaInfoId, CRelTableData& cData)
{
	// 検索
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

		// LINE ID取得
		long lLineId = AheGetAttribute( ipRow, join_line_info::kLineID ).lVal;

		if( cData.find(lLineId) != cData.end())
			continue;

		if( ! readRailwayLineRecord( lLineId, cRecord ) )
			continue;

		cData.add(cRecord);
	}
	return true;
}


// 鉄道会社IDから会社名を取得する
CString CRelationTableHelper::getCorpName(long lCorpId)
{
	// 検索
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

	// 会社名取得して返す
	return str_util::ToString( AheGetAttribute(ipRow, railway_corp::kName));
}
