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
#include "ShieldRoadNoCnv.h"

using namespace sindy::schema;

bool ShieldRoadNoCnv::setRoadLinkFieldIndex()
{
	// 必要indexの取得
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadNo), &m_roadLinkRoadNoIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadNo);
		return false;
	}
	return true;
}

bool ShieldRoadNoCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// RoadNo取得
	CComVariant roadNoVal;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &roadNoVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadNo);
		return false;
	}
	
	// 道路名称データ検索
	roadNoVal.ChangeType(VT_BSTR);
	RoadNameKey roadNameKey(roadNoVal.bstrVal, _T(""));
	std::map<RoadNameKey, RoadNameRecord>::const_iterator itr = m_tgtRdNameRecs.find(roadNameKey);
	if (itr == m_tgtRdNameRecs.end()) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::sj::road_name_list::kTableName);
		return false;
	}	
	roadNameRecord = itr->second;
	return true;
}