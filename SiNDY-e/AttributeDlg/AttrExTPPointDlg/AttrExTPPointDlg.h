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


#ifndef _ATTREXTPPOINTDLG_
#define _ATTREXTPPOINTDLG_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrExTPPointDlg
class CAttrExTPPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrExTPPointDlg();
	~CAttrExTPPointDlg()
	{
		::DeleteObject( m_hFont );
	}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrExTPPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_ADDRCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDRNAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KATAGAKI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KEISAI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_POINTSTAT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ADDRGP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_INFOSRC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール作成
		CreateControlRelation();

		// サブクラス化
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_ADDRCODE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_ADDRNAME));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_KATAGAKI));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_KEISAI));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_POINTSTAT));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_CHECK_ADDRGP));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_INFOSRC));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR1));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR2));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR3));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR4));

		m_wnd1.SetFont( m_hFont );
		m_wnd2.SetFont( m_hFont );
		m_wnd3.SetFont( m_hFont );
		m_wnd4.SetFont( m_hFont );
		m_wnd7.SetFont( m_hFont );
		m_wnd8.SetFont( m_hFont );
		m_wnd9.SetFont( m_hFont );
		m_wnd10.SetFont( m_hFont );
		m_wnd11.SetFont( m_hFont );

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

		// 20桁コードを分解したエディットボックスの背景色を変更
		HWND hWndAddr1,hWndAddr2,hWndAddr3,hWndAddr4;
		hWndAddr1 = CWindow( GetDlgItem( IDC_EDIT_ADDR1 ) );
		hWndAddr2 = CWindow( GetDlgItem( IDC_EDIT_ADDR2 ) );
		hWndAddr3 = CWindow( GetDlgItem( IDC_EDIT_ADDR3 ) );
		hWndAddr4 = CWindow( GetDlgItem( IDC_EDIT_ADDR4 ) );

		if( hWndAddr1 == hWnd || hWndAddr2 == hWnd || hWndAddr3 == hWnd || hWndAddr4 == hWnd )
		{
			if ( !m_hBrush2 )
				m_hBrush2 = CreateSolidBrush( RGB(204,236,255) );

			// Get the DC provided and set the appropriate attributes

			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(150,240,255));

			// Return a handle to the brush
			HBRUSH far* hbr = (HBRUSH far*) m_hBrush2;
			return ((DWORD) hbr);
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
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// コンボボックスのリストボックスの長さを調節
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// ダイアログを更新領域に追加します
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg メンバ関数定義
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
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strFeatureClassName = lpcszFeatureClassName;};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void DevideAddrCode();
	void SetToolTip(INT nTargetItem);

////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5,m_wnd6,
						m_wnd7,m_wnd8,m_wnd9,m_wnd10,m_wnd11;//!< 各コントロールウィンドウ
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
	HFONT				m_hFont;					//!< 文字フォント
	HBRUSH				m_hBrush2;					//!< コントロールの色変更用ブラシ
	CString				m_strFeatureClassName;
};

#endif // _ATTREXTPPOINTDLG_
