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
const CString delNameWord2[] = {_T("東京高速道路"), _T("高速"), _T("")};
const CString delYomiWord2[] = {_T("トウキョウコウソクドウロ"), _T("コウソク"), _T("")};

/**
 * @brief 都市高速略称2変換クラス
 */
class RoadCityHwyAbbr2Cnv :
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
	RoadCityHwyAbbr2Cnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, nullptr)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kRoadCityHwyAbbr2;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("(300000 <= %s AND %s < 400000) OR (500000 <= %s AND %s < 600000) OR (700000 <= %s AND %s < 800000)"),
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode);
	}
	~RoadCityHwyAbbr2Cnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;


private:
	CString MkAbbrStr2(const CString& str, const CString delWords[]);

private:
	long m_roadCodeListDisplayKanjiIndex;		//!< 路線コードリストDISPLAY_KANJIフィールドインデックス
	long m_roadCodeListDisplayYomiIndex;		//!< 路線コードリストDISPLAY_YOMIフィールドインデックス
};

