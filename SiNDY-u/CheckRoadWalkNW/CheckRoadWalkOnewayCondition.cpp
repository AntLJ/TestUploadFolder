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
#include "CheckRoadWalkOnewayCondition.h"


CCheckRoadWalkOnewayCondition::CCheckRoadWalkOnewayCondition(void)
{
}


CCheckRoadWalkOnewayCondition::~CCheckRoadWalkOnewayCondition(void)
{
	clear();
}


bool CCheckRoadWalkOnewayCondition::checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl)
{
	
	m_errorMessageList.clear();

	_ICursorPtr ipCursor;
	if(FAILED(ipTable->Search(NULL, VARIANT_FALSE, &ipCursor)))
		return false;

	_IRowPtr ipRow;
	CDatabaseController::LAYER layer = CDatabaseController::NONE, tmpLayer = CDatabaseController::NONE;
	FIELD_INDEX dbFieldIndex;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){

		CComVariant linkIDValue;
		if( FAILED(ipRow->get_Value(fieldIndex.find(shp05::schema::road::road_walk_oneway_condition::kLinkId)->second, &linkIDValue)))
		{
			pushErrorMessage(_T("LINKIDを取得できません"));
			return false;
		}
		linkIDValue.ChangeType(VT_I4);
		long linkid = linkIDValue.lVal;

		long search_linkid = 0;
		bool walkRecord = false;
		if(linkid < WALK_OBJECTID )
		{
			layer = CDatabaseController::TIME_ONEWAY;
			search_linkid = linkid;
		}
		else 
		{
			layer = CDatabaseController::WALK_ONEWAY;
			search_linkid = linkid - WALK_OBJECTID;
			walkRecord = true;
		}

		CComVariant shpValueLinkDir, shpValueStartMonth, shpValueStartDay, shpValueEndMonth, shpValueEndDay,
			shpValueStartHour, shpValueStartMin, shpValueEndHour, shpValueEndMin,
			shpValueDayOfWeek;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kLinkDir, fieldIndex, shpValueLinkDir))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kFromMonth, fieldIndex, shpValueStartMonth))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kFromDay, fieldIndex, shpValueStartDay))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kToMonth, fieldIndex, shpValueEndMonth))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kToDay, fieldIndex, shpValueEndDay))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kFromHour, fieldIndex, shpValueStartHour))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kFromMin, fieldIndex, shpValueStartMin))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kToHour, fieldIndex, shpValueEndHour))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kToMin, fieldIndex, shpValueEndMin))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kDayOfWeek, fieldIndex, shpValueDayOfWeek))
			continue;

		MAP_VALUE mapValue;
		if(! walkRecord)
		{
			setValue(sindy::schema::time_oneway::kLinkID, search_linkid, mapValue);
			setValue(sindy::schema::time_oneway::kLinkDir, shpValueLinkDir, mapValue);
			setValue(sindy::schema::time_oneway::kStartMonth, shpValueStartMonth, mapValue);
			setValue(sindy::schema::time_oneway::kStartDay, shpValueStartDay, mapValue);
			setValue(sindy::schema::time_oneway::kEndMonth, shpValueEndMonth, mapValue);
			setValue(sindy::schema::time_oneway::kEndDay, shpValueEndDay, mapValue);
			setValue(sindy::schema::time_oneway::kStartHour, shpValueStartHour, mapValue);
			setValue(sindy::schema::time_oneway::kStartMin, shpValueStartMin, mapValue);
			setValue(sindy::schema::time_oneway::kEndHour, shpValueEndHour, mapValue);
			setValue(sindy::schema::time_oneway::kEndMin, shpValueEndMin, mapValue);
			setValue(sindy::schema::time_oneway::kDayOfWeek, shpValueDayOfWeek, mapValue);
		}
		else
		{
			setValue(sindy::schema::time_oneway::kLinkID, search_linkid, mapValue);
			setValue(sindy::schema::walk_oneway::kLinkDir, shpValueLinkDir, mapValue);
			setValue(sindy::schema::walk_oneway::kStartMonth, shpValueStartMonth, mapValue);
			setValue(sindy::schema::walk_oneway::kStartDay, shpValueStartDay, mapValue);
			setValue(sindy::schema::walk_oneway::kEndMonth, shpValueEndMonth, mapValue);
			setValue(sindy::schema::walk_oneway::kEndDay, shpValueEndDay, mapValue);
			setValue(sindy::schema::walk_oneway::kStartHour, shpValueStartHour, mapValue);
			setValue(sindy::schema::walk_oneway::kStartMin, shpValueStartMin, mapValue);
			setValue(sindy::schema::walk_oneway::kEndHour, shpValueEndHour, mapValue);
			setValue(sindy::schema::walk_oneway::kEndMin, shpValueEndMin, mapValue);
			setValue(sindy::schema::walk_oneway::kDayOfWeek, shpValueDayOfWeek, mapValue);
		}

		if(! dbCtrl.findRow(layer, mapValue))
		{
			CString fieldName;
			fieldName.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"), 
				shp05::schema::road::road_walk_oneway_condition::kLinkId,
				shp05::schema::road::road_walk_oneway_condition::kLinkDir,
				shp05::schema::road::road_walk_oneway_condition::kFromMonth,
				shp05::schema::road::road_walk_oneway_condition::kFromDay,
				shp05::schema::road::road_walk_oneway_condition::kToMonth,
				shp05::schema::road::road_walk_oneway_condition::kToDay,
				shp05::schema::road::road_walk_oneway_condition::kFromHour,
				shp05::schema::road::road_walk_oneway_condition::kFromMin,
				shp05::schema::road::road_walk_oneway_condition::kToHour,
				shp05::schema::road::road_walk_oneway_condition::kToMin,
				shp05::schema::road::road_walk_oneway_condition::kDayOfWeek);

			pushColumnErrorMessage(linkid, fieldName, _T("DBから比較レコードを取得できません"), 11,
				CString(linkIDValue), CString(shpValueLinkDir), CString(shpValueStartMonth), CString(shpValueStartDay), CString(shpValueEndMonth),
				CString(shpValueEndDay), CString(shpValueStartHour), CString(shpValueStartMin), CString(shpValueEndHour), CString(shpValueEndMin),
				CString(shpValueDayOfWeek));
			continue;
		}

		// 道路・歩行者種別
		if(! checkFixedValue(linkid, ipRow, shp05::schema::road::road_walk_oneway_condition::kKind_c, fieldIndex,
			shp05::schema::road::kind::kRoad, shp05::schema::road::kind::kWalk))
			continue;

	}
	return true;
}
