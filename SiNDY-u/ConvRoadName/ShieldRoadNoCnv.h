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
#include "FullNameEqSpltNameCnv.h"
/**
 * @brief 標識高速番号変換クラス
 */
class ShieldRoadNoCnv : public FullNameEqSpltNameCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 */
	ShieldRoadNoCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList)
		: FullNameEqSpltNameCnv(ipRoadLink, ipRoadNameList, nullptr), m_roadLinkRoadNoIndex(-1)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kShieldRoadNo;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("%s<>0"), sindy::schema::road_link::kRoadNo);
	}
	~ShieldRoadNoCnv() {};

protected:
	bool setRoadLinkFieldIndex() override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;

private:
	long m_roadLinkRoadNoIndex;			//!< 道路リンクRoadNoフィールドインデックス
};

