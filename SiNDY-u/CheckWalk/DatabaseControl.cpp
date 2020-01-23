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
#include "DatabaseControl.h"

using namespace std;
using namespace sindy;
using namespace sindy::schema;

CDatabaseControl::CDatabaseControl(void)
{
	// メッシュDB
	m_tableDatabasePairMap[table::c_base_mesh]=database::mesh;
	m_tableDatabasePairMap[table::c_city_mesh]=database::mesh;
	// 道路DB
	m_tableDatabasePairMap[table::c_road_link]=database::road;
	m_tableDatabasePairMap[table::c_road_node]=database::road;
	m_tableDatabasePairMap[table::c_segment_attr]=database::road;
	// 歩行者DB
	m_tableDatabasePairMap[table::c_walk_link]=database::walk;
	m_tableDatabasePairMap[table::c_walk_node]=database::walk;
	m_tableDatabasePairMap[table::c_height_node]=database::walk;
	m_tableDatabasePairMap[table::c_walk_code_list]=database::walk;
	m_tableDatabasePairMap[table::c_walk_nopassage]=database::walk;
	m_tableDatabasePairMap[table::c_walk_oneway]=database::walk;
	m_tableDatabasePairMap[table::c_walk_type]=database::walk;
	m_tableDatabasePairMap[table::c_link_relation]=database::walk;
	// スクランブルエリア
	m_tableDatabasePairMap[table::c_scramble_area]=database::walk;
	// 横断ポイント
	m_tableDatabasePairMap[table::c_pedxing]=database::walk;
	// 駅DB
	m_tableDatabasePairMap[table::c_station_gate]=database::station;
	// 地下街出入口POIDB
	m_tableDatabasePairMap[table::c_poi_point_org]=database::poi;
	// 地下街ポリゴン
	m_tableDatabasePairMap[table::c_underground_area]=database::underground_area;
}

CString CDatabaseControl::Init(const CString& roadDB, const CString& walkDB,const CString& stationDB, const CString& poiDB, const CString& underDB, const CString& meshDB)
{

	// ワークスペースオープン
	if( ! OpenWorkspace( roadDB,    m_workspaceMap[database::road] ) )
		return roadDB;
	if( ! OpenWorkspace( walkDB,    m_workspaceMap[database::walk] ) )
		return walkDB;
	if( ! OpenWorkspace( stationDB, m_workspaceMap[database::station] ) )
		return stationDB;
	if( ! OpenWorkspace( poiDB,		m_workspaceMap[database::poi] ) )
		return poiDB;
	if( ! OpenWorkspace( underDB,	m_workspaceMap[database::underground_area] ) )
		return underDB;
	if( ! OpenWorkspace( meshDB,    m_workspaceMap[database::mesh] ) )
		return meshDB;

	return _T("");
}

CString CDatabaseControl::OpenTables(void)
{
	for(table tableCode = static_cast<table>(0); tableCode < table_size; tableCode = static_cast<table>(static_cast<int>(tableCode)+1) )
	{
		if(m_dataType != walk_type::type_class::kDownConvert && (tableCode == table::c_scramble_area || tableCode == table::c_pedxing)) continue;

		if( ! InitialaizeTable( tableCode ) )
			return GetEnumString(tableCode);
	}
	return _T("");
};

bool CDatabaseControl::GetWorkspace(const database value, IWorkspacePtr& ipWorkspace)
{
	std::map<database, IWorkspacePtr>::iterator ite = m_workspaceMap.find(value);
	if(ite == m_workspaceMap.end())
		return false;

	ipWorkspace = ite->second;
	return true;
};

bool CDatabaseControl::GetTable(const table value, ITablePtr& ipTable)
{
	std::map<table, ITablePtr>::iterator ite = m_tableMap.find(value);
	if(ite == m_tableMap.end())
		return false;

	ipTable = ite->second;
	return true;
};

bool CDatabaseControl::GetFieldMap(const table value, sindy::CFieldMap& fieldMap)
{
	std::map<table, sindy::CFieldMap>::iterator ite = m_fieldMap.find(value);
	if(ite == m_fieldMap.end())
		return false;

	fieldMap = ite->second;
	return true;
};

// ワークスペースオープン
bool CDatabaseControl::OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
{
	ipWorkspace = create_workspace( name );
	if ( !ipWorkspace )
		return false;

	return true;
}

bool CDatabaseControl::InitialaizeTable( const table tableValue)
{
	if(! OpenTable( GetEnumString(tableValue), m_workspaceMap[m_tableDatabasePairMap[tableValue]], m_tableMap[tableValue] ))
		// 汎用版データのチェックモードでwalk_typeテーブルは存在しない可能性があることで、
	    // walk_typeテーブルを開けない場合でも、エラーとならないでtrueは戻り値として返す
		return tableValue==c_walk_type;
	if(! CreateFiledMap( m_tableMap[tableValue], m_fieldMap[tableValue] ))
		// 汎用版データのチェックモードでwalk_typeテーブルにデータ存在しない可能性があることで、
	    // walk_typeテーブルにフィールドマップ存在しない場合でも、エラーとならないでtrueは戻り値として返す
		return tableValue==c_walk_type;
	return true;
}

bool CDatabaseControl::OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
	if( !ipTable )
		return false;
	return true;
}

bool CDatabaseControl::CreateFiledMap(const ITablePtr& ipTable, CFieldMap& fieldMap )
{	
	errorcode::sindyErrCode err = fieldMap.CreateFieldMap( ipTable->GetFields() );
	if(err != 0)
		return false;

	return true;
}

CString CDatabaseControl::GetEnumString(const table value)
{
	CString name;
	switch (value)
	{
	case table::c_base_mesh:		name=basemesh::kTableName;break;
	case table::c_city_mesh:		name=citymesh::kTableName;break;
	case table::c_road_link:		name=road_link::kTableName;break;
	case table::c_road_node:		name=road_node::kTableName;break;
	case table::c_segment_attr:		name=segment_attr::kTableName;break;
	case table::c_walk_link:		name=walk_link::kTableName;break;
	case table::c_walk_node:		name=walk_node::kTableName;break;
	case table::c_height_node:		name=height_node::kTableName;break;
	case table::c_walk_code_list:	name=walk_code_list::kTableName;break;
	case table::c_link_relation:	name=link_relation::kTableName;break;
	case table::c_walk_nopassage:	name=walk_nopassage::kTableName;break;
	case table::c_walk_oneway:		name=walk_oneway::kTableName;break;
	case table::c_walk_type:        name=walk_type::kTableName;break;
	case table::c_station_gate:		name=station_gate_point::kTableName;break;
	case table::c_poi_point_org:	name=sindyk::poi_point_org::kTableName;break;
	case table::c_underground_area: name=underground_area::kTableName;break;
	case table::c_scramble_area:    name=scramble_area::kTableName;break;
	case table::c_pedxing:          name=pedxing::kTableName;break;
	default:						name = _T("");
	}

	return name;
};

// データタイプの追加
void CDatabaseControl::setDataType(sindy::schema::walk_type::type_class::ECode dataType)
{
	m_dataType = dataType;
}
