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

#include "MiscFunc.h"
#include "TableDataGrid.h"
#include "WinLib/grid_util.h"
#include "Util.h"
#include "resource.h"

bool TableDataGrid::onGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	long rowNum = 0L;
	m_grid->get_MouseRow(&rowNum);
	
	if (rowNum <= 0) return true;

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

	return true;
}

void TableDataGrid::onBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	if (Cancel == NULL)	return;

	const CString& fieldName = m_gridField.getFieldName(Col);
	if (m_editableFields.count(fieldName) == 0) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	*Cancel = VARIANT_FALSE;
}

bool TableDataGrid::onAfterEdit(LONG Row, LONG Col)
{
	using namespace sindy::schema::adam_v2;

	if (!m_table) return false;

	_IRowPtr ipRow = getRow(Row);
	const CString& fieldName = m_gridField.getFieldName(Col);

	CString value = m_grid->GetTextMatrix( Row, Col );

	// 編集時、gridから取ってきたインプットの値に、スペースが入る可能性があるので、
	// そのスペースを削除するための処理
	value.TrimLeft();
	value.TrimRight();

	CComVariant newVal(value);
	if(value.IsEmpty() && fieldName == nq_compart_node::kWidth)
		newVal.vt = VT_NULL;

	if (!checkField(ipRow, newVal, Row, Col, fieldName) ||
		FAILED(ipRow->put_Value( AheFindField( AheGetFields( ipRow ), fieldName ), newVal ) ) ||
		!sortSequence(Row, Col, fieldName, _ttol(value)))
		return false;
	
	highlightChangedCell();

	return true;
}

void TableDataGrid::init()
{
	using namespace sindy::schema::adam_v2;

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

	long lRow = 0L;
	m_grid->get_Rows( &lRow );
	m_grid->Select( _variant_t(1L), _variant_t(1L), _variant_t(long(lRow-1)), _variant_t(1L));			
	m_grid->put_Sort( flexSortGenericAscending );
}

bool TableDataGrid::selectDelete(const CString & where_clause, const CString& objectID)
{
	if (!m_table) return false;

	if( m_data.end() != m_data.find( _ttol(objectID) ) ) 
	{
		m_data.erase(m_data.find(_ttol(objectID)));
		m_deleteList.push_back( where_clause );
	}

	if( m_addRecord.end() != m_addRecord.find( _ttol(objectID) ) ) 
	{
		m_addRecord.erase(m_addRecord.find(_ttol(objectID)));
	}

	if(m_deleteList.size() == 0 && m_addRecord.size() == 0 && !highlightChangedCell())
		return false;

	return true;

}

// 新規追加
void TableDataGrid::insert()
{
	using namespace uh;
	using namespace sindy::schema::adam_v2;

	if (!m_table) return;

	IFieldsPtr fields;
	m_table->get_Fields(&fields);

	// 出力先のバッファ
	IRowBufferPtr ipBuffer;
	m_table->CreateRowBuffer( &ipBuffer );
	
	// バッファに属性セット(初期値)
	if(FAILED(ipBuffer->put_Value( AheFindField(fields, nq_compart_node::kLastNodeF), CComVariant(0L)))
	|| FAILED(ipBuffer->put_Value( AheFindField(fields, nq_compart_node::kSourceUpdateC), CComVariant(-1L)))
	|| FAILED(ipBuffer->put_Value( AheFindField(fields, nq_compart_node::kUpdateType), CComVariant(1L)))
	|| FAILED(ipBuffer->put_Value( AheFindField(fields, nq_compart_node::kUserClaim), CComVariant(0L)))
	|| FAILED(ipBuffer->put_Value( AheFindField(fields, nq_compart_node::kCompartLinkGID), CComVariant(CComBSTR(m_linkGID)) ))) return;

	m_addRecord[--m_objectID] = ipBuffer;

	CString objectID;
	objectID.Format(_T("%d"),m_objectID);

	std::vector<CString> temp;
	for (const auto& field_col : m_gridField)
	{
		// ADAM の double 型フィールドの桁数に合わせて表示
		CString value = str_util::ToString(AheGetAttribute(ipBuffer, field_col.m_field_name), _T("%12.5f"));

		if( field_col.m_field_name == nq_compart_node::kObjectID)
			temp.push_back(_T(" "));
		else
			temp.push_back(value);
	}

	CString text = str_util::join(temp, _T("\t"));
	m_grid->AddItem(_bstr_t(text), 1);
	m_grid->put_RowData( 1, CComVariant(m_objectID));

	highlightChangedCell();

	return;
}

bool TableDataGrid::isChanged() const
{
	if (!m_table) return false;
	
	if(m_deleteList.size() > 0 || m_addRecord.size() > 0) return true;	

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
	if (!m_table) return false;

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

	for ( const auto& deleteRecord : m_deleteList)
	if (FAILED(m_table->DeleteSearchedRows(AheInitQueryFilter(nullptr, nullptr, deleteRecord)))) return false;

	
	if(m_addRecord.size() <= 0) return true;

	// 出力用カーソル
	_ICursorPtr ipCursor;
	if ( FAILED(m_table->Insert( VARIANT_TRUE, &ipCursor ))) return false;
	for ( const auto& addRecord : m_addRecord)
	{
		AheSetModifyData(addRecord.second, sindyUpdateTypeCreate);

		// カーソルにバッファをセット
		CComVariant vIDVariant;		
		if ( FAILED(ipCursor->InsertRow(addRecord.second, &vIDVariant))
			|| FAILED(ipCursor->Flush())) return false;

	}

	return true;
}

bool TableDataGrid::highlightChangedCell() const
{
	bool flag(false);
	if (!m_table) return flag;

	IFieldsPtr fields;
	m_table->get_Fields(&fields);

	long  rowNums(1L);	
	m_grid->get_Rows(&rowNums);
	if(rowNums <= 1) return flag;

	long gridRow = 1L;
	
	_IRowPtr row;

	for (long index = 1L; index < rowNums; ++index)
	{
		CComBSTR bstrValue;
		m_grid->get_TextMatrix(_variant_t(index), _variant_t(0L), &bstrValue);

		long currentObject = _ttol(bstrValue.m_str);		
		bool rowAddFlag(false);

		// 新規レコードの場合
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			m_grid->get_RowData( index, &vaValue );

			currentObject = vaValue.lVal;
		}

		if(m_addRecord.end() != m_addRecord.find(currentObject))
			rowAddFlag = true;
		else
			row = m_data.at(currentObject);

		for (const auto& field_col : m_gridField)
		{
			// 編集可能列以外はスキップ
			if (m_editableFields.count(field_col.m_field_name) == 0 && !rowAddFlag) continue;

			long idx = AheFindField(fields, field_col.m_field_name);			

			if(rowAddFlag || isValueChanged(row, idx)) {
				flag = true;
				grid_util::SetCellBackColor(m_grid, grid_util::color::sindyModified, gridRow, field_col.m_col);
				m_grid->put_Cell(flexcpForeColor, variant_t(gridRow), variant_t(field_col.m_col),
					variant_t(gridRow), variant_t(field_col.m_col), _variant_t((long)grid_color::notSelectedString));
			}
		}	

		++gridRow;
	}

	return flag;
}

bool TableDataGrid::readData(const ITablePtr & table, const CString& where_clause, std::map<long, _IRowPtr>& row_map)
{
	if (!table)	return false;

	_ICursorPtr cur;
	if (FAILED(table->Search(AheInitQueryFilter(nullptr, nullptr, where_clause), VARIANT_FALSE, &cur)))	return false;

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
	if (!table)	return;

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
		DataTypeSettings gridDataType = grid_util::GetGridDataType(fType);
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
	CString objectIDField = m_grid->GetTextMatrix(grid_row, oidCol);

	if (m_data.count(oid) != 0)	
	{
		return m_data.at(oid);	
	}
	// 新規レコードの場合
	else if( objectIDField == _T(" "))
	{
		CComVariant vaValue;
		m_grid->get_RowData( grid_row, &vaValue );		

		if (m_addRecord.count(vaValue.lVal) != 0)	
			return m_addRecord.at(vaValue.lVal);		
	}

	return _IRowPtr();
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
			
			// 文字列に生成した場合、不要なスペースを削除する処理
			val.TrimLeft();
			val.TrimRight();
			temp.push_back(val);
		}
		CString text = str_util::join(temp, _T("\t"));
		grid->AddItem(_bstr_t(text));
	}

	grid_util::AutoRowColSize(m_grid);
}

void TableDataGrid::setLinkGID(const CString& linkGID)
{
	m_linkGID = linkGID;
}

// 順番フィールドの自動更新
bool TableDataGrid::sortSequence(long row, long col, const CString& fieldName, long value) const
{
	using namespace sindy::schema::adam_v2;
	if(fieldName != nq_compart_node::kSequence) return false;

	bool existFlag(false);
	long rows(0L);
	m_grid->get_Rows( &rows );
	
	for( long indexRow = 1L; indexRow < rows; ++indexRow )
	{
		if( indexRow  == row ) continue;

		_IRowPtr ipRow = getRow(indexRow);
		CComVariant orgVal = AheGetAttribute( ipRow, fieldName);
		if( orgVal.intVal == value )
		{
			existFlag = true;
			break;
		}
	}

	if( !existFlag || MessageBox( m_hwnd, _T("他レコードの「順番」を自動更新しますか？"),_T("確認"), MB_OKCANCEL) != IDOK) return true;

	for( long indexRowTmp = 1L; indexRowTmp < rows; ++indexRowTmp )
	{
		if( indexRowTmp  == row ) continue;

		_IRowPtr ipRow = getRow(indexRowTmp);
		CComVariant orgVal = AheGetAttribute( ipRow, fieldName);

		if( orgVal.intVal < value ) continue;

		++(orgVal.intVal);
		m_grid->PutTextMatrix( indexRowTmp, col, _bstr_t( uh::str_util::ToString(orgVal, _T("%12.5f"))));
		if (!AheSetAttribute(ipRow, fieldName, orgVal)) return false;
	}

	return true;
}

// 選択しているグリッド行に対応する区画線ノードGIDを返す
void TableDataGrid::getSelectedNodeGid(CString& compartNodeGid, CString& compartNodeGidNext, const CString& fieldSequence, const CString& fieldCompartNodeGID) const
{
	long selectedRow = m_grid->GetSelectedRow(0);
	if (selectedRow <= 0) return;

	compartNodeGid = grid_util::GetValue(m_grid, selectedRow, m_gridField.getCol(fieldCompartNodeGID)).bstrVal;
	if(compartNodeGid.IsEmpty()) return;

	long sequence = grid_util::GetValue(m_grid, selectedRow, m_gridField.getCol(fieldSequence)).lVal;

	long rows(0L);
	m_grid->get_Rows( &rows );
	
	for( long indexRow = 1L; indexRow < rows; ++indexRow )
	{
		long sequenceNext = grid_util::GetValue(m_grid, indexRow, m_gridField.getCol(fieldSequence)).lVal;
		if(sequence + 1 == sequenceNext)
		{
			compartNodeGidNext = grid_util::GetValue(m_grid, indexRow, m_gridField.getCol(fieldCompartNodeGID)).bstrVal;
			break;
		}
	}
}

// フィールドをチェック
bool TableDataGrid::checkField( const IRowBufferPtr& ipRow, const CComVariant& vaValue, long row, long col, const CString& fieldName) const
{
	using namespace uh;
	using namespace sindy::schema::adam_v2;

	// セルの値と関連するフィールド情報取得
	IFieldsPtr ipFields;
	esriFieldType type = (esriFieldType)-1;
	IFieldPtr ipField;

	// 属性コピー
	ipRow->get_Fields( &ipFields );	
	ipFields->get_Field( AheFindField( ipFields, fieldName ), &ipField );
	ipField->get_Type( &type );		

	if (type == esriFieldTypeString) return true;

	// 元の値
	IRowChangesPtr rowChanges(ipRow);
	CComVariant oldValue;
	rowChanges->get_OriginalValue(AheFindField( ipFields, fieldName ), &oldValue);
	CString value = str_util::ToString(oldValue, _T("%12.5f"));

	// 整数チェック
	if (type == esriFieldTypeSmallInteger || type == esriFieldTypeInteger) {
		if (!util::isInteger(vaValue) || (fieldName == nq_compart_node::kSequence && CString(vaValue.bstrVal).IsEmpty())) {
			MessageBox(m_hwnd, AheLoadString(IDS_ERROR_INVALID_INPUT), AheLoadString(IDS_ERROR), MB_OK | MB_ICONERROR);
			m_grid->put_TextMatrix(row, col, _bstr_t(value));
			return false;
		}
	}

	// 浮動小数点チェック
	if (type == esriFieldTypeSingle || type == esriFieldTypeDouble) {
		if (!util::isInteger(vaValue) && !util::isFloat(vaValue)) {
			MessageBox(m_hwnd, AheLoadString(IDS_ERROR_INVALID_INPUT), AheLoadString(IDS_ERROR), MB_OK | MB_ICONERROR);
			m_grid->put_TextMatrix(row, col, _bstr_t(value));
			return false;
		}
	}

	return true;
}
