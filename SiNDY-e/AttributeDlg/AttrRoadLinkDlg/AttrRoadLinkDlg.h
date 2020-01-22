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

// AttrRoadLinkDlg.h : CAttrRoadLinkDlg の宣言

#ifndef __ATTRROADLINKDLG_H_
#define __ATTRROADLINKDLG_H_

#include "resource.h"       // メイン シンボル

using namespace std;
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrRoadLinkDlg
class CAttrRoadLinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg,
	public IDispEventImpl<IDC_GRID_NOPASSAGE, CAttrRoadLinkDlg>,	// 時間規制用グリッドコントロール
	public IDispEventImpl<IDC_GRID_ONEWAY, CAttrRoadLinkDlg>		// 時間規制用グリッドコントロール
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
		EDITVIEW_ROAD_KIHON,		//!< 道路の基本属性（第１タブ）
		EDITVIEW_ROAD_TUIKA,		//!< 道路の追加属性（第２タブ）
		EDITVIEW_ROAD_KOTUKISEI,	//!< 道路の交通規制属性（第３タブ）
		EDITVIEW_ROAD_ROSEN,		//!< 道路の路線属性（第４タブ）
		EDITVIEW_ROAD_DRMA,			//!< 道路のDRMA属性（第５タブ）
		EDITVIEW_ROAD_PARKING,		//!< 道路の駐車場属性（第６タブ）
	};

	/**
	* 一括変更用の属性保持型
	*
	*/
	struct BatchChange {
		CString NAME;			//!< 定義名
		CString DISPCLASS;		//!< 道路種別
		CString NAVICLASS;		//!< 経路種別
		CString MANAGER;		//!< 道路管理者
		CString ROADWIDTH;		//!< 幅員
		CString LINKCLASS;		//!< リンク種別
		CString LANE;			//!< 車線数
		CString LEGALSPEED;		//!< 最高速度
		CString ALLEYMAINTE;	//!< 細街路作業年度
		CString ROADCODE;		//!< 路線番号コード
		CString ROADNUMBER;		//!< 路線番号
		CString KUBUNCODE;		//!< 主従区分コード
		CString LIMITSPEED;		//!< 規制速度コード
		CString TRIPSPEED;		//!< 旅行速度
	};

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
public:
BEGIN_MSG_MAP(CAttrRoadLinkDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHPREFCODE, BN_CLICKED, OnClickedSearchPrefButton)           // [bug 12036]
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADNUMBER, BN_CLICKED, OnClickedSearchRoadNumberButton)   // [bug 12036]
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADCODE, BN_CLICKED, OnClickedSearchRoadCodeButton)       // [bug 12036]
	COMMAND_HANDLER(IDC_BUTTON_SEARCHSECTIONCODE, BN_CLICKED, OnClickedSearchSectionCodeButton) // [bug 12036]
	COMMAND_HANDLER(IDC_EDIT_RIGHTCHANGE, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_EDIT_LEFTCHANGE, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_EDIT_FLOORLEVEL, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_EDIT_MIDDLEFLOOR_LEVEL, EN_UPDATE, OnUpdateEditValue)
	COMMAND_HANDLER(IDC_CHECK_ISLANDF, BN_CLICKED, OnCheckWideAreaOrIsland)		// [bug 6712]
	COMMAND_HANDLER(IDC_CHECK_WIDEAREAF, BN_CLICKED, OnCheckWideAreaOrIsland)	// [bug 6712]
	COMMAND_HANDLER(IDC_COMBO_BATCHCHANGE, CBN_SELCHANGE, OnChangeBatchChangeButton)	// [bug 9397]

	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)

ALT_MSG_MAP(IDC_COMBO_BATCHCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_DISPCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_NAVICLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_MANAGER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADWIDTH)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LINKCLASS1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_CHECK_ISLANDF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_BYPASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CARONLYF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_WIDEAREA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_WIDEAREAF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_UPDOWNC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_LANE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LEGALSPEED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RIGHTCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LEFTCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ALLEYMAINTE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_WORKYEAR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_WALKABLE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_COMBO_ONEWAY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_NOPASSAGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

// [bug 12036]
ALT_MSG_MAP(IDC_EDIT_ROADCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHROADCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_PREFCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHPREFCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROADNUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHROADNUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HIGHWAYNUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SECTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCHSECTIONCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)


ALT_MSG_MAP(IDC_COMBO_KUBUNCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LIMITSPEED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TRIPSPEED)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)


ALT_MSG_MAP(IDC_COMBO_PARKINGCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_PARKINGLINKCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FLOORLEVEL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_MIDDLEFLOOR_LEVEL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_TAB_ROADATTR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

	END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( !m_ipRule )
			m_ipRule = GetRule();
		// コントロール
		CreateControlRelation();

		// 時間規制用グリッドコントロール
		CAxWindow wndGrid = GetDlgItem(IDC_GRID_ONEWAY);
		wndGrid.QueryControl(&m_spOnewayGrid);

		wndGrid =GetDlgItem(IDC_GRID_NOPASSAGE);
		wndGrid.QueryControl(&m_spNopassageGrid);

		// グリッドコントロールからのイベントを拾うのに必要
		AtlAdviseSinkMap(this, TRUE);

		CString strTemp;  //!< pszTextに渡す文字列を一時的に格納する変数

		// 道路属性編集用タブコントロールにアイテムを追加します
		HWND hwndTab = GetDlgItem( IDC_TAB_ROADATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_EXTENTION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_TUIKA, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_REGULATION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_KOTUKISEI, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NUMBER);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_ROSEN, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_DRMA);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_DRMA, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_PARKING);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_ROAD_PARKING, (LPARAM)(&pTabCtrlItem));

		// サブクラス化
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_DISPCLASS));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_NAVICLASS));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_COMBO_MANAGER));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_COMBO_ROADWIDTH));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_LINKCLASS1));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_COMBO_BATCHCHANGE));	// [bug 9397] 
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

		m_wndTuika0.SubclassWindow(GetDlgItem(IDC_CHECK_ISLANDF));
		m_wndTuika1.SubclassWindow(GetDlgItem(IDC_CHECK_BYPASS));
		m_wndTuika2.SubclassWindow(GetDlgItem(IDC_CHECK_CARONLYF));
		m_wndTuika3.SubclassWindow(GetDlgItem(IDC_COMBO_WIDEAREA));
		m_wndTuika4.SubclassWindow(GetDlgItem(IDC_COMBO_UPDOWNC));
		m_wndTuika5.SubclassWindow(GetDlgItem(IDC_CHECK_LANE));
		m_wndTuika6.SubclassWindow(GetDlgItem(IDC_EDIT_LANE));
		m_wndTuika7.SubclassWindow(GetDlgItem(IDC_EDIT_RIGHTCHANGE));
		m_wndTuika8.SubclassWindow(GetDlgItem(IDC_EDIT_LEFTCHANGE));
		m_wndTuika9.SubclassWindow(GetDlgItem(IDC_COMBO_ALLEYMAINTE));
		m_wndTuika10.SubclassWindow(GetDlgItem(IDC_EDIT_WORKYEAR));
		m_wndTuika11.SubclassWindow(GetDlgItem(IDC_CHECK_WIDEAREAF));
		m_wndTuika12.SubclassWindow(GetDlgItem(IDC_COMBO_LEGALSPEED));
		m_wndTuika13.SubclassWindow(GetDlgItem(IDC_COMBO_WALKABLE));

		m_wndKisei0.SubclassWindow(GetDlgItem(IDC_COMBO_ONEWAY));
		m_wndKisei1.SubclassWindow(GetDlgItem(IDC_COMBO_NOPASSAGE));

		m_wndRosen0.SubclassWindow(GetDlgItem(IDC_EDIT_ROADCODE));			  // [bug 12036]
		m_wndRosen1.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHROADCODE));	  // [bug 12036]
		m_wndRosen2.SubclassWindow(GetDlgItem(IDC_EDIT_PREFCODE));			  // [bug 12036]
		m_wndRosen3.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHPREFCODE));	  // [bug 12036]
		m_wndRosen4.SubclassWindow(GetDlgItem(IDC_EDIT_ROADNUMBER));		  // [bug 12036]
		m_wndRosen5.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHROADNUMBER));  // [bug 12036]
		m_wndRosen6.SubclassWindow(GetDlgItem(IDC_EDIT_HIGHWAYNUMBER));		  // [bug 12036]
		m_wndRosen7.SubclassWindow(GetDlgItem(IDC_EDIT_SECTIONCODE));		  // [bug 12036]
		m_wndRosen8.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCHSECTIONCODE)); // [bug 12036]

		m_wndDRMA0.SubclassWindow(GetDlgItem(IDC_COMBO_KUBUNCODE));
		m_wndDRMA1.SubclassWindow(GetDlgItem(IDC_COMBO_LIMITSPEED));
		m_wndDRMA2.SubclassWindow(GetDlgItem(IDC_EDIT_TRIPSPEED));

		m_wndParking0.SubclassWindow(GetDlgItem(IDC_COMBO_PARKINGCLASS));
		m_wndParking1.SubclassWindow(GetDlgItem(IDC_COMBO_PARKINGLINKCLASS));
		m_wndParking2.SubclassWindow(GetDlgItem(IDC_EDIT_FLOORLEVEL));
		m_wndParking3.SubclassWindow(GetDlgItem(IDC_EDIT_MIDDLEFLOOR_LEVEL));

		m_wndTab.SubclassWindow(GetDlgItem(IDC_TAB_ROADATTR));
		// 道路コードとを見に行くのを１回にするために、各フラグの初期化
		m_bRoadCordFirstSearch = FALSE;

		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);

		ChangeWindowMgrMap( m_iChangeType );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlAdviseSinkMap(this, FALSE);

		return 0;  // システムにフォーカスを設定させます
	}
	/**
	 * WM_NOTIFY 用イベントハンドラ
	 *
	 * 以下の処理を行います
	 * ・タブの切り替えが発生した時
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_ROADATTR ) {
			// 道路リンクリレーションをチェックしタブのハイライトをする
			CString strMessage;
			CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// コントロール切り替え
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;	// 道路タブの場合のみタブのIDを保持
				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_lTabNum, m_bForce, m_bEditable );

				// 各タブでの初期化処理
				switch( m_lTabNum )
				{
				case EDITVIEW_ROAD_ROSEN:
					if( !m_bRoadCordFirstSearch )
					{
						// 道路名称を出す [bug 12036]
						SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
						::SetFocus( GetDlgItem(IDC_EDIT_ROADCODE) );
						m_bRoadCordFirstSearch = TRUE;

						// 都道府県名を出す [bug 12036]
						SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);

						// 県道名称を出す [bug 12036]
						SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);

						// 区間別称を出す [bug 12036]
						SetSectionName(IDC_EDIT_SECTIONCODE,IDC_EDIT_SECTIONNAME);
					}
					break;
				case EDITVIEW_ROAD_KOTUKISEI:
					SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
					SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );
					break;
				case EDITVIEW_ROAD_TUIKA:
					AdjustTuikaTabLayout();
					break;
				case EDITVIEW_ROAD_KIHON:
					// 一括変更用のコンボに値をセット [bug 9397] 
					AddComboValue();

					// どこかの値が変更されている場合は一括変更コンボを使用不可とする [bug 9449]
					// 編集開始されていないレイヤだったら変更しない（bug 9719）
					if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
						SetDlgStatus(IDC_COMBO_BATCHCHANGE, !Changed() );
					break;
				default:
					break;
				}

				bHandled = TRUE;

				if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
					SetForcusAfterTabChange();
				return 1;
			}
		}

		if( wParam == 0 ) {
			// 道路リンクリレーションをチェックしタブのハイライトをする
			CString strMessage;
			CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
			HWND hwndTab = GetDlgItem( IDC_TAB_ROADATTR );
			if( lParam == -2 || lParam == -1)
			{
				// コントロール切り替え
				if( lParam == -2 )
				{
					if( m_iChangeType + 1 == 6 )
					{
						m_iChangeType = 0;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
					else
					{
						m_iChangeType = m_iChangeType + 1;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
				}
				else if( lParam == -1 )
				{
					if( m_iChangeType - 1 == -1 )
					{
						m_iChangeType = 5;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
					else
					{
						m_iChangeType = m_iChangeType - 1;
						::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0);
					}
				}
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;	// 道路タブの場合のみタブのIDを保持
				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_lTabNum, m_bForce, m_bEditable );

				if( m_lTabNum == 3 && m_bRoadCordFirstSearch == FALSE)
				{
					// 道路名称を出す
					SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
					::SetFocus( GetDlgItem(IDC_EDIT_ROADCODE) );
					m_bRoadCordFirstSearch = TRUE;

					// 都道府県名を出す [bug 12036]
					SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);

					// 県道名称を出す [bug 12036]
					SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);

					// 区間別称を出す [bug 12036]
					SetSectionName(IDC_EDIT_SECTIONCODE, IDC_EDIT_SECTIONNAME);
				}
				bHandled = TRUE;
				SetForcusAfterTabChange();

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			INT nID = (INT)LOWORD(wParam);
			// 値変更
			m_cControlRel.ControlChanged( nID );
			// 道路リンクリレーションをチェックしタブのハイライトをする
			CString strMessage;
			CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
			// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
			CopyMeshRoad();
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
			// どこかの値が変更されている場合は一括変更コンボを使用不可とする [bug 9449]
			SetDlgStatus(IDC_COMBO_BATCHCHANGE, !Changed() );
		}

		return 0;
	}

	// 路線名称コードに対する「code検索」ボタンが押された時の処理 [bug 12036]
	LRESULT OnClickedSearchRoadCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// 都道府県コードに対する「code検索」ボタンが押された時の処理 [bug 12036]
	LRESULT OnClickedSearchPrefButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// 道路番号に対する「No.検索」ボタンが押された時の処理 [bug 12036]
	LRESULT OnClickedSearchRoadNumberButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// 区間別称コードに対する「code検索」ボタンが押された時の処理 [bug 12036]
	LRESULT OnClickedSearchSectionCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * 右側増減入力エディットボックス用イベントハンドラ
	 * 左側増減入力エディットボックス用イベントハンドラ
	 * 駐車場階層入力エディットボックス用イベントハンドラ
	 * 駐車場階層間レベル入力エディットボックス用イベントハンドラ
	 *
	 * エディットボックスの文字が操作される場合、ここで処理
	 * 入力文字を見張って不正文字は入力させないようにする
	 */
	LRESULT OnUpdateEditValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 入力のチェック
		CheckInputValue( hWndCtl, wID );
		// 道路リンクリレーションをチェックしタブのハイライトをする
		CString strMessage;
		CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());
		// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
		CopyMeshRoad();
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );

		return 0;
	}

	LRESULT OnCheckWideAreaOrIsland(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/**
	 * 一括変更コンボボックス用イベントハンドラ
	 *
	 * コンボボックスの値を変更した場合、ここで処理
	 * 変更した値に合わせて一括変更フィールドを全て更新する
	 */
	LRESULT OnChangeBatchChangeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// 選択された一括変更の情報を取得
		CComboBox cbBatchChange(GetDlgItem(IDC_COMBO_BATCHCHANGE));
		m_nIdx = cbBatchChange.GetCurSel();

		if( m_nIdx == CB_ERR )
			return 0;
		int key = cbBatchChange.GetItemData(m_nIdx);
		BatchChange bChange = m_mapBatchChange[key];

		// 一括変更の情報から、実際に属性をセットする
		SetBatchValue( IDC_COMBO_DISPCLASS, bChange.DISPCLASS );	// 道路種別
		SetBatchValue( IDC_COMBO_NAVICLASS, bChange.NAVICLASS );	// 経路種別
		SetBatchValue( IDC_COMBO_MANAGER,	bChange.MANAGER );		// 道路管理者
		SetBatchValue( IDC_COMBO_ROADWIDTH, bChange.ROADWIDTH );	// 幅員
		SetBatchValue( IDC_COMBO_LINKCLASS1,bChange.LINKCLASS );	// リンク種別
		SetBatchValue( IDC_EDIT_LANE,		bChange.LANE );			// 車線数
		SetBatchValue( IDC_COMBO_LEGALSPEED,bChange.LEGALSPEED );	// 最高速度
		SetBatchValue( IDC_COMBO_ALLEYMAINTE,bChange.ALLEYMAINTE );	// 細街路作業年度
		SetBatchValue( IDC_EDIT_ROADCODE,	bChange.ROADCODE );		// 路線番号コード
		SetBatchValue( IDC_EDIT_ROADNUMBER, bChange.ROADNUMBER );	// 路線番号
		SetBatchValue( IDC_COMBO_KUBUNCODE, bChange.KUBUNCODE );	// 主従区分コード
		SetBatchValue( IDC_COMBO_LIMITSPEED,bChange.LIMITSPEED );	// 規制速度コード
		SetBatchValue( IDC_EDIT_TRIPSPEED,	bChange.TRIPSPEED );	// 旅行速度

		// 変更された場合はボタンを Enable に
		SetButton( Changed() );

		// 一括変更で変更した際は、一括変更コンボを使用不可とする [bug 9449]
		SetDlgStatus(IDC_COMBO_BATCHCHANGE, !Changed() );

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg イベントシンクマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAttrRoadLinkDlg)
	///Make sure the Event Handlers have __stdcall calling convention
	SINK_ENTRY(IDC_GRID_NOPASSAGE, DISPID_MOUSEDOWN, OnMouseDownNopassageGrid)
	SINK_ENTRY(IDC_GRID_ONEWAY, DISPID_MOUSEDOWN, OnMouseDownOnewayGrid)
	SINK_ENTRY(IDC_GRID_NOPASSAGE, 0x16, OnStartEditNoPassageGrid)
	SINK_ENTRY(IDC_GRID_ONEWAY, 0x16, OnStartEditOnewayGrid)
END_SINK_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid イベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	/**
	 * 時間規制用でセルがクリックされた時に発生するイベントハンドラ
	 *
	 * 0カラム目のセルの色変更やメニューの表示を行います
	 */
	VOID __stdcall OnMouseDownNopassageGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
	{
		MouseDownTimeRegGrid( IDC_GRID_NOPASSAGE, m_spNopassageGrid, m_lCurrentNopassageIndex, TIME_NOPASSAGE_TABLE_NAME, Button, Shift, X, Y );
	}
	VOID __stdcall OnMouseDownOnewayGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y)
	{
		MouseDownTimeRegGrid( IDC_GRID_ONEWAY, m_spOnewayGrid, m_lCurrentOnewayIndex, TIME_ONEWAY_TABLE_NAME, Button, Shift, X, Y );
	}
	VOID __stdcall OnStartEditNoPassageGrid(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
	{
		*Cancel = true;
	}
	VOID __stdcall OnStartEditOnewayGrid(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
	{
		*Cancel = true;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrRoadLinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID);
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void ChangeWindowMgrMap( INT iIndex );
	void CreateControlRelation();

	/**
	* @brief カーソルを取得する [bug 12036]
	*
	* @param ipTable		[in]	検索対象テーブルのITablePtr
	* @param ipCursor		[out]	取得したカーソルを格納する_ICursorPtr
	* @param strWhereClause	[in]	検索に用いるWHERE句
	* @param strSubFields	[in]	検索に用いるフィールド指定句
	*
	* @retval	true : 取得成功
	* @retval	false: 取得失敗
	*/
	bool GetCursor(const ITablePtr& ipTable, _ICursorPtr& ipCursor, const CString& strWhereClause, const CString& strSubFields);

	/**
	* @brief エディットボックスに入力された数値を取得する [bug 12036]
	*
	* @param nCodeID		[in]	値が入力されているエディットコントロールのID
	*/
	long GetWindowNumberText(int nCodeID);

	/**
	* @brief 路線検索を行い、路線名を表示する [bug 12036]
	*
	* 路線名称テーブルである ROAD_CODE_LIST から名称を検索し、第2引数以下で示される各コントロールに表示します
	*
	* @param nCodeID		[in]	路線名称コードが入っているエディットコントロールのID
	* @param nNameLabelID	[in]	路線名称を表示するスタティックコントロールのID
	* @param nDispLabelID	[in]	路線通称を表示するスタティックコントロールのID
	* @param nShieldNumberLabelID	[in]	都市高速番号を表示するスタティックコントロールのID
	*/
	void SetRoadName(int nCodeID, int nNameLabelID, int nDispLabelID, int nShieldNumberLabelID);

	/**
	* @brief 都道府県名検索を行い、都道府県名称を表示する [bug 12036]
	*
	* 都道府県名リスト から検索し、第2引数で示されるコントロールに表示します
	*
	* @param nCodeID	[in]	都道府県コードが入っているエディットコントロールのID
	* @param nLabelID	[in]	都道府県名称を表示するスタティックコントロールのID
	*/
	void SetPrefName(int nCodeID, int nLabelID);

	/**
	* @brief CITY_ADMINからPREF_CODEを取得し、表示する [bug 12036]
	*
	* @param nCodeID			[in]	都道府県コードが入っているエディットコントロールのID
	* @param nRoadNumberID		[in]	道路番号が入っているエディットコントロールのID
	*/
	void SetPrefCodeFromCityAdmin(int nCodeID);

	/**
	* @brief 道路リンクが存在するCITY_ADMINポリゴンのCITYCODEフィールドからPREF_CODEを取得する [bug 12036]
	*/
	long GetPrefCodeFromCityAdmin();

	/**
	* @brief 都道府県道名称検索を行い、都道府県道名称を表示する [bug 12036]
	*
	* 都道府県名リスト から名称を検索し、第3引数で示されるコントロールに表示します
	* 都道府県道名称リストである PREF_ROAD_LIST から名称を検索し、第4引数で示されるコントロールに表示します
	*
	* @param nPrefCodeID		[in]	都道府県コードが入っているエディットコントロールのID
	* @param nRoadNumberID		[in]	道路番号が入っているエディットコントロールのID
	* @param nPrefNameLabelID	[in]	都道府県名称を表示するスタティックコントロールのID
	* @param nPrefRoadLabelID	[in]	都道府県道名称を表示するスタティックコントロールのID
	*/
	void SetPrefRoadName(int nPrefCodeID, int nRoadNumberID, int nPrefNameLabelID, int nPrefRoadLabelID);

	/**
	* @brief 区間別称コード検索を行い、区間別称名を表示する [bug 12036]
	*
	* 区間別称テーブルである SECTION_CODE_LIST から検索し、第2引数で示されるコントロールに表示します
	*
	* @param nCodeID	[in]	区間別称コードが入っているエディットコントロールのID
	* @param nLabelID	[in]	区間別称を表示するスタティックコントロールのID
	*/
	void SetSectionName(int nCodeID, int nLabelID);

	/**
	* @brief エディットボックスに入力された数値のチェックを行う [bug 12036]
	*
	* @param nCodeID	[in]	値が入力されたエディットコントロールのID
	*
	* @retval	true : 設定値内
	* @retval	false: 設定値外
	*/
	bool CheckNumVal(int nCodeID);

	/**
	* @brief 路線名称コードまたは区間別称コードの値をチェック [bug 12036]
	*
	* @param	lNumVal	[in]	路線名称コードまたは区間別称コードの値
	*
	* @retval	true : 設定値内
	* @retval	false: 設定値外
	*/
	bool CheckRoadCodeSectionCode(long lNumVal);

	/**
	* @brief 都道府県コードの値をチェック [bug 12036]
	*
	* @param	lPrefCode	[in]	路線名称コード値
	*
	* @retval	true : 設定値内
	* @retval	false: 設定値外
	*/
	bool CheckPrefCode(long lPrefCode);

	/**
	* @brief 都道府県コード、道路番号の値をチェック [bug 12036]
	*
	* @param	lRoadNo		[in]	道路番号
	*
	* @retval	true : 設定値内
	* @retval	false: 設定値外
	*/
	bool CheckRoadNo(long lRoadNo);

	void MouseDownTimeRegGrid( INT nID, IVSFlexGridPtr spGrid, LONG& lIndex, LPCTSTR lpcszTableName, SHORT Button, SHORT Shift, FLOAT X, FLOAT Y );
	BOOL CheckOneWayTimeReg();
	BOOL TimeRegErrorCheck(list<CRelationalTableDef>* pRelTableDef, INT iOneWay);

	/**
	* @brief 道路種別に応じて都道府県コード、道路番号の入力に問題がないかをチェックする [bug 12036]
	*
	*        道路種別が(2,4,103,104,106)のときのみ、都道府県コード入力可能
	*        道路種別が(1,2,4,103,104,106)のときのみ、道路番号入力可能
	*         1 : 一般国道
	*         2 : 主要地方道(都府県)
	*         4 : 一般都道府県道
	*        103: 一般国道(有料)
	*        104: 主要地方道(都道府県)(有料)
	*        106: 一般都道府県道(有料)
	*
	* @param strMsg		[out]	ワーニングメッセージの内容
	*
	* @retval	true : 都道府県コード、道路番号を入力可能
	* @retval	false: 都道府県コード、道路番号を入力不可
	*/
	bool CheckPrefCodeRoadNo(CString& strMsg, CFeatureDef& pFeatureDef);
	LONG CheckRoadLinkRelation(CString *strMessage, CFeatureDef *pFeatureDef);

	/**
	* @brief 番号タブに入力された各コード値に該当名称が存在するかチェックする [bug 12036]
	*
	* @param strMsg		[out]	ワーニングメッセージの内容
	*
	* @retval	true : 該当名称あり
	* @retval	false: 該当名称なし
	*/
	bool CheckNameExists(CString& strMsg, CFeatureDef& pFeatureDef);

	/**
	* @brief 入力された路線名称コードに該当名称が存在するかチェックする [bug 12036]
	*
	* @param strMsg		[out]	ワーニングメッセージの内容
	*/
	void CheckRoadCodeNameExists(CString& strMsg, CFeatureDef& pFeatureDef);

	/**
	* @brief 入力された都道府県コード、道路番号に該当名称が存在するかチェックする [bug 12036]
	*
	* @param strMsg		[out]	ワーニングメッセージの内容
	*/
	void CheckPrefRoadNameExists(CString& strMsg, CFeatureDef& pFeatureDef);

	/**
	* @brief 入力された区間別称コードに該当名称が存在するかチェックする [bug 12036]
	*
	* @param strMsg		[out]	ワーニングメッセージの内容
	*/
	void CheckSectionNameExists(CString& strMsg, CFeatureDef& pFeatureDef);
	void SetForcusAfterTabChange();
	void TabHighLight();
	void CopyMeshRoad();
	void AdjustTuikaTabLayout();
	HRESULT CreateActiveXControls(UINT nID);
	void LoadIniFile();
	void GetRegCode();
	void AddComboValue();
	void SetBatchValue(INT nID, CString& Value);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	LONG				m_lTabNum;					//!< 道路属性タブの表示位置保存用
	INT					m_iChangeType;				//!< コントロール切り替え用

private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5,m_wnd6;  //!< 各コントロールウィンドウ
	CContainedWindow	m_wndTuika0,m_wndTuika1,m_wndTuika2,m_wndTuika3,
						m_wndTuika4,m_wndTuika5,m_wndTuika6,m_wndTuika7,
						m_wndTuika8,m_wndTuika9,m_wndTuika10,m_wndTuika11,
						m_wndTuika12,m_wndTuika13;                         //!< 各コントロールウィンドウ
	CContainedWindow	m_wndKisei0,m_wndKisei1;                           //!< 各コントロールウィンドウ
	CContainedWindow	m_wndRosen0,m_wndRosen1,m_wndRosen2,m_wndRosen3,
						m_wndRosen4,m_wndRosen5,m_wndRosen6,m_wndRosen7,
						m_wndRosen8;                                       //!< 各コントロールウィンドウ
	CContainedWindow	m_wndDRMA0,m_wndDRMA1,m_wndDRMA2;                  //!< 各コントロールウィンドウ
	CContainedWindow	m_wndParking0,m_wndParking1,m_wndParking2,m_wndParking3; //!< 各コントロールウィンドウ
	CContainedWindow	m_wndTab;                                          //!< タブのコントロールウィンドウ

	LONG				m_lCurrentNopassageIndex;	//!< 時間規制の0カラム目色変更戻し用インデックス番号保持用
	LONG				m_lCurrentOnewayIndex;		//!< 時間規制の0カラム目色変更戻し用インデックス番号保持用
	IVSFlexGridPtr		m_spNopassageGrid;			//!< 時間規制用のグリッド
	IVSFlexGridPtr		m_spOnewayGrid;				//!< 時間規制用のグリッド

	BOOL	m_bRoadCordFirstSearch;					//!< 道路名称を1度検索したかどうかのフラグ
	LONG	m_lFeatureIndex;						//!< 道路リンクダイアログ側でのフィーチャインデックス保持用
	LONG	m_lTableIndex;							//!< 道路リンクダイアログ側でのテーブルインデックス保持用
	LONG	m_lRowIndex;							//!< 道路リンクダイアログ側でのRowインデックス保持用
	BOOL	m_bForce;								//!< 道路リンクダイアログ側での強制編集モード保持用
	BOOL	m_bEditable;							//!< 道路リンクダイアログ側での編集できるかどうか保持用

	BOOL				m_bIsRoadOnMesh;			//!< 道路リンクがメッシュを跨いでいるかどうかのフラグ
	CTargetItems		m_cRoadsOnMesh;				//!< メッシュを跨いでいる道路リンク保持用
	list<CFeatureDef>	m_pRoadOnMeshList;			//!< メッシュを跨いでいる道路リンクのFeatureDef保持用

	HWND				m_hChildWnd;				//!< 子ダイアログハンドル
	HWND				m_hAttributeDlgWnd;			//!< 親ウィンドハンドル

	CString				m_strIniPath;				//!< Iniファイルパス [bug 9397] 
	int					m_nIdx;					//!< 一括変更用の定義名インデックス保持用 [bug 9397] 

	map<int, BatchChange>	m_mapBatchChange;	//!< 一括変更要素取得用 [bug 9397]
};

#endif //__ATTRROADLINKDLG_H_
