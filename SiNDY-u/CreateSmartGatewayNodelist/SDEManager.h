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

class CSDEManager
{
public:
	/*
	 * @brief コンストラクタ
	 */
	CSDEManager(void);
	/*
	 * @brief デストラクタ
	 */
	~CSDEManager(void);
	/**
	 * @brief SDE接続
	 * @param lpszUserName	[in] ユーザ
	 * @param lpszPassword	[in] パスワード
	 * @param lpszInstance	[in] インスタンス
	 * @param lpszServer	[in] サーバ
	 * @param lpszVersion	[in] バージョン
	 * @param ppWorkspace	[out] ワークスペースポインタ
	 * @return bool
	 */
	bool CSDEManager::ConnectSDE(const LPCTSTR lpszUserName,
									const LPCTSTR lpszPassword,
									const LPCTSTR lpszInstance,
									const LPCTSTR lpszServer,
									const LPCTSTR lpszVersion,
									IWorkspacePtr& ppWorkspace);
	/**
	 * @brief 指定レイヤのオープン
	 * @param lpcszLayerName	[in]	レイヤ名
	 * @param ipWorkspace		[in]	ワークスペースポインタ
	 * @param ppFeatureClass	[out]	フィーチャクラスポインタ
	 * @return bool
	 */
	bool OpenTargetLayer(const LPCTSTR lpcszLayerName, IWorkspacePtr ipWorkspace, IFeatureClass **ppFeatureClass);
	/**
	 * @brief 指定テーブルのオープン
	 * @param lpcszTableName	[in]	テーブル名
	 * @param ipWorkspace		[in]	ワークスペースポインタ
	 * @param ppTable			[out]	テーブルポインタ
	 * @return bool
	 */
	bool OpenTargetTable(const LPCTSTR lpcszTableName, IWorkspacePtr ipWorkspace, ITable **ppTable);
	/**
	 * @brief カーソルの取得(フィーチャなし)
	 * @param ipFeatureClass		[in]  フィーチャクラスポインタ
	 * @param lpszWhereClause		[in]  Where句
	 * @param ppCursor				[out] カーソルポインタ
	 * @return bool
	 */	
	bool GetCursor(ITablePtr ipTable, const LPCTSTR lpszWhereClause, _ICursor **ppCursor);
	/**
	 * @brief フィーチャーカーソルの取得
	 * @param ipFeatureClass		[in]  フィーチャクラスポインタ
	 * @param lpszWhereClause		[in]  Where句
	 * @param ppFeatureCursor	[out] フィーチャカーソルポインタ
	 * @return bool
	 */
	bool GetFeatureCursor(IFeatureClassPtr ipFeatureClass, const LPCTSTR lpszWhereClause, IFeatureCursor **ppFeatureCursor);
	/**
	 * @brief フィールド値の取得
	 * @note 与えられたRowの指定されたフィールドの値を取得
	 * @param ipRow [in] 取得するオブジェクトの行
	 * @param lpcszFieldName [in] フィールド名
	 * @return 指定されたフィールドのCComVariant値
	 */
	CComVariant	 GetValue(_IRowPtr ipRow, LPCTSTR lpcszFieldName );
};
