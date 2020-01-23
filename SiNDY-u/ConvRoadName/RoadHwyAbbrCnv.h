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
/**
 * @brief 都市高速略称変換クラス
 */
class RoadHwyAbbrCnv :
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
	RoadHwyAbbrCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, nullptr)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kRoadHwyAbbr;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("(200000 <= %s AND %s < 300000) OR (400000 <= %s AND %s < 500000) OR (600000 <= %s AND %s < 700000)"),
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode,
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode, 
			sindy::schema::road_code_list::kRoadCode, sindy::schema::road_code_list::kRoadCode);
	}
	~RoadHwyAbbrCnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_roadCodeListDisplayKanjiIndex;		//!< 路線コードリストDISPLAY_KANJIフィールドインデックス
	long m_roadCodeListDisplayYomiIndex;		//!< 路線コードリストDISPLAY_YOMIフィールドインデックス
};

