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

#include <string>
#include <vector>


/**
 * @brief IWorkspaceEdit に子バージョン作成しての編集をサポートさせただけのラッパークラス
 */
class ChildVerEditWorkspace
{
public:
	/**
	 * @brief デフォルトコンストラクタ
	 */
	ChildVerEditWorkspace(void)
		: m_IsVersioned(false), m_ChildCreateMode(false), m_IsCreatedChild(false), m_IsEditFailed(false) {}

	/**
	 * @brief コンストラクタ
	 * @param[in] ipWorkspace IWorkspaceオブジェクト
	 */
	explicit ChildVerEditWorkspace(const IWorkspacePtr &ws)
		: m_Workspace( ws ), m_IsVersioned(false), m_ChildCreateMode(false), m_IsCreatedChild(false), m_IsEditFailed(false) {}

	/**
	 * @brief デストラクタ
	 */
	virtual ~ChildVerEditWorkspace(void){}

	/**
	 * @brief IWorkspace のためのオペレータ
	 */
	operator IWorkspacePtr() const
	{
		return m_Workspace;
	}

	/**
	 * @brief IWorkspace の有効かの確認向けのオペレータ
	 */
	operator bool() const
	{
		return m_Workspace;
	}

	/**
	 * @brief IWorkspace 代入用のオペレータ
	 */
	ChildVerEditWorkspace& operator = (const IWorkspacePtr &ipWorkspace)
	{
		m_Workspace = ipWorkspace;
		return (*this);
	}

	/**
	 * @brief ワークスペースのオープン
	 * @param[in] connection DB接続情報
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool open(const CString& connection);

	/**
	 * @brief テーブルのオープン
	 * @param[in] table_name  テーブル名
	 * @return オープンされたテーブル
	 */
	ITablePtr openTable(const CString& table_name);

	/**
	 * @param[in] 編集開始時に子バージョンを作成するモードに設定する
	 * @detail   　指定したテーブルが全てバージョン対応でなければ、エラー
	 * @param[in] ver_name     子バージョン名
	 * @param[in] edit_targets 編集対象のテーブル名リスト(バージョン対応であるかの判断に使用)
	 * @retval  true  設定に成功
	 * @retval  false 設定に失敗
	 */
	bool childCreateModeOn(const CString& ver_name, const std::vector<CString>& edit_targets);

	/**
	 * @brief 編集開始
	 * @retval true  編集開始成功
	 * @retval false 編集開始失敗
	 */
	bool startEditing();

	/**
	 * @brief 編集終了
	 * @param[in] セーブするか否か
	 * @retval true  編集開始成功
	 * @retval false 編集開始失敗
	 */
	bool stopEditing(bool bSave);

	/**
	 * @brief EditOperation 開始
	 * @retval true  EditOperation 開始成功
	 * @retval false EditOperation 開始失敗
	 */
	bool startEditOperation();

	/**
	 * @brief EditOperation 終了
	 * @retval true  EditOperation 終了成功
	 * @retval false EditOperation 終了失敗
	 */
	bool stopEditOperation();

	/**
	 * @brief EditOperation の Abort 処理
	 * @details バージョン対応にのみ使用可
	 * @retval true  EditOperation 終了成功
	 * @retval false EditOperation 終了失敗
	 */
	bool abortEditOperation();

	/**
	 * @brief 接続先がSDEであるか
	 * @retval true  SDE
	 * @retval false FGDB or PGDB
	 */
	bool isSDE();

private:
	/**
	 * @brief 子バージョンの作成し、管理するワークスペースの対象を子バージョンにスイッチする
	 * @retval true  子バージョン作成成功
	 * @retval false 子バージョン作成失敗
	 */
	bool createChildVer();

	/**
	 * @brief 子バージョンを削除する
	 * @note  子バージョンを作成していて、かつ編集に失敗している場合のみ、削除処理を行う
	 * @retval true  子バージョン削除成功
	 * @retval false 子バージョン削除失敗
	 */
	bool deleteChildVer();

private:
	IWorkspaceEditPtr m_Workspace;		//!< ワークスペース

	bool m_IsVersioned;			//!< 接続先のDBがバージョン対応か否か
	bool m_ChildCreateMode;		//!< 編集開始すると、作成した子バージョンに対して編集を開始する。

	CString m_ChildVerName;		//!< 子バージョン名
	bool m_IsCreatedChild;		//!< 子バージョンを作成したか

	bool m_IsEditFailed;		//!< 編集に失敗したか
};
