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

// AttrGouPointDlg.h : CAttrGouPointDlg の宣言

#ifndef _ATTRGOUPOINTDLG_
#define _ATTRGOUPOINTDLG_

#include "resource.h"
#include "../../../LIB/WinLib/str2.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrGouPointDlg
class CAttrGouPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrGouPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrGouPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_VIEWOK, BN_CLICKED, OnClickRadio)
	COMMAND_HANDLER(IDC_BUTTON_VIEWNG, BN_CLICKED, OnClickRadio)
	COMMAND_HANDLER(IDC_BUTTON_INQDOC, BN_CLICKED, OnClickRadio)
	COMMAND_HANDLER(IDC_BUTTON_PUBDOC, BN_CLICKED, OnClickRadio)
	COMMAND_HANDLER(IDC_BUTTON_NEWBUILDDOC, BN_CLICKED, OnClickRadio)
	COMMAND_HANDLER(IDC_BUTTON_UNCHECK, BN_CLICKED, OnClickRadio)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	COMMAND_HANDLER(IDC_GOU_POINT_EDIT1, EN_UPDATE, OnUpdateGouNumber)

ALT_MSG_MAP(IDC_GOU_POINT_EDIT1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_GOU_POINT_EDIT2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_GOU_POINT_EDIT3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_GOU_POINT_CHECK1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_GOU_POINT_CHECK2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_GOUPOINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_GOU_POINT_EDIT6)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HIMAWARI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_VIEWOK)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_VIEWNG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_UNCHECK)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_NEWBUILDDOC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_INQDOC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PUBDOC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_CONFIRM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_EXIST)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HS1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HS2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LM1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LM2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrGouPointDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_GOU_POINT_EDIT1));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_GOU_POINT_EDIT2));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_GOU_POINT_CHECK1));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_GOU_POINT_EDIT3));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_GOU_POINT_CHECK2));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_GOU_POINT_EDIT6));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_COMBO_GOUPOINT));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_CHECK_HIMAWARI));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_BUTTON_VIEWOK));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_BUTTON_VIEWNG));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_BUTTON_INQDOC));
		m_wnd13.SubclassWindow(GetDlgItem(IDC_BUTTON_PUBDOC));
		m_wnd14.SubclassWindow(GetDlgItem(IDC_COMBO_CONFIRM));
		m_wnd15.SubclassWindow(GetDlgItem(IDC_EDIT_EXIST));
		m_wnd16.SubclassWindow(GetDlgItem(IDC_EDIT_HS1));
		m_wnd17.SubclassWindow(GetDlgItem(IDC_EDIT_HS2));
		m_wnd18.SubclassWindow(GetDlgItem(IDC_EDIT_LM1));
		m_wnd19.SubclassWindow(GetDlgItem(IDC_EDIT_LM2));
		m_wnd20.SubclassWindow(GetDlgItem(IDC_BUTTON_UNCHECK));
		m_wnd21.SubclassWindow(GetDlgItem(IDC_BUTTON_NEWBUILDDOC));

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	LRESULT OnUpdateGouNumber(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszOriginalText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszOriginalText, nTextLen );

			// <null> の場合を考慮
			if( lstrcmp( lpszOriginalText, NULL_VALUE ) != 0 )
			{
				INT nTextLen = lstrlen( lpszOriginalText ) + 1;
				LPTSTR lpszText;

				lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) * 2 );	// 2倍の領域を確保
				// 全て全角にしてしまう
				LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpszOriginalText, nTextLen, lpszText, nTextLen * 2 );

				CString strText( lpszText );
				CString strRet;
				LONG lLen = CString_GetLength2( strText );


				USES_CONVERSION;

				for( LONG i = 0; i < lLen; i++ )
				{
					CString str = CString_GetAt2( strText, i );
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );
					LPTSTR lptstr = (LPTSTR)((LPCTSTR)str);

					if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x94 == (unsigned char)((lpstr[1])&0xff) ) )
						strRet += _T("#");
					else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x96 == (unsigned char)((lpstr[1])&0xff) ) )
						strRet += _T("*");
					else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x7c == (unsigned char)((lpstr[1])&0xff) ) )
						strRet += _T("-");
					else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x40 == (unsigned char)((lpstr[1])&0xff) ) )	// [bug7749]スペースの入力はできないようにする
						strRet += _T("");
					// 数字の場合には半角に直す
					else if( ( 0x82 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x4f <= (unsigned char)((lpstr[1])&0xff) ) && ( (unsigned char)((lpstr[1])&0xff) <= 0x58 ) )
					{
						LPTSTR lpszNumberText = (LPTSTR)_alloca( 2 * sizeof(TCHAR) );
						// 半角にしてしまう
						LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lptstr, 2, lpszNumberText, 2 );

						strRet += lpszNumberText;
					}
					else
					{
						// 先頭が#であるならば、文字そのままくっつける
						if( 0x30 <= (unsigned char)((lpstr[0])&0xff) && (unsigned char)(lpstr[0]&0xff) <= 0x39 )
							strRet += lpstr;
						else
						{
							if( strRet.Find( _T("#") ) == 0 )
								strRet += lpstr;
							else
							{
								// 文字入力の際に#が入っていなかったら先頭に#を付ける
								CString strTemp = strRet;
								strRet = _T("#");
								strRet += strTemp;
								strRet += lpstr;
							}
						}
					}
				}
				LONG lCheckLen = CString_GetLength2( strRet );
				if( strRet.Compare( lpszOriginalText ) != 0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strRet );
					// 入力位置の修正
					INT nDiff = CString( lpszOriginalText ).GetLength() - strRet.GetLength();
					edit.SetSel( nSelStart - nDiff, nSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
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
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		bHandled = FALSE;
		return 0;
	}
	LRESULT OnClickRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrGouPointDlg メンバ関数定義
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

	CAttrGouPointDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
////////////////////////////////////////////////////////////////////////////
//
// CAttrGouPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5;
	CContainedWindow	m_wnd6,m_wnd7,m_wnd8,m_wnd9,m_wnd10;
	CContainedWindow	m_wnd12,m_wnd13,m_wnd14,m_wnd15,m_wnd16;
	CContainedWindow	m_wnd17,m_wnd18,m_wnd19;
	CContainedWindow	m_wnd20,m_wnd21;
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
};

#endif // _ATTRGOUPOINTDLG_

