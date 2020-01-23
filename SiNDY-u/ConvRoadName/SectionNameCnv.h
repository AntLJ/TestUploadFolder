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
 * @brief 区間名称変換クラス
 */
class SectionNameCnv :
	public SplitBasicTokenRuleCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param 辞書データ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 * @param ipRoadCodeList [in] 道路路線テーブル
	 * @param ipPrefRoadList [in] 区間名称テーブル
	 */
	SectionNameCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipSectionCodeList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, ipSectionCodeList)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kSectionName;
		// 道路リンク検索条設定
		m_roadLinkWhere.Format(_T("%s <> 0"), sindy::schema::road_link::kSectionCode);
	}
	~SectionNameCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow) override;
	bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv) override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_roadLinkSectionCodeIndex;			//!< 道路リンクSECTiON_CODEフィールドインデックス
	long m_sectionCodeListSectionCodeIndex;		//!< 区間名称SECTION_CODEフィールドインデックス
	long m_sectionCodeListNameKanjiIndex;		//!< 区間名称NAME_KANJIフィールドインデックス
	long m_sectionCodeListNameYomiIndex;		//!< 区間名称NAME_YOMIフィールドインデックス

	std::map<long, _IRowPtr> m_tgtSecCodeListRecs;	//!< 変換対象区間名称リスト　キー：SECTION_CODE
};

