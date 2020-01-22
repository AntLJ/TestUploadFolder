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

// LQAttrTurnRegDlg.h : CLQAttrTurnRegDlg の宣言

#ifndef __LQATTRTURNREGDLG_H_
#define __LQATTRTURNREGDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "RegulationTime.h"
#include "resource.h"

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrTurnRegDlg
class CLQAttrTurnRegDlg : 
	public LQATTR_COMMON_BASE_CLASS,
	public CExportDlg
{
public:
	typedef struct{
		CComVariant DATA[11];
		BOOL	bIsDelete;
	}TIME_REG_DATA;
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrTurnRegDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrTurnRegDlg)
	COMMAND_HANDLER(IDC_CHECK_ZEROGATU1, BN_CLICKED, OnClickedZerogatu)
	COMMAND_HANDLER(IDC_CHECK_ZEROGATU2, BN_CLICKED, OnClickedZerogatu)
	COMMAND_HANDLER(IDC_CHECK_ZEROGATU3, BN_CLICKED, OnClickedZerogatu)
	COMMAND_HANDLER(IDC_CHECK_ZEROGATU4, BN_CLICKED, OnClickedZerogatu)
	COMMAND_HANDLER(IDC_EDIT_TURNREGSMON1, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGSMON2, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGSMON3, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGSMON4, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGEMON1, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGEMON2, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGEMON3, EN_UPDATE, OnUpdateRegMonth)
	COMMAND_HANDLER(IDC_EDIT_TURNREGEMON4, EN_UPDATE, OnUpdateRegMonth)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	CHAIN_MSG_MAP(LQATTR_COMMON_BASE_CLASS)
ALT_MSG_MAP(IDC_COMBO_TURNREGW1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TURNREGW2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TURNREGW3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TURNREGW4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMON1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMON2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMON3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMON4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSDAY1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSDAY2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSDAY3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSDAY4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMON1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMON2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMON3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMON4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEDAY1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEDAY2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEDAY3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEDAY4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSHOUR1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSHOUR2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSHOUR3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSHOUR4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMIN1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMIN2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMIN3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGSMIN4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEHOUR1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEHOUR2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEHOUR3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEHOUR4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMIN1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMIN2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMIN3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TURNREGEMIN4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_TUUNEN1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_TUUNEN2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_TUUNEN3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_TUUNEN4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SYUJITU1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SYUJITU2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SYUJITU3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SYUJITU4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ZEROGATU1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ZEROGATU2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ZEROGATU3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ZEROGATU4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrTurnRegDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();
		// サブクラス化
		m_wndSMonth1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMON1));
		m_wndSMonth2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMON2));
		m_wndSMonth3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMON3));
		m_wndSMonth4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMON4));
		m_wndEMonth1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMON1));
		m_wndEMonth2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMON2));
		m_wndEMonth3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMON3));
		m_wndEMonth4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMON4));
		m_wndSDay1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSDAY1));
		m_wndSDay2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSDAY2));
		m_wndSDay3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSDAY3));
		m_wndSDay4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSDAY4));
		m_wndEDay1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEDAY1));
		m_wndEDay2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEDAY2));
		m_wndEDay3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEDAY3));
		m_wndEDay4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEDAY4));
		m_wndSHour1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSHOUR1));
		m_wndSHour2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSHOUR2));
		m_wndSHour3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSHOUR3));
		m_wndSHour4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSHOUR4));
		m_wndEHour1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEHOUR1));
		m_wndEHour2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEHOUR2));
		m_wndEHour3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEHOUR3));
		m_wndEHour4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEHOUR4));
		m_wndSMin1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMIN1));
		m_wndSMin2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMIN2));
		m_wndSMin3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMIN3));
		m_wndSMin4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGSMIN4));
		m_wndEMin1.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMIN1));
		m_wndEMin2.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMIN2));
		m_wndEMin3.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMIN3));
		m_wndEMin4.SubclassWindow(GetDlgItem(IDC_EDIT_TURNREGEMIN4));
		m_wndWeek1.SubclassWindow(GetDlgItem(IDC_COMBO_TURNREGW1));
		m_wndWeek2.SubclassWindow(GetDlgItem(IDC_COMBO_TURNREGW2));
		m_wndWeek3.SubclassWindow(GetDlgItem(IDC_COMBO_TURNREGW3));
		m_wndWeek4.SubclassWindow(GetDlgItem(IDC_COMBO_TURNREGW4));
		m_wndTButton1.SubclassWindow(GetDlgItem(IDC_BUTTON_TUUNEN1));
		m_wndTButton2.SubclassWindow(GetDlgItem(IDC_BUTTON_TUUNEN2));
		m_wndTButton3.SubclassWindow(GetDlgItem(IDC_BUTTON_TUUNEN3));
		m_wndTButton4.SubclassWindow(GetDlgItem(IDC_BUTTON_TUUNEN4));
		m_wndSButton1.SubclassWindow(GetDlgItem(IDC_BUTTON_SYUJITU1));
		m_wndSButton2.SubclassWindow(GetDlgItem(IDC_BUTTON_SYUJITU2));
		m_wndSButton3.SubclassWindow(GetDlgItem(IDC_BUTTON_SYUJITU3));
		m_wndSButton4.SubclassWindow(GetDlgItem(IDC_BUTTON_SYUJITU4));
		m_wndDButton1.SubclassWindow(GetDlgItem(IDC_BUTTON_DELETE1));
		m_wndDButton2.SubclassWindow(GetDlgItem(IDC_BUTTON_DELETE2));
		m_wndDButton3.SubclassWindow(GetDlgItem(IDC_BUTTON_DELETE3));
		m_wndDButton4.SubclassWindow(GetDlgItem(IDC_BUTTON_DELETE4));
		m_wndZButton1.SubclassWindow(GetDlgItem(IDC_CHECK_ZEROGATU1));
		m_wndZButton2.SubclassWindow(GetDlgItem(IDC_CHECK_ZEROGATU2));
		m_wndZButton3.SubclassWindow(GetDlgItem(IDC_CHECK_ZEROGATU3));
		m_wndZButton4.SubclassWindow(GetDlgItem(IDC_CHECK_ZEROGATU4));
		m_wndSource.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		
		if( m_cArcHelper.IsStartEditing() )
		{
			LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
			::SetFocus( GetDlgItem( IDC_EDIT_TURNREGSMON1 ) );

			return 0;
		}
		else
			return LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	// モーダルの時にキーを捕まえる
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}
	/**
	 * WM_COMMAND 用イベントハンドラ
	 *
	 * 各コモンコントロールで変更があった場合、ここで処理します
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		INT msg = HIWORD(wParam);						// 操作メッセージ
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理

		// 終日または通年ボタン押されたら
		if( msg == BN_CLICKED )
		{
			if( nTargetControl == IDC_BUTTON_TUUNEN1 ||
				nTargetControl == IDC_BUTTON_TUUNEN2 ||
				nTargetControl == IDC_BUTTON_TUUNEN3 ||
				nTargetControl == IDC_BUTTON_TUUNEN4 )
				SetTuunen( nTargetControl );
			
			if(	nTargetControl == IDC_BUTTON_SYUJITU1 ||
				nTargetControl == IDC_BUTTON_SYUJITU2 ||
				nTargetControl == IDC_BUTTON_SYUJITU3 ||
				nTargetControl == IDC_BUTTON_SYUJITU4 )
				SetSyujitu( nTargetControl );

			if(	nTargetControl == IDC_BUTTON_DELETE1 ||
				nTargetControl == IDC_BUTTON_DELETE2 ||
				nTargetControl == IDC_BUTTON_DELETE3 ||
				nTargetControl == IDC_BUTTON_DELETE4 )
				DeleteReg( nTargetControl );
		}

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

		// 上のOnCommandの後でやらないと、00が表示されないのでここで
		if( msg == EN_KILLFOCUS ) {
			CRegulationTime cReg( m_hWnd, CRegulationTime::CtrlID2KiseiNum(nTargetControl) );
			cReg.DisplayZeroZero();
		}
		return 0;
	}

	LRESULT OnClickedZerogatu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnUpdateRegMonth(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrTurnRegDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ LQATTR_COMMON_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ LQATTR_COMMON_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){ LQATTR_COMMON_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){};
	virtual void ClearFeatureDefs(){ LQATTR_COMMON_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CLQAttrTurnRegDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };

private:
	void CreateControlRelation();
	void SetTuunen(INT nControlID);
	void SetSyujitu(INT nControlID);
	void DeleteReg(INT nControlID);

	inline INT GetTurnRegID(LONG Index){
		switch (Index)
		{
			case 1:
				return IDC_EDIT_TURNREGSMON1;
			case 2:
				return IDC_EDIT_TURNREGSDAY1;
			case 3:
				return IDC_EDIT_TURNREGEMON1;
			case 4:
				return IDC_EDIT_TURNREGEDAY1;
			case 5:
				return IDC_EDIT_TURNREGSHOUR1;
			case 6:
				return IDC_EDIT_TURNREGSMIN1;
			case 7:
				return IDC_EDIT_TURNREGEHOUR1;
			case 8:
				return IDC_EDIT_TURNREGEMIN1;
			case 9:
				return IDC_EDIT_TURNREGSMON2;
			case 10:
				return IDC_EDIT_TURNREGSDAY2;
			case 11:
				return IDC_EDIT_TURNREGEMON2;
			case 12:
				return IDC_EDIT_TURNREGEDAY2;
			case 13:
				return IDC_EDIT_TURNREGSHOUR2;
			case 14:
				return IDC_EDIT_TURNREGSMIN2;
			case 15:
				return IDC_EDIT_TURNREGEHOUR2;
			case 16:
				return IDC_EDIT_TURNREGEMIN2;
			case 17:
				return IDC_EDIT_TURNREGSMON3;
			case 18:
				return IDC_EDIT_TURNREGSDAY3;
			case 19:
				return IDC_EDIT_TURNREGEMON3;
			case 20:
				return IDC_EDIT_TURNREGEDAY3;
			case 21:
				return IDC_EDIT_TURNREGSHOUR3;
			case 22:
				return IDC_EDIT_TURNREGSMIN3;
			case 23:
				return IDC_EDIT_TURNREGEHOUR3;
			case 24:
				return IDC_EDIT_TURNREGEMIN3;
			case 25:
				return IDC_EDIT_TURNREGSMON4;
			case 26:
				return IDC_EDIT_TURNREGSDAY4;
			case 27:
				return IDC_EDIT_TURNREGEMON4;
			case 28:
				return IDC_EDIT_TURNREGEDAY4;
			case 29:
				return IDC_EDIT_TURNREGSHOUR4;
			case 30:
				return IDC_EDIT_TURNREGSMIN4;
			case 31:
				return IDC_EDIT_TURNREGEHOUR4;
			case 32:
				return IDC_EDIT_TURNREGEMIN4;
			default:
				return 0;
		}
	}


/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrTurnRegDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndSMonth1, m_wndSMonth2, m_wndSMonth3, m_wndSMonth4;
	CContainedWindow		m_wndEMonth1, m_wndEMonth2, m_wndEMonth3, m_wndEMonth4;
	CContainedWindow		m_wndSDay1, m_wndSDay2, m_wndSDay3, m_wndSDay4;
	CContainedWindow		m_wndEDay1, m_wndEDay2, m_wndEDay3, m_wndEDay4;
	CContainedWindow		m_wndSHour1, m_wndSHour2, m_wndSHour3, m_wndSHour4;
	CContainedWindow		m_wndEHour1, m_wndEHour2, m_wndEHour3, m_wndEHour4;
	CContainedWindow		m_wndSMin1, m_wndSMin2, m_wndSMin3, m_wndSMin4;
	CContainedWindow		m_wndEMin1, m_wndEMin2, m_wndEMin3, m_wndEMin4;
	CContainedWindow		m_wndWeek1, m_wndWeek2, m_wndWeek3, m_wndWeek4, m_wndSource;
	CContainedWindow		m_wndTButton1,m_wndTButton2,m_wndTButton3,m_wndTButton4;
	CContainedWindow		m_wndSButton1,m_wndSButton2,m_wndSButton3,m_wndSButton4;
	CContainedWindow		m_wndDButton1,m_wndDButton2,m_wndDButton3,m_wndDButton4;
	CContainedWindow		m_wndZButton1,m_wndZButton2,m_wndZButton3,m_wndZButton4;

	BOOL	m_bIsEditable;
	ISiNDYRulePtr		m_ipRule;			//!< 作業目的DB対応
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
};

#endif //__LQATTRTURNREGDLG_H_
