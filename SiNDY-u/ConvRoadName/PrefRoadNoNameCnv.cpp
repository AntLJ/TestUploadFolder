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
#include "PrefRoadNoNameCnv.h"

using namespace sindy::schema;

bool PrefRoadNoNameCnv::setSubTblFieldIndex()
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

bool PrefRoadNoNameCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// 道路リンクからPREF_CODE, ROAD_NO、都道府県リストからNAME_KANJI, NAME_YOMIを取得し、検索用名称作成
	CComVariant linkPrefCodeVal, linkRoadNoVal, prefRdNameKanjiVal, prefRdNameYomiVal;
	if (S_OK != ipLink->get_Value(m_roadLinkPrefCodeIndex, &linkPrefCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &linkRoadNoVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadNo);
		return false;
	}
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameKanjiIndex, &prefRdNameKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameKanji);
		return false;
	}
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameYomiIndex, &prefRdNameYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameYomi);
		return false;
	}
	// 名称・読みを作成
	RoadNameKey roadNameKey(_T(""), _T(""));
	roadNameKey.m_name.Format(_T("%s道%s号%s"), prefCode2PrefName(linkPrefCodeVal.lVal), num2ZenStr(linkRoadNoVal.lVal), prefRdNameKanjiVal.bstrVal);
	roadNameKey.m_yomi.Format(_T("%sドウ%sゴウ%s"), prefCode2PrefYomi(linkPrefCodeVal.lVal), num2YomiStr(linkRoadNoVal.lVal), prefRdNameYomiVal.bstrVal);

	// 道路名称データ検索
	std::map<RoadNameKey, RoadNameRecord>::const_iterator itr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == itr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::pref_road_list::kTableName, subOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = itr->second;
	return true;
}

bool PrefRoadNoNameCnv::cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// 道路名称分割データ シーケンス1登録
	CComVariant linkPrefCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkPrefCodeIndex, &linkPrefCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	CString name1, yomi1;
	name1.Format(_T("%s道"), prefCode2PrefName(linkPrefCodeVal.lVal));
	yomi1.Format(_T("%sドウ"), prefCode2PrefYomi(linkPrefCodeVal.lVal));
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
	// 県道名称データの名称・読みを分割してシーケンス3以降を登録
	CComVariant prefRdNameKanjiVal, prefRdNameYomiVal;
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameKanjiIndex, &prefRdNameKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameKanji);
		return false;
	}
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameYomiIndex, &prefRdNameYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameYomi);
		return false;
	}
	// 名称分割用に対象道路リンクの路線種別取得
	long lineCls = -1;
	if (!getLineCls(linkOID, ipLink, lineCls))
		return false;
	// 県道名称・読みの分割
	std::list<SplitElement> mainNameSpltElem, mainYomiSpltElem;
	basicTokenRuleCnv(subOID, ::pref_road_list::kTableName, lineCls, prefRdNameKanjiVal.bstrVal, prefRdNameYomiVal.bstrVal, mainNameSpltElem, mainYomiSpltElem);
	std::list<SplitElement>::const_iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	long seq = 3;
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// 名称・読みがNULLの場合は飛ばす
		if (nameItr->m_str.IsEmpty() || yomiItr->m_str.IsEmpty()) {
			if (!(nameItr->m_str.IsEmpty() && yomiItr->m_str.IsEmpty())) {
				Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::UnsymmetricalWord, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			}
			++nameItr;
			++yomiItr;
			continue;
		}
		SpltNameRecord spltNameRecord(roadNameRecord.m_nameID, nameItr->m_class, seq, nameItr->m_str, yomiItr->m_str);
		if (!spltRdNameRecs.insert(spltNameRecord).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			return false;
		}
		++seq;
		++nameItr;
		++yomiItr;
	}
	// どちらかのイテレータがend()を指していない場合は、分割数が違うのでメッセージ出力
	if ((mainNameSpltElem.end() != nameItr) || (mainYomiSpltElem.end() != yomiItr)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, ::sj::road_name_list::kTableName, roadNameRecord.m_nameID);
		m_isErr = true;
	}
	return true;
}