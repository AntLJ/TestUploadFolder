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
#include "RoadNameCnv.h"

using namespace sindy::schema;

bool RoadNameCnv::setSubTblFieldIndex()
{
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kNameKanji), &m_roadCodeListNameKanjiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kDisplayKanji);
		return false;
	}
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kNameYomi), &m_roadCodeListNameYomiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kDisplayYomi);
		return false;
	}
	return SplitBasicTokenRuleCnv::setSubTblFieldIndex();
}

bool RoadNameCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// ŠÖ˜A•t‚­“¹˜H˜HüƒŠƒXƒgæ“¾
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;
	if (!ipCode) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::road_code_list::kTableName);
		return false;
	}
	// “¹˜H˜HüƒŠƒXƒg‚©‚çLINE_CLASS_C, NAME_KANJI, NAME_YOMI‚ğæ“¾‚µA–¼Ìƒf[ƒ^ŒŸõ
	CComVariant roadCodeListLineClsVal, roadCodeListNameKanjiVal, roadCodeListNameYomiVal;
	if (S_OK != ipCode->get_Value(m_roadCodeListLineClsIndex, &roadCodeListLineClsVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kLineClass);
		return false;
	}
	if (S_OK != ipCode->get_Value(m_roadCodeListNameKanjiIndex, &roadCodeListNameKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kNameKanji);
		return false;
	}
	if (S_OK != ipCode->get_Value(m_roadCodeListNameYomiIndex, &roadCodeListNameYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kNameYomi);
		return false;
	}
	// –¼ÌA“Ç‚İ‚Ìİ’è
	CString roadCodeListNameKanji = roadCodeListNameKanjiVal.bstrVal,
		roadCodeListNameYomi = roadCodeListNameYomiVal.bstrVal;
	if ((::road_code_list::line_class::kFerry != roadCodeListLineClsVal.lVal) &&
		(::road_code_list::line_class::kNormal != roadCodeListLineClsVal.lVal)) {
		// ––”ö‚Ì”š‚ÆAŠ‡ŒÊ{Š‡ŒÊ“à•¶š‚ğíœ
		roadCodeListNameKanji = roadCodeListNameKanji.TrimRight(_T("‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X"));
		roadCodeListNameYomi = roadCodeListNameYomi.TrimRight(_T("‚O‚P‚Q‚R‚S‚T‚U‚V‚W‚X"));
		roadCodeListNameKanji = removeBracketedPhrase(roadCodeListNameKanji);
		roadCodeListNameYomi = removeBracketedPhrase(roadCodeListNameYomi);
	}
	RoadNameKey roadNameKey(roadCodeListNameKanji, roadCodeListNameYomi);
	std::map<RoadNameKey, RoadNameRecord>::const_iterator roadNameItr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == roadNameItr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_code_list::kTableName, codeOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = roadNameItr->second;
	return true;
}


CString RoadNameCnv::removeBracketedPhrase(CString strRoadName)
{
	// Š‡ŒÊ‚Í³‚µ‚­İ’è‚³‚ê‚Ä‚¢‚é‘O’ñ‚Ìˆ—
	// ƒƒWƒbƒNF
	//    1.æ“ª‚©‚çujv‚ğ’Tõ
	//    2.1‚ÅŒ©‚Â‚©‚Á‚½ˆÊ’u‚©‚ç‹tŒü‚«‚Éuiv‚ğ’Tõ
	//    3.1‚Æ2‚ÌŠÔ‚ğíœ
	//    4.Š‡ŒÊ‚ª‚È‚­‚È‚é‚Ü‚ÅŒJ‚è•Ô‚µ
	while (1) {
		int iEnd = strRoadName.Find(_T("j"));
		if (iEnd < 0) {
			// Š‡ŒÊ‚ª‚È‚¯‚ê‚ÎI—¹
			break;
		}
		CString strRoadNameTemp = strRoadName.Left(iEnd);
		int iStart = strRoadNameTemp.ReverseFind(_T('i'));
		if (iStart < 0) {
			// Š‡ŒÊ‚ª‚È‚¯‚ê‚ÎI—¹
			break;
		}
		// Š‡ŒÊ“à•¶š—ñíœiŠ‡ŒÊ‚àŠÜ‚Şj
		strRoadName.Delete(iStart, iEnd - iStart + 1);
	}
	return strRoadName;
}