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
#include "RelNodeGridController.h"
#include "WinLib/grid_util.h"


static const _TCHAR* GRID_HEADER = _T("関連ノードID");

enum GridCols
{
	kOID = 0,
	kSize
};


void RelNodeGridController::init( HWND hwnd)
{
	m_GridWnd = hwnd;
	CAxWindow axWnd( hwnd );
	axWnd.QueryControl(&m_RelNodeGrid);

	// ヘッダ作成
	grid_util::FormatString(m_RelNodeGrid, GRID_HEADER);
	// 列幅調整
	grid_util::AutoRowColSize(m_RelNodeGrid);

	m_RelNodeGrid->put_ExplorerBar(flexExSortShowAndMove);
	m_RelNodeGrid->put_ExtendLastCol(VARIANT_TRUE);
	m_RelNodeGrid->put_SelectionMode(flexSelectionByRow);
}

void RelNodeGridController::setData(const std::vector<long>& oids)
{
	m_RelNodeGrid->put_Rows(oids.size() + 1);

	int row = 1;
	for( long oid : oids )
	{
		// VS GridCtrlへ登録
		CString strOid = uh::str_util::ToString(oid);
		m_RelNodeGrid->put_TextMatrix(row, kOID, strOid.GetBuffer());
		++row;
	}
}

std::vector<long> RelNodeGridController::getData() const
{
	std::vector<long> oids;

	for( int i = 1; i < m_RelNodeGrid->Rows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		CComVariant vaId = grid_util::GetValue(m_RelNodeGrid, i, GridCols::kOID);
		vaId.ChangeType(VT_I4);

		oids.push_back(vaId.lVal);
	}

	return oids;
}

void RelNodeGridController::addRow()
{
	long rows(-1);
	m_RelNodeGrid->get_Rows(&rows);
	m_RelNodeGrid->put_Rows(rows + 1);
}

void RelNodeGridController::deleteRow(const std::vector<long>& oids)
{
	int i = 1;
	while( i <= m_RelNodeGrid->Rows )
	{
		// 指定した削除対象でなければ何もしない
		CComVariant vaId = grid_util::GetValue(m_RelNodeGrid, i, GridCols::kOID);
		vaId.ChangeType(VT_I4);
		if( std::find(oids.begin(), oids.end(), vaId.lVal) == oids.end() ){
			++i;
			continue;
		}

		m_RelNodeGrid->RemoveItem(i);
		++i;
	}
}

void RelNodeGridController::deleteSelectedRow()
{
	int i = m_RelNodeGrid->FixedRows;
	while( i <= m_RelNodeGrid->Rows )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_RelNodeGrid->get_IsSelected(i, &vb );
		if( vb == VARIANT_FALSE ){
			++i;
			continue;
		}

		m_RelNodeGrid->RemoveItem(i);
	}
}

void RelNodeGridController::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId)
{
	long lRow(-1);
	m_RelNodeGrid->get_MouseRow(&lRow);

	if( lRow < 0 )
		return;

	// 行選択
	for( int i = m_RelNodeGrid->FixedRows; i <= m_RelNodeGrid->Rows - m_RelNodeGrid->FixedRows; ++i ) {
		m_RelNodeGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
	}

	// 右クリックならポップアップメニュー表示
	if( 2 == Button )
	{
		POINT p = {};
		GetCursorPos(&p);
		CMenu menu;
		menu.LoadMenu( menuId );
		menu.GetSubMenu(0).TrackPopupMenu( TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, p.x, p.y, hwnd);
	}
}

std::vector<long> RelNodeGridController::getSelectedData() const
{
	std::vector<long> oids;
	for( int i = 1; i < m_RelNodeGrid->Rows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_RelNodeGrid->get_IsSelected(i, &vb );
		if( vb == VARIANT_FALSE )
			continue;

		CComVariant vaId = grid_util::GetValue(m_RelNodeGrid, i, GridCols::kOID);
		vaId.ChangeType(VT_I4);
		oids.push_back(vaId.lVal);
	}
	
	return oids;
}
