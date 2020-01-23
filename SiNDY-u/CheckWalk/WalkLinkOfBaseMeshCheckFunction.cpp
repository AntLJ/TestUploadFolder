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
#include "WalkLinkOfBaseMeshCheckFunction.h"
#include "value.h"

using namespace sindy::schema;
using namespace std;

void WalkLinkOfBaseMeshCheckFunction::checkWalkLinkOfBaseMesh(
	const IGeometryPtr& ipMeshGeo,
	const IGeometryPtr& ipBoundaryMesh,
	const map<long, std::pair<IGeometryPtr,IGeometryPtr>>& cityMeshMap,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs )
{
	using namespace walk_link;
	using namespace ipc_feature;
	CString info1, info2;

	m_output.SetLayer( kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, _T("WALK_LINK(BASE MESH)") );

	// ���s�҃����N�̃`�F�b�N
	long  fromWalkNodeIDIndex = walkLink.m_FieldMap.FindField( kFromNodeID );
	long  toWalkNodeIDIndex = walkLink.m_FieldMap.FindField( kToNodeID );
	long  onewayCodeIndex = walkLink.m_FieldMap.FindField( kOneway );
	long  noPassFlagIndex = walkLink.m_FieldMap.FindField( kNoPassageRF );
	long modOperatorIndex = walkLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = walkLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = walkLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = walkLink.m_FieldMap.FindField( kProgModifyDate );
	long onewayIndex = walkLink.m_FieldMap.FindField( kOneway );
	long walkNodeClassIndex = walkNode.m_FieldMap.FindField( walk_node::kNodeClass );

	long startMonthIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kStartMonth );
	long startDayIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kStartDay );
	long startHourIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kStartHour );
	long startMinIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kStartMin );
	long endMonthIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kEndMonth );
	long endDayIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kEndDay );
	long endHourIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kEndHour );
	long endMinIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kEndMin );
	long dayOfWeekIndex = m_walkNoPassage.m_FieldMap.FindField( walk_nopassage::kDayOfWeek );

	for ( const auto& walkLinkPair : walkLink.m_FeatureMap ) {

		// �����Ώے��k���b�V�����ł͂Ȃ����̂̓X�L�b�v
		if( !walkLinkPair.second.IsWithin( ipMeshGeo ) && !walkLinkPair.second.IsCross( ipMeshGeo ) )
			continue;

		// �����Ώۓs�s�n�}���b�V�����i�g��܂ށj�ł͂Ȃ����̂̓X�L�b�v
		long cityMeshcode = util::GetExistMeshcode( walkLinkPair.second, cityMeshMap);
		if( cityMeshcode == 0 )
			continue;
		CString strCityMeshcode = uh::str_util::ToString(cityMeshcode);

		long  walkLinkID       = walkLinkPair.first;
		auto& walkLinkObj      = walkLinkPair.second;
		long  fromWalkNodeID = walkLinkObj.GetLongValue( fromWalkNodeIDIndex );
		long  toWalkNodeID = walkLinkObj.GetLongValue( toWalkNodeIDIndex );
		long  onewayCode = walkLinkObj.GetLongValue( onewayCodeIndex );
		long  noPassFlag = walkLinkObj.GetLongValue( noPassFlagIndex );
		m_output.SetMeshCode( strCityMeshcode );
		m_output.SetModifyInfo( walkLinkID , walkLinkObj.GetStringValue( modOperatorIndex ) ,
			walkLinkObj.GetStringValue( modDateIndex ) , walkLinkObj.GetStringValue( modProgramIndex ) , walkLinkObj.GetStringValue( modProgramDateIndex ) );

		// From/To�m�[�h�擾
		bool bFromOK = false, bToOK = false;
		auto fromNodeItr = walkNode.m_FeatureMap.find( fromWalkNodeID );
		if ( fromNodeItr != walkNode.m_FeatureMap.end() )
			bFromOK = true;
		auto toNodeItr = walkNode.m_FeatureMap.find( toWalkNodeID );
		if ( toNodeItr != walkNode.m_FeatureMap.end() )
			bToOK = true;

		bool bFromNodeMesh = false, bToNodeMesh = false;

		// ���b�V���g�Ɗ֘A�����邩�H
		if( ! walkLinkObj.IsDisjoint( ipBoundaryMesh ) ){

			// �N���X���Ă��Ȃ����H
			if ( !walkLinkObj.IsCross( ipBoundaryMesh ) ) {

				// �ǂ��炩�����b�V���}�s�m�[�h����Ȃ��Ƃ����Ȃ�
				bool bOK = false;

				if ( (fromNodeItr != walkNode.m_FeatureMap.end()
					&& fromNodeItr->second.GetLongValue( walkNodeClassIndex ) == walk_node::node_class::kOutline) ) {
					bOK = true;
					bFromNodeMesh = true;
				}
				if ( (toNodeItr != walkNode.m_FeatureMap.end()
					&& toNodeItr->second.GetLongValue( walkNodeClassIndex ) == walk_node::node_class::kOutline) ) {
					bOK = true;
					bToNodeMesh = true;
				}

				// ���G���[ ���b�V���ׂ��̃����N�ɐ}�s�m�[�h�����݂��Ȃ�
				if ( !bOK )
					m_output.OutputRegLog( err_level::error, err_code::NoBorderNode );
			}
			else // ���G���[ 2�����b�V�������f���Ă���
				m_output.OutputRegLog( err_level::error, err_code::CrossMesh );
		}

		// ���b�V���֘A�̓��b�V���𒴂��đ�������v���邩�̊m�F���s��
		if( ( bFromNodeMesh && bFromOK ) || ( bToNodeMesh && bToOK ) ){
			CString diffField;
			long walkNodeID = 0;
			if( bFromNodeMesh )
				walkNodeID = fromWalkNodeID;
			else
				walkNodeID = toWalkNodeID;

			// ���̐ڑ������N���擾����
			auto linkIDSetItr = walkNodeLinkIDs.find( walkNodeID );
			if ( linkIDSetItr != walkNodeLinkIDs.end() && linkIDSetItr->second.size() == 2 ) {
				auto linkIDSet = linkIDSetItr->second;
				auto neighborLinks = util::GetObjects( walkLinkID, linkIDSetItr->second, walkLink.m_FeatureMap );
				auto neighborLink = *(neighborLinks.begin()); //!< (���b�V��)�m�[�h�ɐڑ����郊���N

				// ��������v���邩�H
				diffField = judge_value::IsSameWalkLinkAttr( walkLinkObj, neighborLink, walkLink.m_FieldMap );
				if ( !diffField.IsEmpty() ) {
					// ���G���[ ���b�V�����E�̗אڂő������قȂ�
					long diffIndex = walkLink.m_FieldMap.FindField( diffField );
					info1.Format( _T( "%s(%d)" ), diffField, walkLinkObj.GetLongValue( diffIndex ) );
					info2.Format( _T( "%s(%d)" ), diffField, neighborLink.GetLongValue( diffIndex ) );
					m_output.OutputRegLog( err_level::error, err_code::NotSameBorderAttr, info1, info2 );
				}

				// ����ʍs�����ꍇ�A�������ω����Ă��Ȃ����H
				if ( onewayCode != oneway::kNoOneway ) {
					if ( !judge_value::IsSameOneway( walkLinkObj, neighborLink, walkNodeID, walkLink.m_FieldMap ) ) {
						// ���G���[ ����ʍs��2�����b�V�����E�œr�؂�Ă���
						m_output.OutputRegLog( err_level::error, err_code::NgBorderOneway,
							judge_value::IsFindObj( onewayCode, m_onewayClassMap ),
							judge_value::IsFindObj( neighborLink.GetLongValue( onewayIndex ), m_onewayClassMap ) );
					}
				}
				// �ʍs�֎~����������Ă���ꍇ�A�K�����Ԃ�������
				if ( FLAG_ON == noPassFlag ) {
					long neighborWalLinkID = neighborLink.m_ObjectID;

					if ( m_walkNoPassage.m_Rows.count( walkLinkID ) != m_walkNoPassage.m_Rows.count( neighborWalLinkID ) )
						//���G���[ �ʍs�֎~��2�����b�V�����E�ŕς���Ă���
						m_output.OutputRegLog( err_level::error, err_code::NgBorderNoPassage );
					else
					{
						auto rangeNoPassage = m_walkNoPassage.m_Rows.equal_range( walkLinkID );
						auto rangeNeighborNoPassages = m_walkNoPassage.m_Rows.equal_range( neighborWalLinkID );
						for ( MULTIMAP_ITE ite = rangeNoPassage.first; ite != rangeNoPassage.second; ++ite )
						{
							bool flag = false;
							auto& baseObj = ite->second;
							for ( MULTIMAP_ITE cmpIte = rangeNeighborNoPassages.first; cmpIte != rangeNeighborNoPassages.second; ++cmpIte )
							{
								auto& cmpObj = cmpIte->second;
								if ( baseObj->GetValue( startMonthIndex ) == cmpObj->GetValue( startMonthIndex ) &&
									baseObj->GetValue( startDayIndex ) == cmpObj->GetValue( startDayIndex ) &&
									baseObj->GetValue( startHourIndex ) == cmpObj->GetValue( startHourIndex ) &&
									baseObj->GetValue( startMinIndex ) == cmpObj->GetValue( startMinIndex ) &&
									baseObj->GetValue( endMonthIndex ) == cmpObj->GetValue( endMonthIndex ) &&
									baseObj->GetValue( endDayIndex ) == cmpObj->GetValue( endDayIndex ) &&
									baseObj->GetValue( endHourIndex ) == cmpObj->GetValue( endHourIndex ) &&
									baseObj->GetValue( endMinIndex ) == cmpObj->GetValue( endMinIndex ) &&
									baseObj->GetValue( dayOfWeekIndex ) == cmpObj->GetValue( dayOfWeekIndex ) )
								{
									flag = true;
									break;
								}
							}
							if ( !flag ) //���G���[ �ʍs�֎~��2�����b�V�����E�ŕς���Ă���
								m_output.OutputRegLog( err_level::error, err_code::NgBorderNoPassage );
						}
					}
				}

			}
			else // ���G���[ ���b�V�����E�ŗאڃ����N���s��
				m_output.OutputRegLog( err_level::error, err_code::NgBorderLink );
		}
	}
}

void WalkLinkOfBaseMeshCheckFunction::checkRelease(
	const IGeometryPtr& ipMeshGeo,
	const IGeometryPtr& ipBoundaryMesh,
	const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs
	)
{
	using namespace walk_link;
	using namespace ipc_feature;
	m_output.SetLayer( kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, _T("WALK_LINK(BASE MESH)") );

	// ���s�҃����N�̃`�F�b�N
	long  fromWalkNodeIDIndex = walkLink.m_FieldMap.FindField(  kFromNodeID );
	long  toWalkNodeIDIndex = walkLink.m_FieldMap.FindField(  kToNodeID );

	long modOperatorIndex = walkLink.m_FieldMap.FindField( kOperator );
	long modDateIndex = walkLink.m_FieldMap.FindField( kModifyDate );
	long modProgramIndex = walkLink.m_FieldMap.FindField( kModifyProgName );
	long modProgramDateIndex = walkLink.m_FieldMap.FindField( kProgModifyDate );
	long walkNodeClassIndex = walkNode.m_FieldMap.FindField( walk_node::kNodeClass );
	for ( const auto& walkLinkPair : walkLink.m_FeatureMap ) {

		auto& walkLinkObj = walkLinkPair.second;
		// �����Ώے��k���b�V�����ł͂Ȃ����̂̓X�L�b�v
		if( !walkLinkObj.IsWithin( ipMeshGeo ) && !walkLinkObj.IsCross( ipMeshGeo ) )
			continue;

		// �����Ώۓs�s�n�}���b�V�����i�g��܂ށj�ł͂Ȃ����̂̓X�L�b�v
		long cityMeshcode = util::GetExistMeshcode( walkLinkObj, cityMeshMap);
		if( cityMeshcode == 0 )
			continue;
		CString strCityMeshcode = uh::str_util::ToString(cityMeshcode);

		long  fromWalkNodeID = walkLinkObj.GetLongValue( fromWalkNodeIDIndex );
		long  toWalkNodeID = walkLinkObj.GetLongValue( toWalkNodeIDIndex );
		m_output.SetMeshCode( strCityMeshcode );
		m_output.SetModifyInfo( walkLinkPair.first, walkLinkObj.GetStringValue( modOperatorIndex ), walkLinkObj.GetStringValue( modDateIndex ),
			walkLinkObj.GetStringValue( modProgramIndex ), walkLinkObj.GetStringValue( modProgramDateIndex ) );

		// From/To�m�[�h�擾
		bool bFromOK = false, bToOK = false;
		auto fromNodeItr = walkNode.m_FeatureMap.find( fromWalkNodeID );
		if ( fromNodeItr != walkNode.m_FeatureMap.end() )
			bFromOK = true;
		auto toNodeItr = walkNode.m_FeatureMap.find( toWalkNodeID );
		if ( toNodeItr != walkNode.m_FeatureMap.end() )
			bToOK = true;

		bool bFromNodeMesh = false, bToNodeMesh = false;

		// ���b�V���g�Ɗ֘A�����邩�H
		if( ! walkLinkObj.IsDisjoint( ipBoundaryMesh ) ){

			// �N���X���Ă��Ȃ����H
			if ( !walkLinkObj.IsCross( ipBoundaryMesh ) ) {

				// �ǂ��炩�����b�V���}�s�m�[�h����Ȃ��Ƃ����Ȃ�
				bool bOK = false;

				if ( (fromNodeItr != walkNode.m_FeatureMap.end()
					&& fromNodeItr->second.GetLongValue( walkNodeClassIndex ) == walk_node::node_class::kOutline) ) {
					bOK = true;
					bFromNodeMesh = true;
				}
				if ( (toNodeItr != walkNode.m_FeatureMap.end()
					&& toNodeItr->second.GetLongValue( walkNodeClassIndex ) == walk_node::node_class::kOutline) ) {
					bOK = true;
					bToNodeMesh = true;
				}

				// ���G���[ ���b�V���ׂ��̃����N�ɐ}�s�m�[�h�����݂��Ȃ�
				if ( !bOK )
					m_output.OutputRegLog( err_level::error, err_code::NoBorderNode );
			}
			else // ���G���[ 2�����b�V�������f���Ă���
				m_output.OutputRegLog( err_level::error, err_code::CrossMesh );
		}

		// ���b�V���֘A�̓��b�V���𒴂��đ�������v���邩�̊m�F���s��
		if( ( bFromNodeMesh && bFromOK ) || ( bToNodeMesh && bToOK ) ){
			CString diffField;
			long walkNodeID = 0;
			if( bFromNodeMesh )
				walkNodeID = fromWalkNodeID;
			else
				walkNodeID = toWalkNodeID;

			// ���̐ڑ������N���擾����
			auto linkIDSetItr = walkNodeLinkIDs.find( walkNodeID );
			if ( !(linkIDSetItr != walkNodeLinkIDs.end() && linkIDSetItr->second.size() == 2) )
				// ���G���[ ���b�V�����E�ŗאڃ����N���s��
				m_output.OutputRegLog( err_level::error, err_code::NgBorderLink );
		}
	}
}
