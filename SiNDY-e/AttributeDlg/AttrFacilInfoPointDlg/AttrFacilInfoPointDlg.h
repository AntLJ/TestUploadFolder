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

// AttrFacilInfoPointDlg.h : CAttrFacilInfoPointDlg の宣言

#ifndef _ATTRFACILINFOPOINTDLG_
#define _ATTRFACILINFOPOINTDLG_

#include "resource.h"
#include "../../HwyEditTool/HwyHelper.h"
#include "../../HwyEditTool/HwyObjects.h"
#include "../../LIB/WinLib/str2.h"

typedef CHetCVDMap CCodeValueDomain;	//!< コードドメインを格納する
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrFacilInfoPointDlg
class CAttrFacilInfoPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrFacilInfoPointDlg();
	~CAttrFacilInfoPointDlg()
	{
		::DeleteObject( m_hFont );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrFacilInfoPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_COMBO_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_EDIT_FACILKANJI, EN_UPDATE, OnUpdateFacilKanji)
	COMMAND_HANDLER(IDC_EDIT_FACILKANA, EN_UPDATE, OnUpdateFacilYomi)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
ALT_MSG_MAP(IDC_COMBO_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FACILKANJI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FACILKANA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DUMMYFLAG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_FACILCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEARCHFACIL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_AUTOPOSITION_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DUMMY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_SEQUENCE));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_FACILKANJI));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_FACILKANA));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_CHECK_DUMMYFLAG));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_COMBO_FACILCLASS));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_CHECK_AUTOPOSITION_F));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_CHECK_DUMMY));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_COMBO_SEARCHFACIL));

		m_wnd3.SetFont( m_hFont );
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
	 * IDC_EDIT_FACILKANJI 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateFacilKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				// 半角文字→全角文字変換、使用禁止文字等のチェック
				CString strHoumenKanji = AheConvertToFacilInfoName( lpszText, TRUE );
				// アルファベット小文字→大文字変換
				int nTextLen = lstrlen(strHoumenKanji) + 1;
				LPTSTR lpszChecked = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
				LCMapString( GetUserDefaultLCID(), LCMAP_UPPERCASE, strHoumenKanji, -1, lpszChecked, nTextLen );
				strHoumenKanji = lpszChecked;
				
				// チェックの結果修正が行われた場合、表示に反映する
				if( strHoumenKanji.Compare( lpszText ) != 0 )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strHoumenKanji );
					// 入力位置の修正
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strHoumenKanji);
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
	 * IDC_EDIT_FACILKANA 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateFacilYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strHoumenYomi = AheConvertToFacilInfoName( lpszText, FALSE );
				if( strHoumenYomi.Compare( lpszText ) != 0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strHoumenYomi );
					// 入力位置の修正
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strHoumenYomi);
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
	 * 路線種別参照用が変更されたときのイベントハンドラ
	 *
	 * 路線名称参照用と、路線コードの値を変更します
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// 選択された路線種別の取得
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nLineClass = cbLineClass.GetItemData(nIdx);
		// 路線コードの再描画
		if(! RedrawRoadNameList(0))
			return 0;
		// 路線コードの設定
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

	/**
	 * 路線名称が変更されたときのイベントハンドラ
	 *
	 * 路線コードの値を変更します
	 */
	LRESULT OnSelchangeRsel_Name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 選択された路線コードの取得
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadCode = cbRoadName.GetItemData(nIdx);
		// 路線コードの設定
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp );
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
	bool SetLineClassList(LONG nLineClass);
	bool GetLineClass(int* pClass);
	bool SetLineClass(int nClass);
	bool RedrawRoadNameList(LONG nRoadCode);
	bool GetRoadCode(int* pRoadCode);
	bool SetRoadCode(int nRoadCode);
	
	ITablePtr GetRoadCodeTable();
	bool GetRoadCodeSet(CHetRoadCodeSet* pCodeSet);

////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10;

	CString		m_cRoadCode;				//!< 文字列での路線コード
	HFONT		m_hFont;

	// ハイウェイヘルパークラス
	CHwyHelper  m_cHwyHelper;				//!< ハイウェイヘルパークラス

	// 元の値
	LONG m_lOrgRoadSequence;				//!< 最初の路線内シーケンス
	LONG m_lOrgRoadCode;					//!< 最初の路線コード

	// ダイアログの値保存用データ
	LONG m_nLineClass;						//!< 現在選択中の路線種別コード
	LONG m_nRoadCode;						//!< 現在選択中の路線コード
	LONG m_nRoadSequence;					//!< 現在選択中の路線内シーケンス

	// ハイウェイ編集用各種データのコンテナ
	CHetRoadCodeSet m_cRoadCodeSet;			//!< 路線コード情報コンテナ
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
};

#endif // _ATTRFACILINFOPOINTDLG_

