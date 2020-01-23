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
#include "CheckRoadCode.h"


CCheckRoadCode::CCheckRoadCode(void)
{
}


CCheckRoadCode::~CCheckRoadCode(void)
{
	clear();
}

bool CCheckRoadCode::checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl)
{
	
	m_errorMessageList.clear();

	_ICursorPtr ipCursor;
	if(FAILED(ipTable->Search(NULL, VARIANT_FALSE, &ipCursor)))
		return false;

	_IRowPtr ipRow;
	CDatabaseController::LAYER layer = CDatabaseController::NONE, tmpLayer = CDatabaseController::NONE;
	FIELD_INDEX dbFieldIndex;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){

		CComVariant roadcodeValue;
		if( FAILED(ipRow->get_Value(fieldIndex.find(shp05::schema::road::road_code::kRoadCode)->second, &roadcodeValue)))
		{
			pushErrorMessage(_T("ROADCODE‚ðŽæ“¾‚Å‚«‚Ü‚¹‚ñ"));
			return false;
		}
		roadcodeValue.ChangeType(VT_I4);
		long roadcode = roadcodeValue.lVal;

		CComVariant roadtypeValue;
		if(! getValue(roadcode, ipRow, shp05::schema::road::road_code::kRoadType, fieldIndex, roadtypeValue))
			continue;
		roadtypeValue.ChangeType(VT_I4);
		long roadtype = roadtypeValue.lVal;

		MAP_VALUE mapValue;
		bool walkRecord = false;
		if(roadtype != shp05::schema::road::road_code::road_type::kWalk)
		{
			CString key;
			key.Format(_T("%ld,%ld"), roadcode, roadtype);
			if(! dbCtrl.findRoadCode(CDatabaseController::ROAD_CODE_LIST, key, mapValue))
			{
				pushColumnErrorMessage(roadcode, shp05::schema::road::road_code::kRoadCode, _T("DB‚©‚ç”äŠrƒŒƒR[ƒh‚ðŽæ“¾‚Å‚«‚Ü‚¹‚ñ"), roadtypeValue);
				continue;
			}
		}
		else
		{
			CString key;
			key.Format(_T("%ld"), roadcode);
			if(! dbCtrl.findRoadCode(CDatabaseController::WALK_CODE_LIST, key, mapValue))
			{
				pushColumnErrorMessage(roadcode, shp05::schema::road::road_code::kRoadCode, _T("DB‚©‚ç”äŠrƒŒƒR[ƒh‚ðŽæ“¾‚Å‚«‚Ü‚¹‚ñ"));
				continue;
			}
			walkRecord = true;
		}

		// ˜HüŠ¿Žš–¼Ì
		if( !checkDBValue(roadcode, ipRow, shp05::schema::road::road_code::kRoadName, fieldIndex, 
			mapValue, sindy::schema::walk_code_list::kNameKanji, sindy::schema::road_code_list::kDisplayKanji, walkRecord))
			continue;

		// ˜Hüƒˆƒ~–¼Ì
		if( !checkDBValue(roadcode, ipRow, shp05::schema::road::road_code::kRoadYomi, fieldIndex, 
			mapValue, sindy::schema::walk_code_list::kNameYomi, sindy::schema::road_code_list::kDisplayYomi, walkRecord))
			continue;
	}
	return true;
}


bool CCheckRoadCode::getValue(long roadcode, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, CComVariant& value)
{
	if( FAILED(ipRow->get_Value(fieldIndex.find(fieldName)->second, &value)))
	{
		pushColumnErrorMessage(roadcode, fieldName, MESSAGE_CAN_NOT_GET_VALUE);
		return false;
	}
	return true;
};


bool CCheckRoadCode::checkDBValue(long roadcode, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex,
								  const MAP_VALUE& mapDbValue, const CString& dbWalkFieldName, const CString& dbRoadFieldName, bool walkRecord)
{
	CComVariant shapeValue;
	if( !getValue(roadcode, ipRow, fieldName, fieldIndex, shapeValue))
		return false;

	CComVariant dbValue;
	if( walkRecord)
		dbValue = mapDbValue.find(dbWalkFieldName)->second;
	else
		dbValue = mapDbValue.find(dbRoadFieldName)->second;

	if( ! compShapeDbValue(shapeValue, dbValue))
		pushColumnErrorMessage(roadcode, fieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
	return true;
}
