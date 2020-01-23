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
#include "BaseCnv.h"
/**
 * @brief 県道番号変換クラス
 */
class PrefRoadNoCnv :
	public BaseCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 */
	PrefRoadNoCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList)
		:BaseCnv(ipRoadLink, ipRoadNameList, nullptr)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kPrefRoadNo;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("%s IN (%ld,%ld,%ld,%ld) AND %s <> 0 AND %s <> 0"), sindy::schema::road_link::kRoadClass,
			sindy::schema::road_link::road_class::kPrefectureMain, sindy::schema::road_link::road_class::kPrefectureNormal,
			sindy::schema::road_link::road_class::kTollPrefectureMain, sindy::schema::road_link::road_class::kTollPrefectureNormal,
			sindy::schema::road_link::kPrefCode, sindy::schema::road_link::kRoadNo);

	}
	~PrefRoadNoCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;
	bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) override;

private:
	long m_roadLinkPrefCodeIndex;		//!< 道路リンクPREF_CODEフィールドインデックス
	long m_roadLinkRoadNoIndex;			//!< 道路リンクROAD_NOフィールドインデックス

};

