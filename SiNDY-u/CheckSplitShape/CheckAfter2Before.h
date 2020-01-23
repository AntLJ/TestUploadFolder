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

#include "RWL.h"
#include "RWN.h"
#include "RSA.h"
#include "HeightNode.h"
#include "RelHeight.h"

// 変換後⇒変換前チェック用クラス
class CCheckAfter2Before {
public:
	CCheckAfter2Before(check_mode::ECode CheckMode)
	{
		m_CheckMode = CheckMode;
	}
	~CCheckAfter2Before(){}

public:
	/**
	 * @brief 変換後⇒変換前チェック
	 * @param cBeforeRWLAll         [in] 変換前RWL
	 * @param cBeforeRWNAll         [in] 変換前RWN
	 * @param cBeforeRSAAll         [in] 変換前RSA
	 * @param cBeforeHNodeAll       [in] 変換前HeightNode
	 * @param cAfterRWLAll          [in] 変換後RWL
	 * @param cAfterRWNAll          [in] 変換後RWN
	 * @param cAfterRelHeightAll    [in] 変換後RelHeight
	 * @param mapRebuildHN          [in] 再構築HeightNode
	 * @param setReAfterRH          [in] 再構築HeightNodeに使用されるLINKIDリスト
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool run(const CRWLAll& cBeforeRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll,
			const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, const std::set<UNIQID>& setReAfterRH);

private:
	/**
	 * @brief RWL分割後⇒分割前チェック
	 * @brief cBeforeRWLAll    [in] 変換前RWLデータ
	 * @brief cAfterRWLAll     [in] 変換後RWLデータ
	 * @brief cBeforeRWNAll    [in] 変換前RWNデータ
	 * @brief cBeforeRSAAll    [in] 変換前RSAデータ
	 * @brief cBeforeHNodeAll  [in] 変換前HeightNodeデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckRWLAfter2Before(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll);

	/**
	 * @brief RWN分割後⇒分割前チェック
	 * @brief cBeforeRWNAll    [in] 変換前RWNデータ
	 * @brief cAfterRWNAll     [in] 変換後RWNデータ
	 * @brief cBeforeRSAAll    [in] 変換前RSAデータ
	 * @brief cBeforeHNodeAll  [in] 変換前HeightNodeデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckRWNAfter2Before(const CRWNAll& cBeforeRWNAll, const CRWNAll& cAfterRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll);

	/**
	 * @brief HN分割後⇒分割前チェック
	 * @brief cBeforeHNodeAll     [in] 変換前HNodeデータ
	 * @brief cAfterRelHeightAll  [in] 変換後RelHeightデータ
	 * @brief mapRebuildHN        [in] 変換後RelHeightからHNodeを再構築したもの
	 * @brief setReAfterRH        [in] 変換後RelHeightからHNodeを再構築した時に使用しているLINKID
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckHNAfter2Before(const CHeightNodeAll& cBeforeHNodeAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, const std::set<UNIQID>& setReAfterRH);

	/**
	 * @brief 形状一致確認
	 * @note チェック対象はグループ単位
	 * @brief strTableName     [in] 変換後データのテーブル名（エラーメッセージ用）
	 * @brief lOID             [in] 変換後データのOID（エラーメッセージ用）
	 * @brief lDivID           [in] 変換後データのDivID（エラーメッセージ用）
	 * @brief vecBeforeLonLat  [in] 変換前データのグループ形状
	 * @brief vecAfterLonLat   [in] 変換後データのグループ形状
	 * @brief cBeforeRSAAll    [in] 変換前RSAデータ
	 * @brief cBeforeHNodeAll  [in] 変換前高さノードデータ
	 * @retval true  マッチしている
	 * @retval false マッチしていない
	 */
	bool IsMatchGroupGeom(const CString& strTableName, long lOID, long lDivID, const std::vector<LONLAT>& vecBeforeLonLat, const std::vector<LONLAT>& vecAfterLonLat, const CRWNAll& cBeforeRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll);

	/**
	 * @brief リンク内属性と属性値がマッチしているかチェック
	 * @brief cAfterRWLDiv       [in] 変換後RWLデータ
	 * @brief lAfterRWLRoadAttr  [in] 変換後RWLのRoadAttr属性値
	 * @brief cBeforeRSAAll      [in] 変換前全RSAデータ
	 * @brief lBeforeRWLRoadAttr [in] 変換前RWLのRoadAttr属性値
	 * @retval true  マッチしている
	 * @retval false マッチしていない
	 */
	bool IsMatchSegmentAttr(const CRWLDiv& cAfterRWLDiv, long lAfterRWLRoadAttr, const CRSAAll& cBeforeRSAAll, long& lBeforeRWLRoadAttr);

	/**
	 * @brief マージ対象リンクと属性値がマッチしているかチェック
	 * @brief cAfterRWLDiv       [in] 変換後RWLデータ
	 * @brief cBeforeRWNAll      [in] 変換前全RWNデータ
	 * @retval true  マッチしている
	 * @retval false マッチしていない
	 */
	bool IsMatchMergeLink(const CRWLDiv& cAfterRWLDiv, const CRWLAll& cBeforeRWLAll);

	/**
	 * @brief 変換後⇒変換前チェックでのデータ存在チェック
	 * @brief cAfterRWNAll       [in] 変換後全RWNデータ
	 * @brief cBeforeRSAAll      [in] 変換前全RSAデータ
	 * @brief cBeforeHNodeAll    [in] 変換前全高さノードデータ
	 * @brief cAfterRWLDiv       [in] 変換後RWLデータ
	 * @retval true  チェックエラーあり
	 * @retval false チェックエラーなし
	 */
	bool CheckRWNExist(const CRWNAll& cAfterRWNAll, const CRSAAll& cBeforeRSAAll, const CHeightNodeAll& cBeforeHNodeAll, const CRWNDiv& cAfterRWNDiv);

	/**
	 * @brief 変換後⇒変換前チェックでのグループ内属性チェック用関数
	 * @brief cAfterRWLDivBase      [in] 基準となる変換後RWLのグループ内データ
	 * @brief cAfterRWLDiv          [in] 変換後RWLデータ
	 * @brief strAfterRWLTableName  [in] 変換後RWLテーブル名
	 * @brief strFieldName          [in] チェック対象のフィールド名
	 * @brief lIndex                [in] チェック対象のフィールドインデックス
	 * @retval true  チェックエラーあり
	 * @retval false チェックエラーなし
	 */
	bool CheckRWLGroupAttr(const CRWLDiv& cAfterRWLDivBase, const CRWLDiv& cAfterRWLDiv, const CString& strAfterRWLTableName, const CString& strFieldName, const long lIndex);

private:
	check_mode::ECode m_CheckMode;           //!< 分割処理タイプ
};
