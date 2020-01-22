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
#include "AttrRoadmarkAreaDlg.h"
#include <boost/assign/list_of.hpp>
#include "Util.h"

using namespace sindy::schema;
using namespace adam_v2;

#include "WinMap.h"

// ポリライン描画時の設定
namespace {
	const COLORREF Color_Guideline = RGB(0xC0, 0xC0, 0x80);
	const COLORREF Color_LinkHighlight = RGB(0xD7, 0x1D, 0x3B);
}

extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrRoadmarkAreaDlg();
}

CAttrRoadmarkAreaDlg::CAttrRoadmarkAreaDlg() 
	: ATTR_BASE_CLASS(ROADMARK_AREA_MAP, IDD_ROADMARKAREADLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_COMBO_ROADMARK_C,      CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_ROADMARK_C))
		(IDC_EDIT_DESCRIPTION,      CContainedWindow(_T("EDIT"), this, IDC_EDIT_DESCRIPTION))
		(IDC_COMBO_SPECULATION_C,   CContainedWindow(_T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C))
		(IDC_EDIT_SOURCE_ID,        CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE_ID))
		(IDC_EDIT_SOURCE,           CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE ))
		;
}

LRESULT CAttrRoadmarkAreaDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// コントロールのセット
	CreateControlRelation();

	// グリッド初期化
	m_grid.init(GetDlgItem(IDC_VSFLEXGRID_LANE_LINK));

	// GridCtrlからのイベントを取得
	AtlAdviseSinkMap(this, TRUE);

	// サブクラス化
	for (auto& it : m_mapSubClass) {
		it.second.SubclassWindow(GetDlgItem(it.first));
	}

	return ATTR_BASE_CLASS::OnInitDialog(uMsg, wParam, lParam, bHandled);
}

LRESULT CAttrRoadmarkAreaDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAttrRoadmarkAreaDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, FALSE);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_grid.addRow();
	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL & bHandled)
{
	m_grid.deleteSelectedRow();

	auto oids = m_grid.getData();

	// グリッドでの削除をデータに反映
	std::vector<long> errOids;
	if (!m_relLinks[m_currentOid].reloadData(oids, errOids))
	{
		// エラーになることはないはずだが念のため
		AheMessageBox(NULL, AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID),
			AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), MB_OK | MB_TASKMODAL, NULL);
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);

	return 0;
}

HWND CAttrRoadmarkAreaDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	return ATTR_BASE_CLASS::Create(hWndParent, dwInitParam);
}

void CAttrRoadmarkAreaDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
	SetFocus();

	ClearFeatureDefs();
	if (IsWindow())
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

bool CAttrRoadmarkAreaDlg::Update()
{
	using namespace sindy::schema::adam_v2;
	using namespace uh;

	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadmark_area::kTableName) != 0)
			continue;

		long lOid(-1);
		featureDef.GetFeature()->get_OID(&lOid);

		if (!m_relLinks[lOid].isChange())
			continue;

		if (!m_relLinks[lOid].updateTable())
			return false;
	}
	return true;
}

BOOL CAttrRoadmarkAreaDlg::ErrorCheck()
{
	using namespace sindy::schema::adam_v2;

	BOOL isError = FALSE;
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), roadmark_area::kTableName) != 0)
			continue;

		// OID 取得
		long lOid = -1;
		featureDef.GetFeature()->get_OID(&lOid);

		// 関連テーブルが読み込まれていない(ツリーに存在するが開かれていない)フィーチャはチェックしない
		if (m_relLinks.count(lOid) == 0)
			continue;

		// 関連リンクの妥当性チェック
		if (m_relLinks[lOid].hasError()) {
			CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), lOid);
			AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_LINK), MB_OK | MB_TASKMODAL, NULL);
			isError = TRUE;
		}
	}

	return !isError;
}

BOOL CAttrRoadmarkAreaDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema::adam_v2;
	using namespace sindy::schema::adam_v2_adam_ipc_feature;

	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0,  bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		::SetFocus(GetDlgItem(IDC_EDIT_SOURCE));
	}

	IFeaturePtr roadmark = GetCurrentFeatureDef()->GetFeature();
	if (!roadmark)
		return FALSE;

	ITablePtr roadmarkT = AheGetTable(roadmark);
	if (!roadmarkT)
		return FALSE;

	ITablePtr relTable = AheOpenSameOwnerTable(roadmarkT, rel_roadmark_area_lane_link::kTableName);
	if (!relTable)
		return FALSE;
	ITablePtr linkTable = AheOpenSameOwnerTable(roadmarkT, lane_link::kTableName);
	if (!linkTable)
		return FALSE;


	// まだ関連テーブルを読み込んでいない道路標示エリアならばデータを読み込む
	// ツリービューでの切り替え操作時のために必要な処理
	roadmark->get_OID(&m_currentOid);
	if (m_relLinks.count(m_currentOid) == 0)
	{
		CComVariant vaGid = AheGetAttribute(roadmark, kGlobalID);
		if (vaGid.vt != VT_BSTR)
			return FALSE;

		RelLinkInfos relInfo(relTable, linkTable, vaGid.bstrVal);
		if (!relInfo.read())
			return FALSE;
		m_relLinks[m_currentOid] = relInfo;
	}

	// グリッドへデータをセット
	auto oids = m_relLinks[m_currentOid].getOIDs();
	m_grid.setData(oids);

	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return bRet;
}

BOOL CAttrRoadmarkAreaDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_relLinks[m_currentOid].isChange();
	return 0;
}

LRESULT CAttrRoadmarkAreaDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	DrawLinkageLine();
	return 0;
}

void CAttrRoadmarkAreaDlg::CreateControlRelation()
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
		m_cControlRel.SetControl(IDC_COMBO_ROADMARK_C, TYPE_COMBO, 0, 0, roadmark_area::kRoadMarkC, NULL, roadmark_area::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_DESCRIPTION, TYPE_EDITSTRING, 0, 0, roadmark_area::kDescription, NULL, roadmark_area::kTableName);
		m_cControlRel.SetControl(IDC_COMBO_SPECULATION_C, TYPE_COMBO, 0, 0, roadmark_area::kSperculationC, NULL, roadmark_area::kTableName);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE_ID, TYPE_EDITSTRING, 0, 0, roadmark_area::kSourceID, NULL, roadmark_area::kTableName, 0, -1, FOLLOW_READONLY);
		m_cControlRel.SetControl(IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, roadmark_area::kSource, NULL, roadmark_area::kTableName);
	}
}

void CAttrRoadmarkAreaDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing())
		*Cancel = VARIANT_TRUE;
}

void CAttrRoadmarkAreaDlg::OnAfterEdit(LONG Row, LONG Col)
{
	// グリッドの情報を保持しているデータ反映
	auto oids = m_grid.getData();

	std::vector<long> errOids;
	if (!m_relLinks[m_currentOid].reloadData(oids, errOids))
	{
		CString errMsg = uh::str_util::format(AheLoadString(IDS_ERR_INVALID_VALUE_IN_GRID), m_currentOid);
		AheMessageBox(NULL, errMsg, AheLoadString(IDS_ERR_RELATION_LINK), MB_OK | MB_TASKMODAL, NULL);
		return;
	}

	UpdateChanged();
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, nullptr, nullptr);
}

void CAttrRoadmarkAreaDlg::OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
{
	// 右クリック以外は何も処理しない
	if (!(Button & 0x02)) { return; }

	// 編集未開始も処理しない
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) { return; }

	// マウスアップ時の処理はグリッドコントロールクラスへ委譲
	m_grid.OnMouseUpGrid(Button, Shift, X, Y, m_hWnd, IDR_MENU_GRID);
}

void CAttrRoadmarkAreaDlg::UpdateChanged()
{
	using namespace sindy::schema::adam_v2;

	// 変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(rel_roadmark_area_lane_link::kTableName);
	pRelTableDef->setChanged(m_relLinks[m_currentOid].isChange());

	// ボタン制御
	SetButton(Changed());
}

void CAttrRoadmarkAreaDlg::DrawLinkageLine()
{
	// 再描画イベントでこの関数を呼び出しているので、
	// チェックする
	if (m_relLinks.empty())
		return;

	// 道路標示エリアのジオメトリ取得
	IFeaturePtr feature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr geo;
	feature->get_Shape(&geo);
	IPolygonPtr roadmarkPolygon(geo);
	if (!roadmarkPolygon)
		return;

	ISpatialReferencePtr sr;
	roadmarkPolygon->get_SpatialReference(&sr);

	// 紐づくリンクの形状群を取得
	auto linkPolylines = m_relLinks[m_currentOid].getLinkPolylines();

	for (const auto polyline : linkPolylines)
	{
		IPointPtr fromPoint, toPoint;
		if (!util::getNearestPoint(roadmarkPolygon, polyline, fromPoint, toPoint))
			continue;

		IPolylinePtr drawLine(CLSID_Polyline);
		drawLine->putref_SpatialReference(sr);
		drawLine->put_FromPoint(fromPoint);
		drawLine->put_ToPoint(toPoint);

		DrawPolyLine(drawLine, Color_Guideline);
		DrawPolyLine(polyline, Color_LinkHighlight);
	}
}

void CAttrRoadmarkAreaDlg::DrawPolyLine(const IPolylinePtr & polyline, const DWORD & color)
{
	AheDrawPolyline(m_cArcHelper.GetDisplay(), polyline, color, 5, esriSLSSolid, TRUE, TRUE);
}
