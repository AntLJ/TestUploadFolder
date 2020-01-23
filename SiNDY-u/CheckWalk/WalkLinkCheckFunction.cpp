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
#include "WalkLinkCheckFunction.h"
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

void WalkLinkCheckFunction::CheckWalkLink(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const BGRTree& walkLinkRTree,
	const BGRTree& roadLinkRTree,
	const BGRTree& roadNodeRTree,
	const NODEIDMAP& walkNodeLinkIDs,
	bool bOther,
	const FeaturesPack& scrambleArea,
	const BGRTree& scrambleAreaRTree)
{
	using namespace walk_link;
	using namespace ipc_feature;
	using namespace uh::str_util;
	CString info1, info2;
	map<pair<long, long>, list<long>> dupNodeMap; //!< �L�[�FFrom/To�m�[�hID�iID�̏�����������j�A�l�F�����N��ObjectID

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_link::kTableName);

	// �����Ώۃ��b�V����,����Ȍ`�������walkLinkId�̃��X�g
	std::vector<long> targetWalkLinkId;

	// �G���[�R�[�h353�p�̃����N���X�g
	map<long, GeoClass> walkLinkMapFor353;

	// ���s�҃����N�̃`�F�b�N
	long  fromWalkNodeIDIndex = walkLink.m_FieldMap.FindField(kFromNodeID);
	long  toWalkNodeIDIndex = walkLink.m_FieldMap.FindField(kToNodeID);
	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);
	long  floorMoveClassIndex = walkLink.m_FieldMap.FindField(kFloorMoveClass);
	long  onewayCodeIndex = walkLink.m_FieldMap.FindField(kOneway);
	long  noPassCodeIndex = walkLink.m_FieldMap.FindField(kNoPassage);
	long  roadSurfaceCodeIndex = walkLink.m_FieldMap.FindField(kRoadSurface);
	long  walkCodeIndex = walkLink.m_FieldMap.FindField(kWalkCode);

	long autoWalkFIndex = walkLink.m_FieldMap.FindField(kAutoWalkFlag);
	long stairFIndex = walkLink.m_FieldMap.FindField(kStairFlag);
	long slopeFIndex = walkLink.m_FieldMap.FindField(kSlopeFlag);
	long escalatorFIndex = walkLink.m_FieldMap.FindField(kEscalatorFlag);
	long arcadeFIndex = walkLink.m_FieldMap.FindField(kArcadeFlag);
	long openAreaFIndex = walkLink.m_FieldMap.FindField(kOpenAreaFlag);
	long tunnelFIndex = walkLink.m_FieldMap.FindField(kTunnelFlag);
	long onewayRFIndex = walkLink.m_FieldMap.FindField(kOnewayRF);
	long noPassRFIndex = walkLink.m_FieldMap.FindField(kNoPassageRF);
	long scrambleFIndex = walkLink.m_FieldMap.FindField(kScrambleFlag);

	long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
	long floorLevelIndex = walkLink.m_FieldMap.FindField(kFloorLevel);

	long walkNodeClassIndex = walkNode.m_FieldMap.FindField(walk_node::kNodeClass);
	for (const auto& walkLinkPair : walkLink.m_FeatureMap)
	{
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkLinkPair.second.IsDisjoint(ipMeshGeo))
			continue;

		auto  walkLinkID = walkLinkPair.first;
		auto& walkLinkObj = walkLinkPair.second;
		long  fromWalkNodeID = walkLinkObj.GetLongValue(fromWalkNodeIDIndex);
		long  toWalkNodeID = walkLinkObj.GetLongValue(toWalkNodeIDIndex);
		long  walkLinkClass = walkLinkObj.GetLongValue(walkLinkClassIndex);
		long  floorMoveClass = walkLinkObj.GetLongValue(floorMoveClassIndex);
		long  onewayCode = walkLinkObj.GetLongValue(onewayCodeIndex);
		long  noPassCode = walkLinkObj.GetLongValue(noPassCodeIndex);
		long  roadSurfaceCode = walkLinkObj.GetLongValue(roadSurfaceCodeIndex);
		long  walkCode = walkLinkObj.GetLongValue(walkCodeIndex);

		long autoWalkF = walkLinkObj.GetLongValue(autoWalkFIndex);
		long stairF = walkLinkObj.GetLongValue(stairFIndex);
		long slopeF = walkLinkObj.GetLongValue(slopeFIndex);
		long escalatorF = walkLinkObj.GetLongValue(escalatorFIndex);
		long arcadeF = walkLinkObj.GetLongValue(arcadeFIndex);
		long openAreaF = walkLinkObj.GetLongValue(openAreaFIndex);
		long tunnelF = walkLinkObj.GetLongValue(tunnelFIndex);
		long onewayRF = walkLinkObj.GetLongValue(onewayRFIndex);
		long noPassRF = walkLinkObj.GetLongValue(noPassRFIndex);

		// bug 12738�Œǉ��B�ǉ����_�Œʏ�f�[�^�ɂ͑��݂��Ȃ��t�B�[���h�̂��߁A�Ȃ�������u0�v�Ƃ���
		long scrambleF = scrambleFIndex >= 0 ? walkLinkObj.GetLongValue(scrambleFIndex) : 0;

		m_output.SetModifyInfo(walkLinkID, walkLinkObj.GetStringValue(modOperatorIndex),
			walkLinkObj.GetStringValue(modDateIndex), walkLinkObj.GetStringValue(modProgramIndex), walkLinkObj.GetStringValue(modProgramDateIndex));

		// ���P���ȑ����`�F�b�N
		// ������OK�Ȓl���m�F����
		attributesCheck(walkLinkClass, floorMoveClass, onewayCode, noPassCode, roadSurfaceCode, walkCode, false);
		flagsCheck(autoWalkF, stairF, slopeF, escalatorF, arcadeF, openAreaF, tunnelF, scrambleF, onewayRF, noPassRF);

		// From/To��ID���m�F
		fromToIdCheck(walkLinkID, fromWalkNodeID, toWalkNodeID, dupNodeMap);

		// ����������`����g�����`�F�b�N
		GeoClass fromWalkNodeObj, toWalkNodeObj;
		// �P���Ȍ`��m�F
		if (!shapeCheck(walkLinkObj, walkNode, walkLinkClass, fromWalkNodeID, toWalkNodeID,
			walkNodeClassIndex, targetWalkLinkId, fromWalkNodeObj, toWalkNodeObj))
			continue;
		IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		// �}�b�`���O�Ɋւ���`�F�b�N
		checkMatching(walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt,
			walkLinkClass, walkLink, roadLink, roadNode,
			walkLinkRTree, roadLinkRTree, roadNodeRTree, meshcode, bOther);
		// �݂Ȃ����f�������������Ă��铹�H�����N��2�Ԑ��ȏ�
		inferredCrossRoadCheck(walkLinkObj, walkLinkClass, roadLink, roadLinkRTree);

		// �_��ɕ������Ċm�F
		shapeCheck2(walkLinkObj, fromWalkNodeObj, toWalkNodeObj, false);

		// ����������́A�����[�V�����⑮���Ԃ̐����`�F�b�N
		// TODO: �֐���2�ɕ��������A�Ӗ��̂��镪�������ǂ����͌����̗]�n����
		attributeConformityCheck(floorMoveClass, stairF, slopeF, escalatorF, onewayCode, autoWalkF, arcadeF, openAreaF, tunnelF, scrambleF, walkLinkClass, walkCode);
		attributeConformityCheck2(onewayCode, noPassCode, noPassRF, onewayRF, walkLinkClass, walkLinkID);

		// ���O��̃����N���g�p���Ẵ`�F�b�N
		if (!fromWalkNodeObj.IsOk() || !toWalkNodeObj.IsOk())
			continue;

		// bug 11430 - [u][�v�]]�O��̕��s�Ҏ�ʂƈقȂ�ꍇ�Ɍ��o���Ăق���
		// ���s�Ҏ�ʃR�[�h���������A�{�ݕ~�n���i���O�j�A�{�ݕ~�n���i�����j�A�n���ʘH�A���f�n�����̏ꍇ
		// �{�ԃ`�F�b�N�͌�ōs��
		if (judge_value::IsTargetOfCheck353(walkLinkClass))
		{
			walkLinkMapFor353.insert(walkLinkPair);
		}

		auto fromLinkIDSetItr = walkNodeLinkIDs.find(fromWalkNodeID);
		auto toLinkIDSetItr = walkNodeLinkIDs.find(toWalkNodeID);

		if (fromLinkIDSetItr != walkNodeLinkIDs.end() && toLinkIDSetItr != walkNodeLinkIDs.end())
		{
			auto fromLinks = util::GetObjects(walkLinkID, fromLinkIDSetItr->second, walkLink.m_FeatureMap);
			auto toLinks = util::GetObjects(walkLinkID, toLinkIDSetItr->second, walkLink.m_FeatureMap);
			// �����ω��m�F�i�Ƃ肠�����A2���H����j
			if (fromLinkIDSetItr->second.size() == 2 && toLinkIDSetItr->second.size() == 2)
			{
				attributeChangeCheck(fromLinks, toLinks, walkLinkObj, walkLink, walkLinkClass, floorMoveClass, floorLevelIndex);
			}

			// �L��t���O��ON�̏ꍇ�A�ڑ����Ă��郊���N�ɍL��t���O��ON�̃����N�����[�ɑ��݂��Ă��邩�m�F
			openAreaCheck(openAreaF, fromLinks, toLinks, walkLink);

			// ���s�Ҏ�ʂ��ڑ������N�̏ꍇ�A�ڂ��Ă���m�[�h�����HNW�ڑ��m�[�h�A�܂��͐ڑ����Ă��郊���N���l�ԋ��p�����N�ł��邩�m�F
			connectLinkCheck(walkLinkClass, fromWalkNodeObj, toWalkNodeObj, fromLinks, toLinks, walkLink, walkNodeClassIndex);
			// fromLinkIDSetItr,toLinkIDSetItr��ۏ؂��邽�߁A���Ԃ����ւ���
			underGroundCheck(walkLinkClass, walkLinkID, fromWalkNodeObj, toWalkNodeObj, fromLinks, toLinks, walkLink, walkNode, meshcode);
			// bug 12633- [u][�v�]]�����_���̂��t�^����Ă��Ȃ����f���������N�[�_���G���[�Ƃ��Ăق���
			if (walk_class::kCrossRoad == walkLinkClass) {
				crossRoadNodeCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeIDIndex, toWalkNodeIDIndex,
					walkLinkObj, fromWalkNodeObj, toWalkNodeObj, walkLink, walkNode,
					walkNodeLinkIDs, meshcode);
			}
			// bug 12687 - [u][���s��]MV�������D���f�[�^�̃`�F�b�N�ǉ��̇@
			else if (judge_value::IsInternalTicketGateLink(walkLinkClass)) {
				internalTicketGateLinkCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeObj, toWalkNodeObj,
					fromLinks, toLinks);
			}
		}
		// bug 11433 - [u][�v�]]���s����`��̏ꍇ�ɑ����̖������`�F�b�N���Ăق���
		// �K�w�ړ���ʃR�[�h���u0:�㉺�Ȃ��v�ȊO�̏ꍇ
		parallelCheck(walkLinkClass, floorMoveClass, escalatorF, onewayCode, walkLinkObj, fromWalkNodeID,
			toWalkNodeID, walkNodeLinkIDs, walkLink, walkLinkRTree);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		long wheelChairIndex = walkLink.m_FieldMap.FindField(kWheelChair);
		long platformNumberIndex = walkLink.m_FieldMap.FindField(kPlatformNumber);
		long wheelChairCode = walkLinkObj.GetLongValue(wheelChairIndex);
		long platformNumber = walkLinkObj.GetLongValue(platformNumberIndex);

		// ���G���[ �X�N�����u���t���O���t�^����Ă���̂ɃX�N�����u���G���A���ɖ���
		walkLinkNotOnScrambleAreaCheck(walkLinkObj, scrambleArea, scrambleAreaRTree, scrambleF);
		// ���G���[ �X�N�����u���G���A���Ȃ̂ɃX�N�����u���t���O���t�^����Ă��Ȃ�
		walkLinkOnScrambleAreaCheck(walkLinkObj, scrambleArea, scrambleAreaRTree, walkLinkClass, scrambleF);
		// ���G���[ �Ԉ֎q�ʍs�R�[�h���s���Ȓl
		checkWheelChair(wheelChairCode);
		// ���G���[ �̂�Δԍ����s���Ȓl
		checkPlatformNumber(platformNumber);

		if (walkLinkClass == walk_class::ECode_draft::kLogicalConnection)
		{
			// ���G���[ �������W�J���R�l�N�V����
			checkMicroLogicalConnection(walkLinkObj);
		}
		// ���G���[ ���W�J���R�l�N�V�����ƌ������Ă���
		if (walkLinkClass == walk_class::kLogicalConnection)
			checkCrossWalkLink(walkLinkObj, walkLink, walkLinkRTree);
	}
	// �G���[�R�[�h353�̃`�F�b�N
	attributeConnectionCheck(ipMeshGeo, walkLink, walkNode, walkNodeLinkIDs, walkLinkMapFor353);

	// ���G���[ ����From/To���������N����������
	sameFromToCheck(dupNodeMap, walkLink, meshcode);

	// �\���_���������N�Ɋ܂܂�Ȃ����A������Ń`�F�b�N
	checkPointOnOtherLink(targetWalkLinkId, walkLink, meshcode);
}

void WalkLinkCheckFunction::checkHumanCar(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree
)
{
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_link::kTableName);

	// ���s�҃����N�̃`�F�b�N
	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(walk_link::kWalkClass);
	long walkableIndex = roadLink.m_FieldMap.FindField(road_link::kWalkable);
	/* ���r���[�p�R�����g
		�����uwalk_link::kOperator�v���ɂȂ��Ă������A�uroad_link�v�̊ԈႢ�Ǝv����
		���F������Ȃ̂ŁA���ʂɖ��͂Ȃ������͂�
	*/
	long modOperator_RLIndex = roadLink.m_FieldMap.FindField(road_link::kOperator);
	long modDate_RLIndex = roadLink.m_FieldMap.FindField(road_link::kModifyDate);
	long modProgram_RLIndex = roadLink.m_FieldMap.FindField(road_link::kModifyProgName);
	long modProgramDate_RLIndex = roadLink.m_FieldMap.FindField(road_link::kProgModifyDate);
	for (const auto& walkLink : walkLink.m_FeatureMap)
	{
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkLink.second.IsDisjoint(ipMeshGeo))
			continue;

		auto& walkLinkObj = walkLink.second;
		// �l�ԋ��p�����N�ȊO�̓`�F�b�N�s�v
		long  walkLinkClass = walkLinkObj.GetLongValue(walkLinkClassIndex);
		if (walkLinkClass != walk_link::walk_class::kWalkableRoad)
			continue;

		// �s���`��͖{�`�F�b�N�̑ΏۊO�i���������`�F�b�N�d�l�Ƃ���Ɨ�����)
		esriGeometryType eType = (walkLinkObj.m_ipGeometry)->GetGeometryType();
		IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		if (!walkLinkObj.m_ipGeometry
			|| !ipWalkLinkPointCol
			|| walkLinkPointCnt < 2
			|| eType != esriGeometryPolyline)
			continue;

		// ���s�҃����N�͈̔͂̔��Ɉ��������铹�H�����N���擾
		auto findRoadLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), roadLinkRTree);

		GeoClass findRoadLinkObj; // �}�b�`���O���ʂ��i�[
		bool matchLink = isWalkLinkOnRoadLink(findRoadLinkObj, walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt, walkLinkClass,
			findRoadLinkIDs, roadLink.m_FeatureMap);

		// �}�b�`���铹�H���Ȃ��Ȃ�G���[�o�͑Ώۂ͂Ȃ�
		if (!matchLink) continue;

		if (road_link::walkable_class::kNotWalk == findRoadLinkObj.GetLongValue(walkableIndex))
			continue;

		// ���G���[ �l�ԋ��p�����N�ƈ�v���铹�H�����N�����s�֎~�ȊO
		CString modOperator_RL = findRoadLinkObj.GetStringValue(modOperator_RLIndex);
		CString modDate_RL = findRoadLinkObj.GetStringValue(modDate_RLIndex);
		CString modProgram_RL = findRoadLinkObj.GetStringValue(modProgram_RLIndex);
		CString modProgramDate_RL = findRoadLinkObj.GetStringValue(modProgramDate_RLIndex);

		m_output.OutputLog(road_link::kTableName,
			findRoadLinkObj.m_ObjectID, err_level::error,
			err_code::NgRoadLinkWalkable, meshcode, modOperator_RL, modDate_RL, modProgram_RL, modProgramDate_RL,
			uh::str_util::ToString(findRoadLinkObj.GetLongValue(walkableIndex)));
	}
}


void WalkLinkCheckFunction::checkRelease(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const BGRTree& walkLinkRTree,
	const BGRTree& roadLinkRTree,
	const BGRTree& roadNodeRTree,
	const NODEIDMAP& walkNodeLinkIDs)
{
	using namespace walk_link;
	using namespace ipc_feature;
	using namespace uh::str_util;
	CString info1, info2;
	map<pair<long, long>, list<long>> dupNodeMap; //!< �L�[�FFrom/To�m�[�hID�iID�̏�����������j�A�l�F�����N��ObjectID

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_link::kTableName);

	// �����Ώۃ��b�V����,����Ȍ`�������walkLinkId�̃��X�g
	std::vector<long> targetWalkLinkId;

	// ���s�҃����N�̃`�F�b�N
	long  fromWalkNodeIDIndex = walkLink.m_FieldMap.FindField(kFromNodeID);
	long  toWalkNodeIDIndex = walkLink.m_FieldMap.FindField(kToNodeID);
	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);
	long  onewayCodeIndex = walkLink.m_FieldMap.FindField(kOneway);
	long  noPassCodeIndex = walkLink.m_FieldMap.FindField(kNoPassage);
	long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
	long walkNodeClassIndex = walkNode.m_FieldMap.FindField(walk_node::kNodeClass);

	for (const auto& walkLinkPair : walkLink.m_FeatureMap)
	{
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkLinkPair.second.IsDisjoint(ipMeshGeo))
			continue;

		long  walkLinkID = walkLinkPair.first;
		auto& walkLinkObj = walkLinkPair.second;
		long  fromWalkNodeID = walkLinkObj.GetLongValue(fromWalkNodeIDIndex);
		long  toWalkNodeID = walkLinkObj.GetLongValue(toWalkNodeIDIndex);
		long  walkLinkClass = walkLinkObj.GetLongValue(walkLinkClassIndex);
		long  onewayCode = walkLinkObj.GetLongValue(onewayCodeIndex);
		long  noPassCode = walkLinkObj.GetLongValue(noPassCodeIndex);

		m_output.SetModifyInfo(walkLinkID, walkLinkObj.GetStringValue(modOperatorIndex),
			walkLinkObj.GetStringValue(modDateIndex), walkLinkObj.GetStringValue(modProgramIndex), walkLinkObj.GetStringValue(modProgramDateIndex));

		// ���P���ȑ����`�F�b�N
		// �e�����l����`�����m�F����
		attributesCheck(walkLinkClass, -1, onewayCode, noPassCode, -1, -1, true);
		// From/To��ID���m�F
		fromToIdCheck(walkLinkID, fromWalkNodeID, toWalkNodeID, dupNodeMap);

		// ����������`����g�����`�F�b�N
		// �P���Ȍ`��m�F
		GeoClass fromWalkNodeObj, toWalkNodeObj;
		if (!shapeCheck(walkLinkObj, walkNode, walkLinkClass, fromWalkNodeID, toWalkNodeID,
			walkNodeClassIndex, targetWalkLinkId, fromWalkNodeObj, toWalkNodeObj))
			continue;
		IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		// �}�b�`���O�Ɋւ���`�F�b�N
		checkMatching(walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt,
			walkLinkClass, walkLink, roadLink, roadNode,
			walkLinkRTree, roadLinkRTree, roadNodeRTree, meshcode, false);
		shapeCheck2(walkLinkObj, fromWalkNodeObj, toWalkNodeObj, true);

		auto fromLinkIDSetItr = walkNodeLinkIDs.find(fromWalkNodeID);
		auto toLinkIDSetItr = walkNodeLinkIDs.find(toWalkNodeID);

		// ���s�Ҏ�ʂ��ڑ������N�̏ꍇ�A�ڂ��Ă���m�[�h�����HNW�ڑ��m�[�h�A�܂��͐ڑ����Ă��郊���N���l�ԋ��p�����N�ł��邩�m�F
		if (fromLinkIDSetItr != walkNodeLinkIDs.end() && toLinkIDSetItr != walkNodeLinkIDs.end())
		{
			auto fromLinks = util::GetObjects(walkLinkID, fromLinkIDSetItr->second, walkLink.m_FeatureMap);
			auto toLinks = util::GetObjects(walkLinkID, toLinkIDSetItr->second, walkLink.m_FeatureMap);
			connectLinkCheck(walkLinkClass, fromWalkNodeObj, toWalkNodeObj, fromLinks, toLinks, walkLink, walkNodeClassIndex);
			// bug 12633- [u][�v�]]�����_���̂��t�^����Ă��Ȃ����f���������N�[�_���G���[�Ƃ��Ăق���
			if (walk_class::kCrossRoad == walkLinkClass) {
				crossRoadNodeCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeIDIndex, toWalkNodeIDIndex,
					walkLinkObj, fromWalkNodeObj, toWalkNodeObj, walkLink, walkNode,
					walkNodeLinkIDs, meshcode);
			}
			// bug 12687 - [u][���s��]MV�������D���f�[�^�̃`�F�b�N�ǉ��̇@
			else if (judge_value::IsInternalTicketGateLink(walkLinkClass)) {
				internalTicketGateLinkCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeObj, toWalkNodeObj,
					fromLinks, toLinks);
			}
		}
	}

	sameFromToCheck(dupNodeMap, walkLink, meshcode);

	// �\���_���������N�Ɋ܂܂�Ȃ����A������Ń`�F�b�N
	checkPointOnOtherLink(targetWalkLinkId, walkLink, meshcode);
}

bool WalkLinkCheckFunction::isWalkLinkOnRoadLink(
	GeoClass& findRoadLinkObj,
	const GeoClass& walkLinkObj,
	const IPointCollectionPtr& ipWalkLinkPointCol,
	long walkLinkPointCnt,
	long walkLinkClass,
	const std::set<long>& roadLinkSet,
	const std::map<long, GeoClass>& roadLinkMap
)
{
	using namespace sindy::schema::walk_link;

	if (walk_class::kWalkableRoad == walkLinkClass)
	{
		// �����������H�̃��[�v
		for (const auto& findRoadLinkID : roadLinkSet)
		{
			auto findRoadLinkItr = roadLinkMap.find(findRoadLinkID);
			if (findRoadLinkItr == roadLinkMap.end())
				continue;

			findRoadLinkObj = findRoadLinkItr->second;

			// �`���v����܊֌W
			if (AheIsContain2(findRoadLinkObj.m_ipGeometry, walkLinkObj.m_ipGeometry))
			{
				// From/To�ȊO�̍\���_����v���Ă��邩�`�F�b�N
				bool allPointMatch = true;
				// ���H�����N�̍\���_���擾
				IPointCollectionPtr ipRoadLinkPointCol(findRoadLinkObj.m_ipGeometry);
				long roadLinkPointCnt = ipRoadLinkPointCol->GetPointCount();

				// ���H�����N�̍\���_�̓��A���s�҃����N�Ɋ܂܂��|�C���g���擾
				auto overlapConfPoint = util::getOverlapConfigurationPoints(findRoadLinkObj.m_ipGeometry, walkLinkObj.m_ipGeometry, m_coordinateTolerance);
				for (const auto& roadConfPoint : overlapConfPoint)
				{
					bool bPointMatched = false;
					for (long i = 0; i < walkLinkPointCnt; ++i)
					{
						IPointPtr walkPoint = ipWalkLinkPointCol->GetPoint(i);
						if (AheIsEqual(roadConfPoint, walkPoint))
						{
							bPointMatched = true;
							break;
						}
					}
					if (!bPointMatched)
					{
						allPointMatch = false;
						break;
					}
				}
				// ���s�҃����N�̍\���_�����ׂē��H�����N�̂����ꂩ�̍\���_�ƃ}�b�`���Ă��邩�`�F�b�N
				for (long i = 1; allPointMatch && (i < walkLinkPointCnt - 1); ++i)
				{
					IPointPtr ipPoint = ipWalkLinkPointCol->GetPoint(i);
					bool pointMatch = false;
					for (long j = 1; j < roadLinkPointCnt - 1; ++j)
					{
						IPointPtr roadPoint = ipRoadLinkPointCol->GetPoint(j);
						if (ipPoint->GetX() == roadPoint->GetX() &&
							ipPoint->GetY() == roadPoint->GetY())
						{
							pointMatch = true;
							break;
						}
					}
					allPointMatch &= pointMatch;
				}
				// �l�ԋ��p�����N�̏ꍇ�A�[�_�����H�����N�Ɋ܂܂�邩�`�F�b�N
				// (allPointMatch�����ł�false�ɂȂ��Ă���Ȃ�A���̔���͕s�v)
				// �[�_�͓��H�����N�Ɋ܂܂�邩�A���H�����N�̒[�_�ʒu�Ɠ����łȂ���΂Ȃ�Ȃ�
				if (allPointMatch)
					allPointMatch &= util::isLinkEdgeOnLink(walkLinkObj.m_ipGeometry, findRoadLinkObj.m_ipGeometry, m_coordinateTolerance);

				if (allPointMatch)
					return true;
			}
		}
	}
	else
	{
		// �����������H�̃��[�v
		for (const auto& findRoadLinkID : roadLinkSet)
		{
			auto findRoadLinkItr = roadLinkMap.find(findRoadLinkID);
			if (findRoadLinkItr == roadLinkMap.end())
				continue;
			// �`���v����܊֌W
			if (AheIsOverlap2(findRoadLinkItr->second.m_ipGeometry, walkLinkObj.m_ipGeometry))
			{
				findRoadLinkObj = findRoadLinkItr->second;
				return true;
			}
		}
	}
	return false;
}

void WalkLinkCheckFunction::checkMatching(
	const GeoClass& walkLinkObj,
	const IPointCollectionPtr& ipWalkLinkPointCol,
	long walkLinkPointCnt,
	long walkLinkClass,
	const FeaturesPack& walkLink,
	const FeaturesPack& roadLink,
	const FeaturesPack& roadNode,
	const BGRTree& walkLinkRTree,
	const BGRTree& roadLinkRTree,
	const BGRTree& roadNodeRTree,
	const CString& meshcode,
	bool bOther
)
{
	using namespace uh::str_util;
	auto findRoadLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), roadLinkRTree);
	GeoClass findRoadLinkObj;
	bool matchLink = isWalkLinkOnRoadLink(findRoadLinkObj, walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt, walkLinkClass,
		findRoadLinkIDs, roadLink.m_FeatureMap);

	// �l�ԋ��p�����N�̏ꍇ
	if (walkLinkClass == walk_link::walk_class::kWalkableRoad)
	{
		if (!matchLink) // ���G���[ �l�ԋ��p�����N�����H�����N�Ƀ}�b�`���Ă��Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::UnMatchRoadLink);
		else if (!bOther)
		{
			if (findRoadLinkObj.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kWalkable)) != road_link::walkable_class::kNotWalk)
				// ���G���[ �l�ԋ��p�����N�ƈ�v���铹�H�����N�����s�֎~�ȊO
			{
				auto modOperator_RL = findRoadLinkObj.GetStringValue(roadLink.m_FieldMap.FindField(road_link::kOperator));
				auto modDate_RL = findRoadLinkObj.GetStringValue(roadLink.m_FieldMap.FindField(road_link::kModifyDate));
				auto modProgram_RL = findRoadLinkObj.GetStringValue(roadLink.m_FieldMap.FindField(road_link::kModifyProgName));
				auto modProgramDate_RL = findRoadLinkObj.GetStringValue(roadLink.m_FieldMap.FindField(road_link::kProgModifyDate));

				m_output.OutputLog(road_link::kTableName,
					findRoadLinkObj.m_ObjectID, err_level::error,
					err_code::NgRoadLinkWalkable, meshcode, modOperator_RL, modDate_RL, modProgram_RL, modProgramDate_RL,
					ToString(findRoadLinkObj.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kWalkable))));
			}
		}
	}
	// �l�ԋ��p�����N�ȊO�̏ꍇ
	else if (m_dataType == walk_type::type_class::kGeneral && matchLink) // ���G���[ �l�ԋ��p�����N�ł͂Ȃ��̂ɓ��H�����N�Ƀ}�b�`���Ă���
		m_output.OutputRegLog(err_level::error, err_code::MatchRoadLink,
			judge_value::IsFindObj(walkLinkClass, m_walkLinkClassMap));

	// �_�E���R���o�[�g�����ꍇ�A���W�J���R�l�N�V�����ȊO�̕��s�҃����N��ΏۂƂ���A���s�҃����N�̈ꕔ�A���邢�͑S�������H�����N�ɏd�Ȃ��Ă���ꍇ�G���[
	if (m_dataType == walk_type::type_class::kDownConvert
		&& walkLinkClass != walk_link::walk_class::kLogicalConnection && matchLink)
		m_output.OutputRegLog(err_level::error, err_code::MatchRoadLink,
			judge_value::IsFindObj(walkLinkClass, m_walkLinkClassMap), _T(""), 0.0, 0.0, false);

	// �ėp�����ꍇ �l�ԋ��p�����N�ȊO�̕��s�҃����N�ɓ��H�m�[�h���܂܂�Ă���ꍇ�G���[
	if (m_dataType == walk_type::type_class::kGeneral && walk_link::walk_class::kWalkableRoad == walkLinkClass)
		return;
	// Error code:352, "���s�҃����N�ɓ��H�m�[�h���܂܂�Ă���"�̃`�G�b�N
	long modOperator_RNIndex = roadNode.m_FieldMap.FindField(road_node::kOperator);
	long modDate_RNIndex = roadNode.m_FieldMap.FindField(walk_link::kModifyDate);
	long modProgram_RNIndex = roadNode.m_FieldMap.FindField(walk_link::kModifyProgName);
	long modProgramDate_RNIndex = roadNode.m_FieldMap.FindField(walk_link::kProgModifyDate);
	auto findRoadNodeIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), roadNodeRTree);
	for (auto& roadNodeID : findRoadNodeIDs)
	{
		GeoClass roadNodeObj = roadNode.m_FeatureMap.at(roadNodeID);
		if (roadNodeObj.IsWithin(walkLinkObj.m_ipGeometry)) // ���G���[ ���H�m�[�h�����s�҃����N�Ɋ܂܂�Ă���
		{
			auto modOperator_RN = roadNodeObj.GetStringValue(modOperator_RNIndex);
			auto modDate_RN = roadNodeObj.GetStringValue(modDate_RNIndex);
			auto modProgram_RN = roadNodeObj.GetStringValue(modProgram_RNIndex);
			auto modProgramDate_RN = roadNodeObj.GetStringValue(modProgramDate_RNIndex);

			m_output.OutputLog(road_node::kTableName,
				roadNodeObj.m_ObjectID, err_level::error,
				err_code::WithInRoadNode, meshcode, modOperator_RN, modDate_RN, modProgram_RN, modProgramDate_RN,
				ToString(walkLinkObj.m_ObjectID));
		}
	}
	// �ėp�����ꍇ�A���s�҃����N�i�l�ԋ��p�����N�������j
	if (m_dataType == walk_type::type_class::kGeneral && walk_link::walk_class::kWalkableRoad == walkLinkClass)
		return;
	// �_�E���R���o�[�g�����ꍇ�A���s�҃����N�i�ڑ������N�E���W�J���R�l�N�V�����������j
	if (m_dataType == walk_type::type_class::kDownConvert && (walk_link::walk_class::kConnect == walkLinkClass || walk_link::walk_class::kLogicalConnection == walkLinkClass))
		return;

	// �ȉ��͐l�ԋ��p�����N�ȊO�̃`�F�b�N
	auto findWalkLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), walkLinkRTree);

	std::vector<GeoClass> crossPoints;
	// ���H�����N�A�܂��͕��s�҃����N�ƌ����ӏ���2�����ȏ゠��ꍇ�A�����ӏ��̋�������苗�����邩
	GetCrossPoint(walkLinkObj, findRoadLinkIDs, roadLink, crossPoints, false);
	GetCrossPoint(walkLinkObj, findWalkLinkIDs, walkLink, crossPoints, true);

	if (crossPoints.size() > 1)
	{
		for (unsigned int i = 0; i < crossPoints.size() - 1; ++i)
			for (unsigned int j = i + 1; j < crossPoints.size(); ++j)
			{
				// ����̌����ӏ��������o������ꍇ�����邽��
				if (crossPoints.at(i).m_ObjectID == crossPoints.at(j).m_ObjectID)
					if (crossPoints.at(i).IsEqual(crossPoints.at(j).m_ipGeometry))
						continue;

				if (m_dist_threshold > util::GetDistGeoClasses(crossPoints.at(i), crossPoints.at(j)))
					// ���G���[ 2�ȏ�̃����N�Ƃ̌����ӏ����߂�
				{
					double x = 0, y = 0;
					((IPointPtr)crossPoints.at(i).m_ipGeometry)->get_X(&x);
					((IPointPtr)crossPoints.at(i).m_ipGeometry)->get_Y(&y);

					// �ėp�����ꍇ
					if (m_dataType == walk_type::type_class::kGeneral)
						m_output.OutputRegLog(err_level::error, err_code::ThreeMoreCross,
							ToString(crossPoints.at(i).m_ObjectID),
							ToString(crossPoints.at(j).m_ObjectID), x, y);

					// �_�E���R���o�[�g�����ꍇ
					if (m_dataType == walk_type::type_class::kDownConvert)
					{
						CString info1(ToString(crossPoints.at(i).m_ObjectID) + _T("/") + ToString(crossPoints.at(j).m_ObjectID)), info2;

						info2.Format(_T("����:'%lf'm(臒l:'%lf'm)"), util::GetDistGeoClasses(crossPoints.at(i), crossPoints.at(j)), m_dist_threshold);
						m_output.OutputRegLog(err_level::error, err_code::ThreeMoreCross,
							info1, info2, x, y);
					}
				}
			}
	}
}

void WalkLinkCheckFunction::GetCrossPoint(
	const GeoClass& baseGeo,
	const set<long>& linkIDs,
	const FeaturesPack& link,
	std::vector<GeoClass>& crossPoints,
	bool isWalkLink)
{
	if (linkIDs.size() == 0)
		return;

	for (const auto& findLinkID : linkIDs) {
		auto findLinkItr = link.m_FeatureMap.find(findLinkID);
		if (findLinkItr == link.m_FeatureMap.end())
			continue;

		auto& findLinkObj = findLinkItr->second;

		// �����Ώۂ����s�҃����N�̏ꍇ
		if (isWalkLink)
		{
			// ������N�Ɠ���ID�̏ꍇ�X�L�b�v
			if (baseGeo.m_ObjectID == findLinkID)
				continue;

			// �ėp�����ꍇ�A�����N��ʂ��l�ԋ��p�����N�̏ꍇ�X�L�b�v
			long walkClass = findLinkObj.GetLongValue(link.m_FieldMap.FindField(walk_link::kWalkClass));
			if (m_dataType == walk_type::type_class::kGeneral && walkClass == walk_link::walk_class::kWalkableRoad)
				continue;

			// �_�E���R���o�[�g�����ꍇ�A�����Ώۂ����s�҃����N���ڑ������N�E���W�J���R�l�N�V�����̏ꍇ�X�L�b�v
			if (m_dataType == walk_type::type_class::kDownConvert && (walkClass == walk_link::walk_class::kConnect || walkClass == walk_link::walk_class::kLogicalConnection))
				continue;
		}
		// �����Ώۂ����H�����N�̏ꍇ
		else
		{
			// �����[�X�ΏۊO�̓X�L�b�v�ibug 11999�j
			if (!judge_value::IsOKReleaseRoadLink(findLinkObj, link.m_FieldMap))
				continue;
		}

		if (!baseGeo.IsCross(findLinkObj.m_ipGeometry))
			continue;

		BOOL isDisjoint = FALSE;
		IPointCollectionPtr ipPointCollect(AheGetCrossOrTouchPoints(baseGeo.m_ipGeometry, findLinkObj.m_ipGeometry, 0, &isDisjoint));
		if (isDisjoint || !ipPointCollect)
			continue;


		long pointCount = ipPointCollect->GetPointCount();
		for (long i = 0; i < pointCount; ++i)
		{
			GeoClass crsPoint;
			crsPoint.m_ipGeometry = ipPointCollect->GetPoint(i);
			crsPoint.m_ObjectID = findLinkID;
			crossPoints.push_back(crsPoint);
		}
	}
}

bool WalkLinkCheckFunction::ExistOpenAreaFlag(const list<GeoClass>& walkLinkList, const CFieldMap& fieldMap)
{
	using namespace walk_link;
	long openAreaIndex = fieldMap.FindField(kOpenAreaFlag);
	for (const auto& walkLink : walkLinkList)
	{
		if (FLAG_ON == walkLink.GetLongValue(openAreaIndex))
			return true;
	}

	return false;
}

bool WalkLinkCheckFunction::ExistHumanCarLink(const list<GeoClass>& walkLinkList, const CFieldMap& fieldMap)
{
	using namespace walk_link;
	long walkClassIndex = fieldMap.FindField(kWalkClass);
	for (const auto& walkLink : walkLinkList)
	{
		long walkClass = walkLink.GetLongValue(walkClassIndex);
		if (walkClass == walk_class::kWalkableRoad)
			return true;
	}

	return false;
}

void WalkLinkCheckFunction::SearchConnectLinks(
	long baseLinkID,
	long nodeID,
	const std::set<long>& searchAreaLinkSet,
	const NODEIDMAP& nodeLinkIdMap,
	const FeaturesPack& walkLinkMap,
	std::map<long, long>& findLinkMap)
{
	// TODO:ArcHelper��AheGetConnectedNetworkLink�iAheNetwork.h�j�𗘗p�����@�ɐ؂�ւ���B

	auto searchItr = nodeLinkIdMap.find(nodeID);
	if (searchItr == nodeLinkIdMap.end())
		return;

	for (const auto searchID : searchItr->second)
	{
		// �����Ώۂ�������N�̏ꍇ�X�L�b�v
		if (searchID == baseLinkID)
			continue;

		// �����Ώۂ������͈͊O�̏ꍇ�X�L�b�v
		if (searchAreaLinkSet.find(searchID) == searchAreaLinkSet.end())
			continue;

		auto searchObjectItr = walkLinkMap.m_FeatureMap.find(searchID);
		if (searchObjectItr == walkLinkMap.m_FeatureMap.end())
			continue;

		long searchFromNodeID = searchObjectItr->second.GetLongValue(walkLinkMap.m_FieldMap.FindField(walk_link::kFromNodeID));
		long searchToNodeID = searchObjectItr->second.GetLongValue(walkLinkMap.m_FieldMap.FindField(walk_link::kToNodeID));

		long nextBaseNodeId = 0;
		if (nodeID == searchFromNodeID) nextBaseNodeId = searchToNodeID;
		else if (nodeID == searchToNodeID) nextBaseNodeId = searchFromNodeID;
		else continue;

		findLinkMap[searchID] = nextBaseNodeId;
	}

	return;
}

void WalkLinkCheckFunction::SearchThirdConnectLinks(
	long baseLinkID,
	long nodeID,
	const std::set<long>& searchAreaLinkSet,
	const NODEIDMAP& nodeLinkIdMap,
	const FeaturesPack& walkLink,
	std::map<long, long>& findLinkMap)
{
	map<long, long> firstFindLinkMap;
	SearchConnectLinks(baseLinkID, nodeID, searchAreaLinkSet, nodeLinkIdMap, walkLink, firstFindLinkMap);
	for (const auto& firstFindLink : firstFindLinkMap)
	{
		map<long, long> secondFindLinkMap;
		SearchConnectLinks(firstFindLink.first, firstFindLink.second, searchAreaLinkSet, nodeLinkIdMap, walkLink, secondFindLinkMap);
		for (const auto& secondFindLink : secondFindLinkMap)
		{
			SearchConnectLinks(secondFindLink.first, secondFindLink.second, searchAreaLinkSet, nodeLinkIdMap, walkLink, findLinkMap);
		}
	}

	return;
}

std::set<long> WalkLinkCheckFunction::checkFloorMoveConnectUnder(
	long underLinkID
	, const std::list<GeoClass>& connectLinkMap
	, const GeoClass& walkNodeObj
	, const CFieldMap &fieldMap
	, const FeaturesPack& walkLink)
{
	using namespace sindy::schema::walk_link;

	// �n���ʘH�̊K�w�ړ���ʂ��擾
	const long underFloorMove = walkLink.m_FeatureMap.at(underLinkID).GetLongValue(walkLink.m_FieldMap.FindField(walk_link::kFloorMoveClass));

	long nodeClassIndex = fieldMap.FindField(walk_node::kNodeClass);
	long floorMoveIndex = walkLink.m_FieldMap.FindField(walk_link::kFloorMoveClass);
	std::set<long> errLinks; // �G���[�ƂȂ郊���NID�Q���i�[
	for (auto& checkedLink : connectLinkMap)
	{
		// �n���ʘH�Ɖ��f�n�����̓`�F�b�N�ΏۊO�ibug 12000�j
		// �u�n�����D���v�u�n���v���b�g�z�[���v���ΏۊO�ibug 12738�j
		long walkClass = checkedLink.GetLongValue(walkLink.m_FieldMap.FindField(walk_link::kWalkClass));
		if (judge_value::IsUnderGroundLink(walkClass))
			continue;

		// �G���x�[�^�[�m�[�h���ڑ����Ă���Ȃ�A�G���[�ɂ͂Ȃ�Ȃ�
		if (walk_node::node_class::kElevator == walkNodeObj.GetLongValue(nodeClassIndex))
			continue;

		// �K�w�ړ���ʂ̌������m�F
		long floorMove = checkedLink.GetLongValue(floorMoveIndex);

		// �n���ʘH�Ǝn�_���Őڑ����Ă��邩�ۂ�
		if (util::isFromNode(checkedLink.m_ObjectID, walkLink, walkNodeObj.m_ObjectID))
		{
			// �n�_�̏ꍇ�A�K�w�ړ���ʂ��������łȂ����̂��G���[
			if (walk_link::floor_move_class::kForeUp == floorMove)
				continue;
		}
		else
		{
			// �I�_�̏ꍇ�A�K�w�ړ���ʂ��t�����łȂ����̂��G���[
			if (walk_link::floor_move_class::kReverseUp == floorMove)
				continue;
		}

		// �����N�ɊK�w�ړ���ʂ��t�^����Ă��Ȃ��ꍇ�́A
		// �ڑ����Ă���n���ʘH�����N�̊K�w�ړ���ʂ̌������������Ȃ�G���[�ł͂Ȃ�
		if (walk_link::floor_move_class::kNone == floorMove)
		{
			// �n���ʘH�����N���n�_���Őڑ����Ă��邩�ۂ�
			if (util::isFromNode(underLinkID, walkLink, walkNodeObj.m_ObjectID))
			{
				// �n�_�Őڑ����Ă���ꍇ�A�n���ʘH�����N�̊K�w�ړ���ʂ��t�����łȂ����̂��G���[
				if (walk_link::floor_move_class::kReverseUp == underFloorMove)
					continue;
			}
			else
			{
				// �I�_�Őڑ����Ă���ꍇ�A�n���ʘH�����N�̊K�w�ړ���ʂ��������łȂ����̂��G���[
				if (walk_link::floor_move_class::kForeUp == underFloorMove)
					continue;
			}
		}
		errLinks.insert(checkedLink.m_ObjectID);
	}
	return errLinks;
}

void WalkLinkCheckFunction::checkPointOnOtherLink(
	const std::vector<long>& checkedLinkId
	, const FeaturesPack& walkLink
	, const CString& meshcode)
{
	using namespace sindy::schema::walk_link;

	// �`�F�b�N�Ώۂ̍\���_��otherlink��ɂ��邩�ǂ����̃`�F�b�N�����郉���_��
	auto isPointOnOtherLink = [](
		const IPointCollectionPtr& checkedPointColl //!< �`�F�b�N�Ώۂ̍\���_
		, long checkedPointCount    //!< �`�F�b�N�Ώۂ̍\���_�̐�
		, const IGeometryPtr& otherlink   //!< �`�F�b�N�Ώۂ̍\���_������Ă͂����Ȃ������N
		, const IPointPtr& otherFromPoint //!< otherlink�̎n�_
		, const IPointPtr& otherToPoint   //!< otherlink�̏I�_
		, double coordinateTolerance  //!< �����N��Ƀ|�C���g������Ă��邩�ǂ����𔻒肷��Ƃ��̋��e�l
		)
	{
		for (long index = 0; index < checkedPointCount; ++index)
		{
			IPointPtr point;
			checkedPointColl->get_Point(index, &point);
			if (util::isPointOnLink(otherlink, point, coordinateTolerance))
			{
				// �[�_���m���q�����Ă���̂̓G���[�ł͂Ȃ�
				if (0 == index || checkedPointCount - 1 == index)
				{
					if (AheIsEqual(point, otherFromPoint) || AheIsEqual(point, otherToPoint))
						continue;
				}
				return true;
			}
		}
		return false;
	};

	std::set<long> onOtherLink; // �G���[�o�͑Ώۂ̃����NID
	for (auto itBaseID = checkedLinkId.begin(); itBaseID != checkedLinkId.end(); ++itBaseID)
	{
		auto& baseGeom = walkLink.m_FeatureMap.at(*itBaseID).m_ipGeometry;
		IPointCollectionPtr basePoint(baseGeom);
		long basePointCount = 0;
		basePoint->get_PointCount(&basePointCount);
		IPointPtr baseFromPoint = AheGetFromPoint(baseGeom);
		IPointPtr baseToPoint = AheGetToPoint(baseGeom);

		for (auto itTargetID = std::next(itBaseID); itTargetID != checkedLinkId.end(); ++itTargetID)
		{
			auto& targetGeom = walkLink.m_FeatureMap.at(*itTargetID).m_ipGeometry;
			if (AheIsDisjoint(baseGeom, targetGeom))
				continue;

			// �^�[�Q�b�g���̑S�\���_���擾
			IPointCollectionPtr targetPoint(targetGeom);
			long targetPointCount = 0;
			targetPoint->get_PointCount(&targetPointCount);

			IPointPtr targetFromPoint = AheGetFromPoint(targetGeom);
			IPointPtr targetToPoint = AheGetToPoint(targetGeom);
			// �x�[�X���ɃG���[�ƂȂ���̂��Ȃ����`�F�b�N
			if (isPointOnOtherLink(basePoint, basePointCount, targetGeom, targetFromPoint, targetToPoint, m_coordinateTolerance))
				onOtherLink.insert(*itBaseID);

			// �x�[�X���ɃG���[�ƂȂ���̂��Ȃ����`�F�b�N
			if (isPointOnOtherLink(targetPoint, targetPointCount, baseGeom, baseFromPoint, baseToPoint, m_coordinateTolerance))
				onOtherLink.insert(*itTargetID);
		}
	}
	// �G���[�o��
	long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
	for (const auto& linkId : onOtherLink)
	{
		auto& walkLinkObj = walkLink.m_FeatureMap.at(linkId);
		CString modOperator = walkLinkObj.GetStringValue(modOperatorIndex);
		CString modDate = walkLinkObj.GetStringValue(modDateIndex);
		CString modProgram = walkLinkObj.GetStringValue(modProgramIndex);
		CString modProgramDate = walkLinkObj.GetStringValue(modProgramDateIndex);
		m_output.OutputLog(kTableName, linkId, err_level::error, err_code::PointOnOtherLink,
			meshcode, modOperator, modDate, modProgram, modProgramDate);
	}
}

// ���b�V���O�̐ڑ����s�҃����N���擾����
std::set<long> WalkLinkCheckFunction::getAdjacentWalkLink(
	long& nodeId
	, const long linkId
	, const IGeometryPtr& ipMeshGeo
	, FeaturesPack& walkLink
	, FeaturesPack& walkNode
	, NODEIDMAP& nodeLinkIDs
)
{
	std::set<long> result;

	// �T���̋N�_�ƂȂ�m�[�h���L���b�V�����ꂽMap���ɑ��݂��Ȃ��A
	// �܂��́A�w��m�[�h�ɐڑ����郊���N��1�݂̂̏ꍇ�A
	// ���̃m�[�h��walkNode���b�V�����̃����N�E�m�[�h���������R���e�i�ɒǉ�����
	auto baseNodeIt = walkNode.m_FeatureMap.find(nodeId);
	if ((walkNode.m_FeatureMap.end() == baseNodeIt) ||
		util::isOnlyTargetWalkLink(baseNodeIt, linkId, nodeLinkIDs, walkNode.m_FieldMap.FindField(walk_node::kNodeClass)))
	{
		addMeshInfo(nodeId, ipMeshGeo,
			walkLink, walkNode, nodeLinkIDs);

		baseNodeIt = walkNode.m_FeatureMap.find(nodeId);
	}

	// �Č������Ă�nodeMap�ɑ��݂��Ȃ��ꍇ�́Areturn���Ă��܂�
	if (walkNode.m_FeatureMap.end() == baseNodeIt)
		return result;

	// �m�[�h��ʂ��[�_�m�[�h�̏ꍇ�A�T���I��
	if (walk_node::node_class::kTerminal == walkNode.m_FeatureMap.at(nodeId).GetLongValue(walkNode.m_FieldMap.FindField(walk_node::kNodeClass)))
		return result;

	std::set<long> linkIDSet = nodeLinkIDs.find(nodeId)->second;
	auto adjacentLink = util::GetObjectMap(linkId, linkIDSet, walkLink.m_FeatureMap);

	// ���E�m�[�h�ł͂Ȃ��ꍇ�A�T���I��
	if (!util::isMeshEdgeNode(nodeId, walkNode, true))
	{
		for (auto& linkInfo : adjacentLink)
			result.insert(linkInfo.first);
		return result;
	}

	// �ڑ������N�������i2���H�ł͂Ȃ��j�ꍇ�A�T���I��
	if (adjacentLink.size() > 1)
	{
		for (auto& linkInfo : adjacentLink)
			result.insert(linkInfo.first);
		return result;
	}

	// �����܂ł����ꍇ�A�w��m�[�h�ɐڑ����郊���N��0���ƂȂ邪�A
	// 0�����ƈȍ~�̏������s���Ȃ��̂ŁAreturn���Ă��܂�
	if (adjacentLink.empty())
		return result;

	// �ڑ������N�̔�ڑ����̒[�_�m�[�h���擾����
	long adjLinkId = adjacentLink.begin()->first;
	long otherNodeId = util::getOtherEdgeNode(adjLinkId, walkLink, nodeId);

	// ��ڑ����̃m�[�h���`�F�b�N�͈͊O�̏ꍇ�A���̏��݂��郁�b�V���̏��������R���e�i�[�ɒǉ�����

	auto otherNodeIt = walkNode.m_FeatureMap.find(otherNodeId);
	if ((walkNode.m_FeatureMap.end() == otherNodeIt) ||
		util::isOnlyTargetWalkLink(otherNodeIt, adjLinkId, nodeLinkIDs, walkNode.m_FieldMap.FindField(walk_node::kNodeClass)))
	{
		addMeshInfo(otherNodeId, ipMeshGeo,
			walkLink, walkNode, nodeLinkIDs);

		otherNodeIt = walkNode.m_FeatureMap.find(otherNodeId);
	}

	// �Č������Ă�nodeMap�ɑ��݂��Ȃ��ꍇ�́Areturn���Ă��܂�
	if (walkNode.m_FeatureMap.end() == otherNodeIt)
		return result;

	// 2�����b�V�����E���ׂ������N�ɑ΂��Autil::getAdjacentLink()�Œ[�_�m�[�hID���ύX�����̂ŁA
	// ��̃`�F�b�N�ɉe�����Ȃ��悤�ɁA�Ǐ��I�ȕϐ�tempNodeID���g��
	auto tempNodeID = otherNodeId;
	std::set<long> secondLinkIDSet = nodeLinkIDs.find(otherNodeId)->second;
	auto secondAdjLinks = util::GetObjectMap(adjLinkId, secondLinkIDSet, walkLink.m_FeatureMap);
	result = util::getAdjacentLink(tempNodeID, secondAdjLinks, walkLink, walkNode, nodeLinkIDs, true);

	if (result.empty())
	{
		// nodeId���T���̏I�_
		nodeId = otherNodeId;

		// �אڃ��b�V��������b�V���Ƃ��čċA�I�ɒT��
		std::set<long> meshOverId = getAdjacentWalkLink(
			otherNodeId, adjLinkId, ipMeshGeo,
			walkLink, walkNode, nodeLinkIDs);

		for (long linkId : meshOverId)
			result.insert(linkId);
	}
	else
	{
		nodeId = tempNodeID;
	}

	return result;
}

// �Ώۃm�[�h�̏��݂��郁�b�V���̏���ǉ�����
void WalkLinkCheckFunction::addMeshInfo(
	const long targetNodeId,
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeLinkIDs
)
{
	_IRowPtr ipRow;
	walkNode.m_ipTable->GetRow(targetNodeId, &ipRow);
	if (!ipRow)
	{
		// �w�肵��OID�����m�[�h�����݂��Ȃ��ꍇ�́A�ȍ~�̏����͍s��Ȃ�
		return;
	}
	auto targetNodeObj = GeoClass(ipRow);
	// �[�_�̍��W���擾����
	DOUBLE dx = 0.0f, dy = 0.0f;
	INT nMeshCode = 0, nMeshX = 0, nMeshY = 0;
	IPointPtr ipAdjPoint(targetNodeObj.m_ipGeometry);
	ipAdjPoint->QueryCoords(&dx, &dy);
	nMeshCode = AheLL2MESH(dx, dy, 64, nMeshX, nMeshY, 1);

	// �[�_���s�s�n�}���b�V���̋��E��
	bool isCityMeshEdge = false;
	if (0 == nMeshX || 0 == nMeshY)
		isCityMeshEdge = true;

	// �[�_�̏��݂���s�s�n�}���b�V�����擾����
	ISpatialReferencePtr ipSpRef;
	ipMeshGeo->get_SpatialReference(&ipSpRef);

	IGeometryPtr ipAdjMeshGeo, ipAdjMeshGeo2;
	ipAdjMeshGeo = AheMESH2POLY(nMeshCode, ipSpRef);

	// �[�_�m�[�h���s�s�n�}���b�V���̋��E��̏ꍇ�A�אڂ̃��b�V�����擾����K�v������
	// ��ƉE�̋��E�m�[�h�́AAheLL2MESH�ŕʂ̃��b�V���R�[�h�ɕϊ������̂ŁA�����ł͍��Ɖ�������������
	if (isCityMeshEdge)
	{
		long adjMeshCode = 0;

		// X��Y�������Ƃ�0�̏ꍇ�A���A���A�����̂R���b�V�����擾
		if (0 == nMeshX && 0 == nMeshY)
		{
			adjMeshCode = AheGETMESHCODE(nMeshCode, -1, -1);
			ipAdjMeshGeo2 = AheMESH2POLY(adjMeshCode, ipSpRef);

			adjMeshCode = AheDECMESH_X(nMeshCode);
			IGeometryPtr ipLeftMeshGeo = AheMESH2POLY(adjMeshCode, ipSpRef);
			AheUnion(ipAdjMeshGeo2, ipLeftMeshGeo, ipSpRef);

			adjMeshCode = AheDECMESH_Y(nMeshCode);
			IGeometryPtr ipDownMeshGeo = AheMESH2POLY(adjMeshCode, ipSpRef);
			AheUnion(ipAdjMeshGeo2, ipDownMeshGeo, ipSpRef);
		}
		else
		{
			if (0 == nMeshX)
				adjMeshCode = AheDECMESH_X(nMeshCode);
			if (0 == nMeshY)
				adjMeshCode = AheDECMESH_Y(nMeshCode);
			ipAdjMeshGeo2 = AheMESH2POLY(nMeshCode, ipSpRef);
		}
	}

	AheUnion(ipAdjMeshGeo, ipAdjMeshGeo2, ipSpRef);

	// ���s�҃����N�擾
	if (!util::GetLinkFeaturesInGeometry(ipAdjMeshGeo, field_lists::walkLinkFieldList, _T(""), walkLink, walkNodeLinkIDs))
		return;

	// ���s�҃m�[�h�擾
	if (!util::GetFeaturesInGeometry(ipAdjMeshGeo, field_lists::walkNodeFieldList, _T(""), walkNode))
		return;
}

void WalkLinkCheckFunction::attributesCheck(
	long walkLinkClass,
	long floorMoveClass,
	long onewayCode,
	long noPassCode,
	long roadSurfaceCode,
	long walkCode,
	bool isForRelease
)
{
	using namespace uh::str_util;
	// ���P���ȑ����`�F�b�N
	// �e�����l����`�����m�F����
	if (!judge_value::IsDefineWalkLinkClass(walkLinkClass, m_dataType))   // ���G���[ ���s�Ҏ�ʂ���`�O
	{
		if (m_dataType == walk_type::type_class::kGeneral)
			m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkClass1, ToString(walkLinkClass));
		if (m_dataType == walk_type::type_class::kDownConvert)
			m_output.OutputRegLog(err_level::fatal, err_code::NgWalkLinkClass1, ToString(walkLinkClass), _T(""), 0.0, 0.0, false);
	}


	if (!judge_value::IsDefineOnewayCode(onewayCode))         // ���G���[ ����ʍs��ʃR�[�h����`�O
		m_output.OutputRegLog(err_level::error, err_code::NgOnewayCode1, ToString(onewayCode));

	if (!judge_value::IsDefineNoPassCode(noPassCode))         // ���G���[ ���s�Ғʍs�֎~�R�[�h����`�O
		m_output.OutputRegLog(err_level::error, err_code::NgNoPassCode, ToString(noPassCode));

	if (!isForRelease) {
		if (!judge_value::IsDefineFloorMoveClass(floorMoveClass)) // ���G���[ �K�w�ړ���ʃR�[�h����`�O
			m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveClass, ToString(floorMoveClass));

		if (!judge_value::IsDefineRoadSurfaceClass(roadSurfaceCode))     // ���G���[ �H�ʑf�ރR�[�h����`�O
			m_output.OutputRegLog(err_level::error, err_code::NgRoadSurfaceClass, ToString(roadSurfaceCode));

		if (!judge_value::IsDefineWalkCode(walkCode))             // ���G���[ ���s�ҘH���R�[�h����`�O
			m_output.OutputRegLog(err_level::error, err_code::NgWalkCodeC, ToString(walkCode));
	}

	// ������OK�Ȓl���m�F����
	if (!judge_value::IsOKWalkLinkClass(walkLinkClass)) // ���G���[ ���s�Ҏ�ʂ���`�O
		m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkClass2, judge_value::IsFindObj(walkLinkClass, m_walkLinkClassMap));
}

void WalkLinkCheckFunction::flagsCheck(
	long autoWalkF,
	long stairF,
	long slopeF,
	long escalatorF,
	long arcadeF,
	long openAreaF,
	long tunnelF,
	long scrambleF,
	long onewayRF,
	long noPassRF
)
{
	using namespace walk_link;
	CString info1;
	// �e�t���O�t�B�[���h��0��1�ȊO���������m�F����
	// ���G���[ �t���O�t�B�[���h�Ȃ̂ɁA[0]��[1]�ȊO�̒l
	if (!judge_value::IsOKFlag(autoWalkF)) {
		info1.Format(_T("%s(%d)"), kAutoWalkFlag, autoWalkF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(stairF)) {
		info1.Format(_T("%s(%d)"), kStairFlag, stairF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(slopeF)) {
		info1.Format(_T("%s(%d)"), kSlopeFlag, slopeF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(escalatorF)) {
		info1.Format(_T("%s(%d)"), kEscalatorFlag, escalatorF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(arcadeF)) {
		info1.Format(_T("%s(%d)"), kArcadeFlag, arcadeF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(openAreaF)) {
		info1.Format(_T("%s(%d)"), kOpenAreaFlag, openAreaF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(tunnelF)) {
		info1.Format(_T("%s(%d)"), kTunnelFlag, tunnelF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(scrambleF)) {
		info1.Format(_T("%s(%d)"), kScrambleFlag, scrambleF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(onewayRF)) {
		info1.Format(_T("%s(%d)"), kOnewayRF, onewayRF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
	if (!judge_value::IsOKFlag(noPassRF)) {
		info1.Format(_T("%s(%d)"), kNoPassageRF, noPassRF);
		m_output.OutputRegLog(err_level::error, err_code::NgFlagField, info1);
	}
}

void WalkLinkCheckFunction::fromToIdCheck(
	long walkLinkID,
	long fromWalkNodeID,
	long toWalkNodeID,
	map<pair<long, long>, list<long>>& dupNodeMap
)
{
	using namespace uh::str_util;
	// From/To��ID���m�F�@
	bool bNodeIDErr = false;
	if (fromWalkNodeID <= 0) { // ���G���[ �n�_�m�[�hID���s��
		m_output.OutputRegLog(err_level::error, err_code::NgFromID, ToString(fromWalkNodeID));
		bNodeIDErr = true;
	}
	if (toWalkNodeID <= 0) {   // ���G���[ �I�_�m�[�hID���s��
		m_output.OutputRegLog(err_level::error, err_code::NgToID, ToString(toWalkNodeID));
		bNodeIDErr = true;
	}
	if (fromWalkNodeID == toWalkNodeID) { // ���G���[ �����N�̎n�I�_���������i�����j
		m_output.OutputRegLog(err_level::error, err_code::LoopLinkSingleFT, _T("����"));
		bNodeIDErr = true;
	}
	// ����From/To�����݂��Ȃ����m�F�̂��߁A����Ȃ��̂̓m�[�hID�̃y�A���L�[�ɁA�����NID���i�[���Ă���
	if (!bNodeIDErr) {
		if (fromWalkNodeID < toWalkNodeID)
			dupNodeMap[make_pair(fromWalkNodeID, toWalkNodeID)].push_back(walkLinkID);
		else
			dupNodeMap[make_pair(toWalkNodeID, fromWalkNodeID)].push_back(walkLinkID);
	}
}

bool WalkLinkCheckFunction::shapeCheck(
	const GeoClass& walkLinkObj,
	const FeaturesPack& walkNode,
	long walkLinkClass,
	long fromWalkNodeID,
	long toWalkNodeID,
	long walkNodeClassIndex,
	std::vector<long>& targetWalkLinkId,
	GeoClass& fromWalkNodeObj,
	GeoClass& toWalkNodeObj
)
{
	using namespace walk_link;
	using namespace uh::str_util;

	// TODO�@�`�󑶍݂��Ȃ��ꍇ�ɁA���̃\�[�X���W�b�N��肱�̃G���[�`�F�b�N�����ɓ���Ȃ��̂ŁA�G���[�R�[�h313�G���[���b�Z�[�W���G���[���O�t�@�C���ɏo���Ȃ��B
	// ����������`����g�����`�F�b�N
	// �P���Ȍ`��m�F
	esriGeometryType eType = (walkLinkObj.m_ipGeometry)->GetGeometryType();
	IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
	long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
	if (!walkLinkObj.m_ipGeometry
		|| !ipWalkLinkPointCol
		|| walkLinkPointCnt < 2
		|| eType != esriGeometryPolyline) {
		// ���G���[ ���s�҃����N�`�󂪕s��
		m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkGeom);
		return false;
	}
	// �`�󂪐��������̂�360�`�F�b�N�Ŏg�p����̂ŁA�����ŕێ����Ă���
	targetWalkLinkId.push_back(walkLinkObj.m_ObjectID);

	// From/To�m�[�h�擾
	auto fromNodeItr = walkNode.m_FeatureMap.find(fromWalkNodeID);
	auto toNodeItr = walkNode.m_FeatureMap.find(toWalkNodeID);

	// From�m�[�h�̑��݊m�F
	fromWalkNodeObj = util::SearchObj(walkNode, fromWalkNodeID);
	if (!fromWalkNodeObj.IsOk()) // ���G���[ FromNodeID�̃m�[�h��������Ȃ�
	{
		m_output.OutputRegLog(err_level::error, err_code::NotFoundFromNode, ToString(fromWalkNodeID));
	}

	// To�m�[�h�̑��݊m�F
	toWalkNodeObj = util::SearchObj(walkNode, toWalkNodeID);
	if (!toWalkNodeObj.IsOk()) // ���G���[ ToNodeID�̃m�[�h��������Ȃ�
	{
		m_output.OutputRegLog(err_level::error, err_code::NotFoundToNode, ToString(toWalkNodeID));
	}
	if (fromWalkNodeObj.IsOk() && toWalkNodeObj.IsOk()) {
		// From/To�̌`�󂪈�v���Ă��Ȃ����m�F
		if (fromWalkNodeObj.IsEqual(toWalkNodeObj.m_ipGeometry)) // ���G���[ �����N�̎n�I�_���������i�`��j
			m_output.OutputRegLog(err_level::error, err_code::LoopLinkSingleFT, _T("�`��"));

		// �l�ԋ��p�����N��From/To�̃m�[�h��ʂ������Ƃ����HNW�ڑ��m�[�h�łȂ����m�F
		if (walkLinkClass == walk_class::kWalkableRoad &&
			fromWalkNodeObj.GetLongValue(walkNodeClassIndex) == walk_node::node_class::kRoadNW &&
			toWalkNodeObj.GetLongValue(walkNodeClassIndex) == walk_node::node_class::kRoadNW)
			// ���G���[ From�m�[�h,To�m�[�h���ɓ��HNW�ڑ��m�[�h
			m_output.OutputRegLog(err_level::error, err_code::BothRoadNW);
	}
	return true;
}

// TOD: ���O�͂����ƍl����
void WalkLinkCheckFunction::shapeCheck2(
	const GeoClass& walkLinkObj,
	const GeoClass& fromNodeObj,
	const GeoClass& toNodeObj,
	bool isForRelease
)
{
	IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
	long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
	CString info1, info2;
	// �_��ɕ������Ċm�F
	double cmpX = 0.0, cmpY = 0.0;
	double cmpX2 = 0.0, cmpY2 = 0.0;
	map<pair<double, double>, vector<long>> coordsMap; // �L�[�Fxy�A�l�F�\���_�ԍ�
	for (long i = 0; i < walkLinkPointCnt; ++i) {

		IPointPtr ipPoint = ipWalkLinkPointCol->GetPoint(i);

		// �n�_��From�m�[�h�̈�v���m�F
		if (i == 0 && fromNodeObj.IsOk()) {
			if (!fromNodeObj.IsEqual((IGeometryPtr)ipPoint)) // ���G���[ �����N�̎n�_��From�m�[�h�̍��W����v���Ȃ�
				m_output.OutputRegLog(err_level::error, err_code::UnMatchStartFrom);
		}

		// �I�_��To�m�[�h�̈�v���m�F
		if (i == walkLinkPointCnt - 1 && toNodeObj.IsOk()) {
			if (!toNodeObj.IsEqual((IGeometryPtr)ipPoint)) // ���G���[ �����N�̏I�_��To�m�[�h�̍��W����v���Ȃ�
				m_output.OutputRegLog(err_level::error, err_code::UnMatchEndTo);
		}

		// �Z�O�����g�����m�F
		double x = 0.0, y = 0.0;
		ipPoint->QueryCoords(&x, &y);
		if (!isForRelease &&  i != 0) {
			double dist = g_cnv.GetDist(cmpX, cmpY, x, y);
			if (dist <= m_dist_threshold) {
				// ���G���[ �����Z�O�����g�����݂���
				info1.Format(_T("'%ld'�Ԃ�'%ld'��"), i - 1, i);
				info2.Format(_T("'%lf'm�ȉ��i'%lf'm�j"), m_dist_threshold, dist);
				m_output.OutputRegLog(err_level::error, err_code::UnderThresholdSeg, info1, info2);
			}
		}

		// xy���L�[�Ƃ��āA�\���_�ԍ����l�߂�
		coordsMap[make_pair(x, y)].push_back(i);

		// �p�x�m�F
		if (!isForRelease && i > 1) {
			double angle = util::angle(cmpX2, cmpY2, cmpX, cmpY, x, y);
			if (angle < 0)
				angle = angle * -1;

			if (angle <= m_angle_threshold) {
				// ���G���[ �}���ȃ^�[�������݂���
				info1.Format(_T("'%ld'��"), i - 1);
				info2.Format(_T("'%lf'���ȉ��i'%lf'���j"), m_angle_threshold, angle);
				m_output.OutputRegLog(err_level::error, err_code::UnderThresholdAg, info1, info2);
			}
		}
		cmpX2 = cmpX;
		cmpY2 = cmpY;
		cmpX = x;
		cmpY = y;
	}

	// �\���_�d���̊m�F
	for (const auto& coord : coordsMap) {
		if (coord.second.size() > 1) {
			// ���G���[ �\���_���d�����Ă���
			auto itr = coord.second.begin();
			info1.Format(_T("'%ld'�Ԃ�'%ld'��"), *(itr), *(itr + 1));
			m_output.OutputRegLog(err_level::error, err_code::DupPoint, info1);
		}
	}
}

void WalkLinkCheckFunction::inferredCrossRoadCheck(
	const GeoClass& walkLinkObj,
	long walkLinkClass,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree
)
{
	using namespace walk_link;
	using namespace uh::str_util;
	// �݂Ȃ����f�������������Ă��铹�H�����N��2�Ԑ��ȏ�
	if (walkLinkClass == walk_class::kInferredCrossRoad)
	{
		auto findRoadLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), roadLinkRTree);

		long maxLaneCount = 0;
		long maxRoadLinkOID = 0;
		long laneCountIndex = roadLink.m_FieldMap.FindField(road_link::kLaneCount);
		for (const auto& findRoadLinkID : findRoadLinkIDs) {
			auto findRoadLinkItr = roadLink.m_FeatureMap.find(findRoadLinkID);
			if (findRoadLinkItr == roadLink.m_FeatureMap.end())
				continue;
			auto& findRoadLinkObj = findRoadLinkItr->second;
			if (walkLinkObj.IsCross(findRoadLinkObj.m_ipGeometry))
			{
				long laneCount = findRoadLinkObj.GetLongValue(laneCountIndex);
				if (maxLaneCount < laneCount)
				{
					maxLaneCount = laneCount;
					maxRoadLinkOID = findRoadLinkID;
				}
			}
		}
		if (maxLaneCount >= 2) //�����[�j���O �݂Ȃ����f�������������Ă��铹�H�����N��2�Ԑ��ȏ�
			m_output.OutputRegLog(err_level::warning, err_code::CrossMultiLane, ToString(maxRoadLinkOID));
	}

}

void WalkLinkCheckFunction::attributeConformityCheck(
	long floorMoveClass,
	long stairF,
	long slopeF,
	long escalatorF,
	long onewayCode,
	long autoWalkF,
	long arcadeF,
	long openAreaF,
	long tunnelF,
	long scrambleF,
	long walkLinkClass,
	long walkCode
)
{
	using namespace uh::str_util;
	using namespace walk_link;
	// �K�i�t���O�A�X���[�v�t���O�A�G�X�J���[�^�t���O�����Ă�ꍇ�A�K�w�ړ���ʂ��u0�F�㉺�����v�ȊO���m�F
	if (!judge_value::IsOKFloorMoveClassAndFlag(floorMoveClass, stairF))
		// ���G���[ �K�w�ړ���ʂ��t�^����Ă��Ȃ��iSTAIR_F�j
		m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveAndF, kStairFlag);

	else if (!judge_value::IsOKFloorMoveClassAndFlag(floorMoveClass, slopeF))
		// ���G���[ �K�w�ړ���ʂ��t�^����Ă��Ȃ��iSLOPE_F�j
		m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveAndF, kSlopeFlag);

	else if (!judge_value::IsOKFloorMoveClassAndFlag(floorMoveClass, escalatorF))
		// ���G���[ �K�w�ړ���ʂ��t�^����Ă��Ȃ��iESCALATOR_F�j
		m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveAndF, kEscalatorFlag);

	// �K�i�t���O�A�X���[�v�t���O�A�G�X�J���[�^�t���O�����Ă��Ȃ��ꍇ�A�K�w�ړ���ʂ��u0�F�㉺�����v�ł��邩�m�F
	int floorMoveFlag = stairF | slopeF | escalatorF;

	if (m_dataType == walk_type::type_class::kDownConvert) floorMoveFlag |= autoWalkF;

	if (!judge_value::IsOKFloorMoveClassAndFlagOFF(floorMoveClass, floorMoveFlag))
		// ���G���[ �K�w�ړ���ʂ��t�^����Ă���
		m_output.OutputRegLog(err_level::error, err_code::ExistFloorMove);

	// �I�[�g�E�H�[�N�t���O�A�G�X�J���[�^�t���O�����Ă�ꍇ�A����ʍs��ʂ��t�^���ꂢ�邩�m�F
	if (!judge_value::IsOKOnewayCodeAndFlag(onewayCode, autoWalkF))
		// ���G���[ ����ʍs��ʂ��t�^����Ă��Ȃ��iAUTOWALK_F�j
		m_output.OutputRegLog(err_level::error, err_code::NotOnewayCode, kAutoWalkFlag);

	else if (!judge_value::IsOKOnewayCodeAndFlag(onewayCode, escalatorF))
		// ���G���[ ����ʍs��ʂ��t�^����Ă��Ȃ��iESCALATOR_F�j
		m_output.OutputRegLog(err_level::error, err_code::NotOnewayCode, kEscalatorFlag);

	// �I�[�g�E�H�[�N�t���O�A�G�X�J���[�^�t���O�����Ȃ��ꍇ�A����ʍs��ʂ��t�^����Ă��Ȃ����m�F
	int onwayFlag = autoWalkF | escalatorF;
	if (!judge_value::IsOKFloorMoveClassAndFlagOFF(onewayCode, onwayFlag))
		// ���G���[ ����ʍs��ʂ��t�^����Ă���
		m_output.OutputRegLog(err_level::warning, err_code::ExistOnewayCode);

	// �G�X�J���[�^�[�t���O �܂��� �I�[�g�E�H�[�N�t���O��ON�ŉ����t���t���O��OFF�ł͂Ȃ����m�F
	if (!judge_value::IsOKMovingWalkwayAndArcade(walkLinkClass, autoWalkF, escalatorF, arcadeF))
		// ���G���[ �G�X�J���[�^/�I�[�g�E�H�[�N�t���O��ON�����A�����t���t���O��OFF
	{
		if (autoWalkF)
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::notice, err_code::NgMovingWalkArcade,
				kAutoWalkFlag, ToString(walkLinkClass));
		else if (escalatorF)
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::notice, err_code::NgMovingWalkArcade, kEscalatorFlag, ToString(walkLinkClass));
	}

	// �t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă��Ȃ����m�F
	// ���G���[ �t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���
	if (!judge_value::IsOKWalkLinkClassAndFlag(walkLinkClass, autoWalkF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kAutoWalkFlag, 0.0, 0.0, false);

	else if (!judge_value::IsOKWalkLinkClassAndFlag(walkLinkClass, stairF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kStairFlag, 0.0, 0.0, false);

	else if (!judge_value::IsOKWalkLinkClassAndFlag(walkLinkClass, slopeF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kSlopeFlag, 0.0, 0.0, false);

	else if (!judge_value::IsOKWalkLinkClassAndFlag(walkLinkClass, escalatorF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kEscalatorFlag, 0.0, 0.0, false);

	if (!judge_value::IsOKWalkLinkClassAndArcadeFlag(walkLinkClass, arcadeF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kArcadeFlag, 0.0, 0.0, false);

	if (!judge_value::IsOKWalkLinkClassAndOpenAreaFlag(walkLinkClass, openAreaF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kOpenAreaFlag, 0.0, 0.0, false);

	if (!judge_value::IsOKWalkLinkClassAndTunnelFlag(walkLinkClass, tunnelF, m_dataType))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kTunnelFlag, 0.0, 0.0, false);

	if (m_dataType == walk_type::type_class::kDownConvert && !judge_value::IsOKWalkLinkClassAndScrambleFlag(walkLinkClass, scrambleF))
		m_output.OutputRegLog(err_level::error, err_code::ExistFlag, ToString(walkLinkClass), kScrambleFlag, 0.0, 0.0, false);

	// ���s�ҘH���R�[�h�ɓ����Ă�����̂��AWALK_CODE_LIST�ɑ��݂��邩���m�F
	if (walkCode > 0) {
		if (m_walkCode.m_Rows.end() == m_walkCode.m_Rows.find(walkCode))
			// ���G���[ �Ή�������s�ҘH���R�[�h�f�[�^���Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::NotFoundWalkCode, ToString(walkCode));

		if (walkLinkClass == walk_class::kWalkableRoad)
			// ���G���[ �l�ԋ��p�����N�ɘH���R�[�h����������Ă���
			m_output.OutputRegLog(err_level::error, err_code::NgHCWalkCode);
	}
}

void WalkLinkCheckFunction::attributeConformityCheck2(
	long onewayCode,
	long noPassCode,
	long noPassRF,
	long onewayRF,
	long walkLinkClass,
	long walkLinkID
)
{
	using namespace uh::str_util;
	if (!judge_value::IsOKNoPassageCodeAndRelFlag(noPassCode, noPassRF))
		//���G���[ �ʍs�֎~��ʃR�[�h�Ɗ֘A�t���O�̐��������Ƃ�Ă��Ȃ�
		m_output.OutputRegLog(err_level::error, err_code::NgNoPassageRF, ToString(noPassCode), ToString(noPassRF));

	if (FLAG_ON == noPassRF)
	{
		if (m_walkNoPassage.m_Rows.empty() || m_walkNoPassage.m_Rows.end() == m_walkNoPassage.m_Rows.find(walkLinkID))
			//���G���[ �ʍs�֎~���ԋK���֘A�t���O��ON�����A�ʍs�֎~�K����񂪑��݂��Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::NgNotNoPassInfo);
	}

	if (!judge_value::IsOKOnewayCodeAndRelFlag(onewayCode, onewayRF))
		//���G���[ �����ʃR�[�h�Ɗ֘A�t���O�̐��������Ƃ�Ă��Ȃ�
		m_output.OutputRegLog(err_level::error, err_code::NgOnewayRF, ToString(onewayCode), ToString(onewayRF));

	if (FLAG_ON == onewayRF)
	{
		if (m_walkOneway.m_Rows.empty() || m_walkOneway.m_Rows.end() == m_walkOneway.m_Rows.find(walkLinkID))
			//���G���[ ����ʍs���ԋK���֘A�t���O��ON�����A����ʍs�K����񂪑��݂��Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::NgNotOnewayInfo);
	}

	// �l�ԋ��p�����N�ɋK����񂪐�������Ă��Ȃ����m�F
	if (!judge_value::IsOKHumanCarAndRegFlag(walkLinkClass, onewayCode, noPassCode))
		// ���G���[ �l�ԋ��p�����N�ɋK������������Ă���
		m_output.OutputRegLog(err_level::error, err_code::NgHumanCarAndF,
			ToString(onewayCode), ToString(noPassCode));

}
void WalkLinkCheckFunction::attributeChangeCheck(
	const std::list<GeoClass> & fromLinks,
	const std::list<GeoClass>& toLinks,
	const GeoClass& walkLinkObj,
	const FeaturesPack& walkLink,
	long walkLinkClass,
	long floorMoveClass,
	long floorLevelIndex
)
{
	using namespace walk_link;
	using namespace uh::str_util;
	CString info1, info2;

	// �ǂ����1���̂͂�
	auto& fromLink = *(fromLinks.begin());
	auto& toLink = *(toLinks.begin());

	// ��v�ƌ��Ȃ��t�B�[���h����v����ꍇ�̂݁A�R�[�h�̕ω����m�F����
	if (judge_value::IsSimpleSameWalkLinkAttr(fromLink, walkLinkObj, walkLink.m_FieldMap)
		&& judge_value::IsSimpleSameWalkLinkAttr(walkLinkObj, toLink, walkLink.m_FieldMap)) {

		if (!walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kWalkCode), fromLink)
			|| !walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kWalkCode), toLink)) {

			// ���G���[ ���s�ҁu�H���v�R�[�h���O��̃����N�ňقȂ�i�G���[�R�[�h�F332�j
			info1 = ToString(walkLinkObj.GetLongValue(walkLink.m_FieldMap.FindField(kWalkCode)));
			info2.Format(_T("'F'(%ld)/'T'(%ld)"), fromLink.GetLongValue(walkLink.m_FieldMap.FindField(kWalkCode)),
				toLink.GetLongValue(walkLink.m_FieldMap.FindField(kWalkCode)));
			m_output.OutputRegLog(err_level::error, err_code::DiffWalkCode, info1, info2);
		}
	}

	// bug 11431 - [u][�v�]]�K�w�ړ���ʂ̕t�^�R������o���Ăق���
	// ���s�Ҏ�ʃR�[�h�������A�܂��͒n���X�̏ꍇ
	if (walkLinkClass == walk_class::kIndoor
		|| walkLinkClass == walk_class::kUnderGround)
	{
		// ���s�Ҏ�ʃR�[�h�A�t���O�֘A����v���Ă���A�K�w�ړ���ʂ��Ȃ��̏ꍇ
		if (judge_value::IsSimpleSameWalkLinkAttr(fromLink, walkLinkObj, walkLink.m_FieldMap)
			&& judge_value::IsSimpleSameWalkLinkAttr(walkLinkObj, toLink, walkLink.m_FieldMap)
			&& floorMoveClass == floor_move_class::kNone)
		{
			if (!walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kFloorLevel), fromLink)
				|| !walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kFloorLevel), toLink))
				// ���G���[ �K�w���x�����O��̃����N�ňقȂ�
			{
				info1 = walkLinkObj.GetStringValue(floorLevelIndex);
				info2.Format(_T("'F'(%ls)/'T'(%ls)"), fromLink.GetStringValue(floorLevelIndex), toLink.GetStringValue(floorLevelIndex));
				m_output.OutputRegLog(err_level::warning, err_code::DiffFloorLevel, info1, info2);
			}
		}
	}
}
void WalkLinkCheckFunction::openAreaCheck(
	long openAreaF,
	const std::list<GeoClass> & fromLinks,
	const std::list<GeoClass>& toLinks,
	const FeaturesPack& walkLink
)
{
	if (FLAG_ON == openAreaF)
	{
		if (!ExistOpenAreaFlag(fromLinks, walkLink.m_FieldMap) || !ExistOpenAreaFlag(toLinks, walkLink.m_FieldMap))
			// ���G���[ �L�ꂪ���Ă��Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::NotConOpenAreaF);
	}
}

void WalkLinkCheckFunction::connectLinkCheck(
	long walkLinkClass,
	const GeoClass& fromWalkNodeObj,
	const GeoClass& toWalkNodeObj,
	const std::list<GeoClass> & fromLinks,
	const std::list<GeoClass>& toLinks,
	const FeaturesPack& walkLink,
	long walkNodeClassIndex
)
{
	using namespace walk_link;
	bool isValidLink(false);
	if (walkLinkClass == walk_class::kConnect) isValidLink = true;
	// �_�E���R���o�[�g�����ꍇ�A�����N�̕��s�Ҏ�ʂ��u�ڑ������N�v�u���W�J���R�l�N�V�����v�ŁA
	if (!isValidLink && m_dataType == walk_type::type_class::kDownConvert && walkLinkClass == walk_class::kLogicalConnection)
		isValidLink = true;
	if (isValidLink)
	{
		long fromWalkNodeClass = fromWalkNodeObj.GetLongValue(walkNodeClassIndex);
		long toWalkNodeClass = toWalkNodeObj.GetLongValue(walkNodeClassIndex);

		// �_�E���R���o�[�g�����ꍇ�A�����ꂩ�̐ڑ��m�[�h���u���HNW�����N�ڑ��m�[�h�v�u���H�m�[�h�ڑ��m�[�h�v�ł͂Ȃ��ꍇ�G���[
		if (m_dataType == walk_type::type_class::kDownConvert
			&& fromWalkNodeClass != walk_node::node_class::kRoadNW
			&& toWalkNodeClass != walk_node::node_class::kRoadNW
			&& fromWalkNodeClass != walk_node::node_class::kConnectedRoad
			&& toWalkNodeClass != walk_node::node_class::kConnectedRoad)
			// ���G���[ �ڑ��m�[�h���u���HNW�����N�ڑ��m�[�h�v�u���H�m�[�h�ڑ��m�[�h�v�ł͂Ȃ�
			m_output.OutputRegLog(err_level::error, err_code::NotConRoadWalkLink, _T(""), _T(""), 0.0, 0.0, false);

		// �ėp�����ꍇ�A
		if (m_dataType == walk_type::type_class::kGeneral
			&& fromWalkNodeClass != walk_node::node_class::kRoadNW
			&& toWalkNodeClass != walk_node::node_class::kRoadNW)
		{
			if (!ExistHumanCarLink(fromLinks, walkLink.m_FieldMap) && !ExistHumanCarLink(toLinks, walkLink.m_FieldMap))
				// ���G���[ �ڑ������N�����H�����N�A�܂��͐l�ԋ��p�����N�ɐڑ����Ă��Ȃ�
				m_output.OutputRegLog(err_level::error, err_code::NotConRoadWalkLink);
		}
	}

}

void WalkLinkCheckFunction::parallelCheck(
	long walkLinkClass,
	long floorMoveClass,
	long escalatorF,
	long onewayCode,
	const GeoClass& walkLinkObj,
	long fromWalkNodeID,
	long toWalkNodeID,
	const NODEIDMAP& walkNodeLinkIDs,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree
)
{
	using namespace uh::str_util;
	using namespace walk_link;
	// bug 11433 - [u][�v�]]���s����`��̏ꍇ�ɑ����̖������`�F�b�N���Ăق���
	// �K�w�ړ���ʃR�[�h���u0:�㉺�Ȃ��v�ȊO�̏ꍇ

	if (judge_value::IsParalleCheckTargetlLink(walkLinkClass) && floorMoveClass != floor_move_class::kNone)
	{
		auto findWalkLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(4), walkLinkRTree);

		std::map<long, long> fromLinkIDList, toLinkIDList;	//from/to�̊e�m�[�h����3�����N�ڂɓ��B���郊���N�Q

		// 3�Ԗڂɓ��B���郊���N�Q���擾
		SearchThirdConnectLinks(walkLinkObj.m_ObjectID, fromWalkNodeID, findWalkLinkIDs, walkNodeLinkIDs, walkLink, fromLinkIDList);
		SearchThirdConnectLinks(walkLinkObj.m_ObjectID, toWalkNodeID, findWalkLinkIDs, walkNodeLinkIDs, walkLink, toLinkIDList);
		long fromNodeIDIndex = walkLink.m_FieldMap.FindField(kFromNodeID);
		long onewayCodeIndex = walkLink.m_FieldMap.FindField(kOneway);
		long escalatorFIndex = walkLink.m_FieldMap.FindField(kEscalatorFlag);
		for (const auto& fromLinkID : fromLinkIDList)
		{
			if (fromLinkID.first == walkLinkObj.m_ObjectID)
				continue;

			for (const auto& toLinkID : toLinkIDList)
			{
				if (fromLinkID.first != toLinkID.first)
					continue;

				// �o��������̒T�����ʂł͂Ȃ��ꍇ
				if (fromLinkID.second == toLinkID.second)
					continue;

				auto compObjectItr = walkLink.m_FeatureMap.find(fromLinkID.first);
				if (compObjectItr == walkLink.m_FeatureMap.end())
					continue;

				long compWalkClass = compObjectItr->second.GetLongValue(walkLink.m_FieldMap.FindField(kWalkClass));
				// ���s�Ҏ�ʃR�[�h������łȂ��ꍇ
				if (walkLinkClass != compWalkClass)
					continue;

				long compFloorMoveClass = compObjectItr->second.GetLongValue(walkLink.m_FieldMap.FindField(kFloorMoveClass));
				// �K�w�ړ���ʂ��t�^����Ă��Ȃ��ꍇ
				if (compFloorMoveClass == floor_move_class::kNone) // ���G���[ ���s�����N�̊K�w�ړ���ʂ̕������قȂ�
				{
					m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::DiffFloorMoveCode,
						ToString(compObjectItr->first));
					continue;
				}

				// �K�w�ړ���ʂ��t�^����Ă���ꍇ
				auto compFromNodeID = compObjectItr->second.GetLongValue(fromNodeIDIndex);
				auto compOnewayCode = compObjectItr->second.GetLongValue(onewayCodeIndex);
				auto compEscalatorF = compObjectItr->second.GetLongValue(escalatorFIndex);

				// ���s�Ƃ��Ă݂āA������N��From/To�̕������t�̏ꍇ
				if (compFromNodeID == fromLinkID.second)
				{
					// �K�w�ړ���ʂ����]���Ă��Ȃ����
					if (floorMoveClass == compFloorMoveClass) // ���G���[ ���s�����N�̊K�w�ړ���ʂ̕������قȂ�
						m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::DiffFloorMoveCode, ToString(compObjectItr->first));
					// �������N�Ƃ��G�X�J���[�^�̏ꍇ
					if (escalatorF &&  compEscalatorF)
					{
						if (judge_value::SimpleDirection::noDirect
							!= judge_value::GetOnwayDirection(onewayCode)
							&& judge_value::SimpleDirection::noDirect
							!= judge_value::GetOnwayDirection(compOnewayCode)
							&& judge_value::GetOnwayDirection(onewayCode)
							!= judge_value::GetOnwayDirection(compOnewayCode))
							m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::SameEscOnway, ToString(compObjectItr->first));
					}
					continue;
				}

				// ���s�Ƃ��Ă݂āA������N��From/To�̕����������̏ꍇ
				// �K�w�ړ���ʂ������łȂ����
				if (floorMoveClass != compFloorMoveClass) // ���G���[ ���s�����N�̊K�w�ړ���ʂ̕������قȂ�
					m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::DiffFloorMoveCode, ToString(compObjectItr->first));

				// �������N�Ƃ��G�X�J���[�^�̏ꍇ
				if (escalatorF && compEscalatorF)
				{
					if (judge_value::SimpleDirection::noDirect != judge_value::GetOnwayDirection(onewayCode) &&
						judge_value::SimpleDirection::noDirect != judge_value::GetOnwayDirection(compOnewayCode) &&
						judge_value::GetOnwayDirection(onewayCode) == judge_value::GetOnwayDirection(compOnewayCode))
						m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::SameEscOnway, ToString(compObjectItr->first));
				}
			}
		}
	}
}

void WalkLinkCheckFunction::underGroundCheck(
	long walkLinkClass,
	long walkLinkID,
	const GeoClass& fromWalkNodeObj,
	const GeoClass& toWalkNodeObj,
	const std::list<GeoClass> & fromLinks,
	const std::list<GeoClass>& toLinks,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const CString& meshcode
)
{
	using namespace walk_link;
	// �n���ʘH�A�܂��͉��f�n�����̂Ƃ��̂݃`�F�b�N�B�ibug 12000�j
	// �G���x�[�^�[���~���K�w���Ȃ��̂ɒn���Ɍq�����Ă���
	if (judge_value::IsUnderGroundLink(walkLinkClass))
	{
		// from���̃G���[�ɂȂ郊���N�Q���擾		// from���̃G���[�ɂȂ郊���N�Q���擾
		auto errLinks = checkFloorMoveConnectUnder(walkLinkID, fromLinks, fromWalkNodeObj, walkNode.m_FieldMap, walkLink);
		{
			auto toErrLinks = checkFloorMoveConnectUnder(walkLinkID, toLinks, toWalkNodeObj, walkNode.m_FieldMap, walkLink);
			// to���̃G���[�ɂȂ郊���N�Q��from���̃����N�Q�Ɍ���
			for (long linkId : toErrLinks)
				errLinks.insert(linkId);
		}
		if (errLinks.size() > 0) {
			// �J�����g�f�[�^�̏o�͂ł͂Ȃ����߁AOutputRegLog�Ɉڍs������Ȃ�����
			long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
			long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
			long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
			long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
			for (long linkId : errLinks)
			{
				// ���G���[ �G���x�[�^�[���~���K�w���Ȃ��̂ɒn���Ɍq�����Ă���
				/*
				* ���r���[�p�R�����g
				* ID�Ƃ���ɑ΂���modXXX����v���Ă��Ȃ��������C��
				*/
				auto errLinkObj = walkLink.m_FeatureMap.at(linkId);
				m_output.OutputLog(kTableName, linkId, m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::warning, err_code::NgUnderConnect,
					meshcode, errLinkObj.GetStringValue(modOperatorIndex), errLinkObj.GetStringValue(modDateIndex),
					errLinkObj.GetStringValue(modProgramIndex), errLinkObj.GetStringValue(modProgramDateIndex));
			}
		}
	}

}
void WalkLinkCheckFunction::attributeConnectionCheck(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP walkNodeLinkIDs,
	const std::map<long, GeoClass>& walkLinkMapFor353
)
{
	using namespace walk_link;
	using namespace uh::str_util;
	FeaturesPack tempWalkLink = walkLink;
	FeaturesPack tempWalkNode = walkNode;
	NODEIDMAP tempWalkNodeLinkIDs = walkNodeLinkIDs;

	long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
	long  fromWalkNodeIDIndex = walkLink.m_FieldMap.FindField(kFromNodeID);
	long  toWalkNodeIDIndex = walkLink.m_FieldMap.FindField(kToNodeID);
	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(kWalkClass);
	// �G���[�R�[�h353�̃`�F�b�N
	for (const auto& tmpWalkLink : walkLinkMapFor353)
	{
		long  walkLinkID = tmpWalkLink.first;
		auto& walkLinkObj = tmpWalkLink.second;
		long  fromWalkNodeID = walkLinkObj.GetLongValue(fromWalkNodeIDIndex);
		long  toWalkNodeID = walkLinkObj.GetLongValue(toWalkNodeIDIndex);
		long  walkLinkClass = walkLinkObj.GetLongValue(walkLinkClassIndex);

		m_output.SetModifyInfo(walkLinkID, walkLinkObj.GetStringValue(modOperatorIndex),
			walkLinkObj.GetStringValue(modDateIndex), walkLinkObj.GetStringValue(modProgramIndex), walkLinkObj.GetStringValue(modProgramDateIndex));

		auto fromLinkIDSetItr = walkNodeLinkIDs.find(fromWalkNodeID);
		auto toLinkIDSetItr = walkNodeLinkIDs.find(toWalkNodeID);

		// 2�����b�V�����E���ׂ������N�ɑ΂��Autil::getAdjacentLink()�Œ[�_�m�[�hID���ύX�����̂ŁA
		// ��̃`�F�b�N�ɉe�����Ȃ��悤�ɁA�Ǐ��I�ȕϐ�tempNodeID���g��
		auto tempNodeID = fromWalkNodeID;

		// From���̐ڑ������N���擾
		auto fromWalkLinks = util::GetObjectMap(walkLinkID, fromLinkIDSetItr->second, walkLink.m_FeatureMap);
		std::set<long> adjFromLinkIds = util::getAdjacentLink(tempNodeID, fromWalkLinks, walkLink, walkNode, walkNodeLinkIDs, true);

		if (adjFromLinkIds.empty())
		{
			adjFromLinkIds = getAdjacentWalkLink(fromWalkNodeID, walkLinkID, ipMeshGeo,
				tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs);
		}

		// To���̐ڑ������N���擾
		tempNodeID = toWalkNodeID;
		auto toWalkLinks = util::GetObjectMap(walkLinkID, toLinkIDSetItr->second, walkLink.m_FeatureMap);
		std::set<long> adjToLinkIds = util::getAdjacentLink(tempNodeID, toWalkLinks, walkLink, walkNode, walkNodeLinkIDs, true);

		if (adjToLinkIds.empty())
		{
			adjToLinkIds = getAdjacentWalkLink(toWalkNodeID, walkLinkID, ipMeshGeo,
				tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs);
		}

		// 2���H�̂݁A�`�F�b�N�ΏۂƂȂ�iFrom��To���̐ڑ������N�����݂��Ȃ��A�����͕������݂���ꍇ�A�`�F�b�N�ΏۊO�j
		if (adjFromLinkIds.size() == 1 && adjToLinkIds.size() == 1)
		{
			auto& fromWalkLink = tempWalkLink.m_FeatureMap.at(*(adjFromLinkIds.begin()));
			auto& toWalkLink = tempWalkLink.m_FeatureMap.at(*(adjToLinkIds.begin()));

			// �O��̃����N���������s�Ҏ�ʂŁA���A������N���A�������s�Ҏ�ʂƂ͈قȂ��ʂ̏ꍇ�A�G���[�Ƃ���
			if ((fromWalkLink.CompareAt(walkLink.m_FieldMap.FindField(kWalkClass), toWalkLink))
				&& (!walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kWalkClass), toWalkLink)))
			{
				// ���G���[ ���s�ҁu��ʁv�R�[�h���O��̃����N�ňقȂ�i�G���[�R�[�h�F353�j
				CString info1 = ToString(walkLinkClass);
				CString info2;
				info2.Format(_T("'F'(%ld)/'T'(%ld)"), fromWalkLink.GetLongValue(walkLink.m_FieldMap.FindField(kWalkClass)),
					toWalkLink.GetLongValue(walkLink.m_FieldMap.FindField(kWalkClass)));
				m_output.OutputRegLog(err_level::warning, err_code::DiffWalkClass, info1, info2);
			}
		}
	}
}
void WalkLinkCheckFunction::sameFromToCheck(
	const std::map<std::pair<long, long>, std::list<long>>& dupNodeMap,
	const FeaturesPack& walkLink,
	const CString& meshcode
)
{
	using namespace walk_link;

	long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
	long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
	long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
	long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
	// ����From/To�����݂��Ȃ����m�F����
	for (const auto& dupID : dupNodeMap) {
		if (dupID.second.size() > 1) {
			// ���G���[ ����From/To���������N����������
			auto  itr = dupID.second.begin();
			long  ngID1 = *itr;
			long  ngID2 = *(++itr);
			auto  itrWalkLink = walkLink.m_FeatureMap.find(ngID1);
			CString info1;
			info1.Format(_T("WalkLinkID:%ld"), ngID2);
			CString modOperator, modDate, modProgram, modProgramDate;
			if (itrWalkLink != walkLink.m_FeatureMap.end()) {
				modOperator = itrWalkLink->second.GetStringValue(modOperatorIndex);
				modDate = itrWalkLink->second.GetStringValue(modDateIndex);
				modProgram = itrWalkLink->second.GetStringValue(modProgramIndex);
				modProgramDate = itrWalkLink->second.GetStringValue(modProgramDateIndex);
				m_output.OutputLog(kTableName, ngID1, err_level::error, err_code::LoopLinkDupFT,
					meshcode, modOperator, modDate, modProgram, modProgramDate, info1);
			}
			else {
				//�����ɗ��邱�Ƃ����邩�s�������A�Ӗ����ς���Ă��邯�ǁA���v���O�����݊��̂��ߒǉ�
				m_output.SetID(ngID1);
				m_output.OutputRegLog(err_level::error, err_code::LoopLinkDupFT);
			}
		}
	}

}

long WalkLinkCheckFunction::anotherNodeID(
	const GeoClass& linkObj,
	long nodeId,
	long fromIdIndex,
	long toIdIndex
)
{
	long fromID = linkObj.GetLongValue(fromIdIndex);
	if (fromID != nodeId) {
		return fromID;
	}
	else {
		return linkObj.GetLongValue(toIdIndex);
	}
}

std::list<GeoClass> WalkLinkCheckFunction::searchCrossRoadNode(
	const GeoClass&linkObj,
	const GeoClass& nodeObj,
	long nodeClassIndex,
	long walkClassIndex,
	long fromIdIndex,
	long toIdIndex,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs,
	std::set<long>& inspectedIDs
)
{
	using namespace walk_node;
	using namespace walk_link;
	std::list<GeoClass> resultList;

	// �`�F�b�N�ς݂Ȃ猩�Ȃ�
	auto result = inspectedIDs.insert(nodeObj.m_ObjectID);
	if (!result.second)
		return std::list<GeoClass>();

	auto linkIDSetItr = walkNodeLinkIDs.find(nodeObj.m_ObjectID);
	if (linkIDSetItr == walkNodeLinkIDs.end()) return resultList;

	auto links = util::GetObjects(linkObj.m_ObjectID, linkIDSetItr->second, walkLink.m_FeatureMap);
	if (node_class::kOutline == nodeObj.GetLongValue(nodeClassIndex)) {
		//�}�s��m�[�h�������炻�̐��T���B����}�s�ゾ������X�ɐ��
		auto link = *links.begin();
		//�}�s��m�[�h����L�тĂ��郊���N�̔��Α��m�[�h�ōČ���
		long id = anotherNodeID(link, nodeObj.m_ObjectID, fromIdIndex, toIdIndex);
		auto nextNodeObj = walkNode.m_FeatureMap.at(id);
		auto nextLinks = util::GetObjects(link.m_ObjectID, linkIDSetItr->second, walkLink.m_FeatureMap);
		return searchCrossRoadNode(link, nextNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex, walkLink, walkNode, walkNodeLinkIDs, inspectedIDs);
	}
	else {
		bool doNextSearch = false;

		set<long> crossRoadIDs;
		for (auto link : links) {
			long walkClass = link.GetLongValue(walkClassIndex);
			if (walk_class::kCrossRoad == walkClass) {
				crossRoadIDs.insert(link.m_ObjectID);
			}
			else if (walk_class::kWalkableRoad == walkClass) {
				doNextSearch = true;
			}
			else {
				//���f�����ł��l�ԋ��p�ł��Ȃ��̂��ڑ����Ă�����A�����ŃX�g�b�v
				doNextSearch = false; // �t���O�������Ă��邩������Ȃ��̂ŁA�����I�ɏ���
				break;
			}
		}
		// �l�ԋ��p���h�����Ă�����A��̉��f������T��
		if (doNextSearch) {
			for (auto linkID : crossRoadIDs) {
				auto link = walkLink.m_FeatureMap.at(linkID);
				long id = anotherNodeID(link, nodeObj.m_ObjectID, fromIdIndex, toIdIndex);
				auto nextNodeObj = walkNode.m_FeatureMap.at(id);
				resultList.splice(resultList.end(), searchCrossRoadNode(link, nextNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex, walkLink, walkNode, walkNodeLinkIDs, inspectedIDs));
			}
		}
		else {
			// ���f���������Ȃ̂ŁA�����ŃX�g�b�v
			resultList.push_back(nodeObj);
		}
		return resultList;
	}
}
void WalkLinkCheckFunction::crossRoadNodeCheck(
	long nodeClassIndex,
	long walkClassIndex,
	long fromIdIndex,
	long toIdIndex,
	const GeoClass& linkObj,
	const GeoClass& fromWalkNodeObj,
	const GeoClass& toWalkNodeObj,
	const FeaturesPack& walkLink,
	const FeaturesPack& walkNode,
	const NODEIDMAP& walkNodeLinkIDs,
	const CString& meshcode
)
{
	using namespace walk_node;
	using namespace uh::str_util;
	std::set<long> inspectedIDs;	// �����ς݂̃m�[�hID�Z�b�g
	//from���̃m�[�hID�擾
	auto targetNodes = searchCrossRoadNode(linkObj, fromWalkNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex,
		walkLink, walkNode, walkNodeLinkIDs, inspectedIDs);


	//to���̃m�[�h���擾���Ēǉ�
	targetNodes.splice(targetNodes.end(), searchCrossRoadNode(linkObj, toWalkNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex,
		walkLink, walkNode, walkNodeLinkIDs, inspectedIDs));
	long roadNodeIdIndex = walkNode.m_FieldMap.FindField(kRoadNodeID);
	if (targetNodes.size() < 2) {
		//���f�����̗��[�m�[�h������ł��Ȃ�
		m_output.OutputRegLog(err_level::warning, err_code::NgBorthCrossRoadNode);
	}
	std::list<GeoClass> nodes;
	long baseID = 0;
	for (auto node : targetNodes) {
		if (node.IsOk()) {
			if (node.GetLongValue(roadNodeIdIndex) < 0) {
				nodes.push_back(node);
			}
			else {
				baseID = node.m_ObjectID;	//��ŏ\��������
			}
		}
	}
	// ���H�m�[�hID�������Ă�����̂��O��OK�i�d���Ȃ��j
	// �P�ł���������A�S���ɂȂ���NG
	if (nodes.size() > 0 && nodes.size() != targetNodes.size()) {
		long modOperatorIndex = walkNode.m_FieldMap.FindField(kOperator);
		long modDateIndex = walkNode.m_FieldMap.FindField(kModifyDate);
		long modProgramIndex = walkNode.m_FieldMap.FindField(kModifyProgName);
		long modProgramDateIndex = walkNode.m_FieldMap.FindField(kProgModifyDate);
		for (auto node : nodes) {
			m_output.OutputLog(kTableName,
				node.m_ObjectID, err_level::error,
				err_code::NgCrossRoadNode, meshcode, node.GetStringValue(modOperatorIndex), node.GetStringValue(modDateIndex),
				node.GetStringValue(modProgramIndex), node.GetStringValue(modProgramDateIndex),
				ToString(baseID));
		}
	}
}
bool WalkLinkCheckFunction::ticketGateNodeCheck(
	long nodeClassIndex,
	long walkClassIndex,
	const GeoClass& nodeObj,
	const std::list<GeoClass> & links
)
{
	using namespace walk_node::node_class;
	long nodeClass = nodeObj.GetLongValue(nodeClassIndex);
	if (kTerminal == nodeClass || judge_value::IsTicketGate(nodeClass)) {
		// ���D�m�[�h��������A������OK�i���D���̉��D������j
		// �[�_�m�[�h��������A��Ƀ����N����������NG�����Ǒ��Ń`�F�b�N���Ă邩��OK
		return true;
	}
	// �[�_�m�[�h�ł����D�m�[�ł��Ȃ�������A���̐�ɂ͉��D�������N�����łȂ���NG
	for (auto link : links) {
		if (!judge_value::IsInternalTicketGateLink(link.GetLongValue(walkClassIndex))) {
			// ���D�������N�����D�m�[�h�ȊO�ŉ��D�O�����N�ɐڑ����Ă���
			return false;
		}
	}
	return true;
}

void WalkLinkCheckFunction::internalTicketGateLinkCheck(
	long nodeClassIndex,
	long walkClassIndex,
	const GeoClass& fromWalkNodeObj,
	const GeoClass& toWalkNodeObj,
	const std::list<GeoClass> & fromLinks,
	const std::list<GeoClass>& toLinks
)
{
	if (!ticketGateNodeCheck(nodeClassIndex, walkClassIndex, fromWalkNodeObj, fromLinks)
		|| !ticketGateNodeCheck(nodeClassIndex, walkClassIndex, toWalkNodeObj, toLinks)) {
		m_output.OutputRegLog(err_level::error, err_code::NgInternalTicketGate);
	}
}

// �X�N�����u���t���O���t�^����Ă���̂ɃX�N�����u���G���A���ɖ������`�F�b�N
void WalkLinkCheckFunction::walkLinkNotOnScrambleAreaCheck(
	const GeoClass& walkLinkObj,
	const FeaturesPack& scrambleArea,
	const BGRTree& scrambleAreaRTree,
	long scrambleFlag
) const
{
	using namespace walk_link;

	if (scrambleFlag != 1) return;

	std::vector<GeoClass> crossScrambleAreas;
	if (util::findOverlapGeometry(walkLinkObj, scrambleArea, scrambleAreaRTree, crossScrambleAreas) > 0) return;

	m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkNotOnScrambleArea);
}

// �X�N�����u���G���A���Ȃ̂ɃX�N�����u���t���O���t�^����Ă��Ȃ����`�F�b�N
void WalkLinkCheckFunction::walkLinkOnScrambleAreaCheck(
	const GeoClass& walkLinkObj,
	const FeaturesPack& scrambleArea,
	const BGRTree& scrambleAreaRTree,
	long walkLinkClass,
	long scrambleFlag
) const
{
	using namespace walk_link;

	std::vector<GeoClass> crossScrambleAreas;
	if (util::findOverlapGeometry(walkLinkObj, scrambleArea, scrambleAreaRTree, crossScrambleAreas) <= 0) return;

	for (const auto& scrambleArea : crossScrambleAreas)
	{
		VARIANT_BOOL within(VARIANT_FALSE);
		if (FAILED(((IRelationalOperatorPtr)walkLinkObj.m_ipGeometry)->Within(scrambleArea.m_ipGeometry, &within)) || within == VARIANT_FALSE) continue;

		if ((walkLinkClass == walk_class::kCrossRoad || walkLinkClass == walk_class::kInferredCrossRoad || walkLinkClass == walk_class::kOtherWalkRoad)
			&& scrambleFlag != 1)
		{
			m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkOnScrambleArea);
			return;
		}
	}
}

// �Ԉ֎q�ʍs�R�[�h���s���Ȓl���`�F�b�N
void WalkLinkCheckFunction::checkWheelChair(
	long  walkWheelChair
) const
{
	using namespace walk_link;

	if (walkWheelChair != wheel_chair::kUnInvestigated && walkWheelChair != wheel_chair::kPassable && walkWheelChair != wheel_chair::kNoPassable)
		m_output.OutputRegLog(err_level::fatal, err_code::NgWalkLinkWheelChair);
}

// �̂�Δԍ����s���Ȓl���`�F�b�N
void WalkLinkCheckFunction::checkPlatformNumber(
	long  walkPlatformNumber
) const
{
	if (walkPlatformNumber < 0 || walkPlatformNumber > 99)
		m_output.OutputRegLog(err_level::fatal, err_code::NgWalkLinkPlatformNumber);
}

// �������W�J���R�l�N�V�������`�F�b�N
void WalkLinkCheckFunction::checkMicroLogicalConnection(
	const GeoClass& walkLinkObj
) const
{
	IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);

	IPointPtr ipPoint = ipWalkLinkPointCol->GetPoint(0);
	double x(0.0), y(0.0);
	ipPoint->QueryCoords(&x, &y);

	double length(0.0);
	for (long index = 1L; index < ipWalkLinkPointCol->GetPointCount(); ++index)
	{
		IPointPtr ipPointTmp = ipWalkLinkPointCol->GetPoint(index);
		double xTmp(0.0), yTmp(0.0);
		ipPointTmp->QueryCoords(&xTmp, &yTmp);

		length += g_cnv.GetDist(x, y, xTmp, yTmp);

		x = xTmp;
		y = yTmp;
	}

	if (length < m_microLogicalConnection)
		m_output.OutputRegLog(err_level::warning, err_code::MicroLogicalConnection);
}

// ���W�J���R�l�N�V�����ƌ������Ă��邩�`�F�b�N
void WalkLinkCheckFunction::checkCrossWalkLink(
	const GeoClass& walkLinkObj,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree
) const
{
	std::vector<GeoClass> crossObjects;
	if (util::findOverlapGeometry(walkLinkObj, walkLink, walkLinkRTree, crossObjects) <= 0) return;

	for (const auto& object : crossObjects)
	{
		VARIANT_BOOL crosses(VARIANT_FALSE);
		if (SUCCEEDED(((IRelationalOperatorPtr)walkLinkObj.m_ipGeometry)->Crosses(object.m_ipGeometry, &crosses)) && crosses == VARIANT_TRUE )
		{
			m_output.OutputRegLog( err_level::error, err_code::NgCrossWalkLink );
			return;	
		}
	}		
}
