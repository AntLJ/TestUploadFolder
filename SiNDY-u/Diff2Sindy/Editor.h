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
#include <workspace.h>

/// バージョン対応状態識別コード
enum ObjectType : int
{
	ObjectTypeUnknown, //!< 不明
	ObjectTypeVersioned, //!< バージョン対応のみ
	ObjectTypeNonVersioned, //!< バージョン非対応のみ
	ObjectTypeMix, //!< バージョン対応と非対応が混在
};

/**
 * @brief 編集セッション管理クラス
 *
 * バージョン対応と非対応が混在しているかのうかのチェック機能や、
 * コンフリクト時にリコンサイル＆ポストをする機能も保有します。
 */
class CEditor
{
public:
	CEditor() : m_undoredo(false){}
	CEditor( const sindy::CWorkspace& work ) : m_work(work) ,m_undoredo(false), m_versioned(false){}

	/**
	* @brief 指定されたテーブルのリストのバージョン対応状況を返す
	* retval ObjectTypeVersioned 全てバージョン対応されている
	* retval ObjectTypeNonVersioned 全てバージョン対応されてない
	* retval ObjectTypeMix バージョン対応と非対応が混在している
	*/
	ObjectType CheckObjectType( const std::list<uh::tstring>& listTableName ) const;

	/**
	* @brief 編集を開始する
	* @param undoredo [in] UNDO/REDOを有効にする場合はtrue
	* @param versioned [in,optional] trueならバージョン対応として編集を開始する（デフォルト：true）
	* @retval true 編集開始成功
	* @retval false 編集開始失敗
	*/
	bool StartEditing(bool undoredo, bool versioned = true);

	/**
	* @brief 編集を終了する
	* @note 編集終了時、コンフリクトしている場合には自動的にリコンサイル＆ポスト処理を実行します。
	* @param save [in] trueなら編集を保存して終了する。falseなら破棄して終了する。
	* @retval true 編集終了成功
	* @retval false 編集終了失敗
	*/
	bool StopEditing(bool save);

	/**
	* @brief 編集を保存する
	*
	* 内部的には、StopEditing -> StartEditingしているだけです。
	* @retval true 編集保存成功
	* @retval false 編集保存失敗
	*/
	bool SaveEditing();

	/**
	* @brief 編集セッションを開始する
	* @note StartEditingされている必要があります
	* @retval true 編集セッション開始成功
	* @retval false 編集セッション開始失敗
	*/
	bool StartOperation();

	/**
	* @brief 編集セッションを終了する
	* @retval true 編集セッション終了成功
	* @retval false 編集セッション終了失敗
	*/
	bool StopOperation();

	/**
	* @brief 編集セッションを破棄する
	* @retval true 編集セッション破棄成功
	* @retval false 編集セッション破棄失敗
	*/
	bool AbortOperation();

	/**
	* @brief テーブルのバージョン対応状況に応じて編集を開始する
	* @note バージョン対応と非対応が混在する場合は編集を開始しない
	* @param listTableName [in] 処理対象のテーブルのリスト
	* @retval true 編集開始成功
	* @retval false 編集開始失敗
	*/
	bool StartEditingByObjectType( const std::list<uh::tstring>& listTableName );

	const sindy::CWorkspace& Workspace() const { return m_work; }
	sindy::CWorkspace& Workspace() { return m_work; }
private:
	sindy::CWorkspace m_work; //!< 編集開始終了対象ワークスペース
	bool m_undoredo; //!< UNDO/REDOを有効にするかどうかの管理フラグ
	bool m_versioned; //!< バージョン対応として編集開始するか、バージョン非対応として編集開始するかの管理フラグ
};

