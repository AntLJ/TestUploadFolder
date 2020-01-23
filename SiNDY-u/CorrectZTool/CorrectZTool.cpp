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
#include "CorrectZTool.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/uh_float.h>
#include <TDC/useful_headers/scope_guard.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy::schema;
using namespace sindy::schema::adam_v2;
using namespace uh;
using namespace uh::str_util;
using namespace adam;

// 初期化
bool CCorrectZTool::init()
{
	// ログオープン
	if( !ERRLOGGER.Initialize( m_args.m_err_log.c_str() ) )
	{
		cerr << "エラーログファイルのオープンに失敗" << endl;
		return false;
	}
	if( !RUNLOGGER.Initialize( m_args.m_run_log.c_str() ) )
	{
		cerr << "実行ログファイルのオープンに失敗" << endl;
		return false;
	}
	RUNLOGGER.writeOption( m_args );
	RUNLOGGER.writeInfo( _T( "開始時刻" ) );

	// メッシュリスト取得
	if( !getMeshList( m_args.m_mesh_list.c_str(), m_meshList ) )
	{
		RUNLOGGER.writeInfo( _T( "メッシュリスト取得失敗" ) );
		return false;
	}

	// BASEMESHのみOK
	list<long>::iterator headItr = m_meshList.begin();
	list<long>::reverse_iterator tailItr = m_meshList.rbegin();
	long headLength = static_cast<long>( log10( *headItr ) + 1 );
	long tailLength = static_cast<long>( log10( *tailItr ) + 1 );
	if( headLength != 6 || tailLength != 6 )
	{
		RUNLOGGER.writeInfo( _T( "メッシュ桁数が不正" ) );
		return false;
	}

	// ワークスペース取得
	if( !openWorkspace( m_args.m_db.c_str(), m_ipWorkspace ) ||
		!openWorkspace( m_args.m_mesh_db.c_str(), m_ipMeshWorkspace ) )
		return false;

	// DBタイプをチェック
	switch( m_ipWorkspace->GetType() )
	{
		case esriRemoteDatabaseWorkspace:
		{
			// デフォルトバージョンは処理中止
			if( isDefaultVersion( m_ipWorkspace ) )
				return false;
			break;
		}
		case esriLocalDatabaseWorkspace:
			break;
		default:
			return false;
	}

	// フィーチャクラス・テーブル取得
	m_ipMesh = openTable( m_ipMeshWorkspace, basemesh::kTableName, m_cMeshFM );
	m_ipLaneLink = openTable( m_ipWorkspace, lane_link::kTableName, m_cLaneLinkFM );
	m_ipLaneNode = openTable( m_ipWorkspace, lane_node::kTableName, m_cLaneNodeFM );
	m_ipBorderLink = openTable( m_ipWorkspace, border_link::kTableName, m_cBorderLinkFM );
	m_ipRelLaneLinkBorderLink = openTable( m_ipWorkspace, rel_border_link_lane_link::kTableName, m_cRelLaneLinkBorderLinkFM );
	m_ipNqLaneNode = openTable( m_ipWorkspace, nq_lane_node::kTableName,  m_cNqLaneNodeFM );
	if( !m_ipMesh || !m_ipLaneLink || !m_ipLaneNode || !m_ipBorderLink || !m_ipRelLaneLinkBorderLink || !m_ipNqLaneNode )
		return false;

	// ツール名取得
	CVersion cVersion;
	m_toolName = cVersion.GetInternalName();

	return true;
}

// 実行
bool CCorrectZTool::execute()
{
	// 初期化
	if( !init() )
		return false;

	// ここのスコープを抜けたら強制的にabort
	uh::scope_guard scope( bind( abortEdit, m_ipWorkspace ) );

	// メッシュリストから形状を取得
	if( !getMeshPolygon( m_meshList, m_meshGeoMap ) )
		return false;

	// メッシュ分ループ
	long count = 0;
	for( const auto& mesh : m_meshGeoMap )
	{
		cout << "メッシュ処理中..." + to_string( mesh.first ) + " : " + to_string( ++count ) + " / " + to_string( m_meshGeoMap.size() ) << endl;

		// 編集開始
		if( !startEdit( m_ipWorkspace ) )
			return false;

		// 車線リンク検索
		GidList ipcLaneLinkGids;
		if( !searchLaneLink( mesh.second, ipcLaneLinkGids ) )
			return false;

		// ノード列を検索して紐付く車線ノードを調べる
		GidList laneNodeGids;
		Node2LinkMap node2link;
		Link2NodeMap dummy;
		if( !searchNqTable( ipcLaneLinkGids, laneNodeGids, node2link, dummy ) )
			return false;

		// 車線ノードを検索してATノードの一覧を取得
		GidSet atLaneNodeGids;
		if( !searchLaneNode( laneNodeGids, atLaneNodeGids ) )
			return false;

		// 関連テーブルを検索して紐付く道路境界線リンクを調べる
		GidList borderLinkGids;
		LaneBorderRelMap lane2border;
		if( !searchRelTable( ipcLaneLinkGids, borderLinkGids, lane2border ) )
			return false;

		// 道路境界線リンクを取得
		FeatureMap borderLinks;
		if( !searchBorderLink( borderLinkGids, borderLinks ) )
			return false;

		// リンク更新（Updateカーソルで、リンク構成点と最近傍境界線地点とでZ値更新）
		if( !updateLaneLink( ipcLaneLinkGids, atLaneNodeGids, lane2border, borderLinks, mesh.first ) )
			return false;

		// ノード更新（Updateカーソルで、ノードと最近傍境界線地点とでZ値更新）
		if( !updateLaneNode( laneNodeGids, node2link, lane2border, borderLinks, mesh.first ) )
			return false;

		// 編集終了
		if( !stopEdit( m_ipWorkspace ) )
			return false;
	}

	// 以下は全国分一括処理
	cout << "リンク間段差解消処理...開始" << endl;

	// 編集開始
	if( !startEdit( m_ipWorkspace ) )
		return false;

	// 更新したノードに紐づくリンク同士のZ値が一致しているか確認し、一致していないリンクの構成点はノードのZ値で補正する
	if( !doConsistentZ() )
		return false;

	// 編集終了
	if( !stopEdit( m_ipWorkspace ) )
		return false;

	cout << "リンク間段差解消処理...完了" << endl;

	cout << "急勾配リンク線形補間処理...開始" << endl;

	// 編集開始
	if( !startEdit( m_ipWorkspace ) )
		return false;

	// 急勾配リンクとそこからFIXリンクまでのすべての接続リンクを線形補間する
	// 合わせて関連するノードも線形補間する
	if( !doLinerInterpolation() )
		return false;

	// 編集終了
	if( !stopEdit( m_ipWorkspace ) )
		return false;

	cout << "急勾配リンク線形補間処理...完了" << endl;

	return true;
}

bool CCorrectZTool::searchLaneLink( const IGeometryPtr& mesh, GidList& ipcLaneLinkGids )
{
	using namespace lane_link;

	IFeatureCursorPtr ipFeatureCur;
	if( FAILED( m_ipLaneLink->Search( AheInitSpatialFilter( nullptr, mesh ), VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
	{
		CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( kGlobalID ) ).bstrVal;
		if( gid.IsEmpty() )
			continue;

		// SOURCEID = NULL ならIPCリンク、NOT NULL ならATリンク
		CString sourceId = ipFeature->GetValue( m_cLaneLinkFM.HasField( kSourceID ) ).bstrVal;
		if( sourceId.IsEmpty() )
			ipcLaneLinkGids.emplace_back( gid );
	}

	return true;
}

bool CCorrectZTool::searchRelTable( const GidList& laneLinkGids, GidList& borderLinkGids, LaneBorderRelMap& lane2border )
{
	using namespace rel_border_link_lane_link;

	CString subFieleds = format( _T( "%s,%s,%s" ), kLaneLinkGID, kBorderLinkGID, kLRC );

	// 1000件毎に検索
	for( const auto& str : getString( laneLinkGids ) )
	{
		_ICursorPtr ipCur;
		if( FAILED( m_ipRelLaneLinkBorderLink->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kLaneLinkGID, str ),
			VARIANT_TRUE, &ipCur ) ) || !ipCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), kTableName ) );
			return false;
		}

		_IRowPtr ipRow;
		while( S_OK == ipCur->NextRow( &ipRow ) && ipRow )
		{
			CString laneGid = ipRow->GetValue( m_cRelLaneLinkBorderLinkFM.HasField( kLaneLinkGID ) ).bstrVal;
			CString borderGid = ipRow->GetValue( m_cRelLaneLinkBorderLinkFM.HasField( kBorderLinkGID ) ).bstrVal;
			long lrC = ipRow->GetValue( m_cRelLaneLinkBorderLinkFM.HasField( kLRC ) ).lVal;

			borderLinkGids.emplace_back( borderGid );
			// 車線リンクに対して道路境界線リンクは左右に1本以上ずつ紐付く
			lane2border[laneGid].emplace( lrC, borderGid );
		}
	}

	// 最後に重複削除
	borderLinkGids.sort();
	borderLinkGids.unique();

	return true;
}

bool CCorrectZTool::searchLaneNode( const GidList& laneNodeGids, GidSet& atLaneNodeGids )
{
	using namespace lane_node;

	CString subFieleds = format( _T( "%s,%s" ), kSourceID, kGlobalID );

	// 1000件毎に検索
	for( const auto& str : getString( laneNodeGids ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipLaneNode->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kGlobalID, str ),
			VARIANT_TRUE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			// SOURCEID != NULL ならATノード
			CString sourceId = ipFeature->GetValue( m_cLaneNodeFM.HasField( kSourceID ) ).bstrVal;
			if( !sourceId.IsEmpty() )
			{
				CString gid = ipFeature->GetValue( m_cLaneNodeFM.HasField( kGlobalID ) ).bstrVal;
				atLaneNodeGids.emplace( gid );
			}
		}
	}

	return true;
}

bool CCorrectZTool::searchBorderLink( const GidList& borderLinkGids, FeatureMap& borderLinks )
{
	using namespace border_link;

	CString subFieleds = format( _T( "%s,%s" ), kGlobalID, kShape );

	// 1000件毎に検索
	for( const auto& str : getString( borderLinkGids ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipBorderLink->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kGlobalID, str ),
			VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CString gid = ipFeature->GetValue( m_cBorderLinkFM.HasField( kGlobalID ) ).bstrVal;
			if( gid.IsEmpty() )
				continue;
			borderLinks.emplace( gid, ipFeature );
		}
	}

	return true;
}

bool CCorrectZTool::updateLaneLink( const GidList& laneLinkGids, const GidSet& atLaneNodeGids, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode )
{
	// 1000件毎に検索
	for( const auto& str : getString( laneLinkGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneLink->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_link::kGlobalID, str ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_link::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			if( gid.IsEmpty() )
				continue;

			// ATリンクと接続する場合は、接続部分の構成点だけ更新をスキップ
			set<long> skipPointNums;
			if( 1 == atLaneNodeGids.count( ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kFromNodeGID ) ).bstrVal ) )
				skipPointNums.emplace( 0 ); // FROMノードが乗っかる構成点
			if( 1 == atLaneNodeGids.count( ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kToNodeGID ) ).bstrVal ) )
				skipPointNums.emplace( IPointCollectionPtr( ipFeature->GetShape() )->GetPointCount() - 1 ); // TOノードが乗っかる構成点

			GidSet dummy;
			updateRun( ipFeature, ipUpdateCur, dummy, lane2border, borderLinks, skipPointNums, lane_link::kTableName, m_cLaneLinkFM, meshCode );
		}
	}

	return true;
}

bool CCorrectZTool::searchNqTable( const GidList& laneLinkGids, GidList& laneNodeGids, Node2LinkMap& node2link, Link2NodeMap& link2node )
{
	using namespace nq_lane_node;

	CString subFieleds = format( _T( "%s,%s,%s" ), kLaneLinkGID, kLaneNodeGID, kSequence );

	// 1000件毎に検索
	for( const auto& str : getString( laneLinkGids ) )
	{
		_ICursorPtr ipCur;
		if( FAILED( m_ipNqLaneNode->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), kLaneLinkGID, str ),
			VARIANT_TRUE, &ipCur ) ) || !ipCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), kTableName ) );
			return false;
		}

		_IRowPtr ipRow;
		while( S_OK == ipCur->NextRow( &ipRow ) && ipRow )
		{
			CString linkGid = ipRow->GetValue( m_cNqLaneNodeFM.HasField( kLaneLinkGID ) ).bstrVal;
			CString nodeGid = ipRow->GetValue( m_cNqLaneNodeFM.HasField( kLaneNodeGID ) ).bstrVal;
			long seq = ipRow->GetValue( m_cNqLaneNodeFM.HasField( kSequence ) ).lVal;

			laneNodeGids.emplace_back( nodeGid );
			node2link[nodeGid].emplace( linkGid );
			link2node[linkGid].emplace( nodeGid, seq );
		}
	}

	// 最後に重複削除
	laneNodeGids.sort();
	laneNodeGids.unique();

	return true;
}

bool CCorrectZTool::updateLaneNode( const GidList& laneNodeGids, const Node2LinkMap& node2link, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode )
{
	// 1000件毎に検索
	for( const auto& str : getString( laneNodeGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneNode->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_node::kGlobalID, str ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_node::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			// SOURCEID != NULL ならATノードなので、更新対象外
			CString sourceId = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kSourceID ) ).bstrVal;
			if( !sourceId.IsEmpty() )
				continue;

			CString gid = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kGlobalID ) ).bstrVal;
			if( gid.IsEmpty() )
				continue;

			set<long> dummy;
			updateRun( ipFeature, ipUpdateCur, node2link.at( gid ), lane2border, borderLinks, dummy, lane_node::kTableName, m_cLaneNodeFM, meshCode );
		}
	}

	return true;
}

void CCorrectZTool::updateRun(
	IFeaturePtr& ipFeature,
	IFeatureCursorPtr& ipUpdateCur,
	const GidSet& linkGids,
	const LaneBorderRelMap& lane2border,
	const FeatureMap& borderLinks,
	const set<long>& skipPointNums,
	const CString layer,
	const sindy::CFieldMap& fieldMap,
	long meshCode
)
{
	CString gid = ipFeature->GetValue( fieldMap.HasField( adam_v2_global_ipc_feature::kGlobalID ) ).bstrVal;

	// 一度更新したフィーチャーは更新スキップ
	if( 1 == m_updatedList.count( gid ) )
	{
		ERRLOGGER.warning( layer, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：他のメッシュの処理にて更新済み" ), ToString( meshCode ), CString() );
		return;
	}

	// 左右の道路境界線リンクを取得
	FeatureList leftBorders, rightBorders;
	if( layer.CompareNoCase( lane_link::kTableName ) == 0 )
	{
		if( 0 == lane2border.count( gid ) )
			return; // SA・PAとかは元から境界線がないので更新対象外

		getBorderLink( gid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kLeft, leftBorders );
		getBorderLink( gid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kRight, rightBorders );
	}
	else if( layer.CompareNoCase( lane_node::kTableName ) == 0 )
	{
		// ノードに紐づくリンク分ループ
		long notFoundCount = 0;
		for( const auto& linkGid : linkGids )
		{
			if( 0 == lane2border.count( linkGid ) )
			{
				++notFoundCount;
				continue; // SA・PAとかは元から境界線がないので更新対象外
			}

			getBorderLink( linkGid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kLeft, leftBorders );
			getBorderLink( linkGid, lane2border, borderLinks, rel_border_link_lane_link::lr_code::kRight, rightBorders );

			if( leftBorders.empty() || rightBorders.empty() )
			{
				// 片方でも欠けたらリセットして次へ
				// エラーは全部のリンクでダメだった時に出す
				// TODO: 全ての車線リンクに紐づく左右の道路境界線全てから、Z値の値を決めた方が保管されるZ値の精度は高くなるはず
				leftBorders.clear(); rightBorders.clear();
				continue;
			}
			break; // 両方揃ってればOK
		}

		// 全部のリンクで境界線がなかったらreturn
		if( notFoundCount == linkGids.size() )
			return;
	}
	else
		assert( false ); // ここへ来てはいけない

	if( leftBorders.empty() || rightBorders.empty() )
	{
		ERRLOGGER.error( layer, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：道路境界線リンクが左右揃っていない" ), ToString( meshCode ), CString() );
		return;
	}

	// Z値を更新する
	FeatureMap usedBorders;
	IGeometryPtr ipNewGeo( ipFeature->GetShape() );
	bool bIsSame = false;
	if( !updateZValue( ipFeature->GetShape(), leftBorders, rightBorders, skipPointNums, usedBorders, ipNewGeo, bIsSame ) )
	{
		ERRLOGGER.error( layer, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：Z値更新に失敗" ), ToString( meshCode ), CString() );
		return;
	}

	// ログ出力用の道路境界線リンクSOURCE
	CString source;
	set<CString> sources;
	for( const auto& feature : usedBorders )
		sources.emplace( feature.second->GetValue( m_cBorderLinkFM.HasField( border_link::kSource ) ).bstrVal );
	source = join( sources, _T(",") );

	// Z値に変化が無ければスキップ
	if( bIsSame )
	{
		ERRLOGGER.warning( layer, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：Z値に変化がない" ), ToString( meshCode ), source );
		return;
	}

	// フィーチャーを更新する
	if( !putValue( ipFeature, ipNewGeo, fieldMap ) || FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
	{
		ERRLOGGER.error( layer, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：フィーチャー更新に失敗" ), ToString( meshCode ), source );
		return;
	}

	ERRLOGGER.info( layer, ToString( ipFeature->GetOID() ), gid, _T( "更新成功" ), ToString( meshCode ), source );
	m_updatedList.emplace( gid );

	// ノードなら、後々のチェック用に情報を保持
	if( layer.CompareNoCase( lane_node::kTableName ) == 0 )
		m_updatedNode.emplace( gid, ipFeature );
}

bool CCorrectZTool::doConsistentZ()
{
	// 更新されたノードGIDを取得
	list<CString> nodeGids;
	for( const auto& pair : m_updatedNode )
		nodeGids.emplace_back( pair.first );

	// 1000件毎に検索
	map<CString, map<CString, vector<IFeaturePtr>>> node2Links; // Key:ノードGID, Value:マップ（Key:フィールド, Value:リンク）
	for( const auto& ids : getString( nodeGids ) )
	{
		if( !getNode2Links( lane_link::kFromNodeGID, ids, node2Links ) ||
			!getNode2Links( lane_link::kToNodeGID, ids, node2Links ) )
			return false;
	}

	// 紐づくリンク同士のZ値が一致しているか確認し、不一致な場合はリンク同士を接続させる
	map<CString, pair<IGeometryPtr, CString>> linkInfo; // Key:リンクGID, Value:ペア（First:更新後リンク形状, Value:更新箇所（From/To））
	for( const auto& pair : node2Links )
	{
		if( !connectLink( m_updatedNode[pair.first], pair.second, linkInfo ) )
			return false;
	}

	// リンクGIDを取得
	list<CString> linkGids;
	for( const auto& pair : linkInfo )
		linkGids.emplace_back( pair.first );

	// 1000件毎に更新
	for( const auto& ids : getString( linkGids ) )
	{
		if( !reUpdateLinkZ( ids, linkInfo ) )
			return false;
	}

	return true;
}

bool CCorrectZTool::getNode2Links( const CString& field, const CString& ids, map<CString, map<CString, vector<IFeaturePtr>>>& node2Links ) const
{
	IFeatureCursorPtr ipFeatureCur;
	if( FAILED( m_ipLaneLink->Search(
		AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), field, ids ),
		VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_link::kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
	{
		CString nodeGid = ipFeature->GetValue( m_cLaneLinkFM.HasField( field ) ).bstrVal;
		node2Links[nodeGid][field].emplace_back( ipFeature );
	}

	return true;
}

bool CCorrectZTool::connectLink( const IFeaturePtr& ipFeature, const map<CString, vector<IFeaturePtr>>& field2Links, map<CString, pair<IGeometryPtr, CString>>& linkInfo )
{
	// ノードのZ値を取得
	double nodeZ = IPointPtr( ipFeature->GetShape() )->GetZ();

	// リンクでループ
	for( const auto& pair : field2Links )
	{
		for( const auto& link : pair.second )
		{
			CString linkGid = link->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			IGeometryPtr linkGeo = link->GetShapeCopy();

			// もし既に反対側のノードの処理でジオメトリを更新していたら、そのジオメトリをまた使う
			// こうしないと1本のリンクで両側ともズレているケースに対応できない
			if( 1 == linkInfo.count( linkGid ) )
				linkGeo = linkInfo[linkGid].first;

			// From/Toに応じたリンクの構成点から、Z値を取得
			IPointCollectionPtr ipPointCol( linkGeo );
			long targetNum = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ? 0 : ipPointCol->GetPointCount() - 1;
			IPointPtr ipPoint = ipPointCol->GetPoint( targetNum );
			double linkZ = ipPoint->GetZ();

			// ノードのZ値と一致していればOK
			if( nodeZ == linkZ )
				continue;

			// 勾配を計算する
			long nextNum = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ? 1 : ipPointCol->GetPointCount() - 2;
			IPointPtr nextPt = ipPointCol->GetPoint( nextNum );
			double nextLinkZ = nextPt->GetZ();

			double height = nextLinkZ - nodeZ;						// 高さ（垂直）
			double distance = AheGetMeterLength( ipPoint, nextPt ); // 距離（水平）
			double slope = height * 100.0 / distance;				// 勾配（高さ * 100 / 距離[%]）

			// 急勾配なら、後で線形補間するため確保
			if( m_args.m_slope_max < fabs( slope ) )
				m_steepLink.emplace( linkGid );

			// リンクの構成点が持つZ値を更新する
			if( FAILED( ipPoint->put_Z( nodeZ ) ) || FAILED( ipPointCol->UpdatePoint( targetNum, ipPoint ) ) )
				return false;

			CString msg = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ?
				str_util::format( _T( "FROMノード側の構成点が持つZ値を再更新：勾配(%lf)" ), slope ) :
				str_util::format( _T( "TOノード側の構成点が持つZ値を再更新：勾配(%lf)" ), slope );

			// もし既に反対側のノードで処理していたら、メッセージも追加
			if( 1 == linkInfo.count( linkGid ) )
				msg += ( _T( ", " ) + linkInfo[linkGid].second );

			// 更新したリンクの情報を確保
			linkInfo[linkGid] = make_pair( IGeometryPtr( ipPointCol ), msg );

			// 更新したリンクに接続するリンクのZ値もまた真なので、線形補間処理で経路探索対象外にするリンクとして確保
			CString otherField = pair.first.CompareNoCase( lane_link::kFromNodeGID ) == 0 ? lane_link::kToNodeGID : lane_link::kFromNodeGID;
			if( 0 == field2Links.count( otherField ) )
				continue;
			for( const auto& fixLink : field2Links.at( otherField ) )
				m_fixLink.emplace( fixLink->GetOID() );
		}
	}

	return true;
}

bool CCorrectZTool::reUpdateLinkZ( const CString& ids, map<CString, pair<IGeometryPtr, CString>> linkInfo )
{
	IFeatureCursorPtr ipUpdateCur;
	if( FAILED( m_ipLaneLink->Update(
		AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_link::kGlobalID, ids ),
		VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_link::kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
	{
		CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
		IGeometryPtr ipNewGeo = linkInfo.at( gid ).first;
		CString msg = linkInfo.at( gid ).second;

		if( !putValue( ipFeature, ipNewGeo, m_cLaneLinkFM ) || FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
		{
			ERRLOGGER.error( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：フィーチャー更新に失敗" ), CString(), CString() );
			continue;
		}

		ERRLOGGER.info( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, format( _T( "更新成功：%s" ), msg ), CString(), CString() );
	}

	return true;
}

bool CCorrectZTool::doLinerInterpolation()
{
	// 不要なメモリの開放＋使い回しのため、明示的にリセット
	m_updatedList.clear();

	// 急勾配リンクを取得する
	map<long, IFeaturePtr> steepLinkMap;
	for( const auto& ids : getString( GidList( m_steepLink.begin(), m_steepLink.end() ) ) )
	{
		if( !GetFeatures( m_ipLaneLink, steepLinkMap, nullptr, format( _T( "%s IN (%s)" ), lane_link::kGlobalID, ids ) ) )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_link::kTableName ) );
			return false;
		}
	}

	// 始める前から急勾配のリンクが対象外扱いされていたら困るので対象外から弾く（対象にする）
	for( const auto& pair : steepLinkMap )
	{
		if( 1 == m_fixLink.count( pair.first ) )
			m_fixLink.erase( pair.first );
	}

	// 急勾配リンクを1本ずつループする
	for( const auto& pair : steepLinkMap )
	{
		map<long, IFeaturePtr> laneLinkFeatureMap;
		laneLinkFeatureMap.emplace( pair.first, pair.second );

		// 線形補完した車線リンク群
		map<long, IGeometryPtr> newGeomMap;

		// 急勾配リンクとそこからFIXリンクまでのすべての接続リンクを線形補間する
		CRestrictableZValueAdjuster adjuster;
		adjuster.SetFixLink( m_fixLink );
		if( !adjuster.AdjustZValue( laneLinkFeatureMap, newGeomMap ) )
		{
			CString gid = pair.second->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			ERRLOGGER.error( lane_link::kTableName, ToString( pair.first ), gid, _T( "スキップ：線形補間に失敗" ), CString(), CString() );
			continue;
		}

		// 線形補間したリンクを確保（次にm_fixLinkがCRestrictableZValueAdjusterに渡される時、線形補間したリンクもFIXリンクになるように）
		// あとついでにキャスト
		GidList linkGids;
		map<CString, IGeometryPtr> linkGeomMap;
		for( const auto& link : newGeomMap )
		{
			m_fixLink.emplace( link.first );

			CString gid = laneLinkFeatureMap[link.first]->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			linkGids.emplace_back( gid );
			linkGeomMap.emplace( gid, link.second );
		}

		// リンク更新
		if( !reUpdateLinkZ( linkGids, linkGeomMap ) )
			return false;

		// ノード列を検索して紐付く車線ノードを調べる
		GidList nodeGids;
		Node2LinkMap node2link;
		Link2NodeMap link2node;
		if( !searchNqTable( linkGids, nodeGids, node2link, link2node ) )
			return false;

		// ノード更新
		if( !reUpdateNodeZ( nodeGids, node2link, link2node, linkGeomMap ) )
			return false;
	}

	return true;
}

bool CCorrectZTool::reUpdateLinkZ( const GidList& linkGids, const std::map<CString, IGeometryPtr>& newGeomMap )
{
	for( const auto& ids : getString( linkGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneLink->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_link::kGlobalID, ids ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_link::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CString gid = ipFeature->GetValue( m_cLaneLinkFM.HasField( lane_link::kGlobalID ) ).bstrVal;
			IGeometryPtr ipNewGeo = newGeomMap.at( gid );

			if( !putValue( ipFeature, ipNewGeo, m_cLaneLinkFM ) || FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
			{
				ERRLOGGER.error( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, _T( "スキップ：フィーチャー更新に失敗" ), CString(), CString() );
				continue;
			}

			ERRLOGGER.info( lane_link::kTableName, ToString( ipFeature->GetOID() ), gid, format( _T( "更新成功：線形補間" ) ), CString(), CString() );
		}
	}

	return true;
}

bool CCorrectZTool::reUpdateNodeZ( const GidList& nodeGids, const Node2LinkMap& node2link, const Link2NodeMap& link2node, const map<CString, IGeometryPtr>& linkGeomMap )
{
	// Z値を小数点以下第何位で四捨五入するか
	// DB側のZ値座標精度の都合上、get_Z()で取得したZ値と計算で求めたZ値が微妙に一致しないため、
	// 丸め込みを行なって一致判定を行う
	// TODO: あり得ないと思うが、Z値の座標精度に変更があった場合は、こちらも変更すること
	static const long lRoundDigit = 5;

	for( const auto& ids : getString( nodeGids ) )
	{
		IFeatureCursorPtr ipUpdateCur;
		if( FAILED( m_ipLaneNode->Update(
			AheInitQueryFilter( nullptr, nullptr, _T( "%s IN (%s)" ), lane_node::kGlobalID, ids ),
			VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), lane_node::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
		{
			// SOURCEID != NULL ならATノードなので、更新対象外
			CString sourceId = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kSourceID ) ).bstrVal;
			if( !sourceId.IsEmpty() )
				continue;

			CString nodeGid = ipFeature->GetValue( m_cLaneNodeFM.HasField( lane_node::kGlobalID ) ).bstrVal;
			if( 1 == m_updatedList.count( sourceId ) )
			{
				ERRLOGGER.warning( lane_node::kTableName, ToString( ipFeature->GetOID() ), sourceId, _T( "スキップ：既に線形補間済み" ), CString(), CString() );
				continue;
			}

			// ノードが乗っているリンクを取得
			// 区間点ノードに対しては、リンクは1本しか存在しないはず
			// FromToノードに対しては、複数リンクが存在するが、
			// どのリンクの構成点もZ値は同じ（修正されて接続している）はずなので、最初の1本を取る
			set<CString>::const_iterator it = node2link.at( nodeGid ).begin();
			CString linkGid = *it;

			// リンクの構成点のZ値を取得
			// 構成点の番号はノードのSEQUENCEと一致している前提
			IPointCollectionPtr ipPointCol( linkGeomMap.at( linkGid ) );
			long targetNum = link2node.at( linkGid ).at( nodeGid );
			IPointPtr ipPoint = ipPointCol->GetPoint( targetNum );
			double linkZ = ipPoint->GetZ();

			// ノードのZ値を取得
			IGeometryPtr ipNewGeo = ipFeature->GetShapeCopy();
			double nodeZ = IPointPtr( ipNewGeo )->GetZ();

			// リンクの構成点とノードのZ値が一緒ならスキップ
			if( roundingOff( linkZ, lRoundDigit ) == roundingOff( nodeZ, lRoundDigit ) )
				continue;

			if( FAILED( IPointPtr( ipNewGeo )->put_Z( linkZ ) ) ||
				!putValue( ipFeature, ipNewGeo, m_cLaneLinkFM ) ||
				FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
			{
				ERRLOGGER.error( lane_node::kTableName, ToString( ipFeature->GetOID() ), nodeGid, _T( "スキップ：フィーチャー更新に失敗" ), CString(), CString() );
				continue;
			}

			ERRLOGGER.info( lane_node::kTableName, ToString( ipFeature->GetOID() ), nodeGid, format( _T( "更新成功：線形補間" ) ), CString(), CString() );
			m_updatedList.emplace( nodeGid );
		}
	}

	return true;
}

void CCorrectZTool::getBorderLink( const CString& linkGid, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long lrCode, FeatureList& borders )
{
	auto range = lane2border.at( linkGid ).equal_range( lrCode );
	for( auto it = range.first; it != range.second; ++it )
	{
		if( 0 == borderLinks.count( it->second ) )
			continue;

		borders.emplace_back( borderLinks.at( it->second ) );
	}
}

bool CCorrectZTool::putValue( IFeaturePtr& ipFeature, const IGeometryPtr& ipGeo, const sindy::CFieldMap& fieldMap )
{
	// 属性セット
	if( FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kOperator ),			CComVariant( format( _T( "sindy" ) ) ) ) ) ||
		FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kUpdateType ),		CComVariant( ipc_feature::update_type::kShapeUpdated ) ) ) || // 3：形状変更
		FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kProgModifyDate ),	CComVariant( log_utility::GetDateTimeInfo() ) ) ) ||
		FAILED( ipFeature->put_Value( fieldMap.HasField( ipc_feature::kModifyProgName ),	CComVariant( m_toolName ) ) ) ||
		FAILED( ipFeature->putref_Shape( ipGeo ) ) )
		return false;

	return true;
}

list<CString> CCorrectZTool::getString( const list<CString>& target, const CString& sep /* =_T("'") */ )
{
	list<CString> strings;
	IDs2Str( target, strings, sep );
	
	return strings;
}

list<CString> CCorrectZTool::getString( const list<long>& target )
{
	list<CString> strings;
	IDs2Str( target, strings );

	return strings;
}



bool CCorrectZTool::openWorkspace( const CString& db, IWorkspacePtr& ipWorkspace )
{
	ipWorkspace = sindy::create_workspace( _bstr_t( db ) );
	if( !ipWorkspace )
	{
		RUNLOGGER.writeInfo( format( _T( "%s接続失敗" ), db ) );
		return false;
	}

	return true;
}

bool CCorrectZTool::isDefaultVersion( const IWorkspacePtr& ipWorkspace )
{
	IVersionedWorkspacePtr ipVerWorkspace( ipWorkspace );
	if( !ipVerWorkspace ) return false;

	IVersionPtr ipVersion( ipWorkspace );
	CComBSTR bstr;
	ipVersion->get_VersionName( &bstr );
	CString strVerName = COLE2T( bstr );
	if( strVerName.Find( _T( "DEFAULT" ) ) != -1 )
	{
		RUNLOGGER.writeInfo( _T( "接続先がDEFAULTバージョン" ) );
		return true;
	}

	return false;
}

bool CCorrectZTool::getMeshList( const CString& fileName, MeshList& meshList )
{
	if( fileName.IsEmpty() )
		return false;

	ifstream ifs( fileName );
	if( !ifs )
		return false;

	string line;
	while( getline( ifs, line ) )
	{
		// 空の行や行頭「#」は無視
		if( line.size() == 0 || line.at( 0 ) == '#' )
			continue;

		meshList.emplace_back( stoul( line ) );
	}
	if( ifs.fail() && !ifs.eof() )
		return false;

	meshList.sort();

	return true;
}

bool CCorrectZTool::getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap )
{
	long count = 0; // 検索完了メッシュ数
	CString subFieleds = format( _T( "%s,%s" ), category::mesh_code_table::kMeshCode, feature::kShape );

	// 1000メッシュ毎に検索
	for( const auto& str : getString( meshList ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipMesh->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), category::mesh_code_table::kMeshCode, str ),
			VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), basemesh::kTableName ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CComVariant var = ipFeature->GetValue( m_cMeshFM.FindField( category::mesh_code_table::kMeshCode ) );
			var.ChangeType( VT_I4 );
			meshGeoMap.emplace( var.lVal, ipFeature->GetShape() );
			++count;
		}
		system( "cls" );
		cout << "メッシュ検索完了... " + to_string( count ) + " / " + to_string( meshList.size() ) << endl;
	}

	return true;
}

ITablePtr CCorrectZTool::openTable( const IWorkspacePtr& ipWorkspace, const CString& tableName, sindy::CFieldMap& fieldMap )
{
	ITablePtr ipTable;
	if( FAILED( IFeatureWorkspacePtr( ipWorkspace )->OpenTable( _bstr_t( tableName ), &ipTable ) ) || !ipTable )
	{
		RUNLOGGER.writeInfo( format( _T( "%s取得失敗" ), tableName ) );
		return nullptr;
	}
	fieldMap.CreateFieldMap( ipTable->GetFields() );

	return ipTable;
}

bool CCorrectZTool::isVersioned( const IFeatureClassPtr& ipFeatureClass )
{
	VARIANT_BOOL vbIsVersioned = VARIANT_FALSE;
	IVersionedObjectPtr( ipFeatureClass )->get_IsRegisteredAsVersioned( &vbIsVersioned );

	return vbIsVersioned ? true : false;
}

bool CCorrectZTool::startEdit( IWorkspacePtr& ipWorkspace )
{
	IMultiuserWorkspaceEditPtr ipMultiuserWorkspace( ipWorkspace );
	if( ipMultiuserWorkspace )
	{
		// 車線リンクのバージョン対応を代表として見ておく
		esriMultiuserEditSessionMode mode = isVersioned( m_ipLaneLink ) ? esriMESMVersioned : esriMESMNonVersioned;
		if( FAILED( ipMultiuserWorkspace->StartMultiuserEditing( mode ) ) )
		{
			RUNLOGGER.writeInfo( _T( "編集開始失敗" ) );
			return false;
		}

		if( mode == esriMESMVersioned )
		{
			if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditOperation() ) )
			{
				RUNLOGGER.writeInfo( _T( "編集開始失敗" ) );
				return false;
			}
		}
	}
	else
	{
		if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditing( VARIANT_FALSE ) ) )
		{
			RUNLOGGER.writeInfo( _T( "編集開始失敗" ) );
			return false;
		}
	}

	return true;
}

bool CCorrectZTool::stopEdit( IWorkspacePtr& ipWorkspace )
{
	IMultiuserWorkspaceEditPtr ipMultiuserWorkspace( ipWorkspace );
	if( ipMultiuserWorkspace && isVersioned( m_ipLaneLink ) )
	{
		if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditOperation() ) )
		{
			RUNLOGGER.writeInfo( _T( "編集終了失敗" ) );
			abortEdit( ipWorkspace );
			return false;
		}
	}

	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditing( VARIANT_TRUE ) ) )
	{
		RUNLOGGER.writeInfo( _T( "編集終了失敗" ) );
		abortEdit( ipWorkspace );
		return false;
	}

	return true;
}

void CCorrectZTool::abortEdit( IWorkspacePtr& ipWorkspace )
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	IWorkspaceEditPtr( ipWorkspace )->IsBeingEdited( &vb );
	if( !vb )
		return;

	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->AbortEditOperation() ) ||
		FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditing( VARIANT_FALSE ) ) )
		RUNLOGGER.writeInfo( _T( "編集破棄失敗" ) );
}