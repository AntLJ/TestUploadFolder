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

// AttrCompartLinkDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrCompartLinkDlg.h"
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/registry_util.h>
#include "CompartMap.h"	// 区画線リンクダイアログのウィンドウ配置マップ

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;

const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLink =REGPATH_SINDY_E_CHILDDLG  _T("COMPART_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrCompartLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCompartLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCompartLinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCompartLinkDlg::CAttrCompartLinkDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCompartLinkDlg::CAttrCompartLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_COMPARTLINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// TODO[SiNDYChildDlg] 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_COMPART_LINE_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_COMPART_LINE_C ) )
		( IDC_COMBO_COMPART_STYLE_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_COMPART_STYLE_C ) )
		( IDC_COMBO_SLOWDOWN_LABEL_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SLOWDOWN_LABEL_C) )
		( IDC_CHECK_SLOWDOWN_LABEL_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_SLOWDOWN_LABEL_F ) )
		( IDC_CHECK_INSTALLATION_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_INSTALLATION_F ) )
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		( IDC_TAB_COMPARTATTR, CContainedWindow(  _T("TAB"), this, IDC_TAB_COMPARTATTR ));
}

/** 
 * @fn HWND CAttrCompartLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCompartLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrCompartLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCompartLinkDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCompartLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCompartLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_iChangeType, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// TODO[SiNDYChildDlg] 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	// リストビューを初期化する
	initLaneLink();

	// リストボックスの内容を表示
	if ( !setLaneLink() )
		return 0;

	createGrid();

	// グリッドの編集可能フィールドを設定
	readReg();

	return bRet;
}

/**
 * @fn void CAttrCompartLinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCompartLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// TODO[SiNDYChildDlg] ControlRelにフィールド名とテーブル名を登録
		
		// 線種別コード
		m_cControlRel.SetControl( IDC_COMBO_COMPART_LINE_C, TYPE_COMBO, 0, 0, compart_link::kCompartLineC, 0, compart_link::kTableName );
		// 区画線設置様式コード
		m_cControlRel.SetControl( IDC_COMBO_COMPART_STYLE_C, TYPE_COMBO, 0, 0, compart_link::kCompartStyleC, 0, compart_link::kTableName );
		// 減速表示フラグ
		m_cControlRel.SetControl(IDC_COMBO_SLOWDOWN_LABEL_C, TYPE_COMBO, 0, 0, compart_link::kSlowdownLabelC, 0, compart_link::kTableName );
		// 設置物フラグ
		m_cControlRel.SetControl( IDC_CHECK_INSTALLATION_F, TYPE_CHECKBOX, 0, 0, compart_link::kInstallationF, 0, compart_link::kTableName );
		// 推測コード
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, compart_link::kSperculationC, 0, compart_link::kTableName );
		// 提供元ID
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, compart_link::kSourceID, 0, compart_link::kTableName, 0, -1, FOLLOW_READONLY);
		// Static Source
		m_cControlRel.SetControl( IDC_STATIC_VAR, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, EDITVIEW_KIHON, EDITVIEW_NODE, ipc_table::kSource, 0, compart_link::kTableName );

		// 線種別コード
		m_cControlRel.SetControl( IDC_STATIC_COMPART_LINE_C,		TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 区画線設置様式コード
		m_cControlRel.SetControl( IDC_STATIC_COMPART_STYLE_C,		TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 減速標示コード
		m_cControlRel.SetControl( IDC_STATIC_SLOWDOWN_LABEL_C,		TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 推測コード
		m_cControlRel.SetControl( IDC_STATIC_SPECULATION_C,		    TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 提供元ID
		m_cControlRel.SetControl( IDC_STATIC_SOURCE_ID,		        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 車線リンク
		m_cControlRel.SetControl( IDC_STATIC_LANE_LINK,		        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// List
		m_cControlRel.SetControl( IDC_LIST_LANE_LINK,		        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);		
		// Grid Info
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);
		// Grid
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);

		// 制御する必要のないコントロール
		//m_cControlRel.SetControl( IDC_BUTTON_SEARCH,	TYPE_BUTTON,	0,	0 );
	}
}

// 車線リンクを取得
bool CAttrCompartLinkDlg::getLaneLink()
{
	// 関連テーブルから区画線リンクのGIDに紐づくレコードを取得
	IFeaturePtr currentFeature = GetCurrentFeatureDef()->GetFeature();

	// 区画線リンク－車線リンク関連テーブルのフィーチャクラスを取得
	ITablePtr iRelCompartLinkLaneLinkT = AheOpenSameOwnerTable(
		(ITablePtr)AheGetFeatureClass(currentFeature), rel_compart_link_lane_link::kTableName);
	if(!iRelCompartLinkLaneLinkT) return false;

	// 現在選択されているオブジェクトのGIDを取得する

	// 2016/10/03 GIDが空の場合が考慮されていない為修正
	CComVariant gid =  AheGetAttribute(currentFeature, updown_link::kGlobalID);
	if (gid.vt == VT_NULL)
		return true;

	CString lGID (gid);
	
	// Where句を作成
	CString strWhere( uh::str_util::format(_T("%s='%s'"), rel_compart_link_lane_link::kCompartLinkGID, lGID ) );

	_ICursorPtr iRelCompartLinkLaneLinkCursor;
	iRelCompartLinkLaneLinkT->Search( AheInitQueryFilter( nullptr, nullptr, strWhere),
															VARIANT_FALSE,
															&iRelCompartLinkLaneLinkCursor );

	_IRowPtr relRow;
	std::list<std::string> aLaneLinkGIDs;  // 紐づいている車線リンクのGID
	while( ( S_OK == iRelCompartLinkLaneLinkCursor->NextRow( &relRow ) ) && relRow )
	{
		long sequence = AheGetAttribute( relRow,rel_compart_link_lane_link::kSequence).lVal;
		// メンバ変数に格納

		// 重複しているものがあったら除外(普通は無いはず)
		bool doContinue(false);
		for ( const auto& tmp : m_relCompartLinkLaneLink)
		{		
			if( AheGetAttribute(tmp.second, rel_compart_link_lane_link::kLaneLinkGID) ==  
				AheGetAttribute(relRow, rel_compart_link_lane_link::kLaneLinkGID)
			  )
			{
				doContinue = true;
				break;
			}
		}

		if ( doContinue )
			continue;
		
		m_relCompartLinkLaneLink.insert(std::make_pair(sequence,relRow));
		// ついでに紐づいている車線リンクのGIDも取得しておく
		aLaneLinkGIDs.push_back( uh::toStr(AheGetAttribute( relRow, rel_compart_link_lane_link::kLaneLinkGID).bstrVal ) );
	}

	// IN句でGIDから紐づいている車線リンクをすべて取得
	IFeatureClassPtr iLaneLinkFC = AheOpenSameOwnerTable(
		AheGetTable(GetCurrentFeatureDef()->GetFeature()), lane_link::kTableName);
	if(!iLaneLinkFC) return false;

	uh::ElementsStrings gidStrings = uh::enumerate_elements<std::list<std::string>>( aLaneLinkGIDs, 1000, "'" );

	// 1000ごとにオブジェクトを取得
	for( const auto gidStr : gidStrings )
	{
		CString whereClause = uh::str_util::format( _T("%s IN (%s)"), lane_link::kGlobalID, uh::toTStr(gidStr).c_str() );

		IFeatureCursorPtr iLaneLinkCursor;
		iLaneLinkFC->Search( AheInitQueryFilter( nullptr, nullptr, whereClause), VARIANT_FALSE, &iLaneLinkCursor );

		IFeaturePtr laneLinkFeature;
		while( ( S_OK == iLaneLinkCursor->NextFeature( &laneLinkFeature ) ) && laneLinkFeature )
		{
			long OID(0);
			laneLinkFeature->get_OID(&OID);

			// メンバ変数に車線リンクの形状をセット
			m_laneLinkGeo[OID] = AheGetShape(laneLinkFeature);
			// GID-OIDの変換表を作成
			m_laneLinkGidOid[AheGetAttribute(laneLinkFeature, lane_link::kGlobalID)] = OID;
		}
	}

	return true;
}

// 車線リンクを検索しリストボックスに表示
bool CAttrCompartLinkDlg::setLaneLink()
{
	// 車線リンクを取得
	if (!getLaneLink())
		return false;

	CListViewCtrl lvCtrl(GetDlgItem(IDC_LIST_LANE_LINK));
	lvCtrl.DeleteAllItems();
	int itemCount = lvCtrl.GetItemCount();
	for( const auto& relRec : m_relCompartLinkLaneLink )
	{
		if (!relRec.second)
			continue;

		int index = 0;
		CString laneLinkGID( AheGetAttribute(relRec.second, rel_compart_link_lane_link::kLaneLinkGID) );

		if ( m_laneLinkGidOid.find(laneLinkGID) == m_laneLinkGidOid.end() )
			continue;

		lvCtrl.AddItem( itemCount, 0, uh::str_util::ToString(m_laneLinkGidOid[laneLinkGID]) );
		++itemCount;
	}
	return true;
}

// リストビューを初期化する
void CAttrCompartLinkDlg::initListView()
{
	const UINT COLUMN_MASK = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;  //!< 列の有効メンバフラグ
	const UINT MAX_SIZE = 265;								 //!< 列の最大サイズ(※本当はどこかからとってきたい)

	CListViewCtrl lvCtrl(GetDlgItem(IDC_LIST_LANE_LINK));

	// アイテムを初期化
	lvCtrl.DeleteAllItems();
	// カラム列を初期化
	long lHeaderCount = lvCtrl.GetHeader().GetItemCount();
	for(int i=0; i<lHeaderCount; ++i)
		lvCtrl.DeleteColumn(0);

	lvCtrl.AddColumn( _T("Object ID"), 0, -1, COLUMN_MASK, LVCFMT_LEFT );
	lvCtrl.SetColumnWidth(0, MAX_SIZE);

	// リストビューのスタイルを指定
	// 選択されたときに行全体を選択状態にし、グリッド線を入れる
	DWORD word = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES ;
	lvCtrl.SetExtendedListViewStyle( word , word );
}

// リストビューを初期化する
void CAttrCompartLinkDlg::initLaneLink()
{
	m_laneLinkGeo.clear();
	m_laneLinkGidOid.clear();
	m_ipSelectedLine = nullptr;
	m_relCompartLinkLaneLink.clear();
}

// 車線リンクリストをクリックしたときの処理
LRESULT CAttrCompartLinkDlg::OnClickSearchResult(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	CListViewCtrl lv( GetDlgItem(IDC_LIST_LANE_LINK) );
	// 選択中のレコードのオブジェクトIDを取得
	int nCurIdx = lv.GetSelectedIndex();
	if( -1 == nCurIdx ) return 0;

	TCHAR buffer[256];
	lv.GetItemText(nCurIdx, 0,  buffer, sizeof(buffer) / sizeof(TCHAR));
	CString strOID(buffer);
	if(strOID.IsEmpty()) return 0;

	// メンバ変数にセット
	m_ipSelectedLine = m_laneLinkGeo[_ttoi(strOID)];

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	return 0;
}

// 再描画
LRESULT CAttrCompartLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 編集中のLane_LINKの形状を緑色で描画する
	if(m_ipSelectedLine)
		AheDrawPolyline(m_cArcHelper.GetDisplay(), m_ipSelectedLine, RGB(0,255,50), 5, esriSLSSolid, TRUE, TRUE);

	highlightLine();
	return 0;
}

// ウィンドウマネージャのマップの切り替えを行う
void CAttrCompartLinkDlg::ChangeWindowMgrMap( int iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_KIHON:		// 基本属性タブ
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_NODE:		// ノード列属性タブ
		m_winMgr.ChangeMgrMap( NODEMAP );
		break;	
	default:
		break;
	}
	// 再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}

// グリッドを生成する
void CAttrCompartLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable)	return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_compart_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// 始めてダイアログに表示したフィーチャであれば、ノード列グリッドを登録
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_compart_node::kObjectID)
			.addField(nq_compart_node::kSequence)
			.addField(nq_compart_node::kWidth)
			.addField(nq_compart_node::kCompartNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// グリッド初期化
	m_nqDataGridMap[m_currentOid].init();

	// ノード列データをグリッドへ読み込む
	CComVariant linkGid = AheGetAttribute(ipFeature, compart_link::kGlobalID);
	if (linkGid.vt == VT_NULL) return;

	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_compart_node::kCompartLinkGID, linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrCompartLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// グリッド列ごとの編集可否設定はグリッドクラス側へ委譲
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

// MouseUp Event on Grid
void CAttrCompartLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_gridSelect = false;
	// クリック
	if ((!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) && Button == 1 )
	{
		m_gridSelect = true;
		m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
	}
	else if(IsEditableFeature() && m_cArcHelper.IsStartEditing() && (Button == 1 || Button == 2))
	{
		m_gridSelect = true;
		m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);	
		// 右クリックならメニューを出す
		if(Button == 2) gridMenu( IDC_GRID_NODELIST, m_nqCompartNodeGrid, X, Y );
	}
}

// After Edit
void CAttrCompartLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// 関連テーブルの更新情報の更新
void CAttrCompartLinkDlg::updateChanged()
{
	// 関連テーブル(ノード列)の変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// ボタン制御
	SetButton(Changed());
}

// 右クリックすると <削除> ポップアップ
void CAttrCompartLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
{
	long selectedRow = ipGrid->GetSelectedRow(0);

	// カーソル位置取得
	POINT stPos = {0,0};
	::GetCursorPos( &stPos);

	CMenu menu;
	menu.LoadMenuW( IDR_MENU );
	CMenu popupMenu = menu.GetSubMenu(0);

	if(selectedRow <= 0) popupMenu.EnableMenuItem(ID_DEL, MF_GRAYED |MF_BYCOMMAND |MF_DISABLED);

	// Popupメニュー表示
	int nResult = popupMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, stPos.x, stPos.y, m_hWnd, 0);

	if(nResult == ID_DEL && MessageBox(_T("削除しますか？"),_T("確認"), MB_OKCANCEL) == IDOK)
	{
		CComBSTR bstrValue;
		ipGrid->get_TextMatrix(_variant_t(selectedRow), _variant_t(0L), &bstrValue);

		CString whereClause = uh::str_util::format(_T("%s=%s"),
			nq_compart_node::kObjectID, bstrValue);

		CString objectID = bstrValue.m_str;

		// 以下のif分は、新規レコードの場合、OIDをRowDataに入れて管理するための処理である
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_compart_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}		

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);

		m_nqDataGridMap[m_currentOid].highlightChangedCell();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
		pRelTableDef->setChanged(changeFlag);

		// ボタン制御
		SetButton(changeFlag);
	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
		pRelTableDef->setChanged(true);

		// ボタン制御
		SetButton(true);
	}
}

// Update Grid Data
bool CAttrCompartLinkDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), compart_link::kTableName) != 0)	continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged()) continue;

		if (!m_nqDataGridMap[oid].store()) return false;
	}
	return true;
}

// Check Data Changes
bool CAttrCompartLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrCompartLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// ダイアログで表示している区画線リンクフィーチャを取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// グリッドで選択列のノード列レコードから、"順番","区画線リンクGID"を取得する
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNodeT = AheOpenSameOwnerTable(ipTable, compart_node::kTableName);
	if (!ipNodeT) return;

	CString nodeGid, nodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(nodeGid, nodeGidNext, nq_compart_node::kSequence, nq_compart_node::kCompartNodeGID);

	//   検索されたノード列のノードGIDから次のノードを検索
	CString whereClause = uh::str_util::format(_T("%s='%s'"), compart_node::kGlobalID, nodeGid);
	_ICursorPtr ipNodeCur;
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	IFeaturePtr ipNodeFeature;
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), compart_node::kGlobalID, nodeGidNext);
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipToPoint);
}

// グリッドで選択しているノード列のラインをハイライトする
void CAttrCompartLinkDlg::highlightLine()
{
	if (m_nqDataGridMap.empty()) return;

	// グリッド選択行のノード列レコードの次のノードを取得する
	//   ノード列から次のノードにあたるレコードを検索
	IGeometryPtr ipFromPoint, ipToPoint;
	getSequenceGeometry(ipFromPoint, ipToPoint);
	
	if(!ipFromPoint) return;

	ISpatialReferencePtr ipSpatialReference;
	ipFromPoint->get_SpatialReference(&ipSpatialReference);

	IPolylinePtr ipDrawLine(CLSID_Polyline);
	ipDrawLine->putref_SpatialReference(ipSpatialReference);
	ipDrawLine->put_FromPoint(IPointPtr(ipFromPoint));
	ipDrawLine->put_ToPoint(IPointPtr(ipToPoint));

	moveToCenter(ipFromPoint, ipToPoint);

	// ライン描画（描画中のラインは消す)
	AheDrawPolyline(m_cArcHelper.GetDisplay(), ipDrawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);	
}

// Tear down connection points
LRESULT CAttrCompartLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// ノード列のジャンプ,中心へ移動
void CAttrCompartLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
{
	// 中心へ移動
	IActiveViewPtr ipActiveView = m_cArcHelper.GetActiveView();
	
	if (ipActiveView && m_gridSelect && ipFromPoint) {
		m_gridSelect = false;
		IEnvelopePtr ipEnvelope;
		
		// Pointの座標
		double fromX(0), fromY(0), toX(0), toY(0);

		IPointPtr(ipFromPoint)->QueryCoords(&fromX, &fromY);
		if(ipToPoint)
		{
			IPointPtr(ipToPoint)->QueryCoords(&toX, &toY);
		}
		else
		{
			toX = fromX;
			toY = fromY;
		}

		ISpatialReferencePtr ipSpatialRefer;
		ipFromPoint->get_SpatialReference(&ipSpatialRefer);

		IPointPtr ipPoint( CLSID_Point );
		ipPoint->putref_SpatialReference(ipSpatialRefer);
		ipPoint->PutCoords( (fromX + toX)/2, (fromY + toY)/2 );

		ipActiveView->get_Extent( &ipEnvelope );
		ipEnvelope->CenterAt(ipPoint);
		ipActiveView->put_Extent( ipEnvelope );

		ipActiveView->Refresh();		
	}
}

// レジストリからデータを読み込
void CAttrCompartLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID フィールドは指定されても変更不可
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_compart_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
