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
#include "ConvRoadName.h"
#include "ShieldRoadNoCnv.h"
#include "ShieldHwyNoCnv.h"
#include "NationalRoadNoCnv.h"
#include "PrefRoadNoNameCnv.h"
#include "PrefRoadNoCnv.h"
#include "PrefRoadNoAbbrCnv.h"
#include "PrefRoadNameCnv.h"
#include "RoadNameCnv.h"
#include "RoadHwyAbbrCnv.h"
#include "RoadCityHwyAbbrCnv.h"
#include "RoadCityHwyAbbr2Cnv.h"
#include "SectionNameCnv.h"
#include "OutputFile.h"
#include <WinLib/CStringTokenizer.h>

// 処理実行
bool ConvRoadName::run(const Arguments& args)
{
	CString strMsg;
	// 出力クラス初期化
	OutputFile outputFile(args.m_outDir.c_str());
	if (!outputFile.init())
		return false;
	// 変換元テーブル取得
	strMsg.Format(_T("[%s] 変換元データ取得"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	if (!getTgtTbl(args))
		return false;
	// 辞書取得
	if (!getDictionary(args.m_dicPath.c_str()))
		return false;
	// 各種名称変換
	strMsg.Format(_T("[%s] 各種名称変換"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	std::set<RoadNameRecord> rdNameRecs;
	std::set<RelNameLinkRecord> rlNameLnkRecs;
	std::set<SpltNameRecord> spltRdNameRecs;
	if (!cnvEachName(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	// テキスト出力
	strMsg.Format(_T("[%s] ファイル出力"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	if (!outputFile.write(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	return true;
}

// 変換元テーブル取得
bool ConvRoadName::getTgtTbl(const Arguments& args)
{
	// 道路DB接続
	m_ipRoadWork = sindy::create_workspace(args.m_roadDB.c_str());
	if (!m_ipRoadWork) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDBConnect, args.m_roadDB.c_str());
		return false;
	}
	// 道路DB用のオーナ名
	CString roadOwner = args.m_roadOwner.c_str();
	// 道路名称DB接続
	m_ipNameWork = sindy::create_workspace(args.m_nameDB.c_str());
	if (!m_ipNameWork) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDBConnect, args.m_nameDB.c_str());
		return false;
	}
	// 道路名称DB用のオーナ名
	CString nameOwner = args.m_nameOwner.c_str();
	// 道路リンクレイヤ取得
	CString roadLinkTableName = sindy::schema::road_link::kTableName;
	if (!roadOwner.IsEmpty())
		roadLinkTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::road_link::kTableName);
	if (S_OK != m_ipRoadWork->OpenFeatureClass(CComBSTR(roadLinkTableName), &m_ipRoadLink)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, roadLinkTableName);
		return false;
	}
	// 路線名称テーブル取得
	CString roadCodeListTableName = sindy::schema::road_code_list::kTableName;
	if (!roadOwner.IsEmpty())
		roadCodeListTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::road_code_list::kTableName);
	if (S_OK != m_ipRoadWork->OpenTable(CComBSTR(roadCodeListTableName), &m_ipRoadCodeList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, roadCodeListTableName);
		return false;
	}
	// 県道名称テーブル取得
	CString prefRoadListTableName = sindy::schema::pref_road_list::kTableName;
	if (!roadOwner.IsEmpty())
		prefRoadListTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::pref_road_list::kTableName);
	if (S_OK != m_ipRoadWork->OpenTable(CComBSTR(prefRoadListTableName), &m_ipPrefRoadList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, prefRoadListTableName);
		return false;
	}
	// 区間名称テーブル取得
	CString sectionCodeListTableName = sindy::schema::section_code_list::kTableName;
	if (!roadOwner.IsEmpty())
		sectionCodeListTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::section_code_list::kTableName);
	if (S_OK != m_ipRoadWork->OpenTable(CComBSTR(sectionCodeListTableName), &m_ipSectionCodeList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, sectionCodeListTableName);
		return false;
	}
	// 道路名称テーブル取得
	CString roadNameListTableName = sindy::schema::sj::road_name_list::kTableName;
	if (!nameOwner.IsEmpty())
		roadNameListTableName.Format(_T("%s.%s"), nameOwner, sindy::schema::sj::road_name_list::kTableName);
	if (S_OK != m_ipNameWork->OpenTable(CComBSTR(roadNameListTableName), &m_ipRoadNameList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, roadNameListTableName);
		return false;
	}
	return true;
}

bool ConvRoadName::getDictionary(const CString& dicPath)
{
	// コンテナ初期化
	m_dictionary.clear();
	// ファイルオープン
	std::ifstream dicFile(dicPath);
	if (!dicFile.is_open()) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::FailOpenInputFile, dicPath);
		return false;
	}
	// 辞書取得
	std::string temp;
	int lineNum = 0;		// 行数(エラー出力用)
	while (std::getline(dicFile, temp)) {
		// タブで区切る
		CString record = temp.c_str();
		CStringTokenizer cTokenizer(record, _T("\t"), TOKEN_RET_EMPTY_ALL);
		int fieldCount = cTokenizer.CountTokens();
		if (1 > fieldCount) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDictionaryFieldNum, lineNum);
			return false;
		}
		// 分割元文字列取得
		CString orgStr = cTokenizer.GetNextToken();
		// 分割先文字列取得
		std::vector<CString> splitStrs;
		while (cTokenizer.HasMoreTokens()) {
			CString partStr = cTokenizer.GetNextToken();
			if (partStr.IsEmpty() || (partStr == _T(""))) {
				Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDictionaryFieldVal, lineNum);
				return false;
			}
			partStr.Trim();
			splitStrs.push_back(partStr);
		}
		// 分割元と分割先をつなげると一致するかチェック
		CString connectStr;
		for (const auto partStr : splitStrs) {
			connectStr.AppendFormat(partStr);
		}
		if (0 != connectStr.Compare(orgStr)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDictionaryRecord, lineNum);
			return false;
		}
		if (!m_dictionary.insert(std::make_pair(orgStr, splitStrs)).second) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::DuplicateDictionary, lineNum);
			return false;
		}
	}
	return true;
}

bool ConvRoadName::cnvEachName(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// 分割エラー存在判定用変数
	bool splitResult = true;
	// 標識道路番号
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 標識道路番号"));
	ShieldRoadNoCnv shieldRoadNoCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!shieldRoadNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= shieldRoadNoCnv.GetSplitResult();

	// 標識高速番号
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 標識高速番号"));
	ShieldHwyNoCnv shieldHwyNoCnv(m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!shieldHwyNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= shieldHwyNoCnv.GetSplitResult();

	// 国道番号
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 国道番号"));
	NationalRoadNoCnv naRoadNoCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!naRoadNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= naRoadNoCnv.GetSplitResult();

	// 県道番号・名称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 県道番号・名称"));
	PrefRoadNoNameCnv prefRoadNoNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList, m_ipPrefRoadList);
	if (!prefRoadNoNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNoNameCnv.GetSplitResult();

	// 県道番号
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 県道番号"));
	PrefRoadNoCnv prefRoadNoCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!prefRoadNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNoCnv.GetSplitResult();
	
	// 県道番号略称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 県道番号略称"));
	PrefRoadNoAbbrCnv prefRoadNoAbbrCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!prefRoadNoAbbrCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNoAbbrCnv.GetSplitResult();

	// 県道名称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 県道名称"));
	PrefRoadNameCnv prefRoadNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList, m_ipPrefRoadList);
	if (!prefRoadNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNameCnv.GetSplitResult();

	// 路線名称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 路線名称"));
	RoadNameCnv roadNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadNameCnv.GetSplitResult();

	// 高速道路略称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 高速道路略称"));
	RoadHwyAbbrCnv roadHwyAbbrCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadHwyAbbrCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadHwyAbbrCnv.GetSplitResult();

	// 都市高速略称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 都市高速略称"));
	RoadCityHwyAbbrCnv roadCityHwyAbbrCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadCityHwyAbbrCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadCityHwyAbbrCnv.GetSplitResult();
	
	// 都市高速略称2
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 都市高速略称2"));
	RoadCityHwyAbbr2Cnv roadCityHwyAbbr2Cnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadCityHwyAbbr2Cnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadCityHwyAbbr2Cnv.GetSplitResult();

	// 区間名称
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t■ 区間名称"));
	SectionNameCnv sectionNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList, m_ipSectionCodeList);
	if (!sectionNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= sectionNameCnv.GetSplitResult();

	return splitResult;
}