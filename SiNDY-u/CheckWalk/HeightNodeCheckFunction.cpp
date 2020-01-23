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

	// 重なりリンク高さノードのチェック
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

	// ★エラー HEIGHT_NODEの形状が不正
	if (m_dataType == walk_type::type_class::kDownConvert)
		checkHeightNodeGeometry(heightNode.m_ipTable);

	for (const auto& heightNodeTmp : heightNode.m_FeatureMap)
	{

		auto& heightNodeObj = heightNodeTmp.second;

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
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


		// リンク同士のチェック
		if (link1.linkID == link2.linkID) // ★エラー 自己交差箇所に付与されている
			m_output.OutputRegLog(err_level::error, err_code::NgSelhCross,
				ToString(link1.linkID), ToString(link2.linkID));

		if (link1.linkIDTable == sindy::schema::height_node::link_table::kRoadLink &&
			link2.linkIDTable == sindy::schema::height_node::link_table::kRoadLink)
			// ★エラー 道路リンク同士の相対高さ
			m_output.OutputRegLog(err_level::error, err_code::NgBothRoadLink,
				ToString(link1.linkID), ToString(link2.linkID));

		if (link1.linkIDHeight == link2.linkIDHeight) // ★エラー 相対高さが同じ
		{
			// いずれか片方がROAD_LINKの場合
			if (link1.linkIDTable == sindy::schema::height_node::link_table::kRoadLink ||
				link2.linkIDTable == sindy::schema::height_node::link_table::kRoadLink)
			{
				// 0-0はエラー対象外
				if (link1.linkIDHeight != 0)
					m_output.OutputRegLog(err_level::error, err_code::NgHeightEqual,
						ToString(link1.linkID), ToString(link2.linkID));
			}
			else
				m_output.OutputRegLog(err_level::error, err_code::NgHeightEqual,
					ToString(link1.linkID), ToString(link2.linkID));
		}
		else if (!isRelease) //( Releaseモード：チェック対象外 )
		{
			// 片方が 2 または -2の時 相対高さの差が開きすぎていないか
			if (abs(link1.linkIDHeight) == 2 || abs(link2.linkIDHeight) == 2)
				if (abs(link1.linkIDHeight - link2.linkIDHeight) > 1) // ★エラー 相対高さの差が大きい
					m_output.OutputRegLog(err_level::error, err_code::LargeDifference,
						ToString(link1.linkIDHeight), ToString(link2.linkIDHeight));
		}

		// HEIGHT_NODE付近に道路ノード、または歩行者ノードが存在するか
		set<long> nearWalkNodes = util::getIntersectsIDs(heightNodeObj.GetBox(m_dist_threshold), walkNodeRTree);
		set<long> nearRoadNodes = util::getIntersectsIDs(heightNodeObj.GetBox(m_dist_threshold), roadNodeRTree);
		if (!nearWalkNodes.empty() || !nearRoadNodes.empty()) // ★エラー ポイント付近にノードが存在する
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

		// HEIGHT_NODE付近に別のHEIGHT_NODEが存在するか
		set<long> nearHeightNodes = util::getIntersectsIDs(heightNodeObj.GetBox(m_dist_threshold), heightNodeRTree);
		if (nearHeightNodes.size() > 1) // ★エラー ポイント付近に別のHEIGT_NODEが存在する
		{
			for (auto& heightNodeID : nearHeightNodes)
			{
				if (objectId == heightNodeID)
					continue;

				m_output.OutputRegLog(err_level::error, err_code::NearHeightNode,
					uh::str_util::ToString(heightNodeID));
			}
		}

		// リンク単位でのチェック
		for (const auto& link : heightNodeInfo)
		{
			if (!isRelease)
			{
				if (!judge_value::IsDefineHeight(link.linkIDHeight)) // ★エラー 相対高さが範囲外
					m_output.OutputRegLog(err_level::error, err_code::NgHeight, ToString(link.linkIDHeight));
			}

			if (!judge_value::IsDefineLinkTable(link.linkIDTable)) // ★エラー リンクテーブルが定義外
				m_output.OutputRegLog(err_level::error, err_code::NgLinkTable1, ToString(link.linkIDTable));

			if (!judge_value::IsOKLinkTable(link.linkIDTable)) // ★エラー リンクテーブルが整備対象外
				m_output.OutputRegLog(err_level::error, err_code::NgLinkTable2, ToString(link.linkIDTable));


			GeoClass geoClass;
			// リンクの存在チェック
			if (link.linkIDTable == sindy::schema::height_node::link_table::kWalkLink)
			{
				auto linkIte = walkLink.m_FeatureMap.find(link.linkID);
				if (linkIte == walkLink.m_FeatureMap.end()) // ★エラー リンクIDがリンクデータに存在しない
				{
					m_output.OutputRegLog(err_level::error, err_code::NoExistLink,
						ToString(link.linkID), ToString(link.linkIDTable));
					continue;
				}
				geoClass = linkIte->second;

				// 歩行者リンクの場合、歩行者種別コードをチェック(Releaseモード：チェック対象外)
				if (!isRelease)
				{
					long walkClass = geoClass.GetLongValue(walkClassIndex);
					if (!judge_value::IsOKHeightNodeWalkCode(walkClass, m_dataType)) // ★エラー 歩行者種別が相対高さ付与対象外
					{
						// 汎用向け場合、ログ出力
						if (m_dataType == walk_type::type_class::kGeneral)
							m_output.OutputRegLog(err_level::error, err_code::NgHNWalkClass,
								ToString(link.linkID), ToString(link.linkIDTable));
						// ダウンコンバート向け場合、ログ出力
						if (m_dataType == walk_type::type_class::kDownConvert)
							m_output.OutputRegLog(err_level::error, err_code::NgHNWalkClass,
								ToString(link.linkID), ToString(link.linkIDTable), 0.0, 0.0, false);
					}
					else
					{
						int iniValue = m_heightNodeMgr.getWalkHeight(walkClass);
						if (!judge_value::IsOKCompareHeightRef(link.linkIDHeight, iniValue))
							// ★エラー 歩行者種別に対する相対高さが範囲外
							m_output.OutputRegLog(err_level::warning, err_code::NgHeightWalk,
								ToString(link.linkID), ToString(link.linkIDTable));
					}
				}
			}
			else if (link.linkIDTable == sindy::schema::height_node::link_table::kRoadLink)
			{
				auto linkIte = roadLink.m_FeatureMap.find(link.linkID);
				if (linkIte == roadLink.m_FeatureMap.end()) // ★エラー リンクIDがリンクデータに存在しない
				{
					m_output.OutputRegLog(err_level::error, err_code::NoExistLink,
						ToString(link.linkID), ToString(link.linkIDTable));
					continue;
				}
				geoClass = linkIte->second;

				// 道路リンクの場合、リンク内属性リンクをチェック(Releaseモード：チェック対象外)
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
								// ★エラー リンク内属性に対する相対高さが範囲外
								m_output.OutputRegLog(err_level::warning, err_code::NgHeightRoad,
									ToString(link.linkID), ToString(link.linkIDTable));
						}
					}
				}
			}
			else
				continue;

			// 道路リンク、または歩行者リンク上にHEIGHT_NODEがポイントされているか
			if (geoClass.m_ipGeometry)
			{
				if (AheIsDisjoint(geoClass.m_ipGeometry, heightNodeObj.m_ipGeometry))
				{
					// ★エラー ポイントがリンク上にない
					m_output.OutputRegLog(err_level::error, err_code::NoExistPointOnLink,
						ToString(link.linkID), ToString(link.linkIDTable));
				}
			}
			else
			{
				m_output.OutputRegLog(err_level::error, err_code::GetRoadFail);
			}

			// HEIGHT_NODE付近にリンクの構成点が存在する。
			vector<IPointPtr> segmentPoints;
			if (!util::getLine2Points(geoClass.m_ipGeometry, segmentPoints))
				m_output.OutputRegLog(err_level::error, err_code::GetRoadFail);
			else {
				// 両端以外の構成点が存在する場合（ノードは別の箇所でチェックしているため）
				if (segmentPoints.size() > 2)
				{
					for (auto& segmentPoint : segmentPoints)
					{
						double dist = util::GetDistGeometries(heightNodeObj.m_ipGeometry, segmentPoint);
						if (0 < dist && dist < m_dist_threshold)
							// ★エラー ポイント付近にリンクID1またはリンクID2の構成点が存在する
							m_output.OutputRegLog(err_level::error, err_code::NearSegment,
								ToString(link.linkID), ToString(link.linkIDTable));
					}
				}
			}
		}

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ★エラー リンクIDが不正な値
		checkLinkID(link1.linkID, link2.linkID);

		if(checktarget == check_target::all || checktarget == check_target::release)
		{
			// ★エラー ポイント同士HEIGHT_NODEの距離が近い
			checkNearPoint( heightNodeObj, heightNode, heightNodeRTree );
		}
	}
}

// otherモード用の全チェック
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

		// 処理対象メッシュ内（枠上含む）じゃないものはスキップ
		if (heightNodeObj.IsDisjoint(ipMeshGeo))
			continue;

		long link1ID = heightNodeObj.GetLongValue(linkID1Index);
		long link2ID = heightNodeObj.GetLongValue(linkID2Index);

		if (m_dataType != walk_type::type_class::kDownConvert) continue;

		// ★エラー リンクIDが不正な値
		checkLinkID(link1ID, link2ID);
	}
}

// HEIGHT_NODEの形状が不正かチェック
void HeightNodeCheckFunction::checkHeightNodeGeometry(const ITablePtr& ipTable) const
{
	// 全件取得
	IFeatureCursorPtr ipCursor = ipTable->_Search(NULL, VARIANT_FALSE);
	if (!ipCursor) return;

	IFeaturePtr ipFeature;
	// オブジェクトごとの処理
	while (S_OK == ipCursor->NextFeature(&ipFeature) && ipFeature) {
		IGeometryPtr ipGeometry = ipFeature->GetShape();
		if( !ipGeometry)
		{
			// ★エラー 形状が存在しない
			m_output.OutputRegLog( err_level::fatal, err_code::NgHeightNodeGeometry);
			return;
		}

		esriGeometryType eType = ipGeometry->GetGeometryType();
		if( eType != esriGeometryPoint )
		{
			// ★エラー ポイント型ではない
			m_output.OutputRegLog( err_level::fatal, err_code::NgHeightNodeGeometry);
			return;
		}
	}
}

// リンクIDが不正な値かチェック
void HeightNodeCheckFunction::checkLinkID(
	long  link1ID,
	long  link2ID
) const
{
	if (link1ID <= 0 || link2ID <= 0)
		m_output.OutputRegLog(err_level::fatal, err_code::NgHeightNodeLinkID);
}

// 例外モード用のチェック
void HeightNodeCheckFunction::checkException(
	const FeaturesPack& heightNode
)
{
	if (m_dataType != walk_type::type_class::kDownConvert) return;

	// ★エラー HEIGHT_NODEの形状が不正
	checkHeightNodeGeometry(heightNode.m_ipTable);
}

// ポイント同士HEIGHT_NODEの距離が近いかチェック
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
