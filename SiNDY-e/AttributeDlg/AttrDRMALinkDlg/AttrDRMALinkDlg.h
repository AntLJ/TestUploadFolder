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

// AttrDRMALinkDlg.h : CAttrDRMALinkDlg の宣言

#ifndef _ATTRDRMALINKDLG_H_
#define _ATTRDRMALINKDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrDRMALinkDlg
class CAttrDRMALinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
private:
	/**
	 * 編集ビューのモード
	 *
	 * 主に道路属性を編集する際にどのコントロールを表示したらよいかの指針として使用します。
	 * CRowsControlRelation::SetControl() の第３、第４引数として使用します。
	 * @see CreateControlRelation()
	 */
	enum EditView {
		EDITVIEW_DRMA_KIHON,		/**< 道路の基本属性（第１タブ） */
		EDITVIEW_DRMA_TUIKA,		/**< 道路の追加属性（第２タブ） */
		EDITVIEW_DRMA_KOTUKISEI,	/**< 道路の交通規制属性（第３タブ） */
		EDITVIEW_DRMA_ROSEN,		/**< 道路の路線属性（第４タブ） */
		EDITVIEW_DRMA_DRMA,			/**< 道路のDRMA属性（第５タブ） */
	};
/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
public:
BEGIN_MSG_MAP(CAttrDRMADlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();

		CString strTemp;  //!< pszTextに渡す文字列を一時的に格納する変数
		
		// 道路属性編集用タブコントロールにアイテムを追加します
		HWND hwndTab = GetDlgItem( IDC_TAB_DRMAATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_EXTENTION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_TUIKA, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_REGULATION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_KOTUKISEI, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NUMBER);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_ROSEN, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_DRMA);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_DRMA, (LPARAM)(&pTabCtrlItem));
		
		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);
		ChangeWindowMgrMap( m_iChangeType );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	/**
	 * WM_NOTIFY 用イベントハンドラ
	 *
	 * 以下の処理を行います
	 * ・タブの切り替えが発生した時
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_DRMAATTR ) {
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// コントロール切り替え
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lDRMATabNum = m_iChangeType;	// 道路タブの場合のみタブのIDを保持

				bHandled = TRUE;
				return 1;
			}
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
			INT nID = (INT)LOWORD(wParam);
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg メンバ関数定義
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
	virtual BOOL ErrorCheck(){return TRUE;};
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName( LPCTSTR lpcszFeatureClassName ){ m_strFeatureClassName = (CString)lpcszFeatureClassName;};

	CAttrDRMALinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void ChangeWindowMgrMap( INT iIndex );
	void CreateControlRelation();
	HRESULT CreateActiveXControls(UINT nID);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	LONG				m_lDRMATabNum;				//!< 道路属性タブの表示位置保存用
	INT					m_iChangeType;				//!< コントロール切り替え用

private:
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
	CString				m_strFeatureClassName;		//!< フィーチャクラス名
};

#endif // _ATTRDRMALINKDLG_H_
