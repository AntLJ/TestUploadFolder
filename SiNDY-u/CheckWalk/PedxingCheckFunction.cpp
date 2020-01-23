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
#include "PedxingCheckFunction.h"
#include "value.h"
#include "CheckWalk.h"

#include <ArcHelperEx/AheGeometryOp.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;

extern crd_cnv g_cnv;

void PedxingCheckFunction::check(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& pedxing,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const FeaturesPack& scrambleArea,
	const FeaturesPack& heightNode,
	const BGRTree& pedxingRTree,
	const BGRTree& walkLinkRTree,
	const BGRTree& walkNodeRTree,
	const BGRTree& roadLinkRTree,
	const BGRTree& roadNodeRTree,		
	const BGRTree& scrambleAreaRTree,
	const BGRTree& heightNodeRTree,
	properties::check_target checktarget) const
{
	using namespace pedxing;
	using namespace properties;

	if(m_dataType !=  walk_type::type_class::kDownConvert) return;

	m_output.SetLayer( kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, kTableName );

	long modOperatorIndex = pedxing.m_FieldMap.FindField( kOperator );
	long modDateIndex = pedxing.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = pedxing.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = pedxing.m_FieldMap.FindField( kProgModifyDate );
	long  pedxingClassIndex = pedxing.m_FieldMap.FindField( kPedxingClass );

	for ( const auto& pedxingPair : pedxing.m_FeatureMap )
	{
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if( pedxingPair.second.IsDisjoint( ipMeshGeo ) )
			continue;

		auto  pedxingID       = pedxingPair.first;
		auto& pedxingObject   = pedxingPair.second;
		long  pedxingClass    = pedxingObject.GetLongValue( pedxingClassIndex );

		m_output.SetModifyInfo( pedxingID, pedxingObject.GetStringValue( modOperatorIndex ),
			pedxingObject.GetStringValue( modDateIndex ), pedxingObject.GetStringValue( modProgramIndex ), pedxingObject.GetStringValue( modProgramDateIndex ) );

		if( checktarget == check_target::all || checktarget == check_target::other || 
			checktarget == check_target::release || checktarget == check_target::pedxing)
		{
			if(checktarget != check_target::release)
			{
				// ★エラー 横断ポイントがスクランブルエリア内にある
				checkPedxingOnScrambleArea( pedxingObject, scrambleArea, scrambleAreaRTree );			
				// ★エラー 横断ポイントに関係ない歩行者リンクが存在する
				checkPedxingWithWalkLink( pedxingObject, walkLink, walkLinkRTree, pedxingClass );
			}

			// ★エラー 横断ポイントが道路リンク上に無い
			checkPedxingOnRoadLink( pedxingObject, roadLink, roadLinkRTree );
			// ★エラー 横断ポイントが道路リンク交差箇所にある
			checkPedxingCrossRoadLink( pedxingObject, roadLink, roadLinkRTree, roadNode.m_FeatureMap, roadNodeRTree );
			// ★エラー 横断ポイントが歩行者リンク交差箇所にある
			checkPedxingCrossWalkLink( pedxingObject, walkLink, walkLinkRTree, walkNode.m_FeatureMap, walkNodeRTree );
			// ★エラー 横断タイプコードが不正な値
			checkPedxingClass( pedxingClass );

			if(checktarget != check_target::other && checktarget != check_target::pedxing)
			{
				// ★エラー ポイント同士（PEDXING、HEIGHT_NODE）の距離が近い
				checkNearPoint( pedxingObject, pedxing,	heightNode, pedxingRTree, heightNodeRTree );
			}

			if(checktarget != check_target::other && checktarget != check_target::release)
			{
				// ★エラー 意味なしPEDXINGかチェック
				checkCrossRoadLink( pedxingObject, roadLink, walkLink, roadLinkRTree, walkLinkRTree	);
			}
		}
	}	
}

// 横断ポイント形状が不正であるかチェック
void PedxingCheckFunction::checkPedxingGeometry( const ITablePtr& ipTable ) const
{
	// 全件取得
	IFeatureCursorPtr ipCursor = ipTable->_Search( NULL, VARIANT_FALSE );
	if ( !ipCursor ) return;

	IFeaturePtr ipFeature;
	// オブジェクトごとの処理
	while( S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature ){
		IGeometryPtr ipGeometry = ipFeature->GetShape(); 
		
		if( !ipGeometry	)
		{
			// ★エラー 形状が存在しない
			m_output.OutputRegLog( err_level::fatal, err_code::NgPedxingGeometry);
			return;
		}

		esriGeometryType eType = ipGeometry->GetGeometryType();
		if( eType != esriGeometryPoint )
		{
			// ★エラー ポイント型ではない
			m_output.OutputRegLog( err_level::fatal, err_code::NgPedxingGeometry);
			return;
		}
	}
}

// 横断ポイントがスクランブルエリア内にあるかチェック
void PedxingCheckFunction::checkPedxingOnScrambleArea(
	const GeoClass& pedxingObject,
	const FeaturesPack& scrambleArea,
	const BGRTree& scrambleAreaRTree	
	) const
{
	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, scrambleArea, scrambleAreaRTree, crossObjects) <= 0) return;

	for(const auto& crossObject:crossObjects)
	{
		VARIANT_BOOL within(VARIANT_FALSE);
		if (FAILED(((IRelationalOperatorPtr)pedxingObject.m_ipGeometry)->Within(crossObject.m_ipGeometry, &within)) || within == VARIANT_FALSE) continue;

		m_output.OutputRegLog( err_level::error, err_code::NgPedxingOnScrambleArea );
		return;
	}
}

// 横断ポイントが道路リンク上に無いかチェック
void PedxingCheckFunction::checkPedxingOnRoadLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree	
	) const
{
	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, roadLink, roadLinkRTree, crossObjects, m_sindyTolerance_threshold) <= 0)
		m_output.OutputRegLog( err_level::error, err_code::NgPedxingOnRoadLink );
}

// 交差箇所のポイントを取得する。
bool PedxingCheckFunction::getCrossPoint(
	const GeoClass& baseGeo,
	const std::vector<GeoClass>& crossObjects) const
{
	if(crossObjects.empty())
		return false;

	for( const auto& crossObject : crossObjects ){
		for( const auto& crossObjectAfter : crossObjects ){

			// 基準リンクと同じIDの場合スキップ
			if( crossObject.m_ObjectID == crossObjectAfter.m_ObjectID )
				continue;

			if( ! crossObject.IsCross( crossObjectAfter.m_ipGeometry ) )
				continue;

			BOOL isDisjoint = FALSE;
			IPointCollectionPtr ipPointCollect( AheGetCrossOrTouchPoints( crossObject.m_ipGeometry, crossObjectAfter.m_ipGeometry, 0, &isDisjoint) );
			if( isDisjoint || ! ipPointCollect )
				continue;

			long pointCount = ipPointCollect->GetPointCount();
			for( long index = 0; index < pointCount; ++index )
			{
				if(AheGetMeterLength( IPointPtr(baseGeo.m_ipGeometry), IPointPtr(ipPointCollect->GetPoint(index))) < m_sindyTolerance_threshold) return true;
			}
		}
	}

	return false;
}

// 横断ポイントが道路リンク交差箇所にあるかチェック
void PedxingCheckFunction::checkPedxingCrossRoadLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree, 
	const std::map<long, GeoClass>& roadNodeMap,
	const BGRTree& roadNodeRTree) const	
{
	// 横断ポイント上の道路ノードを検索
	if ( findOverlapNode( pedxingObject, roadNodeMap, roadNodeRTree ) ) return;

	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, roadLink, roadLinkRTree, crossObjects, m_sindyTolerance_threshold) < 2) return;
	
	if(getCrossPoint(pedxingObject, crossObjects))
	{
		m_output.OutputRegLog( err_level::error, err_code::NgPedxingCrossRoadLink );
	}
}

// 横断ポイントに重なるノードを検索
bool PedxingCheckFunction::findOverlapNode(
		const GeoClass& pedxingObject,
		const std::map<long, GeoClass>& nodeMap,
		const BGRTree& nodeRTree
		) const
{
	// 道路ノードを探索
	auto findNodeIDs = util::getIntersectsIDs( pedxingObject.GetBox( 0.1 ), nodeRTree );

	// 見つかった道路ノードのループ
	for( const auto& findNodeID : findNodeIDs ){

		auto findNodeItr = nodeMap.find( findNodeID );
		if( findNodeItr == nodeMap.end() )
			continue;

		// 一致するか？
		if( pedxingObject.IsEqual( findNodeItr->second.m_ipGeometry ) ){
			return true;
		}
	}
	return false;
}

// 横断ポイントが歩行者リンク交差箇所にあるかチェック
void PedxingCheckFunction::checkPedxingCrossWalkLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree,
	const std::map<long, GeoClass>& walkNodeMap,
	const BGRTree& walkNodeRTree) const	
{
	// 横断ポイント上の歩行者ノードを検索
	if ( findOverlapNode( pedxingObject, walkNodeMap, walkNodeRTree ) ) return;

	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, walkLink, walkLinkRTree, crossObjects, m_sindyTolerance_threshold) < 2) return;

	if(getCrossPoint(pedxingObject, crossObjects))
	{
		m_output.OutputRegLog( err_level::error, err_code::NgPedxingCrossWalkLink );
	}
}

// 横断ポイントに関係ない歩行者リンクが存在するかチェック
void PedxingCheckFunction::checkPedxingWithWalkLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree,
	long  pedxingClass
	) const
{
	using namespace walk_link;
	using namespace pedxing;
	using namespace uh::str_util;
	
	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, walkLink, walkLinkRTree, crossObjects, m_sindyTolerance_threshold) <= 0) return;

	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField( kWalkClass );	
	for( const auto& crossObject : crossObjects )
	{
		auto walkLinkObj = walkLink.m_FeatureMap.find(crossObject.m_ObjectID);
		if ( walkLinkObj == walkLink.m_FeatureMap.end() ) continue;

		long  walkLinkClass = walkLinkObj->second.GetLongValue( walkLinkClassIndex );
		if( pedxingClass == pedxing_class::kCrossRoad && (walkLinkClass == walk_class::kCrossRoad || walkLinkClass == walk_class::kLogicalConnection) ||
			pedxingClass == pedxing_class::kInferredCrossRoad && (walkLinkClass == walk_class::kInferredCrossRoad || walkLinkClass == walk_class::kLogicalConnection))
			continue;

		m_output.OutputRegLog( err_level::error, err_code::NgPedxingWithWalkLink, ToString( pedxingClass ), ToString( crossObject.m_ObjectID ) );
	}
}

// 横断タイプコードが不正な値かチェック
void PedxingCheckFunction::checkPedxingClass(
	long  pedxingClass
	) const
{
	using namespace pedxing;

	if( pedxingClass != pedxing_class::kCrossRoad &&  pedxingClass != pedxing_class::kInferredCrossRoad )
		m_output.OutputRegLog( err_level::fatal, err_code::NgPedxingClass );
}

// 例外モード用のチェック
void PedxingCheckFunction::checkException(
	const FeaturesPack& pedxing
	)
{
	if(m_dataType !=  walk_type::type_class::kDownConvert) return;

	// ★エラー 横断ポイントの形状が不正
	checkPedxingGeometry( pedxing.m_ipTable);

}

// ポイント同士（PEDXING、HEIGHT_NODE）の距離が近いかチェック
void PedxingCheckFunction::checkNearPoint(
	const GeoClass& pedxingObject,
	const FeaturesPack& pedxing,	
	const FeaturesPack& heightNode,
	const BGRTree& pedxingRTree,
	const BGRTree& heightNodeRTree
	) const
{
	std::vector<GeoClass> crossHeightNodes;
	if(util::findOverlapGeometry(pedxingObject, heightNode, heightNodeRTree, crossHeightNodes, 0.01) > 0) 
	{
		for( const auto& node : crossHeightNodes )
		{
			if( pedxingObject.IsEqual( node.m_ipGeometry ) ) continue;

			CString info;
			info.Format(_T("%s(%d)"), height_node::kTableName, node.m_ObjectID);
			m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
		}
	}

	std::vector<GeoClass> crossPedxings;
	if(util::findOverlapGeometry(pedxingObject, pedxing, pedxingRTree, crossPedxings, 0.01) > 0)
	{
		for( const auto& node : crossPedxings )
		{
			// 一致するか？
			if(pedxingObject.m_ObjectID == node.m_ObjectID || !pedxingObject.IsEqual( node.m_ipGeometry ) ) continue;

			CString info;
			info.Format(_T("%s(%d)"), pedxing::kTableName, node.m_ObjectID);
			m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
		}
	}
}

// 意味なしPEDXINGかチェック
void PedxingCheckFunction::checkCrossRoadLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& roadLink,
	const FeaturesPack& walkLink,
	const BGRTree& roadLinkRTree,
	const BGRTree& walkLinkRTree
	) const
{
	std::vector<GeoClass> crossRoadLinks;
	if(util::findOverlapGeometry(pedxingObject, roadLink, roadLinkRTree, crossRoadLinks) <= 0) return;

	std::vector<GeoClass> crossWalkLinks;
	if(util::findOverlapGeometry(pedxingObject, walkLink, walkLinkRTree, crossWalkLinks) > 0) return;

	long walkableIndex = roadLink.m_FieldMap.FindField( road_link::kWalkable );
	for( const auto& link : crossRoadLinks )
	{
		// WALKABLE_Cが「歩行禁止」、「右側(歩行禁止)、「左側(歩行禁止)で場合、エラー
		switch(roadLink.m_FeatureMap.at( link.m_ObjectID ).GetLongValue( walkableIndex ))
		{
		case road_link::walkable_class::kNotWalk:
		case road_link::walkable_class::kRightSideWalkNonWalkable:
		case road_link::walkable_class::kLeftSideWalkNonWalkable:
			break;
		default:
			return;
		}

		m_output.OutputRegLog( err_level::warning, err_code::CrossRoadLink );
	}

}