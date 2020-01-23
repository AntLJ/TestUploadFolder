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
#include "CheckNWIntegrity.h"
#include "CheckAfter2Before.h"
#include "CheckBefore2After.h"

class CCheckSplitMesh {
public:
	CCheckSplitMesh(){}
	~CCheckSplitMesh(){}

public:
	/**
	 * @brief 初期化用関数
	 * @param strMeshCode              [in] 処理対象メッシュコード
	 * @param CheckMode                [in] 分割処理タイプ
	 * @param ipBeforeHNodeClass       [in] 分割前高さノードクラス
	 * @param m_ipAfterRelHeightTable  [in] 分割後相対高さテーブル
	 * @param ipMeshClass              [in] メッシュクラス
	 * @param bJGD2000                 [in] JGD2000化するかどうか（高さノード用）
	 * @param dMinSegment              [in] 微小セグメント長
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool Init(const CString& strMeshCode,
				const CString& strBeforeShpPath,
				const CString& strAfterShpPath,
				check_mode::ECode CheckMode,
				IFeatureClassPtr ipBeforeHNodeClass,
				ITablePtr m_ipAfterRelHeightTable,
				IFeatureClassPtr ipMeshClass,
				bool bJGD2000,
				double dMinSegment);

	/**
	 * @brief 各テーブルデータ読み込み用関数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool LoadData();

	/**
	 * @brief 各テーブルデータのチェック処理用関数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool CheckData();

private:
	/**
	 * @brief H分割前⇒N分割後チェック
	 * @brief cBeforeHNodeAll     [in] 変換前HNodeデータ
	 * @brief cAfterRWLAll        [in] 変換後RWLデータ
	 * @brief cAfterRWNAll        [in] 変換後RWNデータ
	 * @brief mapRebuildHN        [out] 変換後RelHeightからHNodeを再構築したもの
	 * @brief setRebuildLinkId    [out] 変換後RelHeightからHNodeを再構築した時に使用しているLINKID
	 * @retval true  処理中にチェックエラーなし
	 * @retval false 処理中にチェックエラーあり
	 */
	bool RebuldHeightNodeFromRelReight(const CHeightNodeAll& cBeforeHNodeAll, const CRWLAll& cAfterRWLAll, const CRWNAll& cAfterRWNAll, std::map<LONLAT,std::vector<UNIQID>>& mapRebuildHN, std::set<UNIQID>& setRebuildLinkId);

private:
	bool m_bJGD2000;                         //!< JGD2000化するかどうか
	CString m_strMeshCode;                   //!< 処理対象メッシュコード
	check_mode::ECode m_CheckMode;           //!< 分割処理タイプ
	double m_dMinSegment;                    //!< 微小セグメント長

	IWorkspacePtr m_ipBeforeShpWork;         //!< 分割前シェープファイルのワークスペース
	IWorkspacePtr m_ipAfterShpWork;          //!< 分割後シェープファイルのワークスペース

	IFeatureClassPtr m_ipBeforeHNodeClass;   //!< 分割前高さノードクラス
	ITablePtr m_ipAfterRelHeightTable;       //!< 分割後相対高さテーブル
	IFeatureClassPtr m_ipMeshClass;          //!< メッシュクラス

	CRWLAll m_cBeforeRWLAll;                 //!< 変換前RWL全データ
	CRWLAll m_cAfterRWLAll;                  //!< 変換後RWL全データ
	CRWNAll m_cBeforeRWNAll;                 //!< 変換前RWN全データ
	CRWNAll m_cAfterRWNAll;                  //!< 変換後RWN全データ
	CRSAAll m_cBeforeRSAAll;                 //!< 変換前RSA全データ
	CHeightNodeAll m_cBeforeHNodeAll;        //!< 変換前相対高さ全データ
	CRelHeightAll m_cAfterRelHeightAll;      //!< 変換前相対高さ全データ
};
