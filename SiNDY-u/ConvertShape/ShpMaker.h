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

#include "global.h"

class CShpMaker
{
public:
	CShpMaker(void);
	~CShpMaker(void);

	/**
	 * @brief シェープ作成
	 * @param bIsNew [in] 新規作成かどうか
	 * @param listField [in] フィールド情報
	 * @param listShpInfo [in] データリスト
	 * @param lpszOutputFile [out] シェープファイルのパス
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool MakeShape(bool bIsNew, const std::list<PAIR_FIELD>& listField, const std::list<SHAPE_INFO>& listShpInfo, LPCTSTR lpszOutputFile);

	/**
	 * @brief シェープデータ追加
	 * @param listShpInfo [in] データリスト
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool AddData(const std::list<SHAPE_INFO>& listShpInfo);

	void Clear()
	{
		m_ipTable = NULL;
		m_mapFieldIndex.clear();
	};

private:
	/**
	 * @brief フィールド作成
	 * @param listField [in] フィールド情報
	 * @param ipGeom [in] 形状
	 * @return フィールド
	 */
	IFieldsPtr CreateFields(const std::list<PAIR_FIELD>& listField, const IGeometryPtr& ipGeom);

	/**
	 * @brief シェープ削除
	 * @param strFile [in] 削除対象ファイル
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool DeleteShape(const CString& strFile);

	/**
	 * @brief シェープ作成
	 * @param bIsNew [in] 新規作成かどうか
	 * @param ipWorkspace [in] シェープのワークスペース
	 * @param strShpDir [in] シェープファイル出力先
	 * @param strFeatureName [in] テーブル名（シェープファイル名）
	 * @param listField [in] フィールド情報
	 * @param ipGeom [in] 形状フィールド設定用の形状データ
	 * @param ipOutTable [out] シェープファイルのテーブル
	 * @param mapOutFieldIndex [out] シェープファイルのフィールドインデックス
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool CreateNewShape(bool bIsNew, IWorkspacePtr ipWorkspace, const CString& strShpDir, const CString& strFeatureName, const std::list<PAIR_FIELD>& listField, const IGeometryPtr& ipGeom, ITablePtr& ipOutTable, std::map<CString, long>& mapOutFieldIndex);

private:
	ITablePtr m_ipTable;
	std::map<CString, long> m_mapFieldIndex;
};

