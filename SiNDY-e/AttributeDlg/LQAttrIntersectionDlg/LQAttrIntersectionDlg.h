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

// LQAttrIntersectionDlg.h: CLQAttrIntersectionDlg の宣言

#ifndef __LQATTRINTERSECTIONDLG_H_
#define __LQATTRINTERSECTIONDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "resource.h"

#include "WinLib/str2.h"

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrIntersectionDlg
class CLQAttrIntersectionDlg : 
	public LQATTR_COMMON_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrIntersectionDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrIntersectionDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	COMMAND_HANDLER(IDC_BUTTON_BASE_COPY, BN_CLICKED, OnBnClickedButtonBaseCopy)
	COMMAND_HANDLER(IDC_EDIT_KANJI, EN_UPDATE, OnUpdateEditKanji)
	COMMAND_HANDLER(IDC_EDIT_YOMI, EN_UPDATE, OnUpdateEditYomi)
	CHAIN_MSG_MAP(LQATTR_COMMON_BASE_CLASS)

ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KANJI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_YOMI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
// タブオーダーがおかしかったので、XXX_BASE_XXXについても追加 (bug9458)
ALT_MSG_MAP(IDC_EDIT_BASE_KANJI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)	
ALT_MSG_MAP(IDC_EDIT_BASE_YOMI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_BASE_COPY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrIntersectionDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();
		// サブクラス化
		m_wndSource.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wndSourceKanji.SubclassWindow(GetDlgItem(IDC_EDIT_KANJI));
		m_wndSourceYomi.SubclassWindow(GetDlgItem(IDC_EDIT_YOMI));
		// タブオーダーがおかしかったので、XXX_BASE_XXXについても追加 (bug9458)
		m_wndSourceBaseKanji.SubclassWindow(GetDlgItem(IDC_EDIT_BASE_KANJI));
		m_wndSourceBaseYomi.SubclassWindow(GetDlgItem(IDC_EDIT_BASE_YOMI));
		m_wndSourceCopyBtn.SubclassWindow(GetDlgItem(IDC_BUTTON_BASE_COPY));

		if( m_cArcHelper.IsStartEditing() )
		{
			LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
			::SetFocus( GetDlgItem(IDC_EDIT_KANJI) );

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
public:
	LRESULT OnBnClickedButtonBaseCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	/**
	 * IDC_EDIT_KANJI 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateEditKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 文字ルールのチェック
		CheckIntersectionName(wID, hWndCtl, TRUE);

		return 0;
	}

	/**
	 * IDC_EDIT_YOMI 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateEditYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 文字ルールのチェック
		CheckIntersectionName(wID, hWndCtl, FALSE);

		return 0;
	}
	void CheckIntersectionName(WORD wID, HWND hWndCtl, BOOL bIsKanji)
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
				CString strCheck = AheConvertToIntersectionName( lpszText, bIsKanji );
				if( strCheck.Compare( lpszText ) !=0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strCheck );
					// 入力位置の修正
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strCheck );
					edit.SetSel( nSelStart - nDiff, nSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}
	}

	// モーダルの時にキーを捕まえる
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrIntersectionDlg メンバ関数定義
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

	CLQAttrIntersectionDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
	BOOL FeatureIsValid( CString& strErrMsg ){ return TRUE; };

private:
	void CreateControlRelation();
	BOOL SetCtrlColorChange(HWND hTargetWND);

	IFeatureClassPtr GetRoadLinkWithLQ(CLQRowDef* pRowDef);
	IFeaturePtr GetRoadFeature(CLQRowDef* pRowDef, LONG lOID);
	IFeatureClassPtr GetRoadNodeFeatureClass(CLQRowDef* pRowDef);
	void SetBaseIntersectionInfo();
	bool GetTargetLinkAndDir(CLQRowDef* pRowDef, LONG &lDirC, LONG &lLinkID);
	LONG GetLongValue(CRowDef* pRowDef, LPCTSTR lpcszFieldName);
	void SetToolTip(INT nTargetItem, BOOL bIsChange = FALSE);
	void InitToolTip(INT nTargetItem, TOOLINFO *ti);

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrIntersectionDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndSource, m_wndSourceKanji, m_wndSourceYomi, m_wndSourceBaseKanji, m_wndSourceBaseYomi, m_wndSourceCopyBtn;
	ISiNDYRulePtr		m_ipRule;			//!< 作業目的DB対応
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
	HWND				m_hTipNameKanji, m_hTipNameYomi, m_hTipBaseNameKanji, m_hTipBaseNameYomi;

};

#endif // __LQAttrIntersectionDlg_H_
