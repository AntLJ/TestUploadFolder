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
#include "CheckRailwayNW.h"
#include "util.h"
#include "value.h"

#include <TDC/useful_headers/str_util.h>
#include <sindy/workspace.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::sj;
using namespace uh;

namespace
{
	crd_cnv g_crd;
}


// 実行
bool CCheckRailwayNW::execute()
{
	// 初期化
	if( !init() )
		return false;

	// ヘッダ情報等出力
	m_output.OutputHeader();
	m_output.OutputLog( err_level::info, err_code::StrRailNWDB,      m_strRailNwDB );
	m_output.OutputLog( err_level::info, err_code::StrRailPoiDB,     m_strRailPoiDB );
	m_output.OutputLog( err_level::info, err_code::StrStationDB,     m_strStationDB );
	m_output.OutputLog( err_level::info, err_code::StrMeshDB,        m_strMeshDB );
	m_output.OutputLog( err_level::info, err_code::StrLogFile,       m_strLog );
	m_output.OutputLog( err_level::info, err_code::StrMeshList,      m_strMeshList );
	m_output.OutputLog( err_level::info, err_code::StrDistThreshold, m_strDistThreshold );
	m_output.OutputLog( err_level::info, err_code::StrNeiThreshold,  m_strNeighborThreshold);
	m_output.OutputLog( err_level::info, err_code::StrStartTime,     util::GetCurTime() );

	// 関連テーブル類取得
	if( ! GetRelRecAll(   m_ipRelRailLinkTable, m_relRailLinkFM, rel_railway_link::kTableName, m_relRailwayLink ) ) return false;
	if( ! GetRelRecAll(   m_ipRelRailNodeTable, m_relRailNodeFM, rel_railway_node::kTableName, m_relRailwayNode ) ) return false;
	if( ! GetOtherRecAll( m_ipJoinLineTable,    m_joinLineFM,    join_line_info::kTableName,   m_joinLineInfo ) )   return false;
	if( ! GetOtherRecAll( m_ipRailwayLineTable, m_railwayLineFM, railway_line::kTableName,     m_railwayLine ) )    return false;

	// メッシュごとの処理
	for( auto& mesh : m_meshList ){

		CString strMesh;
		strMesh.Format( _T("%ld"), mesh );

		m_output.OutputStdErr( err_level::info, err_code::StrStart, strMesh );
		m_output.OutputLog( err_level::info, err_code::StrStart, strMesh );

		// 隣接9メッシュコード取得
		set<long> rin9Mesh;
		Get9Mesh( mesh, rin9Mesh );

		// 9メッシュ分のポリゴン取得
		IGeometryPtr ipMeshGeo, ip9MeshGeo;
		if( ! GetMeshPolygons( mesh, rin9Mesh, ipMeshGeo, ip9MeshGeo ) ){
			m_output.OutputStdErr( err_level::fatal, err_code::GetNineMeshFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetNineMeshFail, strMesh );
			continue;
		}

		// 鉄道リンクと鉄道ノード取得＆RTree作成
		map<long, GeoClass> railLinkMap, railNodeMap;
		IDMAP nodeNodeIDs, nodeLinkIDs;
		if( ! GetRailwayInMesh( ip9MeshGeo, railLinkMap, railNodeMap, nodeNodeIDs, nodeLinkIDs )){
			m_output.OutputStdErr( err_level::fatal, err_code::GetRailFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetRailFail, strMesh );
			continue;
		}
		BGRTree railLinkRTree, railNodeRTree;
		createRTree( railLinkMap, railLinkRTree );
		createRTree( railNodeMap, railNodeRTree );

		// その他形状系取得
		map<long, GeoClass> staPointMap, baseStaMap;
		if( ! GetObjInMesh( ip9MeshGeo, m_ipStationPointFC, m_stationPointFM, staPointMap ) ){
			m_output.OutputStdErr( err_level::fatal, err_code::GetStaPoiFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetStaPoiFail, strMesh );
			continue;
		}
		if( ! GetObjInMesh( ip9MeshGeo, m_ipBaseStationFC, m_baseStationFM, baseStaMap ) ){
			m_output.OutputStdErr( err_level::fatal, err_code::GetBaseStaFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetBaseStaFail, strMesh );
			continue;
		}
		BGRTree staPointRTree, baseStaRTree;
		createRTree( staPointMap, staPointRTree );
		createRTree( baseStaMap, baseStaRTree );

		// メッシュ単位のチェック処理
		MeshCheck( mesh, ipMeshGeo, railLinkMap, railNodeMap, staPointMap, baseStaMap, railLinkRTree, railNodeRTree, staPointRTree, baseStaRTree, nodeNodeIDs, nodeLinkIDs );
	}

	// クエリチェック処理
	m_output.OutputStdErr( err_level::info, err_code::StrQueryStart, _T("") );
	m_output.OutputLog( err_level::info, err_code::StrQueryStart, _T("") );

	QueryCheck();

	// 終了時間出力
	m_output.OutputLog( err_level::info, err_code::StrEndTime, util::GetCurTime() );
	m_output.Close();

	return true;
}

// 初期化
bool CCheckRailwayNW::init()
{
	// ログファイルクラスオープン
	if( ! m_output.Open( m_strLog ) ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenLogFail, m_strLog );
		return false;
	}

	// ワークスペースオープン
	if( ! OpenWorkspace( m_strRailNwDB,  m_ipRailNwWorkspace  ) ) return false;
	if( ! OpenWorkspace( m_strRailPoiDB, m_ipRailPoiWorkspace ) ) return false;

	if( ! OpenWorkspace( m_strMeshDB,    m_ipMeshWorkspace    ) ) return false;
	if( ! OpenWorkspace( m_strStationDB, m_ipStationWorkspace ) ) return false;

	// フィーチャクラス・テーブルオープン
	if( ! OpenFeatureClass( railway_link::kTableName,     m_ipRailNwWorkspace,  m_ipRailLinkFC,       m_railLinkFM     ) ) return false;
	if( ! OpenFeatureClass( railway_node::kTableName,     m_ipRailNwWorkspace,  m_ipRailNodeFC,       m_railNodeFM     ) ) return false;
	if( ! OpenTable(        rel_railway_link::kTableName, m_ipRailNwWorkspace,  m_ipRelRailLinkTable, m_relRailLinkFM  ) ) return false;
	if( ! OpenTable(        rel_railway_node::kTableName, m_ipRailNwWorkspace,  m_ipRelRailNodeTable, m_relRailNodeFM  ) ) return false;

	if( ! OpenFeatureClass( station_point::kTableName,    m_ipRailPoiWorkspace, m_ipStationPointFC,   m_stationPointFM ) ) return false;
	if( ! OpenTable(        railway_line::kTableName,     m_ipRailPoiWorkspace, m_ipRailwayLineTable, m_railwayLineFM  ) ) return false;
	if( ! OpenTable(        join_line_info::kTableName,   m_ipRailPoiWorkspace, m_ipJoinLineTable,    m_joinLineFM     ) ) return false;

	if( ! OpenFeatureClass( base_station::kTableName,     m_ipStationWorkspace, m_ipBaseStationFC,    m_baseStationFM  ) ) return false;

	if( ! OpenFeatureClass( basemesh::kTableName,         m_ipMeshWorkspace,    m_ipBaseMeshFC,       m_baseMeshFM     ) ) return false;

	// 必要なコード値ドメインmapを取得しておく
	util::GetDomainMap( railway_link::kRailwayClass_C, m_railLinkFM, m_railLinkClassMap );
	util::GetDomainMap( railway_link::kDispScale_C,    m_railLinkFM, m_dispScaleClassMap );
	util::GetDomainMap( railway_node::kNodeClass_C,    m_railNodeFM, m_railNodeClassMap );


	// メッシュリスト読み込み
	if( !util::loadMeshList( m_strMeshList, m_meshList ) ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList );
		m_output.OutputLog( err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList );
		return false;
	}

	return true;
}

// ワークスペースオープン
bool CCheckRailwayNW::OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
{
	ipWorkspace = create_workspace( name );
	if( !ipWorkspace ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenWsFail, name );
		m_output.OutputLog( err_level::fatal, err_code::OpenWsFail, name );
		return false;
	}
	return true;
}

// フィーチャクラスオープン＆フィールドマップ作成
bool CCheckRailwayNW::OpenFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
	if( !ipFeatureClass ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenFcFail, name );
		m_output.OutputLog( err_level::fatal, err_code::OpenFcFail, name );
		return false;
	}
	fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

	return true;
}

// テーブルオープン＆フィールドマップ作成
bool CCheckRailwayNW::OpenTable(const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
	if( !ipTable ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenTblFail, name );
		m_output.OutputLog( err_level::fatal, err_code::OpenTblFail, name );
		return false;
	}
	fieldMap.CreateFieldMap( ipTable->GetFields() );

	return true;
}

// 与えたメッシュ隣接9メッシュリストを作成する
void CCheckRailwayNW::Get9Mesh( long mesh, std::set<long>& meshlist )
{
	meshlist.insert( mesh );                              // 中心
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  1 ) ); // 左上
	meshlist.insert( g_crd.GetMeshCode( mesh,  0,  1 ) ); // 上
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  1 ) ); // 右上
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  0 ) ); // 左
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  0 ) ); // 右
	meshlist.insert( g_crd.GetMeshCode( mesh, -1, -1 ) ); // 左下
	meshlist.insert( g_crd.GetMeshCode( mesh,  0, -1 ) ); // 下
	meshlist.insert( g_crd.GetMeshCode( mesh,  1, -1 ) ); // 右下
}

// メッシュリストから、結合したメッシュポリゴンを取得
bool CCheckRailwayNW::GetMeshPolygons( long mesh, set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo )
{
	ISpatialReferencePtr ipSpRef;
	auto& strWheres = CreateWhereByList( meshlist, basemesh::kMeshCode );

	IGeometryCollectionPtr ipGeoCol( CLSID_GeometryBag );
	int i = 0;

	for( const auto& strWhere : strWheres ){

		IFeatureCursorPtr ipMeshCursor = m_ipBaseMeshFC->_Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE );
		if( !ipMeshCursor )
			return false;

		IFeaturePtr ipMeshFeature;
		while( S_OK == ipMeshCursor->NextFeature( &ipMeshFeature ) && ipMeshFeature ){

			IGeometryPtr ipTempMeshGeo = ipMeshFeature->GetShapeCopy();
			if( !ipTempMeshGeo )
				return false;

			// 空間参照も取得しておく
			if( i == 0 ){
				ipTempMeshGeo->get_SpatialReference( &ipSpRef );
				((IGeometryPtr)ipGeoCol)->putref_SpatialReference( ipSpRef );
			}

			// 自身のメッシュを取得しておく
			if( mesh == (ipMeshFeature->GetValue( m_baseMeshFM.FindField( basemesh::kMeshCode ) )).lVal )
				ipMeshGeo = ipTempMeshGeo;

			// 後で一括でConstructUnionするために、ジオメトリを格納する
			ipGeoCol->AddGeometry( ipTempMeshGeo );

			++i;
		}
	}

	// 詰めたものをConstructUnion
	IGeometryPtr ipTempUnionMeshGeo( CLSID_Polygon );
	IEnumGeometryPtr ipEnumGeo( ipGeoCol );
	if( FAILED(((ITopologicalOperatorPtr)ipTempUnionMeshGeo)->ConstructUnion( ipEnumGeo ) ))
		return false;

	ipTempUnionMeshGeo->putref_SpatialReference( ipSpRef );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->put_IsKnownSimple( VARIANT_FALSE );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->Simplify();

	ipUnionMeshGeo = ipTempUnionMeshGeo;

	return true;
}

vector<CString> CCheckRailwayNW::CreateWhereByList( const set<long>& list, const CString& field )
{
	CString strWhere;
	vector<CString> retWheres;
	strWhere.Format( _T("%s in ( "), field );
	int i = 0;
	for( const auto& num : list ){
		if( i != 0 )
			strWhere.AppendFormat( _T(", ") );
		strWhere.AppendFormat( _T("%ld"), num );
		if( i < 1000)
			++i;
		else{
			strWhere.AppendFormat( _T(" )") );
			retWheres.push_back( strWhere );
			strWhere.Format( _T("%s in ("), field );
			i = 0;
		}
	}
	if( i != 0){
		strWhere.AppendFormat( _T(" )") );
		retWheres.push_back( strWhere );
	}
	return retWheres;
}

bool CCheckRailwayNW::GetRailwayInMesh( const IGeometryPtr& ipMeshGeo, map<long, GeoClass>& railLinkMap, map<long, GeoClass>& railNodeMap, IDMAP& nodeNodeIDs, IDMAP& nodeLinkIDs )
{
	CString subField;
	subField.Format( _T("%s,%s,%s,%s,%s,%s,%s,%s,%s"), ipc_feature::kObjectID, railway_link::kFromNodeOID, railway_link::kToNodeOID, 
		railway_link::kRailwayClass_C, railway_link::kDispScale_C, railway_link::kUnderGround_F, ipc_feature::kShape, 
		ipc_feature::kOperator, ipc_feature::kModifyDate
		);

	// ジオメトリを保持するため、VARIANT_FALSE
	IFeatureCursorPtr ipLinkCursor = m_ipRailLinkFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipLinkCursor )
		return false;
	IFeaturePtr       ipLinkFeature;

	while( S_OK == ipLinkCursor->NextFeature( &ipLinkFeature ) && ipLinkFeature ){

		// ObjectIDや必要情報取得
		GeoClass railLink;
		long railLinkID    = ipLinkFeature->GetOID();
		railLink.geometry  = ipLinkFeature->GetShapeCopy();
		railLink.attr[railway_link::kFromNodeOID]    = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kFromNodeOID ) ) );
		railLink.attr[railway_link::kToNodeOID]      = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kToNodeOID ) ) );
		railLink.attr[railway_link::kRailwayClass_C] = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kRailwayClass_C ) ) );
		railLink.attr[railway_link::kDispScale_C]    = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kDispScale_C ) ) );
		railLink.attr[railway_link::kUnderGround_F]  = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kUnderGround_F ) ) );
		railLink.attr[ipc_feature::kOperator]        = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kOperator ) ) );
		railLink.attr[ipc_feature::kModifyDate]      = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kModifyDate ) ) );

		long fromID = _ttol( railLink.attr.at( railway_link::kFromNodeOID ) );
		long toID   = _ttol( railLink.attr.at( railway_link::kToNodeOID ) );

		nodeNodeIDs[fromID].insert( toID );
		nodeNodeIDs[toID].insert( fromID );

		nodeLinkIDs[fromID].insert( railLinkID );
		nodeLinkIDs[toID].insert( railLinkID );

		railLinkMap[railLinkID] = move(railLink);
	}

	// 道路ノード用クエリ
	subField.Format( _T("%s,%s,%s,%s,%s"), ipc_feature::kObjectID, railway_node::kNodeClass_C, ipc_feature::kShape, ipc_feature::kOperator, ipc_feature::kModifyDate );
	// ジオメトリを保持するため、VARIANT_FALSE
	IFeatureCursorPtr ipNodeCursor = m_ipRailNodeFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipNodeCursor )
		return false;
	IFeaturePtr       ipNodeFeature;

	// 道路ノードのループ
	while( S_OK == ipNodeCursor->NextFeature( &ipNodeFeature ) && ipNodeFeature ){

		// ObjectIDや必要情報取得
		GeoClass railNode;
		long railNodeID     = ipNodeFeature->GetOID();
		railNode.geometry   = ipNodeFeature->GetShapeCopy();
		railNode.attr[railway_node::kNodeClass_C] = str_util::ToString( ipNodeFeature->GetValue( m_railNodeFM.FindField( railway_node::kNodeClass_C ) ) );
		railNode.attr[ipc_feature::kOperator]     = str_util::ToString( ipNodeFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kOperator ) ) );
		railNode.attr[ipc_feature::kModifyDate]   = str_util::ToString( ipNodeFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kModifyDate ) ) );

		railNodeMap[railNodeID] = move( railNode );
	}

	return true;
}

bool CCheckRailwayNW::GetObjInMesh( const IGeometryPtr& ipMeshGeo, const IFeatureClassPtr& ipFeatureClass, const CFieldMap& fieldMap, map<long, GeoClass>& objMap )
{
	CString subField;
	subField.Format( _T("%s,%s"), ipc_feature::kObjectID, ipc_feature::kShape );

	// ジオメトリを保持するため、VARIANT_FALSE
	IFeatureCursorPtr ipCursor = ipFeatureClass->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipCursor )
		return false;
	IFeaturePtr ipFeature;

	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

		// ObjectIDや必要情報取得
		GeoClass obj;
		long oID    = ipFeature->GetOID();
		obj.geometry  = ipFeature->GetShapeCopy();

		objMap[oID] = move(obj);
	}
	return true;
}

bool CCheckRailwayNW::GetRelRecAll( const ITablePtr& ipTable, const sindy::CFieldMap& fieldMap, const CString& tableName, map<long, vector<recClass>>& recMap )
{
	CString subField;
	if( 0 == tableName.CompareNoCase( rel_railway_link::kTableName ) )
		subField.Format( _T("%s,%s,%s,%s,%s"), ipc_feature::kObjectID, rel_railway_link::kRwLinkID, rel_railway_link::kRailwayLineID, ipc_feature::kOperator, ipc_feature::kModifyDate);
	else if( 0 == tableName.CompareNoCase( rel_railway_node::kTableName ) )
		subField.Format( _T("%s,%s,%s,%s,%s,%s"), ipc_feature::kObjectID, rel_railway_node::kRwNodeID, rel_railway_node::kJoinLineInfoID, rel_railway_node::kStationNo, ipc_feature::kOperator, ipc_feature::kModifyDate );
	else 
		return false;

	_ICursorPtr ipCursor = ipTable->_Search( AheInitQueryFilter( nullptr, subField, nullptr ), VARIANT_TRUE );
	if( ! ipCursor )
		return false;
	_IRowPtr ipRow;

	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){

		// 必要情報取得
		recClass rec;
		rec.attr[ipc_feature::kObjectID]   = str_util::ToString( ipRow->GetOID() );
		rec.attr[ipc_feature::kOperator]   = str_util::ToString( ipRow->GetValue( fieldMap.FindField( ipc_feature::kOperator ) ) );
		rec.attr[ipc_feature::kModifyDate] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( ipc_feature::kModifyDate ) ) );

		if( 0 == tableName.CompareNoCase( rel_railway_link::kTableName ) ){
			long rwLinkID = _ttol( str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_link::kRwLinkID ) ) ) );
			rec.attr[rel_railway_link::kRailwayLineID] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_link::kRailwayLineID ) ) );
			recMap[rwLinkID].push_back( move( rec ) );
		}
		else if( 0 == tableName.CompareNoCase( rel_railway_node::kTableName ) ){
			long rwNodeID = _ttol( str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_node::kRwNodeID ) ) ) );
			rec.attr[rel_railway_node::kJoinLineInfoID] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_node::kJoinLineInfoID ) ) );
			rec.attr[rel_railway_node::kStationNo]      = str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_node::kStationNo ) ) );
			recMap[rwNodeID].push_back( move( rec ) );
		}
		else
			continue;
	}

	return true;
}

bool CCheckRailwayNW::GetOtherRecAll( const ITablePtr& ipTable, const sindy::CFieldMap& fieldMap, const CString& tableName, map<long, recClass>& recMap )
{
	CString subField;
	if( 0 == tableName.CompareNoCase( join_line_info::kTableName ) )
		subField.Format( _T("%s,%s,%s"), ipc_feature::kObjectID, join_line_info::kStationID, join_line_info::kLineID );
	else if( 0 == tableName.CompareNoCase( railway_line::kTableName ) )
		subField.Format( _T("%s,%s"), ipc_feature::kObjectID, railway_line::kName );
	else 
		return false;

	_ICursorPtr ipCursor = ipTable->_Search( AheInitQueryFilter( nullptr, subField, nullptr ), VARIANT_TRUE );
	if( ! ipCursor )
		return false;
	_IRowPtr ipRow;

	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){

		// 必要情報取得
		recClass rec;
		if( 0 == tableName.CompareNoCase( join_line_info::kTableName ) ){
			long oID = ipRow->GetOID();
			rec.attr[join_line_info::kStationID] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( join_line_info::kStationID ) ) );
			rec.attr[join_line_info::kLineID]    = str_util::ToString( ipRow->GetValue( fieldMap.FindField( join_line_info::kLineID ) ) );
			recMap[oID] = move( rec );
		}
		else if( 0 == tableName.CompareNoCase( railway_line::kTableName ) ){
			long oID = ipRow->GetOID();
			rec.attr[railway_line::kName] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( railway_line::kName ) ) );
			recMap[oID] = move( rec );
		}
		else
			continue;
	}

	return true;
}

void CCheckRailwayNW::createRTree( const map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long oID  = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( make_pair( box, static_cast<unsigned int>( oID ) ));
	}
}

// R-Treeから、与えた箱の範囲に引っかかるID群を取得
set<long> CCheckRailwayNW::getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	set<long> retIDs;
	vector<pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

void CCheckRailwayNW::MeshCheck( long mesh, const IGeometryPtr& ipMeshGeo, const map<long, GeoClass>& railLinkMap, const map<long, GeoClass>& railNodeMap, const std::map<long, GeoClass>& staPointMap, const std::map<long, GeoClass>& baseStaMap, const BGRTree& railLinkRTree, const BGRTree& railNodeRTree, const BGRTree& staPointRTree, const BGRTree& baseStaRTree, const IDMAP& nodeNodeIDs, const IDMAP& nodeLinkIDs)
{
	CString strMesh;
	strMesh.Format( _T("%ld"), mesh );

	ITopologicalOperatorPtr ipMeshTopo( ipMeshGeo );
	IGeometryPtr ipBoundaryMesh;
	if( FAILED( ipMeshTopo->get_Boundary( &ipBoundaryMesh ) )){
		m_output.OutputStdErr( err_level::fatal, err_code::GetMeshBoundFail, strMesh );
		m_output.OutputLog( err_level::fatal, err_code::GetMeshBoundFail, strMesh );
		return;
	}
	ipBoundaryMesh->PutRefSpatialReference( ipMeshGeo->GetSpatialReference() );

	// RailLinkチェック
	CheckRailLink( ipMeshGeo, ipBoundaryMesh, railLinkMap, railNodeMap, railLinkRTree, railNodeRTree, nodeLinkIDs );

	// RailNode単独チェック
	CheckRailNode( ipMeshGeo, ipBoundaryMesh, railLinkMap, railNodeMap, staPointMap, baseStaMap, railLinkRTree, railNodeRTree, staPointRTree, baseStaRTree, nodeLinkIDs );

}

void CCheckRailwayNW::CheckRailLink( const IGeometryPtr& ipMeshGeo, const IGeometryPtr& ipBoundaryMesh, const map<long, GeoClass>& railLinkMap, const map<long, GeoClass>& railNodeMap, const BGRTree& railLinkRTree, const BGRTree& railNodeRTree, const IDMAP& nodeLinkIDs )
{
	using namespace railway_link;
	using namespace ipc_feature;
	CString info1, info2;

	map<pair<long, long>, list<long>> dupNodeMap; //!< キー：From/ToノードID（IDの小さい方が先）、値：リンクのObjectID

	// 鉄道リンクのチェック
	for( auto& railLink : railLinkMap ){

		long  railLinkID     = railLink.first;
		auto& railLinkObj    = railLink.second;

		long  fromID        = _ttol( railLinkObj.attr.at( kFromNodeOID ) );
		long  toID          = _ttol( railLinkObj.attr.at( kToNodeOID ) );
		long  railLinkClass = _ttol( railLinkObj.attr.at( kRailwayClass_C ) );
		long  dispScale     = _ttol( railLinkObj.attr.at( kDispScale_C ) );
		auto& underGroundF  = railLinkObj.attr.at( railway_link::kUnderGround_F );
		auto& modOperator   = railLinkObj.attr.at( ipc_feature::kOperator );
		auto& modDate       = railLinkObj.attr.at( ipc_feature::kModifyDate );
	
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( railLinkObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// ■単純な属性チェック

		// 各属性値が定義内か確認する
		if( ! judge_value::IsDefineRailLinkClass( railLinkClass ) )   // ★エラー 鉄道種別コードが定義外
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRailLinkClass, modOperator, modDate, railLinkObj.attr.at( kRailwayClass_C ) );

		if( ! judge_value::IsDefineDispScale( dispScale ) )   // ★エラー スケール表示コードが定義外
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgDispScaleClass, modOperator, modDate, railLinkObj.attr.at( kDispScale_C ) );

		// ★エラー フラグフィールドなのに、[0]と[1]以外の値
		if( ! judge_value::IsOKFlag( underGroundF ) ){
			info1.Format( _T("%s(%s)"), kUnderGround_F, underGroundF );
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgFlagField, modOperator, modDate, info1 );
		}

		// From/ToのIDを確認
		bool bNodeIDErr = false;
		if( fromID <= 0 ){ // ★エラー 始点ノードIDが不正
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgFromID, modOperator, modDate, railLinkObj.attr.at( kFromNodeOID ) );
			bNodeIDErr = true;
		}
		if( toID <= 0 ){   // ★エラー 終点ノードIDが不正
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgToID, modOperator, modDate, railLinkObj.attr.at( kToNodeOID ) );
			bNodeIDErr = true;
		}
		if( fromID == toID ){ // ★エラー リンクの始終点が等しい（属性）
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::LoopLinkSingleFT, modOperator, modDate, _T("属性") );
			bNodeIDErr = true;
		}
		// 同一From/Toが存在しないか確認のため、正常なものはノードIDのペアをキーに、リンクIDを格納しておく
		if( ! bNodeIDErr ){
			if( fromID < toID )
				dupNodeMap[make_pair( fromID, toID )].push_back( railLinkID );
			else
				dupNodeMap[make_pair( toID, fromID )].push_back( railLinkID );
		}

		// ■ここから形状も使ったチェック

		// 単純な形状確認
		esriGeometryType eType = (railLinkObj.geometry.m_T)->GetGeometryType();
		IPointCollectionPtr ipRailLinkPointCol( railLinkObj.geometry.m_T );
		long railLinkPointCnt = ipRailLinkPointCol->GetPointCount();
		if( ! railLinkObj.geometry.m_T || ! ipRailLinkPointCol || railLinkPointCnt < 2 || eType != esriGeometryPolyline ){
			// ★エラー 鉄道リンク形状が不正
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRailLinkGeom, modOperator, modDate );
			continue;
		}

		// From/Toノード取得
		auto fromNodeItr = railNodeMap.find( fromID );
		auto toNodeItr   = railNodeMap.find( toID );

		bool bFromNodeMesh = false, bToNodeMesh = false;

		// メッシュ枠と関連があるか？
		if( ! railLinkObj.IsDisjoint( ipBoundaryMesh ) ){

			// クロスしていないか？
			if( ! railLinkObj.IsCross( ipBoundaryMesh ) ){

				// どちらかがメッシュ図郭ノードじゃないといけない
				bool bOK = false;

				if( (fromNodeItr != railNodeMap.end() 
					&& judge_value::GetLongValue( fromNodeItr->second, railway_node::kNodeClass_C ) == railway_node::node_class::kOutline ) ){
						bOK = true;
						bFromNodeMesh = true;
				}
				if( ( toNodeItr != railNodeMap.end()
					&& judge_value::GetLongValue( toNodeItr->second, railway_node::kNodeClass_C ) == railway_node::node_class::kOutline ) ){
						bOK = true;
						bToNodeMesh = true;
				}

				// ★エラー メッシュ跨ぎのリンクに図郭ノードが存在しない
				if( ! bOK )
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NoBorderNode, modOperator, modDate );
			}
			else // ★エラー 2次メッシュを横断している	
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::CrossMesh, modOperator, modDate );
		}

		// Fromノードの存在確認
		GeoClass fromRailNodeObj;
		bool bFromOK = false;
		if( fromNodeItr == railNodeMap.end() ) // ★エラー FromNodeIDのノードが見つからない
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NotFoundFromNode, modOperator, modDate, railLinkObj.attr.at( kFromNodeOID ) );
		else{
			bFromOK = true;
			fromRailNodeObj = fromNodeItr->second;
		}

		// Toノードの存在確認
		GeoClass toRailNodeObj;
		bool bToOK = false;
		if( toNodeItr == railNodeMap.end() ) // ★エラー ToNodeIDのノードが見つからない
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NotFoundToNode, modOperator, modDate, railLinkObj.attr.at( kToNodeOID ) );
		else{
			bToOK = true;
			toRailNodeObj = toNodeItr->second;
		}

		// From/Toの形状が一致していないか確認
		if( bFromOK && bToOK ){
			if( fromRailNodeObj.IsEqual( toRailNodeObj.geometry.m_T ) ) // ★エラー リンクの始終点が等しい（形状）
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::LoopLinkSingleFT, modOperator, modDate, _T("形状") );
		}

		// メッシュ関連はメッシュを超えて属性が一致するかの確認を行う
		if( ( bFromNodeMesh && bFromOK ) || ( bToNodeMesh && bToOK ) ){
			CString diffField;
			bool    bFind = false;

			long railNodeID = 0;
			if( bFromNodeMesh )
				railNodeID = fromID;
			else
				railNodeID = toID;

			// 他の接続リンクを取得する
			auto linkIDSetItr = nodeLinkIDs.find( railNodeID );
			if( linkIDSetItr != nodeLinkIDs.end() && linkIDSetItr->second.size() == 2){
				auto& linkIDSet = linkIDSetItr->second;
				auto& neighborLinks = GetObjects( railLinkID, linkIDSetItr->second, railLinkMap );
				auto& neighborLink  = *(neighborLinks.begin()); //!< (メッシュ)ノードに接続するリンク

				// 属性が一致するか？
				diffField = judge_value::GetDiffRailLinkAttr( railLinkObj, neighborLink );
				if( ! diffField.IsEmpty() ){ 
					// ★エラー 2次メッシュ境界の隣接でリンクの属性が異なる
					info1.Format( _T("%s(%s)"), diffField, railLinkObj.attr.at( diffField ) );
					info2.Format( _T("%s(%s)"), diffField, neighborLink.attr.at( diffField ) );
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NotSameBorderAttr, modOperator, modDate, info1, info2 );
				}
			}
			else // ★エラー メッシュ境界で隣接リンクが不正
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgBorderLink, modOperator, modDate );
		}

		// 点列に分解して確認
		double cmpX  = 0.0, cmpY  = 0.0;
		map<pair<double,double>, vector<long>> coordsMap; // キー：xy、値：構成点番号
		for( long i = 0 ; i < railLinkPointCnt ; ++i ){

			IPointPtr ipPoint = ipRailLinkPointCol->GetPoint( i );

			// 始点とFromノードの一致を確認
			if( i == 0 && bFromOK ){
				if( ! fromRailNodeObj.IsEqual( (IGeometryPtr)ipPoint ) ) // ★エラー リンクの始点とFromノードの座標が一致しない
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::UnMatchStartFrom, modOperator, modDate );
			}

			// 終点とToノードの一致を確認
			if( i == railLinkPointCnt - 1 && bToOK ){
				if( ! toRailNodeObj.IsEqual( (IGeometryPtr)ipPoint ) ) // ★エラー リンクの終点とToノードの座標が一致しない
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::UnMatchEndTo, modOperator, modDate );
			}

			// セグメント距離確認
			double x = 0.0, y = 0.0;
			ipPoint->QueryCoords( &x, &y );
			if( i != 0 ){
				double dist = g_crd.GetDist( cmpX, cmpY, x, y );
				if( dist <= m_dist_threshold ){
					// ★エラー 微小セグメントが存在する
					info1.Format( _T("'%ld'番と'%ld'番"), i - 1, i );
					info2.Format( _T("'%lf'm以下（'%lf'm）"), m_dist_threshold, dist );
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::UnderThresholdSeg, modOperator, modDate, info1, info2 );
				}
			}

			// xyをキーとして、構成点番号を詰める
			coordsMap[make_pair(x, y)].push_back( i );

			cmpX  = x;
			cmpY  = y;
		}

		// 構成点重複の確認
		for( const auto& coord : coordsMap ){
			if( coord.second.size() > 1 ){
				// ★エラー 構成点が重複している
				auto itr = coord.second.begin();
				info1.Format( _T("'%ld'番と'%ld'番"), *(itr), *(itr + 1) );
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::DupPoint, modOperator, modDate, info1 );
			}
		}


		// 路線リストと紐づきを確認（鉄道POI種別で分類のみ、紐づいててOK）
		set<long> tempRailLineIDs;
		bool bFindRailwayLine = FindRailwayLine( railLinkID, tempRailLineIDs );
		if( railLinkClass == link_class::kDivideByPoi ){
			if( ! bFindRailwayLine ){ // ★エラー 鉄道種別コードが0(鉄道POI種別で分類)で路線リストの値がnull
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRelRailwayLine1, modOperator, modDate );
			}
		}
		else{
			if( bFindRailwayLine ){ // ★エラー 鉄道種別コードが1or2(貨物線・鉄道側線部)で路線リストの値がnull以外
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRelRailwayLine2, modOperator, modDate );
			}
		}

	}

	// 同一From/Toが存在しないか確認する
	for( const auto& dupID : dupNodeMap ){
		if( dupID.second.size() > 1 ){
			// ★エラー From/Toの組み合わせが同じリンクが複数存在する
			auto  itr = dupID.second.begin();
			long  ngID1 = *itr;
			long  ngID2 = *(++itr);
			auto  itrRailLink = railLinkMap.find( ngID1 );
			CString modOperator, modDate;
			if( itrRailLink != railLinkMap.end() ){
				modOperator = itrRailLink->second.attr.at( kOperator );
				modDate     = itrRailLink->second.attr.at( kModifyDate );
			}
			info1.Format( _T("RailLinkID:%ld"), ngID2 );
			m_output.OutputLog( kTableName, ngID1, err_level::error, err_code::LoopLinkDupFT, modOperator, modDate, info1 );
		}
	}
}

void CCheckRailwayNW::CheckRailNode( const IGeometryPtr& ipMeshGeo, const IGeometryPtr& ipBoundaryMesh, const map<long, GeoClass>& railLinkMap, const map<long, GeoClass>& railNodeMap, const map<long, GeoClass>& staPointMap, const map<long, GeoClass>& baseStaMap, const BGRTree& railLinkRTree, const BGRTree& railNodeRTree, const BGRTree& staPointRTree, const BGRTree& baseStaRTree, const IDMAP& nodeLinkIDs )
{
	using namespace railway_node;
	using namespace ipc_feature;
	CString info1, info2;

	// 鉄道ノードのチェック
	for( auto& railNode : railNodeMap ){

		long  railNodeID    = railNode.first;
		auto& railNodeObj   = railNode.second;

		long  railNodeClass = _ttol( railNodeObj.attr.at( kNodeClass_C ) );

		auto& modOperator  = railNodeObj.attr.at( ipc_feature::kOperator );
		auto& modDate      = railNodeObj.attr.at( ipc_feature::kModifyDate );

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( railNodeObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// 各属性値が定義内か確認する
		if( ! judge_value::IsDefineRailNodeClass( railNodeClass ) ){
			// ★エラー ノード種別コードが定義外
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRailNodeClass, modOperator, modDate, str_util::ToString( railNodeClass ) );
		}

		// ★ここから形状も使ったチェック

		// 単純な形状確認
		esriGeometryType eType = (railNodeObj.geometry.m_T)->GetGeometryType();
		IPointPtr ipRailNodePoint( railNodeObj.geometry.m_T );
		double railNodeX = 0.0, railNodeY = 0.0;
		if( ! railNodeObj.geometry.m_T || ! ipRailNodePoint || FAILED( ipRailNodePoint->QueryCoords( &railNodeX, &railNodeY ) ) || eType != esriGeometryPoint ){
			// ★エラー 形状が不正
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRailNodeGeom, modOperator, modDate );
			continue;
		}

		int iMeshCode = 0, iX = 0, iY = 0;
		g_crd.LLtoMesh( railNodeX, railNodeY, 2, &iMeshCode, &iX, &iY, 1);
		if( ( iX == 0 && ( iY == 0 || iY == 1000000 ) ) || ( iX == 1000000 && ( iY == 0 || iY == 1000000 ) ) ){
			// ★エラー 図郭の隅にノードがある
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::BorderCornerNode, modOperator, modDate );
		}

		// 座標からノード種別の確認
		if( iX == 0 || iX == 1000000 || iY == 0 || iY == 1000000 ){
			if( railNodeClass != node_class::kOutline ) // ★エラー 2次メッシュ境界線上にあるのに図郭上ノードになっていない
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgBorderNodeAttr, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
		}
		else{
			if( railNodeClass == node_class::kOutline ) // ★エラー 図郭上ノードが2次メッシュ境界線上に無い
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgBorderNodeGeom, modOperator, modDate );
		}

		// 別のノードが近くにないか確認（距離閾値で広げて検索）
		auto& findRailNodeIDs = getIntersectsIDs( railNodeObj.GetBox( m_dist_threshold ), railNodeRTree );

		// 見つかった鉄道ノードのループ
		for( auto& findRailNodeID : findRailNodeIDs ){

			// 自身はスキップ
			if( findRailNodeID == railNodeID )
				continue;

			auto findRailNodeItr = railNodeMap.find( findRailNodeID );
			if( findRailNodeItr == railNodeMap.end() )
				continue;

			// 距離を測る
			IPointPtr ipPoint( findRailNodeItr->second.geometry.m_T );
			double findX = 0.0, findY = 0.0;
			ipPoint->QueryCoords( &findX, &findY );
			double dist = g_crd.GetDist( findX, findY, railNodeX, railNodeY );

			if( dist < m_dist_threshold ){
				// ★エラー ノードが別のノードに接触している
				info1.Format( _T("RailNodeID:'%ld'"), findRailNodeID );
				info2.Format( _T("距離:'%lf'm(閾値:'%lf'm)"), dist, m_dist_threshold );
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::TouchNgNode, modOperator, modDate, info1, info2 );
			}
		}

		// 接続リンク数を取得
		long connectRailLinkCnt = -1;
		set<long> connectRailLinkIDs;
		auto railNodeLinkItr = nodeLinkIDs.find( railNodeID );
		if( railNodeLinkItr != nodeLinkIDs.end() ){
			connectRailLinkCnt = railNodeLinkItr->second.size();
			connectRailLinkIDs = railNodeLinkItr->second;
		}

		// 接続リンク数で確認(2本は確認不要)
		if( connectRailLinkCnt < 1 ){
			// ★エラー 鉄道リンクに接続していない（属性）
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NoConnect, modOperator, modDate, _T("属性") );
		}
		else if( connectRailLinkCnt == 1 ){
			if( ! judge_value::IsOKTerminalNode( railNodeClass ) ){
				// ★エラー 端点なのに端点ノードになっていない
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgTerminalNode, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
			}
		}
		else if( connectRailLinkCnt > 2 ){
			if( ! judge_value::IsOKBranchNode( railNodeClass ) ){
				// ★エラー 分岐なのに分岐ノードになっていない
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgBranchNode, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
			}
		}

		// 接続リンクを確認
		{
			bool bFindLink = false, bAlter = false;
			auto& findRailLinkIDs = getIntersectsIDs( railNodeObj.GetBox( 0.1 ), railLinkRTree );

			// 見つかった鉄道リンクのループ
			int i = 0;
			GeoClass neighborLink;
			for( auto& findRailLinkID : findRailLinkIDs ){

				auto findRailLinkItr = railLinkMap.find( findRailLinkID );
				if( findRailLinkItr == railLinkMap.end() )
					continue;

				// From/Toのどちらかと一致するか確認
				IPolylinePtr ipFindRailLinkLine( findRailLinkItr->second.geometry.m_T );
				IPointPtr ipFindFromPoint = ipFindRailLinkLine->GetFromPoint();
				IPointPtr ipFindToPoint   = ipFindRailLinkLine->GetToPoint();
				if( ! ipFindFromPoint || ! ipFindToPoint )
					continue;

				if( ! railNodeObj.IsEqual( ipFindFromPoint ) && ! railNodeObj.IsEqual( ipFindToPoint ) )
					continue;

				bFindLink = true;

				if( i != 0 ){
					if( ! judge_value::IsSameRailLinkAttr( findRailLinkItr->second, neighborLink ) )
						bAlter = true;
				}

				neighborLink = findRailLinkItr->second; //!< 比較用のコピー
				++i;
			}
			if( ! bFindLink ) // ★エラー 鉄道リンクに接続していない（形状）
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NoConnect, modOperator, modDate, _T("形状") );

			// 属性変化していた場合は、OKなノード属性か確認する
			if( bAlter ){
				if( ! judge_value::IsOKAlterNode( railNodeClass ) ){
					// ★エラー 接続するリンクの属性が異なるのに属性変化点ノードになっていない
					m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgAlterNode, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
				}
			}
		}

		// 端点の場合、近傍に接続していないリンクが無いか確認（距離閾値で広げて検索）
		if( railNodeClass == node_class::kTerminal ){
			// 歩行者ノードのXY取得
			double x = 0.0, y = 0.0;
			IPointPtr ipRailNodePoint( railNodeObj.geometry.m_T );
			ipRailNodePoint->QueryCoords( &x, &y );

			// 接続リンクsetを取得する
			auto connectLinkItr = nodeLinkIDs.find( railNodeID );

			auto& findRailLinkIDs = getIntersectsIDs( railNodeObj.GetBox( m_dist_threshold ), railLinkRTree );

			// 見つかった歩行者リンクのループ
			for( auto& findRailLinkID : findRailLinkIDs ){

				auto& findRailLinkItr = railLinkMap.find( findRailLinkID );
				if( findRailLinkItr == railLinkMap.end() )
					continue;

				// 接続リンクsetにあればスキップ
				if( connectLinkItr != nodeLinkIDs.end() && connectLinkItr->second.find( findRailLinkID ) != connectLinkItr->second.end() )
					continue;

				// 最近傍ポイント取得
				IProximityOperatorPtr ipProximity( findRailLinkItr->second.geometry.m_T );
				IPointPtr ipNearPoint = ipProximity->_ReturnNearestPoint( ipRailNodePoint, esriNoExtension );
				if( !ipNearPoint )
					continue;

				// 距離を測る
				double nearX = 0.0, nearY = 0.0;
				ipNearPoint->QueryCoords( &nearX, &nearY );
				double dist = g_crd.GetDist( x, y, nearX, nearY );

				// 閾値を超える距離の場合、スキップ（エンベロープのため、斜めで閾値を超えるため）
				if( dist > m_dist_threshold )
					continue;

				// ★エラー ノードが関係ないリンクに接触している
				info1.Format( _T("RailLinkID:'%ld'"), findRailLinkID );
				info2.Format( _T("距離:'%lf'm(閾値:'%lf'm)"), dist, m_dist_threshold );
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::TouchNgLink, modOperator, modDate, info1, info2 );
			}
		}

		// 路線リストと紐づきを確認（駅POIノードのみ、紐づいててOK）
		set<long> staPointIDs, lineIDs;
		bool bFindJoinLineInfo = FindJoinLineInfo( railNodeID, staPointIDs, lineIDs );
		if( railNodeClass == node_class::kStation ){
			
			// 中縮駅舎との関係を確認する
			auto& findBaseStaIDs = getIntersectsIDs( railNodeObj.GetBox( 0.1 ), baseStaRTree );

			// 見つかった中縮駅舎のループ
			bool bFindBaseSta = false;
			for( auto& findBaseStaID : findBaseStaIDs ){

				auto findBaseStaItr = baseStaMap.find( findBaseStaID );
				if( findBaseStaItr == baseStaMap.end() )
					continue;

				// 接触してたらOK
				if( ! findBaseStaItr->second.IsDisjoint( railNodeObj.geometry.m_T ) )
					bFindBaseSta = true;
			}
			if( ! bFindBaseSta ){ // 接触していない
				// ★エラー 中縮駅舎ポリゴン上に存在しない駅POIノードが存在する
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NoTouchBaseSta, modOperator, modDate );
			}

			
			if( ! bFindJoinLineInfo ) // ★エラー 駅POIノードなのに路線リストの値がnull
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRelJoinLine1, modOperator, modDate );
			else{
				// 取得したJOIN_LINE_INFOのSTATION_IDを持つものが近傍範囲内にいるか確認する（近傍閾値で広げて検索）
				auto& findStaPointIDs = getIntersectsIDs( railNodeObj.GetBox( m_neighbor_threshold ), staPointRTree );

				// 見つかった駅出入り口ポイントのループ
				bool bFindStaPoint = false;
				for( auto& findStaPointID : findStaPointIDs ){

					// IDが等しいか？
					auto itrStaPointIDs = staPointIDs.find( findStaPointID );
					if( itrStaPointIDs == staPointIDs.end() )
						continue;

					auto findStaPointItr = staPointMap.find( findStaPointID );
					if( findStaPointItr == staPointMap.end() )
						continue;

					// 距離を測る
					IPointPtr ipPoint( findStaPointItr->second.geometry.m_T );
					double findX = 0.0, findY = 0.0;
					ipPoint->QueryCoords( &findX, &findY );
					double dist = g_crd.GetDist( findX, findY, railNodeX, railNodeY );

					// 近傍閾値以下ならOK
					if( dist < m_neighbor_threshold )
						bFindStaPoint = true;
				}

				if( ! bFindStaPoint ){ // 近傍範囲内で見つからなかった
					// ★エラー 駅POIノードの近傍に対応する駅POIが存在しない
					info1.Format( _T("閾値:'%lf'm"), m_neighbor_threshold );
					m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::TooFarStaPoint, modOperator, modDate, info1 );
				}

				// 取得したLINE_IDが全て接続リンクに存在するか確認
				set<long> connectLineIDs;
				for( const auto& connectRailLinkID : connectRailLinkIDs ){
					// 接続しているRailLinkに紐づくLineIDを全て取得する
					set<long> tempLineIDs;
					if( ! FindRailwayLine( connectRailLinkID, tempLineIDs ) )
						continue;
					for( const auto& tempLineID : tempLineIDs )
						connectLineIDs.insert( tempLineID );
				}
				for( const auto& lineID : lineIDs ){
					// ないものがあったらエラー
					if( connectLineIDs.end() != connectLineIDs.find( lineID ) )
						continue;
										
					// ★エラー 接続しているリンクに存在しない路線IDがノードに存在している
					info1.Format(_T("%ld(%s)"), lineID, GetLineName( lineID ) );
					m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRelJoinLine3, modOperator, modDate, info1 );
				}
			}
		}
		else{
			if( bFindJoinLineInfo ) // ★エラー 駅POIノード以外のノードで路線リストの値がnull以外
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRelJoinLine2, modOperator, modDate );
		}

		


	}
}

bool CCheckRailwayNW::FindJoinLineInfo( long railNodeID, set<long>& stationIDs, set<long>& lineIDs )
{
	auto itrRel = m_relRailwayNode.find( railNodeID );
	if( itrRel == m_relRailwayNode.end() )
		return false;

	for( auto& rel : itrRel->second ){
		long joinLineInfoID = _ttol( rel.attr.at( rel_railway_node::kJoinLineInfoID ) );
		auto itrJoinLineInfo = m_joinLineInfo.find( joinLineInfoID );
		if( itrJoinLineInfo == m_joinLineInfo.end() )
			continue;

		stationIDs.insert( _ttol( itrJoinLineInfo->second.attr.at( join_line_info::kStationID ) ) );
		lineIDs.insert( _ttol( itrJoinLineInfo->second.attr.at( join_line_info::kLineID ) ) );
	}
	return stationIDs.empty()? false : true;
}

bool CCheckRailwayNW::FindRailwayLine( long railLinkID, set<long>& railwayLineIDs )
{
	auto itrRel = m_relRailwayLink.find( railLinkID );
	if( itrRel == m_relRailwayLink.end() )
		return false;

	for( auto& rel : itrRel->second ){
		long railwayLineID = _ttol( rel.attr.at( rel_railway_link::kRailwayLineID ) );
		auto itrRailwayLine = m_railwayLine.find( railwayLineID );
		if( itrRailwayLine == m_railwayLine.end() )
			continue;

		railwayLineIDs.insert( railwayLineID );
	}
	return railwayLineIDs.empty()? false : true;
}

list<GeoClass> CCheckRailwayNW::GetObjects( long oID, const set<long>& oIDs, const map<long, GeoClass>& geoMap )
{
	list<GeoClass> retList;

	for( const auto& id : oIDs ){

		// 自身を除いてgeoMapから探して、retListに格納
		if( oID == id )
			continue;

		auto itr = geoMap.find( id );
		if( itr == geoMap.end() )
			continue;

		retList.push_back( itr->second );
	}

	return retList;
}

void CCheckRailwayNW::QueryCheck()
{

	// 鉄道リンク関連テーブルの野良チェック
	CheckNonRel( (ITablePtr)m_ipRailLinkFC, railway_link::kTableName, rel_railway_link::kTableName, m_relRailwayLink );

	// 鉄道ノード関連テーブルの野良チェック
	CheckNonRel( (ITablePtr)m_ipRailNodeFC, railway_node::kTableName, rel_railway_node::kTableName, m_relRailwayNode );
	
	// 同一路線のリンク連続性チェック
	SameLineSeqLinkChk();

	// 路線内の駅番号のチェック
	CheckStaNumInLine();

	// 鉄道ノードと紐付く乗り入れ路線テーブルの存在チェック（bug12289で追加）
	checkRelRecordExstence(railway_node::kTableName, m_relRailwayNode, m_joinLineInfo);

	// 鉄道リンクと紐付く鉄道路線テーブルの存在チェック（bug12289で追加）
	checkRelRecordExstence(railway_link::kTableName, m_relRailwayLink, m_railwayLine);
}

void CCheckRailwayNW::CheckNonRel( const ITablePtr& ipTable, const CString& fcTableName, const CString& relTableName, const map<long, vector<recClass>>& relAll )
{
	err_code::ECode eCode;
	if( 0 == fcTableName.CompareNoCase( railway_link::kTableName ) )
		eCode = err_code::NonRelRailLink;
	else
		eCode = err_code::NonRelRailNode;

	set<long> railOIDs;
	if( ! GetOIDAll( ipTable, fcTableName, railOIDs ) ){
		m_output.OutputStdErr( err_level::fatal, err_code::GetOIDAllFail, fcTableName );
		m_output.OutputLog( err_level::fatal, err_code::GetStaPoiFail, fcTableName );
	}

	for( const auto& rel : relAll ){
		long relRwID = rel.first;
		if( railOIDs.end() != railOIDs.find( relRwID ) )
			continue; // 見つかればOK

		// エラー出力
		for( const auto& relRec : rel.second ){
			// ★エラー 紐づく鉄道リンクが存在しない
			// ★エラー 紐づく鉄道ノードが存在しない
			long relOID   = _ttol( relRec.attr.at( ipc_feature::kObjectID ) );
			//auto& modOperator = relRec.attr.at( ipc_feature::kOperator );
			//auto& modDate     = relRec.attr.at( ipc_feature::kModifyDate );
			//m_output.OutputLog( relTableName, relOID, err_level::error, eCode, modOperator, modDate, str_util::ToString( relRwID ) );
			m_output.OutputLog( relTableName, relOID, err_level::error, eCode, _T(""), _T(""), str_util::ToString( relRwID ) );
		}
	}
}

bool CCheckRailwayNW::GetOIDAll( const ITablePtr& ipTable, const CString& tableName, std::set<long>& oIDs )
{
	CString subStr;
	subStr.Format( _T("%s"), ipc_feature::kObjectID );

	_ICursorPtr ipCursor = ipTable->_Search( AheInitQueryFilter( nullptr, subStr, nullptr ), VARIANT_TRUE );
	if( ! ipCursor )
		return false;
	_IRowPtr ipRow;

	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){
		oIDs.insert( ipRow->GetOID() );
	}

	return true;
}

void CCheckRailwayNW::SameLineSeqLinkChk()
{
	map<long, set<long>> lineIDs;
	for( const auto& rel : m_relRailwayLink ){
		long rwLinkID = rel.first;
		for( const auto& rel2 : rel.second ){
			long lineID = _ttol( rel2.attr.at( rel_railway_link::kRailwayLineID ) );
			lineIDs[lineID].insert( rwLinkID );
		}
	}

	CString subStr;
	subStr.Format( _T("%s,%s,%s"), ipc_feature::kObjectID, railway_link::kFromNodeOID, railway_link::kToNodeOID );
	for( const auto& line : lineIDs ){

		// 同じLineIDを持つものをグループ化する
		IDMAP nodeNodeIDs, nodeLinkIDs;
		set<long> nodeIDs;
		auto& linkIDs = line.second;
		auto& strWheres = CreateWhereByList( linkIDs, ipc_feature::kObjectID );

		for( const auto& strWhere : strWheres ){

			IFeatureCursorPtr ipCursor = m_ipRailLinkFC->_Search( AheInitQueryFilter( NULL, subStr, strWhere ), VARIANT_TRUE );
			if( ! ipCursor )
				continue;

			IFeaturePtr ipFeature;
			while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

				long oID = ipFeature->GetOID();
				long fromID = _ttol( str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( railway_link::kFromNodeOID ) ) ) );
				long toID   = _ttol( str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( railway_link::kToNodeOID ) ) ) );

				nodeNodeIDs[fromID].insert( toID );
				nodeNodeIDs[toID].insert( fromID );

				nodeLinkIDs[fromID].insert( oID );
				nodeLinkIDs[toID].insert( oID );

				nodeIDs.insert( fromID );
				nodeIDs.insert( toID );
			}
		}

		// ノード列を作成して、差異を確認する
		long startID = *(nodeIDs.begin());
		set<long> relNodeIDs = GetRelNodeIDs( startID, nodeNodeIDs );
		if( nodeIDs.size() == relNodeIDs.size() )
			continue; // サイズが同じ＝全て繋がっているのでOK

		set<long> ngLinkIDs;
		for( const auto& id : nodeIDs ){ // 違う子を探す
			auto itr = relNodeIDs.find( id );
			if( itr != relNodeIDs.end() )
				continue;

			// NGノードに紐づくリンクを取得
			auto ngItr = nodeLinkIDs.find( id );
			for( const auto& ngLinkID : ngItr->second ){
				ngLinkIDs.insert( ngLinkID );
			}
		}

		for( const auto& ngLinkID : ngLinkIDs ){
			// ★エラー 駅POIノード以外のノードで路線リストの値がnull以外
			//IFeaturePtr ipFeature = m_ipRailLinkFC->_GetFeature( ngLinkID );
			//auto& modOperator = str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kOperator ) ) );
			//auto& modDate     = str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kModifyDate ) ) );
			//m_output.OutputLog( railway_link::kTableName, ngLinkID, err_level::error, err_code::NotOnlySeqLine, modOperator, modDate, str_util::ToString( startID ) );
			m_output.OutputLog( railway_link::kTableName, ngLinkID, err_level::error, err_code::NotOnlySeqLine, _T(""), _T(""), str_util::ToString( startID ) );
		}
	}
}

set<long> CCheckRailwayNW::GetRelNodeIDs( long nodeID, const IDMAP& nodeNodeIDs )
{
	set<long> relNodeIDs;
	relNodeIDs.insert( nodeID );
	while(1){
		int oldSize = relNodeIDs.size();
		for( auto& nodeNodeID : nodeNodeIDs ){

			bool bFind = false;
			if( relNodeIDs.find( nodeNodeID.first ) != relNodeIDs.end() )
				bFind = true;
			else{
				for( auto& walkNodeID : nodeNodeID.second ){
					if( relNodeIDs.find( walkNodeID ) != relNodeIDs.end() ){
						bFind = true;
						break;
					}
				}
			}
			if( bFind ){
				relNodeIDs.insert( nodeNodeID.first );
				relNodeIDs.insert( nodeNodeID.second.begin(), nodeNodeID.second.end() );
			}

		}

		int newSize = relNodeIDs.size();
		if( oldSize == newSize )
			break;
	}

	return relNodeIDs;
}

void CCheckRailwayNW::CheckStaNumInLine()
{
	CString info1, info2;
	map<long, map<CString, long>> rwNodeMap; // キー：路線ID、値：map< キー：駅番号、値：rwNodeID >
	for( const auto& relRailwayNode : m_relRailwayNode ){
		long rwNodeID = relRailwayNode.first;
		for( const auto& relNodeRec : relRailwayNode.second ){
			auto& stationNum = relNodeRec.attr.at( rel_railway_node::kStationNo );
			if( stationNum.IsEmpty() )
				continue; // 駅番号が空の場合はスキップ
			long  joinLineID = _ttol( relNodeRec.attr.at( rel_railway_node::kJoinLineInfoID ) );
			long  relNodeOID = _ttol( relNodeRec.attr.at( ipc_feature::kObjectID ) );

			// 鉄道路線IDを取得
			auto joinLineItr = m_joinLineInfo.find( joinLineID );
			if( joinLineItr == m_joinLineInfo.end() )
				continue;

			long lineID = _ttol( joinLineItr->second.attr.at( join_line_info::kLineID ) );
			if( rwNodeMap[lineID].count( stationNum ) ){
				// ★エラー 駅番号が重複している
				info1.Format( _T("%ld(%s)"), lineID, GetLineName(lineID) );
				info2.Format( _T("RailNodeID:%ld([%s])"), rwNodeMap.at( lineID ).at( stationNum ), stationNum );
				//IFeaturePtr ipFeature = m_ipRailNodeFC->_GetFeature( rwNodeID );
				//auto& modOperator = str_util::ToString( ipFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kOperator ) ) );
				//auto& modDate     = str_util::ToString( ipFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kModifyDate ) ) );
				//m_output.OutputLog( railway_node::kTableName, rwNodeID, err_level::error, err_code::DupStationNum, modOperator, modDate, info1, stationNum );
				m_output.OutputLog( railway_node::kTableName, rwNodeID, err_level::error, err_code::DupStationNum, _T(""), _T(""), info1, info2 );
			}
			else{
				rwNodeMap[lineID].emplace( stationNum, rwNodeID );
			}
		}
	}

	// 路線IDごとに駅番号の並び順を確認する
	for( const auto& line : rwNodeMap ){
		long lineID = line.first;
		CString compPreFix;
		long    compNumber = -1;
		int     i = 0;
		for( const auto& staNum : line.second ){
			int numPos = staNum.first.FindOneOf( _T("0123456789") );
			if( numPos < 0 ){
				// ★エラー 駅番号に数字が存在しない
				info1.Format( _T("%ld(%s)"), lineID, GetLineName(lineID) );
				m_output.OutputLog( railway_node::kTableName, staNum.second, err_level::error, err_code::NoNumberStaNum, _T(""), _T(""), info1, staNum.first );
			}

			CString preFix = staNum.first.Left( numPos );
			long number    = _ttol( staNum.first.Mid( numPos ) );
			if( i != 0 && ( 0 != preFix.Compare( compPreFix ) || number != compNumber + 1 ) ){
				// ★エラー 駅番号の並び順が不正
				info1.Format( _T("%ld(%s)"), lineID, GetLineName(lineID) );
				info2.Format(_T("%s("), staNum.first );
				for( const auto& errSta : line.second )
					info2.AppendFormat(_T("'%s' "), errSta.first );
				info2 += _T(")");
				m_output.OutputLog( railway_node::kTableName, staNum.second, err_level::error, err_code::NgSeqStaNum, _T(""), _T(""), info1, info2 );
				break;
			}
			compPreFix = preFix;
			compNumber = number;
			++i;
		}
	}
}

// 指定した関連テーブルと紐付く鉄道POIのテーブルのレコードが存在するかチェックする
void CCheckRailwayNW::checkRelRecordExstence(const CString& featureClassName, const map<long, vector<recClass>>& relRecMap, const std::map<long, recClass>& poiRecMap){
	err_code::ECode eCode;
	CString targetFieldName;

	// 鉄道ノードの場合はJOIN_LINE_INFO_IDをチェックする
	if(featureClassName.CompareNoCase(railway_node::kTableName)==0){
		eCode = err_code::NonJoinLinkInfo;
		targetFieldName = rel_railway_node::kJoinLineInfoID;
	}
	// 鉄道リンクの場合はRAILWAY_LINE_IDをチェックする
	else if(featureClassName.CompareNoCase(railway_link::kTableName)==0){
		eCode = err_code::NonRailwayLine;
		targetFieldName = rel_railway_link::kRailwayLineID;
	}
	else
		return;

	for(const auto& rel : relRecMap){
		for(const auto& relRec : rel.second){
			CString poiRecOID = relRec.attr.at(targetFieldName);
			if(poiRecMap.find(_ttol(poiRecOID)) != poiRecMap.end())
				continue; // 見つかればOK

			// ★エラー 紐付く乗り入れ路線が存在しない
			// ★エラー 紐付く鉄道路線が存在しない
			m_output.OutputLog(featureClassName, rel.first, err_level::error, eCode, _T(""), _T(""), poiRecOID);
		}
	}
}
