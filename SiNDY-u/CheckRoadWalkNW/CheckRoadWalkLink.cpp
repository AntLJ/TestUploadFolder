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
#include "CheckRoadWalkLink.h"


CCheckRoadWalkLink::CCheckRoadWalkLink(void)
{
}


CCheckRoadWalkLink::~CCheckRoadWalkLink(void)
{
	CCheckRoadWalkNode::clear();
}


bool CCheckRoadWalkLink::checkFormat(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, CDatabaseController dbCtrl){
	
	m_errorMessageList.clear();

	IFeatureCursorPtr ipFeatureCursor;
	if(FAILED(ipFeatureClass->Search(NULL, VARIANT_FALSE, &ipFeatureCursor)))
		return false;

	IFeaturePtr ipFeature;
	CDatabaseController::LAYER layer = CDatabaseController::NONE, tmpLayer = CDatabaseController::NONE;
	FIELD_INDEX dbFieldIndex;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK){

		CComVariant objectIDValue;
		if( FAILED(ipFeature->get_Value(fieldIndex.find(sindy::schema::kObjectID)->second, &objectIDValue)))
		{
			pushErrorMessage(_T("OBJECTID���擾�ł��܂���"));
			return false;
		}
		objectIDValue.ChangeType(VT_I4);
		long objectid = objectIDValue.lVal;

		bool walkRecord = false;
		if(objectid < WALK_OBJECTID )
			layer = CDatabaseController::ROAD_LINK;
		else 
		{
			layer = CDatabaseController::WALK_LINK;
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
		}

		IFeaturePtr ipDbFeature;

		long search_objectid = 0;
		if(walkRecord)
			search_objectid = objectid - WALK_OBJECTID;
		else
			search_objectid = objectid;
		
		if(! dbCtrl.getFeature(layer, search_objectid, ipDbFeature))
		{
			pushColumnErrorMessage(objectid, sindy::schema::kObjectID, _T("DB�����r���R�[�h���擾�ł��܂���"), search_objectid, 0);
			continue;
		}

		// FROM�m�[�hID
		if(! checkNodeID(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kFromNodeId, fieldIndex, 
			ipDbFeature, sindy::schema::walk_link::kFromNodeID,sindy::schema::road_link::kFromNodeID, dbFieldIndex))
			continue;

		// TO�m�[�hID
		if(! checkNodeID(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kToNodeId, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kToNodeID, sindy::schema::road_link::kToNodeID, dbFieldIndex))
			continue;

		// ���H�\�����
		if(! checkDBValueFixedValue(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kRoadClass_c, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kRoadClass, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// ���H�o�H���
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kNaviClass_c, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kNaviClass,dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// ���H�����N���
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kLinkClass_c, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kMainLinkClass,dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// ���s�Ҏ��
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kWalkClass_c, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kWalkClass, dbFieldIndex,
			false, SHAPE_INT_DEFAULT))
			continue;

		// ���H�Ǘ���
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kManager_c, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kRoadManager,dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// ���H�����敪
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kWidth_c, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kRoadWidth,dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// �L�摶�݃t���O
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kWideArea_f, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kWideArea, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// �����t���O
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kIsland_f, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kIsland, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// �o�C�p�X�t���O
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kBypass_f, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kBypass, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// �����Ԑ�p���t���O
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kCaronly_f, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kCarOnly, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// �����E�����ԍ�
		if(! checkDBValueFixedValue(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kRoadNo, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kRoadNo, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;
		
		// ���H�R�[�h
		if(! checkDBValue(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kRoadCode, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kWalkCode, sindy::schema::road_link::kRoadCode, dbFieldIndex))
			continue;

		// �����N�Ԑ���
		if(! checkDBValueFixedValue(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kLaneCount, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kLaneCount, dbFieldIndex,
			true, SHAPE_INT_DEFAULT))
			continue;

		// �K�w�ړ����
		if(! checkDBValueFixedValue(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kFloorMove_c, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kFloorMoveClass, dbFieldIndex,
			false, SHAPE_INT_DEFAULT))
			continue;

		// ���s�ґ���
		CString shapeFieldName = shp05::schema::road::road_walk_link::kWalkAttr_c;
		CComVariant shapeValue;
		if( !getFeatureValue(objectid, ipFeature, shapeFieldName, fieldIndex, shapeValue))
			continue;
		shapeValue.ChangeType(VT_I4);
		if(walkRecord)
		{
			int dbWalkAttr = 0;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kAutoWalkFlag, dbFieldIndex,
				eAUTOWALK, dbWalkAttr))
				continue;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kStairFlag, dbFieldIndex,
				eSTAIR, dbWalkAttr))
				continue;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kSlopeFlag, dbFieldIndex,
				eSLOPE, dbWalkAttr))
				continue;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kEscalatorFlag, dbFieldIndex,
				eESCALATOR, dbWalkAttr))
				continue;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kArcadeFlag, dbFieldIndex,
				eARCADE, dbWalkAttr))
				continue;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kTunnelFlag, dbFieldIndex,
				eTUNNEL, dbWalkAttr))
				continue;
			if( !getWalkAttr(objectid, ipDbFeature, sindy::schema::walk_link::kOpenAreaFlag, dbFieldIndex,
				eOPENAREA, dbWalkAttr))
				continue;
			
			if(shapeValue.lVal != dbWalkAttr)
				pushColumnErrorMessage(objectid, shapeFieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, CComVariant(dbWalkAttr));
		}
		else
		{
			if(shapeValue.lVal != SHAPE_INT_DEFAULT)
				pushColumnErrorMessage(objectid, shapeFieldName, MESSAGE_NOT_FIXED_VALUE, shapeValue, CComVariant(SHAPE_INT_DEFAULT));
		}

		// �����Ԑ�p���t���O
		if(! checkDBValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kNoPassage_c, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kNoPassage, sindy::schema::road_link::kNoPassage, dbFieldIndex))
			continue;

		// ����ʍs�t���O
		if(! checkDBValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kOneway_c, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kOneway, sindy::schema::road_link::kOneway, dbFieldIndex))
			continue;

		// ���s�\��ʃR�[�h
		if(! checkDBValueFixedValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kWalkable_c, fieldIndex,
			ipDbFeature, sindy::schema::road_link::kWalkable, dbFieldIndex,
			true, sindy::schema::road_link::walkable_class::kWalkOnly))
			continue;
		
		// �H�ʑf�ރR�[�h
		if(! checkDBValueFixedValue(objectid, 
			ipFeature, shp05::schema::road::road_walk_link::kSurface_c, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kRoadSurface, dbFieldIndex,
			false, SHAPE_INT_DEFAULT))
			continue;

		// �Ԉ֎q�ʍs�R�[�h
		if(! checkDBValue(objectid,
			ipFeature, shp05::schema::road::road_walk_link::kWheelChair_c, fieldIndex,
			ipDbFeature, sindy::schema::walk_link::kWheelChair, sindy::schema::road_link::kWheelChair, dbFieldIndex))
			continue;

		if(!checkGeomPoints(objectid, ipFeature, ipDbFeature))
			continue;

	}
	return true;
}

bool CCheckRoadWalkLink::checkNodeID(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex, const IFeaturePtr ipDbFeature,CString dbRoadFieldName, CString dbWalkFieldName, FIELD_INDEX dbFieldIndex){

	CComVariant shapeValue, dbValue;
	if( !getFeatureValue(objectID, ipFeature, fieldName, fieldIndex, shapeValue))
		return false;
	if(objectID > WALK_OBJECTID)
	{
		shapeValue.ChangeType(VT_I4);
		// ���s�҃m�[�h�̏ꍇ
		if(shapeValue.lVal > WALK_OBJECTID)
		{
			if( !getFeatureValue(objectID, ipDbFeature, dbWalkFieldName, dbFieldIndex, dbValue))
				return false;
			shapeValue.ChangeType(VT_I4);
			if(shapeValue.lVal != dbValue.lVal + WALK_OBJECTID)
				pushColumnErrorMessage(objectID, fieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
		}
		// ���H�m�[�h�̏ꍇ
		else
		{
			//CCheckRoadWalkNW::checkExist�ɂăm�[�h���C���ɑΏۃm�[�h�����݂��邱�Ƃ��m�F���Ă��邽��
			//TODO:�u���m�[�h���u3:���HNW�ڑ��m�[�h�v�݂̂ł��邱�Ƃ��m�F���������悢
			if(shapeValue.lVal == SHAPE_INT_DEFAULT || shapeValue.lVal == SHAPE_INT_NULL)
				pushColumnErrorMessage(objectID, fieldName, MESSAGE_IS_NULL, shapeValue, 0);
		}
	}
	else
	{
		if( !getFeatureValue(objectID, ipDbFeature, dbRoadFieldName, dbFieldIndex, dbValue))
			return false;
		if(shapeValue != dbValue)
			pushColumnErrorMessage(objectID, fieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
	}
	return true;
}

bool CCheckRoadWalkLink::checkDBValue(long objectID, const IFeaturePtr ipFeature, CString fieldName, FIELD_INDEX fieldIndex, const IFeaturePtr ipDbFeature, CString dbWalkFieldName, CString dbRoadFieldName, FIELD_INDEX dbFieldIndex)
{
	CComVariant shapeValue;
	if( !getFeatureValue(objectID, ipFeature, fieldName, fieldIndex, shapeValue))
		return false;
	
	CString dbFieldName;
	if(objectID > WALK_OBJECTID)
		dbFieldName = dbWalkFieldName;
	else
		dbFieldName = dbRoadFieldName;

	CComVariant dbValue;
	if( !getFeatureValue(objectID, ipDbFeature, dbFieldName, dbFieldIndex, dbValue))
		return false;
	if( ! compShapeDbValue(shapeValue, dbValue))
		pushColumnErrorMessage(objectID, fieldName, MESSAGE_NOT_EQUALS_VALUE, shapeValue, dbValue);
	return true;
}



bool CCheckRoadWalkLink::getWalkAttr(long objectID, const IFeaturePtr ipDbFeature, CString dbFieldName, FIELD_INDEX dbFieldIndex, walkAttrType walkAttTyp, int& walkAttrValue)
{
	CComVariant dbValue;
	if( !getFeatureValue(objectID, ipDbFeature, dbFieldName, dbFieldIndex, dbValue))
		return false;

	dbValue.ChangeType(VT_I4);
	walkAttrValue += dbValue.lVal * walkAttTyp;

	return true;
}
;
