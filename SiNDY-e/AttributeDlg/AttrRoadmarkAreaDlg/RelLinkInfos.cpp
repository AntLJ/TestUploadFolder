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
#include "RelLinkInfos.h"


// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindylib_base
#include <TDC/sindylib_base/TableContainer.h>


bool RelLinkInfos::read()
{
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	using namespace uh;

	m_error = true;

	if (! m_relTable || ! m_linkTable)
		return false;

	CString subFields = str_util::format( _T("%s,%s,%s"), 
		rel_roadmark_area_lane_link::kObjectID, 
		rel_roadmark_area_lane_link::kRoadMarkAreaGID,
		rel_roadmark_area_lane_link::kLaneLinkGID);
	CString whereClause = str_util::format( _T("%s='%s'"), 
		rel_roadmark_area_lane_link::kRoadMarkAreaGID,
		m_roadmarkGid);
								
	_ICursorPtr cur; 
	if (FAILED(m_relTable->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &cur)))
		return false;
	if(! cur)
		return false;
	
	// ���H�W���|�C���g�ɕR�Â��m�[�h�̏����擾
	_IRowPtr row;
	while (cur->NextRow(&row) == S_OK && row)
	{
		CComVariant vaLinkGid = AheGetAttribute( row, rel_roadmark_area_lane_link::kLaneLinkGID);
		if( vaLinkGid.vt != VT_BSTR )
			return false;

		whereClause = str_util::format( _T("%s='%s'"), kGlobalID, vaLinkGid.bstrVal );
		_ICursorPtr linkCur;
		if( FAILED(m_linkTable->Search( AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &linkCur)))
			return false;
		if(! linkCur)
			return false;

		IFeaturePtr feature;
		if (IFeatureCursorPtr(linkCur)->NextFeature(&feature) != S_OK)
			return false;
		if(! feature)
			return false;

		long linkOid(-1);
		feature->get_OID(&linkOid);

		IGeometryPtr geo;
		feature->get_ShapeCopy(&geo);
		IPolylinePtr polyline(geo);
		if(! polyline)
			return false;

		m_relLinks.emplace(linkOid, vaLinkGid.bstrVal, polyline);
	}

	// �ύX�󋵂�m�邽�߂ɏ����̃m�[�h��Ԃ������Ă���
	m_prevRelLinks = m_relLinks;

	m_error = false;
	return true;
}

bool RelLinkInfos::reloadData(const std::vector<long>& oids, std::vector<long>& err_oids)
{
	using namespace sindy::schema::adam_v2_global_ipc_feature;

	m_relLinks.clear();
	for( long oid : oids )
	{
		// ��ʂ̃f�[�^��ǂݍ��ނ��Ƃ͍l���ɂ������߁AGetRow �ōs�Ȃ��Ă��܂�
		_IRowPtr row;
		if( FAILED(m_linkTable->GetRow(oid, &row)) ){
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
		IPolylinePtr polyline(geo);
		if(! polyline){
			err_oids.push_back(oid);
			continue;
		}

		m_relLinks.emplace(oid, vGid.bstrVal, polyline);
	}

	if(! err_oids.empty() ){
		m_error = true;
		return false;
	}

	m_error = false;
	return true;
}

bool RelLinkInfos::updateTable()
{
	using namespace sindy::schema::adam_v2::rel_roadmark_area_lane_link;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;
	using namespace uh;

	if (! AheIsEditableTable(m_relTable))
		return false;

	// �ǉ��Ώۂ̃��R�[�h�擾
	std::vector<RelLink> insertRecords;
	for( const auto& rec : m_relLinks ) {
		if( m_prevRelLinks.find(rec) == m_prevRelLinks.end() )
			insertRecords.push_back(rec);
	}

	// �폜�Ώۂ�GID�Q�擾
	std::list<CString> deleteRecords;
	for( const auto& rec : m_prevRelLinks ) {
		if( m_relLinks.find(rec) == m_relLinks.end() )
			deleteRecords.push_back(rec.m_LinkGid);
	}

	// �}��
	if(! insertRecords.empty() )
	{
		for( const auto& rec : insertRecords )
		{
			_IRowPtr ipBuffer;
			m_relTable->CreateRow(&ipBuffer);

			AheSetModifyData(ipBuffer, sindyUpdateTypeCreate);
			AheSetAttribute (ipBuffer, kRoadMarkAreaGID, m_roadmarkGid);
			AheSetAttribute (ipBuffer, kLaneLinkGID, rec.m_LinkGid);
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
			kRoadMarkAreaGID, m_roadmarkGid,
			kLaneLinkGID, inClauses.front());

		IQueryFilterPtr query = AheInitQueryFilter(nullptr, nullptr, where_clause);
		if(FAILED( m_relTable->DeleteSearchedRows(query)))
			return false;
	}

	return true;
}

std::vector<long> RelLinkInfos::getOIDs() const
{
	std::vector<long> oids;
	for( auto info : m_relLinks )
		oids.push_back(info.m_LinkOid);

	return oids;
}

std::vector<IPolylinePtr> RelLinkInfos::getLinkPolylines() const
{
	std::vector<IPolylinePtr> retPolylines;
	for( auto rec : m_relLinks )
		retPolylines.push_back(rec.m_Polyline);

	return retPolylines;
}

bool RelLinkInfos::isChange() const
{
	// �ǉ����ꂽ���R�[�h�����邩
	for( auto& rec : m_relLinks ) {
		if( m_prevRelLinks.count(rec) == 0 )
			return true;
	}

	// �폜���ꂽ���R�[�h�����邩
	for( auto& rec : m_prevRelLinks ) {
		if( m_relLinks.count(rec) == 0 )
			return true;
	}

	return false;
}

std::vector<IPolylinePtr> RelLinkInfos::getPolylines(const std::vector<long>& oids) const
{
	std::vector<IPolylinePtr> polylines;

	for( long oid : oids )
	{
		auto iter = m_relLinks.find( RelLink(oid,_T(""), IPolylinePtr()));
		if( iter == m_relLinks.end() )
			continue;
		
		polylines.push_back( iter->m_Polyline );
	}

	return polylines;
}

bool RelLinkInfos::isExist(const RelLink& rel_link)
{
	return 
		std::find_if(std::begin(m_relLinks), std::end(m_relLinks), 
			[&rel_link](const RelLink& target ){
				return rel_link.m_LinkOid == target.m_LinkOid;

		})
		!= m_relLinks.end();
}
