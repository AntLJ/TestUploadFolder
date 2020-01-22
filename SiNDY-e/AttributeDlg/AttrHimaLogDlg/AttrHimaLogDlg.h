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

// AttrHimaLogDlg.h : CAttrHimaLogDlg の宣言

#ifndef __ATTRHIMALOGDLG_H_
#define __ATTRHIMALOGDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrHimaLogDlg
class CAttrHimaLogDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrHimaLogDlg();
	~CAttrHimaLogDlg()
	{
		::DeleteObject( m_hFont1 );
		::DeleteObject( m_hFont2 );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrHimaLogDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_LOG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_WORKER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SHEETID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnClickSheetID)
ALT_MSG_MAP(IDC_EDIT_KIGOU)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENTYOU_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENTYOU_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KIZON_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KIZON_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_DISP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_DISP_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TDCSTATUS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RESULT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_MATCHING)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RESULT_SHA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_REMARKS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();
		// サブクラス化
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_LOG));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_WORKER));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_SHEETID));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_KIGOU));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_GENTYOU_C));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_GENTYOU_NAME));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_KIZON_C));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_KIZON_NAME));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_DISP_C));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_DISP_NAME));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_TDCSTATUS));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_EDIT_RESULT));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_EDIT_MATCHING));
		m_wnd13.SubclassWindow(GetDlgItem(IDC_EDIT_RESULT_SHA));
		m_wnd14.SubclassWindow(GetDlgItem(IDC_EDIT_REMARKS));

		m_wnd2.SetFont( m_hFont1 );
		m_wnd5.SetFont( m_hFont2 );
		m_wnd7.SetFont( m_hFont2 );
		m_wnd9.SetFont( m_hFont2 );

		CRegKey cRegKey;
		cRegKey.Open(HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\LogHandleTool\\Himawari"));
		
		TCHAR buffer[4096];
		DWORD dwSize = sizeof(buffer) / sizeof(TCHAR);
		if(cRegKey.QueryValue(buffer, _T("RasterPath"), &dwSize) == ERROR_SUCCESS) {
			m_strRasterPath = buffer;
		}

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * コモンコントロールの色変更用イベントハンドラ
	 *
	 * フィールド値の変更が合った場合、ここでコモンコントロールの色が変わります
	 */
	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hdc = (HDC) wParam;    // handle to display context 
		HWND hWnd = (HWND) lParam; // handle to static control 

		CControlDef* def = m_cControlRel.GetControlDef( hWnd );

		if( hWnd == GetDlgItem( IDC_EDIT_SHEETID ) )
		{
			SetTextColor( hdc, RGB(0,0,255) );
			if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
			{
				if( !def->Changed() )
					SetBkMode(hdc, TRANSPARENT );
				else
					SetBkMode(hdc, RGB(255,168,126) );
			}
			else
				SetBkMode(hdc, TRANSPARENT );

			return (long)1;
			return (long)GetSysColorBrush( COLOR_WINDOW );
		}

		return ATTR_BASE_CLASS::OnCtlColorEdit( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_COMMAND 用イベントハンドラ
	 *
	 * 各コモンコントロールで変更があった場合、ここで処理します
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
		if( !m_bFinishInit ) return 0;


		INT msg = HIWORD(wParam);						// 操作メッセージ
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

		// エディットボックスの時に全選択にする
		if( msg == EN_SETFOCUS && nTargetControl != IDC_EDIT_REMARKS)
			SelectDlgItem( nTargetControl );

		// コメントのエディットボックスは全選択にしない
		if( msg == EN_SETFOCUS && nTargetControl == IDC_EDIT_REMARKS )
		{
			SetIMEMode( TRUE );
			CEdit	cString1(GetDlgItem(IDC_EDIT_REMARKS));
			cString1.SetSel(0,0,TRUE);

			LPTSTR lpszRoadCode = (LPTSTR)_alloca((cString1.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< 文字数分メモリ確保
			cString1.GetWindowText( lpszRoadCode, cString1.GetWindowTextLength() + 1 );

			if(lstrcmpi(lpszRoadCode, _T("<NULL>")) == 0)
				cString1.SetWindowText(_T(""));
		}

		// コンボボックスのリストボックスの長さを調節
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			if( nTargetControl == IDC_EDIT_REMARKS )
				SetIMEMode( FALSE );
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * TABキーでのコントロール移動、及び矢印キーの上下でコンボボックスのアイテムの変更のエミュレート
	 */
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CWindow wnd = GetFocus();
		INT iWndID = wnd.GetDlgCtrlID();

		if( wParam == VK_TAB )
		{
			if( iWndID == IDC_EDIT_REMARKS )
				return 1;

			UINT uiShiftState = GetAsyncKeyState(VK_SHIFT);
			UINT uiControlState = GetAsyncKeyState(VK_CONTROL);

			if( (_rotl(uiControlState, 1 ) & 0x01) == 0x01 )
			{
				if( (_rotl(uiShiftState, 1 ) & 0x01) == 0x01 )
					PostMessage( WM_NOTIFY, 0, -1 );
				else
					PostMessage( WM_NOTIFY, 0, -2 );
			}
			else
			{
				if( (_rotl(uiShiftState, 1 ) & 0x01) == 0x01 )
					PrevDlgCtrl();
				else
					NextDlgCtrl();
			}

			bHandled = TRUE;
			return 1;
		}
		else if( wParam == VK_RETURN && iWndID != IDC_EDIT_REMARKS)
		{
			// 編集開始状態を取得
			esriEditState state = esriStateNotEditing;
			IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
			ipEditor->get_EditState( &state );

			if( state != esriStateNotEditing )
			{
				// リターンキーが押されたItemを更新する
				CWindow wnd = GetFocus();
				INT iWndID = wnd.GetDlgCtrlID();
				m_cControlRel.ControlChanged(iWndID);

				if( !CheckReturnKeyItem(iWndID) ) 		// エンターキーが押されたItemのチェック
					return 1;				// エラー・検索動作の場合、以降の処理を抜ける

				InvalidateRect( NULL, FALSE );

				::PostMessage( GetParent(), WM_CLOSE, 0, -1);
				bHandled = TRUE;
				return 1;
			}
		}
		else if( wParam == VK_ESCAPE )
		{
			// リターンキーが押されたItemを更新する
			CWindow wnd = GetFocus();
			INT iWndID = wnd.GetDlgCtrlID();
			if( !CheckEscKeyItem(iWndID) )
				return 1;

			::PostMessage( GetParent(), WM_CLOSE, 0, 0 );
			bHandled = TRUE;
			return 1;			
		}

		bHandled = FALSE;	// 明示的に設定する必要アリ
		return 0;
	}

	/**
	 * IDC_EDIT_CLAIMNO 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnClickSheetID(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CEdit cEdit( GetDlgItem( IDC_EDIT_SHEETID ));

		INT nTextLen = cEdit.GetWindowTextLength() + 1;
		if( nTextLen > 0 && !m_strRasterPath.IsEmpty() )
		{
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			cEdit.GetWindowText( lpszText, nTextLen );
			CString strFullNumber = lpszText;

			LPCTSTR szSuffixs[2] = { _T("tif"), _T("TIF") };

			for(int i = 0; i < 2; i++) {
				CString strFilePath;
				strFilePath.Format(_T("%s\\%s\\%s.%s"), m_strRasterPath, strFullNumber.Left(5), strFullNumber, szSuffixs[i]);
				if(reinterpret_cast<int>(::ShellExecute(NULL, NULL, strFilePath, NULL, NULL, SW_SHOWNORMAL)) > 32)
					break;
			}
		}
		return 0;
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE;};
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10,m_wnd11,m_wnd12,m_wnd13,m_wnd14;	//!< 各コントロールウィンドウ
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
	CString				m_strRasterPath;
	HFONT				m_hFont1,m_hFont2;
};

#endif //__ATTRHIMALOGDLG_H_
