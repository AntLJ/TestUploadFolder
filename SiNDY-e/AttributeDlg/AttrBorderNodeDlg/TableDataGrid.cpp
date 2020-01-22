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

#include "stdafx.h"

#include "ControlUtil.h"
#include "MiscFunc.h"
#include "ControlUtil.h"
#include "TableDataGrid.h"


bool TableDataGrid::onGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	long rowNum = 0;
	m_grid->get_MouseRow(&rowNum);

	// 左クリック
	if (Button == 1)
	{
		if (rowNum <= 0)
			return true;

		// ハイライトされていた行を元に戻す
		long selectedRow =  m_grid->GetSelectedRow(0);
		grid_util::SetCellBackColor(m_grid, grid_color::notSelectedBack, selectedRow, 0, selectedRow, m_grid->GetCols() - 1);
		m_grid->put_Cell(flexcpForeColor, variant_t(selectedRow), variant_t(0), variant_t(selectedRow), variant_t(m_grid->GetCols() - 1), _variant_t((long)grid_color::notSelectedString));

		// クリックされた行をハイライトする
		grid_util::SetCellBackColor(m_grid, grid_color::selectedBack, rowNum, 0, rowNum, m_grid->GetCols() - 1);
		m_grid->put_Cell(flexcpForeColor, variant_t(rowNum), variant_t(0), variant_t(rowNum), variant_t(m_grid->GetCols() - 1), _variant_t((long)grid_color::selectedString));

		// 選択行を変更
		m_grid->PutIsSelected(selectedRow, VARIANT_FALSE);
		m_grid->PutIsSelected(rowNum, VARIANT_TRUE);

		// 変更セルを色付け
		highlightChangedCell();
	}

	return true;
}

void TableDataGrid::onBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	if (Cancel == NULL)
		return;

	const CString& fieldName = m_gridField.getFieldName(Col);
	if (m_editableFields.count(fieldName) == 0) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	*Cancel = VARIANT_FALSE;
}

bool TableDataGrid::onAfterEdit(LONG Row, LONG Col)
{
	if (!m_table)
		return false;

	CComVariant newVal = grid_util::GetValue(m_grid, Row, Col);
	_IRowPtr row = getRow(Row);
	const CString& fieldName = m_gridField.getFieldName(Col);
	if (!AheSetAttribute(row, fieldName, newVal))
		return false;

	highlightChangedCell();

	return true;
}

void TableDataGrid::init()
{
	grid_util::FormatString(m_grid, getHeader());
	grid_util::AutoRowColSize(m_grid);
	m_grid->put_Editable(flexEDKbdMouse);
}

void TableDataGrid::select(const CString & where_clause)
{
	if (!readData(m_table, where_clause, m_data))
		return;

	// グリッドの列ごとの設定を行う
	initColSettings(m_table, m_grid, m_gridField);

	// 読み込んだデータをグリッドへセットする
	setDataToGrid(m_grid, m_gridField, m_data);
}

bool TableDataGrid::isChanged() const
{
	if (!m_table)
		return false;

	IFieldsPtr fields;
	m_table->get_Fields(&fields);

	for (const auto& record : m_data)
	{
		_IRowPtr row = record.second;
		for (const auto& field_col : m_gridField)
		{
			long idx = AheFindField(fields, field_col.m_field_name);
			if (isValueChanged(row, idx))
				return true;
		}
	}

	return false;
}

bool TableDataGrid::store()
{
	if (!m_table)
		return false;

	IFieldsPtr fields;
	m_table->get_Fields(&fields);

	for ( const auto& record : m_data)
	{
		_IRowPtr row = record.second;
		bool isChange = false;
		for (const auto& field_col : m_gridField)
		{
			long idx = AheFindField(fields, field_col.m_field_name);
			if (isValueChanged(row, idx))
				isChange = true;
		}
		if (isChange) {
			AheSetModifyData(row, sindyUpdateTypeProperty);
			if (FAILED(row->Store()))
				return false;
		}
	}

	return true;
}

_IRowPtr TableDataGrid::getSelectedRow() const
{
	using namespace sindy::schema::object;

	long selectedRow = m_grid->GetSelectedRow(0);
	if (selectedRow <= 0)
		return nullptr;

	long oid = grid_util::GetValue(m_grid, selectedRow, m_gridField.getCol(kObjectID)).lVal;
	_ASSERTE(m_data.count(oid) != 0);

	return m_data.at(oid);
}

void TableDataGrid::highlightChangedCell() const
{
	if (!m_table)
		return;

	IFieldsPtr fields;
	m_table->get_Fields(&fields);

	long gridRow = 1;
	for (const auto& record : m_data)
	{
		_IRowPtr row = record.second;
		for (const auto& field_col : m_gridField)
		{
			// 編集可能列以外はスキップ
			if (m_editableFields.count(field_col.m_field_name) == 0)
				continue;

			long idx = AheFindField(fields, field_col.m_field_name);

			if (isValueChanged(row, idx)) {
				grid_util::SetCellBackColor(m_grid, grid_util::color::sindyModified, gridRow, field_col.m_col);
				m_grid->put_Cell(flexcpForeColor, variant_t(gridRow), variant_t(field_col.m_col),
					variant_t(gridRow), variant_t(field_col.m_col), _variant_t((long)grid_color::notSelectedString));
			}
		}
		++gridRow;
	}
}

bool TableDataGrid::readData(const ITablePtr & table, const CString& where_clause, std::map<long, _IRowPtr>& row_map)
{
	if (!table)
		return false;

	_ICursorPtr cur;
	if (FAILED(table->Search(AheInitQueryFilter(nullptr, nullptr, where_clause), VARIANT_FALSE, &cur)))
		return false;

	_IRowPtr row;
	while (cur->NextRow(&row) == S_OK && row) {
		long oid(0);
		row->get_OID(&oid);
		row_map[oid] = row;
	}

	return true;
}

void TableDataGrid::initColSettings(const ITablePtr& table, const IVSFlexGridPtr& grid, const GridField& grid_field)
{
	if (!table)
		return;

	IFieldsPtr fields = AheGetFields(m_table);

	for (const auto& field_col : grid_field)
	{
		// コード値ドメインがあればコンボボックスに設定
		std::map<long, CString> cvDomain;
		if (miscfunc::GetCodedValueDomainMap(m_table, field_col.m_field_name, cvDomain)) {
			grid_util::SetCombo(grid, field_col.m_col, cvDomain);
			continue;
		}

		// フィールドタイプに対応したグリッドのデータタイプを設定
		esriFieldType fType;
		miscfunc::GetFieldType(fields, field_col.m_field_name, fType);
		DataTypeSettings gridDataType = control_util::GetGridDataType(fType);
		if (gridDataType != flexDTEmpty) {
			grid->PutColDataType(field_col.m_col, gridDataType);
		}
	}
}

CString TableDataGrid::getHeader() const
{
	std::vector<CString> temp;
	for (const auto& field_col : m_gridField)
	{
		IFieldPtr field;
		if (!AheGetFieldByFieldName(m_table, field_col.m_field_name, &field))
			temp.push_back(CString());

		CComBSTR aliasName;
		field->get_AliasName(&aliasName);
		temp.push_back(CString(aliasName));
	}

	return uh::str_util::join(temp, _T("|"));
}

_IRowPtr TableDataGrid::getRow(long grid_row) const
{
	using namespace sindy::schema;

	// 編集した行に対応するテーブルのレコードを取得
	long oidCol = m_gridField.getCol(kObjectID);
	long oid = grid_util::GetValue(m_grid, grid_row, oidCol).lVal;
	if (m_data.count(oid) == 0)
		return _IRowPtr();

	return m_data.at(oid);
}

bool TableDataGrid::isValueChanged(const _IRowPtr row, long field_index) const
{
	IRowChangesPtr rowChanges(row);

	CComVariant oldVal;
	rowChanges->get_OriginalValue(field_index, &oldVal);
	CComVariant newVal = AheGetAttribute(row, field_index);

	return oldVal != newVal;
}

void TableDataGrid::setDataToGrid(const IVSFlexGridPtr& grid, const GridField& grid_field, const std::map<long, _IRowPtr>& row_map)
{
	using namespace uh;

	for (auto& element : row_map)
	{
		std::vector<CString> temp;
		for (const auto& field_col : grid_field)
		{
			// ADAM の double 型フィールドの桁数に合わせて表示
			CString val = str_util::ToString(AheGetAttribute(element.second, field_col.m_field_name), _T("%12.5f"));
			temp.push_back(val);
		}
		CString text = str_util::join(temp, _T("\t"));
		grid->AddItem(_bstr_t(text));
	}

	grid_util::AutoRowColSize(m_grid);
}
