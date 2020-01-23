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
 * @brief PREF_CODE_LISTmap格納用キー
 */
class PrefCodeKey {
public:
	PrefCodeKey(const long& roadNo, const long& roadCls, const long& prefCode)
		:m_roadNo(roadNo), m_roadCls(roadCls), m_prefCode(prefCode) {}
	~PrefCodeKey() {}
public:
	bool operator < (const PrefCodeKey& prefCodeKey) const
	{
		return this->m_prefCode != prefCodeKey.m_prefCode ? this->m_prefCode < prefCodeKey.m_prefCode :
			this->m_roadNo != prefCodeKey.m_roadNo ? this->m_roadNo < prefCodeKey.m_roadNo :
			this->m_roadCls < prefCodeKey.m_roadCls;
	}
	bool operator == (const PrefCodeKey& prefCodeKey) const
	{
		return this->m_prefCode == prefCodeKey.m_prefCode &&
			this->m_roadNo == prefCodeKey.m_roadNo &&
			this->m_roadCls == prefCodeKey.m_roadCls;
	}
public:
	long m_prefCode;		//!< 県コード
	long m_roadNo;			//!< 道路No
	long m_roadCls;			//!< 道路表示種別
};

/**
 * @brief 県道名称変換ベースクラス
 */
class PrefNameBaseCnv :
	public SplitBasicTokenRuleCnv
{
public:
	/*
	 * @brief コンストラクタ
	 * @param 辞書データ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 * @param ipRoadCodeList [in] 道路路線テーブル
	 * @param ipPrefRoadList [in] 県道名称テーブル
	 */
	PrefNameBaseCnv(const std::map<CString, std::vector<CString>>& dictionary, const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipRoadCodeList, const ITablePtr& ipPrefRoadList)
		:SplitBasicTokenRuleCnv(dictionary, ipRoadLink, ipRoadNameList, ipRoadCodeList, ipPrefRoadList) {}
	~PrefNameBaseCnv() {}

protected:
	virtual bool setRoadLinkFieldIndex() override;
	virtual bool setSubTblFieldIndex() override;
	bool loadSubTable() override;
	bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow) override;
	bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv) override;

protected:
	long m_roadLinkPrefCodeIndex;		//!< 道路リンクPREF_CODEフィールドインデックス
	long m_roadLinkRoadNoIndex;			//!< 道路リンクROAD_NOフィールドインデックス
	long m_roadLinkRoadClsIndex;		//!< 道路リンクROAD_CLASS_Cフィールドインデックス
	long m_prefRdListPrefCodeIndex;	//!< 都道府県リストPREF_CODEフィールドインデックス
	long m_prefRdListRoadNoIndex;		//!< 都道府県リストROAD_NOフィールドインデックス
	long m_prefRdListRoadClsIndex;	//!< 都道府県リストRoad_CLASS_Cフィールドインデックス

	std::map<PrefCodeKey, _IRowPtr> m_tgtPrefRdListRecs;	//!< 変換対象都道府県リスト

};

