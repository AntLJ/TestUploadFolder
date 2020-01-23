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

// 変換前⇒変換後チェック用クラス
class CCheckBefore2After {
public:
	CCheckBefore2After(check_mode::ECode CheckMode)
	{
		m_CheckMode = CheckMode;
	}
	~CCheckBefore2After(){}

public:
	/**
	 * @brief 変換前⇒変換後チェック
	 * @param cBeforeRWLAll         [in] 変換前RWL
	 * @param cBeforeRWNAll         [in] 変換前RWN
	 * @param cBeforeHNodeAll       [in] 変換前HeightNode
	 * @param cAfterRWLAll          [in] 変換後RWL
	 * @param cAfterRWNAll          [in] 変換後RWN
	 * @param cAfterRelHeightAll    [in] 変換後RelHeight
	 * @param mapRebuildHN          [in] 再構築HeightNode
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool run(const CRWLAll& cBeforeRWLAll, const CRWNAll& cBeforeRWNAll, const CHeightNodeAll& cBeforeHNodeAll,
			const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>> mapRebuildHN);

private:
	/**
	 * @brief RWL分割前⇒分割後チェック
	 * @brief cBeforeRWLAll    [in] 変換前RWLデータ
	 * @brief cAfterRWLAll     [in] 変換後RWLデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckRWLBefore2After(const CRWLAll& cBeforeRWLAll, const CRWLAll& cAfterRWLAll);

	/**
	 * @brief RWN分割前⇒分割後チェック
	 * @brief cBeforeRWNAll    [in] 変換前RWNデータ
	 * @brief cAfterRWNAll     [in] 変換後RWNデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckRWNBefore2After(const CRWNAll& cBeforeRWNAll, const CRWNAll& cAfterRWNAll);

	/**
	 * @brief HN分割前⇒HN分割後チェック
	 * @brief cBeforeHNodeAll     [in] 変換前HNodeデータ
	 * @brief cAfterRelHeightAll  [in] 変換後RelHeightデータ
	 * @brief mapRebuildHN        [in] 変換後RelHeightからHNodeを再構築したもの
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckHNBefore2After(const CHeightNodeAll& cBeforeHNodeAll, const CRelHeightAll& cAfterRelHeightAll, const std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN);

	/**
	 * @brief マージ対象RWL取得
	 * @brief cBeforeRWLDiv        [in] 変換前RWLデータ
	 * @brief cAfterRWLAll         [in] 変換後全RWLデータ
	 * @brief mapAfterMergeRWLDiv  [out] マージ対象RWLデータ
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool GetAfterMergeRWL(const CRWLDiv& cBeforeRWLDiv, const CRWLAll& cAfterRWLAll, std::map<UNIQID,CRWLDiv>& mapAfterMergeRWLDiv);

private:
	check_mode::ECode m_CheckMode;           //!< 分割処理タイプ
};
