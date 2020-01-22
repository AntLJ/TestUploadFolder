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

namespace util
{
	/**
	 * @brief   ファイル名に日付を追記する
	 * @param   time  [in]  現在時間
	 * @param   orgFilePath [in] 元のファイル名
	 * @return  変換後のファイル名
	 */
	CString AddDateToFileName(const SYSTEMTIME& time, const CString& orgFilePath);

	/**
	 * @brief   フィーチャクラスを取得
	 * @param   name [in] フィーチャクラス名
	 * @param   work [in] ワークスペース
	 * @param   featureC [out] フィーチャクラス
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool OpenFeatureClass(const CString& name, IWorkspacePtr work, IFeatureClassPtr& featureC);

	/**
	 * @brief   テーブルを取得
	 * @param   name [in] テーブル名
	 * @param   work [in] ワークスペース
	 * @param   table [out] テーブル
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool OpenTable(const CString& name, IWorkspacePtr work, ITablePtr& table);

	extern int JAPANESE_LANGUAGE ;               ///< 日本語の言語コード(システム言語判定用)

}