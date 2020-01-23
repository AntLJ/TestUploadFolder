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
#include "RoadHwyAbbrCnv.h"

using namespace sindy::schema;

bool RoadHwyAbbrCnv::setSubTblFieldIndex()
{
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kDisplayKanji), &m_roadCodeListDisplayKanjiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kDisplayKanji);
		return false;
	}
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kDisplayYomi), &m_roadCodeListDisplayYomiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kDisplayYomi);
		return false;
	}
	return SplitBasicTokenRuleCnv::setSubTblFieldIndex();
}

bool RoadHwyAbbrCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// 関連付く道路路線リスト取得
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;
	if (!ipCode) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::road_code_list::kTableName);
		return false;
	}
	// 道路路線リストからDISPLAY_KANJI, DISPLAY_YOMIを取得し、名称データ検索
	CComVariant roadCodeListDisplayKanjiVal, roadCodeListDisplayYomiVal;
	if (S_OK != ipCode->get_Value(m_roadCodeListDisplayKanjiIndex, &roadCodeListDisplayKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kDisplayKanji);
		return false;
	}
	if (S_OK != ipCode->get_Value(m_roadCodeListDisplayYomiIndex, &roadCodeListDisplayYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kDisplayYomi);
		return false;
	}
	RoadNameKey roadNameKey(roadCodeListDisplayKanjiVal.bstrVal, roadCodeListDisplayYomiVal.bstrVal);
	std::map<RoadNameKey, RoadNameRecord>::const_iterator roadNameItr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == roadNameItr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_code_list::kTableName, codeOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = roadNameItr->second;
	return true;
}