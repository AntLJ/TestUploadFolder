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
#include "RelLinkGridController.h"
#include "WinLib/grid_util.h"


static const _TCHAR* GRID_HEADER = _T("関連リンクID");

enum GridCols
{
	kOID = 0,
	kSize
};


void RelLinkGridController::init( HWND hwnd)
{
	m_gridWnd = hwnd;
	CAxWindow axWnd( hwnd );
	axWnd.QueryControl(&m_relLinkGrid);

	// ヘッダ作成
	grid_util::FormatString(m_relLinkGrid, GRID_HEADER);
	// 列幅調整
	grid_util::AutoRowColSize(m_relLinkGrid);

	m_relLinkGrid->put_ExplorerBar(flexExSortShowAndMove);
	m_relLinkGrid->put_ExtendLastCol(VARIANT_TRUE);
	m_relLinkGrid->put_SelectionMode(flexSelectionByRow);
}

void RelLinkGridController::setData(const std::vector<long>& oids)
{
	m_relLinkGrid->put_Rows(oids.size() + 1);

	int row = 1;
	for( long oid : oids )
	{
		// VS GridCtrlへ登録
		CString strOid = uh::str_util::ToString(oid);
		m_relLinkGrid->put_TextMatrix(row, kOID, strOid.GetBuffer());
		++row;
	}
}

std::vector<long> RelLinkGridController::getData() const
{
	std::vector<long> oids;

	for( int i = 1; i < m_relLinkGrid->Rows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		CComVariant vaId = grid_util::GetValue(m_relLinkGrid, i, GridCols::kOID);
		vaId.ChangeType(VT_I4);

		oids.push_back(vaId.lVal);
	}

	return oids;
}

void RelLinkGridController::addRow()
{
	long rows(-1);
	m_relLinkGrid->get_Rows(&rows);
	m_relLinkGrid->put_Rows(rows + 1);
}

void RelLinkGridController::deleteRow(const std::vector<long>& oids)
{
	int i = 1;
	while( i <= m_relLinkGrid->Rows )
	{
		// 指定した削除対象でなければ何もしない
		CComVariant vaId = grid_util::GetValue(m_relLinkGrid, i, GridCols::kOID);
		vaId.ChangeType(VT_I4);
		if( std::find(oids.begin(), oids.end(), vaId.lVal) == oids.end() ){
			++i;
			continue;
		}

		m_relLinkGrid->RemoveItem(i);
		++i;
	}
}

void RelLinkGridController::deleteSelectedRow()
{
	int i = m_relLinkGrid->FixedRows;
	while( i <= m_relLinkGrid->Rows )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_relLinkGrid->get_IsSelected(i, &vb );
		if( vb == VARIANT_FALSE ){
			++i;
			continue;
		}

		m_relLinkGrid->RemoveItem(i);
	}
}

void RelLinkGridController::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId)
{
	long lRow(-1);
	m_relLinkGrid->get_MouseRow(&lRow);

	if( lRow < 0 )
		return;

	// 行選択
	for( int i = m_relLinkGrid->FixedRows; i <= m_relLinkGrid->Rows - m_relLinkGrid->FixedRows; ++i ) {
		m_relLinkGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
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

std::vector<long> RelLinkGridController::getSelectedData() const
{
	std::vector<long> oids;
	for( int i = 1; i < m_relLinkGrid->Rows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_relLinkGrid->get_IsSelected(i, &vb );
		if( vb == VARIANT_FALSE )
			continue;

		CComVariant vaId = grid_util::GetValue(m_relLinkGrid, i, GridCols::kOID);
		vaId.ChangeType(VT_I4);
		oids.push_back(vaId.lVal);
	}
	
	return oids;
}
