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

	// ���H�����N�̃`�F�b�N
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

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;
		m_output.SetModifyInfo( linkID , linkObj.GetStringValue( modOperatorIndex ),
			linkObj.GetStringValue( modDateIndex ) , linkObj.GetStringValue( modProgramIndex ) ,
			linkObj.GetStringValue( modProgramDateIndex ) );
		// ���s�\��ʊ֘A�̃t�B�[���h���`�F�b�N
		checkWalkableField( linkObj, roadLink.m_FieldMap );

		// ���s�\��ʈȊO�̃t�B�[���h���`�F�b�N
		checkOtherField( linkObj, roadLink.m_FieldMap );

		// ���g��FromID/ToID�ɕR�Â������N���擾�i���g���܂ށj
		auto fromItr = roadNodeLinkIDs.find( fromID );
		auto toItr   = roadNodeLinkIDs.find( toID );
		if( fromItr == roadNodeLinkIDs.end() || toItr == roadNodeLinkIDs.end() )
			continue; // ����������͂��Ȃ̂ŁA�N���蓾�Ȃ��͂�
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

		// �_�E���R���o�[�g�����ꍇ�̂݁A�u�������Ȃ����Ă��Ȃ��v���`�G�b�N����
		checkNotConnectedSideWalk(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink);

		// ���G���[ ���s�\��ʃR�[�h���s���Ȓl
		checkWalkableClass(walkable);

		auto  fromNodeObject = roadNode.m_FeatureMap.find( fromID );
		auto  toNodeObject= roadNode.m_FeatureMap.find( toID );
		if( fromNodeObject == roadNode.m_FeatureMap.end() || toNodeObject == roadNode.m_FeatureMap.end() )
			continue;
		long  fromNodeClass = fromNodeObject->second.GetLongValue( nodeClassIndex );
		long  toNodeClass = toNodeObject->second.GetLongValue( nodeClassIndex );
		// ���G���[ ���b�V�����E�ŕ��s�\��ʂ��ς���Ă���		
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

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// ���s�\��ʊ֘A�̃t�B�[���h���`�F�b�N
		checkWalkableField( linkObj, roadLink.m_FieldMap );

		// ���g��FromID/ToID�ɕR�Â������N���擾�i���g���܂ށj
		auto fromItr = roadNodeLinkIDs.find( fromID );
		auto toItr   = roadNodeLinkIDs.find( toID );
		if( fromItr == roadNodeLinkIDs.end() || toItr == roadNodeLinkIDs.end() )
			continue; // ����������͂��Ȃ̂ŁA�N���蓾�Ȃ��͂�
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
		// �_�E���R���o�[�g�����ꍇ�̂݁A�u�������Ȃ����Ă��Ȃ��v���`�G�b�N����
		checkNotConnectedSideWalk(linkID, linkObj
			, connectFromLinkIDs, connectToLinkIDs
			, tempRoadLink);

		// ���G���[ ���s�\��ʃR�[�h���s���Ȓl
		checkWalkableClass(walkable);
		
		auto  fromNodeObject = roadNode.m_FeatureMap.find( fromID );
		auto  toNodeObject= roadNode.m_FeatureMap.find( toID );
		if( fromNodeObject == roadNode.m_FeatureMap.end() || toNodeObject == roadNode.m_FeatureMap.end() )
			continue;
		long  fromNodeClass = fromNodeObject->second.GetLongValue( nodeClassIndex );
		long  toNodeClass = toNodeObject->second.GetLongValue( nodeClassIndex );

		// ���G���[ ���b�V�����E�ŕ��s�\��ʂ��ς���Ă���		
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

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;

		m_output.SetModifyInfo( linkID, linkObj.GetStringValue( modOperatorIndex ),
			linkObj.GetStringValue( modDateIndex ), linkObj.GetStringValue( modProgramIndex ),
			linkObj.GetStringValue( modProgramDateIndex ) );
		// �����l�Ɋւ���`�F�b�N
		checkOtherField( linkObj, roadLink.m_FieldMap );

		if(m_dataType != walk_type::type_class::kDownConvert) continue;

		// ���G���[ ���s�\��ʃR�[�h���s���Ȓl
		checkWalkableClass(walkable);

		// ���G���[ �������Ȃ����Ă��Ȃ�
		long  fromID = linkObj.GetLongValue( fromNodeIdIndex );
		long  toID = linkObj.GetLongValue( toNodeIdIndex );

		// ���g��FromID/ToID�ɕR�Â������N���擾�i���g���܂ށj
		auto fromItr = roadNodeLinkIDs.find( fromID );
		auto toItr   = roadNodeLinkIDs.find( toID );
		if( fromItr == roadNodeLinkIDs.end() || toItr == roadNodeLinkIDs.end() )
			continue; // ����������͂��Ȃ̂ŁA�N���蓾�Ȃ��͂�

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

	// ���H�����N�̃`�F�b�N
	long modOperatorIndex = roadLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = roadLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = roadLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = roadLink.m_FieldMap.FindField( kProgModifyDate );
	for ( const auto& link : roadLink.m_FeatureMap )
	{
		long  linkID   = link.first;
		auto& linkObj  = link.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( linkObj.IsDisjoint( ipMeshGeo ) )
			continue;
		m_output.SetModifyInfo( linkID, linkObj.GetStringValue( modOperatorIndex ),
			linkObj.GetStringValue( modDateIndex ), linkObj.GetStringValue( modProgramIndex ),
			linkObj.GetStringValue( modProgramDateIndex ) );

		// ���s�\��ʊ֘A�̃t�B�[���h���`�F�b�N
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
	// Walkable����`��OK���ǂ���
	if( ! judge_value::IsDefineWalkable( walkable ) ) // ���G���[ ���s�҃����N��ʃR�[�h����`�O
		m_output.OutputRegLog( err_level::error, err_code::OutWalkable,
			judge_value::IsFindObj( walkable, m_walkableMap ) );

	// Walkable���g�pOK���i2�F���s�Ґ�p��NG�j
	if( judge_value::IsWalkOnly( walkable ) ){
		// ���G���[ ���H�����N�Ȃ̂ɕ��s�\��ʃR�[�h�����s�Ґ�p�ɂȂ��Ă���
		m_output.OutputRegLog( err_level::error, err_code::NgWalkable );
	}

	// �����Ԑ�p�Ȃ̂ɁA���s�֎~�ɂȂ��Ă��Ȃ����̂͂Ȃ���
	if( carOnly != 0 && walkable != road_link::walkable_class::kNotWalk )
		// ���G���[ �����Ԑ�p���Ȃ̂ɕ��s�\��ʃR�[�h�����s�֎~�ɂȂ��Ă��Ȃ�
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

	if ( !judge_value::IsOKFlag( pedestrian ) ) // ���G���[ ���s�ғV���t���O����`�O
		m_output.OutputRegLog( err_level::error, err_code::NgPedestrianZone );

	if( pedestrian == 1 && noPassageRF ==0 ) // ���G���[ ���s�ғV���t���O��ON�����A�ʍs�֎~�K������������Ă��Ȃ�
		m_output.OutputRegLog( err_level::error, err_code::NgPedestrianAndF );

	if( pedestrian == 1 && carOnly == 1 ) // ���G���[ ���s�ғV���t���O��ON�������H�����N��ʂ������Ԑ�p��
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
	// �`�F�b�N�Ώۂ�WALKABLE_C�̒l
	long linkWalkableCode = linkObj.GetLongValue(roadLink.m_FieldMap.FindField( road_link::kWalkable ) );

	//�@From/To��Node�ԍ����擾
	long  fromID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID ) );
	long  toID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID ) );

	// From���̐ڑ������N���擾
	auto fromLinks = util::GetObjectMap( linkId, connectFromLinkIds, roadLink.m_FeatureMap );

	// To���̐ڑ������N���擾
	auto toLinks = util::GetObjectMap( linkId, connectToLinkIds, roadLink.m_FeatureMap );

	// From�ATo�m�[�h�ɐڑ����郊���N�����A���g���܂߂Ȃ���2�{�ȏ�ꍇ�A�`�F�b�N�ΏۊO�B
	if( fromLinks.empty() && toLinks.empty() || fromLinks.size() >1 || toLinks.size() > 1 )
		return;
	
	// From�m�[�h�ɐڑ����郊���N������ꍇ�A�G���[�ł͂Ȃ��ꍇ�A�߂�
	bool isFromDirect(true) ;
	long fromWalkableCode(-1L); 
	if( !fromLinks.empty() )
	{
		isFromDirect = ( fromID == fromLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID) ) );
		fromWalkableCode= fromLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );

		// �G���[�ł͂Ȃ��ꍇ�A�߂�
		if( !isConnectSideWalkError(isFromDirect, linkWalkableCode, fromWalkableCode ) )
			return;
	}

	// To�m�[�h�ɐڑ����郊���N������ꍇ�A�G���[�ł͂Ȃ��ꍇ�A�߂�
	bool isToDirect(true) ; 	
	long toWalkableCode(-1L);
	if( !toLinks.empty() )
	{
		isToDirect = ( toID == toLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID) ) ) ;
		toWalkableCode = toLinks.begin()->second->GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );

		// �G���[�ł͂Ȃ��ꍇ�A�߂�
		if( !isConnectSideWalkError(isToDirect, linkWalkableCode, toWalkableCode ) )
			return;
	}

	// �ڑ����郊���N�̒��ŁA����������T�C�h�iWALKABLE_C�̒l�j�����������N�����݂��Ȃ��ꍇ�G���[�B
	m_output.OutputRegLog( err_level::error, err_code::NgConnectedSideWalk );
}

bool RoadLinkCheckFunction::isConnectSideWalkError( bool isDirect, long linkWalkableCode, long connectedWalkableCode  )
{
	// �`�F�b�N�Ώۂ�WALKABLE_C�̒l��0,1,2,3�i�����A���s�֎~�Ȃǁj�̏ꍇ�A�ڑ������N�̒���WALKABLE_C�̒l����v���Ă���ꍇ�̓G���[�ł͂Ȃ��B
	switch(linkWalkableCode)
	{
	case road_link::walkable_class::kNotWalk:
	case road_link::walkable_class::kWalkable:
	case road_link::walkable_class::kWalkOnly:
	case road_link::walkable_class::kBothSideWalk:
		return !( linkWalkableCode == connectedWalkableCode );
	}

	// �`�F�b�N�Ώۂ�WALKABLE_C�̒l��4,5,6,7�i�E�A���j�̏ꍇ
	// �����N�̌������t�����ł͂Ȃ�
	if( isDirect )
	{
		return !( linkWalkableCode == connectedWalkableCode );
	} else
	// �����N�̌������t������
	{
		// ����������T�C�h�iWALKABLE_C�̒l�j��������������i�l�ԋ��p�j���ڑ������N�̕���������T�C�h�iWALKABLE_C�̒l�j���E����������i�l�ԋ��p�j�ꍇ�A�G���[�ł͂Ȃ�
		if( linkWalkableCode == road_link::walkable_class::kLeftSideWalkWalkable && connectedWalkableCode == road_link::walkable_class::kRightSideWalkWalkable )
			return false;
			
		// ����������T�C�h�iWALKABLE_C�̒l�j���E����������i�l�ԋ��p�j���ڑ������N�̕���������T�C�h�iWALKABLE_C�̒l�j��������������i�l�ԋ��p�j�ꍇ�A�G���[�ł͂Ȃ�
		if( linkWalkableCode == road_link::walkable_class::kRightSideWalkWalkable && connectedWalkableCode == road_link::walkable_class::kLeftSideWalkWalkable )
			return false;

		// ����������T�C�h�iWALKABLE_C�̒l�j��������������i���s�֎~�j���ڑ������N�̕���������T�C�h�iWALKABLE_C�̒l�j���E����������i���s�֎~�j�ꍇ�A�G���[�ł͂Ȃ�
		if( linkWalkableCode == road_link::walkable_class::kLeftSideWalkNonWalkable && connectedWalkableCode == road_link::walkable_class::kRightSideWalkNonWalkable )
			return false;
			
		// ����������T�C�h�iWALKABLE_C�̒l�j���E����������i���s�֎~�j���ڑ������N�̕���������T�C�h�iWALKABLE_C�̒l�j��������������i���s�֎~�j�ꍇ�A�G���[�ł͂Ȃ�
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
	// �ėp�����ꍇ
	if (  m_dataType == walk_type::type_class::kGeneral && road_link::walkable_class::kWalkable !=  walkableCode)
		return;
	// �_�E���R���o�[�g�����ꍇ
	if (  m_dataType == walk_type::type_class::kDownConvert )
	{
		// ���s�\�ȓ��H�����N�iWALKABLE_C=1�E3�`7�j�̂ݏꍇ
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

	// 2�����b�V�����E���ׂ������N�ɑ΂��Autil::getAdjacentLink()�Œ[�_�m�[�hID���ύX�����̂ŁA
	// ��̃`�F�b�N�ɉe�����Ȃ��悤�ɁA�Ǐ��I�ȕϐ�tempNodeID���g��
	auto tempNodeID = fromID;

	// From���̐ڑ������N���擾
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

	// To���̐ڑ������N���擾
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

	// from��to���̃����N�̎擾�Ɏ��s=�s���f�[�^�ŕʂ̃G���[�ň����|����̂Ŗ{�G���[�͕s�v
	if(checkedFromLinkIds.empty() || checkedToLinkIds.empty()) return;

	// WALKKABLE�����s�֎~�̎擾�p�����_��
	auto checkNotWalkable = [&roadLink](long id)
	{
		return road_link::walkable_class::kNotWalk == roadLink.m_FeatureMap.at( id ).GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
	};

	// �ڑ����Ă���n�_�E�I�_���̃����N�����s�֎~�ɂȂ�{�������ꂼ��1�{�ȏ�̏ꍇ�A�`�F�b�N�ΏۂƂȂ�
	long nFromRoadCount = std::count_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), checkNotWalkable);
	long nToRoadCount = std::count_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), checkNotWalkable);
	if( 0 == nFromRoadCount || 0 == nToRoadCount )
		return;

	// �_�E���R���o�[�g�����ꍇ
	std::vector<long> fromRoadLinks( nFromRoadCount ), toRoadLinks( nToRoadCount );
	if( m_dataType ==  walk_type::type_class::kDownConvert )
	{
		std::copy_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), fromRoadLinks.begin(), checkNotWalkable);
		std::copy_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), toRoadLinks.begin(), checkNotWalkable);
	}

	if ( doRebuildTree ) {
		//�ēǂݍ��݂���������(getAdjacentRoadLink���s������)�����c���[�č\�z
		util::createRTree( walkNode.m_FeatureMap, walkNodeRTree );
	}
	// from/to���̓��H�m�[�h�Əd�Ȃ������s�҃m�[�h���擾����
	if( roadNode.m_FeatureMap.find( fromID ) == roadNode.m_FeatureMap.end() || roadNode.m_FeatureMap.find( toID ) == roadNode.m_FeatureMap.end() ) return;

	auto fromRoadNodeObj = roadNode.m_FeatureMap.at( fromID );
	auto fromWalkNodeIDs = util::getIntersectsIDs( fromRoadNodeObj.GetBox(0), walkNodeRTree );
	auto toRoadNodeObj = roadNode.m_FeatureMap.at( toID );
	auto toWalkNodeIDs = util::getIntersectsIDs( toRoadNodeObj.GetBox(0), walkNodeRTree );

	// �G���[�Afrom��to���ɐڑ�������s�҃����N�����݂��Ȃ�
	if( m_dataType ==  walk_type::type_class::kGeneral && ( fromWalkNodeIDs.empty() || toWalkNodeIDs.empty() ) )
	{
		m_output.OutputRegLog( err_level::error, err_code::HumanCarSandwiched );
		return;
	}

	// ���s�ҕ\����ʃR�[�h���l�ԋ��p�̎擾�p�����_��
	auto checkHumanCarGeneral = [&walkLink](long id)
	{
		return walk_link::walk_class::kWalkableRoad == walkLink.m_FeatureMap.at( id ).GetLongValue( walkLink.m_FieldMap.FindField( walk_link::kWalkClass ) );
	};
	// ���s�ҕ\����ʃR�[�h�����W�J���R�l�N�V�����̎擾�p�����_��
	auto checkHumanCarDownConvert = [&walkLink](long id)
	{
		return walk_link::walk_class::kLogicalConnection == walkLink.m_FeatureMap.at( id ).GetLongValue( walkLink.m_FieldMap.FindField( walk_link::kWalkClass ) );
	};

	// from/to���̕��s�҃����N���擾����
	long nFromWalkCount = 0;
	for(const long fromWalkNodeId : fromWalkNodeIDs)
	{
		auto fromLinkIDSetItr = walkNodeLinkIDs.find( fromWalkNodeId );
		// �ėp�����ꍇ�A
		if( m_dataType == walk_type::type_class::kGeneral )
			nFromWalkCount += std::count_if(fromLinkIDSetItr->second.begin(), fromLinkIDSetItr->second.end(), checkHumanCarGeneral);
		// �_�E���R���o�[�g�����ꍇ
		if( m_dataType == walk_type::type_class::kDownConvert )
			nFromWalkCount += std::count_if(fromLinkIDSetItr->second.begin(), fromLinkIDSetItr->second.end(), checkHumanCarDownConvert);
	}

	long nToWalkCount = 0;
	for(const long toWalkNodeId : toWalkNodeIDs)
	{
		auto toLinkIDSetItr = walkNodeLinkIDs.find( toWalkNodeId );
		// �ėp�����ꍇ�A
		if( m_dataType == walk_type::type_class::kGeneral )
			nToWalkCount += std::count_if(toLinkIDSetItr->second.begin(), toLinkIDSetItr->second.end(), checkHumanCarGeneral);
		// �_�E���R���o�[�g�����ꍇ
		if( m_dataType == walk_type::type_class::kDownConvert )
			nToWalkCount += std::count_if(toLinkIDSetItr->second.begin(), toLinkIDSetItr->second.end(), checkHumanCarDownConvert);
	}

	// �ėp�����ꍇ�A���s�҃����N�Ɠ��H�����N�̖{������v���Ȃ��ꍇ�A�G���[
	if( m_dataType == walk_type::type_class::kGeneral  && (nFromWalkCount + nToWalkCount) != (nFromRoadCount + nToRoadCount) )
	{
		// ���G���[ ���s�֎~�̒��Ől�ԋ��p�������Ă���
		m_output.OutputRegLog( err_level::error, err_code::HumanCarSandwiched );
	}


	// �ėp�����ꍇ�A�߂�
	if( m_dataType == walk_type::type_class::kGeneral )
		return;

	// �_�E���R���o�[�g�����ꍇ	
	// from���̕��s�҃����N���擾����
	std::vector<long> fromWalkLink(nFromWalkCount), toWalkLink(nToWalkCount);
	for(const long fromWalkNodeId : fromWalkNodeIDs)
	{
		auto fromLinkIDSetItr = walkNodeLinkIDs.find( fromWalkNodeId );
		std::copy_if(fromLinkIDSetItr->second.begin(), fromLinkIDSetItr->second.end(), fromWalkLink.begin(), checkHumanCarDownConvert);
	}
	// to���̕��s�҃����N���擾����
	for(const long toWalkNodeId : toWalkNodeIDs)
	{
		auto toLinkIDSetItr = walkNodeLinkIDs.find( toWalkNodeId );
		std::copy_if(toLinkIDSetItr->second.begin(), toLinkIDSetItr->second.end(), toWalkLink.begin(), checkHumanCarDownConvert);
	}
	// ���W�J���R�l�N�V�����̕��s�҃����N���s�݂̏ꍇ�A�o��
	if( ( fromWalkLink.empty() && toWalkLink.empty() ) || ( checkedFromLinkIds.size() > fromWalkLink.size()  && checkedToLinkIds.size() > toWalkLink.size() ))
	{
		// ���G���[ ���s�֎~�̒��Ől�ԋ��p�������Ă���
		m_output.OutputRegLog( err_level::notice, err_code::HumanCarSandwiched );
		return;
	}

	// from���s�Ґڑ������N�͓��H�����N�Ɗ��S��v�̃����N����
	long fromLinkCount = util::getDifferenceRoadLinkAndWalkLinkCount(checkedFromLinkIds, fromWalkLink, roadLink, walkLink );

	// to���s�Ґڑ������N�͓��H�����N�Ɗ��S��v�̃����N����
	long toLinkCount = util::getDifferenceRoadLinkAndWalkLinkCount(checkedToLinkIds, toWalkLink, roadLink, walkLink );
	// ���s�s�\�ȓ��H�����N��Ƀ��W�J���R�l�N�V�������s�Ґڑ������N�����S��v�Ő�������Ă��郊���N���O�Afrom/to���s�֎~���H�ڑ������N���݂���ꍇ�A�o��
	if( toLinkCount > 0 && fromLinkCount > 0 )
	{
		// ���G���[ ���s�֎~�̒��Ől�ԋ��p�������Ă���
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

	// ���s�֎~�̂݁A�`�F�b�N�Ώ�
	long  walkable = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
	if ( road_link::walkable_class::kNotWalk != walkable )
		return;

	long  fromID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kFromNodeID ) );
	long  toID = linkObj.GetLongValue( roadLink.m_FieldMap.FindField( road_link::kToNodeID ) );

	// 2�����b�V�����E���ׂ������N�ɑ΂��Autil::getAdjacentLink()�Œ[�_�m�[�hID���ύX�����̂ŁA
	// ��̃`�F�b�N�ɉe�����Ȃ��悤�ɁA�Ǐ��I�ȕϐ�tempNodeID���g��
	auto tempNodeID = fromID;

	// From���̐ڑ������N���擾
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

	// To���̐ڑ������N���擾
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

	// from��to���̃����N�̎擾�Ɏ��s=�s���f�[�^�ŕʂ̃G���[�ň����|����̂Ŗ{�G���[�͕s�v
	if(checkedFromLinkIds.empty() || checkedToLinkIds.empty()) return;

	long nFromRoadCount(0L), nToRoadCount(0L);
	// WALKKABLE���l�ԋ��p�̎擾�p�����_��
	if( m_dataType == walk_type::type_class::kGeneral )
	{
		auto checkHumanCar = [&roadLink](long id)
		{
			long walkatbleCode = roadLink.m_FeatureMap.at( id ).GetLongValue( roadLink.m_FieldMap.FindField( road_link::kWalkable ) );
			return road_link::walkable_class::kWalkable == walkatbleCode;
		};	
		
		// �ڑ����Ă���n�_�E�I�_���̃����N���l�ԋ��p�ɂȂ�{�������ꂼ��1�{�ȏ�̏ꍇ�A�`�F�b�N�ΏۂƂȂ�
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

		// �ڑ����Ă���n�_�E�I�_���̃����N���l�ԋ��p�ɂȂ�{�������ꂼ��1�{�ȏ�̏ꍇ�A�`�F�b�N�ΏۂƂȂ�
		nFromRoadCount = std::count_if(checkedFromLinkIds.begin(), checkedFromLinkIds.end(), checkHumanCar);
		nToRoadCount = std::count_if(checkedToLinkIds.begin(), checkedToLinkIds.end(), checkHumanCar);
	}

	if( 0 == nFromRoadCount || 0 == nToRoadCount )
		return;
	if ( bSearchFlg )
	{
		//�ēǂݍ��݂���������(getAdjacentRoadLink���s������)�����c���[�č\�z
		util::createRTree( walkNode.m_FeatureMap, walkNodeRTree );
	}
	// from/to���̓��H�m�[�h�Əd�Ȃ������s�҃m�[�h���擾����
	if( roadNode.m_FeatureMap.find( fromID ) == roadNode.m_FeatureMap.end() || roadNode.m_FeatureMap.find( toID ) == roadNode.m_FeatureMap.end() )	return;
	auto fromRoadNodeObj = roadNode.m_FeatureMap.at( fromID );
	auto fromWalkNodeIDs = util::getIntersectsIDs( fromRoadNodeObj.GetBox(0), walkNodeRTree );
	auto toRoadNodeObj = roadNode.m_FeatureMap.at( toID );
	auto toWalkNodeIDs = util::getIntersectsIDs( toRoadNodeObj.GetBox(0), walkNodeRTree );

	// �G���[�Afrom��to���ɐڑ�������s�҃����N�����݂��Ȃ�
	if( m_dataType ==  walk_type::type_class::kGeneral && ( fromWalkNodeIDs.empty() || toWalkNodeIDs.empty() ) )
	{
		m_output.OutputRegLog( err_level::error, err_code::NotWalkSandwiched,
			judge_value::IsFindObj( walkable, m_walkableMap ) );
		return;
	}

	// �Ώۃ����N�Əd�Ȃ������s�҃����N���擾����
	set<long> overWalkLinkIDs;
	IGeometryPtr unionTargetLink = util::getUnionGeometry( roadLink.m_FeatureMap, targetLinkIDs );
	if(bSearchFlg)
	{
		// �Ώۃ����N���s�s�n�}���b�V�����т����ꍇ�A���̓s�s�n�}���b�V�����`�F�b�N�͈͂Ɋi�[����Ȃ����߁A
		// �Ώۃ����N�̌`���DB���������A���s�҃����N���擾����
		FeaturesPack tempOverWalkLinkMap( walkLink, false ); //m_FeatureMap�̓R�s�[����Ȃ�
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

	// �d�Ȃ������s�҃����N�̕\����ʃR�[�h���u�l�ԋ��p�����N�v�̏ꍇ�̂݁A�������ꂽ�Ƃ݂Ȃ�
	std::set<long> overHumanCar;
	long walkClassindex = walkLink.m_FieldMap.FindField( walk_link::kWalkClass );
	for(auto overWalkLinkId : overWalkLinkIDs)
	{
		long walkClass = walkLink.m_FeatureMap.at( overWalkLinkId ).GetLongValue( walkClassindex );
		// �ėp�����ꍇ
		if ( walk_type::type_class::kGeneral == m_dataType && walk_link::walk_class::kWalkableRoad == walkClass )
			overHumanCar.insert(overWalkLinkId);
		// �_�E���R���o�[�g�����ꍇ�A���s�s�\�Ȏ�ʂ̓��H�����N��Ƀ��W�J���R�l�N�V���������S��v�Ő�������Ă���ꍇ�͕��s�\�̓��H�����N�Ƃ݂Ȃ�
		if ( walk_type::type_class::kDownConvert == m_dataType && walk_link::walk_class::kLogicalConnection == walkClass )
			overHumanCar.insert(overWalkLinkId);
	}

	// �l�ԋ��p�̕��s�҃����N��Union����
	IGeometryPtr unionWalkLink = util::getUnionGeometry( walkLink.m_FeatureMap, overHumanCar );

	// �Ώۃ����N�����s�҃����N�ɕ�܂���Ă��Ȃ��ꍇ�A�G���[
	if( !AheIsWithin(unionTargetLink, unionWalkLink) )
	{
		// ���G���[ �l�ԋ��p�̒��ŕ��s�֎~�������Ă���
		m_output.OutputRegLog( walk_type::type_class::kGeneral == m_dataType ?err_level::error : err_level::notice, err_code::NotWalkSandwiched,
			judge_value::IsFindObj( walkable, m_walkableMap ) );
	}
}

// ���b�V���O�̐ڑ����s�҃����N���擾����
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

	// �T���̋N�_�ƂȂ�m�[�h���L���b�V�����ꂽMap���ɑ��݂��Ȃ��A
	// �܂��́A�w��m�[�h�ɐڑ����郊���N��1�݂̂̏ꍇ�A
	// ���̃m�[�h�����݂��郁�b�V�����̃����N�E�m�[�h���������R���e�i�ɒǉ�����
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
	
	// �Č������Ă�roadNodeMap�ɑ��݂��Ȃ��ꍇ�́A��ŕԂ�
	if ( roadNode.m_FeatureMap.end() == baseNodeIt )
		return result;

	// �m�[�h��ʂ��[�_�m�[�h�̏ꍇ�A�T���I��
	if ( road_node::node_class::kTerminal == roadNode.m_FeatureMap.at( nodeId ).GetLongValue( nodeClassIndex ) )
		return result;
	
	std::set<long> linkIDSet = roadNodeLinkIDs.find( nodeId )->second;
	auto adjacentLink = util::GetObjectMap( linkId, linkIDSet, roadLink.m_FeatureMap );

	// ���E�m�[�h�ł͂Ȃ��ꍇ�A�T���I��
	if ( !util::isMeshEdgeNode( nodeId, roadNode, false ) )
	{
		for( auto& linkInfo : adjacentLink )
			result.insert(linkInfo.first);
		return result;
	}

	// �ڑ������N�������i2���H�ł͂Ȃ��j�ꍇ�A�T���I��
	if( adjacentLink.size() > 1 )
	{
		for( auto& linkInfo : adjacentLink )
			result.insert(linkInfo.first);
		return result;
	}

	// �����܂ł����ꍇ�A�w��m�[�h�ɐڑ����郊���N��0���ƂȂ邪�A
	// 0�����ƈȍ~�̏������s���Ȃ��̂ŁAreturn���Ă��܂�
	if( adjacentLink.empty() )
		return result;

	// �ڑ������N�̔�ڑ����̒[�_�m�[�h���擾����
	long adjLinkId = adjacentLink.begin()->first;
	long otherNodeId = util::getOtherEdgeNode(adjLinkId, roadLink, nodeId);

	// �T���Ōo�R���������N��Ώۃ����N�Ƃ��ĕۑ�����
	targetLinkIDs.insert(linkId);

	// ��ڑ����̃m�[�h���`�F�b�N�͈͊O�̏ꍇ�A���̏��݂��郁�b�V���̏��������R���e�i�[�ɒǉ�����
	auto otherNodeIt = roadNode.m_FeatureMap.find( otherNodeId );
	if ( roadNode.m_FeatureMap.end() == otherNodeIt ||
		util::isOnlyTargetRoadLink( otherNodeIt, adjLinkId, roadNodeLinkIDs, nodeClassIndex ) )
	{
		addMeshInfo( otherNodeId, ipMeshGeo,
			roadLink, roadNode, roadNodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs );

		otherNodeIt = roadNode.m_FeatureMap.find( otherNodeId );
	}

	// �Č������Ă�roadNodeMap�ɑ��݂��Ȃ��ꍇ�́Areturn���Ă��܂�
	if ( roadNode.m_FeatureMap.end() == otherNodeIt )
		return result;

	// 2�����b�V�����E���ׂ������N�ɑ΂��Autil::getAdjacentLink()�Œ[�_�m�[�hID���ύX�����̂ŁA
	// ��̃`�F�b�N�ɉe�����Ȃ��悤�ɁA�Ǐ��I�ȕϐ�tempNodeID���g��
	auto tempNodeID = otherNodeId;
	std::set<long> secondLinkIDSet = roadNodeLinkIDs.find( otherNodeId )->second;
	auto secondAdjLinks = util::GetObjectMap( adjLinkId, secondLinkIDSet, roadLink.m_FeatureMap );
	result = util::getAdjacentLink( tempNodeID, secondAdjLinks, roadLink, roadNode, roadNodeLinkIDs, false );

	if( result.empty() )
	{
		// nodeId���T���̏I�_
		nodeId = otherNodeId;

		// �אڃ��b�V��������b�V���Ƃ��čċA�I�ɒT��
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
		// �T���I�_�̃����N��⑫�itempNodeID���T���̏I�_�AotherNodeId��targetLinkIDs�Ɋi�[���ꂽ�����N�Q�̏I�_�j
		if( otherNodeId != tempNodeID )
		{
			long tempID = secondAdjLinks.begin()->first;
			targetLinkIDs.insert(tempID);
		}

		nodeId = tempNodeID;
	}

	return result;
}

// �Ώۃm�[�h�̏��݂��郁�b�V���̏���ǉ�����
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
	// �w�肵��OID�����m�[�h�����݂��Ȃ��ꍇ�́A�ȍ~�̏����͍s��Ȃ�
	if ( !ipRow )
		return;
	auto targetNodeObj = GeoClass( ipRow );
	// �[�_�̍��W���擾����
	DOUBLE dx = 0.0f, dy = 0.0f;
	INT nMeshCode = 0, nMeshX = 0, nMeshY = 0;

	IPointPtr ipAdjPoint(targetNodeObj.m_ipGeometry);
	ipAdjPoint->QueryCoords( &dx, &dy );
	nMeshCode = AheLL2MESH( dx, dy, 64, nMeshX, nMeshY, 1 );

	// �[�_���s�s�n�}���b�V���̋��E��
	bool isCityMeshEdge = false;
	if( 0 == nMeshX ||  0 == nMeshY )
		isCityMeshEdge = true;

	// �[�_�̏��݂���s�s�n�}���b�V�����擾����
	ISpatialReferencePtr ipSpRef;
	ipMeshGeo->get_SpatialReference( &ipSpRef );

	IGeometryPtr ipAdjMeshGeo, ipAdjMeshGeo2;
	ipAdjMeshGeo = AheMESH2POLY( nMeshCode, ipSpRef );

	// �[�_�m�[�h���s�s�n�}���b�V���̋��E��̏ꍇ�A�אڂ̃��b�V�����擾����K�v������
	// ��ƉE�̋��E�m�[�h�́AAheLL2MESH�ŕʂ̃��b�V���R�[�h�ɕϊ������̂ŁA�����ł͍��Ɖ�������������
	if( isCityMeshEdge )
	{
		long adjMeshCode = 0;

		// X��Y�������Ƃ�0�̏ꍇ�A���A���A�����̂R���b�V�����擾
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

	// ���H�����N�擾
	if ( !util::GetLinkFeaturesInGeometry( ipAdjMeshGeo, field_lists::roadLinkFieldList, _T( "" ), roadLink, roadNodeLinkIDs ) )
		return;

	// ���H�m�[�h�擾
	if ( !util::GetFeaturesInGeometry( ipAdjMeshGeo, field_lists::roadNodeFieldList, _T( "" ), roadNode ) )
		return;

	// ���s�҃����N�擾
	if ( !util::GetLinkFeaturesInGeometry( ipAdjMeshGeo, field_lists::walkLinkFieldList, _T( "" ),  walkLink, walkNodeLinkIDs ) )
		return;

	// ���s�҃m�[�h�擾
	if ( !util::GetFeaturesInGeometry( ipAdjMeshGeo, field_lists::walkNodeFieldList, _T( "" ), walkNode ) )
		return;
}

// ���s�\��ʃR�[�h���s���Ȓl���`�F�b�N
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

// ���b�V�����E�ŕ��s�\��ʂ��ς���Ă��邩�`�F�b�N
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
