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

// AttrAccessPointDlg.h : CAttrAccessPointDlg の宣言

#ifndef _ATTRACCESSPOINTDLG_
#define _ATTRACCESSPOINTDLG_

#include "resource.h"
#include "../../HwyEditTool/HwyHelper.h"
#include "WinLib/str2.h"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include "AccessStoreCache.h"


///< 路線種別コード情報コンテナ
typedef CHetCVDMap CCodeValueDomain;

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrAccessPointDlg
class CAttrAccessPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public IDispEventImpl<IDC_GRID_ACCESS_STORE,CAttrAccessPointDlg>
{
public:
	CAttrAccessPointDlg();
	~CAttrAccessPointDlg()
	{
		::DeleteObject( m_hFont );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrAccessPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	COMMAND_HANDLER(IDC_COMBO_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_COMBO_SEQUENCE, CBN_SELCHANGE, OnSelchangeRoadSequence)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANA, EN_UPDATE, OnUpdateHoumenYomi)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)

ALT_MSG_MAP(IDC_CHECK_HWYMODEFLAG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROSENCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DIRECTION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_JUNCTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	
ALT_MSG_MAP(IDC_CHECK_RESTAURANT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_RESTING)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_INFOMATION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SPA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CASHSERVICE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_TOILET)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_GS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CAFETERIA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NAP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SHOWER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_FAX)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HWYOASIS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HANDYCAP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_MULTIPLESTORE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SOUBENIR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_INFODESK)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_LAUNDRY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_MAILBOX)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DOGRUN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_EDIT_MASTERCODE)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_START_HOUR)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_START_MIN)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_END_HOUR)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_END_MIN)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_MONDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_TUESDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_WEDNESDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_THURSDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_FRIDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SATURDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_SUNDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_HOLIDAY)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_GOLDENWEEK)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_BON)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_YEAREND)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NEWYEAR)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_CHECK_ALL)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_UNCHECK_ALL)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_ADD_ACCESS_STORE)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_UPDATE_ACCESS_STORE)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETE_ACCESS_STORE)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SAVE_ACCESS_STORE)			// [Bug5556]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_TAB_ACCESSPOINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

BEGIN_SINK_MAP(CAttrAccessPointDlg)					// [Bug5556]で追加
	SINK_ENTRY(IDC_GRID_ACCESS_STORE, 1, SelChangeGridAccessStore)
END_SINK_MAP()
void __stdcall SelChangeGridAccessStore();


/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロールとフィールドの関連付け
		CreateControlRelation();

		// チェーン店マスターコードエディットコントロールの最大桁数設定（[Bug5556]で追加）
		CEdit( GetDlgItem( IDC_EDIT_MASTERCODE ) ).SetLimitText( m_MasterCodeMaxLength );
		// 時間系エディットコントロール群の最大桁数設定（[Bug5556]で追加）
		std::vector<long> aTimeEditControlIDs = boost::assign::list_of
			(IDC_EDIT_START_HOUR)(IDC_EDIT_START_MIN)(IDC_EDIT_END_HOUR)(IDC_EDIT_END_MIN);
		BOOST_FOREACH (long aIDC, aTimeEditControlIDs)
		{
			CEdit( GetDlgItem( aIDC ) ).SetLimitText( m_TimeStrMaxLength );
		}

		// ACCESSPOINT 用タブコントロール
		HWND hwndTab = GetDlgItem( IDC_TAB_ACCESSPOINT );
		TC_ITEM pTabCtrlItem;

		ATLASSERT( ::IsWindow( hwndTab ) );

		CString strTemp;  //!< pszTextに渡す文字列を一時的に格納する変数
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString( IDS_BASEINFO );
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, 0, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString( IDS_FACILITYINFO );
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, 1, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString( IDS_ACCESSSTORE );
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, 2, (LPARAM)(&pTabCtrlItem));
		::SetWindowPos( hwndTab, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );

		// サブクラス化
		m_wnd0.SubclassWindow(GetDlgItem(IDC_CHECK_HWYMODEFLAG));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_ROSENCODE));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_SEQUENCE));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_SEQUENCE));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_COMBO_DIRECTION));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_COMBO_JUNCTIONCODE));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANA));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

		m_wndInfo1.SubclassWindow(GetDlgItem(IDC_CHECK_RESTAURANT));
		m_wndInfo2.SubclassWindow(GetDlgItem(IDC_CHECK_RESTING));
		m_wndInfo3.SubclassWindow(GetDlgItem(IDC_CHECK_INFOMATION));
		m_wndInfo4.SubclassWindow(GetDlgItem(IDC_CHECK_SPA));
		m_wndInfo5.SubclassWindow(GetDlgItem(IDC_CHECK_CASHSERVICE));
		m_wndInfo6.SubclassWindow(GetDlgItem(IDC_CHECK_TOILET));
		m_wndInfo7.SubclassWindow(GetDlgItem(IDC_CHECK_CAFETERIA));
		m_wndInfo8.SubclassWindow(GetDlgItem(IDC_CHECK_NAP));
		m_wndInfo9.SubclassWindow(GetDlgItem(IDC_CHECK_SHOWER));
		m_wndInfo10.SubclassWindow(GetDlgItem(IDC_CHECK_FAX));
		m_wndInfo11.SubclassWindow(GetDlgItem(IDC_CHECK_HWYOASIS));
		m_wndInfo12.SubclassWindow(GetDlgItem(IDC_CHECK_HANDYCAP));
		m_wndInfo13.SubclassWindow(GetDlgItem(IDC_CHECK_SOUBENIR));
		m_wndInfo14.SubclassWindow(GetDlgItem(IDC_CHECK_INFODESK));
		m_wndInfo15.SubclassWindow(GetDlgItem(IDC_CHECK_LAUNDRY));
		m_wndInfo16.SubclassWindow(GetDlgItem(IDC_CHECK_MAILBOX));
		m_wndInfo19.SubclassWindow(GetDlgItem(IDC_CHECK_GS));
		m_wndInfo20.SubclassWindow(GetDlgItem(IDC_CHECK_MULTIPLESTORE));
		m_wndInfo21.SubclassWindow(GetDlgItem(IDC_CHECK_DOGRUN));

		m_wndAS1.SubclassWindow( GetDlgItem(IDC_EDIT_MASTERCODE) );					// [Bug5556]で追加
		m_wndAS2.SubclassWindow( GetDlgItem(IDC_EDIT_START_HOUR) );					// [Bug5556]で追加
		m_wndAS3.SubclassWindow( GetDlgItem(IDC_EDIT_START_MIN) );					// [Bug5556]で追加
		m_wndAS4.SubclassWindow( GetDlgItem(IDC_EDIT_END_HOUR) );					// [Bug5556]で追加
		m_wndAS5.SubclassWindow( GetDlgItem(IDC_EDIT_END_MIN) );					// [Bug5556]で追加
		m_wndAS6.SubclassWindow( GetDlgItem(IDC_CHECK_MONDAY) );					// [Bug5556]で追加
		m_wndAS7.SubclassWindow( GetDlgItem(IDC_CHECK_TUESDAY) );					// [Bug5556]で追加
		m_wndAS8.SubclassWindow( GetDlgItem(IDC_CHECK_WEDNESDAY) );					// [Bug5556]で追加
		m_wndAS9.SubclassWindow( GetDlgItem(IDC_CHECK_THURSDAY) );					// [Bug5556]で追加
		m_wndAS10.SubclassWindow( GetDlgItem(IDC_CHECK_FRIDAY) );					// [Bug5556]で追加
		m_wndAS11.SubclassWindow( GetDlgItem(IDC_CHECK_SATURDAY) );					// [Bug5556]で追加
		m_wndAS12.SubclassWindow( GetDlgItem(IDC_CHECK_SUNDAY) );					// [Bug5556]で追加
		m_wndAS13.SubclassWindow( GetDlgItem(IDC_CHECK_HOLIDAY) );					// [Bug5556]で追加
		m_wndAS14.SubclassWindow( GetDlgItem(IDC_CHECK_GOLDENWEEK) );				// [Bug5556]で追加
		m_wndAS15.SubclassWindow( GetDlgItem(IDC_CHECK_BON) );						// [Bug5556]で追加
		m_wndAS16.SubclassWindow( GetDlgItem(IDC_CHECK_YEAREND) );					// [Bug5556]で追加
		m_wndAS17.SubclassWindow( GetDlgItem(IDC_CHECK_NEWYEAR) );					// [Bug5556]で追加
		m_wndAS18.SubclassWindow( GetDlgItem(IDC_BUTTON_CHECK_ALL) );				// [Bug5556]で追加
		m_wndAS19.SubclassWindow( GetDlgItem(IDC_BUTTON_UNCHECK_ALL) );				// [Bug5556]で追加
		m_wndAS20.SubclassWindow( GetDlgItem(IDC_BUTTON_ADD_ACCESS_STORE) );		// [Bug5556]で追加
		m_wndAS21.SubclassWindow( GetDlgItem(IDC_BUTTON_UPDATE_ACCESS_STORE) );		// [Bug5556]で追加
		m_wndAS22.SubclassWindow( GetDlgItem(IDC_BUTTON_DELETE_ACCESS_STORE) );		// [Bug5556]で追加
		m_wndAS23.SubclassWindow( GetDlgItem(IDC_BUTTON_SAVE_ACCESS_STORE) );		// [Bug5556]で追加

		m_wndTab.SubclassWindow(GetDlgItem(IDC_TAB_ACCESSPOINT ) );

		// 方面名称に文字フォントを設定する
		m_wnd8.SetFont( m_hFont );

		// Access_Storeグリッドコントロールとm_ipAccessStoreGridの関連付け（[Bug5556]で追加）
		CAxWindow( GetDlgItem( IDC_GRID_ACCESS_STORE )).QueryControl( &m_ipAccessStoreGrid );

		// グリッドコントロールからのイベントを拾う（[Bug5556]で追加）
		AtlAdviseSinkMap(this, TRUE);

		// Debugビルドでは第0列（オブジェクトID）を表示（[Bug5556]で追加）
#ifdef _DEBUG
		m_ipAccessStoreGrid->PutColHidden( 0, false );
#endif

		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);
		ChangeWindowMgrMap( m_iChangeType );

		// システムにフォーカスを設定させるため
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
		if( wParam == IDC_TAB_ACCESSPOINT ) {
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// コントロール切り替え
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lAccessTabNum = m_iChangeType; // タブの位置保存
				//[Bug 5510]路線種別などの独自にコンボボックス設定しているようなやつは、値が再セットされるときに
				//一度リセットされてしまう。そのため、タブ切り替えのタイミングで何も入らなくなった。
				//なので、タブ切り替えタイミングでコンボボックスを再設定してやるためにAttrBaseDlg::SetCurrentFeatureDefIndexではなく
				//AttrAccessPointDlg::SetCurrentFeatureDefIndexを呼んでやる。
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				bHandled = TRUE;
				return 1;
			}
		}

		if( wParam == 0 )
		{
			HWND hwndTab = GetDlgItem( IDC_TAB_ACCESSPOINT );
			if( lParam == -2 )
			{
				// コントロール切り替え
				if( m_iChangeType + 1 == 2 )
				{
					m_iChangeType = 0;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				else
				{
					m_iChangeType = m_iChangeType + 1;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lAccessTabNum = m_iChangeType;	// タブの位置保存
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				bHandled = TRUE;

				if( IsEditableFeature() == TRUE && m_cArcHelper.IsStartEditing() == TRUE )
				{
					if( m_lAccessTabNum == 0 )
						::SetFocus( GetDlgItem(IDC_CHECK_HWYMODEFLAG) );
					else 
						::SetFocus( GetDlgItem(IDC_CHECK_RESTAURANT) );
				}
				return 1;
			}
			else if( lParam == -1 ) {
				// コントロール切り替え
				if( m_iChangeType - 1 == -1 )
				{
					m_iChangeType = 1;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				else
				{
					m_iChangeType = m_iChangeType - 1;
					::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
				}
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lAccessTabNum = m_iChangeType;	// タブの位置保存
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				bHandled = TRUE;

				if( IsEditableFeature() == TRUE && m_cArcHelper.IsStartEditing() == TRUE )
				{
					if( m_lAccessTabNum == 0 )
						::SetFocus( GetDlgItem(IDC_CHECK_HWYMODEFLAG) );
					else 
						::SetFocus( GetDlgItem(IDC_CHECK_RESTAURANT) );
				}
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
		if( ( msg == CBN_SELCHANGE ) || ( msg == EN_KILLFOCUS ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		// 「全選択」「全解除」ボタンが押された時の処理（[Bug5556]で追加）
		if (nTargetControl == IDC_BUTTON_CHECK_ALL || nTargetControl == IDC_BUTTON_UNCHECK_ALL)
		{
			BOOST_FOREACH (long aID, GetASCheckBoxIDs())
			{
				CButton( GetDlgItem( aID ) ).SetCheck( nTargetControl == IDC_BUTTON_CHECK_ALL ? BST_CHECKED : BST_UNCHECKED );
			}
		}

		// 「追加」「更新」「削除」ボタンが押された時の処理（[Bug5556]で追加）
		EditAccessStore( nTargetControl );

		// 「保存」ボタンが押された時の処理（[Bug5556]で追加）
		if (nTargetControl == IDC_BUTTON_SAVE_ACCESS_STORE)
		{
			SaveAccessStore();
		}

		return 0;
	}

	/**
	 * IDC_EDIT_HOUMENKANJI 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateHoumenKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strHoumenKanji = AheConvertToAccessPointName( lpszText, TRUE );
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
	 * IDC_EDIT_HOUMENKANA 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 */
	LRESULT OnUpdateHoumenYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strHoumenYomi = AheConvertToAccessPointName( lpszText, FALSE );
				if( strHoumenYomi.Compare( lpszText ) != 0)
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
		m_lLineClass = cbLineClass.GetItemData(nIdx);
		// 路線コードの再描画
		if(! RedrawRoadNameList(0))
			return 0;
		// 路線コードの設定
		if(! SetRoadCode(m_lRoadCode))
			return 0;
		// 路線内シーケンスの値を0に設定する
		CComVariant		vaRoadSequence;
		CFieldDef*		pFieldDef;
		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		// 路線コードの値に従ってハイウェイモードフラグのチェックボックスを更新
		ChangeHwyModeCheckBox(m_lRoadCode);

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
		m_lRoadCode = cbRoadName.GetItemData(nIdx);
		// 路線コードの設定
		if(! SetRoadCode(m_lRoadCode))
			return 0;
		// 路線内シーケンスの値を0に設定する
		CComVariant		vaRoadSequence;
		CFieldDef*		pFieldDef;
		pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
		pFieldDef->m_vaValue = 0L;
		m_cRoadSeq.Format(_T("%d"), 0 );	

		ChangeRoadSeqCombobox();

		return 0;
	}

	/**
	 * 路線内シーケンス参照用が変更されたときのイベントハンドラ
	 *
	 * 路線内シーケンスの値を変更します。
	 */
	LRESULT OnSelchangeRoadSequence(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 選択された路線コードの取得
		CComboBox cbRoadSequence = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_SEQUENCE));
		int nIdx = cbRoadSequence.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		LONG lRoadSequence = cbRoadSequence.GetItemData(nIdx);
		m_cRoadSeq.Format(_T("%d"), lRoadSequence );	

		SetRoadSequence();

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg メンバ関数定義
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
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

	LONG					m_lAccessTabNum;			//!< アクセスポイントタブの表示位置保存用
	INT						m_iChangeType;				//!< コントロール切り替え用

private:
	void CreateControlRelation();
	void ChangeWindowMgrMap( INT iIndex );
	void SetRoadSequence();
	void ChangeRoadSeqCombobox();
	bool SetLineClassList(LONG nLineClass);
	bool GetLineClass(int* pClass);
	bool SetLineClass(int nClass);
	bool RedrawRoadNameList(LONG nRoadCode);
	bool GetRoadCode(int* pRoadCode);
	bool SetRoadCode(int nRoadCode);

	ITablePtr GetRoadCodeTable();
	bool GetRoadCodeSet(CHetRoadCodeSet* pCodeSet);
	void ChangeHwyModeCheckBox(long cRoadCode);

	/**
	 * @brief Access_Store情報グリッドの内容を更新する（[Bug5556]で追加）
	 * @note	キャッシュから値を取得してグリッド内を全て書き換える。
	 */
	void UpdateASGrid();

	/**
	 * @brief Access_Storeタブ内のコントロール群をAccess_Store情報グリッドの状態に従って更新する（[Bug5556]で追加）
	 */
	void UpdateASControls();

	/**
	 * @brief Access_Store情報グリッド内のセルの色を設定する（[Bug5556]で追加）
	 */
	void SetASGridColor();

	/**
	 * @brief Access_Store情報グリッドの指定されたセルの値をcControlIDで指定されたエディットコントロールに反映する
	 * @param[in]	cRow		対象行の行番号
	 * @param[in]	cCol		対象列の列番号
	 * @param[in]	cControlID	対象エディットコントロールのコントロールID
	 */
	void UpdateASEditControl( long cRow, long cCol, long cControlID );

	/**
	 * @brief Access_Store情報グリッドの指定されたセルの値をcControlIDで指定されたチェックボックスに反映する
	 * @param[in]	cRow		対象行の行番号
	 * @param[in]	cCol		対象列の列番号
	 * @param[in]	cControlID	対象エディットコントロールのコントロールID
	 */
	void UpdateASCheckBox( long cRow, long cCol, long cControlID );

	/**
	 * @brief Access_Storeタブの編集可能なコントロール群のコントロールID集合を返す（[Bug5556]で追加）
	 * @note	編集可否に応じてEnabled/Disabledを切り替えるべき対象コントロール群を得るのに必要。
	 * @return	コントロールID集合
	 */
	std::vector<long> GetASEditableControlIDs();

	/**
	 * @brief Access_Storeタブのチェックボックス群のコントロールID集合を返す（[Bug5556]で追加）
	 * @note	「全選択」「全解除」ボタンによる状態変更対象コントロール群を得るのに必要。
	 * @return	コントロールID集合
	 */
	std::vector<long> GetASCheckBoxIDs();

	/**
	 * @brief Access_Storeテーブルを読み込む（[Bug5556]で追加）
	 * @note	ダイアログに表示されているアクセスポイント群に関連付いたAccess_Storeオブジェクトを読み込み、キャッシュを作成する。
	 */
	void LoadAccessStore();

	/**
	 * @brief 「追加」「更新」「削除」ボタンが押された時、その内容に応じてキャッシュを更新する（[Bug5556]で追加）
	 * @note	OnCommand()内で呼び出され、押されたボタンが何であるかは呼び出し後に判定する。
	 * @param[in]	cControlID	押されたボタンのコントロールID
	 */
	void EditAccessStore( long cControlID );

	/**
	 * @brief Access_Storeグリッドの内容をDBに反映する
	 */
	void SaveAccessStore();

	/**
	 * @brief Access_Storeキャッシュの対象行に含まれる全フィールドの値の妥当性をチェックする
	 * @param[in]	crRow	対象行
	 * @retval	true	全ての値は妥当である
	 * @retval	false	妥当でない値が存在する
	 */
	bool CheckASValuesValidity( const access_store_cache::CASRow& crRow );

		/**
		 * @brief 指定されたフィールドの値が許容範囲内にあるか否かを判定し、範囲外にあればエラーダイアログを表示する
		 * @param[in]	crRow		対象行
		 * @param[in]	cMin		許容範囲の最小値
		 * @param[in]	cMax		許容範囲の最大値
		 * @param[in]	cFieldName	フィールド名
		 * @param[in]	cFieldAlias	フィールド名の別名（エラーメッセージ用）
		 * @return	判定結果
		 */
	bool IsValidASValue( const access_store_cache::CASRow& crRow, tstring cFieldName, const CString& cFieldAlias, long cMin, long cMax );

	/**
	 * @brief エディットコントロール群から取得した情報によってAccess_Storeキャッシュを更新する
	 * @param[in]	cControlID	対象エディットコントロールのコントロールID
	 * @param[in]	cObjectID	対象Access_StoreオブジェクトのID
	 * @return	成否
	 */
	bool UpdateASCacheByEditControls( long cControlID, long cObjectID );

		/**
		 * @brief エディットコントロールから取得した情報によってAccess_Storeキャッシュを更新する
		 * @param[in]	cControlID	対象エディットコントロールのコントロールID
		 * @param[in]	cObjectID	対象Access_StoreオブジェクトのID
		 * @param[in]	cFieldName	対象フィールド名
		 */
		void UpdateASCacheByEditControl( long cControlID, long cObjectID, tstring cFieldName );

		/**
		 * @brief エディットコントロールから数値を取得する
		 * @note	数値を正常に取得できない場合は、boost::lexical_castの失敗による例外が送出される。
		 * @param[in]	cControlID	対象エディットコントロールのコントロールID
		 * @return	取得された数値
		 */
		long GetNumFromEditControl( long cControlID );

	/**
	 * @brief チェックボックスから取得した情報によってAccess_Storeキャッシュを更新する
	 * @param[in]	cControlID	対象チェックボックスのコントロールID
	 * @param[in]	cObjectID	対象Access_StoreオブジェクトのID
	 * @param[in]	cFieldName	対象フィールド名
	 */
	void UpdateASCacheByCheckBox( long cControlID, long cObjectID, tstring cFieldName );

////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,				//!< 「基本属性」タブの各コントロールウィンドウ
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10;
	CContainedWindow	m_wndInfo1,m_wndInfo2,m_wndInfo3,				//!< 「施設情報」タブの各コントロールウィンドウ
						m_wndInfo4,m_wndInfo5,m_wndInfo6,m_wndInfo7,
						m_wndInfo8,m_wndInfo9,m_wndInfo10,m_wndInfo11,
						m_wndInfo12,m_wndInfo13,m_wndInfo14,m_wndInfo15,
						m_wndInfo16,m_wndInfo17,m_wndInfo18,m_wndInfo19,
						m_wndInfo20,m_wndInfo21;
	CContainedWindow	m_wndAS1,m_wndAS2,m_wndAS3,m_wndAS4,m_wndAS5,	//!< 「Access_Store」タブの各コントロールウィンドウ（[Bug5556]で追加）
						m_wndAS6,m_wndAS7,m_wndAS8,m_wndAS9,m_wndAS10,
						m_wndAS11,m_wndAS12,m_wndAS13,m_wndAS14,m_wndAS15,
						m_wndAS16,m_wndAS17,m_wndAS18,m_wndAS19,m_wndAS20,
						m_wndAS21,m_wndAS22,m_wndAS23;
	CContainedWindow	m_wndTab;										//!< タブコントロール
	
	IVSFlexGridPtr		m_ipAccessStoreGrid;	//!< Access_Store情報グリッド（[Bug5556]で追加）
	access_store_cache::CASCache	m_ASCache;	//!< Access_Storeテーブルのキャッシュ（[Bug5556]で追加）

	CString		m_cRoadCode;				//!< 文字列での路線コード
	CString		m_cRoadSeq;					//!< 文字列での路線内シーケンス
	// ダイアログの値保存用データ
	LONG	m_lLineClass;					//!< 現在選択中の路線種別コード
	LONG	m_lRoadCode;					//!< 現在選択中の路線コード
	// 元の値
	LONG	m_lOrgRoadSequence;				//!< 最初の路線内シーケンス
	LONG	m_lOrgRoadCode;					//!< 最初の路線コード

	LONG	m_lFeatureIndex;				//!< アクセスポイントダイアログ側でのフィーチャインデックス保持用
	LONG	m_lTableIndex;					//!< アクセスポイントダイアログ側でのテーブルインデックス保持用
	LONG	m_lRowIndex;					//!< アクセスポイントダイアログ側でのRowインデックス保持用
	BOOL	m_bForce;						//!< アクセスポイントダイアログ側での強制編集モード保持用
	BOOL	m_bEditable;					//!< アクセスポイントダイアログ側での編集できるかどうか保持用

	// ハイウェイ編集用各種データのコンテナ
	CHetRoadCodeSet m_cRoadCodeSet;			//!< 路線コード情報コンテナ
	CCodeValueDomain m_cLineClassDomain;	//!< 路線種別コード情報コンテナ
	HFONT	m_hFont;						//!< 文字フォント
	HWND	m_hChildWnd;					//!< 子ダイアログハンドル

	static const long m_MasterCodeMaxLength;	//!< チェーン店マスターコードエディットコントロール内の数値の最大桁数（[Bug5556]で追加）
	static const long m_TimeStrMaxLength;		//!< 時刻（時または分）を表すエディットコントロール内の数値の最大桁数（[Bug5556]で追加）
	static const long m_ASEditBufferLength;		//!< Access_Storeタブのエディットコントロールから文字列を取得する際に使用するバッファの長さ（[Bug5556]で追加）
};

#endif // _ATTRACCESSPOINTDLG_

