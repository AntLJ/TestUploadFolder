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

// AttrAccessPointDlg.cpp: CAttrAccessPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinLib/libwinlib.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include "AttrAccessPointDlg.h"
#include <sstream>					// [Bug5556]で追加
#include <boost/lexical_cast.hpp>	// [Bug5556]で追加

const long CAttrAccessPointDlg::m_MasterCodeMaxLength = 8;	// [Bug5556]で追加
const long CAttrAccessPointDlg::m_TimeStrMaxLength = 2;		// [Bug5556]で追加
const long CAttrAccessPointDlg::m_ASEditBufferLength= 32;	// [Bug5556]で追加。「9」で十分なはずだが、念のため大きめに確保。

/// ウィンドウマネージャ配置マップ（アクセスポイント、基本情報）
BEGIN_WINDOW_MAP(ACCESS_POINT_MAP)
	BEGINROWS( WRCT_REST, 0,RCMARGINS(2,2)) 
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_ACCESSPOINT, RCMARGINS(2,2))
				RCSPACE(20)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_CHECK_HWYMODEFLAG)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_ROADCLASS_REF)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_ROADCLASS_REF)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_ROADNAME_REF)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_ROADNAME_REF)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_ROSENCODE)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_ROSENCODE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_SEQUENCE_REF)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_SEQUENCE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_SEQUENCE)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_SEQUENCE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_DIRECTION)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_DIRECTION)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_JUNCTIONCODE)
						RCTOFIT(-1)
						RCREST(IDC_COMBO_JUNCTIONCODE)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_HOUMENKANJI)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_HOUMENKANJI)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_STATIC_HOUMENKANA)
						RCTOFIT(-1)
						RCREST(IDC_EDIT_HOUMENKANA)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 20, 20)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
/// ウィンドウマネージャ配置マップ（アクセスポイント、施設情報）
BEGIN_WINDOW_MAP(ACCESS_POINT_MAP2)
	BEGINROWS( WRCT_REST, 0,RCMARGINS(2,2)) 
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_ACCESSPOINT, RCMARGINS(2,2))
				RCSPACE(30)
				BEGINROWS(WRCT_REST, IDC_STATIC_SHISETUINFO, RCMARGINS(10,10))        // 施設情報
					RCSPACE(10)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_CHECK_RESTAURANT)
							RCTOFIT(IDC_CHECK_RESTING)
							RCTOFIT(IDC_CHECK_INFOMATION)
							RCTOFIT(IDC_CHECK_SPA)
							RCTOFIT(IDC_CHECK_CASHSERVICE)
							RCTOFIT(IDC_CHECK_TOILET)
							RCTOFIT(IDC_CHECK_GS)
						ENDGROUP()
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_CHECK_CAFETERIA)
							RCTOFIT(IDC_CHECK_NAP)
							RCTOFIT(IDC_CHECK_SHOWER)
							RCTOFIT(IDC_CHECK_FAX)
							RCTOFIT(IDC_CHECK_HWYOASIS)
							RCTOFIT(IDC_CHECK_HANDYCAP)
							RCTOFIT(IDC_CHECK_MULTIPLESTORE)
						ENDGROUP()
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCTOFIT(IDC_CHECK_SOUBENIR)
							RCTOFIT(IDC_CHECK_INFODESK)
							RCTOFIT(IDC_CHECK_LAUNDRY)
							RCTOFIT(IDC_CHECK_MAILBOX)
							RCTOFIT(IDC_CHECK_DOGRUN)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
 			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 20, 20)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
/// ウィンドウマネージャ配置マップ（アクセスポイント、Access_Store）（[Bug5556]で追加）
BEGIN_WINDOW_MAP(ACCESS_POINT_MAP3)
	BEGINROWS(WRCT_REST, 0,RCMARGINS(2,2)) 
		BEGINROWS(WRCT_PCT,0,90)											// タブペイン
			BEGINROWS(WRCT_REST, IDC_TAB_ACCESSPOINT, RCMARGINS(2,2))
				RCSPACE(30)
				BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
					RCPERCENT(IDC_GRID_ACCESS_STORE,50)							// グリッドペイン
					BEGINROWS(WRCT_REST, 0, RCMARGINS(10,10))					// 編集可能情報ペイン（マージン指定用）
						BEGINROWS(WRCT_PCT, 0, 60)									// 編集可能情報ペイン1
							BEGINCOLS(WRCT_TOFIT, 0, 0)
								BEGINROWS(WRCT_PCT, 0, 35)								// スタティックテキスト列
									RCFIXED(IDC_STATIC_MASTERCODE,20)
									RCSPACE(5)
									RCFIXED(IDC_STATIC_START_TIME,20)
									RCSPACE(5)
									RCFIXED(IDC_STATIC_END_TIME,20)
								ENDGROUP()
								BEGINROWS(WRCT_PCT, 0, 20)								// エディットボックス列
									BEGINCOLS(WRCT_TOFIT, 0, 0)
										RCFIXED(IDC_EDIT_MASTERCODE,70)
									ENDGROUP()
									RCSPACE(5)
									BEGINCOLS(WRCT_TOFIT, 0, 0)
										RCFIXED(IDC_EDIT_START_HOUR,30)
										RCFIXED(IDC_STATIC_COLON1,10)
										RCFIXED(IDC_EDIT_START_MIN,30)
									ENDGROUP()
									RCSPACE(5)
									BEGINCOLS(WRCT_TOFIT, 0, 0)
										RCFIXED(IDC_EDIT_END_HOUR,30)
										RCFIXED(IDC_STATIC_COLON2,10)
										RCFIXED(IDC_EDIT_END_MIN,30)
									ENDGROUP()
								ENDGROUP()
								BEGINROWS(WRCT_PCT, 0, 25)							// 店舗種別ラジオボタン列
									BEGINROWS(WRCT_REST, IDC_GROUP_STORETYPE, RCMARGINS(12,20))
										RCTOFIT(IDC_RADIO_GS)
										RCSPACE(5)
										RCTOFIT(IDC_RADIO_CHAIN)
									ENDGROUP()
								ENDGROUP()
								RCREST(-1)
								BEGINROWS(WRCT_TOFIT, 0, 0)							// 編集ボタン列
									BEGINROWS(WRCT_TOFIT, 0, 0)
										RCTOFIT(IDC_BUTTON_ADD_ACCESS_STORE)
										RCSPACE(5)
										RCTOFIT(IDC_BUTTON_UPDATE_ACCESS_STORE)
										RCSPACE(5)
										RCTOFIT(IDC_BUTTON_DELETE_ACCESS_STORE)
									ENDGROUP()
								ENDGROUP()
							ENDGROUP()
						ENDGROUP()
						RCREST(-1)
						BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))									// 編集可能情報ペイン2
							BEGINCOLS(WRCT_TOFIT, 0, 0)									// チェックボックス行
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_MONDAY)
									RCTOFIT(IDC_CHECK_MONDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_TUESDAY)
									RCTOFIT(IDC_CHECK_TUESDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_WEDNESDAY)
									RCTOFIT(IDC_CHECK_WEDNESDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_THURSDAY)
									RCTOFIT(IDC_CHECK_THURSDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_FRIDAY)
									RCTOFIT(IDC_CHECK_FRIDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_SATURDAY)
									RCTOFIT(IDC_CHECK_SATURDAY)
								ENDGROUP()
								RCSPACE(2)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_SUNDAY)
									RCTOFIT(IDC_CHECK_SUNDAY)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_HOLIDAY)
									RCTOFIT(IDC_CHECK_HOLIDAY)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_FIXED, 20, 0)
									RCTOFIT(IDC_STATIC_GOLDENWEEK)
									RCTOFIT(IDC_CHECK_GOLDENWEEK)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_BON)
									RCTOFIT(IDC_CHECK_BON)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_YEAREND)
									RCTOFIT(IDC_CHECK_YEAREND)
								ENDGROUP()
								RCSPACE(7)
								BEGINROWS(WRCT_TOFIT, 0, 0)
									RCTOFIT(IDC_STATIC_NEWYEAR)
									RCTOFIT(IDC_CHECK_NEWYEAR)
								ENDGROUP()
								RCSPACE(10)
								RCTOFIT(IDC_BUTTON_CHECK_ALL)
								RCSPACE(5)
								RCTOFIT(IDC_BUTTON_UNCHECK_ALL)
								RCREST(-1)
								RCTOFIT(IDC_BUTTON_SAVE_ACCESS_STORE)
							ENDGROUP()
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
 			ENDGROUP()
		ENDGROUP()
		RCSPACE(10)
		BEGINROWS(WRCT_REST, 0, 0)											// 情報ソースペイン
			BEGINCOLS(WRCT_TOFIT, 20, 20)
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrAccessPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrAccessPointDlg();
}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrAccessPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrAccessPointDlg::CAttrAccessPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化、さらにタブ位置、フォントタイプの初期化をしておきます。
 */
CAttrAccessPointDlg::CAttrAccessPointDlg()
	: ATTR_BASE_CLASS(ACCESS_POINT_MAP, IDD_ATTRACCESSPOINTDLG),
		m_wnd0( _T("CHECKBOX"), this, IDC_CHECK_HWYMODEFLAG ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_ROADCLASS_REF ),
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_ROADNAME_REF ), 
		m_wnd3( _T("EDIT"), this, IDC_EDIT_ROSENCODE ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_SEQUENCE ), 
		m_wnd5( _T("EDIT"), this, IDC_EDIT_SEQUENCE ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_COMBO_DIRECTION ), 
		m_wnd7( _T("COMBOBOX"), this, IDC_COMBO_JUNCTIONCODE ), 
		m_wnd8( _T("EDIT"), this, IDC_EDIT_HOUMENKANJI ), 
		m_wnd9( _T("EDIT"), this, IDC_EDIT_HOUMENKANA ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wndInfo1( _T("CHECKBOX"), this, IDC_CHECK_RESTAURANT ),
		m_wndInfo2( _T("CHECKBOX"), this, IDC_CHECK_RESTING ),
		m_wndInfo3( _T("CHECKBOX"), this, IDC_CHECK_INFOMATION ),
		m_wndInfo4( _T("CHECKBOX"), this, IDC_CHECK_SPA ),
		m_wndInfo5( _T("CHECKBOX"), this, IDC_CHECK_CASHSERVICE ),
		m_wndInfo6( _T("CHECKBOX"), this, IDC_CHECK_TOILET ),
		m_wndInfo7( _T("CHECKBOX"), this, IDC_CHECK_CAFETERIA ),
		m_wndInfo8( _T("CHECKBOX"), this, IDC_CHECK_NAP ),
		m_wndInfo9( _T("CHECKBOX"), this, IDC_CHECK_SHOWER ),
		m_wndInfo10( _T("CHECKBOX"), this, IDC_CHECK_FAX ),
		m_wndInfo11( _T("CHECKBOX"), this, IDC_CHECK_HWYOASIS ),
		m_wndInfo12( _T("CHECKBOX"), this, IDC_CHECK_HANDYCAP ),
		m_wndInfo13( _T("CHECKBOX"), this, IDC_CHECK_SOUBENIR ),
		m_wndInfo14( _T("CHECKBOX"), this, IDC_CHECK_INFODESK ),
		m_wndInfo15( _T("CHECKBOX"), this, IDC_CHECK_LAUNDRY ),
		m_wndInfo16( _T("CHECKBOX"), this, IDC_CHECK_MAILBOX ),
		m_wndInfo19( _T("CHECKBOX"), this, IDC_CHECK_GS ),
		m_wndInfo20( _T("CHECKBOX"), this, IDC_CHECK_MULTIPLESTORE ),
		m_wndInfo21( _T("CHECKBOX"), this, IDC_CHECK_DOGRUN ),
		m_wndAS1( _T("EDIT"), this, IDC_EDIT_MASTERCODE ),					// [Bug5556]で追加
		m_wndAS2( _T("EDIT"), this, IDC_EDIT_START_HOUR ),					// [Bug5556]で追加
		m_wndAS3( _T("EDIT"), this, IDC_EDIT_START_MIN ),					// [Bug5556]で追加
		m_wndAS4( _T("EDIT"), this, IDC_EDIT_END_HOUR ),					// [Bug5556]で追加
		m_wndAS5( _T("EDIT"), this, IDC_EDIT_END_MIN ),						// [Bug5556]で追加
		m_wndAS6( _T("CHECKBOX"), this, IDC_CHECK_MONDAY ),					// [Bug5556]で追加
		m_wndAS7( _T("CHECKBOX"), this, IDC_CHECK_TUESDAY ),				// [Bug5556]で追加
		m_wndAS8( _T("CHECKBOX"), this, IDC_CHECK_WEDNESDAY ),				// [Bug5556]で追加
		m_wndAS9( _T("CHECKBOX"), this, IDC_CHECK_THURSDAY ),				// [Bug5556]で追加
		m_wndAS10( _T("CHECKBOX"), this, IDC_CHECK_FRIDAY ),				// [Bug5556]で追加
		m_wndAS11( _T("CHECKBOX"), this, IDC_CHECK_SATURDAY ),				// [Bug5556]で追加
		m_wndAS12( _T("CHECKBOX"), this, IDC_CHECK_SUNDAY ),				// [Bug5556]で追加
		m_wndAS13( _T("CHECKBOX"), this, IDC_CHECK_HOLIDAY ),				// [Bug5556]で追加
		m_wndAS14( _T("CHECKBOX"), this, IDC_CHECK_GOLDENWEEK ),			// [Bug5556]で追加
		m_wndAS15( _T("CHECKBOX"), this, IDC_CHECK_BON ),					// [Bug5556]で追加
		m_wndAS16( _T("CHECKBOX"), this, IDC_CHECK_YEAREND ),				// [Bug5556]で追加
		m_wndAS17( _T("CHECKBOX"), this, IDC_CHECK_NEWYEAR ),				// [Bug5556]で追加
		m_wndAS18( _T("BUTTON"), this, IDC_BUTTON_CHECK_ALL ),				// [Bug5556]で追加
		m_wndAS19( _T("BUTTON"), this, IDC_BUTTON_UNCHECK_ALL ),			// [Bug5556]で追加
		m_wndAS20( _T("BUTTON"), this, IDC_BUTTON_ADD_ACCESS_STORE ),		// [Bug5556]で追加
		m_wndAS21( _T("BUTTON"), this, IDC_BUTTON_UPDATE_ACCESS_STORE ),	// [Bug5556]で追加
		m_wndAS22( _T("BUTTON"), this, IDC_BUTTON_DELETE_ACCESS_STORE ),	// [Bug5556]で追加
		m_wndAS23( _T("BUTTON"), this, IDC_BUTTON_SAVE_ACCESS_STORE ),		// [Bug5556]で追加
		m_wndTab( _T("TAB"), this, IDC_TAB_ACCESSPOINT )
{
	// タブ位置の初期化
	m_iChangeType = 0;
	m_lAccessTabNum = 0;
	// フォント作成
	m_hFont = CreateFont(
		18, 0, 0, 0, 100,		// 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,    // 斜体，下線，打消線
		SHIFTJIS_CHARSET,       // キャラクタ設定
		OUT_DEFAULT_PRECIS,     // 出力精度
		CLIP_DEFAULT_PRECIS,    // クリップ精度
		DEFAULT_QUALITY,        // 品質
		FIXED_PITCH | FF_DONTCARE,// ピッチ
		AheLoadString(IDS_FONT));   // フォント種類
}

/** 
 * @fn HWND CAttrAccessPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrAccessPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrAccessPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrAccessPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn void CAttrAccessPointDlg::SetArcHelper( IApplication* ipApp )
 * @brief ATTR_BASE_CLASSにArcHelperを渡します。
 */
void CAttrAccessPointDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
}

/**
 * @fn BOOL CAttrAccessPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * 路線コード、路線内シーケンスの設定をしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrAccessPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lAccessTabNum, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

    // 路線コード、路線内シーケンスの初期値確保
    CComVariant		vaRoadSequence;
    CFieldDef*		pFieldDef;
    CComVariant		vaValue;
	// 現在選択中のフィーチャから路線内シーケンスを取得
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
    vaValue= pFieldDef->m_vaValue;
    m_lOrgRoadSequence = vaValue.lVal;
	// 現在選択中のフィーチャから路線コード取得
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_CODE") );
    vaValue = pFieldDef->m_vaValue;
    m_lOrgRoadCode = vaValue.lVal;

    // 最初から路線コードを出すため
    SetLineClassList(m_lOrgRoadCode);
	// 参照用の路線内シーケンスコンボボックスのセット
    ChangeRoadSeqCombobox();

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		if( m_lAccessTabNum == 0 )
			::SetFocus( GetDlgItem(IDC_CHECK_HWYMODEFLAG) );
		else 
			::SetFocus( GetDlgItem(IDC_CHECK_RESTAURANT) );
	}

	// ハイウェイモードフラグのチェックボックスは常にDisable（表示専用）にする（[Bug 6217]対応時に追加）
	SetDlgStatus(IDC_CHECK_HWYMODEFLAG, FALSE);

	// Access_Storeタブがアクティブなら（[Bug5556]で追加）
	if (m_lAccessTabNum == 2)
	{
		// Access_Storeテーブルを読み込む
		LoadAccessStore();
		// Access_Store情報グリッド更新
		UpdateASGrid();
	}

    return bRet;
}

/**
 * @brief フィールド名に対応するグリッド上のフィールドインデックスを返す
 * @note	無効なフィールド名が指定されたらstd::runtime_error例外を送出する。
 * @param[in]	cFieldName	対象フィールド名
 * @return	フィールドインデックス
 */
long ASGridFieldIndex( tstring cFieldName )
{
	using namespace sindy::schema;
	typedef tstring FieldName;
	typedef long FieldIndex;
	typedef std::map<FieldName, FieldIndex> FieldIndexMap;
	static FieldIndexMap aFieldIndexMap = boost::assign::map_list_of
		( tstring( _T("OBJECTID") ),					0 )
		( tstring( access_store::kMultipleStore ),		1 )
		( tstring( access_store::kStartHour ),			2 )
		( tstring( access_store::kStartMin ),			3 )
		( tstring( access_store::kEndHour ),			4 )
		( tstring( access_store::kEndMin ),				5 )
		( tstring( access_store::kMonday ),				6 )
		( tstring( access_store::kTuesday ),			7 )
		( tstring( access_store::kWednesday ),			8 )
		( tstring( access_store::kThursday ),			9 )
		( tstring( access_store::kFriday ),				10 )
		( tstring( access_store::kSaturday ),			11 )
		( tstring( access_store::kSunday ),				12 )
		( tstring( access_store::kHoliday ),			13 )
		( tstring( access_store::kGoldenWeek ),			14 )
		( tstring( access_store::kBon ),				15 )
		( tstring( access_store::kYearEnd ),			16 )
		( tstring( access_store::kNewYear ),			17 )
		( tstring( access_store::kStoreType ),			18 )
	;

	FieldIndexMap::const_iterator it = aFieldIndexMap.find( cFieldName );
	if (it == aFieldIndexMap.end())
	{
		ATLASSERT(0);
		throw std::runtime_error( "ASGridFieldIndex()失敗" );
	}

	return it->second;
}

void CAttrAccessPointDlg::UpdateASGrid()
{
	// 固定行以外の行を削除
	m_ipAccessStoreGrid->PutRows( 1 );

	std::locale loc( std::locale( "japanese" ), "C", std::locale::numeric );

	// カレントなアクセスポイントのIDを取得
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	ATLASSERT( pFeatureDef );
	if (! pFeatureDef) { return; }
	long aCurrentAccessPointID = pFeatureDef->GetFeature()->OID;

	bool aSucceeded = true;
	using namespace access_store_cache;
	const CSortedASRows* pSortedRows = m_ASCache.sortRows();
	if (! pSortedRows) { aSucceeded = false; }

	if (aSucceeded)
	{
		try
		{
			long aRowIndex = 0;
			BOOST_FOREACH (const CSortedASRows::value_type& rElement, *pSortedRows)
			{
				const CASRow& rRow = rElement.second;

				// カレントなアクセスポイントに関連付けられていない行ならスキップ
				using namespace sindy::schema;
				if (aCurrentAccessPointID != rRow.validField( access_store::kAccessPointID ).m_NewValue)
					{ continue;	}

				++aRowIndex;

				std::stringstream ss;
				ss.imbue( loc );
				std::string fd = "\t";

				ss << rRow.validField( _T("OBJECTID") ).m_NewValue << fd;
				ss << rRow.validField( access_store::kMultipleStore ).m_NewValue << fd;
				ss << rRow.validField( access_store::kStartHour ).m_NewValue << fd;
				ss << rRow.validField( access_store::kStartMin ).m_NewValue << fd;
				ss << rRow.validField( access_store::kEndHour ).m_NewValue << fd;
				ss << rRow.validField( access_store::kEndMin ).m_NewValue << fd;
				ss << rRow.validField( access_store::kMonday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kTuesday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kWednesday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kThursday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kFriday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kSaturday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kSunday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kHoliday ).m_NewValue << fd;
				ss << rRow.validField( access_store::kGoldenWeek ).m_NewValue << fd;
				ss << rRow.validField( access_store::kBon ).m_NewValue << fd;
				ss << rRow.validField( access_store::kYearEnd ).m_NewValue << fd;
				ss << rRow.validField( access_store::kNewYear ).m_NewValue << fd;
				ss << rRow.validField( access_store::kStoreType ).m_NewValue;

				m_ipAccessStoreGrid->AddItem( _bstr_t( ss.str().c_str() ), aRowIndex );
			}
		}
		// CASRow::validField()失敗時に送出される例外をcatchしたら処理を中断する。
		catch (...)
		{
			ATLASSERT(0);
			aSucceeded = false;
		}
	}

	// 失敗していたら処理を中断する
	if (! aSucceeded)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	// グリッドに固定行以外の行が存在する場合
	long aRows = m_ipAccessStoreGrid->GetRows();
	if (m_ipAccessStoreGrid->GetRows() > 1)
	{
		// １行目を選択
		m_ipAccessStoreGrid->PutRow( 1 );

		// セルの色を設定
		SetASGridColor();
	}

	// グリッド以外のコントロールを更新
	UpdateASControls();
}

void CAttrAccessPointDlg::UpdateASControls()
{
	// 固定行以外の行が存在しない場合
	if (m_ipAccessStoreGrid->GetRows() <= 1)
	{
		BOOST_FOREACH (long aControlID, GetASEditableControlIDs())
		{
			// 「追加」ボタンのEnabled/Disabledを編集可否に応じて設定する
			if (aControlID == IDC_BUTTON_ADD_ACCESS_STORE)
			{
				SetDlgStatus( aControlID, IsEditableFeature() );
			}
			// 「追加」ボタン以外の編集可能コントロールはDisabledにする
			else
			{
				SetDlgStatus( aControlID, FALSE );
			}
		}
		return;		// 以下の処理は行わない
	}
	// 固定行以外の行が存在する場合
	// 編集可能状態なら全ての編集可能コントロールをEnabledに、不能状態ならDisabledにする
	BOOST_FOREACH (long aControlID, GetASEditableControlIDs())
	{
		SetDlgStatus( aControlID, IsEditableFeature() );
	}

	long aRow = m_ipAccessStoreGrid->GetRow();

	try
	{
		using namespace sindy::schema;
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kMultipleStore ),	IDC_EDIT_MASTERCODE );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kStartHour ),		IDC_EDIT_START_HOUR );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kStartMin ),			IDC_EDIT_START_MIN );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kEndHour ),			IDC_EDIT_END_HOUR );
		UpdateASEditControl( aRow, ASGridFieldIndex( access_store::kEndMin ),			IDC_EDIT_END_MIN );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kMonday ),				IDC_CHECK_MONDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kTuesday ),				IDC_CHECK_TUESDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kWednesday ),			IDC_CHECK_WEDNESDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kThursday ),			IDC_CHECK_THURSDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kFriday ),				IDC_CHECK_FRIDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kSaturday ),			IDC_CHECK_SATURDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kSunday ),				IDC_CHECK_SUNDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kHoliday ),				IDC_CHECK_HOLIDAY );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kGoldenWeek ),			IDC_CHECK_GOLDENWEEK );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kBon ),					IDC_CHECK_BON );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kYearEnd ),				IDC_CHECK_YEAREND );
		UpdateASCheckBox( aRow, ASGridFieldIndex( access_store::kNewYear ),				IDC_CHECK_NEWYEAR );

		long aStoreType = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( aRow, ASGridFieldIndex( access_store::kStoreType ) ) );
		switch (aStoreType)
		{
			using namespace sindy::schema::access_store::store_type;
			case kGS:
				CButton( GetDlgItem( IDC_RADIO_GS ) ).SetCheck( BST_CHECKED );
				CButton( GetDlgItem( IDC_RADIO_CHAIN ) ).SetCheck( BST_UNCHECKED );
				break;
			case kChainStore:
				CButton( GetDlgItem( IDC_RADIO_CHAIN ) ).SetCheck( BST_CHECKED );
				CButton( GetDlgItem( IDC_RADIO_GS ) ).SetCheck( BST_UNCHECKED );
				break;
		}
	}
	// ASGridFieldIndex失敗時に送出される例外をcatchしたら処理を中断する。
	catch (...)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}
}

void CAttrAccessPointDlg::SetASGridColor()
{
	// 固定行以外の行が存在しなければスキップ
	if (m_ipAccessStoreGrid->GetRows() <= 1)
		{ return; }

	// グリッド用にソートされた行群を取得
	long aCurrentRow = m_ipAccessStoreGrid->GetRow();
	long aRowIndex = 0;
	using namespace access_store_cache;
	const CSortedASRows& rSortedRows = m_ASCache.sortedRows();

	// カレントなアクセスポイントのIDを取得
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	ATLASSERT( pFeatureDef );
	if (! pFeatureDef) { return; }
	long aCurrentAccessPointID = pFeatureDef->GetFeature()->OID;

	try
	{
		BOOST_FOREACH (const CSortedASRows::value_type& rElement, rSortedRows)
		{
			const CASRow& rRow = rElement.second;
			
			// カレントなアクセスポイントに関連付けられていない行ならスキップ
			using namespace sindy::schema;
			if (aCurrentAccessPointID != rRow.validField( access_store::kAccessPointID ).m_NewValue)
				{ continue;	}

			++aRowIndex;
			bool aIsCurrentRow = aCurrentRow == aRowIndex;

			// セルの色を変える
			// 優先順位は「削除行ならグレー」＞「追加行ならオレンジ」＞「更新セルならオレンジ」
			// 削除or追加or更新されているカレントセルなら色を濃くし、更新されていないカレントセルは緑色にする
			if (rRow.m_IsDeleted)
			{
				m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, 0, aRowIndex, m_ipAccessStoreGrid->GetCols() - 1, 
					aIsCurrentRow ? RGB(105,105,105) : RGB(211,211,211) );
			}
			else if (rRow.m_IsAdded)
			{
				m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, 0, aRowIndex, m_ipAccessStoreGrid->GetCols() - 1, 
					aIsCurrentRow ? RGB(255,140,0) : RGB(255,218,185) );
			}
			else
			{
				// 変更されているフィールドだけ色を変える
				BOOST_FOREACH (const CASFields::value_type& rElement, rRow.m_Fields)
				{
					tstring aFieldName = rElement.first;
					const CASField& rField = rElement.second;

					// アクセスポイントIDならスキップ（対応するフィールドがグリッド上にないため）
					if (aFieldName == sindy::schema::access_store::kAccessPointID)
						{ continue; }

					long aFieldIndex = ASGridFieldIndex( aFieldName );

					if (rField.m_OrgValue != rField.m_NewValue)
					{
						m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, aFieldIndex, aRowIndex, aFieldIndex, 
							aIsCurrentRow ? RGB(255,140,0) : RGB(255,218,185) );
					}
					else
					{
						m_ipAccessStoreGrid->PutCell( flexcpBackColor, aRowIndex, aFieldIndex, aRowIndex, aFieldIndex,
							aIsCurrentRow ? RGB(0,255,127) : RGB(255,255,255));
					}
				}
			}
		}
	}
	// CASRow::validField()またはASGridFieldIndex失敗時に送出される例外をcatchしたら処理を中断する。
	catch (...)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}


}

void CAttrAccessPointDlg::UpdateASEditControl( long cRow, long cCol, long cControlID )
{
	ATLASSERT( m_ipAccessStoreGrid );
	if (! m_ipAccessStoreGrid) { return; }

	long aGridValue = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( cRow, cCol ) );
	CString aStr;
	aStr.Format( _T("%d"), aGridValue );
	CEdit( GetDlgItem( cControlID ) ).SetWindowText( aStr );
}

void CAttrAccessPointDlg::UpdateASCheckBox( long cRow, long cCol, long cControlID )
{
	ATLASSERT( m_ipAccessStoreGrid );
	if (! m_ipAccessStoreGrid) { return; }

	long aGridValue = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( cRow, cCol ) );
	CButton( GetDlgItem( cControlID ) ).SetCheck( aGridValue ==0 ? BST_UNCHECKED : BST_CHECKED );
}

void CAttrAccessPointDlg::LoadAccessStore()
{
	m_ASCache.clear();

	typedef std::set<long> OIDs;
	OIDs aSelectedAccessPointIDs;

	ATLASSERT( m_pFeatureDefList->size() );
	if (! m_pFeatureDefList->size())
		{ return; }
	IFeaturePtr ipAccessPoint( m_pFeatureDefList->begin()->GetFeature() );

	// 選択されているアクセスポイント群の１つ目と同じDBのAccess_Storeテーブルを取得
	ITablePtr ipAccessStoreTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipAccessPoint ), sindy::schema::access_store::kTableName );
	if (! ipAccessStoreTable)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	// 選択されているアクセスポイント群に関連付いているAccess_Storeオブジェクト群を取得
	BOOST_FOREACH (CFeatureDef& rFeatureDef, *m_pFeatureDefList)
	{
		aSelectedAccessPointIDs.insert( rFeatureDef.GetFeature()->OID );
	}

	std::stringstream ss;
	ss.imbue( std::locale( std::locale( "japanese" ), "C", std::locale::numeric ) );
	ss << CT2A( sindy::schema::access_store::kAccessPointID ) << " IN(";
	for (OIDs::iterator it = aSelectedAccessPointIDs.begin(); it != aSelectedAccessPointIDs.end(); ++it)
	{
		if (it != aSelectedAccessPointIDs.begin())
		{
			ss << ",";
		}
		ss << *it;
	}
	ss << ")";

	_ICursorPtr ipCursor( ipAccessStoreTable->_Search( AheInitQueryFilter( NULL, NULL, CA2T( ss.str().c_str() ) ), VARIANT_FALSE ) );

	// 取得したオブジェクト群をキャッシュに格納
	_IRowPtr ipRow;
	while (ipCursor->NextRow( &ipRow ) == S_OK && ipRow)
	{
		m_ASCache.addRow( ipRow );
	}
}

void CAttrAccessPointDlg::EditAccessStore( long cControlID )
{
	try
	{
		// 「追加」ボタンが押された時の処理
		if (cControlID == IDC_BUTTON_ADD_ACCESS_STORE)
		{
			// 選択されているアクセスポイントのIDを取得
			CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
			ATLASSERT( pFeatureDef );
			if (! pFeatureDef) { return; }
			long aCurrentAccessPointID = pFeatureDef->GetFeature()->OID;

			// キャッシュに行を追加
			m_ASCache.addRow( aCurrentAccessPointID );

			// グリッドを更新
			UpdateASGrid();
		}

		// 「更新」ボタンが押された時の処理
		if (cControlID == IDC_BUTTON_UPDATE_ACCESS_STORE)
		{
			long aCurrentRowIndex = m_ipAccessStoreGrid->GetRow();
			long aAccessStoreID = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( aCurrentRowIndex, ASGridFieldIndex( _T("OBJECTID") ) ) );

			using namespace sindy::schema;
			// エディットコントロール群から取得した値をキャッシュに反映
			if (! UpdateASCacheByEditControls( IDC_EDIT_MASTERCODE,	aAccessStoreID ))
				{ return; }		// 失敗したら中断

			// チェックボックス群から取得した値をキャッシュに反映
			UpdateASCacheByCheckBox( IDC_CHECK_MONDAY,		aAccessStoreID,	access_store::kMonday );
			UpdateASCacheByCheckBox( IDC_CHECK_TUESDAY,		aAccessStoreID,	access_store::kTuesday );
			UpdateASCacheByCheckBox( IDC_CHECK_WEDNESDAY,	aAccessStoreID,	access_store::kWednesday );
			UpdateASCacheByCheckBox( IDC_CHECK_THURSDAY,	aAccessStoreID,	access_store::kThursday );
			UpdateASCacheByCheckBox( IDC_CHECK_FRIDAY,		aAccessStoreID,	access_store::kFriday );
			UpdateASCacheByCheckBox( IDC_CHECK_SATURDAY,	aAccessStoreID,	access_store::kSaturday );
			UpdateASCacheByCheckBox( IDC_CHECK_SUNDAY,		aAccessStoreID,	access_store::kSunday );
			UpdateASCacheByCheckBox( IDC_CHECK_HOLIDAY,		aAccessStoreID,	access_store::kHoliday );
			UpdateASCacheByCheckBox( IDC_CHECK_GOLDENWEEK,	aAccessStoreID,	access_store::kGoldenWeek );
			UpdateASCacheByCheckBox( IDC_CHECK_BON,			aAccessStoreID,	access_store::kBon );
			UpdateASCacheByCheckBox( IDC_CHECK_YEAREND,		aAccessStoreID,	access_store::kYearEnd );
			UpdateASCacheByCheckBox( IDC_CHECK_NEWYEAR,		aAccessStoreID,	access_store::kNewYear );

			// ラジオボタン群から取得した値をキャッシュに反映
			if (CButton( GetDlgItem( IDC_RADIO_GS ) ).GetCheck() == BST_CHECKED)
			{
				m_ASCache.updateField( aAccessStoreID, access_store::kStoreType, access_store::store_type::kGS );
			}
			else if (CButton( GetDlgItem( IDC_RADIO_CHAIN ) ).GetCheck() == BST_CHECKED)
			{
				m_ASCache.updateField( aAccessStoreID, access_store::kStoreType, access_store::store_type::kChainStore );
			}

			// グリッドを更新
			UpdateASGrid();

			// グリッドの行の中から更新された行を探して被選択状態にする
			for (long i = 0; i < m_ipAccessStoreGrid->GetRows(); ++i)
			{
				long aAccessStoreID2 = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( i, ASGridFieldIndex( _T("OBJECTID") ) ) );
				if (aAccessStoreID == aAccessStoreID2)
				{
					m_ipAccessStoreGrid->PutRow( i );
					break;
				}
			}

			// カレントセルの位置までスクロール
			m_ipAccessStoreGrid->ShowCell( m_ipAccessStoreGrid->GetRow(), m_ipAccessStoreGrid->GetCol() );
		}

		// 「削除」ボタンが押された時の処理
		if (cControlID == IDC_BUTTON_DELETE_ACCESS_STORE)
		{
			long aCurrentRowIndex = m_ipAccessStoreGrid->GetRow();
			long aAccessStoreID = static_cast<long>( m_ipAccessStoreGrid->GetValueMatrix( aCurrentRowIndex, ASGridFieldIndex( _T("OBJECTID") ) ) );

			m_ASCache.changeDeleteFlag( aAccessStoreID );

			// グリッドを更新
			UpdateASGrid();

			// 元々選択されていた行を選択
			m_ipAccessStoreGrid->PutRow( aCurrentRowIndex );
		}
	}
	// ASGridFieldIndex失敗時に送出される例外をcatchしたら処理を中断する。
	catch (...)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}
}

void CAttrAccessPointDlg::UpdateASCacheByEditControl( long cControlID, long cObjectID, tstring cFieldName )
{
	long aNum = GetNumFromEditControl( cControlID );
	m_ASCache.updateField( cObjectID, cFieldName, aNum );
}

long CAttrAccessPointDlg::GetNumFromEditControl( long cControlID )
{
	CString lpszStr;
	CEdit( GetDlgItem( cControlID ) ).GetWindowText( lpszStr );
	return boost::lexical_cast<long>( lpszStr.GetString() );
}

bool CAttrAccessPointDlg::UpdateASCacheByEditControls( long cControlID, long cObjectID )
{
	CString aControlName;
	try
	{
		// エディットコントロール群から数値を正常に取得できるかどうか試す
		aControlName = AheLoadString( IDS_CHAINMASTERCODE );
		GetNumFromEditControl( IDC_EDIT_MASTERCODE );
		aControlName = AheLoadString( IDS_OPENINGTIME );
		GetNumFromEditControl( IDC_EDIT_START_HOUR );
		GetNumFromEditControl( IDC_EDIT_START_MIN );
		aControlName = AheLoadString( IDS_CLOSINGTIME );
		GetNumFromEditControl( IDC_EDIT_END_HOUR );
		GetNumFromEditControl( IDC_EDIT_END_MIN );
	}
	// エディットコントロールへのペーストにより半角数字以外の文字が入力されているとUpdateASCacheByEditControl()内のboost::lexical_castが失敗して
	// 例外が送出されるのでここでcatchし、その場合は処理を中断する。
	catch (...)
	{
		CString aStr;
		aStr.Format( AheLoadString( IDS_INVALID ), aControlName );
		AttrMessageBox( aStr, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return false;
	}

	// 問題なければキャッシュを更新
	using namespace sindy::schema;
	UpdateASCacheByEditControl( IDC_EDIT_MASTERCODE,	cObjectID, access_store::kMultipleStore );
	UpdateASCacheByEditControl( IDC_EDIT_START_HOUR,	cObjectID, access_store::kStartHour );
	UpdateASCacheByEditControl( IDC_EDIT_START_MIN,		cObjectID, access_store::kStartMin );
	UpdateASCacheByEditControl( IDC_EDIT_END_HOUR,		cObjectID, access_store::kEndHour );
	UpdateASCacheByEditControl( IDC_EDIT_END_MIN,		cObjectID, access_store::kEndMin );

	return true;
}

void CAttrAccessPointDlg::UpdateASCacheByCheckBox( long cControlID, long cObjectID, tstring cFieldName )
{
	m_ASCache.updateField( cObjectID, cFieldName, CButton( GetDlgItem( cControlID ) ).GetCheck() == BST_CHECKED ? 1 : 0 );
}

namespace access_store_row_edit_type
{
	/**
	 * @brief 行に対する更新内容の種類
	 */
	enum ECode
	{
		none,				///< 無効
		added,				///< 追加
		deleted,			///< 削除
		may_be_updated,		///< 更新または変更なし
	};

	/**
	 * @brief 追加フラグと削除フラグに対応する「更新内容の種類」（access_store_row_edit_type::ECode）を返す
	 * @param[in]	cIsAdded	追加フラグ
	 * @param[in]	cIsDeleted	削除フラグ
	 * @return	更新内容の種類
	 */
	ECode code( bool cIsAdded, bool cIsDeleted )
	{
		ECode aEditType = none;
		if (cIsAdded)
		{
			if (cIsDeleted)
			{
				aEditType = none;
			}
			else
			{
				aEditType = added;
			}
		}
		else
		{
			if (cIsDeleted)
			{
				aEditType = deleted;
			}
			else
			{
				aEditType = may_be_updated;
			}
		}
		return aEditType;
	}
}

void CAttrAccessPointDlg::SaveAccessStore()
{
	using namespace access_store_cache;

	// 選択されているアクセスポイント群の１つ目と同じDBのAccess_Storeテーブルを取得
	IFeaturePtr ipAccessPoint( m_pFeatureDefList->begin()->GetFeature() );
	ITablePtr ipAccessStoreTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipAccessPoint ), sindy::schema::access_store::kTableName );
	if (! ipAccessStoreTable)
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOUPDATEACCESSSTORE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	long aSucceededRowCount = 0;	// DBへの反映が成功した行の数
	long aFailedRowCount = 0;		// DBへの反映が失敗した行の数

	m_cArcHelper.StartEditOperation();

	// キャッシュの全ての行について
	BOOST_FOREACH (const CASRows::value_type& rElement, m_ASCache.rows())
	{
		long aAccessStoreID = rElement.first;
		const CASRow& rRow = rElement.second;
		_IRowPtr ipRow = rRow.m_pRow;

		using namespace access_store_row_edit_type;
		ECode aEditType = code( rRow.m_IsAdded, rRow.m_IsDeleted );

		// 「無効」ならスキップ
		if (aEditType == none)
			{ continue; }
		// 「削除」された行なら削除を反映
		else if (aEditType == deleted)
		{
			ATLASSERT( ipRow );
			if (ipRow)
			{
				ipRow->Delete();
				++aSucceededRowCount;
			}
		}
		// 「追加」または「更新または変更なし」の場合
		else
		{
			ATLASSERT( aEditType == added || aEditType == may_be_updated );

			// 値の妥当性チェック
			if (! CheckASValuesValidity( rRow ))
			{
				++aFailedRowCount;
				break;
			}

			_IRowPtr ipNewRow;
			// 追加ならRowを新規作成
			if (aEditType == added)
			{
				ipAccessStoreTable->CreateRow( &ipNewRow );
			}

			long aUpdatedFieldCount = 0;
			// 全てのフィールドについて
			BOOST_FOREACH (const CASFields::value_type& rElement, rRow.m_Fields)
			{
				tstring aFieldName = rElement.first;
				const CASField& rField = rElement.second;

				// ObjectIDならスキップ
				if (aFieldName == _T("OBJECTID"))
					{ continue; }

				// 「追加」された行なら、全フィールドの値を反映
				if (aEditType == added)
				{
					ipNewRow->PutValue( ipNewRow->Fields->_FindField( aFieldName.c_str() ), rField.m_NewValue );
				}
				// 「更新または変更なし」の行なら、更新されたフィールドのみ値を反映
				else
				{
					if (rField.m_OrgValue != rField.m_NewValue)
					{
						ipRow->PutValue( ipRow->Fields->_FindField( aFieldName.c_str() ), rField.m_NewValue );
						++aUpdatedFieldCount;
					}
				}
			}

			if (aEditType == added)
			{
				
				AheSetModifyData( ipNewRow, sindyUpdateTypeCreate );		// 新規作成履歴を格納
				ipNewRow->Store();
				++aSucceededRowCount;
			}
			else
			{
				// 更新されたフィールドが１つ以上あれば
				if (aUpdatedFieldCount > 0)
				{
					AheSetModifyData( ipRow, sindyUpdateTypeProperty );		// 更新履歴を格納
					ipRow->Store();
					++aSucceededRowCount;
				}
			}
		}
	}

	// 何か失敗していたら編集中止
	if (aFailedRowCount > 0)
	{
		m_cArcHelper.AbortEditOperation();
	}
	// 実質的に何も変更されていなければ編集中止
	else if (aSucceededRowCount == 0)
	{
		AttrMessageBox( AheLoadString( IDS_NOTSAVE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		m_cArcHelper.AbortEditOperation();
	}
	// それ以外なら編集確定
	else
	{
		// 追加→保存後にUndoされて無効になったRowを更新しようとするとここで失敗する。
		// ArcHelperによるエラーダイアログが表示されるが、ArcMapが終了してしまうわけではないので、
		// もう１つ状況説明のダイアログを表示し、キャッシュとグリッドの状態を最新にして処理を終える。
		if (! m_cArcHelper.StopEditOperation( AheLoadString( IDS_NAME ) ))
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSAVEFORUNDO ),  AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		}
		// キャッシュ再作成
		LoadAccessStore();
		// グリッド更新
		UpdateASGrid();
	}
}

bool CAttrAccessPointDlg::IsValidASValue( const access_store_cache::CASRow& crRow, tstring cFieldName, const CString& cFieldAlias, long cMin, long cMax )
{
	bool aResult = true;
	long aValue = crRow.validField( cFieldName.c_str() ).m_NewValue;
	if (aValue < cMin || cMax < aValue)
	{
		CString aStr;
		aStr.Format( AheLoadString( IDS_INVALIDASVALUE ), cFieldAlias, aValue );
		AttrMessageBox( aStr, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		aResult = false;
	}
	return aResult;
}

bool CAttrAccessPointDlg::CheckASValuesValidity( const access_store_cache::CASRow& crRow )
{
	bool aResult = true;
	try
	{
		using namespace sindy::schema;
		if (
			! IsValidASValue( crRow, access_store::kMultipleStore, AheLoadString( IDS_CHAINMASTERCODE ), 1, 99999999 ) ||	// チェーン店マスターコードが0ならエラー
			! IsValidASValue( crRow, access_store::kStartHour, AheLoadString( IDS_OPENINGTIME ), 0, 23 ) ||		// 「開始時間」「終了時間」が0以上23以下でなければエラー
			! IsValidASValue( crRow, access_store::kStartMin, AheLoadString( IDS_OPENINGTIMEMIN ), 0, 59 ) ||		// 「開始分」「終了分」が0以上59以下でなければエラー
			! IsValidASValue( crRow, access_store::kEndHour, AheLoadString( IDS_CLOSINGTIME ), 0, 23 ) ||
			! IsValidASValue( crRow, access_store::kEndMin, AheLoadString( IDS_CLOSINGTIMEMIN ), 0, 59 )
		)
		{
			aResult = false;
		}
	}
	catch (...)
	{
		ATLASSERT(0);
		aResult = false;
	}
	return aResult;
}

/**
 * @fn LONG CAttrAccessPointDlg::GetTabNum()
 * @brief 現在のタブ位置を渡します
 *
 * @retval 現在のタブ位置
 */
LONG CAttrAccessPointDlg::GetTabNum()
{
	return m_lAccessTabNum;
}

/**
 * @fn void CAttrAccessPointDlg::SetTabNum(LONG lTabNum)
 * @brief タブ位置をセットします
 *
 * @param lTabNum	[in]	タブ位置
 */
void CAttrAccessPointDlg::SetTabNum(LONG lTabNum)
{
	m_lAccessTabNum = lTabNum;
	m_iChangeType = lTabNum;
}

/**
 * @fn void CAttrAccessPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrAccessPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );
	
	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// タブ
		m_cControlRel.SetControl( IDC_TAB_ACCESSPOINT,		TYPE_OTHER, 	0,	2);
		// 情報ソース
		m_cControlRel.SetControl( IDC_STATIC_SOURCE,	TYPE_OTHER,			0,	2 );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0,	2, _T("SOURCE"),		NULL,	ACCESS_POINT_TABLE_NAME );

		// ハイウェイモードフラグ
		m_cControlRel.SetControl( IDC_CHECK_HWYMODEFLAG,	TYPE_CHECKBOX,	0,	0, _T("HWYMODE_F"),		NULL,	ACCESS_POINT_TABLE_NAME);
		// 路線種別参照用
		m_cControlRel.SetControl( IDC_STATIC_ROADCLASS_REF, TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_ROADCLASS_REF,	TYPE_COMBO,		0,	0 );
		// 路線名称参照用
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME_REF,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_ROADNAME_REF,	TYPE_COMBO,		0,	0 );
		// 路線コード
		m_cControlRel.SetControl( IDC_STATIC_ROSENCODE, 	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_ROSENCODE,		TYPE_EDIT,		0,	0, _T("ROAD_CODE"),		NULL,	ACCESS_POINT_TABLE_NAME ,	NULL,	-1, FOLLOW_READONLY );
		// 路線内シーケンス参照用 
		m_cControlRel.SetControl( IDC_STATIC_SEQUENCE_REF,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_SEQUENCE,		TYPE_COMBO,		0,	0 );
		// 路線内シーケンス
		m_cControlRel.SetControl( IDC_STATIC_SEQUENCE,		TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_SEQUENCE,		TYPE_EDIT,		0,	0, _T("ROAD_SEQ"), 		NULL,	ACCESS_POINT_TABLE_NAME,	NULL,	-1, FOLLOW_READONLY );
		// 方向コード
		m_cControlRel.SetControl( IDC_STATIC_DIRECTION, 	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_DIRECTION,		TYPE_COMBO, 	0,	0, _T("DIRECTION_C"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// 分岐・合流コード
		m_cControlRel.SetControl( IDC_STATIC_JUNCTIONCODE,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_COMBO_JUNCTIONCODE,	TYPE_COMBO, 	0,	0, _T("INOUT_C"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// 方面漢字名称
		m_cControlRel.SetControl( IDC_STATIC_HOUMENKANJI,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANJI, 	TYPE_EDITSTRING,0,	0, _T("DIRECTION_KANJI"),NULL,	ACCESS_POINT_TABLE_NAME );
		// 方面カナ名称
		m_cControlRel.SetControl( IDC_STATIC_HOUMENKANA,	TYPE_OTHER, 	0,	0 );
		m_cControlRel.SetControl( IDC_EDIT_HOUMENKANA,		TYPE_EDITSTRING,0,	0, _T("DIRECTION_YOMI"),NULL,	ACCESS_POINT_TABLE_NAME );
		
		// 施設情報用のグループボックス
		m_cControlRel.SetControl( IDC_STATIC_SHISETUINFO,	TYPE_OTHER, 	1,	1 );
		// レストラン
		m_cControlRel.SetControl( IDC_CHECK_RESTAURANT, 	TYPE_CHECKBOX,	1,	1, _T("RESTAURANT_F"), 	NULL,	ACCESS_POINT_TABLE_NAME );
		// 休憩所
		m_cControlRel.SetControl( IDC_CHECK_RESTING,		TYPE_CHECKBOX,	1,	1, _T("RESTING_F"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// HWY情報ターミナル（旧「インフォメーション」）
		m_cControlRel.SetControl( IDC_CHECK_INFOMATION, 	TYPE_CHECKBOX,	1,	1, _T("INFOMATION_F"), 	NULL,	ACCESS_POINT_TABLE_NAME );
		// 風呂
		m_cControlRel.SetControl( IDC_CHECK_SPA,			TYPE_CHECKBOX,	1,	1, _T("SPA_F"),			NULL,	ACCESS_POINT_TABLE_NAME );
		// キャッシュサービス
		m_cControlRel.SetControl( IDC_CHECK_CASHSERVICE,	TYPE_CHECKBOX,	1,	1, _T("CASHSERVICE_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// トイレ
		m_cControlRel.SetControl( IDC_CHECK_TOILET, 		TYPE_CHECKBOX,	1,	1, _T("TOILET_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// ガソリンスタンド
		m_cControlRel.SetControl( IDC_CHECK_GS, 			TYPE_CHECKBOX,	1,	1, _T("GS_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// 軽食
		m_cControlRel.SetControl( IDC_CHECK_CAFETERIA,		TYPE_CHECKBOX,	1,	1, _T("CAFETERIA_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// 仮眠休憩施設
		m_cControlRel.SetControl( IDC_CHECK_NAP,			TYPE_CHECKBOX,	1,	1, _T("NAP_F"),			NULL,	ACCESS_POINT_TABLE_NAME );
		// シャワールーム
		m_cControlRel.SetControl( IDC_CHECK_SHOWER, 		TYPE_CHECKBOX,	1,	1, _T("SHOWER_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// FAX
		m_cControlRel.SetControl( IDC_CHECK_FAX,			TYPE_CHECKBOX,	1,	1, _T("FAX_F"),			NULL,	ACCESS_POINT_TABLE_NAME );
		// ハイウェイオアシス
		m_cControlRel.SetControl( IDC_CHECK_HWYOASIS,		TYPE_CHECKBOX,	1,	1, _T("HWYOASIS_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// 身障者用トイレ
		m_cControlRel.SetControl( IDC_CHECK_HANDYCAP,		TYPE_CHECKBOX,	1,	1, _T("HANDICAP_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// チェーン店
		m_cControlRel.SetControl( IDC_CHECK_MULTIPLESTORE, 	TYPE_CHECKBOX,	1,	1, _T("MULTIPLESTORE_F"), 		NULL,	ACCESS_POINT_TABLE_NAME );
		// 売店
		m_cControlRel.SetControl( IDC_CHECK_SOUBENIR,		TYPE_CHECKBOX,	1,	1, _T("SOUVENIR_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// インフォメーション（旧「案内所（対人）」）
		m_cControlRel.SetControl( IDC_CHECK_INFODESK,		TYPE_CHECKBOX,	1,	1, _T("INFODESK_F"),	NULL,	ACCESS_POINT_TABLE_NAME );
		// コインランドリー
		m_cControlRel.SetControl( IDC_CHECK_LAUNDRY,		TYPE_CHECKBOX,	1,	1, _T("LAUNDRY_F"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// 郵便ポスト
		m_cControlRel.SetControl( IDC_CHECK_MAILBOX,		TYPE_CHECKBOX,	1,	1, _T("MAILBOX_F"),		NULL,	ACCESS_POINT_TABLE_NAME );
		// ドッグラン
		m_cControlRel.SetControl( IDC_CHECK_DOGRUN,			TYPE_CHECKBOX,	1,	1, _T("DOGRUN_F"),		NULL,	ACCESS_POINT_TABLE_NAME );

		// Access_Storeタブのコントロール群（[Bug5556]で追加）
		// Access_Store情報グリッド
		m_cControlRel.SetControl( IDC_GRID_ACCESS_STORE,	TYPE_OTHER, 	2,	2 );
		// チェーン店マスターコード
		m_cControlRel.SetControl( IDC_STATIC_MASTERCODE,	TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_MASTERCODE,		TYPE_EDIT,		2,	2 );
		// 営業開始時間
		m_cControlRel.SetControl( IDC_STATIC_START_TIME,	TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_START_HOUR,		TYPE_EDIT,		2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_COLON1,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_START_MIN,		TYPE_EDIT,		2,	2 );
		// 営業終了時間
		m_cControlRel.SetControl( IDC_STATIC_END_TIME,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_END_HOUR,		TYPE_EDIT,		2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_COLON2,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_EDIT_END_MIN,			TYPE_EDIT,		2,	2 );
		// 店舗種別
		m_cControlRel.SetControl( IDC_GROUP_STORETYPE,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_RADIO_GS,				TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_RADIO_CHAIN,			TYPE_BUTTON, 	2,	2 );
		// 曜日・期間系チェックボックス群
		m_cControlRel.SetControl( IDC_STATIC_MONDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_MONDAY,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_TUESDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_TUESDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_WEDNESDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_WEDNESDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_THURSDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_THURSDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_FRIDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_FRIDAY,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_SATURDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_SATURDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_SUNDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_SUNDAY,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_HOLIDAY,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_HOLIDAY,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_GOLDENWEEK,	TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_GOLDENWEEK,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_BON,			TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_BON,			TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_YEAREND,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_YEAREND,		TYPE_CHECKBOX, 	2,	2 );
		m_cControlRel.SetControl( IDC_STATIC_NEWYEAR,		TYPE_OTHER, 	2,	2 );
		m_cControlRel.SetControl( IDC_CHECK_NEWYEAR,		TYPE_CHECKBOX, 	2,	2 );
		// 全選択・全解除ボタン
		m_cControlRel.SetControl( IDC_BUTTON_CHECK_ALL,		TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_UNCHECK_ALL,	TYPE_BUTTON, 	2,	2 );
		// 編集ボタン
		m_cControlRel.SetControl( IDC_BUTTON_ADD_ACCESS_STORE,		TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_UPDATE_ACCESS_STORE,	TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_DELETE_ACCESS_STORE,	TYPE_BUTTON, 	2,	2 );
		m_cControlRel.SetControl( IDC_BUTTON_SAVE_ACCESS_STORE,		TYPE_BUTTON, 	2,	2 );
	}
}

/**
* @fn void CAttrAccessPointDlg::ChangeWindowMgrMap( INT iIndex )
* @brief ウィンドウマネージャのマップの切り替えを行う
*
* @param	iIndex	[in]	マップのインデックス番号
**/
void CAttrAccessPointDlg::ChangeWindowMgrMap( INT iIndex )
{
	switch( iIndex ) {
	case 0:
			m_winMgr.ChangeMgrMap(ACCESS_POINT_MAP);
			break;
	case 1:
			m_winMgr.ChangeMgrMap(ACCESS_POINT_MAP2);
			break;
	case 2:												// [Bug5556]で追加
			m_winMgr.ChangeMgrMap(ACCESS_POINT_MAP3);
			break;
	default:
			break;
	}
	// 再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
}

/** 
* @fn void CAttrAccessPointDlg::ChangeRoadSeqCombobox()
* @brief 路線コードから路線内シーケンスの値を取得し、参照用路線内シーケンスのコンボボックスに設定する
**/
void CAttrAccessPointDlg::ChangeRoadSeqCombobox()
{
    // 路線コードのダイアログを取得し、路線コード取得
    CWindow edit( GetDlgItem( IDC_EDIT_ROSENCODE ) );
    LPTSTR lpszRoadCode = (LPTSTR)_alloca((edit.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< 文字数分メモリ確保
    edit.GetWindowText( lpszRoadCode, edit.GetWindowTextLength() + 1 );
    m_cRoadCode = lpszRoadCode;

    IMapPtr				ipMap;
    ITablePtr			ipTable;
    _ICursorPtr			ipCursor;
    IQueryFilterPtr		ipQueryFilter( CLSID_QueryFilter );
    CString		strWhereClause, strSubFields;

	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( !ipDataset )
		return;
	ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return;

	// マップを取得
    ipMap = m_cArcHelper.GetMap();
    
    // Facil_Info_Pointテーブル取得
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("Facil_Info_Point");
	else
		strOwnerName += _T(".Facil_Info_Point");

	ipTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );

    // 取得した路線番号をテーブルから検索
    strWhereClause.Format(_T("ROAD_CODE = %s"), m_cRoadCode );
    strSubFields.Format(_T("NAME_KANJI,ROAD_SEQ"));
    ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
    ipQueryFilter->put_SubFields( CComBSTR(_T("NAME_KANJI,ROAD_SEQ")) );
    ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

    // 路線内シーケンス取得
    CComVariant			vaRoadSequence;
    CFieldDef*			pFieldDef;

	// 選択中のフィーチャの路線内シーケンスを取得
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
    vaRoadSequence = pFieldDef->m_vaValue;
    m_cRoadSeq.Format(_T("%d"), vaRoadSequence.lVal);

    // コンボボックスに入れるための値を作成する
    _IRowPtr ipRow;
    std::map<LONG, CString> mapDomain;
    if( ipCursor != NULL)
    {
		// 検索して出てきた路線内シーケンス結果全てを調べる
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			if( ipRow == NULL ) 
					break;
			CComVariant vaKanjiName, vaRoadSeq, vaKanaName, vaClass;

			LONG lIndex = -1;
			IFieldsPtr ipFields;
			ipRow->get_Fields( &ipFields );
			// 施設名称を取得
			ipFields->FindField( CComBSTR(_T("NAME_KANJI")), &lIndex );
			ipRow->get_Value( lIndex, &vaKanjiName );
			// 路線内シーケンスを取得
			ipFields->FindField( CComBSTR(_T("ROAD_SEQ")), &lIndex );
			ipRow->get_Value( lIndex, &vaRoadSeq );

			CString        strVal;
			strVal.Format(_T("%d"), vaRoadSeq.lVal );        

			// 施設名称と路線内シーケンスをmapにセットする
			USES_CONVERSION;
			CString strText;
			strText.Format(_T("%s(%s)"), OLE2T(vaKanjiName.bstrVal), strVal );

			mapDomain[vaRoadSeq.lVal] = strText;
		}

		// コンボボックスに値を入れる
		CComboBox cCombo( GetDlgItem( IDC_COMBO_SEQUENCE ) );
		cCombo.ResetContent();
		for( std::map<LONG, CString>::iterator it = mapDomain.begin(); it != mapDomain.end(); it++ )
		{
			INT iCurPos = cCombo.AddString( it->second );
			cCombo.SetItemData( iCurPos, it->first );
			// 現在選択中のフィーチャの路線内シーケンスならば、それを出す
			if( vaRoadSequence.lVal == it->first )
				cCombo.SetCurSel( iCurPos );
		}
    }

	// 路線内シーケンスエディタに値を出力する
	SetRoadSequence();
}

/**
* @fn void CAttrAccessPointDlg::SetRoadSequence()
* @brief 路線内シーケンスエディタに値を出力する
**/
void CAttrAccessPointDlg::SetRoadSequence()
{

	// 路線内シーケンスエディタを取得
	CEdit        cRoadSeq(GetDlgItem( IDC_EDIT_SEQUENCE ));
	//　路線内シーケンスを実際に設定
	cRoadSeq.SetSel(0,-1);
	cRoadSeq.ReplaceSel(m_cRoadSeq);
	// 値が変更されいるか確認
	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_SEQ") );
	pFieldDef->m_vaValue = _ttol(m_cRoadSeq);
	LONG        lDummy = _ttol(m_cRoadSeq);
	if(  lDummy != m_lOrgRoadSequence)
	{
		pFieldDef->m_bChanged = TRUE;
		SetButton( Changed() );
	}
}

/** 
 * @fn bool CAttrAccessPointDlg::SetLineClassList(LONG nLineClass)
 * @brief        路線種別リストのコンボボックスを設定する
 *
 * @param	nLineClass	[in]	路線種別コード
 *
 * @return	コンボボックスの設定できたらTRUE、失敗したらFALSE
 */
bool CAttrAccessPointDlg::SetLineClassList(LONG nLineClass)
{
    // 路線種別リストのコンボボックス
    CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
    // アイテムのクリア
    cbLineClass.ResetContent();

	// 路線コードテーブルを取得していなければ、取得する
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;

	// 与えられた路線コードの路線の路線種別コードを取得
	_ICursorPtr		ipCursor;
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("ROAD_CODE = %ld"), nLineClass );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;

	long aLineClass = -1;
	if (ipCursor )
	{
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursorに格納されたRoad_Code_Listのレコードのうち、最初の１つからLineClass_cを取得
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}

	// 路線種別コードのコード値ドメインを取得
	ATLASSERT(ipRoadCodeListTable);
	IFieldsPtr ipFields(ipRoadCodeListTable->Fields);
	IDomainPtr ipDomain(ipFields->GetField(ipFields->_FindField(sindy::schema::road_code_list::kLineClass))->GetDomain());
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);

	// コンボボックスにアイテムを追加
	int nCurIdx = 0;
	for (int i = 0; i < ipCodedValueDomain->GetCodeCount(); ++i)
	{
		long aValue	= ipCodedValueDomain->GetValue(i).lVal;
		CString aName = COLE2T(ipCodedValueDomain->GetName(i));

		int nIndex = cbLineClass.AddString(aName);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbLineClass.SetItemData(nIndex, aValue);
			if(aValue == aLineClass)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return false;
		}
	}

    // カーソルの設定
    cbLineClass.SetCurSel(nCurIdx);
    // 現在の路線種別の取得
    m_lLineClass = cbLineClass.GetItemData(nCurIdx);

    CFieldDef* pFieldDef;
    CComVariant                vaRoadCode;        
    pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_CODE") );
    vaRoadCode = pFieldDef->m_vaValue;

    RedrawRoadNameList(vaRoadCode.lVal);
    return true;
}

/**
 * @fn bool CAttrAccessPointDlg::GetLineClass(int* pClass)
 * @brief 設定されている路線種別を取得する
 *
 * @param pClass	[out]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrAccessPointDlg::GetLineClass(int* pClass)
{
	if(pClass == NULL)
		return false;
	// コンボボックスの取得
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// インデックスの取得
	int nIndex = cbLineClass.GetCurSel();
	if(nIndex == CB_ERR)
		return false;
	// 値の取得
	*pClass = cbLineClass.GetItemData(nIndex);

	return true;
}

/**
 * @fn bool CAttrAccessPointDlg::SetLineClass(int nClass)
 * @brief 路線種別の値からコンボボックスを設定
 *
 * @param nClass	[in]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrAccessPointDlg::SetLineClass(int nClass)
{
	// コンボボックスの取得
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
	// インデックスの計算
	int i = 0;
	int nMaxCount = cbLineClass.GetCount();
	for( ; i < nMaxCount; ++i)
	{
		int nLineClass = cbLineClass.GetItemData(i);
		if(nLineClass == nClass)
			break;
	}
	if(i == nMaxCount)
		return false;
	// コンボボックスの設定
	cbLineClass.SetCurSel(i);

	return true;
}

/**
 * @fn bool CAttrAccessPointDlg::RedrawRoadNameList(LONG nRoadCode)
 * @brief 路線名称コンボボックスの更新
 *
 * @param nRoadCode	[in]	路線コードの値
 *
 * @return	路線名称が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrAccessPointDlg::RedrawRoadNameList(LONG nRoadCode)
{
	// 路線名称コンボボックス
	CComboBox cbRoadName = CComboBox(GetDlgItem(IDC_COMBO_ROADNAME_REF));
	// コンボボックスの情報のクリア
	cbRoadName.ResetContent();
	// カナ名称でマップの作成
	std::map<CString, int> cYomiMap;

	// 路線コード情報の再読み込み
	if(! GetRoadCodeSet(&m_cRoadCodeSet))
	{
		AttrMessageBox( AheLoadString( IDS_FAILTOGETROUTECODE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return 0;
	}
	for(CHetRoadCodeSet::const_iterator aIter = m_cRoadCodeSet.begin(); aIter != m_cRoadCodeSet.end(); ++aIter)
	{
		if(m_lLineClass == 0 || aIter->second.m_nLineClass == m_lLineClass)
			cYomiMap.insert(std::map<CString, int>::value_type(aIter->second.m_strNameYomi, aIter->second.m_nRoadCode));
	}

	// リストボックスにアイテムの追加
	int nCurIdx = 0;
	for(std::map<CString, int>::const_iterator aIter2 = cYomiMap.begin(); aIter2 != cYomiMap.end(); ++aIter2)
	{
		const CHetRoadCode* pRoadCode = m_cRoadCodeSet.FindRoadCode(aIter2->second);
		if(pRoadCode)
		{
			int nIndex = cbRoadName.AddString(pRoadCode->m_strNameKanji);
			if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
			{
				cbRoadName.SetItemData(nIndex, pRoadCode->m_nRoadCode);
				if(pRoadCode->m_nRoadCode == nRoadCode)
					nCurIdx = nIndex;
			}
			else
			{
				AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTENAME ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return false;
			}
		}
	}
    // 現在のカーソルの設定
    cbRoadName.SetCurSel(nCurIdx);
    // 現在の路線コードの取得
    m_lRoadCode = cbRoadName.GetItemData(nCurIdx);

    return true;
}

/**
 * @fn bool CAttrAccessPointDlg::GetRoadCode(int* pRoadCode)
 * @brief 路線コードエディットボックスから路線コードの取得
 *
 * @param nRoadCode	[out]	路線コードの値
 *
 * @return	路線コードが取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrAccessPointDlg::GetRoadCode(int* pRoadCode)
{
	// エディットボックスの取得
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// 値の取得
	TCHAR lpszRoadCode[32];
	if(edRoadCode.GetLine(0, lpszRoadCode, 32) == 0)
		return false;
	// 数値に変換
	if(! CHwyHelper::StrToRoadCode(pRoadCode, lpszRoadCode))
		return false;

	return true;
}


/**
 * @fn bool CAttrHwyNodeDlg::SetRoadCode(int nRoadCode)
 * @brief 路線コードの値を指定されたコードに置き換える
 *
 * @param nRoadCode	[in]	路線コードの値
 *
 * @return	路線コードが設定できたらTRUE、設定できなかったらFALSE
 */
bool CAttrAccessPointDlg::SetRoadCode(int nRoadCode)
{
	// 文字列変換
	TCHAR lpszRoadCode[8];
	if(! CHwyHelper::RoadCodeToStr(nRoadCode, lpszRoadCode))
		return true;
	// エディットボックスの取得
	CEdit edRoadCode = CEditT<CWindow>(GetDlgItem(IDC_EDIT_ROSENCODE));
	// 値の設定
	edRoadCode.SetSel(0, -1);
	edRoadCode.ReplaceSel(lpszRoadCode);

	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("ROAD_CODE") );
	pFieldDef->m_vaValue = nRoadCode;
	if( nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;        
		SetButton( Changed() );
	}
	return true;
}

/**
 * @fn BOOL CAttrAccessPointDlg::ErrorCheck()
 * @brief エラーがないかどうか調べる
 *
 * ダイアログを閉じる際に、AttributeDlgObjから呼ばれる
 * 方面名称と方面カナ名称がちゃんと対になって両方入っているか、または入っていないかたしかめる
 *
 * @return	エラーがなかったらTRUE、エラーがあればFALSE
 */
BOOL CAttrAccessPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == ACCESS_POINT_TABLE_NAME )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaName_Kanji, vaName_Yomi;
			pFieldDef = it->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("DIRECTION_KANJI") );
			vaName_Kanji = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( ACCESS_POINT_TABLE_NAME, _T("DIRECTION_YOMI") );
			vaName_Yomi = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			if( vaName_Kanji.vt != VT_NULL && vaName_Yomi.vt == VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_INVALIDDIRSIGNKANA ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( vaName_Kanji.vt == VT_NULL && vaName_Yomi.vt != VT_NULL )
			{
				strMessage.Format(AheLoadString( IDS_INVALIDDIRSIGNKANJI ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

/**
* @brief	路線コードテーブル取得
*/
ITablePtr CAttrAccessPointDlg::GetRoadCodeTable()
{
	ITablePtr ipRoadCodeListTable;
	IWorkspacePtr ipWorkspace;
	ITablePtr ipCurrentTable;
	IFeaturePtr ipCurrentFeature;
	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( ipCurrentFeature == NULL )
		return NULL;
	ipCurrentFeature->get_Table( &ipCurrentTable );
	IDatasetPtr ipDataset( ipCurrentTable );
	if( ipDataset == NULL )
		return NULL;
	ipDataset->get_Workspace( &ipWorkspace );
	if( ipWorkspace == NULL )
		return NULL;
	
	// REFERENCE.ROAD_CODE_LISTテーブル取得
	CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
	if( strOwnerName.IsEmpty() == TRUE )
		strOwnerName = _T("ROAD_CODE_LIST");
	else
		strOwnerName += _T(".ROAD_CODE_LIST");

	return AheOpenTableByTableName( ipWorkspace, strOwnerName );
}

/**
* @brief	路線コード取得
*/
bool CAttrAccessPointDlg::GetRoadCodeSet(CHetRoadCodeSet* pCodeSet)
{
	HRESULT hr;
	// テーブルの取得
	ITablePtr ipRoadCodeListTable = GetRoadCodeTable();
	if(!ipRoadCodeListTable)
		return false;
	// 行取得のパラメータの設定
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// 行の取得
	_ICursorPtr ipCursor;
	hr = ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor);
	if(FAILED(ipRoadCodeListTable->Search(ipQFilter, VARIANT_FALSE, &ipCursor))){
		::MessageBox(m_hWnd, AheLoadString( IDS_FAILTOGETROW ), AheLoadString( IDS_CAPTION ), MB_OK);
		return false;
	}
	// 各行から情報の取得
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CHetRoadCode cRoadCode(ipRow);
		pCodeSet->Append(cRoadCode);
	}
	return true;
}

/**
 * @brief 路線コードの値に従ってハイウェイモードフラグを変更
 * @note	[Bug7342]対応時に追加
 * @param[in]	cRoadCode	路線コード
 */
void CAttrAccessPointDlg::ChangeHwyModeCheckBox(long cRoadCode)
{
	// 路線コードが200000以上ならハイウェイモードフラグをON、200000未満ならOFFにする
	CButton(GetDlgItem(IDC_CHECK_HWYMODEFLAG)).SetCheck(cRoadCode >= 200000 ? BST_CHECKED : BST_UNCHECKED);
	m_cControlRel.ControlChanged( IDC_CHECK_HWYMODEFLAG );
}

void __stdcall CAttrAccessPointDlg::SelChangeGridAccessStore()
{
	// カレント行を強調表示
	SetASGridColor();

	// グリッドの値を他のコントロールに反映
	UpdateASControls();
}

std::vector<long> CAttrAccessPointDlg::GetASEditableControlIDs()
{
	return boost::assign::list_of
		(IDC_EDIT_MASTERCODE)
		(IDC_EDIT_START_HOUR)
		(IDC_EDIT_START_MIN)
		(IDC_EDIT_END_HOUR)
		(IDC_EDIT_END_MIN)
		(IDC_RADIO_GS)
		(IDC_RADIO_CHAIN)
		(IDC_CHECK_MONDAY)
		(IDC_CHECK_TUESDAY)
		(IDC_CHECK_WEDNESDAY)
		(IDC_CHECK_THURSDAY)
		(IDC_CHECK_FRIDAY)
		(IDC_CHECK_SATURDAY)
		(IDC_CHECK_SUNDAY)
		(IDC_CHECK_HOLIDAY)
		(IDC_CHECK_GOLDENWEEK)
		(IDC_CHECK_BON)
		(IDC_CHECK_YEAREND)
		(IDC_CHECK_NEWYEAR)
		(IDC_BUTTON_CHECK_ALL)
		(IDC_BUTTON_UNCHECK_ALL)
		(IDC_BUTTON_ADD_ACCESS_STORE)
		(IDC_BUTTON_UPDATE_ACCESS_STORE)
		(IDC_BUTTON_DELETE_ACCESS_STORE)
		(IDC_BUTTON_SAVE_ACCESS_STORE)
	;
}

std::vector<long> CAttrAccessPointDlg::GetASCheckBoxIDs()
{
	return boost::assign::list_of
		(IDC_CHECK_MONDAY)
		(IDC_CHECK_TUESDAY)
		(IDC_CHECK_WEDNESDAY)
		(IDC_CHECK_THURSDAY)
		(IDC_CHECK_FRIDAY)
		(IDC_CHECK_SATURDAY)
		(IDC_CHECK_SUNDAY)
		(IDC_CHECK_HOLIDAY)
		(IDC_CHECK_GOLDENWEEK)
		(IDC_CHECK_BON)
		(IDC_CHECK_YEAREND)
		(IDC_CHECK_NEWYEAR)
	;
}


