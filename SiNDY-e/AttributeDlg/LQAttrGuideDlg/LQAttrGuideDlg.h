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

// LQAttrGuideDlg.h : CLQAttrGuideDlg の宣言

#ifndef __LQATTRGUIDEDLG_H_
#define __LQATTRGUIDEDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "resource.h"

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrGuideDlg
class CLQAttrGuideDlg : 
	public LQATTR_COMMON_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrGuideDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	CHAIN_MSG_MAP(LQATTR_COMMON_BASE_CLASS)

ALT_MSG_MAP(IDC_COMBO_GUIDECLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();
		// サブクラス化
		m_wndGuideClass.SubclassWindow(GetDlgItem(IDC_COMBO_GUIDECLASS));
		m_wndSource.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

		if( m_cArcHelper.IsStartEditing() )
		{
			LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
			::SetFocus( GetDlgItem(IDC_COMBO_GUIDECLASS) );

			return 0;
		}
		else
			return LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
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

	// モーダルの時にキーを捕まえる
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg メンバ関数定義
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

	CLQAttrGuideDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };

private:
	void CreateControlRelation();
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndGuideClass, m_wndSource;
	ISiNDYRulePtr		m_ipRule;			//!< 作業目的DB対応
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
};

#endif //__LQATTRGUIDEDLG_H_
