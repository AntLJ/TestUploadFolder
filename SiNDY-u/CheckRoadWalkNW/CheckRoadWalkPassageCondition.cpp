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
#include <stdarg.h>
#include "CheckRoadWalkPassageCondition.h"


CCheckRoadWalkPassageCondition::CCheckRoadWalkPassageCondition(void)
{
}


CCheckRoadWalkPassageCondition::~CCheckRoadWalkPassageCondition(void)
{
	clear();
}


bool CCheckRoadWalkPassageCondition::checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl)
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
		if( FAILED(ipRow->get_Value(fieldIndex.find(shp05::schema::road::road_walk_nopassage_condition::kLinkId)->second, &linkIDValue)))
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
			layer = CDatabaseController::TIME_NOPASSAGE;
			search_linkid = linkid;
		}
		else 
		{
			layer = CDatabaseController::WALK_NOPASSAGE;
			search_linkid = linkid - WALK_OBJECTID;
			walkRecord = true;
		}
		
		CComVariant shpValueStartMonth, shpValueStartDay, shpValueEndMonth, shpValueEndDay,
			shpValueStartHour, shpValueStartMin, shpValueEndHour, shpValueEndMin,
			shpValueDayOfWeek;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kFromMonth, fieldIndex, shpValueStartMonth))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kFromDay, fieldIndex, shpValueStartDay))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kToMonth, fieldIndex, shpValueEndMonth))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kToDay, fieldIndex, shpValueEndDay))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kFromHour, fieldIndex, shpValueStartHour))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kFromMin, fieldIndex, shpValueStartMin))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kToHour, fieldIndex, shpValueEndHour))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kToMin, fieldIndex, shpValueEndMin))
			continue;
		if(! getValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kDayOfWeek, fieldIndex, shpValueDayOfWeek))
			continue;

		MAP_VALUE mapValue;
		if(! walkRecord)
		{
			setValue(sindy::schema::time_nopassage::kLinkID, search_linkid, mapValue);
			setValue(sindy::schema::time_nopassage::kStartMonth, shpValueStartMonth, mapValue);
			setValue(sindy::schema::time_nopassage::kStartDay, shpValueStartDay, mapValue);
			setValue(sindy::schema::time_nopassage::kEndMonth, shpValueEndMonth, mapValue);
			setValue(sindy::schema::time_nopassage::kEndDay, shpValueEndDay, mapValue);
			setValue(sindy::schema::time_nopassage::kStartHour, shpValueStartHour, mapValue);
			setValue(sindy::schema::time_nopassage::kStartMin, shpValueStartMin, mapValue);
			setValue(sindy::schema::time_nopassage::kEndHour, shpValueEndHour, mapValue);
			setValue(sindy::schema::time_nopassage::kEndMin, shpValueEndMin, mapValue);
			setValue(sindy::schema::time_nopassage::kDayOfWeek, shpValueDayOfWeek, mapValue);
		}
		else
		{
			setValue(sindy::schema::walk_nopassage::kLinkID, search_linkid, mapValue);
			setValue(sindy::schema::walk_nopassage::kStartMonth, shpValueStartMonth, mapValue);
			setValue(sindy::schema::walk_nopassage::kStartDay, shpValueStartDay, mapValue);
			setValue(sindy::schema::walk_nopassage::kEndMonth, shpValueEndMonth, mapValue);
			setValue(sindy::schema::walk_nopassage::kEndDay, shpValueEndDay, mapValue);
			setValue(sindy::schema::walk_nopassage::kStartHour, shpValueStartHour, mapValue);
			setValue(sindy::schema::walk_nopassage::kStartMin, shpValueStartMin, mapValue);
			setValue(sindy::schema::walk_nopassage::kEndHour, shpValueEndHour, mapValue);
			setValue(sindy::schema::walk_nopassage::kEndMin, shpValueEndMin, mapValue);
			setValue(sindy::schema::walk_nopassage::kDayOfWeek, shpValueDayOfWeek, mapValue);
		}

		if(! dbCtrl.findRow(layer, mapValue))
		{
			CString fieldName;
			fieldName.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"), 
				shp05::schema::road::road_walk_nopassage_condition::kLinkId,
				shp05::schema::road::road_walk_nopassage_condition::kFromMonth,
				shp05::schema::road::road_walk_nopassage_condition::kFromDay,
				shp05::schema::road::road_walk_nopassage_condition::kToMonth,
				shp05::schema::road::road_walk_nopassage_condition::kToDay,
				shp05::schema::road::road_walk_nopassage_condition::kFromHour,
				shp05::schema::road::road_walk_nopassage_condition::kFromMin,
				shp05::schema::road::road_walk_nopassage_condition::kToHour,
				shp05::schema::road::road_walk_nopassage_condition::kToMin,
				shp05::schema::road::road_walk_nopassage_condition::kDayOfWeek);

			pushColumnErrorMessage(linkid, fieldName, _T("DBから比較レコードを取得できません"), 10,
				CString(linkIDValue), CString(shpValueStartMonth), CString(shpValueStartDay), CString(shpValueEndMonth), CString(shpValueEndDay),
				CString(shpValueStartHour), CString(shpValueStartMin), CString(shpValueEndHour), CString(shpValueEndMin), CString(shpValueDayOfWeek));
			continue;
		}

		// 道路・歩行者種別
		if(! checkFixedValue(linkid, ipRow, shp05::schema::road::road_walk_nopassage_condition::kKind_c, fieldIndex,
			shp05::schema::road::kind::kRoad, shp05::schema::road::kind::kWalk))
			continue;

	}
	return true;
}


bool CCheckRoadWalkPassageCondition::getValue(long linkID, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, CComVariant& value)
{
	if( FAILED(ipRow->get_Value(fieldIndex.find(fieldName)->second, &value)))
	{
		pushColumnErrorMessage(linkID, fieldName, MESSAGE_CAN_NOT_GET_VALUE);
		return false;
	}
	return true;
};


void CCheckRoadWalkPassageCondition::setValue(const CString& dbFieldName, const long& lValue, MAP_VALUE& mapValue)
{
	CString strValue;
	strValue.Format(_T("%ld"), lValue);
	CComVariant varWkValue = strValue;
	mapValue.insert(std::pair<CString, CAdapt<CComVariant>>(dbFieldName, varWkValue));
}


bool CCheckRoadWalkPassageCondition::setValue(const CString& dbFieldName, const CComVariant& varValue, MAP_VALUE& mapValue)
{
	CComVariant varWkValue;
	if( ( varValue.vt == VT_I4 && varValue.lVal == SHAPE_INT_NULL ) ||
		( varValue.vt == VT_BSTR && CString(varValue.bstrVal) == SHAPE_STRING_NULL ))
		varWkValue = _T("");
	else
	{
		varWkValue = varValue;
		varWkValue.ChangeType(VT_BSTR);
	}

	mapValue.insert(std::pair<CString, CAdapt<CComVariant>>(dbFieldName, varWkValue));
	return true;
};


bool CCheckRoadWalkPassageCondition::checkFixedValue(long linkID, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, long fixedTimeValue, long fixedWalkValue)
{
	CComVariant shapeValue;
	if( !getValue(linkID, ipRow, fieldName, fieldIndex, shapeValue))
		return false;

	if(linkID < WALK_OBJECTID )
	{
		if( !compShapeDbValue(shapeValue, CComVariant(fixedTimeValue)))
		{
			pushColumnErrorMessage(linkID, fieldName, MESSAGE_NOT_FIXED_VALUE, 2, CString(shapeValue), CString(CComVariant(fixedTimeValue)));
		}
	}
	else
	{
		if( !compShapeDbValue(shapeValue, CComVariant(fixedWalkValue)))
		{
			pushColumnErrorMessage(linkID, fieldName, MESSAGE_NOT_FIXED_VALUE, 2, CString(shapeValue), CString(CComVariant(fixedWalkValue)));
		}
	}
	return true;
}

void CCheckRoadWalkPassageCondition::pushColumnErrorMessage(long linkID, const CString& filedName, const CString& message, int count, ...)
{
	va_list list;
	va_start(list, count);
	CString DetailMessage;
	for( int i=0 ; i<count ; i++ ) {
		CString msg;
		msg.Format(_T("\t%s"), va_arg( list, CString ));
		DetailMessage += msg;
	}
	va_end( list );

	CString errorMessage;
	errorMessage.Format(_T("%ld\t%s\t%s%s"), linkID, filedName, message, DetailMessage);

	m_errorMessageList.push_back(errorMessage);
}
