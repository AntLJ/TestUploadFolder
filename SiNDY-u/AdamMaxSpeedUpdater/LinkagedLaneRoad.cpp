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

#include <TDC/useful_headers/str_util.h>
#include <ArcHelperEx/ArcHelperEx.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/adamlib/AdamGlobals.h>
#include "LinkagedLaneRoad.h"

using namespace sindy::schema;
using namespace adam_v2;

LinkagedLaneRoad::LinkagedLaneRoad(IFeatureClassPtr lane_fc, IFeatureClassPtr road_fc, ITablePtr rel_road_lane ) : 
	m_roadLinkFc(road_fc), 
	m_relRoadLaneT(rel_road_lane)
{
	// 道路リンク－車線リンク関連テーブルのフィールドマップを取得する
	IFieldsPtr relFields = m_relRoadLaneT->GetFields();
	m_relRoadLaneFM.CreateFieldMap(relFields);

	IFieldsPtr laneFields = lane_fc->GetFields();
	m_lanelinkFM.CreateFieldMap(laneFields);

	IFieldsPtr roadFields = m_roadLinkFc->GetFields();
	m_roadlinkFM.CreateFieldMap(roadFields);
}

bool LinkagedLaneRoad::read(const std::vector<IFeaturePtr>& lane_links)
{
	using namespace rel_road_link_lane_link;

	if (lane_links.empty())
		return true;

	// 車線リンクGIDリストを取得
	std::list<CString> laneGids;	//!< 検索用
	std::map<CString, IFeaturePtr> laneLinkMap;	//!< 道路リンクとの対応作成用
	for (const auto& link : lane_links) {
		CComVariant gid = AheGetAttribute(link, lane_link::kGlobalID);
		if (gid.vt == VT_NULL)
			continue;
		laneGids.push_back(gid.bstrVal);
		laneLinkMap[gid.bstrVal] = link;
	}

	// 車線リンクGIDリストから、"車線リンクと道路リンクのID対応表"と"道路リンクIDリスト"を取得
	std::multimap<CString, long> laneToRoadMap;
	std::list<long> roadOids;
	std::list<CString> inClauses;
	sindy::CTableBase::IDs2Str(laneGids, inClauses, _T("'"));
	for (const auto& inClause : inClauses)
	{
		static const CString subFields = uh::str_util::format(_T("%s,%s"), 
			rel_road_link_lane_link::kLaneLinkGID,
			rel_road_link_lane_link::kRoadLinkID);
		CString whereClause = uh::str_util::format(_T("%s in (%s)"), rel_road_link_lane_link::kLaneLinkGID, inClause);

		_ICursorPtr cur;
		if (FAILED(m_relRoadLaneT->Search(AheInitQueryFilter(nullptr, subFields, whereClause), VARIANT_TRUE, &cur)))
			return false;
		if (!cur)
			return false;

		_IRowPtr row;
		while (cur->NextRow(&row) == S_OK && row)
		{
			CString laneGid = row->GetValue(m_relRoadLaneFM.FindField(kLaneLinkGID));
			long roadOid = row->GetValue(m_relRoadLaneFM.FindField(kRoadLinkID));
			laneToRoadMap.insert(std::make_pair(laneGid, roadOid));
			roadOids.push_back(roadOid);
		}
	}

	// 道路リンクIDリストから道路リンクフィーチャ群を読み込み
	std::map<long, IFeaturePtr> roadLinkMap;
	roadOids.sort();
	roadOids.erase(std::unique(roadOids.begin(), roadOids.end()), roadOids.end());
	inClauses.clear();
	sindy::CTableBase::IDs2Str(roadOids, inClauses);
	for (const auto& inClause : inClauses)
	{
		CString whereClause = uh::str_util::format(_T("%s in (%s)"), road_link::kObjectID, inClause);

		IFeatureCursorPtr cur;
		if (FAILED(m_roadLinkFc->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &cur)))
			return false;
		if (!cur)
			return false;

		IFeaturePtr feature;
		while (cur->NextFeature(&feature) == S_OK && feature) {
			long oid = feature->GetOID();
			roadLinkMap[oid] = feature;
		}
	}

	// "車線リンクと道路リンクのID対応表を使用して紐付け関係を構築し、
	// レコードをコンテナへ格納する
	for (const auto& laneLink : laneLinkMap)
	{
		CString gid = laneLink.first;
		IFeaturePtr feature = laneLink.second;

		Record record;
		record.m_laneLink = feature;
		auto range = laneToRoadMap.equal_range(gid);
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			long oid = iter->second;
			record.m_roadLinks.push_back(roadLinkMap[oid]);
		}
		m_data.push_back(record);
	}

	return true;
}

LinkageAccuracy LinkagedLaneRoad::getHigherPriorityPairLink(const Record& record, IFeaturePtr& lane_link, IFeaturePtr& road_link) const
{
	if (record.m_roadLinks.empty()) {
		lane_link = record.m_laneLink;
		return kNoPair;
	}

	// 車線リンクと道路リンクの紐付きが1つのときはそれを返す
	if (record.m_roadLinks.size() == 1) {
		lane_link = record.m_laneLink;
		road_link = record.m_roadLinks.front();
		return isInaccuracyPair(lane_link) ? kInaccurate : kAccurate;
	}

	// 車線リンクと道路リンクの紐付きが複数ある場合は、
	// 優先度順に並べ、優先度が最も高いものが1つであればそれを返す
	std::multimap<long, IFeaturePtr> prioritizedLinks;
	for (auto link : record.m_roadLinks) {
		long priority = getPriority(record.m_laneLink, link);
		prioritizedLinks.emplace(priority, link);
	}
	long highestPriority = prioritizedLinks.begin()->first;
	if (prioritizedLinks.count(highestPriority) == 1)
	{
		lane_link = record.m_laneLink;
		road_link = prioritizedLinks.begin()->second;
		return isInaccuracyPair(lane_link) ? kInaccurate : kMostlyAccurate;
	}

	// 最も優先度の高い車線リンクと道路リンクの紐付きが複数ある場合は、
	// 全て同じ最高速度コードではないかを確認する
	// 全て同じであれば、どれでもよいのでそのうちの1つを返す
	auto rangePair = prioritizedLinks.equal_range(highestPriority);
	long idxLegalSpeed = m_roadlinkFM.FindField(road_link::kLegalSpeed);
	std::map<long, IFeaturePtr> maxSpeedAndFeature;
	for (auto iter = rangePair.first; iter != rangePair.second; ++iter)
	{
		IFeaturePtr roadLink = iter->second;
		long legalSpeed = roadLink->GetValue(idxLegalSpeed).lVal;
		if (maxSpeedAndFeature.count(legalSpeed) == 0) {
			maxSpeedAndFeature.emplace(legalSpeed, roadLink);
		}
	}
	if (maxSpeedAndFeature.size() == 1) {
		lane_link = record.m_laneLink;
		road_link = prioritizedLinks.begin()->second;
		return isInaccuracyPair(lane_link) ? kInaccurate : kMostlyAccurate;
	}

	lane_link = record.m_laneLink;
	return kNoPair;
}

long LinkagedLaneRoad::getPriority(IFeaturePtr lane_link, IFeaturePtr road_link) const
{
	using namespace lane_link;
	using namespace road_link;

	long idxRoadStruct = m_lanelinkFM.FindField(kRoadStructureC);
	long idxRoadLinkC = m_roadlinkFM.FindField(kMainLinkClass);

	long roadStruct = lane_link->GetValue(idxRoadStruct).lVal;
	long roadLinkC = road_link->GetValue(idxRoadLinkC).lVal;
	if (roadStruct == road_structure_code::kBranchConfluence && roadLinkC == main_link_class::kMainUpDown)
		return 1;
	else if (roadStruct == road_structure_code::kBranchConfluence && roadLinkC == main_link_class::kConnect)
		return 2;
	else if (roadStruct == road_structure_code::kBranchConfluence && roadLinkC == main_link_class::kRamp)
		return 3;
	else
		return 4;

	return false;
}

bool LinkagedLaneRoad::isInaccuracyPair(IFeaturePtr lane_link) const
{
	using namespace lane_link;

	long idxRoadStruct = m_lanelinkFM.FindField(kRoadStructureC);
	long roadStruct = lane_link->GetValue(idxRoadStruct).lVal;

	return roadStruct == road_structure_code::kBranchConfluence;
}
