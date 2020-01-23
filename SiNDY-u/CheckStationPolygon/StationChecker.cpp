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


// 初期化
bool StationChecker::init( const CWorkspace& workspace )
{
	// テーブルファインダー初期化
	if( ! initTableFinder( workspace ) )
		return false;

	// ポリゴン種別のコード値ドメイン読み込み
	if( ! readCodeValueDomain() )
		return false;

	// 駅ポリゴンのOID読み込み
	readStationPolygonOid();

	// 駅IDの読み込み
	readStationIds();

	return true;
}


// 駅情報紐付きチェック
bool StationChecker::checkLinkedStation( const CRowContainer& row, const CString& table_name ) const
{
	using namespace rel_station_site;
	using namespace station_layer_helper;

	_ASSERTE( isStationPolygonLayer( table_name ) );

	// 駅ポリゴン紐付きテーブル取得
	auto& table = *m_TableFinder.FindTable( rel_station_site::kTableName );
	table.clear();

	// レコード検索 ( LAYERNO_C=* and LAYER_OID=*  )
	CString where = uh::str_util::format( _T("%s=%d and %s=%d"),
		                                  rel_station_site::kLayerNo_C, getLayerNo( table_name ),
										  rel_station_site::kLayerOID,  row.GetOID() );

	// 駅ポリゴン紐付きテーブルに当該ポリゴンのレコードがなければエラー
	if( table.RowCount( AheInitQueryFilter( nullptr, nullptr, where ) ) == 0 )
		return false;

	return true;
}


// ポリゴン種別チェック
bool StationChecker::checkPolygonC( const CRowContainer& row  ) const
{
	// ポリゴン種別を取得
	long polygonC = row.GetValue( station_detail::kPolygonType_C, 0L );

	// コード値ドメイン外ならエラー
	if( m_CVDomains.count( polygonC ) == 0 )
		return false;

	return true;
}


// ポリゴン包括チェック
bool StationChecker::checkContain( const sindy::CRowContainer& row, const CString& outer_fc_name ) const
{
	using namespace station_layer_helper;

	_ASSERTE( isStationPolygonLayer( outer_fc_name ) );

	// 駅IDを考慮せずに包括関係チェック
	if( ! checkContainedByFeatureClass( row, outer_fc_name, std::set<long>() ) )
		return false;

	return true;
}


// ポリゴン包括チェック(駅ID考慮)
bool StationChecker::checkContainedBySameStationPolygon( const sindy::CRowContainer& row, const CString& outer_fc_name ) const
{
	using namespace station_layer_helper;

	_ASSERTE( isStationPolygonLayer( outer_fc_name ) );

	// 単純に包括するポリゴンがない場合は別のチェックで引っかかるのでOKとする。
	if( ! checkContain( row, outer_fc_name ) )
		return true;

	// 駅ポリゴン紐付きテーブル取得
	auto& table = *m_TableFinder.FindTable(rel_station_site::kTableName);

	// チェック対象の駅IDを取得
	long stationId = getStationId( row, table );
	// 駅に紐付いていない場合は別のチェックでエラーとなるのでOKとする
	if( stationId == -1 )
		return true;

	// 駅ポリゴン紐付きテーブルからチェック対象の駅IDに一致するレコードを検索
	CString where = uh::str_util::format( _T("%s=%d and %s=%d"),
		                                  rel_station_site::kGroupID,   stationId,
		                                  rel_station_site::kLayerNo_C, getLayerNo( outer_fc_name ) );

	table.clear();
	table._Select( AheInitQueryFilter( nullptr, nullptr, where ), true );
	table.CreateCache();

	// 駅IDに一致するポリゴンがない場合はエラー
	if( table.empty() )
		return false;

	// 検索したレコードからレイヤ内OIDを取得
	std::set<long> oids;
	for( const auto& target_row : table ) {
		long oid = CAST_ROWC( target_row )->GetValue( rel_station_site::kLayerOID, -1L );
		oids.insert( oid );
	}

	// 包括するポリゴンはあるが、駅IDが一致していない場合はエラー
	if( ! checkContainedByFeatureClass( row, outer_fc_name, oids ) )
		return false;

	// 検索結果をクリア
	table.clear();

	return true;
}


// 駅ポリゴンとのの紐付き正当性チェック
bool StationChecker::checkLinkedNotExistPolygon( const sindy::CRowContainer& row )
{
	// レイヤ番号,レイヤ内OIDを取得
	long layerNo  = row.GetValue( rel_station_site::kLayerNo_C, 0L );
	long layerOid = row.GetValue( rel_station_site::kLayerOID, 0L );

	// レイヤ番号、OIDの組み合わせが存在するか、事前に読み込んだリストを検索
	//   レイヤ番号で検索
	auto result = m_OidList.equal_range( layerNo );
	if( result.first == result.second )
		return false;
	//   同レイヤ番号内で当該オブジェクトのOIDのがあればOK
	for( auto iter = result.first; iter != result.second; ++iter ) {
		if( iter->second == layerOid )
			return true;
	}

	return false;
}


// 駅との紐付き正当性チェック
bool StationChecker::checkLinkedNotExistStation( const sindy::CRowContainer& row )
{
	// 事前に読み込んだ駅IDリストに存在しなければNG
	long stationId = row.GetValue( rel_station_site::kGroupID, 0L );
	if( m_StationIdList.count( stationId ) == 0 )
		return false;

	return true;
}


// テーブルファインダ初期化
bool StationChecker::initTableFinder( const sindy::CWorkspace& workspace )
{
	using namespace sindy::schema::sj;
	using namespace station_layer_helper;

	// 駅レイヤ情報リスト( key:レイヤ名, value: レイヤ番号 )でループ
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


// SJ駅ポリゴン種別コードのコード値ドメイン読み込み
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


// SJ駅ポリゴンのOID群を読み込み
void StationChecker::readStationPolygonOid()
{
	using namespace sindy::schema::sj;
	using namespace station_layer_helper;

	// 駅レイヤ情報リスト( key:レイヤ名, value: レイヤ番号 )でループ
	for( const auto& layer : getStationLayerInfos() )
	{
		auto& table = *m_TableFinder.FindTable( layer.first );

		// 全検索
		table._Select( IQueryFilterPtr(), true );
		table.CreateCache();

		// レイヤのOIDをレイヤ番号とのペアで格納
		for( const auto& row : table )
		{
			long oid = CAST_ROWC(row)->GetOID();
			m_OidList.insert( std::make_pair( layer.second, oid ) );
		}
	}
}


// 駅IDの読み込み
void StationChecker::readStationIds()
{
	auto& table = *m_TableFinder.FindTable( groupid_master::kTableName );

	// 全検索
	table._Select( IQueryFilterPtr(), true );
	table.CreateCache();

	// 駅ID( グループIDマスタのOID )を格納
	for( const auto& row : table )
		m_StationIdList.insert( CAST_ROWC(row)->GetOID() );
}


// 指定のフィーチャが指定のフィーチャクラスのポリゴンに包括されているかをチェックする
bool StationChecker::checkContainedByFeatureClass( const sindy::CRowContainer& row, const CString& outer_fc_name, const std::set<long>& oids ) const
{
	// チェック対象のジオメトリ取得
	IGeometryPtr srcGeo = row.CRowBase::GetShapeCopy();

	// テーブル取得
	auto& featureTable = *m_TableFinder.FindTable( outer_fc_name );
	featureTable.clear();

	IFeatureCursorPtr cur = featureTable.Search( AheInitSpatialFilter( nullptr, srcGeo ), true );
	IFeaturePtr feature;
	while( cur->NextFeature( &feature ) == S_OK && feature )
	{
		// OID指定があれば、指定OIDフィーチャ以外は包括関係があってもOKとしない
		if( ! oids.empty() && ! oids.count( feature->GetOID() ) )
			continue;

		// 包括関係があればOKとする
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