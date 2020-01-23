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
#include "CheckWalk.h"
#include "util.h"
#include <sindy/workspace.h>
#include <TDC/useful_headers/str_util.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/AheLayerFunctions.h>
#include <WinLib/VersionInfo.h>
#include "value.h"

#include "RoadLinkCheckFunction.h"
#include "WalkLinkCheckFunction.h"
#include "WalkNodeCheckFunction.h"
#include "HeightNodeCheckFunction.h"
#include "HeightNodePositionCheckFunction.h"
#include "POICheckFunction.h"
#include "UndergroundCheckFunction.h"
#include "WalkCodeListCheckFunction.h"
#include "WalkOnewayCheckFunction.h"
#include "WalkNoPassageCheckFunction.h"
#include "WalkLinkOfBaseMeshCheckFunction.h"
#include "LinkRelationCheckFunction.h"
#include "PedxingCheckFunction.h"
#include "ScrambleAreaCheckFunction.h"

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace field_lists;
extern crd_cnv g_cnv;

namespace
{
	const long UNDERGROUNT_GATE_CONTENTS_CODE = 289000001; // �n���X�o�����R���e���c�R�[�h

/**
 * @brief �w��̃x�[�X���b�V���R�[�h�����x�[�X���b�V���ɏ�������
 * �S�Ă̓s�s�n�}���b�V���̃��b�V���R�[�h���擾
 * @param baseMeshCode [in] �s�s�n�}���b�V���R�[�h���擾����x�[�X���b�V���R�[�h
 * @param cityMeshTable [in] �s�s�n�}���b�V���̃e�[�u��
 */
	inline std::set<long> getCityMeshCodeOnBaseMesh(long baseMeshCode, const ITablePtr& cityMeshTable)
	{
		std::set<long> cityMeshCode;

		// ������������
		CString strSubField = meshcode_class::kMeshCode;
		CString strWhere;
		strWhere.Format(_T("%ld00 <= %s AND %s <= %ld99")
			, baseMeshCode, meshcode_class::kMeshCode, meshcode_class::kMeshCode, baseMeshCode);

		_ICursorPtr cursor;
		if (FAILED(cityMeshTable->Search(AheInitQueryFilter(nullptr, strSubField, strWhere), VARIANT_TRUE, &cursor))
			|| !cursor)
			return cityMeshCode;

		long meshCodeIndex = -1;
		if (FAILED(cityMeshTable->FindField(CComBSTR(meshcode_class::kMeshCode), &meshCodeIndex)
			|| meshCodeIndex < 0))
			return cityMeshCode;

		_IRowPtr record;
		while (S_OK == cursor->NextRow(&record) && record)
		{
			CComVariant val = record->GetValue(meshCodeIndex);
			cityMeshCode.insert(val.lVal);
		}
		return cityMeshCode;
	}

}

CCheckWalk::CCheckWalk(const Arguments& args, const CProperties& prop) :
	m_coordinateTolerance(0.0), m_dataType(walk_type::type_class::kGeneral)
{

	m_strOutput = (args.log_dir).c_str();

	m_strStationDB = prop.GetValue(properties::stationDB);
	m_strRoadDB = prop.GetValue(properties::roadDB);
	m_strWalkDB = prop.GetValue(properties::walkDB);
	m_strMeshDB = prop.GetValue(properties::meshDB);
	m_strPoiDB = prop.GetValue(properties::poiDB);
	m_strUnderDB = prop.GetValue(properties::underDB);
	m_strMeshList = prop.GetValue(properties::meshList);

	m_dist_threshold = prop.GetValueDouble(properties::dist_threshold);
	m_neighbor_threshold = prop.GetValueDouble(properties::neighbor_threshold);
	m_refer_id_threshold = prop.GetValueDouble(properties::refer_id_threshold);
	m_angle_threshold = prop.GetValueDouble(properties::angle_threshold);
	m_buddy_dist_threshold = prop.GetValueDouble(properties::buddy_dist_threshold);
	m_buddy_angle_threshold = prop.GetValueDouble(properties::buddy_angle_threshold);
	m_sindyTolerance_threshold = prop.GetValueDouble(properties::sindyTolerance_threshold);
	m_microLogicalConnection   = prop.GetValueDouble(properties::microLogicalConnection);

	m_strDistThreshold.Format(_T("%s m"), prop.GetValue(properties::dist_threshold));
	m_strNeighborThreshold.Format(_T("%s m"), prop.GetValue(properties::neighbor_threshold));
	m_strReferIDThreshold.Format(_T("%s m"), prop.GetValue(properties::refer_id_threshold));
	m_strAngleThreshold.Format(_T("%s ��"), prop.GetValue(properties::angle_threshold));
	m_strBuddyDistThreshold.Format(_T("%s m"), prop.GetValue(properties::buddy_dist_threshold));
	m_strBuddyAngleThreshold.Format(_T("%s ��"), prop.GetValue(properties::buddy_angle_threshold));

	m_height_node_ini = prop.GetValue(properties::heightNode_ini);
	m_strHeightNodeIni = prop.GetValue(properties::heightNode_ini);
	m_checkTarget = prop.GetValueTarget(properties::target);
	m_strCheckTarget = prop.GetValue(properties::target);

	m_modifyFilter = prop.getModifyFilter();
	m_filterDay = prop.GetValue(properties::day);
	m_filterTime = prop.GetValue(properties::time);

	if (!args.data_type.empty())
		m_dataType = walk_type::type_class::ECode(_ttol((args.data_type).c_str()));

	m_database.setDataType(m_dataType);
}

// ���s
bool CCheckWalk::execute()
{
	// ������
	if (!init())
		return false;
	// �w�b�_��񓙏o��
	OutputHeader();

	try {
		// �`�F�b�N�Ώۃ��b�V���̓ǂݍ���
		LoadMesh();

		// �s�s�n�}���b�V�����Ƃ̃`�F�b�N
		CheckByCityMesh();

		// �S���b�V���Ώۂ̃`�F�b�N
		CheckByAllMesh();

		// ���k���b�V���P�ʂ̏���
		CheckByBaseMesh();

		// �I�����ԏo��
		m_output.OutputLog(err_level::info, err_code::StrEnd, _T("����I��"));
		m_output.OutputLog(err_level::info, err_code::StrEndTime, util::GetCurTime());

		return true;
	}
	catch (const std::exception& e)
	{
		m_output.OutputLog(err_level::error, err_code::StrException, e.what());
		m_output.OutputLog(err_level::info, err_code::StrEnd, _T("�ُ�I��"));
		m_output.OutputLog(err_level::info, err_code::StrEndTime, util::GetCurTime());
		return false;
	}
}

// �w�b�_��񓙏o��
void CCheckWalk::OutputHeader()
{
	m_output.OutputHeader();
	m_output.OutputLog(err_level::info, err_code::StrLogDir, m_strOutput);

	m_output.OutputLog(err_level::info, err_code::StrCheckType, m_strCheckTarget);
	m_output.OutputLog(err_level::info, err_code::StrWalkDB, m_strWalkDB);
	m_output.OutputLog(err_level::info, err_code::StrRoadDB, m_strRoadDB);
	m_output.OutputLog(err_level::info, err_code::StrStationDB, m_strStationDB);
	m_output.OutputLog(err_level::info, err_code::StrPoiDB, m_strPoiDB);
	m_output.OutputLog(err_level::info, err_code::StrUnderDB, m_strUnderDB);
	m_output.OutputLog(err_level::info, err_code::StrMeshDB, m_strMeshDB);
	m_output.OutputLog(err_level::info, err_code::StrMeshList, m_strMeshList);
	m_output.OutputLog(err_level::info, err_code::StrHeightNodeINI, m_strHeightNodeIni);
	m_output.OutputLog(err_level::info, err_code::StrDistThreshold, m_strDistThreshold);
	m_output.OutputLog(err_level::info, err_code::StrNeiThreshold, m_strNeighborThreshold);
	m_output.OutputLog(err_level::info, err_code::StrRefThreshold, m_strReferIDThreshold);
	m_output.OutputLog(err_level::info, err_code::StrAngleThreshold, m_strAngleThreshold);
	m_output.OutputLog(err_level::info, err_code::StrBuddyDistThreshold, m_strBuddyDistThreshold);
	m_output.OutputLog(err_level::info, err_code::StrBuddyAngleThreshold, m_strBuddyAngleThreshold);
	m_output.OutputLog(err_level::info, err_code::StrDayFilter, m_filterDay);
	m_output.OutputLog(err_level::info, err_code::StrTimeFilter, m_filterTime);

	m_output.OutputLog(err_level::info, err_code::StrStartTime, util::GetCurTime());
}

void CCheckWalk::LoadMesh()
{
	std::set<string> baseMeshSet;
	for (const auto& cityMesh : m_cityMeshList) {
		CString  strCityMesh = uh::str_util::ToString(cityMesh);
		baseMeshSet.insert(std::to_string(cityMesh / 100));

		// �Ώۃ��b�V���|���S���Ǝ���8���b�V���܂߂��|���S���擾
		IGeometryPtr ipMeshGeo, ipUnionMeshGeo;
		if (!GetMeshPolygons(cityMesh, ipMeshGeo, ipUnionMeshGeo)) {
			m_output.OutputStdErr(err_level::fatal, err_code::GetNineMeshFail, strCityMesh);
			m_output.OutputLog(err_level::fatal, err_code::GetNineMeshFail, strCityMesh);
			continue;
		}
		m_cityMeshMap[cityMesh] = make_pair(ipMeshGeo, ipUnionMeshGeo);
	}

	ITablePtr table;
	m_database.GetTable(table::c_base_mesh, table);
	m_baseMeshMap = util::getBaseMesh(baseMeshSet, table);
}

void CCheckWalk::CheckByCityMesh()
{
	switch (m_checkTarget)
	{
	case properties::check_target::all:
		CheckFunctionByCityMesh(&CCheckWalk::allCheck);
		allCheckException();
		break;
	case properties::check_target::humancar:
		CheckFunctionByCityMesh(&CCheckWalk::humancarCheck);
		break;
	case properties::check_target::walkable:
		CheckFunctionByCityMesh(&CCheckWalk::walkableCheck);
		break;
	case properties::check_target::height_node:
		CheckFunctionByCityMesh(&CCheckWalk::heightNodeCheck);
		heightNodeCheckException();
		break;
	case properties::check_target::link_relation:
		CheckFunctionByCityMesh(&CCheckWalk::linkRelationCheck);
		break;
	case properties::check_target::under:
		CheckFunctionByCityMesh(&CCheckWalk::underGroundCheck);
		break;
	case properties::check_target::other:
		CheckFunctionByCityMesh(&CCheckWalk::otherCheck);
		otherCheckException();
		break;
	case properties::check_target::release:
		CheckFunctionByCityMesh(&CCheckWalk::releaseCheck);
		releaseCheckException();
		break;
	case properties::check_target::pedxing:
		CheckFunctionByCityMesh(&CCheckWalk::pedxingCheck);
		pedxingCheckException();
		break;
	default:
		assert(1);
		break;
	}
}

void CCheckWalk::CheckByAllMesh()
{
	switch (m_checkTarget)
	{
	case properties::check_target::all:
	case properties::check_target::link_relation:
	case properties::check_target::other:
	case properties::check_target::release:
	{
		// �S���b�V�����ʕ��s�Ҋ֘A�e�[�u���`�F�b�N�J�n
		m_output.OutputStdErr(err_level::info, err_code::StrStart, _T("ALL MESH"));
		m_output.OutputLog(err_level::info, err_code::StrStart, _T("ALL MESH"));
		m_output.SetMeshCode(_T(""));
		const FIELD_LIST fieldList = boost::assign::list_of
		(CString(walk_link::kObjectID))
			(CString(walk_link::kWalkCode))
			(CString(walk_link::kLinkRelationRF));

		CFieldMap fieldMap;
		_ICursorPtr ipCursor;
		if (!GetTableCursorAndFieldMap(c_walk_link, fieldList, ipCursor, fieldMap))
		{
			m_output.OutputStdErr(err_level::fatal, err_code::GetWalkFail, walk_link::kTableName);
			m_output.OutputLog(err_level::fatal, err_code::GetWalkFail, walk_link::kTableName);
		}

		std::set<long> walkLinkOIDSet, walkCodeSet, linkRelOIDSet;
		_IRowPtr ipRow;
		long walkCodeIndex = fieldMap.FindField(walk_link::kWalkCode);
		long linkRelationRFIndex = fieldMap.FindField(walk_link::kLinkRelationRF);
		while (S_OK == ipCursor->NextRow(&ipRow) && ipRow)
		{
			walkLinkOIDSet.insert(ipRow->GetOID());
			walkCodeSet.insert(ipRow->GetValue(walkCodeIndex).lVal);
			if (1 == ipRow->GetValue(linkRelationRFIndex).lVal)
				linkRelOIDSet.insert(ipRow->GetOID());
		}

		switch (m_checkTarget)
		{
		case properties::check_target::all:
			// other�̃`�F�b�N�����s���邽�߁Abreak�͏����Ȃ�
			CheckLinkRelationAll(linkRelOIDSet, m_linkRelationMap);
		case properties::check_target::other:
		{
			WalkCodeListCheckFunction codeCheck(m_output, m_modifyFilter);
			codeCheck.checkWalkCodeList(walkCodeSet, m_walkCodeMap);
			CheckWalkNoPassageAll(walkLinkOIDSet, m_walkNoPassageMap);
			CheckWalkOnewayAll(walkLinkOIDSet, m_walkOnewayMap);
		}
		break;
		case properties::check_target::release:
		{
			CheckLinkRelationAll(linkRelOIDSet, m_linkRelationMap);
			WalkCodeListCheckFunction codeCheck(m_output, m_modifyFilter);
			codeCheck.checkRelease(m_walkCodeMap);
			CheckWalkNoPassageAll(walkLinkOIDSet, m_walkNoPassageMap);
			CheckWalkOnewayAll(walkLinkOIDSet, m_walkOnewayMap);
		}
		break;
		case properties::check_target::link_relation:
			CheckLinkRelationAll(linkRelOIDSet, m_linkRelationMap);
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

}

void CCheckWalk::CheckByBaseMesh()
{
	switch (m_checkTarget)
	{
	case properties::check_target::all:
	case properties::check_target::other:
	case properties::check_target::release:
		checkBaseMesh();
		break;
	default:
		break;
	}
}

void CCheckWalk::CheckFunctionByCityMesh(err_code::ECode(CCheckWalk::*function)(const CityMeshPair&))
{
	err_code::ECode fatalErr = err_code::NoneError;
	for (const auto& cityMeshMapRec : m_cityMeshMap)
	{
		CString strCityMesh = uh::str_util::ToString(cityMeshMapRec.first);

		m_output.OutputStdErr(err_level::info, err_code::StrStart, strCityMesh);
		m_output.OutputLog(err_level::info, err_code::StrStart, strCityMesh);
		m_output.SetMeshCode(strCityMesh);
		fatalErr = (this->*function)(cityMeshMapRec);
		if (err_code::NoneError != fatalErr)
		{
			m_output.OutputStdErr(err_level::fatal, fatalErr, strCityMesh);
			m_output.OutputLog(err_level::fatal, fatalErr, strCityMesh);
		}
	}
}

err_code::ECode CCheckWalk::allCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree, segmentAttrRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);
	util::createRTree(segmentAttr.m_FeatureMap, segmentAttrRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	FeaturesPack underground = CreateFeaturesPack(c_underground_area);
	if (!GetUndergroundInMesh(ipMeshUnionGeo, underground))
		return err_code::GetUndergroundFail;

	// �w�o������|�C���g�擾
	FeaturesPack stationGate = CreateFeaturesPack(c_station_gate);
	if (!GetStationGateInMesh(ipMeshUnionGeo, stationGate))
		return err_code::GetStationFail;

	// POI�|�C���g�i�n���X�o�����j�擾
	FeaturesPack poiPoint = CreateFeaturesPack(c_poi_point_org);
	if (!GetPoiPointInMesh(ipMeshUnionGeo, poiPoint))
		return err_code::GetPoiPointFail;

	// �d�Ȃ胊���N�����m�[�h�擾&RTree�쐬
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// �X�N�����u���G���A
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// ���f�|�C���g
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODE�`�F�b�N
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkHeightNode(ipMeshGeo, roadLink, segmentAttr,
			walkLink, heightNode, nodeRTree, segmentAttrRTree, walkNodeRTree, heightNodeRTree, properties::check_target::all);
		HeightNodePositionCheckFunction posCheck(m_output, m_heightNodeMgr, m_coordinateTolerance);
		posCheck.checkPosition(meshCode, ipMeshGeo, roadLink,
			walkLink, walkNode, heightNode, heightNodeRTree);
	}
	{
		LinkRelationCheckFunction relCheck(m_output, m_modifyFilter, m_buddy_dist_threshold, m_buddy_angle_threshold);
		relCheck.checkLinkRelation(meshCode, ipMeshGeo, roadLink, walkLink, m_linkRelationMap, false);
	}
	// RoadLink�P�ƃ`�F�b�N
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.CheckRoadLink(ipMeshGeo, roadLink, roadNode, nodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs, walkLinkRTree, walkNodeRTree);
	}
	// RoadNode�P�ƃ`�F�b�N
	CheckRoadNode(meshCode, ipMeshGeo, roadNode, walkNode, walkNodeRTree);
	// WalkLink�P�ƃ`�F�b�N
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.CheckWalkLink(meshCode, ipMeshGeo, walkLink, walkNode, roadLink, roadNode,
			walkLinkRTree, linkRTree, nodeRTree, walkNodeLinkIDs, false, scrambleArea, scrambleAreaRTree);
	}
	// WalkNode�P�ƃ`�F�b�N
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.CheckWalkNode(meshCode, ipMeshGeo, roadLink, roadNode, walkLink, walkNode,
			stationGate, poiPoint, scrambleArea, heightNode,
			linkRTree, nodeRTree, walkLinkRTree, walkNodeRTree, scrambleAreaRTree, heightNodeRTree,
			walkNodeLinkIDs, m_baseMeshMap, false);
	}
	// WalkNoPassage�`�F�b�N
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkWalkNoPassage(ipMeshGeo, walkLink, m_walkNoPassageMap);
	}
	// WalkOneway�`�F�b�N
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkWalkOneway(ipMeshGeo, walkLink.m_FeatureMap, m_walkOnewayMap, m_walkNoPassageMap);
	}
	// StatinGatePoint�`�F�b�N
	CheckStationGatePoint(meshCode, ipMeshGeo, stationGate, walkNode, walkNodeRTree);

	// PoiPoin�`�F�b�N
	{
		POICheckFunction poi(m_output, m_refer_id_threshold);
		poi.CheckPoiPoint(meshCode, ipMeshGeo, poiPoint, walkNode, walkNodeRTree);
	}
	// �n���X�����`�F�b�N
	{
		UndergroundCheckFunction under(m_output, m_modifyFilter);
		under.CheckUnderground(meshCode, ipMeshGeo, underground, walkLink);
	}

	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::all);
	}

	// �X�N�����u���G���A�P�ƃ`�F�b�N
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.check(meshCode, ipMeshGeo, scrambleArea, walkLink,
			walkLinkRTree, properties::check_target::all);
	}

	return err_code::NoneError;
}

err_code::ECode CCheckWalk::humancarCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;


	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.checkHumanCar(meshCode, ipMeshGeo, walkLink, roadLink, linkRTree);
	}
	{
		LinkRelationCheckFunction relCheck(m_output, m_modifyFilter, m_buddy_dist_threshold, m_buddy_angle_threshold);
		relCheck.checkHumanCarOrWalkable(meshCode, ipMeshGeo, roadLink, walkLink, m_linkRelationMap);
	}

	return err_code::NoneError;
}

err_code::ECode CCheckWalk::walkableCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.checkWalkable(ipMeshGeo, roadLink, roadNode, nodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs, walkLinkRTree, walkNodeRTree);
	}
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.checkHumanCar(meshCode, ipMeshGeo, walkLink, roadLink, linkRTree);
	}
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.checkWalkable(ipMeshGeo, roadLink, roadNode, walkNode,
			linkRTree, nodeRTree, walkNodeLinkIDs);
	}

	{
		LinkRelationCheckFunction relCheck(m_output, m_modifyFilter, m_buddy_dist_threshold, m_buddy_angle_threshold);
		relCheck.checkHumanCarOrWalkable(meshCode, ipMeshGeo, roadLink, walkLink, m_linkRelationMap);
	}

	return err_code::NoneError;
}

err_code::ECode CCheckWalk::heightNodeCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree nodeRTree, segmentAttrRTree;
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);
	util::createRTree(segmentAttr.m_FeatureMap, segmentAttrRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkNodeRTree;
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// �d�Ȃ胊���N�����m�[�h�擾&RTree�쐬
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODE�`�F�b�N
	HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
	heightCheck.checkHeightNode(ipMeshGeo, roadLink, segmentAttr,
		walkLink, heightNode, nodeRTree, segmentAttrRTree, walkNodeRTree, heightNodeRTree, properties::check_target::height_node );

	HeightNodePositionCheckFunction posCheck(m_output, m_heightNodeMgr, m_coordinateTolerance);
	posCheck.checkPosition(meshCode, ipMeshGeo, roadLink,
		walkLink, walkNode, heightNode, heightNodeRTree);
	return err_code::NoneError;
}

err_code::ECode CCheckWalk::linkRelationCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�擾
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	// ���H�����N�擾
	if (!util::GetFeaturesInGeometry(ipMeshUnionGeo, field_lists::roadLinkFieldList, _T(""), roadLink))
		return err_code::GetRoadFail;


	// ���s�҃����N�擾
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	// ���s�҃����N�擾
	if (!util::GetFeaturesInGeometry(ipMeshUnionGeo, field_lists::walkLinkFieldList, _T(""), walkLink))
		return err_code::GetWalkFail;

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	LinkRelationCheckFunction relCheck(m_output, m_modifyFilter, m_buddy_dist_threshold, m_buddy_angle_threshold);
	relCheck.checkLinkRelation(meshCode, ipMeshGeo, roadLink, walkLink, m_linkRelationMap, true);

	return err_code::NoneError;
}

err_code::ECode CCheckWalk::underGroundCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���s�҃����N�擾
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	if (!util::GetFeaturesInGeometry(ipMeshUnionGeo, field_lists::walkLinkFieldList, _T(""), walkLink))
		return err_code::GetWalkFail;


	FeaturesPack underground = CreateFeaturesPack(c_underground_area);
	if (!GetUndergroundInMesh(ipMeshUnionGeo, underground))
		return err_code::GetUndergroundFail;

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	// �n���X�����`�F�b�N
	UndergroundCheckFunction under(m_output, m_modifyFilter);
	under.CheckUnderground(meshCode, ipMeshGeo, underground, walkLink);
	return err_code::NoneError;
}

err_code::ECode CCheckWalk::otherCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	FeaturesPack underground = CreateFeaturesPack(c_underground_area);
	if (!GetUndergroundInMesh(ipMeshUnionGeo, underground))
		return err_code::GetUndergroundFail;

	// �w�o������|�C���g�擾
	FeaturesPack stationGate = CreateFeaturesPack(c_station_gate);
	if (!GetStationGateInMesh(ipMeshUnionGeo, stationGate))
		return err_code::GetStationFail;

	// POI�|�C���g�i�n���X�o�����j�擾
	FeaturesPack poiPoint = CreateFeaturesPack(c_poi_point_org);
	if (!GetPoiPointInMesh(ipMeshUnionGeo, poiPoint))
		return err_code::GetPoiPointFail;

	// �d�Ȃ胊���N�����m�[�h�擾&RTree�쐬
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// �X�N�����u���G���A
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// ���f�|�C���g
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODE�`�F�b�N
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkOther(ipMeshGeo, heightNode);
	}
	// RoadLink�P�ƃ`�F�b�N
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.checkOther(ipMeshGeo, roadLink, nodeLinkIDs);
	}
	// RoadNode�P�ƃ`�F�b�N
	CheckRoadNode(meshCode, ipMeshGeo, roadNode, walkNode, walkNodeRTree);
	// WalkLink�P�ƃ`�F�b�N
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.CheckWalkLink(meshCode, ipMeshGeo, walkLink, walkNode, roadLink, roadNode,
			walkLinkRTree, linkRTree, nodeRTree, walkNodeLinkIDs, true, scrambleArea, scrambleAreaRTree);
	}
	// WalkNode�P�ƃ`�F�b�N
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.CheckWalkNode(meshCode, ipMeshGeo, roadLink, roadNode, walkLink, walkNode,
			stationGate, poiPoint, scrambleArea, heightNode,
			linkRTree, nodeRTree, walkLinkRTree, walkNodeRTree, scrambleAreaRTree, heightNodeRTree,
			walkNodeLinkIDs, m_baseMeshMap, true);
	}
	// WalkNoPassage�`�F�b�N
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkWalkNoPassage(ipMeshGeo, walkLink, m_walkNoPassageMap);
	}
	// WalkOneway�`�F�b�N
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkWalkOneway(ipMeshGeo, walkLink.m_FeatureMap, m_walkOnewayMap, m_walkNoPassageMap);
	}
	// StatinGatePoint�`�F�b�N
	CheckStationGatePoint(meshCode, ipMeshGeo, stationGate, walkNode, walkNodeRTree);
	// PoiPoin�`�F�b�N
	{
		POICheckFunction poi(m_output, m_refer_id_threshold);
		poi.CheckPoiPoint(meshCode, ipMeshGeo, poiPoint, walkNode, walkNodeRTree);
	}
	// �n���X�����`�F�b�N
	{
		UndergroundCheckFunction under(m_output, m_modifyFilter);
		under.CheckUnderground(meshCode, ipMeshGeo, underground, walkLink);
	}

	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::other);
	}

	// �X�N�����u���G���A�P�ƃ`�F�b�N
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.check(meshCode, ipMeshGeo, scrambleArea, walkLink,
			walkLinkRTree, properties::check_target::other);
	}

	return err_code::NoneError;
}

err_code::ECode CCheckWalk::releaseCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree, segmentAttrRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);
	util::createRTree(segmentAttr.m_FeatureMap, segmentAttrRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// �w�o������|�C���g�擾
	FeaturesPack stationGate = CreateFeaturesPack(c_station_gate);
	if (!GetStationGateInMesh(ipMeshUnionGeo, stationGate))
		return err_code::GetStationFail;

	// POI�|�C���g�i�n���X�o�����j�擾
	FeaturesPack poiPoint = CreateFeaturesPack(c_poi_point_org);
	if (!GetPoiPointInMesh(ipMeshUnionGeo, poiPoint))
		return err_code::GetPoiPointFail;

	// �d�Ȃ胊���N�����m�[�h�擾&RTree�쐬
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// ���f�|�C���g
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// �X�N�����u���G���A
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODE�`�F�b�N
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkRelease(ipMeshGeo, roadLink, segmentAttr,
			walkLink, heightNode, nodeRTree, segmentAttrRTree, walkNodeRTree, heightNodeRTree, properties::check_target::release );
		HeightNodePositionCheckFunction posCheck(m_output, m_heightNodeMgr, m_coordinateTolerance);
		posCheck.checkPosition(meshCode, ipMeshGeo, roadLink,
			walkLink, walkNode, heightNode, heightNodeRTree);
	}

	LinkRelationCheckFunction relCheck(m_output, m_modifyFilter, m_buddy_dist_threshold, m_buddy_angle_threshold);
	relCheck.checkRelease(ipMeshGeo, roadLink, walkLink, m_linkRelationMap);

	// RoadLink�P�ƃ`�F�b�N
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.checkRelease(ipMeshGeo, roadLink);
	}
	// WalkLink�P�ƃ`�F�b�N
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.checkRelease(meshCode, ipMeshGeo, walkLink, walkNode, roadLink, roadNode,
			walkLinkRTree, linkRTree, nodeRTree, walkNodeLinkIDs);
	}
	// WalkNode�P�ƃ`�F�b�N
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.checkRelease(meshCode, ipMeshGeo, roadLink, roadNode, walkLink, walkNode,
			stationGate, poiPoint, heightNode,
			linkRTree, nodeRTree, walkLinkRTree, walkNodeRTree, heightNodeRTree,
			walkNodeLinkIDs, m_baseMeshMap);
	}
	// WalkNoPassage�`�F�b�N
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkRelease(ipMeshGeo, walkLink, m_walkNoPassageMap);
	}
	// WalkOneway�`�F�b�N
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkRelease(ipMeshGeo, walkLink, m_walkOnewayMap);
	}

	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::release);
	}

	// �X�N�����u���G���A�P�ƃ`�F�b�N
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.check(meshCode, ipMeshGeo, scrambleArea, walkLink,
			walkLinkRTree, properties::check_target::release);
	}

	return err_code::NoneError;
}


void CCheckWalk::checkBaseMesh()
{
	for (const auto& baseMeshCode2Geometry : m_baseMeshMap)
	{
		int baseMesh = _ttoi(baseMeshCode2Geometry.first);
		const auto& strBaseMesh = baseMeshCode2Geometry.first;

		m_output.OutputStdErr(err_level::info, err_code::StrStart, strBaseMesh);
		m_output.OutputLog(err_level::info, err_code::StrStart, strBaseMesh);

		// TODO: baseMeshMap�̌`����g�p����悤�ɏC��
		// �Ώۃ��b�V���|���S���Ǝ���8���b�V���܂߂��|���S���擾
		IGeometryPtr ipBaseMeshGeo, ipUnionMeshGeo;
		if (!GetMeshPolygons(baseMesh, ipBaseMeshGeo, ipUnionMeshGeo)) {
			m_output.OutputStdErr(err_level::fatal, err_code::GetNineMeshFail, strBaseMesh);
			m_output.OutputLog(err_level::fatal, err_code::GetNineMeshFail, strBaseMesh);
			continue;
		}

		ITopologicalOperatorPtr ipMeshTopo(ipBaseMeshGeo);
		IGeometryPtr ipBoundaryMesh;
		if (FAILED(ipMeshTopo->get_Boundary(&ipBoundaryMesh))) {
			m_output.OutputStdErr(err_level::fatal, err_code::GetMeshBoundFail, strBaseMesh);
			m_output.OutputLog(err_level::fatal, err_code::GetMeshBoundFail, strBaseMesh);
			return;
		}
		ipBoundaryMesh->PutRefSpatialReference(ipBaseMeshGeo->GetSpatialReference());

		switch (m_checkTarget)
		{
		case properties::check_target::all:
		case properties::check_target::other:
		{
			FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
			FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
			NODEIDMAP walkNodeNodeIDs, walkNodeLinkIDs;
			// ���s�҃����N�E���s�҃m�[�h�擾
			if (!GetWalkInMesh(ipUnionMeshGeo, walkLink, walkNode, walkNodeNodeIDs, walkNodeLinkIDs)) {
				m_output.OutputStdErr(err_level::fatal, err_code::GetWalkFail, strBaseMesh);
				m_output.OutputLog(err_level::fatal, err_code::GetWalkFail, strBaseMesh);
				continue;
			}
			// �l�b�g���[�N�`�F�b�N
			CheckWalkRel(ipBaseMeshGeo, m_cityMeshMap, walkNode, walkNodeNodeIDs, walkNodeLinkIDs);
			// WalkLink�P�ƃ`�F�b�N
			{
				WalkLinkOfBaseMeshCheckFunction walkCheck(m_output, m_modifyFilter, m_onewayClassMap, m_walkNoPassageMap);
				walkCheck.checkWalkLinkOfBaseMesh(ipBaseMeshGeo, ipBoundaryMesh, m_cityMeshMap,
					walkLink, walkNode, walkNodeLinkIDs);
			}
		}
		break;
		case properties::check_target::release:
			// WalkLink�P�ƃ`�F�b�N
		{
			FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
			FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
			NODEIDMAP walkNodeLinkIDs;
			// ���s�҃����N�E���s�҃m�[�h�擾
			if (!GetWalkInMesh(ipUnionMeshGeo, walkLink, walkNode, walkNodeLinkIDs)) {
				m_output.OutputStdErr(err_level::fatal, err_code::GetWalkFail, strBaseMesh);
				m_output.OutputLog(err_level::fatal, err_code::GetWalkFail, strBaseMesh);
				continue;
			}
			WalkLinkOfBaseMeshCheckFunction walkCheck(m_output, m_modifyFilter, m_onewayClassMap, m_walkNoPassageMap);
			walkCheck.checkRelease(ipBaseMeshGeo, ipBoundaryMesh, m_cityMeshMap,
				walkLink, walkNode, walkNodeLinkIDs);
		}
		break;
		default:
			break;
		}
	}
}


// ������
bool CCheckWalk::init()
{
	// �I�v�V����data_type�Ɉُ�l(1�A2�ȊO�̒l)���w�肳���ۂɂ́A�I�v�V�����G���[�̃��b�Z�[�W���o�͂���
	if (m_dataType != walk_type::type_class::kGeneral && m_dataType != walk_type::type_class::kDownConvert)
	{
		m_output.OutputStdErr(err_level::fatal, err_code::DataTypeError, m_strOutput);
		return false;
	}

	// �I�v�V����data_type������l1(�ėp�ł̃f�[�^)���w�肵�A�ݒ�t�@�C����[target]�I�v�V������pedxing���w�肵�A�G���[���b�Z�[�W���o�͂���B
	if (m_checkTarget == properties::check_target::pedxing && m_dataType == walk_type::type_class::kGeneral)
	{
		m_output.OutputStdErr(err_level::fatal, err_code::DataTypeTargetError, m_strOutput);
		return false;
	}

	// ���O�t�@�C���N���X�I�[�v��
	if (!m_output.Open(m_strOutput)) {
		m_output.OutputStdErr(err_level::fatal, err_code::OpenLogFail, m_strOutput);
		return false;
	}

	CString db_result = m_database.Init(m_strRoadDB, m_strWalkDB, m_strStationDB, m_strPoiDB, m_strUnderDB, m_strMeshDB);
	if (!db_result.IsEmpty())
	{
		m_output.OutputStdErr(err_level::fatal, err_code::OpenWsFail, db_result);
		m_output.OutputLog(err_level::fatal, err_code::OpenMeshFileFail, db_result);
		return false;
	}

	db_result = m_database.OpenTables();
	if (!db_result.IsEmpty())
	{
		m_output.OutputStdErr(err_level::fatal, err_code::OpenTblFail, db_result);
		m_output.OutputLog(err_level::fatal, err_code::OpenTblFail, db_result);
		return false;
	}

	// �f�[�^�x�[�X����WALKTYPE_C�Ǝw��I�v�V�����l�������Ă��Ȃ����̃`�F�b�N
	if (getWalkTypeValue() != m_dataType)
	{
		// �f�[�^�x�[�X����WALKTYPE_C�Ǝw��I�v�V�����l�������Ă��Ȃ��ۂɂ́A�G���[�̃��b�Z�[�W���o�͂��āA�ُ�I��
		m_output.OutputStdErr(err_level::fatal, err_code::walkTypeError, m_strOutput);
		return false;
	}

	// ���b�V�����X�g�ǂݍ���
	ITablePtr cityMeshTable;
	m_database.GetTable(table::c_city_mesh, cityMeshTable);
	if (!loadMeshList(m_strMeshList, m_cityMeshList, cityMeshTable)) {
		m_output.OutputStdErr(err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList);
		m_output.OutputLog(err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList);
		return false;
	}

	// �K�v�ȃR�[�h�l�h���C��map���擾���Ă���
	CFieldMap fieldMap;
	m_database.GetFieldMap(table::c_walk_link, fieldMap);
	util::GetDomainMap(walk_link::kWalkClass, fieldMap, m_walkLinkClassMap);
	util::GetDomainMap(walk_link::kFloorMoveClass, fieldMap, m_floorMoveClassMap);
	util::GetDomainMap(walk_link::kOneway, fieldMap, m_onewayClassMap);
	util::GetDomainMap(walk_link::kRoadSurface, fieldMap, m_roadSurfaceClassMap);

	m_database.GetFieldMap(table::c_walk_node, fieldMap);
	util::GetDomainMap(walk_node::kNodeClass, fieldMap, m_walkNodeClassMap);

	m_database.GetFieldMap(table::c_road_link, fieldMap);
	util::GetDomainMap(road_link::kWalkable, fieldMap, m_walkableMap);

	// �d�Ȃ胊���N���� �ݒ�t�@�C��
	if (!m_heightNodeMgr.read(m_height_node_ini))
		return false;

	// ���s�Ҋ֘A�e�[�u����S�Ď擾���Ă���
	if (!GetWalkRelationTables()) {
		m_output.OutputStdErr(err_level::fatal, err_code::GetWalkRelFail, _T(""));
		m_output.OutputLog(err_level::fatal, err_code::GetWalkRelFail, _T(""));
		return false;
	}

	// ���s�҃m�[�h�̍��W���e�l�擾
	// ���H�����N�̕ӏ�ɐl�ԋ��p�����N�̒[�_������Ă��邩�ǂ����𔻒肷��Ƃ��Ɏg�p����
	ITablePtr walkNodeTable;
	m_database.GetTable(table::c_walk_node, walkNodeTable);
	m_coordinateTolerance = AheGetXYDomain1UnitLength(AheGetSpatialReference(walkNodeTable));
	return true;
}

// ���s�҃^�C�v�e�[�u������WALK_TYPE�̒l���擾����B
long CCheckWalk::getWalkTypeValue()
{
	// ���s�҃^�C�v�e�[�u���ǂݍ���
	ITablePtr ipWalkTypeTable;
	m_database.GetTable(table::c_walk_type, ipWalkTypeTable);
	if (!ipWalkTypeTable)
		return walk_type::type_class::kGeneral;

	// WALK_TYPE�̃C���f�b�N�X���擾����
	long indexWalkType(-1L);
	ipWalkTypeTable->FindField(CComBSTR(walk_type::kWalkTypeClass), &indexWalkType);

	// WALK_TYPE�̒l���擾��
	_ICursorPtr ipWalkTypeCursor;
	if (FAILED(ipWalkTypeTable->Search(AheInitQueryFilter(nullptr, nullptr, nullptr), VARIANT_FALSE, &ipWalkTypeCursor)) || !ipWalkTypeCursor)
		return walk_type::type_class::kGeneral;

	_IRowPtr ipWalkTypeRow;
	while (S_OK == ipWalkTypeCursor->NextRow(&ipWalkTypeRow) && ipWalkTypeRow)
	{
		CComVariant walkType = ipWalkTypeRow->GetValue(indexWalkType);
		return walkType.lVal;
	}
	return walk_type::type_class::kGeneral;
}

bool CCheckWalk::loadMeshList(const CString& fileName, std::set<long>& integerList, const ITablePtr& cityMeshTable)
{
	enum class NUM_OF_MESH_DIGITS : size_t
	{
		BASEMESH = 6,
		CITYMESH = 8,
	};

	std::ifstream ifs(fileName);

	// �X�g���[�����擾�ł��Ȃ����false
	if (!ifs) return false;

	bool bSuccess = true;
	std::string line;
	for (int lineNum = 0; !ifs.eof() && std::getline(ifs, line); ++lineNum)
	{
		// �����łȂ����͖̂���
		if (!std::all_of(line.cbegin(), line.cend(), isdigit))
		{
			m_output.OutputStdErr(err_level::fatal, err_code::NumErrInMeshList, m_strOutput);
			bSuccess = false;
			continue;
		}

		switch (line.size())
		{
		case static_cast<size_t>(NUM_OF_MESH_DIGITS::BASEMESH) :
		{
			auto cityMeshs = getCityMeshCodeOnBaseMesh(std::stol(line), cityMeshTable);
			integerList.insert(cityMeshs.cbegin(), cityMeshs.cend());
		}
															   break;
		case static_cast<size_t>(NUM_OF_MESH_DIGITS::CITYMESH) :
			// �s�s�n�}���b�V��
			integerList.insert(std::stol(line));
			break;
		default: // ����������Ȃ����̂̓G���[
			m_output.OutputStdErr(err_level::fatal, err_code::DigitErrInMeshList, m_strOutput);
			bSuccess = false;
			continue;
			break;
		}
	}
	return bSuccess;
}

// ���s�Ҋ֘A�e�[�u������S���擾���Ă���
bool CCheckWalk::GetWalkRelationTables()
{
	if (!GetRelationTable(table::c_walk_code_list, field_lists::walkCodeFieldList, walk_code_list::kWalkCode, m_walkCodeMap))
		return false;

	if (!GetRelationTable(table::c_walk_nopassage, field_lists::walkNoPassageFieldList, walk_nopassage::kLinkID, m_walkNoPassageMap))
		return false;

	if (!GetRelationTable(table::c_walk_oneway, field_lists::walkOnewayFieldList, walk_oneway::kLinkID, m_walkOnewayMap))
		return false;

	if (!GetRelationTable(table::c_link_relation, field_lists::linkRelationFieldList, link_relation::kBaseLinkID, m_linkRelationMap))
		return false;

	return true;
}


// �^�������b�V���א�9���b�V�����X�g���쐬����
void CCheckWalk::Get9Mesh(long mesh, std::set<long>& meshlist)
{
	meshlist.insert(mesh);                              // ���S
	meshlist.insert(g_cnv.GetMeshCode(mesh, -1, 1)); // ����
	meshlist.insert(g_cnv.GetMeshCode(mesh, 0, 1)); // ��
	meshlist.insert(g_cnv.GetMeshCode(mesh, 1, 1)); // �E��
	meshlist.insert(g_cnv.GetMeshCode(mesh, -1, 0)); // ��
	meshlist.insert(g_cnv.GetMeshCode(mesh, 1, 0)); // �E
	meshlist.insert(g_cnv.GetMeshCode(mesh, -1, -1)); // ����
	meshlist.insert(g_cnv.GetMeshCode(mesh, 0, -1)); // ��
	meshlist.insert(g_cnv.GetMeshCode(mesh, 1, -1)); // �E��
}

// �w�肳�ꂽ���b�V���R�[�h�̃��b�V���|���S���A�y�ю���9���b�V���̃��b�V���|���S�����擾
bool CCheckWalk::GetMeshPolygons(long mesh, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo)
{
	table meshTable;
	if (10000000 <= mesh && mesh < 100000000)
		meshTable = table::c_city_mesh;
	else if (100000 <= mesh && mesh < 1000000)
		meshTable = table::c_base_mesh;
	else
		return false;

	std::set<long> nineMeshList;
	Get9Mesh(mesh, nineMeshList);

	ISpatialReferencePtr ipSpRef;
	CString strWhere;
	strWhere.Format(_T("%s IN (%s)"), category::mesh_code_table::kMeshCode, uh::str_util::join(nineMeshList, _T(",")));

	ITablePtr ipMeshFC;
	if (!m_database.GetTable(meshTable, ipMeshFC))
	{
		m_output.OutputLog(err_level::error, 9999, _T("m_database.GetTable()"));
		return false;
	}

	IFeatureCursorPtr ipMeshCursor = ((IFeatureClassPtr)ipMeshFC)->_Search(AheInitQueryFilter(NULL, NULL, strWhere), VARIANT_FALSE);
	if (!ipMeshCursor)
	{
		m_output.OutputLog(err_level::error, 9999, _T("ipMeshCursor"));
		return false;
	}

	CFieldMap meshFM;
	if (!m_database.GetFieldMap(meshTable, meshFM))
	{
		m_output.OutputLog(err_level::error, 9999, _T("m_database.GetFieldMap()"));
		return false;
	}

	int i = 0;
	IFeaturePtr ipMeshFeature;
	IGeometryCollectionPtr ipGeoCol(CLSID_GeometryBag);
	while (S_OK == ipMeshCursor->NextFeature(&ipMeshFeature) && ipMeshFeature) {

		IGeometryPtr ipTempMeshGeo = ipMeshFeature->GetShapeCopy();
		if (!ipTempMeshGeo)
		{
			m_output.OutputLog(err_level::error, 9999, _T("ipTempMeshGeo"));
			return false;
		}

		// ��ԎQ�Ƃ��擾���Ă���
		if (i == 0) {
			ipTempMeshGeo->get_SpatialReference(&ipSpRef);
			((IGeometryPtr)ipGeoCol)->putref_SpatialReference(ipSpRef);
		}

		// ���g�̃��b�V�����擾���Ă���
		if (mesh == (ipMeshFeature->GetValue(meshFM.FindField(category::mesh_code_table::kMeshCode))).lVal)
			ipMeshGeo = ipTempMeshGeo;

		// ��ňꊇ��ConstructUnion���邽�߂ɁA�W�I���g�����i�[����
		ipGeoCol->AddGeometry(ipTempMeshGeo);

		++i;
	}

	// �l�߂����̂�ConstructUnion
	IGeometryPtr ipTempUnionMeshGeo(CLSID_Polygon);
	IEnumGeometryPtr ipEnumGeo(ipGeoCol);
	auto hr = ((ITopologicalOperatorPtr)ipTempUnionMeshGeo)->ConstructUnion(ipEnumGeo);
	if (FAILED(hr))
	{
		m_output.OutputLog(err_level::error, 9999, uh::str_util::format(_T("ConstructUnion: %ld"), hr));
		return false;
	}

	ipTempUnionMeshGeo->putref_SpatialReference(ipSpRef);
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->put_IsKnownSimple(VARIANT_FALSE);
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->Simplify();

	ipUnionMeshGeo = ipTempUnionMeshGeo;

	return true;
}

bool CCheckWalk::GetFeatureCursorAndFieldMap(
	const table tableValue,
	const FIELD_LIST& fieldList,
	const IGeometryPtr& ipMeshGeo,
	const CString& strWhere,
	IFeatureCursorPtr& ipCursor,
	CFieldMap& fieldMap)
{
	ITablePtr ipFeatureClass;
	if (!m_database.GetTable(tableValue, ipFeatureClass))
		return false;

	CString subField = uh::str_util::join(fieldList, _T(","));
	ipCursor = ((IFeatureClassPtr)ipFeatureClass)->_Search(AheInitQueryFilter(AheInitSpatialFilter(NULL, ipMeshGeo), subField, strWhere), VARIANT_FALSE);
	if (!ipCursor)
		return false;

	if (!m_database.GetFieldMap(tableValue, fieldMap))
		return false;

	return true;
}

bool CCheckWalk::GetTableCursor(const table tableValue, const FIELD_LIST& fieldList, _ICursorPtr& ipCursor)
{
	ITablePtr ipTable;
	if (!m_database.GetTable(tableValue, ipTable))
		return false;

	CString subField = uh::str_util::join(fieldList, _T(","));
	ipCursor = ipTable->_Search(AheInitQueryFilter(nullptr, subField, nullptr), VARIANT_FALSE);
	if (!ipCursor)
		return false;

	return true;
}

bool CCheckWalk::GetTableCursorAndFieldMap(
	const table tableValue,
	const FIELD_LIST& fieldList,
	_ICursorPtr& ipCursor,
	CFieldMap& fieldMap)
{
	if (!GetTableCursor(tableValue, fieldList, ipCursor))
		return false;

	if (!m_database.GetFieldMap(tableValue, fieldMap))
		return false;

	return true;
}

bool CCheckWalk::GetRelationTable(
	const table tableValue,
	const FIELD_LIST& fieldList,
	const CString& keyFieldName,
	RowsPack& dataMap)
{
	_ICursorPtr ipCursor;
	if (!GetTableCursorAndFieldMap(tableValue, fieldList, ipCursor, dataMap.m_FieldMap))
		return false;
	long keyIndex = dataMap.m_FieldMap.FindField(keyFieldName);
	if (keyIndex < 0) {
		return false;
	}
	_IRowPtr ipRow;
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow) {
		// �������擾���Amap�Ɋi�[����
		long keyValue = (ipRow->GetValue(keyIndex)).lVal;

		dataMap.m_Rows.emplace(keyValue, ipRow);
	}

	return true;
}

bool CCheckWalk::GetRelationTable(
	const table tableValue,
	const FIELD_LIST& fieldList,
	const CString& keyFieldName,
	MultiRowsPack& dataMap)
{
	_ICursorPtr ipCursor;
	if (!GetTableCursorAndFieldMap(tableValue, fieldList, ipCursor, dataMap.m_FieldMap))
		return false;
	long keyIndex = dataMap.m_FieldMap.FindField(keyFieldName);
	if (keyIndex < 0) {
		return false;
	}

	_IRowPtr ipRow;
	while (S_OK == ipCursor->NextRow(&ipRow) && ipRow) {
		// �������擾���Amap�Ɋi�[����
		long keyValue = (ipRow->GetValue(keyIndex)).lVal;
		dataMap.m_Rows.emplace(keyValue, ipRow);
	}

	return true;
}

// �w��e�[�u���ŏ���������C���[�W
FeaturesPack CCheckWalk::CreateFeaturesPack(table tableValue)
{
	ITablePtr ipTable;
	m_database.GetTable(tableValue, ipTable);
	CFieldMap fieldMap;
	m_database.GetFieldMap(tableValue, fieldMap);
	return FeaturesPack(ipTable, fieldMap);
}

// �^����ꂽ���b�V���`����̓��H�����N�Ɠ��H�m�[�h���擾
bool CCheckWalk::GetRoadInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& roadLink,
	FeaturesPack& roadNode,
	FeaturesPack& segmentAttr,
	NODEIDMAP& nodeLinkIDs)
{
	// ���H�����N�擾
	if (!util::GetLinkFeaturesInGeometry(ipMeshGeo, field_lists::roadLinkFieldList,
		_T(""), roadLink, nodeLinkIDs))
		return false;

	// ���H�m�[�h�擾
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::roadNodeFieldList,
		_T(""), roadNode))
		return false;

	// �����N�������擾
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::segmentAttrFieldList,
		_T(""), segmentAttr))
		return false;

	return true;
}

// �^����ꂽ���b�V���`����̕��s�҃����N�ƕ��s�҃m�[�h���擾
bool CCheckWalk::GetWalkInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeNodeIDs,
	NODEIDMAP& walkNodeLinkIDs)
{
	// ���s�҃����N�擾
	if (!util::GetLinkFeaturesWithNodeIdInGeometry(ipMeshGeo, field_lists::walkLinkFieldList,
		_T(""), walkLink, walkNodeNodeIDs, walkNodeLinkIDs))
		return false;

	// ���s�҃m�[�h�擾
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::walkNodeFieldList,
		_T(""), walkNode))
		return false;

	return true;
}

// �^����ꂽ���b�V���`����̕��s�҃����N�ƕ��s�҃m�[�h���擾
bool CCheckWalk::GetWalkInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeLinkIDs)
{
	// ���s�҃����N�擾
	if (!util::GetLinkFeaturesInGeometry(ipMeshGeo, field_lists::walkLinkFieldList,
		_T(""), walkLink, walkNodeLinkIDs))
		return false;

	// ���s�҃m�[�h�擾
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::walkNodeFieldList,
		_T(""), walkNode))
		return false;

	return true;
}

bool CCheckWalk::GetStationGateInMesh(const IGeometryPtr& ipMeshGeo, FeaturesPack& stationGate)
{
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::stationGatePointFieldList, _T(""), stationGate))
		return false;

	return true;
}

bool CCheckWalk::GetPoiPointInMesh(const IGeometryPtr& ipMeshGeo, FeaturesPack& poiPoint)
{
	CString strWhere;
	strWhere.Format(_T("%s = %ld"),
		sindyk::poi_point_org::kContentsCode, UNDERGROUNT_GATE_CONTENTS_CODE);
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::poiPointFieldList, strWhere, poiPoint))
		return false;

	return true;
}

bool CCheckWalk::GetHeightNodeInMesh(const IGeometryPtr& ipMeshGeo, FeaturesPack& heightNode)
{
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::heightNodeFieldList, _T(""), heightNode))
		return false;

	return true;
}
bool CCheckWalk::GetUndergroundInMesh(const IGeometryPtr& ipMeshGeo, FeaturesPack& underArea)
{
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::underGroundAeraFiledList,
		_T(""), underArea))
		return false;

	return true;
}

// �`�F�b�N
void CCheckWalk::CheckRoadNode(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& roadNode,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree)
{
	using namespace road_node;
	using namespace ipc_feature;

	m_output.OutputStdErr(err_level::info, err_code::StrChecking, road_node::kTableName);
	long nameKanjiIndex = roadNode.m_FieldMap.FindField(kNameKanji);
	long nameYomiIndex = roadNode.m_FieldMap.FindField(kNameYomi);
	long operatorIndex = roadNode.m_FieldMap.FindField(kOperator);
	long modifyDateIndex = roadNode.m_FieldMap.FindField(kModifyDate);
	long modifyProgIndex = roadNode.m_FieldMap.FindField(kModifyProgName);
	long progModifyIndex = roadNode.m_FieldMap.FindField(kProgModifyDate);

	// ���H�m�[�h�̃`�F�b�N
	for (const auto& node : roadNode.m_FeatureMap)
	{
		long  objectId = node.first;
		auto& nodeObj = node.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (nodeObj.IsDisjoint(ipMeshGeo))
			continue;

		auto nameKanji = nodeObj.GetStringValue(nameKanjiIndex);
		// ���g�ɖ��̂��Ȃ��ꍇ�X�L�b�v
		if (nameKanji.IsEmpty())
			continue;

		if (!util::existAroundWalkNodeSameName(nodeObj, roadNode, walkNode, walkNodeRTree,
			kNameKanji, kNameYomi, walk_node::kRoadNodeID, m_refer_id_threshold))
			// �����[�j���O �t�߂ɓ���̌����_���������s�҃m�[�h�����݂��Ȃ�
		{
			m_output.OutputLog(kTableName, objectId, err_level::warning, err_code::NotExistSameNameRND,
				meshcode, nodeObj.GetStringValue(operatorIndex), nodeObj.GetStringValue(modifyDateIndex),
				nodeObj.GetStringValue(modifyProgIndex), nodeObj.GetStringValue(progModifyIndex), nameKanji, nodeObj.GetStringValue(nameYomiIndex));
		}
	}

}

void CCheckWalk::CheckLinkRelationAll(
	const std::set<long>& linkRelOIDSet,
	const MultiRowsPack& linkRelation)
{
	using namespace link_relation;
	using namespace ipc_table;
	using namespace uh::str_util;
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, _T("LINK_RELATION(ALL MESH)"));
	long operatorIndex = linkRelation.m_FieldMap.FindField(kOperator);
	long modifyDateIndex = linkRelation.m_FieldMap.FindField(kModifyDate);
	long modifyProgIndex = linkRelation.m_FieldMap.FindField(kModifyProgName);
	long progModifyIndex = linkRelation.m_FieldMap.FindField(kProgModifyDate);
	long relatedLinkIdIndex = linkRelation.m_FieldMap.FindField(kRelatedLinkID);

	// �d�Ȃ胊���N�����e�[�u���Ń��[�v���A������N�̃����NID�����݂��邩�`�F�b�N
	// �������N�����[�V�����e�[�u���Ń��[�v���A�֘A�t���OON�̕��s�҃����N���X�g��ID�����݂��邩�`�F�b�N�ɂȂ��Ă邯��OK?
	//   �d���`�F�b�N���ǉ�[bug 12473]
	std::set<std::pair<long, long>> relMap;
	for (const auto& linkRel : linkRelation.m_Rows)
	{
		auto ite = linkRelOIDSet.find(linkRel.first);
		if (ite == linkRelOIDSet.end()) // ���G���[ ������NID�����s�҃����N�ɑ��݂��Ȃ�
		{
			m_output.OutputLog(walk_link::kTableName, linkRel.first,
				err_level::error, err_code::NotExistWalkLink, _T(""),
				ToString(linkRel.second->GetValue(operatorIndex)), ToString(linkRel.second->GetValue(modifyDateIndex)),
				ToString(linkRel.second->GetValue(modifyProgIndex)), ToString(linkRel.second->GetValue(progModifyIndex)),
				ToString(linkRel.second->GetOID()));
		}
		auto ret = relMap.emplace(linkRel.first, linkRel.second->GetValue(relatedLinkIdIndex).lVal);
		if (!ret.second) {
			m_output.OutputLog(walk_link::kTableName, linkRel.first,
				err_level::error, err_code::NgDuplicatedRecord, _T(""),
				ToString(linkRel.second->GetValue(operatorIndex)), ToString(linkRel.second->GetValue(modifyDateIndex)),
				ToString(linkRel.second->GetValue(modifyProgIndex)), ToString(linkRel.second->GetValue(progModifyIndex)),
				ToString(linkRel.second->GetOID()));
		}
	}
}

void CCheckWalk::CheckStationGatePoint(
	const CString& meshcode,
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& stationGate,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree)
{
	using namespace station_gate_point;
	using namespace ipc_feature;
	using namespace sindyk;
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, station_gate_point::kTableName);
	long nameIndex = stationGate.m_FieldMap.FindField(kName);
	long yomiIndex = stationGate.m_FieldMap.FindField(kYomi);
	long operatorIndex = stationGate.m_FieldMap.FindField(kOperator);
	long modifyDateIndex = stationGate.m_FieldMap.FindField(kModifyDate);
	long modifyProgIndex = stationGate.m_FieldMap.FindField(kModifyProgName);
	long progModifyIndex = stationGate.m_FieldMap.FindField(kProgModifyDate);

	// �w�o�����|�C���g�̃`�F�b�N
	for (const auto& staionGatePoint : stationGate.m_FeatureMap)
	{
		long  objectId = staionGatePoint.first;
		auto& pointObj = staionGatePoint.second;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (pointObj.IsDisjoint(ipMeshGeo))
			continue;

		auto name = pointObj.GetStringValue(nameIndex);

		// ���g�ɖ��̂��Ȃ��ꍇ�X�L�b�v
		if (name.IsEmpty())
			continue;

		if (!util::existAroundWalkNodeSameName(pointObj, stationGate, walkNode, walkNodeRTree,
			kName, kYomi, walk_node::kStationGatePointID, m_refer_id_threshold, walk_node::node_class::kGate))
			// �����[�j���O �t�߂ɓ���̉w�o�������������s�҃m�[�h�����݂��Ȃ�
		{
			m_output.OutputLog(kTableName, objectId, err_level::warning, err_code::NotExistSameNameSGP,
				meshcode, pointObj.GetStringValue(operatorIndex), pointObj.GetStringValue(modifyDateIndex),
				pointObj.GetStringValue(modifyProgIndex), pointObj.GetStringValue(progModifyIndex),
				name, pointObj.GetStringValue(yomiIndex));
		}
	}
}

void CCheckWalk::CheckWalkRel(const IGeometryPtr& ipMeshGeo, const map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
	const FeaturesPack& walkNode, const NODEIDMAP& walkNodeNodeIDs, const NODEIDMAP& walkNodeLinkIDs)
{
	using namespace walk_node;
	using namespace ipc_feature;

	m_output.OutputStdErr(err_level::info, err_code::StrChecking, _T("NETWORK(BASE MESH)"));
	long nodeClassIndex = walkNode.m_FieldMap.FindField(kNodeClass);
	long operatorIndex = walkNode.m_FieldMap.FindField(kOperator);
	long modifyDateIndex = walkNode.m_FieldMap.FindField(kModifyDate);
	long modifyProgIndex = walkNode.m_FieldMap.FindField(kModifyProgName);
	long progModifyIndex = walkNode.m_FieldMap.FindField(kProgModifyDate);
	set<long> checkCompNodes;
	// �m�[�hID�Z�b�g�̃m�[�hID���Ƃ̏���
	for (const auto& walkNodeNode : walkNodeNodeIDs) {

		bool bNWConNode = false;

		long  walkNodeID = walkNodeNode.first;
		auto& walkNodeIDs = walkNodeNode.second;

		// �`�F�b�N�ς݃m�[�h�̏ꍇ�X�L�b�v
		if (!checkCompNodes.empty() && checkCompNodes.find(walkNodeID) != checkCompNodes.end())
			continue;

		auto walkNodeItr = walkNode.m_FeatureMap.find(walkNodeID);
		if (walkNodeItr == walkNode.m_FeatureMap.end())
			continue;

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if (walkNodeItr->second.IsDisjoint(ipMeshGeo))
			continue;

		// �����Ώۓs�s�n�}���b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		long cityMeshcode = util::GetExistMeshcode(walkNodeItr->second, cityMeshMap);
		if (cityMeshcode == 0)
			continue;

		long walkNodeClass = walkNodeItr->second.GetLongValue(nodeClassIndex);

		set<long> relWalkNodeIDs;
		// ���g�����H�m�[�h�ڑ��m�[�h�̏ꍇ
		if (walkNodeClass == node_class::kRoadNW)
			bNWConNode = true;
		// �_�E���R���o�[�g�����ꍇ�A ���g�����H�m�[�h�ڑ��m�[�h�������͓��H�����N�ڑ��m�[�h��������݂��Ȃ��ꍇ�G���[
		if (m_dataType == walk_type::type_class::kDownConvert && walkNodeClass == node_class::kConnectedRoad)
			bNWConNode = true;
		// �R�Â��m�[�hID�̒���NW�ڑ��m�[�h���Ȃ����T��
		if (!bNWConNode)
		{
			relWalkNodeIDs = GetRelNodeIDs(walkNodeID, walkNodeNodeIDs);
			// �R�Â��m�[�hID�̒���NW�ڑ��m�[�h���Ȃ����T��
			for (const auto& relWalkNodeID : relWalkNodeIDs) {

				auto relWalkNodeItr = walkNode.m_FeatureMap.find(relWalkNodeID);
				if (relWalkNodeItr == walkNode.m_FeatureMap.end())
					continue;

				long relWalkNodeClass = relWalkNodeItr->second.GetLongValue(nodeClassIndex);
				if (relWalkNodeClass == node_class::kRoadNW) {
					bNWConNode = true;
					break;
				}
				// �_�E���R���o�[�g�����ꍇ�A ���s��NW�̈�Q�̒��ɁA���H�m�[�h�ڑ��m�[�h�������͓��H�����N�ڑ��m�[�h��������݂��Ȃ��ꍇ�G���[
				if (m_dataType == walk_type::type_class::kDownConvert && relWalkNodeClass == node_class::kConnectedRoad) {
					bNWConNode = true;
					break;
				}
			}
		}

		// ��A�̃m�[�hID�̒���NW�ڑ��m�[�h���Ȃ�������G���[
		if (!bNWConNode) // ���G���[ ���HNW�ڑ��m�[�h�����݂��Ȃ�
		{
			// ��A�̃m�[�h�Q�̒�����`�F�b�N�Ώۃ��b�V�����̒[�_���������A���O�o��
			bool existEndNode = false;
			long minOid(0L);
			for (auto relWalkNodeId : relWalkNodeIDs)
			{
				if (walkNode.m_FeatureMap.find(relWalkNodeId) == walkNode.m_FeatureMap.end())
					continue;

				const auto& relWalkNode = walkNode.m_FeatureMap.at(relWalkNodeId);

				if (relWalkNode.IsDisjoint(ipMeshGeo))
					continue;

				long relCityMeshcode = util::GetExistMeshcode(relWalkNode, cityMeshMap);
				if (relCityMeshcode == 0)
					continue;

				// NW�Q����OID����ԏ������m�[�h���擾����
				if (minOid == 0) minOid = relWalkNodeId;

				// �[�_�m�[�h(�����N����{�̂ݐڑ�����m�[�h)�����݂���ꍇ
				if ((walkNodeLinkIDs.at(relWalkNodeId)).size() == 1)
				{
					existEndNode = true;
					// �ėp�����ꍇ�A���O�o��
					if (m_dataType == walk_type::type_class::kGeneral)
						m_output.OutputLog(kTableName, relWalkNodeId,
							err_level::error, err_code::NotConnectNW, uh::str_util::ToString(relCityMeshcode),
							relWalkNode.GetStringValue(operatorIndex), relWalkNode.GetStringValue(modifyDateIndex),
							relWalkNode.GetStringValue(modifyProgIndex), relWalkNode.GetStringValue(progModifyIndex));
					// �_�E���R���o�[�g�����ꍇ�A���O�o��
					if (m_dataType == walk_type::type_class::kDownConvert)
						m_output.OutputLog(kTableName, relWalkNodeId,
							err_level::warning, err_code::NotConnectNW, uh::str_util::ToString(cityMeshcode),
							relWalkNode.GetStringValue(operatorIndex), relWalkNode.GetStringValue(modifyDateIndex),
							relWalkNode.GetStringValue(modifyProgIndex), relWalkNode.GetStringValue(progModifyIndex), _T(""), _T(""), 0.0, 0.0, false);
				}
			}

			// �[�_�m�[�h�����݂��Ȃ��ꍇ
			if (!existEndNode)
			{
				// �ėp�����ꍇ�A�[�_�����݂��Ȃ��ꍇ�A���g�����O�o�́B
				if (m_dataType == walk_type::type_class::kGeneral)
					m_output.OutputLog(kTableName, walkNodeID,
						err_level::error, err_code::NotConnectNW, uh::str_util::ToString(cityMeshcode),
						walkNodeItr->second.GetStringValue(operatorIndex), walkNodeItr->second.GetStringValue(modifyDateIndex),
						walkNodeItr->second.GetStringValue(modifyProgIndex), walkNodeItr->second.GetStringValue(progModifyIndex));
				// �_�E���R���o�[�g�����ꍇ�A�[�_�����݂��Ȃ��ꍇ�ANW�Q����OID����ԏ������m�[�h��ӏ��̂݃��O�o��
				const auto& minNode = walkNode.m_FeatureMap.find(minOid);
				if (m_dataType == walk_type::type_class::kDownConvert)
					m_output.OutputLog(kTableName, walkNodeID,
						err_level::warning, err_code::NotConnectNW, uh::str_util::ToString(cityMeshcode),
						minNode->second.GetStringValue(operatorIndex), minNode->second.GetStringValue(modifyDateIndex),
						minNode->second.GetStringValue(modifyProgIndex), minNode->second.GetStringValue(progModifyIndex), _T(""), _T(""), 0.0, 0.0, false);
			}

		}

		// �`�F�b�N�ς݃m�[�h��ێ�
		checkCompNodes.insert(walkNodeID);
		checkCompNodes.insert(walkNodeIDs.begin(), walkNodeIDs.end());
		// �`�F�b�N���ɂƂ��Ă����֘A�m�[�hID���ێ�
		if (!relWalkNodeIDs.empty())
			checkCompNodes.insert(relWalkNodeIDs.begin(), relWalkNodeIDs.end());
	}
}

void CCheckWalk::CheckWalkNoPassageAll(const std::set<long>& walkLinkOIDSet, const MultiRowsPack& walkNoPassage)
{
	using namespace sindy::schema::walk_nopassage;

	m_output.OutputStdErr(err_level::info, err_code::StrChecking, _T("WALK_NOPASSAGE(ALL MESH)"));

	CheckLinkIDExist(walkLinkOIDSet, walkNoPassage, err_code::NotExistNoPassWL);
}

void CCheckWalk::CheckWalkOnewayAll(const std::set<long>& walkLinkOIDSet, const MultiRowsPack& walkOneway)
{
	using namespace sindy::schema::walk_oneway;

	m_output.OutputStdErr(err_level::info, err_code::StrChecking, _T("WALK_ONEWAY(ALL MESH)"));

	CheckLinkIDExist(walkLinkOIDSet, walkOneway, err_code::NotExistOnewayWL);
}

void CCheckWalk::CheckLinkIDExist(const std::set<long>& walkLinkOIDSet, const MultiRowsPack& linkID, const err_code::ECode errCode)
{
	using namespace ipc_table;
	using namespace uh::str_util;
	long operatorIndex = linkID.m_FieldMap.FindField(kOperator);
	long modifyDateIndex = linkID.m_FieldMap.FindField(kModifyDate);
	long modifyProgIndex = linkID.m_FieldMap.FindField(kModifyProgName);
	long progModifyIndex = linkID.m_FieldMap.FindField(kProgModifyDate);

	long linkId = -1;
	for (const auto& timeReg : linkID.m_Rows)
	{
		long objectId = timeReg.first;
		if (linkId == objectId)
			continue;

		if (0 == walkLinkOIDSet.count(objectId)) //���G���[ �Ή����郊���N�����݂��Ȃ�
		{
			m_output.OutputLog(walk_link::kTableName, timeReg.first, err_level::error, errCode, _T(""),
				ToString(timeReg.second->GetValue(operatorIndex)), ToString(timeReg.second->GetValue(modifyDateIndex)),
				ToString(timeReg.second->GetValue(modifyProgIndex)), ToString(timeReg.second->GetValue(progModifyIndex)),
				ToString(objectId));
		}
		linkId = timeReg.first;
	}
}

set<long> CCheckWalk::GetRelNodeIDs(long nodeID, const NODEIDMAP& walkNodeNodeIDs)
{
	set<long> relNodeIDs;
	relNodeIDs.insert(nodeID);
	while (1) {
		int oldSize = relNodeIDs.size();
		for (const auto& walkNodeNodeID : walkNodeNodeIDs) {

			bool bFind = false;
			if (relNodeIDs.find(walkNodeNodeID.first) != relNodeIDs.end())
				bFind = true;
			else {
				for (const auto& walkNodeID : walkNodeNodeID.second) {
					if (relNodeIDs.find(walkNodeID) != relNodeIDs.end()) {
						bFind = true;
						break;
					}
				}
			}
			if (bFind) {
				relNodeIDs.insert(walkNodeNodeID.first);
				relNodeIDs.insert(walkNodeNodeID.second.begin(), walkNodeNodeID.second.end());
			}
		}

		int newSize = relNodeIDs.size();
		if (oldSize == newSize)
			break;
	}

	return relNodeIDs;
}

//	�w�肳�ꂽ���b�V�����̃X�N�����u���G���A�f�[�^���擾
bool CCheckWalk::getScrambleAreaInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& scrambleArea
)
{
	return util::GetFeaturesInGeometry(ipMeshGeo, field_lists::scrambleAreaList, _T(""), scrambleArea);
}

//	�w�肳�ꂽ���b�V�����̉��f�|�C���g�f�[�^���擾
bool CCheckWalk::getPedxingInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& pedxing
)
{
	return util::GetFeaturesInGeometry(ipMeshGeo, field_lists::pedxingList, _T(""), pedxing);
}

// pedxing���[�h�p�̃`�F�b�N
err_code::ECode CCheckWalk::pedxingCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// ���H�����N�E���H�m�[�h�擾��RTree�쐬
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);

	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree, segmentAttrRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);
	util::createRTree(segmentAttr.m_FeatureMap, segmentAttrRTree);

	// ���s�҃����N�E���s�҃m�[�h�擾��RTree�쐬
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// �X�N�����u���G���A
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// ���f�|�C���g
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// �d�Ȃ胊���N�����m�[�h�擾&RTree�쐬
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// �ȉ��A�G���[�`�F�b�N
	auto& ipMeshGeo = cityMeshPair.second.first;

	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);

		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::pedxing);
	}

	return err_code::NoneError;
}

// all��O���[�h�p�̃`�F�b�N
void CCheckWalk::allCheckException()
{
	// WalkNoPassage�`�F�b�N
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkException(m_walkNoPassageMap);
	}
	// WalkOneway�`�F�b�N
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkException(m_walkOnewayMap);
	}

	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	// HEIGHT_NODE�`�F�b�N
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkException(heightNode);
	}

	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	// �X�N�����u���G���A�P�ƃ`�F�b�N
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.checkException(scrambleArea);
	}

	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}

// other��O���[�h�p�̃`�F�b�N
void CCheckWalk::otherCheckException()
{
	// WalkNoPassage�`�F�b�N
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkException(m_walkNoPassageMap);
	}
	// WalkOneway�`�F�b�N
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkException(m_walkOnewayMap);
	}

	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	// HEIGHT_NODE�`�F�b�N
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkException(heightNode);
	}

	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	// �X�N�����u���G���A�P�ƃ`�F�b�N
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.checkException(scrambleArea);
	}

	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}

// height_node��O���[�h�p�̃`�F�b�N
void CCheckWalk::heightNodeCheckException()
{
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	// HEIGHT_NODE�`�F�b�N
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkException(heightNode);
	}
}

// release��O���[�h�p�̃`�F�b�N
void CCheckWalk::releaseCheckException()
{
	// HEIGHT_NODE�`�F�b�N
	heightNodeCheckException();

	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	// �X�N�����u���G���A�P�ƃ`�F�b�N
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.checkException(scrambleArea);
	}

	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}

// pedxing��O���[�h�p�̃`�F�b�N
void CCheckWalk::pedxingCheckException()
{
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// ���f�|�C���g�P�ƃ`�F�b�N
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}
