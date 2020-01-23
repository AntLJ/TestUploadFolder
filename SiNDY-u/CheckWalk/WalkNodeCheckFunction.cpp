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

	// TODO: リファクタリングで前提条件を見直す
	// 該当のコードをもつベースメッシュが存在しない場合はエラー吐くべきかも知れないが
	// そんなものは想定していないのでサポートしない。
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

	// 歩行者ノードのチェック
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

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (walkNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long  walkNodeID = walkNodePair.first;
		long  walkNodeClass = walkNodeObj.GetLongValue(walkNodeClassIndex);
		long  roadNodeID = walkNodeObj.GetLongValue(roadNodeIDIndex);

		m_output.SetModifyInfo(walkNodeID, walkNodeObj.GetStringValue(modOperatorIndex),
			walkNodeObj.GetStringValue(modDateIndex), walkNodeObj.GetStringValue(modProgramIndex),
			walkNodeObj.GetStringValue(modProgramDateIndex));
		// 各属性値が定義内か確認する
		if (!judge_value::IsDefineWalkNodeClass(walkNodeClass, m_dataType)) // ★エラー 歩行者ノード種別コードが定義外
		{
			// 汎用向け場合
			if (m_dataType == walk_type::type_class::kGeneral)
				m_output.OutputRegLog(err_level::error, err_code::NgWalkNodeClass, uh::str_util::ToString(walkNodeClass));
			// ダウンコンバート向け場合
			if (m_dataType == walk_type::type_class::kDownConvert)
				m_output.OutputRegLog(err_level::fatal, err_code::NgWalkNodeClass);
		}


		// ★ここから形状も使ったチェック

		// 単純な形状確認
		IPointPtr ipWalkNodePoint(walkNodeObj.m_ipGeometry);
		double walkNodeX = 0.0, walkNodeY = 0.0;

		// TODO　形状存在しない場合に、今のソースロジックよりこのエラーチェック処理に入れないので、エラーコード102エラーメッセージがエラーログファイルに出せない。
		// ★エラー 形状が存在しない
		checkNodeGeometry(walkNodeObj, walkNodeX, walkNodeY);
		// ★エラー 図郭の隅にノードがある
		checkBorderCorner(walkNodeX, walkNodeY);

		// ★エラー 2次メッシュ境界線上にあるのに図郭上ノードになっていない
		// ★エラー 図郭上ノードが2次メッシュ境界線上に無い
		checkBorderLine(walkNodeObj, walkNodeClass, basePolyline);

		// ★エラー 道路ノード上にあるのに道路NW接続ノードになっていない
		checkRoadNWConnectNode(walkNodeObj, walkNodeClass, roadNode, nodeRTree);

		// 接続リンクに関わるチェックを実施
		checkConenctLink(walkNodeObj, walkNode, walkLink, walkNodeClass, roadNodeID, walkClassIndex,
			walkNodeClassIndex, fromIdIndex, toIdIndex, walkNodeLinkIDs, roadLink, roadNode, linkRTree, nodeRTree, checkType);

		// ★エラー 歩行者リンクに接続していない（形状）
		checkWalkLinkConnectNode(walkNodeObj, walkLink, walkLinkRTree);

		// ★エラー ノードが関係ないリンクに接触している
		checkTouchOtherLink(walkNodeObj, walkLink, walkLinkRTree, walkNodeClass, walkNodeLinkIDs);

		// ★エラー ノードが別のノードに接触している
		checkTouchOtherNode(walkNodeObj, walkNode, walkNodeRTree, walkNodeX, walkNodeY);

		// 道路ノードIDのチェック
		if (roadNodeID != -1 && roadNodeID != 0) {
			compairRoadNodeCheck(walkNodeClass, roadNodeID, nodeNameIndex, walkNodeRoadIndex,
				walkNodeObj, roadNode, walkNode, walkNodeRTree, false);
		}

		// 駅出入口ポイントIDのチェック
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kStationGatePointID, stationGate, false);
		// 地下街出入口ポイントIDのチェック
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kUndergroundGatePointID, poiPoint, false);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ★エラー 道路リンク上にあるのに道路リンク接続ノードになっていない
		checkRoadLinkConnectNode(walkNodeObj, walkNodeClass, roadLink, linkRTree);

		// ★エラー 道路リンク接続ノードが道路リンク交差箇所にある
		checkRoadLinkCrossNode(walkNodeObj, walkNodeClass, roadLink, linkRTree, roadNode.m_FeatureMap, nodeRTree);

		// ★エラー スクランブル交差点なのにスクランブルエリアポリゴンが無い
		checkWalkNodeCrossWalkLink(walkNodeObj, walkLink, walkLinkRTree, scrambleArea, scrambleAreaRTree);

		if(checkType == check_target::all && walkNodeClass == node_class::kConnectedRoad)
		{
			// ★エラー ポイント同士（WALK_NODE、HEIGHT_NODE）の距離が近い
			checkNearPoint( walkNodeObj, walkNode, heightNode, walkNodeRTree, heightNodeRTree );
		}
		
		// ★エラー ロジカルコネクションが複数接続している
		if (walkNodeClass != node_class::kConnectedRoad && walkNodeClass != node_class::kRoadNW)
			checkConnectWalkLink(walkNodeObj, walkLink, walkLinkRTree);

		// ロジカルコネクションが鋭角に接続しているかチェック
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
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (walkNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		// チェック対象は道路NW接続ノードのみ
		if (walkNodeClass != walk_node::node_class::kRoadNW)
			continue;
		// ダウンコンバート向け場合、道路ノード接続ノードもしくは道路リンク接続ノードを対象とする。
		if (m_dataType == walk_type::type_class::kDownConvert && walkNodeClass != walk_node::node_class::kConnectedRoad && walkNodeClass != walk_node::node_class::kRoadNW)	continue;

		// 単純な形状確認
		double walkNodeX = 0.0, walkNodeY = 0.0;
		if (isIllegalShape(walkNodeObj.m_ipGeometry, walkNodeX, walkNodeY))
			continue;

		// 接続リンク数を取得
		long connectWalkLinkCnt = -1;
		auto walkNodeLinkItr = walkNodeLinkIDs.find(walkNodeID);
		if (walkNodeLinkItr != walkNodeLinkIDs.end())
			connectWalkLinkCnt = walkNodeLinkItr->second.size();

		if (connectWalkLinkCnt < 0) continue;

		// 歩行者ノード上の道路ノードを検索
		GeoClass roadNodeObj;
		// 検索できないなら本チェックでは対象外
		if (!findOverlapRoadNode(roadNodeObj, walkNodeObj, roadNode.m_FeatureMap, nodeRTree))
			continue;

		if (checkWalkableConnect(roadNodeObj, connectWalkLinkCnt, roadNode.m_FieldMap, roadLink, linkRTree))
		{
			// ★ワーニング 接続する道路リンクが歩行禁止のリンクのみ
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

	// TODO: リファクタリングで前提条件を見直す
	// 該当のコードをもつベースメッシュが存在しない場合はエラー吐くべきかも知れないが
	// そんなものは想定していないのでサポートしない。
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

	// 歩行者ノードのチェック
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
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (walkNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long  walkNodeID = walkNodePair.first;
		long  walkNodeClass = walkNodeObj.GetLongValue(walkNodeClassIndex);
		long  roadNodeID = walkNodeObj.GetLongValue(roadNodeIDIndex);

		m_output.SetModifyInfo(walkNodeID, walkNodeObj.GetStringValue(modOperatorIndex),
			walkNodeObj.GetStringValue(modDateIndex), walkNodeObj.GetStringValue(modProgramIndex),
			walkNodeObj.GetStringValue(modProgramDateIndex));

		double walkNodeX = 0.0, walkNodeY = 0.0;

		// ★エラー 形状が存在しない
		checkNodeGeometry(walkNodeObj, walkNodeX, walkNodeY);
		// ★エラー 図郭の隅にノードがある
		checkBorderCorner(walkNodeX, walkNodeY);

		// ★エラー 2次メッシュ境界線上にあるのに図郭上ノードになっていない
		// ★エラー 図郭上ノードが2次メッシュ境界線上に無い
		checkBorderLine(walkNodeObj, walkNodeClass, basePolyline);

		// ★エラー 道路ノード上にあるのに道路NW接続ノードになっていない
		checkRoadNWConnectNode(walkNodeObj, walkNodeClass, roadNode, nodeRTree);


		// 接続リンクに関わるチェックを実施
		checkConenctLink(walkNodeObj, walkNode, walkLink, walkNodeClass, roadNodeID, walkClassIndex,
			walkNodeClassIndex, fromIdIndex, toIdIndex, walkNodeLinkIDs, roadLink, roadNode, linkRTree, nodeRTree, CheckType::Release);

		// ★エラー 歩行者リンクに接続していない（形状）
		checkWalkLinkConnectNode(walkNodeObj, walkLink, walkLinkRTree);

		// ★エラー ノードが関係ないリンクに接触している
		checkTouchOtherLink(walkNodeObj, walkLink, walkLinkRTree, walkNodeClass, walkNodeLinkIDs);

		// ★エラー ノードが別のノードに接触している
		checkTouchOtherNode(walkNodeObj, walkNode, walkNodeRTree, walkNodeX, walkNodeY);

		// 道路ノードIDのチェック
		// TODO: 下記の処理はCheckWalkNodeとまとめられる箇所をまとめる1
		if (roadNodeID != -1 && roadNodeID != 0) {
			// walkNodeRoadIndexは使われないので-1、walkNodeRTreeも使われないけど、渡せるので渡す
			compairRoadNodeCheck(walkNodeClass, roadNodeID, nodeNameIndex, -1,
				walkNodeObj, roadNode, walkNode, walkNodeRTree, true);
		}

		// 駅出入口ポイントIDのチェック
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kStationGatePointID, stationGate, true);
		// 地下街出入口ポイントIDのチェック
		CheckWalkNodeWithPoint(walkNodeObj, walkNode.m_FieldMap, kUndergroundGatePointID, poiPoint, true);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ★道路リンク接続ノードが道路リンク交差箇所にある
		checkRoadLinkCrossNode(walkNodeObj, walkNodeClass, roadLink, linkRTree, roadNode.m_FeatureMap, nodeRTree);

		
		if(walkNodeClass == node_class::kConnectedRoad)
		{
			// ★エラー ポイント同士（WALK_NODE、HEIGHT_NODE）の距離が近い
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

	// ★エラー 形状が存在しない
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

		// ★エラー 図郭の隅にノードがある
		m_output.OutputRegLog(err_level::error, err_code::BorderCornerNode);
	}
}
void WalkNodeCheckFunction::checkBorderLine(
	const GeoClass& walkNodeObj, long walkNodeClass, const IPolylinePtr& basePolyline)
{
	using namespace walk_node;
	// 座標からノード種別の確認
	if (AheIsContain4(basePolyline, walkNodeObj.m_ipGeometry))
	{
		// ★エラー 2次メッシュ境界線上にあるのに図郭上ノードになっていない
		if (walkNodeClass != walk_node::node_class::kOutline)
		{
			m_output.OutputRegLog(err_level::error, err_code::NgBorderNodeAttr,
				judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap));
		}
	}
	else
	{
		// ★エラー 図郭上ノードが2次メッシュ境界線上に無い
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
	// 道路ノード上か？
	bool bOnRoadNode = false;
	auto onNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(0.1), roadNodeRTree);
	for (const auto& onNodeID : onNodeIDs)
	{
		auto onNodeItr = roadNode.m_FeatureMap.find(onNodeID);
		if (onNodeItr == roadNode.m_FeatureMap.end())
			continue;

		// 一致するか？
		if (walkNodeObj.IsEqual(onNodeItr->second.m_ipGeometry)) {
			bOnRoadNode = true;
			break;
		}
	}

	// 状態と属性が一致するか確認
	if (!bOnRoadNode) return;
	// 道路ノード上＝道路NW接続ノードかどうか(図郭上ノードは対象外)
	if (walkNodeClass != walk_node::node_class::kRoadNW
		&& walkNodeClass != walk_node::node_class::kOutline) // ★エラー 道路ノード上にあるのに道路NW接続ノードになっていない
		m_output.OutputRegLog(err_level::error, err_code::NgOnRoadNode1,
			judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap));
}

void WalkNodeCheckFunction::checkWalkLinkConnectNode(
	const GeoClass& walkNodeObj,
	const FeaturesPack& walkLink,
	const BGRTree& walkLinkRTree)
{
	// 接続リンクを検索
	bool bFindLink = false;
	auto findWalkLinkIDs = util::getIntersectsIDs(walkNodeObj.GetBox(0.1), walkLinkRTree);

	// 見つかった歩行者リンクのループ
	for (const auto& findWalkLinkID : findWalkLinkIDs) {

		auto findWalkLinkItr = walkLink.m_FeatureMap.find(findWalkLinkID);
		if (findWalkLinkItr == walkLink.m_FeatureMap.end())
			continue;

		// From/Toのどちらかと一致するか確認
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
	// ★エラー 歩行者リンクに接続していない（形状）
	m_output.OutputRegLog(err_level::error, err_code::NoConnect, _T("形状"));
}

// TODO: リリースチェックと共通化したけど、更に細分化したいところ
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

	// 接続リンク数を取得
	long connectWalkLinkCnt = -1;
	auto walkNodeLinkItr = walkNodeLinkIDs.find(walkNodeObj.m_ObjectID);
	if (walkNodeLinkItr != walkNodeLinkIDs.end())
		connectWalkLinkCnt = walkNodeLinkItr->second.size();


	// 接続リンク数が1の場合、状態によって属性の一致を確認する
	if (connectWalkLinkCnt <= 0)
	{
		// ★エラー 歩行者リンクに接続していない（属性）
		m_output.OutputRegLog(err_level::error, err_code::NoConnect, _T("属性"));
	}
	else if (CheckType::Release != checkType) {
		if (connectWalkLinkCnt == 1)
		{
			// (接続リンク数が1)端点になりうるノードかどうか
			if (!judge_value::IsOKTerminalNode(walkNodeClass))
			{
				// ★エラー 端点なのに端点ノードになっていない
				m_output.OutputRegLog(err_level::error, err_code::NgTerminalNode1,
					judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap));
			}
		}
		else // 接続リンク数が1以外の場合、端点になっていないか確認する
		{
			if (walkNodeClass == walk_node::node_class::kTerminal)
			{
				// ★エラー 接続リンクが複数なのに、端点ノード
				m_output.OutputRegLog(err_level::error, err_code::NgTerminalNode2);
			}
		}
	}

	// 端点の場合、接続リンクが端点に接続して良い属性か確認する
	if (CheckType::Release != checkType && connectWalkLinkCnt != -1 && walkNodeClass == walk_node::node_class::kTerminal)
	{
		for (const auto& connectWalkLinkID : walkNodeLinkItr->second) {

			// 接続リンク取得
			auto connectWalkLinkItr = walkLink.m_FeatureMap.find(connectWalkLinkID);
			if (connectWalkLinkItr == walkLink.m_FeatureMap.end())
				continue;

			// 歩行者種別コードを確認
			long connectWalkLinkClass = connectWalkLinkItr->second.GetLongValue(walkLink.m_FieldMap.FindField(walk_link::kWalkClass));
			if (!judge_value::IsOKTerminalLink(connectWalkLinkClass, m_dataType)) {
				// ★エラー 端点ノードに接続してはいけない歩行者リンク種別
				CString info1, info2;
				info1.Format(_T("WalkLinkID:'%ld'"), connectWalkLinkID);
				info2 = judge_value::IsFindObj(connectWalkLinkClass, m_walkLinkClassMap);
				m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::warning, err_code::NgTerminalLink, info1, info2);
			}
		}
	}

	// 道路NW接続ノードの場合
	bool isConnectNode(false);
	if (m_dataType == walk_type::type_class::kGeneral)
		isConnectNode = walkNodeClass == walk_node::node_class::kRoadNW ? true : false;
	if (m_dataType == walk_type::type_class::kDownConvert)
		isConnectNode = ((walkNodeClass == walk_node::node_class::kRoadNW) || (walkNodeClass == walk_node::node_class::kConnectedRoad)) ? true : false;
	if (isConnectNode)
	{
		// 歩行者ノード上の道路ノードを検索
		GeoClass roadNodeObj;
		if (!findOverlapRoadNode(roadNodeObj, walkNodeObj, roadNode.m_FeatureMap, roadNodeRTree))
		{
			// ★エラー 道路NW接続ノードが道路ノード上に無い
			m_output.OutputRegLog(err_level::error, err_code::NgOnRoadNode2);
		}
		else if (CheckType::All == checkType && checkWalkableConnect(roadNodeObj, connectWalkLinkCnt, roadNode.m_FieldMap, roadLink, roadLinkRTree))
		{
			// 汎用向け場合 ★ワーニング 接続する道路リンクが歩行禁止のリンクのみ;ダウンコンバート向け場合 ★エラー
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::error, err_code::ConnectCarOnly);
		}
	}
	// 道路NW接続ノード以外の場合
	else
	{
		bool checkFlag = true;
		if (connectWalkLinkCnt != -1)
		{
			for (const auto& connectWalkLinkID : walkNodeLinkItr->second) {
				// 接続リンク取得
				auto connectWalkLinkItr = walkLink.m_FeatureMap.find(connectWalkLinkID);
				if (connectWalkLinkItr == walkLink.m_FeatureMap.end())
					continue;

				// 歩行者種別コードを確認
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
					if (!roadLinkObj.IsDisjoint(walkNodeObj.m_ipGeometry))// ★エラー 歩行者ノードが道路リンクに乗っている
						m_output.OutputRegLog(err_level::error, err_code::NgOnRoadLink, uh::str_util::ToString(roadLinkObj.m_ObjectID));
				}
			}
		}
	}

	// 道路ノード対応IDを持つ場合
	if (CheckType::Release != checkType && roadNodeID > -1 && connectWalkLinkCnt > 0) {

		bool bConnectCrossRoad = false;
		for (const auto& connectWalkLinkID : walkNodeLinkItr->second) {

			// 接続リンク取得
			auto connectWalkLinkItr = walkLink.m_FeatureMap.find(connectWalkLinkID);
			if (connectWalkLinkItr == walkLink.m_FeatureMap.end())
				continue;

			// 歩行者種別コードが横断歩道であるかを確認
			long connectWalkLinkClass = connectWalkLinkItr->second.GetLongValue(walkClassIndex);
			if (connectWalkLinkClass == walk_link::walk_class::kCrossRoad) {
				bConnectCrossRoad = true;
				break;
			}
		}
		if (!bConnectCrossRoad) // ★エラー 道路ノード対応IDをもつのに横断歩道リンクに接続していない
			m_output.OutputRegLog(err_level::error, err_code::NotConnectCross);
	}
	// bug 12687 - [u][歩行者]MV向け改札内データのチェック追加（CheckWalk.exe）②
	if (judge_value::IsTicketGate(walkNodeClass) && connectWalkLinkCnt > 0) {
		internalTicketGateConenctCheck(walkLink, walkNode, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex,
			toNodeIdIndex, walkNodeLinkItr->second, walkNodeLinkIDs);
	}

	// 道路リンク接続ノードの場合
	if ((CheckType::All == checkType || CheckType::Other == checkType) && walkNodeClass == walk_node::node_class::kConnectedRoad)
	{
		// 歩行者ノード上の道路リンクを検索
		std::vector<GeoClass> crossRoadLinks;
		if (util::findOverlapGeometry(walkNodeObj, roadLink, roadLinkRTree, crossRoadLinks) <= 0)
		{
			// ★エラー 道路リンク接続ノードが道路リンク上に無い
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
	// 端点ノードのみチェックする
	if (walkNodeClass != walk_node::node_class::kTerminal) return;
	using namespace walk_node;

	// 歩行者ノードのXY取得
	double x = 0.0, y = 0.0;
	IPointPtr ipWalkNodePoint(walkNodeObj.m_ipGeometry);
	ipWalkNodePoint->QueryCoords(&x, &y);

	// 接続リンクsetを取得する
	auto connectLinkItr = walkNodeLinkIDs.find(walkNodeObj.m_ObjectID);

	// 端点の場合、近傍に接続していないリンクが無いか確認（距離閾値で広げて検索）
	auto findWalkLinkIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_dist_threshold), walkLinkRTree);

	// 見つかった歩行者リンクのループ
	for (const auto& findWalkLinkID : findWalkLinkIDs)
	{
		auto findWalkLinkItr = walkLink.m_FeatureMap.find(findWalkLinkID);
		if (findWalkLinkItr == walkLink.m_FeatureMap.end())
			continue;

		// 接続リンクsetにあればスキップ
		if (connectLinkItr != walkNodeLinkIDs.end()
			&& connectLinkItr->second.find(findWalkLinkID) != connectLinkItr->second.end())
			continue;

		// 最近傍ポイント取得
		IProximityOperatorPtr ipProximity(findWalkLinkItr->second.m_ipGeometry);
		IPointPtr ipNearPoint = ipProximity->_ReturnNearestPoint(ipWalkNodePoint, esriNoExtension);
		if (!ipNearPoint)
			continue;

		// ダウンコンバート向け、ロジカルコネクションの歩行者リンク場合スキップ
		if (m_dataType == walk_type::type_class::kDownConvert)
		{
			long index = walkLink.m_FieldMap.FindField(walk_link::kWalkCode);
			if (index == -1) continue;
			if (findWalkLinkItr->second.GetLongValue(index) == walk_link::walk_class::kLogicalConnection)
				continue;
		}

		// 距離を測る
		double nearX = 0.0, nearY = 0.0;
		ipNearPoint->QueryCoords(&nearX, &nearY);
		double dist = g_cnv.GetDist(x, y, nearX, nearY);

		// 閾値を超える距離の場合、スキップ（エンベロープのため、斜めで閾値を超えるため）
		if (dist > m_dist_threshold)
			continue;

		CString info1, info2;
		info1.Format(_T("WalkLinkID:'%ld'"), findWalkLinkID);
		info2.Format(_T("距離:'%lf'm(閾値:'%lf'm)"), dist, m_dist_threshold);

		// ★エラー ノードが関係ないリンクに接触している
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
	// 別のノードが近くにないか確認（距離閾値で広げて検索）
	auto findWalkNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_dist_threshold), walkNodeRTree);

	// 見つかった歩行者ノードのループ
	for (const auto& findWalkNodeID : findWalkNodeIDs)
	{
		// 自身はスキップ
		if (findWalkNodeID == walkNodeObj.m_ObjectID)
			continue;

		auto findWalkNodeItr = walkNode.m_FeatureMap.find(findWalkNodeID);
		if (findWalkNodeItr == walkNode.m_FeatureMap.end())
			continue;

		// 距離を測る
		IPointPtr ipPoint(findWalkNodeItr->second.m_ipGeometry);
		double findX = 0.0, findY = 0.0;
		ipPoint->QueryCoords(&findX, &findY);
		double dist = g_cnv.GetDist(findX, findY, walkNodeX, walkNodeY);

		if (dist < m_dist_threshold)
		{
			CString info1, info2;
			info1.Format(_T("WalkNodeID:'%ld'"), findWalkNodeID);
			info2.Format(_T("距離:'%lf'm(閾値:'%lf'm)"), dist, m_dist_threshold);
			// ★エラー ノードが別のノードに接触している
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
	// 道路ノードを探索
	auto findNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(range), roadNodeRTree);

	// 見つかった道路ノードのループ
	for (const auto& findNodeID : findNodeIDs) {

		auto findNodeItr = roadNodeMap.find(findNodeID);
		if (findNodeItr == roadNodeMap.end())
			continue;

		// 一致するか？
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
	// エラーになるのは道路NW接続ノードに接続する歩行者リンクが1本のみの場合
	if (connectWalkLinkCnt != 1) return false;

	// 道路ノード種別が端点ノード以外の場合
	if (roadNodeObj.GetLongValue(fieldMap.FindField(road_node::kNodeClass)) != road_node::node_class::kTerminal)
	{
		auto findLinkIDs = util::getIntersectsIDs(roadNodeObj.GetBox(), roadLinkRTree);
		long roadNodeID = roadNodeObj.m_ObjectID;

		bool isAllNotwalk(false);  // 接続する道路リンクがすべて歩行禁止（Walkable=0）のフラグ
		for (const auto& findLinkID : findLinkIDs)
		{
			auto findLinkItr = roadLink.m_FeatureMap.find(findLinkID);
			if (findLinkItr == roadLink.m_FeatureMap.end())
				continue;

			// 歩行可能種別コード
			if (findLinkItr->second.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kFromNodeID)) == roadNodeID ||
				findLinkItr->second.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kToNodeID)) == roadNodeID)
			{
				if( road_link::walkable_class::kNotWalk != findLinkItr->second.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kWalkable)) )
					return false;

				isAllNotwalk = true;
			}
		}
		// 接続する道路リンクがすべて歩行禁止（Walkable=0）の場合
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
		// ★エラー 駅出入口ノードなのに駅出入口ポイントIDが付与されていない
		// ★エラー 地下街出入口ノードなのに地下街出入口ポイントIDが付与されていない
		if (walkNodeClass == checkWalkNodeClass)
			m_output.OutputRegLog(err_level::error, errorCodeList.at(0));
		return;
	}

	// ★エラー 駅出入口ポイントIDが付与されているのに駅出入口ノードではない
	// ★エラー 地下街出入口ポイントIDが付与されているのに地下街出入口ノードではない
	if (walkNodeClass != checkWalkNodeClass)
	{
		info1 = judge_value::IsFindObj(walkNodeClass, m_walkNodeClassMap);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(1), info1);
	}

	// 該当のポイントを取得
	auto pointItr = point.m_FeatureMap.find(pointId);

	// ★エラー 駅出入口ポイントIDに対応する駅出入口POIがない
	// ★エラー 地下街出入口ポイントIDに対応する地下街出入口POIがない
	if (pointItr == point.m_FeatureMap.end())
	{
		info1.Format(_T("%s:'%ld'"), checkField, pointId);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(2), info1);
		return;
	}
	// [bug 11619] 地下街出入口ポイントのチェックの場合、削除コードが「生存」でないなら、エラーとする
	// (releaseモードでは実行しない)
	else if (!bRelease
		&& node_class::kUndergroundGate == checkWalkNodeClass
		&& poi_point_org::delete_code::kAlive != pointItr->second.GetLongValue(point.m_FieldMap.FindField(poi_point_org::kDelete)))
	{
		// ★エラー ノードに紐付く地下街出入口POIの削除コードが「生存」ではない
		info1.Format(_T("%s:'%ld'"), checkField, pointId);
		m_output.OutputRegLog(err_level::error, errorCodeList.at(4), info1);
		return;
	}

	// 距離を測る
	auto& pointObj = pointItr->second;
	double dist = util::GetDistGeoClasses(walkNode, pointObj);

	// ★エラー 駅出入口ポイントIDに対応する駅出入口POIが近くにない
	// ★エラー 地下街出入口ポイントIDに対応する地下街出入口POIが近くにない
	if (dist > m_refer_id_threshold)
	{
		info1.Format(_T("%s:'%ld'"), checkField, pointId);
		info2.Format(_T("距離:'%lf'm(閾値:'%lf'm)"), dist, m_refer_id_threshold);
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
	// 図郭上ノードの場合
	if (walkNodeClass == walk_node::node_class::kOutline)
	{
		// ★エラー 図郭上ノードに道路ノード対応IDが付与されている
		info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
		m_output.OutputRegLog(err_level::error, err_code::BorderRoadNodeID, info1);
	}

	// 該当道路ノード取得
	auto roadNodeItr = roadNode.m_FeatureMap.find(roadNodeID);
	if (roadNodeItr == roadNode.m_FeatureMap.end()) {
		// ★エラー 道路ノード対応IDに対応する道路ノードがない
		info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
		m_output.OutputRegLog(err_level::error, err_code::NotFindRoadNode, info1);
	}
	else {
		// 距離を測る
		auto& roadNodeObj = roadNodeItr->second;
		double dist = util::GetDistGeoClasses(walkNodeObj, roadNodeObj);
		if (dist > m_refer_id_threshold) {
			// ★エラー 道路ノード対応IDに対応する道路ノードが近くにない
			info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
			info2.Format(_T("距離:'%lf'm(閾値:'%lf'm)"), dist, m_refer_id_threshold);
			m_output.OutputRegLog(err_level::error, err_code::TooFarRoadNode, info1, info2);
		}

		// 取得した道路ノードの交差点名称取得
		auto strName = roadNodeObj.GetStringValue(nodeNameIndex);
		if (strName.IsEmpty()) {
			// ★エラー 道路ノード対応IDに対応する道路ノードに交差点名称がない
			info1.Format(_T("RoadNodeID:'%ld'"), roadNodeID);
			m_output.OutputRegLog(err_level::error, err_code::NoNameRoadNode, info1);
		}
		else if (!isForRelease) {
			// 名称が存在する場合は、近傍に異なる名称が無いか確認する
			auto neighborWalkNodeIDs = util::getIntersectsIDs(walkNodeObj.GetBox(m_neighbor_threshold), walkNodeRTree);

			// 見つかった歩行者ノードのループ
			for (const auto& neighborWalkNodeID : neighborWalkNodeIDs) {

				// 自身はスキップ
				if (neighborWalkNodeID == walkNodeObj.m_ObjectID)
					continue;

				auto neighborWalkNodeItr = walkNode.m_FeatureMap.find(neighborWalkNodeID);
				if (neighborWalkNodeItr == walkNode.m_FeatureMap.end())
					continue;

				auto& neighborWalkNodeObj = neighborWalkNodeItr->second;
				// 距離を測って近傍閾値以上の場合はスキップ
				double neighborDist = util::GetDistGeoClasses(walkNodeObj, neighborWalkNodeObj);
				if (neighborDist > m_neighbor_threshold)
					continue;

				// 道路ノードIDが入ってなければスキップ
				long neighborRoadNodeID = neighborWalkNodeObj.GetLongValue(walkNodeRoadIndex);
				if (neighborRoadNodeID == 0 || neighborRoadNodeID == -1)
					continue;

				// IDの道路ノード取得
				auto neighborRoadNodeItr = roadNode.m_FeatureMap.find(neighborRoadNodeID);
				if (neighborRoadNodeItr == roadNode.m_FeatureMap.end())
					continue;

				// その交差点名称取得
				auto neighborName = neighborRoadNodeItr->second.GetStringValue(nodeNameIndex);

				// 空ならスキップ
				if (neighborName.IsEmpty())
					continue;

				// 比較して、異なる場合はエラー
				if (0 != strName.Compare(neighborName)) {
					// ★エラー 近傍に別の交差点名称を持つノードが存在する
					info1.Format(_T("'%s'/'%s'(WalkNodeID:%ld)"), strName, neighborName, neighborWalkNodeID);
					info2.Format(_T("距離:'%lf'm(閾値:'%lf'm)"), neighborDist, m_neighbor_threshold);
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
	// この関数内では、辿った結果、キャッシュ外になるかもしれないので、
	// .at(id)ではなくSearchObjを使用

	// チェック済みなら見ない
	auto result = inspectedIDs.insert(linkID);
	if (!result.second)
		return false;

	auto externalLink = util::SearchObj(walkLink, linkID);
	if (!externalLink.IsOk()) {
		// 見つからないのはエラーだが、ここはどうしようもない（他のところで出るはず）
		return false;
	}
	long walkClass = externalLink.GetLongValue(walkNodeClassIndex);
	if (judge_value::IsInternalTicketGateLink(walkClass)) {
		// 改札内リンクは除外
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
	// そもそも接続リンク数が２未満はエラー
	if (links.size() < 2) {
		// エラー 改札ノードなのに端点になっている
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
	// 改札内リンクのみならOK
	if (externalLinks.empty()) {
		return;
	}
	else if (externalLinks.size() == links.size()) {
		// 改札外リンクのみなら、ノードのエラー
		// エラー 改札ノードに改札内リンクが接続していない
		m_output.OutputRegLog(err_level::error, err_code::NgTicketGate2);

		return;
	}
	// 改札内/改札外あったら、ノードとしてはOKなので、改札外リンクが
	// 本当に改札外でいいのか（ネットワークとして道路に接続しているか）チェック
	std::set<long> inspectedIDs;
	for (auto linkID : externalLinks) {
		if (isConnectedRoadNode(linkID, walkClassIndex, walkNodeClassIndex, fromNodeIdIndex, toNodeIdIndex,
			walkLink, walkNode, walkNodeLinkIDs, inspectedIDs)) {
			return;
		}
	}
	// ここまで来たらアウト
	// エラー 改札ノードから道路接続ノードまで接続されていない
	m_output.OutputRegLog(err_level::error, err_code::NgTicketGate3);

}

// 道路リンク上にあるのに道路リンク接続ノードになっているかチェック
void WalkNodeCheckFunction::checkRoadLinkConnectNode(
	const GeoClass& walkNodeObject,
	long walkNodeClass,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree) const
{
	using namespace walk_node;
	// 道路リンク上
	std::vector<GeoClass> crossRoadLinks;
	if (util::findOverlapGeometry(walkNodeObject, roadLink, roadLinkRTree, crossRoadLinks) <= 0) return;

	if (walkNodeClass != walk_node::node_class::kConnectedRoad)
		m_output.OutputRegLog(err_level::error, err_code::NgOnRoadLinkException);
}

// 道路リンク接続ノードが道路リンク交差箇所にあるかチェック
void WalkNodeCheckFunction::checkRoadLinkCrossNode(
	const GeoClass& walkNodeObject,
	long walkNodeClass,
	const FeaturesPack& roadLink,
	const BGRTree& roadLinkRTree, 
	const std::map<long, GeoClass>& roadNodeMap,
	const BGRTree& roadNodeRTree) const
{
	using namespace walk_node;
	// 道路リンク交差箇所
	std::vector<GeoClass> crossRoadLinks;
	if(util::findOverlapGeometry( walkNodeObject, roadLink, roadLinkRTree, crossRoadLinks ) < 2) return;

	// 歩行者ノード上の道路ノードを検索
	GeoClass roadNodeObj;
	// 道路ノード上であれば、本チェックでは対象外
	if ( findOverlapRoadNode( roadNodeObj, walkNodeObject, roadNodeMap, roadNodeRTree, m_coordinateTolerance ) ) return;

	m_output.OutputRegLog( err_level::error, err_code::NgOnRoadLinkCross );
}

// スクランブル交差点なのにスクランブルエリアポリゴンが無いかチェック
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

// ポイント同士（WALK_NODE、HEIGHT_NODE）の距離が近いかチェック
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
			// 一致するか？
			if(walkNodeObject.m_ObjectID == node.m_ObjectID || !walkNodeObject.IsEqual( node.m_ipGeometry ) ) continue;

			CString info;
			info.Format(_T("%s(%d)"), walk_node::kTableName, node.m_ObjectID);
			m_output.OutputRegLog( err_level::fatal, err_code::PositionNearError, info, _T(""), 0.0, 0.0, false );
		}
	}
}

// ロジカルコネクションが複数接続しているかチェック
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

// ロジカルコネクションが鋭角に接続しているかチェック
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

// 中心点の隣接点の座標が取得
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
