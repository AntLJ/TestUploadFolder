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
#include "common.h"

/**
 * @brief 名称データmap格納用キー
 */
class RoadNameKey {
public:
	RoadNameKey(const CString& name, const CString& yomi)
		:m_name(name), m_yomi(yomi) {}
	~RoadNameKey() {}
public:
	bool operator < (const RoadNameKey& roadNameKey) const
	{
		return (0 != this->m_name.Compare(roadNameKey.m_name)) ? this->m_name < roadNameKey.m_name :
			this->m_yomi < roadNameKey.m_yomi;
	}
	bool operator == (const RoadNameKey& roadNameKey) const
	{
		return (0 == this->m_name.Compare(roadNameKey.m_name)) &&
			(0 == this->m_yomi.Compare(roadNameKey.m_yomi));
	}
public:
	CString m_name;			//!< 名称
	CString m_yomi;			//!< 読み
};

// 分割種別コード
namespace split_class {
	enum ECode {
		kBody = 1,			//!< Body(通常の名称に割り当てられる属性)	
		kDirectional =2,	//!< Directional(日本のデータでは使用されない属性)
		kParticle = 3,		//!< Particle(日本のデータでは使用されない属性)
		kClassifier = 4,	//!< Classifier("自動車道"、"有料道路"、"道路"に割り当てられる属性)
		kConnector = 5,		//!< Connector("～"に割り当てられる属性)
		kHonorific = 6,		//!< Honorific(日本のデータでは使用されない属性)
	};
}

class BaseCnv
{
public:
	/**
	 * @brief コンストラクタ
	 * @param ipRoadLink [in] 道路リンクフィーチャクラス
	 * @param ipRoadNameList [in] 道路名称テーブル
	 * @param ipSubNameTable [in] 道路名称以外に必要な名称テーブル
	 */
	BaseCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipSubNameTable)
		:m_ipRoadLink(ipRoadLink), m_ipRoadNameList(ipRoadNameList), m_ipSubNameTable(ipSubNameTable), m_isErr(false) {}
	/*
	 * @brief デストラクタ
	 */
	~BaseCnv(){}

public:
	/**
	 * @brief 変換処理
	 * @note 各種out変数は処理時に初期化をしない。(複数クラスでの変換結果をまとめて格納した場合があるため)
	 * @param rdNameRecs	[out] 道路名称データレコード変換結果
	 * @param rlNameLnkRecs [out] 道路リンクに紐づく道路名称レコード変換結果
	 * @param spltRdNameRecs [out] 道路名称分割データレコード変換結果
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool cnv(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs);

	/**
 	 * @brief 正常に分割処理が完了してるどうかを返す関数
	 * @retval true: 正常終了
	 * @retval false: 分割に失敗したケースが存在する。
	 */
	bool GetSplitResult() { return !m_isErr; }

protected:
	/**
	 * @brief 初期化
	 * @retval true 成功
	 * @retval false 失敗
 	 */
	bool init();

	/** 
	 * @brief 道路名称リストテーブル必要インデックス取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool setRoadNameFieldIndex();

	/**
	 * @brief 道路リンクレイヤ必要インデックス取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool setRoadLinkFieldIndex() = 0;

	/**
	 * @brief サブテーブル必要インデックス取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool setSubTblFieldIndex();

	/**
	 * @brief 変換対象road_name_listの取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool loadRoadNameList();

	/**
	 * @brief 変換対象サブテーブルの取得
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool loadSubTable();

	/**
	 * @brief サブテーブルの関連Row取得
	 * @param linkOID [in] 変換対象リンクOBJECTID
	 * @param ipLink [in] 変換対象リンクフィーチャ
	 * @param subOID [out] サブテーブル関連RowのOBJECTID
	 * @param ipSubRow [out] サブテーブルの関連Row
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow);

	/**
	 * @brief 処理対象レコードかチェック
	 * @param linkOID [in] 変換対象リンクOBJECTID
	 * @param ipLink [in] 変換対象リンクフィーチャ
	 * @param subOID [in] 変換対象サブレコードOBJECTID
	 * @param ipSubRow [in] 変換対象サブレコード
	 * @param chkCnv [out] true:変換対象 false:変換対象外
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv);

	/**
	 * @brief リンクに紐づく道路名称データ取得
	 * @param linkOID [in] 変換対象リンクOBJECTID
	 * @param ipLink [in] 変換対象リンクフィーチャ
	 * @param subOID [in] 変換対象サブレコードOBJECTID
	 * @param ipSubRow [in] 変換対象サブレコード
	 * @param roadNameRecord [out] 変換対象名称データ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) = 0;

	/**
	 * @brief 道路リンクに紐づく道路名称データ変換
	 * @param linkOID [in] 変換対象リンクOBJECTID
	 * @param roadNameRecord [in] 道路名称データ
	 * @param rlNameLnkRecs [out] 道路リンクに紐づく道路名称レコード変換結果
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool cnvRelNameLink(const long& linkOID, const RoadNameRecord& roadNameRecord, std::set<RelNameLinkRecord>& rlNameLnkRecs);

	/**
	 * @brief 道路名称・道路名称分割データ変換済みチェック
	 * @param roadNameRecord [in] 道路名称データ
	 * @param rdNameRecs [in] 道路名称データ変換結果
	 * @return ture:変換済み false:未変換
	 */
	bool isConverted(const RoadNameRecord& roadNameRecord, const std::set<RoadNameRecord>& rdNameRecs);

	/**
	 * @brief 道路名称データ変換
	 * @param roadNameRecord [in] 道路名称データ
	 * @param rdNameRecs [out] 道路名称データ変換結果
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool cnvRoadName(const RoadNameRecord& roadNameRecord, std::set<RoadNameRecord>& rdNameRecs);

	/**
	 * @brief 道路名称分割データ変換
	 * @param linkOID [in] 変換対象リンクOBJECTID
	 * @param ipLink [in] 変換対象リンクフィーチャ
	 * @param subOID [in] 変換対象サブレコードOBJECTID
	 * @param ipSubRow [in] 変換対象サブレコード
	 * @param roadNameRecord [in] 道路名称データ
	 * @param spltRdNameRecs [out] 道路名称分割データレコード変換結果
	 */
	virtual bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) = 0;

protected:
	IFeatureClassPtr m_ipRoadLink;		//!< 道路リンクレイヤ
	ITablePtr m_ipRoadNameList;			//!< 道路名称リストテーブル
	ITablePtr m_ipSubNameTable;			//!< 道路リンクレイヤ・道路名称テーブル以外に参照する名称テーブル
	CString m_subNametableName;			//!< 道路リンクレイヤ・道路名称テーブル以外に参照する名称テーブルのテーブル名
	CString m_roadLinkWhere;			//!< 道路リンク検索条件

	long m_nameListClsIndex;			//!< 道路名称リストテーブル名称種別インデックス
	long m_nameListNameIndex;			//!< 道路名称リストテーブル名称インデックス
	long m_nameListYomiIndex;			//!< 道路名称リストテーブル読みインデックス

	long m_tgtNameClass;				//!< 出力名称種別
	
	std::map<RoadNameKey, RoadNameRecord> m_tgtRdNameRecs;		//!< 変換対象道路名称レコード

	bool m_isErr;									//!< 分割が正常に行われたか判断する変数(true:分割が正常に行われていないケースあり.false:全て正常に分割されている)
};

