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

// AttrStationPolygonDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrStationPolygonDlg.h"

// boost
#include <boost/assign/list_of.hpp>
#include <boost/range/algorithm.hpp>

using namespace sindy::schema;
using namespace sindy::schema::sj;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(STATION_POLYGON_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_POLYGON_CLASS)
			RCTOFIT(-1)
			RCSPACE(4)
			RCTOFIT(IDC_COMBO_POLYGON_CLASS)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_STATION_INFO)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 25)
			RCREST(IDC_GRID_STATION_INFO)
		ENDGROUP()
		RCSPACE(15)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_SEARCH_STATION)
			RCSPACE(12)
			RCTOFIT(IDC_STATIC_STATION_NAME)
			RCSPACE(8)
			RCTOFIT(IDC_EDIT_STATION_NAME)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_SEARCH_STATION)
			RCREST(-1)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 30)
			RCREST(IDC_GRID_SEARCH_STATION)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_BUTTON_APPLY)
		ENDGROUP()
		BEGINCOLS(WRCT_PCT, 0, 45)
			RCREST(-1)
		ENDGROUP()
		RCSPACE(5)
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrStationPolygonDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrStationPolygonDlg();
}



/// CAttrStationPolygonDlg メンバ関数


// コンストラクタ
CAttrStationPolygonDlg::CAttrStationPolygonDlg() 
	: ATTR_BASE_CLASS(STATION_POLYGON_MAP, IDD_STATIONPOLYGONDLG),
	  m_CurrentOid(0L),
	  m_bEditable(false)
{
	 // 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_POLYGON_CLASS, CContainedWindow(  _T("COMBO")    , this, IDC_COMBO_POLYGON_CLASS ) )
		( IDC_BUTTON_APPLY       , CContainedWindow(  _T("BUTTON")   , this, IDC_BUTTON_APPLY ) )
		;
}



// ダイアログ作成する際に一番最初に呼ばれる
LRESULT CAttrStationPolygonDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

	// コントロール
	CreateControlRelation();
	// サブクラス化
	for( auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow( GetDlgItem( it.first ) );
	}

	// シンクイベントのアドバイズ
	AtlAdviseSinkMap(this, true);

	return 1;
}


// WM_COMMAND 用イベントハンドラ
LRESULT CAttrStationPolygonDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if( ! m_bFinishInit ) return 0;

	// 操作メッセージ取得
	INT msg = HIWORD(wParam);						
	// 操作対象コントロール取得
	INT	nTargetControl = (INT)LOWORD(wParam);

	// エディットボックスの時に全選択にする
	if( msg == EN_SETFOCUS )
		SelectDlgItem( nTargetControl );
	// コンボボックスのリストボックスの長さを調節
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
		// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		// ダイアログを更新領域に追加します
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}

	return 0;
}


// 「適用」ボタンが押下時の処理
LRESULT CAttrStationPolygonDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// グリッドで指定したレコードをコンテナに追加
	getCurrentStationInfo()->addCheckedData();

	// グリッドの表示を更新
	getCurrentStationInfo()->updateStationInfoGrid();
	getCurrentStationInfo()->updateSearchStationInfoGrid();

	updateChanged();

	return 0;
}


// 駅名の検索ボタンが押下時の処理
LRESULT CAttrStationPolygonDlg::OnClickStationSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// エディットボックスの文字列の取得/has
	CWindow edit( GetDlgItem( IDC_EDIT_STATION_NAME ) );

	// 値を取得
	CString strStationName;
	edit.GetWindowText(strStationName);

	/// 駅名で検索 + グリッド更新
	getCurrentStationInfo()->searchStationName(strStationName);
	getCurrentStationInfo()->updateSearchStationInfoGrid();

	return 0;
}


// グリッド右クリックのメニューから「削除」ボタンが押されたときの処理
LRESULT CAttrStationPolygonDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// データ削除
	getCurrentStationInfo()->deleteSelectedRow();

	// グリッド更新
	getCurrentStationInfo()->updateStationInfoGrid();
	getCurrentStationInfo()->updateSearchStationInfoGrid();

	// 変更状態を更新
	updateChanged();

	return 0;
}


// [駅情報リストグリッド]編集前の処理
void __stdcall CAttrStationPolygonDlg::OnStationInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	getCurrentStationInfo()->OnStationInfoGridBeforeEdit( Row, Col, Cancel);
}


// [駅情報リストグリッド]マウスアップ時の処理
void __stdcall CAttrStationPolygonDlg::OnStationInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	getCurrentStationInfo()->OnStationInfoGridMouseUp(Button, Shift, X, Y, m_hWnd, IDR_MENU_STATION_INFO_GRID, m_bEditable );
}


// [駅情報検索グリッド]編集前の処理
void __stdcall CAttrStationPolygonDlg::OnStationSearchBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	getCurrentStationInfo()->OnSearchStationInfoBeforeEdit( Row, Col, Cancel, m_bEditable);
}


// 子ダイアログを作成する
HWND CAttrStationPolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// 各種エラーチェック後に呼び出される 
bool CAttrStationPolygonDlg::Update()
{
	// ツリーで表示されているすべてのフィーチャを更新
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), station_detail::kTableName )   != 0 &&
		    _tcsicmp( featureDef.GetTableName(), midzoom_platform::kTableName ) != 0 &&
		    _tcsicmp( featureDef.GetTableName(), highzoom_station::kTableName ) != 0 &&
		    _tcsicmp( featureDef.GetTableName(), midzoom_station::kTableName )  != 0 &&
		    _tcsicmp( featureDef.GetTableName(), lowzoom_platform::kTableName ) != 0 )
		{
			continue;
		}

		// 更新フィーチャのレイヤ番号とOIDを取得
		_IRowPtr ipRow = featureDef.GetFeature();
		ipRow->get_OID( &m_CurrentOid );
		m_CurrentLayerNo = CRelationTableHelper::getLayerNo( ipRow );

		// 関連テーブル情報を持っていなければ何もしない
		if( ! hasStationInfo( m_CurrentLayerNo, m_CurrentOid ) )
			continue;

		// 関連テーブルに変更がなければ何もしない
		if( ! getCurrentStationInfo()->isChange() )
			continue;

		// 関連テーブル情報の更新
		if( ! getCurrentStationInfo()->updateTable() )
			return false;
	}
	return true;
}


// 作成した子ダイアログを削除します。
void CAttrStationPolygonDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}


// 現在表示されるべきフィーチャのFeatureDefインデックスを渡す
BOOL CAttrStationPolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// STATION_DETAIL 以外ならば、ポリゴン種別の設定部分は非表示にする
	if( m_strClassName != station_detail::kTableName )
	{
		CWindow cComboBox( GetDlgItem( IDC_COMBO_POLYGON_CLASS ) );
		cComboBox.ShowWindow( SW_HIDE );
		CWindow cText( GetDlgItem( IDC_STATIC_POLYGON_CLASS ) );
		cText.ShowWindow( SW_HIDE );
	}

	// 当該フィーチャのレイヤ番号とOIDを取得
	m_CurrentLayerNo = CRelationTableHelper::getLayerNo( m_strClassName );
	GetCurrentFeatureDef()->GetFeature()->get_OID( &m_CurrentOid );
	
	// 当該フィーチャの表示が始めてなら関連テーブルの情報を登録する
	if( ! hasStationInfo( m_CurrentLayerNo, m_CurrentOid ) )
	{
		// 駅情報の初期化
		SPStationInfo cStationInfo = std::make_shared<CStationInfoController>();
		if( ! cStationInfo->init( GetDlgItem(IDC_GRID_STATION_INFO), GetDlgItem(IDC_GRID_SEARCH_STATION), GetCurrentFeatureDef()->GetFeature() ))
		{
			AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_LAYER_NOT_ENOUGH ), AheLoadString( IDS_ERR_LAYER_READ_ERR ), MB_OK|MB_TASKMODAL, NULL );
			return 0;
		}
		// 駅情報の登録
		m_StationInfoList.insert( std::make_pair(StaObjKey( m_CurrentLayerNo, m_CurrentOid ), cStationInfo ));
	}


	// 編集可否設定
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	esriEditState state = esriStateNotEditing;
	ipEditor->get_EditState( &state );
	m_bEditable = ( state != esriStateNotEditing );


	// 当該フィーチャの路線リスト情報表示
	getCurrentStationInfo()->updateStationInfoGrid();


	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_POLYGON_CLASS) );
	}

	return bRet;
}


// データに変更があったか
BOOL CAttrStationPolygonDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || getCurrentStationInfo()->isChange();
}


// 子ダイアログを実際に作成
void CAttrStationPolygonDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// 詳細ポリゴンのみ
		if( m_strClassName == station_detail::kTableName )
		{
			// ポリゴン種別
			m_cControlRel.SetControl( IDC_COMBO_POLYGON_CLASS, TYPE_COMBO, 0, 0, station_detail::kPolygonType_C, 0, station_detail::kTableName );
		}
	}
}


// ボタンのEnable/Disableの更新、関連テーブルの更新情報の更新
void CAttrStationPolygonDlg::updateChanged()
{
	// 関連テーブルの変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_station_site::kTableName);
	pRelTableDef->setChanged( getCurrentStationInfo()->isChange());

	// ボタン制御
	SetButton( Changed() );
}


// 現在表示中のフィーチャの駅情報レコードを返す
CAttrStationPolygonDlg::SPStationInfo CAttrStationPolygonDlg::getCurrentStationInfo()
{
	auto iter = m_StationInfoList.find( StaObjKey( m_CurrentLayerNo, m_CurrentOid ) );
	_ASSERTE( iter != m_StationInfoList.end() );
	_ASSERTE( iter->second != nullptr );
	
	return iter->second;
}


// 指定のレイヤ番号&レイヤ内OIDのレコードを既にもっているか否か
bool CAttrStationPolygonDlg::hasStationInfo( long lLayerNo, long lLayerOid) const
{
	auto iter = m_StationInfoList.find( StaObjKey( m_CurrentLayerNo, m_CurrentOid ) );
	return ( iter != m_StationInfoList.end() );
}
