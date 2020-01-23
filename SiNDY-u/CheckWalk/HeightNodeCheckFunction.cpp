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
#include "HeightNodeCheckFunction.h"
#include "value.h"

#include <ArcHelperEx/AheGeometryOp.h>

using namespace sindy::schema;
using namespace std;

void HeightNodeCheckFunction::checkHeightNode(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& segmentAttr,
	const FeaturesPack& walkLink,
	const FeaturesPack& heightNode,
	const BGRTree& roadNodeRTree,
	const BGRTree& segmentAttrRTree,
	const BGRTree& walkNodeRTree,
	const BGRTree& heightNodeRTree,
	properties::check_target checktarget,
	bool isRelease /* = false */
)
{
	using namespace height_node;
	using namespace ipc_feature;
	using namespace uh::str_util;
	using namespace properties;
	CString info1, info2;

	m_output.SetLayer(kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	// �d�Ȃ胊���N�����m�[�h�̃`�F�b�N
	long linkID1Index = heightNode.m_FieldMap.FindField(kLinkID1);
	long linkID1HeightIndex = heightNode.m_FieldMap.FindField(kLinkID1Height);
	long linkID1TableIndex = heightNode.m_FieldMap.FindField(kLinkID1Table);

	long linkID2Index = heightNode.m_FieldMap.FindField(kLinkID2);
	long linkID2HeightIndex = heightNode.m_FieldMap.FindField(kLinkID2Height);
	long linkID2TableIndex = heightNode.m_FieldMap.FindField(kLinkID2Table);

	long modOperatorIndex = heightNode.m_FieldMap.FindField(kOperator);
	long modDateIndex = heightNode.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = heightNode.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = heightNode.m_FieldMap.FindField(kProgModifyDate);

	long walkClassIndex = walkLink.m_FieldMap.FindField(walk_link::kWalkClass);

	long segmentLinkIDIndex = segmentAttr.m_FieldMap.FindField(segment_attr::kLinkID);
	long segmentLinkAttrIndex = segmentAttr.m_FieldMap.FindField(segment_attr::kLinkAttr);

	// ���G���[ HEIGHT_NODE�̌`�󂪕s��
	if (m_dataType == walk_type::type_class::kDownConvert)
		checkHeightNodeGeometry(heightNode.m_ipTable);

	for (const auto& heightNodeTmp : heightNode.m_FeatureMap)
	{

		auto& heightNodeObj = heightNodeTmp.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (heightNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long  objectId = heightNodeTmp.first;
		std::list<HEIGHT_NODE_INFO> heightNodeInfo;

		HEIGHT_NODE_INFO link1, link2;
		link1.linkID = heightNodeObj.GetLongValue(linkID1Index);
		link1.linkIDHeight = heightNodeObj.GetLongValue(linkID1HeightIndex);
		link1.linkIDTable = heightNodeObj.GetLongValue(linkID1TableIndex);

		link2.linkID = heightNodeObj.GetLongValue(linkID2Index);
		link2.linkIDHeight = heightNodeObj.GetLongValue(linkID2HeightIndex);
		link2.linkIDTable = heightNodeObj.GetLongValue(linkID2TableIndex);

		heightNodeInfo.push_back(link1);
		heightNodeInfo.push_back(link2);

		m_output.SetModifyInfo(objectId, heightNodeObj.GetStringValue(modOperatorIndex),
			heightNodeObj.GetStringValue(modDateIndex), heightNodeObj.GetStringValue(modProgramIndex),
			heightNodeObj.GetStringValue(modProgramDateIndex));


		// �����N���m�̃`�F�b�N
		if (link1.linkID == link2.linkID) // ���G���[ ���Ȍ����ӏ��ɕt�^����Ă���
			m_output.OutputRegLog(err_level::error, err_code::NgSelhCross,
				ToString(link1.linkID), ToString(link2.linkID));

		if (link1.linkIDTable == sindy::schema::height_node::link_table::kRoadLink &&
			link2.linkIDTable == sindy::schema::height_node::link_table::kRoadLink)
			// ���G���[ ���H�����N���m�̑��΍���
			m_output.OutputRegLog(err_level::error, err_code::NgBothRoadLink,
				ToString(link1.linkID), ToString(link2.linkID));

		if (link1.linkIDHeight == link2.linkIDHeight) // ���G���[ ���΍���������
		{
			// �����ꂩ�Е���ROAD_LINK�̏ꍇ
			if (link1.linkIDTable == sindy::schema::height_node::link_table::kRoadLink ||
				link2.linkIDTable == sindy::schema::height_node::link_table::kRoadLink)
			{
				// 0-0�̓G���[�ΏۊO
				if (link1.linkIDHeight != 0)
					m_output.OutputRegLog(err_level::error, err_code::NgHeightEqual,
						ToString(link1.linkID), ToString(link2.linkID));
			}
			else
				m_output.OutputRegLog(err_level::error, err_code::NgHeightEqual,
					ToString(link1.linkID), ToString(link2.linkID));
		}
		else if (!isRelease) //( Release���[�h�F�`�F�b�N�ΏۊO )
		{
			// �Е��� 2 �܂��� -2�̎� ���΍����̍����J�������Ă��Ȃ���
			if (abs(link1.linkIDHeight) == 2 || abs(link2.linkIDHeight) == 2)
				if (abs(link1.linkIDHeight - link2.linkIDHeight) > 1) // ���G���[ ���΍����̍����傫��
					m_output.OutputRegLog(err_level::error, err_code::LargeDifference,
						ToString(link1.linkIDHeight), ToString(link2.linkIDHeight));
		}

		// HEIGHT_NODE�t�߂ɓ��H�m�[�h�A�܂��͕��s�҃m�[�h�����݂��邩
		set<long> nearWalkNodes = util::getIntersectsIDs(heightNodeObj.GetBox(m_dist_threshold), walkNodeRTree);
		set<long> nearRoadNodes = util::getIntersectsIDs(heightNodeObj.GetBox(m_dist_threshold), roadNodeRTree);
		if (!nearWalkNodes.empty() || !nearRoadNodes.empty()) // ���G���[ �|�C���g�t�߂Ƀm�[�h�����݂���
		{
			for (const auto& walkNodeID : nearWalkNodes)
			{
				m_output.OutputRegLog(err_level::error, err_code::NearNoramlNode,
					uh::str_util::ToString(walkNodeID), sindy::schema::walk_node::kTableName);
			}
			for (const auto& roadNodeID : nearRoadNodes)
			{
				m_output.OutputRegLog(err_level::error, err_code::NearNoramlNode,
					uh::str_util::ToString(roadNodeID), sindy::schema::road_node::kTableName);
			}
		}

		// HEIGHT_NODE�t�߂ɕʂ�HEIGHT_NODE�����݂��邩
		set<long> nearHeightNodes = util::getIntersectsIDs(heightNodeObj.GetBox(m_dist_threshold), heightNodeRTree);
		if (nearHeightNodes.size() > 1) // ���G���[ �|�C���g�t�߂ɕʂ�HEIGT_NODE�����݂���
		{
			for (auto& heightNodeID : nearHeightNodes)
			{
				if (objectId == heightNodeID)
					continue;

				m_output.OutputRegLog(err_level::error, err_code::NearHeightNode,
					uh::str_util::ToString(heightNodeID));
			}
		}

		// �����N�P�ʂł̃`�F�b�N
		for (const auto& link : heightNodeInfo)
		{
			if (!isRelease)
			{
				if (!judge_value::IsDefineHeight(link.linkIDHeight)) // ���G���[ ���΍������͈͊O
					m_output.OutputRegLog(err_level::error, err_code::NgHeight, ToString(link.linkIDHeight));
			}

			if (!judge_value::IsDefineLinkTable(link.linkIDTable)) // ���G���[ �����N�e�[�u������`�O
				m_output.OutputRegLog(err_level::error, err_code::NgLinkTable1, ToString(link.linkIDTable));

			if (!judge_value::IsOKLinkTable(link.linkIDTable)) // ���G���[ �����N�e�[�u���������ΏۊO
				m_output.OutputRegLog(err_level::error, err_code::NgLinkTable2, ToString(link.linkIDTable));


			GeoClass geoClass;
			// �����N�̑��݃`�F�b�N
			if (link.linkIDTable == sindy::schema::height_node::link_table::kWalkLink)
			{
				auto linkIte = walkLink.m_FeatureMap.find(link.linkID);
				if (linkIte == walkLink.m_FeatureMap.end()) // ���G���[ �����NID�������N�f�[�^�ɑ��݂��Ȃ�
				{
					m_output.OutputRegLog(err_level::error, err_code::NoExistLink,
						ToString(link.linkID), ToString(link.linkIDTable));
					continue;
				}
				geoClass = linkIte->second;

				// ���s�҃����N�̏ꍇ�A���s�Ҏ�ʃR�[�h���`�F�b�N(Release���[�h�F�`�F�b�N�ΏۊO)
				if (!isRelease)
				{
					long walkClass = geoClass.GetLongValue(walkClassIndex);
					if (!judge_value::IsOKHeightNodeWalkCode(walkClass, m_dataType)) // ���G���[ ���s�Ҏ�ʂ����΍����t�^�ΏۊO
					{
						// �ėp�����ꍇ�A���O�o��
						if (m_dataType == walk_type::type_class::kGeneral)
							m_output.OutputRegLog(err_level::error, err_code::NgHNWalkClass,
								ToString(link.linkID), ToString(link.linkIDTable));
						// �_�E���R���o�[�g�����ꍇ�A���O�o��
						if (m_dataType == walk_type::type_class::kDownConvert)
							m_output.OutputRegLog(err_level::error, err_code::NgHNWalkClass,
								ToString(link.linkID), ToString(link.linkIDTable), 0.0, 0.0, false);
					}
					else
					{
						int iniValue = m_heightNodeMgr.getWalkHeight(walkClass);
						if (!judge_value::IsOKCompareHeightRef(link.linkIDHeight, iniValue))
							// ���G���[ ���s�Ҏ�ʂɑ΂��鑊�΍������͈͊O
							m_output.OutputRegLog(err_level::warning, err_code::NgHeightWalk,
								ToString(link.linkID), ToString(link.linkIDTable));
					}
				}
			}
			else if (link.linkIDTable == sindy::schema::height_node::link_table::kRoadLink)
			{
				auto linkIte = roadLink.m_FeatureMap.find(link.linkID);
				if (linkIte == roadLink.m_FeatureMap.end()) // ���G���[ �����NID�������N�f�[�^�ɑ��݂��Ȃ�
				{
					m_output.OutputRegLog(err_level::error, err_code::NoExistLink,
						ToString(link.linkID), ToString(link.linkIDTable));
					continue;
				}
				geoClass = linkIte->second;

				// ���H�����N�̏ꍇ�A�����N�����������N���`�F�b�N(Release���[�h�F�`�F�b�N�ΏۊO)
				if (!isRelease)
				{
					set<long> segmentAttrs = util::getIntersectsIDs(heightNodeObj.GetBox(0.01), segmentAttrRTree);
					if (!segmentAttrs.empty())
					{
						for (long seg_objID : segmentAttrs)
						{
							auto& segAttr = segmentAttr.m_FeatureMap.at(seg_objID);
							if (link.linkID != segAttr.GetLongValue(segmentLinkIDIndex))
								continue;

							long linkAttr = segAttr.GetLongValue(segmentLinkAttrIndex);
							int iniValue = m_heightNodeMgr.getRoadHeight(linkAttr);
							if (!judge_value::IsOKCompareHeightRef(link.linkIDHeight, iniValue))
								// ���G���[ �����N�������ɑ΂��鑊�΍������͈͊O
								m_output.OutputRegLog(err_level::warning, err_code::NgHeightRoad,
									ToString(link.linkID), ToString(link.linkIDTable));
						}
					}
				}
			}
			else
				continue;

			// ���H�����N�A�܂��͕��s�҃����N���HEIGHT_NODE���|�C���g����Ă��邩
			if (geoClass.m_ipGeometry)
			{
				if (AheIsDisjoint(geoClass.m_ipGeometry, heightNodeObj.m_ipGeometry))
				{
					// ���G���[ �|�C���g�������N��ɂȂ�
					m_output.OutputRegLog(err_level::error, err_code::NoExistPointOnLink,
						ToString(link.linkID), ToString(link.linkIDTable));
				}
			}
			else
			{
				m_output.OutputRegLog(err_level::error, err_code::GetRoadFail);
			}

			// HEIGHT_NODE�t�߂Ƀ����N�̍\���_�����݂���B
			vector<IPointPtr> segmentPoints;
			if (!util::getLine2Points(geoClass.m_ipGeometry, segmentPoints))
				m_output.OutputRegLog(err_level::error, err_code::GetRoadFail);
			else {
				// ���[�ȊO�̍\���_�����݂���ꍇ�i�m�[�h�͕ʂ̉ӏ��Ń`�F�b�N���Ă��邽�߁j
				if (segmentPoints.size() > 2)
				{
					for (auto& segmentPoint : segmentPoints)
					{
						double dist = util::GetDistGeometries(heightNodeObj.m_ipGeometry, segmentPoint);
						if (0 < dist && dist < m_dist_threshold)
							// ���G���[ �|�C���g�t�߂Ƀ����NID1�܂��̓����NID2�̍\���_�����݂���
							m_output.OutputRegLog(err_level::error, err_code::NearSegment,
								ToString(link.linkID), ToString(link.linkIDTable));
					}
				}
			}
		}

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ���G���[ �����NID���s���Ȓl
		checkLinkID(link1.linkID, link2.linkID);

		if(checktarget == check_target::all || checktarget == check_target::release)
		{
			// ���G���[ �|�C���g���mHEIGHT_NODE�̋������߂�
			checkNearPoint( heightNodeObj, heightNode, heightNodeRTree );
		}
	}
}

// other���[�h�p�̑S�`�F�b�N
void HeightNodeCheckFunction::checkOther(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& heightNode
) const
{
	using namespace height_node;

	long linkID1Index = heightNode.m_FieldMap.FindField(kLinkID1);
	long linkID2Index = heightNode.m_FieldMap.FindField(kLinkID2);

	for (const auto& heightNodeFeature : heightNode.m_FeatureMap)
	{
		auto& heightNodeObj = heightNodeFeature.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (heightNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long link1ID = heightNodeObj.GetLongValue(linkID1Index);
		long link2ID = heightNodeObj.GetLongValue(linkID2Index);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ���G���[ �����NID���s���Ȓl
		checkLinkID(link1ID, link2ID);
	}
}

// HEIGHT_NODE�̌`�󂪕s�����`�F�b�N
void HeightNodeCheckFunction::checkHeightNodeGeometry(const ITablePtr& ipTable) const
{
	// �S���擾
	IFeatureCursorPtr ipCursor = ipTable->_Search(NULL, VARIANT_FALSE);
	if (!ipCursor) return;

	IFeaturePtr ipFeature;
	// �I�u�W�F�N�g���Ƃ̏���
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature) {
		IGeometryPtr ipGeometry = ipFeature->GetShape();
		if( !ipGeometry)
		{
			// ���G���[ �`�󂪑��݂��Ȃ�
			m_output.OutputRegLog( err_level::fatal, err_code::NgHeightNodeGeometry);
			return;
		}

		esriGeometryType eType = ipGeometry->GetGeometryType();
		if( eType != esriGeometryPoint )
		{
			// ���G���[ �|�C���g�^�ł͂Ȃ�
			m_output.OutputRegLog( err_level::fatal, err_code::NgHeightNodeGeometry);
			return;
		}
	}
}

// �����NID���s���Ȓl���`�F�b�N
void HeightNodeCheckFunction::checkLinkID(
	long  link1ID,
	long  link2ID
) const
{
	if (link1ID <= 0 || link2ID <= 0)
		m_output.OutputRegLog(err_level::fatal, err_code::NgHeightNodeLinkID);
}

// ��O���[�h�p�̃`�F�b�N
void HeightNodeCheckFunction::checkException(
	const FeaturesPack& heightNode
)
{
	if (m_dataType != walk_type::type_class::kDownConvert) return;

	// ���G���[ HEIGHT_NODE�̌`�󂪕s��
	checkHeightNodeGeometry(heightNode.m_ipTable);
}

// �|�C���g���mHEIGHT_NODE�̋������߂����`�F�b�N
void HeightNodeCheckFunction::checkNearPoint(
	const GeoClass& heightNodeObject,
	const FeaturesPack& heightNode,
	const BGRTree& heightNodeRTree
) const
{
	std::vector<GeoClass> crossHeightNodes;
	if(util::findOverlapGeometry(heightNodeObject, heightNode, heightNodeRTree, crossHeightNodes, 0.01) <= 0) return;

	for( const auto& node : crossHeightNodes )
	{
		if(heightNodeObject.m_ObjectID == node.m_ObjectID || !heightNodeObject.IsEqual( node.m_ipGeometry ) ) continue;

		CString info;
		info.Format(_T("%s(%d)"), height_node::kTableName, node.m_ObjectID);
		m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
	}
}
