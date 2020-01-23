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

namespace common_proc
{
	/**
	 * @brief ワークスペースの取得
	 * @param strConnect [in] ワークスペース名（DB名）
	 * @param ipWorkspace [out] ワークスペース
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getWorkspace( const CString& strConnect, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief テーブルの取得
	 * @param strName [in] テーブル名
	 * @param ipWorkspace [in] ワークスペース
	 * @param ipTable [out] テーブル
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getTable( const CString& strName, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable );

	/**
	 * @brief フィールドの取得
	 * @param ipTable [in] 対象テーブル
	 * @param strFieldName [in] 対象フィールド名
	 * @param ipField [out] フィールド
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getField( const ITablePtr& ipTable, const CString& strFieldName, IFieldPtr& ipField );

	/**
	 * @breif コード値ドメインで定義されているコード値のセットを取得
	 * @param ipField [in] 対象フィールド
	 * @param setValues [out] コード値ドメインで定義されているコード値のセット
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getCodedValues( const IFieldPtr& ipField, std::set< long, std::less<long> >& setValues );

	/**
	 * @breif コード値ドメインで定義されているコード値のセットを取得
	 * @param ipTable [in] 対象テーブル
	 * @param strFieldName [in] 対象フィールド名
	 * @param setValues [out] コード値ドメインで定義されているコード値のセット
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getCodedValues( const ITablePtr& ipTable, const CString& strFieldName, std::set< long, std::less<long> >& setValues );
}
