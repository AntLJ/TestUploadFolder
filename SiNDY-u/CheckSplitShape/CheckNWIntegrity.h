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

// NW整合性チェッククラス
class CCheckNWIntegrity {
public:
	CCheckNWIntegrity(check_mode::ECode CheckMode, bool bJGD2000, double dMinSegment)
	{
		m_CheckMode = CheckMode;
		m_bJGD2000 = bJGD2000;
		m_dMinSegment = dMinSegment;
	}
	~CCheckNWIntegrity(){}

public:
	/**
	 * @brief NW整合チェック
	 * @param cAfterRWLAll         [in] 変換後RWL
	 * @param cAfterRWNAll         [in] 変換後RWN
	 * @param cAfterRelHeightAll   [in] 変換後RelHeight
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool run(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, const CRelHeightAll& cAfterRelHeightAll);

private:
	/**
	 * @brief NW整合性チェック（RWL用）
	 * @note CheckNWIntegrity()から呼ばれる
	 * @brief cAfterRWLAll     [in] RWLデータ
	 * @brief cAfterRWNAll     [in] RWNデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckNWRWLIntegrity(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll);

	/**
	 * @brief NW整合性チェック（RWN用）
	 * @note CheckNWIntegrity()から呼ばれる
	 * @brief cAfterRWLAll     [in] RWLデータ
	 * @brief cAfterRWNAll     [in] RWNデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckNWRWNIntegrity(const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll);

	/**
	 * @brief NW整合性チェック（rel_height用）
	 * @note CheckNWIntegrity()から呼ばれる
	 * @brief cAfterHNodeAll   [in] 相対高さノードデータ
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool CheckNWRelHeightIntegrity(const CRWLAll& cAfterRWLAll, const CRelHeightAll& cAfterRelHeightAll);

	/**
	 * @brief RWN形状重複確認
	 * @note 基本的に形状重複していたらエラーだが、リンク内属性分割時は特殊処理
	 * @brief cLonLat       [in] RWNの緯度経度情報
	 * @brief cRWNAll       [in] 全RWLデータ
	 * @retval true  重複してる
	 * @retval false 重複していない
	 */
	bool IsRWNDuplicateGeom(const LONLAT& cLonLat, const CRWNAll& cRWNAll);

private:
	check_mode::ECode m_CheckMode;           //!< 分割処理タイプ
	bool m_bJGD2000;                         //!< JGD2000化するかどうか
	double m_dMinSegment;                    //!< 微小セグメント長
};
