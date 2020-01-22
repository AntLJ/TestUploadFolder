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

// AttrRoadSignPointDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrRoadSignPointDlg.h"
#include <boost/assign/list_of.hpp>
#include "Util.h"

using namespace sindy::schema;
using namespace adam_v2;

#include "WinMap.h"

// ポリライン描画時の設定
namespace {
	const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
}

extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrRoadSignPointDlg();
}

CAttrRoadSignPointDlg::CAttrRoadSignPointDlg() 
	: ATTR_BASE_CLASS(ROADSIGN_POINT_MAP, IDD_ROADSIGNPOINTDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_COMBO_ROADSIGN_C,      CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_ROADSIGN_C))
		(IDC_EDIT_ROADSIGN_C,       CContainedWindow(_T("EDIT"), this, IDC_EDIT_ROADSIGN_C))
		(IDC_BUTTON_SEARCH,         CContainedWindow(_T("BUTTON"), this, IDC_BUTTON_SEARCH))
		(IDC_EDIT_HORIZONTAL_ANGLE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_HORIZONTAL_ANGLE))
		(IDC_CHECK_VARIABLE_F,      CContainedWindow(_T("CHECKBOX"), this, IDC_CHECK_VARIABLE_F))
		(IDC_COMBO_SPECULATION_C,   CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C))
		(IDC_EDIT_SOURCE_ID,        CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		(IDC_EDIT_SOURCE,           CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE ))
		;

	// メンバ変数初期化
	m_TabNum		= 0;
	m_FeatureIndex  = 0;
	m_TableIndex	= 0;
	m_RowIndex		= 0;
	m_IsForce		= false;
	m_Editable		= false;
}

LRESULT CAttrRoadSignPointDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// コントロールのセット
	CreateControlRelation();

	// グリッド初期化
	m_Grid.init(GetDlgItem(IDC_VSFLEXGRID_NODE));

	// GridCtrlからのイベントを取得
	AtlAdviseSinkMap(this, TRUE);

	// サブクラス化
	for (auto& it : m_mapSubClass) {
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	//// タブコントロールの作成
	//CTabCtrl cTab( GetDlgItem( IDC_TAB_ROADSIGN ) );
	//cTab.AddItem( AheLoadString( IDS_ROADSIGN_BASE ) );
	//cTab.AddItem( AheLoadString( IDS_ROADSIGN_CLASS ) );

	return CAttrBaseDlg::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrRoadSignPointDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( !m_bFinishInit ) return 0;
	INT msg = HIWORD(wParam);						// 操作メッセージ
	INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

	// エディットボックスの時に全選択にする
	if( msg == EN_SETFOCUS )
	SelectDlgItem( nTargetControl );

	if (msg == CBN_DROPDOWN)
		SetComboboxList(nTargetControl);
	if ((msg == CBN_SELCHANGE) || (msg == BN_CLICKED) || (msg == EN_UPDATE)) {
		m_cControlRel.ControlChanged((INT)LOWORD(wParam));
		InvalidateRect(NULL, FALSE);
		SetButton(Changed());
	}

	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	SetRoadsignCodeToComboBox(IDC_EDIT_ROADSIGN_C, IDC_COMBO_ROADSIGN_C);
	return 0;
}

// 道路標識コードを別タブで編集可能にする場合に備えて、
// 以下のコードはコメントアウトとして残しておく
//LRESULT CAttrRoadSignPointDlg::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
//{
	//if( IDC_TAB_ROADSIGN == wParam )
	//{
	//	// セルが切り替わりのイベントか
	//	if( (((NMHDR*)lParam)->code) == TCN_SELCHANGE )
	//	{
	//		// 選択されたタブを取得
	//		m_ChangeType = TabCtrl_GetCurSel(((NMHDR*)lParam)->hwndFrom);

	//		m_cControlRel.Visible(m_ChangeType);
	//		ChangeWindowMgrMap( m_ChangeType );
	//		m_TabNum = m_ChangeType;				// タブ位置保存

	//		ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_FeatureIndex, m_TableIndex, m_RowIndex, m_TabNum, m_IsForce, m_Editable );
	//					
	//		switch( m_TabNum )
	//		{
	//		case EDITVIEW_ROADSIGN_KIHON:
	//			if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
	//				::SetFocus( GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE) );
	//			break;
	//		case EDITVIEW_ROADSIGN_CLASS:
	//			break;
	//		default:
	//			break;
	//		}

	//		bHandled = TRUE;
	//		return 1;
	//	}
	//}

	//if( 0 == wParam )
	//{
	//	HWND hwndTab = GetDlgItem( IDC_TAB_ROADSIGN );
	//	if( -2 == lParam || -1 == lParam )
	//	{
	//		// コントロール切り替え
	//		if( -1 == lParam )
	//		{	
	//			if( 1 == m_ChangeType )	
	//				m_ChangeType = 0;
	//			else
	//				m_ChangeType = m_ChangeType + 1;
	//		}
	//		else
	//		{
	//			if( 0 == m_ChangeType )
	//				m_ChangeType = 1;
	//			else
	//				m_ChangeType = m_ChangeType - 1;
	//		}
	//		::SendMessage( hwndTab, TCM_SETCURSEL, m_ChangeType, 0 );

	//		m_cControlRel.Visible( m_ChangeType );
	//		ChangeWindowMgrMap( m_ChangeType );
	//		m_TabNum = m_ChangeType;	// タブの位置保存
	//		SetCurrentFeatureDefIndex( m_FeatureIndex, m_TableIndex, m_RowIndex, m_IsForce, m_Editable );

	//		bHandled = TRUE;
	//		return 1;
	//	}
	//}

//}

LRESULT CAttrRoadSignPointDlg::OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_Grid.addRow();
	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_Grid.deleteSelectedRow();

	auto oids = m_Grid.getData();

	// グリッドでの削除をデータに反映
	std::vector<long> errOids;
	if (!m_RelNodes[m_CurrentOid].reloadData(oids, errOids))
	{
		// エラーになることはないはずだが念のため
		AheMessageBox(NULL, AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID),
			AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), MB_OK | MB_TASKMODAL, NULL);
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);

	return 0;
}

HWND CAttrRoadSignPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	return ATTR_BASE_CLASS::Create(hWndParent, dwInitParam);
}

void CAttrRoadSignPointDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
	SetFocus();

	ClearFeatureDefs();
	if (IsWindow())
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

bool CAttrRoadSignPointDlg::Update()
{
	using namespace sindy::schema::adam_v2;
	using namespace uh;

	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadsign_point::kTableName) != 0)
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID(&lOid);

		if (!m_RelNodes[lOid].isChange())
			continue;

		if (!m_RelNodes[lOid].updateTable())
			return false;
	}
	return true;
}

BOOL CAttrRoadSignPointDlg::ErrorCheck()
{
	using namespace sindy::schema::adam_v2;

	BOOL isError = FALSE;
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadsign_point::kTableName) != 0)
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID(&lOid);

		// 関連テーブルが読み込まれていない(ツリーに存在するが開かれていない)フィーチャはチェックしない
		if (m_RelNodes.find(lOid) == m_RelNodes.end())
			continue;

		// 関連ノードの妥当性チェック
		if (m_RelNodes[lOid].hasError()) {
			CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), lOid);
			AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE), MB_OK | MB_TASKMODAL, NULL);
			isError = TRUE;
		}
	}

	return !isError;
}

BOOL CAttrRoadSignPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_TabNum, bForce, bEditable );

	m_FeatureIndex = lFeatureIndex;
	m_TableIndex = lTableIndex;
	m_RowIndex = lRowIndex;
	m_IsForce = bForce;
	m_Editable = bEditable;

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		::SetFocus(GetDlgItem(IDC_EDIT_SOURCE));
	}

	IFeaturePtr roadsign = GetCurrentFeatureDef()->GetFeature();
	if (!roadsign)
		return FALSE;

	ITablePtr roadsignTable = AheGetTable(roadsign);
	if (!roadsignTable)
		return FALSE;

	ITablePtr relTable = AheOpenSameOwnerTable(roadsignTable, rel_lane_node_roadsign_point::kTableName);
	if (!relTable)
		return FALSE;
	ITablePtr nodeTable = AheOpenSameOwnerTable(roadsignTable, lane_node::kTableName);
	if (!nodeTable)
		return FALSE;


	// まだ関連テーブルを読み込んでいない道路標識ポイントならばデータを読み込む
	// ツリービューでの切り替え操作時のために必要な処理
	roadsign->get_OID(&m_CurrentOid);
	if (m_RelNodes.count(m_CurrentOid) == 0)
	{
		CComVariant vaGid = AheGetAttribute(roadsign, kGlobalID);
		if (vaGid.vt != VT_BSTR)
			return FALSE;

		RelNodeInfos relInfo(relTable, nodeTable, vaGid.bstrVal);
		if (!relInfo.read())
			return FALSE;
		m_RelNodes[m_CurrentOid] = relInfo;
	}

	// グリッドへデータをセット
	auto oids = m_RelNodes[m_CurrentOid].getOIDs();
	m_Grid.setData(oids);

	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrRoadSignPointDlg::CheckReturnKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_ROADSIGN_C) || (nClassID == IDC_BUTTON_SEARCH))
	{
		if (!SetRoadsignCodeToComboBox(IDC_EDIT_ROADSIGN_C, IDC_COMBO_ROADSIGN_C))
			return 0;
	}

	return 0;
}


BOOL CAttrRoadSignPointDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_RelNodes[m_CurrentOid].isChange();
	return 0;
}

LRESULT CAttrRoadSignPointDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	DrawLinkageLine();
	return 0;
}

void CAttrRoadSignPointDlg::CreateControlRelation()
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

		m_cControlRel.SetControl(IDC_COMBO_ROADSIGN_C, TYPE_COMBO, 0, 0, roadsign_point::kRoadSignC, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_ROADSIGN_C, TYPE_EDIT, 0, 0);
		m_cControlRel.SetControl(IDC_COMBO_ROADSIGN_C, TYPE_COMBO, 0, 0, roadsign_point::kRoadSignC, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_HORIZONTAL_ANGLE, TYPE_EDITDOUBLE, 0, 0, roadsign_point::kHorizontalAngle, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_CHECK_VARIABLE_F, TYPE_CHECKBOX, 0, 0, roadsign_point::kVariableF, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, roadsign_point::kSperculationC, NULL, roadsign_point::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, roadsign_point::kSourceID, NULL, roadsign_point::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, roadsign_point::kSource, NULL, roadsign_point::kTableName, 0, -1, FOLLOW_READONLY);
	}
}

void CAttrRoadSignPointDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing())
		*Cancel = VARIANT_TRUE;
}

void CAttrRoadSignPointDlg::OnAfterEdit(LONG Row, LONG Col)
{
	// グリッドの情報を保持しているデータ反映
	auto oids = m_Grid.getData();

	std::vector<long> errOids;
	if (!m_RelNodes[m_CurrentOid].reloadData(oids, errOids))
	{
		//m_Grid.deleteRow(errOids);
		CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), m_CurrentOid);
		AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_NODE), MB_OK | MB_TASKMODAL, NULL);
		return;
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
}

void CAttrRoadSignPointDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	// 右クリック以外は何も処理しない
	if (!(Button & 0x02)) { return; }

	// 編集未開始も処理しない
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) { return; }

	// マウスアップ時の処理はグリッドコントロールクラスへ委譲
	m_Grid.OnMouseUpGrid(Button, Shift, X, Y, m_hWnd, IDR_MENU_GRID);
}

void CAttrRoadSignPointDlg::UpdateChanged()
{
	using namespace sindy::schema::adam_v2;

	// 変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_lane_node_roadsign_point::kTableName);
	pRelTableDef->setChanged(m_RelNodes[m_CurrentOid].isChange());

	// ボタン制御
	SetButton(Changed());
}

void CAttrRoadSignPointDlg::DrawLinkageLine()
{
	// 再描画イベントでこの関数を呼び出しているので、
	// チェックする
	if (m_RelNodes.empty())
		return;

	// 道路標識ポイントのジオメトリ取得
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr geo;
	feature->get_Shape(&geo);
	IPointPtr roadsignPont(geo);
	if (!roadsignPont)
		return;

	ISpatialReferencePtr sr;
	roadsignPont->get_SpatialReference(&sr);

	// ポイントの形状を取得
	auto nodePoints = m_RelNodes[m_CurrentOid].getNodePoints();

	for (const auto point : nodePoints)
	{
		IPolylinePtr drawLine(CLSID_Polyline);
		drawLine->putref_SpatialReference(sr);
		drawLine->put_FromPoint(roadsignPont);
		drawLine->put_ToPoint(point);

		DrawPolyLine(drawLine, LINE_COLOR);
	}
}

void CAttrRoadSignPointDlg::DrawPolyLine(const IPolylinePtr & polyline, const DWORD & color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), polyline, color, 5, esriSLSSolid, TRUE, TRUE);
}

bool CAttrRoadSignPointDlg::SetRoadsignCodeToComboBox(INT editControlId, INT comboBoxId)
{
	// EditControl に入力された値を取得
	CEdit edit(GetDlgItem(editControlId));
	CString strSearchCode;
	edit.GetWindowText(strSearchCode);
	if (strSearchCode.IsEmpty())
		return true;

	CComboBox combo(GetDlgItem(comboBoxId));
	CFieldDef* fieldDef = GetCurrentFeatureDef()->GetFieldDef(roadsign_point::kTableName, roadsign_point::kRoadSignC);
	const auto domainMap = fieldDef->GetDomain();
	for (const auto& domain : *domainMap)
	{
		bool isMatched = false;
		// コード値と検索文字が全一致しているかどうかを見る
		if (strSearchCode.CompareNoCase(domain.code.bstrVal) == 0) {
			isMatched = true;
		}

		// "コード値説明"と検索文字が部分一致しているか否かを見る
		if (util::partialMatch(domain.code_discription, strSearchCode)) {
			isMatched = true;
		}

		if (isMatched) {
			combo.SetCurSel(domain.idx);
			m_cControlRel.ControlChanged(comboBoxId);
			SetButton(Changed());
			// 値変更によるコンボボックスの色変更を有効にするため再描画させる
			InvalidateRect(NULL, FALSE);
			break;
		}
	}

	return true;
}

// 道路標識コードを別タブで編集可能にする場合に備えて、
// 以下のコードはコメントアウトとして残しておく
//void CAttrRoadSignPointDlg::ChangeWindowMgrMap( INT nIndex )
//{
//	switch( nIndex )
//	{
//	case EDITVIEW_ROADSIGN_KIHON:	
//		ChangeCtrlEnableToKihonTab();
//		m_winMgr.ChangeMgrMap( ROADSIGN_KIHON_MAP );
//		break;
//	case EDITVIEW_ROADSIGN_CLASS:
//		ChangeCtrlEnableToKiseiTab();
//		m_winMgr.ChangeMgrMap( ROADSIGN_CLASS_MAP );
//		break;
//	default:
//		break;
//	}
//
//	// 再配置
//	m_winMgr.InitToFitSizeFromCurrent(this);
//	m_winMgr.CalcLayout(this);
//	m_winMgr.SetWindowPositions(this);
//}

// 道路標識コードを別タブで編集可能にする場合に備えて、
// 以下のコードはコメントアウトとして残しておく
//void CAttrRoadSignPointDlg::ChangeCtrlEnableToKihonTab()
//{
//	::ShowWindow(GetDlgItem(IDC_STATIC_HORIZONTAL_ANGLE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_CHECK_VARIABLE_F), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_STATIC_REL_LANE_NODE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_VSFLEXGRID_NODE), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_STATIC_SPECULATION_C), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_COMBO_SPECULATION_C), SW_SHOW);
//
//	::ShowWindow(GetDlgItem(IDC_STATIC_ROADSIGN_C), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_COMBO_ROADSIGN_C), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_LIST_TYPE_ICON), SW_HIDE);
//}
//
//void CAttrRoadSignPointDlg::ChangeCtrlEnableToKiseiTab()
//{
//	::ShowWindow(GetDlgItem(IDC_STATIC_HORIZONTAL_ANGLE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_EDIT_HORIZONTAL_ANGLE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_CHECK_VARIABLE_F), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_STATIC_REL_LANE_NODE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_VSFLEXGRID_NODE), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_STATIC_SPECULATION_C), SW_HIDE);
//	::ShowWindow(GetDlgItem(IDC_COMBO_SPECULATION_C), SW_HIDE);
//
//	::ShowWindow(GetDlgItem(IDC_STATIC_ROADSIGN_C), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_COMBO_ROADSIGN_C), SW_SHOW);
//	::ShowWindow(GetDlgItem(IDC_LIST_TYPE_ICON), SW_SHOW);
//}
