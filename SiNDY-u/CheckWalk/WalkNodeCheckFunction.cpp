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
#include "WalkNodeCheckFunction.h"
#include "value.h"

#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGeometryConvert.h>

namespace
{
	const int MAX_MESHX = 1000000;
	const int MIN_MESHX = 0;

	const int MAX_MESHY = 1000000;
	const int MIN_MESHY = 0;
} // namespace

extern crd_cnv g_cnv;

using namespace std;
using namespace sindy::schema;

void WalkNodeCheckFunction::CheckWalkNode(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const FeaturesPack& stationGate,
	const FeaturesPack& poiPoint,
	const FeaturesPack& scrambleArea,
	const FeaturesPack& heightNode,
	const BGRTree& linkRTree,
	const BGRTree& nodeRTree,
	const BGRTree& walkLinkRTree,
	const BGRTree& walkNodeRTree,
	const BGRTree& scrambleAreaRTree,
	const BGRTree& heightNodeRTree,
	const NODEIDMAP& walkNodeLinkIDs,
	const std::map<CString, IGeometryPtr>& baseMeshes,
	bool bOther)
{
	using namespace walk_node;
	using namespace ipc_feature;
	using namespace properties;

	CString info1, info2;

	// TODO: ���t�@�N�^�����O�őO�������������
	// �Y���̃R�[�h�����x�[�X���b�V�������݂��Ȃ��ꍇ�̓G���[�f���ׂ������m��Ȃ���
	// ����Ȃ��̂͑z�肵�Ă��Ȃ��̂ŃT�|�[�g���Ȃ��B
	IPolylinePtr basePolyline;
	{
		CString basemeshcode = meshcode.Left(meshcode.GetLength() - 2);
		auto it = baseMeshes.find(basemeshcode);
		IGeometryPtr baseMeshGeometry;
		if (baseMeshes.end() != it) baseMeshGeometry = it->second;
		basePolyline = ToPolyline(baseMeshGeometry);
	}
	m_output.SetLayer(walk_node::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_node::kTableName);

	// ���s�҃m�[�h�̃`�F�b�N
	long  walkNodeClassIndex = walkNode.m_FieldMap.FindField(kNodeClass);
	long  roadNodeIDIndex = walkNode.m_FieldMap.FindField(kRoadNodeID);

	long modOperatorIndex = walkNode.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkNode.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkNode.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkNode.m_FieldMap.FindField(kProgModifyDate);
	long nodeNameIndex = roadNode.m_FieldMap.FindField(road_node::kNameKanji);
	long walkNodeRoadIndex = walkNode.m_FieldMap.FindField(kRoadNodeID);
	CheckType checkType = bOther ? CheckType::Other : CheckType::All;
	long walkClassIndex = walkLink.m_FieldMap.FindField(walk_link::kWalkClass);
	long fromIdIndex = walkLink.m_FieldMap.FindField(walk_link::kFromNodeID);
	long toIdIndex = walkLink.m_FieldMap.FindField(walk_link::kToNodeID);

	for (const auto& walkNodePair : walkNode.m_FeatureMap)
	{
		auto& walkNodeObj = walkNodePair.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long  walkNodeID = walkNodePair.first;
		long  walkNodeClass = walkNodeObj.GetLongValue(walkNodeClassIndex);
		long  roadNodeID = walkNodeObj.GetLongValue(roadNodeIDIndex);

		m_output.SetModifyInfo(walkNodeID, walkNodeObj.GetStringValue(modOperatorIndex),
			walkNodeObj.GetStringValue(modDateIndex), walkNodeObj.GetStringValue(modProgramIndex),
			walkNodeObj.GetStringValue(modProgramDateIndex));
		// �e�����l����`�����m�F����
		if (!judge_value::IsDefineWalkNodeClass(walkNodeClass, m_dataType)) // ���G���[ ���s�҃m�[�h��ʃR�[�h����`�O
		{
			// �ėp�����ꍇ
			if (m_dataType == walk_type::type_class::kGeneral)
				m_output.OutputRegLog(err_level::error, err_code::NgWalkNodeClass, uh::str_util::ToString(walkNodeClass));
			// �_�E���R���o�[�g�����ꍇ
			if (m_dataType == walk_type::type_class::kDownConvert)
				m_output.OutputRegLog(err_level::fatal, err_code::NgWalkNodeClass);
		}


		// ����������`����g�����`�F�b�N

		// �P���Ȍ`��m�F
		IPointPtr ipWalkNodePoint(walkNodeObj.m_ipGeometry);
		double walkNodeX = 0.0, walkNodeY = 0.0;

		// TODO�@�`�󑶍݂��Ȃ��ꍇ�ɁA���̃\�[�X���W�b�N��肱�̃G���[�`�F�b�N�����ɓ���Ȃ��̂ŁA�G���[�R�[�h102�G���[���b�Z�[�W���G���[���O�t�@�C���ɏo���Ȃ��B
		// ���G���[ �`�󂪑��݂��Ȃ�
		checkNodeGeometry(walkNodeObj, walkNodeX, walkNodeY);
		// ���G���[ �}�s�̋��Ƀm�[�h������
		checkBorderCorner(walkNodeX, walkNodeY);

		// ���G���[ 2�����b�V�����E����ɂ���̂ɐ}�s��m�[�h�ɂȂ��Ă��Ȃ�
		// ���G���[ �}�s��m�[�h��2�����b�V�����E����ɖ���
		checkBorderLine(walkNodeObj, walkNodeClass, basePolyline);

		// ���G���[ ���H�m�[�h��ɂ���̂ɓ��HNW�ڑ��m�[�h�ɂȂ��Ă��Ȃ�
		checkRoadNWConnectNode(walkNodeObj, walkNodeClass, roadNode, nodeRTree);

		// �ڑ������N�Ɋւ��`�F�b�N�����{
		checkConenctLink(walkNodeObj, walkNode, walkLink, walkNodeClass, roadNodeID, walkClassIndex,
			walkNodeClassIndex, fromIdIndex, toIdIndex, walkNodeLinkIDs, roadLink, roadNode, linkRTree, nodeRTree, checkType);

		// ���G���[ ���s�҃����N�ɐڑ����Ă��Ȃ��i�`��j
		checkWalkLinkConnectNode(walkNodeObj, walkLink, walkLinkRTree);

		// ���G���[ �m�[�h���֌W�Ȃ������N�ɐڐG���Ă���
		checkTouchOtherLink(walkNodeObj, walkLink, walkLinkRTree, walkNodeClass, walkNodeLinkIDs);

		// ���G���[ �m�[�h���ʂ̃m�[�h�ɐڐG���Ă���
		checkTouchOtherNode(walkNodeObj, walkNode, walkNodeRTree, walkNodeX, walkNodeY);

		// ���H�m�[�hID�̃`�F�b�N
		if (roadNodeID != -1 && roadNodeID != 0) {
			compairRoadNodeCheck(walkNodeClass, roadNodeID, nodeNameIndex, walkNodeRoadIndex,
				walkNodeObj, roadNode, walkNode, walkNodeRTree, false);
		}

		// �w�o�����|�C���gID�̃`�F�b�N
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kStationGatePointID, stationGate, false);
		// �n���X�o�����|�C���gID�̃`�F�b�N
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kUndergroundGatePointID, poiPoint, false);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ���G���[ ���H�����N��ɂ���̂ɓ��H�����N�ڑ��m�[�h�ɂȂ��Ă��Ȃ�
		checkRoadLinkConnectNode(walkNodeObj, walkNodeClass, roadLink, linkRTree);

		// ���G���[ ���H�����N�ڑ��m�[�h�����H�����N�����ӏ��ɂ���
		checkRoadLinkCrossNode(walkNodeObj, walkNodeClass, roadLink, linkRTree, roadNode.m_FeatureMap, nodeRTree);

		// ���G���[ �X�N�����u�������_�Ȃ̂ɃX�N�����u���G���A�|���S��������
		checkWalkNodeCrossWalkLink(walkNodeObj, walkLink, walkLinkRTree, scrambleArea, scrambleAreaRTree);

		if(checkType == check_target::all && walkNodeClass == node_class::kConnectedRoad)
		{
			// ���G���[ �|�C���g���m�iWALK_NODE�AHEIGHT_NODE�j�̋������߂�
			checkNearPoint( walkNodeObj, walkNode, heightNode, walkNodeRTree, heightNodeRTree );
		}
		
		// ���G���[ ���W�J���R�l�N�V�����������ڑ����Ă���
		if (walkNodeClass != node_class::kConnectedRoad && walkNodeClass != node_class::kRoadNW)
			checkConnectWalkLink(walkNodeObj, walkLink, walkLinkRTree);

		// ���W�J���R�l�N�V�������s�p�ɐڑ����Ă��邩�`�F�b�N
		checkConnectWalkLinkAngle(walkNodeObj, walkLink, walkLinkRTree);
	}
}

void WalkNodeCheckFunction::checkWalkable(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const FeaturesPack& walkNode,
	const BGRTree& linkRTree,
	const BGRTree& nodeRTree,
	const NODEIDMAP& walkNodeLinkIDs
)
{
	using namespace walk_node;
	using namespace ipc_feature;

	m_output.SetLayer(walk_node::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_node::kTableName);

	long  walkNodeClassIndex = walkNode.m_FieldMap.FindField(kNodeClass);
	long modOperatorIndex = walkNode.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkNode.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkNode.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkNode.m_FieldMap.FindField(kProgModifyDate);
	for (const auto& walkNodePair : walkNode.m_FeatureMap)
	{

		long  walkNodeID = walkNodePair.first;
		auto& walkNodeObj = walkNodePair.second;
		long  walkNodeClass = walkNodeObj.GetLongValue(walkNodeClassIndex);
		m_output.SetModifyInfo(walkNodeID, walkNodeObj.GetStringValue(modOperatorIndex),
			walkNodeObj.GetStringValue(modDateIndex), walkNodeObj.GetStringValue(modProgramIndex),
			walkNodeObj.GetStringValue(modProgramDateIndex));
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		// �`�F�b�N�Ώۂ͓��HNW�ڑ��m�[�h�̂�
		if (walkNodeClass != walk_node::node_class::kRoadNW)
			continue;
		// �_�E���R���o�[�g�����ꍇ�A���H�m�[�h�ڑ��m�[�h�������͓��H�����N�ڑ��m�[�h��ΏۂƂ���B
		if (m_dataType == walk_type::type_class::kDownConvert && walkNodeClass != walk_node::node_class::kConnectedRoad && walkNodeClass != walk_node::node_class::kRoadNW)	continue;

		// �P���Ȍ`��m�F
		double walkNodeX = 0.0, walkNodeY = 0.0;
		if (isIllegalShape(walkNodeObj.m_ipGeometry, walkNodeX, walkNodeY))
			continue;

		// �ڑ������N�����擾
		long connectWalkLinkCnt = -1;
		auto walkNodeLinkItr = walkNodeLinkIDs.find(walkNodeID);
		if (walkNodeLinkItr != walkNodeLinkIDs.end())
			connectWalkLinkCnt = walkNodeLinkItr->second.size();

		if (connectWalkLinkCnt < 0) continue;

		// ���s�҃m�[�h��̓��H�m�[�h������
		GeoClass roadNodeObj;
		// �����ł��Ȃ��Ȃ�{�`�F�b�N�ł͑ΏۊO
		if (!findOverlapRoadNode(roadNodeObj, walkNodeObj, roadNode.m_FeatureMap, nodeRTree))
			continue;

		if (checkWalkableConnect(roadNodeObj, connectWalkLinkCnt, roadNode.m_FieldMap, roadLink, linkRTree))
		{
			// �����[�j���O �ڑ����铹�H�����N�����s�֎~�̃����N�̂�
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::error, err_code::ConnectCarOnly);
		}
	}
}

void WalkNodeCheckFunction::checkRelease(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const FeaturesPack& stationGate,
	const FeaturesPack& poiPoint,
	const FeaturesPack& heightNode,
	const BGRTree& linkRTree,
	const BGRTree& nodeRTree,
	const BGRTree& walkLinkRTree,
	const BGRTree& walkNodeRTree,
	const BGRTree& heightNodeRTree,
	const NODEIDMAP& walkNodeLinkIDs,
	const std::map<CString, IGeometryPtr>& baseMeshes
)
{
	using namespace walk_node;
	using namespace ipc_feature;
	CString info1, info2;

	// TODO: ���t�@�N�^�����O�őO�������������
	// �Y���̃R�[�h�����x�[�X���b�V�������݂��Ȃ��ꍇ�̓G���[�f���ׂ������m��Ȃ���
	// ����Ȃ��̂͑z�肵�Ă��Ȃ��̂ŃT�|�[�g���Ȃ��B
	IPolylinePtr basePolyline;
	{
		CString basemeshcode = meshcode.Left(meshcode.GetLength() - 2);
		auto it = baseMeshes.find(basemeshcode);
		IGeometryPtr baseMeshGeometry;
		if (baseMeshes.end() != it) baseMeshGeometry = it->second;
		basePolyline = ToPolyline(baseMeshGeometry);
	}
	m_output.SetLayer(walk_node::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_node::kTableName);

	// ���s�҃m�[�h�̃`�F�b�N
	long  walkNodeClassIndex = walkNode.m_FieldMap.FindField(kNodeClass);
	long  roadNodeIDIndex = walkNode.m_FieldMap.FindField(kRoadNodeID);

	long modOperatorIndex = walkNode.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkNode.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkNode.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkNode.m_FieldMap.FindField(kProgModifyDate);
	long nodeNameIndex = roadNode.m_FieldMap.FindField(road_node::kNameKanji);
	long walkClassIndex = walkLink.m_FieldMap.FindField(walk_link::kWalkClass);
	long fromIdIndex = walkLink.m_FieldMap.FindField(walk_link::kFromNodeID);
	long toIdIndex = walkLink.m_FieldMap.FindField(walk_link::kToNodeID);
	for (const auto& walkNodePair : walkNode.m_FeatureMap)
	{
		auto& walkNodeObj = walkNodePair.second;
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long  walkNodeID = walkNodePair.first;
		long  walkNodeClass = walkNodeObj.GetLongValue(walkNodeClassIndex);
		long  roadNodeID = walkNodeObj.GetLongValue(roadNodeIDIndex);

		m_output.SetModifyInfo(walkNodeID, walkNodeObj.GetStringValue(modOperatorIndex),
			walkNodeObj.GetStringValue(modDateIndex), walkNodeObj.GetStringValue(modProgramIndex),
			walkNodeObj.GetStringValue(modProgramDateIndex));

		double walkNodeX = 0.0, walkNodeY = 0.0;

		// ���G���[ �`�󂪑��݂��Ȃ�
		checkNodeGeometry(walkNodeObj, walkNodeX, walkNodeY);
		// ���G���[ �}�s�̋��Ƀm�[�h������
		checkBorderCorner(walkNodeX, walkNodeY);

		// ���G���[ 2�����b�V�����E����ɂ���̂ɐ}�s��m�[�h�ɂȂ��Ă��Ȃ�
		// ���G���[ �}�s��m�[�h��2�����b�V�����E����ɖ���
		checkBorderLine(walkNodeObj, walkNodeClass, basePolyline);

		// ���G���[ ���H�m�[�h��ɂ���̂ɓ��HNW�ڑ��m�[�h�ɂȂ��Ă��Ȃ�
		checkRoadNWConnectNode(walkNodeObj, walkNodeClass, roadNode, nodeRTree);


		// �ڑ������N�Ɋւ��`�F�b�N�����{
		checkConenctLink(walkNodeObj, walkNode, walkLink, walkNodeClass, roadNodeID, walkClassIndex,
			walkNodeClassIndex, fromIdIndex, toIdIndex, walkNodeLinkIDs, roadLink, roadNode, linkRTree, nodeRTree, CheckType::Release);

		// ���G���[ ���s�҃����N�ɐڑ����Ă��Ȃ��i�`��j
		checkWalkLinkConnectNode(walkNodeObj, walkLink, walkLinkRTree);

		// ���G���[ �m�[�h���֌W�Ȃ������N�ɐڐG���Ă���
		checkTouchOtherLink(walkNodeObj, walkLink, walkLinkRTree, walkNodeClass, walkNodeLinkIDs);

		// ���G���[ �m�[�h���ʂ̃m�[�h�ɐڐG���Ă���
		checkTouchOtherNode(walkNodeObj, walkNode, walkNodeRTree, walkNodeX, walkNodeY);

		// ���H�m�[�hID�̃`�F�b�N
		// TODO: ���L�̏�����CheckWalkNode�Ƃ܂Ƃ߂���ӏ����܂Ƃ߂�1
		if (roadNodeID != -1 && roadNodeID != 0) {
			// walkNodeRoadIndex�͎g���Ȃ��̂�-1�AwalkNodeRTree���g���Ȃ����ǁA�n����̂œn��
			compairRoadNodeCheck(walkNodeClass, roadNodeID, nodeNameIndex, -1,
				walkNodeObj, roadNode, walkNode, walkNodeRTree, true);
		}

		// �w�o�����|�C���gID�̃`�F�b�N
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kStationGatePointID, stationGate, true);
		// �n���X�o�����|�C���gID�̃`�F�b�N
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kUndergroundGatePointID, poiPoint, true);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// �����H�����N�ڑ��m�[�h�����H�����N�����ӏ��ɂ���
		checkRoadLinkCrossNode(walkNodeObj, walkNodeClass, roadLink, linkRTree, roadNode.m_FeatureMap, nodeRTree);

		
		if(walkNodeClass == node_class::kConnectedRoad)
		{
			// ���G���[ �|�C���g���m�iWALK_NODE�AHEIGHT_NODE�j�̋������߂�
			checkNearPoint( walkNodeObj, walkNode, heightNode, walkNodeRTree, heightNodeRTree );
		}
	}
}


bool WalkNodeCheckFunction::isIllegalShape(const IGeometryPtr& targetNode, double& walkNodeX, double& walkNodeY)
{
	esriGeometryType eType = targetNode->GetGeometryType();
	IPointPtr ipWalkNodePoint(targetNode);
	if (!targetNode
		|| !ipWalkNodePoint
		|| FAILED(ipWalkNodePoint->QueryCoords(&walkNodeX, &walkNodeY))
		|| eType != esriGeometryPoint)
		return true;
	return false;
}

void WalkNodeCheckFunction::checkNodeGeometry(
	const GeoClass& walkNodeObj, double& walkNodeX, double& walkNodeY)
{
	using namespace walk_node;
	if (!isIllegalShape(walkNodeObj.m_ipGeometry, walkNodeX, walkNodeY)) return;

	// ���G���[ �`�󂪑��݂��Ȃ�
	m_output.OutputRegLog(err_level::error, err_code::NgWalkNodeGeom);
}

void WalkNodeCheckFunction::checkBorderCorner(
	double walkNodeX, double walkNodeY)
{
	using namespace walk_node;
	int iMeshCode = 0, iX = 0, iY = 0;
	g_cnv.LLtoMesh(walkNodeX, walkNodeY, 2, &iMeshCode, &iX, &iY, 1);
	if ((iX == MIN_MESHX && (iY == MIN_MESHY || iY == MAX_MESHY))
		|| (iX == MAX_MESHX && (iY == MIN_MESHY || iY == MAX_MESHY)))
	{

		// ���G���[ �}�s�̋��Ƀm�[�h������
		m_output.OutputRegLog(err_level::error, err_code::BorderCornerNode);
	}
}
void WalkNodeCheckFunction::checkBorderLine(
	const GeoClass& walkNodeObj, long walkNodeClass, const IPolylinePtr& basePolyline)
{
	using namespace walk_node;
	// ���W����m�[�h��ʂ̊m�F
	if (AheIsContain4(basePolyline, walkNodeObj.m_ipGeometry))
	{
		// ���G���[ 2�����b�V�����E����ɂ���̂ɐ}�s��m�[�h�ɂȂ��Ă��Ȃ�
		if (walkNodeClass != walk_node::node_class::kOutline)
		{
			m_output.OutputRegLog(err_level::error, err_code::NgBorderNodeAttr,
				judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap));
		}
	}
	else
	{
		// ���G���[ �}�s��m�[�h��2�����b�V�����E����ɖ���
		if (walkNodeClass == walk_node::node_class::kOutline)
		{
			m_output.OutputRegLog(err_level::error, err_code::NgBorderNodeGeom);
		}
	}
}

void WalkNodeCheckFunction::checkRoadNWConnectNode(
	const GeoClass& walkNodeObj,
	long walkNodeClass,
	const FeaturesPack& roadNode,
	const BGRTree& roadNodeRTree)
{
	using namespace walk_node;
	// ���H�m�[�h�ォ�H
	bool bOnRoadNode = false;
	auto onNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(0.1), roadNodeRTree);
	for (const auto& onNodeID : onNodeIDs)
	{
		auto onNodeItr = roadNode.m_FeatureMap.find(onNodeID);
		if (onNodeItr == roadNode.m_FeatureMap.end())
			continue;

		// ��v���邩�H
		if (walkNodeObj.IsEqual(onNodeItr->second.m_ipGeometry)) {
			bOnRoadNode = true;
			break;
		}
	}

	// ��ԂƑ�������v���邩�m�F
	if (!bOnRoadNode) return;
	// ���H�m�[�h�ぁ���HNW�ڑ��m�[�h���ǂ���(�}�s��m�[�h�͑ΏۊO)
	if (walkNodeClass != walk_node::node_class::kRoadNW
		&& walkNodeClass != walk_node::node_class::kOutline) // ���G���[ ���H�m�[�h��ɂ���̂ɓ��HNW�ڑ��m�[�h�ɂȂ��Ă��Ȃ�
		m_output.OutputRegLog(err_level::error, err_code::NgOnRoadNode1,
			judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap));
}

void WalkNodeCheckFunction::checkWalkLinkConnectNode(
	const GeoClass& walkNodeObj,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree)
{
	// �ڑ������N������
	bool bFindLink = false;
	auto findWalkLinkIDs = util::getIntersectsIDs(walkNodeObj.GetBox(0.1), walkLinkRTree);

	// �����������s�҃����N�̃��[�v
	for (const auto& findWalkLinkID : findWalkLinkIDs) {

		auto findWalkLinkItr = walkLink.m_FeatureMap.find(findWalkLinkID);
		if (findWalkLinkItr == walkLink.m_FeatureMap.end())
			continue;

		// From/To�̂ǂ��炩�ƈ�v���邩�m�F
		IPolylinePtr ipFindWalkLinkLine(findWalkLinkItr->second.m_ipGeometry);
		IPointPtr ipFindFromPoint = ipFindWalkLinkLine->GetFromPoint();
		IPointPtr ipFindToPoint = ipFindWalkLinkLine->GetToPoint();
		if (ipFindFromPoint) {
			if (walkNodeObj.IsEqual(ipFindFromPoint)) {
				bFindLink = true;
				break;
			}
		}
		if (ipFindToPoint) {
			if (walkNodeObj.IsEqual(ipFindToPoint)) {
				bFindLink = true;
				break;
			}
		}
	}
	if (bFindLink) return;

	using namespace walk_node;
	// ���G���[ ���s�҃����N�ɐڑ����Ă��Ȃ��i�`��j
	m_output.OutputRegLog(err_level::error, err_code::NoConnect, _T("�`��"));
}

// TODO: �����[�X�`�F�b�N�Ƌ��ʉ��������ǁA�X�ɍו����������Ƃ���
void WalkNodeCheckFunction::checkConenctLink(
	const GeoClass& walkNodeObj,
	const FeaturesPack& walkNode,
	const FeaturesPack& walkLink,
	long walkNodeClass,
	long roadNodeID,
	long walkClassIndex,
	long walkNodeClassIndex,
	long fromNodeIdIndex,
	long toNodeIdIndex,
	const NODEIDMAP& walkNodeLinkIDs,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const BGRTree& roadLinkRTree,
	const BGRTree& roadNodeRTree,
	CheckType checkType)
{

	// �ڑ������N�����擾
	long connectWalkLinkCnt = -1;
	auto walkNodeLinkItr = walkNodeLinkIDs.find(walkNodeObj.m_ObjectID);
	if (walkNodeLinkItr != walkNodeLinkIDs.end())
		connectWalkLinkCnt = walkNodeLinkItr->second.size();


	// �ڑ������N����1�̏ꍇ�A��Ԃɂ���đ����̈�v���m�F����
	if (connectWalkLinkCnt <= 0)
	{
		// ���G���[ ���s�҃����N�ɐڑ����Ă��Ȃ��i�����j
		m_output.OutputRegLog(err_level::error, err_code::NoConnect, _T("����"));
	}
	else if (CheckType::Release != checkType) {
		if (connectWalkLinkCnt == 1)
		{
			// (�ڑ������N����1)�[�_�ɂȂ肤��m�[�h���ǂ���
			if (!judge_value::IsOKTerminalNode(walkNodeClass))
			{
				// ���G���[ �[�_�Ȃ̂ɒ[�_�m�[�h�ɂȂ��Ă��Ȃ�
				m_output.OutputRegLog(err_level::error, err_code::NgTerminalNode1,
					judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap));
			}
		}
		else // �ڑ������N����1�ȊO�̏ꍇ�A�[�_�ɂȂ��Ă��Ȃ����m�F����
		{
			if (walkNodeClass == walk_node::node_class::kTerminal)
			{
				// ���G���[ �ڑ������N�������Ȃ̂ɁA�[�_�m�[�h
				m_output.OutputRegLog(err_level::error, err_code::NgTerminalNode2);
			}
		}
	}

	// �[�_�̏ꍇ�A�ڑ������N���[�_�ɐڑ����ėǂ��������m�F����
	if (CheckType::Release != checkType && connectWalkLinkCnt != -1 && walkNodeClass == walk_node::node_class::kTerminal)
	{
		for (const auto& connectWalkLinkID : walkNodeLinkItr->second) {

			// �ڑ������N�擾
			auto connectWalkLinkItr = walkLink.m_FeatureMap.find(connectWalkLinkID);
			if (connectWalkLinkItr == walkLink.m_FeatureMap.end())
				continue;

			// ���s�Ҏ�ʃR�[�h���m�F
			long connectWalkLinkClass = connectWalkLinkItr->second.GetLongValue(walkLink.m_FieldMap.FindField(walk_link::kWalkClass));
			if (!judge_value::IsOKTerminalLink(connectWalkLinkClass, m_dataType)) {
				// ���G���[ �[�_�m�[�h�ɐڑ����Ă͂����Ȃ����s�҃����N���
				CString info1, info2;
				info1.Format(_T("WalkLinkID:'%ld'"), connectWalkLinkID);
				info2 = judge_value::IsFindObj(connectWalkLinkClass, m_walkLinkClassMap);
				m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::warning, err_code::NgTerminalLink, info1, info2);
			}
		}
	}

	// ���HNW�ڑ��m�[�h�̏ꍇ
	bool isConnectNode(false);
	if (m_dataType == walk_type::type_class::kGeneral)
		isConnectNode = walkNodeClass == walk_node::node_class::kRoadNW ? true : false;
	if (m_dataType == walk_type::type_class::kDownConvert)
		isConnectNode = ((walkNodeClass == walk_node::node_class::kRoadNW) || (walkNodeClass == walk_node::node_class::kConnectedRoad)) ? true : false;
	if (isConnectNode)
	{
		// ���s�҃m�[�h��̓��H�m�[�h������
		GeoClass roadNodeObj;
		if (!findOverlapRoadNode(roadNodeObj, walkNodeObj, roadNode.m_FeatureMap, roadNodeRTree))
		{
			// ���G���[ ���HNW�ڑ��m�[�h�����H�m�[�h��ɖ���
			m_output.OutputRegLog(err_level::error, err_code::NgOnRoadNode2);
		}
		else if (CheckType::All == checkType && checkWalkableConnect(roadNodeObj, connectWalkLinkCnt, roadNode.m_FieldMap, roadLink, roadLinkRTree))
		{
			// �ėp�����ꍇ �����[�j���O �ڑ����铹�H�����N�����s�֎~�̃����N�̂�;�_�E���R���o�[�g�����ꍇ ���G���[
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::error, err_code::ConnectCarOnly);
		}
	}
	// ���HNW�ڑ��m�[�h�ȊO�̏ꍇ
	else
	{
		bool checkFlag = true;
		if (connectWalkLinkCnt != -1)
		{
			for (const auto& connectWalkLinkID : walkNodeLinkItr->second) {
				// �ڑ������N�擾
				auto connectWalkLinkItr = walkLink.m_FeatureMap.find(connectWalkLinkID);
				if (connectWalkLinkItr == walkLink.m_FeatureMap.end())
					continue;

				// ���s�Ҏ�ʃR�[�h���m�F
				long connectWalkLinkClass = connectWalkLinkItr->second.GetLongValue(walkClassIndex);
				if (connectWalkLinkClass == walk_link::walk_class::kWalkableRoad ||
					connectWalkLinkClass == walk_link::walk_class::kConnect)
				{
					checkFlag = false;
					break;
				}
			}
			if (checkFlag)
			{
				auto roadLinkIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_dist_threshold), roadLinkRTree);
				for (auto& roadLinkID : roadLinkIDs)
				{
					GeoClass roadLinkObj = roadLink.m_FeatureMap.at(roadLinkID);
					if (!roadLinkObj.IsDisjoint(walkNodeObj.m_ipGeometry))// ���G���[ ���s�҃m�[�h�����H�����N�ɏ���Ă���
						m_output.OutputRegLog(err_level::error, err_code::NgOnRoadLink, uh::str_util::ToString(roadLinkObj.m_ObjectID));
				}
			}
		}
	}

	// ���H�m�[�h�Ή�ID�����ꍇ
	if (CheckType::Release != checkType && roadNodeID > -1 && connectWalkLinkCnt > 0) {

		bool bConnectCrossRoad = false;
		for (const auto& connectWalkLinkID : walkNodeLinkItr->second) {

			// �ڑ������N�擾
			auto connectWalkLinkItr = walkLink.m_FeatureMap.find(connectWalkLinkID);
			if (connectWalkLinkItr == walkLink.m_FeatureMap.end())
				continue;

			// ���s�Ҏ�ʃR�[�h�����f�����ł��邩���m�F
			long connectWalkLinkClass = connectWalkLinkItr->second.GetLongValue(walkClassIndex);
			if (connectWalkLinkClass == walk_link::walk_class::kCrossRoad) {
				bConnectCrossRoad = true;
				break;
			}
		}
		if (!bConnectCrossRoad) // ���G���[ ���H�m�[�h�Ή�ID�����̂ɉ��f���������N�ɐڑ����Ă��Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::NotConnectCross);
	}
	// bug 12687 - [u][���s��]MV�������D���f�[�^�̃`�F�b�N�ǉ��iCheckWalk.exe�j�A
	if (judge_value::IsTicketGate(walkNodeClass) && connectWalkLinkCnt > 0) {
		internalTicketGateConenctCheck(walkLink, walkNode, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex,
			toNodeIdIndex, walkNodeLinkItr->second, walkNodeLinkIDs);
	}

	// ���H�����N�ڑ��m�[�h�̏ꍇ
	if ((CheckType::All == checkType || CheckType::Other == checkType) && walkNodeClass == walk_node::node_class::kConnectedRoad)
	{
		// ���s�҃m�[�h��̓��H�����N������
		std::vector<GeoClass> crossRoadLinks;
		if (util::findOverlapGeometry(walkNodeObj, roadLink, roadLinkRTree, crossRoadLinks) <= 0)
		{
			// ���G���[ ���H�����N�ڑ��m�[�h�����H�����N��ɖ���
			m_output.OutputRegLog(err_level::error, err_code::NgNotOnRoadLink);
		}
	}
}

void WalkNodeCheckFunction::checkTouchOtherLink(
	const GeoClass& walkNodeObj,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree,
	long walkNodeClass,
	const NODEIDMAP& walkNodeLinkIDs)
{
	// �[�_�m�[�h�̂݃`�F�b�N����
	if (walkNodeClass != walk_node::node_class::kTerminal) return;
	using namespace walk_node;

	// ���s�҃m�[�h��XY�擾
	double x = 0.0, y = 0.0;
	IPointPtr ipWalkNodePoint(walkNodeObj.m_ipGeometry);
	ipWalkNodePoint->QueryCoords(&x, &y);

	// �ڑ������Nset���擾����
	auto connectLinkItr = walkNodeLinkIDs.find(walkNodeObj.m_ObjectID);

	// �[�_�̏ꍇ�A�ߖT�ɐڑ����Ă��Ȃ������N���������m�F�i����臒l�ōL���Č����j
	auto findWalkLinkIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_dist_threshold), walkLinkRTree);

	// �����������s�҃����N�̃��[�v
	for (const auto& findWalkLinkID : findWalkLinkIDs)
	{
		auto findWalkLinkItr = walkLink.m_FeatureMap.find(findWalkLinkID);
		if (findWalkLinkItr == walkLink.m_FeatureMap.end())
			continue;

		// �ڑ������Nset�ɂ���΃X�L�b�v
		if (connectLinkItr != walkNodeLinkIDs.end()
			&& connectLinkItr->second.find(findWalkLinkID) != connectLinkItr->second.end())
			continue;

		// �ŋߖT�|�C���g�擾
		IProximityOperatorPtr ipProximity(findWalkLinkItr->second.m_ipGeometry);
		IPointPtr ipNearPoint = ipProximity->_ReturnNearestPoint(ipWalkNodePoint, esriNoExtension);
		if (!ipNearPoint)
			continue;

		// �_�E���R���o�[�g�����A���W�J���R�l�N�V�����̕��s�҃����N�ꍇ�X�L�b�v
		if (m_dataType == walk_type::type_class::kDownConvert)
		{
			long index = walkLink.m_FieldMap.FindField(walk_link::kWalkCode);
			if (index == -1) continue;
			if (findWalkLinkItr->second.GetLongValue(index) == walk_link::walk_class::kLogicalConnection)
				continue;
		}

		// �����𑪂�
		double nearX = 0.0, nearY = 0.0;
		ipNearPoint->QueryCoords(&nearX, &nearY);
		double dist = g_cnv.GetDist(x, y, nearX, nearY);

		// 臒l�𒴂��鋗���̏ꍇ�A�X�L�b�v�i�G���x���[�v�̂��߁A�΂߂�臒l�𒴂��邽�߁j
		if (dist > m_dist_threshold)
			continue;

		CString info1, info2;
		info1.Format(_T("WalkLinkID:'%ld'"), findWalkLinkID);
		info2.Format(_T("����:'%lf'm(臒l:'%lf'm)"), dist, m_dist_threshold);

		// ���G���[ �m�[�h���֌W�Ȃ������N�ɐڐG���Ă���
		m_output.OutputRegLog(err_level::error, err_code::TouchNgLink, info1, info2);
	}
}

void WalkNodeCheckFunction::checkTouchOtherNode(
	const GeoClass& walkNodeObj,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree,
	double walkNodeX,
	double walkNodeY)
{
	using namespace walk_node;
	// �ʂ̃m�[�h���߂��ɂȂ����m�F�i����臒l�ōL���Č����j
	auto findWalkNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_dist_threshold), walkNodeRTree);

	// �����������s�҃m�[�h�̃��[�v
	for (const auto& findWalkNodeID : findWalkNodeIDs)
	{
		// ���g�̓X�L�b�v
		if (findWalkNodeID == walkNodeObj.m_ObjectID)
			continue;

		auto findWalkNodeItr = walkNode.m_FeatureMap.find(findWalkNodeID);
		if (findWalkNodeItr == walkNode.m_FeatureMap.end())
			continue;

		// �����𑪂�
		IPointPtr ipPoint(findWalkNodeItr->second.m_ipGeometry);
		double findX = 0.0, findY = 0.0;
		ipPoint->QueryCoords(&findX, &findY);
		double dist = g_cnv.GetDist(findX, findY, walkNodeX, walkNodeY);

		if (dist < m_dist_threshold)
		{
			CString info1, info2;
			info1.Format(_T("WalkNodeID:'%ld'"), findWalkNodeID);
			info2.Format(_T("����:'%lf'm(臒l:'%lf'm)"), dist, m_dist_threshold);
			// ���G���[ �m�[�h���ʂ̃m�[�h�ɐڐG���Ă���
			m_output.OutputRegLog(err_level::error, err_code::TouchNgNode, info1, info2);
		}
	}
}

bool WalkNodeCheckFunction::findOverlapRoadNode(
	GeoClass& roadNodeObj
	, const GeoClass& walkNodeObj
	, const std::map<long, GeoClass>& roadNodeMap
	, const BGRTree& roadNodeRTree
	, double range
) const
{
	// ���H�m�[�h��T��
	auto findNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(range), roadNodeRTree);

	// �����������H�m�[�h�̃��[�v
	for (const auto& findNodeID : findNodeIDs) {

		auto findNodeItr = roadNodeMap.find(findNodeID);
		if (findNodeItr == roadNodeMap.end())
			continue;

		// ��v���邩�H
		if (walkNodeObj.IsEqual(findNodeItr->second.m_ipGeometry)) {
			roadNodeObj = findNodeItr->second;
			return true;
		}
	}
	return false;
}

bool WalkNodeCheckFunction::checkWalkableConnect(
	const GeoClass& roadNodeObj
	, long connectWalkLinkCnt
	, const sindy::CFieldMap& fieldMap
	, const FeaturesPack& roadLink
	, const BGRTree& roadLinkRTree
)
{
	// �G���[�ɂȂ�͓̂��HNW�ڑ��m�[�h�ɐڑ�������s�҃����N��1�{�݂̂̏ꍇ
	if (connectWalkLinkCnt != 1) return false;

	// ���H�m�[�h��ʂ��[�_�m�[�h�ȊO�̏ꍇ
	if (roadNodeObj.GetLongValue(fieldMap.FindField(road_node::kNodeClass)) != road_node::node_class::kTerminal)
	{
		auto findLinkIDs = util::getIntersectsIDs(roadNodeObj.GetBox(), roadLinkRTree);
		long roadNodeID = roadNodeObj.m_ObjectID;

		bool isAllNotwalk(false);  // �ڑ����铹�H�����N�����ׂĕ��s�֎~�iWalkable=0�j�̃t���O
		for (const auto& findLinkID : findLinkIDs)
		{
			auto findLinkItr = roadLink.m_FeatureMap.find(findLinkID);
			if (findLinkItr == roadLink.m_FeatureMap.end())
				continue;

			// ���s�\��ʃR�[�h
			if (findLinkItr->second.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kFromNodeID)) == roadNodeID ||
				findLinkItr->second.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kToNodeID)) == roadNodeID)
			{
				if( road_link::walkable_class::kNotWalk != findLinkItr->second.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kWalkable)) )
					return false;

				isAllNotwalk = true;
			}
		}
		// �ڑ����铹�H�����N�����ׂĕ��s�֎~�iWalkable=0�j�̏ꍇ
		return isAllNotwalk;
	}
	return false;
}

void WalkNodeCheckFunction::CheckWalkNodeWithPoint(
	const GeoClass& walkNode,
	const sindy::CFieldMap& fieldMap,
	const CString& checkField,
	const FeaturesPack& point,
	bool bRelease)
{
	using namespace sindy::schema::walk_node;
	using namespace sindy::schema::sindyk;

	CString info1, info2;

	long checkWalkNodeClass = 0;
	std::vector<err_code::ECode> errorCodeList;
	if (0 == checkField.CompareNoCase(kStationGatePointID))
	{
		checkWalkNodeClass = node_class::kGate;
		errorCodeList.push_back(err_code::NgStationNode);
		errorCodeList.push_back(err_code::NgStationGateID);
		errorCodeList.push_back(err_code::NotFindStation);
		errorCodeList.push_back(err_code::TooFarStation);
	}
	else if (0 == checkField.CompareNoCase(kUndergroundGatePointID))
	{
		checkWalkNodeClass = node_class::kUndergroundGate;
		errorCodeList.push_back(err_code::NgUnderGateNode);
		errorCodeList.push_back(err_code::NgUnderGateID);
		errorCodeList.push_back(err_code::NotFindUnderGate);
		errorCodeList.push_back(err_code::TooFarUnderGate);
		errorCodeList.push_back(err_code::NotAlivePOI);
	}
	else
		return;
	using namespace walk_node;
	long walkNodeClass = walkNode.GetLongValue(fieldMap.FindField(kNodeClass));
	long pointId = walkNode.GetLongValue(fieldMap.FindField(checkField));

	if (pointId == -1 || pointId == 0)
	{
		// ���G���[ �w�o�����m�[�h�Ȃ̂ɉw�o�����|�C���gID���t�^����Ă��Ȃ�
		// ���G���[ �n���X�o�����m�[�h�Ȃ̂ɒn���X�o�����|�C���gID���t�^����Ă��Ȃ�
		if (walkNodeClass == checkWalkNodeClass)
			m_output.OutputRegLog(err_level::error, errorCodeList.at(0));
		return;
	}

	// ���G���[ �w�o�����|�C���gID���t�^����Ă���̂ɉw�o�����m�[�h�ł͂Ȃ�
	// ���G���[ �n���X�o�����|�C���gID���t�^����Ă���̂ɒn���X�o�����m�[�h�ł͂Ȃ�
	if (walkNodeClass != checkWalkNodeClass)
	{
		info1 = judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(1), info1);
	}

	// �Y���̃|�C���g���擾
	auto pointItr = point.m_FeatureMap.find(pointId);

	// ���G���[ �w�o�����|�C���gID�ɑΉ�����w�o����POI���Ȃ�
	// ���G���[ �n���X�o�����|�C���gID�ɑΉ�����n���X�o����POI���Ȃ�
	if (pointItr == point.m_FeatureMap.end())
	{
		info1.Format(_T("%s:'%ld'"), checkField, pointId);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(2), info1);
		return;
	}
	// [bug 11619] �n���X�o�����|�C���g�̃`�F�b�N�̏ꍇ�A�폜�R�[�h���u�����v�łȂ��Ȃ�A�G���[�Ƃ���
	// (release���[�h�ł͎��s���Ȃ�)
	else if (!bRelease
		&& node_class::kUndergroundGate == checkWalkNodeClass
		&& poi_point_org::delete_code::kAlive != pointItr->second.GetLongValue(point.m_FieldMap.FindField(poi_point_org::kDelete)))
	{
		// ���G���[ �m�[�h�ɕR�t���n���X�o����POI�̍폜�R�[�h���u�����v�ł͂Ȃ�
		info1.Format(_T("%s:'%ld'"), checkField, pointId);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(4), info1);
		return;
	}

	// �����𑪂�
	auto& pointObj = pointItr->second;
	double dist = util::GetDistGeoClasses(walkNode, pointObj);

	// ���G���[ �w�o�����|�C���gID�ɑΉ�����w�o����POI���߂��ɂȂ�
	// ���G���[ �n���X�o�����|�C���gID�ɑΉ�����n���X�o����POI���߂��ɂȂ�
	if (dist > m_refer_id_threshold)
	{
		info1.Format(_T("%s:'%ld'"), checkField, pointId);
		info2.Format(_T("����:'%lf'm(臒l:'%lf'm)"), dist, m_refer_id_threshold);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(3), info1, info2);
	}
}

void WalkNodeCheckFunction::compairRoadNodeCheck(
	long walkNodeClass,
	long roadNodeID,
	long nodeNameIndex,
	long walkNodeRoadIndex,
	const GeoClass& walkNodeObj,
	const FeaturesPack& roadNode,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree,
	bool isForRelease
)
{
	CString info1, info2;
	// �}�s��m�[�h�̏ꍇ
	if (walkNodeClass == walk_node::node_class::kOutline)
	{
		// ���G���[ �}�s��m�[�h�ɓ��H�m�[�h�Ή�ID���t�^����Ă���
		info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
		m_output.OutputRegLog(err_level::error, err_code::BorderRoadNodeID, info1);
	}

	// �Y�����H�m�[�h�擾
	auto roadNodeItr = roadNode.m_FeatureMap.find(roadNodeID);
	if (roadNodeItr == roadNode.m_FeatureMap.end()) {
		// ���G���[ ���H�m�[�h�Ή�ID�ɑΉ����铹�H�m�[�h���Ȃ�
		info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
		m_output.OutputRegLog(err_level::error, err_code::NotFindRoadNode, info1);
	}
	else {
		// �����𑪂�
		auto& roadNodeObj = roadNodeItr->second;
		double dist = util::GetDistGeoClasses(walkNodeObj, roadNodeObj);
		if (dist > m_refer_id_threshold) {
			// ���G���[ ���H�m�[�h�Ή�ID�ɑΉ����铹�H�m�[�h���߂��ɂȂ�
			info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
			info2.Format(_T("����:'%lf'm(臒l:'%lf'm)"), dist, m_refer_id_threshold);
			m_output.OutputRegLog(err_level::error, err_code::TooFarRoadNode, info1, info2);
		}

		// �擾�������H�m�[�h�̌����_���̎擾
		auto strName = roadNodeObj.GetStringValue(nodeNameIndex);
		if (strName.IsEmpty()) {
			// ���G���[ ���H�m�[�h�Ή�ID�ɑΉ����铹�H�m�[�h�Ɍ����_���̂��Ȃ�
			info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
			m_output.OutputRegLog(err_level::error, err_code::NoNameRoadNode, info1);
		}
		else if (!isForRelease) {
			// ���̂����݂���ꍇ�́A�ߖT�ɈقȂ閼�̂��������m�F����
			auto neighborWalkNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_neighbor_threshold), walkNodeRTree);

			// �����������s�҃m�[�h�̃��[�v
			for (const auto& neighborWalkNodeID : neighborWalkNodeIDs) {

				// ���g�̓X�L�b�v
				if (neighborWalkNodeID == walkNodeObj.m_ObjectID)
					continue;

				auto neighborWalkNodeItr = walkNode.m_FeatureMap.find(neighborWalkNodeID);
				if (neighborWalkNodeItr == walkNode.m_FeatureMap.end())
					continue;

				auto& neighborWalkNodeObj = neighborWalkNodeItr->second;
				// �����𑪂��ċߖT臒l�ȏ�̏ꍇ�̓X�L�b�v
				double neighborDist = util::GetDistGeoClasses(walkNodeObj, neighborWalkNodeObj);
				if (neighborDist > m_neighbor_threshold)
					continue;

				// ���H�m�[�hID�������ĂȂ���΃X�L�b�v
				long neighborRoadNodeID = neighborWalkNodeObj.GetLongValue(walkNodeRoadIndex);
				if (neighborRoadNodeID == 0 || neighborRoadNodeID == -1)
					continue;

				// ID�̓��H�m�[�h�擾
				auto neighborRoadNodeItr = roadNode.m_FeatureMap.find(neighborRoadNodeID);
				if (neighborRoadNodeItr == roadNode.m_FeatureMap.end())
					continue;

				// ���̌����_���̎擾
				auto neighborName = neighborRoadNodeItr->second.GetStringValue(nodeNameIndex);

				// ��Ȃ�X�L�b�v
				if (neighborName.IsEmpty())
					continue;

				// ��r���āA�قȂ�ꍇ�̓G���[
				if (0 != strName.Compare(neighborName)) {
					// ���G���[ �ߖT�ɕʂ̌����_���̂����m�[�h�����݂���
					info1.Format(_T("'%s'/'%s'(WalkNodeID:%ld)"), strName, neighborName, neighborWalkNodeID);
					info2.Format(_T("����:'%lf'm(臒l:'%lf'm)"), neighborDist, m_neighbor_threshold);
					m_output.OutputRegLog(err_level::error, err_code::NearOtherName, info1, info2);
				}
			}
		}
	}
}

bool WalkNodeCheckFunction::isConnectedRoadNodeAtNode(
	const GeoClass& walkNodeObj,
	long walkClassIndex,
	long walkNodeClassIndex,
	long fromNodeIdIndex,
	long toNodeIdIndex,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs,
	std::set<long>& inspectedIDs

)
{
	if (walk_node::node_class::kRoadNW == walkNodeObj.GetLongValue(walkNodeClassIndex)) {
		return true;
	}
	auto linkIDSetItr = walkNodeLinkIDs.find(walkNodeObj.m_ObjectID);
	if (walkNodeLinkIDs.end() != linkIDSetItr) {
		for (auto linkid : linkIDSetItr->second) {
			if (isConnectedRoadNode(linkid, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex, toNodeIdIndex, walkLink, walkNode, walkNodeLinkIDs, inspectedIDs)) {
				return true;
			}
		}
	}
	return false;
}
bool WalkNodeCheckFunction::isConnectedRoadNode(
	long linkID,
	long walkClassIndex,
	long walkNodeClassIndex,
	long fromNodeIdIndex,
	long toNodeIdIndex,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs,
	std::set<long>& inspectedIDs
)
{
	// ���̊֐����ł́A�H�������ʁA�L���b�V���O�ɂȂ邩������Ȃ��̂ŁA
	// .at(id)�ł͂Ȃ�SearchObj���g�p

	// �`�F�b�N�ς݂Ȃ猩�Ȃ�
	auto result = inspectedIDs.insert(linkID);
	if (!result.second)
		return false;

	auto externalLink = util::SearchObj(walkLink, linkID);
	if (!externalLink.IsOk()) {
		// ������Ȃ��̂̓G���[�����A�����͂ǂ����悤���Ȃ��i���̂Ƃ���ŏo��͂��j
		return false;
	}
	long walkClass = externalLink.GetLongValue(walkNodeClassIndex);
	if (judge_value::IsInternalTicketGateLink(walkClass)) {
		// ���D�������N�͏��O
		return false;
	}
	long nodeID = externalLink.GetLongValue(fromNodeIdIndex);
	auto nodeObj = util::SearchObj(walkNode, nodeID);
	if (isConnectedRoadNodeAtNode(nodeObj, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex, toNodeIdIndex, walkLink,
		walkNode, walkNodeLinkIDs, inspectedIDs)) {
		return true;
	}
	nodeID = externalLink.GetLongValue(toNodeIdIndex);
	nodeObj = util::SearchObj(walkNode, nodeID);
	if (isConnectedRoadNodeAtNode(nodeObj, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex, toNodeIdIndex, walkLink,
		walkNode, walkNodeLinkIDs, inspectedIDs)) {
		return true;
	}
	return false;
}

void WalkNodeCheckFunction::internalTicketGateConenctCheck(
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	long walkClassIndex,
	long walkNodeClassIndex,
	long fromNodeIdIndex,
	long toNodeIdIndex,
	const std::set<long>& links,
	const NODEIDMAP& walkNodeLinkIDs
)
{
	// ���������ڑ������N�����Q�����̓G���[
	if (links.size() < 2) {
		// �G���[ ���D�m�[�h�Ȃ̂ɒ[�_�ɂȂ��Ă���
		m_output.OutputRegLog(err_level::error, err_code::NgTicketGate1);

		return;
	}
	std::set<long> externalLinks;
	for (auto linkID : links) {
		auto link = walkLink.m_FeatureMap.at(linkID);
		if (!judge_value::IsInternalTicketGateLink(link.GetLongValue(walkClassIndex))) {
			externalLinks.insert(linkID);
		}
	}
	// ���D�������N�݂̂Ȃ�OK
	if (externalLinks.empty()) {
		return;
	}
	else if (externalLinks.size() == links.size()) {
		// ���D�O�����N�݂̂Ȃ�A�m�[�h�̃G���[
		// �G���[ ���D�m�[�h�ɉ��D�������N���ڑ����Ă��Ȃ�
		m_output.OutputRegLog(err_level::error, err_code::NgTicketGate2);

		return;
	}
	// ���D��/���D�O��������A�m�[�h�Ƃ��Ă�OK�Ȃ̂ŁA���D�O�����N��
	// �{���ɉ��D�O�ł����̂��i�l�b�g���[�N�Ƃ��ē��H�ɐڑ����Ă��邩�j�`�F�b�N
	std::set<long> inspectedIDs;
	for (auto linkID : externalLinks) {
		if (isConnectedRoadNode(linkID, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex, toNodeIdIndex,
			walkLink, walkNode, walkNodeLinkIDs, inspectedIDs)) {
			return;
		}
	}
	// �����܂ŗ�����A�E�g
	// �G���[ ���D�m�[�h���瓹�H�ڑ��m�[�h�܂Őڑ�����Ă��Ȃ�
	m_output.OutputRegLog(err_level::error, err_code::NgTicketGate3);

}

// ���H�����N��ɂ���̂ɓ��H�����N�ڑ��m�[�h�ɂȂ��Ă��邩�`�F�b�N
void WalkNodeCheckFunction::checkRoadLinkConnectNode(
	const GeoClass& walkNodeObject,
	long walkNodeClass,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree) const
{
	using namespace walk_node;
	// ���H�����N��
	std::vector<GeoClass> crossRoadLinks;
	if (util::findOverlapGeometry(walkNodeObject, roadLink, roadLinkRTree, crossRoadLinks) <= 0) return;

	if (walkNodeClass != walk_node::node_class::kConnectedRoad)
		m_output.OutputRegLog(err_level::error, err_code::NgOnRoadLinkException);
}

// ���H�����N�ڑ��m�[�h�����H�����N�����ӏ��ɂ��邩�`�F�b�N
void WalkNodeCheckFunction::checkRoadLinkCrossNode(
	const GeoClass& walkNodeObject,
	long walkNodeClass,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree, 
	const std::map<long, GeoClass>& roadNodeMap,
	const BGRTree& roadNodeRTree) const
{
	using namespace walk_node;
	// ���H�����N�����ӏ�
	std::vector<GeoClass> crossRoadLinks;
	if(util::findOverlapGeometry( walkNodeObject, roadLink, roadLinkRTree, crossRoadLinks ) < 2) return;

	// ���s�҃m�[�h��̓��H�m�[�h������
	GeoClass roadNodeObj;
	// ���H�m�[�h��ł���΁A�{�`�F�b�N�ł͑ΏۊO
	if ( findOverlapRoadNode( roadNodeObj, walkNodeObject, roadNodeMap, roadNodeRTree, m_coordinateTolerance ) ) return;

	m_output.OutputRegLog( err_level::error, err_code::NgOnRoadLinkCross );
}

// �X�N�����u�������_�Ȃ̂ɃX�N�����u���G���A�|���S�����������`�F�b�N
void WalkNodeCheckFunction::checkWalkNodeCrossWalkLink(
	const GeoClass& walkNodeObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree,
	const FeaturesPack& scrambleArea,
	const BGRTree& scrambleAreaRTree) const
{
	using namespace walk_link;

	std::vector<GeoClass> crossScrambleObjects;
	if (util::findOverlapGeometry(walkNodeObject, scrambleArea, scrambleAreaRTree, crossScrambleObjects) > 0) return;

	std::vector<GeoClass> crossWalkLinkObjects;
	if (util::findOverlapGeometry(walkNodeObject, walkLink, walkLinkRTree, crossWalkLinkObjects) < 4) return;

	long crossNum(0L);
	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);
	for (const auto& crossObject : crossWalkLinkObjects)
	{
		auto walkLinkObj = walkLink.m_FeatureMap.find(crossObject.m_ObjectID);

		if (walkLinkObj != walkLink.m_FeatureMap.end())
		{
			long  walkLinkClass = walkLinkObj->second.GetLongValue( walkLinkClassIndex );

			if(walkLinkClass != walk_class::kCrossRoad ) return;
		}
	}

	m_output.OutputRegLog( err_level::error, err_code::NgOnWalkLinkCross );
}

// �|�C���g���m�iWALK_NODE�AHEIGHT_NODE�j�̋������߂����`�F�b�N
void WalkNodeCheckFunction::checkNearPoint(
	const GeoClass& walkNodeObject,
	const FeaturesPack& walkNode,	
	const FeaturesPack& heightNode,
	const BGRTree& walkNodeRTree,
	const BGRTree& heightNodeRTree
	) const
{
	std::vector<GeoClass> crossHeightNodes;
	if(util::findOverlapGeometry(walkNodeObject, heightNode, heightNodeRTree, crossHeightNodes, 0.005) > 0) 
	{
		for( const auto& node : crossHeightNodes )
		{
			if( walkNodeObject.IsEqual( node.m_ipGeometry ) ) continue;

			CString info;
			info.Format(_T("%s(%d)"), height_node::kTableName, node.m_ObjectID);
			m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
		}
	}

	std::vector<GeoClass> crossWalkNodes;
	if(util::findOverlapGeometry(walkNodeObject, walkNode, walkNodeRTree, crossWalkNodes, 0.01) > 0)
	{
		for( const auto& node : crossWalkNodes )
		{
			// ��v���邩�H
			if(walkNodeObject.m_ObjectID == node.m_ObjectID || !walkNodeObject.IsEqual( node.m_ipGeometry ) ) continue;

			CString info;
			info.Format(_T("%s(%d)"), walk_node::kTableName, node.m_ObjectID);
			m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
		}
	}
}

// ���W�J���R�l�N�V�����������ڑ����Ă��邩�`�F�b�N
void WalkNodeCheckFunction::checkConnectWalkLink(
	const GeoClass& walkNodeObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree)const
{
	using namespace walk_link;

	std::vector<GeoClass> crossWalkLinks;
	if (util::findOverlapGeometry(walkNodeObject, walkLink, walkLinkRTree, crossWalkLinks) <= 1) return;

	long crossNum(0L);
	long walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);
	for (const auto& link : crossWalkLinks)
	{
		auto walkLinkObj = walkLink.m_FeatureMap.find(link.m_ObjectID);
		if (walkLinkObj == walkLink.m_FeatureMap.end()) continue;

		long  walkLinkClass = walkLinkObj->second.GetLongValue(walkLinkClassIndex);

		if (walkLinkClass == walk_class::kLogicalConnection)
			++crossNum;

		if (crossNum >= 2)
		{
			m_output.OutputRegLog(err_level::warning, err_code::ConnectWalkLink);
			return;
		}
	}
}

// ���W�J���R�l�N�V�������s�p�ɐڑ����Ă��邩�`�F�b�N
void WalkNodeCheckFunction::checkConnectWalkLinkAngle(
	const GeoClass& walkNodeObject,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree)const
{
	using namespace walk_link;

	std::vector<GeoClass> crossWalkLinks;
	if (util::findOverlapGeometry(walkNodeObject, walkLink, walkLinkRTree, crossWalkLinks) <= 1) return;

	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);

	bool existLogicalConnection(false);
	for( const auto& link : crossWalkLinks )
	{
		auto walkLinkObj = walkLink.m_FeatureMap.find(link.m_ObjectID);
		if ( walkLinkObj == walkLink.m_FeatureMap.end() ) continue;

		long  walkLinkClass = walkLinkObj->second.GetLongValue( walkLinkClassIndex );

		if(walkLinkClass != walk_class::kLogicalConnection ) continue;

		existLogicalConnection = true;
		break;
	}

	if(!existLogicalConnection) return;

	double x = 0.0, y = 0.0;
	IPointPtr(walkNodeObject.m_ipGeometry)->QueryCoords( &x, &y );

	double cmpX(0.0), cmpY(0.0), cmpX_After(0.0), cmpY_After(0.0);

	for( long index = 0L; index < crossWalkLinks.size() - 1; ++index )
	{
		getCoordinate(walkNodeObject.m_ipGeometry, crossWalkLinks[index].m_ipGeometry, cmpX, cmpY);

		for( long indexAfter = index + 1; indexAfter < crossWalkLinks.size(); ++indexAfter )
		{
			getCoordinate(walkNodeObject.m_ipGeometry, crossWalkLinks[indexAfter].m_ipGeometry, cmpX_After, cmpY_After);

			double angle = util::angle(cmpX, cmpY, x, y, cmpX_After, cmpY_After);
			angle = abs(angle);

			if (angle > 20) continue;

			m_output.OutputRegLog(err_level::error, err_code::NgConnectWalkLinkAngle);
			return;
		}
	}
}

// ���S�_�̗אړ_�̍��W���擾
void WalkNodeCheckFunction::getCoordinate(
	const IGeometryPtr& ipGeometry,
	const IGeometryPtr& ipGeometryCompare,
	double& x,
	double& y) const
{
	IPointCollectionPtr ipWalkLinkPointCol(ipGeometryCompare);
	IPointPtr ipFromPoint, ipToPoint;
	if (FAILED(((IPolylinePtr)ipGeometryCompare)->get_FromPoint(&ipFromPoint)))
		return;
	if (FAILED(((IPolylinePtr)ipGeometryCompare)->get_ToPoint(&ipToPoint)))
		return;

	long index(0L);
	if (AheIsEqual(ipFromPoint, ipGeometry))
	{
		index = 1L;
	}
	else if (AheIsEqual(ipToPoint, ipGeometry))
	{
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		index = walkLinkPointCnt - 2;
	}
	else
		return;

	IPointPtr ipPoint = ipWalkLinkPointCol->GetPoint(index);
	ipPoint->QueryCoords(&x, &y);
}
