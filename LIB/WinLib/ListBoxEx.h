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

// ListBoxEx.h: CListBoxEx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTBOXEX_H__344A3867_783D_49AC_B65C_1070738E87F9__INCLUDED_)
#define AFX_LISTBOXEX_H__344A3867_783D_49AC_B65C_1070738E87F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __cplusplus
   #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
   #error EditListBox.h requires atlctrls.h to be included first
#endif

#if 0
#include <atlmisc.h>

#define TID_SCROLLDOWN  100
#define TID_SCROLLUP    101

/**
 * @class CListBoxT
 * @biref <b>リストボックス内項目のドラッグ＆ドロップによる並び替えをサポートした拡張リストボックスクラス</b>\n
 * このクラスは「THE CODE PROJECT」上にある「Drag and Drop ListBox Items」をWTLで使用できるように修正したものです。
 * @see http://www.codeproject.com/combobox/DragAndDropListBox.asp
 */
template<class T, class TBase = CListBox, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CListBoxExT	: public CWindowImpl<T, TBase, TWinTraits>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CListBox::GetWndClassName())
	CListBoxExT() : 
		m_MovingIndex(-1),
		m_MoveToIndex(-1),
		m_Captured(FALSE),
		m_Interval(0)
	{
	}

//	CListBoxExT< TBase >& operator =(HWND hWnd)
//	{
//		m_hWnd = hWnd;
//		return *this;
//	}
BEGIN_MSG_MAP(CListBoxExT)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
//		CListBox::OnLButtonDown(nFlags, point);

		POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		//clear all the flags
		m_MovingIndex = LB_ERR;
		m_MoveToIndex = LB_ERR;
		m_Captured = FALSE;
		m_Interval = 0;
		BOOL Outside;
		//find the item that they want to drag
		//and keep track of it. Later in the mouse move
		//we will capture the mouse if this value is set
		int Index = ItemFromPoint(point,Outside);
		if (Index != LB_ERR && !Outside)
		{
			m_MovingIndex = Index;
			SetCurSel(Index);
		}

		return 0;
	}
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
//		CListBox::OnMouseMove(nFlags, point);
		int nFlags = wParam;
		POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		if (nFlags & MK_LBUTTON)
		{
			if (m_MovingIndex != LB_ERR && !m_Captured)
			{
				SetCapture();
				m_Captured = TRUE;
			}
			BOOL Outside;
			int Index = ItemFromPoint(point,Outside);
			//if they our not on a particular item
			if (Outside)
			{
				CRect ClientRect;
				GetClientRect(&ClientRect);
				
				//if they are still within the listbox window, then
				//simply select the last item as the drop point
				//else if they are outside the window then scroll the items
				if (ClientRect.PtInRect(point))
				{
					KillTimer(TID_SCROLLDOWN);
					KillTimer(TID_SCROLLUP);
					m_Interval = 0;
					Index = LB_ERR;
					Outside = FALSE;
				}
				else
				{
					DoTheScrolling(point,ClientRect);
				}
			}
			else
			{
				KillTimer(TID_SCROLLDOWN);
				KillTimer(TID_SCROLLUP);
				m_Interval = 0;
			}
			
			if (Index != m_MoveToIndex && !Outside)
			{
				DrawTheLines(Index);
			}
		}
		return 0;
	}
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		if (m_MovingIndex != LB_ERR && m_Captured)
		{
			KillTimer(TID_SCROLLDOWN);
			KillTimer(TID_SCROLLUP);
			m_Interval = 0;
			m_Captured = FALSE;
			ReleaseCapture();

			CRect Rect;
			GetClientRect(&Rect);
			//if they are still within the listbox window
			if (Rect.PtInRect(point))
			{
				InsertDraggedItem();
			}
			else
			{
				Invalidate();
				UpdateWindow();
			}
			m_MovingIndex = LB_ERR;
			m_MoveToIndex = LB_ERR;
		}

//		CListBox::OnLButtonUp(nFlags, point);

		return 0;
	}
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		int nIDEvent = wParam;
		if (nIDEvent == TID_SCROLLDOWN)
		{
			DrawTheLines(m_MoveToIndex+1);
			SetTopIndex(GetTopIndex()+1);
		}
		else if (nIDEvent == TID_SCROLLUP)
		{
			DrawTheLines(m_MoveToIndex-1);
			SetTopIndex(GetTopIndex()-1);
		}
		
		//	   CListBox::OnTimer(nIDEvent);
		return 0;
	}
private:
   int   m_MovingIndex;
   int   m_MoveToIndex;
   BOOL  m_Captured;
   DWORD m_Interval;    //scroll speed when an item is dragged above or below the window

   void DrawTheLines(int Index)
   {
	   CRect ClientRect;
	   GetClientRect(&ClientRect);
	   
	   CClientDC cDC( m_hWnd );
	   
	   CRect Rect;
	   int Width = 2;
	   if (m_MoveToIndex != m_MovingIndex)
	   {
		   CPen Pen;
		   Pen.CreatePen(PS_SOLID,Width,RGB(255,255,255));
		   CPenHandle cOldPen( cDC.SelectPen((HPEN)Pen) );
		   if (m_MoveToIndex != LB_ERR)
		   {
			   GetItemRect(m_MoveToIndex,&Rect);
			   if (ClientRect.PtInRect(Rect.TopLeft()))
			   {
				   cDC.MoveTo(Rect.left,Rect.top+1);
				   cDC.LineTo(Rect.right-(Width/2),Rect.top+1);
			   }
		   }
		   else
		   {
			   GetItemRect(GetCount()-1,&Rect);
			   if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
			   {
				   cDC.MoveTo(Rect.left,Rect.bottom);
				   cDC.LineTo(Rect.right-(Width/2),Rect.bottom);
			   }
		   }
		   cDC.SelectPen((HPEN)cOldPen);
	   }
	   m_MoveToIndex = Index;
	   if (m_MoveToIndex != m_MovingIndex)
	   {
		   CPen Pen;
		   Pen.CreatePen(PS_SOLID,Width,RGB(0,0,0));
		   CPenHandle cOldPen( cDC.SelectPen((HPEN)Pen) );
		   if (m_MoveToIndex != LB_ERR)
		   {
			   GetItemRect(Index,&Rect);
			   if (ClientRect.PtInRect(Rect.TopLeft()))
			   {
				   cDC.MoveTo(Rect.left,Rect.top+1);
				   cDC.LineTo(Rect.right-(Width/2),Rect.top+1);
			   }
		   }
		   else
		   {
			   GetItemRect(GetCount()-1,&Rect);
			   if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
			   {
				   cDC.MoveTo(Rect.left,Rect.bottom);
				   cDC.LineTo(Rect.right-(Width/2),Rect.bottom);
			   }
		   }
		   cDC.SelectPen((HPEN)cOldPen);
	   }
	   ReleaseDC((HDC)cDC);
   }
   
   void InsertDraggedItem()
   {
	   USES_CONVERSION;

	   BSTR bstrText = 0;
	   DWORD_PTR ItemData;
	   GetTextBSTR(m_MovingIndex,bstrText);
	   ItemData = GetItemData(m_MovingIndex);
	   if (m_MovingIndex < m_MoveToIndex)
	   {
		   SetRedraw(FALSE);
		   int TopIndex = GetTopIndex();
		   int NewIndex = InsertString(m_MoveToIndex,OLE2T(bstrText));
		   SetItemData(NewIndex,ItemData);
		   DeleteString(m_MovingIndex);
		   SetCurSel(NewIndex-1);
		   SetTopIndex(TopIndex);
		   SetRedraw(TRUE);
		   //we have to redraw the window since we are deleting a string
		   //after we set the current selection. DeleteString causes a 
		   //focus rect to show up under the current selection
		   Invalidate();
		   UpdateWindow();
	   }
	   else if (m_MovingIndex > m_MoveToIndex)
	   {
		   SetRedraw(FALSE);
		   int TopIndex = GetTopIndex();
		   DeleteString(m_MovingIndex);
		   int NewIndex = InsertString(m_MoveToIndex,OLE2T(bstrText));
		   SetItemData(NewIndex,ItemData);
		   SetCurSel(NewIndex);
		   SetTopIndex(TopIndex);
		   SetRedraw(TRUE);
		   //we have to redraw the window since we are deleting a string
		   //after we set the current selection. DeleteString causes a 
		   //focus rect to show up under the current selection
		   Invalidate();
		   UpdateWindow();
	   }
	   ::SysFreeString(bstrText);
	   ::SendMessage(GetParent(),WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),LBN_SELCHANGE),(LPARAM)m_hWnd);
   }
   
   void DoTheScrolling(CPoint Point,CRect ClientRect)
   {
	   if (Point.y > ClientRect.Height())
	   {
		   DWORD Interval = 250 / (1+ ((Point.y-ClientRect.Height())/GetItemHeight(0)));
		   if (Interval != m_Interval)
		   {
			   m_Interval = Interval;
			   SetTimer(TID_SCROLLDOWN,Interval);
			   //OnTimer(TID_SCROLLDOWN);
			   SendMessage( m_hWnd, WM_TIMER, TID_SCROLLDOWN, 0 );
		   }
	   }
	   else if (Point.y < 0)
	   {
		   DWORD Interval = 250 / (1+(abs(Point.y)/GetItemHeight(1)));
		   if (Interval != m_Interval)
		   {
			   m_Interval = Interval;
			   SetTimer(TID_SCROLLUP,Interval);
			   //OnTimer(TID_SCROLLUP);
			   SendMessage( m_hWnd, WM_TIMER, TID_SCROLLUP, 0 );
		   }
	   }
	   else
	   {
		   KillTimer(TID_SCROLLDOWN);
		   KillTimer(TID_SCROLLUP);
		   m_Interval = 0;
	   }
   }
};

class CListBoxEx : public CListBoxExT<CListBoxEx>
{
public:
	DECLARE_WND_CLASS(_T("WTL_ListBoxEx"));
};



#endif // if 0



// EditListBox Window styles
#define EDLS_NOMOVEBUTTONS   0x00000001
#define EDLS_BROWSE          0x00000002

// EditListBox Notification messages
#define EDLN_FIRST           (0U-1900U)
#define EDLN_LAST            (0U-1950U)

#define EDLN_BROWSE          (EDLN_FIRST-1)
#define EDLN_ITEMCHANGING    (EDLN_FIRST-2)
#define EDLN_ITEMCHANGED     (EDLN_FIRST-3)
#define EDLN_INSERTITEM      (EDLN_FIRST-4)
#define EDLN_DELETEITEM      (EDLN_FIRST-5)
#define EDLN_BEGINLABELEDIT  (EDLN_FIRST-6)
#define EDLN_ENDLABELEDIT    (EDLN_FIRST-7)
#define EDLN_ITEMMOVED       (EDLN_FIRST-8)	// アイテムがドラッグで移動したことを通知するメッセージ

typedef struct 
{
    NMHDR hdr;
    int iIndex;
} NMEDITLIST, *PNMEDITLIST;

typedef struct
{
	NMHDR hdr;
	int iIndexBefore;
	int iIndexAfter;
} NMITEMDRAG, *PNMITEMDRAG;

// Internal EditListBox command messages
#define EDLM_NEW        0x0210
#define EDLM_DELETE     0x0211
#define EDLM_ITEMUP     0x0212
#define EDLM_ITEMDOWN   0x0213

#define WM_EDITLIST_CLEANUP WM_USER+124
/**
 * @class CListBoxEx2T
 * @brief <b>リストボックス拡張クラス</b>\n
 * このクラスは、WTLのCDragListBoxを使用し、且つドラッグ処理も自分のクラスで
 * 行う拡張クラスです。また、各アイテムのオンザフライ編集に対応しています。
 */
template<class T, class TBase = CDragListBox, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CListBoxExT : public CWindowImpl<T, TBase, TWinTraits>, CDragListNotifyImpl<CListBoxExT<T> >
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CDragListBox::GetWndClassName())
BEGIN_MSG_MAP(CListBoxExT)
	CHAIN_MSG_MAP(CDragListNotifyImpl<CListBoxExT>)
	COMMAND_CODE_HANDLER(LBN_DBLCLK, OnDblClick)
ALT_MSG_MAP(1) // Edit control
	MESSAGE_HANDLER(WM_GETDLGCODE, OnEditGetDlgCode)
	MESSAGE_HANDLER(WM_CHAR, OnEditKey)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnEditLostFocus)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

	LRESULT OnDblClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BeginEdit( GetCurSel() );
		return 0;
	}
	LRESULT OnEditGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		return DefWindowProc(uMsg, wParam, lParam) | DLGC_WANTALLKEYS;
	}
	LRESULT OnEditLostFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Check if we lost focus to the browse button
		HWND hwndNew = lParam==0 ? ::GetFocus() : (HWND)lParam;
		//if( hwndNew==m_wndBrowse ) return DefWindowProc(uMsg, wParam, lParam);
		
		int iIndex = GetCurSel();
		SendNotifyMsg(EDLN_ENDLABELEDIT, iIndex);
		// Hide controls
		m_wndEdit.ShowWindow(SW_HIDE);
		//m_wndBrowse.ShowWindow(SW_HIDE);
		// Ask parent if it accepts the change
		if( SendNotifyMsg(EDLN_ITEMCHANGING, iIndex)==0 ) {
			// Owner accepted text change.
			// Copy edit text to selected listview item
			int len = m_wndEdit.GetWindowTextLength()+1;
			LPTSTR pstr = (LPTSTR)_alloca(len*sizeof(TCHAR));
			ATLASSERT(pstr);
			m_wndEdit.GetWindowText(pstr, len);        
			SetItemText(iIndex, pstr);
			// Send "Item Changed" notify message
			SendNotifyMsg(EDLN_ITEMCHANGED, iIndex);
		}
		SetCurSel(iIndex);

		PostMessage(WM_EDITLIST_CLEANUP);
		bHandled = FALSE; // Windows needs this to disable cursor

		// Delete EDIT control
		m_wndEdit.DestroyWindow();

		return 0;
	}
	LRESULT OnEditKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		switch( wParam ) {
		case VK_ESCAPE:
			{
				// Copy original listview text to edit control.
				// When the edit control looses focus, it will
				// transfer this text back to the list.
				int iIndex = GetCurSel();
				int len = GetTextLen(iIndex)+1;
				LPTSTR pstr = (LPTSTR)_alloca(len*sizeof(TCHAR));
				ATLASSERT(pstr);
				GetText(iIndex, pstr);
				m_wndEdit.SetWindowText(pstr);           
			}
			// FALL THROUGH...
		case VK_RETURN:
			{
				SetFocus(); // Causes Edit WM_KILLFOCUS
				return 0;
			}
		}
		bHandled = FALSE;
		return 0;
	}

public:
    BOOL OnBeginDrag(int nCtlID, HWND hWndDragList, POINT ptCursor){
        // カーソル位置のアイテムの左側に挿入マークを描画
        //DrawInsert(LBItemFromPt(ptCursor));
        return TRUE;
    }

    void OnCancelDrag(int nCtlID, HWND hWndDragList, POINT ptCursor){
        // 挿入マークを消去
        DrawInsert(-1);
    }

    int OnDragging(int nCtlID, HWND hWndDragList, POINT ptCursor){
        // カーソル位置のアイテムの左側に挿入マークを描画
        DrawInsert(LBItemFromPt(ptCursor));
        return 0;
    }

    void OnDropped(int nCtlID, HWND hWndDragList, POINT ptCursor){
        // 挿入マークを消去
        DrawInsert(-1);

        int nSrcIndex = GetCurSel();              // 移動元のインデックス
        int nDestIndex = LBItemFromPt(ptCursor);  // 移動先のインデックス

        if(nSrcIndex == -1 || nDestIndex == -1 || nDestIndex == nSrcIndex)
            return;

		USES_CONVERSION;

        BSTR bstrText = 0;
        GetTextBSTR(nSrcIndex, bstrText);
        DeleteString(nSrcIndex);

        InsertString(nDestIndex, OLE2CT(bstrText));
        SetCurSel(nDestIndex);
		::SysFreeString(bstrText);

		// メッセージ通知
		NMITEMDRAG nm = { 0 };
		nm.hdr.hwndFrom = m_hWnd;
		nm.hdr.idFrom = GetDlgCtrlID();
		nm.hdr.code = EDLN_ITEMMOVED;
		nm.iIndexBefore = nSrcIndex;
		nm.iIndexAfter  = nDestIndex;
		::SendMessage(GetParent(), WM_NOTIFY, nm.hdr.idFrom, (LPARAM)&nm);
    }

	void BeginEdit(int iIndex)
	{
		CreateEditBox();

		ATLASSERT(iIndex>=0);
		if( iIndex<0 ) return;
		// Allow owner to cancel action
		if( SendNotifyMsg(EDLN_BEGINLABELEDIT, iIndex) != 0 ) return;

		// Copy text to Edit control
		int len = GetTextLen(iIndex)+1;
		LPTSTR pstr = (LPTSTR)_alloca(len*sizeof(TCHAR));
		ATLASSERT(pstr);
		GetText(iIndex, pstr);
		m_wndEdit.SetWindowText(pstr);

		RECT rc;
		GetItemRect(iIndex, &rc);
/*		if( GetStyle() & EDLS_BROWSE ) {
			 int cx = rc.bottom - rc.top;
			 RECT rcBrowse = { rc.right-cx, rc.top, rc.right, rc.bottom };
			 m_wndList.MapWindowPoints(m_hWnd, &rcBrowse);
			 m_wndBrowse.SetWindowPos(HWND_TOP, &rcBrowse, SWP_SHOWWINDOW|SWP_NOACTIVATE);
			 rc.right -= cx;
		} ブラウズボタン用コード*/

		m_wndEdit.SetWindowPos(HWND_TOP, &rc, SWP_SHOWWINDOW);
		m_wndEdit.SetSel(0,-1);
		m_wndEdit.SetFocus();
	}
private:
	BOOL SetItemText(int iIndex, LPCTSTR pstrText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(iIndex>=0);
		ATLASSERT(!::IsBadStringPtr(pstrText,-1));
		DeleteString(iIndex);
		return InsertString(iIndex, pstrText) != LB_ERR;
	}
	LRESULT SendNotifyMsg(UINT code, int iIndex) const
	{
		NMEDITLIST nm = { 0 };
		nm.hdr.hwndFrom = m_hWnd;
		nm.hdr.idFrom = GetDlgCtrlID();
		nm.hdr.code = code;
		nm.iIndex = iIndex;
		return ::SendMessage(GetParent(), WM_NOTIFY, nm.hdr.idFrom, (LPARAM)&nm);
	}
	void CreateEditBox()
	{
		RECT rc;
		GetClientRect(&rc);

		m_wndEdit.Create(this, 1, m_hWnd, &rcDefault, NULL, WS_BORDER | WS_CHILD | ES_AUTOHSCROLL);
		ATLASSERT(m_wndEdit.IsWindow());
		m_wndEdit.SetFont(AtlGetDefaultGuiFont());
	}
private:
	CContainedWindowT<CEdit>	m_wndEdit;				// オンザフライ編集用エディットコントロール
 
};

class CListBoxEx : public CListBoxExT<CListBoxEx>
{
public:
	DECLARE_WND_CLASS(_T("WTL_ListBoxEx"));
};

#endif // !defined(AFX_LISTBOXEX_H__344A3867_783D_49AC_B65C_1070738E87F9__INCLUDED_)
