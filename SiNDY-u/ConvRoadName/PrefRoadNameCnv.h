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
#include "PrefNameBaseCnv.h"

/**
 * @brief 県道名称変換クラス
 */
class PrefRoadNameCnv :
	public PrefNameBaseCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param 辞書データ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 * @param ipRoadCodeList [in] 道路路線テーブル
	 * @param ipPrefRoadList [in] 県道名称テーブル
	 */
	PrefRoadNameCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipPrefRoadList)
		:PrefNameBaseCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, ipPrefRoadList)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kPrefRoadName;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("%s IN (%ld,%ld,%ld,%ld) AND %s <> 0 AND %s <> 0"), sindy::schema::road_link::kRoadClass,
			sindy::schema::road_link::road_class::kPrefectureMain, sindy::schema::road_link::road_class::kPrefectureNormal,
			sindy::schema::road_link::road_class::kTollPrefectureMain, sindy::schema::road_link::road_class::kTollPrefectureNormal,
			sindy::schema::road_link::kPrefCode, sindy::schema::road_link::kRoadNo);;
	}
	~PrefRoadNameCnv() {}

protected:
	bool setSubTblFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_prefRdListNameKanjiIndex;		//!< 都道府県リストNAME_KANJIフィールドインデックス
	long m_prefRdListNameYomiIndex;			//!< 都道府県リストNAME_YOMIフィールドインデックス
};

