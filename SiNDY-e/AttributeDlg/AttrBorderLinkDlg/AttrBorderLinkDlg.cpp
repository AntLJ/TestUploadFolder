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

// AttrBorderLinkDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrBorderLinkDlg.h"
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/registry_util.h>

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;


#include "WinMap.h"

const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLink =REGPATH_SINDY_E_CHILDDLG  _T("BORDER_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrBorderLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrBorderLinkDlg();
}

CAttrBorderLinkDlg::CAttrBorderLinkDlg() 
	: ATTR_BASE_CLASS(BORDERLINK_MAP, IDD_BORDERLINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		(IDC_EDIT_SOURCE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE))
		(IDC_COMBO_SPECULATION_C, CContainedWindow(_T("COMBO"), this, IDC_COMBO_SPECULATION_C))
		(IDC_CHECK_IMAGINARY_BODER_F, CContainedWindow(_T("CHECKBOX"), this, IDC_CHECK_IMAGINARY_BODER_F))
		(IDC_EDIT_SOURCE_ID, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		;
}

LRESULT CAttrBorderLinkDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// コントロール
	CreateControlRelation();

	// ノード列グリッドコントロール
	CAxWindow wndGrid = GetDlgItem(IDC_GRID_NODELIST);
	wndGrid.QueryControl(&m_nqBorderNodeGrid);

	// グリッドコントロールからのイベントを拾うのに必要
	AtlAdviseSinkMap(this, true);

	// サブクラス化
	for (auto& it : m_mapSubClass) {
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	return ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrBorderLinkDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if (!m_bFinishInit) return 0;

	INT msg = HIWORD(wParam);						// 操作メッセージ
	INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

	// エディットボックスの時に全選択にする
	if (msg == EN_SETFOCUS)
		SelectDlgItem(nTargetControl);
	// コンボボックスのリストボックスの長さを調節
	if (msg == CBN_DROPDOWN)
		SetComboboxList(nTargetControl);

	// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
	if ((msg == CBN_SELCHANGE) || (msg == BN_CLICKED) || (msg == EN_UPDATE)) {
		// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
		m_cControlRel.ControlChanged((INT)LOWORD(wParam));
		// ダイアログを更新領域に追加します
		InvalidateRect(NULL, FALSE);
		// 変更された場合はボタンを Enable に
		SetButton(Changed());
	}

	return 0;
}

/** 
 * @fn HWND CAttrBorderLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrBorderLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrBorderLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrBorderLinkDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrBorderLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBorderLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable ) {
		::SetFocus( GetDlgItem(IDC_COMBO_SPECULATION_C) );
	}

	createGrid();

	// グリッドの編集可能フィールドを設定
	readReg();

	return bRet;
}

/**
 * @fn void CAttrBorderLinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrBorderLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 推測コード
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, border_link::kSperculationC, 0, border_link::kTableName);
		// 提供元ID
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, border_link::kSourceID, 0, border_link::kTableName, 0, -1, FOLLOW_READONLY);
		// 仮想線フラグ
		m_cControlRel.SetControl(IDC_CHECK_IMAGINARY_BODER_F, TYPE_CHECKBOX, 0, 0, border_link::kImaginaryBorderF, 0, border_link::kTableName);
		// 情報ソース
		m_cControlRel.SetControl(IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, border_link::kSource, 0, border_link::kTableName);

		// Grid Info
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	0, 0);
		// Grid
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	0, 0);
	}
}


// 再描画
LRESULT CAttrBorderLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	highlightLine();
	return 0;
}

// グリッドを生成する
void CAttrBorderLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_border_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// 始めてダイアログに表示したフィーチャであれば、ノード列グリッドを登録
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_border_node::kObjectID)
			.addField(nq_border_node::kSequence)
			.addField(nq_border_node::kBorderNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// グリッド初期化
	m_nqDataGridMap[m_currentOid].init();

	// ノード列データをグリッドへ読み込む
	CComVariant linkGid = AheGetAttribute(ipFeature, border_link::kGlobalID);
	if (linkGid.vt == VT_NULL) return;

	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_border_node::kBorderLinkGID, linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrBorderLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
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
void CAttrBorderLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
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
		if(Button == 2)	gridMenu( IDC_GRID_NODELIST, m_nqBorderNodeGrid, X, Y );
	}
}

// After Edit
void CAttrBorderLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// 関連テーブルの更新情報の更新
void CAttrBorderLinkDlg::updateChanged()
{
	// 関連テーブル(ノード列)の変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_border_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// ボタン制御
	SetButton(Changed());
}

// 右クリックすると <削除> ポップアップ
void CAttrBorderLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
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
			nq_border_node::kObjectID, bstrValue);

		CString objectID = bstrValue.m_str;

		// 以下のif分は、新規レコードの場合、OIDをRowDataに入れて管理するための処理である
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_border_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}		

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);

		m_nqDataGridMap[m_currentOid].highlightChangedCell();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_border_node::kTableName);

		pRelTableDef->setChanged(changeFlag);

		// ボタン制御
		SetButton(changeFlag);

	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_border_node::kTableName);
		pRelTableDef->setChanged(true);

		// ボタン制御
		SetButton(true);
	}
}

// Update Grid Data
bool CAttrBorderLinkDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), border_link::kTableName) != 0)	continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged()) continue;

		if (!m_nqDataGridMap[oid].store()) return false;
	}
	return true;
}

// Check Data Changes
bool CAttrBorderLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrBorderLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// ダイアログで表示している道路境界線リンクフィーチャを取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// グリッドで選択列のノード列レコードから、"順番","道路境界線リンクGID"を取得する
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNodeT = AheOpenSameOwnerTable(ipTable, border_node::kTableName);
	if (!ipNodeT) return;

	CString nodeGid, nodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(nodeGid, nodeGidNext, nq_border_node::kSequence, nq_border_node::kBorderNodeGID);

	//   検索されたノード列のノードGIDから次のノードを検索
	CString whereClause = uh::str_util::format(_T("%s='%s'"), border_node::kGlobalID, nodeGid);
	_ICursorPtr ipNodeCur;
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	IFeaturePtr ipNodeFeature;
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), border_node::kGlobalID, nodeGidNext);
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipToPoint);
}

// グリッドで選択しているノード列のラインをハイライトする
void CAttrBorderLinkDlg::highlightLine()
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
LRESULT CAttrBorderLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// ノード列のジャンプ,中心へ移動
void CAttrBorderLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
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
void CAttrBorderLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID フィールドは指定されても変更不可
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_border_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
