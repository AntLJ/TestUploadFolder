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

#include "RailwayLineController.h"
#include "WinLib/type_convert.h"


using namespace sindy::schema;

// グリッドのヘッダ
static const _TCHAR* RAILWAYLINE_GRID_HEADER    = _T("RAILWAY_LINE_ID|路線名|会社名");
static const _TCHAR* SEARCH_RAILWAY_GRID_HEADER = _T(" |RAILWAY_LINE_ID|路線名|会社名");


// グリッドの初期化
bool CRailwayLineController::init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper )
{
	// グリッドコントロール作成
	CAxWindow axWnd1( hwnd1 );
	CAxWindow axWnd2( hwnd2 );
	axWnd1.QueryControl( &m_spRailwayLineGrid );
	axWnd2.QueryControl( &m_spSearchRailwayGrid );

	m_spRailwayLineGrid->put_Editable( flexEDKbdMouse );
	m_spSearchRailwayGrid->put_Editable( flexEDKbdMouse );

	// ヘッダ作成
	grid_util::FormatString( m_spRailwayLineGrid, RAILWAYLINE_GRID_HEADER );
	grid_util::FormatString( m_spSearchRailwayGrid, SEARCH_RAILWAY_GRID_HEADER );
	// 列幅調整
	grid_util::AutoRowColSize( m_spRailwayLineGrid );
	grid_util::AutoRowColSize( m_spSearchRailwayGrid );

	// チェックボックス列設定
	m_spSearchRailwayGrid->put_ColDataType( search_railway_grid::grid_cols::Check, flexDTBoolean );

	m_cArcHelper = cArcHelper.GetApp();

	// 関連テーブル操作オブジェクト初期化
	if( ! m_RelTableMgr.init( ipRow, m_cArcHelper ))
		return false;

	// 路線リストデータ初期化
	m_RailwayLineData.clear();
	m_SearchFromAroundStaData.clear();
	m_RailwayLineData_Org.clear();
	// 路線情報読み込み
	m_RelTableMgr.readRelRailwayLink( m_RailwayLineData );
	m_RelTableMgr.readRelRailwayLink( m_RailwayLineData_Org );

	return true;
}


// 路線リストグリッドの表示を更新する
void CRailwayLineController::updateRailwayLineGrid()
{
	using namespace railwayline_grid;

	// グリッドのサイズ設定(+1はヘッダ分)
	m_spRailwayLineGrid->PutRows(m_RailwayLineData.size() + 1);

	// グリッドに表示
	long row = 1;
	for( const auto& record : m_RailwayLineData )
	{
		// RAILWAY_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.lRailwayLineOid);
		m_spRailwayLineGrid->put_TextMatrix( row, grid_cols::RailwayLineOid, CComBSTR(strJoinLineId));	
		// 路線名
		m_spRailwayLineGrid->put_TextMatrix( row, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	
		// 会社名
		m_spRailwayLineGrid->put_TextMatrix( row, grid_cols::CorpName    , CComBSTR(record.strCorpName));	

		++row;
	}
	
	// 列幅調整
	grid_util::AutoRowColSize( m_spRailwayLineGrid );
	drawColor();
}


// 路線検索グリッドの表示を更新する
void CRailwayLineController::updateSearchRailwayGrid()
{
	using namespace search_railway_grid;

	CRelTableData* data = nullptr;

	// 検索モードで表示データ決定
	switch(m_SearchMode)
	{
		case kAroundSearch: data = &m_SearchFromAroundStaData;    break;
		case kStarionNameSearch: data = &m_SearchFromStationNameData; break;
		default: 
			return; 
			break;
	}

	putIntoSearchGrid(*data);

}


// 路線検索グリッドのチェックの入ったレコードを路線リストへ追加
void CRailwayLineController::addCheckedData()
{
	using namespace search_railway_grid;

	// チェックが入っているIDを取得
	std::vector<long> IDs = getCheckedId();
	if( IDs.empty() ) return;

	// 検索モードにより格納対象変更
	for( int id : IDs )
	{
		if( m_SearchMode == kAroundSearch)
			addCheckedRecord(id, m_SearchFromAroundStaData);
		else if( m_SearchMode == kStarionNameSearch)
			addCheckedRecord(id, m_SearchFromStationNameData);
	}

	updateChangeStatus();
}


// 指定フィーチャから一定範囲の駅ポイント情報を読み込む
void CRailwayLineController::searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer)
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
	m_SearchFromAroundStaData.clear();
	for( const auto& sta_point : cItems)
	{
		long lOid = AheGetOID( sta_point.GetFeature() );

		// STATION_POINTのOIDをキーにデータ読み込み
		m_RelTableMgr.readRailLineFromStaPtOid(lOid, m_SearchFromAroundStaData);
	}
}


// 会社名を検索しグリッドに表示 
void CRailwayLineController::searchStaName(CString strStaName)
{
	if( strStaName.IsEmpty() )
		return;

	// 駅名検索 & 表示
	m_SearchFromStationNameData.clear();
	m_RelTableMgr.readRailwayLineFromStaName(strStaName, m_SearchFromStationNameData);
}


// リストビューの変更点を関連テーブルに反映
bool CRailwayLineController::updateTable()
{
	CRelTableData AddData;
	std::vector<long> DeleteIds;

	// 追加・変更データ群取得
	for( const auto& rec : m_RailwayLineData )
	{
		// 追加データ取得
		const auto org_iter = m_RailwayLineData_Org.find(rec.lRailwayLineOid);
		if( org_iter == m_RailwayLineData_Org.end() )
		{
			AddData.add(rec);
			continue;
		}
	}

	// 削除データ群取得
	for( const auto& rec : m_RailwayLineData_Org )
	{
		if( ! m_RailwayLineData.isExist( rec.lRailwayLineOid ))
			DeleteIds.push_back(rec.lRailwayLineOid);
	}
	

	// 変更を反映
	bool bResult = true;
	bResult &= m_RelTableMgr.addData(AddData);
	bResult &= m_RelTableMgr.deleteData(DeleteIds);

	return bResult;
}


// グリットの変更箇所を色付けする 
void CRailwayLineController::drawColor()
{
	long idx = 1;
	for( const auto& rec : m_RailwayLineData )
	{
		// 変更があればオレンジ、なければ白にレコードを色づけする
		COLORREF color = m_RailwayLineData_Org.isExist(rec.lRailwayLineOid) ? RGB(255,255,255) : RGB(255,218,185);

		m_spRailwayLineGrid->PutCell( flexcpBackColor, idx, 0, idx, m_spRailwayLineGrid->GetCols() - 1, color );

		++idx;
	}
}


// 路線リストグリッドクリック時の処理
void CRailwayLineController::OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable )
{
	if( ! bEnable )
		return;

	long lRow = -1;
	m_spRailwayLineGrid->get_MouseRow(&lRow);

	if( lRow < 0 )
		return;

	// 行選択
	for( int i = m_spRailwayLineGrid->FixedRows; i <= m_spRailwayLineGrid->Rows - m_spRailwayLineGrid->FixedRows; ++i )
	{
		m_spRailwayLineGrid->put_IsSelected(i, (i==lRow) ? VARIANT_TRUE : VARIANT_FALSE );
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


// 路線リストグリッド編集前の処理
void CRailwayLineController::OnRailwayLineGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel)
{
	// RAILWAY_LINE_IDに一致するRAILWAY_LINKのフィーチャー群を取得する(リンクのハイライト向け)
	long lid = getIdFromRailwayLineGrid(Row);
	m_HighlightFeatures = m_RelTableMgr.getRailwayLinkFeatures(lid);

	*Cancel = VARIANT_TRUE;
}


// 路線検索グリッド編集前の処理
void CRailwayLineController::OnSearchRailwayBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable)
{
	// 編集開始されている場合のチェックボックスのみ編集可
	if( bCheckEnable && Col == search_railway_grid::grid_cols::Check ) {
		return;
	}

	// RAILWAY_LINE_IDに一致するRAILWAY_LINKのフィーチャー群を取得する(リンクのハイライト向け)
	long lid = getIdFromSearchRailwayGrid(Row);
	m_HighlightFeatures = m_RelTableMgr.getRailwayLinkFeatures(lid);

	*Cancel = VARIANT_TRUE;
}


// 選択中の行を削除する
void CRailwayLineController::deleteSelectedRow()
{
	// 選択列のJOIN_INFO_ID を取得
	for( int i = m_spRailwayLineGrid->FixedRows; i <= m_spRailwayLineGrid->Rows - m_spRailwayLineGrid->FixedRows; ++i )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		m_spRailwayLineGrid->get_IsSelected(i, &vb );
		if( vb )
		{
			long lid = getIdFromRailwayLineGrid(i);
			// コンテナから削除
			m_RailwayLineData.erase(lid);
		}
	}

	updateChangeStatus();
}


// フィーチャーをハイライトさせる
void CRailwayLineController::highlightFeatures()
{
	for( const auto& feature : m_HighlightFeatures )
	{
		IGeometryPtr ipGeo;
		feature->get_Shape(&ipGeo);
		if( FAILED( feature->get_Shape(&ipGeo)) || !ipGeo )
			continue;

		AheDrawPolyline( m_cArcHelper.GetFocusMapScreenDisplay(), ipGeo, RGB( 255, 255, 0 ), 7, esriSLSSolid, TRUE );
	}
}


// 路線検索リストグリッドからチェックの入っているレコードのID群を取得
std::vector<long> CRailwayLineController::getCheckedId()
{
	std::vector<long> retVec;

	for( int i = 0; i < m_spSearchRailwayGrid->GetRows(); ++i )
	{
		// チェックが入っていなければスキップ
		if( ! isCheckedSearchRailwayGrid(i) ) continue;

		// ID格納
		retVec.push_back(getIdFromSearchRailwayGrid(i));
	}

	return retVec;
}


// チェックが入っているかどうかを取得
bool CRailwayLineController::isCheckedSearchRailwayGrid(int index) const
{
	CComVariant vaCheck = grid_util::GetValue(m_spSearchRailwayGrid, index, search_railway_grid::grid_cols::Check);
	vaCheck.ChangeType(VT_BOOL);
	return VB2bool(vaCheck.boolVal);
}


// 路線検索グリッドの指定行のRAILWAY_LINE_IDを取得 
long CRailwayLineController::getIdFromSearchRailwayGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spSearchRailwayGrid, index, search_railway_grid::grid_cols::RailwayLineOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// 路線リストグリッドの指定行のRAILWAY_LINE_IDを取得
long CRailwayLineController::getIdFromRailwayLineGrid(int index) const
{
	CComVariant vaId = grid_util::GetValue(m_spRailwayLineGrid, index, railwayline_grid::grid_cols::RailwayLineOid);
	vaId.ChangeType(VT_I4);
	return vaId.lVal;
}


// 変更したか否かのフラグを更新	 
void CRailwayLineController::updateChangeStatus()
{
	if( m_RailwayLineData.size() != m_RailwayLineData_Org.size() )
	{
		m_bIsChange = true;
		return;
	}

	for( const auto& rec : m_RailwayLineData )
	{
		// 追加レコード確認
		if( ! m_RailwayLineData_Org.isExist(rec.lRailwayLineOid) )
		{
			m_bIsChange = true;
			return;
		}
	}

	// 削除レコード確認
	for( const auto& rec : m_RailwayLineData_Org )
	{
		if( ! m_RailwayLineData.isExist( rec.lRailwayLineOid ))
		{
			m_bIsChange = true;
			return;
		}
	}

	m_bIsChange = false;
}


// 路線検索グリッドへ値を格納する
void CRailwayLineController::putIntoSearchGrid(const CRelTableData& cData)
{
	using namespace search_railway_grid;

	// グリッドのサイズ設定
	// 周辺路線データ数 - 路線リストに追加済みのデータ数 + ヘッダ(+1)
	m_spSearchRailwayGrid->PutRows(cData.size() - cData.getDupliCount(m_RailwayLineData) + 1);

	// グリッドに表示
	long row = 1;
	for( const auto& record : cData )
	{
		// 「路線リスト」グリッドに存在するレコードは表示しない
		if( m_RailwayLineData.isExist( record.lRailwayLineOid ) )
			continue;
		
		// チェックボックス初期化
		m_spSearchRailwayGrid->put_Cell(flexcpChecked, CComVariant(row), CComVariant(0L), CComVariant(row), CComVariant(0L), CComVariant(flexUnchecked));

		// RAILWAY_LINE_ID
		CString strJoinLineId = uh::str_util::format(_T("%d"), record.lRailwayLineOid);

		m_spSearchRailwayGrid->put_TextMatrix( row, grid_cols::RailwayLineOid, CComBSTR(strJoinLineId));	
		// 駅名
		m_spSearchRailwayGrid->put_TextMatrix( row, grid_cols::RailwayName    , CComBSTR(record.strRailwayName));	
		// 路線名
		m_spSearchRailwayGrid->put_TextMatrix( row, grid_cols::CorpName    , CComBSTR(record.strCorpName));	

		++row;
	}

	// 列幅調整
	grid_util::AutoRowColSize( m_spSearchRailwayGrid );
}


//指定コンテナの指定鉄道路線IDのレコードを路線リストデータコンテナへ格納する
void CRailwayLineController::addCheckedRecord(long lId, CRelTableData& cData)
{
	const auto rec = cData.find( lId );
	if( rec == cData.end() )
		return;

	m_RailwayLineData.add(*rec);
}


// 路線リストデータを同期する
void CRailwayLineController::syncRailwayLineData(CRailwayLineController& cRailwayLineCtlr)
{
	m_RailwayLineData.copyData( cRailwayLineCtlr.m_RailwayLineData );
	cRailwayLineCtlr.updateChangeStatus();
}

