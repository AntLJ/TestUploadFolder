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

// AttrMiddleSiteDlg.h : CAttrMiddleSiteDlg の宣言

#ifndef _ATTRMIDDLESITEDLG_
#define _ATTRMIDDLESITEDLG_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrMiddleSiteDlg
class CAttrMiddleSiteDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrMiddleSiteDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
public:
BEGIN_MSG_MAP(CAttrMiddleSiteDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_SAME1, BN_CLICKED, OnClickedButton1)
	COMMAND_HANDLER(IDC_BUTTON_SAME2, BN_CLICKED, OnClickedButton2)
	COMMAND_HANDLER(IDC_BUTTON_SAME3, BN_CLICKED, OnClickedButton3)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_COMBO_SC1BGCLASS_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SAME1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SC2BGCLASS_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SAME2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SC3BGCLASS_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SAME3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SC4BGCLASS_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENKOU1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENKOU2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENKOU3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENKOU4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCH1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCH2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCH3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCH4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrMiddleSiteDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_SC1BGCLASS_C));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_BUTTON_SAME1));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_SC2BGCLASS_C));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_BUTTON_SAME2));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_SC3BGCLASS_C));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_BUTTON_SAME3));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_COMBO_SC4BGCLASS_C));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_GENKOU1));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_GENKOU2));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_GENKOU3));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_GENKOU4));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCH1));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCH2));
		m_wnd13.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCH3));
		m_wnd14.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCH4));
		m_wnd15.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

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
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );

		// コンボボックスのリストボックスの長さを調節
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );


		// 背景種別のコンボボックスの値がセルチェンジしたときに原稿番号入力の値を変える
		if( ( nTargetControl == IDC_COMBO_SC1BGCLASS_C ) && ( msg == CBN_SELCHANGE) ||
		( ( nTargetControl == IDC_COMBO_SC2BGCLASS_C ) && ( msg == CBN_SELCHANGE)) ||
		( ( nTargetControl == IDC_COMBO_SC3BGCLASS_C ) && ( msg == CBN_SELCHANGE)) ||
		( ( nTargetControl == IDC_COMBO_SC4BGCLASS_C ) && ( msg == CBN_SELCHANGE)))
		{
			ShowGenkouByBackgroundClass(nTargetControl);
		}

		// 原稿番号入力の値が変わったときに、背景種別のコンボボックスの値を変える
		if( ( nTargetControl == IDC_EDIT_GENKOU1 ) && ( msg == EN_KILLFOCUS) ||
		(( nTargetControl == IDC_EDIT_GENKOU2 ) && ( msg == EN_KILLFOCUS)) ||
		(( nTargetControl == IDC_EDIT_GENKOU3 ) && ( msg == EN_KILLFOCUS)) ||
		(( nTargetControl == IDC_EDIT_GENKOU4 ) && ( msg == EN_KILLFOCUS)))
		{
			INT cClassID = 0;

			if( nTargetControl == IDC_EDIT_GENKOU1 )
				cClassID = IDC_COMBO_SC1BGCLASS_C;
			else if( nTargetControl == IDC_EDIT_GENKOU2 )
				cClassID = IDC_COMBO_SC2BGCLASS_C;
			else if( nTargetControl == IDC_EDIT_GENKOU3 )
				cClassID = IDC_COMBO_SC3BGCLASS_C;
			else if( nTargetControl == IDC_EDIT_GENKOU4 )
				cClassID = IDC_COMBO_SC4BGCLASS_C;

			SetBackgroundClassCode(nTargetControl, cClassID);
		}

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * 以上同じにボタンが押された時の処理
	 */
	LRESULT OnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CopyValue(IDC_COMBO_SC1BGCLASS_C, IDC_COMBO_SC2BGCLASS_C);
		CopyValue(IDC_COMBO_SC1BGCLASS_C, IDC_COMBO_SC3BGCLASS_C);
		CopyValue(IDC_COMBO_SC1BGCLASS_C, IDC_COMBO_SC4BGCLASS_C);
		return 0;
	}
	LRESULT OnClickedButton2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CopyValue(IDC_COMBO_SC2BGCLASS_C, IDC_COMBO_SC3BGCLASS_C);
		CopyValue(IDC_COMBO_SC2BGCLASS_C, IDC_COMBO_SC4BGCLASS_C);
		return 0;
	}
	LRESULT OnClickedButton3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CopyValue(IDC_COMBO_SC3BGCLASS_C, IDC_COMBO_SC4BGCLASS_C);
		return 0;
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrMiddleSiteDlg メンバ関数定義
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
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strClassName = lpcszFeatureClassName;};

	CAttrMiddleSiteDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL CheckEscKeyItem(INT nClassID);

private:
	void CreateControlRelation();
	void CopyValue(INT nOrgID, INT nCpyID);
	BOOL SetBackgroundClassCode(INT nCodeID, INT nClassID);
	void ShowGenkouByBackgroundClass(INT nCodeID);
	void GetBackgroundClass(LONG *lBackgroundClass, INT nCodeID);

////////////////////////////////////////////////////////////////////////////
//
// CAttrMiddleSiteDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10,m_wnd11,m_wnd12,m_wnd13,
						m_wnd14,m_wnd15;				//!< 各コントロールウィンドウ
	HWND		m_hChildWnd;							//!< 子ダイアログハンドル
};
#endif // _ATTRMIDDLESITEDLG_
