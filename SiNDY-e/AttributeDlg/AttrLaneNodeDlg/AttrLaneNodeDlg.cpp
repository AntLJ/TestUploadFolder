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

// AttrLaneNodeDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrLaneNodeDlg.h"
#include <boost/assign/list_of.hpp>

#include "WinMap.h"

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;
using namespace sindy::schema::category;

namespace {
	const CString UNDEF_NAME = _T("未設定");
	const long UNDEF_OID = 0;
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}


/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrLaneNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrLaneNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrLaneNodeDlg::CAttrLaneNodeDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrLaneNodeDlg::CAttrLaneNodeDlg() 
	: ATTR_BASE_CLASS(LANE_NODE_MAP, IDD_LANENODEDLG),
	m_currentOid(0)
{
	// [SiNDYChildDlg] 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODECLASS_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_NODECLASS_C ) )
		( IDC_COMBO_POSITION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_POSITION_C ) )
		( IDC_CHECK_STOP_POINT_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_STOP_POINT_F ) )
		( IDC_CHECK_COVERING_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_COVERING_F ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID ) )

		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );

	// フォント作成
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,    // 斜体，下線，打消線
		SHIFTJIS_CHARSET,       // キャラクタ設定
		OUT_DEFAULT_PRECIS,     // 出力精度
		CLIP_DEFAULT_PRECIS,    // クリップ精度
		DEFAULT_QUALITY,        // 品質
		FIXED_PITCH | FF_DONTCARE,// ピッチ
		AheLoadString(IDS_FONT));   // フォント種類
}

LRESULT CAttrLaneNodeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// コントロール
	CreateControlRelation();

	// グリッドコントロールからのイベントを取得
	AtlAdviseSinkMap(this, TRUE);

	// サブクラス化
	for (auto& it : m_mapSubClass)
	{
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	return ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrLaneNodeDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAttrLaneNodeDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrLaneNodeDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	highlightLine();
	return 0;
}

LRESULT CAttrLaneNodeDlg::OnUpdateCrossName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (::IsWindow(hWndCtl))
	{
		CEdit edit(hWndCtl);
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca(nTextLen * sizeof(TCHAR));
		edit.GetWindowText(lpszText, nTextLen);

		// <null> の場合を考慮
		if (lstrcmp(lpszText, NULL_VALUE) != 0)
		{
			CString strName = AheConvertToIntersectionName(lpszText, TRUE);

			if (strName.Compare(lpszText) != 0)
			{
				INT dwSelStart = 0, dwSelEnd = 0;
				edit.GetSel(dwSelStart, dwSelEnd);
				edit.SetWindowText(strName);
				// 入力位置の修正
				INT nDiff = CString_GetLength2(CString(lpszText)) - CString_GetLength2(strName);
				edit.SetSel(dwSelStart - nDiff, dwSelEnd - nDiff);
			}
		}
		m_cControlRel.ControlChanged(wID);
		SetButton(Changed());
		InvalidateRect(NULL, FALSE);
	}

	return 0;
}

LRESULT CAttrLaneNodeDlg::OnUpdateCrossYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (::IsWindow(hWndCtl))
	{
		CEdit edit(hWndCtl);
		INT nTextLen = edit.GetWindowTextLength() + 1;
		LPTSTR lpszText = (LPTSTR)_alloca(nTextLen * sizeof(TCHAR));
		edit.GetWindowText(lpszText, nTextLen);


		// <null> の場合を考慮
		if (lstrcmp(lpszText, NULL_VALUE) != 0)
		{
			CString strYomi = AheConvertToIntersectionName(lpszText, FALSE);
			if (strYomi.Compare(lpszText) != 0)
			{
				INT nSelStart = 0, nSelEnd = 0;
				edit.GetSel(nSelStart, nSelEnd);
				edit.SetWindowText(strYomi);
				// 入力位置の修正
				INT nDiff = CString_GetLength2(CString(lpszText)) - CString_GetLength2(strYomi);
				edit.SetSel(nSelStart - nDiff, nSelEnd - nDiff);
			}
		}
		m_cControlRel.ControlChanged(wID);
		SetButton(Changed());
		InvalidateRect(NULL, FALSE);
	}

	return 0;
}

void CAttrLaneNodeDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// グリッド列ごとの編集可否設定はグリッドクラス側へ委譲
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

void CAttrLaneNodeDlg::OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

void CAttrLaneNodeDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);
	updateChanged();
}

/** 
 * @fn HWND CAttrLaneNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrLaneNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrLaneNodeDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrLaneNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

bool CAttrLaneNodeDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), lane_node::kTableName) != 0)
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID(&lOid);

		if (!m_nqDataGridMap[lOid].isChanged())
			continue;

		if (!m_nqDataGridMap[lOid].store())
			return false;
	}
	return true;
}

/**
 * @fn BOOL CAttrLaneNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrLaneNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// [SiNDYChildDlg] 最初にフォーカスされるコントロール
		// 名称・読みはNULL可なので、車線番号を最初にしておく
		//::SetFocus( GetDlgItem(IDC_EDIT_LANE_NUMBER) );
	}

	createGrid();

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrLaneNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

/**
 * @fn void CAttrLaneNodeDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrLaneNodeDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// [SiNDYChildDlg] ControlRelにフィールド名とテーブル名を登録
		m_cControlRel.SetControl( IDC_COMBO_NODECLASS_C,		TYPE_COMBO,			0, 0,	lane_node::kNodeClassC,			0,	lane_node::kTableName, 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_COMBO_POSITION_C,			TYPE_COMBO,			0, 0,	lane_node::kPositionC,			0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_STOP_POINT_F,		TYPE_CHECKBOX,		0, 0,	lane_node::kStopPointF,			0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_COVERING_F,			TYPE_CHECKBOX,		0, 0,	lane_node::kCoveringF,			0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C,		TYPE_COMBO,			0, 0,	lane_node::kSperculationC,		0,	lane_node::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID,           TYPE_EDITSTRING,	0, 0,	lane_node::kSourceID,			0,	lane_node::kTableName,  0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用

		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,	0, 0,	lane_node::kSource,			0,	lane_node::kTableName, 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
	}
}

/**
 * @fn void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
 * @brief ツールチップを作成します。
 *
 * @note 作成するツールチップは属性に変更がかからないため、作りっぱなしで問題なし
 *
 * @param nTargetItem	[in]	ツールチップを作成するコントロールアイテムのID
 * 
 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg を参考
*/
void CAttrLaneNodeDlg::SetToolTip(INT nTargetItem)
{
	// エディットボックスにかかれている文字列を取得
	CEdit edit( GetDlgItem(nTargetItem) );
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	// まだツールチップ作成されていなかったら作成する（一番最初のみ入ってくる）
	HWND hToolTip;
	hToolTip =
		CreateWindowEx(
		0,				//拡張ウィンドウスタイル
		TOOLTIPS_CLASS,	//クラスネーム
		NULL,			//ウィンドウネーム
		TTS_ALWAYSTIP,	// ウィンドウスタイル
		CW_USEDEFAULT,	// Ｘ座標
		CW_USEDEFAULT,	// Ｙ座標
		CW_USEDEFAULT,	// 幅
		CW_USEDEFAULT,	// 高さ
		GetDlgItem(nTargetItem),// 親ウィンドウのハンドル
		NULL,			// メニューハンドル
		NULL,			// インスタンスハンドル
		NULL);			// WM_CREATEデータ

	// ToolInfo構造体 
	static TOOLINFO ti;
	ZeroMemory(&ti , sizeof(TOOLINFO)) ; 
	ti.cbSize = sizeof(TOOLINFO) ; 
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS ; 
	ti.hwnd = GetDlgItem(nTargetItem);
	ti.uId =  (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;

	ti.lpszText = lpszText;

	::SendMessage( hToolTip, TTM_ADDTOOL , 0 , (LPARAM)&ti);
	::SendMessage( hToolTip, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
	::SendMessage( hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

	CWindow cWnd( hToolTip );
	cWnd.SetFont( m_hFont );
}

/**
 * エラーチェック
 */
BOOL CAttrLaneNodeDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == lane_link::kTableName )
		{
			//エラー処理
		}
	}

	return TRUE;
}

void CAttrLaneNodeDlg::createGrid()
{
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	if (!feature)
		return;

	ITablePtr table;
	feature->get_Table(&table);
	if (!table)
		return;

	ITablePtr nqTable = AheOpenSameOwnerTable(table, nq_lane_node::kTableName);
	if (!nqTable)
		return;

	long oid(0);
	feature->get_OID(&oid);

	// 始めてダイアログに表示したフィーチャであれば、ノード列グリッドを登録
	if (m_nqDataGridMap.count(oid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(nqTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NQ));
		builder.addField(nq_lane_node::kObjectID)
			.addField(nq_lane_node::kSequence)
			.addField(nq_lane_node::kWidth)
			.addField(nq_lane_node::kLinearElement)
			.addField(nq_lane_node::kCurvature)
			.addField(nq_lane_node::kCurvatureRadius)
			.addField(nq_lane_node::kCurvatureChangeRate)
			.addField(nq_lane_node::kClothoidParameter)
			.addField(nq_lane_node::kLongitudinalSlope)
			.addField(nq_lane_node::kCrossSlope);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[oid] = nqGrid;
	}

	// グリッド初期化
	m_currentOid = oid;
	m_nqDataGridMap[m_currentOid].init();

	// グリッドの編集可能フィールドを設定
	const static std::set<CString> editableFields = boost::assign::list_of(CString(nq_lane_node::kWidth));
	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);

	// ノード列データをグリッドへ読み込む
	// 終端フラグが立っているものはグリッドへ表示しない
	CComVariant laneNodeGid = AheGetAttribute(feature, lane_node::kGlobalID);
	if (laneNodeGid.vt == VT_NULL) {
		return;
	}
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_lane_node::kLaneNodeGID, laneNodeGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

void CAttrLaneNodeDlg::updateChanged()
{
	// 関連テーブル(ノード列)の変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// ボタン制御
	SetButton(Changed());
}

void CAttrLaneNodeDlg::highlightLine()
{
	if (m_nqDataGridMap.empty())
		return;

	// グリッド選択行のノード列レコードを取得
	_IRowPtr nqRow = m_nqDataGridMap[m_currentOid].getSelectedRow();
	if (!nqRow)
		return;

	// ダイアログで表示している車線ノードフィーチャを取得
	IFeaturePtr laneNodeFeature = GetCurrentFeatureDef()->GetFeature();
	if (!laneNodeFeature)
		return;

	// グリッドで選択列のノード列レコードから、"順番","車線リンクGID"を取得する
	ITablePtr laneNodeT;
	laneNodeFeature->get_Table(&laneNodeT);
	if (!laneNodeT)
		return;
	ITablePtr nqLaneNodeT = AheOpenSameOwnerTable(laneNodeT, nq_lane_node::kTableName);
	if (!nqLaneNodeT)
		return;

	CComVariant laneLinkGid = AheGetAttribute(nqRow, nq_lane_node::kLaneLinkGID);
	if (laneLinkGid.vt == VT_NULL)
		return;
	long sequence = AheGetAttribute(nqRow, nq_lane_node::kSequence).lVal;


	// グリッド選択行のノード列レコードの次のノードを取得する
	//   ノード列から次のノードにあたるレコードを検索
	CString whereClause = uh::str_util::format(_T("%s='%s' and %s=%d"),
		nq_lane_node::kLaneLinkGID, laneLinkGid.bstrVal,
		nq_lane_node::kSequence, sequence + 1);
	
	_ICursorPtr nqCur;
	if (FAILED(nqLaneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &nqCur)))
		return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	_IRowPtr nqNextRow;
	if (nqCur->NextRow(&nqNextRow) != S_OK)
		return;
	if (!nqNextRow)
		return;
	CComVariant nextLaneNodeGid = AheGetAttribute(nqNextRow, nq_lane_node::kLaneNodeGID);
	if (nextLaneNodeGid.vt == VT_NULL)
		return;

	//   検索されたノード列の車線ノードGIDから次のノードを検索
	whereClause = uh::str_util::format(_T("%s='%s'"), lane_node::kGlobalID, nextLaneNodeGid.bstrVal);
	_ICursorPtr laneNodeCur;
	if (FAILED(laneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &laneNodeCur)))
		return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	IFeaturePtr laneNodeNextFeature;
	if (IFeatureCursorPtr(laneNodeCur)->NextFeature(&laneNodeNextFeature) != S_OK)
		return;
	if (!laneNodeNextFeature)
		return;


	// ダイアログで表示している車線ノードと次の車線ノードを結ぶラインを作成する
	IGeometryPtr fromPt, toPt;
	laneNodeFeature->get_ShapeCopy(&fromPt);
	laneNodeNextFeature->get_ShapeCopy(&toPt);

	ISpatialReferencePtr sr;
	fromPt->get_SpatialReference(&sr);

	IPolylinePtr drawLine(CLSID_Polyline);
	drawLine->putref_SpatialReference(sr);
	drawLine->put_FromPoint(IPointPtr(fromPt));
	drawLine->put_ToPoint(IPointPtr(toPt));

	// ライン描画（描画中のラインは消す)
	AheDrawPolyline(m_cArcHelper.GetDisplay(), drawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);
}
