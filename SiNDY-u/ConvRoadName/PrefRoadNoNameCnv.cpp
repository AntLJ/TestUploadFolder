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
	// ���H�����N����PREF_CODE, ROAD_NO�A�s���{�����X�g����NAME_KANJI, NAME_YOMI���擾���A�����p���̍쐬
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
	// ���́E�ǂ݂��쐬
	RoadNameKey roadNameKey(_T(""), _T(""));
	roadNameKey.m_name.Format(_T("%s��%s��%s"), prefCode2PrefName(linkPrefCodeVal.lVal), num2ZenStr(linkRoadNoVal.lVal), prefRdNameKanjiVal.bstrVal);
	roadNameKey.m_yomi.Format(_T("%s�h�E%s�S�E%s"), prefCode2PrefYomi(linkPrefCodeVal.lVal), num2YomiStr(linkRoadNoVal.lVal), prefRdNameYomiVal.bstrVal);

	// ���H���̃f�[�^����
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
	// ���H���̕����f�[�^ �V�[�P���X1�o�^
	CComVariant linkPrefCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkPrefCodeIndex, &linkPrefCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	CString name1, yomi1;
	name1.Format(_T("%s��"), prefCode2PrefName(linkPrefCodeVal.lVal));
	yomi1.Format(_T("%s�h�E"), prefCode2PrefYomi(linkPrefCodeVal.lVal));
	SpltNameRecord spltNameRecord1(roadNameRecord.m_nameID, ::split_class::kBody, 1, name1, yomi1);
	if (!spltRdNameRecs.insert(spltNameRecord1).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		return false;
	}
	// �V�[�P���X2�o�^
	CComVariant linkRoadNoVal;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &linkRoadNoVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadNo);
		return false;
	}
	CString name2, yomi2;
	name2.Format(_T("%s��"), num2ZenStr(linkRoadNoVal.lVal));
	yomi2.Format(_T("%s�S�E"), num2YomiStr(linkRoadNoVal.lVal));
	SpltNameRecord spltNameRecord2(roadNameRecord.m_nameID, ::split_class::kBody, 2, name2, yomi2);
	if (!spltRdNameRecs.insert(spltNameRecord2).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		return false;
	}
	// �������̃f�[�^�̖��́E�ǂ݂𕪊����ăV�[�P���X3�ȍ~��o�^
	CComVariant prefRdNameKanjiVal, prefRdNameYomiVal;
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameKanjiIndex, &prefRdNameKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameKanji);
		return false;
	}
	if (S_OK != ipSubRow->get_Value(m_prefRdListNameYomiIndex, &prefRdNameYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, subOID, ::pref_road_list::kNameYomi);
		return false;
	}
	// ���̕����p�ɑΏۓ��H�����N�̘H����ʎ擾
	long lineCls = -1;
	if (!getLineCls(linkOID, ipLink, lineCls))
		return false;
	// �������́E�ǂ݂̕���
	std::list<SplitElement> mainNameSpltElem, mainYomiSpltElem;
	basicTokenRuleCnv(subOID, ::pref_road_list::kTableName, lineCls, prefRdNameKanjiVal.bstrVal, prefRdNameYomiVal.bstrVal, mainNameSpltElem, mainYomiSpltElem);
	std::list<SplitElement>::const_iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	long seq = 3;
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// ���́E�ǂ݂�NULL�̏ꍇ�͔�΂�
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
	// �ǂ��炩�̃C�e���[�^��end()���w���Ă��Ȃ��ꍇ�́A���������Ⴄ�̂Ń��b�Z�[�W�o��
	if ((mainNameSpltElem.end() != nameItr) || (mainYomiSpltElem.end() != yomiItr)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, ::sj::road_name_list::kTableName, roadNameRecord.m_nameID);
		m_isErr = true;
	}
	return true;
}