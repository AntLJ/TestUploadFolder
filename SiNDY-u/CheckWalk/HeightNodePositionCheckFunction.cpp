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
	// linkMap�̒����烁�b�V�����ɑ���&�Ώۑ����̂��̂������o
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

			// HEIGHT_NODE���쐬���鑮���łȂ����continue
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

			// HEIGHT_NODE���쐬���鑮���łȂ����continue
			if( !isCreatableAttribute(link.second, roadLink.m_FieldMap, ROAD))
				continue;
			targetRoadLinkMap.emplace(link.first, &link.second);
		}
	}
	m_ipSP = AheGetSpatialReference( targetWalkLinkMap.begin()->second->m_ipGeometry );


	std::map<long, const GeoClass*> targetHeightNodeMap;
	// �Ώۃ��b�V������HEIGHT_NODE���擾
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
		// ���s�҃����N���m�̌����ʒu���Z�o
		for(auto targetIt = std::next(baseIt); targetWalkLinkMap.end()!=targetIt; ++targetIt)
		{
			checkCrossPoint(baseIt->second->m_ipGeometry, targetIt->second->m_ipGeometry,
				walkNodeUnion, heightNodeUnion, heightNodeRTree, meshcode, meshGeom);
		}
		// ���s�҃����N-���H�����N�̌����ʒu���Z�o
		for(auto targetIt = targetRoadLinkMap.begin(); targetRoadLinkMap.end()!=targetIt; ++targetIt)
		{
			checkCrossPoint(baseIt->second->m_ipGeometry, targetIt->second->m_ipGeometry,
				walkNodeUnion, heightNodeUnion, heightNodeRTree, meshcode, meshGeom);
		}
	}
}

// �m�[�h���쐬���邱�Ƃ��o���鑮����
bool HeightNodePositionCheckFunction::isCreatableAttribute(const GeoClass& geoClass, const sindy::CFieldMap& fieldMap, TYPE type)
{
	using namespace sindy::schema;

	// ���H�̏ꍇ
	if( type == ROAD )
	{
		return judge_value::IsOKReleaseRoadLink( geoClass, fieldMap );
	}
	// ���s�҂̏ꍇ
	long walkC = geoClass.GetLongValue( fieldMap.FindField(walk_link::kWalkClass ) );
	return judge_value::IsOKHeightNodeWalkCode( walkC );
	
}

void HeightNodePositionCheckFunction::checkCrossPoint(
	const IGeometryPtr& base, const IGeometryPtr& target,
	const IGeometryPtr& walkNodeUnion, const IGeometryPtr& heightNodeUnion, const BGRTree& heightNodeRTree,
	const CString& meshcode, const IGeometryPtr& meshGeom
	)
{
	// �}���`�|�C���g��(�����Ƃ��ă|�C���g�E�}���`�|�C���g�����Ȃ����Ƃ��O��B�}���`�|�C���g�͂��̂܂�)
	auto pointToMultiPoint = [&](IGeometryPtr base)
	{
		esriGeometryType geometryType = esriGeometryType::esriGeometryAny;
		base->get_GeometryType(&geometryType);
		if( esriGeometryPoint==geometryType )
			return ToMultipoint(base);
		return base;
	};

	// base/target���}���`�|�C���g�����āAbase����target�̌`����������`����擾
	auto getGeometryDiff = [&](const IGeometryPtr& base, const IGeometryPtr& target, bool& hasError)->IGeometryPtr
	{
		if(!base)
		{
			hasError = true;
			return nullptr;
		}
		if(!target) return base;

		// Point�ɑ΂���MultiPoint��Difference�����s����Ɨ�����̂ŁA�}���`�|�C���g��
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

	// base��target��Touch�̏ꍇ���`�F�b�N���Ȃ��悤�ύX
	if( AheIsDisjoint( base, target ) || AheIsTouch( base, target ) ) return;

	auto crossPointList = util::getCrossPoint( base, target );
	if(crossPointList.empty()) return;

	// ��_�̃}���`�|�C���g��HeihgtNod�̃}���`�|�C���g�̍������擾
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
	// �������ʂƕ��s�҃m�[�h�̃}���`�|�C���g�̍������擾
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

		// bug12072 ��_�̌v�Z���ʂ�HeightNode�̈ʒu�����S�Ɉ�v���Ȃ��ꍇ�ł��A
		// ���������ł̃G���x���[�v����HeightNode������΃G���[���b�Z�[�W���o�͂��Ȃ��悤�ύX
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
