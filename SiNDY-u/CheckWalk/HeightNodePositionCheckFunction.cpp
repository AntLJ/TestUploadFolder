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
#include "HeightNodePositionCheckFunction.h"
#include "value.h"

#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/AheGeometryConvert.h>

using namespace sindy::schema;

void HeightNodePositionCheckFunction::checkPosition(
	const CString& meshcode,
	const IGeometryPtr& meshGeom,
	const FeaturesPack& roadLink,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const FeaturesPack& heightNode,
	const BGRTree& heightNodeRTree
	)
{
	// linkMapの中からメッシュ内に存在&対象属性のものだけ抽出
	std::map<long, const GeoClass*> targetWalkLinkMap;
	std::map<long, const GeoClass*> targetWalkNodeMap;
	std::map<long, const GeoClass*> targetRoadLinkMap;
	{
		auto fromIndex = walkLink.m_FieldMap.FindField( walk_link::kFromNodeID );
		auto toIndex = walkLink.m_FieldMap.FindField( walk_link::kToNodeID );
		for(auto& link : walkLink.m_FeatureMap)
		{
			if( AheIsDisjoint(meshGeom, link.second.m_ipGeometry) )
				continue;

			// HEIGHT_NODEを作成する属性でなければcontinue
			if( !isCreatableAttribute(link.second, walkLink.m_FieldMap, WALK))
				continue;
			targetWalkLinkMap.emplace(link.first, &link.second);

			long fromID = link.second.GetLongValue( fromIndex );
			long toID = link.second.GetLongValue( toIndex );

			auto fromIt = walkNode.m_FeatureMap.find(fromID);
			if ( walkNode.m_FeatureMap.end() != fromIt )
				targetWalkNodeMap.emplace(fromID, &fromIt->second);
			auto toIt = walkNode.m_FeatureMap.find( toID );
			if ( walkNode.m_FeatureMap.end() != toIt )
				targetWalkNodeMap.emplace(toID, &toIt->second);
		}
		if(targetWalkLinkMap.empty()) return;

		for(auto& link : roadLink.m_FeatureMap)
		{
			if( AheIsDisjoint(meshGeom, link.second.m_ipGeometry ) )
				continue;

			// HEIGHT_NODEを作成する属性でなければcontinue
			if( !isCreatableAttribute(link.second, roadLink.m_FieldMap, ROAD))
				continue;
			targetRoadLinkMap.emplace(link.first, &link.second);
		}
	}
	m_ipSP = AheGetSpatialReference( targetWalkLinkMap.begin()->second->m_ipGeometry );


	std::map<long, const GeoClass*> targetHeightNodeMap;
	// 対象メッシュ内のHEIGHT_NODEを取得
	for(auto& node : heightNode.m_FeatureMap)
	{
		if( AheIsDisjoint(meshGeom, node.second.m_ipGeometry ) )
			continue;
		targetHeightNodeMap.emplace(node.first, &node.second);
	}

	IGeometryPtr walkNodeUnion = util::getUnionGeometry( targetWalkNodeMap );
	IGeometryPtr heightNodeUnion = util::getUnionGeometry( targetHeightNodeMap );
	for(auto baseIt = targetWalkLinkMap.begin(); targetWalkLinkMap.end()!=baseIt; ++baseIt)
	{
		// 歩行者リンク同士の交差位置を算出
		for(auto targetIt = std::next(baseIt); targetWalkLinkMap.end()!=targetIt; ++targetIt)
		{
			checkCrossPoint(baseIt->second->m_ipGeometry, targetIt->second->m_ipGeometry,
				walkNodeUnion, heightNodeUnion, heightNodeRTree, meshcode, meshGeom);
		}
		// 歩行者リンク-道路リンクの交差位置を算出
		for(auto targetIt = targetRoadLinkMap.begin(); targetRoadLinkMap.end()!=targetIt; ++targetIt)
		{
			checkCrossPoint(baseIt->second->m_ipGeometry, targetIt->second->m_ipGeometry,
				walkNodeUnion, heightNodeUnion, heightNodeRTree, meshcode, meshGeom);
		}
	}
}

// ノードを作成することが出来る属性か
bool HeightNodePositionCheckFunction::isCreatableAttribute(const GeoClass& geoClass, const sindy::CFieldMap& fieldMap, TYPE type)
{
	using namespace sindy::schema;

	// 道路の場合
	if( type == ROAD )
	{
		return judge_value::IsOKReleaseRoadLink( geoClass, fieldMap );
	}
	// 歩行者の場合
	long walkC = geoClass.GetLongValue( fieldMap.FindField(walk_link::kWalkClass ) );
	return judge_value::IsOKHeightNodeWalkCode( walkC );
	
}

void HeightNodePositionCheckFunction::checkCrossPoint(
	const IGeometryPtr& base, const IGeometryPtr& target,
	const IGeometryPtr& walkNodeUnion, const IGeometryPtr& heightNodeUnion, const BGRTree& heightNodeRTree,
	const CString& meshcode, const IGeometryPtr& meshGeom
	)
{
	// マルチポイント化(引数としてポイント・マルチポイントしかないことが前提。マルチポイントはそのまま)
	auto pointToMultiPoint = [&](IGeometryPtr base)
	{
		esriGeometryType geometryType = esriGeometryType::esriGeometryAny;
		base->get_GeometryType(&geometryType);
		if( esriGeometryPoint==geometryType )
			return ToMultipoint(base);
		return base;
	};

	// base/targetをマルチポイント化して、baseからtargetの形状を引いた形状を取得
	auto getGeometryDiff = [&](const IGeometryPtr& base, const IGeometryPtr& target, bool& hasError)->IGeometryPtr
	{
		if(!base)
		{
			hasError = true;
			return nullptr;
		}
		if(!target) return base;

		// Pointに対してMultiPointでDifferenceを実行すると落ちるので、マルチポイント化
		IGeometryPtr multiBase = pointToMultiPoint(base);
		IGeometryPtr multiTarget = pointToMultiPoint(target);

		ITopologicalOperatorPtr topo(multiBase);
		IGeometryPtr result;
		if( FAILED(topo->Difference( multiTarget, &result)) )
		{
			hasError = true;
			return nullptr;
		}
		if(!result) return nullptr;
		result->putref_SpatialReference( m_ipSP );
		return result;
	};

	// baseとtargetがTouchの場合もチェックしないよう変更
	if( AheIsDisjoint( base, target ) || AheIsTouch( base, target ) ) return;

	auto crossPointList = util::getCrossPoint( base, target );
	if(crossPointList.empty()) return;

	// 交点のマルチポイントとHeihgtNodのマルチポイントの差分を取得
	bool hasError = false;
	
	IGeometryPtr crossPointUnion = util::getUnionGeometry( crossPointList );
	IGeometryPtr result = getGeometryDiff(crossPointUnion, heightNodeUnion, hasError);
	if( hasError )
	{
		CString strErr;
		strErr.Format(_T("%ld"), err_code::NonExistHeightNode);
		m_output.OutputLog( err_level::error, err_code::FailedGeometryCulc, strErr );
		return;
	}
	if( !result ) return;
	// 差分結果と歩行者ノードのマルチポイントの差分を取得
	IGeometryPtr result2 = getGeometryDiff(result, walkNodeUnion, hasError);
	if( hasError )
	{
		CString strErr;
		strErr.Format(_T("%ld"), err_code::NonExistHeightNode);
		m_output.OutputLog( err_level::error, err_code::FailedGeometryCulc, strErr );
		return;
	}

	IPointCollectionPtr pointColl(result2);
	if(!pointColl) return;

	long lCount = pointColl->GetPointCount();
	for(long i=0;i<lCount;++i)
	{
		IPointPtr point;
		pointColl->get_Point(i, &point);
		if( AheIsDisjoint(meshGeom, point) )
			continue;
		if( !point ) continue;

		// bug12072 交点の計算結果とHeightNodeの位置が完全に一致しない場合でも、
		// 微小距離でのエンベロープ内にHeightNodeがあればエラーメッセージを出力しないよう変更
		WKSPoint minPoint = { 0.0, 0.0 };
		WKSPoint maxPoint = { 0.0, 0.0 };
		minPoint.X = point->GetX() - m_coordinateTolerance;
		minPoint.Y = point->GetY() - m_coordinateTolerance;
		maxPoint.X = point->GetX() + m_coordinateTolerance;
		maxPoint.Y = point->GetY() + m_coordinateTolerance;

		auto nearHeightNodeSet = util::getIntersectsIDs( BGBox(BGPoint(minPoint.X, minPoint.Y), BGPoint( maxPoint.X, maxPoint.Y ) ), heightNodeRTree );
		if( nearHeightNodeSet.empty() )
		{
			double lx = 0.0, ly = 0.0;
			point->QueryCoords( &lx, &ly );
			m_output.OutputLog( height_node::kTableName, meshcode, lx, ly, err_level::error, err_code::NonExistHeightNode );
		}
	}
}
