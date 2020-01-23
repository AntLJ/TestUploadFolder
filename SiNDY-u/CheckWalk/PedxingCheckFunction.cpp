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
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
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
				// ���G���[ ���f�|�C���g���X�N�����u���G���A���ɂ���
				checkPedxingOnScrambleArea( pedxingObject, scrambleArea, scrambleAreaRTree );			
				// ���G���[ ���f�|�C���g�Ɋ֌W�Ȃ����s�҃����N�����݂���
				checkPedxingWithWalkLink( pedxingObject, walkLink, walkLinkRTree, pedxingClass );
			}

			// ���G���[ ���f�|�C���g�����H�����N��ɖ���
			checkPedxingOnRoadLink( pedxingObject, roadLink, roadLinkRTree );
			// ���G���[ ���f�|�C���g�����H�����N�����ӏ��ɂ���
			checkPedxingCrossRoadLink( pedxingObject, roadLink, roadLinkRTree, roadNode.m_FeatureMap, roadNodeRTree );
			// ���G���[ ���f�|�C���g�����s�҃����N�����ӏ��ɂ���
			checkPedxingCrossWalkLink( pedxingObject, walkLink, walkLinkRTree, walkNode.m_FeatureMap, walkNodeRTree );
			// ���G���[ ���f�^�C�v�R�[�h���s���Ȓl
			checkPedxingClass( pedxingClass );

			if(checktarget != check_target::other && checktarget != check_target::pedxing)
			{
				// ���G���[ �|�C���g���m�iPEDXING�AHEIGHT_NODE�j�̋������߂�
				checkNearPoint( pedxingObject, pedxing,	heightNode, pedxingRTree, heightNodeRTree );
			}

			if(checktarget != check_target::other && checktarget != check_target::release)
			{
				// ���G���[ �Ӗ��Ȃ�PEDXING���`�F�b�N
				checkCrossRoadLink( pedxingObject, roadLink, walkLink, roadLinkRTree, walkLinkRTree	);
			}
		}
	}	
}

// ���f�|�C���g�`�󂪕s���ł��邩�`�F�b�N
void PedxingCheckFunction::checkPedxingGeometry( const ITablePtr& ipTable ) const
{
	// �S���擾
	IFeatureCursorPtr ipCursor = ipTable->_Search( NULL, VARIANT_FALSE );
	if ( !ipCursor ) return;

	IFeaturePtr ipFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while( S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature ){
		IGeometryPtr ipGeometry = ipFeature->GetShape(); 
		
		if( !ipGeometry	)
		{
			// ���G���[ �`�󂪑��݂��Ȃ�
			m_output.OutputRegLog( err_level::fatal, err_code::NgPedxingGeometry);
			return;
		}

		esriGeometryType eType = ipGeometry->GetGeometryType();
		if( eType != esriGeometryPoint )
		{
			// ���G���[ �|�C���g�^�ł͂Ȃ�
			m_output.OutputRegLog( err_level::fatal, err_code::NgPedxingGeometry);
			return;
		}
	}
}

// ���f�|�C���g���X�N�����u���G���A���ɂ��邩�`�F�b�N
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

// ���f�|�C���g�����H�����N��ɖ������`�F�b�N
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

// �����ӏ��̃|�C���g���擾����B
bool PedxingCheckFunction::getCrossPoint(
	const GeoClass& baseGeo,
	const std::vector<GeoClass>& crossObjects) const
{
	if(crossObjects.empty())
		return false;

	for( const auto& crossObject : crossObjects ){
		for( const auto& crossObjectAfter : crossObjects ){

			// ������N�Ɠ���ID�̏ꍇ�X�L�b�v
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

// ���f�|�C���g�����H�����N�����ӏ��ɂ��邩�`�F�b�N
void PedxingCheckFunction::checkPedxingCrossRoadLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree, 
	const std::map<long, GeoClass>& roadNodeMap,
	const BGRTree& roadNodeRTree) const	
{
	// ���f�|�C���g��̓��H�m�[�h������
	if ( findOverlapNode( pedxingObject, roadNodeMap, roadNodeRTree ) ) return;

	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, roadLink, roadLinkRTree, crossObjects, m_sindyTolerance_threshold) < 2) return;
	
	if(getCrossPoint(pedxingObject, crossObjects))
	{
		m_output.OutputRegLog( err_level::error, err_code::NgPedxingCrossRoadLink );
	}
}

// ���f�|�C���g�ɏd�Ȃ�m�[�h������
bool PedxingCheckFunction::findOverlapNode(
		const GeoClass& pedxingObject,
		const std::map<long, GeoClass>& nodeMap,
		const BGRTree& nodeRTree
		) const
{
	// ���H�m�[�h��T��
	auto findNodeIDs = util::getIntersectsIDs( pedxingObject.GetBox( 0.1 ), nodeRTree );

	// �����������H�m�[�h�̃��[�v
	for( const auto& findNodeID : findNodeIDs ){

		auto findNodeItr = nodeMap.find( findNodeID );
		if( findNodeItr == nodeMap.end() )
			continue;

		// ��v���邩�H
		if( pedxingObject.IsEqual( findNodeItr->second.m_ipGeometry ) ){
			return true;
		}
	}
	return false;
}

// ���f�|�C���g�����s�҃����N�����ӏ��ɂ��邩�`�F�b�N
void PedxingCheckFunction::checkPedxingCrossWalkLink(
	const GeoClass& pedxingObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree,
	const std::map<long, GeoClass>& walkNodeMap,
	const BGRTree& walkNodeRTree) const	
{
	// ���f�|�C���g��̕��s�҃m�[�h������
	if ( findOverlapNode( pedxingObject, walkNodeMap, walkNodeRTree ) ) return;

	std::vector<GeoClass> crossObjects;
	if(util::findOverlapGeometry(pedxingObject, walkLink, walkLinkRTree, crossObjects, m_sindyTolerance_threshold) < 2) return;

	if(getCrossPoint(pedxingObject, crossObjects))
	{
		m_output.OutputRegLog( err_level::error, err_code::NgPedxingCrossWalkLink );
	}
}

// ���f�|�C���g�Ɋ֌W�Ȃ����s�҃����N�����݂��邩�`�F�b�N
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

// ���f�^�C�v�R�[�h���s���Ȓl���`�F�b�N
void PedxingCheckFunction::checkPedxingClass(
	long  pedxingClass
	) const
{
	using namespace pedxing;

	if( pedxingClass != pedxing_class::kCrossRoad &&  pedxingClass != pedxing_class::kInferredCrossRoad )
		m_output.OutputRegLog( err_level::fatal, err_code::NgPedxingClass );
}

// ��O���[�h�p�̃`�F�b�N
void PedxingCheckFunction::checkException(
	const FeaturesPack& pedxing
	)
{
	if(m_dataType !=  walk_type::type_class::kDownConvert) return;

	// ���G���[ ���f�|�C���g�̌`�󂪕s��
	checkPedxingGeometry( pedxing.m_ipTable);

}

// �|�C���g���m�iPEDXING�AHEIGHT_NODE�j�̋������߂����`�F�b�N
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
			// ��v���邩�H
			if(pedxingObject.m_ObjectID == node.m_ObjectID || !pedxingObject.IsEqual( node.m_ipGeometry ) ) continue;

			CString info;
			info.Format(_T("%s(%d)"), pedxing::kTableName, node.m_ObjectID);
			m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
		}
	}
}

// �Ӗ��Ȃ�PEDXING���`�F�b�N
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
		// WALKABLE_C���u���s�֎~�v�A�u�E��(���s�֎~)�A�u����(���s�֎~)�ŏꍇ�A�G���[
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