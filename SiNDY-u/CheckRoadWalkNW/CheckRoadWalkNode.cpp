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
#include "CheckRoadWalkNode.h"

// 歩行者ノード種別（仮）追加用（（仮）が不要になった際は併せて削除すること）
#include <sindy/schema_draft/road_draft.h>

CCheckRoadWalkNode::CCheckRoadWalkNode(void)
{
}


CCheckRoadWalkNode::~CCheckRoadWalkNode(void)
{
	clear();
}

void CCheckRoadWalkNode::clear(void){
	m_errorMessageList.clear();
}

std::vector<CString> CCheckRoadWalkNode::getMeessageList(){
	return m_errorMessageList;
}

bool CCheckRoadWalkNode::checkFormat(const IFeatureClassPtr ipFeatureClass,FIELD_INDEX fieldIndex, CDatabaseController dbCtrl)
{
	//TODO:共通化クラスを別途作成し、共通化する。他の関数も共通クラスに移動する。
	m_errorMessageList.clear();

	IFeatureCursorPtr ipFeatureCursor;
	if(FAILED(ipFeatureClass->Search(NULL, VARIANT_FALSE, &ipFeatureCursor)))
		return false;

	IFeaturePtr ipFeature;
	CDatabaseController::LAYER layer = CDatabaseController::NONE, tmpLayer = CDatabaseController::NONE;
	FIELD_INDEX dbFieldIndex, roadNodeFieldIndex;

	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK){

		CComVariant objectIDValue;
		if( FAILED(ipFeature->get_Value(fieldIndex.find(sindy::schema::kObjectID)->second, &objectIDValue)))
		{
			pushErrorMessage(_T("OBJECTIDを取得できません"));
			return false;
		}
		objectIDValue.ChangeType(VT_I4);
		long objectid = objectIDValue.lVal;

		bool walkRecord = false;
		if(objectid < WALK_OBJECTID )
			layer = CDatabaseController::ROAD_NODE;
		else 
		{
			layer = CDatabaseController::WALK_NODE;
			walkRecord = true;
		}
		
		if(layer != tmpLayer)
		{
			tmpLayer = layer;
			if( ! dbCtrl.getFieldIndex(layer, dbFieldIndex))
			{
				pushErrorMessage(MESSAGE_CAN_NOT_GET_FIELDINDEX);
				return false;
			}
			if(layer == CDatabaseController::WALK_NODE)
			{
				if(! dbCtrl.getFieldIndex(CDatabaseController::ROAD_NODE, roadNodeFieldIndex))
				{
					pushErrorMessage(MESSAGE_CAN_NOT_GET_FIELDINDEX );
					return false;
				}
			}
		}

		IFeaturePtr ipDbFeature;

		long search_objectid = 0;
		if(walkRecord)
			search_objectid = objectid - WALK_OBJECTID;
		else
			search_objectid = objectid;
		
		if(! dbCtrl.getFeature(layer, search_objectid, ipDbFeature))
		{
			pushColumnErrorMessage(objectid, sindy::schema::kObjectID, _T("DBから比較レコードを取得できません"), search_objectid, 0);
			continue;
		}

		CComVariant shapeValue, dbValue;
		CString shapeFieldName, dbFieldName;

		// 信号機フラグ
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_node::kSignal_f, fieldIndex,
			ipDbFeature, sindy::schema::road_node::kSignal, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// 歩行者ノード種別
		shapeFieldName = shp05::schema::road::road_walk_node::kWalkNodeClass_c;
		if( !getFeatureValue(objectid, ipFeature, shapeFieldName, fieldIndex, shapeValue))
			continue;
		shapeValue.ChangeType(VT_I4);
		if(walkRecord)
		{
			if( !getFeatureValue(objectid, ipDbFeature, sindy::schema::walk_node::kNodeClass, dbFieldIndex, dbValue))
				continue;
			
			dbValue.ChangeType(VT_I4);
			switch (dbValue.lVal) {
				case sindy::schema::walk_node::node_class::kElevator :
				case sindy::schema::walk_node::node_class::kUndergroundGate :
				case sindy::schema::walk_node::node_class::kGate :
				case sindy::schema::walk_node::node_class::kKaisatsu :
				case sindy::schema::walk_node::node_class::kCarPratform :
				case sindy::schema::walk_node::node_class::kBusPratform :
				case sindy::schema::walk_node::node_class::kTaxiPratform :
				case sindy::schema::walk_node::node_class::kParking :
				case sindy::schema::walk_node::node_class::kDepartureGate :
				case sindy::schema::walk_node::node_class::kArrivalGate :
				case sindy::schema::walk_node::node_class::kTicketGate:			// 駅改札ノード
				case sindy::schema::walk_node::node_class::kUnsurveyTicketGate:	// 改札ノード（仮）
					if(shapeValue != dbValue)
						pushColumnErrorMessage(objectid, shapeFieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
					break;
				default :
					if(shapeValue.lVal != sindy::schema::walk_node::node_class::kPublic)
						pushColumnErrorMessage(objectid, shapeFieldName, _T("期待値と異なります"), shapeValue, CComVariant(sindy::schema::walk_node::node_class::kPublic));
					break;
			}
		}
		else
		{
			if(shapeValue.lVal != SHAPE_INT_DEFAULT)
				pushColumnErrorMessage(objectid, shapeFieldName, MESSAGE_NOT_FIXED_VALUE, shapeValue, CComVariant(SHAPE_INT_DEFAULT));
		}

		// 漢字名称、読み
		std::vector<std::pair<CString,CString>> checkFieldName;
		checkFieldName.push_back(std::pair<CString,CString>(shp05::schema::road::road_walk_node::kNameKanji, sindy::schema::road_node::kNameKanji));
		checkFieldName.push_back(std::pair<CString,CString>(shp05::schema::road::road_walk_node::kNameYomi, sindy::schema::road_node::kNameYomi));
		
		IFeaturePtr ipDbNameFeature;
		for(std::pair<CString,CString> fieldNames : checkFieldName)
		{
			if( !getFeatureValue(objectid, ipFeature, fieldNames.first, fieldIndex, shapeValue))
				break;
			if(walkRecord){
				CString dbRoadNodeIDFieldName = sindy::schema::walk_node::kRoadNodeID;
				if( !getFeatureValue(objectid, ipDbFeature, dbRoadNodeIDFieldName, dbFieldIndex, dbValue))
					break;
				if( dbValue.vt == VT_NULL || dbValue.vt == VT_EMPTY )
				{
					if(shapeValue != CComVariant(SHAPE_STRING_NULL))
						pushColumnErrorMessage(objectid, fieldNames.first, MESSAGE_IS_NOT_NULL, shapeValue, dbValue);
				}
				else
				{
					if(!ipDbNameFeature)
					{
						dbValue.ChangeType(VT_I4);
						// 関連付け用の道路ノードは、メッシュ+バッファ形状に含まれるものから取得
						if(! dbCtrl.getBufferFeature(CDatabaseController::ROAD_NODE, dbValue.lVal, ipDbNameFeature))
						{
							pushColumnErrorMessage(objectid, dbRoadNodeIDFieldName, _T("対象ObjectIDを持つROAD_NODEを取得できません"), dbValue.lVal, 0);
							break;
						}
					}
					if( !getFeatureValue(objectid, ipDbNameFeature, fieldNames.second, roadNodeFieldIndex, dbValue))
						break;
					// ROAD_NODE_IDを持つ場合、必ず名称が格納されているため
					if(shapeValue != dbValue)
						pushColumnErrorMessage(objectid, fieldNames.first, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
				}
			}
			else
			{
				if( !getFeatureValue(objectid, ipDbFeature, fieldNames.second, dbFieldIndex, dbValue))
					break;
				if( ! compShapeDbValue(shapeValue, dbValue))
					pushColumnErrorMessage(objectid, fieldNames.first, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
			}
		}

		if(!checkGeomPoint(objectid, ipFeature, ipDbFeature))
			continue;

	}

	return true;
}

bool CCheckRoadWalkNode::checkDBValueFixedValue(long objectID, 
												IFeaturePtr ipFeature, CString fieldName, FIELD_INDEX fieldIndex,
												IFeaturePtr ipDbFeature, CString dbFieldName, FIELD_INDEX dbFieldIndex,
												bool walkIsFixedValue, long fixedValue)
{
	CComVariant shapeValue;
	if( !getFeatureValue(objectID, ipFeature, fieldName, fieldIndex, shapeValue))
		return false;

	if(!(walkIsFixedValue ^ (objectID > WALK_OBJECTID)))
	{
		shapeValue.ChangeType(VT_I4);
		if(shapeValue.lVal != fixedValue)
			pushColumnErrorMessage(objectID, fieldName, MESSAGE_NOT_FIXED_VALUE, shapeValue, CComVariant(fixedValue));
	}
	else
	{
		CComVariant dbValue;
		if( !getFeatureValue(objectID, ipDbFeature, dbFieldName, dbFieldIndex, dbValue))
			return false;
		if(! compShapeDbValue(shapeValue, dbValue))
			pushColumnErrorMessage(objectID, fieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
	}
	return true;
}

bool CCheckRoadWalkNode::compShapeDbValue(CComVariant shapeValue, CComVariant dbValue)
{
	if(dbValue.vt == VT_NULL || dbValue.vt == VT_EMPTY)
	{
		if( shapeValue.vt == VT_I4)
			dbValue = CComVariant(SHAPE_INT_NULL);
		else if(shapeValue.vt == VT_BSTR)
			dbValue = CComVariant(SHAPE_STRING_NULL);
	}
	return (shapeValue == dbValue);
}

bool CCheckRoadWalkNode::checkGeomPoints(long objectID, const IFeaturePtr ipShapeFeature, const IFeaturePtr ipDBFeature)
{

	IGeometryPtr shapeGeometry, dbGeometry;
	if (FAILED( ipShapeFeature->get_ShapeCopy(&shapeGeometry)) ||
		FAILED(ipDBFeature->get_ShapeCopy(&dbGeometry))) 
	{
		pushColumnErrorMessage(objectID, _T("SHAPE"), _T("形状の取得に失敗しました"));
		return false;
	}


	std::vector< WKSPoint > shapePointVec, dbPointVec;
	m_geomProcess.getGeom2PointVec( shapeGeometry, shapePointVec);
	m_geomProcess.getGeom2PointVec( dbGeometry, dbPointVec);

	if(! m_geomProcess.judgePointVecSame(shapePointVec, dbPointVec))
	{
		pushColumnErrorMessage(objectID, _T("SHAPE"), _T("比較元の形状と一致しません"));
		return false;
	}
	return true;
}

bool CCheckRoadWalkNode::checkGeomPoint(long objectID, const IFeaturePtr ipShapeFeature, const IFeaturePtr ipDBFeature)
{

	IGeometryPtr shapeGeometry, dbGeometry;
	if (FAILED( ipShapeFeature->get_ShapeCopy(&shapeGeometry)) ||
		FAILED(ipDBFeature->get_ShapeCopy(&dbGeometry))) 
	{
		pushColumnErrorMessage(objectID, _T("SHAPE"), _T("形状の取得に失敗しました"));
		return false;
	}
		
	WKSPoint shapePoint, dbPoint;
	m_geomProcess.getPoint( shapeGeometry, shapePoint);
	m_geomProcess.getPoint( dbGeometry, dbPoint);

	if(! m_geomProcess.judgePointSame(shapePoint, dbPoint))
	{
		pushColumnErrorMessage(objectID, _T("SHAPE"), _T("比較元の形状と一致しません"));
		return false;
	}
	return true;
}


bool CCheckRoadWalkNode::getFeatureValue(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex, CComVariant& value)
{
	if( FAILED(ipFeature->get_Value(fieldIndex.find(fieldName)->second, &value)))
	{
		pushColumnErrorMessage(objectID, fieldName, MESSAGE_CAN_NOT_GET_VALUE);
		return false;
	}
	return true;
};

void CCheckRoadWalkNode::pushColumnErrorMessage(long objectID, CString filedName, CString message, CComVariant value, CComVariant compValue){

	CString errorMessage;
	if(value.vt != VT_NULL && value.vt != VT_EMPTY){
		if(value.vt == VT_BSTR)
		{
			CString csVal;
			CString csCompVal;
			value.ChangeType(VT_BSTR);
			csVal = value.bstrVal;
			if(compValue.vt != VT_NULL && compValue.vt != VT_EMPTY){
				compValue.ChangeType(VT_BSTR);
				csCompVal = compValue.bstrVal;
			}
			errorMessage.Format(_T("%ld\t%s\t%s\t%s,%s"), objectID, filedName, message, csVal, csCompVal);
		}
		else if(value.vt == VT_I4)
		{
			value.ChangeType(VT_I4);
			if(compValue.vt != VT_NULL && compValue.vt != VT_EMPTY){
				compValue.ChangeType(VT_I4);
				errorMessage.Format(_T("%ld\t%s\t%s\t%ld,%ld"), objectID, filedName, message, value.lVal, compValue.lVal);
			}
			else
				errorMessage.Format(_T("%ld\t%s\t%s\t%ld,"), objectID, filedName, message, value.lVal);
		}
	}
	else
	{
		errorMessage.Format(_T("%ld\t%s\t%s"), objectID, filedName, message );
	}

	m_errorMessageList.push_back(errorMessage);
}

void CCheckRoadWalkNode::pushErrorMessage(CString message){
	m_errorMessageList.push_back(message);
}
