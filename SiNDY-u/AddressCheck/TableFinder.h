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
 * @class	TableFinder
 * @brief	テーブル管理クラス
 * @note	このクラスを通じてテーブルを取得する
 *			一度オープンしたらそれをそのまま使用できる
 *			ワークスペース・フィールドインデックスも管理できる
 */
class TableFinder
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	TableFinder(void);

	 /**
	 * @brief  デストラクタ
	 */
	virtual ~TableFinder(void);

	/**
	 * @brief	ワークスペース追加
	 * @param	[in] lpcszKey                ワークスペースキー
	 * @param	[in] lpcszWorkspaceKey       ワークスペースキーワード
	 * @retval	true:成功  false:失敗
	 */
	bool addWorkspace( const CString& lpcszKey, const CString& lpcszWorkspaceKey );

	/**
	 * @brief	リセット
	 * @note	ワークスペースリスト・テーブルリスト・フィールドマップリスト全て削除
	 */
	void reset()
	{
		m_mapWorkspace.clear();
		m_mapTable.clear();
	}

	/**
	 * @brief	ワークスペース取得
	 * @note	オープンされていないときは、オープンする
	 * @param	[in] lpcszKey		ワークスペースキー
	 * @return	IWorkspacePtr
	 */
	IWorkspacePtr getWorkspace( const CString& lpcszKey );

	/**
	 * @brief	テーブル検索(ワークスペース指定版)
	 * @param	[in] lpcszWorkspaceKey       ワークスペースキー(db_map等)
	 * @param	[in] lpcszTable              テーブル名(キー)
	 * @return	ITable
	 */
	ITablePtr findTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable );

private:

	/**
	 * @brief	テーブルオープン
	 * @param	[in] lpcszWorkspaceKey       ワークスペースキー
	 * @param	[in] lpcszTable              テーブル名
	 * @return	ITable
	 */
	ITablePtr openTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable );	

private:

	typedef std::map< CString, CAdapt<IWorkspacePtr> >	WORKSPACEMAP;   //!< ワークスペースマップ
	typedef std::map< CString, CAdapt<ITablePtr> >		TABLEMAP;       //!< テーブルマップ

	WORKSPACEMAP                m_mapWorkspace;                     //!< ワークスペースリスト
	TABLEMAP                    m_mapTable;                         //!< テーブルリスト

};
