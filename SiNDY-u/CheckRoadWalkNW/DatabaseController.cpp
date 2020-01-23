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
#include "DatabaseController.h"
#include <sindy/workspace.h>

CDatabaseController::CDatabaseController(void)
{
	tmp_layer = NONE;
	tmp_buffer_layer = NONE;
}


CDatabaseController::~CDatabaseController(void)
{

}

void CDatabaseController::clear(void){
}



bool CDatabaseController::init(std::string roadDbConnection, std::string walkDbConnection, std::string refDbConnection, std::string roadSchema, std::string walkSchema, std::string refSchema, std::string inputLayer, std::string& errDb){

	CString dbConnect(roadDbConnection.c_str());
	m_ipRoadWorkspace = sindy::create_workspace(dbConnect);
	if(! m_ipRoadWorkspace) {
		errDb = roadDbConnection;
		return false;
	}

	CString walkDbConnect(walkDbConnection.c_str());
	m_ipWalkWorkspace = sindy::create_workspace(walkDbConnect);
	if(! m_ipWalkWorkspace) {
		errDb = walkDbConnection;
		return false;
	}

	CString refDbConnect(refDbConnection.c_str());
	m_ipRefWorkspace = sindy::create_workspace(refDbConnect);
	if(! m_ipRefWorkspace) {
		errDb = refDbConnection;
		return false;
	}

	m_roadSchema = CString(roadSchema.c_str());
	m_walkSchema = CString(walkSchema.c_str());
	m_refSchema = CString(refSchema.c_str());

	CString inLayer(inputLayer.c_str());
	if(inLayer != shp05::schema::road::road_code::kTableName){
		CString refTable;
		if (! m_refSchema.IsEmpty())
			refTable = m_refSchema + _T(".");
		refTable += sindy::schema::basemesh::kTableName;
		if(FAILED(((IFeatureWorkspacePtr)m_ipRefWorkspace)->OpenFeatureClass(CComBSTR(refTable), &m_ipMeshFeatureClass))) {
			errDb = refDbConnection;
			return false;
		}

		if(FAILED(m_ipMeshFeatureClass->FindField(CComBSTR(sindy::schema::basemesh::kMeshCode), &m_meshcodeIdx))) {
			errDb = refDbConnection;
			return false;
		}
	}

	CString roadTableName, walkTableName;
	if(inLayer == shp05::schema::road::road_walk_link::kSuffix)
	{
		roadTableName = sindy::schema::road_link::kTableName;
		walkTableName = sindy::schema::walk_link::kTableName;
	}
	else if(inLayer == shp05::schema::road::road_walk_node::kSuffix)
	{
		roadTableName = sindy::schema::road_node::kTableName;
		walkTableName = sindy::schema::walk_node::kTableName;
	}
	else if(inLayer == shp05::schema::road::road_walk_nopassage_condition::kSuffix)
	{
		roadTableName = sindy::schema::time_nopassage::kTableName;
		walkTableName = sindy::schema::walk_nopassage::kTableName;
	}
	else if(inLayer == shp05::schema::road::road_walk_oneway_condition::kSuffix)
	{
		roadTableName = sindy::schema::time_oneway::kTableName;
		walkTableName = sindy::schema::walk_oneway::kTableName;
	}
	else if(inLayer == shp05::schema::road::road_code::kTableName)
	{
		roadTableName = sindy::schema::road_code_list::kTableName;
		walkTableName = sindy::schema::walk_code_list::kTableName;
	}
	
	databaseInfo road_db,walk_db; 
	if(! init_db(m_ipRoadWorkspace, roadTableName, m_roadSchema, road_db)) {
		errDb = roadDbConnection;
		return false;
	}
	if(! init_db(m_ipWalkWorkspace, walkTableName, m_walkSchema, walk_db)) {
		errDb = walkDbConnection;
		return false;
	}
	
	m_dbList.push_back(road_db);
	m_dbList.push_back(walk_db);

	return true;
}

bool CDatabaseController::init_db(const IWorkspacePtr ipWorkspace , CString tableName, CString schema, databaseInfo& dbInfo ){
	
	CString fullTableName;
	if (! schema.IsEmpty())
		fullTableName = schema + _T(".");
	fullTableName += tableName;

	ITablePtr ipTable;
	if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenTable(CComBSTR(fullTableName), &ipTable)))
		return false;


	CString sql;
	LAYER layer = NONE;
	if(tableName == sindy::schema::road_link::kTableName)
	{
		sql = ROAD_LINK_WHERE;
		layer = ROAD_LINK;
	}
	else if(tableName == sindy::schema::road_node::kTableName)
	{
		sql = ROAD_NODE_WHERE;
		layer = ROAD_NODE;
	}
	else if(tableName == sindy::schema::walk_link::kTableName)
	{
		sql = WALK_LINK_WHERE;
		layer = WALK_LINK;
	}
	else if(tableName == sindy::schema::walk_node::kTableName)
	{
		sql = WALK_NODE_WHERE;
		layer = WALK_NODE;
	}
	else if(tableName == sindy::schema::time_nopassage::kTableName)
	{
		sql = TIME_NOPASSAGE_WHERE;
		layer = TIME_NOPASSAGE;
	}
	else if(tableName == sindy::schema::walk_nopassage::kTableName)
	{
		sql = WALK_NOPASSAGE_WHERE;
		layer = WALK_NOPASSAGE;
	}
	else if(tableName == sindy::schema::time_oneway::kTableName)
	{
		sql = TIME_ONEWAY_WHERE;
		layer = TIME_ONEWAY;
	}
	else if(tableName == sindy::schema::walk_oneway::kTableName)
	{
		sql = WALK_ONEWAY_WHERE;
		layer = WALK_ONEWAY;
	}
	else if(tableName == sindy::schema::road_code_list::kTableName)
	{
		sql = ROAD_CODE_LIST_WHERE;
		layer = ROAD_CODE_LIST;
	}
	else if(tableName == sindy::schema::walk_code_list::kTableName)
	{
		sql = WALK_CODE_LIST_WHERE;
		layer = WALK_CODE_LIST;
	}


	
	FIELD_INDEX fieldIdx;
	CDBUtils::createFieldIndex(ipTable, fieldIdx);
	
	dbInfo.eLayer = layer;
	dbInfo.ipTable = ipTable;
	dbInfo.sql = sql;
	dbInfo.fieldIndex = fieldIdx;
	
	return true;
}

bool CDatabaseController::setArea(std::string cMeshcode){
	
	// メッシュ検索
	MAP_FEATURE meshFeasures;
	if(! getMesh(cMeshcode, meshFeasures))
		return false;

	for(auto meshFeature:meshFeasures)
	{
		IGeometryPtr ipMeshShape;
		if(FAILED(meshFeature.second.m_T->get_ShapeCopy(&ipMeshShape)))
			return false;

		for(unsigned int i= 0; i < m_dbList.size(); ++i)
		{
			databaseInfo& dbInfo = m_dbList[i];
			if( ! createFeatureMap(ipMeshShape, dbInfo, dbInfo.features))
				return false;
		}
	}

	return true;
}


bool CDatabaseController::setAreaBuffer(std::string cMeshcode, LAYER layer){
	
	// メッシュ検索
	MAP_FEATURE meshFeasures;
	if(! getMesh(cMeshcode, meshFeasures))
		return false;

	for(auto meshFeature:meshFeasures)
	{
		IGeometryPtr ipMeshShape;
		if(FAILED(meshFeature.second.m_T->get_ShapeCopy(&ipMeshShape)))
			return false;

		for(unsigned int i= 0; i < m_dbList.size(); ++i)
		{
			databaseInfo& dbInfo = m_dbList[i];
			if(dbInfo.eLayer == layer)
			{
				IGeometryPtr ipMeshBufferGeo;
				if(! getBuffer(ipMeshShape, ipMeshBufferGeo))
					return false;

				if( ! createFeatureMap(ipMeshBufferGeo, dbInfo, dbInfo.buffer_features))
					return false;
			}
		}
	}

	return true;
}


bool CDatabaseController::createFeatureMap(const IGeometryPtr meshGeometry, databaseInfo dbInfo, MAP_FEATURE& features){

	ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
	if(FAILED(ipSpatialFilter->put_SpatialRel(esriSpatialRelIntersects)))
		return false;
	CComBSTR shapeFieldName;
	if(FAILED(((IFeatureClassPtr)dbInfo.ipTable.m_T)->get_ShapeFieldName(&shapeFieldName)))
		return false;
	if(FAILED(ipSpatialFilter->put_GeometryField(shapeFieldName)))
		return false;
	if(FAILED(ipSpatialFilter->put_WhereClause(CComBSTR(dbInfo.sql))))
		return false;
	if(FAILED(ipSpatialFilter->putref_Geometry(meshGeometry)))
		return false;

	if( ! CDBUtils::createFeatureMap((IFeatureClassPtr)dbInfo.ipTable.m_T, dbInfo.fieldIndex, ipSpatialFilter, features))
		return false;

	return true;
}


bool CDatabaseController::setAreaRecords(std::string cMeshcode)
{
	CString meshcode(cMeshcode.c_str());

	for(unsigned int i= 0; i < m_dbList.size(); ++i)
	{
		databaseInfo& dbInfo = m_dbList[i];

		CString wheresql;
		wheresql.Format(dbInfo.sql, meshcode);

		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		if(FAILED(ipQueryFilter->put_WhereClause(CComBSTR(wheresql))))
			return false;

		MAP_ROW rows;
		if( ! CDBUtils::createRowMap((ITablePtr)dbInfo.ipTable.m_T, dbInfo.fieldIndex, ipQueryFilter, rows))
			return false;

		for(auto row:rows)
		{
			MAP_VALUE mapValue;
			if(dbInfo.eLayer == TIME_NOPASSAGE)
			{
				if( ! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kLinkID, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kStartMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kStartDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kEndMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kEndDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kStartHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kStartMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kEndHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kEndMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_nopassage::kDayOfWeek, true, mapValue))
					return false;
			}
			else if(dbInfo.eLayer == WALK_NOPASSAGE)
			{
				if( ! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kLinkID, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kStartMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kStartDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kEndMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kEndDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kStartHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kStartMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kEndHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kEndMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_nopassage::kDayOfWeek, true, mapValue))
					return false;
			}
			else if(dbInfo.eLayer == TIME_ONEWAY)
			{
				if( ! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kLinkID, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kLinkDir, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kStartMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kStartDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kEndMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kEndDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kStartHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kStartMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kEndHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kEndMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::time_oneway::kDayOfWeek, true, mapValue))
					return false;
			}
			else if(dbInfo.eLayer == WALK_ONEWAY)
			{
				if( ! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kLinkID, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kLinkDir, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kStartMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kStartDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kEndMonth, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kEndDay, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kStartHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kStartMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kEndHour, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kEndMin, true, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_oneway::kDayOfWeek, true, mapValue))
					return false;
			}
			else
				return false;

			dbInfo.rows.insert(mapValue);	
		}
	}

	return true;
}


bool CDatabaseController::setRoadcode(void)
{
	for(unsigned int i= 0; i < m_dbList.size(); ++i)
	{
		databaseInfo& dbInfo = m_dbList[i];

		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		MAP_ROW rows;
		if( ! CDBUtils::createRowMap((ITablePtr)dbInfo.ipTable.m_T, dbInfo.fieldIndex, ipQueryFilter, rows))
			return false;

		for(auto row:rows)
		{
			CString key;
			MAP_VALUE mapValue;
			if(dbInfo.eLayer == ROAD_CODE_LIST)
			{
				if( ! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::road_code_list::kRoadCode, false, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::road_code_list::kLineClass, false, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::road_code_list::kDisplayKanji, false, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::road_code_list::kDisplayYomi, false, mapValue))
					return false;
				CComVariant roadcode = mapValue.find(sindy::schema::road_code_list::kRoadCode)->second;
				CComVariant roadtype = mapValue.find(sindy::schema::road_code_list::kLineClass)->second;
				roadcode.ChangeType(VT_I4);
				roadtype.ChangeType(VT_I4);
				key.Format(_T("%ld,%ld"), roadcode.lVal, roadtype.lVal);
			}
			else if(dbInfo.eLayer == WALK_CODE_LIST)
			{
				if( ! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_code_list::kWalkCode, false, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_code_list::kNameKanji, false, mapValue) ||
					! setValue(dbInfo, (_IRowPtr)row.second.m_T, sindy::schema::walk_code_list::kNameYomi, false, mapValue))
					return false;
				CComVariant roadcode = mapValue.find(sindy::schema::walk_code_list::kWalkCode)->second;
				roadcode.ChangeType(VT_I4);
				key.Format(_T("%ld"), roadcode.lVal);
			}
			else
				return false;

			dbInfo.roadcode_rows.insert(std::pair<CString, MAP_VALUE>(key, mapValue));
		}
	}

	return true;
}

bool CDatabaseController::setValue(const databaseInfo& dbInfo, const _IRowPtr& ipRow, const CString& field, const bool bSTR, MAP_VALUE& mapValue)
{
	CComVariant value;
	if( FAILED(ipRow->get_Value(dbInfo.fieldIndex.find(field)->second, &value)))
		return false;

	if(bSTR)
		value.ChangeType(VT_BSTR);

	mapValue.insert(std::pair<CString, CAdapt<CComVariant>>(field, value));
	return true;
}

bool CDatabaseController::getFeature(LAYER layer,const long objectId, IFeaturePtr& ipFeature)
{
	if(tmp_layer != layer){
		
		for(databaseInfo dbInfo:m_dbList)
		{
			if(dbInfo.eLayer == layer)
			{
				tmp_features = dbInfo.features;
				break;
			}
		}
		tmp_layer = layer;
	}

	if( !tmp_features.count(objectId))
		return false;

	ipFeature= tmp_features.find(objectId)->second;

	return true;
}

bool CDatabaseController::getBufferFeature(LAYER layer, const long objectId, IFeaturePtr& ipFeature)
{
	if(tmp_buffer_layer != layer){
		
		for(databaseInfo dbInfo:m_dbList)
		{
			if(dbInfo.eLayer == layer)
			{
				tmp_buffer_features = dbInfo.buffer_features;
				break;
			}
		}
		tmp_buffer_layer = layer;
	}

	if( !tmp_buffer_features.count(objectId))
		return false;

	ipFeature= tmp_buffer_features.find(objectId)->second;

	return true;
}

bool CDatabaseController::findRow(LAYER layer, MAP_VALUE& mapValue)
{
	if(tmp_layer != layer){
		
		for(databaseInfo dbInfo:m_dbList)
		{
			if(dbInfo.eLayer == layer)
			{
				tmp_rows = dbInfo.rows;
				break;
			}
		}
		tmp_layer = layer;
	}

	if( !tmp_rows.count(mapValue))
		return false;

	return true;
}

bool CDatabaseController::findRoadCode(LAYER layer, CString key, MAP_VALUE& mapValue)
{
	if(tmp_layer != layer){
		
		for(databaseInfo dbInfo:m_dbList)
		{
			if(dbInfo.eLayer == layer)
			{
				tmp_roadcode_rows = dbInfo.roadcode_rows;
				break;
			}
		}
		tmp_layer = layer;
	}

	if( !tmp_roadcode_rows.count(key))
		return false;

	mapValue= tmp_roadcode_rows.find(key)->second;

	return true;
}

bool CDatabaseController::getFieldIndex(LAYER layer, FIELD_INDEX& mfields){

	for(databaseInfo dbInfo:m_dbList)
	{
		if(dbInfo.eLayer == layer)
		{
			mfields = dbInfo.fieldIndex;
			break;
		}
	}
	if(mfields.empty())
		return false;

	return true;
}

bool CDatabaseController::getRecordCount(LAYER layer,long& count)
{
	MAP_FEATURE features;
	for(databaseInfo dbInfo:m_dbList)
	{
		if(dbInfo.eLayer == layer)
		{
			features = dbInfo.features;
			break;
		}
	}
	count = features.size();
	
	return true;
}



bool CDatabaseController::getFeatures(std::string cMeshcode, const LAYER layer, MAP_FEATURE& MapFeatures, FIELD_INDEX& FiledIndex)
{
	for(databaseInfo dbInfo:m_dbList)
	{
		if(dbInfo.eLayer == layer)
		{
			MapFeatures = dbInfo.features;
			FiledIndex = dbInfo.fieldIndex;
			break;
		}
	}
	if(FiledIndex.empty())
	{
		if( !getNewFeatures(cMeshcode, layer, MapFeatures,FiledIndex))
			return false;
	}

	return true;
}

bool CDatabaseController::getNewFeatures(std::string cMeshcode, const LAYER layer, MAP_FEATURE& features, FIELD_INDEX& filedIndex)
{
	// レイヤからワークスペース、テーブル名、WHERE句取得
	IWorkspacePtr ipWorkspace;
	CString strTableName, strWhere;
	if(layer == WALK_LINK)
	{
		ipWorkspace = m_ipWalkWorkspace;
		if (! m_walkSchema.IsEmpty())
			strTableName = m_walkSchema + _T(".");
		strTableName += sindy::schema::walk_link::kTableName;
		strWhere = WALK_LINK_WHERE;
	}
	else
	{
		return false;
	}

	// メッシュ検索
	MAP_FEATURE meshFeasures;
	if(! getMesh(cMeshcode, meshFeasures))
		return false;

	// メッシュのSHAPEフィールド名取得
	CComBSTR shapeFieldName;
	if(FAILED(m_ipMeshFeatureClass->get_ShapeFieldName(&shapeFieldName)))
		return false;

	// フィーチャオープン
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(strTableName),&ipFeatureClass)))
		return false;

	if(! CDBUtils::createFieldIndex(ipFeatureClass, filedIndex))
		return false;

	for(auto meshFeature:meshFeasures)
	{
		// メッシュ矩形取得
		IGeometryPtr ipMeshShape;
		if(FAILED(meshFeature.second.m_T->get_ShapeCopy(&ipMeshShape)))
			return false;

		// メッシュ矩形のフィーチャ検索
		ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
		if(FAILED(ipSpatialFilter->put_SpatialRel(esriSpatialRelIntersects)))
			return false;

		if(FAILED(ipSpatialFilter->put_GeometryField(shapeFieldName)))
			return false;

		if(FAILED(ipSpatialFilter->putref_Geometry(ipMeshShape)))
			return false;

		if(FAILED(ipSpatialFilter->put_WhereClause(CComBSTR(strWhere))))
			return false;

		if(! CDBUtils::createFeatureMap(ipFeatureClass, filedIndex, ipSpatialFilter, features))
			return false;
	}
	return true;
}

bool CDatabaseController::getMesh(const std::string& cMeshcode, MAP_FEATURE& feasures)
{
	// メッシュ矩形検索
	CString sql, meshcode(cMeshcode.c_str());
	sql.Format(_T("%s = %s"), sindy::schema::basemesh::kMeshCode, meshcode);

	ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
	if(FAILED(ipSpatialFilter->put_WhereClause(CComBSTR(sql))))
		return false;

	FIELD_INDEX filedIndex;
	if(! CDBUtils::createFieldIndex(m_ipMeshFeatureClass, filedIndex))
		return false;

	if(! CDBUtils::createFeatureMap(m_ipMeshFeatureClass, filedIndex, ipSpatialFilter, feasures))
		return false;

	return true;
}

bool CDatabaseController::meter2Coordinate(double meter, const IGeometryPtr& meshGeometry, double& coordinate)
{
	IPointPtr ipPoint;
	if (FAILED(((IPointCollectionPtr)meshGeometry)->get_Point(0, &ipPoint)))
		return false;

	double dX(0.0f), dY(0.0f);
	ipPoint->QueryCoords(&dX, &dY);
	
	double dLon(0.0f), dLat(0.0f);
	// X座標のみで計算する
	g_crdCnv.GetLLPt(dX, dY, meter, 0, &dLon, &dLat);

	coordinate = abs(dLon - dX);
	return true;
}


bool CDatabaseController::getBuffer(const IGeometryPtr& meshGeometry, IGeometryPtr& ipBuffer, double bufMeter)
{
	double bufCoordinate = 0.0;
	if (! meter2Coordinate(bufMeter, meshGeometry, bufCoordinate))
		return false;

	if (FAILED(((ITopologicalOperatorPtr)meshGeometry)->Buffer(bufCoordinate, &ipBuffer)))
		return false;

	return true;
}
