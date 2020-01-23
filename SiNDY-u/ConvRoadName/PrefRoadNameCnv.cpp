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
#include "PrefRoadNameCnv.h"

using namespace sindy::schema;

bool PrefRoadNameCnv::setSubTblFieldIndex()
{
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::pref_road_list::kNameKanji), &m_prefRdListNameKanjiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::pref_road_list::kTableName, ::pref_road_list::kNameKanji);
		return false;
	}
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::pref_road_list::kNameYomi), &m_prefRdListNameYomiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::pref_road_list::kTableName, ::pref_road_list::kNameYomi);
		return false;
	}
	return PrefNameBaseCnv::setSubTblFieldIndex();
}

bool PrefRoadNameCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// 都道府県リストからNAME_KANJI, NAME_YOMIを取得し、名称データ検索
	CComVariant prefRdNameKanji, prefRdNameYomi;
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameKanjiIndex, &prefRdNameKanji)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameKanji);
		return false;
	}
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameYomiIndex, &prefRdNameYomi)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameYomi);
		return false;
	}
	RoadNameKey roadNameKey(prefRdNameKanji.bstrVal, prefRdNameYomi.bstrVal);

	std::map<RoadNameKey, RoadNameRecord>::const_iterator itr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == itr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::pref_road_list::kTableName, subOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = itr->second;
	return true;
}

