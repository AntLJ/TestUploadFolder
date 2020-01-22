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

// AttrVICSDlg.h : CAttrVICSDlg の宣言

#ifndef _ATTRVICSDLG_
#define _ATTRVICSDLG_

#include "resource.h"

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< 再描画の依頼を受けるためのメッセージ
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrDRMAInLineDlg
class CAttrVICSDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrVICSDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrDRMAInLineDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	COMMAND_HANDLER(IDC_BUTTON_DRAW, BN_CLICKED, OnClickedDrawButton)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
ALT_MSG_MAP(IDC_EDIT_MESH)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_VICSCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_VICSID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DRAW)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrVICSDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_MESH));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_VICSCLASS));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_VICSID));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_BUTTON_DRAW));

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
	 *　同一VICS表示ボタンが押されたときの処理 
	 */
	LRESULT OnClickedDrawButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		DrawSameVICS( GetCurrentFeatureDef()->GetFeature() );
		return 0;
	}

	/**
	 * WM_REFRESHFEATURE 用イベントハンドラ
	 *
	 * 再描画のイベントが属性ダイアログから送られてきます
	 * Refresh()で再描画します
	 */
    LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
		if( wParam == 1 )
		{
			if( m_ipVICSGeom )
			{
				IEnvelopePtr ipFeatEnv;
				m_ipVICSGeom->get_Envelope( &ipFeatEnv );
				m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, ipFeatEnv );
			}
			m_ipVICSGeom = NULL;
		}
		else
			Refresh();

		return 0;
    }

/////////////////////////////////////////////////////////////////////////////
//
// CAttrVICSDlg メンバ関数定義
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
	virtual BOOL ErrorCheck(){return FALSE;};
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){ m_strFeatureClassName = (CString)lpcszFeatureClassName;};

	CAttrVICSDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };
 	void DrawSameVICS(_IRowPtr ipVICSRow);
	void Refresh();

private:
	void CreateControlRelation();
////////////////////////////////////////////////////////////////////////////
//
// CAttrVICSDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	IGeometryPtr		m_ipVICSGeom;				//!< VICSジオメトリ
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4;	//!< 各コントロールウィンドウ
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
	CString				m_strFeatureClassName;		//!< フィーチャクラス名
	IGeometryPtr		m_ipDummy;					//!< ダミー用VICSジオメトリ
};

#endif // _ATTRVICSDLG_
