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
	map<pair<long, long>, list<long>> dupNodeMap; //!< キー：From/ToノードID（IDの小さい方が先）、値：リンクのObjectID

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_link::kTableName);

	// 処理対象メッシュ内,正常な形状をもつwalkLinkIdのリスト
	std::vector<long> targetWalkLinkId;

	// エラーコード353用のリンクリスト
	map<long, GeoClass> walkLinkMapFor353;

	// 歩行者リンクのチェック
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
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
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

		// bug 12738で追加。追加時点で通常データには存在しないフィールドのため、なかったら「0」とする
		long scrambleF = scrambleFIndex >= 0 ? walkLinkObj.GetLongValue(scrambleFIndex) : 0;

		m_output.SetModifyInfo(walkLinkID, walkLinkObj.GetStringValue(modOperatorIndex),
			walkLinkObj.GetStringValue(modDateIndex), walkLinkObj.GetStringValue(modProgramIndex), walkLinkObj.GetStringValue(modProgramDateIndex));

		// ■単純な属性チェック
		// 整備上OKな値か確認する
		attributesCheck(walkLinkClass, floorMoveClass, onewayCode, noPassCode, roadSurfaceCode, walkCode, false);
		flagsCheck(autoWalkF, stairF, slopeF, escalatorF, arcadeF, openAreaF, tunnelF, scrambleF, onewayRF, noPassRF);

		// From/ToのIDを確認
		fromToIdCheck(walkLinkID, fromWalkNodeID, toWalkNodeID, dupNodeMap);

		// ■ここから形状も使ったチェック
		GeoClass fromWalkNodeObj, toWalkNodeObj;
		// 単純な形状確認
		if (!shapeCheck(walkLinkObj, walkNode, walkLinkClass, fromWalkNodeID, toWalkNodeID,
			walkNodeClassIndex, targetWalkLinkId, fromWalkNodeObj, toWalkNodeObj))
			continue;
		IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		// マッチングに関するチェック
		checkMatching(walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt,
			walkLinkClass, walkLink, roadLink, roadNode,
			walkLinkRTree, roadLinkRTree, roadNodeRTree, meshcode, bOther);
		// みなし横断歩道が交差している道路リンクが2車線以上
		inferredCrossRoadCheck(walkLinkObj, walkLinkClass, roadLink, roadLinkRTree);

		// 点列に分解して確認
		shapeCheck2(walkLinkObj, fromWalkNodeObj, toWalkNodeObj, false);

		// ■ここからは、リレーションや属性間の整合チェック
		// TODO: 関数を2つに分けたが、意味のある分け方かどうかは検討の余地あり
		attributeConformityCheck(floorMoveClass, stairF, slopeF, escalatorF, onewayCode, autoWalkF, arcadeF, openAreaF, tunnelF, scrambleF, walkLinkClass, walkCode);
		attributeConformityCheck2(onewayCode, noPassCode, noPassRF, onewayRF, walkLinkClass, walkLinkID);

		// ■前後のリンクを使用してのチェック
		if (!fromWalkNodeObj.IsOk() || !toWalkNodeObj.IsOk())
			continue;

		// bug 11430 - [u][要望]前後の歩行者種別と異なる場合に検出してほしい
		// 歩行者種別コードが歩道橋、施設敷地内（屋外）、施設敷地内（屋内）、地下通路、横断地下道の場合
		// 本番チェックは後で行う
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
			// 属性変化確認（とりあえず、2差路限定）
			if (fromLinkIDSetItr->second.size() == 2 && toLinkIDSetItr->second.size() == 2)
			{
				attributeChangeCheck(fromLinks, toLinks, walkLinkObj, walkLink, walkLinkClass, floorMoveClass, floorLevelIndex);
			}

			// 広場フラグがONの場合、接続しているリンクに広場フラグがONのリンクが両端に存在しているか確認
			openAreaCheck(openAreaF, fromLinks, toLinks, walkLink);

			// 歩行者種別が接続リンクの場合、接しているノードが道路NW接続ノード、または接続しているリンクが人車共用リンクであるか確認
			connectLinkCheck(walkLinkClass, fromWalkNodeObj, toWalkNodeObj, fromLinks, toLinks, walkLink, walkNodeClassIndex);
			// fromLinkIDSetItr,toLinkIDSetItrを保証するため、順番を入れ替えた
			underGroundCheck(walkLinkClass, walkLinkID, fromWalkNodeObj, toWalkNodeObj, fromLinks, toLinks, walkLink, walkNode, meshcode);
			// bug 12633- [u][要望]交差点名称が付与されていない横断歩道リンク端点をエラーとしてほしい
			if (walk_class::kCrossRoad == walkLinkClass) {
				crossRoadNodeCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeIDIndex, toWalkNodeIDIndex,
					walkLinkObj, fromWalkNodeObj, toWalkNodeObj, walkLink, walkNode,
					walkNodeLinkIDs, meshcode);
			}
			// bug 12687 - [u][歩行者]MV向け改札内データのチェック追加の①
			else if (judge_value::IsInternalTicketGateLink(walkLinkClass)) {
				internalTicketGateLinkCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeObj, toWalkNodeObj,
					fromLinks, toLinks);
			}
		}
		// bug 11433 - [u][要望]並行する形状の場合に属性の矛盾をチェックしてほしい
		// 階層移動種別コードが「0:上下なし」以外の場合
		parallelCheck(walkLinkClass, floorMoveClass, escalatorF, onewayCode, walkLinkObj, fromWalkNodeID,
			toWalkNodeID, walkNodeLinkIDs, walkLink, walkLinkRTree);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		long wheelChairIndex = walkLink.m_FieldMap.FindField(kWheelChair);
		long platformNumberIndex = walkLink.m_FieldMap.FindField(kPlatformNumber);
		long wheelChairCode = walkLinkObj.GetLongValue(wheelChairIndex);
		long platformNumber = walkLinkObj.GetLongValue(platformNumberIndex);

		// ★エラー スクランブルフラグが付与されているのにスクランブルエリア内に無い
		walkLinkNotOnScrambleAreaCheck(walkLinkObj, scrambleArea, scrambleAreaRTree, scrambleF);
		// ★エラー スクランブルエリア内なのにスクランブルフラグが付与されていない
		walkLinkOnScrambleAreaCheck(walkLinkObj, scrambleArea, scrambleAreaRTree, walkLinkClass, scrambleF);
		// ★エラー 車椅子通行コードが不正な値
		checkWheelChair(wheelChairCode);
		// ★エラー のりば番号が不正な値
		checkPlatformNumber(platformNumber);

		if (walkLinkClass == walk_class::ECode_draft::kLogicalConnection)
		{
			// ★エラー 微小ロジカルコネクション
			checkMicroLogicalConnection(walkLinkObj);
		}
		// ★エラー ロジカルコネクションと交差している
		if (walkLinkClass == walk_class::kLogicalConnection)
			checkCrossWalkLink(walkLinkObj, walkLink, walkLinkRTree);
	}
	// エラーコード353のチェック
	attributeConnectionCheck(ipMeshGeo, walkLink, walkNode, walkNodeLinkIDs, walkLinkMapFor353);

	// ★エラー 同一From/Toを持つリンクが複数存在
	sameFromToCheck(dupNodeMap, walkLink, meshcode);

	// 構成点が他リンクに含まれないか、総当りでチェック
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

	// 歩行者リンクのチェック
	long  walkLinkClassIndex = walkLink.m_FieldMap.FindField(walk_link::kWalkClass);
	long walkableIndex = roadLink.m_FieldMap.FindField(road_link::kWalkable);
	/* レビュー用コメント
		元が「walk_link::kOperator」等になっていたが、「road_link」の間違いと思われる
		所詮文字列なので、結果に問題はなかったはず
	*/
	long modOperator_RLIndex = roadLink.m_FieldMap.FindField(road_link::kOperator);
	long modDate_RLIndex = roadLink.m_FieldMap.FindField(road_link::kModifyDate);
	long modProgram_RLIndex = roadLink.m_FieldMap.FindField(road_link::kModifyProgName);
	long modProgramDate_RLIndex = roadLink.m_FieldMap.FindField(road_link::kProgModifyDate);
	for (const auto& walkLink : walkLink.m_FeatureMap)
	{
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (walkLink.second.IsDisjoint(ipMeshGeo))
			continue;

		auto& walkLinkObj = walkLink.second;
		// 人車共用リンク以外はチェック不要
		long  walkLinkClass = walkLinkObj.GetLongValue(walkLinkClassIndex);
		if (walkLinkClass != walk_link::walk_class::kWalkableRoad)
			continue;

		// 不正形状は本チェックの対象外（そもそもチェック仕様とすると落ちる)
		esriGeometryType eType = (walkLinkObj.m_ipGeometry)->GetGeometryType();
		IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		if (!walkLinkObj.m_ipGeometry
			|| !ipWalkLinkPointCol
			|| walkLinkPointCnt < 2
			|| eType != esriGeometryPolyline)
			continue;

		// 歩行者リンクの範囲の箱に引っかかる道路リンクを取得
		auto findRoadLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), roadLinkRTree);

		GeoClass findRoadLinkObj; // マッチング結果を格納
		bool matchLink = isWalkLinkOnRoadLink(findRoadLinkObj, walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt, walkLinkClass,
			findRoadLinkIDs, roadLink.m_FeatureMap);

		// マッチする道路がないならエラー出力対象はない
		if (!matchLink) continue;

		if (road_link::walkable_class::kNotWalk == findRoadLinkObj.GetLongValue(walkableIndex))
			continue;

		// ★エラー 人車共用リンクと一致する道路リンクが歩行禁止以外
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
	map<pair<long, long>, list<long>> dupNodeMap; //!< キー：From/ToノードID（IDの小さい方が先）、値：リンクのObjectID

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, walk_link::kTableName);

	// 処理対象メッシュ内,正常な形状をもつwalkLinkIdのリスト
	std::vector<long> targetWalkLinkId;

	// 歩行者リンクのチェック
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
		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
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

		// ■単純な属性チェック
		// 各属性値が定義内か確認する
		attributesCheck(walkLinkClass, -1, onewayCode, noPassCode, -1, -1, true);
		// From/ToのIDを確認
		fromToIdCheck(walkLinkID, fromWalkNodeID, toWalkNodeID, dupNodeMap);

		// ■ここから形状も使ったチェック
		// 単純な形状確認
		GeoClass fromWalkNodeObj, toWalkNodeObj;
		if (!shapeCheck(walkLinkObj, walkNode, walkLinkClass, fromWalkNodeID, toWalkNodeID,
			walkNodeClassIndex, targetWalkLinkId, fromWalkNodeObj, toWalkNodeObj))
			continue;
		IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
		long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
		// マッチングに関するチェック
		checkMatching(walkLinkObj, ipWalkLinkPointCol, walkLinkPointCnt,
			walkLinkClass, walkLink, roadLink, roadNode,
			walkLinkRTree, roadLinkRTree, roadNodeRTree, meshcode, false);
		shapeCheck2(walkLinkObj, fromWalkNodeObj, toWalkNodeObj, true);

		auto fromLinkIDSetItr = walkNodeLinkIDs.find(fromWalkNodeID);
		auto toLinkIDSetItr = walkNodeLinkIDs.find(toWalkNodeID);

		// 歩行者種別が接続リンクの場合、接しているノードが道路NW接続ノード、または接続しているリンクが人車共用リンクであるか確認
		if (fromLinkIDSetItr != walkNodeLinkIDs.end() && toLinkIDSetItr != walkNodeLinkIDs.end())
		{
			auto fromLinks = util::GetObjects(walkLinkID, fromLinkIDSetItr->second, walkLink.m_FeatureMap);
			auto toLinks = util::GetObjects(walkLinkID, toLinkIDSetItr->second, walkLink.m_FeatureMap);
			connectLinkCheck(walkLinkClass, fromWalkNodeObj, toWalkNodeObj, fromLinks, toLinks, walkLink, walkNodeClassIndex);
			// bug 12633- [u][要望]交差点名称が付与されていない横断歩道リンク端点をエラーとしてほしい
			if (walk_class::kCrossRoad == walkLinkClass) {
				crossRoadNodeCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeIDIndex, toWalkNodeIDIndex,
					walkLinkObj, fromWalkNodeObj, toWalkNodeObj, walkLink, walkNode,
					walkNodeLinkIDs, meshcode);
			}
			// bug 12687 - [u][歩行者]MV向け改札内データのチェック追加の①
			else if (judge_value::IsInternalTicketGateLink(walkLinkClass)) {
				internalTicketGateLinkCheck(walkNodeClassIndex, walkLinkClassIndex, fromWalkNodeObj, toWalkNodeObj,
					fromLinks, toLinks);
			}
		}
	}

	sameFromToCheck(dupNodeMap, walkLink, meshcode);

	// 構成点が他リンクに含まれないか、総当りでチェック
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
		// 見つかった道路のループ
		for (const auto& findRoadLinkID : roadLinkSet)
		{
			auto findRoadLinkItr = roadLinkMap.find(findRoadLinkID);
			if (findRoadLinkItr == roadLinkMap.end())
				continue;

			findRoadLinkObj = findRoadLinkItr->second;

			// 形状一致か包含関係
			if (AheIsContain2(findRoadLinkObj.m_ipGeometry, walkLinkObj.m_ipGeometry))
			{
				// From/To以外の構成点が一致しているかチェック
				bool allPointMatch = true;
				// 道路リンクの構成点を取得
				IPointCollectionPtr ipRoadLinkPointCol(findRoadLinkObj.m_ipGeometry);
				long roadLinkPointCnt = ipRoadLinkPointCol->GetPointCount();

				// 道路リンクの構成点の内、歩行者リンクに含まれるポイントを取得
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
				// 歩行者リンクの構成点がすべて道路リンクのいずれかの構成点とマッチしているかチェック
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
				// 人車共用リンクの場合、端点が道路リンクに含まれるかチェック
				// (allPointMatchがすでにfalseになっているなら、この判定は不要)
				// 端点は道路リンクに含まれるか、道路リンクの端点位置と同じでなければならない
				if (allPointMatch)
					allPointMatch &= util::isLinkEdgeOnLink(walkLinkObj.m_ipGeometry, findRoadLinkObj.m_ipGeometry, m_coordinateTolerance);

				if (allPointMatch)
					return true;
			}
		}
	}
	else
	{
		// 見つかった道路のループ
		for (const auto& findRoadLinkID : roadLinkSet)
		{
			auto findRoadLinkItr = roadLinkMap.find(findRoadLinkID);
			if (findRoadLinkItr == roadLinkMap.end())
				continue;
			// 形状一致か包含関係
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

	// 人車共用リンクの場合
	if (walkLinkClass == walk_link::walk_class::kWalkableRoad)
	{
		if (!matchLink) // ★エラー 人車共用リンクが道路リンクにマッチしていない
			m_output.OutputRegLog(err_level::error, err_code::UnMatchRoadLink);
		else if (!bOther)
		{
			if (findRoadLinkObj.GetLongValue(roadLink.m_FieldMap.FindField(road_link::kWalkable)) != road_link::walkable_class::kNotWalk)
				// ★エラー 人車共用リンクと一致する道路リンクが歩行禁止以外
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
	// 人車共用リンク以外の場合
	else if (m_dataType == walk_type::type_class::kGeneral && matchLink) // ★エラー 人車共用リンクではないのに道路リンクにマッチしている
		m_output.OutputRegLog(err_level::error, err_code::MatchRoadLink,
			judge_value::IsFindObj(walkLinkClass, m_walkLinkClassMap));

	// ダウンコンバート向け場合、ロジカルコネクション以外の歩行者リンクを対象とする、歩行者リンクの一部、あるいは全部が道路リンクに重なっている場合エラー
	if (m_dataType == walk_type::type_class::kDownConvert
		&& walkLinkClass != walk_link::walk_class::kLogicalConnection && matchLink)
		m_output.OutputRegLog(err_level::error, err_code::MatchRoadLink,
			judge_value::IsFindObj(walkLinkClass, m_walkLinkClassMap), _T(""), 0.0, 0.0, false);

	// 汎用向け場合 人車共用リンク以外の歩行者リンクに道路ノードが含まれている場合エラー
	if (m_dataType == walk_type::type_class::kGeneral && walk_link::walk_class::kWalkableRoad == walkLinkClass)
		return;
	// Error code:352, "歩行者リンクに道路ノードが含まれている"のチエック
	long modOperator_RNIndex = roadNode.m_FieldMap.FindField(road_node::kOperator);
	long modDate_RNIndex = roadNode.m_FieldMap.FindField(walk_link::kModifyDate);
	long modProgram_RNIndex = roadNode.m_FieldMap.FindField(walk_link::kModifyProgName);
	long modProgramDate_RNIndex = roadNode.m_FieldMap.FindField(walk_link::kProgModifyDate);
	auto findRoadNodeIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), roadNodeRTree);
	for (auto& roadNodeID : findRoadNodeIDs)
	{
		GeoClass roadNodeObj = roadNode.m_FeatureMap.at(roadNodeID);
		if (roadNodeObj.IsWithin(walkLinkObj.m_ipGeometry)) // ★エラー 道路ノードが歩行者リンクに含まれている
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
	// 汎用向け場合、歩行者リンク（人車共用リンクを除く）
	if (m_dataType == walk_type::type_class::kGeneral && walk_link::walk_class::kWalkableRoad == walkLinkClass)
		return;
	// ダウンコンバート向け場合、歩行者リンク（接続リンク・ロジカルコネクションを除く）
	if (m_dataType == walk_type::type_class::kDownConvert && (walk_link::walk_class::kConnect == walkLinkClass || walk_link::walk_class::kLogicalConnection == walkLinkClass))
		return;

	// 以下は人車共用リンク以外のチェック
	auto findWalkLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(0), walkLinkRTree);

	std::vector<GeoClass> crossPoints;
	// 道路リンク、または歩行者リンクと交差箇所が2か所以上ある場合、交差箇所の距離が一定距離あるか
	GetCrossPoint(walkLinkObj, findRoadLinkIDs, roadLink, crossPoints, false);
	GetCrossPoint(walkLinkObj, findWalkLinkIDs, walkLink, crossPoints, true);

	if (crossPoints.size() > 1)
	{
		for (unsigned int i = 0; i < crossPoints.size() - 1; ++i)
			for (unsigned int j = i + 1; j < crossPoints.size(); ++j)
			{
				// 同一の交差箇所が複数出現する場合があるため
				if (crossPoints.at(i).m_ObjectID == crossPoints.at(j).m_ObjectID)
					if (crossPoints.at(i).IsEqual(crossPoints.at(j).m_ipGeometry))
						continue;

				if (m_dist_threshold > util::GetDistGeoClasses(crossPoints.at(i), crossPoints.at(j)))
					// ★エラー 2つ以上のリンクとの交差箇所が近い
				{
					double x = 0, y = 0;
					((IPointPtr)crossPoints.at(i).m_ipGeometry)->get_X(&x);
					((IPointPtr)crossPoints.at(i).m_ipGeometry)->get_Y(&y);

					// 汎用向け場合
					if (m_dataType == walk_type::type_class::kGeneral)
						m_output.OutputRegLog(err_level::error, err_code::ThreeMoreCross,
							ToString(crossPoints.at(i).m_ObjectID),
							ToString(crossPoints.at(j).m_ObjectID), x, y);

					// ダウンコンバート向け場合
					if (m_dataType == walk_type::type_class::kDownConvert)
					{
						CString info1(ToString(crossPoints.at(i).m_ObjectID) + _T("/") + ToString(crossPoints.at(j).m_ObjectID)), info2;

						info2.Format(_T("距離:'%lf'm(閾値:'%lf'm)"), util::GetDistGeoClasses(crossPoints.at(i), crossPoints.at(j)), m_dist_threshold);
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

		// 検索対象が歩行者リンクの場合
		if (isWalkLink)
		{
			// 基準リンクと同じIDの場合スキップ
			if (baseGeo.m_ObjectID == findLinkID)
				continue;

			// 汎用向け場合、リンク種別が人車共用リンクの場合スキップ
			long walkClass = findLinkObj.GetLongValue(link.m_FieldMap.FindField(walk_link::kWalkClass));
			if (m_dataType == walk_type::type_class::kGeneral && walkClass == walk_link::walk_class::kWalkableRoad)
				continue;

			// ダウンコンバート向け場合、検索対象が歩行者リンクが接続リンク・ロジカルコネクションの場合スキップ
			if (m_dataType == walk_type::type_class::kDownConvert && (walkClass == walk_link::walk_class::kConnect || walkClass == walk_link::walk_class::kLogicalConnection))
				continue;
		}
		// 検索対象が道路リンクの場合
		else
		{
			// リリース対象外はスキップ（bug 11999）
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
	// TODO:ArcHelperのAheGetConnectedNetworkLink（AheNetwork.h）を利用する手法に切り替える。

	auto searchItr = nodeLinkIdMap.find(nodeID);
	if (searchItr == nodeLinkIdMap.end())
		return;

	for (const auto searchID : searchItr->second)
	{
		// 検索対象が基準リンクの場合スキップ
		if (searchID == baseLinkID)
			continue;

		// 検索対象が検索範囲外の場合スキップ
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

	// 地下通路の階層移動種別を取得
	const long underFloorMove = walkLink.m_FeatureMap.at(underLinkID).GetLongValue(walkLink.m_FieldMap.FindField(walk_link::kFloorMoveClass));

	long nodeClassIndex = fieldMap.FindField(walk_node::kNodeClass);
	long floorMoveIndex = walkLink.m_FieldMap.FindField(walk_link::kFloorMoveClass);
	std::set<long> errLinks; // エラーとなるリンクID群を格納
	for (auto& checkedLink : connectLinkMap)
	{
		// 地下通路と横断地下道はチェック対象外（bug 12000）
		// 「地下改札内」「地下プラットホーム」も対象外（bug 12738）
		long walkClass = checkedLink.GetLongValue(walkLink.m_FieldMap.FindField(walk_link::kWalkClass));
		if (judge_value::IsUnderGroundLink(walkClass))
			continue;

		// エレベーターノードが接続しているなら、エラーにはならない
		if (walk_node::node_class::kElevator == walkNodeObj.GetLongValue(nodeClassIndex))
			continue;

		// 階層移動種別の向きを確認
		long floorMove = checkedLink.GetLongValue(floorMoveIndex);

		// 地下通路と始点側で接続しているか否か
		if (util::isFromNode(checkedLink.m_ObjectID, walkLink, walkNodeObj.m_ObjectID))
		{
			// 始点の場合、階層移動種別が正方向でないものがエラー
			if (walk_link::floor_move_class::kForeUp == floorMove)
				continue;
		}
		else
		{
			// 終点の場合、階層移動種別が逆方向でないものがエラー
			if (walk_link::floor_move_class::kReverseUp == floorMove)
				continue;
		}

		// リンクに階層移動種別が付与されていない場合は、
		// 接続している地下通路リンクの階層移動種別の向きが上る方向ならエラーではない
		if (walk_link::floor_move_class::kNone == floorMove)
		{
			// 地下通路リンクが始点側で接続しているか否か
			if (util::isFromNode(underLinkID, walkLink, walkNodeObj.m_ObjectID))
			{
				// 始点で接続している場合、地下通路リンクの階層移動種別が逆方向でないものがエラー
				if (walk_link::floor_move_class::kReverseUp == underFloorMove)
					continue;
			}
			else
			{
				// 終点で接続している場合、地下通路リンクの階層移動種別が正方向でないものがエラー
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

	// チェック対象の構成点がotherlink上にあるかどうかのチェックをするラムダ式
	auto isPointOnOtherLink = [](
		const IPointCollectionPtr& checkedPointColl //!< チェック対象の構成点
		, long checkedPointCount    //!< チェック対象の構成点の数
		, const IGeometryPtr& otherlink   //!< チェック対象の構成点が乗ってはいけないリンク
		, const IPointPtr& otherFromPoint //!< otherlinkの始点
		, const IPointPtr& otherToPoint   //!< otherlinkの終点
		, double coordinateTolerance  //!< リンク上にポイントが乗っているかどうかを判定するときの許容値
		)
	{
		for (long index = 0; index < checkedPointCount; ++index)
		{
			IPointPtr point;
			checkedPointColl->get_Point(index, &point);
			if (util::isPointOnLink(otherlink, point, coordinateTolerance))
			{
				// 端点同士が繋がっているのはエラーではない
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

	std::set<long> onOtherLink; // エラー出力対象のリンクID
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

			// ターゲット側の全構成点を取得
			IPointCollectionPtr targetPoint(targetGeom);
			long targetPointCount = 0;
			targetPoint->get_PointCount(&targetPointCount);

			IPointPtr targetFromPoint = AheGetFromPoint(targetGeom);
			IPointPtr targetToPoint = AheGetToPoint(targetGeom);
			// ベース側にエラーとなるものがないかチェック
			if (isPointOnOtherLink(basePoint, basePointCount, targetGeom, targetFromPoint, targetToPoint, m_coordinateTolerance))
				onOtherLink.insert(*itBaseID);

			// ベース側にエラーとなるものがないかチェック
			if (isPointOnOtherLink(targetPoint, targetPointCount, baseGeom, baseFromPoint, baseToPoint, m_coordinateTolerance))
				onOtherLink.insert(*itTargetID);
		}
	}
	// エラー出力
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

// メッシュ外の接続歩行者リンクを取得する
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

	// 探索の起点となるノードがキャッシュされたMap内に存在しない、
	// または、指定ノードに接続するリンクが1つのみの場合、
	// そのノードがwalkNodeメッシュ内のリンク・ノード情報を既存コンテナに追加する
	auto baseNodeIt = walkNode.m_FeatureMap.find(nodeId);
	if ((walkNode.m_FeatureMap.end() == baseNodeIt) ||
		util::isOnlyTargetWalkLink(baseNodeIt, linkId, nodeLinkIDs, walkNode.m_FieldMap.FindField(walk_node::kNodeClass)))
	{
		addMeshInfo(nodeId, ipMeshGeo,
			walkLink, walkNode, nodeLinkIDs);

		baseNodeIt = walkNode.m_FeatureMap.find(nodeId);
	}

	// 再検索してもnodeMapに存在しない場合は、returnしてしまう
	if (walkNode.m_FeatureMap.end() == baseNodeIt)
		return result;

	// ノード種別が端点ノードの場合、探索終了
	if (walk_node::node_class::kTerminal == walkNode.m_FeatureMap.at(nodeId).GetLongValue(walkNode.m_FieldMap.FindField(walk_node::kNodeClass)))
		return result;

	std::set<long> linkIDSet = nodeLinkIDs.find(nodeId)->second;
	auto adjacentLink = util::GetObjectMap(linkId, linkIDSet, walkLink.m_FeatureMap);

	// 境界ノードではない場合、探索終了
	if (!util::isMeshEdgeNode(nodeId, walkNode, true))
	{
		for (auto& linkInfo : adjacentLink)
			result.insert(linkInfo.first);
		return result;
	}

	// 接続リンクが複数（2差路ではない）場合、探索終了
	if (adjacentLink.size() > 1)
	{
		for (auto& linkInfo : adjacentLink)
			result.insert(linkInfo.first);
		return result;
	}

	// ここまできた場合、指定ノードに接続するリンクが0件となるが、
	// 0件だと以降の処理が行えないので、returnしてしまう
	if (adjacentLink.empty())
		return result;

	// 接続リンクの非接続側の端点ノードを取得する
	long adjLinkId = adjacentLink.begin()->first;
	long otherNodeId = util::getOtherEdgeNode(adjLinkId, walkLink, nodeId);

	// 非接続側のノードがチェック範囲外の場合、その所在するメッシュの情報を既存コンテナーに追加する

	auto otherNodeIt = walkNode.m_FeatureMap.find(otherNodeId);
	if ((walkNode.m_FeatureMap.end() == otherNodeIt) ||
		util::isOnlyTargetWalkLink(otherNodeIt, adjLinkId, nodeLinkIDs, walkNode.m_FieldMap.FindField(walk_node::kNodeClass)))
	{
		addMeshInfo(otherNodeId, ipMeshGeo,
			walkLink, walkNode, nodeLinkIDs);

		otherNodeIt = walkNode.m_FeatureMap.find(otherNodeId);
	}

	// 再検索してもnodeMapに存在しない場合は、returnしてしまう
	if (walkNode.m_FeatureMap.end() == otherNodeIt)
		return result;

	// 2次メッシュ境界を跨ぐリンクに対し、util::getAdjacentLink()で端点ノードIDが変更されるので、
	// 後のチェックに影響しないように、局所的な変数tempNodeIDを使う
	auto tempNodeID = otherNodeId;
	std::set<long> secondLinkIDSet = nodeLinkIDs.find(otherNodeId)->second;
	auto secondAdjLinks = util::GetObjectMap(adjLinkId, secondLinkIDSet, walkLink.m_FeatureMap);
	result = util::getAdjacentLink(tempNodeID, secondAdjLinks, walkLink, walkNode, nodeLinkIDs, true);

	if (result.empty())
	{
		// nodeIdが探索の終点
		nodeId = otherNodeId;

		// 隣接メッシュを基準メッシュとして再帰的に探索
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

// 対象ノードの所在するメッシュの情報を追加する
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
		// 指定したOIDを持つノードが存在しない場合は、以降の処理は行わない
		return;
	}
	auto targetNodeObj = GeoClass(ipRow);
	// 端点の座標を取得する
	DOUBLE dx = 0.0f, dy = 0.0f;
	INT nMeshCode = 0, nMeshX = 0, nMeshY = 0;
	IPointPtr ipAdjPoint(targetNodeObj.m_ipGeometry);
	ipAdjPoint->QueryCoords(&dx, &dy);
	nMeshCode = AheLL2MESH(dx, dy, 64, nMeshX, nMeshY, 1);

	// 端点が都市地図メッシュの境界か
	bool isCityMeshEdge = false;
	if (0 == nMeshX || 0 == nMeshY)
		isCityMeshEdge = true;

	// 端点の所在する都市地図メッシュを取得する
	ISpatialReferencePtr ipSpRef;
	ipMeshGeo->get_SpatialReference(&ipSpRef);

	IGeometryPtr ipAdjMeshGeo, ipAdjMeshGeo2;
	ipAdjMeshGeo = AheMESH2POLY(nMeshCode, ipSpRef);

	// 端点ノードが都市地図メッシュの境界上の場合、隣接のメッシュも取得する必要がある
	// 上と右の境界ノードは、AheLL2MESHで別のメッシュコードに変換されるので、ここでは左と下だけ処理する
	if (isCityMeshEdge)
	{
		long adjMeshCode = 0;

		// XとYが両方とも0の場合、左、下、左下の３メッシュを取得
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

	// 歩行者リンク取得
	if (!util::GetLinkFeaturesInGeometry(ipAdjMeshGeo, field_lists::walkLinkFieldList, _T(""), walkLink, walkNodeLinkIDs))
		return;

	// 歩行者ノード取得
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
	// ■単純な属性チェック
	// 各属性値が定義内か確認する
	if (!judge_value::IsDefineWalkLinkClass(walkLinkClass, m_dataType))   // ★エラー 歩行者種別が定義外
	{
		if (m_dataType == walk_type::type_class::kGeneral)
			m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkClass1, ToString(walkLinkClass));
		if (m_dataType == walk_type::type_class::kDownConvert)
			m_output.OutputRegLog(err_level::fatal, err_code::NgWalkLinkClass1, ToString(walkLinkClass), _T(""), 0.0, 0.0, false);
	}


	if (!judge_value::IsDefineOnewayCode(onewayCode))         // ★エラー 一方通行種別コードが定義外
		m_output.OutputRegLog(err_level::error, err_code::NgOnewayCode1, ToString(onewayCode));

	if (!judge_value::IsDefineNoPassCode(noPassCode))         // ★エラー 歩行者通行禁止コードが定義外
		m_output.OutputRegLog(err_level::error, err_code::NgNoPassCode, ToString(noPassCode));

	if (!isForRelease) {
		if (!judge_value::IsDefineFloorMoveClass(floorMoveClass)) // ★エラー 階層移動種別コードが定義外
			m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveClass, ToString(floorMoveClass));

		if (!judge_value::IsDefineRoadSurfaceClass(roadSurfaceCode))     // ★エラー 路面素材コードが定義外
			m_output.OutputRegLog(err_level::error, err_code::NgRoadSurfaceClass, ToString(roadSurfaceCode));

		if (!judge_value::IsDefineWalkCode(walkCode))             // ★エラー 歩行者路線コードが定義外
			m_output.OutputRegLog(err_level::error, err_code::NgWalkCodeC, ToString(walkCode));
	}

	// 整備上OKな値か確認する
	if (!judge_value::IsOKWalkLinkClass(walkLinkClass)) // ★エラー 歩行者種別が定義外
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
	// 各フラグフィールドに0と1以外が無いか確認する
	// ★エラー フラグフィールドなのに、[0]と[1]以外の値
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
	// From/ToのIDを確認　
	bool bNodeIDErr = false;
	if (fromWalkNodeID <= 0) { // ★エラー 始点ノードIDが不正
		m_output.OutputRegLog(err_level::error, err_code::NgFromID, ToString(fromWalkNodeID));
		bNodeIDErr = true;
	}
	if (toWalkNodeID <= 0) {   // ★エラー 終点ノードIDが不正
		m_output.OutputRegLog(err_level::error, err_code::NgToID, ToString(toWalkNodeID));
		bNodeIDErr = true;
	}
	if (fromWalkNodeID == toWalkNodeID) { // ★エラー リンクの始終点が等しい（属性）
		m_output.OutputRegLog(err_level::error, err_code::LoopLinkSingleFT, _T("属性"));
		bNodeIDErr = true;
	}
	// 同一From/Toが存在しないか確認のため、正常なものはノードIDのペアをキーに、リンクIDを格納しておく
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

	// TODO　形状存在しない場合に、今のソースロジックよりこのエラーチェック処理に入れないので、エラーコード313エラーメッセージがエラーログファイルに出せない。
	// ■ここから形状も使ったチェック
	// 単純な形状確認
	esriGeometryType eType = (walkLinkObj.m_ipGeometry)->GetGeometryType();
	IPointCollectionPtr ipWalkLinkPointCol(walkLinkObj.m_ipGeometry);
	long walkLinkPointCnt = ipWalkLinkPointCol->GetPointCount();
	if (!walkLinkObj.m_ipGeometry
		|| !ipWalkLinkPointCol
		|| walkLinkPointCnt < 2
		|| eType != esriGeometryPolyline) {
		// ★エラー 歩行者リンク形状が不正
		m_output.OutputRegLog(err_level::error, err_code::NgWalkLinkGeom);
		return false;
	}
	// 形状が正しいものは360チェックで使用するので、ここで保持しておく
	targetWalkLinkId.push_back(walkLinkObj.m_ObjectID);

	// From/Toノード取得
	auto fromNodeItr = walkNode.m_FeatureMap.find(fromWalkNodeID);
	auto toNodeItr = walkNode.m_FeatureMap.find(toWalkNodeID);

	// Fromノードの存在確認
	fromWalkNodeObj = util::SearchObj(walkNode, fromWalkNodeID);
	if (!fromWalkNodeObj.IsOk()) // ★エラー FromNodeIDのノードが見つからない
	{
		m_output.OutputRegLog(err_level::error, err_code::NotFoundFromNode, ToString(fromWalkNodeID));
	}

	// Toノードの存在確認
	toWalkNodeObj = util::SearchObj(walkNode, toWalkNodeID);
	if (!toWalkNodeObj.IsOk()) // ★エラー ToNodeIDのノードが見つからない
	{
		m_output.OutputRegLog(err_level::error, err_code::NotFoundToNode, ToString(toWalkNodeID));
	}
	if (fromWalkNodeObj.IsOk() && toWalkNodeObj.IsOk()) {
		// From/Toの形状が一致していないか確認
		if (fromWalkNodeObj.IsEqual(toWalkNodeObj.m_ipGeometry)) // ★エラー リンクの始終点が等しい（形状）
			m_output.OutputRegLog(err_level::error, err_code::LoopLinkSingleFT, _T("形状"));

		// 人車共用リンクでFrom/Toのノード種別が両方とも道路NW接続ノードでないか確認
		if (walkLinkClass == walk_class::kWalkableRoad &&
			fromWalkNodeObj.GetLongValue(walkNodeClassIndex) == walk_node::node_class::kRoadNW &&
			toWalkNodeObj.GetLongValue(walkNodeClassIndex) == walk_node::node_class::kRoadNW)
			// ★エラー Fromノード,Toノード共に道路NW接続ノード
			m_output.OutputRegLog(err_level::error, err_code::BothRoadNW);
	}
	return true;
}

// TOD: 名前はちゃんと考える
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
	// 点列に分解して確認
	double cmpX = 0.0, cmpY = 0.0;
	double cmpX2 = 0.0, cmpY2 = 0.0;
	map<pair<double, double>, vector<long>> coordsMap; // キー：xy、値：構成点番号
	for (long i = 0; i < walkLinkPointCnt; ++i) {

		IPointPtr ipPoint = ipWalkLinkPointCol->GetPoint(i);

		// 始点とFromノードの一致を確認
		if (i == 0 && fromNodeObj.IsOk()) {
			if (!fromNodeObj.IsEqual((IGeometryPtr)ipPoint)) // ★エラー リンクの始点とFromノードの座標が一致しない
				m_output.OutputRegLog(err_level::error, err_code::UnMatchStartFrom);
		}

		// 終点とToノードの一致を確認
		if (i == walkLinkPointCnt - 1 && toNodeObj.IsOk()) {
			if (!toNodeObj.IsEqual((IGeometryPtr)ipPoint)) // ★エラー リンクの終点とToノードの座標が一致しない
				m_output.OutputRegLog(err_level::error, err_code::UnMatchEndTo);
		}

		// セグメント距離確認
		double x = 0.0, y = 0.0;
		ipPoint->QueryCoords(&x, &y);
		if (!isForRelease &&  i != 0) {
			double dist = g_cnv.GetDist(cmpX, cmpY, x, y);
			if (dist <= m_dist_threshold) {
				// ★エラー 微小セグメントが存在する
				info1.Format(_T("'%ld'番と'%ld'番"), i - 1, i);
				info2.Format(_T("'%lf'm以下（'%lf'm）"), m_dist_threshold, dist);
				m_output.OutputRegLog(err_level::error, err_code::UnderThresholdSeg, info1, info2);
			}
		}

		// xyをキーとして、構成点番号を詰める
		coordsMap[make_pair(x, y)].push_back(i);

		// 角度確認
		if (!isForRelease && i > 1) {
			double angle = util::angle(cmpX2, cmpY2, cmpX, cmpY, x, y);
			if (angle < 0)
				angle = angle * -1;

			if (angle <= m_angle_threshold) {
				// ★エラー 急激なターンが存在する
				info1.Format(_T("'%ld'番"), i - 1);
				info2.Format(_T("'%lf'°以下（'%lf'°）"), m_angle_threshold, angle);
				m_output.OutputRegLog(err_level::error, err_code::UnderThresholdAg, info1, info2);
			}
		}
		cmpX2 = cmpX;
		cmpY2 = cmpY;
		cmpX = x;
		cmpY = y;
	}

	// 構成点重複の確認
	for (const auto& coord : coordsMap) {
		if (coord.second.size() > 1) {
			// ★エラー 構成点が重複している
			auto itr = coord.second.begin();
			info1.Format(_T("'%ld'番と'%ld'番"), *(itr), *(itr + 1));
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
	// みなし横断歩道が交差している道路リンクが2車線以上
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
		if (maxLaneCount >= 2) //★ワーニング みなし横断歩道が交差している道路リンクが2車線以上
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
	// 階段フラグ、スロープフラグ、エスカレータフラグがついてる場合、階層移動種別が「0：上下無し」以外か確認
	if (!judge_value::IsOKFloorMoveClassAndFlag(floorMoveClass, stairF))
		// ★エラー 階層移動種別が付与されていない（STAIR_F）
		m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveAndF, kStairFlag);

	else if (!judge_value::IsOKFloorMoveClassAndFlag(floorMoveClass, slopeF))
		// ★エラー 階層移動種別が付与されていない（SLOPE_F）
		m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveAndF, kSlopeFlag);

	else if (!judge_value::IsOKFloorMoveClassAndFlag(floorMoveClass, escalatorF))
		// ★エラー 階層移動種別が付与されていない（ESCALATOR_F）
		m_output.OutputRegLog(err_level::error, err_code::NgFloorMoveAndF, kEscalatorFlag);

	// 階段フラグ、スロープフラグ、エスカレータフラグがついていない場合、階層移動種別が「0：上下無し」であるか確認
	int floorMoveFlag = stairF | slopeF | escalatorF;

	if (m_dataType == walk_type::type_class::kDownConvert) floorMoveFlag |= autoWalkF;

	if (!judge_value::IsOKFloorMoveClassAndFlagOFF(floorMoveClass, floorMoveFlag))
		// ★エラー 階層移動種別が付与されている
		m_output.OutputRegLog(err_level::error, err_code::ExistFloorMove);

	// オートウォークフラグ、エスカレータフラグがついてる場合、一方通行種別が付与されいるか確認
	if (!judge_value::IsOKOnewayCodeAndFlag(onewayCode, autoWalkF))
		// ★エラー 一方通行種別が付与されていない（AUTOWALK_F）
		m_output.OutputRegLog(err_level::error, err_code::NotOnewayCode, kAutoWalkFlag);

	else if (!judge_value::IsOKOnewayCodeAndFlag(onewayCode, escalatorF))
		// ★エラー 一方通行種別が付与されていない（ESCALATOR_F）
		m_output.OutputRegLog(err_level::error, err_code::NotOnewayCode, kEscalatorFlag);

	// オートウォークフラグ、エスカレータフラグがついない場合、一方通行種別が付与されていないか確認
	int onwayFlag = autoWalkF | escalatorF;
	if (!judge_value::IsOKFloorMoveClassAndFlagOFF(onewayCode, onwayFlag))
		// ★エラー 一方通行種別が付与されている
		m_output.OutputRegLog(err_level::warning, err_code::ExistOnewayCode);

	// エスカレーターフラグ または オートウォークフラグがONで屋根付きフラグがOFFではないか確認
	if (!judge_value::IsOKMovingWalkwayAndArcade(walkLinkClass, autoWalkF, escalatorF, arcadeF))
		// ★エラー エスカレータ/オートウォークフラグがONだが、屋根付きフラグがOFF
	{
		if (autoWalkF)
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::notice, err_code::NgMovingWalkArcade,
				kAutoWalkFlag, ToString(walkLinkClass));
		else if (escalatorF)
			m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::error : err_level::notice, err_code::NgMovingWalkArcade, kEscalatorFlag, ToString(walkLinkClass));
	}

	// フラグ整備対象外の歩行者種別に対しフラグが整備されていないか確認
	// ★エラー フラグ整備対象外の歩行者種別に対しフラグが整備されている
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

	// 歩行者路線コードに入っているものが、WALK_CODE_LISTに存在するかを確認
	if (walkCode > 0) {
		if (m_walkCode.m_Rows.end() == m_walkCode.m_Rows.find(walkCode))
			// ★エラー 対応する歩行者路線コードデータがない
			m_output.OutputRegLog(err_level::error, err_code::NotFoundWalkCode, ToString(walkCode));

		if (walkLinkClass == walk_class::kWalkableRoad)
			// ★エラー 人車共用リンクに路線コードが整備されている
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
		//★エラー 通行禁止種別コードと関連フラグの整合性がとれていない
		m_output.OutputRegLog(err_level::error, err_code::NgNoPassageRF, ToString(noPassCode), ToString(noPassRF));

	if (FLAG_ON == noPassRF)
	{
		if (m_walkNoPassage.m_Rows.empty() || m_walkNoPassage.m_Rows.end() == m_walkNoPassage.m_Rows.find(walkLinkID))
			//★エラー 通行禁止時間規制関連フラグがONだが、通行禁止規制情報が存在しない
			m_output.OutputRegLog(err_level::error, err_code::NgNotNoPassInfo);
	}

	if (!judge_value::IsOKOnewayCodeAndRelFlag(onewayCode, onewayRF))
		//★エラー 一方種別コードと関連フラグの整合性がとれていない
		m_output.OutputRegLog(err_level::error, err_code::NgOnewayRF, ToString(onewayCode), ToString(onewayRF));

	if (FLAG_ON == onewayRF)
	{
		if (m_walkOneway.m_Rows.empty() || m_walkOneway.m_Rows.end() == m_walkOneway.m_Rows.find(walkLinkID))
			//★エラー 一方通行時間規制関連フラグがONだが、一方通行規制情報が存在しない
			m_output.OutputRegLog(err_level::error, err_code::NgNotOnewayInfo);
	}

	// 人車共用リンクに規制情報が整備されていないか確認
	if (!judge_value::IsOKHumanCarAndRegFlag(walkLinkClass, onewayCode, noPassCode))
		// ★エラー 人車共用リンクに規制が整備されている
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

	// どちらも1個ずつのはず
	auto& fromLink = *(fromLinks.begin());
	auto& toLink = *(toLinks.begin());

	// 一致と見なすフィールドが一致する場合のみ、コードの変化を確認する
	if (judge_value::IsSimpleSameWalkLinkAttr(fromLink, walkLinkObj, walkLink.m_FieldMap)
		&& judge_value::IsSimpleSameWalkLinkAttr(walkLinkObj, toLink, walkLink.m_FieldMap)) {

		if (!walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kWalkCode), fromLink)
			|| !walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kWalkCode), toLink)) {

			// ★エラー 歩行者「路線」コードが前後のリンクで異なる（エラーコード：332）
			info1 = ToString(walkLinkObj.GetLongValue(walkLink.m_FieldMap.FindField(kWalkCode)));
			info2.Format(_T("'F'(%ld)/'T'(%ld)"), fromLink.GetLongValue(walkLink.m_FieldMap.FindField(kWalkCode)),
				toLink.GetLongValue(walkLink.m_FieldMap.FindField(kWalkCode)));
			m_output.OutputRegLog(err_level::error, err_code::DiffWalkCode, info1, info2);
		}
	}

	// bug 11431 - [u][要望]階層移動種別の付与漏れを検出してほしい
	// 歩行者種別コードが屋内、または地下街の場合
	if (walkLinkClass == walk_class::kIndoor
		|| walkLinkClass == walk_class::kUnderGround)
	{
		// 歩行者種別コード、フラグ関連が一致しており、階層移動種別がなしの場合
		if (judge_value::IsSimpleSameWalkLinkAttr(fromLink, walkLinkObj, walkLink.m_FieldMap)
			&& judge_value::IsSimpleSameWalkLinkAttr(walkLinkObj, toLink, walkLink.m_FieldMap)
			&& floorMoveClass == floor_move_class::kNone)
		{
			if (!walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kFloorLevel), fromLink)
				|| !walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kFloorLevel), toLink))
				// ★エラー 階層レベルが前後のリンクで異なる
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
			// ★エラー 広場が閉じていない
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
	// ダウンコンバート向け場合、リンクの歩行者種別が「接続リンク」「ロジカルコネクション」で、
	if (!isValidLink && m_dataType == walk_type::type_class::kDownConvert && walkLinkClass == walk_class::kLogicalConnection)
		isValidLink = true;
	if (isValidLink)
	{
		long fromWalkNodeClass = fromWalkNodeObj.GetLongValue(walkNodeClassIndex);
		long toWalkNodeClass = toWalkNodeObj.GetLongValue(walkNodeClassIndex);

		// ダウンコンバート向け場合、いずれかの接続ノードが「道路NWリンク接続ノード」「道路ノード接続ノード」ではない場合エラー
		if (m_dataType == walk_type::type_class::kDownConvert
			&& fromWalkNodeClass != walk_node::node_class::kRoadNW
			&& toWalkNodeClass != walk_node::node_class::kRoadNW
			&& fromWalkNodeClass != walk_node::node_class::kConnectedRoad
			&& toWalkNodeClass != walk_node::node_class::kConnectedRoad)
			// ★エラー 接続ノードが「道路NWリンク接続ノード」「道路ノード接続ノード」ではない
			m_output.OutputRegLog(err_level::error, err_code::NotConRoadWalkLink, _T(""), _T(""), 0.0, 0.0, false);

		// 汎用向け場合、
		if (m_dataType == walk_type::type_class::kGeneral
			&& fromWalkNodeClass != walk_node::node_class::kRoadNW
			&& toWalkNodeClass != walk_node::node_class::kRoadNW)
		{
			if (!ExistHumanCarLink(fromLinks, walkLink.m_FieldMap) && !ExistHumanCarLink(toLinks, walkLink.m_FieldMap))
				// ★エラー 接続リンクが道路リンク、または人車共用リンクに接続していない
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
	// bug 11433 - [u][要望]並行する形状の場合に属性の矛盾をチェックしてほしい
	// 階層移動種別コードが「0:上下なし」以外の場合

	if (judge_value::IsParalleCheckTargetlLink(walkLinkClass) && floorMoveClass != floor_move_class::kNone)
	{
		auto findWalkLinkIDs = util::getIntersectsIDs(walkLinkObj.GetBox(4), walkLinkRTree);

		std::map<long, long> fromLinkIDList, toLinkIDList;	//from/toの各ノードから3リンク目に到達するリンク群

		// 3番目に到達するリンク群を取得
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

				// 双方向からの探索結果ではない場合
				if (fromLinkID.second == toLinkID.second)
					continue;

				auto compObjectItr = walkLink.m_FeatureMap.find(fromLinkID.first);
				if (compObjectItr == walkLink.m_FeatureMap.end())
					continue;

				long compWalkClass = compObjectItr->second.GetLongValue(walkLink.m_FieldMap.FindField(kWalkClass));
				// 歩行者種別コードが同一でない場合
				if (walkLinkClass != compWalkClass)
					continue;

				long compFloorMoveClass = compObjectItr->second.GetLongValue(walkLink.m_FieldMap.FindField(kFloorMoveClass));
				// 階層移動種別が付与されていない場合
				if (compFloorMoveClass == floor_move_class::kNone) // ★エラー 並行リンクの階層移動種別の方向が異なる
				{
					m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::DiffFloorMoveCode,
						ToString(compObjectItr->first));
					continue;
				}

				// 階層移動種別が付与されている場合
				auto compFromNodeID = compObjectItr->second.GetLongValue(fromNodeIDIndex);
				auto compOnewayCode = compObjectItr->second.GetLongValue(onewayCodeIndex);
				auto compEscalatorF = compObjectItr->second.GetLongValue(escalatorFIndex);

				// 並行としてみて、基準リンクとFrom/Toの方向が逆の場合
				if (compFromNodeID == fromLinkID.second)
				{
					// 階層移動種別が反転していなければ
					if (floorMoveClass == compFloorMoveClass) // ★エラー 並行リンクの階層移動種別の方向が異なる
						m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::DiffFloorMoveCode, ToString(compObjectItr->first));
					// 両リンクともエスカレータの場合
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

				// 並行としてみて、基準リンクとFrom/Toの方向が同じの場合
				// 階層移動種別が同じでなければ
				if (floorMoveClass != compFloorMoveClass) // ★エラー 並行リンクの階層移動種別の方向が異なる
					m_output.OutputRegLog(m_dataType == walk_type::type_class::kGeneral ? err_level::warning : err_level::notice, err_code::DiffFloorMoveCode, ToString(compObjectItr->first));

				// 両リンクともエスカレータの場合
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
	// 地下通路、または横断地下道のときのみチェック。（bug 12000）
	// エレベーターも降りる階層もないのに地下に繋がっている
	if (judge_value::IsUnderGroundLink(walkLinkClass))
	{
		// from側のエラーになるリンク群を取得		// from側のエラーになるリンク群を取得
		auto errLinks = checkFloorMoveConnectUnder(walkLinkID, fromLinks, fromWalkNodeObj, walkNode.m_FieldMap, walkLink);
		{
			auto toErrLinks = checkFloorMoveConnectUnder(walkLinkID, toLinks, toWalkNodeObj, walkNode.m_FieldMap, walkLink);
			// to側のエラーになるリンク群をfrom側のリンク群に結合
			for (long linkId : toErrLinks)
				errLinks.insert(linkId);
		}
		if (errLinks.size() > 0) {
			// カレントデータの出力ではないため、OutputRegLogに移行しきれなかった
			long modOperatorIndex = walkLink.m_FieldMap.FindField(kOperator);
			long modDateIndex = walkLink.m_FieldMap.FindField(kModifyDate);
			long modProgramIndex = walkLink.m_FieldMap.FindField(kModifyProgName);
			long modProgramDateIndex = walkLink.m_FieldMap.FindField(kProgModifyDate);
			for (long linkId : errLinks)
			{
				// ★エラー エレベーターも降りる階層もないのに地下に繋がっている
				/*
				* レビュー用コメント
				* IDとそれに対するmodXXXが一致していなかった分修正
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
	// エラーコード353のチェック
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

		// 2次メッシュ境界を跨ぐリンクに対し、util::getAdjacentLink()で端点ノードIDが変更されるので、
		// 後のチェックに影響しないように、局所的な変数tempNodeIDを使う
		auto tempNodeID = fromWalkNodeID;

		// From側の接続リンクを取得
		auto fromWalkLinks = util::GetObjectMap(walkLinkID, fromLinkIDSetItr->second, walkLink.m_FeatureMap);
		std::set<long> adjFromLinkIds = util::getAdjacentLink(tempNodeID, fromWalkLinks, walkLink, walkNode, walkNodeLinkIDs, true);

		if (adjFromLinkIds.empty())
		{
			adjFromLinkIds = getAdjacentWalkLink(fromWalkNodeID, walkLinkID, ipMeshGeo,
				tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs);
		}

		// To側の接続リンクを取得
		tempNodeID = toWalkNodeID;
		auto toWalkLinks = util::GetObjectMap(walkLinkID, toLinkIDSetItr->second, walkLink.m_FeatureMap);
		std::set<long> adjToLinkIds = util::getAdjacentLink(tempNodeID, toWalkLinks, walkLink, walkNode, walkNodeLinkIDs, true);

		if (adjToLinkIds.empty())
		{
			adjToLinkIds = getAdjacentWalkLink(toWalkNodeID, walkLinkID, ipMeshGeo,
				tempWalkLink, tempWalkNode, tempWalkNodeLinkIDs);
		}

		// 2差路のみ、チェック対象となる（FromかTo側の接続リンクが存在しない、或いは複数存在する場合、チェック対象外）
		if (adjFromLinkIds.size() == 1 && adjToLinkIds.size() == 1)
		{
			auto& fromWalkLink = tempWalkLink.m_FeatureMap.at(*(adjFromLinkIds.begin()));
			auto& toWalkLink = tempWalkLink.m_FeatureMap.at(*(adjToLinkIds.begin()));

			// 前後のリンクが同じ歩行者種別で、かつ、基準リンクが、それら歩行者種別とは異なる種別の場合、エラーとする
			if ((fromWalkLink.CompareAt(walkLink.m_FieldMap.FindField(kWalkClass), toWalkLink))
				&& (!walkLinkObj.CompareAt(walkLink.m_FieldMap.FindField(kWalkClass), toWalkLink)))
			{
				// ★エラー 歩行者「種別」コードが前後のリンクで異なる（エラーコード：353）
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
	// 同一From/Toが存在しないか確認する
	for (const auto& dupID : dupNodeMap) {
		if (dupID.second.size() > 1) {
			// ★エラー 同一From/Toを持つリンクが複数存在
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
				//ここに来ることがあるか不明だし、意味が変わってくるけど、元プログラム互換のため追加
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

	// チェック済みなら見ない
	auto result = inspectedIDs.insert(nodeObj.m_ObjectID);
	if (!result.second)
		return std::list<GeoClass>();

	auto linkIDSetItr = walkNodeLinkIDs.find(nodeObj.m_ObjectID);
	if (linkIDSetItr == walkNodeLinkIDs.end()) return resultList;

	auto links = util::GetObjects(linkObj.m_ObjectID, linkIDSetItr->second, walkLink.m_FeatureMap);
	if (node_class::kOutline == nodeObj.GetLongValue(nodeClassIndex)) {
		//図郭上ノードだったらその先を探す。先も図郭上だったら更に先を
		auto link = *links.begin();
		//図郭上ノードから伸びているリンクの反対側ノードで再検索
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
				//横断歩道でも人車共用でもないのが接続していたら、そこでストップ
				doNextSearch = false; // フラグが立っているかもしれないので、強制的に消す
				break;
			}
		}
		// 人車共用が刺さっていたら、先の横断歩道を探す
		if (doNextSearch) {
			for (auto linkID : crossRoadIDs) {
				auto link = walkLink.m_FeatureMap.at(linkID);
				long id = anotherNodeID(link, nodeObj.m_ObjectID, fromIdIndex, toIdIndex);
				auto nextNodeObj = walkNode.m_FeatureMap.at(id);
				resultList.splice(resultList.end(), searchCrossRoadNode(link, nextNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex, walkLink, walkNode, walkNodeLinkIDs, inspectedIDs));
			}
		}
		else {
			// 横断歩道だけなので、そこでストップ
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
	std::set<long> inspectedIDs;	// 検査済みのノードIDセット
	//from側のノードID取得
	auto targetNodes = searchCrossRoadNode(linkObj, fromWalkNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex,
		walkLink, walkNode, walkNodeLinkIDs, inspectedIDs);


	//to側のノードを取得して追加
	targetNodes.splice(targetNodes.end(), searchCrossRoadNode(linkObj, toWalkNodeObj, nodeClassIndex, walkClassIndex, fromIdIndex, toIdIndex,
		walkLink, walkNode, walkNodeLinkIDs, inspectedIDs));
	long roadNodeIdIndex = walkNode.m_FieldMap.FindField(kRoadNodeID);
	if (targetNodes.size() < 2) {
		//横断歩道の両端ノードが特定できない
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
				baseID = node.m_ObjectID;	//一個で十分だけど
			}
		}
	}
	// 道路ノードIDを持っているものが０個はOK（仕方ない）
	// １つでもあったら、全部にないとNG
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
		// 改札ノードだったら、無条件OK（改札内の改札もある）
		// 端点ノードだったら、先にリンクがあったらNGだけど他でチェックしてるからOK
		return true;
	}
	// 端点ノードでも改札ノーでもなかったら、その先には改札内リンクだけでないとNG
	for (auto link : links) {
		if (!judge_value::IsInternalTicketGateLink(link.GetLongValue(walkClassIndex))) {
			// 改札内リンクが改札ノード以外で改札外リンクに接続している
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

// スクランブルフラグが付与されているのにスクランブルエリア内に無いかチェック
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

// スクランブルエリア内なのにスクランブルフラグが付与されていないかチェック
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

// 車椅子通行コードが不正な値かチェック
void WalkLinkCheckFunction::checkWheelChair(
	long  walkWheelChair
) const
{
	using namespace walk_link;

	if (walkWheelChair != wheel_chair::kUnInvestigated && walkWheelChair != wheel_chair::kPassable && walkWheelChair != wheel_chair::kNoPassable)
		m_output.OutputRegLog(err_level::fatal, err_code::NgWalkLinkWheelChair);
}

// のりば番号が不正な値かチェック
void WalkLinkCheckFunction::checkPlatformNumber(
	long  walkPlatformNumber
) const
{
	if (walkPlatformNumber < 0 || walkPlatformNumber > 99)
		m_output.OutputRegLog(err_level::fatal, err_code::NgWalkLinkPlatformNumber);
}

// 微小ロジカルコネクションかチェック
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

// ロジカルコネクションと交差しているかチェック
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
