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

// AttrClaimPolyMapDlg.h : CAttrClaimPolyMapDlg の宣言

#ifndef __ATTRCLAIMPOLYMAPDLG_H_
#define __ATTRCLAIMPOLYMADPLG_H_

#include "resource.h"
#include "WinLib/str2.h"
#include "WinLib/charutil.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrClaimPolyMapDlg
class CAttrClaimPolyMapDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrClaimPolyMapDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_CLAIMNO, EN_UPDATE, OnUpdateClaimNo)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_COMBO_CLAIMTYPE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_CLAIMNO)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_COMFIRM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_USELESS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_REMARKS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_CLAIMTYPE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_CLAIMNO));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_CHECK_COMFIRM));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_CHECK_USELESS));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_REMARKS));

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
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
		if( msg == EN_SETFOCUS && nTargetControl == IDC_EDIT_REMARKS)
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

		// メモ欄からフォーカス外れたらIMEを切る
		if( nTargetControl == IDC_EDIT_REMARKS && msg == EN_KILLFOCUS)
			SetIMEMode( FALSE );

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
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

				if (CheckReturnKeyItem(iWndID) == FALSE) 		// エンターキーが押されたItemのチェック
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
			if( CheckEscKeyItem(iWndID) == FALSE)
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
	LRESULT OnUpdateClaimNo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> の場合を考慮
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				LPCTSTR lpcszText = lpszText;
				INT nTextLen = lstrlen( lpcszText ) + 1;
				LPTSTR lpszText2;

				lpszText2 = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );	// 同じ領域を確保
				// 全て半角にしてしまう
				LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText2, nTextLen );

				CString strText( lpszText2 );
				CString strRet;
				LONG lLen = CString_GetLength2( strText );

				USES_CONVERSION;

				INT  nOcnt   = 0;		// 出力文字数
				BOOL bMinus  = FALSE;
				BOOL bPeriod = FALSE;
				for( LONG i = 0; i < lLen; i++ )
				{
					CString str = CString_GetAt2( strText, i );
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

					// 半角の時、許されるもの
					// 数字
					if ( isasciinumber( lpstr[0] ) )
					{
						;
					}
					// 大文字英字
					else if (isasciialphabet_large(lpstr[0])) {
						;
					}
					// 小文字英字
					else if (isasciialphabet_small(lpstr[0])) {
						;
					}
					// カンマ
					else if ( 0x2c == (unsigned char)((lpstr[0])&0xff) ){
						;
					}
					// マイナス
					else if ( isasciiminus(lpstr[0]) ){
						;
					}
					else
						continue;

					strRet += A2CT( lpstr );
					nOcnt++;
				}				
							
				if( strRet.Compare( lpszText ) != 0 )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// 入力位置の修正
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strRet );
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg メンバ関数定義
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
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrClaimPolyMapDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4;	//!< 各コントロールウィンドウ
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
};

#endif //__ATTRCLAIMPOLYMAPDLG_H_
