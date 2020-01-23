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
#include "WalkOnewayCheckFunction.h"
#include "value.h"

using namespace sindy::schema;

void WalkOnewayCheckFunction::checkWalkOneway(
	const IGeometryPtr& ipMeshGeo,
	const std::map<long, GeoClass>& walkLink,
	const MultiRowsPack& walkOneway,
	const MultiRowsPack& walkNoPassage)
{
	using namespace sindy::schema::walk_oneway;
	using namespace uh::str_util;
	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	long linkID = -1;
	std::vector< std::pair< _IRowPtr, CTimeReg > > timeRegList;
	long start_monthIndex = walkOneway.m_FieldMap.FindField(kStartMonth);
	long start_dayIndex = walkOneway.m_FieldMap.FindField(kStartDay);
	long start_hourIndex = walkOneway.m_FieldMap.FindField(kStartHour);
	long start_minIndex = walkOneway.m_FieldMap.FindField(kStartMin);
	long end_monthIndex = walkOneway.m_FieldMap.FindField(kEndMonth);
	long end_dayIndex = walkOneway.m_FieldMap.FindField(kEndDay);
	long end_hourIndex = walkOneway.m_FieldMap.FindField(kEndHour);
	long end_minIndex = walkOneway.m_FieldMap.FindField(kEndMin);
	long dayOfweekIndex = walkOneway.m_FieldMap.FindField(kDayOfWeek);

	auto modOperatorIndex = walkOneway.m_FieldMap.FindField(kOperator);
	auto modDateIndex = walkOneway.m_FieldMap.FindField(kModifyDate);
	auto modProgramIndex = walkOneway.m_FieldMap.FindField(kModifyProgName);
	auto modProgramDateIndex = walkOneway.m_FieldMap.FindField(kProgModifyDate);
	for (const auto& walkOnewayPair : walkOneway.m_Rows)
	{
		auto& walkOnewayObj = walkOnewayPair.second;
		long objectId = walkOnewayObj->GetOID();
		long start_month = walkOnewayObj->GetValue(start_monthIndex).lVal;
		long start_day = walkOnewayObj->GetValue(start_dayIndex).lVal;
		long start_hour = walkOnewayObj->GetValue(start_hourIndex).lVal;
		long start_min = walkOnewayObj->GetValue(start_minIndex).lVal;
		long end_month = walkOnewayObj->GetValue(end_monthIndex).lVal;
		long end_day = walkOnewayObj->GetValue(end_dayIndex).lVal;
		long end_hour = walkOnewayObj->GetValue(end_hourIndex).lVal;
		long end_min = walkOnewayObj->GetValue(end_minIndex).lVal;
		long dayOfweek = walkOnewayObj->GetValue(dayOfweekIndex).lVal;

		auto walkLinkIte = walkLink.find(walkOnewayPair.first);
		if (walkLinkIte == walkLink.end())
			continue;
		if (walkLinkIte->second.IsDisjoint(ipMeshGeo))
			continue;
		m_output.SetModifyInfo(walkOnewayPair.first, ToString(walkOnewayObj->GetValue(modOperatorIndex)),
			ToString(walkOnewayObj->GetValue(modDateIndex)), ToString(walkOnewayObj->GetValue(modProgramIndex)),
			ToString(walkOnewayObj->GetValue(modProgramDateIndex)));

		CTimeReg timeReg = CTimeReg(start_month, start_day, start_hour, start_min, end_month, end_day, end_hour, end_min, dayOfweek);
		if (!timeReg.check()) //★エラー 一方通行の規制条件が不正な値
			m_output.OutputRegLog(err_level::error, err_code::NgOnewayTime, ToString(objectId));
		if (!timeReg.checkAllDay()) //★エラー 規制条件の通年が1/1～12/31になっていない
			m_output.OutputRegLog(err_level::error, err_code::NgOnewayFullYear, ToString(objectId));

		if (linkID != walkOnewayPair.first)
		{
			if (linkID != -1)
				checkTimeRegulationRel(linkID, timeRegList, walkOneway.m_FieldMap, err_code::SameOnewayTime, err_code::OverLapOnewayTime);

			linkID = walkOnewayPair.first;
			timeRegList.clear();
			// 同一リンクIDに紐づく通行禁止規制の情報を取得、追加
			std::vector< std::pair< _IRowPtr, CTimeReg > > noPassageTimeRegList;
			createTimeRegList(linkID, walkNoPassage, noPassageTimeRegList);
			timeRegList.insert(timeRegList.end(), noPassageTimeRegList.begin(), noPassageTimeRegList.end());
		}
		timeRegList.push_back(std::pair<_IRowPtr, CTimeReg>(walkOnewayObj, timeReg));
	}

	checkTimeRegulationRel(linkID, timeRegList, walkOneway.m_FieldMap, err_code::SameOnewayTime, err_code::OverLapOnewayTime);
}


void WalkOnewayCheckFunction::checkRelease(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink,
	const MultiRowsPack& walkOneway)
{
	using namespace sindy::schema::walk_oneway;
	using namespace uh::str_util;

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);
	long start_monthIndex = walkOneway.m_FieldMap.FindField(kStartMonth);
	long start_dayIndex = walkOneway.m_FieldMap.FindField(kStartDay);
	long start_hourIndex = walkOneway.m_FieldMap.FindField(kStartHour);
	long start_minIndex = walkOneway.m_FieldMap.FindField(kStartMin);
	long end_monthIndex = walkOneway.m_FieldMap.FindField(kEndMonth);
	long end_dayIndex = walkOneway.m_FieldMap.FindField(kEndDay);
	long end_hourIndex = walkOneway.m_FieldMap.FindField(kEndHour);
	long end_minIndex = walkOneway.m_FieldMap.FindField(kEndMin);
	long dayOfweekIndex = walkOneway.m_FieldMap.FindField(kDayOfWeek);

	auto modOperatorIndex = walkOneway.m_FieldMap.FindField(kOperator);
	auto modDateIndex = walkOneway.m_FieldMap.FindField(kModifyDate);
	auto modProgramIndex = walkOneway.m_FieldMap.FindField(kModifyProgName);
	auto modProgramDateIndex = walkOneway.m_FieldMap.FindField(kProgModifyDate);
	for (const auto& walkOnewayPair : walkOneway.m_Rows)
	{
		auto& walkOnewayObj = walkOnewayPair.second;
		long objectId = walkOnewayObj->GetOID();
		long start_month = walkOnewayObj->GetValue(start_monthIndex).lVal;
		long start_day = walkOnewayObj->GetValue(start_dayIndex).lVal;
		long start_hour = walkOnewayObj->GetValue(start_hourIndex).lVal;
		long start_min = walkOnewayObj->GetValue(start_minIndex).lVal;
		long end_month = walkOnewayObj->GetValue(end_monthIndex).lVal;
		long end_day = walkOnewayObj->GetValue(end_dayIndex).lVal;
		long end_hour = walkOnewayObj->GetValue(end_hourIndex).lVal;
		long end_min = walkOnewayObj->GetValue(end_minIndex).lVal;
		long dayOfweek = walkOnewayObj->GetValue(dayOfweekIndex).lVal;

		auto walkLinkIte = walkLink.m_FeatureMap.find(walkOnewayPair.first);
		if (walkLinkIte == walkLink.m_FeatureMap.end())
			continue;
		if (walkLinkIte->second.IsDisjoint(ipMeshGeo))
			continue;
		m_output.SetModifyInfo(walkOnewayPair.first, ToString(walkOnewayObj->GetValue(modOperatorIndex)),
			ToString(walkOnewayObj->GetValue(modDateIndex)), ToString(walkOnewayObj->GetValue(modProgramIndex)),
			ToString(walkOnewayObj->GetValue(modProgramDateIndex)));

		CTimeReg timeReg = CTimeReg(start_month, start_day, start_hour, start_min, end_month, end_day, end_hour, end_min, dayOfweek);
		if (!timeReg.check()) //★エラー 一方通行の規制条件が不正な値
			m_output.OutputRegLog(err_level::error, err_code::NgOnewayTime, ToString(objectId));

		if (!timeReg.checkAllDay()) //★エラー 規制条件の通年が1/1～12/31になっていない
			m_output.OutputRegLog(err_level::error, err_code::NgOnewayFullYear, ToString(objectId));
	}
}

void WalkOnewayCheckFunction::checkTimeRegulationRel(
	long linkID,
	const std::vector<std::pair<_IRowPtr, CTimeReg>>& timeRegList,
	const sindy::CFieldMap& fieldMap,
	const err_code::ECode sameTimeErrCode, const err_code::ECode overlapTimeErrCode)
{
	using namespace uh::str_util;
	// リストが0、または1の場合リターン
	if (timeRegList.empty() || timeRegList.size() == 1) return;
	auto modOperatorIndex = fieldMap.FindField(ipc_table::kOperator);
	auto modDateIndex = fieldMap.FindField(ipc_table::kModifyDate);
	auto modProgramIndex = fieldMap.FindField(ipc_table::kModifyProgName);
	auto modProgramDateIndex = fieldMap.FindField(ipc_table::kProgModifyDate);
	for (unsigned int i = 0; i < timeRegList.size() - 1; ++i)
	{
		auto objectId = timeRegList.at(i).first->GetOID();
		m_output.SetModifyInfo(linkID,
			ToString(timeRegList.at(i).first->GetValue(modOperatorIndex)),
			ToString(timeRegList.at(i).first->GetValue(modDateIndex)),
			ToString(timeRegList.at(i).first->GetValue(modProgramIndex)),
			ToString(timeRegList.at(i).first->GetValue(modProgramDateIndex)));
		auto& baseTimeReg = timeRegList.at(i).second;
		for (unsigned int j = i + 1; j < timeRegList.size(); ++j)
		{
			auto& compTimeReg = timeRegList.at(j).second;
			auto compObjectId = timeRegList.at(j).first->GetOID();

			if (baseTimeReg == compTimeReg)//★エラー 全く同じ規制条件が複数存在している
				m_output.OutputRegLog(err_level::error, sameTimeErrCode, ToString(objectId), ToString(compObjectId));
			else
				if (baseTimeReg.contain(compTimeReg) || baseTimeReg.cross(compTimeReg)) //★エラー 規制時間が重なっている
					m_output.OutputRegLog(err_level::error, overlapTimeErrCode, ToString(objectId), ToString(compObjectId));
		}
	}
}

void WalkOnewayCheckFunction::createTimeRegList(
	long linkID,
	const MultiRowsPack& walkNopassage,
	std::vector<std::pair< _IRowPtr, CTimeReg >>& timeRegList)
{
	using namespace category::time_condition_table;

	long start_monthIndex = walkNopassage.m_FieldMap.FindField(kStartMonth);
	long start_dayIndex = walkNopassage.m_FieldMap.FindField(kStartDay);
	long start_hourIndex = walkNopassage.m_FieldMap.FindField(kStartHour);
	long start_minIndex = walkNopassage.m_FieldMap.FindField(kStartMin);
	long end_monthIndex = walkNopassage.m_FieldMap.FindField(kEndMonth);
	long end_dayIndex = walkNopassage.m_FieldMap.FindField(kEndDay);
	long end_hourIndex = walkNopassage.m_FieldMap.FindField(kEndHour);
	long end_minIndex = walkNopassage.m_FieldMap.FindField(kEndMin);
	long dayOfweekIndex = walkNopassage.m_FieldMap.FindField(kDayOfWeek);
	auto NoPassRange = walkNopassage.m_Rows.equal_range(linkID);

	for (auto NoPassIte = NoPassRange.first; NoPassIte != NoPassRange.second; ++NoPassIte)
	{
		long start_month = NoPassIte->second->GetValue(start_monthIndex).lVal;
		long start_day = NoPassIte->second->GetValue(start_dayIndex).lVal;
		long start_hour = NoPassIte->second->GetValue(start_hourIndex).lVal;
		long start_min = NoPassIte->second->GetValue(start_minIndex).lVal;
		long end_month = NoPassIte->second->GetValue(end_monthIndex).lVal;
		long end_day = NoPassIte->second->GetValue(end_dayIndex).lVal;
		long end_hour = NoPassIte->second->GetValue(end_hourIndex).lVal;
		long end_min = NoPassIte->second->GetValue(end_minIndex).lVal;
		long dayOfweek = NoPassIte->second->GetValue(dayOfweekIndex).lVal;

		CTimeReg timeReg = CTimeReg(start_month, start_day, start_hour, start_min,
			end_month, end_day, end_hour, end_min, dayOfweek);

		timeRegList.push_back(std::pair<_IRowPtr, CTimeReg>(NoPassIte->second, timeReg));
	}
}

// リンクIDが不正な値かチェック
void WalkOnewayCheckFunction::checkLinkID(
	long  linkID
) const
{
	if (linkID <= 0)
		m_output.OutputRegLog(err_level::fatal, err_code::NgOnewayLinkID);
}

// 例外モード用のチェック
void WalkOnewayCheckFunction::checkException(
	const MultiRowsPack& walkOneway
)
{
	using namespace sindy::schema::walk_oneway;
	using namespace uh::str_util;

	if (m_dataType != walk_type::type_class::kDownConvert) return;

	m_output.SetLayer(kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	long linkIDIndex = walkOneway.m_FieldMap.FindField(kLinkID);
	long linkDirIndex = walkOneway.m_FieldMap.FindField(kLinkDir);

	auto modOperatorIndex = walkOneway.m_FieldMap.FindField(kOperator);
	auto modDateIndex = walkOneway.m_FieldMap.FindField(kModifyDate);
	auto modProgramIndex = walkOneway.m_FieldMap.FindField(kModifyProgName);
	auto modProgramDateIndex = walkOneway.m_FieldMap.FindField(kProgModifyDate);

	for (const auto& walkOnewayPair : walkOneway.m_Rows)
	{
		auto& walkOnewayObj = walkOnewayPair.second;
		long objectId = walkOnewayObj->GetOID();
		long linkID = walkOnewayObj->GetValue(linkIDIndex).lVal;
		long linkDir = walkOnewayObj->GetValue(linkDirIndex).lVal;

		m_output.SetModifyInfo(objectId, ToString(walkOnewayObj->GetValue(modOperatorIndex)),
			ToString(walkOnewayObj->GetValue(modDateIndex)), ToString(walkOnewayObj->GetValue(modProgramIndex)),
			ToString(walkOnewayObj->GetValue(modProgramDateIndex)));

		//★エラー リンクIDが不正な値
		checkLinkID(linkID);
		//★エラー リンク方向コードが不正な値
		checkLinkDir(linkDir);
	}
}

// リンク方向コードが不正な値かチェック
void WalkOnewayCheckFunction::checkLinkDir(
	long  linkDir
) const
{
	using namespace sindy::schema::walk_oneway;

	if (linkDir != link_dir::ECode::kFore && linkDir != link_dir::ECode::kReverse)
		m_output.OutputRegLog(err_level::fatal, err_code::NgOnewayLinkDir);
}
