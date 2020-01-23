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

#include <atltime.h>
#include <VersionInfo.h>

// boost
#include <boost/assign/list_of.hpp>

// ArcHelperEx
#include <GlobalFunctions.h>
#include <AheGlobalsMisc.h>
#include <AheLayerFunctions.h>
#include <AheGeometryOp.h>

// useful_headders
#include <TDC/useful_headers/str_util.h>

// sindylib_base
#include <TDC/sindylib_base/TableContainer.h>

#include "Logger.h"
#include "CreateRoadLinkage.h"
#include "FeatureReader.h"
#include "RoadLinkMatcher.h"
#include "Util.h"

namespace {
	// 道路リンクのマッチング範囲
	const double MATCHING_METER = 30.0;
	// 道路リンクの読み込み時の検索距離
	const double SEARCH_METER = MATCHING_METER + 10.0;
}

// 差分FGDBスキーマ
namespace diff_db {
	const CString kChangeType = _T( "CHANGE_TYPE" );	// 差分タイプ
	const CString kObjectIdOrg = _T( "OBJECTID_ORG" );	// 元データ側OBJECTID

	namespace change_type_code {
		enum ECode {
			kNew = 1,		// 追加
			kDelete = 2,	// 削除
			kUpdate = 3,	// 更新
		};
	}
}

bool CCreateRoadLinkage::execute()
{
	if(! initLog()){
		finish(false);
		return false;
	}

	if(! openTable()){
		finish(false);
		return false;
	}

	if(! startEdit()){
		finish(false);
		return false;
	}

	if(! createLinkage()){
		abortEdit();
		finish(false);
		return false;
	}

	stopEdit();
	finish(true);

	return true;
}


bool CCreateRoadLinkage::initLog() const
{
	// 処理ログファイルオープン
	if(! RUNLOGGER.Initialize( m_Args.m_LogDir ))
	{
		std::cerr <<  "処理ログファイルのオープンに失敗しました : " <<  uh::toStr( m_Args.m_LogDir.c_str() ) << std::endl;
		return false;
	}

	// ツール実行情報出力
	RUNLOGGER.Log( _T("接続先DB(道路):\t%s"),               m_Args.m_RoadDB.c_str()          );
	RUNLOGGER.Log( _T("接続先DB(ADAM):\t%s"),               m_Args.m_AdamDB.c_str()          );
	RUNLOGGER.Log( _T("接続先DB(形状差分):\t%s"),            m_Args.m_DiffDB.c_str()          );
	RUNLOGGER.Log( _T("メッシュリスト:\t%s"),               m_Args.m_MeshListStr.c_str()     );
	RUNLOGGER.Log( _T("紐付け対象道路リンク検索条件:\t%s"), m_Args.m_TargetRoadWhere.c_str() );
	RUNLOGGER.Log( _T("ログ出力フォルダパス:\t%s"),         m_Args.m_LogDir.c_str()          );

	// エラーログファイルオープン
	if(! ERRLOGGER.Initialize( m_Args.m_LogDir ))
	{
		RUNLOGGER.Error( _T("エラーログファイルのオープンに失敗しました : ") + m_Args.m_LogDir );
		return false;
	}

	RUNLOGGER.Info(_T("実行開始 "));

	return true;
}

bool CCreateRoadLinkage::openTable()
{
	using namespace sindy::schema;
	using namespace sindy::schema::category;
	using namespace sindy::schema::adam_v2;
	using namespace boost::assign;

	// 道路DB接続
	IWorkspacePtr road_ws = sindy::create_workspace( m_Args.m_RoadDB.c_str() );
	if(! road_ws){
		RUNLOGGER.Error( _T("道路DBへの接続に失敗しました : ") + m_Args.m_RoadDB );
		return false;
	}
	// 道路関連テーブルオープン
	if(FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(road_link::kTableName), &m_RoadLinkTable))){
		RUNLOGGER.Error( _T("道路リンクのオープンに失敗しました"));
		return false;
	}
	if(FAILED(IFeatureWorkspacePtr(road_ws)->OpenTable(CComBSTR(basemesh::kTableName), &m_BaseMeshTable)))
	{
		m_BaseMeshTable = AheOpenTableByTableName( road_ws, basemesh::kTableName, TRUE );
		if( ! m_BaseMeshTable ){
			RUNLOGGER.Error( _T("ベースメッシュのオープンに失敗しました"));
			return false;
		}
	}

	// ADAMDB接続
	if(! m_Workspace.open(m_Args.m_AdamDB.c_str())){
		RUNLOGGER.Error( _T("ADAM DBへの接続に失敗しました : ") + m_Args.m_AdamDB );
		return false;
	}

	// SDEならば子バージョンを作成して編集を行うモードにする
	if(m_Workspace.isSDE())
	{
		const std::vector<CString> edit_tables = list_of( CString( rel_road_link_lane_link::kTableName ) );
		if(! m_Workspace.childCreateModeOn( getChildVersionName(), edit_tables ) )
		{
			RUNLOGGER.Error( _T("道路リンク－車線リンク関連テーブルはバージョン対応ではありません") );
			return false;
		}
	}

	// ADAM関連テーブルオープン
	m_LaneLinkTable = m_Workspace.openTable(lane_link::kTableName);
	if(! m_LaneLinkTable){
		RUNLOGGER.Error( _T("車線リンクのオープンに失敗しました"));
		return false;
	}
	m_RelRoadLinkLaneLinkTable = m_Workspace.openTable(rel_road_link_lane_link::kTableName);
	if(! m_RelRoadLinkLaneLinkTable){
		RUNLOGGER.Error( _T("道路リンク－車線リンク関連テーブルのオープンに失敗しました"));
		return false;
	}

	// 道路リンク－車線リンク関連テーブル、車線リンクのフィールドマップ、ベースメッシュのフィールドマップを取得する
	auto getFieldMap = []( ITablePtr table ){
		sindy::CFieldMap fm;
		fm.CreateFieldMap( table->GetFields() );
		return fm;
	};
	m_RelRoadLinkLaneLinkFM = getFieldMap( m_RelRoadLinkLaneLinkTable );
	m_LaneLinkFM = getFieldMap( m_LaneLinkTable );
	m_BaseMeshFM = getFieldMap( m_BaseMeshTable );

	// 車線リンクの空間参照を取得する
	m_LaneSpRef = IGeoDatasetPtr( m_LaneLinkTable )->GetSpatialReference();

	// 形状差分FGDBが指定されていなければここでreturn
	if( m_Args.m_DiffDB.empty() )
		return true;


	// 形状差分FGDB接続
	IWorkspacePtr ipDiffWS = sindy::create_workspace( m_Args.m_DiffDB.c_str() );
	if( !ipDiffWS ) {
		RUNLOGGER.Error( _T( "形状差分FGDBへの接続に失敗しました : " ) + m_Args.m_DiffDB );
		return false;
	}

	// 形状差分テーブルオープン
	ITablePtr ipRoadTable, ipLaneTable;
	IFeatureWorkspacePtr( ipDiffWS )->OpenTable( CComBSTR( road_link::kTableName ), &ipRoadTable );
	IFeatureWorkspacePtr( ipDiffWS )->OpenTable( CComBSTR( lane_link::kTableName ), &ipLaneTable );
	// どちらか片方だけ取れている状況でなければ全てNG
	if( !ipRoadTable && !ipLaneTable ) {
		RUNLOGGER.Error( _T( "形状差分テーブルのオープンに失敗しました" ) );
		return false;
	}
	if( ipRoadTable && ipLaneTable ) {
		RUNLOGGER.Error( _T( "道路リンクと車線リンクの形状差分が両方とも存在しています" ) );
		return false;
	}
	m_DiffTable = ipRoadTable ? ipRoadTable : ipLaneTable;

	// 形状差分テーブルのフィールドマップを取得する
	m_DiffFM = getFieldMap( m_DiffTable );

	// 形状差分テーブルの空間参照を取得する
	m_DiffSpRef = IGeoDatasetPtr( m_DiffTable )->GetSpatialReference();

	return true;
}

bool CCreateRoadLinkage::startEdit()
{
	if( ! m_Workspace.startEditing() ){
		RUNLOGGER.Error( _T("編集開始に失敗しました") );
		return false;
	}
	if( ! m_Workspace.startEditOperation() ) {
		m_Workspace.stopEditing(false);
		RUNLOGGER.Error( _T("編集開始に失敗しました") );
		return false;
	}
	return true;
}

void CCreateRoadLinkage::stopEdit()
{
	if (! m_Workspace.stopEditOperation()){
		m_Workspace.stopEditing(false);
		RUNLOGGER.Error( _T("編集終了に失敗しました") );
		return;
	}
	if (! m_Workspace.stopEditing(true)){
		RUNLOGGER.Error( _T("編集終了に失敗しました") );
	}
}

void CCreateRoadLinkage::abortEdit()
{
	RUNLOGGER.Error( _T("編集に失敗しました") );
	if (! m_Workspace.abortEditOperation() ) {
		m_Workspace.stopEditing(false);
		RUNLOGGER.Error( _T("編集終了に失敗しました") );
		return;
	}
	if (! m_Workspace.stopEditing(false)) {
		RUNLOGGER.Error( _T("編集終了に失敗しました") );
	}
}

bool CCreateRoadLinkage::createLinkage()
{
	// メッシュポリゴン取得
	std::map<long, IGeometryPtr> meshGeoMap;
	if( !getMeshPolygon( m_Args.m_MeshList, meshGeoMap ) )
		return false;

	// メッシュ単位で車線リンク->道路リンクのマッチング判定を行う
	for( const auto& mesh : meshGeoMap )
	{
		std::vector<IFeaturePtr> lanelinkFeatures;	// 紐付け対象の車線リンク
		std::multimap<CString, long> lane2Road;		// 削除した関連（Key：車線リンク、Value：道路リンク）

		// 形状差分FGDB指定なし
		if( !m_DiffTable )
		{
			if( !run4NotUseDiff( mesh, lanelinkFeatures ) )
				return false;
		}
		// 形状差分FGDB指定あり
		else
		{
			if( !run4UseDiff( mesh, lanelinkFeatures, lane2Road ) )
				return false;
		}

		std::multimap<CString, long> existRel;		// 関連テーブルに存在する関連（Key：車線リンク、Value：道路リンク）

		// 重複インポート防止対策処理
		preventDuplication( lanelinkFeatures, existRel );

		// 道路リンク - 車線リンクの紐付け処理
		if( !createRel( lanelinkFeatures, lane2Road, existRel, mesh.first ) )
			return false;
	}

	return true;
}

bool CCreateRoadLinkage::run4NotUseDiff( const std::pair<long, IGeometryPtr>& mesh, std::vector<IFeaturePtr>& lanelinkFeatures ) const
{
	// 検索対象メッシュ分の車線リンク読み込み
	FeatureReader laneLinkReader( m_LaneLinkTable, m_BaseMeshTable );
	if( !laneLinkReader.read( mesh.second ) )
	{
		RUNLOGGER.Error( _T( "車線リンクの読み込みに失敗しました : " ) + uh::toTStr( std::to_string( mesh.first ) ) );
		return false;
	}
	// 1メッシュ分の車線リンク取得
	lanelinkFeatures = laneLinkReader.getFeatures();

	return true;
}

bool CCreateRoadLinkage::run4UseDiff(
	const std::pair<long, IGeometryPtr>& mesh,
	std::vector<IFeaturePtr>& lanelinkFeatures,
	std::multimap<CString, long>& lane2Road
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace Util;

	// 1メッシュ分の差分レコード取得
	std::map<long, std::vector<IFeaturePtr>> diffMap;
	if( !getDiff( diffMap, mesh.second ) )
		return false;

	std::map<CString, Info4ErrLog> lane4DeleteRel;	// 車線リンクと情報（関連削除対象）

	// 道路リンクか車線リンクかで処理を分ける
	if( isRoadDiff() )
	{
		// 形状差分を分析
		std::vector<long> orgOids;
		analyzeRoadDiff( diffMap, orgOids );

		// 紐付きのある関連レコードのみ削除
		if( !deleteRel( getWhereClauseINSearch( orgOids, rel_road_link_lane_link::kRoadLinkID ), lane2Road ) )
			return false;

		// 紐付け対象の車線リンクと、削除された車線リンクの情報を取得する
		if( !getLinkInfo4CreateAndDeleted( lane2Road, mesh.first, lanelinkFeatures, lane4DeleteRel ) )
			return false;
	}
	else
	{
		// 検索対象メッシュ分の車線リンク読み込み
		FeatureReader laneLinkReader( m_LaneLinkTable, m_BaseMeshTable );
		if( !laneLinkReader.read( mesh.second ) )
		{
			RUNLOGGER.Error( _T( "車線リンクの読み込みに失敗しました : " ) + uh::toTStr( std::to_string( mesh.first ) ) );
			return false;
		}
		std::map<CString, IFeaturePtr> baseLane;
		getGid2Feature( laneLinkReader.getFeatures(), m_LaneLinkFM, baseLane );

		// 形状差分を分析
		analyzeLaneDiff( diffMap, baseLane, lanelinkFeatures, lane4DeleteRel );

		std::set<CString> gids;
		for( const auto& pair : lane4DeleteRel )
			gids.emplace( pair.first );

		// GID が一致する道路リンク－車線リンク関連テーブルのレコードを削除
		if( !deleteRel( getWhereClauseINSearch( gids, rel_road_link_lane_link::kLaneLinkGID ), lane2Road ) )
			return false;
	}

	// 削除した関連についてエラーログに出力
	writeDeletedRelInfo( lane4DeleteRel, lane2Road, mesh.first );

	return true;
}

void CCreateRoadLinkage::analyzeLaneDiff(
	const std::map<long, std::vector<IFeaturePtr>>& diffMap,
	const std::map<CString, IFeaturePtr>& baseLane,
	std::vector<IFeaturePtr>& lanelinkFeatures,
	std::map<CString, Info4ErrLog>& lane4DeleteRel
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace diff_db::change_type_code;
	using namespace Util;

	// 差分の種類毎にループ
	for( const auto& pair : diffMap )
	{
		// 追加・更新
		if( pair.first == kNew || pair.first == kUpdate )
		{
			for( const auto& feature : pair.second )
			{
				CString gid = feature->GetValue( m_DiffFM.HasField( lane_link::kGlobalID ) ).bstrVal;
				if( 0 == baseLane.count( gid ) )
					continue;
				lanelinkFeatures.emplace_back( baseLane.at( gid ) );
			}
		}

		// 削除・更新
		if( pair.first == kDelete || pair.first == kUpdate )
		{
			for( const auto& feature : pair.second )
			{
				CString gid = feature->GetValue( m_DiffFM.HasField( lane_link::kGlobalID ) ).bstrVal;

				Info4ErrLog info;
				if( pair.first == kUpdate )
				{
					info.laneOid = baseLane.at( gid )->GetOID();
					info.msg = _T( "再紐付実施" );
					getXY( baseLane.at( gid ), info.x, info.y );
				}
				else
				{
					info.laneOid = feature->GetValue( m_DiffFM.HasField( diff_db::kObjectIdOrg ) ).lVal; // 旧OID
					info.msg = _T( "関連削除のみ実施" );
					getXY( feature, info.x, info.y, m_LaneSpRef ); // ADAM側の空間参照を当てる
				}

				lane4DeleteRel.emplace( gid, info );
			}
		}
	}
}

void CCreateRoadLinkage::analyzeRoadDiff(
	const std::map<long, std::vector<IFeaturePtr>>& diffMap,
	std::vector<long>& orgOids
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace diff_db::change_type_code;
	
	// 差分の種類毎にループ
	for( const auto& pair : diffMap )
	{
		// 削除・更新でなければスキップ（追加は使わない）
		if( pair.first != kDelete && pair.first != kUpdate )
			continue;

		for( const auto& feature : pair.second )
		{
			long orgOid = feature->GetValue( m_DiffFM.HasField( diff_db::kObjectIdOrg ) ).lVal;
			orgOids.emplace_back( orgOid );
		}
	}
}

bool CCreateRoadLinkage::deleteRel( const std::vector<CString>& wheres, std::multimap<CString, long>& lane2Road ) const
{
	using namespace sindy::schema::adam_v2;

	for( const auto& where : wheres )
	{
		_ICursorPtr ipUpdateCursor;
		if( FAILED( m_RelRoadLinkLaneLinkTable->Update(
			AheInitQueryFilter( nullptr, nullptr, where ), VARIANT_FALSE, &ipUpdateCursor ) ) &&
			!ipUpdateCursor )
		{
			RUNLOGGER.Error( _T( "道路リンク－車線リンク関連テーブルの読み込みに失敗しました" ) );
			return false;
		}

		_IRowPtr ipRow;
		while( S_OK == ipUpdateCursor->NextRow( &ipRow ) && ipRow )
		{
			// 削除した関連情報を後で出力するため保持
			CString lane = ipRow->GetValue( m_RelRoadLinkLaneLinkFM.HasField( rel_road_link_lane_link::kLaneLinkGID ) ).bstrVal;
			long road = ipRow->GetValue( m_RelRoadLinkLaneLinkFM.HasField( rel_road_link_lane_link::kRoadLinkID ) ).lVal;
			lane2Road.emplace( lane, road );

			if( FAILED( ipUpdateCursor->DeleteRow() ) )
			{
				RUNLOGGER.Error( _T( "道路リンク－車線リンク関連テーブルの削除に失敗しました" ) );
				return false;
			}
		}
	}

	return true;
}

bool CCreateRoadLinkage::getLinkInfo4CreateAndDeleted(
	const std::multimap<CString, long>& lane2Road,
	long meshCode,
	std::vector<IFeaturePtr>& lanelinkFeatures,
	std::map<CString, Info4ErrLog>& lane4DeleteRel
) const
{
	using namespace sindy::schema::adam_v2;
	using namespace Util;

	std::set<CString> gids;
	for( const auto& pair : lane2Road )
		gids.emplace( pair.first );

	// 道路差分が紐づいていた車線リンクのフィーチャーを取得
	FeatureReader laneLinkReader( m_LaneLinkTable, m_BaseMeshTable );
	std::map<CString, IFeaturePtr> linkedLane;
	for( const auto& where : getWhereClauseINSearch( gids, lane_link::kGlobalID ) )
	{
		if( !laneLinkReader.read( where ) )
		{
			RUNLOGGER.Error( _T( "車線リンクの読み込みに失敗しました : " ) + uh::toTStr( std::to_string( meshCode ) ) );
			return false;
		}
		getGid2Feature( laneLinkReader.getFeatures(), m_LaneLinkFM, linkedLane );
	}

	for( const auto& gid : gids )
	{
		// 紐づく車線リンクが既に削除されている場合は再紐付けまでできない
		// 差分FGDBは道路なので、車線リンクの旧OIDもXYも取れない
		// 車線FGDB→道路FGDBの順に実行すればこの問題は起きないが、運用方法が決まっていないとのことなので、
		// 一旦この対応としておく（対応方法については整備側と合意済み）
		if( 0 == linkedLane.count( gid ) )
		{
			Info4ErrLog info( -1, 0.0, 0.0, _T( "関連削除のみ実施" ) );
			lane4DeleteRel.emplace( gid, info );
			continue;
		}
		// 紐付け対象を確保
		lanelinkFeatures.emplace_back( linkedLane.at( gid ) );

		// ログ出力用に関連情報を確保
		double dX = 0.0, dY = 0.0;
		getXY( linkedLane.at( gid ), dX, dY );
		Info4ErrLog info( linkedLane.at( gid )->GetOID(), dX, dY, _T( "再紐付実施" ) );
		lane4DeleteRel.emplace( gid, info );
	}

	return true;
}

void CCreateRoadLinkage::writeDeletedRelInfo( const std::map<CString, Info4ErrLog>& lane4DeleteRel, const std::multimap<CString, long>& lane2Road, long meshCode ) const
{
	using namespace uh;

	// 削除した関連情報（車線リンクに対して紐付いていた道路リンク）をエラーログに出力
	for( const auto& pair : lane4DeleteRel )
	{
		// 道路リンクを取得
		std::set<long> oids;
		auto range = lane2Road.equal_range( pair.first );
		for( auto it = range.first; it != range.second; ++it )
			oids.emplace( it->second );

		CString strOids = str_util::join( oids, _T( "," ) );
		if( strOids.IsEmpty() )
			continue; // 無ければ削除もしていないのでスキップ

		ERRLOGGER.Info( pair.second.laneOid, pair.first,
			meshCode, pair.second.x, pair.second.y,
			str_util::format( _T( "%s_旧道路OID:%s" ), pair.second.msg, strOids ) );
	}
}

void CCreateRoadLinkage::preventDuplication( std::vector<IFeaturePtr>& lanelinkFeatures, std::multimap<CString, long>& existRel ) const
{
	if( m_DiffTable && isRoadDiff() )
	{
		// 形状差分FGDBが道路の時は、紐付けが残っていても再紐付けしたいので絞り込みはしない
		// その代わり、重複レコードをインポートしないよう、この時点で関連のキャッシュを取る
		std::set<CString> dummy; // ダミー
		getExistRel( lanelinkFeatures, dummy, existRel );
	}
	else
	{
		// 道路リンク－車線リンク関連テーブルに紐付けが存在しないレコードのみに絞り込む
		extractNotLinkedFeature( lanelinkFeatures );
	}
}

void CCreateRoadLinkage::extractNotLinkedFeature( std::vector<IFeaturePtr>& lane_features ) const
{
	using namespace sindy::schema::adam_v2;

	std::set<CString> existGids;
	std::multimap<CString, long> dummy; // ダミー
	getExistRel( lane_features, existGids, dummy );
	long indexLaneLinkGid = m_LaneLinkFM.FindField( lane_link::kGlobalID );

	// 道路リンク－車線リンク関連テーブルに紐付け情報が存在する車線リンクを、処理対象リスト(lane_features)から削除
	lane_features.erase( std::remove_if(
		std::begin( lane_features ), std::end( lane_features ), [&indexLaneLinkGid, &existGids]( const IFeaturePtr& feature )
	{
		CComVariant vaGid = feature->GetValue( indexLaneLinkGid );
		if( vaGid.vt == VT_NULL )
			return true;
		return existGids.count( vaGid.bstrVal ) != 0;
	} ),
		std::end( lane_features ) );
}

void CCreateRoadLinkage::getExistRel( const std::vector<IFeaturePtr>& lane_features, std::set<CString>& existGids, std::multimap<CString, long>& exsistRel ) const
{
	using namespace sindy::schema::adam_v2;

	// 車線リンクのGIDリストを取得
	std::set<CString> gids;
	for( const auto& feature : lane_features ) {
		CComVariant vaGid = feature->GetValue( m_LaneLinkFM.FindField( lane_link::kGlobalID ) );
		if( vaGid.vt != VT_NULL )
			gids.emplace( vaGid.bstrVal );
	}

	// GID が一致する道路リンク－車線リンク関連テーブルのレコードを検索
	std::vector<CString> whereClauses = getWhereClauseINSearch( gids, rel_road_link_lane_link::kLaneLinkGID );
	for( const auto& where_clause : whereClauses )
	{
		_ICursorPtr cursor;
		if( FAILED( m_RelRoadLinkLaneLinkTable->Search(
			AheInitQueryFilter( nullptr, nullptr, where_clause ),
			VARIANT_FALSE,
			&cursor ) ) )
		{
			return;
		}

		// 検索したレコードのGID群を取得
		_IRowPtr row;
		while( SUCCEEDED( cursor->NextRow( &row ) ) && row ) {
			CComVariant vaGid = row->GetValue( m_RelRoadLinkLaneLinkFM.FindField( rel_road_link_lane_link::kLaneLinkGID ) );
			CComVariant vaLinkId = row->GetValue( m_RelRoadLinkLaneLinkFM.HasField( rel_road_link_lane_link::kRoadLinkID ) );
			if( vaGid.vt != VT_NULL )
			{
				existGids.emplace( vaGid.bstrVal );
				exsistRel.emplace( vaGid.bstrVal, vaLinkId.lVal );
			}
		}
	}
}

std::vector<CString> CCreateRoadLinkage::getWhereClauseINSearch( const std::set<CString>& vals, const CString& field_name  ) const
{
	// IN文用の文字列生成
	std::list<CString> listId(vals.begin(), vals.end());
	std::list<CString> inClauses;
	sindy::CTableBase::IDs2Str(listId, inClauses, _T("'"));

	return getWhereClauseINSearch( inClauses, field_name );
}

std::vector<CString> CCreateRoadLinkage::getWhereClauseINSearch( const std::vector<long>& vals, const CString& field_name ) const
{
	// IN文用の文字列生成
	std::list<CString> listId;
	for( const auto& val : vals )
		listId.emplace_back( uh::str_util::ToString( val ) );
	std::list<CString> inClauses;
	sindy::CTableBase::IDs2Str( listId, inClauses );

	return getWhereClauseINSearch( inClauses, field_name );
}

std::vector<CString> CCreateRoadLinkage::getWhereClauseINSearch( const std::list<CString>& vals, const CString& field_name ) const
{
	std::vector<CString> retWhereClauses;
	for( const auto& where : vals )
	{
		CString whereClause = uh::str_util::format( _T( "%s in ( %s )" ), field_name, where );
		retWhereClauses.push_back( whereClause );
	}

	return retWhereClauses;
}

bool CCreateRoadLinkage::createRel(
	const std::vector<IFeaturePtr>& lanelinkFeatures,
	const std::multimap<CString, long> lane2Road,
	const std::multimap<CString, long> existRel,
	long meshCode
) const
{
	std::vector<InsertRecord> insertRecords;
	for( const auto& laneFeature : lanelinkFeatures )
	{
		// 車線リンク周辺の道路リンク読み込み
		// オプションで指定した条件も考慮
		FeatureReader roadLinkReader( m_RoadLinkTable );
		if( !roadLinkReader.readAround( laneFeature->GetShapeCopy(), SEARCH_METER, m_Args.m_TargetRoadWhere.c_str() ) )
			continue;
		std::vector<IFeaturePtr> roadFeatures = roadLinkReader.getFeatures();

		// マッチング処理
		matchLaneLinkToRoadLinks( laneFeature, roadFeatures, lane2Road, existRel, meshCode, insertRecords );
	}

	// 道路リンク－車線リンク関連テーブルへ追加
	if( !insertRecord( insertRecords ) ) {
		RUNLOGGER.Error( _T( "道路リンク－車線リンク関連テーブルの追加に失敗しました" ) );
		return false;
	}

	return true;
}

void CCreateRoadLinkage::matchLaneLinkToRoadLinks(
	const IFeaturePtr& lane_link,
	const std::vector<IFeaturePtr> road_links,
	const std::multimap<CString, long>& lane2Road,
	const std::multimap<CString, long>& exsistRel,
	long base_mesh,
	std::vector<InsertRecord>& insert_records
) const
{
	using namespace uh;
	using namespace sindy::schema::adam_v2::lane_link;
	using namespace sindy::schema::category::adam_v2_link_code;
	using namespace sindy::schema::category::adam_v2_link_dir_code;
	using namespace Util;

	// 必要なフィールドのインデックスを取得
	long indexLaneLinkGid = m_LaneLinkFM.FindField( kGlobalID );
	long indexOnewayC = m_LaneLinkFM.FindField( kOneWayC );

	// 経緯度（ログ出力用）
	double x = 0.0, y = 0.0;
	getXY( lane_link, x, y );

	CComVariant vaGid = lane_link->GetValue(indexLaneLinkGid);
	CString laneGid;
	if( vaGid.vt != VT_NULL )
		laneGid = vaGid.bstrVal;

	// 車線リンクと道路リンクの紐付けを判定
	// 紐付かなかった場合も確認向けにFREESTYLEログへ出力する
	RoadLinkMatcher linkMatcher(m_RoadLinkTable, MATCHING_METER);
	bool isReverse(false);
	IFeaturePtr matchRoadFeature = linkMatcher.getMatchLink( lane_link, road_links, isReverse);

	if( !matchRoadFeature )
	{
		ERRLOGGER.Warn(lane_link->GetOID(), laneGid, base_mesh, x, y, str_util::format(_T("紐づく道路リンクが見つかりませんでした")));
		return;
	}

	// 道路差分の時だけ、レコード重複しないようチェック
	if( m_DiffTable && isRoadDiff() )
	{
		std::set<long> delOids;
		auto range = lane2Road.equal_range( laneGid );
		for( auto itr = range.first; itr != range.second; ++itr )
			delOids.emplace( itr->second );

		// 関連テーブルで削除した道路リンクのIDと一緒ならスルー
		if( delOids.count( matchRoadFeature->GetOID() ) == 0 )
		{
			std::set<long> exOids;
			auto rg = exsistRel.equal_range( laneGid );
			for( auto it = rg.first; it != rg.second; ++it )
				exOids.emplace( it->second );

			// 関連テーブルの中で既存の紐付きがあるならスキップ
			// ただし、他に削除された関連があるため警告
			if( exOids.count( matchRoadFeature->GetOID() ) == 1 )
			{
				ERRLOGGER.Warn(lane_link->GetOID(), laneGid, base_mesh, x, y, str_util::format(_T("複数の関連のうち、削除された関連があります")));
				return;
			}
		}
	}

	ERRLOGGER.Info(lane_link->GetOID(), laneGid, base_mesh, x, y,
		str_util::format(_T("紐づく道路リンクが見つかりました : ROAD_LINK_ID : %ld ( %s )"),
		matchRoadFeature->GetOID(),
		isReverse ? _T("逆方向") : _T("順方向")));

	insert_records.emplace_back(matchRoadFeature->GetOID(),
		laneGid,
		isReverse ? roadlink_dir_code::kReverse : roadlink_dir_code::kForward);
}

bool CCreateRoadLinkage::insertRecord(const std::vector<InsertRecord>& insert_records ) const
{
	using namespace sindy::schema::adam_v2::rel_road_link_lane_link;

	_ICursorPtr cursor;
	if(FAILED(m_RelRoadLinkLaneLinkTable->Insert( VARIANT_TRUE, &cursor)) )
		return false;

	// フィールドインデックス取得
	long indexOperator       = m_RelRoadLinkLaneLinkFM.FindField( kOperator );
	long indexProgModifyDate = m_RelRoadLinkLaneLinkFM.FindField( kProgModifyDate );
	long indexModifyProgName = m_RelRoadLinkLaneLinkFM.FindField( kModifyProgName );
	long indexUpdateType     = m_RelRoadLinkLaneLinkFM.FindField( kUpdateType );
	long indexRoadLinkId  = m_RelRoadLinkLaneLinkFM.FindField( kRoadLinkID );
	long indexLaneLinkGid = m_RelRoadLinkLaneLinkFM.FindField( kLaneLinkGID );
	long indexRoadLinkDC  = m_RelRoadLinkLaneLinkFM.FindField( kRoadLinkDirectionC );

	for( const auto& rec : insert_records )
	{
		IRowBufferPtr buffer;
		if( FAILED(m_RelRoadLinkLaneLinkTable->CreateRowBuffer( &buffer )) ) return false;

		if( FAILED(buffer->put_Value( indexOperator, CComVariant(_T("sindy")) ))) return false; 
		if( FAILED(buffer->put_Value( indexProgModifyDate, CComVariant( getCurrentTime())))) return false; 
		if( FAILED(buffer->put_Value( indexModifyProgName, CComVariant( CVersion().GetInternalName() )))) return false; 
		if( FAILED(buffer->put_Value( indexUpdateType, CComVariant( update_type::kCreated )))) return false; 

		if( FAILED(buffer->put_Value( indexRoadLinkId, CComVariant(rec.roadOid) ))) return false; 
		if( FAILED(buffer->put_Value( indexLaneLinkGid, CComVariant(rec.laneGid) ))) return false; 
		if( FAILED(buffer->put_Value( indexRoadLinkDC, CComVariant(rec.roadDC) ))) return false; 

		CComVariant oid;
		if( FAILED( cursor->InsertRow( buffer, &oid )) ) 
			return false;
	}

	return true;
}

bool CCreateRoadLinkage::getDiff( std::map<long, std::vector<IFeaturePtr>>& diffMap, const IGeometryPtr& mesh ) const
{
	using namespace sindy::schema::adam_v2;
	using namespace diff_db::change_type_code;

	// 差分がなければreturn
	if( !m_DiffTable )
		return true;

	// 読み込み
	FeatureReader diffReader( m_DiffTable );
	if( !diffReader.read( mesh, CString() ) ) {
		RUNLOGGER.Error( _T( "形状差分テーブルの読み込みに失敗しました" ) );
		return false;
	}

	for( const auto& feature : diffReader.getFeatures() )
	{
		long changeType = feature->GetValue( m_DiffFM.HasField( diff_db::kChangeType ) ).lVal;

		if( isRoadDiff() )
		{
			// 道路リンクの「追加」は使わない
			if( changeType == kNew )
				continue;

			// 道路リンクはメッシュ境界でリンクが切れているが、
			// 検索の段階では、境界に対して内側のリンクも外側のリンクも取得している
			// ここで外側のリンクを弾く
			if( !AheIsContain( mesh, feature->GetShapeCopy() ) )
				continue;
		}

		diffMap[changeType].emplace_back( feature );
	}

	return true;
}

bool CCreateRoadLinkage::isRoadDiff() const
{
	using namespace sindy::schema::adam_v2;

	// やっつけだがフラグ管理も微妙なのでこの方法を取る
	return ( -1 == m_DiffFM.HasField( lane_link::kGlobalID ) );
}

bool CCreateRoadLinkage::getMeshPolygon( const std::vector<long>& meshList, std::map<long, IGeometryPtr>& meshGeoMap ) const
{
	using namespace sindy::schema;
	using namespace uh::str_util;

	CString subFieleds = format( _T( "%s,%s" ), category::mesh_code_table::kMeshCode, feature::kShape );
	std::vector<CString> whereClauses = getWhereClauseINSearch( meshList, category::mesh_code_table::kMeshCode );

	// 1000メッシュ毎に検索
	for( const auto& where : whereClauses )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( IFeatureClassPtr( m_BaseMeshTable )->Search(
			AheInitQueryFilter( nullptr, subFieleds, where ),
			VARIANT_TRUE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.Error( _T( "メッシュテーブルの読み込みに失敗しました" ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CComVariant var = ipFeature->GetValue( m_BaseMeshFM.FindField( category::mesh_code_table::kMeshCode ) );
			var.ChangeType( VT_I4 );
			meshGeoMap.emplace( var.lVal, ipFeature->GetShapeCopy() );
		}
	}

	return true;
}

CString CCreateRoadLinkage::getCurrentTime() const
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	return uh::str_util::format( _T("%d/%02d/%02d %02d:%02d:%02d")
        ,st.wYear ,st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
}

void CCreateRoadLinkage::finish(bool isSucceeded) const
{
	// 終了メッセージ出力
	uh::tstring result = isSucceeded ? _T("正常終了") : _T("異常終了");
	RUNLOGGER.Info( _T("処理結果: ") + result);

	// ファイルのクローズ
	RUNLOGGER.Finalize();
	ERRLOGGER.Finalize();
}

CString CCreateRoadLinkage::getChildVersionName() const
{
	const CString prefix = _T("ETC_ADAMEDIT_");

	// 現在の日時を"YYYYMMDDHHDDSS" で取得
	CTime cTime = CTime::GetCurrentTime();
	const CString timeString = cTime.Format( _T("%Y%m%d%H%M%S") );

	return prefix + timeString;
}