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
class ShieldHwyNoCnv : public FullNameEqSpltNameCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 * @param ipRoadCodeList [in] 道路路線テーブル
	 */
	ShieldHwyNoCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList)
		: FullNameEqSpltNameCnv(ipRoadLink, ipRoadNameList, ipRoadCodeList),
		 m_roadLinkRoadCodeIndex(-1), m_roadCodeListRoadCodeIndex(-1), m_roadCodeListShieldNoStrIndex(-1)
	{
		// 出力名称種別設定
		m_tgtNameClass = sindy::schema::sj::road_name_list::name_class::kShieldHwyNo;
		// 道路リンク検索条件設定
		m_roadLinkWhere.Format(_T("%s<>0"), sindy::schema::road_link::kRoadCode);
	}
	~ShieldHwyNoCnv() {}

protected:
	bool setRoadLinkFieldIndex() override;
	bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow) override;
	bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv) override;
	bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) override;


private:
	long m_roadLinkRoadCodeIndex;		//!< 道路リンクRoadCodeフィールドインデックス
	long m_roadCodeListRoadCodeIndex;	//!< 路線コードリストRoadCodeフィールドインデックス
	long m_roadCodeListShieldNoStrIndex;	//!< 路線コードリストSHIELD_NO_STRINGフィールドインデックス

	std::map<long, _IRowPtr> m_tgtRdCodeListRecs;	//!< 変換対象道路路線リストレコード キー：路線コード
	
};

