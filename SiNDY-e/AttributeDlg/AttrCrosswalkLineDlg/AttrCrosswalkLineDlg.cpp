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

// AttrCrosswalkLineDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include <boost/assign/list_of.hpp>

#include "AttrCrosswalkLineDlg.h"
#include "WinMap.h"


// ポリライン描画時の設定
namespace{
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}


extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCrosswalkLineDlg();
}

CAttrCrosswalkLineDlg::CAttrCrosswalkLineDlg() 
	: CAttrBaseDlg(CROSSWALK_LINE_MAP,
	  IDD_ATTR_CROSSWALK_LINE_DIALOG)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_WIDTH,  CContainedWindow(  _T("EDIT"), this, IDC_EDIT_WIDTH ) );
		( IDC_COMBO_SPECULATION_C,  CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) );
		( IDC_EDIT_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) );
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

CAttrCrosswalkLineDlg::~CAttrCrosswalkLineDlg() 
{}

LRESULT CAttrCrosswalkLineDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// コントロールのセット
	CreateControlRelation();

	// グリッド初期化
	m_Grid.init(GetDlgItem(IDC_VSFLEXGRID_NODE));

	// GridCtrlからのイベントを取得
	AtlAdviseSinkMap(this, TRUE);

	// サブクラス化
	for( auto& it : m_mapSubClass){
		it.second.SubclassWindow( GetDlgItem( it.first ) );
	}

	return CAttrBaseDlg::OnInitDialog( uMsg, wParam, lParam, bHandled );
}

LRESULT CAttrCrosswalkLineDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if( !m_bFinishInit ) return 0;
	INT msg = HIWORD(wParam);						// 操作メッセージ
	INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

	// エディットボックスの時に全選択にする
	if( msg == EN_SETFOCUS )
	SelectDlgItem( nTargetControl );

	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		InvalidateRect( NULL, FALSE );
		SetButton( Changed() );
	}

	return 0;
}

LRESULT CAttrCrosswalkLineDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	return 0;
}

LRESULT CAttrCrosswalkLineDlg::OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_Grid.addRow();
	return 0;
}

LRESULT CAttrCrosswalkLineDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_Grid.deleteSelectedRow();

	auto oids = m_Grid.getData();

	// グリッドでの削除をデータに反映
	std::vector<long> errOids;
	if( ! m_RelNodes[m_CurrentOid].reloadData(oids, errOids) )
	{
		// エラーになることはないはずだが念のため
		AheMessageBox( NULL, AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), 
		               AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID ), MB_OK|MB_TASKMODAL, NULL );
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
	
	return 0;
}

HWND CAttrCrosswalkLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return CAttrBaseDlg::Create(hWndParent,dwInitParam);
}

void CAttrCrosswalkLineDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
	SetFocus(); 

	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

bool CAttrCrosswalkLineDlg::Update()
{
	using namespace sindy::schema::adam_v2;
	using namespace uh;

	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), crosswalk_line::kTableName ) != 0 )
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID( &lOid );

		if( ! m_RelNodes[lOid].isChange() )
			continue;

		if( ! m_RelNodes[lOid].updateTable() )
			return false;
	}
	return true;
}

BOOL CAttrCrosswalkLineDlg::ErrorCheck()
{
	using namespace sindy::schema::adam_v2;

	BOOL isError = FALSE;
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), crosswalk_line::kTableName ) != 0 )
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID( &lOid );

		// 関連テーブルが読み込まれていない(ツリーに存在するが開かれていない)フィーチャはチェックしない
		if( m_RelNodes.find(lOid) == m_RelNodes.end() )
			continue;

		// 関連ノードの妥当性チェック
		if( m_RelNodes[lOid].hasError() ) {
			CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), lOid); 
			AheMessageBox( NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE ), MB_OK|MB_TASKMODAL, NULL );
			isError = TRUE;
		}
	}

	return !isError;
}

BOOL CAttrCrosswalkLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	BOOL bRet = CAttrBaseDlg::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
	}

	IFeaturePtr crosswalkFeature = GetCurrentFeatureDef()->GetFeature();
	if (! crosswalkFeature)
		return FALSE;

	ITablePtr cwTable = AheGetTable(crosswalkFeature);
	if(! cwTable)
		return FALSE;

	ITablePtr relTable = AheOpenSameOwnerTable(cwTable, rel_lane_node_crosswalk_line::kTableName);
	if(! relTable)
		return FALSE;
	ITablePtr nodeTable = AheOpenSameOwnerTable(cwTable, lane_node::kTableName);
	if(! nodeTable )
		return FALSE;


	// まだ関連テーブルを読み込んでいない横断歩道ならばデータを読み込む
	// ツリービューでの切り替え操作時のために必要な処理
	crosswalkFeature->get_OID(&m_CurrentOid);
	if( m_RelNodes.count( m_CurrentOid ) == 0 )
	{
		CComVariant vaGid = AheGetAttribute(crosswalkFeature, kGlobalID);
		if( vaGid.vt != VT_BSTR )
			return FALSE;

		RelNodeInfos relInfo(relTable, nodeTable, vaGid.bstrVal);
		if(! relInfo.read())
			return FALSE;
		m_RelNodes[m_CurrentOid] = relInfo;
	}

	// グリッドへデータをセット
	auto oids = m_RelNodes[m_CurrentOid].getOIDs();
	m_Grid.setData(oids);

	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	return bRet;
}

BOOL CAttrCrosswalkLineDlg::Changed()
{
	return CAttrBaseDlg::Changed() || m_RelNodes[m_CurrentOid].isChange();
}

LRESULT CAttrCrosswalkLineDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DrawLinkageLine();

	return 0;
}

void CAttrCrosswalkLineDlg::CreateControlRelation()
{
	using namespace sindy::schema::adam_v2;

	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		m_cControlRel.SetControl(IDC_EDIT_WIDTH,          TYPE_EDITDOUBLE,  0, 0, crosswalk_line::kWidth,        NULL, crosswalk_line::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO,       0, 0, crosswalk_line::kSperculationC, NULL, crosswalk_line::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID,      TYPE_EDITSTRING,  0, 0, crosswalk_line::kSourceID,      NULL, crosswalk_line::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE,         TYPE_EDITSTRING,  0, 0, crosswalk_line::kSource,        NULL, crosswalk_line::kTableName);
	}
}

void __stdcall CAttrCrosswalkLineDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing())
		*Cancel = VARIANT_TRUE;
}

void __stdcall CAttrCrosswalkLineDlg::OnAfterEdit(LONG Row, LONG Col)
{ 
	// グリッドの情報を保持しているデータ反映
	auto oids = m_Grid.getData();
	
	std::vector<long> errOids;
	if(! m_RelNodes[m_CurrentOid].reloadData(oids, errOids))
	{
		//m_Grid.deleteRow(errOids);
		CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), m_CurrentOid); 
		AheMessageBox( NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE ), MB_OK|MB_TASKMODAL, NULL );
		return;
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
}

void __stdcall CAttrCrosswalkLineDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	using namespace sindy::schema::adam_v2;

	// 右クリック以外は何も処理しない
	if (! (Button & 0x02)) { return; }

	// 編集未開始も処理しない
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()){ return; }

	// マウスアップ時の処理はグリッドコントロールクラスへ委譲
	m_Grid.OnMouseUpGrid(Button, Shift, X, Y, m_hWnd, IDR_MENU_GRID);
}

void __stdcall CAttrCrosswalkLineDlg::OnCellChangedGrid()
{
	//DrawSelectedNode();
}

void CAttrCrosswalkLineDlg::UpdateChanged()
{
	using namespace sindy::schema::adam_v2;

	// 変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_lane_node_crosswalk_line::kTableName);
	pRelTableDef->setChanged( m_RelNodes[m_CurrentOid].isChange() );

	// ボタン制御
	SetButton( Changed() );
}

void CAttrCrosswalkLineDlg::DrawLinkageLine()
{
	// 再描画イベントでこの関数を呼び出しているので、
	// チェックする
	if( m_RelNodes.empty() )
		return;

	// 横断歩道ラインのジオメトリ取得
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr geo;
	feature->get_Shape(&geo);
	IPolylinePtr cwPolyline(geo);
	if(! cwPolyline)
		return;

	// ポイントの形状を取得
	auto nodePoints = m_RelNodes[m_CurrentOid].getNodePoints();

	for( auto point : nodePoints )
	{
		// ライン上で一番近い点を取得し、ラインを作成
		// 最近傍点を取得
		IPointPtr nearestPoint;
	    if( FAILED((( IProximityOperatorPtr)cwPolyline)->ReturnNearestPoint( point, esriSegmentExtension::esriNoExtension, &nearestPoint )))
			continue;

		IPolylinePtr drawLine(CLSID_Polyline);
		drawLine->put_FromPoint(nearestPoint);
		drawLine->put_ToPoint(point);

		DrawPolyLine(drawLine, LINE_COLOR);
	}
}

void CAttrCrosswalkLineDlg::DrawPolyLine(const IPolylinePtr& polyline, const DWORD& color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), polyline, color, 5, esriSLSSolid, TRUE, TRUE);
}
