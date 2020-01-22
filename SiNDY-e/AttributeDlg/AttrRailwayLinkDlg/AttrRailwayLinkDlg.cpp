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

// AttrRailwayLinkDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrRailwayLinkDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;
using namespace sindy::schema::sj;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(LINK_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_RAILWAY_CLASS)
			RCTOFIT(-1)
			RCSPACE(4)
			RCTOFIT(IDC_COMBO_RAILWAY_CLASS)
			RCSPACE(30)
			RCTOFIT(IDC_UNDERGROUND_F)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_DISPSCALE)
			RCTOFIT(-1)
			RCSPACE(4)
			RCTOFIT(IDC_COMBO_DISPSCALE)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_RAILWAY_LIST)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 25)
			RCREST(IDC_GRID_LINE_INFO)
		ENDGROUP()
		RCSPACE(15)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(2,2))
			RCTOFIT(IDC_STATIC_RAILWAY_SEARCH)
			RCSPACE(8)
			RCTOFIT(IDC_STATIC_STATION_NAME)
			RCSPACE(4)
			RCTOFIT(IDC_EDIT_STATION_SEARCH)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_STATION_SEARCH)
			RCSPACE(4)
			RCREST(0)
			RCTOFIT(IDC_BUTTON_AROUND_SEARCH)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 30)
			RCREST(IDC_GRID_SEARCH_RESULT)
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
 * @retval CAttrRailwayLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg2 * __stdcall CreateDlg()
{
	return new CAttrRailwayLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayLinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrRailwayLinkDlg::CAttrRailwayLinkDlg() 
	: ATTR_BASE_CLASS(LINK_MAP, IDD_RAILWAYLINKDLG),
	  m_CurrentOid(-1),
	  m_bIsLinkOnMesh(false),
	  m_bEditable(false)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_RAILWAY_CLASS, CContainedWindow(  _T("COMBO")    , this, IDC_COMBO_RAILWAY_CLASS ) ),
		( IDC_COMBO_DISPSCALE    , CContainedWindow(  _T("COMBO")    , this, IDC_COMBO_DISPSCALE ) ),
		( IDC_UNDERGROUND_F      , CContainedWindow(  _T("CHECKBOX") , this, IDC_UNDERGROUND_F ) ),
		( IDC_BUTTON_APPLY       , CContainedWindow(  _T("BUTTON")   , this, IDC_BUTTON_APPLY ) )
		;
}

/**
 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
 */
LRESULT CAttrRailwayLinkDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

/**
 * WM_COMMAND 用イベントハンドラ
 *
 * 各コモンコントロールで変更があった場合、ここで処理します
 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
 */
LRESULT CAttrRailwayLinkDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if( !m_bFinishInit ) return 0;

	INT msg = HIWORD(wParam);						// 操作メッセージ
	INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

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
		// 2次メッシュ境界を跨いで反対側の接続リンクを更新
		syncLinkAttrOnMesh();
		// ダイアログを更新領域に追加します
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}

	return 0;
}


// 「適用」ボタンが押された時の処理
LRESULT CAttrRailwayLinkDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_RailwayLineList[m_CurrentOid].addCheckedData();
	syncRelTableOnMesh();

	m_RailwayLineList[m_CurrentOid].updateRailwayLineGrid();
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();
	updateChanged();

	return 0;
}


// 駅名で検索ボタンが押されたときの処理
LRESULT CAttrRailwayLinkDlg::OnClickStaSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// エディットボックスの文字列の取得
	CWindow edit( GetDlgItem( IDC_EDIT_STATION_SEARCH ) );

	// 値を取得
	CString strStationName;
	edit.GetWindowText(strStationName);

	// 検索モード変更
	m_RailwayLineList[m_CurrentOid].setSearchMode(search_railway_grid::kStarionNameSearch);
	// 駅名で検索 + グリッド更新
	m_RailwayLineList[m_CurrentOid].searchStaName(strStationName);
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();

	return 0;
}


// 周辺駅の路線検索ボタンが押されたときの処理
LRESULT CAttrRailwayLinkDlg::OnClickAroundSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 検索モード変更
	m_RailwayLineList[m_CurrentOid].setSearchMode(search_railway_grid::kAroundSearch);
	// グリッド更新
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();

	return 0;

}


// グリッド右クリックのメニューから「削除」ボタンが押されたときの処理
LRESULT CAttrRailwayLinkDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_RailwayLineList[m_CurrentOid].deleteSelectedRow();
	syncRelTableOnMesh();

	// グリッド更新
	m_RailwayLineList[m_CurrentOid].updateRailwayLineGrid();
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();

	updateChanged();

	return 0;
}


// 再描画
LRESULT CAttrRailwayLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ハイライト
	m_RailwayLineList[m_CurrentOid].highlightFeatures();

	return 0;
}


// 路線リストグリッド編集前の処理
void __stdcall CAttrRailwayLinkDlg::OnLineInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_RailwayLineList[m_CurrentOid].OnRailwayLineGridBeforeEdit( Row, Col, Cancel);

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}


// 路線リストグリッドマウスアップ時の処理
void __stdcall CAttrRailwayLinkDlg::OnLineInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	m_RailwayLineList[m_CurrentOid].OnLineInfoGridMouseUp(Button, Shift, X, Y, m_hWnd, IDR_MENU_LINE_INFO_GRID, m_bEditable );

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}


// 路線検索グリッド編集前の処理
void __stdcall CAttrRailwayLinkDlg::OnRailwaySearchBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_RailwayLineList[m_CurrentOid].OnSearchRailwayBeforeEdit( Row, Col, Cancel, m_bEditable);

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
}


/** 
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrRailwayLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// OKボタンを押された後に呼び出される
BOOL CAttrRailwayLinkDlg::ErrorCheck()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_link::kTableName ) != 0 )
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// 関連テーブルが読み込まれていない(ツリーに存在するが開かれていない)フィーチャは
		// 関連テーブルを読み込んでからチェックする
		if( m_RailwayLineList.find(lOid) == m_RailwayLineList.end() )
		{
			CRailwayLineController cRLCtlr;
			cRLCtlr.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_SEARCH_RESULT), GetCurrentFeatureDef()->GetFeature(), m_cArcHelper );
			m_RailwayLineList.insert(std::make_pair(lOid, cRLCtlr));
		}

		for( const auto& fieldDef : featureDef )
		{
			if( fieldDef.m_strFieldName != sj::railway_link::kRailwayClass_C )
				continue;

			// ノード種別と関連テーブルの整合性チェック
			if( ! checkLinkClass( m_RailwayLineList[lOid], fieldDef.m_vaValue , lOid) )
				return FALSE;
		}
	}

	return TRUE;
}


// 各種エラーチェック後に呼び出される 
bool CAttrRailwayLinkDlg::Update()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_link::kTableName ) != 0 )
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// 関連テーブルに変更があれば更新
		if( ! m_RailwayLineList[lOid].isChange() )
			continue;

		if( ! m_RailwayLineList[lOid].updateTable() )
			return false;
	}
	return true;
}


/**
 * @fn void CAttrRailwayLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrRailwayLinkDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}


/**
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrRailwayLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// TOCからSTATION_POINTを取得
	// 2つ以上存在するならエラー
	int stCount = 0;
	ILayerPtr ipLayer = getLayer( station_point::kTableName, stCount, m_cArcHelper.GetFocusMap() );
	if( stCount >= 2 )
	{
		AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_HAS_TWO_STATION_POINT ), AheLoadString( IDS_ERR_FAILED_SPECIFIC_STATION_POINT ), MB_OK|MB_TASKMODAL, NULL );
		return 0;
	}

	GetCurrentFeatureDef()->GetFeature()->get_OID( &m_CurrentOid );
	// 当該フィーチャの表示が初めてなら関連テーブルグリッドの情報を登録する
	if( m_RailwayLineList.find( m_CurrentOid ) == m_RailwayLineList.end() )
	{
		CRailwayLineController cLineInfoGrid;
		// 路線リストグリッドの初期化
		if( ! cLineInfoGrid.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_SEARCH_RESULT), 
							GetCurrentFeatureDef()->GetFeature(), m_cArcHelper ))
		{
			AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_LAYER_NOT_ENOUGH ), AheLoadString( IDS_ERR_LAYER_READ_ERR ), MB_OK|MB_TASKMODAL, NULL );
			return 0;
		}
		m_RailwayLineList.insert( std::make_pair(m_CurrentOid, cLineInfoGrid ));
	}

	// 2次メッシュを跨いでいる鉄道リンクを取得
	m_LinkOnMeshList.clear();
	m_bIsLinkOnMesh = getLinkOnMeshFeatureDefs(m_LinkOnMeshList);

	// 編集可否設定
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	esriEditState state = esriStateNotEditing;
	ipEditor->get_EditState( &state );
	m_bEditable = ( state != esriStateNotEditing );

	// 選択しているフィーチャーの形状を取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	// 周辺の駅情報を検索してリストビューへ表示
	m_RailwayLineList[m_CurrentOid].searchStationInfo(ipFeature, ipLayer);
	m_RailwayLineList[m_CurrentOid].updateSearchRailwayGrid();
	// 当該フィーチャの路線リスト情報表示
	m_RailwayLineList[m_CurrentOid].updateRailwayLineGrid();

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_COMBO_RAILWAY_CLASS) );
	}

	return bRet;
}


// データに変更があったか
BOOL CAttrRailwayLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_RailwayLineList[m_CurrentOid].isChange();
}


/**
 * @fn void CAttrRailwayLinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrRailwayLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( ! m_cControlRel.empty() )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 鉄道種別
		m_cControlRel.SetControl( IDC_COMBO_RAILWAY_CLASS, TYPE_COMBO, 0, 0, railway_link::kRailwayClass_C, 0, railway_link::kTableName );
		// 表示スケール種別
		m_cControlRel.SetControl( IDC_COMBO_DISPSCALE, TYPE_COMBO, 0, 0, railway_link::kDispScale_C, 0, railway_link::kTableName );
		// 地下フラグ
		m_cControlRel.SetControl( IDC_UNDERGROUND_F, TYPE_CHECKBOX, 0, 0, railway_link::kUnderGround_F, 0, railway_link::kTableName );
		// 適用ボタン
		m_cControlRel.SetControl( IDC_BUTTON_APPLY,	TYPE_BUTTON, 	0,	0 );
	}
}


// TOCから指定レイヤを取得
ILayerPtr CAttrRailwayLinkDlg::getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap )
{
	// TOCからlayerNameを取得
	CMap cMap( ipMap );
	// TOC上のlayerNameの数をカウント
	stCount = 0;
	ILayerPtr ipLayer;
	for( const auto& it : cMap )
	{
		if( AheGetFeatureClassName( it ).Compare( layerName ) == 0 )
		{
			ipLayer = it;
			++stCount;
		}
	}
	return ipLayer;
}


// ボタンのEnable/Disableの更新、関連テーブルの更新情報の更新
void CAttrRailwayLinkDlg::updateChanged()
{
	// 関連テーブルの変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(sj::rel_railway_link::kTableName);
	pRelTableDef->setChanged( m_RailwayLineList[m_CurrentOid].isChange());

	// ボタン制御
	SetButton( Changed() );
}


// 鉄道リンク種別と関連テーブルの整合成チェック
bool CAttrRailwayLinkDlg::checkLinkClass(const CRailwayLineController& cRailLineCntr, const CComVariant& vaLinkType, long lOid)
{
	using namespace sj::railway_link;

	if( vaLinkType.lVal == link_class::kDivideByPoi )
	{
		if( cRailLineCntr.isEmptyRelData() )
		{	
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_RAILWAYLINE_IS_NOT_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	// 鉄道側線部は路線が割り当てられない
	else if( vaLinkType.lVal == link_class::kSideTrack )
	{
		if( ! cRailLineCntr.isEmptyRelData() )
		{
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_RAILWAYLINE_IS_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	return true;
}


// メッシュ境界を跨ぐフィーチャの属性をコピーする  
void CAttrRailwayLinkDlg::syncLinkAttrOnMesh()
{
	using namespace sj::railway_link;

	if ( ! m_bIsLinkOnMesh )
		return;

	// 現在保持しているリストすべてにおいてループ
	for( auto iter = m_pFeatureDefList->begin(); iter != m_pFeatureDefList->end(); ++iter )
	{
		IFeaturePtr temp;
		long lOid = -1;
		temp = iter->GetFeature();
		temp->get_OID( &lOid );


		// 現在選択中の2次メッシュを跨いでいる道路リンクに関連しているリストとの照合
		CFeatureDef* pFeatureDef = nullptr;
		for( auto feature = m_LinkOnMeshList.begin(); feature != m_LinkOnMeshList.end(); ++feature )
		{
			long lMeshOid = -1;
			feature->GetFeature()->get_OID( &lMeshOid );
			if( lOid == lMeshOid )
			{
				pFeatureDef = &(*iter);
				break;
			}
		}
		// メッシュ跨ぎ鉄道リンクが取得できなければ、何もしない
		if( ! pFeatureDef )
			continue;


		// すべてのフィールドでループ
		for( auto field = GetCurrentFeatureDef()->begin(); field != GetCurrentFeatureDef()->end(); ++field )
		{
			CString strFieldName = field->m_strFieldName;

			// コピー対象は鉄道種別、スケール表示種別、地下フラグのみ
			if( strFieldName != kRailwayClass_C && strFieldName != kDispScale_C && strFieldName != kUnderGround_F )
				continue;

			// 対象フィールドの属性をコピー
			CFieldDef *pFieldDef = pFeatureDef->GetFieldDef( kTableName, field->m_strFieldName );
			CComVariant vaValue;
			pFeatureDef->GetFeature()->get_Value( field->m_lFieldIndex, &vaValue );
			if( field->m_vaValue != vaValue )
			{
				pFieldDef->m_vaValue = field->m_vaValue;
				pFieldDef->m_bChanged = TRUE;
			}
			else{
				pFieldDef->m_vaValue = field->m_vaValue;
				pFieldDef->m_bChanged = FALSE;
			}
		}
	}
}


// メッシュ境界を跨ぐフィーチャの関連テーブルを同期する
void CAttrRailwayLinkDlg::syncRelTableOnMesh()
{
	if ( ! m_bIsLinkOnMesh )
		return;

	// 現在保持しているリストすべてにおいてループ
	for( auto iter = m_pFeatureDefList->begin(); iter != m_pFeatureDefList->end(); ++iter )
	{
		IFeaturePtr temp;
		long lOid = -1;
		temp = iter->GetFeature();
		temp->get_OID( &lOid );

		// 現在選択中の2次メッシュを跨いでいる道路リンクに関連しているリストとの照合
		CFeatureDef* pFeatureDef = nullptr;
		for( auto feature = m_LinkOnMeshList.begin(); feature != m_LinkOnMeshList.end(); ++feature )
		{
			long lMeshOid = -1;
			feature->GetFeature()->get_OID( &lMeshOid );
			if( lOid == lMeshOid )
			{
				pFeatureDef = &(*iter);
				break;
			}
		}
		// メッシュ跨ぎ鉄道リンクが取得できなければ、何もしない
		if( ! pFeatureDef )
			continue;

		// コピー先のOID取得
		long lTargetOid = -1;
		pFeatureDef->GetFeature()->get_OID( &lTargetOid );

		// コピー元のOID取得
		long lCurrentOid = -1;
		GetCurrentFeatureDef()->GetFeature()->get_OID( &lCurrentOid );

		// 関連テーブルの同期
		m_RailwayLineList[lCurrentOid].syncRailwayLineData(m_RailwayLineList[lTargetOid]);
		// 関連テーブルの変更を通知
		CRelationalTableDef* pRelTableDef = pFeatureDef->GetRelationalTableDef(sj::rel_railway_link::kTableName);
		pRelTableDef->setChanged( m_RailwayLineList[lTargetOid].isChange());
	}
}


// メッシュ境界を跨ぐフィーチャ群を取得する
bool CAttrRailwayLinkDlg::getLinkOnMeshFeatureDefs( std::list<CFeatureDef>& cMeshCrossedLinks )
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cMeshCrossedLink;

	// メッシュを跨いでいる先の鉄道リンクを取得する
	if( AheIsNetworkLinkCrossMesh( m_cArcHelper.GetMap(), ipCurrentFeature, cMeshCrossedLink ) )
	{
		for( auto& iter = cMeshCrossedLink.begin(); iter != cMeshCrossedLink.end(); ++iter )
		{
			// メッシュを跨いだ先の鉄道リンクの属性を格納
			IFeaturePtr ipFeatureOnMesh = iter->GetFeature();
			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			cMeshCrossedLinks.push_back(featDef);

			// メッシュを跨いだ先の鉄道リンク紐付けテーブルを取得
			CRailwayLineController cLineInfoCtlr;
			cLineInfoCtlr.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_SEARCH_RESULT), 
				_IRowPtr(ipFeatureOnMesh), 
				m_cArcHelper );
			long lOid = -1;
			ipFeatureOnMesh->get_OID( &lOid );
			m_RailwayLineList.insert( std::make_pair(lOid, cLineInfoCtlr ));
		}
	}

	return ! cMeshCrossedLinks.empty();
}
