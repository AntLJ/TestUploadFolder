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

// AttrUpdownLinkDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrUpdownLinkDlg.h"
#include <Map.h>
#include <boost/assign/list_of.hpp>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/registry_util.h>
#include "UpdownMap.h"	// 上下線リンクダイアログのウィンドウ配置マップ

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;

const CString CAttrUpdownLinkDlg::UNDEF_NAME = _T("未設定");
const long CAttrUpdownLinkDlg::UNDEF_OID = 0;
const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLink =REGPATH_SINDY_E_CHILDDLG  _T("UPDOWN_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrUpdownLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrUpdownLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrUpdownLinkDlg::CAttrUpdownLinkDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrUpdownLinkDlg::CAttrUpdownLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_UPDOWNLINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// [SiNDYChildDlg] 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_UPDOWN_GROUP_ID,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_UPDOWN_GROUP_ID ) )
		( IDC_EDIT_GROUP_C,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_GROUP_C ) )
		( IDC_EDIT_LANE_COUNT,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_COUNT ) )
		( IDC_EDIT_RIGHT_CHANGE,CContainedWindow(  _T("EDIT"), this, IDC_EDIT_RIGHT_CHANGE ) )
		( IDC_EDIT_LEFT_CHANGE,	CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LEFT_CHANGE ) )

		( IDC_COMBO_MAX_LEGAL_SPEED_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MAX_LEGAL_SPEED_C ) )
		( IDC_COMBO_MIN_LEGAL_SPEED_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MIN_LEGAL_SPEED_C ) )
		( IDC_COMBO_TRAVEL_DIRECTION_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_TRAVEL_DIRECTION_C ) )
		( IDC_COMBO_ROAD_STRUCTURE_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_ROAD_STRUCTURE_C ) )
		( IDC_COMBO_ONEWAY_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_ONEWAY_C ) )

		( IDC_CHECK_RUBBING_F,	CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RUBBING_F ) )
		( IDC_CHECK_TUNNEL_F,	CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_TUNNEL_F ) )

		( IDC_COMBO_SPECULATION_C,	CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID,		CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) )
		( IDC_EDIT_SOURCE,		CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		
		( IDC_TAB_UPDOWNATTR, CContainedWindow(  _T("TAB"), this, IDC_TAB_UPDOWNATTR ));
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
 * @fn HWND CAttrUpdownLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrUpdownLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrUpdownLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrUpdownLinkDlg::Delete()
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrUpdownLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
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
BOOL CAttrUpdownLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_iChangeType, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// [SiNDYChildDlg] 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_EDIT_LANE_COUNT) );
	}
	
		// 初期化処理
	init();

	// 名称・読み用のツールチップ作成
	//
	UpdateGroupC();
	UpdateUnDefOID();

	createGrid();
			
	// グリッドの編集可能フィールドを設定
	readReg();

	return bRet;
}

/**
 * @fn void CAttrUpdownLinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrUpdownLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// [SiNDYChildDlg] ControlRelにフィールド名とテーブル名を登録

		m_cControlRel.SetControl( IDC_EDIT_LANE_COUNT,			TYPE_EDIT,			0, 0,	updown_link::kLaneCount,			0,	updown_link::kTableName );	
		m_cControlRel.SetControl( IDC_EDIT_RIGHT_CHANGE,		TYPE_EDIT,			0, 0,	updown_link::kRightChange,			0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_EDIT_LEFT_CHANGE,			TYPE_EDIT,			0, 0,	updown_link::kLeftChange,			0,	updown_link::kTableName );
		
		m_cControlRel.SetControl( IDC_COMBO_MAX_LEGAL_SPEED_C,	TYPE_COMBO,			0, 0,	updown_link::kMaxLegalSpeedC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_MIN_LEGAL_SPEED_C,	TYPE_COMBO,			0, 0,	updown_link::kMinLegalSpeedC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_TRAVEL_DIRECTION_C,	TYPE_COMBO,			0, 0,	updown_link::kTravelDirectionC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_ROAD_STRUCTURE_C,	TYPE_COMBO,			0, 0,	updown_link::kRoadStructureC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_COMBO_ONEWAY_C,			TYPE_COMBO,			0, 0,	updown_link::kOneWayC,				0,	updown_link::kTableName );
		
		m_cControlRel.SetControl( IDC_CHECK_RUBBING_F,			TYPE_CHECKBOX,		0, 0,	updown_link::kRubbingF,				0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_CHECK_TUNNEL_F,			TYPE_CHECKBOX,		0, 0,	updown_link::kTunnelF,				0,	updown_link::kTableName );
		
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C,		TYPE_COMBO,			0, 0,	updown_link::kSperculationC,		0,	updown_link::kTableName );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID,			TYPE_EDITSTRING,	0, 0,	updown_link::kSourceID,				0,	updown_link::kTableName, 0, -1, FOLLOW_READONLY);
				
		// Static Source
		m_cControlRel.SetControl( IDC_STATIC_VAR, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);

		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDIT,			EDITVIEW_KIHON, EDITVIEW_NODE,	updown_link::kSource,				0,	updown_link::kTableName );

		// 制御する必要のないコントロール
		m_cControlRel.SetControl( IDC_EDIT_UPDOWN_GROUP_ID,		TYPE_EDIT,			0, 0,	updown_link::kUpdoenGroupID,		0,	updown_link::kTableName, 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_EDIT_GROUP_C,				TYPE_EDIT,			0, 0,	0,			0, 0 , 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
	
		
		// 上下線グループOID
		m_cControlRel.SetControl( IDC_STATIC_UPDOWN_GROUP_ID,   TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 上下線グループ属性
		m_cControlRel.SetControl( IDC_STATIC_GROUP_C,           TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 車線数
		m_cControlRel.SetControl( IDC_STATIC_LANE_COUNT,        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 右側増減車線数
		m_cControlRel.SetControl( IDC_STATIC_RIGHT_CHANGE,      TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 左側増減車線数
		m_cControlRel.SetControl( IDC_STATIC_LEFT_CHANGE,       TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 制限速度コード(最大)
		m_cControlRel.SetControl( IDC_STATIC_MAX_LEGAL_SPEED_C, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 制限速度コード(最小)
		m_cControlRel.SetControl( IDC_STATIC_MIN_LEGAL_SPEED_C, TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 進行方向種別コード
		m_cControlRel.SetControl( IDC_STATIC_TRAVEL_DIRECTION_C,TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 道路構造種別コード
		m_cControlRel.SetControl( IDC_STATIC_ROAD_STRUCTURE_C,  TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 一方通行種別コード
		m_cControlRel.SetControl( IDC_STATIC_ONEWAY_C,          TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 推測コード
		m_cControlRel.SetControl( IDC_STATIC_SPECULATION_C,     TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// 提供元ID
		m_cControlRel.SetControl( IDC_STATIC_SOURCE_ID,         TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// ATTR_GROUP
		m_cControlRel.SetControl( IDC_STATIC_ATTR_GROUP,        TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
		// PECULATION_C_GROUP
		m_cControlRel.SetControl( IDC_STATIC_PECULATION_C_GROUP,TYPE_NONE,	EDITVIEW_KIHON, EDITVIEW_KIHON);
				
		// Grid Info
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);
		// Grid
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	EDITVIEW_NODE, EDITVIEW_NODE);
	
	}
}
/**
 * エラーチェック
 * @TODO	名称・読みの片方がNULLだった場合のみチェック
 *			他チェックを要見直し
 */
BOOL CAttrUpdownLinkDlg::ErrorCheck()
{
	// 今の所空
	return TRUE;
}

/**
 * @fn void CAttrRoadNodeDlg::SetToolTip(INT nTargetItem)
 * @brief ツールチップを作成します。
 *
 * @note 作成するツールチップは属性に変更がかからないため、作りっぱなしで問題なし
 *
 * @param nTargetItem	[in]	ツールチップを作成するコントロールアイテムのID
 * 
 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg を参考
 * @TODO	車線モデルでも使っているので共通化したい
*/
void CAttrUpdownLinkDlg::SetToolTip(INT nTargetItem)
{
	// エディットボックスにかかれている文字列を取得
	CEdit edit( GetDlgItem(nTargetItem) );
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	// まだツールチップ作成されていなかったら作成する（一番最初のみ入ってくる）
	HWND hToolTip;
	hToolTip =
		CreateWindowEx(
		0,				//拡張ウィンドウスタイル
		TOOLTIPS_CLASS,	//クラスネーム
		NULL,			//ウィンドウネーム
		TTS_ALWAYSTIP,	// ウィンドウスタイル
		CW_USEDEFAULT,	// Ｘ座標
		CW_USEDEFAULT,	// Ｙ座標
		CW_USEDEFAULT,	// 幅
		CW_USEDEFAULT,	// 高さ
		GetDlgItem(nTargetItem),// 親ウィンドウのハンドル
		NULL,			// メニューハンドル
		NULL,			// インスタンスハンドル
		NULL);			// WM_CREATEデータ

	// ToolInfo構造体 
	static TOOLINFO ti;
	ZeroMemory(&ti , sizeof(TOOLINFO)) ; 
	ti.cbSize = sizeof(TOOLINFO) ; 
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS ; 
	ti.hwnd = GetDlgItem(nTargetItem);
	ti.uId =  (UINT_PTR)GetDlgItem(nTargetItem).m_hWnd;

	ti.lpszText = lpszText;

	::SendMessage( hToolTip, TTM_ADDTOOL , 0 , (LPARAM)&ti);
	::SendMessage( hToolTip, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
	::SendMessage( hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);

	CWindow cWnd( hToolTip );
	cWnd.SetFont( m_hFont );
}


void CAttrUpdownLinkDlg::UpdateGroupC()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	//グループ種別コード取得
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return;
	}
	ITablePtr ipUpdownLinkGroup = AheOpenSameOwnerTable(ipTable, updown_link_group::kTableName);
	if(!ipUpdownLinkGroup)
	{
		return;
	}

	//上下線グループOIDから属性コード値取得
	long updoenGroupID = AheGetAttribute(ipFeature, updown_link::kUpdoenGroupID).lVal;
	CString where_clause;
	where_clause.Format(_T("%s=%d"), updown_link_group::kObjectID, updoenGroupID);
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());
	ipUpdownLinkGroup->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	_IRowPtr ipRow;
	long groupC = 1;
	while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
	{
		if(!ipRow)
			return;
		groupC = AheGetAttribute(ipRow, updown_link_group::kGroupC).lVal;
	}
	//グループ種別コーから属性値名称取得
	if(!SetDomain())
	{
		return;
	}
	CString str;
	if(updoenGroupID == UNDEF_OID)
	{
		str = UNDEF_NAME;
	}
	else
	{
		str = m_segAttrDomain[groupC];
	}

	//コントロール更新
	CEdit(GetDlgItem(IDC_EDIT_GROUP_C)).SetWindowText(str);
}

bool CAttrUpdownLinkDlg::SetDomain(bool update/* = false*/)
{
	//更新しない場合
	if(!update)
	{
		//設定済み(空でなければOK返す)
		if(!m_segAttrDomain.empty())
		{
			return true;
		}
	}

	m_segAttrDomain.clear();

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if(!ipFeature)
	{
		return false;
	}

	//上下線リンクグループテーブル接続
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return false;
	}
	ITablePtr ipUpdownLinkGroup = AheOpenSameOwnerTable(ipTable, updown_link_group::kTableName);
	if(!ipUpdownLinkGroup)
	{
		return false;
	}

	//ドメイン値取得
	IDomainPtr ipDomain;
	long index = -1;
	ipUpdownLinkGroup->FindField(const_cast<BSTR>( updown_link_group::kGroupC), &index);
	IFieldsPtr ipFields;
	ipUpdownLinkGroup->get_Fields(&ipFields);
	if(!ipFields)
	{
		return false;
	}
	IFieldPtr ipField;
	ipFields->get_Field(index, &ipField);
	if(!ipField)
	{
		return false;
	}

	ipField->get_Domain(&ipDomain);
	if (!ipDomain)
	{
		return false;
	}
	ICodedValueDomainPtr ipCodeDomain(ipDomain);
	if (!ipCodeDomain)
	{
		return false;
	}

	LONG n = 0;
	ipCodeDomain->get_CodeCount(&n);
	for (long i=0 ; i<n ; i++)
	{
		BSTR name;
		CComVariant val;

		HRESULT hr1 = ipCodeDomain->get_Name (i, &name);
		HRESULT hr2 = ipCodeDomain->get_Value(i, &val);
		if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
		{
			m_segAttrDomain[val.lVal] = CString(name);
		}
	}

	if(m_segAttrDomain.empty())
	{
		return false;
	}

	return true;
}

void CAttrUpdownLinkDlg::UpdateUnDefOID()
{
	//OIDがUNDEF_OIDなら表示名更新
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
		return;

	//セグメントID
	if(AheGetAttribute(ipFeature, updown_link::kUpdoenGroupID).lVal == UNDEF_OID)
	{
		//コントロール更新
		CEdit(GetDlgItem(IDC_EDIT_SEGMENT)).SetWindowText(UNDEF_NAME);
	}
}

// 初期化処理
void CAttrUpdownLinkDlg::init()
{
	_IRowPtr currentRow = GetCurrentFeatureDef()->GetFeature();
	long OID = AheGetOID( currentRow );
	
	BOOL isEnableWindow(TRUE);
	if(m_GIDMap.find(OID) == m_GIDMap.end())
	{
		// 2016/09/13 GIDが空の場合が考慮されていない為修正
		CComVariant gid =  AheGetAttribute(currentRow, updown_link::kGlobalID);
		if(gid.vt != VT_NULL)
		{
			m_GIDMap[OID] = AheGetAttribute(currentRow, updown_link::kGlobalID);
		}
		else
		{
			// GIDが空の場合場合は紐付け機能を機能させない
			MessageBox( AheLoadString( IDS_NOT_USE_RELATION ), AheLoadString( IDS_TOOLTIP ), MB_OK | MB_ICONINFORMATION );
			isEnableWindow = FALSE;
		}
	}


	if(m_relRoadLinkOrgs.find(OID) == m_relRoadLinkOrgs.end())
		m_relRoadLinkOrgs[OID] = std::map<long, _IRowPtr>();

	if(m_relRoadLinkLists.find(OID) == m_relRoadLinkLists.end())
		m_relRoadLinkLists[OID] = InvolvedRoadLinkMap();

	if(m_searchedRoadLinkLists.find(OID) == m_searchedRoadLinkLists.end())
		m_searchedRoadLinkLists[OID] = InvolvedRoadLinkMap();

	// 道路リンク方向コードのドメインを取得しておく
	m_directionCDomain = getDomainCodeValue( m_cArcHelper.GetFocusMap(),
		                                   rel_road_link_updown_link::kTableName,
		                                   rel_road_link_updown_link::kRoadLinkDirectionC);
}

// 編集対象の道路リンク－上下線リンク関連レコード(ORG)を返却する
std::map<long, _IRowPtr>& CAttrUpdownLinkDlg::getCurrentRelRoadLinkOrg()
{
	long OID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	return m_relRoadLinkOrgs[OID];
}

// 編集対象の上下線リンクに紐付いている道路リンクを返却する
InvolvedRoadLinkMap& CAttrUpdownLinkDlg::getCurrentRelRoadLinkList()
{
	long OID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	return m_relRoadLinkLists[OID];
}

// 編集対象の周辺検索を行った道路リンクを返却する
InvolvedRoadLinkMap& CAttrUpdownLinkDlg::getCurrentSearchedList()
{
	long OID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	return m_searchedRoadLinkLists[OID];
}

// 再描画
LRESULT CAttrUpdownLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	highlightLine();
	return 0;
}

// 道路リンクを取得
IFeatureClassPtr CAttrUpdownLinkDlg::getRoadLinkFC()
{
	if( m_roadLinkFeatureClass )
		return m_roadLinkFeatureClass;

	m_roadLinkFeatureClass = getSameTable(road_link::kTableName, nullptr);

	return m_roadLinkFeatureClass;
}

// TOCにある指定したテーブル名のITableを取得
ITablePtr CAttrUpdownLinkDlg::getSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner)
{
	// TOCにある指定したテーブル名のITableを取得
	CMap cMap =  m_cArcHelper.GetFocusMap();

	ITablePtr ret = AheGetTable( cMap.FindLayer( nullptr, lpcszOwner, lpcszName ) );

	return ret;
}


// 道路リンク－上下線リンク関連レコードの更新
bool CAttrUpdownLinkDlg::Update()
{
	using namespace sindy::schema::adam_v2::rel_road_link_updown_link;

	// 更新対象の関連テーブル取得しておく
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	ITablePtr ipRelUpdownLink = AheOpenSameOwnerTable(AheGetTable(ipFeature), rel_road_link_updown_link::kTableName);

	for (const auto& orgRelMap: m_relRoadLinkOrgs)
	{
		long updownLinkOid = orgRelMap.first;

		InvolvedRoadLinkMap chgRelLinks = m_relRoadLinkLists.at(updownLinkOid);

		for (auto& chgRelLink : chgRelLinks)
		{
			// 変更があれば更新
			if ( orgRelMap.second.find(chgRelLink.first) != orgRelMap.second.end() )
			{
				if( isChangeDirectionC(chgRelLink.first) )
				{
					_IRowPtr org =  orgRelMap.second.at(chgRelLink.first);

					if( !AheSetModifyData( org, sindyUpdateTypeProperty ))
						return false;

					org->put_Value( AheFindField( AheGetFields( org ), kRoadLinkDirectionC ),
									CComVariant( chgRelLink.second.m_directionC ) );
					org->Store();
				}
			}
			// 元々の関連テーブルに存在しない場合は新規追加
			else
			{
				_IRowPtr row = AheCreateRow( ipRelUpdownLink );
				if( !row )
					return false;

				// 属性付与
				if( !AheSetModifyData( row, sindyUpdateTypeCreate ) ||
					!AheSetAttribute( row, rel_road_link_updown_link::kUpdownLinkGID,  m_GIDMap[updownLinkOid]) ||
					!AheSetAttribute( row, rel_road_link_updown_link::kRoadLinkID, chgRelLink.first) ||
					!AheSetAttribute( row, rel_road_link_updown_link::kRoadLinkDirectionC, (long)chgRelLink.second.m_directionC)
					)
					return false;

				row->Store();
			}
		}
		
		// 削除された道路リンク－上下線リンク関連レコードを見つける
		std::list<long> deleteList;
		for (const auto& orgRelLink : orgRelMap.second)
		{
			// リストボックスからなくなってしまっているものは削除されたと判断
			if ( chgRelLinks.find(orgRelLink.first) == chgRelLinks.end() )
				deleteList.push_back(orgRelLink.first);
		}

		// 削除処理
		for( const auto& deleteOid : deleteList )
		{
			_IRowPtr row = orgRelMap.second.at(deleteOid);
			row->Delete();
			row->Store();
		}
	}

	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), updown_link::kTableName) != 0) continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged()) continue;

		if (!m_nqDataGridMap[oid].store()) return false;
	}

	return true;
}

// 道路リンク－上下線リンク関連レコードに変更があったか
bool CAttrUpdownLinkDlg::isChangeRefTbl()
{
	// 先ず、数が一致するか確認
	if( getCurrentRelRoadLinkOrg().size() != getCurrentRelRoadLinkList().size() )
		return true;

	// 道路リンクのOID、道路リンク方向コードに変更があるか確認
	for( const auto& relRec : getCurrentRelRoadLinkList() )
	{
		if(getCurrentRelRoadLinkOrg().find(relRec.first) == getCurrentRelRoadLinkOrg().end() ) 
			return true;
	}
	return false;
}

// 道路リンク方向コードに変更があったか
bool CAttrUpdownLinkDlg::isChangeDirectionC(long oid)
{
	// 新規に追加されたレコードは常にtrue
	if (getCurrentRelRoadLinkOrg().find(oid) == getCurrentRelRoadLinkOrg().end())
		return true;
	
	_IRowPtr org = getCurrentRelRoadLinkOrg().at(oid);

	int srcC  = AheGetAttribute( org, rel_road_link_updown_link::kRoadLinkDirectionC).intVal;
	int dstC  = getCurrentRelRoadLinkList().at(oid).m_directionC;
	
	return srcC != dstC;
}

// コード値ドメインを取得する
std::map<CString,long> CAttrUpdownLinkDlg::getDomainCodeValue( IMap* ipMap ,LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName )
{
	std::map<CString,long> mapCodeValue;
	// コード値ドメインを取得する
	IDomainPtr ipDomain;
	if( AheGetDomainByFieldName( ipMap, lpcszTableName, lpcszFieldName, &ipDomain ) )
	{
		ICodedValueDomainPtr ipCVDomain( ipDomain );
		if( ipCVDomain )
		{
			long lCount = 0;
			ipCVDomain->get_CodeCount( &lCount );
			for( int i = 0; i < lCount; ++i )
			{
				CComBSTR bstrValue;
				CComVariant val;
				ipCVDomain->get_Name( i, &bstrValue );
				ipCVDomain->get_Value( i, &val );
				mapCodeValue.insert( std::pair<CString,long>( CString(bstrValue), val.intVal ) );
			}
		}
	}
	return mapCodeValue;
}

// ウィンドウマネージャのマップの切り替えを行う
void CAttrUpdownLinkDlg::ChangeWindowMgrMap( int iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_KIHON:		// 基本属性タブ
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_NODE:		// ノード列属性タブ
		m_winMgr.ChangeMgrMap( NODEMAP );
		break;	
	default:
		break;
	}
	// 再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}

// グリッドを生成する
void CAttrUpdownLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable)	return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_updown_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// 始めてダイアログに表示したフィーチャであれば、ノード列グリッドを登録
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_updown_node::kObjectID)
			.addField(nq_updown_node::kSequence)
			.addField(nq_updown_node::kWidth)
			.addField(nq_updown_node::kUpdownNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// グリッド初期化
	m_nqDataGridMap[m_currentOid].init();

	// ノード列データをグリッドへ読み込む
	CComVariant linkGid = AheGetAttribute(ipFeature, updown_link::kGlobalID);
	if (linkGid.vt == VT_NULL) return;

	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_updown_node::kUpdownLinkGID, linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(linkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrUpdownLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
{
	// 編集未開始時はグリッドの編集不可
	if (!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) {
		*Cancel = VARIANT_TRUE;
		return;
	}

	// グリッド列ごとの編集可否設定はグリッドクラス側へ委譲
	m_nqDataGridMap[m_currentOid].onBeforeEdit(Row, Col, Cancel);
}

// MouseUp Event on Grid
void CAttrUpdownLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	m_gridSelect = false;
	// クリック
	if ((!IsEditableFeature() || !m_cArcHelper.IsStartEditing()) && Button == 1 )
	{
		m_gridSelect = true;
		m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);
	}
	else if(IsEditableFeature() && m_cArcHelper.IsStartEditing() && (Button == 1 || Button == 2))
	{
		m_gridSelect = true;
		m_nqDataGridMap[m_currentOid].onGridMouseDown(Button, Shift, X, Y, Cancel);
		m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);	

		// 右クリックならメニューを出す
		if(Button == 2) gridMenu( IDC_GRID_NODELIST, m_nqUpdownNodeGrid, X, Y );
	}
}

// After Edit
void CAttrUpdownLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// 関連テーブルの更新情報の更新
void CAttrUpdownLinkDlg::updateChanged()
{
	// 関連テーブル(ノード列)の変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_updown_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// ボタン制御
	SetButton(Changed());
}

// 右クリックすると <削除> ポップアップ
void CAttrUpdownLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
{
	long selectedRow = ipGrid->GetSelectedRow(0);

	// カーソル位置取得
	POINT stPos = {0,0};
	::GetCursorPos( &stPos);

	CMenu menu;
	menu.LoadMenuW( IDR_MENU );
	CMenu popupMenu = menu.GetSubMenu(0);

	if(selectedRow <= 0) popupMenu.EnableMenuItem(ID_DEL, MF_GRAYED |MF_BYCOMMAND |MF_DISABLED);

	// Popupメニュー表示
	int nResult = popupMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, stPos.x, stPos.y, m_hWnd, 0);

	if(nResult == ID_DEL && MessageBox(_T("削除しますか？"),_T("確認"), MB_OKCANCEL) == IDOK)
	{
		CComBSTR bstrValue;
		ipGrid->get_TextMatrix(_variant_t(selectedRow), _variant_t(0L), &bstrValue);

		CString whereClause = uh::str_util::format(_T("%s=%s"),
			nq_updown_node::kObjectID, bstrValue);

		CString objectID = bstrValue.m_str;

		// 以下のif分は、新規レコードの場合、OIDをRowDataに入れて管理するための処理である
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_updown_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}		

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_updown_node::kTableName);
		pRelTableDef->setChanged(changeFlag);

		// ボタン制御
		SetButton(changeFlag);
	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_updown_node::kTableName);
		pRelTableDef->setChanged(true);

		// ボタン制御
		SetButton(true);
	}
}

// Check Data Changes
bool CAttrUpdownLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrUpdownLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// ダイアログで表示している上下線リンクフィーチャを取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// グリッドで選択列のノード列レコードから、"順番","上下線リンクGID"を取得する
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNodeT = AheOpenSameOwnerTable(ipTable, updown_node::kTableName);
	if (!ipNodeT) return;

	CString nodeGid, nodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(nodeGid, nodeGidNext, nq_updown_node::kSequence, nq_updown_node::kUpdownNodeGID);

	//   検索されたノード列のノードGIDから次のノードを検索
	CString whereClause = uh::str_util::format(_T("%s='%s'"), updown_node::kGlobalID, nodeGid);
	_ICursorPtr ipNodeCur;
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	IFeaturePtr ipNodeFeature;
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), updown_node::kGlobalID, nodeGidNext);
	if (FAILED(ipNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	if (IFeatureCursorPtr(ipNodeCur)->NextFeature(&ipNodeFeature) != S_OK) return;
	if (!ipNodeFeature) return;

	ipNodeFeature->get_ShapeCopy(&ipToPoint);
}

// グリッドで選択しているノード列のラインをハイライトする
void CAttrUpdownLinkDlg::highlightLine()
{
	if (m_nqDataGridMap.empty()) return;

	// グリッド選択行のノード列レコードの次のノードを取得する
	//   ノード列から次のノードにあたるレコードを検索
	IGeometryPtr ipFromPoint, ipToPoint;
	getSequenceGeometry(ipFromPoint, ipToPoint);
	
	if(!ipFromPoint) return;

	ISpatialReferencePtr ipSpatialReference;
	ipFromPoint->get_SpatialReference(&ipSpatialReference);

	IPolylinePtr ipDrawLine(CLSID_Polyline);
	ipDrawLine->putref_SpatialReference(ipSpatialReference);
	ipDrawLine->put_FromPoint(IPointPtr(ipFromPoint));
	ipDrawLine->put_ToPoint(IPointPtr(ipToPoint));

	moveToCenter(ipFromPoint, ipToPoint);

	// ライン描画（描画中のラインは消す)
	AheDrawPolyline(m_cArcHelper.GetDisplay(), ipDrawLine, LINE_COLOR, 5, esriSLSSolid, TRUE, TRUE);	
}

// Tear down connection points
LRESULT CAttrUpdownLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// ノード列のジャンプ,中心へ移動
void CAttrUpdownLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
{
	// 中心へ移動
	IActiveViewPtr ipActiveView = m_cArcHelper.GetActiveView();
	
	if (ipActiveView && m_gridSelect && ipFromPoint) {
		m_gridSelect = false;
		IEnvelopePtr ipEnvelope;
		
		// Pointの座標
		double fromX(0), fromY(0), toX(0), toY(0);

		IPointPtr(ipFromPoint)->QueryCoords(&fromX, &fromY);
		if(ipToPoint)
		{
			IPointPtr(ipToPoint)->QueryCoords(&toX, &toY);
		}
		else
		{
			toX = fromX;
			toY = fromY;
		}

		ISpatialReferencePtr ipSpatialRefer;
		ipFromPoint->get_SpatialReference(&ipSpatialRefer);

		IPointPtr ipPoint( CLSID_Point );
		ipPoint->putref_SpatialReference(ipSpatialRefer);
		ipPoint->PutCoords( (fromX + toX)/2, (fromY + toY)/2 );

		ipActiveView->get_Extent( &ipEnvelope );
		ipEnvelope->CenterAt(ipPoint);
		ipActiveView->put_Extent( ipEnvelope );

		ipActiveView->Refresh();		
	}
}

// レジストリからデータを読み込
void CAttrUpdownLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID フィールドは指定されても変更は不可
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_updown_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
