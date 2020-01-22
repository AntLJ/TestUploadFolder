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
#include "CheckFollowingRoad.h"
#include "ArcHelperEx/AheMeshUtilArcObjects.h"
#include "ArcHelperEx/AheGlobalsCoord.h"
#include "ArcHelperEx/GlobalFunctions.h"
#include "TDC/sindylib_base/ContainerBase.h"
#include "TDC/sindylib_base/RowContainer.h"
#include "TDC/sindylib_base/TableContainer.h"
#include "ArcHelperEx/AheMeshUtil.h"
#include "ErrTypeDef.h"
#include <sindy/libschema.h>

using namespace std;
using namespace uh;
using namespace uh::str_util;
using namespace uh::variant_util;
using namespace sindy;
using namespace sindy::schema;
using namespace err_type;

bool CCheckFollowingRoad::execute()
{
	// 事前準備
	if( !preProcessing() )
		throw exception( "処理を開始できません" );

	// メッシュ単位で処理を実施
	bool isSucceedAll = true;
	for( const long mesh : m_Meshes )
	{
		m_RunLogger.writeLogAndConsole( format( _T("meshcode: %d ...処理開始"), mesh ) );
		bool isSucceed = doProcessing( mesh );
		m_RunLogger.writeLogAndConsole( 
			format( _T("meshcode: %d ...%s"), mesh, (isSucceed ? _T("正常終了") : _T("異常終了")) )
		);
		isSucceedAll &= isSucceed;
	}

	return isSucceedAll;
}

bool CCheckFollowingRoad::preProcessing()
{
	CWorkspace wsBaseDB, wsCompDB;
	
	// 各DBに接続できるか確認
	bool isConnected = true;
	isConnected &= connectDB( m_Args.m_BaseDB, wsBaseDB );
	isConnected &= connectDB( m_Args.m_CompDB, wsCompDB );
	if( !isConnected )
		return false;

	// ノードとリンクが各DBに存在するか確認
	ITablePtr ipBaseNode, ipBaseLink, ipCompNode, ipCompLink;
	bool isExistTable = true;
	isExistTable &= openTargetTable( wsBaseDB, road_node::kTableName, ipBaseNode );
	isExistTable &= openTargetTable( wsCompDB, road_node::kTableName, ipCompNode );
	isExistTable &= openTargetTable( wsBaseDB, road_link::kTableName, ipBaseLink );
	isExistTable &= openTargetTable( wsCompDB, road_link::kTableName, ipCompLink );
	if( !isExistTable )
		return false;

	// モデル設定
	CModel model;
	model.Add( road_node::kTableName, road_node::kObjectID, nullptr, nullptr );
	model.Add( road_link::kTableName, road_link::kFromNodeID, road_node::kTableName, road_node::kObjectID );
	model.Add( road_link::kTableName, road_link::kToNodeID, road_node::kTableName, road_node::kObjectID );
	m_BaseFinder.InitCollection(ipBaseNode, model);
	m_CompFinder.InitCollection(ipCompNode, model);

	return true;
}

bool CCheckFollowingRoad::doProcessing( const long mesh )
{
	m_ErrLogger.write( format( _T("#%d"), mesh ) );

	auto& baseNodeT = *m_BaseFinder.FindTable( road_node::kTableName );
	baseNodeT.clear();
	auto& compNodeT = *m_CompFinder.FindTable( road_node::kTableName );
	compNodeT.clear();

	// 道なりリンク列をチェック
	return chkDiffFollowingRoad( mesh, baseNodeT, compNodeT );
}

bool CCheckFollowingRoad::connectDB( const tstring& dbName, CWorkspace& ws )
{
	if( ws.Connect( dbName.c_str() ) )
		return true;

	const CString msg( format( _T("%sに接続できません"), dbName.c_str() ) );
	m_RunLogger.writeErrMsg( msg );
	return false;
}

bool CCheckFollowingRoad::openTargetTable( const CWorkspace& ws, const tstring& tableName, ITablePtr& table )
{
	table = ws.OpenSameOwnersTable( tableName.c_str() );
	if( table )
		return true;

	const CString msg( format( _T("%sに%sが存在しません"), ws.GetNameString()->GetPathName(), tableName.c_str() ) );
	m_RunLogger.writeErrMsg( msg );
	return false;
}

bool CCheckFollowingRoad::getFollowingRoad( const long nodeOID, const CTableContainer& linkT, FollowingRoad& followingRoad )
{
	// 道路ノードから道路リンクの構成点までの直線がなす角度をそれぞれ求める
	map<long, double> mapAngle; // (道路リンクのOID, 道路ノードから構成点までの角度)
	for( auto& linkCon : linkT )
	{
		const auto& linkRow = *CAST_ROWC( linkCon );

		IPolylinePtr ipLinkLine( linkRow.CRowBase::GetShapeCopy() );
		if( !ipLinkLine )
		{
			const CString msg(
				format( _T("%s[%d]:リンク形状を取得できません"), road_link::kTableName, linkRow.GetOID() ) 
				); 
			throw exception( toStr( msg.GetString() ).c_str() );
		}

		IPointCollectionPtr ipPointCol( ipLinkLine );
		long fromIdx = 0, toIdx = 1;
		// 処理中の道路ノードが道路リンクの終点であれば、終点側から遡る
		if( nodeOID == linkRow.GetValue(road_link::kToNodeID).lVal )
		{
			fromIdx = ipPointCol->GetPointCount() - 1;
			toIdx = ipPointCol->GetPointCount() - 2;
		}

		// 道路ノード側の端点を取得
		IPointPtr ipFromPoint;
		if( FAILED( ipPointCol->get_Point( fromIdx, &ipFromPoint ) ) )
		{
			const CString msg(
				format( _T("%s[%d]:ノード側の端点を取得できません"), road_link::kTableName, linkRow.GetOID() )
				); 
			throw exception( toStr( msg.GetString() ).c_str() );
		}

		// 1つ隣の構成点を取得
		IPointPtr ipToPoint;
		if( FAILED( ipPointCol->get_Point( toIdx, &ipToPoint ) ) )
		{
			const CString msg(
				format( _T("%s[%d]:構成点を取得できません"), road_link::kTableName, linkRow.GetOID() )
				); 
			throw exception( toStr( msg.GetString() ).c_str() );
		}

		// FROMからTOまでの角度[°]を取得
		double angle = getAngle( ipFromPoint, ipToPoint );

		mapAngle.emplace( make_pair( linkRow.GetOID(), angle ) );
	}

	// ペアができなくなるまで道なりリンク列を特定
	const size_t followingPairNum = mapAngle.size() / 2;
	unsigned int getCnt = 0;
	while( getCnt < followingPairNum )
	{
		// リンク同士の接続角度を求め、道なりリンク列を特定する
		multimap< double, pair<long, long> > mapDiffAngle;
		for( auto& it_angle1 = mapAngle.begin(); it_angle1 != mapAngle.end(); ++it_angle1 )
		{
			auto it_angle2 = it_angle1;
			++it_angle2;

			for( ; it_angle2 != mapAngle.end(); ++it_angle2 )
			{
				double diffAngle = abs( it_angle1->second - it_angle2->second );
				// 0° ≦ diffAngle ≦ 180°
				if( 180 < diffAngle ) diffAngle = 360 - diffAngle;
				
				mapDiffAngle.emplace(make_pair(diffAngle, make_pair(it_angle1->first, it_angle2->first)));
			}
		}

		// 最大接続角度を持つペアが複数あり、
		// これが(存在するペアの数 - これまでに見つけたペアの数)よりも多くなった場合は、
		// 道なりリンク列を特定できない
		double maxDiffAngle = mapDiffAngle.rbegin()->first;
		const size_t maxDiffCnt = mapDiffAngle.count( maxDiffAngle );
		if( (followingPairNum - getCnt) < maxDiffCnt )
			return false;

		auto& it_max = mapDiffAngle.find( maxDiffAngle );
		if( 1 < maxDiffCnt )
		{
			// 複数のペアが同じ道路リンクを通る場合は、道なりリンク列を特定できない
			auto it = it_max;
			map<long, int> mapOidCnt;
			for( unsigned int i = 0; i < maxDiffCnt; ++i )
			{
				++mapOidCnt[it->second.first];
				++mapOidCnt[it->second.second];
				++it;
			}
			for( const auto& oid : mapOidCnt )
			{
				if( 1 < oid.second )
					return false;
			}
		}

		for( unsigned int i = 0; i < maxDiffCnt; ++i )
		{
			followingRoad.emplace( make_pair( it_max->second, it_max->first ) );
			// 道なりリンク列となった道路リンクは除外
			mapAngle.erase(it_max->second.first);
			mapAngle.erase(it_max->second.second);
			++it_max;
			++getCnt;
		}
	}

	return true;
}

double CCheckFollowingRoad::getAngle( const IPointPtr& fromPoint, const IPointPtr& toPoint )
{
	double fromX = -1.0, fromY = -1.0, toX = -1.0, toY = -1.0;
	fromPoint->QueryCoords( &fromX, &fromY );
	toPoint->QueryCoords( &toX, &toY );

	double distX = -1.0, distY = -1.0;
	g_cnv.GetDistXY(fromX, fromY, toX, toY, &distX, &distY );

	double angle = atan2( distY, distX );

	// 0 ≦ angle ≦ 2π
	if( 0 > angle )	angle += 2 * M_PI;
	
	// [rad]から[°]に変換
	return (angle * 180 / M_PI);
}

bool CCheckFollowingRoad::hasChangedConnectedLink( const CTableContainer& baseLinkT, const CTableContainer& compLinkT )
{
	for( const auto& compLinkCon : compLinkT )
	{
		const auto& compLinkRow = *CAST_ROWC(compLinkCon);
		const long compLinkOID = compLinkRow.GetOID();

		// 比較先バージョンの道路リンクのOIDで、比較元バージョンの道路リンクを検索
		// ヒットしなかった場合はバージョン間で変化あり
		if( baseLinkT.end() == baseLinkT.find_by_key( road_link::kObjectID, compLinkOID ) )
			return true;
	}
	return false;
}

bool CCheckFollowingRoad::isModifiedSinceDesignedDate( const CRowContainer& nodeRow )
{
	// 最終更新日が指定されていなければ、更新日のチェックはしない
	if( isNullOrEmpty( m_Args.m_ModDate ) )
		return true;

	// 始めに道路ノードをチェック

	// 最終更新日
	const auto& nodeModDate = nodeRow.GetValue(road_node::kModifyDate);
	if( !isNullOrEmpty(nodeModDate) && m_Args.m_ModDate < nodeModDate )
		return true;

	// 最終プログラム更新日時
	const auto& nodeProgModDate = nodeRow.GetValue(road_node::kProgModifyDate);
	if(!isNullOrEmpty(nodeProgModDate) && m_Args.m_ModDate < nodeProgModDate )
		return true;

	// 次に道路ノードに接続する道路リンクをチェック
	const auto& linkT = *nodeRow.GetItemTable( road_link::kTableName );
	for( const auto& linkCon : linkT )
	{
		const auto& linkRow = *CAST_ROWC(linkCon);

		// 最終更新日
		const auto& linkModDate = linkRow.GetValue(road_link::kModifyDate);
		if( !isNullOrEmpty(linkModDate) && m_Args.m_ModDate < linkModDate )
			return true;

		// 最終プログラム更新日時
		const auto& linkProgModDate = linkRow.GetValue(road_link::kProgModifyDate);
		if( !isNullOrEmpty(linkProgModDate) && m_Args.m_ModDate < linkProgModDate )
			return true;
	}
	
	return false;
}

bool CCheckFollowingRoad::getMapFollowingRoad( const CTableContainer& nodeT, map<long, FollowingRoad>& mapFollowingRoad )
{
	// 処理上のエラーが発生したらfalse
	bool isSucceed = true;

	for( const auto& nodeCon : nodeT )
	{
		const auto& baseNodeRow = *CAST_ROWC(nodeCon);
		const size_t baseNodeOID = baseNodeRow.GetOID();
		const auto& baseLinkT = *baseNodeRow.GetItemTable( road_link::kTableName );
		// 接続する道路リンクが3本未満であれば対象外
		if( 3 > baseLinkT.size() )
			continue;

		// 道なりリンク列を取得する
		FollowingRoad baseFollowingRoad;
		try
		{
			if( !getFollowingRoad( baseNodeOID, baseLinkT, baseFollowingRoad ) )
			{
				// 道なりリンク列を特定できなかったらエラー(105)
				m_ErrLogger.writeErrInfo( baseNodeRow, kBaseUnspecified );
				continue;
			}
		}
		catch(const exception& e)
		{
			m_RunLogger.writeErrMsg( e.what() );
			isSucceed = false;
			continue;
		}

		mapFollowingRoad.emplace( make_pair( baseNodeOID, baseFollowingRoad ) );
	}

	return isSucceed;
}

bool CCheckFollowingRoad::chkDiffFollowingRoad( const long mesh, CTableContainer& baseNodeT, CTableContainer& compNodeT )
{
	// 処理上のエラーが発生したらfalse
	bool isSucceed = true;

	// メッシュ矩形取得
	IEnvelopePtr env( AheMESH2ENV( mesh, AheGetSpatialReference( (ITable*)baseNodeT ) ) );
	if( !env )
	{
		m_RunLogger.writeErrMsg( _T("メッシュポリゴンの取得に失敗") );
		return false;
	}

	// 比較元より、メッシュ矩形内の道路ノードと、それに接続する道路リンクを取得
	map<CString, IQueryFilterPtr> mapQuery;
	const IQueryFilterPtr ipSpFilter( AheInitSpatialFilter( nullptr, env ) );
	mapQuery.emplace( make_pair( road_node::kTableName, ipSpFilter ) );
	baseNodeT.Select( mapQuery );

	// 比較元の道路ノードと道なりリンク列の対応マップを取得
	map<long, FollowingRoad> mapBaseFollowingRoad;
	isSucceed &= getMapFollowingRoad( baseNodeT, mapBaseFollowingRoad );
	
	// 比較元の道路ノードのOIDをキーに、比較先から同じOIDの道路ノードをセレクトする
	list<long> baseNodeList;
	for( const auto& followingRoad : mapBaseFollowingRoad )
	{
		baseNodeList.emplace_back( followingRoad.first );
	}
	compNodeT.Select( baseNodeList );

	// 比較元と比較先で道なりリンク列の変化をチェック
	for( auto& compNodeCon : compNodeT )
	{
		const auto& compNodeRow = *CAST_ROWC(compNodeCon);
		const auto& compLinkT = *compNodeRow.GetItemTable( road_link::kTableName );
		const size_t compLinkNum = compLinkT.size();
		// 接続する道路リンクの本数が3本未満ならスキップ
		if( 3 > compLinkNum )
			continue;

		// 最終更新日が指定されている場合、道路ノードとそれに接続する道路リンクが
		// いずれも指定された日付以降に更新されていなければ、チェック対象としない
		if( !isModifiedSinceDesignedDate( compNodeRow ) )
			continue;

		const long compNodeOID = compNodeRow.GetOID();

		const auto& it_Base = baseNodeT.find_by_key( road_node::kObjectID, compNodeOID );
		const auto& baseNodeRow = *CAST_ROWC(*it_Base);
		const auto& baseLinkT = *baseNodeRow.GetItemTable( road_link::kTableName );
		const size_t baseLinkNum = baseLinkT.size();

		// 道路リンクの本数変化をチェック
		if( compLinkNum > baseLinkNum )
		{
			// 道路リンクの本数が増えていたらエラー(103)
			m_ErrLogger.writeErrInfo( compNodeRow, kIncreaseLink );
			continue;
		}
		else if( compLinkNum < baseLinkNum )
		{
			// 道路リンクの本数が減っていたらエラー(104)
			m_ErrLogger.writeErrInfo( compNodeRow, kDecreaseLink );
			continue;
		}

		// バージョン間で道路ノードに接続する道路リンクが変化していたらエラー(102)
		if( hasChangedConnectedLink( baseLinkT, compLinkT ) )
		{
			m_ErrLogger.writeErrInfo( compNodeRow, kChangeLink );
			continue;
		}

		FollowingRoad compFollowingRoad;
		try
		{
			if( !getFollowingRoad( compNodeOID, compLinkT, compFollowingRoad ) )
			{
				// 道なりリンク列を特定できなかったらエラー(106)
				m_ErrLogger.writeErrInfo( compNodeRow, kCompUnspecified );
				continue;
			}
		}
		catch(const exception& e)
		{
			m_RunLogger.writeErrMsg( e.what() );
			isSucceed = false;
			continue;
		}

		const auto& baseFollowingRoad = mapBaseFollowingRoad[compNodeOID];
		for( auto& follow : compFollowingRoad )
		{
			if( baseFollowingRoad.end() != baseFollowingRoad.find(follow.first) )
			{
#ifdef _DEBUG
				// 道なりリンク列が変化していなくても、20°以上変化していたら(一応)ログに残す
				static double angleThreshold = 20.0; // 角度変化の閾値
				const auto& it = baseFollowingRoad.find(follow.first);
				double diff = abs( it->second - follow.second );
				if( 180 < diff ) diff = 360 - diff;
				if( angleThreshold < diff )
					m_ErrLogger.writeErrInfo( compNodeRow, kDiffAngle );
#endif //DEBUG
				continue;
			}

			// 道なりリンク列が変化していたらエラー(101)
			m_ErrLogger.writeErrInfo( compNodeRow, kChangeFollow );
			break;
		}
	}

	return isSucceed;
}
