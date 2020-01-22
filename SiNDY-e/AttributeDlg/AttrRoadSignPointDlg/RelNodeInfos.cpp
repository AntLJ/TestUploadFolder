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
#include "RelNodeInfos.h"


// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindylib_base
#include <TDC/sindylib_base/TableContainer.h>


bool RelNodeInfos::read()
{
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	using namespace uh;

	m_Error = true;

	if (! m_RelTable || ! m_NodeTable)
		return false;

	CString subFields = str_util::format( _T("%s,%s,%s"), 
								rel_lane_node_roadsign_point::kObjectID, 
								rel_lane_node_roadsign_point::kRoadsignPointGID,
								rel_lane_node_roadsign_point::kLaneNodeGID);
	CString whereClause = str_util::format( _T("%s='%s'"), 
								rel_lane_node_roadsign_point::kRoadsignPointGID,
								m_RoadsignGid);
								
	_ICursorPtr cur; 
	if (FAILED(m_RelTable->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &cur)))
		return false;
	if(! cur)
		return false;
	
	// ���H�W���|�C���g�ɕR�Â��m�[�h�̏����擾
	_IRowPtr row;
	while (cur->NextRow(&row) == S_OK && row)
	{
		CComVariant vaNodeGid = AheGetAttribute( row, rel_lane_node_roadsign_point::kLaneNodeGID );
		if( vaNodeGid.vt != VT_BSTR )
			return false;

		whereClause = str_util::format( _T("%s='%s'"), kGlobalID, vaNodeGid.bstrVal );
		_ICursorPtr nodeCur;
		if( FAILED(m_NodeTable->Search( AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &nodeCur)))
			return false;
		if(! nodeCur)
			return false;

		IFeaturePtr feature;
		if (IFeatureCursorPtr(nodeCur)->NextFeature(&feature) != S_OK)
			return false;
		if(! feature)
			return false;

		long nodeOid(-1);
		feature->get_OID(&nodeOid);

		IGeometryPtr geo;
		feature->get_ShapeCopy(&geo);
		IPointPtr point(geo);
		if(! point)
			return false;

		m_RelNodes.emplace(nodeOid, vaNodeGid.bstrVal, point);
	}

	// �ύX�󋵂�m�邽�߂ɏ����̃m�[�h��Ԃ������Ă���
	m_prevRelNodes = m_RelNodes;

	m_Error = false;
	return true;
}

bool RelNodeInfos::reloadData(const std::vector<long>& oids, std::vector<long>& err_oids)
{
	using namespace sindy::schema::adam_v2_global_ipc_feature;

	m_RelNodes.clear();
	for( long oid : oids )
	{
		// ��ʂ̃f�[�^��ǂݍ��ނ��Ƃ͍l���ɂ������߁AGetRow �ōs�Ȃ��Ă��܂�
		_IRowPtr row;
		if( FAILED(m_NodeTable->GetRow(oid, &row)) ){
			err_oids.push_back(oid);
			continue;
		}

		if(! row )
			return false;

		CComVariant vGid = AheGetAttribute( row, kGlobalID);
		if( vGid.vt == VT_NULL ){
			err_oids.push_back(oid);
			continue;
		}

		IGeometryPtr geo;
		IFeaturePtr(row)->get_ShapeCopy(&geo);
		IPointPtr point(geo);
		if(! point){
			err_oids.push_back(oid);
			continue;
		}

		m_RelNodes.emplace(oid, vGid.bstrVal, point);
	}

	if(! err_oids.empty() ){
		m_Error = true;
		return false;
	}

	m_Error = false;
	return true;
}

bool RelNodeInfos::updateTable()
{
	using namespace sindy::schema::adam_v2::rel_lane_node_roadsign_point;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;
	using namespace uh;

	if (! AheIsEditableTable(m_RelTable))
		return false;

	// �ǉ��Ώۂ̃��R�[�h�擾
	std::vector<RelNode> insertRecords;
	for( const auto& rec : m_RelNodes ) {
		if( m_prevRelNodes.find(rec) == m_prevRelNodes.end() )
			insertRecords.push_back(rec);
	}

	// �폜�Ώۂ�GID�Q�擾
	std::list<CString> deleteRecords;
	for( const auto& rec : m_prevRelNodes ) {
		if( m_RelNodes.find(rec) == m_RelNodes.end() )
			deleteRecords.push_back(rec.m_NodeGid);
	}

	// �}��
	if(! insertRecords.empty() )
	{
		for( const auto& rec : insertRecords )
		{
			_IRowPtr ipBuffer;
			m_RelTable->CreateRow(&ipBuffer);

			AheSetModifyData(ipBuffer, sindyUpdateTypeCreate);
			AheSetAttribute (ipBuffer, kRoadsignPointGID, m_RoadsignGid);
			AheSetAttribute (ipBuffer, kLaneNodeGID, rec.m_NodeGid);
			if (FAILED(ipBuffer->Store()))
				return false;
		}
	}

	// �폜
	if(! deleteRecords.empty())
	{
	    std::list<CString> inClauses;
		sindy::CTableBase::IDs2Str(deleteRecords, inClauses, _T("'"));

		// 1000���ȏ㓯���ɍ폜����邱�Ƃ͍l���ɂ����̂�
		// IN��̐����͖��l��
		CString where_clause = uh::str_util::format(_T("%s='%s' and %s in (%s)"), 
								kRoadsignPointGID, m_RoadsignGid,
								kLaneNodeGID, inClauses.front());

		IQueryFilterPtr query = AheInitQueryFilter(nullptr, nullptr, where_clause);
		if(FAILED( m_RelTable->DeleteSearchedRows(query)))
			return false;
	}

	return true;
}

std::vector<long> RelNodeInfos::getOIDs() const
{
	std::vector<long> oids;
	for( auto info : m_RelNodes )
		oids.push_back(info.m_NodeOid);

	return oids;
}

std::vector<IPointPtr> RelNodeInfos::getNodePoints() const
{
	std::vector<IPointPtr> retPoints;
	for( auto rec : m_RelNodes )
		retPoints.push_back(rec.m_Point);

	return retPoints;
}

bool RelNodeInfos::isChange() const
{
	// �ǉ����ꂽ���R�[�h�����邩
	for( auto& rec : m_RelNodes ) {
		if( m_prevRelNodes.find(rec) == m_prevRelNodes.end() )
			return true;
	}

	// �폜���ꂽ���R�[�h�����邩
	for( auto& rec : m_prevRelNodes ) {
		if( m_RelNodes.find(rec) == m_RelNodes.end() )
			return true;
	}

	return false;
}

std::vector<IPointPtr> RelNodeInfos::getPoints(const std::vector<long>& oids) const
{
	std::vector<IPointPtr> points;

	for( long oid : oids )
	{
		auto iter = m_RelNodes.find( RelNode(oid,_T(""), IPointPtr()));
		if( iter == m_RelNodes.end() )
			continue;
		
		points.push_back( iter->m_Point );
	}

	return points;
}

bool RelNodeInfos::isExist(const RelNode& rel_node)
{
	return 
		std::find_if(std::begin(m_RelNodes), std::end(m_RelNodes), 
			[&rel_node](const RelNode& target ){
				return rel_node.m_NodeOid == target.m_NodeOid;

		})
		!= m_RelNodes.end();
}
