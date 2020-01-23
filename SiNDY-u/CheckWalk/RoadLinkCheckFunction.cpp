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
#include "RoadLinkCheckFunction.h"
#include "value.h"
#include "CheckWalk.h"

#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <ArcHelperEx/AheMeshUtil.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;

extern crd_cnv g_cnv;

void RoadLinkCheckFunction::CheckRoadLink(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const NODEIDMAP& roadNodeLinkIDs,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs,
	const BGRTree& walkLinkRTree,
	const BGRTree& walkNodeRTree
	)
{
	using namespace sindy::schema;
	using namespace sindy::schema::ipc_feature;

	m_output.SetLayer( road_link::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, road_link::kTableName );

	FeaturesPack tempRoadLink = roadLink;
	FeaturesPack tempRoadNode = roadNode;
	NODEIDMAP tempRoadNodeLinkIDs = roadNodeLinkIDs;

	FeaturesPack tempWalkLink = walkLink;
	FeaturesPack tempWalkNode = walkNode;
	NODEIDMAP tempWalkNodeLinkIDs = walkNodeLinkIDs;

	BGRTree tempWalkLinkRTree = walkLinkRTree;
	BGRTree tempWalkNodeRTree = walkNodeRTree;

	// 道路リンクのチェック
	long fromNodeIdIndex = roadLink.m_FieldMap.FindField( road_link::kFromNodeID );
	long toNodeIdIndex = roadLink.m_FieldMap.FindField( road_link::kToNodeID );
	long walkableIndex = roadLink.m_FieldMap.FindField( road_link::kWalkable );
	long nodeClassIndex = roadNode.m_FieldMap.FindField( road_node::kNodeClass );

	long modOperatorIndex = roadLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = roadLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = roadLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = roadLink.m_FieldMap.FindField( kProgModifyDate );	

	for ( const auto& link : roadLink.m_FeatureMap )
	{
		long  linkID   = link.first;
		auto& linkObj  = link.second;
		long  fromID = linkObj.GetLongValue( fromNodeIdIndex );
		long  toID = linkObj.GetLongValue( toNodeIdIndex );
		long  walkable = linkObj.GetLongValue( walkableIndex );

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;
		m_output.SetModifyInfo( linkID , linkObj.GetStringValue( modOperatorIndex ),
			linkObj.GetStringValue( modDateIndex ) , linkObj.GetStringValue( modProgramIndex ) ,
			linkObj.GetStringValue( modProgramDateIndex ) );
		// 歩行可能種別関連のフィールドをチェック
		checkWalkableField( linkObj, roadLink.m_FieldMap );

		// 歩行可能種別以外のフィールドをチェック
		checkOtherField( linkObj, roadLink.m_FieldMap );

		// 自身のFromID/ToIDに紐づくリンクを取得（自身も含む）
		auto fromItr = roadNodeLinkIDs.find( fromID );
		auto toItr   = roadNodeLinkIDs.find( toID );
		if( fromItr == roadNodeLinkIDs.end() || toItr == roadNodeLinkIDs.end() )
			continue; // 自分があるはずなので、起こり得ないはず
		auto& connectFromLinkIDs = fromItr->second;
		auto& connectToLinkIDs = toItr->second;

		checkHumanCarSandwichedBetweenNotWalk(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink, tempRoadNode, tempRoadNodeLinkIDs
			, tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs
			, tempWalkNodeRTree, ipMeshGeo);

		checkBeforeAfterLinkWalkable(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink, tempRoadNode, tempRoadNodeLinkIDs
			, tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs
			, tempWalkLinkRTree, tempWalkNodeRTree, ipMeshGeo);

		if(m_dataType != walk_type::type_class::kDownConvert) continue;

		// ダウンコンバート向け場合のみ、「歩道がつながっていない」をチエックする
		checkNotConnectedSideWalk(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink);

		// ★エラー 歩行可能種別コードが不正な値
		checkWalkableClass(walkable);

		auto  fromNodeObject = roadNode.m_FeatureMap.find( fromID );
		auto  toNodeObject= roadNode.m_FeatureMap.find( toID );
		if( fromNodeObject == roadNode.m_FeatureMap.end() || toNodeObject == roadNode.m_FeatureMap.end() )
			continue;
		long  fromNodeClass = fromNodeObject->second.GetLongValue( nodeClassIndex );
		long  toNodeClass = toNodeObject->second.GetLongValue( nodeClassIndex );
		// ★エラー メッシュ境界で歩行可能種別が変わっている		
		checkOverMeshWalkableClass( connectFromLinkIDs, tempRoadLink, fromNodeClass, walkable );
		checkOverMeshWalkableClass( connectToLinkIDs, tempRoadLink, toNodeClass, walkable  );
	}
}

void RoadLinkCheckFunction::checkWalkable(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const NODEIDMAP& roadNodeLinkIDs,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs,
	const BGRTree& walkLinkRTree,
	const BGRTree& walkNodeRTree
	)
{
	using namespace sindy::schema;
	using namespace sindy::schema::ipc_feature;

	m_output.SetLayer( road_link::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, road_link::kTableName );

	FeaturesPack tempRoadLink = roadLink;
	FeaturesPack tempRoadNode = roadNode;
	NODEIDMAP tempRoadNodeLinkIDs = roadNodeLinkIDs;

	FeaturesPack tempWalkLink = walkLink;
	FeaturesPack tempWalkNode = walkNode;
	NODEIDMAP tempWalkNodeLinkIDs = walkNodeLinkIDs;

	BGRTree tempWalkLinkRTree = walkLinkRTree;
	BGRTree tempWalkNodeRTree = walkNodeRTree;

	long fromIDIndex = roadLink.m_FieldMap.FindField( road_link::kFromNodeID );
	long toIDIndex = roadLink.m_FieldMap.FindField( road_link::kToNodeID );
	long walkableIndex = roadLink.m_FieldMap.FindField( road_link::kWalkable );
	long nodeClassIndex = roadNode.m_FieldMap.FindField( road_node::kNodeClass );

	long modOperatorIndex = roadLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = roadLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = roadLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = roadLink.m_FieldMap.FindField( kProgModifyDate );	

	for ( const auto& link : roadLink.m_FeatureMap )
	{
		long  linkID   = link.first;
		auto& linkObj  = link.second;
		long  fromID = linkObj.GetLongValue( fromIDIndex );
		long  toID = linkObj.GetLongValue( toIDIndex );
		long  walkable = linkObj.GetLongValue( walkableIndex );

		m_output.SetModifyInfo( linkID , linkObj.GetStringValue( modOperatorIndex ) ,
			linkObj.GetStringValue( modDateIndex ) , linkObj.GetStringValue( modProgramIndex ) ,
			linkObj.GetStringValue( modProgramDateIndex ) );

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// 歩行可能種別関連のフィールドをチェック
		checkWalkableField( linkObj, roadLink.m_FieldMap );

		// 自身のFromID/ToIDに紐づくリンクを取得（自身も含む）
		auto fromItr = roadNodeLinkIDs.find( fromID );
		auto toItr   = roadNodeLinkIDs.find( toID );
		if( fromItr == roadNodeLinkIDs.end() || toItr == roadNodeLinkIDs.end() )
			continue; // 自分があるはずなので、起こり得ないはず
		auto& connectFromLinkIDs = fromItr->second;
		auto& connectToLinkIDs = toItr->second;

		checkHumanCarSandwichedBetweenNotWalk(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink, tempRoadNode, tempRoadNodeLinkIDs
			, tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs
			, tempWalkNodeRTree, ipMeshGeo);

		checkBeforeAfterLinkWalkable(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink, tempRoadNode, tempRoadNodeLinkIDs
			, tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs
			, tempWalkLinkRTree, tempWalkNodeRTree, ipMeshGeo);

		if(m_dataType != walk_type::type_class::kDownConvert) continue;
		// ダウンコンバート向け場合のみ、「歩道がつながっていない」をチエックする
		checkNotConnectedSideWalk(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink);

		// ★エラー 歩行可能種別コードが不正な値
		checkWalkableClass(walkable);
		
		auto  fromNodeObject = roadNode.m_FeatureMap.find( fromID );
		auto  toNodeObject= roadNode.m_FeatureMap.find( toID );
		if( fromNodeObject == roadNode.m_FeatureMap.end() || toNodeObject == roadNode.m_FeatureMap.end() )
			continue;
		long  fromNodeClass = fromNodeObject->second.GetLongValue( nodeClassIndex );
		long  toNodeClass = toNodeObject->second.GetLongValue( nodeClassIndex );

		// ★エラー メッシュ境界で歩行可能種別が変わっている		
		checkOverMeshWalkableClass( connectFromLinkIDs, tempRoadLink, fromNodeClass, walkable );
		checkOverMeshWalkableClass( connectToLinkIDs, tempRoadLink, toNodeClass, walkable  );
	}
}

void RoadLinkCheckFunction::checkOther(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const NODEIDMAP roadNodeLinkIDs
	)
{
	using namespace sindy::schema;
	using namespace sindy::schema::ipc_feature;

	m_output.SetLayer( road_link::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, road_link::kTableName );

	long walkableIndex = roadLink.m_FieldMap.FindField( road_link::kWalkable );

	long modOperatorIndex = roadLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = roadLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = roadLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = roadLink.m_FieldMap.FindField( kProgModifyDate );
	long fromNodeIdIndex = roadLink.m_FieldMap.FindField( road_link::kFromNodeID );
	long toNodeIdIndex = roadLink.m_FieldMap.FindField( road_link::kToNodeID );

	for ( const auto& link : roadLink.m_FeatureMap )
	{
		long  linkID   = link.first;
		auto& linkObj  = link.second;
		long  walkable = linkObj.GetLongValue( walkableIndex );

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;

		m_output.SetModifyInfo( linkID, linkObj.GetStringValue( modOperatorIndex ),
			linkObj.GetStringValue( modDateIndex ), linkObj.GetStringValue( modProgramIndex ),
			linkObj.GetStringValue( modProgramDateIndex ) );
		// 属性値に関するチェック
		checkOtherField( linkObj, roadLink.m_FieldMap );

		if(m_dataType != walk_type::type_class::kDownConvert) continue;

		// ★エラー 歩行可能種別コードが不正な値
		checkWalkableClass(walkable);

		// ★エラー 歩道がつながっていない
		long  fromID = linkObj.GetLongValue( fromNodeIdIndex );
		long  toID = linkObj.GetLongValue( toNodeIdIndex );

		// 自身のFromID/ToIDに紐づくリンクを取得（自身も含む）
		auto fromItr = roadNodeLinkIDs.find( fromID );
		auto toItr   = roadNodeLinkIDs.find( toID );
		if( fromItr == roadNodeLinkIDs.end() || toItr == roadNodeLinkIDs.end() )
			continue; // 自分があるはずなので、起こり得ないはず

		checkNotConnectedSideWalk(linkID, linkObj
			, fromItr->second, toItr->second
			, FeaturesPack(roadLink));
	}
}

void RoadLinkCheckFunction::checkRelease(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink)
{
	using namespace sindy::schema;
	using namespace sindy::schema::ipc_feature;

	m_output.SetLayer( road_link::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, road_link::kTableName );

	// 道路リンクのチェック
	long modOperatorIndex = roadLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = roadLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = roadLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = roadLink.m_FieldMap.FindField( kProgModifyDate );
	for ( const auto& link : roadLink.m_FeatureMap )
	{
		long  linkID   = link.first;
		auto& linkObj  = link.second;

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;
		m_output.SetModifyInfo( linkID, linkObj.GetStringValue( modOperatorIndex ),
			linkObj.GetStringValue( modDateIndex ), linkObj.GetStringValue( modProgramIndex ),
			linkObj.GetStringValue( modProgramDateIndex ) );

		// 歩行可能種別関連のフィールドをチェック
		checkWalkableField( linkObj, roadLink.m_FieldMap );
	}
}


void RoadLinkCheckFunction::checkWalkableField(
	const GeoClass& linkObj
	, const sindy::CFieldMap& fieldMap)
{
	using namespace sindy::schema;
	long  walkable = linkObj.GetLongValue( fieldMap.FindField( road_link::kWalkable ) );
	long  carOnly =  linkObj.GetLongValue( fieldMap.FindField( road_link::kCarOnly ) );
	// Walkableが定義上OKかどうか
	if( ! judge_value::IsDefineWalkable( walkable ) ) // ★エラー 歩行者リンク種別コードが定義外
		m_output.OutputRegLog( err_level::error, err_code::OutWalkable,
			judge_value::IsFindObj( walkable, m_walkableMap ) );

	// Walkableが使用OKか（2：歩行者専用はNG）
	if( judge_value::IsWalkOnly( walkable ) ){
		// ★エラー 道路リンクなのに歩行可能種別コードが歩行者専用になっている
		m_output.OutputRegLog( err_level::error, err_code::NgWalkable );
	}

	// 自動車専用なのに、歩行禁止になっていないものはないか
	if( carOnly != 0 && walkable != road_link::walkable_class::kNotWalk )
		// ★エラー 自動車専用道なのに歩行可能種別コードが歩行禁止になっていない
		m_output.OutputRegLog( err_level::error, err_code::NgCarOnly,
			judge_value::IsFindObj( walkable, m_walkableMap ) );
}

void RoadLinkCheckFunction::checkOtherField(
	const GeoClass& linkObj
	, const sindy::CFieldMap& fieldMap)
{
	using namespace sindy::schema;
	long pedestrian = linkObj.GetLongValue( fieldMap.FindField( road_link::kPedestrianZone ) );
	long noPassageRF = linkObj.GetLongValue( fieldMap.FindField( road_link::kNoPassageRF ) );
	long carOnly = linkObj.GetLongValue( fieldMap.FindField( road_link::kCarOnly ) );

	if ( !judge_value::IsOKFlag( pedestrian ) ) // ★エラー 歩行者天国フラグが定義外
		m_output.OutputRegLog( err_level::error, err_code::NgPedestrianZone );

	if( pedestrian == 1 && noPassageRF ==0 ) // ★エラー 歩行者天国フラグがONだが、通行禁止規制が整備されていない
		m_output.OutputRegLog( err_level::error, err_code::NgPedestrianAndF );

	if( pedestrian == 1 && carOnly == 1 ) // ★エラー 歩行者天国フラグがONだが道路リンク種別が自動車専用道
		m_output.OutputRegLog( err_level::error, err_code::NgPedZoneCarOnly );
}

void RoadLinkCheckFunction::checkNotConnectedSideWalk(
	long linkId
	, const GeoClass& linkObj
	, const std::set<long>& connectFromLinkIds
	, const std::set<long>& connectToLinkIds
	, FeaturesPack& roadLink)
{
	using namespace sindy::schema;
	// チェック対象のWALKABLE_Cの値
	long linkWalkableCode = linkObj.GetLongValue(roadLink.m_FieldMap.FindField( road_link::kWalkable ) );

	//　From/To側Node番号を取得
	long  fromID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID ) );
	long  toID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID ) );

	// From側の接続リンクを取得
	auto fromLinks = util::GetObjectMap( linkId, connectFromLinkIds, roadLink.m_FeatureMap );

	// To側の接続リンクを取得
	auto toLinks = util::GetObjectMap( linkId, connectToLinkIds, roadLink.m_FeatureMap );

	// From、Toノードに接続するリンク数が、自身も含めないて2本以上場合、チェック対象外。
	if( fromLinks.empty() && toLinks.empty() || fromLinks.size() >1 || toLinks.size() > 1 )
		return;
	
	// Fromノードに接続するリンクがある場合、エラーではない場合、戻る
	bool isFromDirect(true) ;
	long fromWalkableCode(-1L); 
	if( !fromLinks.empty() )
	{
		isFromDirect = ( fromID == fromLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID) ) );
		fromWalkableCode= fromLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );

		// エラーではない場合、戻る
		if( !isConnectSideWalkError(isFromDirect, linkWalkableCode, fromWalkableCode ) )
			return;
	}

	// Toノードに接続するリンクがある場合、エラーではない場合、戻る
	bool isToDirect(true) ; 	
	long toWalkableCode(-1L);
	if( !toLinks.empty() )
	{
		isToDirect = ( toID == toLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID) ) ) ;
		toWalkableCode = toLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );

		// エラーではない場合、戻る
		if( !isConnectSideWalkError(isToDirect, linkWalkableCode, toWalkableCode ) )
			return;
	}

	// 接続するリンクの中で、歩道があるサイド（WALKABLE_Cの値）が同じリンクが存在しない場合エラー。
	m_output.OutputRegLog( err_level::error, err_code::NgConnectedSideWalk );
}

bool RoadLinkCheckFunction::isConnectSideWalkError( bool isDirect, long linkWalkableCode, long connectedWalkableCode  )
{
	// チェック対象のWALKABLE_Cの値が0,1,2,3（両側、歩行禁止など）の場合、接続リンクの中でWALKABLE_Cの値が一致している場合はエラーではない。
	switch(linkWalkableCode)
	{
	case road_link::walkable_class::kNotWalk:
	case road_link::walkable_class::kWalkable:
	case road_link::walkable_class::kWalkOnly:
	case road_link::walkable_class::kBothSideWalk:
		return !( linkWalkableCode == connectedWalkableCode );
	}

	// チェック対象のWALKABLE_Cの値が4,5,6,7（右、左）の場合
	// リンクの向きが逆向きではない
	if( isDirect )
	{
		return !( linkWalkableCode == connectedWalkableCode );
	} else
	// リンクの向きが逆向きで
	{
		// 歩道があるサイド（WALKABLE_Cの値）が左側歩道あり（人車共用）がつ接続リンクの歩道があるサイド（WALKABLE_Cの値）が右側歩道あり（人車共用）場合、エラーではない
		if( linkWalkableCode == road_link::walkable_class::kLeftSideWalkWalkable && connectedWalkableCode == road_link::walkable_class::kRightSideWalkWalkable )
			return false;
			
		// 歩道があるサイド（WALKABLE_Cの値）が右側歩道あり（人車共用）がつ接続リンクの歩道があるサイド（WALKABLE_Cの値）が左側歩道あり（人車共用）場合、エラーではない
		if( linkWalkableCode == road_link::walkable_class::kRightSideWalkWalkable && connectedWalkableCode == road_link::walkable_class::kLeftSideWalkWalkable )
			return false;

		// 歩道があるサイド（WALKABLE_Cの値）が左側歩道あり（歩行禁止）がつ接続リンクの歩道があるサイド（WALKABLE_Cの値）が右側歩道あり（歩行禁止）場合、エラーではない
		if( linkWalkableCode == road_link::walkable_class::kLeftSideWalkNonWalkable && connectedWalkableCode == road_link::walkable_class::kRightSideWalkNonWalkable )
			return false;
			
		// 歩道があるサイド（WALKABLE_Cの値）が右側歩道あり（歩行禁止）がつ接続リンクの歩道があるサイド（WALKABLE_Cの値）が左側歩道あり（歩行禁止）場合、エラーではない
		if( linkWalkableCode == road_link::walkable_class::kRightSideWalkNonWalkable && connectedWalkableCode == road_link::walkable_class::kLeftSideWalkNonWalkable )
			return false;	
	}

	return true;
}

void RoadLinkCheckFunction::checkHumanCarSandwichedBetweenNotWalk(
	long linkId
	, const GeoClass& linkObj
	, const std::set<long>& connectFromLinkIds
	, const std::set<long>& connectToLinkIds
	, FeaturesPack& roadLink
	, FeaturesPack& roadNode
	, NODEIDMAP& roadNodeLinkIDs
	, FeaturesPack& walkLink
	, FeaturesPack& walkNode
	, NODEIDMAP& walkNodeLinkIDs
	, BGRTree& walkNodeRTree
	, const IGeometryPtr& ipMeshGeo)
{
	using namespace sindy::schema;
	long walkableCode = linkObj.GetLongValue(roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
	// 汎用向け場合
	if (  m_dataType == walk_type::type_class::kGeneral && road_link::walkable_class::kWalkable !=  walkableCode)
		return;
	// ダウンコンバート向け場合
	if (  m_dataType == walk_type::type_class::kDownConvert )
	{
		// 歩行可能な道路リンク（WALKABLE_C=1・3～7）のみ場合
		switch(walkableCode)
		{
		case road_link::walkable_class::kWalkable:
		case road_link::walkable_class::kBothSideWalk:
		case road_link::walkable_class::kRightSideWalkWalkable:
		case road_link::walkable_class::kRightSideWalkNonWalkable:
		case road_link::walkable_class::kLeftSideWalkWalkable:
		case road_link::walkable_class::kLeftSideWalkNonWalkable:
			break;
		default:
				return;
		}
	}

	long  fromID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID ) );
	long  toID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID ) );

	// 2次メッシュ境界を跨ぐリンクに対し、util::getAdjacentLink()で端点ノードIDが変更されるので、
	// 後のチェックに影響しないように、局所的な変数tempNodeIDを使う
	auto tempNodeID = fromID;

	// From側の接続リンクを取得
	auto fromLinks = util::GetObjectMap( linkId, connectFromLinkIds, roadLink.m_FeatureMap );
	std::set<long> checkedFromLinkIds = util::getAdjacentLink( tempNodeID, fromLinks, roadLink, roadNode, roadNodeLinkIDs, false );

	std::set<long> targetLinkIDs;
	targetLinkIDs.insert(linkId);
	bool doRebuildTree = false;
	if( checkedFromLinkIds.empty() )
	{
		checkedFromLinkIds = getAdjacentRoadLink( fromID, linkId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs,
			targetLinkIDs );
		doRebuildTree = true;
	}
	else
	{
		fromID = tempNodeID;
	}

	tempNodeID = toID;

	// To側の接続リンクを取得
	auto toLinks = util::GetObjectMap( linkId, connectToLinkIds, roadLink.m_FeatureMap );
	std::set<long> checkedToLinkIds = util::getAdjacentLink( tempNodeID, toLinks, roadLink, roadNode, roadNodeLinkIDs, false );

	if( checkedToLinkIds.empty() )
	{
		checkedToLinkIds = getAdjacentRoadLink( toID, linkId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs,
			targetLinkIDs );
		doRebuildTree = true;
	}
	else
	{
		toID = tempNodeID;
	}

	// fromかto側のリンクの取得に失敗=不正データで別のエラーで引っ掛かるので本エラーは不要
	if(checkedFromLinkIds.empty() || checkedToLinkIds.empty()) return;

	// WALKKABLEが歩行禁止の取得用ラムダ式
	auto checkNotWalkable = [&roadLink](long id)
	{
		return road_link::walkable_class::kNotWalk == roadLink.m_FeatureMap.at( id ).GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
	};

	// 接続している始点・終点側のリンクが歩行禁止になる本数がそれぞれ1本以上の場合、チェック対象となる
	long nFromRoadCount = std::count_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), checkNotWalkable);
	long nToRoadCount = std::count_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), checkNotWalkable);
	if( 0 == nFromRoadCount || 0 == nToRoadCount )
		return;

	// ダウンコンバート向け場合
	std::vector<long> fromRoadLinks( nFromRoadCount ), toRoadLinks( nToRoadCount );
	if( m_dataType ==  walk_type::type_class::kDownConvert )
	{
		std::copy_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), fromRoadLinks.begin(), checkNotWalkable);
		std::copy_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), toRoadLinks.begin(), checkNotWalkable);
	}

	if ( doRebuildTree ) {
		//再読み込みが走った時(getAdjacentRoadLinkを行った時)だけツリー再構築
		util::createRTree( walkNode.m_FeatureMap, walkNodeRTree );
	}
	// from/to側の道路ノードと重なった歩行者ノードを取得する
	if( roadNode.m_FeatureMap.find( fromID ) == roadNode.m_FeatureMap.end() || roadNode.m_FeatureMap.find( toID ) == roadNode.m_FeatureMap.end() ) return;

	auto fromRoadNodeObj = roadNode.m_FeatureMap.at( fromID );
	auto fromWalkNodeIDs = util::getIntersectsIDs( fromRoadNodeObj.GetBox(0), walkNodeRTree );
	auto toRoadNodeObj = roadNode.m_FeatureMap.at( toID );
	auto toWalkNodeIDs = util::getIntersectsIDs( toRoadNodeObj.GetBox(0), walkNodeRTree );

	// エラー、fromかto側に接続する歩行者リンクが存在しない
	if( m_dataType ==  walk_type::type_class::kGeneral && ( fromWalkNodeIDs.empty() || toWalkNodeIDs.empty() ) )
	{
		m_output.OutputRegLog( err_level::error, err_code::HumanCarSandwiched );
		return;
	}

	// 歩行者表示種別コードが人車共用の取得用ラムダ式
	auto checkHumanCarGeneral = [&walkLink](long id)
	{
		return walk_link::walk_class::kWalkableRoad == walkLink.m_FeatureMap.at( id ).GetLongValue( walkLink.m_FieldMap.FindField( walk_link::kWalkClass ) );
	};
	// 歩行者表示種別コードがロジカルコネクションの取得用ラムダ式
	auto checkHumanCarDownConvert = [&walkLink](long id)
	{
		return walk_link::walk_class::kLogicalConnection == walkLink.m_FeatureMap.at( id ).GetLongValue( walkLink.m_FieldMap.FindField( walk_link::kWalkClass ) );
	};

	// from/to側の歩行者リンクを取得する
	long nFromWalkCount = 0;
	for(const long fromWalkNodeId : fromWalkNodeIDs)
	{
		auto fromLinkIDSetItr = walkNodeLinkIDs.find( fromWalkNodeId );
		// 汎用向け場合、
		if( m_dataType == walk_type::type_class::kGeneral )
			nFromWalkCount += std::count_if(fromLinkIDSetItr->second.begin(), fromLinkIDSetItr->second.end(), checkHumanCarGeneral);
		// ダウンコンバート向け場合
		if( m_dataType == walk_type::type_class::kDownConvert )
			nFromWalkCount += std::count_if(fromLinkIDSetItr->second.begin(), fromLinkIDSetItr->second.end(), checkHumanCarDownConvert);
	}

	long nToWalkCount = 0;
	for(const long toWalkNodeId : toWalkNodeIDs)
	{
		auto toLinkIDSetItr = walkNodeLinkIDs.find( toWalkNodeId );
		// 汎用向け場合、
		if( m_dataType == walk_type::type_class::kGeneral )
			nToWalkCount += std::count_if(toLinkIDSetItr->second.begin(), toLinkIDSetItr->second.end(), checkHumanCarGeneral);
		// ダウンコンバート向け場合
		if( m_dataType == walk_type::type_class::kDownConvert )
			nToWalkCount += std::count_if(toLinkIDSetItr->second.begin(), toLinkIDSetItr->second.end(), checkHumanCarDownConvert);
	}

	// 汎用向け場合、歩行者リンクと道路リンクの本数が一致しない場合、エラー
	if( m_dataType == walk_type::type_class::kGeneral  && (nFromWalkCount + nToWalkCount) != (nFromRoadCount + nToRoadCount) )
	{
		// ★エラー 歩行禁止の中で人車共用が浮いている
		m_output.OutputRegLog( err_level::error, err_code::HumanCarSandwiched );
	}


	// 汎用向け場合、戻る
	if( m_dataType == walk_type::type_class::kGeneral )
		return;

	// ダウンコンバート向け場合	
	// from側の歩行者リンクを取得する
	std::vector<long> fromWalkLink(nFromWalkCount), toWalkLink(nToWalkCount);
	for(const long fromWalkNodeId : fromWalkNodeIDs)
	{
		auto fromLinkIDSetItr = walkNodeLinkIDs.find( fromWalkNodeId );
		std::copy_if(fromLinkIDSetItr->second.begin(), fromLinkIDSetItr->second.end(), fromWalkLink.begin(), checkHumanCarDownConvert);
	}
	// to側の歩行者リンクを取得する
	for(const long toWalkNodeId : toWalkNodeIDs)
	{
		auto toLinkIDSetItr = walkNodeLinkIDs.find( toWalkNodeId );
		std::copy_if(toLinkIDSetItr->second.begin(), toLinkIDSetItr->second.end(), toWalkLink.begin(), checkHumanCarDownConvert);
	}
	// ロジカルコネクションの歩行者リンクが不在の場合、出力
	if( ( fromWalkLink.empty() && toWalkLink.empty() ) || ( checkedFromLinkIds.size() > fromWalkLink.size()  && checkedToLinkIds.size() > toWalkLink.size() ))
	{
		// ★エラー 歩行禁止の中で人車共用が浮いている
		m_output.OutputRegLog( err_level::notice, err_code::HumanCarSandwiched );
		return;
	}

	// from歩行者接続リンクは道路リンクと完全一致のリンク件数
	long fromLinkCount = util::getDifferenceRoadLinkAndWalkLinkCount(checkedFromLinkIds, fromWalkLink, roadLink, walkLink );

	// to歩行者接続リンクは道路リンクと完全一致のリンク件数
	long toLinkCount = util::getDifferenceRoadLinkAndWalkLinkCount(checkedToLinkIds, toWalkLink, roadLink, walkLink );
	// 歩行不可能な道路リンク上にロジカルコネクション歩行者接続リンクが完全一致で整備されているリンク除外、from/to歩行禁止道路接続リンク存在する場合、出力
	if( toLinkCount > 0 && fromLinkCount > 0 )
	{
		// ★エラー 歩行禁止の中で人車共用が浮いている
		m_output.OutputRegLog( err_level::notice, err_code::HumanCarSandwiched );
		return;
	}
}

void RoadLinkCheckFunction::checkBeforeAfterLinkWalkable(
	long linkId
	, const GeoClass& linkObj
	, const std::set<long>& connectFromLinkIds
	, const std::set<long>& connectToLinkIds
	, FeaturesPack& roadLink
	, FeaturesPack& roadNode
	, NODEIDMAP& roadNodeLinkIDs
	, FeaturesPack& walkLink
	, FeaturesPack& walkNode
	, NODEIDMAP& walkNodeLinkIDs
	, BGRTree& walkLinkRTree
	, BGRTree& walkNodeRTree
	, const IGeometryPtr& ipMeshGeo)
{
	using namespace sindy::schema;

	// 歩行禁止のみ、チェック対象
	long  walkable = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
	if ( road_link::walkable_class::kNotWalk != walkable )
		return;

	long  fromID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID ) );
	long  toID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID ) );

	// 2次メッシュ境界を跨ぐリンクに対し、util::getAdjacentLink()で端点ノードIDが変更されるので、
	// 後のチェックに影響しないように、局所的な変数tempNodeIDを使う
	auto tempNodeID = fromID;

	// From側の接続リンクを取得
	auto fromLinks = util::GetObjectMap( linkId, connectFromLinkIds, roadLink.m_FeatureMap );
	std::set<long> checkedFromLinkIds = util::getAdjacentLink( tempNodeID, fromLinks, roadLink, roadNode, roadNodeLinkIDs, false );

	std::set<long> targetLinkIDs;
	targetLinkIDs.insert(linkId);

	bool bSearchFlg = false;
	if( checkedFromLinkIds.empty() )
	{
		checkedFromLinkIds = getAdjacentRoadLink( fromID, linkId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs,
			targetLinkIDs);
		bSearchFlg = true;
	}
	else
	{
		fromID = tempNodeID;
	}

	tempNodeID = toID;

	// To側の接続リンクを取得
	auto toLinks = util::GetObjectMap( linkId, connectToLinkIds, roadLink.m_FeatureMap );
	std::set<long> checkedToLinkIds = util::getAdjacentLink( tempNodeID, toLinks, roadLink, roadNode, roadNodeLinkIDs, false );

	if( checkedToLinkIds.empty() )
	{
		checkedToLinkIds = getAdjacentRoadLink( toID, linkId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs,
			targetLinkIDs);
		bSearchFlg = true;
	}
	else
	{
		toID = tempNodeID;
	}

	// fromかto側のリンクの取得に失敗=不正データで別のエラーで引っ掛かるので本エラーは不要
	if(checkedFromLinkIds.empty() || checkedToLinkIds.empty()) return;

	long nFromRoadCount(0L), nToRoadCount(0L);
	// WALKKABLEが人車共用の取得用ラムダ式
	if( m_dataType == walk_type::type_class::kGeneral )
	{
		auto checkHumanCar = [&roadLink](long id)
		{
			long walkatbleCode = roadLink.m_FeatureMap.at( id ).GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
			return road_link::walkable_class::kWalkable == walkatbleCode;
		};	
		
		// 接続している始点・終点側のリンクが人車共用になる本数がそれぞれ1本以上の場合、チェック対象となる
		nFromRoadCount = std::count_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), checkHumanCar);
		nToRoadCount = std::count_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), checkHumanCar);
	}
	if( m_dataType == walk_type::type_class::kDownConvert )
	{
		auto checkHumanCar = [&roadLink](long id)
		{
			long walkatbleCode = roadLink.m_FeatureMap.at( id ).GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
			return ( ( road_link::walkable_class::kWalkable == walkatbleCode ) || ( walkatbleCode >= road_link::walkable_class::kBothSideWalk && walkatbleCode <= road_link::walkable_class::kLeftSideWalkNonWalkable ) );
		};

		// 接続している始点・終点側のリンクが人車共用になる本数がそれぞれ1本以上の場合、チェック対象となる
		nFromRoadCount = std::count_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), checkHumanCar);
		nToRoadCount = std::count_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), checkHumanCar);
	}

	if( 0 == nFromRoadCount || 0 == nToRoadCount )
		return;
	if ( bSearchFlg )
	{
		//再読み込みが走った時(getAdjacentRoadLinkを行った時)だけツリー再構築
		util::createRTree( walkNode.m_FeatureMap, walkNodeRTree );
	}
	// from/to側の道路ノードと重なった歩行者ノードを取得する
	if( roadNode.m_FeatureMap.find( fromID ) == roadNode.m_FeatureMap.end() || roadNode.m_FeatureMap.find( toID ) == roadNode.m_FeatureMap.end() )	return;
	auto fromRoadNodeObj = roadNode.m_FeatureMap.at( fromID );
	auto fromWalkNodeIDs = util::getIntersectsIDs( fromRoadNodeObj.GetBox(0), walkNodeRTree );
	auto toRoadNodeObj = roadNode.m_FeatureMap.at( toID );
	auto toWalkNodeIDs = util::getIntersectsIDs( toRoadNodeObj.GetBox(0), walkNodeRTree );

	// エラー、fromかto側に接続する歩行者リンクが存在しない
	if( m_dataType ==  walk_type::type_class::kGeneral && ( fromWalkNodeIDs.empty() || toWalkNodeIDs.empty() ) )
	{
		m_output.OutputRegLog( err_level::error, err_code::NotWalkSandwiched,
			judge_value::IsFindObj( walkable, m_walkableMap ) );
		return;
	}

	// 対象リンクと重なった歩行者リンクを取得する
	set<long> overWalkLinkIDs;
	IGeometryPtr unionTargetLink = util::getUnionGeometry( roadLink.m_FeatureMap, targetLinkIDs );
	if(bSearchFlg)
	{
		// 対象リンクが都市地図メッシュを貫いた場合、その都市地図メッシュがチェック範囲に格納されないため、
		// 対象リンクの形状でDBを検索し、歩行者リンクを取得する
		FeaturesPack tempOverWalkLinkMap( walkLink, false ); //m_FeatureMapはコピーされない
		if ( !util::GetFeaturesInGeometry( unionTargetLink, field_lists::walkLinkFieldList, _T( "" ), tempOverWalkLinkMap ) )
			return;

		for ( const auto& overLink : tempOverWalkLinkMap.m_FeatureMap )
		{
			overWalkLinkIDs.insert( overLink.first );
			walkLink.m_FeatureMap.insert( overLink );
		}
	}
	else
	{
		overWalkLinkIDs = util::getIntersectsIDs( linkObj.GetBox(0), walkLinkRTree );
	}

	// 重なった歩行者リンクの表示種別コードが「人車共用リンク」の場合のみ、整備されたとみなす
	std::set<long> overHumanCar;
	long walkClassindex = walkLink.m_FieldMap.FindField( walk_link::kWalkClass );
	for(auto overWalkLinkId : overWalkLinkIDs)
	{
		long walkClass = walkLink.m_FeatureMap.at( overWalkLinkId ).GetLongValue( walkClassindex );
		// 汎用向け場合
		if ( walk_type::type_class::kGeneral == m_dataType && walk_link::walk_class::kWalkableRoad == walkClass )
			overHumanCar.insert(overWalkLinkId);
		// ダウンコンバート向け場合、歩行不可能な種別の道路リンク上にロジカルコネクションが完全一致で整備されている場合は歩行可能の道路リンクとみなす
		if ( walk_type::type_class::kDownConvert == m_dataType && walk_link::walk_class::kLogicalConnection == walkClass )
			overHumanCar.insert(overWalkLinkId);
	}

	// 人車共用の歩行者リンクをUnionする
	IGeometryPtr unionWalkLink = util::getUnionGeometry( walkLink.m_FeatureMap, overHumanCar );

	// 対象リンクが歩行者リンクに包含されていない場合、エラー
	if( !AheIsWithin(unionTargetLink, unionWalkLink) )
	{
		// ★エラー 人車共用の中で歩行禁止が浮いている
		m_output.OutputRegLog( walk_type::type_class::kGeneral == m_dataType ?err_level::error : err_level::notice, err_code::NotWalkSandwiched,
			judge_value::IsFindObj( walkable, m_walkableMap ) );
	}
}

// メッシュ外の接続歩行者リンクを取得する
std::set<long> RoadLinkCheckFunction::getAdjacentRoadLink(
	long& nodeId,
	const long linkId,
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& roadLink,
	FeaturesPack& roadNode,
	NODEIDMAP& roadNodeLinkIDs,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeLinkIDs,
	std::set<long>& targetLinkIDs
	)
{
	std::set<long> result;

	// 探索の起点となるノードがキャッシュされたMap内に存在しない、
	// または、指定ノードに接続するリンクが1つのみの場合、
	// そのノードが所在するメッシュ内のリンク・ノード情報を既存コンテナに追加する
	long nodeClassIndex = roadNode.m_FieldMap.FindField( road_node::kNodeClass );
	auto baseNodeIt = roadNode.m_FeatureMap.find( nodeId );
	if ( (roadNode.m_FeatureMap.end() == baseNodeIt) ||
		util::isOnlyTargetRoadLink( baseNodeIt, linkId, roadNodeLinkIDs, nodeClassIndex ) )
	{
		addMeshInfo(nodeId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs );

		baseNodeIt = roadNode.m_FeatureMap.find( nodeId );
	}
	
	// 再検索してもroadNodeMapに存在しない場合は、空で返す
	if ( roadNode.m_FeatureMap.end() == baseNodeIt )
		return result;

	// ノード種別が端点ノードの場合、探索終了
	if ( road_node::node_class::kTerminal == roadNode.m_FeatureMap.at( nodeId ).GetLongValue( nodeClassIndex ) )
		return result;
	
	std::set<long> linkIDSet = roadNodeLinkIDs.find( nodeId )->second;
	auto adjacentLink = util::GetObjectMap( linkId, linkIDSet, roadLink.m_FeatureMap );

	// 境界ノードではない場合、探索終了
	if ( !util::isMeshEdgeNode( nodeId, roadNode, false ) )
	{
		for( auto& linkInfo : adjacentLink )
			result.insert(linkInfo.first);
		return result;
	}

	// 接続リンクが複数（2差路ではない）場合、探索終了
	if( adjacentLink.size() > 1 )
	{
		for( auto& linkInfo : adjacentLink )
			result.insert(linkInfo.first);
		return result;
	}

	// ここまできた場合、指定ノードに接続するリンクが0件となるが、
	// 0件だと以降の処理が行えないので、returnしてしまう
	if( adjacentLink.empty() )
		return result;

	// 接続リンクの非接続側の端点ノードを取得する
	long adjLinkId = adjacentLink.begin()->first;
	long otherNodeId = util::getOtherEdgeNode(adjLinkId, roadLink, nodeId);

	// 探索で経由したリンクを対象リンクとして保存する
	targetLinkIDs.insert(linkId);

	// 非接続側のノードがチェック範囲外の場合、その所在するメッシュの情報を既存コンテナーに追加する
	auto otherNodeIt = roadNode.m_FeatureMap.find( otherNodeId );
	if ( roadNode.m_FeatureMap.end() == otherNodeIt ||
		util::isOnlyTargetRoadLink( otherNodeIt, adjLinkId, roadNodeLinkIDs, nodeClassIndex ) )
	{
		addMeshInfo( otherNodeId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs );

		otherNodeIt = roadNode.m_FeatureMap.find( otherNodeId );
	}

	// 再検索してもroadNodeMapに存在しない場合は、returnしてしまう
	if ( roadNode.m_FeatureMap.end() == otherNodeIt )
		return result;

	// 2次メッシュ境界を跨ぐリンクに対し、util::getAdjacentLink()で端点ノードIDが変更されるので、
	// 後のチェックに影響しないように、局所的な変数tempNodeIDを使う
	auto tempNodeID = otherNodeId;
	std::set<long> secondLinkIDSet = roadNodeLinkIDs.find( otherNodeId )->second;
	auto secondAdjLinks = util::GetObjectMap( adjLinkId, secondLinkIDSet, roadLink.m_FeatureMap );
	result = util::getAdjacentLink( tempNodeID, secondAdjLinks, roadLink, roadNode, roadNodeLinkIDs, false );

	if( result.empty() )
	{
		// nodeIdが探索の終点
		nodeId = otherNodeId;

		// 隣接メッシュを基準メッシュとして再帰的に探索
		std::set<long> meshOverId = getAdjacentRoadLink(
			nodeId, adjLinkId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs,
			targetLinkIDs);

		for(long linkId : meshOverId)
			result.insert(linkId);
	}
	else
	{
		// 探索終点のリンクを補足（tempNodeIDが探索の終点、otherNodeIdがtargetLinkIDsに格納されたリンク群の終点）
		if( otherNodeId != tempNodeID )
		{
			long tempID = secondAdjLinks.begin()->first;
			targetLinkIDs.insert(tempID);
		}

		nodeId = tempNodeID;
	}

	return result;
}

// 対象ノードの所在するメッシュの情報を追加する
void RoadLinkCheckFunction::addMeshInfo(
	const long targetNodeId,
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& roadLink,
	FeaturesPack& roadNode,
	NODEIDMAP& roadNodeLinkIDs,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeLinkIDs
	)
{
	_IRowPtr ipRow;
	roadNode.m_ipTable->GetRow( targetNodeId, &ipRow );
	// 指定したOIDを持つノードが存在しない場合は、以降の処理は行わない
	if ( !ipRow )
		return;
	auto targetNodeObj = GeoClass( ipRow );
	// 端点の座標を取得する
	DOUBLE dx = 0.0f, dy = 0.0f;
	INT nMeshCode = 0, nMeshX = 0, nMeshY = 0;

	IPointPtr ipAdjPoint(targetNodeObj.m_ipGeometry);
	ipAdjPoint->QueryCoords( &dx, &dy );
	nMeshCode = AheLL2MESH( dx, dy, 64, nMeshX, nMeshY, 1 );

	// 端点が都市地図メッシュの境界か
	bool isCityMeshEdge = false;
	if( 0 == nMeshX ||  0 == nMeshY )
		isCityMeshEdge = true;

	// 端点の所在する都市地図メッシュを取得する
	ISpatialReferencePtr ipSpRef;
	ipMeshGeo->get_SpatialReference( &ipSpRef );

	IGeometryPtr ipAdjMeshGeo, ipAdjMeshGeo2;
	ipAdjMeshGeo = AheMESH2POLY( nMeshCode, ipSpRef );

	// 端点ノードが都市地図メッシュの境界上の場合、隣接のメッシュも取得する必要がある
	// 上と右の境界ノードは、AheLL2MESHで別のメッシュコードに変換されるので、ここでは左と下だけ処理する
	if( isCityMeshEdge )
	{
		long adjMeshCode = 0;

		// XとYが両方とも0の場合、左、下、左下の３メッシュを取得
		if( 0 == nMeshX && 0 == nMeshY )
		{
			adjMeshCode = AheGETMESHCODE( nMeshCode, -1, -1 );
			ipAdjMeshGeo2 = AheMESH2POLY( adjMeshCode, ipSpRef );

			adjMeshCode = AheDECMESH_X( nMeshCode );
			IGeometryPtr ipLeftMeshGeo = AheMESH2POLY( adjMeshCode, ipSpRef );
			AheUnion( ipAdjMeshGeo2, ipLeftMeshGeo, ipSpRef );

			adjMeshCode = AheDECMESH_Y( nMeshCode );
			IGeometryPtr ipDownMeshGeo = AheMESH2POLY( adjMeshCode, ipSpRef );
			AheUnion( ipAdjMeshGeo2, ipDownMeshGeo, ipSpRef );
		}
		else
		{
			if( 0 == nMeshX )
				adjMeshCode = AheDECMESH_X( nMeshCode );
			if( 0 == nMeshY )
				adjMeshCode = AheDECMESH_Y( nMeshCode );
			ipAdjMeshGeo2 = AheMESH2POLY( nMeshCode, ipSpRef );
		}
	}

	AheUnion( ipAdjMeshGeo, ipAdjMeshGeo2, ipSpRef );

	// 道路リンク取得
	if ( !util::GetLinkFeaturesInGeometry( ipAdjMeshGeo, field_lists::roadLinkFieldList, _T( "" ), roadLink, roadNodeLinkIDs ) )
		return;

	// 道路ノード取得
	if ( !util::GetFeaturesInGeometry( ipAdjMeshGeo, field_lists::roadNodeFieldList, _T( "" ), roadNode ) )
		return;

	// 歩行者リンク取得
	if ( !util::GetLinkFeaturesInGeometry( ipAdjMeshGeo, field_lists::walkLinkFieldList, _T( "" ),  walkLink, walkNodeLinkIDs ) )
		return;

	// 歩行者ノード取得
	if ( !util::GetFeaturesInGeometry( ipAdjMeshGeo, field_lists::walkNodeFieldList, _T( "" ), walkNode ) )
		return;
}

// 歩行可能種別コードが不正な値かチェック
void RoadLinkCheckFunction::checkWalkableClass(
	long  walkableClass
	) const
{
	using namespace road_link;

	if( walkableClass != walkable_class::kNotWalk && 
		walkableClass != walkable_class::kWalkable &&
		walkableClass != walkable_class::kBothSideWalk &&
		walkableClass != walkable_class::kRightSideWalkWalkable &&
		walkableClass != walkable_class::kRightSideWalkNonWalkable &&
		walkableClass != walkable_class::kLeftSideWalkWalkable &&
		walkableClass != walkable_class::kLeftSideWalkNonWalkable
		)
		m_output.OutputRegLog( err_level::fatal, err_code::NgWalkableClass );
}

// メッシュ境界で歩行可能種別が変わっているかチェック
void RoadLinkCheckFunction::checkOverMeshWalkableClass(
	const std::set<long>& connectLinkIds,
	const FeaturesPack& roadLink,
	long nodeClass,
	long walkableClass
	) const
{
	using namespace road_link;
	using namespace road_node;

	if( nodeClass != node_class::kEdge && nodeClass != node_class::kPecEdge) return;

	long walkableIndex = roadLink.m_FieldMap.FindField( kWalkable );
	for(const auto& linkID:connectLinkIds)
	{
		if( walkableClass == roadLink.m_FeatureMap.at( linkID ).GetLongValue( walkableIndex )) continue;

		m_output.OutputRegLog( err_level::error, err_code::NgOverMeshWalkableClass );
		return;
	}	 
}
