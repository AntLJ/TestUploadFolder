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

#include "LineInfoController.h"
#include "util.h"

using namespace sindy::schema;

// グリッドのヘッダ
static const _TCHAR* LINEINFO_GRID_HEADER   = _T("JOIN_LINE_INFO_OID|駅名|路線名|駅番号");
static const _TCHAR* AROUND_STA_GRID_HEADER = _T(" |JOIN_LINE_INFO_OID|駅名|路線名");


// グリッドの初期化
bool CLineInfoController::init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper )
{
	// グリッドコントロール作成
	CAxWindow axWnd1( hwnd1 );
	CAxWindow axWnd2( hwnd2 );
	axWnd1.QueryControl( &m_spLineInfoGrid );
	axWnd2.QueryControl( &m_spAroundStaGrid );

	m_spLineInfoGrid->put_Editable( flexEDKbdMouse );
	m_spAroundStaGrid->put_Editable( flexEDKbdMouse );

	// ヘッダ作成
	grid_util::FormatString( m_spLineInfoGrid, LINEINFO_GRID_HEADER );
	grid_util::FormatString( m_spAroundStaGrid, AROUND_STA_GRID_HEADER );
	// 列幅調整
	grid_util::AutoRowColSize( m_spLineInfoGrid );
	grid_util::AutoRowColSize( m_spAroundStaGrid );

	// チェックボックス列設定
	m_spAroundStaGrid->put_ColDataType( around_sta_grid::grid_cols::Check, flexDTBoolean );

	m_cArcHelper = cArcHelper.GetApp();

	// 関連テーブル操作オブジェクト初期化
	if( !m_RelTableMgr.init( ipRow, m_cArcHelper) )
		return false;

	// 路線リストデータ初期化
	m_LineInfoData.clear();
	m_AroundStaData.clear();
	m_LineInfoData_Org.clear();
	// 路線情報読み込み
	m_RelTableMgr.read( m_LineInfoData );
	m_RelTableMgr.read( m_LineInfoData_Org );

	return true;
}


// 路線リストグリッドの表示を更新する
void CLineInfoController::updateLineInfoGrid()
{
	using namespace lineinfo_grid;

	// グリッドのサイズ設定(+1はヘッダ分)
	m_spLineInfoGrid->PutRows(m_LineInfoData.size() + 1);

	// グリッドに表示
	long r = 1;
	for( auto& record : m_LineInfoData )
	{
		// JOIN_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.nJoinLineId);
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::JoinLineInfoOid, CComBSTR(strJoinLineId));	
		// 駅名
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::StationName    , CComBSTR(record.strStationName));	
		// 路線名
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	
		// 駅番号
		m_spLineInfoGrid->put_TextMatrix( r, grid_cols::StationNumber  , CComBSTR(record.strStationNumber));	

		++r;
	}
	
	// 列幅調整
	grid_util::AutoRowColSize( m_spLineInfoGrid );
	drawColor();
}


// 周辺駅リストグリッドの表示を更新する
void CLineInfoController::updateAroundStaGrid()
{
	using namespace around_sta_grid;

	// グリッドのサイズ設定
	// 周辺駅データ数 - 路線リストに追加済みのデータ数 + ヘッダ(+1)
	m_spAroundStaGrid->PutRows(m_AroundStaData.size() - m_AroundStaData.getDupliCount(m_LineInfoData) + 1);

	// グリッドに表示
	long row = 1;
	for( auto& record : m_AroundStaData )
	{
		// 「路線リスト」リストビューに存在するレコードは表示しない
		if( m_LineInfoData.isExist( record.nJoinLineId ) )
			continue;

		// チェックボックス初期化
		m_spAroundStaGrid->put_Cell(flexcpChecked, CComVariant(row), CComVariant(0L), CComVariant(row), CComVariant(0L), CComVariant(flexUnchecked));

		// JOIN_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.nJoinLineId);

		m_spAroundStaGrid->put_TextMatrix( row, grid_cols::JoinLineInfoOid, CComBSTR(strJoinLineId));	
		// 駅名
		m_spAroundStaGrid->put_TextMatrix( row, grid_cols::StationName    , CComBSTR(record.strStationName));	
		// 路線名
		m_spAroundStaGrid->put_TextMatrix( row, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	

		++row;
	}

	// 列幅調整
	grid_util::AutoRowColSize( m_spAroundStaGrid );

}


// 周辺駅リストグリッドのチェックの入ったレコードを路線リストへ追加
void CLineInfoController::addCheckedRecord()
{
	// チェックが入っているIDを取得
	std::vector<long> IDs = getCheckedId();
	if( IDs.empty() ) return;

	for( int id : IDs )
	{
		// データからIDで検索
		auto rec = m_AroundStaData.find( id );
		if( rec == m_AroundStaData.end() ) continue;
		m_LineInfoData.add(*rec);
	}

	// リストビューの表示に反映
	updateLineInfoGrid();
	updateAroundStaGrid();

	updateChangeStatus();
}


// 駅情報を検索し表示
void CLineInfoController::searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer)
{
	if( !ipFeature || !ipLayer)
		return;

	// 選択しているフィーチャの形状を取得
	IGeometryPtr ipGeom;
	ipFeature->get_Shape( &ipGeom );
	ITopologicalOperatorPtr ipTopo( ipGeom );
	if( !ipTopo )
		return;

	// 検索範囲を度で取得
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( SEARCH_METER, esriMeters, esriDecimalDegrees, &dConvRange );

	// 空間検索
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );
	if( !ipGeoBuffer )
		return;

	// 一定エリア内のフィーチャ群取得
	CTargetItems cItems;
	AheSelectByShapeFromLayer( ipGeoBuffer, cItems, ipLayer );

	// 格納
	m_AroundStaData.clear();
	for( auto& sta_point : cItems)
	{
		long lOid = AheGetOID( sta_point.GetFeature() );

		// STATION_POINTのOIDをキーにデータ読み込み
		m_RelTableMgr.readJoinLineInfo(lOid, m_AroundStaData);

	}

	// グリッドを更新
	updateAroundStaGrid();
}


// リストビューの変更点を関連テーブルに反映
bool CLineInfoController::updateTable()
{
	CRelTableData AddData;
	std::vector<long> DeleteIds;
	CRelTableData ChangeStaNumData;

	// 追加・変更データ群取得
	for( auto iter = m_LineInfoData.begin(); iter != m_LineInfoData.end(); ++iter)
	{
		// 追加データ取得
		auto org_iter = m_LineInfoData_Org.find(iter->nJoinLineId);
		if( org_iter == m_LineInfoData_Org.end() )
		{
			AddData.add(*iter);
			continue;
		}
		// 駅番号が変更されたID格納
		if( ! iter->equalStaNumber(*org_iter) )
		{
			ChangeStaNumData.add(*iter);
		}
	}

	// 削除データ群取得
	for( auto org_iter = m_LineInfoData_Org.begin(); org_iter != m_LineInfoData_Org.end(); ++org_iter)
	{
		if( ! m_LineInfoData.isExist( org_iter->nJoinLineId ))
			DeleteIds.push_back(org_iter->nJoinLineId);
	}

	// 変更を反映
	bool bResult = true;
	bResult &= m_RelTableMgr.addData(AddData);
	bResult &= m_RelTableMgr.changeStationNumber(ChangeStaNumData);
	bResult &= m_RelTableMgr.deleteData(DeleteIds);

	return bResult;
}


// グリットの変更箇所を色付けする 
void CLineInfoController::drawColor()
{
	long idx = 1;
	for( auto& rec : m_LineInfoData )
	{
		// 駅番号が変更されたなら駅番号セルだけ色づけ
		auto iter = m_LineInfoData_Org.find( rec.nJoinLineId );
		if( iter != m_LineInfoData_Org.end() &&  ! rec.equalStaNumber(*iter)  )
		{
			m_spLineInfoGrid->PutCell( flexcpBackColor, idx, lineinfo_grid::grid_cols::StationNumber, 
										                idx, lineinfo_grid::grid_cols::StationNumber,
														RGB(255,218,185));
			++idx;
			continue;
		}

		// 変更がないなら色づけなしにする
		if( m_LineInfoData_Org.isExist( rec.nJoinLineId ))
		{
			m_spLineInfoGrid->PutCell( flexcpBackColor, idx, 0, 
													idx, m_spLineInfoGrid->GetCols() - 1, 
													RGB(255,255,255));
			++idx;
			continue;
		}

		// 追加されたレコードなら色づけ
		m_spLineInfoGrid->PutCell( flexcpBackColor, idx, 0, 
													idx, m_spLineInfoGrid->GetCols() - 1, 
													RGB(255,218,185));
		++idx;
	}
}


// グリッドを編集不可にする 
void CLineInfoController::disable()
{
	m_spLineInfoGrid->Enabled = false;
	m_spAroundStaGrid->Enabled = false;
}


// 路線リストグリッドクリック時の処理
void CLineInfoController::OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId)
{
	long lRow = -1;
	m_spLineInfoGrid->get_MouseRow(&lRow);

	if( lRow < 0 )
		return;

	// 行選択
	for( int i = m_spLineInfoGrid->FixedRows; i <= m_spLineInfoGrid->Rows - m_spLineInfoGrid->FixedRows; ++i )
	{
		m_spLineInfoGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
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


// 路線リストグリッド編集後の処理
void CLineInfoController::OnLineInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	// 駅番号以外編集不可
	if( Col != lineinfo_grid::grid_cols::StationNumber)
		*Cancel = VARIANT_TRUE;
}


// 路線リストグリッド編集後の処理
void CLineInfoController::OnLineInfoGridAfterEdit(long Row, long Col)
{
	// 駅番号以外編集不可
	if( Col != lineinfo_grid::grid_cols::StationNumber)
		return;

	// コンテナ更新
	long lid = getIdFromLineInfoGrid(Row);
	auto iter = m_LineInfoData.find(lid);
	iter->strStationNumber = getStaNumFromLineInfoGrid(Row); 

	updateLineInfoGrid();

	updateChangeStatus();
}


// 路線リストグリッド編集前の処理
void CLineInfoController::OnAroundStaBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	// チェックボックスは編集可
	if( Col == around_sta_grid::grid_cols::Check)
		return;

	*Cancel = VARIANT_TRUE;

	// 乗り入れ路線IDから駅ポイントのフィーチャを取得
	long lid = getIdFromAroundStaGrid(Row);
	IFeaturePtr ipFeature = m_RelTableMgr.getStationPointFeature(lid);
	if( ! ipFeature )
		return;

	// 駅ポイントを点滅させる
	for( int i = 0; i < 3; ++i )
	{
		AheDrawVertex( m_cArcHelper.GetFocusMapScreenDisplay(), ipFeature, FALSE, 5, 255, 69, 0, esriSMSSquare, TRUE );
		Sleep(50);
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	}
}


// 選択中の行を削除する
void CLineInfoController::deleteSelectedRow()
{
	// 選択列のJOIN_INFO_ID を取得
	std::vector<long> DeleteIds;
	for( int i = m_spLineInfoGrid->FixedRows; i <= m_spLineInfoGrid->Rows - m_spLineInfoGrid->FixedRows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_spLineInfoGrid->get_IsSelected(i, &vb );
		if( vb )
		{
			long lid = getIdFromLineInfoGrid(i);
			DeleteIds.push_back(lid);
			// コンテナから削除
			m_LineInfoData.erase(lid);
		}
	}

	updateLineInfoGrid();
	updateAroundStaGrid();

	updateChangeStatus();
}


// 「周辺駅リスト」リストビューからチェックの入っているレコードのID群を取得
std::vector<long> CLineInfoController::getCheckedId()
{
	using namespace around_sta_grid;

	std::vector<long> retVec;

	for( int i = 0; i < m_spAroundStaGrid->GetRows(); ++i )
	{
		// チェックが入っていなければスキップ
		if( ! isCheckedAroundStaGrid(i) ) continue;

		// ID格納
		retVec.push_back(getIdFromAroundStaGrid(i));
	}

	return retVec;
}


// チェックが入っているかどうかを取得
bool CLineInfoController::isCheckedAroundStaGrid(int index) const
{
	CComVariant vaCheck = grid_util::GetValue(m_spAroundStaGrid, index, around_sta_grid::grid_cols::Check);
	vaCheck.ChangeType(VT_BOOL);
	return (vaCheck.boolVal == -1);
}


//周辺駅グリッドの指定列のJOIN_INFO_IDを取得 
long CLineInfoController::getIdFromAroundStaGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spAroundStaGrid, index, around_sta_grid::grid_cols::JoinLineInfoOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


//周辺駅グリッドの指定列のJOIN_INFO_IDを取得 
long CLineInfoController::getIdFromLineInfoGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spLineInfoGrid, index, lineinfo_grid::grid_cols::JoinLineInfoOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// 路線リストグリッドの指定列の駅番号を取得 
CString CLineInfoController::getStaNumFromLineInfoGrid(int index) const
{
	CComVariant vaString = grid_util::GetValue(m_spLineInfoGrid, index, lineinfo_grid::grid_cols::StationNumber);
	CString strStaNum(vaString);
	return strStaNum;
}


// 変更したか否かのフラグを更新	 
void CLineInfoController::updateChangeStatus()
{
	for( auto rec : m_LineInfoData )
	{
		auto iter = m_LineInfoData_Org.find(rec.nJoinLineId);
		// 追加レコード確認
		if( iter == m_LineInfoData_Org.end() )
		{
			m_bIsChange = true;
			return;
		}
		// 駅番号の変更確認
		if( ! rec.equalStaNumber(*iter) )
		{
			m_bIsChange = true;
			return;
		}
	}

	// 削除レコード確認
	for( auto rec : m_LineInfoData_Org )
	{
		if( ! m_LineInfoData.isExist( rec.nJoinLineId ))
		{
			m_bIsChange = true;
			return;
		}
	}

	m_bIsChange = false;
}
