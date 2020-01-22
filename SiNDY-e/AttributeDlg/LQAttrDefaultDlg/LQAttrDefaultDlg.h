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

// LQAttrDefaultDlg.h : CLQAttrDefaultDlg の宣言

#ifndef __LQATTRDEFAULTDLG_H_
#define __LQATTRDEFAULTDLG_H_

#include "ExportDlg.h"
#include "LQAttrBaseDlg.h"
#include "resource.h"

typedef CLQAttrBaseDlg LQATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrDefaultDlg
class CLQAttrDefaultDlg : 
	public LQATTR_BASE_CLASS,
	public IDispEventImpl<IDC_GRIDCTRL, CLQAttrDefaultDlg>, // デフォルトのグリッドコントロール
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrDefaultDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
	CHAIN_MSG_MAP(LQATTR_BASE_CLASS)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDefaultDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();

		// デフォルト用グリッドコントロール
		CAxWindow wndGrid = GetDlgItem(IDC_GRIDCTRL);
		wndGrid.QueryControl(&m_spGrid);
		wndGrid.SetFocus();		

		m_spGrid->put_AllowUserResizing( flexResizeColumns );
		m_spGrid->put_AllowSelection( TRUE );
		m_spGrid->put_AutoSizeMouse( TRUE );
		m_spGrid->put_Editable( flexEDKbdMouse );
		m_spGrid->put_ExtendLastCol( TRUE );
		
		// グリッドコントロールからのイベントを拾うのに必要
		AtlAdviseSinkMap(this, TRUE);

		LQATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

		// グリッドエディットモードかどうかをチェック
		// SiNDYのレジストリキーを取得
		CString strRegOpenKey;
		strRegOpenKey.Format( _T("%s%s%sAttributeTool"), SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

		// レジストリオープン
		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
		if ( hOpenKey )
		{
			// グリッド強制編集モードフラグの値を取得
			DWORD dwResult = 0;
			if( EmxGetRegDword( _T("GridEditMode"), &dwResult, hOpenKey ) == TRUE )
				m_bIsGridEditMode = (BOOL)dwResult;

			RegCloseKey( hOpenKey );
		}

		return 1;  // システムにフォーカスを設定させます
	}

	/**
	 * ダイアログが破棄されたときに呼ばれます。
	 */
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlAdviseSinkMap(this, FALSE);

		return 0;  // システムにフォーカスを設定させます
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg イベントシンクマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CLQAttrDefaultDlg)
	///Make sure the Event Handlers have __stdcall calling convention
	SINK_ENTRY(IDC_GRIDCTRL, 0x18, OnAfterEditDefaultGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x19, OnKeyPressEditGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x1c, OnChangeEditGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x6, OnBeforeRowColChangeGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x8, OnBeforeSelChangeGrid)
	SINK_ENTRY(IDC_GRIDCTRL, DISPID_KEYDOWN, OnKeyDown)
	SINK_ENTRY(IDC_GRIDCTRL, 0x16, OnStartEditDefaultGrid)
END_SINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid イベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////
public:
	/**
	 * 全てのグリッドのセル以外でキー入力があった場合のイベントハンドラ
	 * 現在は未使用
	 */
	VOID __stdcall OnKeyDown(SHORT * KeyCode, SHORT Shift)
	{
		//[Bug 4822]グリッドモードでESCやENTERが使えるようにハンドリング
		if( *KeyCode == VK_TAB )
		{
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
		}
		else if( *KeyCode == VK_RETURN )
		{
			// 編集開始状態を取得
			esriEditState state;
			IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
			ipEditor->get_EditState( &state );

			if( state != esriStateNotEditing )
			{
				// リターンキーが押されたItemを更新する
				CWindow wnd = GetFocus();
				INT iWndID = wnd.GetDlgCtrlID();
				m_cControlRel.ControlChanged(iWndID);

				if (CheckReturnKeyItem(iWndID) == FALSE) {		// エンターキーが押されたItemのチェック
					return;				// エラー・検索動作の場合、以降の処理を抜ける
				}

				InvalidateRect( NULL, FALSE );

				::PostMessage( GetParent(), WM_CLOSE, 0, -1);
			}
		}
		else if( *KeyCode == VK_ESCAPE )
		{
			// リターンキーが押されたItemを更新する
			CWindow wnd = GetFocus();
			INT iWndID = wnd.GetDlgCtrlID();
			if( CheckEscKeyItem(iWndID) == FALSE){
				return;
			}
			::PostMessage( GetParent(), WM_CLOSE, 0, 0 );
		}
		return;	
	}
	/**
	 * 全てのグリッドで選択セルの変更があった場合のイベントハンドラ
	 *
	 * 編集開始されていなければ、イベントがキャンセルされます
	 */
	VOID __stdcall OnBeforeSelChangeGrid(LONG OldRowSel, LONG OldColSel, LONG NewRowSel, LONG NewColSel, VARIANT_BOOL * Cancel)
	{
		// 編集開始されていない、または、グリッド強制編集モードでないときは、グリッドはいじれない
		if( IsStartEditing() == FALSE || m_bIsGridEditMode == FALSE )
			*Cancel = VARIANT_TRUE;
	}
	/**
	 * 全てのグリッドのセルの選択が変更されたときに発生するイベントハンドラ
	 *
	 * 編集開始されていなければ、イベントがキャンセルされます
	 */
	VOID __stdcall OnBeforeRowColChangeGrid(LONG OldRow, LONG OldCol, LONG NewRow, LONG NewCol, VARIANT_BOOL * Cancel)
	{
		// 編集開始されていない、または、グリッド強制編集モードでないときは、グリッドはいじれない
		if( IsStartEditing() == FALSE || m_bIsGridEditMode == FALSE )
			*Cancel = VARIANT_TRUE;
	}
	/**
	 * 全てのグリッドのセルの内容が変更されたときに発生するイベントハンドラ
	 * 現在は未使用
	 */
	VOID __stdcall OnChangeEditGrid()
	{
	}
	/**
	 * 編集モード中にキーが押されたときに発生するイベントハンドラ
	 *
	 * 入力時の正当性の判定を行います
	 * 現在は未使用
	 */
	VOID __stdcall OnKeyPressEditGrid(LONG Row, LONG Col, SHORT * KeyAscii)
	{
	}
	/**
	 * 標準グリッドで編集が終了した後に発生するイベントハンドラ
	 */
	VOID __stdcall OnAfterEditDefaultGrid( LONG Row, LONG Col )
	{
		AfterEditGridSel( IDC_GRIDCTRL, m_spGrid, Row, Col );
	}
	/**
	 * 標準グリッドでセルがクリックされたときに発生するイベントハンドラ
	 */
	VOID __stdcall OnStartEditDefaultGrid(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
	{
		// 編集開始されていない、または、グリッド強制編集モードでないときは、グリッドはいじれない
		// [Bug4821]編集されてはいけないもの（たとえばOIDなど）まで編集できてしまうバグ
		// そのために、Arc的に編集できないもの＋SHAPEフィールドなどを選択されていないかを調べる
		if( IsStartEditing() == FALSE || m_bIsGridEditMode == FALSE ||
			m_cControlRel.IsClickCelEditable(IDC_GRIDCTRL, Row, Col) == FALSE )
		{
			*Cancel = VARIANT_TRUE;
			return;
		}
		StartEditGridSel( IDC_GRIDCTRL, m_spGrid, Row, Col );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ CLQAttrBaseDlg::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ CLQAttrBaseDlg::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){ CLQAttrBaseDlg::SetFeatureDefList( pFeatureDefList ); };
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){};
	virtual void ClearFeatureDefs(){ CLQAttrBaseDlg::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CLQAttrDefaultDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return CLQAttrBaseDlg::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return CLQAttrBaseDlg::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
	BOOL FeatureIsValid( CString& strErrMsg ){ return TRUE; };
private:
	void CreateControlRelation();
	BOOL StartEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col );
	void AfterEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col );

	/** 
	* @fn		HRESULT CreateActiveXControls(UINT nID);
	*
	* @brief	FlexGridのライセンスウィンドウを出さないようにするための関数
	*
	* @note		CAxDialogImpl::CreateActiveXControlsをオーバーライドしActiveXControl作成の際に
	*			FlexGridのキーを無理くり当てている
	*/
	HRESULT CreateActiveXControls(UINT nID);

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	IVSFlexGridPtr		m_spGrid;			//!< グリッド表示用
	ISiNDYRulePtr		m_ipRule;			//!< 作業目的DB対応
	BOOL				m_bIsGridEditMode;	//!< グリッド強制編集モードフラグ
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
};

#endif //__LQATTRDEFAULTDLG_H_
