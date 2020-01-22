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
	// ���H�����N�|�Ԑ������N�֘A�e�[�u���̃t�B�[���h�}�b�v���擾����
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

	// �Ԑ������NGID���X�g���擾
	std::list<CString> laneGids;	//!< �����p
	std::map<CString, IFeaturePtr> laneLinkMap;	//!< ���H�����N�Ƃ̑Ή��쐬�p
	for (const auto& link : lane_links) {
		CComVariant gid = AheGetAttribute(link, lane_link::kGlobalID);
		if (gid.vt == VT_NULL)
			continue;
		laneGids.push_back(gid.bstrVal);
		laneLinkMap[gid.bstrVal] = link;
	}

	// �Ԑ������NGID���X�g����A"�Ԑ������N�Ɠ��H�����N��ID�Ή��\"��"���H�����NID���X�g"���擾
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

	// ���H�����NID���X�g���瓹�H�����N�t�B�[�`���Q��ǂݍ���
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

	// "�Ԑ������N�Ɠ��H�����N��ID�Ή��\���g�p���ĕR�t���֌W���\�z���A
	// ���R�[�h���R���e�i�֊i�[����
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

	// �Ԑ������N�Ɠ��H�����N�̕R�t����1�̂Ƃ��͂����Ԃ�
	if (record.m_roadLinks.size() == 1) {
		lane_link = record.m_laneLink;
		road_link = record.m_roadLinks.front();
		return isInaccuracyPair(lane_link) ? kInaccurate : kAccurate;
	}

	// �Ԑ������N�Ɠ��H�����N�̕R�t������������ꍇ�́A
	// �D��x���ɕ��ׁA�D��x���ł��������̂�1�ł���΂����Ԃ�
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

	// �ł��D��x�̍����Ԑ������N�Ɠ��H�����N�̕R�t������������ꍇ�́A
	// �S�ē����ō����x�R�[�h�ł͂Ȃ������m�F����
	// �S�ē����ł���΁A�ǂ�ł��悢�̂ł��̂�����1��Ԃ�
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
