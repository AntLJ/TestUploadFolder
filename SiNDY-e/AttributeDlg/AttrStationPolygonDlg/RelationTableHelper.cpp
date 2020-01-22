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


// 初期化
bool CRelationTableHelper::init( const _IRowPtr& ipRow )
{
	if( !ipRow )
		return false;

	// OID取得
	if( FAILED( ipRow->get_OID( &m_Oid )) )
		return false;

	// レイヤ番号取得
	m_LayerNo = getLayerNo( ipRow );

	// フィーチャーからテーブルを取得
	ITablePtr ipCurrentTable;
	ipRow->get_Table(&ipCurrentTable);
	if( !ipCurrentTable )
		return false;

	// テーブル取得
	m_ipRelStationSite = AheOpenSameOwnerTable( ipCurrentTable, rel_station_site::kTableName );
	m_ipGroupIdMaster  = AheOpenSameOwnerTable( ipCurrentTable, groupid_master::kTableName );
	if( ! m_ipRelStationSite || ! m_ipGroupIdMaster )
		return false;

	// フィールドマップの作成
	IFieldsPtr ipFields;
	//   駅ポリゴン紐付きテーブル
	m_ipRelStationSite->get_Fields( &ipFields );
	m_ipRelStationSiteFM.CreateFieldMap( ipFields );
	//   グループIDマスタ
	m_ipGroupIdMaster->get_Fields( &ipFields );
	m_ipGroupIdMasterFM.CreateFieldMap( ipFields );

	return true;
}


// 駅ポリゴン紐付きテーブルの読み込み
bool CRelationTableHelper::readRelStationSite(CRelStationSiteData& cData )
{
	cData.clear();

	// 検索
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

	// データを格納
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



// 駅名から駅ポリゴン紐付きテーブルを読み込む
bool CRelationTableHelper::readRelStationSiteByStationName(const CString& strStationName, CRelStationSiteData& cData)
{
	// LIKE検索用に検索文字列を整形
	CString strSearchWord = _T("%") + strStationName + _T("%");
	CString strWhereClause = str_util::format( _T("%s LIKE '%s'"), groupid_master::kStationName, strSearchWord );

	// Where文に"%"を含むと AheInitQueryFilter は使えないので、直にIQueryFilterを作る
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


// 駅ポリゴン紐付きテーブルへデータ追加
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

		// GROUP_ID, レイヤー番号, レイヤー内OID
		if( ! AheSetAttribute(ipRow, rel_station_site::kGroupID,   CComVariant(rec.lGroupId)) )
			bSucceeded = false;
		if( ! AheSetAttribute(ipRow, rel_station_site::kLayerNo_C, CComVariant(rec.lLayerNoC)) )
			bSucceeded = false;
		if( ! AheSetAttribute(ipRow, rel_station_site::kLayerOID,  CComVariant(rec.lLayerOid)) )
			bSucceeded = false;

		// 修正情報更新
		if( bSucceeded && ! AheSetModifyData(ipRow, sindyUpdateTypeCreate) )
			return false;

		if( ! bSucceeded )
			return false;

		if( FAILED(ipRow->Store()) )
			return false;
	}

	return true;
}


// 鉄駅ポリゴン紐付きテーブルのデータ削除
bool CRelationTableHelper::deleteData( const CRelStationSiteData& cData ) const
{
	if( cData.empty() )
		return true;

	CString strWhereClause = getDeleteWhereClause( cData );

	if( FAILED(m_ipRelStationSite->DeleteSearchedRows( AheInitQueryFilter( nullptr, nullptr, strWhereClause ) )) )
		return false;

	return true;
}


// グループIDマスタから駅名を取得する
CString CRelationTableHelper::getStationName( long lGroupId ) const
{
	_IRowPtr ipRow;
	m_ipGroupIdMaster->GetRow( lGroupId, & ipRow );
	return uh::str_util::ToString( AheGetAttribute( ipRow, groupid_master::kStationName ));
}


// 駅ポリゴン紐付きテーブルの削除条件文作成
CString CRelationTableHelper::getDeleteWhereClause( const CRelStationSiteData& cData ) const
{
	if( cData.empty() )
		return false;

	// IN句の制限
	if( cData.size() > 1000 )
		return false;

	CString strWhereClause;
	// 条件文生成: GROUPID in (*)
	strWhereClause.AppendFormat( _T("%s=%ld and %s in ("), rel_station_site::kLayerNo_C, m_LayerNo, rel_station_site::kGroupID );

	// IN文用の文字列生成
	std::list<long> group_ids;
	for( const auto& rec : cData )
		group_ids.push_back( rec.lGroupId );

	std::list<CString> strInWhereClause;
	sindy::CTableBase::IDs2Str(group_ids, strInWhereClause);
	strWhereClause.AppendFormat( _T("%s"), strInWhereClause.front() );
	strWhereClause.Append(_T(")"));

	return strWhereClause;
}


// 駅ポリゴン紐付きテーブルのデータを駅IDから取得する
bool CRelationTableHelper::readRelStationSiteByStationId( long lStationId, CRelStationSiteData& cData )
{
	// 検索
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


// レイヤ番号を取得する
int CRelationTableHelper::getLayerNo( const _IRowPtr& ipRow )
{
	ITablePtr ipTable;
	ipRow->get_Table( &ipTable );
	if( ! ipTable )
		return -1;

	// レイヤ名取得
	CString strLayerName = AheGetFeatureClassName( ipTable );

	return getLayerNo( strLayerName );
}


// レイヤ番号を取得する
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

