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
	const long UNDERGROUNT_GATE_CONTENTS_CODE = 289000001; // 地下街出入口コンテンツコード

/**
 * @brief 指定のベースメッシュコードをもつベースメッシュに所属する
 * 全ての都市地図メッシュのメッシュコードを取得
 * @param baseMeshCode [in] 都市地図メッシュコードを取得するベースメッシュコード
 * @param cityMeshTable [in] 都市地図メッシュのテーブル
 */
	inline std::set<long> getCityMeshCodeOnBaseMesh(long baseMeshCode, const ITablePtr& cityMeshTable)
	{
		std::set<long> cityMeshCode;

		// 検索条件生成
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
	m_strAngleThreshold.Format(_T("%s °"), prop.GetValue(properties::angle_threshold));
	m_strBuddyDistThreshold.Format(_T("%s m"), prop.GetValue(properties::buddy_dist_threshold));
	m_strBuddyAngleThreshold.Format(_T("%s °"), prop.GetValue(properties::buddy_angle_threshold));

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

// 実行
bool CCheckWalk::execute()
{
	// 初期化
	if (!init())
		return false;
	// ヘッダ情報等出力
	OutputHeader();

	try {
		// チェック対象メッシュの読み込み
		LoadMesh();

		// 都市地図メッシュごとのチェック
		CheckByCityMesh();

		// 全メッシュ対象のチェック
		CheckByAllMesh();

		// 中縮メッシュ単位の処理
		CheckByBaseMesh();

		// 終了時間出力
		m_output.OutputLog(err_level::info, err_code::StrEnd, _T("正常終了"));
		m_output.OutputLog(err_level::info, err_code::StrEndTime, util::GetCurTime());

		return true;
	}
	catch (const std::exception& e)
	{
		m_output.OutputLog(err_level::error, err_code::StrException, e.what());
		m_output.OutputLog(err_level::info, err_code::StrEnd, _T("異常終了"));
		m_output.OutputLog(err_level::info, err_code::StrEndTime, util::GetCurTime());
		return false;
	}
}

// ヘッダ情報等出力
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

		// 対象メッシュポリゴンと周辺8メッシュ含めたポリゴン取得
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
		// 全メッシュ共通歩行者関連テーブルチェック開始
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
			// otherのチェックも実行するため、breakは書かない
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

	// 道路リンク・道路ノード取得＆RTree作成
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

	// 歩行者リンク・歩行者ノード取得＆RTree作成
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

	// 駅出入り口ポイント取得
	FeaturesPack stationGate = CreateFeaturesPack(c_station_gate);
	if (!GetStationGateInMesh(ipMeshUnionGeo, stationGate))
		return err_code::GetStationFail;

	// POIポイント（地下街出入口）取得
	FeaturesPack poiPoint = CreateFeaturesPack(c_poi_point_org);
	if (!GetPoiPointInMesh(ipMeshUnionGeo, poiPoint))
		return err_code::GetPoiPointFail;

	// 重なりリンク高さノード取得&RTree作成
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// スクランブルエリア
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// 横断ポイント
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// 以下、エラーチェック
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODEチェック
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
	// RoadLink単独チェック
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.CheckRoadLink(ipMeshGeo, roadLink, roadNode, nodeLinkIDs,
			walkLink, walkNode, walkNodeLinkIDs, walkLinkRTree, walkNodeRTree);
	}
	// RoadNode単独チェック
	CheckRoadNode(meshCode, ipMeshGeo, roadNode, walkNode, walkNodeRTree);
	// WalkLink単独チェック
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.CheckWalkLink(meshCode, ipMeshGeo, walkLink, walkNode, roadLink, roadNode,
			walkLinkRTree, linkRTree, nodeRTree, walkNodeLinkIDs, false, scrambleArea, scrambleAreaRTree);
	}
	// WalkNode単独チェック
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.CheckWalkNode(meshCode, ipMeshGeo, roadLink, roadNode, walkLink, walkNode,
			stationGate, poiPoint, scrambleArea, heightNode,
			linkRTree, nodeRTree, walkLinkRTree, walkNodeRTree, scrambleAreaRTree, heightNodeRTree,
			walkNodeLinkIDs, m_baseMeshMap, false);
	}
	// WalkNoPassageチェック
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkWalkNoPassage(ipMeshGeo, walkLink, m_walkNoPassageMap);
	}
	// WalkOnewayチェック
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkWalkOneway(ipMeshGeo, walkLink.m_FeatureMap, m_walkOnewayMap, m_walkNoPassageMap);
	}
	// StatinGatePointチェック
	CheckStationGatePoint(meshCode, ipMeshGeo, stationGate, walkNode, walkNodeRTree);

	// PoiPoinチェック
	{
		POICheckFunction poi(m_output, m_refer_id_threshold);
		poi.CheckPoiPoint(meshCode, ipMeshGeo, poiPoint, walkNode, walkNodeRTree);
	}
	// 地下街整合チェック
	{
		UndergroundCheckFunction under(m_output, m_modifyFilter);
		under.CheckUnderground(meshCode, ipMeshGeo, underground, walkLink);
	}

	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::all);
	}

	// スクランブルエリア単独チェック
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

	// 道路リンク・道路ノード取得＆RTree作成
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);

	// 歩行者リンク・歩行者ノード取得＆RTree作成
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;


	// 以下、エラーチェック
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

	// 道路リンク・道路ノード取得＆RTree作成
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);

	// 歩行者リンク・歩行者ノード取得＆RTree作成
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// 以下、エラーチェック
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

	// 道路リンク・道路ノード取得＆RTree作成
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree nodeRTree, segmentAttrRTree;
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);
	util::createRTree(segmentAttr.m_FeatureMap, segmentAttrRTree);

	// 歩行者リンク・歩行者ノード取得＆RTree作成
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkNodeRTree;
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// 重なりリンク高さノード取得&RTree作成
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// 以下、エラーチェック
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODEチェック
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

	// 道路リンク取得
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	// 道路リンク取得
	if (!util::GetFeaturesInGeometry(ipMeshUnionGeo, field_lists::roadLinkFieldList, _T(""), roadLink))
		return err_code::GetRoadFail;


	// 歩行者リンク取得
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	// 歩行者リンク取得
	if (!util::GetFeaturesInGeometry(ipMeshUnionGeo, field_lists::walkLinkFieldList, _T(""), walkLink))
		return err_code::GetWalkFail;

	// 以下、エラーチェック
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

	// 歩行者リンク取得
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	if (!util::GetFeaturesInGeometry(ipMeshUnionGeo, field_lists::walkLinkFieldList, _T(""), walkLink))
		return err_code::GetWalkFail;


	FeaturesPack underground = CreateFeaturesPack(c_underground_area);
	if (!GetUndergroundInMesh(ipMeshUnionGeo, underground))
		return err_code::GetUndergroundFail;

	// 以下、エラーチェック
	auto& ipMeshGeo = cityMeshPair.second.first;

	// 地下街整合チェック
	UndergroundCheckFunction under(m_output, m_modifyFilter);
	under.CheckUnderground(meshCode, ipMeshGeo, underground, walkLink);
	return err_code::NoneError;
}

err_code::ECode CCheckWalk::otherCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// 道路リンク・道路ノード取得＆RTree作成
	FeaturesPack roadLink = CreateFeaturesPack(c_road_link);
	FeaturesPack roadNode = CreateFeaturesPack(c_road_node);
	FeaturesPack segmentAttr = CreateFeaturesPack(c_segment_attr);
	NODEIDMAP nodeLinkIDs;
	if (!GetRoadInMesh(ipMeshUnionGeo, roadLink, roadNode, segmentAttr, nodeLinkIDs))
		return err_code::GetRoadFail;

	BGRTree linkRTree, nodeRTree;
	util::createRTree(roadLink.m_FeatureMap, linkRTree);
	util::createRTree(roadNode.m_FeatureMap, nodeRTree);

	// 歩行者リンク・歩行者ノード取得＆RTree作成
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

	// 駅出入り口ポイント取得
	FeaturesPack stationGate = CreateFeaturesPack(c_station_gate);
	if (!GetStationGateInMesh(ipMeshUnionGeo, stationGate))
		return err_code::GetStationFail;

	// POIポイント（地下街出入口）取得
	FeaturesPack poiPoint = CreateFeaturesPack(c_poi_point_org);
	if (!GetPoiPointInMesh(ipMeshUnionGeo, poiPoint))
		return err_code::GetPoiPointFail;

	// 重なりリンク高さノード取得&RTree作成
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// スクランブルエリア
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// 横断ポイント
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// 以下、エラーチェック
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODEチェック
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkOther(ipMeshGeo, heightNode);
	}
	// RoadLink単独チェック
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.checkOther(ipMeshGeo, roadLink, nodeLinkIDs);
	}
	// RoadNode単独チェック
	CheckRoadNode(meshCode, ipMeshGeo, roadNode, walkNode, walkNodeRTree);
	// WalkLink単独チェック
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.CheckWalkLink(meshCode, ipMeshGeo, walkLink, walkNode, roadLink, roadNode,
			walkLinkRTree, linkRTree, nodeRTree, walkNodeLinkIDs, true, scrambleArea, scrambleAreaRTree);
	}
	// WalkNode単独チェック
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.CheckWalkNode(meshCode, ipMeshGeo, roadLink, roadNode, walkLink, walkNode,
			stationGate, poiPoint, scrambleArea, heightNode,
			linkRTree, nodeRTree, walkLinkRTree, walkNodeRTree, scrambleAreaRTree, heightNodeRTree,
			walkNodeLinkIDs, m_baseMeshMap, true);
	}
	// WalkNoPassageチェック
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkWalkNoPassage(ipMeshGeo, walkLink, m_walkNoPassageMap);
	}
	// WalkOnewayチェック
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkWalkOneway(ipMeshGeo, walkLink.m_FeatureMap, m_walkOnewayMap, m_walkNoPassageMap);
	}
	// StatinGatePointチェック
	CheckStationGatePoint(meshCode, ipMeshGeo, stationGate, walkNode, walkNodeRTree);
	// PoiPoinチェック
	{
		POICheckFunction poi(m_output, m_refer_id_threshold);
		poi.CheckPoiPoint(meshCode, ipMeshGeo, poiPoint, walkNode, walkNodeRTree);
	}
	// 地下街整合チェック
	{
		UndergroundCheckFunction under(m_output, m_modifyFilter);
		under.CheckUnderground(meshCode, ipMeshGeo, underground, walkLink);
	}

	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::other);
	}

	// スクランブルエリア単独チェック
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

	// 道路リンク・道路ノード取得＆RTree作成
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

	// 歩行者リンク・歩行者ノード取得＆RTree作成
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// 駅出入り口ポイント取得
	FeaturesPack stationGate = CreateFeaturesPack(c_station_gate);
	if (!GetStationGateInMesh(ipMeshUnionGeo, stationGate))
		return err_code::GetStationFail;

	// POIポイント（地下街出入口）取得
	FeaturesPack poiPoint = CreateFeaturesPack(c_poi_point_org);
	if (!GetPoiPointInMesh(ipMeshUnionGeo, poiPoint))
		return err_code::GetPoiPointFail;

	// 重なりリンク高さノード取得&RTree作成
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// 横断ポイント
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// スクランブルエリア
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// 以下、エラーチェック
	auto& ipMeshGeo = cityMeshPair.second.first;

	// HEIGHT_NODEチェック
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

	// RoadLink単独チェック
	{
		RoadLinkCheckFunction roadLinkCheck(m_output, m_modifyFilter, m_walkableMap, m_dataType);
		roadLinkCheck.checkRelease(ipMeshGeo, roadLink);
	}
	// WalkLink単独チェック
	{
		WalkLinkCheckFunction linkCheck(
			m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkCodeMap, m_walkNoPassageMap, m_walkOnewayMap
			, m_dist_threshold, m_angle_threshold, m_coordinateTolerance, m_dataType, m_microLogicalConnection);

		linkCheck.checkRelease(meshCode, ipMeshGeo, walkLink, walkNode, roadLink, roadNode,
			walkLinkRTree, linkRTree, nodeRTree, walkNodeLinkIDs);
	}
	// WalkNode単独チェック
	{
		WalkNodeCheckFunction nodeCheck(m_output, m_modifyFilter
			, m_walkLinkClassMap, m_walkNodeClassMap
			, m_dist_threshold, m_neighbor_threshold, m_refer_id_threshold, m_dataType, m_coordinateTolerance);

		nodeCheck.checkRelease(meshCode, ipMeshGeo, roadLink, roadNode, walkLink, walkNode,
			stationGate, poiPoint, heightNode,
			linkRTree, nodeRTree, walkLinkRTree, walkNodeRTree, heightNodeRTree,
			walkNodeLinkIDs, m_baseMeshMap);
	}
	// WalkNoPassageチェック
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkRelease(ipMeshGeo, walkLink, m_walkNoPassageMap);
	}
	// WalkOnewayチェック
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkRelease(ipMeshGeo, walkLink, m_walkOnewayMap);
	}

	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::release);
	}

	// スクランブルエリア単独チェック
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

		// TODO: baseMeshMapの形状を使用するように修正
		// 対象メッシュポリゴンと周辺8メッシュ含めたポリゴン取得
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
			// 歩行者リンク・歩行者ノード取得
			if (!GetWalkInMesh(ipUnionMeshGeo, walkLink, walkNode, walkNodeNodeIDs, walkNodeLinkIDs)) {
				m_output.OutputStdErr(err_level::fatal, err_code::GetWalkFail, strBaseMesh);
				m_output.OutputLog(err_level::fatal, err_code::GetWalkFail, strBaseMesh);
				continue;
			}
			// ネットワークチェック
			CheckWalkRel(ipBaseMeshGeo, m_cityMeshMap, walkNode, walkNodeNodeIDs, walkNodeLinkIDs);
			// WalkLink単独チェック
			{
				WalkLinkOfBaseMeshCheckFunction walkCheck(m_output, m_modifyFilter, m_onewayClassMap, m_walkNoPassageMap);
				walkCheck.checkWalkLinkOfBaseMesh(ipBaseMeshGeo, ipBoundaryMesh, m_cityMeshMap,
					walkLink, walkNode, walkNodeLinkIDs);
			}
		}
		break;
		case properties::check_target::release:
			// WalkLink単独チェック
		{
			FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
			FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
			NODEIDMAP walkNodeLinkIDs;
			// 歩行者リンク・歩行者ノード取得
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


// 初期化
bool CCheckWalk::init()
{
	// オプションdata_typeに異常値(1、2以外の値)が指定される際には、オプションエラーのメッセージを出力する
	if (m_dataType != walk_type::type_class::kGeneral && m_dataType != walk_type::type_class::kDownConvert)
	{
		m_output.OutputStdErr(err_level::fatal, err_code::DataTypeError, m_strOutput);
		return false;
	}

	// オプションdata_typeが正常値1(汎用版のデータ)を指定し、設定ファイルの[target]オプションがpedxingを指定し、エラーメッセージを出力する。
	if (m_checkTarget == properties::check_target::pedxing && m_dataType == walk_type::type_class::kGeneral)
	{
		m_output.OutputStdErr(err_level::fatal, err_code::DataTypeTargetError, m_strOutput);
		return false;
	}

	// ログファイルクラスオープン
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

	// データベース中のWALKTYPE_Cと指定オプション値が合っていないかのチェック
	if (getWalkTypeValue() != m_dataType)
	{
		// データベース中のWALKTYPE_Cと指定オプション値が合っていない際には、エラーのメッセージを出力して、異常終了
		m_output.OutputStdErr(err_level::fatal, err_code::walkTypeError, m_strOutput);
		return false;
	}

	// メッシュリスト読み込み
	ITablePtr cityMeshTable;
	m_database.GetTable(table::c_city_mesh, cityMeshTable);
	if (!loadMeshList(m_strMeshList, m_cityMeshList, cityMeshTable)) {
		m_output.OutputStdErr(err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList);
		m_output.OutputLog(err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList);
		return false;
	}

	// 必要なコード値ドメインmapを取得しておく
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

	// 重なりリンク高さ 設定ファイル
	if (!m_heightNodeMgr.read(m_height_node_ini))
		return false;

	// 歩行者関連テーブルを全て取得しておく
	if (!GetWalkRelationTables()) {
		m_output.OutputStdErr(err_level::fatal, err_code::GetWalkRelFail, _T(""));
		m_output.OutputLog(err_level::fatal, err_code::GetWalkRelFail, _T(""));
		return false;
	}

	// 歩行者ノードの座標許容値取得
	// 道路リンクの辺上に人車共用リンクの端点が乗っているかどうかを判定するときに使用する
	ITablePtr walkNodeTable;
	m_database.GetTable(table::c_walk_node, walkNodeTable);
	m_coordinateTolerance = AheGetXYDomain1UnitLength(AheGetSpatialReference(walkNodeTable));
	return true;
}

// 歩行者タイプテーブルからWALK_TYPEの値を取得する。
long CCheckWalk::getWalkTypeValue()
{
	// 歩行者タイプテーブル読み込み
	ITablePtr ipWalkTypeTable;
	m_database.GetTable(table::c_walk_type, ipWalkTypeTable);
	if (!ipWalkTypeTable)
		return walk_type::type_class::kGeneral;

	// WALK_TYPEのインデックスを取得する
	long indexWalkType(-1L);
	ipWalkTypeTable->FindField(CComBSTR(walk_type::kWalkTypeClass), &indexWalkType);

	// WALK_TYPEの値を取得す
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

	// ストリームを取得できなければfalse
	if (!ifs) return false;

	bool bSuccess = true;
	std::string line;
	for (int lineNum = 0; !ifs.eof() && std::getline(ifs, line); ++lineNum)
	{
		// 数字でないものは無視
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
			// 都市地図メッシュ
			integerList.insert(std::stol(line));
			break;
		default: // 桁数が合わないものはエラー
			m_output.OutputStdErr(err_level::fatal, err_code::DigitErrInMeshList, m_strOutput);
			bSuccess = false;
			continue;
			break;
		}
	}
	return bSuccess;
}

// 歩行者関連テーブルから全件取得しておく
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


// 与えたメッシュ隣接9メッシュリストを作成する
void CCheckWalk::Get9Mesh(long mesh, std::set<long>& meshlist)
{
	meshlist.insert(mesh);                              // 中心
	meshlist.insert(g_cnv.GetMeshCode(mesh, -1, 1)); // 左上
	meshlist.insert(g_cnv.GetMeshCode(mesh, 0, 1)); // 上
	meshlist.insert(g_cnv.GetMeshCode(mesh, 1, 1)); // 右上
	meshlist.insert(g_cnv.GetMeshCode(mesh, -1, 0)); // 左
	meshlist.insert(g_cnv.GetMeshCode(mesh, 1, 0)); // 右
	meshlist.insert(g_cnv.GetMeshCode(mesh, -1, -1)); // 左下
	meshlist.insert(g_cnv.GetMeshCode(mesh, 0, -1)); // 下
	meshlist.insert(g_cnv.GetMeshCode(mesh, 1, -1)); // 右下
}

// 指定されたメッシュコードのメッシュポリゴン、及び周辺9メッシュのメッシュポリゴンを取得
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

		// 空間参照も取得しておく
		if (i == 0) {
			ipTempMeshGeo->get_SpatialReference(&ipSpRef);
			((IGeometryPtr)ipGeoCol)->putref_SpatialReference(ipSpRef);
		}

		// 自身のメッシュを取得しておく
		if (mesh == (ipMeshFeature->GetValue(meshFM.FindField(category::mesh_code_table::kMeshCode))).lVal)
			ipMeshGeo = ipTempMeshGeo;

		// 後で一括でConstructUnionするために、ジオメトリを格納する
		ipGeoCol->AddGeometry(ipTempMeshGeo);

		++i;
	}

	// 詰めたものをConstructUnion
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
		// 属性を取得し、mapに格納する
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
		// 属性を取得し、mapに格納する
		long keyValue = (ipRow->GetValue(keyIndex)).lVal;
		dataMap.m_Rows.emplace(keyValue, ipRow);
	}

	return true;
}

// 指定テーブルで初期化するイメージ
FeaturesPack CCheckWalk::CreateFeaturesPack(table tableValue)
{
	ITablePtr ipTable;
	m_database.GetTable(tableValue, ipTable);
	CFieldMap fieldMap;
	m_database.GetFieldMap(tableValue, fieldMap);
	return FeaturesPack(ipTable, fieldMap);
}

// 与えられたメッシュ形状内の道路リンクと道路ノードを取得
bool CCheckWalk::GetRoadInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& roadLink,
	FeaturesPack& roadNode,
	FeaturesPack& segmentAttr,
	NODEIDMAP& nodeLinkIDs)
{
	// 道路リンク取得
	if (!util::GetLinkFeaturesInGeometry(ipMeshGeo, field_lists::roadLinkFieldList,
		_T(""), roadLink, nodeLinkIDs))
		return false;

	// 道路ノード取得
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::roadNodeFieldList,
		_T(""), roadNode))
		return false;

	// リンク内属性取得
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::segmentAttrFieldList,
		_T(""), segmentAttr))
		return false;

	return true;
}

// 与えられたメッシュ形状内の歩行者リンクと歩行者ノードを取得
bool CCheckWalk::GetWalkInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeNodeIDs,
	NODEIDMAP& walkNodeLinkIDs)
{
	// 歩行者リンク取得
	if (!util::GetLinkFeaturesWithNodeIdInGeometry(ipMeshGeo, field_lists::walkLinkFieldList,
		_T(""), walkLink, walkNodeNodeIDs, walkNodeLinkIDs))
		return false;

	// 歩行者ノード取得
	if (!util::GetFeaturesInGeometry(ipMeshGeo, field_lists::walkNodeFieldList,
		_T(""), walkNode))
		return false;

	return true;
}

// 与えられたメッシュ形状内の歩行者リンクと歩行者ノードを取得
bool CCheckWalk::GetWalkInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& walkLink,
	FeaturesPack& walkNode,
	NODEIDMAP& walkNodeLinkIDs)
{
	// 歩行者リンク取得
	if (!util::GetLinkFeaturesInGeometry(ipMeshGeo, field_lists::walkLinkFieldList,
		_T(""), walkLink, walkNodeLinkIDs))
		return false;

	// 歩行者ノード取得
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

// チェック
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

	// 道路ノードのチェック
	for (const auto& node : roadNode.m_FeatureMap)
	{
		long  objectId = node.first;
		auto& nodeObj = node.second;

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (nodeObj.IsDisjoint(ipMeshGeo))
			continue;

		auto nameKanji = nodeObj.GetStringValue(nameKanjiIndex);
		// 自身に名称がない場合スキップ
		if (nameKanji.IsEmpty())
			continue;

		if (!util::existAroundWalkNodeSameName(nodeObj, roadNode, walkNode, walkNodeRTree,
			kNameKanji, kNameYomi, walk_node::kRoadNodeID, m_refer_id_threshold))
			// ★ワーニング 付近に同一の交差点名をもつ歩行者ノードが存在しない
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

	// 重なりリンク高さテーブルでループし、基準リンクのリンクIDが存在するかチェック
	// →リンクリレーションテーブルでループし、関連フラグONの歩行者リンクリストにIDが存在するかチェックになってるけどOK?
	//   重複チェックも追加[bug 12473]
	std::set<std::pair<long, long>> relMap;
	for (const auto& linkRel : linkRelation.m_Rows)
	{
		auto ite = linkRelOIDSet.find(linkRel.first);
		if (ite == linkRelOIDSet.end()) // ★エラー 基準リンクIDが歩行者リンクに存在しない
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

	// 駅出入口ポイントのチェック
	for (const auto& staionGatePoint : stationGate.m_FeatureMap)
	{
		long  objectId = staionGatePoint.first;
		auto& pointObj = staionGatePoint.second;

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (pointObj.IsDisjoint(ipMeshGeo))
			continue;

		auto name = pointObj.GetStringValue(nameIndex);

		// 自身に名称がない場合スキップ
		if (name.IsEmpty())
			continue;

		if (!util::existAroundWalkNodeSameName(pointObj, stationGate, walkNode, walkNodeRTree,
			kName, kYomi, walk_node::kStationGatePointID, m_refer_id_threshold, walk_node::node_class::kGate))
			// ★ワーニング 付近に同一の駅出入口名をもつ歩行者ノードが存在しない
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
	// ノードIDセットのノードIDごとの処理
	for (const auto& walkNodeNode : walkNodeNodeIDs) {

		bool bNWConNode = false;

		long  walkNodeID = walkNodeNode.first;
		auto& walkNodeIDs = walkNodeNode.second;

		// チェック済みノードの場合スキップ
		if (!checkCompNodes.empty() && checkCompNodes.find(walkNodeID) != checkCompNodes.end())
			continue;

		auto walkNodeItr = walkNode.m_FeatureMap.find(walkNodeID);
		if (walkNodeItr == walkNode.m_FeatureMap.end())
			continue;

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (walkNodeItr->second.IsDisjoint(ipMeshGeo))
			continue;

		// 処理対象都市地図メッシュ内（枠上含む）じゃないものはスキップ
		long cityMeshcode = util::GetExistMeshcode(walkNodeItr->second, cityMeshMap);
		if (cityMeshcode == 0)
			continue;

		long walkNodeClass = walkNodeItr->second.GetLongValue(nodeClassIndex);

		set<long> relWalkNodeIDs;
		// 自身が道路ノード接続ノードの場合
		if (walkNodeClass == node_class::kRoadNW)
			bNWConNode = true;
		// ダウンコンバート向け場合、 自身が道路ノード接続ノードもしくは道路リンク接続ノードが一つも存在しない場合エラー
		if (m_dataType == walk_type::type_class::kDownConvert && walkNodeClass == node_class::kConnectedRoad)
			bNWConNode = true;
		// 紐づくノードIDの中にNW接続ノードがないか探す
		if (!bNWConNode)
		{
			relWalkNodeIDs = GetRelNodeIDs(walkNodeID, walkNodeNodeIDs);
			// 紐づくノードIDの中にNW接続ノードがないか探す
			for (const auto& relWalkNodeID : relWalkNodeIDs) {

				auto relWalkNodeItr = walkNode.m_FeatureMap.find(relWalkNodeID);
				if (relWalkNodeItr == walkNode.m_FeatureMap.end())
					continue;

				long relWalkNodeClass = relWalkNodeItr->second.GetLongValue(nodeClassIndex);
				if (relWalkNodeClass == node_class::kRoadNW) {
					bNWConNode = true;
					break;
				}
				// ダウンコンバート向け場合、 歩行者NWの一群の中に、道路ノード接続ノードもしくは道路リンク接続ノードが一つも存在しない場合エラー
				if (m_dataType == walk_type::type_class::kDownConvert && relWalkNodeClass == node_class::kConnectedRoad) {
					bNWConNode = true;
					break;
				}
			}
		}

		// 一連のノードIDの中にNW接続ノードがなかったらエラー
		if (!bNWConNode) // ★エラー 道路NW接続ノードが存在しない
		{
			// 一連のノード群の中からチェック対象メッシュ内の端点を検索し、ログ出力
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

				// NW群内でOIDが一番小さいノードを取得する
				if (minOid == 0) minOid = relWalkNodeId;

				// 端点ノード(リンクが一本のみ接続するノード)が存在する場合
				if ((walkNodeLinkIDs.at(relWalkNodeId)).size() == 1)
				{
					existEndNode = true;
					// 汎用向け場合、ログ出力
					if (m_dataType == walk_type::type_class::kGeneral)
						m_output.OutputLog(kTableName, relWalkNodeId,
							err_level::error, err_code::NotConnectNW, uh::str_util::ToString(relCityMeshcode),
							relWalkNode.GetStringValue(operatorIndex), relWalkNode.GetStringValue(modifyDateIndex),
							relWalkNode.GetStringValue(modifyProgIndex), relWalkNode.GetStringValue(progModifyIndex));
					// ダウンコンバート向け場合、ログ出力
					if (m_dataType == walk_type::type_class::kDownConvert)
						m_output.OutputLog(kTableName, relWalkNodeId,
							err_level::warning, err_code::NotConnectNW, uh::str_util::ToString(cityMeshcode),
							relWalkNode.GetStringValue(operatorIndex), relWalkNode.GetStringValue(modifyDateIndex),
							relWalkNode.GetStringValue(modifyProgIndex), relWalkNode.GetStringValue(progModifyIndex), _T(""), _T(""), 0.0, 0.0, false);
				}
			}

			// 端点ノードが存在しない場合
			if (!existEndNode)
			{
				// 汎用向け場合、端点が存在しない場合、自身をログ出力。
				if (m_dataType == walk_type::type_class::kGeneral)
					m_output.OutputLog(kTableName, walkNodeID,
						err_level::error, err_code::NotConnectNW, uh::str_util::ToString(cityMeshcode),
						walkNodeItr->second.GetStringValue(operatorIndex), walkNodeItr->second.GetStringValue(modifyDateIndex),
						walkNodeItr->second.GetStringValue(modifyProgIndex), walkNodeItr->second.GetStringValue(progModifyIndex));
				// ダウンコンバート向け場合、端点が存在しない場合、NW群内でOIDが一番小さいノード一箇所のみログ出力
				const auto& minNode = walkNode.m_FeatureMap.find(minOid);
				if (m_dataType == walk_type::type_class::kDownConvert)
					m_output.OutputLog(kTableName, walkNodeID,
						err_level::warning, err_code::NotConnectNW, uh::str_util::ToString(cityMeshcode),
						minNode->second.GetStringValue(operatorIndex), minNode->second.GetStringValue(modifyDateIndex),
						minNode->second.GetStringValue(modifyProgIndex), minNode->second.GetStringValue(progModifyIndex), _T(""), _T(""), 0.0, 0.0, false);
			}

		}

		// チェック済みノードを保持
		checkCompNodes.insert(walkNodeID);
		checkCompNodes.insert(walkNodeIDs.begin(), walkNodeIDs.end());
		// チェック時にとってきた関連ノードIDも保持
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

		if (0 == walkLinkOIDSet.count(objectId)) //★エラー 対応するリンクが存在しない
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

//	指定されたメッシュ内のスクランブルエリアデータを取得
bool CCheckWalk::getScrambleAreaInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& scrambleArea
)
{
	return util::GetFeaturesInGeometry(ipMeshGeo, field_lists::scrambleAreaList, _T(""), scrambleArea);
}

//	指定されたメッシュ内の横断ポイントデータを取得
bool CCheckWalk::getPedxingInMesh(
	const IGeometryPtr& ipMeshGeo,
	FeaturesPack& pedxing
)
{
	return util::GetFeaturesInGeometry(ipMeshGeo, field_lists::pedxingList, _T(""), pedxing);
}

// pedxingモード用のチェック
err_code::ECode CCheckWalk::pedxingCheck(
	const CityMeshPair& cityMeshPair)
{
	const auto& meshCode = uh::str_util::ToString(cityMeshPair.first);
	const auto& ipMeshUnionGeo = cityMeshPair.second.second;

	// 道路リンク・道路ノード取得＆RTree作成
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

	// 歩行者リンク・歩行者ノード取得＆RTree作成
	FeaturesPack walkLink = CreateFeaturesPack(c_walk_link);
	FeaturesPack walkNode = CreateFeaturesPack(c_walk_node);
	NODEIDMAP walkNodeLinkIDs;
	if (!GetWalkInMesh(ipMeshUnionGeo, walkLink, walkNode, walkNodeLinkIDs))
		return err_code::GetWalkFail;

	BGRTree walkLinkRTree, walkNodeRTree;
	util::createRTree(walkLink.m_FeatureMap, walkLinkRTree);
	util::createRTree(walkNode.m_FeatureMap, walkNodeRTree);

	// スクランブルエリア
	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	if (m_dataType == walk_type::type_class::kDownConvert && !getScrambleAreaInMesh(cityMeshPair.second.first, scrambleArea))
		return err_code::GetScrambleAreaFail;

	BGRTree scrambleAreaRTree;
	util::createRTree(scrambleArea.m_FeatureMap, scrambleAreaRTree);

	// 横断ポイント
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	if (m_dataType == walk_type::type_class::kDownConvert && !getPedxingInMesh(cityMeshPair.second.first, pedxing))
		return err_code::GetPedxingFail;

	BGRTree pedxingRTree;
	util::createRTree(pedxing.m_FeatureMap, pedxingRTree);

	// 重なりリンク高さノード取得&RTree作成
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	if (!GetHeightNodeInMesh(cityMeshPair.second.first, heightNode))
		return err_code::GetWalkRelFail;

	BGRTree heightNodeRTree;
	util::createRTree(heightNode.m_FeatureMap, heightNodeRTree);

	// 以下、エラーチェック
	auto& ipMeshGeo = cityMeshPair.second.first;

	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);

		pedxingCheck.check(meshCode, ipMeshGeo, pedxing, walkLink, walkNode, roadLink, roadNode, scrambleArea, heightNode,
			pedxingRTree, walkLinkRTree, walkNodeRTree, linkRTree, nodeRTree, scrambleAreaRTree, heightNodeRTree, properties::check_target::pedxing);
	}

	return err_code::NoneError;
}

// all例外モード用のチェック
void CCheckWalk::allCheckException()
{
	// WalkNoPassageチェック
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkException(m_walkNoPassageMap);
	}
	// WalkOnewayチェック
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkException(m_walkOnewayMap);
	}

	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	// HEIGHT_NODEチェック
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkException(heightNode);
	}

	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	// スクランブルエリア単独チェック
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.checkException(scrambleArea);
	}

	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}

// other例外モード用のチェック
void CCheckWalk::otherCheckException()
{
	// WalkNoPassageチェック
	{
		WalkNoPassageCheckFunction noPassage(m_output, m_modifyFilter, m_dataType);
		noPassage.checkException(m_walkNoPassageMap);
	}
	// WalkOnewayチェック
	{
		WalkOnewayCheckFunction onewayCheck(m_output, m_modifyFilter, m_dataType);
		onewayCheck.checkException(m_walkOnewayMap);
	}

	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	// HEIGHT_NODEチェック
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkException(heightNode);
	}

	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	// スクランブルエリア単独チェック
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.checkException(scrambleArea);
	}

	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}

// height_node例外モード用のチェック
void CCheckWalk::heightNodeCheckException()
{
	FeaturesPack heightNode = CreateFeaturesPack(c_height_node);
	// HEIGHT_NODEチェック
	{
		HeightNodeCheckFunction heightCheck(m_output, m_dist_threshold, m_coordinateTolerance, m_heightNodeMgr, m_dataType);
		heightCheck.checkException(heightNode);
	}
}

// release例外モード用のチェック
void CCheckWalk::releaseCheckException()
{
	// HEIGHT_NODEチェック
	heightNodeCheckException();

	FeaturesPack scrambleArea = CreateFeaturesPack(c_scramble_area);
	// スクランブルエリア単独チェック
	{
		ScrambleAreaCheckFunction ScrambleAreaCheck(m_output, m_modifyFilter, m_dataType);
		ScrambleAreaCheck.checkException(scrambleArea);
	}

	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}

// pedxing例外モード用のチェック
void CCheckWalk::pedxingCheckException()
{
	FeaturesPack pedxing = CreateFeaturesPack(c_pedxing);
	// 横断ポイント単独チェック
	{
		PedxingCheckFunction pedxingCheck(m_output, m_modifyFilter, m_dataType, m_sindyTolerance_threshold);
		pedxingCheck.checkException(pedxing);
	}
}
