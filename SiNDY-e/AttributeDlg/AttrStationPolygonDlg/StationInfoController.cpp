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

#include "StationInfoController.h"

// WinLib
#include "WinLib/type_convert.h"


using namespace sindy::schema;


// 初期化
bool CStationInfoController::init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow )
{
	// グリッドコントロール設定
	//   グリッド初期化
	CAxWindow axWnd1( hwnd1 );
	CAxWindow axWnd2( hwnd2 );
	axWnd1.QueryControl( &m_spStationGrid );
	axWnd2.QueryControl( &m_spSearchStationGrid );
	m_spStationGrid->put_Editable( flexEDKbdMouse );
	m_spSearchStationGrid->put_Editable( flexEDKbdMouse );

	//   グリッドのヘッダ作成
	grid_util::FormatString( m_spStationGrid, station_grid::HEADER );
	grid_util::FormatString( m_spSearchStationGrid, search_station_grid::HEADER );

	//   グリッドの列幅調整
	grid_util::AutoRowColSize( m_spStationGrid );
	grid_util::AutoRowColSize( m_spSearchStationGrid );

	//   グリッドのチェックボックス列設定
	m_spSearchStationGrid->put_ColDataType( search_station_grid::grid_cols::Check, flexDTBoolean );


	// 関連テーブル操作オブジェクトの初期化
	if( ! m_RelTableMgr.init( ipRow ))
		return false;


	// 駅情報紐付きテーブルの読み込み
	m_StationInfoData.clear();
	m_StationInfoData_Org.clear();
	m_RelTableMgr.readRelStationSite( m_StationInfoData );
	m_RelTableMgr.readRelStationSite( m_StationInfoData_Org );

	return true;
}


// 駅情報グリッドの表示を更新する
void CStationInfoController::updateStationInfoGrid()
{
	using namespace station_grid;
	using namespace uh;

	// グリッドのサイズ設定(+1はヘッダ分)
	m_spStationGrid->PutRows(m_StationInfoData.size() + 1);

	// グリッドに表示
	long row = 1;
	for( const auto& record : m_StationInfoData )
	{
		// グループID,駅名取得
		CString strGroupId     = str_util::ToString( record.lGroupId );
		CString strStationName = m_RelTableMgr.getStationName( record.lGroupId );

		// グリッドに格納
		m_spStationGrid->put_TextMatrix( row, grid_cols::GroupId, CComBSTR(strGroupId));	
		m_spStationGrid->put_TextMatrix( row, grid_cols::StationName, CComBSTR(strStationName));	

		++row;
	}
	
	// 列幅調整
	grid_util::AutoRowColSize( m_spStationGrid );
	drawColor();
}


// 駅名検索グリッドの表示を更新する
void CStationInfoController::updateSearchStationInfoGrid()
{
	using namespace search_station_grid;
	using namespace uh;

	// グリッドのサイズ設定
	// 周辺路線データ数 - 路線リストに追加済みのデータ数 + ヘッダ(+1)
	m_spSearchStationGrid->PutRows(m_SearchStationNameData.size() - m_SearchStationNameData.getDupliCount(m_StationInfoData) + 1);

	// グリッドに表示
	long row = 1;
	for( const auto& record : m_SearchStationNameData )
	{
		// 駅情報グリッドに存在するレコードは表示しない
		if( m_StationInfoData.isExist( record ) )
			continue;
		
		// チェックボックス初期化
		m_spSearchStationGrid->put_Cell(flexcpChecked, CComVariant(row), CComVariant(0L), CComVariant(row), CComVariant(0L), CComVariant(flexUnchecked));

		// グループID取得
		CString strGroupId = str_util::ToString( record.lGroupId );
		// 駅名取得
		CString strStationName = m_RelTableMgr.getStationName( record.lGroupId );

		// グリッドに格納
		m_spSearchStationGrid->put_TextMatrix( row, grid_cols::GroupId, CComBSTR( strGroupId ));	
		m_spSearchStationGrid->put_TextMatrix( row, grid_cols::StationName, CComBSTR( strStationName ));	

		++row;
	}

	// 列幅調整
	grid_util::AutoRowColSize( m_spSearchStationGrid );
}


// グリッドでの変更点(追加/削除)を関連テーブルに反映
bool CStationInfoController::updateTable()
{
	CRelStationSiteData AddData;
	CRelStationSiteData DeleteData;

	// 追加・変更データ群取得
	for( const auto& rec : m_StationInfoData )
	{
		// 追加データ取得
		const auto org_iter = m_StationInfoData_Org.find( rec );
		if( org_iter == m_StationInfoData_Org.end() )
		{
			AddData.add(rec);
		}
	}

	// 削除データ群取得
	for( const auto& rec : m_StationInfoData_Org )
	{
		if( ! m_StationInfoData.isExist( rec ))
			DeleteData.add( rec );
	}
	

	// 変更を反映
	bool bResult = true;
	bResult &= m_RelTableMgr.insertData( AddData );
	bResult &= m_RelTableMgr.deleteData( DeleteData );

	return bResult;
}


// チェックの入った駅情報検索データを駅情報データへ追加
void CStationInfoController::addCheckedData()
{
	using namespace station_grid;

	// チェックが入っているレコードのグループIDを取得
	std::vector<long> IDs = getCheckedId();
	if( IDs.empty() ) return;

	// 駅情報データにレコードを追加
	for( int id : IDs )
	{
		const auto rec = m_SearchStationNameData.find( id );
		if( rec == m_SearchStationNameData.end() )
			return;

		m_StationInfoData.add( *rec );
	}

	updateChangeStatus();
}


// 駅情報グリッドの選択中の行を削除する
void CStationInfoController::deleteSelectedRow()
{
	// 駅情報グリッドを走査
	for( int i = m_spStationGrid->FixedRows; i <= m_spStationGrid->Rows - m_spStationGrid->FixedRows; ++i )
	{
		// 選択中でない列には何もしない
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_spStationGrid->get_IsSelected(i, &vb );
		if( ! vb )
			continue;

		// コンテナから削除
		long lGroupId = getGroupIdFromStationGrid(i);
		m_StationInfoData.erase( lGroupId );
	}

	// 変更したか否かの状態を更新
	updateChangeStatus();
}


// 指定した駅名のレコードを読み込む
void CStationInfoController::searchStationName( const CString& strStationName )
{
	if( strStationName.IsEmpty() )
		return;

	// 駅名から読み込んでコンテナに格納
	m_SearchStationNameData.clear();
	m_RelTableMgr.readRelStationSiteByStationName( strStationName, m_SearchStationNameData);
}


// 駅情報グリッドクリック時の処理
void CStationInfoController::OnStationInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable )
{
	if( ! bEnable )
		return;

	// クリック行取得
	long lRow = -1;
	m_spStationGrid->get_MouseRow(&lRow);
	if( lRow < 0 )
		return;

	// 行選択
	for( int i = m_spStationGrid->FixedRows; i <= m_spStationGrid->Rows - m_spStationGrid->FixedRows; ++i )
	{
		m_spStationGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
	}

	// 右クリックのときポップアップメニュー表示
	if( Button == clicked::kRight )
	{
		POINT p = {};
		GetCursorPos(&p);
		CMenu menu;
		menu.LoadMenu( menuId );
		menu.GetSubMenu(0).TrackPopupMenu( TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, p.x, p.y, hwnd);
	}
}


// 駅情報グリッド編集前の処理
void CStationInfoController::OnStationInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	*Cancel = VARIANT_TRUE;
}


// 駅情報検索グリッド編集前の処理
void CStationInfoController::OnSearchStationInfoBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable)
{
	// 編集開始されている場合はチェックボックスのみ編集可
	if( bCheckEnable && Col == search_station_grid::grid_cols::Check )
		return;

	*Cancel = VARIANT_TRUE;
}


// 駅情報検索グリッドからチェックの入っているレコードのグループID群を取得
std::vector<long> CStationInfoController::getCheckedId()
{
	std::vector<long> group_ids;

	for( int i = 0; i < m_spSearchStationGrid->GetRows(); ++i )
	{
		// チェックが入っていなければスキップ
		if( ! isCheckedSearchStationGrid(i) ) continue;

		// グループID取得
		group_ids.push_back(getGroupIdFromSearchStationGrid(i));
	}

	return group_ids;
}


// 駅情報検索グリッドの指定行にチェックが入っているかどうかを取得
bool CStationInfoController::isCheckedSearchStationGrid(int index) const
{
	CComVariant vaCheck = grid_util::GetValue(m_spSearchStationGrid, index, search_station_grid::grid_cols::Check);
	vaCheck.ChangeType(VT_BOOL);
	return VB2bool(vaCheck.boolVal);
}


// 駅情報検索グリッドの指定行のグループIDを取得
long CStationInfoController::getGroupIdFromSearchStationGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue( m_spSearchStationGrid, index, search_station_grid::grid_cols::GroupId );
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// 駅情報グリッドの指定行のグループIDを取得
long CStationInfoController::getGroupIdFromStationGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue( m_spStationGrid, index, station_grid::grid_cols::GroupId );
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// 変更したか否かのフラグを更新	 
void CStationInfoController::updateChangeStatus()
{
	if( m_StationInfoData.size() != m_StationInfoData_Org.size() )
	{
		m_bIsChange = true;
		return;
	}

	for( const auto& rec : m_StationInfoData )
	{
		// 追加レコード確認
		if( ! m_StationInfoData_Org.isExist( rec ))
		{
			m_bIsChange = true;
			return;
		}
	}

	// 削除レコード確認
	for( const auto& rec : m_StationInfoData_Org )
	{
		if( ! m_StationInfoData.isExist( rec ))
		{
			m_bIsChange = true;
			return;
		}
	}

	m_bIsChange = false;
}


// 駅情報グリットの変更行を色付けする 
void CStationInfoController::drawColor()
{
	long idx = 1;
	for( const auto& rec : m_StationInfoData )
	{
		// 変更があればオレンジ、なければ白にレコードを色づけする
		COLORREF color = m_StationInfoData_Org.isExist( rec ) ? grid_util::color::sindyDefault : grid_util::color::sindyModified;

		m_spStationGrid->PutCell( flexcpBackColor, idx, 0, idx, m_spStationGrid->GetCols() - 1, color );

		++idx;
	}
}
