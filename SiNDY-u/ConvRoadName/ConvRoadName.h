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
#include "Arguments.h"

class ConvRoadName
{
public:
	ConvRoadName(){}
	~ConvRoadName(){}

public:
	/*
	 * @brief 処理実行
	 * @param args [in] コマンドライン引数
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool run(const Arguments& args);

private:
	/*
	 * @brief 変換元テーブル取得
	 * @param args [in] コマンドライン引数
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool getTgtTbl(const Arguments& args);

	/*
	 * @brief 辞書取得
	 * @param dicFile [in] 辞書ファイルパス
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool getDictionary(const CString& dicFile);

	/*
	 * @breif 各種名称変換
	 * @param rdNameRecs	[out] 道路名称データレコード変換結果
	 * @param rlNameLnkRecs [out] 道路リンクに紐づく道路名称レコード変換結果
	 * @param spltRdNameRecs [out] 道路名称分割データレコード変換結果
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool cnvEachName(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs);

private:
	IFeatureWorkspacePtr m_ipRoadWork;	//!< 道路DB用ワークスペース
	IFeatureWorkspacePtr m_ipNameWork;	//!< 道路名称用ワークスペース
	IFeatureClassPtr m_ipRoadLink;		//!< 道路リンクフィーチャクラス
	ITablePtr m_ipRoadCodeList;			//!< 路線名称テーブル
	ITablePtr m_ipPrefRoadList;			//!< 県道名称テーブル
	ITablePtr m_ipSectionCodeList;		//!< 区間名称テーブル
	ITablePtr m_ipRoadNameList;			//!< 道路名称リストテーブル

	std::map<CString, std::vector<CString>> m_dictionary;	//!< 辞書
};

