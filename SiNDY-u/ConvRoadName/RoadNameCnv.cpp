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
	// 関連付く道路路線リスト取得
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;
	if (!ipCode) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::road_code_list::kTableName);
		return false;
	}
	// 道路路線リストからLINE_CLASS_C, NAME_KANJI, NAME_YOMIを取得し、名称データ検索
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
	// 名称、読みの設定
	CString roadCodeListNameKanji = roadCodeListNameKanjiVal.bstrVal,
		roadCodeListNameYomi = roadCodeListNameYomiVal.bstrVal;
	if ((::road_code_list::line_class::kFerry != roadCodeListLineClsVal.lVal) &&
		(::road_code_list::line_class::kNormal != roadCodeListLineClsVal.lVal)) {
		// 末尾の数字と、括弧＋括弧内文字を削除
		roadCodeListNameKanji = roadCodeListNameKanji.TrimRight(_T("０１２３４５６７８９"));
		roadCodeListNameYomi = roadCodeListNameYomi.TrimRight(_T("０１２３４５６７８９"));
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
	// 括弧は正しく設定されている前提の処理
	// ロジック：
	//    1.先頭から「）」を探索
	//    2.1で見つかった位置から逆向きに「（」を探索
	//    3.1と2の間を削除
	//    4.括弧がなくなるまで繰り返し
	while (1) {
		int iEnd = strRoadName.Find(_T("）"));
		if (iEnd < 0) {
			// 括弧がなければ終了
			break;
		}
		CString strRoadNameTemp = strRoadName.Left(iEnd);
		int iStart = strRoadNameTemp.ReverseFind(_T('（'));
		if (iStart < 0) {
			// 括弧がなければ終了
			break;
		}
		// 括弧内文字列削除（括弧も含む）
		strRoadName.Delete(iStart, iEnd - iStart + 1);
	}
	return strRoadName;
}