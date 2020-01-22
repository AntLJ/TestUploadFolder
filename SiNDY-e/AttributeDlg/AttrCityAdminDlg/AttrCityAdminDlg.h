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

// AttrCityAdminDlg.h : CAttrCityAdminDlg の宣言

#ifndef _ATTRCITYADMINDLG_
#define _ATTRCITYADMINDLG_

#include "resource.h"
#include "WinLib/str2.h"
#include "WinLib/charutil.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrCityAdminDlg
class CAttrCityAdminDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityAdminDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrCityAdminDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_CITY_ADMIN_EDIT3, EN_UPDATE, OnUpdateCityAdmin)
	COMMAND_HANDLER(IDC_COMBOBOXEX_CITY_ADMIN, CBN_SELCHANGE, OnSelchangeComboCityAdmin)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_CITY_ADMIN_EDIT2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_EDIT3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_EDIT4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_EDIT5)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBOBOXEX_CITY_ADMIN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_CHECK1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_CHECK2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_CHECK3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_COMBO1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_COMBO2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CITY_ADMIN_COMBO3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDRCODE2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityAdminDlg メッセージハンドラ
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_EDIT2));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_EDIT3));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_CHECK1));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_EDIT4));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_EDIT5));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_COMBO1));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_COMBO2));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_CHECK2));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_CHECK3));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_COMBOBOXEX_CITY_ADMIN));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_CITY_ADMIN_COMBO3));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_EDIT_ADDRCODE2));

		// 拡張コンボボックスの設定
		// [bug 7588] [e][要望]属性編集ダイアログで行政界の街区色を編集したい
		CComboBoxEx cComboBoxCity = GetDlgItem( IDC_COMBOBOXEX_CITY_ADMIN );
		COMBOBOXEXITEM    exItem = {0};
		CString str;
		
		// イメージリスト作成
		m_imageList.CreateFromImage( IDB_BITMAP_COLOR_CITY, 12, 1, RGB(0, 128, 128), IMAGE_BITMAP, LR_CREATEDIBSECTION );

		// 拡張コンボボックスにイメージリストをセット
		CImageList cImage = cComboBoxCity.SetImageList( (HIMAGELIST)m_imageList );

		// アイテム挿入
		// <Null>値は、コンボボックスの最後にする
		exItem.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		exItem.iItem = -1;
		for( long i = 1; i<= 6; ++i )
		{
			str.Format( _T(" %d "), i );
			exItem.iImage = i;
			exItem.iSelectedImage = i;
			exItem.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(str));
			cComboBoxCity.InsertItem( &exItem );
		}
		str = _T(" <Null> ");
		exItem.iImage = 0;
		exItem.iSelectedImage = 0;
		exItem.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(str));
		cComboBoxCity.InsertItem(&exItem);

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	LRESULT OnUpdateCityAdmin(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				INT nTextLen = lstrlen( lpszText ) + 1;
				CString strText( lpszText );
				CString strRet;
				LONG lLen = CString_GetLength2(strText);

				USES_CONVERSION;

				for( LONG i = 0; i < lLen; i++ )
				{
					CString str = CString_GetAt2(strText, i);
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

					if( ( isasciinumber( lpstr[0] ) || ( 0x41 <= (unsigned char)((lpstr[0])&0xff) && (unsigned char)(lpstr[0]&0xff) <= 0x46 ) ) )
					{
						strRet += A2CT( lpstr );
					}
				}
				LONG lCheckLen = CString_GetLength2(strRet);
				if( lLen != lCheckLen )
				{

					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// 入力位置の修正
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strRet);
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

		// エリア判別種別のダイアログに変更があったときにチェック
		if( nTargetControl == IDC_CITY_ADMIN_COMBO2 && msg == CBN_SELCHANGE )
			ChangeToSeaValue();

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

	LRESULT OnSelchangeComboCityAdmin(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [bug 7588] [e][要望]属性編集ダイアログで行政界の街区色を編集したい
		if( wID == IDC_COMBOBOXEX_CITY_ADMIN )
		{
			CComboBoxEx cCombo = GetDlgItem( IDC_COMBOBOXEX_CITY_ADMIN );
			long lSel = cCombo.GetCurSel();
			CFieldDef* pFieldDef = GetCurrentFeatureDef()->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("COLORCODE") );
			if( !pFieldDef ) return 0;
			long lNowValue = 0;
			if( pFieldDef->m_vaValue.vt != VT_NULL )
				lNowValue = pFieldDef->m_vaValue.lVal;

			// [bug 8628] [e]属性編集ダイアログで行政界の街区色を変更できない場合がある
			if( lNowValue != lSel+1 )	// 実際に格納する数値は、lSel値+1なので、比較もlSel+1
			{
				if( lSel >= 6 )		// 6番目はnull値
				{
					CComVariant vaValue;
					vaValue.vt = VT_NULL;
					pFieldDef->SetNewValue( vaValue, m_hChildWnd );
				}
				else
					pFieldDef->SetNewValue( CComVariant( lSel+1 ), m_hChildWnd );

				m_cControlRel.ControlChanged( IDC_COMBOBOXEX_CITY_ADMIN );
				SetButton( Changed() );
				InvalidateRect( NULL, FALSE );
			}
		}

		bHandled = TRUE;

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityAdminDlg メンバ関数定義
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

	CAttrCityAdminDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID){ return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID){ return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void ChangeToSeaValue();

////////////////////////////////////////////////////////////////////////////
//
// CAttrCityAdminDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
public:

private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10,m_wnd11,m_wnd12;	//!< 各コントロールウィンドウ
	HWND				m_hChildWnd;				//!< 子ダイアログハンドル

	CImageList			m_imageList;
};

#endif // _ATTRCITYADMINDLG_

