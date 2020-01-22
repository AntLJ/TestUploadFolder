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
#include "CheckRailwayNW.h"
#include "util.h"
#include "value.h"

#include <TDC/useful_headers/str_util.h>
#include <sindy/workspace.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace sindy::schema::sj;
using namespace uh;

namespace
{
	crd_cnv g_crd;
}


// ���s
bool CCheckRailwayNW::execute()
{
	// ������
	if( !init() )
		return false;

	// �w�b�_��񓙏o��
	m_output.OutputHeader();
	m_output.OutputLog( err_level::info, err_code::StrRailNWDB,      m_strRailNwDB );
	m_output.OutputLog( err_level::info, err_code::StrRailPoiDB,     m_strRailPoiDB );
	m_output.OutputLog( err_level::info, err_code::StrStationDB,     m_strStationDB );
	m_output.OutputLog( err_level::info, err_code::StrMeshDB,        m_strMeshDB );
	m_output.OutputLog( err_level::info, err_code::StrLogFile,       m_strLog );
	m_output.OutputLog( err_level::info, err_code::StrMeshList,      m_strMeshList );
	m_output.OutputLog( err_level::info, err_code::StrDistThreshold, m_strDistThreshold );
	m_output.OutputLog( err_level::info, err_code::StrNeiThreshold,  m_strNeighborThreshold);
	m_output.OutputLog( err_level::info, err_code::StrStartTime,     util::GetCurTime() );

	// �֘A�e�[�u���ގ擾
	if( ! GetRelRecAll(   m_ipRelRailLinkTable, m_relRailLinkFM, rel_railway_link::kTableName, m_relRailwayLink ) ) return false;
	if( ! GetRelRecAll(   m_ipRelRailNodeTable, m_relRailNodeFM, rel_railway_node::kTableName, m_relRailwayNode ) ) return false;
	if( ! GetOtherRecAll( m_ipJoinLineTable,    m_joinLineFM,    join_line_info::kTableName,   m_joinLineInfo ) )   return false;
	if( ! GetOtherRecAll( m_ipRailwayLineTable, m_railwayLineFM, railway_line::kTableName,     m_railwayLine ) )    return false;

	// ���b�V�����Ƃ̏���
	for( auto& mesh : m_meshList ){

		CString strMesh;
		strMesh.Format( _T("%ld"), mesh );

		m_output.OutputStdErr( err_level::info, err_code::StrStart, strMesh );
		m_output.OutputLog( err_level::info, err_code::StrStart, strMesh );

		// �א�9���b�V���R�[�h�擾
		set<long> rin9Mesh;
		Get9Mesh( mesh, rin9Mesh );

		// 9���b�V�����̃|���S���擾
		IGeometryPtr ipMeshGeo, ip9MeshGeo;
		if( ! GetMeshPolygons( mesh, rin9Mesh, ipMeshGeo, ip9MeshGeo ) ){
			m_output.OutputStdErr( err_level::fatal, err_code::GetNineMeshFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetNineMeshFail, strMesh );
			continue;
		}

		// �S�������N�ƓS���m�[�h�擾��RTree�쐬
		map<long, GeoClass> railLinkMap, railNodeMap;
		IDMAP nodeNodeIDs, nodeLinkIDs;
		if( ! GetRailwayInMesh( ip9MeshGeo, railLinkMap, railNodeMap, nodeNodeIDs, nodeLinkIDs )){
			m_output.OutputStdErr( err_level::fatal, err_code::GetRailFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetRailFail, strMesh );
			continue;
		}
		BGRTree railLinkRTree, railNodeRTree;
		createRTree( railLinkMap, railLinkRTree );
		createRTree( railNodeMap, railNodeRTree );

		// ���̑��`��n�擾
		map<long, GeoClass> staPointMap, baseStaMap;
		if( ! GetObjInMesh( ip9MeshGeo, m_ipStationPointFC, m_stationPointFM, staPointMap ) ){
			m_output.OutputStdErr( err_level::fatal, err_code::GetStaPoiFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetStaPoiFail, strMesh );
			continue;
		}
		if( ! GetObjInMesh( ip9MeshGeo, m_ipBaseStationFC, m_baseStationFM, baseStaMap ) ){
			m_output.OutputStdErr( err_level::fatal, err_code::GetBaseStaFail, strMesh );
			m_output.OutputLog( err_level::fatal, err_code::GetBaseStaFail, strMesh );
			continue;
		}
		BGRTree staPointRTree, baseStaRTree;
		createRTree( staPointMap, staPointRTree );
		createRTree( baseStaMap, baseStaRTree );

		// ���b�V���P�ʂ̃`�F�b�N����
		MeshCheck( mesh, ipMeshGeo, railLinkMap, railNodeMap, staPointMap, baseStaMap, railLinkRTree, railNodeRTree, staPointRTree, baseStaRTree, nodeNodeIDs, nodeLinkIDs );
	}

	// �N�G���`�F�b�N����
	m_output.OutputStdErr( err_level::info, err_code::StrQueryStart, _T("") );
	m_output.OutputLog( err_level::info, err_code::StrQueryStart, _T("") );

	QueryCheck();

	// �I�����ԏo��
	m_output.OutputLog( err_level::info, err_code::StrEndTime, util::GetCurTime() );
	m_output.Close();

	return true;
}

// ������
bool CCheckRailwayNW::init()
{
	// ���O�t�@�C���N���X�I�[�v��
	if( ! m_output.Open( m_strLog ) ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenLogFail, m_strLog );
		return false;
	}

	// ���[�N�X�y�[�X�I�[�v��
	if( ! OpenWorkspace( m_strRailNwDB,  m_ipRailNwWorkspace  ) ) return false;
	if( ! OpenWorkspace( m_strRailPoiDB, m_ipRailPoiWorkspace ) ) return false;

	if( ! OpenWorkspace( m_strMeshDB,    m_ipMeshWorkspace    ) ) return false;
	if( ! OpenWorkspace( m_strStationDB, m_ipStationWorkspace ) ) return false;

	// �t�B�[�`���N���X�E�e�[�u���I�[�v��
	if( ! OpenFeatureClass( railway_link::kTableName,     m_ipRailNwWorkspace,  m_ipRailLinkFC,       m_railLinkFM     ) ) return false;
	if( ! OpenFeatureClass( railway_node::kTableName,     m_ipRailNwWorkspace,  m_ipRailNodeFC,       m_railNodeFM     ) ) return false;
	if( ! OpenTable(        rel_railway_link::kTableName, m_ipRailNwWorkspace,  m_ipRelRailLinkTable, m_relRailLinkFM  ) ) return false;
	if( ! OpenTable(        rel_railway_node::kTableName, m_ipRailNwWorkspace,  m_ipRelRailNodeTable, m_relRailNodeFM  ) ) return false;

	if( ! OpenFeatureClass( station_point::kTableName,    m_ipRailPoiWorkspace, m_ipStationPointFC,   m_stationPointFM ) ) return false;
	if( ! OpenTable(        railway_line::kTableName,     m_ipRailPoiWorkspace, m_ipRailwayLineTable, m_railwayLineFM  ) ) return false;
	if( ! OpenTable(        join_line_info::kTableName,   m_ipRailPoiWorkspace, m_ipJoinLineTable,    m_joinLineFM     ) ) return false;

	if( ! OpenFeatureClass( base_station::kTableName,     m_ipStationWorkspace, m_ipBaseStationFC,    m_baseStationFM  ) ) return false;

	if( ! OpenFeatureClass( basemesh::kTableName,         m_ipMeshWorkspace,    m_ipBaseMeshFC,       m_baseMeshFM     ) ) return false;

	// �K�v�ȃR�[�h�l�h���C��map���擾���Ă���
	util::GetDomainMap( railway_link::kRailwayClass_C, m_railLinkFM, m_railLinkClassMap );
	util::GetDomainMap( railway_link::kDispScale_C,    m_railLinkFM, m_dispScaleClassMap );
	util::GetDomainMap( railway_node::kNodeClass_C,    m_railNodeFM, m_railNodeClassMap );


	// ���b�V�����X�g�ǂݍ���
	if( !util::loadMeshList( m_strMeshList, m_meshList ) ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList );
		m_output.OutputLog( err_level::fatal, err_code::OpenMeshFileFail, m_strMeshList );
		return false;
	}

	return true;
}

// ���[�N�X�y�[�X�I�[�v��
bool CCheckRailwayNW::OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
{
	ipWorkspace = create_workspace( name );
	if( !ipWorkspace ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenWsFail, name );
		m_output.OutputLog( err_level::fatal, err_code::OpenWsFail, name );
		return false;
	}
	return true;
}

// �t�B�[�`���N���X�I�[�v�����t�B�[���h�}�b�v�쐬
bool CCheckRailwayNW::OpenFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t( name ), &ipFeatureClass );
	if( !ipFeatureClass ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenFcFail, name );
		m_output.OutputLog( err_level::fatal, err_code::OpenFcFail, name );
		return false;
	}
	fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

	return true;
}

// �e�[�u���I�[�v�����t�B�[���h�}�b�v�쐬
bool CCheckRailwayNW::OpenTable(const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, CFieldMap& fieldMap )
{
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t( name ), &ipTable );
	if( !ipTable ){
		m_output.OutputStdErr( err_level::fatal, err_code::OpenTblFail, name );
		m_output.OutputLog( err_level::fatal, err_code::OpenTblFail, name );
		return false;
	}
	fieldMap.CreateFieldMap( ipTable->GetFields() );

	return true;
}

// �^�������b�V���א�9���b�V�����X�g���쐬����
void CCheckRailwayNW::Get9Mesh( long mesh, std::set<long>& meshlist )
{
	meshlist.insert( mesh );                              // ���S
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  1 ) ); // ����
	meshlist.insert( g_crd.GetMeshCode( mesh,  0,  1 ) ); // ��
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  1 ) ); // �E��
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  0 ) ); // ��
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  0 ) ); // �E
	meshlist.insert( g_crd.GetMeshCode( mesh, -1, -1 ) ); // ����
	meshlist.insert( g_crd.GetMeshCode( mesh,  0, -1 ) ); // ��
	meshlist.insert( g_crd.GetMeshCode( mesh,  1, -1 ) ); // �E��
}

// ���b�V�����X�g����A�����������b�V���|���S�����擾
bool CCheckRailwayNW::GetMeshPolygons( long mesh, set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo )
{
	ISpatialReferencePtr ipSpRef;
	auto& strWheres = CreateWhereByList( meshlist, basemesh::kMeshCode );

	IGeometryCollectionPtr ipGeoCol( CLSID_GeometryBag );
	int i = 0;

	for( const auto& strWhere : strWheres ){

		IFeatureCursorPtr ipMeshCursor = m_ipBaseMeshFC->_Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE );
		if( !ipMeshCursor )
			return false;

		IFeaturePtr ipMeshFeature;
		while( S_OK == ipMeshCursor->NextFeature( &ipMeshFeature ) && ipMeshFeature ){

			IGeometryPtr ipTempMeshGeo = ipMeshFeature->GetShapeCopy();
			if( !ipTempMeshGeo )
				return false;

			// ��ԎQ�Ƃ��擾���Ă���
			if( i == 0 ){
				ipTempMeshGeo->get_SpatialReference( &ipSpRef );
				((IGeometryPtr)ipGeoCol)->putref_SpatialReference( ipSpRef );
			}

			// ���g�̃��b�V�����擾���Ă���
			if( mesh == (ipMeshFeature->GetValue( m_baseMeshFM.FindField( basemesh::kMeshCode ) )).lVal )
				ipMeshGeo = ipTempMeshGeo;

			// ��ňꊇ��ConstructUnion���邽�߂ɁA�W�I���g�����i�[����
			ipGeoCol->AddGeometry( ipTempMeshGeo );

			++i;
		}
	}

	// �l�߂����̂�ConstructUnion
	IGeometryPtr ipTempUnionMeshGeo( CLSID_Polygon );
	IEnumGeometryPtr ipEnumGeo( ipGeoCol );
	if( FAILED(((ITopologicalOperatorPtr)ipTempUnionMeshGeo)->ConstructUnion( ipEnumGeo ) ))
		return false;

	ipTempUnionMeshGeo->putref_SpatialReference( ipSpRef );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->put_IsKnownSimple( VARIANT_FALSE );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->Simplify();

	ipUnionMeshGeo = ipTempUnionMeshGeo;

	return true;
}

vector<CString> CCheckRailwayNW::CreateWhereByList( const set<long>& list, const CString& field )
{
	CString strWhere;
	vector<CString> retWheres;
	strWhere.Format( _T("%s in ( "), field );
	int i = 0;
	for( const auto& num : list ){
		if( i != 0 )
			strWhere.AppendFormat( _T(", ") );
		strWhere.AppendFormat( _T("%ld"), num );
		if( i < 1000)
			++i;
		else{
			strWhere.AppendFormat( _T(" )") );
			retWheres.push_back( strWhere );
			strWhere.Format( _T("%s in ("), field );
			i = 0;
		}
	}
	if( i != 0){
		strWhere.AppendFormat( _T(" )") );
		retWheres.push_back( strWhere );
	}
	return retWheres;
}

bool CCheckRailwayNW::GetRailwayInMesh( const IGeometryPtr& ipMeshGeo, map<long, GeoClass>& railLinkMap, map<long, GeoClass>& railNodeMap, IDMAP& nodeNodeIDs, IDMAP& nodeLinkIDs )
{
	CString subField;
	subField.Format( _T("%s,%s,%s,%s,%s,%s,%s,%s,%s"), ipc_feature::kObjectID, railway_link::kFromNodeOID, railway_link::kToNodeOID, 
		railway_link::kRailwayClass_C, railway_link::kDispScale_C, railway_link::kUnderGround_F, ipc_feature::kShape, 
		ipc_feature::kOperator, ipc_feature::kModifyDate
		);

	// �W�I���g����ێ����邽�߁AVARIANT_FALSE
	IFeatureCursorPtr ipLinkCursor = m_ipRailLinkFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipLinkCursor )
		return false;
	IFeaturePtr       ipLinkFeature;

	while( S_OK == ipLinkCursor->NextFeature( &ipLinkFeature ) && ipLinkFeature ){

		// ObjectID��K�v���擾
		GeoClass railLink;
		long railLinkID    = ipLinkFeature->GetOID();
		railLink.geometry  = ipLinkFeature->GetShapeCopy();
		railLink.attr[railway_link::kFromNodeOID]    = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kFromNodeOID ) ) );
		railLink.attr[railway_link::kToNodeOID]      = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kToNodeOID ) ) );
		railLink.attr[railway_link::kRailwayClass_C] = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kRailwayClass_C ) ) );
		railLink.attr[railway_link::kDispScale_C]    = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kDispScale_C ) ) );
		railLink.attr[railway_link::kUnderGround_F]  = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( railway_link::kUnderGround_F ) ) );
		railLink.attr[ipc_feature::kOperator]        = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kOperator ) ) );
		railLink.attr[ipc_feature::kModifyDate]      = str_util::ToString( ipLinkFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kModifyDate ) ) );

		long fromID = _ttol( railLink.attr.at( railway_link::kFromNodeOID ) );
		long toID   = _ttol( railLink.attr.at( railway_link::kToNodeOID ) );

		nodeNodeIDs[fromID].insert( toID );
		nodeNodeIDs[toID].insert( fromID );

		nodeLinkIDs[fromID].insert( railLinkID );
		nodeLinkIDs[toID].insert( railLinkID );

		railLinkMap[railLinkID] = move(railLink);
	}

	// ���H�m�[�h�p�N�G��
	subField.Format( _T("%s,%s,%s,%s,%s"), ipc_feature::kObjectID, railway_node::kNodeClass_C, ipc_feature::kShape, ipc_feature::kOperator, ipc_feature::kModifyDate );
	// �W�I���g����ێ����邽�߁AVARIANT_FALSE
	IFeatureCursorPtr ipNodeCursor = m_ipRailNodeFC->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipNodeCursor )
		return false;
	IFeaturePtr       ipNodeFeature;

	// ���H�m�[�h�̃��[�v
	while( S_OK == ipNodeCursor->NextFeature( &ipNodeFeature ) && ipNodeFeature ){

		// ObjectID��K�v���擾
		GeoClass railNode;
		long railNodeID     = ipNodeFeature->GetOID();
		railNode.geometry   = ipNodeFeature->GetShapeCopy();
		railNode.attr[railway_node::kNodeClass_C] = str_util::ToString( ipNodeFeature->GetValue( m_railNodeFM.FindField( railway_node::kNodeClass_C ) ) );
		railNode.attr[ipc_feature::kOperator]     = str_util::ToString( ipNodeFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kOperator ) ) );
		railNode.attr[ipc_feature::kModifyDate]   = str_util::ToString( ipNodeFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kModifyDate ) ) );

		railNodeMap[railNodeID] = move( railNode );
	}

	return true;
}

bool CCheckRailwayNW::GetObjInMesh( const IGeometryPtr& ipMeshGeo, const IFeatureClassPtr& ipFeatureClass, const CFieldMap& fieldMap, map<long, GeoClass>& objMap )
{
	CString subField;
	subField.Format( _T("%s,%s"), ipc_feature::kObjectID, ipc_feature::kShape );

	// �W�I���g����ێ����邽�߁AVARIANT_FALSE
	IFeatureCursorPtr ipCursor = ipFeatureClass->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipCursor )
		return false;
	IFeaturePtr ipFeature;

	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

		// ObjectID��K�v���擾
		GeoClass obj;
		long oID    = ipFeature->GetOID();
		obj.geometry  = ipFeature->GetShapeCopy();

		objMap[oID] = move(obj);
	}
	return true;
}

bool CCheckRailwayNW::GetRelRecAll( const ITablePtr& ipTable, const sindy::CFieldMap& fieldMap, const CString& tableName, map<long, vector<recClass>>& recMap )
{
	CString subField;
	if( 0 == tableName.CompareNoCase( rel_railway_link::kTableName ) )
		subField.Format( _T("%s,%s,%s,%s,%s"), ipc_feature::kObjectID, rel_railway_link::kRwLinkID, rel_railway_link::kRailwayLineID, ipc_feature::kOperator, ipc_feature::kModifyDate);
	else if( 0 == tableName.CompareNoCase( rel_railway_node::kTableName ) )
		subField.Format( _T("%s,%s,%s,%s,%s,%s"), ipc_feature::kObjectID, rel_railway_node::kRwNodeID, rel_railway_node::kJoinLineInfoID, rel_railway_node::kStationNo, ipc_feature::kOperator, ipc_feature::kModifyDate );
	else 
		return false;

	_ICursorPtr ipCursor = ipTable->_Search( AheInitQueryFilter( nullptr, subField, nullptr ), VARIANT_TRUE );
	if( ! ipCursor )
		return false;
	_IRowPtr ipRow;

	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){

		// �K�v���擾
		recClass rec;
		rec.attr[ipc_feature::kObjectID]   = str_util::ToString( ipRow->GetOID() );
		rec.attr[ipc_feature::kOperator]   = str_util::ToString( ipRow->GetValue( fieldMap.FindField( ipc_feature::kOperator ) ) );
		rec.attr[ipc_feature::kModifyDate] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( ipc_feature::kModifyDate ) ) );

		if( 0 == tableName.CompareNoCase( rel_railway_link::kTableName ) ){
			long rwLinkID = _ttol( str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_link::kRwLinkID ) ) ) );
			rec.attr[rel_railway_link::kRailwayLineID] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_link::kRailwayLineID ) ) );
			recMap[rwLinkID].push_back( move( rec ) );
		}
		else if( 0 == tableName.CompareNoCase( rel_railway_node::kTableName ) ){
			long rwNodeID = _ttol( str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_node::kRwNodeID ) ) ) );
			rec.attr[rel_railway_node::kJoinLineInfoID] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_node::kJoinLineInfoID ) ) );
			rec.attr[rel_railway_node::kStationNo]      = str_util::ToString( ipRow->GetValue( fieldMap.FindField( rel_railway_node::kStationNo ) ) );
			recMap[rwNodeID].push_back( move( rec ) );
		}
		else
			continue;
	}

	return true;
}

bool CCheckRailwayNW::GetOtherRecAll( const ITablePtr& ipTable, const sindy::CFieldMap& fieldMap, const CString& tableName, map<long, recClass>& recMap )
{
	CString subField;
	if( 0 == tableName.CompareNoCase( join_line_info::kTableName ) )
		subField.Format( _T("%s,%s,%s"), ipc_feature::kObjectID, join_line_info::kStationID, join_line_info::kLineID );
	else if( 0 == tableName.CompareNoCase( railway_line::kTableName ) )
		subField.Format( _T("%s,%s"), ipc_feature::kObjectID, railway_line::kName );
	else 
		return false;

	_ICursorPtr ipCursor = ipTable->_Search( AheInitQueryFilter( nullptr, subField, nullptr ), VARIANT_TRUE );
	if( ! ipCursor )
		return false;
	_IRowPtr ipRow;

	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){

		// �K�v���擾
		recClass rec;
		if( 0 == tableName.CompareNoCase( join_line_info::kTableName ) ){
			long oID = ipRow->GetOID();
			rec.attr[join_line_info::kStationID] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( join_line_info::kStationID ) ) );
			rec.attr[join_line_info::kLineID]    = str_util::ToString( ipRow->GetValue( fieldMap.FindField( join_line_info::kLineID ) ) );
			recMap[oID] = move( rec );
		}
		else if( 0 == tableName.CompareNoCase( railway_line::kTableName ) ){
			long oID = ipRow->GetOID();
			rec.attr[railway_line::kName] = str_util::ToString( ipRow->GetValue( fieldMap.FindField( railway_line::kName ) ) );
			recMap[oID] = move( rec );
		}
		else
			continue;
	}

	return true;
}

void CCheckRailwayNW::createRTree( const map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long oID  = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( make_pair( box, static_cast<unsigned int>( oID ) ));
	}
}

// R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
set<long> CCheckRailwayNW::getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	set<long> retIDs;
	vector<pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

void CCheckRailwayNW::MeshCheck( long mesh, const IGeometryPtr& ipMeshGeo, const map<long, GeoClass>& railLinkMap, const map<long, GeoClass>& railNodeMap, const std::map<long, GeoClass>& staPointMap, const std::map<long, GeoClass>& baseStaMap, const BGRTree& railLinkRTree, const BGRTree& railNodeRTree, const BGRTree& staPointRTree, const BGRTree& baseStaRTree, const IDMAP& nodeNodeIDs, const IDMAP& nodeLinkIDs)
{
	CString strMesh;
	strMesh.Format( _T("%ld"), mesh );

	ITopologicalOperatorPtr ipMeshTopo( ipMeshGeo );
	IGeometryPtr ipBoundaryMesh;
	if( FAILED( ipMeshTopo->get_Boundary( &ipBoundaryMesh ) )){
		m_output.OutputStdErr( err_level::fatal, err_code::GetMeshBoundFail, strMesh );
		m_output.OutputLog( err_level::fatal, err_code::GetMeshBoundFail, strMesh );
		return;
	}
	ipBoundaryMesh->PutRefSpatialReference( ipMeshGeo->GetSpatialReference() );

	// RailLink�`�F�b�N
	CheckRailLink( ipMeshGeo, ipBoundaryMesh, railLinkMap, railNodeMap, railLinkRTree, railNodeRTree, nodeLinkIDs );

	// RailNode�P�ƃ`�F�b�N
	CheckRailNode( ipMeshGeo, ipBoundaryMesh, railLinkMap, railNodeMap, staPointMap, baseStaMap, railLinkRTree, railNodeRTree, staPointRTree, baseStaRTree, nodeLinkIDs );

}

void CCheckRailwayNW::CheckRailLink( const IGeometryPtr& ipMeshGeo, const IGeometryPtr& ipBoundaryMesh, const map<long, GeoClass>& railLinkMap, const map<long, GeoClass>& railNodeMap, const BGRTree& railLinkRTree, const BGRTree& railNodeRTree, const IDMAP& nodeLinkIDs )
{
	using namespace railway_link;
	using namespace ipc_feature;
	CString info1, info2;

	map<pair<long, long>, list<long>> dupNodeMap; //!< �L�[�FFrom/To�m�[�hID�iID�̏�����������j�A�l�F�����N��ObjectID

	// �S�������N�̃`�F�b�N
	for( auto& railLink : railLinkMap ){

		long  railLinkID     = railLink.first;
		auto& railLinkObj    = railLink.second;

		long  fromID        = _ttol( railLinkObj.attr.at( kFromNodeOID ) );
		long  toID          = _ttol( railLinkObj.attr.at( kToNodeOID ) );
		long  railLinkClass = _ttol( railLinkObj.attr.at( kRailwayClass_C ) );
		long  dispScale     = _ttol( railLinkObj.attr.at( kDispScale_C ) );
		auto& underGroundF  = railLinkObj.attr.at( railway_link::kUnderGround_F );
		auto& modOperator   = railLinkObj.attr.at( ipc_feature::kOperator );
		auto& modDate       = railLinkObj.attr.at( ipc_feature::kModifyDate );
	
		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( railLinkObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// ���P���ȑ����`�F�b�N

		// �e�����l����`�����m�F����
		if( ! judge_value::IsDefineRailLinkClass( railLinkClass ) )   // ���G���[ �S����ʃR�[�h����`�O
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRailLinkClass, modOperator, modDate, railLinkObj.attr.at( kRailwayClass_C ) );

		if( ! judge_value::IsDefineDispScale( dispScale ) )   // ���G���[ �X�P�[���\���R�[�h����`�O
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgDispScaleClass, modOperator, modDate, railLinkObj.attr.at( kDispScale_C ) );

		// ���G���[ �t���O�t�B�[���h�Ȃ̂ɁA[0]��[1]�ȊO�̒l
		if( ! judge_value::IsOKFlag( underGroundF ) ){
			info1.Format( _T("%s(%s)"), kUnderGround_F, underGroundF );
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgFlagField, modOperator, modDate, info1 );
		}

		// From/To��ID���m�F
		bool bNodeIDErr = false;
		if( fromID <= 0 ){ // ���G���[ �n�_�m�[�hID���s��
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgFromID, modOperator, modDate, railLinkObj.attr.at( kFromNodeOID ) );
			bNodeIDErr = true;
		}
		if( toID <= 0 ){   // ���G���[ �I�_�m�[�hID���s��
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgToID, modOperator, modDate, railLinkObj.attr.at( kToNodeOID ) );
			bNodeIDErr = true;
		}
		if( fromID == toID ){ // ���G���[ �����N�̎n�I�_���������i�����j
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::LoopLinkSingleFT, modOperator, modDate, _T("����") );
			bNodeIDErr = true;
		}
		// ����From/To�����݂��Ȃ����m�F�̂��߁A����Ȃ��̂̓m�[�hID�̃y�A���L�[�ɁA�����NID���i�[���Ă���
		if( ! bNodeIDErr ){
			if( fromID < toID )
				dupNodeMap[make_pair( fromID, toID )].push_back( railLinkID );
			else
				dupNodeMap[make_pair( toID, fromID )].push_back( railLinkID );
		}

		// ����������`����g�����`�F�b�N

		// �P���Ȍ`��m�F
		esriGeometryType eType = (railLinkObj.geometry.m_T)->GetGeometryType();
		IPointCollectionPtr ipRailLinkPointCol( railLinkObj.geometry.m_T );
		long railLinkPointCnt = ipRailLinkPointCol->GetPointCount();
		if( ! railLinkObj.geometry.m_T || ! ipRailLinkPointCol || railLinkPointCnt < 2 || eType != esriGeometryPolyline ){
			// ���G���[ �S�������N�`�󂪕s��
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRailLinkGeom, modOperator, modDate );
			continue;
		}

		// From/To�m�[�h�擾
		auto fromNodeItr = railNodeMap.find( fromID );
		auto toNodeItr   = railNodeMap.find( toID );

		bool bFromNodeMesh = false, bToNodeMesh = false;

		// ���b�V���g�Ɗ֘A�����邩�H
		if( ! railLinkObj.IsDisjoint( ipBoundaryMesh ) ){

			// �N���X���Ă��Ȃ����H
			if( ! railLinkObj.IsCross( ipBoundaryMesh ) ){

				// �ǂ��炩�����b�V���}�s�m�[�h����Ȃ��Ƃ����Ȃ�
				bool bOK = false;

				if( (fromNodeItr != railNodeMap.end() 
					&& judge_value::GetLongValue( fromNodeItr->second, railway_node::kNodeClass_C ) == railway_node::node_class::kOutline ) ){
						bOK = true;
						bFromNodeMesh = true;
				}
				if( ( toNodeItr != railNodeMap.end()
					&& judge_value::GetLongValue( toNodeItr->second, railway_node::kNodeClass_C ) == railway_node::node_class::kOutline ) ){
						bOK = true;
						bToNodeMesh = true;
				}

				// ���G���[ ���b�V���ׂ��̃����N�ɐ}�s�m�[�h�����݂��Ȃ�
				if( ! bOK )
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NoBorderNode, modOperator, modDate );
			}
			else // ���G���[ 2�����b�V�������f���Ă���	
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::CrossMesh, modOperator, modDate );
		}

		// From�m�[�h�̑��݊m�F
		GeoClass fromRailNodeObj;
		bool bFromOK = false;
		if( fromNodeItr == railNodeMap.end() ) // ���G���[ FromNodeID�̃m�[�h��������Ȃ�
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NotFoundFromNode, modOperator, modDate, railLinkObj.attr.at( kFromNodeOID ) );
		else{
			bFromOK = true;
			fromRailNodeObj = fromNodeItr->second;
		}

		// To�m�[�h�̑��݊m�F
		GeoClass toRailNodeObj;
		bool bToOK = false;
		if( toNodeItr == railNodeMap.end() ) // ���G���[ ToNodeID�̃m�[�h��������Ȃ�
			m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NotFoundToNode, modOperator, modDate, railLinkObj.attr.at( kToNodeOID ) );
		else{
			bToOK = true;
			toRailNodeObj = toNodeItr->second;
		}

		// From/To�̌`�󂪈�v���Ă��Ȃ����m�F
		if( bFromOK && bToOK ){
			if( fromRailNodeObj.IsEqual( toRailNodeObj.geometry.m_T ) ) // ���G���[ �����N�̎n�I�_���������i�`��j
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::LoopLinkSingleFT, modOperator, modDate, _T("�`��") );
		}

		// ���b�V���֘A�̓��b�V���𒴂��đ�������v���邩�̊m�F���s��
		if( ( bFromNodeMesh && bFromOK ) || ( bToNodeMesh && bToOK ) ){
			CString diffField;
			bool    bFind = false;

			long railNodeID = 0;
			if( bFromNodeMesh )
				railNodeID = fromID;
			else
				railNodeID = toID;

			// ���̐ڑ������N���擾����
			auto linkIDSetItr = nodeLinkIDs.find( railNodeID );
			if( linkIDSetItr != nodeLinkIDs.end() && linkIDSetItr->second.size() == 2){
				auto& linkIDSet = linkIDSetItr->second;
				auto& neighborLinks = GetObjects( railLinkID, linkIDSetItr->second, railLinkMap );
				auto& neighborLink  = *(neighborLinks.begin()); //!< (���b�V��)�m�[�h�ɐڑ����郊���N

				// ��������v���邩�H
				diffField = judge_value::GetDiffRailLinkAttr( railLinkObj, neighborLink );
				if( ! diffField.IsEmpty() ){ 
					// ���G���[ 2�����b�V�����E�̗אڂŃ����N�̑������قȂ�
					info1.Format( _T("%s(%s)"), diffField, railLinkObj.attr.at( diffField ) );
					info2.Format( _T("%s(%s)"), diffField, neighborLink.attr.at( diffField ) );
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NotSameBorderAttr, modOperator, modDate, info1, info2 );
				}
			}
			else // ���G���[ ���b�V�����E�ŗאڃ����N���s��
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgBorderLink, modOperator, modDate );
		}

		// �_��ɕ������Ċm�F
		double cmpX  = 0.0, cmpY  = 0.0;
		map<pair<double,double>, vector<long>> coordsMap; // �L�[�Fxy�A�l�F�\���_�ԍ�
		for( long i = 0 ; i < railLinkPointCnt ; ++i ){

			IPointPtr ipPoint = ipRailLinkPointCol->GetPoint( i );

			// �n�_��From�m�[�h�̈�v���m�F
			if( i == 0 && bFromOK ){
				if( ! fromRailNodeObj.IsEqual( (IGeometryPtr)ipPoint ) ) // ���G���[ �����N�̎n�_��From�m�[�h�̍��W����v���Ȃ�
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::UnMatchStartFrom, modOperator, modDate );
			}

			// �I�_��To�m�[�h�̈�v���m�F
			if( i == railLinkPointCnt - 1 && bToOK ){
				if( ! toRailNodeObj.IsEqual( (IGeometryPtr)ipPoint ) ) // ���G���[ �����N�̏I�_��To�m�[�h�̍��W����v���Ȃ�
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::UnMatchEndTo, modOperator, modDate );
			}

			// �Z�O�����g�����m�F
			double x = 0.0, y = 0.0;
			ipPoint->QueryCoords( &x, &y );
			if( i != 0 ){
				double dist = g_crd.GetDist( cmpX, cmpY, x, y );
				if( dist <= m_dist_threshold ){
					// ���G���[ �����Z�O�����g�����݂���
					info1.Format( _T("'%ld'�Ԃ�'%ld'��"), i - 1, i );
					info2.Format( _T("'%lf'm�ȉ��i'%lf'm�j"), m_dist_threshold, dist );
					m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::UnderThresholdSeg, modOperator, modDate, info1, info2 );
				}
			}

			// xy���L�[�Ƃ��āA�\���_�ԍ����l�߂�
			coordsMap[make_pair(x, y)].push_back( i );

			cmpX  = x;
			cmpY  = y;
		}

		// �\���_�d���̊m�F
		for( const auto& coord : coordsMap ){
			if( coord.second.size() > 1 ){
				// ���G���[ �\���_���d�����Ă���
				auto itr = coord.second.begin();
				info1.Format( _T("'%ld'�Ԃ�'%ld'��"), *(itr), *(itr + 1) );
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::DupPoint, modOperator, modDate, info1 );
			}
		}


		// �H�����X�g�ƕR�Â����m�F�i�S��POI��ʂŕ��ނ̂݁A�R�Â��Ă�OK�j
		set<long> tempRailLineIDs;
		bool bFindRailwayLine = FindRailwayLine( railLinkID, tempRailLineIDs );
		if( railLinkClass == link_class::kDivideByPoi ){
			if( ! bFindRailwayLine ){ // ���G���[ �S����ʃR�[�h��0(�S��POI��ʂŕ���)�ŘH�����X�g�̒l��null
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRelRailwayLine1, modOperator, modDate );
			}
		}
		else{
			if( bFindRailwayLine ){ // ���G���[ �S����ʃR�[�h��1or2(�ݕ����E�S��������)�ŘH�����X�g�̒l��null�ȊO
				m_output.OutputLog( kTableName, railLinkID, err_level::error, err_code::NgRelRailwayLine2, modOperator, modDate );
			}
		}

	}

	// ����From/To�����݂��Ȃ����m�F����
	for( const auto& dupID : dupNodeMap ){
		if( dupID.second.size() > 1 ){
			// ���G���[ From/To�̑g�ݍ��킹�����������N���������݂���
			auto  itr = dupID.second.begin();
			long  ngID1 = *itr;
			long  ngID2 = *(++itr);
			auto  itrRailLink = railLinkMap.find( ngID1 );
			CString modOperator, modDate;
			if( itrRailLink != railLinkMap.end() ){
				modOperator = itrRailLink->second.attr.at( kOperator );
				modDate     = itrRailLink->second.attr.at( kModifyDate );
			}
			info1.Format( _T("RailLinkID:%ld"), ngID2 );
			m_output.OutputLog( kTableName, ngID1, err_level::error, err_code::LoopLinkDupFT, modOperator, modDate, info1 );
		}
	}
}

void CCheckRailwayNW::CheckRailNode( const IGeometryPtr& ipMeshGeo, const IGeometryPtr& ipBoundaryMesh, const map<long, GeoClass>& railLinkMap, const map<long, GeoClass>& railNodeMap, const map<long, GeoClass>& staPointMap, const map<long, GeoClass>& baseStaMap, const BGRTree& railLinkRTree, const BGRTree& railNodeRTree, const BGRTree& staPointRTree, const BGRTree& baseStaRTree, const IDMAP& nodeLinkIDs )
{
	using namespace railway_node;
	using namespace ipc_feature;
	CString info1, info2;

	// �S���m�[�h�̃`�F�b�N
	for( auto& railNode : railNodeMap ){

		long  railNodeID    = railNode.first;
		auto& railNodeObj   = railNode.second;

		long  railNodeClass = _ttol( railNodeObj.attr.at( kNodeClass_C ) );

		auto& modOperator  = railNodeObj.attr.at( ipc_feature::kOperator );
		auto& modDate      = railNodeObj.attr.at( ipc_feature::kModifyDate );

		// �����Ώۃ��b�V�����i�g��܂ށj����Ȃ����̂̓X�L�b�v
		if( railNodeObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// �e�����l����`�����m�F����
		if( ! judge_value::IsDefineRailNodeClass( railNodeClass ) ){
			// ���G���[ �m�[�h��ʃR�[�h����`�O
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRailNodeClass, modOperator, modDate, str_util::ToString( railNodeClass ) );
		}

		// ����������`����g�����`�F�b�N

		// �P���Ȍ`��m�F
		esriGeometryType eType = (railNodeObj.geometry.m_T)->GetGeometryType();
		IPointPtr ipRailNodePoint( railNodeObj.geometry.m_T );
		double railNodeX = 0.0, railNodeY = 0.0;
		if( ! railNodeObj.geometry.m_T || ! ipRailNodePoint || FAILED( ipRailNodePoint->QueryCoords( &railNodeX, &railNodeY ) ) || eType != esriGeometryPoint ){
			// ���G���[ �`�󂪕s��
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRailNodeGeom, modOperator, modDate );
			continue;
		}

		int iMeshCode = 0, iX = 0, iY = 0;
		g_crd.LLtoMesh( railNodeX, railNodeY, 2, &iMeshCode, &iX, &iY, 1);
		if( ( iX == 0 && ( iY == 0 || iY == 1000000 ) ) || ( iX == 1000000 && ( iY == 0 || iY == 1000000 ) ) ){
			// ���G���[ �}�s�̋��Ƀm�[�h������
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::BorderCornerNode, modOperator, modDate );
		}

		// ���W����m�[�h��ʂ̊m�F
		if( iX == 0 || iX == 1000000 || iY == 0 || iY == 1000000 ){
			if( railNodeClass != node_class::kOutline ) // ���G���[ 2�����b�V�����E����ɂ���̂ɐ}�s��m�[�h�ɂȂ��Ă��Ȃ�
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgBorderNodeAttr, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
		}
		else{
			if( railNodeClass == node_class::kOutline ) // ���G���[ �}�s��m�[�h��2�����b�V�����E����ɖ���
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgBorderNodeGeom, modOperator, modDate );
		}

		// �ʂ̃m�[�h���߂��ɂȂ����m�F�i����臒l�ōL���Č����j
		auto& findRailNodeIDs = getIntersectsIDs( railNodeObj.GetBox( m_dist_threshold ), railNodeRTree );

		// ���������S���m�[�h�̃��[�v
		for( auto& findRailNodeID : findRailNodeIDs ){

			// ���g�̓X�L�b�v
			if( findRailNodeID == railNodeID )
				continue;

			auto findRailNodeItr = railNodeMap.find( findRailNodeID );
			if( findRailNodeItr == railNodeMap.end() )
				continue;

			// �����𑪂�
			IPointPtr ipPoint( findRailNodeItr->second.geometry.m_T );
			double findX = 0.0, findY = 0.0;
			ipPoint->QueryCoords( &findX, &findY );
			double dist = g_crd.GetDist( findX, findY, railNodeX, railNodeY );

			if( dist < m_dist_threshold ){
				// ���G���[ �m�[�h���ʂ̃m�[�h�ɐڐG���Ă���
				info1.Format( _T("RailNodeID:'%ld'"), findRailNodeID );
				info2.Format( _T("����:'%lf'm(臒l:'%lf'm)"), dist, m_dist_threshold );
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::TouchNgNode, modOperator, modDate, info1, info2 );
			}
		}

		// �ڑ������N�����擾
		long connectRailLinkCnt = -1;
		set<long> connectRailLinkIDs;
		auto railNodeLinkItr = nodeLinkIDs.find( railNodeID );
		if( railNodeLinkItr != nodeLinkIDs.end() ){
			connectRailLinkCnt = railNodeLinkItr->second.size();
			connectRailLinkIDs = railNodeLinkItr->second;
		}

		// �ڑ������N���Ŋm�F(2�{�͊m�F�s�v)
		if( connectRailLinkCnt < 1 ){
			// ���G���[ �S�������N�ɐڑ����Ă��Ȃ��i�����j
			m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NoConnect, modOperator, modDate, _T("����") );
		}
		else if( connectRailLinkCnt == 1 ){
			if( ! judge_value::IsOKTerminalNode( railNodeClass ) ){
				// ���G���[ �[�_�Ȃ̂ɒ[�_�m�[�h�ɂȂ��Ă��Ȃ�
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgTerminalNode, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
			}
		}
		else if( connectRailLinkCnt > 2 ){
			if( ! judge_value::IsOKBranchNode( railNodeClass ) ){
				// ���G���[ ����Ȃ̂ɕ���m�[�h�ɂȂ��Ă��Ȃ�
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgBranchNode, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
			}
		}

		// �ڑ������N���m�F
		{
			bool bFindLink = false, bAlter = false;
			auto& findRailLinkIDs = getIntersectsIDs( railNodeObj.GetBox( 0.1 ), railLinkRTree );

			// ���������S�������N�̃��[�v
			int i = 0;
			GeoClass neighborLink;
			for( auto& findRailLinkID : findRailLinkIDs ){

				auto findRailLinkItr = railLinkMap.find( findRailLinkID );
				if( findRailLinkItr == railLinkMap.end() )
					continue;

				// From/To�̂ǂ��炩�ƈ�v���邩�m�F
				IPolylinePtr ipFindRailLinkLine( findRailLinkItr->second.geometry.m_T );
				IPointPtr ipFindFromPoint = ipFindRailLinkLine->GetFromPoint();
				IPointPtr ipFindToPoint   = ipFindRailLinkLine->GetToPoint();
				if( ! ipFindFromPoint || ! ipFindToPoint )
					continue;

				if( ! railNodeObj.IsEqual( ipFindFromPoint ) && ! railNodeObj.IsEqual( ipFindToPoint ) )
					continue;

				bFindLink = true;

				if( i != 0 ){
					if( ! judge_value::IsSameRailLinkAttr( findRailLinkItr->second, neighborLink ) )
						bAlter = true;
				}

				neighborLink = findRailLinkItr->second; //!< ��r�p�̃R�s�[
				++i;
			}
			if( ! bFindLink ) // ���G���[ �S�������N�ɐڑ����Ă��Ȃ��i�`��j
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NoConnect, modOperator, modDate, _T("�`��") );

			// �����ω����Ă����ꍇ�́AOK�ȃm�[�h�������m�F����
			if( bAlter ){
				if( ! judge_value::IsOKAlterNode( railNodeClass ) ){
					// ���G���[ �ڑ����郊���N�̑������قȂ�̂ɑ����ω��_�m�[�h�ɂȂ��Ă��Ȃ�
					m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgAlterNode, modOperator, modDate, judge_value::IsFindObj( railNodeClass, m_railNodeClassMap ) );
				}
			}
		}

		// �[�_�̏ꍇ�A�ߖT�ɐڑ����Ă��Ȃ������N���������m�F�i����臒l�ōL���Č����j
		if( railNodeClass == node_class::kTerminal ){
			// ���s�҃m�[�h��XY�擾
			double x = 0.0, y = 0.0;
			IPointPtr ipRailNodePoint( railNodeObj.geometry.m_T );
			ipRailNodePoint->QueryCoords( &x, &y );

			// �ڑ������Nset���擾����
			auto connectLinkItr = nodeLinkIDs.find( railNodeID );

			auto& findRailLinkIDs = getIntersectsIDs( railNodeObj.GetBox( m_dist_threshold ), railLinkRTree );

			// �����������s�҃����N�̃��[�v
			for( auto& findRailLinkID : findRailLinkIDs ){

				auto& findRailLinkItr = railLinkMap.find( findRailLinkID );
				if( findRailLinkItr == railLinkMap.end() )
					continue;

				// �ڑ������Nset�ɂ���΃X�L�b�v
				if( connectLinkItr != nodeLinkIDs.end() && connectLinkItr->second.find( findRailLinkID ) != connectLinkItr->second.end() )
					continue;

				// �ŋߖT�|�C���g�擾
				IProximityOperatorPtr ipProximity( findRailLinkItr->second.geometry.m_T );
				IPointPtr ipNearPoint = ipProximity->_ReturnNearestPoint( ipRailNodePoint, esriNoExtension );
				if( !ipNearPoint )
					continue;

				// �����𑪂�
				double nearX = 0.0, nearY = 0.0;
				ipNearPoint->QueryCoords( &nearX, &nearY );
				double dist = g_crd.GetDist( x, y, nearX, nearY );

				// 臒l�𒴂��鋗���̏ꍇ�A�X�L�b�v�i�G���x���[�v�̂��߁A�΂߂�臒l�𒴂��邽�߁j
				if( dist > m_dist_threshold )
					continue;

				// ���G���[ �m�[�h���֌W�Ȃ������N�ɐڐG���Ă���
				info1.Format( _T("RailLinkID:'%ld'"), findRailLinkID );
				info2.Format( _T("����:'%lf'm(臒l:'%lf'm)"), dist, m_dist_threshold );
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::TouchNgLink, modOperator, modDate, info1, info2 );
			}
		}

		// �H�����X�g�ƕR�Â����m�F�i�wPOI�m�[�h�̂݁A�R�Â��Ă�OK�j
		set<long> staPointIDs, lineIDs;
		bool bFindJoinLineInfo = FindJoinLineInfo( railNodeID, staPointIDs, lineIDs );
		if( railNodeClass == node_class::kStation ){
			
			// ���k�w�ɂƂ̊֌W���m�F����
			auto& findBaseStaIDs = getIntersectsIDs( railNodeObj.GetBox( 0.1 ), baseStaRTree );

			// �����������k�w�ɂ̃��[�v
			bool bFindBaseSta = false;
			for( auto& findBaseStaID : findBaseStaIDs ){

				auto findBaseStaItr = baseStaMap.find( findBaseStaID );
				if( findBaseStaItr == baseStaMap.end() )
					continue;

				// �ڐG���Ă���OK
				if( ! findBaseStaItr->second.IsDisjoint( railNodeObj.geometry.m_T ) )
					bFindBaseSta = true;
			}
			if( ! bFindBaseSta ){ // �ڐG���Ă��Ȃ�
				// ���G���[ ���k�w�Ƀ|���S����ɑ��݂��Ȃ��wPOI�m�[�h�����݂���
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NoTouchBaseSta, modOperator, modDate );
			}

			
			if( ! bFindJoinLineInfo ) // ���G���[ �wPOI�m�[�h�Ȃ̂ɘH�����X�g�̒l��null
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRelJoinLine1, modOperator, modDate );
			else{
				// �擾����JOIN_LINE_INFO��STATION_ID�������̂��ߖT�͈͓��ɂ��邩�m�F����i�ߖT臒l�ōL���Č����j
				auto& findStaPointIDs = getIntersectsIDs( railNodeObj.GetBox( m_neighbor_threshold ), staPointRTree );

				// ���������w�o������|�C���g�̃��[�v
				bool bFindStaPoint = false;
				for( auto& findStaPointID : findStaPointIDs ){

					// ID�����������H
					auto itrStaPointIDs = staPointIDs.find( findStaPointID );
					if( itrStaPointIDs == staPointIDs.end() )
						continue;

					auto findStaPointItr = staPointMap.find( findStaPointID );
					if( findStaPointItr == staPointMap.end() )
						continue;

					// �����𑪂�
					IPointPtr ipPoint( findStaPointItr->second.geometry.m_T );
					double findX = 0.0, findY = 0.0;
					ipPoint->QueryCoords( &findX, &findY );
					double dist = g_crd.GetDist( findX, findY, railNodeX, railNodeY );

					// �ߖT臒l�ȉ��Ȃ�OK
					if( dist < m_neighbor_threshold )
						bFindStaPoint = true;
				}

				if( ! bFindStaPoint ){ // �ߖT�͈͓��Ō�����Ȃ�����
					// ���G���[ �wPOI�m�[�h�̋ߖT�ɑΉ�����wPOI�����݂��Ȃ�
					info1.Format( _T("臒l:'%lf'm"), m_neighbor_threshold );
					m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::TooFarStaPoint, modOperator, modDate, info1 );
				}

				// �擾����LINE_ID���S�Đڑ������N�ɑ��݂��邩�m�F
				set<long> connectLineIDs;
				for( const auto& connectRailLinkID : connectRailLinkIDs ){
					// �ڑ����Ă���RailLink�ɕR�Â�LineID��S�Ď擾����
					set<long> tempLineIDs;
					if( ! FindRailwayLine( connectRailLinkID, tempLineIDs ) )
						continue;
					for( const auto& tempLineID : tempLineIDs )
						connectLineIDs.insert( tempLineID );
				}
				for( const auto& lineID : lineIDs ){
					// �Ȃ����̂���������G���[
					if( connectLineIDs.end() != connectLineIDs.find( lineID ) )
						continue;
										
					// ���G���[ �ڑ����Ă��郊���N�ɑ��݂��Ȃ��H��ID���m�[�h�ɑ��݂��Ă���
					info1.Format(_T("%ld(%s)"), lineID, GetLineName( lineID ) );
					m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRelJoinLine3, modOperator, modDate, info1 );
				}
			}
		}
		else{
			if( bFindJoinLineInfo ) // ���G���[ �wPOI�m�[�h�ȊO�̃m�[�h�ŘH�����X�g�̒l��null�ȊO
				m_output.OutputLog( kTableName, railNodeID, err_level::error, err_code::NgRelJoinLine2, modOperator, modDate );
		}

		


	}
}

bool CCheckRailwayNW::FindJoinLineInfo( long railNodeID, set<long>& stationIDs, set<long>& lineIDs )
{
	auto itrRel = m_relRailwayNode.find( railNodeID );
	if( itrRel == m_relRailwayNode.end() )
		return false;

	for( auto& rel : itrRel->second ){
		long joinLineInfoID = _ttol( rel.attr.at( rel_railway_node::kJoinLineInfoID ) );
		auto itrJoinLineInfo = m_joinLineInfo.find( joinLineInfoID );
		if( itrJoinLineInfo == m_joinLineInfo.end() )
			continue;

		stationIDs.insert( _ttol( itrJoinLineInfo->second.attr.at( join_line_info::kStationID ) ) );
		lineIDs.insert( _ttol( itrJoinLineInfo->second.attr.at( join_line_info::kLineID ) ) );
	}
	return stationIDs.empty()? false : true;
}

bool CCheckRailwayNW::FindRailwayLine( long railLinkID, set<long>& railwayLineIDs )
{
	auto itrRel = m_relRailwayLink.find( railLinkID );
	if( itrRel == m_relRailwayLink.end() )
		return false;

	for( auto& rel : itrRel->second ){
		long railwayLineID = _ttol( rel.attr.at( rel_railway_link::kRailwayLineID ) );
		auto itrRailwayLine = m_railwayLine.find( railwayLineID );
		if( itrRailwayLine == m_railwayLine.end() )
			continue;

		railwayLineIDs.insert( railwayLineID );
	}
	return railwayLineIDs.empty()? false : true;
}

list<GeoClass> CCheckRailwayNW::GetObjects( long oID, const set<long>& oIDs, const map<long, GeoClass>& geoMap )
{
	list<GeoClass> retList;

	for( const auto& id : oIDs ){

		// ���g��������geoMap����T���āAretList�Ɋi�[
		if( oID == id )
			continue;

		auto itr = geoMap.find( id );
		if( itr == geoMap.end() )
			continue;

		retList.push_back( itr->second );
	}

	return retList;
}

void CCheckRailwayNW::QueryCheck()
{

	// �S�������N�֘A�e�[�u���̖�ǃ`�F�b�N
	CheckNonRel( (ITablePtr)m_ipRailLinkFC, railway_link::kTableName, rel_railway_link::kTableName, m_relRailwayLink );

	// �S���m�[�h�֘A�e�[�u���̖�ǃ`�F�b�N
	CheckNonRel( (ITablePtr)m_ipRailNodeFC, railway_node::kTableName, rel_railway_node::kTableName, m_relRailwayNode );
	
	// ����H���̃����N�A�����`�F�b�N
	SameLineSeqLinkChk();

	// �H�����̉w�ԍ��̃`�F�b�N
	CheckStaNumInLine();

	// �S���m�[�h�ƕR�t��������H���e�[�u���̑��݃`�F�b�N�ibug12289�Œǉ��j
	checkRelRecordExstence(railway_node::kTableName, m_relRailwayNode, m_joinLineInfo);

	// �S�������N�ƕR�t���S���H���e�[�u���̑��݃`�F�b�N�ibug12289�Œǉ��j
	checkRelRecordExstence(railway_link::kTableName, m_relRailwayLink, m_railwayLine);
}

void CCheckRailwayNW::CheckNonRel( const ITablePtr& ipTable, const CString& fcTableName, const CString& relTableName, const map<long, vector<recClass>>& relAll )
{
	err_code::ECode eCode;
	if( 0 == fcTableName.CompareNoCase( railway_link::kTableName ) )
		eCode = err_code::NonRelRailLink;
	else
		eCode = err_code::NonRelRailNode;

	set<long> railOIDs;
	if( ! GetOIDAll( ipTable, fcTableName, railOIDs ) ){
		m_output.OutputStdErr( err_level::fatal, err_code::GetOIDAllFail, fcTableName );
		m_output.OutputLog( err_level::fatal, err_code::GetStaPoiFail, fcTableName );
	}

	for( const auto& rel : relAll ){
		long relRwID = rel.first;
		if( railOIDs.end() != railOIDs.find( relRwID ) )
			continue; // �������OK

		// �G���[�o��
		for( const auto& relRec : rel.second ){
			// ���G���[ �R�Â��S�������N�����݂��Ȃ�
			// ���G���[ �R�Â��S���m�[�h�����݂��Ȃ�
			long relOID   = _ttol( relRec.attr.at( ipc_feature::kObjectID ) );
			//auto& modOperator = relRec.attr.at( ipc_feature::kOperator );
			//auto& modDate     = relRec.attr.at( ipc_feature::kModifyDate );
			//m_output.OutputLog( relTableName, relOID, err_level::error, eCode, modOperator, modDate, str_util::ToString( relRwID ) );
			m_output.OutputLog( relTableName, relOID, err_level::error, eCode, _T(""), _T(""), str_util::ToString( relRwID ) );
		}
	}
}

bool CCheckRailwayNW::GetOIDAll( const ITablePtr& ipTable, const CString& tableName, std::set<long>& oIDs )
{
	CString subStr;
	subStr.Format( _T("%s"), ipc_feature::kObjectID );

	_ICursorPtr ipCursor = ipTable->_Search( AheInitQueryFilter( nullptr, subStr, nullptr ), VARIANT_TRUE );
	if( ! ipCursor )
		return false;
	_IRowPtr ipRow;

	while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow ){
		oIDs.insert( ipRow->GetOID() );
	}

	return true;
}

void CCheckRailwayNW::SameLineSeqLinkChk()
{
	map<long, set<long>> lineIDs;
	for( const auto& rel : m_relRailwayLink ){
		long rwLinkID = rel.first;
		for( const auto& rel2 : rel.second ){
			long lineID = _ttol( rel2.attr.at( rel_railway_link::kRailwayLineID ) );
			lineIDs[lineID].insert( rwLinkID );
		}
	}

	CString subStr;
	subStr.Format( _T("%s,%s,%s"), ipc_feature::kObjectID, railway_link::kFromNodeOID, railway_link::kToNodeOID );
	for( const auto& line : lineIDs ){

		// ����LineID�������̂��O���[�v������
		IDMAP nodeNodeIDs, nodeLinkIDs;
		set<long> nodeIDs;
		auto& linkIDs = line.second;
		auto& strWheres = CreateWhereByList( linkIDs, ipc_feature::kObjectID );

		for( const auto& strWhere : strWheres ){

			IFeatureCursorPtr ipCursor = m_ipRailLinkFC->_Search( AheInitQueryFilter( NULL, subStr, strWhere ), VARIANT_TRUE );
			if( ! ipCursor )
				continue;

			IFeaturePtr ipFeature;
			while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

				long oID = ipFeature->GetOID();
				long fromID = _ttol( str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( railway_link::kFromNodeOID ) ) ) );
				long toID   = _ttol( str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( railway_link::kToNodeOID ) ) ) );

				nodeNodeIDs[fromID].insert( toID );
				nodeNodeIDs[toID].insert( fromID );

				nodeLinkIDs[fromID].insert( oID );
				nodeLinkIDs[toID].insert( oID );

				nodeIDs.insert( fromID );
				nodeIDs.insert( toID );
			}
		}

		// �m�[�h����쐬���āA���ق��m�F����
		long startID = *(nodeIDs.begin());
		set<long> relNodeIDs = GetRelNodeIDs( startID, nodeNodeIDs );
		if( nodeIDs.size() == relNodeIDs.size() )
			continue; // �T�C�Y���������S�Čq�����Ă���̂�OK

		set<long> ngLinkIDs;
		for( const auto& id : nodeIDs ){ // �Ⴄ�q��T��
			auto itr = relNodeIDs.find( id );
			if( itr != relNodeIDs.end() )
				continue;

			// NG�m�[�h�ɕR�Â������N���擾
			auto ngItr = nodeLinkIDs.find( id );
			for( const auto& ngLinkID : ngItr->second ){
				ngLinkIDs.insert( ngLinkID );
			}
		}

		for( const auto& ngLinkID : ngLinkIDs ){
			// ���G���[ �wPOI�m�[�h�ȊO�̃m�[�h�ŘH�����X�g�̒l��null�ȊO
			//IFeaturePtr ipFeature = m_ipRailLinkFC->_GetFeature( ngLinkID );
			//auto& modOperator = str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kOperator ) ) );
			//auto& modDate     = str_util::ToString( ipFeature->GetValue( m_railLinkFM.FindField( ipc_feature::kModifyDate ) ) );
			//m_output.OutputLog( railway_link::kTableName, ngLinkID, err_level::error, err_code::NotOnlySeqLine, modOperator, modDate, str_util::ToString( startID ) );
			m_output.OutputLog( railway_link::kTableName, ngLinkID, err_level::error, err_code::NotOnlySeqLine, _T(""), _T(""), str_util::ToString( startID ) );
		}
	}
}

set<long> CCheckRailwayNW::GetRelNodeIDs( long nodeID, const IDMAP& nodeNodeIDs )
{
	set<long> relNodeIDs;
	relNodeIDs.insert( nodeID );
	while(1){
		int oldSize = relNodeIDs.size();
		for( auto& nodeNodeID : nodeNodeIDs ){

			bool bFind = false;
			if( relNodeIDs.find( nodeNodeID.first ) != relNodeIDs.end() )
				bFind = true;
			else{
				for( auto& walkNodeID : nodeNodeID.second ){
					if( relNodeIDs.find( walkNodeID ) != relNodeIDs.end() ){
						bFind = true;
						break;
					}
				}
			}
			if( bFind ){
				relNodeIDs.insert( nodeNodeID.first );
				relNodeIDs.insert( nodeNodeID.second.begin(), nodeNodeID.second.end() );
			}

		}

		int newSize = relNodeIDs.size();
		if( oldSize == newSize )
			break;
	}

	return relNodeIDs;
}

void CCheckRailwayNW::CheckStaNumInLine()
{
	CString info1, info2;
	map<long, map<CString, long>> rwNodeMap; // �L�[�F�H��ID�A�l�Fmap< �L�[�F�w�ԍ��A�l�FrwNodeID >
	for( const auto& relRailwayNode : m_relRailwayNode ){
		long rwNodeID = relRailwayNode.first;
		for( const auto& relNodeRec : relRailwayNode.second ){
			auto& stationNum = relNodeRec.attr.at( rel_railway_node::kStationNo );
			if( stationNum.IsEmpty() )
				continue; // �w�ԍ�����̏ꍇ�̓X�L�b�v
			long  joinLineID = _ttol( relNodeRec.attr.at( rel_railway_node::kJoinLineInfoID ) );
			long  relNodeOID = _ttol( relNodeRec.attr.at( ipc_feature::kObjectID ) );

			// �S���H��ID���擾
			auto joinLineItr = m_joinLineInfo.find( joinLineID );
			if( joinLineItr == m_joinLineInfo.end() )
				continue;

			long lineID = _ttol( joinLineItr->second.attr.at( join_line_info::kLineID ) );
			if( rwNodeMap[lineID].count( stationNum ) ){
				// ���G���[ �w�ԍ����d�����Ă���
				info1.Format( _T("%ld(%s)"), lineID, GetLineName(lineID) );
				info2.Format( _T("RailNodeID:%ld([%s])"), rwNodeMap.at( lineID ).at( stationNum ), stationNum );
				//IFeaturePtr ipFeature = m_ipRailNodeFC->_GetFeature( rwNodeID );
				//auto& modOperator = str_util::ToString( ipFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kOperator ) ) );
				//auto& modDate     = str_util::ToString( ipFeature->GetValue( m_railNodeFM.FindField( ipc_feature::kModifyDate ) ) );
				//m_output.OutputLog( railway_node::kTableName, rwNodeID, err_level::error, err_code::DupStationNum, modOperator, modDate, info1, stationNum );
				m_output.OutputLog( railway_node::kTableName, rwNodeID, err_level::error, err_code::DupStationNum, _T(""), _T(""), info1, info2 );
			}
			else{
				rwNodeMap[lineID].emplace( stationNum, rwNodeID );
			}
		}
	}

	// �H��ID���Ƃɉw�ԍ��̕��я����m�F����
	for( const auto& line : rwNodeMap ){
		long lineID = line.first;
		CString compPreFix;
		long    compNumber = -1;
		int     i = 0;
		for( const auto& staNum : line.second ){
			int numPos = staNum.first.FindOneOf( _T("0123456789") );
			if( numPos < 0 ){
				// ���G���[ �w�ԍ��ɐ��������݂��Ȃ�
				info1.Format( _T("%ld(%s)"), lineID, GetLineName(lineID) );
				m_output.OutputLog( railway_node::kTableName, staNum.second, err_level::error, err_code::NoNumberStaNum, _T(""), _T(""), info1, staNum.first );
			}

			CString preFix = staNum.first.Left( numPos );
			long number    = _ttol( staNum.first.Mid( numPos ) );
			if( i != 0 && ( 0 != preFix.Compare( compPreFix ) || number != compNumber + 1 ) ){
				// ���G���[ �w�ԍ��̕��я����s��
				info1.Format( _T("%ld(%s)"), lineID, GetLineName(lineID) );
				info2.Format(_T("%s("), staNum.first );
				for( const auto& errSta : line.second )
					info2.AppendFormat(_T("'%s' "), errSta.first );
				info2 += _T(")");
				m_output.OutputLog( railway_node::kTableName, staNum.second, err_level::error, err_code::NgSeqStaNum, _T(""), _T(""), info1, info2 );
				break;
			}
			compPreFix = preFix;
			compNumber = number;
			++i;
		}
	}
}

// �w�肵���֘A�e�[�u���ƕR�t���S��POI�̃e�[�u���̃��R�[�h�����݂��邩�`�F�b�N����
void CCheckRailwayNW::checkRelRecordExstence(const CString& featureClassName, const map<long, vector<recClass>>& relRecMap, const std::map<long, recClass>& poiRecMap){
	err_code::ECode eCode;
	CString targetFieldName;

	// �S���m�[�h�̏ꍇ��JOIN_LINE_INFO_ID���`�F�b�N����
	if(featureClassName.CompareNoCase(railway_node::kTableName)==0){
		eCode = err_code::NonJoinLinkInfo;
		targetFieldName = rel_railway_node::kJoinLineInfoID;
	}
	// �S�������N�̏ꍇ��RAILWAY_LINE_ID���`�F�b�N����
	else if(featureClassName.CompareNoCase(railway_link::kTableName)==0){
		eCode = err_code::NonRailwayLine;
		targetFieldName = rel_railway_link::kRailwayLineID;
	}
	else
		return;

	for(const auto& rel : relRecMap){
		for(const auto& relRec : rel.second){
			CString poiRecOID = relRec.attr.at(targetFieldName);
			if(poiRecMap.find(_ttol(poiRecOID)) != poiRecMap.end())
				continue; // �������OK

			// ���G���[ �R�t��������H�������݂��Ȃ�
			// ���G���[ �R�t���S���H�������݂��Ȃ�
			m_output.OutputLog(featureClassName, rel.first, err_level::error, eCode, _T(""), _T(""), poiRecOID);
		}
	}
}
