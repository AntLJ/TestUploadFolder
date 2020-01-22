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

// AttrRoadNodeDlg.h : CAttrRoadNodeDlg の宣言

#ifndef __ATTRROADNODEDLG_H_
#define __ATTRROADNODEDLG_H_

#include "resource.h"
#include "WinLib/Str2.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrRoadNodeDlg
class CAttrRoadNodeDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrRoadNodeDlg();
	~CAttrRoadNodeDlg()
	{
		::DeleteObject( m_hFont );
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadNodeDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrRoadNodeDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_HEIGHTFLAG_COMBO, CBN_SELCHANGE, OnUpdateHeightFlag)
	COMMAND_HANDLER(IDC_KANJI_EDIT, EN_UPDATE, OnUpdateCrossKanji)
	COMMAND_HANDLER(IDC_KANA_EDIT, EN_UPDATE, OnUpdateCrossYomi)
	COMMAND_HANDLER(IDC_HEIGHT_EDIT, EN_UPDATE, OnUpdateHeightEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_WORKYEAR_EDIT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_NODEKINDS_COMBO)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SIGNAL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_KANJI_EDIT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_KANA_EDIT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_HEIGHTFLAG_COMBO)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_HEIGHT_EDIT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_PARKINGID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadNodeDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_WORKYEAR_EDIT));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_NODEKINDS_COMBO));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_SIGNAL));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_KANJI_EDIT));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_KANA_EDIT));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_HEIGHTFLAG_COMBO));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_HEIGHT_EDIT));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_PARKINGID));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		
		m_wnd3.SetFont( m_hFont );
		m_wnd4.SetFont( m_hFont );
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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ))
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		// 本来はControlRelのSetControlで行うべきだが、クレームフラグがたっているときに、
		// うまく相対高さのエディットボックスが切り替わらないため、ここで強引に行う
		IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		sindyeEditableType	sindyeEditableType;
		CComBSTR bstrEditable;

		// 編集可能かどうかを取得
		m_ipRule->IsEditableFeature( ipFeature, ipGeom, sindyeEditAttribute, &sindyeEditableType, &bstrEditable );
		// 編集ボタンを押しているかどうかをゲット
		BOOL bIsWarnDelete = GetCurrentFeatureDef()->GetWarnDelete();

		// 編集開始状態を取得
		esriEditState state = esriStateNotEditing;
		IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
		ipEditor->get_EditState( &state );

		// 編集開始されていて、さらに編集できるときにだけ中に入る
		if( ( sindyeEditableType == sindyeEditableEditable || bIsWarnDelete ) && state == esriStateEditing )
		{
			CFieldDef *pFieldDef;
			pFieldDef = GetCurrentFeatureDef()->GetFieldDef( NODE_TABLE_NAME, _T("HEIGHT_C"));
			if( pFieldDef )
			{
				LONG lHeight_c = pFieldDef->m_vaValue.lVal;
				if( lHeight_c == 1 )
				{
					CWindow wnd( GetDlgItem( IDC_HEIGHT_EDIT ));
					::SendMessage( wnd, EM_SETREADONLY, FALSE, 0L );
					wnd.ModifyStyle( WS_DISABLED|ES_READONLY, NULL );		//!< EditBoxをEnableに
				}else
				{
					CWindow wnd( GetDlgItem( IDC_HEIGHT_EDIT ));
					::SendMessage( wnd, EM_SETREADONLY, TRUE, 0L );
					wnd.ModifyStyle( NULL, WS_DISABLED );					//!< EditBoxをDisableに
				}
			}
		}

		return 0;
	}

	/**
	 * IDC_EDIT_HOUMENKANJI 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateCrossKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strHoumenKanji = AheConvertToIntersectionName( lpszText, TRUE );

				if( strHoumenKanji.Compare( lpszText ) != 0)
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strHoumenKanji );
					// 入力位置の修正
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strHoumenKanji );
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	 * IDC_EDIT_HOUMENKANA 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateCrossYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strHoumenYomi = AheConvertToIntersectionName( lpszText, FALSE );
				if( strHoumenYomi.Compare( lpszText ) !=0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strHoumenYomi );
					// 入力位置の修正
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strHoumenYomi );
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
	 * IDC_HEIGHTFLAG_COMBO 用イベントハンドラ
	 *
	 * 相対高さフラグの変更があったときには、ここで相対高さの値の変更などを行います。
	 */
	LRESULT	OnUpdateHeightFlag(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CComboBox	cCombo( hWndCtl );
			CEdit	cEdit( GetDlgItem( IDC_HEIGHT_EDIT ) );

			int iHeightFlag = cCombo.GetCurSel();

			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( IDC_HEIGHTFLAG_COMBO );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );

			if( iHeightFlag != 1 )
			{
				LPCTSTR lpszNewText = _T("0");
				cEdit.SetWindowText( lpszNewText );
			}
			else
			{
				CWindow wnd( GetDlgItem( IDC_HEIGHT_EDIT ));
				::SendMessage( wnd, EM_SETREADONLY, FALSE, 0L );
				wnd.ModifyStyle( WS_DISABLED|ES_READONLY, NULL );		//!< EditBoxをEnableに
			}
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( IDC_HEIGHT_EDIT );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}
		return 0;
	}


	/**
	 * 相対高さ入力エディットボックス用イベントハンドラ
	 *
	 * エディットボックスの文字が操作される場合、ここで処理
	 * 入力文字を見張って不正文字は入力させないようにする
	 */
	LRESULT OnUpdateHeightEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CheckInputValue( hWndCtl, wID );
		return 0;
	}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadNodeDlg メンバ関数定義
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

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void SetToolTip(INT nTargetItem);

////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadNodeDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8;//!< 各コントロールウィンドウ
	HFONT				m_hFont;					//!< 文字フォント
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
};

#endif // __ATTRROADNODEDLG_H_
