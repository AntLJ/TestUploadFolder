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
#include "PrefRoadNoAbbrCnv.h"

using namespace sindy::schema;

bool PrefRoadNoAbbrCnv::setRoadLinkFieldIndex()
{
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kPrefCode), &m_roadLinkPrefCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kPrefCode);
		return false;
	}
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadNo), &m_roadLinkRoadNoIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadNo);
		return false;
	}
	return true;
}

bool PrefRoadNoAbbrCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// 道路リンクからPREF_CODE, ROAD_NOを取得して検索用名称作成
	CComVariant linkPrefCodeVal, linkRoadNoVal;
	if (S_OK != ipLink->get_Value(m_roadLinkPrefCodeIndex, &linkPrefCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &linkRoadNoVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	// 名称・読みを作成
	RoadNameKey roadNameKey(_T(""), _T(""));
	roadNameKey.m_name.Format(_T("%s道%s号"), prefCode2PrefSuffixName(linkPrefCodeVal.lVal), num2ZenStr(linkRoadNoVal.lVal));
	roadNameKey.m_yomi.Format(_T("%sドウ%sゴウ"), prefCode2PrefSuffixYomi(linkPrefCodeVal.lVal), num2YomiStr(linkRoadNoVal.lVal));

	// 道路名称データ検索
	std::map<RoadNameKey, RoadNameRecord>::const_iterator itr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == itr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = itr->second;
	return true;
}

bool PrefRoadNoAbbrCnv::cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// 道路名称分割データ シーケンス1登録
	CComVariant linkPrefCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkPrefCodeIndex, &linkPrefCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	CString name1, yomi1;
	name1.Format(_T("%s道"), prefCode2PrefSuffixName(linkPrefCodeVal.lVal));
	yomi1.Format(_T("%sドウ"), prefCode2PrefSuffixYomi(linkPrefCodeVal.lVal));
	SpltNameRecord spltNameRecord1(roadNameRecord.m_nameID, ::split_class::kBody, 1, name1, yomi1);
	if (!spltRdNameRecs.insert(spltNameRecord1).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		return false;
	}
	// シーケンス2登録
	CComVariant linkRoadNoVal;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &linkRoadNoVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadNo);
		return false;
	}
	CString name2, yomi2;
	name2.Format(_T("%s号"), num2ZenStr(linkRoadNoVal.lVal));
	yomi2.Format(_T("%sゴウ"), num2YomiStr(linkRoadNoVal.lVal));
	SpltNameRecord spltNameRecord2(roadNameRecord.m_nameID, ::split_class::kBody, 2, name2, yomi2);
	if (!spltRdNameRecs.insert(spltNameRecord2).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		return false;
	}
	return true;
}