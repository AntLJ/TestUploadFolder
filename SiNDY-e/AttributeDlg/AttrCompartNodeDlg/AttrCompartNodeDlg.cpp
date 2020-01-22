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

// AttrCompartNodeDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrCompartNodeDlg.h"
#include <boost/assign/list_of.hpp>

#include "WinMap.h"

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;

namespace {
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrCompartNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCompartNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCompartNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCompartNodeDlg::CAttrCompartNodeDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCompartNodeDlg::CAttrCompartNodeDlg() 
	: ATTR_BASE_CLASS(COMPART_NODE_MAP, IDD_COMPARTNODEDLG),
	  m_currentOid(0)
{
	// TODO[SiNDYChildDlg] 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_NODECLASS_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_NODECLASS_C ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_STATIC_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) );
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

void CAttrCompartNodeDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// グリッド列ごとの編集可否設定はグリッドクラス側へ委譲
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

void CAttrCompartNodeDlg::OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
}

void CAttrCompartNodeDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);
	updateChanged();
}

/**
 * @fn HWND CAttrCompartNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCompartNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrCompartNodeDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCompartNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

bool CAttrCompartNodeDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), compart_node::kTableName) != 0)
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
 * @fn BOOL CAttrCompartNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCompartNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	createGrid();

	// 地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrCompartNodeDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

/**
 * @fn void CAttrCompartNodeDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCompartNodeDlg::CreateControlRelation()
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

		// ノード種別コード
		m_cControlRel.SetControl( IDC_COMBO_NODECLASS_C, TYPE_COMBO, 0, 0, compart_node::kNodeClassC, 0, compart_node::kTableName );
		// 推測コード
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, compart_node::kSperculationC, 0, compart_node::kTableName );
		// 提供元ID
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, compart_node::kSourceID, 0, compart_node::kTableName, 0, -1, FOLLOW_READONLY);
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, compart_node::kTableName );
	}
}

void CAttrCompartNodeDlg::createGrid()
{
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	if (!feature)
		return;

	ITablePtr nodeTable;
	feature->get_Table(&nodeTable);
	if (!nodeTable) {
		return;
	}
	ITablePtr nqTable = AheOpenSameOwnerTable(nodeTable, nq_compart_node::kTableName);
	if (!nqTable) {
		return;
	}

	long oid(0);
	feature->get_OID(&oid);

	// 始めてダイアログに表示したフィーチャであれば、ノード列グリッドを登録
	if (m_nqDataGridMap.count(oid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(nqTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NQ));
		builder.addField(nq_compart_node::kObjectID)
			.addField(nq_compart_node::kSequence)
			.addField(nq_compart_node::kWidth);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[oid] = nqGrid;
	}
	
	// グリッド初期化
	m_currentOid = oid;
	m_nqDataGridMap[m_currentOid].init();

	// グリッドの編集可能フィールドを設定
	const static std::set<CString> editableFields = boost::assign::list_of(CString(nq_compart_node::kWidth));
	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);

	// ノード列データをグリッドへ読み込む
	// 終端フラグが立っているものはグリッドへ表示しない
	CComVariant compartNodeGid = AheGetAttribute(feature, compart_node::kGlobalID);
	if (compartNodeGid.vt == VT_NULL) {
		return;
	}
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_compart_node::kCompartNodeGID, compartNodeGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

void CAttrCompartNodeDlg::updateChanged()
{
	// 関連テーブル(ノード列)の変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_compart_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// ボタン制御
	SetButton(Changed());
}

void CAttrCompartNodeDlg::highlightLine()
{
	if (m_nqDataGridMap.empty())
		return;

	// グリッド選択行のノード列レコードを取得
	_IRowPtr nqRow = m_nqDataGridMap[m_currentOid].getSelectedRow();
	// ダイアログで表示している区画線ノードフィーチャを取得
	IFeaturePtr compartNodeFeature = GetCurrentFeatureDef()->GetFeature();
	if (!compartNodeFeature)
		return;

	// グリッドで選択列のノード列レコードから、"順番","区画線リンクGID"を取得する
	ITablePtr compartNodeT;
	compartNodeFeature->get_Table(&compartNodeT);
	if (!compartNodeT)
		return;
	ITablePtr nqCompartNodeT = AheOpenSameOwnerTable(compartNodeT, nq_compart_node::kTableName);
	if (!nqCompartNodeT)
		return;

	CComVariant compartLinkGid = AheGetAttribute(nqRow, nq_compart_node::kCompartLinkGID);
	if (compartLinkGid.vt == VT_NULL)
		return;
	long sequence = AheGetAttribute(nqRow, nq_compart_node::kSequence).lVal;


	// グリッド選択行のノード列レコードの次のノードを取得する
	//   ノード列から次のノードにあたるレコードを検索
	CString whereClause = uh::str_util::format(_T("%s='%s' and %s=%d"),
		nq_compart_node::kCompartLinkGID, compartLinkGid.bstrVal,
		nq_compart_node::kSequence, sequence + 1);

	_ICursorPtr nqCur;
	if (FAILED(nqCompartNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_TRUE, &nqCur)))
		return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	_IRowPtr nqNextRow;
	if (nqCur->NextRow(&nqNextRow) != S_OK)
		return;
	if (!nqNextRow)
		return;
	CComVariant nextCompartNodeGid = AheGetAttribute(nqNextRow, nq_compart_node::kCompartNodeGID);
	if (nextCompartNodeGid.vt == VT_NULL)
		return;

	//   検索されたノード列の区画線ノードGIDから次のノードを検索
	whereClause = uh::str_util::format(_T("%s='%s'"), compart_node::kGlobalID, nextCompartNodeGid.bstrVal);
	_ICursorPtr compartNodeCur;
	if (FAILED(compartNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &compartNodeCur)))
		return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	IFeaturePtr compartNodeNextFeature;
	if (IFeatureCursorPtr(compartNodeCur)->NextFeature(&compartNodeNextFeature) != S_OK)
		return;
	if (!compartNodeNextFeature)
		return;


	// ダイアログで表示している区画線ノードと次の区画線ノードを結ぶラインを作成する
	IGeometryPtr fromPt, toPt;
	compartNodeFeature->get_ShapeCopy(&fromPt);
	compartNodeNextFeature->get_ShapeCopy(&toPt);

	ISpatialReferencePtr sr;
	fromPt->get_SpatialReference(&sr);

	IPolylinePtr drawLine(CLSID_Polyline);
	drawLine->putref_SpatialReference(sr);
	drawLine->put_FromPoint(IPointPtr(fromPt));
	drawLine->put_ToPoint(IPointPtr(toPt));

	// ライン描画
	AheDrawPolyline(m_cArcHelper.GetDisplay(), drawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);
}
