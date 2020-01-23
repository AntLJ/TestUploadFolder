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

/**
 * @file	DBUtils.h
 */

// フィールドIndex
typedef std::map<CString, long> FIELD_INDEX;

/**
 * @class	CDBUtils
 * @brief	DBユーティリティクラス
 */
class CDBUtils
{
public:
	CDBUtils(void);
	~CDBUtils(void);

	/**
	* @brief	DB初期化
	* @param	dbConnection	[in]	DB接続情報
	* @param	ipWorkspace		[out]	ワークスペース
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool initDB(const CString& dbConnection, IWorkspacePtr& ipWorkspace);

	/**
	* @brief	フィーチャクラスオープン
	* @param	ipWorkspace			[in]	ワークスペース
	* @param	featureClassName	[in]	フィーチャクラス名
	* @param	ipFeatureClass		[out]	フィーチャクラス
	* @param	fieldIndex			[out]	フィールドINDEX
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool openFeatureClass(const IWorkspacePtr& ipWorkspace, const CString& featureClassName, IFeatureClassPtr& ipFeatureClass, FIELD_INDEX& fieldIndex);

	/**
	* @brief	テーブルオープン
	* @param	ipWorkspace		[in]	ワークスペース
	* @param	tabelName		[in]	テーブル名
	* @param	ipTable			[out]	フィーチャクラス/テーブル
	* @param	fieldIndex		[out]	フィールドINDEX
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool openTable(const IWorkspacePtr& ipWorkspace, const CString& tabelName, ITablePtr& ipTable, FIELD_INDEX& fieldIndex);

	/**
	* @brief	テーブル件数取得
	* @param	ipTable			[in]	フィーチャクラス/テーブル
	* @param	strWhere		[in]	検索SQL
	* @param	lCount			[out]	テーブル件数
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool getRowCount(const ITablePtr& ipTable, const CString& strWhere, long& lCount);

	/**
	* @brief	DBカーソル取得
	* @param	ipTable			[in]	フィーチャクラス/テーブル
	* @param	strWhere		[in]	検索SQL
	* @param	strSort			[in]	ソートフィールド
	* @param	ipCursor		[out]	カーソル
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool getCursor(const ITablePtr& ipTable, const CString& strWhere, const CString& strSort, _ICursorPtr& ipCursor);

	/**
	* @brief	フィールドINDEX取得
	* @param	fieldIndex		[in]	フィールドINDEX
	* @param	strFieldName	[in]	フィールド名
	* @param	index			[out]	INDEX
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool getFieldIndex(const FIELD_INDEX& fieldIndex, const CString& strFieldName, long& index);

	/**
	* @brief	値取得
	* @param	ipRow			[in]	レコード/フィーチャ
	* @param	strFieldName	[in]	フィールド名
	* @param	index			[in]	INDEX
	* @param	strVal			[out]	値
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool getValue(const _IRowPtr& ipRow, const CString& strFieldName, const long& index, CString& strVal);

private:
	/**
	* @brief	フィールドINDEX作成
	* @param	ipTable			[in]	フィーチャクラス/テーブル
	* @param	fieldIndex		[out]	フィールドINDEX
	* @retval	true	成功
	* @retval	false	失敗
	*/
	static bool createFieldIndex(const ITablePtr& ipTable, FIELD_INDEX& fieldIndex);
};


