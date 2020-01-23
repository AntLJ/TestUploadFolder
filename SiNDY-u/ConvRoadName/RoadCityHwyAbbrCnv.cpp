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
#include "RoadCityHwyAbbrCnv.h"
#include <boost/regex.hpp>

using namespace sindy::schema;

bool RoadCityHwyAbbrCnv::setSubTblFieldIndex()
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

bool RoadCityHwyAbbrCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// �֘A�t�����H�H�����X�g�擾
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;
	if (!ipCode) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::road_code_list::kTableName);
		return false;
	}
	// ���H�H�����X�g����DISPLAY_KANJI, DISPLAY_YOMI���擾
	CComVariant roadCodeListDisplayKanjiVal, roadCodeListDisplayYomiVal;
	if (S_OK != ipCode->get_Value(m_roadCodeListDisplayKanjiIndex, &roadCodeListDisplayKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kDisplayKanji);
		return false;
	}
	if (S_OK != ipCode->get_Value(m_roadCodeListDisplayYomiIndex, &roadCodeListDisplayYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kDisplayYomi);
		return false;
	}
	// ��������H
	CString nameAbbr = MkAbbrStr(roadCodeListDisplayKanjiVal.bstrVal, delNameWord);
	CString yomiAbbr = MkAbbrStr(roadCodeListDisplayYomiVal.bstrVal, delYomiWord);
	// ��������H�̌��ʁANULL�ƂȂ����ꍇ��
	if (nameAbbr.IsEmpty() && yomiAbbr.IsEmpty()) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::info, err_code::NullStrData, ::road_code_list::kTableName, codeOID);
		return true;
	}
	// ���H���̃f�[�^����
	RoadNameKey roadNameKey(nameAbbr, yomiAbbr);
	std::map<RoadNameKey, RoadNameRecord>::const_iterator roadNameItr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == roadNameItr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_code_list::kTableName, codeOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = roadNameItr->second;
	return true;
}

CString RoadCityHwyAbbrCnv::MkAbbrStr(const CString& str, const CString delWords[])
{
	int i = 0;
	while (delWords[i] != _T("")) {
		//����������쐬
		std::wstring strW = CT2CW(str);
		CStringW searchStr;
		searchStr.Format(_T("%s.*"), delWords[i]);
		boost::wregex reg(searchStr);
		if (boost::regex_match(strW, reg)) {
			long delWordCount = delWords[i].GetLength();
			CStringW procStr = strW.c_str();
			CStringW tgtStr = procStr.Mid(delWordCount);
			return CW2CT(tgtStr);
		}
		++i;
	}
	return str;
}