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
#include "SectionNameCnv.h"

using namespace sindy::schema;

bool SectionNameCnv::setRoadLinkFieldIndex()
{
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kSectionCode), &m_roadLinkSectionCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName);
		return false;
	}
	return SplitBasicTokenRuleCnv::setRoadLinkFieldIndex();
}

bool SectionNameCnv::setSubTblFieldIndex()
{
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::section_code_list::kSectionCode), &m_sectionCodeListSectionCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::section_code_list::kTableName);
		return false;
	}
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::section_code_list::kNameKanji), &m_sectionCodeListNameKanjiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::section_code_list::kTableName);
		return false;
	}
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::section_code_list::kNameYomi), &m_sectionCodeListNameYomiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::section_code_list::kTableName);
		return false;
	}
	return SplitBasicTokenRuleCnv::setSubTblFieldIndex();
}

bool SectionNameCnv::loadSubTable()
{
	// �S�Ẵ��R�[�h���擾���邪�A���v���J�ł������悤IQeryPtr���`(��������IQueryPtr��^���Ȃ��ƃ��v���J�œ��삵�Ȃ�)
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// �J�[�\���擾
	_ICursorPtr ipSectionCodeListCursor;
	if (S_OK != m_ipSubNameTable->Search(ipQFilter, VARIANT_FALSE, &ipSectionCodeListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::section_code_list::kTableName);
		return false;
	}
	// �ϊ��Ώ�SectionCodeList�擾
	_IRowPtr ipSec;
	while (S_OK == ipSectionCodeListCursor->NextRow(&ipSec) && ipSec) {
		// OBJECTID�擾
		long secOID = -1;
		if (S_OK != ipSec->get_OID(&secOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::section_code_list::kTableName);
			return false;
		}
		// SECTION_CODE���擾���A�R���e�i�ɑ}��
		CComVariant sectionCodeVal;
		if (S_OK != ipSec->get_Value(m_sectionCodeListSectionCodeIndex, &sectionCodeVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::section_code_list::kTableName, secOID, ::section_code_list::kSectionCode);
			return false;
		}
		if (!m_tgtSecCodeListRecs.insert(std::make_pair(sectionCodeVal.lVal, ipSec)).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, ::section_code_list::kTableName, secOID);
			return false;
		}
	}
	return SplitBasicTokenRuleCnv::loadSubTable();
}

bool SectionNameCnv::findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow)
{
	// ���H�����N���SECTION_CODE���擾���A����
	CComVariant sectionCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkSectionCodeIndex, &sectionCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kSectionCode);
		return false;
	}
	std::map<long, _IRowPtr>::const_iterator itr = m_tgtSecCodeListRecs.find(sectionCodeVal.lVal);
	if (itr == m_tgtSecCodeListRecs.end()) {
		// �擾�ł��Ȃ��ꍇ������̂ŁA�G���[�ɂ͂��Ȃ�
		ipSubRow = nullptr;
		subOID = -1;
		return true;
	}
	ipSubRow = itr->second;
	if (S_OK != ipSubRow->get_OID(&subOID)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::section_code_list::kTableName);
		return false;
	}
	return true;
}

bool SectionNameCnv::chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv)
{
	chkCnv = true;
	// ipSubRow��NULL�̏ꍇ�͕ϊ��ΏۊO
	if (!ipSubRow) {
		chkCnv = false;
		return true;
	}
	return true;
}

bool SectionNameCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// ��Ԗ��̃��X�g��NAME_KANJI, NAME_YOMI���擾������
	CComVariant nameKanjiVal, nameYomiVal;
	if (S_OK != ipSubRow->get_Value(m_sectionCodeListNameKanjiIndex, &nameKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::section_code_list::kTableName, subOID);
		return false;
	}
	if (S_OK != ipSubRow->get_Value(m_sectionCodeListNameYomiIndex, &nameYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::section_code_list::kTableName, subOID);
		return false;
	}
	// ���H���̃f�[�^����
	RoadNameKey roadNameKey(nameKanjiVal.bstrVal, nameYomiVal.bstrVal);
	std::map<RoadNameKey, RoadNameRecord>::const_iterator roadNameItr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == roadNameItr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::section_code_list::kTableName, subOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = roadNameItr->second;
	return true;
}