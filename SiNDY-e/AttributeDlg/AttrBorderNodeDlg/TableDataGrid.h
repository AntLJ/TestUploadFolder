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

#include <memory>
#include <WinLib/grid_util.h>
#include "GridField.h"

// グリッドの色
namespace grid_color {
	const COLORREF selectedString = RGB(255, 255, 255);		// 選択行の文字色
	const COLORREF selectedBack = RGB(51, 153, 255);		// 選択行の背景色
	const COLORREF notSelectedString = RGB(0, 0, 0);		// 非選択行の文字色
	const COLORREF notSelectedBack = RGB(255, 255, 255);	// 非選択行の背景色
}

/**
 *	@brief   テーブル情報を表示するグリッドクラス
 */
class TableDataGrid
{
public:
	class Builder;

public:
	TableDataGrid() {}
	TableDataGrid(ITablePtr table, GridField grid_field)
		: m_table(table), m_gridField(grid_field) {};
	virtual ~TableDataGrid() {};

public:
	// イベントハンドラの処理を委譲するための関数
	bool onGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);
	void onBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel);
	bool onAfterEdit(LONG Row, LONG Col);

public:
	/**
	*  @brief グリッドの列の初期化
	*/
	void init();

	/**
	*  @brief グリッドで編集可能なフィールドを設定する
	*  @param[in] field_names  編集可能とするフィールド名リスト
	*/
	void enableEditing(const std::set<CString>& field_names) { m_editableFields = field_names; }

	/**
	*  @brief データを読み込んでグリッドに表示する
	*  @param[in] where_clause 検索条件句
	*/
	void select(const CString& where_clause);

	/**
	*  @brief グリッドの保持するデータに変更が入っているか
	*/
	bool isChanged() const;

	/**
	*  @brief データの更新
	*/
	bool store();

	/**
	*  @brief 選択しているグリッド行に対応するレコードを返す
	*  @return レコード
	*/
	_IRowPtr getSelectedRow() const;

	/**
	*  @brief 変更のあったセルをハイライトする
	*/
	void highlightChangedCell() const;

private:
	/**
	 *  @brief テーブルからデータを読み込みコンテナに格納する
	 *  @param[in]  table         読み込むテーブル
	 *  @param[in]  where_clause  データ読み込み時のWhere句
	 *  @param[out] data          読み込んだデータの格納先
	 *  @retval  true  読み込み成功
	 *  @retval  false 読み込み失敗
	 */
	bool readData(const ITablePtr & table, const CString& where_clause, std::map<long, _IRowPtr>& row_map);

	/**
	 *  @brief コード値ドメインを持つフィールドのグリッド列をコンボボックスにする
	 *  @param[in]  table      テーブル
	 *  @param[in]  grid       コンボボックスにする対象のグリッド
	 *  @param[in]  grid_field コンボボックスにするグリッドのフィールド情報
	 */
	void initColSettings(const ITablePtr& table, const IVSFlexGridPtr& grid, const GridField& grid_field);

	/**
	 *  @brief 読み込んだ関連テーブルの情報をグリッドへ格納する
	 *  @param[in]  grid       格納対象のグリッド
	 *  @param[in]  grid_field 値を格納するグリッドのフィールド情報
	 *  @param[in]  row_map    読み込んだ関連テーブルの情報 (key: OID, value: 関連テーブルのレコード)
	 */
	void setDataToGrid(const IVSFlexGridPtr& grid, const GridField& grid_field, const std::map<long, _IRowPtr>& row_map);
	
	/**
	*  @brief  各フィールドのヘッダーを"|"区切りで返す
	*  @detail (e.g. OBJECTID|道路表示種別|ハイウェイ路線コード)
	*  @return ヘッダーの"|"区切り
	*/
	CString getHeader() const;

	/**
	*  @brief  グリッドの指定行に対応するテーブルのレコードを取得する
	*  @param[in] grid_row  グリッドの行番号
	*  @return  テーブルの1レコード(IRow)
	*/
	_IRowPtr getRow(long grid_row) const;

	/**
	*  @brief  指定フィールドの値が変更されているかを返す
	*  @param[in] row         変更されているかの確認対象のレコード
	*  @param[in] field_name  変更されているかを確認するフィールドのインデックス
	*  @retval true  変更されている
	*  @retval false 変更されていない
	*/
	bool isValueChanged(const _IRowPtr row, long field_index) const;

protected:
	IVSFlexGridPtr   m_grid;

	ITablePtr m_table;			//!< 関連テーブル

	// グリッドで保持しているデータ群 (key: OID, value: レコード)
	std::map<long, _IRowPtr> m_data;	

	GridField m_gridField;					//!< フィールド名とグリッド列の対応情報
	std::set<CString> m_editableFields;		//!< グリッドで編集可能なフィールド名リスト
};


/**
 *	@brief   TableDataGrid オブジェクトの作成を行うクラス
 */
class TableDataGrid::Builder
{
public:
	Builder() {}

public:
	Builder& setGridHwnd(HWND hwnd) { m_hwnd = hwnd; return *this; }
	Builder& setTable(ITablePtr table) { m_table = table; return *this; }
	Builder& addField(const CString& fieldName) {
		m_gridField.add(fieldName, m_gridField.size());
		return *this;
	}

	virtual TableDataGrid build() const
	{
		TableDataGrid grid(m_table, m_gridField);
		CAxWindow wndGrid = m_hwnd;
		wndGrid.QueryControl(&(grid.m_grid));

		return grid;
	}

protected:
	HWND m_hwnd;				//!< グリッドコントロールのハンドル
	ITablePtr m_table;			//!< テーブル
	GridField m_gridField;		//!< グリッドのフィールド情報
};
