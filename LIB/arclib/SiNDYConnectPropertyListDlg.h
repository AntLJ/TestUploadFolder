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

// SiNDYConnectPropertyListDlg.h : CSiNDYConnectPropertyListDlg の宣言

#ifndef __SINDYCONNECTPROPERTYLISTDLG_H_
#define __SINDYCONNECTPROPERTYLISTDLG_H_

#include "../../../LIB/arclib/Resource.h"       // メイン シンボル
#include <atlwin.h>
#include <commctrl.h>
#include "ATLControls.h"
#include "SiNDYConnectPropertySetList.h"
#include "SiNDYConnectPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSiNDYConnectPropertyListDlg
/////////////////////////////////////////////////////////////////////////////

class CSiNDYConnectPropertyListDlg : 
	public CAxDialogImpl<CSiNDYConnectPropertyListDlg>
{
public:
	CSiNDYConnectPropertyListDlg();

	enum { IDD = SG_IDD_SINDYCONNECTPROPERTYLISTDLG };

BEGIN_MSG_MAP(CSiNDYConnectPropertyListDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_HANDLER(SG_IDC_BUTTON_ADD, BN_CLICKED, OnClickedButtonAdd)
	COMMAND_HANDLER(SG_IDC_BUTTON_DELETE, BN_CLICKED, OnClickedButtonDelete)
	COMMAND_HANDLER(SG_IDC_BUTTON_EDIT, BN_CLICKED, OnClickedButtonEdit)
	NOTIFY_HANDLER(SG_IDC_LIST_CONNECTPROPERTY, LVN_ITEMCHANGED, OnItemchangedListConnectProperty)
	NOTIFY_HANDLER(SG_IDC_LIST_CONNECTPROPERTY, LVN_BEGINDRAG, OnBegindragListConnectProperty)
	NOTIFY_HANDLER(SG_IDC_LIST_CONNECTPROPERTY, NM_DBLCLK, OnDblclkListConnectProperty)

ALT_MSG_MAP(SG_IDC_LIST_CONNECTPROPERTY)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMoveListConnectProperty)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownListConnectProperty)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUpListConnectProperty)

END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedButtonAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedButtonDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedButtonEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnItemchangedListConnectProperty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnBegindragListConnectProperty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblclkListConnectProperty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnMouseMoveListConnectProperty(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDownListConnectProperty(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUpListConnectProperty(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
public:
// 設定/取得
	void setSelectMode(bool bSelectMode) { m_bSelectMode = bSelectMode; }
	void setTitle(LPCTSTR lpszTitle);

	int getSelectedConnectPropertySet(SiNDYConnectPropertySet& rConnectPropertySet);

	int HitTest(const POINTS& rPoints) const { return HitTest(rPoints.x, rPoints.y); }
	int HitTest(LONG x, LONG y) const;

private:
	void edit();

// プロパティ
	CContainedWindowT<ATLControls::CListViewCtrl> m_lcConnectProperty;
	bool m_bSelectMode;
	CString m_strTitle;
	SiNDYConnectPropertySet m_cSelectedConnectPropertySetAfterDestroy;

	bool m_bDragging;
	ATLControls::CImageList m_cDragImageList;
	POINT m_ptWhenLButtonDowned;
};

	/////////////////////////////////////////////////////////////////////////////
// CSiNDYConnectPropertyListDlg

inline CSiNDYConnectPropertyListDlg::CSiNDYConnectPropertyListDlg() :
m_lcConnectProperty(_T("LISTCTRL"), this, SG_IDC_LIST_CONNECTPROPERTY),
m_bSelectMode(false),
m_bDragging(false)
{
}

/////////////////////////////////////////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////////////////////////////////////////

inline LRESULT CSiNDYConnectPropertyListDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_lcConnectProperty.SubclassWindow(GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY));

	SiNDYConnectPropertySetList cList;
	cList.setFromRegistry();

	ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
	lc.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	RECT rect;
	lc.GetClientRect(&rect);
	lc.InsertColumn(0, _T("接続プロパティ"), LVCFMT_LEFT, rect.right - rect.left, -1);
	for(int i = 0; i < (int)cList.size(); i++) {
		const SiNDYConnectPropertySet& rPropertySet = cList[i];
		lc.InsertItem(i, rPropertySet.getPropertiesString());
	}

	if(! m_strTitle.IsEmpty()) {
		SetWindowText(m_strTitle);
	}

	if(m_bSelectMode) {
		::EnableWindow(GetDlgItem(IDOK), FALSE);
	}

	m_cSelectedConnectPropertySetAfterDestroy.clear();

	return 1;  // システムにフォーカスを設定させます
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	getSelectedConnectPropertySet(m_cSelectedConnectPropertySetAfterDestroy);

	SiNDYConnectPropertySetList cList;

	ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
	for(int i = 0; i < lc.GetItemCount(); i++) {
		TCHAR buffer[256];
		lc.GetItemText(i, 0, buffer, 256);
		cList.push_back(SiNDYConnectPropertySet(buffer));
	}

	cList.writeToRegistry();

	EndDialog(wID);
	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnClickedButtonAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
	SiNDYConnectPropertySet cPropertySet;
	getSelectedConnectPropertySet(cPropertySet);

	CSiNDYConnectPropertyDlg cDlg(cPropertySet);
	if(cDlg.DoModal() == IDOK) {
		lc.InsertItem(lc.GetItemCount(), cPropertySet.getPropertiesString());
	}

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnClickedButtonDelete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
	int nIndex = lc.GetSelectedIndex();
	if(nIndex >= 0) {
		lc.DeleteItem(nIndex);
	}

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnClickedButtonEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	edit();

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnItemchangedListConnectProperty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
	BOOL bEditable = lc.GetSelectedIndex() >= 0;
	::EnableWindow(GetDlgItem(SG_IDC_BUTTON_EDIT), bEditable);
	::EnableWindow(GetDlgItem(SG_IDC_BUTTON_DELETE), bEditable);
	if(m_bSelectMode) {
		::EnableWindow(GetDlgItem(IDOK), bEditable);
	}

	bHandled = FALSE;

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnBegindragListConnectProperty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	POINT pt;
	int nIndex = m_lcConnectProperty.GetSelectedIndex();
	if(nIndex >= 0) {
		m_cDragImageList = m_lcConnectProperty.CreateDragImage(nIndex, &pt);
	}

	DWORD dw = GetMessagePos();
	POINT ptCursor = { MAKEPOINTS(dw).x, MAKEPOINTS(dw).y };
	POINT ptHotSpot = { m_ptWhenLButtonDowned.x - pt.x, m_ptWhenLButtonDowned.y - pt.y };
	m_cDragImageList.BeginDrag(0, ptHotSpot);
	m_cDragImageList.DragEnter(m_lcConnectProperty.m_hWnd, ptCursor);
	m_bDragging = true;

	m_lcConnectProperty.SetCapture();
	
	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnDblclkListConnectProperty(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	edit();

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnMouseMoveListConnectProperty(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLControls::CListViewCtrl& lc = m_lcConnectProperty;

	if(! m_bDragging)
		return 0;

	POINT pt = { MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y };

	m_cDragImageList.DragMove(pt);

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnLButtonDownListConnectProperty(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;

	m_ptWhenLButtonDowned.x = MAKEPOINTS(lParam).x;
	m_ptWhenLButtonDowned.y = MAKEPOINTS(lParam).y;

	return 0;
}

inline LRESULT CSiNDYConnectPropertyListDlg::OnLButtonUpListConnectProperty(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(! m_bDragging)
		return 0;

	ReleaseCapture();

	m_bDragging = 0;
	m_cDragImageList.DragLeave(m_lcConnectProperty.m_hWnd);
	m_cDragImageList.EndDrag();
	m_cDragImageList.Destroy();

	int nToIndex = HitTest(MAKEPOINTS(lParam));
	if(nToIndex < 0)
		return 0;

	int nFromIndex = m_lcConnectProperty.GetSelectedIndex();
	if(nFromIndex < 0 || nToIndex == nFromIndex)
		return 0;

	TCHAR buffer[256];

	m_lcConnectProperty.GetItemText(nFromIndex, 0, buffer, sizeof(buffer));
	CString strName = buffer;

	m_lcConnectProperty.GetItemText(nFromIndex, 1, buffer, sizeof(buffer));
	CString strRoadCode = buffer;

	m_lcConnectProperty.GetItemText(nFromIndex, 2, buffer, sizeof(buffer));
	CString strRoadSeq = buffer;

	DWORD dwData = m_lcConnectProperty.GetItemData(nFromIndex);

	if(nFromIndex > nToIndex) {
		m_lcConnectProperty.DeleteItem(nFromIndex);
	}

	m_lcConnectProperty.InsertItem(nToIndex, strName);
	m_lcConnectProperty.SetItemText(nToIndex, 1, strRoadCode);
	m_lcConnectProperty.SetItemText(nToIndex, 2, strRoadSeq);
	m_lcConnectProperty.SetItemData(nToIndex, dwData);

	if(nFromIndex < nToIndex) {
		m_lcConnectProperty.DeleteItem(nFromIndex);
	}

	return 0;
}

// 設定/取得
/////////////////////////////////////////////////////////////////////////////

inline void CSiNDYConnectPropertyListDlg::setTitle(LPCTSTR lpszTitle)
{
	if(lpszTitle) {
		m_strTitle = lpszTitle;
	}
	else {
		m_strTitle.Empty();
	}
}

inline int CSiNDYConnectPropertyListDlg::HitTest(LONG x, LONG y) const
{
	RECT rect;
	m_lcConnectProperty.GetClientRect(&rect);
	if(x < rect.left || y < rect.top || x > rect.right || y > rect.bottom)
		return -1;

	LVHITTESTINFO lvhti;
	ZeroMemory(&lvhti, sizeof(lvhti));
	lvhti.pt.x = x;
	lvhti.pt.y = y;

	m_lcConnectProperty.HitTest(&lvhti);

	int nResult = lvhti.iItem;
	if(nResult <= 0) {
		POINT ptLastItem;
		m_lcConnectProperty.GetItemPosition(m_lcConnectProperty.GetItemCount(), &ptLastItem);
		if(y >= ptLastItem.y) {
			nResult = m_lcConnectProperty.GetItemCount();
		}
	}

	if (((lvhti.flags & LVHT_ONITEMLABEL) == 0) && ((lvhti.flags & LVHT_ONITEMSTATEICON) == 0))
		return -1;

	return nResult;
}

inline int CSiNDYConnectPropertyListDlg::getSelectedConnectPropertySet(SiNDYConnectPropertySet& rConnectPropertySet)
{
	if(IsWindow()) {
		ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
		int nIndex = lc.GetSelectedIndex();
		if(nIndex >= 0) {
			TCHAR buffer[256];
			lc.GetItemText(nIndex, 0, buffer, 256);
			rConnectPropertySet.setPropertiesString(buffer);
		}
		return nIndex;
	}
	else {
		rConnectPropertySet = m_cSelectedConnectPropertySetAfterDestroy;
		return -1;
	}
}

// 処理
/////////////////////////////////////////////////////////////////////////////

inline void CSiNDYConnectPropertyListDlg::edit()
{
	ATLControls::CListViewCtrl lc = GetDlgItem(SG_IDC_LIST_CONNECTPROPERTY);
	SiNDYConnectPropertySet cPropertySet;
	int nIndex = getSelectedConnectPropertySet(cPropertySet);

	CSiNDYConnectPropertyDlg cDlg(cPropertySet);
	if(nIndex >= 0 && cDlg.DoModal() == IDOK) {
		lc.SetItemText(nIndex, 0, cPropertySet.getPropertiesString());
	}
}

#endif //__SINDYCONNECTPROPERTYLISTDLG_H_
