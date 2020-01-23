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
#include "WalkNoPassageCheckFunction.h"
#include "value.h"

using namespace sindy::schema;

void WalkNoPassageCheckFunction::checkWalkNoPassage(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink, const MultiRowsPack& walkNoPassage)
{
	using namespace sindy::schema::walk_nopassage;
	using namespace uh::str_util;

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	long linkID = -1;
	long start_monthIndex = walkNoPassage.m_FieldMap.FindField(kStartMonth);
	long start_dayIndex = walkNoPassage.m_FieldMap.FindField(kStartDay);
	long start_hourIndex = walkNoPassage.m_FieldMap.FindField(kStartHour);
	long start_minIndex = walkNoPassage.m_FieldMap.FindField(kStartMin);
	long end_monthIndex = walkNoPassage.m_FieldMap.FindField(kEndMonth);
	long end_dayIndex = walkNoPassage.m_FieldMap.FindField(kEndDay);
	long end_hourIndex = walkNoPassage.m_FieldMap.FindField(kEndHour);
	long end_minIndex = walkNoPassage.m_FieldMap.FindField(kEndMin);
	long dayOfweekIndex = walkNoPassage.m_FieldMap.FindField(kDayOfWeek);

	auto modOperatorIndex = walkNoPassage.m_FieldMap.FindField(kOperator);
	auto modDateIndex = walkNoPassage.m_FieldMap.FindField(kModifyDate);
	auto modProgramIndex = walkNoPassage.m_FieldMap.FindField(kModifyProgName);
	auto modProgramDateIndex = walkNoPassage.m_FieldMap.FindField(kProgModifyDate);
	std::vector< std::pair< _IRowPtr, CTimeReg > > timeRegList;
	for (const auto& walkNoPass : walkNoPassage.m_Rows)
	{
		long objectId = walkNoPass.second->GetOID();
		long start_month = walkNoPass.second->GetValue(start_monthIndex).lVal;
		long start_day = walkNoPass.second->GetValue(start_dayIndex).lVal;
		long start_hour = walkNoPass.second->GetValue(start_hourIndex).lVal;
		long start_min = walkNoPass.second->GetValue(start_minIndex).lVal;
		long end_month = walkNoPass.second->GetValue(end_monthIndex).lVal;
		long end_day = walkNoPass.second->GetValue(end_dayIndex).lVal;
		long end_hour = walkNoPass.second->GetValue(end_hourIndex).lVal;
		long end_min = walkNoPass.second->GetValue(end_minIndex).lVal;
		long dayOfweek = walkNoPass.second->GetValue(dayOfweekIndex).lVal;


		auto walkLinkIte = walkLink.m_FeatureMap.find(walkNoPass.first);
		if (walkLinkIte == walkLink.m_FeatureMap.end())
			continue;
		if (walkLinkIte->second.IsDisjoint(ipMeshGeo))
			continue;

		m_output.SetModifyInfo(walkNoPass.first, ToString(walkNoPass.second->GetValue(modOperatorIndex)),
			ToString(walkNoPass.second->GetValue(modDateIndex)),
			ToString(walkNoPass.second->GetValue(modProgramIndex)),
			ToString(walkNoPass.second->GetValue(modProgramDateIndex)));
		CTimeReg timeReg = CTimeReg(start_month, start_day, start_hour, start_min,
			end_month, end_day, end_hour, end_min, dayOfweek);
		if (!timeReg.check()) //★エラー 通行禁止の規制条件が不正な値
			m_output.OutputRegLog(err_level::error, err_code::NgNoPassageTime, ToString(objectId));

		if (!timeReg.checkAllDay()) //★エラー 規制条件の通年が1/1～12/31になっていない
			m_output.OutputRegLog(err_level::error, err_code::NgNoPassFullYear, ToString(objectId));

		if (linkID != walkNoPass.first)
		{
			if (linkID != -1)
			{
				checkTimeRegulationRel(linkID, timeRegList, walkNoPassage.m_FieldMap, err_code::SameNoPassageTime, err_code::OverLapNoPassTime);
			}
			linkID = walkNoPass.first;
			timeRegList.clear();
		}
		timeRegList.push_back(std::pair<_IRowPtr, CTimeReg>(walkNoPass.second, timeReg));
	}
	checkTimeRegulationRel(linkID, timeRegList, walkNoPassage.m_FieldMap, err_code::SameNoPassageTime, err_code::OverLapNoPassTime);
}

void WalkNoPassageCheckFunction::checkRelease(
	const IGeometryPtr& ipMeshGeo,
	const FeaturesPack& walkLink,
	const MultiRowsPack& walkNoPassage
)
{
	using namespace sindy::schema::walk_nopassage;
	using namespace uh::str_util;

	m_output.SetLayer(walk_link::kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	long start_monthIndex = walkNoPassage.m_FieldMap.FindField(kStartMonth);
	long start_dayIndex = walkNoPassage.m_FieldMap.FindField(kStartDay);
	long start_hourIndex = walkNoPassage.m_FieldMap.FindField(kStartHour);
	long start_minIndex = walkNoPassage.m_FieldMap.FindField(kStartMin);
	long end_monthIndex = walkNoPassage.m_FieldMap.FindField(kEndMonth);
	long end_dayIndex = walkNoPassage.m_FieldMap.FindField(kEndDay);
	long end_hourIndex = walkNoPassage.m_FieldMap.FindField(kEndHour);
	long end_minIndex = walkNoPassage.m_FieldMap.FindField(kEndMin);
	long dayOfweekIndex = walkNoPassage.m_FieldMap.FindField(kDayOfWeek);

	auto modOperatorIndex = walkNoPassage.m_FieldMap.FindField(kOperator);
	auto modDateIndex = walkNoPassage.m_FieldMap.FindField(kModifyDate);
	auto modProgramIndex = walkNoPassage.m_FieldMap.FindField(kModifyProgName);
	auto modProgramDateIndex = walkNoPassage.m_FieldMap.FindField(kProgModifyDate);
	for (const auto& walkNoPass : walkNoPassage.m_Rows)
	{
		long objectId = walkNoPass.second->GetOID();
		long start_month = walkNoPass.second->GetValue(start_monthIndex).lVal;
		long start_day = walkNoPass.second->GetValue(start_dayIndex).lVal;
		long start_hour = walkNoPass.second->GetValue(start_hourIndex).lVal;
		long start_min = walkNoPass.second->GetValue(start_minIndex).lVal;
		long end_month = walkNoPass.second->GetValue(end_monthIndex).lVal;
		long end_day = walkNoPass.second->GetValue(end_dayIndex).lVal;
		long end_hour = walkNoPass.second->GetValue(end_hourIndex).lVal;
		long end_min = walkNoPass.second->GetValue(end_minIndex).lVal;
		long dayOfweek = walkNoPass.second->GetValue(dayOfweekIndex).lVal;

		auto walkLinkIte = walkLink.m_FeatureMap.find(walkNoPass.first);
		if (walkLinkIte == walkLink.m_FeatureMap.end())
			continue;
		if (walkLinkIte->second.IsDisjoint(ipMeshGeo))
			continue;

		m_output.SetModifyInfo(walkNoPass.first, ToString(walkNoPass.second->GetValue(modOperatorIndex)),
			ToString(walkNoPass.second->GetValue(modDateIndex)),
			ToString(walkNoPass.second->GetValue(modProgramIndex)),
			ToString(walkNoPass.second->GetValue(modProgramDateIndex)));
		CTimeReg timeReg = CTimeReg(start_month, start_day, start_hour, start_min,
			end_month, end_day, end_hour, end_min, dayOfweek);
		if (!timeReg.check()) //★エラー 通行禁止の規制条件が不正な値
			m_output.OutputRegLog(err_level::error, err_code::NgNoPassageTime, ToString(objectId));

		if (!timeReg.checkAllDay()) //★エラー 規制条件の通年が1/1～12/31になっていない
			m_output.OutputRegLog(err_level::error, err_code::NgNoPassFullYear, ToString(objectId));
	}
}

void WalkNoPassageCheckFunction::checkTimeRegulationRel(
	long linkID,
	const std::vector<std::pair<_IRowPtr, CTimeReg>>& timeRegList,
	const sindy::CFieldMap& fieldMap,
	const err_code::ECode sameTimeErrCode, const err_code::ECode overlapTimeErrCode)
{
	using namespace uh::str_util;
	// リストが0、または1の場合リターン
	if (timeRegList.empty() || timeRegList.size() == 1) return;
	long modOperatorIndex = fieldMap.FindField(ipc_table::kOperator);
	long modDateIndex = fieldMap.FindField(ipc_table::kModifyDate);
	long modProgramIndex = fieldMap.FindField(ipc_table::kModifyProgName);
	long modProgramDateIndex = fieldMap.FindField(ipc_table::kProgModifyDate);
	for (unsigned int i = 0; i < timeRegList.size() - 1; ++i)
	{
		auto objectId = timeRegList.at(i).first->GetOID();
		m_output.SetModifyInfo(linkID, ToString(timeRegList.at(i).first->GetValue(modOperatorIndex)),
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

// リンクIDが不正な値かチェック
void WalkNoPassageCheckFunction::checkLinkID(
	long  linkID
) const
{
	if (linkID <= 0)
		m_output.OutputRegLog(err_level::fatal, err_code::NgNoPassLinkID);
}

// 例外モード用のチェック
void WalkNoPassageCheckFunction::checkException(
	const MultiRowsPack& walkNoPassage
)
{
	using namespace sindy::schema::walk_nopassage;
	using namespace uh::str_util;

	if (m_dataType != walk_type::type_class::kDownConvert) return;

	m_output.SetLayer(kTableName);
	m_output.OutputStdErr(err_level::info, err_code::StrChecking, kTableName);

	long linkIDIndex = walkNoPassage.m_FieldMap.FindField(kLinkID);

	auto modOperatorIndex = walkNoPassage.m_FieldMap.FindField(kOperator);
	auto modDateIndex = walkNoPassage.m_FieldMap.FindField(kModifyDate);
	auto modProgramIndex = walkNoPassage.m_FieldMap.FindField(kModifyProgName);
	auto modProgramDateIndex = walkNoPassage.m_FieldMap.FindField(kProgModifyDate);

	for (const auto& walkNoPass : walkNoPassage.m_Rows)
	{
		long objectId = walkNoPass.second->GetOID();
		long linkID = walkNoPass.second->GetValue(linkIDIndex).lVal;

		m_output.SetModifyInfo(objectId, ToString(walkNoPass.second->GetValue(modOperatorIndex)),
			ToString(walkNoPass.second->GetValue(modDateIndex)),
			ToString(walkNoPass.second->GetValue(modProgramIndex)),
			ToString(walkNoPass.second->GetValue(modProgramDateIndex)));

		//★エラー リンクIDが不正な値
		checkLinkID(linkID);
	}
}
