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

// AttrRailwayNodeDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrRailwayNodeDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;
using namespace sindy::schema::sj;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(NODE_MAP)
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_STATIC_NODE_C)
				RCTOFIT(IDC_COMBO_NODE_C)
				//RCREST(-1)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_STATIC_RAILWAY_LIST)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_PCT, 0, 30)
				RCREST(IDC_GRID_LINE_INFO)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCPERCENT(IDC_STATIC_STATION_LIST, 30)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_PCT, 0, 40)
				RCREST(IDC_GRID_AROUND_STA)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCTOFIT(IDC_BUTTON_APPLY)
				//RCREST(-1)
			ENDGROUP()
			RCSPACE(5)
		ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrRailwayNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg2 * __stdcall CreateDlg()
{
	return new CAttrRailwayNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrRailwayNodeDlg::CAttrRailwayNodeDlg() 
	: ATTR_BASE_CLASS(NODE_MAP, IDD_RAILWAYNODEDLG),
	  m_CurrentOid(-1)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODE_C, CContainedWindow(  _T("COMBO") , this, IDC_COMBO_NODE_C ) ),
		( IDC_BUTTON_APPLY, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_APPLY ) )
		;
}


/**
 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
 */
LRESULT CAttrRailwayNodeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CAttrRailwayNodeDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		// ダイアログを更新領域に追加します
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}

	return 0;
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
HWND CAttrRailwayNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// 「適用」ボタンが押された時の処理
LRESULT CAttrRailwayNodeDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_LineInfoList[m_CurrentOid].addCheckedRecord();
	updateChanged();

	return 0;
}


// グリッド右クリックのメニューから「削除」ボタンが押されたときの処理
LRESULT CAttrRailwayNodeDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_LineInfoList[m_CurrentOid].deleteSelectedRow();
	updateChanged();

	return 0;
}


// 路線リストグリッド編集前の処理
void __stdcall CAttrRailwayNodeDlg::OnLineInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_LineInfoList[m_CurrentOid].OnLineInfoGridBeforeEdit(Row, Col, Cancel);
}


// 路線リストグリッド編集後の処理
void __stdcall CAttrRailwayNodeDlg::OnLineInfoAfterEdit(LONG Row, LONG Col)
{ 
	m_LineInfoList[m_CurrentOid].OnLineInfoGridAfterEdit(Row, Col);
	updateChanged();
}


// 路線リストグリッドマウスアップ時の処理
void __stdcall CAttrRailwayNodeDlg::OnLineInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{ 
	m_LineInfoList[m_CurrentOid].OnLineInfoGridMouseUp(Button, Shift, X, Y, m_hWnd, IDR_MENU_LINE_INFO_GRID);
}


// 周辺駅リストグリッド編集前の処理
void __stdcall CAttrRailwayNodeDlg::OnAroundStaBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{ 
	m_LineInfoList[m_CurrentOid].OnAroundStaBeforeEdit(Row, Col, Cancel);
}


// OKボタンを押された後に呼び出される
BOOL CAttrRailwayNodeDlg::ErrorCheck()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_node::kTableName ) != 0 )
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );
		
		// 関連テーブルが読み込まれていない(ツリーに存在するが開かれていない)フィーチャは
		// 関連テーブルを読み込んでからチェックする
		if( m_LineInfoList.find(lOid) == m_LineInfoList.end() )
		{
			CLineInfoController cLICtlr;
			cLICtlr.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_AROUND_STA), GetCurrentFeatureDef()->GetFeature(), m_cArcHelper );
			m_LineInfoList.insert(std::make_pair(lOid, cLICtlr));
		}

		for( auto fieldDef : featureDef )
		{
			if( fieldDef.m_strFieldName != sj::railway_node::kNodeClass_C )
				continue;

			// ノード種別と関連テーブルの整合性チェック
			if( ! checkNodeClass( m_LineInfoList[lOid], fieldDef.m_vaValue , lOid) )
				return FALSE;
		}
	}

	return TRUE;
}


// 各種エラーチェック後に呼び出される
bool CAttrRailwayNodeDlg::Update()
{
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), sj::railway_node::kTableName ) != 0 )
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// 関連テーブルに変更があれば更新
		if( ! m_LineInfoList[lOid].isChange() )
			continue;

		if( ! m_LineInfoList[lOid].updateTable() )
			return false;
	}
	return true;
}


// ノード種別と関連テーブルの整合成チェック
bool CAttrRailwayNodeDlg::checkNodeClass( const CLineInfoController& cLineInfoCntr, const CComVariant& vaNodeType, long lOid )
{
	// ノード種別と関連テーブルの整合性チェック
	if( vaNodeType.lVal == sj::railway_node::node_class::kStation )
	{
		if( cLineInfoCntr.isEnptyRelData() )
		{	
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_LINEINFO_IS_NOT_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	else
	{
		if( ! cLineInfoCntr.isEnptyRelData() )
		{
			CString strErr = uh::str_util::format( _T("( ID : %ld ) %s"), lOid, AheLoadString(IDS_ERR_LINEINFO_IS_REGISTERED));
			AheMessageBox( NULL, strErr, AheLoadString( IDS_ERR_CONSISTENCY_CHECK ), MB_OK|MB_TASKMODAL, NULL );
			return false;
		}
	}
	return true;
}


/**
 * @brief 作成した子ダイアログを削除します。
 *
 * @warning  ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrRailwayNodeDlg::Delete()
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
BOOL CAttrRailwayNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
	if( m_LineInfoList.find( m_CurrentOid ) == m_LineInfoList.end() )
	{
		CLineInfoController cLineInfoGrid;
		// 路線リストグリッドの初期化
		if( ! cLineInfoGrid.init( GetDlgItem(IDC_GRID_LINE_INFO), GetDlgItem(IDC_GRID_AROUND_STA), 
							GetCurrentFeatureDef()->GetFeature(), m_cArcHelper ) )
		{
			AheMessageBox( NULL, AheLoadString( IDS_ERR_TOC_LAYER_NOT_ENOUGH ), AheLoadString( IDS_ERR_LAYER_READ_ERR ), MB_OK|MB_TASKMODAL, NULL );
			return 0;
		}
		m_LineInfoList.insert( std::make_pair(m_CurrentOid, cLineInfoGrid ));
	}

	// グリッドの編集可否は個別に制御
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	esriEditState state = esriStateNotEditing;
	ipEditor->get_EditState( &state );
	if( state == esriStateNotEditing )
		m_LineInfoList[m_CurrentOid].disable();

	// 選択しているフィーチャーの形状を取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	// 周辺の駅情報を検索してリストビューへ表示
	m_LineInfoList[m_CurrentOid].searchStationInfo(ipFeature, ipLayer);
	m_LineInfoList[m_CurrentOid].updateLineInfoGrid();

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_BUTTON_APPLY) );
	}

	// 最初にフォーカスされるコントロール
	::SetFocus( GetDlgItem(IDC_COMBO_NODE_C) );

	return bRet;
}


// データに変更があったか
BOOL CAttrRailwayNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_LineInfoList[m_CurrentOid].isChange();
}


/**
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrRailwayNodeDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ノード種別
		m_cControlRel.SetControl( IDC_COMBO_NODE_C, TYPE_COMBO, 0, 0, railway_node::kNodeClass_C, 0, railway_node::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, railway_node::kTableName );
		// 適用ボタン
		m_cControlRel.SetControl( IDC_BUTTON_APPLY,	TYPE_BUTTON, 	0, 0 );
	}
}


/**
* @brief TOCから指定レイヤを取得
* @param layerName [in]     TOCから取得するレイヤ名
* @param stCount   [in,out] TOC上に存在するレイヤ名の数を取得
* @param ipMap     [in]     IMapPtr
*/
ILayerPtr CAttrRailwayNodeDlg::getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap )
{
	// TOCからlayerNameを取得
	CMap cMap( ipMap );
	// TOC上のlayerNameの数をカウント
	stCount = 0;
	ILayerPtr ipLayer;
	for( auto& it : cMap )
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
void CAttrRailwayNodeDlg::updateChanged()
{
	// 関連テーブルの変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(sj::rel_railway_node::kTableName);
	pRelTableDef->setChanged( m_LineInfoList[m_CurrentOid].isChange());

	// ボタン制御
	SetButton( Changed() );
}
