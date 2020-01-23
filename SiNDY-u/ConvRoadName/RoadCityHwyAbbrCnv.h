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

#pragma once
#include "SplitBasicTokenRuleCnv.h"

// 削除対象文字配列
const CString delNameWord[] = {_T("首都"), _T("阪神"), _T("名古屋"), _T("広島"), _T("北九州"), _T("福岡"), _T("東京"), _T("")};
const CString delYomiWord[] = {_T("シュト"), _T("ハンシン"), _T("ナゴヤ"), _T("ヒロシマ"), _T("キタキュウシュウ"), _T("フクオカ"), _T("トウキョウ"), _T("")};

/**
* @brief 都市高速略称変換クラス
*/
class RoadCityHwyAbbrCnv :
	public SplitBasicTokenRuleCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param 辞書データ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 * @param ipRoadCodeList [in] 道路路線テーブル
	 */
	RoadCityHwyAbbrCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, nullptr)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kRoadCityHwyAbbr;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("(300000 <= %s AND %s < 400000) OR (500000 <= %s AND %s < 600000) OR (700000 <= %s AND %s < 800000)"),
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode);
	}
	~RoadCityHwyAbbrCnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	CString MkAbbrStr(const CString& str, const CString delWords[]);


private:
	long m_roadCodeListDisplayKanjiIndex;		//!< 路線コードリストDISPLAY_KANJIフィールドインデックス
	long m_roadCodeListDisplayYomiIndex;		//!< 路線コードリストDISPLAY_YOMIフィールドインデックス

};

