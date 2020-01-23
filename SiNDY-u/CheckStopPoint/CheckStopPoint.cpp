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
#include "CheckStopPoint.h"
#include "util.h"
#include <boost/range/algorithm.hpp>
#include <sindy/workspace.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <TDC/useful_headers/str_util.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace uh::str_util;

namespace
{
	crd_cnv g_crd;

	/**
	 * @brief 始点・終点間の距離をメートル単位で返す関数
	 * @param ipFrom [in] 始点
	 * @param ipTo   [in] 終点
	 * @retrun 始点・終点間の距離[m]
	 */
	double GetLengthBetween2Point( const IPointPtr& ipFrom, const IPointPtr& ipTo )
	{
		double dX1=0,dY1=0,dX2=0,dY2=0;
		ipFrom->QueryCoords( &dX1, &dY1 );
		ipTo->QueryCoords( &dX2, &dY2 );
		return g_crd.GetDist( dX1, dY1, dX2, dY2 );
	}

	/**
	 * @brief ポリラインの長さをメートルで返す関数
	 * @param ipLine [in] ポリライン
	 * @return ポリラインの長さ[m]
	 */
	double GetLineLength( const IPolylinePtr& ipLine )
	{
		double dLength = 0;
		IPointCollectionPtr ipPointCol = ipLine;
		long lPointCol = ipPointCol->GetPointCount();
		for( int i = 0; i < lPointCol-1; ++i )
		{
			IPointPtr ipPoint1, ipPoint2;
			ipPointCol->get_Point( i, &ipPoint1 );
			ipPointCol->get_Point( i+1, &ipPoint2 );
			dLength += GetLengthBetween2Point( ipPoint1, ipPoint2 );
		}
		return dLength;
	}

	/**
	 * @brief オープンしたワークスペースを取得
	 * @param  strWorkspaceName [in] オープンするワークスペース名
	 * @retval IWorkspacePtr ワークスペースのオープンに成功
	 * @retval nullptr       ワークスペースのオープンに失敗
	 */
	inline IWorkspacePtr openWorkspace(const CString& strWorkspaceName)
	{
		IWorkspacePtr ipWorkspace = create_workspace( strWorkspaceName );
		if( !ipWorkspace ){
			cerr << "#ERROR ワークスペースのオープンに失敗：" << CT2CA( strWorkspaceName ) << endl;
			return nullptr;
		}
		return ipWorkspace;
	}

	/**
	 * @brief エラーログを出力するための関数
	 * @param logM         [in] ログ出力管理インスタンス
	 * @param featureName  [in] ユーザー名付きフィーチャクラス名
	 * @param stopPoint    [in] エラーチェック対象一時停止ポイントの情報を持つ構造体
	 * @param stopPointOID [in] エラーチェック対象一時停止ポイントのオブジェクトID
	 * @param errorCode    [in] エラーコード
	 * @param meshcode     [in] チェック中のメッシュコード
	 */
	inline void writeErrorLog(CLogManager& logM, const CStringA& featureName, const CheckStopPoint::STOP& stopPoint, long stopPointOID, long errorCode, long meshcode)
	{
		logM.WriteCheckLog( featureName, stopPointOID, errorCode,
			stopPoint.ipPoint->GetX(), stopPoint.ipPoint->GetY(),
			meshcode, stopPoint.linkID, stopPoint.nodeID );
	}

}

bool CheckStopPoint::init()
{
	m_logManager.Init( m_strOutput );

	if( !util::loadMeshList( m_strMeshlist, m_meshList ) ){
		cerr << "#ERROR メッシュリストが開けません：" << CT2CA( m_strMeshlist ) << endl;
		return false;
	}

	// ワークスペースをオープンする
	if( !(m_ipRoadWorkspace     = openWorkspace( m_strDBRoad )) )      return false;
	if( !(m_ipStopWorkspace     = openWorkspace( m_strDBStop )) )      return false;
	if( !(m_ipMeshWorkspace     = openWorkspace( m_strDBMesh )) )      return false;

	// フィーチャクラスをオープンする
	if( !OpenFeatureClass( road_link::kTableName,    m_ipRoadWorkspace,     m_ipRoadLinkFC,       m_RoadLinkFM      ) ) return false;
	if( !OpenFeatureClass( road_node::kTableName,    m_ipRoadWorkspace,     m_ipRoadNodeFC,       m_RoadNodeFM      ) ) return false;
	if( !OpenFeatureClass( stop_point::kTableName,   m_ipStopWorkspace,     m_ipStopPointFC,      m_StopPointFM     ) ) return false;
	if( !OpenFeatureClass( basemesh::kTableName,     m_ipMeshWorkspace,     m_ipBaseMeshFC,       m_BaseMeshFM      ) ) return false;

	// チェックモードの場合は経年変化前の道路DBもオープンする
	if( m_check )
	{
		if( !(m_ipRoadPastWorkspace = openWorkspace( m_strDBRoad_past )) ) return false;
		if( !OpenFeatureClass( road_link::kTableName,    m_ipRoadPastWorkspace, m_ipRoadLinkFC_past,  m_RoadLinkFM_past ) ) return false;
		if( !OpenFeatureClass( road_node::kTableName,    m_ipRoadPastWorkspace, m_ipRoadNodeFC_past,  m_RoadNodeFM_past ) ) return false;
	}

	return true;
}

// フィーチャクラスオープン＆フィールドマップ作成
bool CheckStopPoint::OpenFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
	if( !ipFeatureClass ){
		cerr << "#ERROR フィーチャクラスのオープンに失敗：" << CT2CA( name ) << endl;
		return false;
	}
	fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

	return true;
}

// テーブルオープン＆フィールドマップ作成
bool CheckStopPoint::OpenTable(const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
	if( !ipTable ){
		cerr << "#ERROR テーブルのオープンに失敗：" << CT2CA( name ) << endl;
		return false;
	}
	fieldMap.CreateFieldMap( ipTable->GetFields() );

	return true;
}

bool CheckStopPoint::execute()
{
	if( !init() )
		return false;

	if( m_extract )
	{
		m_logManager.WriteExtractLogHeader();
		m_logManager.WriteInfo( "#INFO 抽出モード実行" );
	}
	if( m_check )
	{
		m_logManager.WriteCheckLogHeader();
		m_logManager.WriteInfo( "#INFO チェックモード実行" );
	}
	m_logManager.WriteInfo( "#INFO 道路系DB				：%s", (CStringA)m_strDBRoad );
	if( !m_strDBRoad_past.IsEmpty() )
		m_logManager.WriteInfo( "#INFO 道路系DB(経年変化前)	：%s", (CStringA)m_strDBRoad_past );
	m_logManager.WriteInfo( "#INFO 一時停止DB			：%s", (CStringA)m_strDBStop );
	m_logManager.WriteInfo( "#INFO メッシュDB			：%s", (CStringA)m_strDBMesh );
	m_logManager.WriteInfo( "#INFO エラーログファイル名	：%s", (CStringA)m_strOutput );
	m_logManager.WriteInfo( "#INFO メッシュリスト		：%s", (CStringA)m_strMeshlist );

	// メッシュごとの処理
	for( const auto& mesh : m_meshList ){
		m_logManager.WriteInfo( "#%d", mesh );
		cerr << "#" << mesh << endl;
		LINKMAP linkMap;
		NODEMAP nodeMap;

		// メッシュポリゴン取得
		IGeometryPtr ipMeshGeo;
		if( !GetMeshPolygon( mesh, ipMeshGeo ) ){
			m_logManager.WriteInfo( "#FATAL メッシュポリゴン取得失敗" );
			return false;
		}

		// メッシュ内の一時停止を取得
		STOPMAP stops;
		if( !GetStopInMesh( ipMeshGeo, stops ) ){
			m_logManager.WriteInfo( "#FATAL 既存一時停止の取得失敗" );
			return false;
		}

		// メッシュ内の道路リンクを取得
		if( !GetLinkNodeInMesh( ipMeshGeo, linkMap, nodeMap, m_ipRoadLinkFC, m_RoadLinkFM, m_ipRoadNodeFC, m_RoadNodeFM ) ){
			m_logManager.WriteInfo( "#FATAL メッシュ内の道路リンク取得失敗" );
			return false;
		}

		// 抽出処理
		if( m_extract )
			Extract( linkMap, nodeMap, stops, mesh );
		// チェック処理
		if( m_check )
		{
			// 経年変化前の形状を読み込んでおく
			LINKMAP linkMap_past;
			NODEMAP nodeMap_past;
			// メッシュ内の経年変化前の道路リンクを取得
			if( !GetLinkNodeInMesh( ipMeshGeo, linkMap_past, nodeMap_past, m_ipRoadLinkFC_past, m_RoadLinkFM_past, m_ipRoadNodeFC_past, m_RoadNodeFM_past ) ){
				m_logManager.WriteInfo( "#FATAL メッシュ内の経年変化前の道路リンク取得失敗" );
				return false;
			}
			ErrorCheck( linkMap, nodeMap, linkMap_past, nodeMap_past, stops, mesh );
		}
	}

	m_logManager.WriteInfo( "#INFO 正常終了" );
	
	return true;
}

void CheckStopPoint::Extract( const LINKMAP& linkMap, const NODEMAP& nodeMap, const STOPMAP& stops, long mesh )
{
	// TODO: 確認用なので後で削除してOK
	auto& domainValue = m_RoadLinkFM.GetDomain( road_link::kRoadClass );
	map<long, CString> roadClassName;
	for( auto& value : domainValue ){
		// なぜにキーが文字列なんだろう
		roadClassName[value.second] = value.first;
	}

	// 各ノードごとの処理
	for( auto& node : nodeMap ){

		// メッシュ境界ノードの場合、スキップ
		if( IsMeshEdge(node.second) )
			continue;

		long nodeID = node.first;

		map<long, LINK> okLinks;
		list<long> mainUpDownLinks;
		
		// 紐づいているリンク分の処理
		for( auto& linkID : node.second.linkIDs ){
			
			// リンク情報を取得
			auto& link = linkMap.find( linkID );

			// 取得できないものは、メッシュ外のものなのでスキップ
			if( link == linkMap.end() )
				continue;

			// メッシュ外のもの(接しているだけのもの)はスキップ
			if( link->second.outOfMesh )
				continue;

			long roadClass = link->second.roadClass; //!< ログ用

			// 既存一時停止に存在するか？
			if( FindStopPoint( linkID, nodeID, stops ) ){
				CStringA strLog;
				strLog.Format( "%d\t%d(%s)" , linkID, roadClass, CStringA(roadClassName.at( roadClass )) );
				m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "既存一時停止に存在", strLog );
				continue;
			}

			// 規制は問題ないか？
			if( !IsRegOK( link->second, nodeID ) ){
				CStringA strLog;
				strLog.Format( "%d\t%d(%s)" , linkID, roadClass, CStringA(roadClassName.at( roadClass )) );
				m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "一方通行/通行禁止に問題あり", strLog );
				continue;
			}

			// 抽出対象か？
			if( !IsExtractTarget( link->second, nodeID, linkMap, nodeMap ) ){
				CStringA strLog;
				strLog.Format( "%d\t%d(%s)" , linkID, roadClass, CStringA(roadClassName.at( roadClass )) );
				m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "抽出条件を満たさない", strLog );
				continue;
			}

			// --ここまでで抽出対象外を弾く

			// OKなものはひとまず格納しておく
			okLinks[linkID] = link->second;

			// 2条道路をなす可能性があるリンクはここで保持しておく
			// 今のところ上下分離リンクのみ
			if( road_link::main_link_class::kMainUpDown == link->second.linkClass )
				mainUpDownLinks.push_back( linkID );
		}

		// 2条道路をなすものが揃っている場合は、それらをOKから除外
		if( !mainUpDownLinks.empty() ){
			// TODO: 角度や他の条件が必要ならここに追記
			CString strLog;
			for( auto& mainUpDownID : mainUpDownLinks ){
				auto itr = okLinks.find( mainUpDownID );
				if( okLinks.end() != itr ){
					strLog.AppendFormat( _T("%d\t%d(%s)\t"), mainUpDownID, itr->second.roadClass, roadClassName.at( itr->second.roadClass ) );
					okLinks.erase( itr );
				}
			}
			m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "WARNING", "2条道路のため除外", CStringA(strLog) );
		}

		if( !( okLinks.empty() ) ){
			CString strLog;
			for( auto& ok : okLinks )
				strLog.AppendFormat( _T("%d\t%d(%s)\t"), ok.first, ok.second.roadClass, roadClassName.at( ok.second.roadClass ) );
			m_logManager.WriteExtractLog( road_node::kTableName, nodeID, mesh, "INFO", "抽出", CStringA(strLog) );
		}
	}
}

// 不正な一時停止ポイントを検出する
void CheckStopPoint::ErrorCheck( const LINKMAP& linkMap, const NODEMAP& nodeMap, const LINKMAP& linkMap_past, const NODEMAP& nodeMap_past, const STOPMAP& stops, long mesh )
{
	// ログに出力するオーナー付きフィーチャクラス名を用意しておく
	CStringA stopPointName( AheGetOwnerDotClassName(m_ipStopPointFC) );

	for( auto stopPairIt=stops.begin(); stopPairIt!=stops.end(); ++stopPairIt )
	{
		const STOP& targetPoint = stopPairIt->second;

		// 流入リンク・流出ノードが処理中のメッシュ内に存在するか確認
		// 流入リンクについては、メッシュに接しているだけのものは存在しない扱いとする
		bool bLinkExist = (linkMap.find(targetPoint.linkID)!=linkMap.end() && !linkMap.at(targetPoint.linkID).outOfMesh);
		bool bNodeExist = nodeMap.find(targetPoint.nodeID)!=nodeMap.end();

		// 以下、HC1000番台のエラー出力
		// 現在参照中のポイントの次のポイントから最後のポイントまで比較
		for(auto compPointIt = std::next(stopPairIt); compPointIt!=stops.end(); ++compPointIt)
		{
			// HC1001 複数のポイントが同一地点に位置
			if( AheIsEqual(targetPoint.ipPoint, compPointIt->second.ipPoint) )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1001, mesh );
			// HC1002 複数のポイントが位置する地点は同一ではないが、属性(流入リンク、流出ノード)の値が同じ
			else if( targetPoint==compPointIt->second )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1002, mesh );
		}

		// HC1003 流入リンクが無条件の一通の逆走、無条件通禁・通行不可道路
		if( bLinkExist && !IsRegOK( linkMap.at(targetPoint.linkID), targetPoint.nodeID ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1003, mesh );

		// HC1004 流出ノードが、信号機あり交差点ノード
		if( bNodeExist && IsSignalCross(nodeMap.at(targetPoint.nodeID)) )
			writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1004, mesh );

		// HC1005 一時停止ポイントに関連付く流入リンクの形状が経年変化orリンク上に存在しない場合にエラー
		if( bLinkExist && bNodeExist )
		{
			const LINK& lLink = linkMap.at(targetPoint.linkID);
			const NODE& lNode = nodeMap.at(targetPoint.nodeID);
			bool bError = false;
			// 流入リンク上に一時停止ポイントが存在しない場合、エラーとする
			IRelationalOperatorPtr ipRel( lLink.ipGeo );
			if( ipRel->_Disjoint( targetPoint.ipPoint ) != VARIANT_FALSE )
				bError = true;
			else
			{
				// 一時停止付与ツールと同じアルゴリズムでポイントを生成
				IGeometryPtr ipCompGeo = CreateStopPointGeometry(targetPoint.linkID, targetPoint.nodeID, lLink, lNode, lNode.ipGeo->GetSpatialReference());
				// 付与時と違う地点にできているなら、エラーとする
				if( !AheIsEqual(targetPoint.ipPoint,ipCompGeo) )
					bError = true;
			}
			// いずれか一つのエラー条件を満たしていたなら、1005エラーを出力する
			if( bError )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 1005, mesh );
		}

		if( bNodeExist && ! IsCross( nodeMap.at( targetPoint.nodeID ) ) )
		{
			// HC1006 流出ノードが2次メッシュ境界ノード
			if( IsMeshEdge( nodeMap.at( targetPoint.nodeID ) ) )
				writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1006, mesh );
			// HC1010 流出ノードが交差点ノード以外
			else
				writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1010, mesh );
		}
		
		// HC1007 流入リンクが環状交差点リンク
		if( bLinkExist && IsRoundAbout( linkMap.at(targetPoint.linkID) ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1007, mesh );

		// HC1008 流入リンクが本線（上下分離）リンク
		if( bLinkExist && IsMainUpDown( linkMap.at(targetPoint.linkID) ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1008, mesh );

		// HC1009 流入リンクがHWY本線
		if( bLinkExist && IsHighwayMain( linkMap.at(targetPoint.linkID) ) )
			writeErrorLog( m_logManager, stopPointName, targetPoint, stopPairIt->first, 1009, mesh );

		//以下 HC 2000番台のエラー検出
		if( !bLinkExist )
		{
			// HC2001 ノードは存在し、リンクは存在しない
			if( bNodeExist )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2001, mesh );
			else // HC2003 リンク・ノード共存在しない
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2003, mesh );
		}
		else if( !bNodeExist )  // HC2002 流入ノードが存在しない
			writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2002, mesh );

		// HC2005 指定のIDの流入リンク、および流出ノードが存在するが、形状が一致しない
		if( bLinkExist && bNodeExist )
		{
			// 流入リンクのFromIDともToIDとも流出ノードが一致しない
			if( linkMap.at(targetPoint.linkID).fromID != targetPoint.nodeID &&
				linkMap.at(targetPoint.linkID).toID != targetPoint.nodeID )
				writeErrorLog( m_logManager, stopPointName, targetPoint,  stopPairIt->first, 2004, mesh );

			// 経年変化前後でNW形状が異なる場合の出力(ERROR_CODE:2005)は除外された（bug 11276）
		}
	}
}

// メッシュポリゴン取得
bool CheckStopPoint::GetMeshPolygon( long mesh, IGeometryPtr& ipMeshGeo )
{
	// メッシュコードからメッシュポリゴン取得
	CString strWhere;
	strWhere.Format( _T("%s = %d"), basemesh::kMeshCode, mesh );
	IFeatureCursorPtr ipMeshCursor = m_ipBaseMeshFC->_Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE );
	if( !ipMeshCursor )
		return false;
	IFeaturePtr ipMeshFeature;
	if( S_OK == ipMeshCursor->NextFeature( &ipMeshFeature ) && ipMeshFeature ){

		ipMeshGeo = ipMeshFeature->GetShapeCopy();
	}

	return ( ipMeshGeo != nullptr );
}

// メッシュ内の既存一時停止を取得
bool CheckStopPoint::GetStopInMesh( const IGeometryPtr& ipMeshGeo, STOPMAP& stops )
{
	CString subField;
	subField.Format( _T("%s,%s,%s,SHAPE"), kObjectID, stop_point::kLinkID, stop_point::kNodeID );
	IFeatureCursorPtr ipCursor = m_ipStopPointFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_TRUE );
	if( !ipCursor )
		return false;
	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

		STOP stop;
		stop.ipPoint = IPointPtr(ipFeature->GetShapeCopy());
		if( !stop.ipPoint )
		{
			CStringA strError;
			strError.Format("#FATAL 一時停止ポイントが不正な形状 : OBJECTID[%ld]", ipFeature->OID);
			m_logManager.WriteInfo( strError );
			return false;
		}
		stop.linkID = (ipFeature->GetValue( m_StopPointFM.FindField( stop_point::kLinkID ) )).lVal;
		stop.nodeID = (ipFeature->GetValue( m_StopPointFM.FindField( stop_point::kNodeID ) )).lVal;
		stops.insert( make_pair(ipFeature->OID, stop) );
	}
	
	return true;
}

// メッシュ内の道路リンクと道路ノードを取得
bool CheckStopPoint::GetLinkNodeInMesh( const IGeometryPtr& ipMeshGeo, LINKMAP& linkMap, NODEMAP& nodeMap,
	IFeatureClassPtr& ipLinkFC, sindy::CFieldMap& linkFM, IFeatureClassPtr& ipNodeFC, sindy::CFieldMap& nodeFM )
{
	IRelationalOperatorPtr ipRelOpe( ipMeshGeo );
	CString subField;
	subField.Format( _T("%s,%s,%s,%s,%s,%s,%s,%s,%s,SHAPE"), kObjectID, road_link::kRoadClass, road_link::kMainLinkClass, road_link::kUpDownClass,
		road_link::kRoadWidth, road_link::kFromNodeID, road_link::kToNodeID, road_link::kOneway, road_link::kNoPassage );
	// ジオメトリを保持するため、VARIANT_FALSE
	IFeatureCursorPtr ipLinkCursor = ipLinkFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipLinkCursor )
		return false;
	IFeaturePtr ipLinkFeature;

	// 道路リンクのループ
	while( S_OK == ipLinkCursor->NextFeature( &ipLinkFeature ) && ipLinkFeature ){
		long roadClass = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kRoadClass ) )).lVal;
			
		// roadclass_c >= 201 の道路リンクは、存在しないものとしてみなす。
		if( IsNoTarget( roadClass ) )
			continue;

		// ObjectIDや必要情報取得
		long linkID = ipLinkFeature->GetOID();
		long fromID = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kFromNodeID ) )).lVal;
		long toID   = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kToNodeID ) )).lVal;
		LINK link;
		link.fromID      = fromID;
		link.toID        = toID;
		link.roadClass   = roadClass;
		link.linkClass   = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kMainLinkClass ) )).lVal;
		link.upDownClass = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kUpDownClass ) )).lVal;
		link.roadWidth   = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kRoadWidth ) )).lVal;
		long onewayC     = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kOneway ) )).lVal;
		long nopassC     = (ipLinkFeature->GetValue( linkFM.FindField( road_link::kNoPassage ) )).lVal;
		if( road_link::oneway::kFore == onewayC )
			link.foreOneway = true;
		else if( road_link::oneway::kReverse == onewayC )
			link.revOneway = true;
		if( road_link::no_passage::kNoPassage == nopassC || road_link::no_passage::kInpassable == nopassC )
			link.noPassage = true;

		link.ipGeo = ipLinkFeature->GetShapeCopy();

		// ノードIDをキーに、接続リンク数を格納
		nodeMap[fromID].linkIDs.push_back( linkID );
		nodeMap[toID].linkIDs.push_back( linkID );

		VARIANT_BOOL vaContains;
		if( FAILED( ipRelOpe->Contains( ipLinkFeature->GetShape(), &vaContains ) )){
			m_logManager.WriteInfo( "#FATAL メッシュとのContains判定に失敗：リンクID(%d)", linkID );
			continue;
		}
		if( !vaContains )
			link.outOfMesh = true;

		linkMap[linkID] = move(link);
	}

	subField.Format( _T("%s,%s,%s,SHAPE"), kObjectID, road_node::kNodeClass, road_node::kSignal );
	IFeatureCursorPtr ipNodeCursor = ipNodeFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_TRUE );
	if( !ipNodeCursor )
		return false;
	IFeaturePtr ipNodeFeature;

	// 道路ノードのループ
	while( S_OK == ipNodeCursor->NextFeature( &ipNodeFeature ) && ipNodeFeature ){
		long nodeID = ipNodeFeature->GetOID();

		// ノードIDをキーに、形状・種別・信号機属性を格納
		nodeMap[nodeID].ipGeo     = ipNodeFeature->GetShapeCopy();
		nodeMap[nodeID].nodeClass = (ipNodeFeature->GetValue( nodeFM.FindField( road_node::kNodeClass ) )).lVal;
		nodeMap[nodeID].signal    = (ipNodeFeature->GetValue( nodeFM.FindField( road_node::kSignal ) )).lVal;
	}

	return true;
}

// 存在しないものと見なす道路種別か？
bool CheckStopPoint::IsNoTarget( long roadClass )
{
	using namespace road_link::road_class;
	switch( roadClass ){

		// roadclass_c >= 201 の道路リンクは、存在しないものとしてみなす。
		case kFerryNoDisp:
		case kFerryS2:
		case kFerryS3:
		case kFerryS4:
		case kVirtual:
		case kGarden:
		case kBridge:
		case kFacilityEntrance:
		case kParkingEntrance:
		case kParking:
		case kMatching:
		case kMatchingBT:
			return true;
		default:
			return false;
	}
	return false;
}

// リンクIDとノードIDの組み合わせが既存一時停止に存在するか？
bool CheckStopPoint::FindStopPoint( long linkID, long nodeID, const STOPMAP& stops )
{
	STOP stop;
	stop.linkID = linkID;
	stop.nodeID = nodeID;
	return (stops.end() != boost::find_if(stops, [&](const std::pair<long, STOP>& st)->bool{ return (st.second==stop);} ) );
}

// リンクとノードの関係から、抽出条件を満たすか確認する
bool CheckStopPoint::IsExtractTarget( const LINK& link, long nodeID, const LINKMAP& links, const NODEMAP& nodeMap )
{
	using namespace sindy::schema::road_link::road_class;
	using namespace sindy::schema::road_link::road_width;

	// リンクが3本以上接続していない場合は条件に合わない
	auto itr = nodeMap.find( nodeID );
	if( itr == nodeMap.end() || itr->second.linkIDs.size() < 3 )
		return false;

	// main_lincclass_c <> 8 & updownclass_c in (0, 6) を満たさないものは条件に合わない
	if( !IsNotBicycleNotHWY( link ) )
		return false;

	switch( link.roadClass ){

		// 細街路L2：roadclass_c = 9
		// 細街路L1：roadclass_c = 8
		// その他道路（間引き対象）：roadclass_c = 7
		// その他道路：roadclass_c in (6, 108)
		case kNarrowL2:
		case kNarrowL1:
		case kOtherNoDisp:
		case kOther:
		case kTollOther:
		{
			/* 
			ひとまず幅員条件は外してよいということ
			TODO: 幅員条件が必要であれば、上記caseも必要に応じて分離すること
			if( link.roadWidth == kBetween30And55 || link.roadWidth == kBetween55And130 )
				return true;
			*/
			return true;
		}

		// 指定市の一般市道：roadclass_c in (5, 107)
		case kCityNormal:
		case kTollCityNormal:
		{
			// 交差点ノードである必要あり
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// 信号機なし（signal_c = 0）
			if( road_node::signal::kNoSignal == itr->second.signal ){
				// 幅員：3.0ｍ以上5.5ｍ未満, 5.5m以上13.0m未満
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}
			// 信号機あり（signal_c = 1）
			else if( road_node::signal::kSignal == itr->second.signal ){
				// 幅員：3.0ｍ以上5.5ｍ未満
				if( link.roadWidth != kBetween30And55 )
					return false;

				// 主要地方道以上のリンクが2本以上
				int OkLinkCount = 0;
				for( auto& linkID : itr->second.linkIDs ){
					if( IsPrefOrOver( links, linkID ) )
						++OkLinkCount;
				}
				if( OkLinkCount > 1 )
					return true;
			}

			return false;
		}

		// 一般都道府県道：roadclass_c in (4, 106)
		case kPrefectureNormal:
		case kTollPrefectureNormal:
		{
			// 交差点ノードである必要あり
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// 信号機なし（signal_c = 0）
			if( road_node::signal::kNoSignal == itr->second.signal ){
				
				// 幅員：3.0ｍ以上5.5ｍ未満, 5.5m以上13.0m未満
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}
			// 信号機あり（signal_c = 1）
			else if( road_node::signal::kSignal == itr->second.signal ){

				int OkLinkCount = 0;
				for( auto& linkID : itr->second.linkIDs ){
					if( IsCountryOrOver( links, linkID ) )
						++OkLinkCount;
				}
				if( OkLinkCount > 1 )
					return true;
			}

			return false;
		}

		// 主要地方道（都府県）/主要地方道（指定市）：roadclass_c in (2, 3, 104, 105)
		case kPrefectureMain:
		case kCityMain:
		case kTollPrefectureMain:
		case kTollCityMain:
		{
			// 交差点ノードである必要あり
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// 交差点ノード（信号機なし）：signal_c = 0
			if( road_node::signal::kNoSignal == itr->second.signal ){

				// 幅員：3.0ｍ以上5.5ｍ未満, 5.5m以上13.0m未満
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}

			return false;
		}

		// 国道：roadclass_c in (1, 103)
		case kCountry:
		case kTollCountry:
		{
			// 交差点ノードである必要あり
			if( road_node::node_class::kCross != itr->second.nodeClass )
				return false;

			// 交差点ノード（信号機なし）：signal_c = 0
			if( road_node::signal::kNoSignal == itr->second.signal ){

				// 幅員：3.0ｍ以上5.5ｍ未満, 5.5m以上13.0m未満
				if( link.roadWidth != kBetween30And55 && link.roadWidth != kBetween55And130 )
					return false;

				return true;
			}
			return false;
		}
		default:
			break;
	}

	return false;
}

// リンクIDからリンクを取得し、主要地方道以上か確認する
bool CheckStopPoint::IsPrefOrOver( const LINKMAP& links, long linkID )
{
	//     HWY対象道路(updownclass_c not in (0,6))
	//     または
    //     HWY対象道路ではなくて、主要地方道以上(updownclass_c in (0,6) and roadclass_c in (1,2,3,103,104,105))
	using namespace sindy::schema::road_link::road_class;
	using namespace sindy::schema::road_link::up_down_class;

	try{
		auto& link = links.at( linkID );

		if( kNoHighway != link.upDownClass && kEtcAccess != link.upDownClass )
			return true;

		else{
			switch( link.roadClass ){

				case kCountry:
				case kTollCountry:
				case kPrefectureMain:
				case kTollPrefectureMain:
				case kCityMain:
				case kTollCityMain:
					return true;
				default:
					return false;
			}
		}
	}	
	catch( ... ){
		return false;
	}
	return false;

}

// リンクIDからリンクを取得し、国道以上か確認する
bool CheckStopPoint::IsCountryOrOver( const LINKMAP& links, long linkID )
{
	//     HWY対象道路(updownclass_c not in (0,6))
	//     または
    //     HWY対象道路ではなくて、国道以上(updownclass_c in (0,6) and roadclass_c in (1,103))
	using namespace sindy::schema::road_link::road_class;
	using namespace sindy::schema::road_link::up_down_class;

	try{
		auto& link = links.at( linkID );

		if( kNoHighway != link.upDownClass && kEtcAccess != link.upDownClass )
			return true;

		if( kCountry == link.roadClass || kTollCountry == link.roadClass )
			return true;
	}	
	catch( ... ){
		return false;
	}
	return false;
}

// 自転車道ではなく(リンク種別(not 8))＆HYWではない(ハイウェイ路線(0,6))か確認する
bool CheckStopPoint::IsNotBicycleNotHWY( const LINK& link )
{
	using namespace sindy::schema::road_link::main_link_class;
	using namespace sindy::schema::road_link::up_down_class;
	
	if( link.linkClass != kBicycle && ( link.upDownClass == kNoHighway || link.upDownClass == kEtcAccess ))
		return true;

	return false;
}

// 通行禁止がついていないか、一時停止の向きが問題ないか確認する
bool CheckStopPoint::IsRegOK( const LINK& link, long nodeID )
{
	// 通行禁止がついている場合
	if( link.noPassage )
		return false;

	// 正方向一通 ＆ リンクから見て流出ノードがfromIDとなっている場合(一通逆走)
	if( link.foreOneway && nodeID == link.fromID )
		return false;

	// 逆方向一通 ＆ リンクから見て流出ノードがtoIDとなっている場合(一通逆走)
	if( link.revOneway && nodeID == link.toID )
		return false;
	
	return true;
}

// 道路リンクが環状交差点リンクであるかを判定する
bool CheckStopPoint::IsRoundAbout(const LINK & link)
{
	return ( link.linkClass == sindy::schema::road_link::link_class::kRoundAbout );
}

// 道路リンクが本線（上下分離）リンクであるかを判定する
bool CheckStopPoint::IsMainUpDown(const LINK & link)
{
	return ( link.linkClass == sindy::schema::road_link::link_class::kMainUpDown );
}

// 道路リンクがHWY本線であるかを判定する
bool CheckStopPoint::IsHighwayMain(const LINK & link)
{
	return (link.upDownClass==sindy::schema::road_link::up_down_class::kUpInWest ||
			link.upDownClass==sindy::schema::road_link::up_down_class::kDownOutEast ||
			link.upDownClass==sindy::schema::road_link::up_down_class::kCommon );
}

// 2次メッシュ境界ノードかどうかを判定する
bool CheckStopPoint::IsMeshEdge( const NODE& node)
{
	return (road_node::node_class::kEdge == node.nodeClass ||
			road_node::node_class::kPecEdge == node.nodeClass);
}

// 道路ノードが交差点ノードかどうかを判定する
bool CheckStopPoint::IsCross(const NODE& node)
{
	return (road_node::node_class::kCross == node.nodeClass);
}

// 道路ノードが信号機付き交差点であるかどうかを判定する
bool CheckStopPoint::IsSignalCross( const NODE& node )
{
	return (road_node::node_class::kCross == node.nodeClass &&
			road_node::signal::kSignal == node.signal);
}

// 2つのリンクがなす角度を求める
double CheckStopPoint::GetAngle( const LINK& link1, const LINK& link2 )
{
	return AheGetAngle( (ICurvePtr)link1.ipGeo, (ICurvePtr)link2.ipGeo );
}

// 一時停止ポイントを生成する関数
IGeometryPtr CheckStopPoint::CreateStopPointGeometry(long linkID, long nodeID, const LINK& link,  const NODE& node, ISpatialReferencePtr ipSpRef)
{
	IGeometryPtr ipRoadGeom(AheCreateClone(link.ipGeo)), ipNodeGeom(AheCreateClone(node.ipGeo));
	IConstructPointPtr ipConstructPoint( ipNodeGeom );
	double dLineLength = GetLineLength( (IPolylinePtr)ipRoadGeom );

	if( dLineLength > 11.0 )
	{
		// Fromから5mの地点のポイント取得
		if( link.fromID == nodeID )
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, 5.0 / dLineLength, VARIANT_TRUE );
		// Toから5mのポイント取得
		else
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, ( dLineLength - 5.0 ) / dLineLength, VARIANT_TRUE );
	}
	else
	{
		// Fromから、リンクの長さの1/4m地点のポイント取得
		if( link.fromID == nodeID )
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, 0.25 , VARIANT_TRUE );
		// Toから、リンクの長さの1/4m地点のポイント取得
		else
			ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipRoadGeom, esriExtendAtFrom, 0.75 , VARIANT_TRUE );

	}
	((IGeometryPtr)ipConstructPoint)->putref_SpatialReference( ipSpRef );

	return ipConstructPoint;
}
