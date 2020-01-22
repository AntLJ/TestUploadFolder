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

// AttrLaneLinkDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrLaneLinkDlg.h"
#include <boost/assign/list_of.hpp>
#include "LaneMap.h"	// 車線リンクダイアログのウィンドウ配置マップ
#include <TDC/useful_headers/registry_util.h>

using namespace sindy::schema;
using namespace sindy::schema::adam_v2;
using namespace sindy::schema::category;

const CString CAttrLaneLinkDlg::UNDEF_NAME = _T("未設定");
const long CAttrLaneLinkDlg::UNDEF_OID = 0;
const COLORREF LINE_COLOR = RGB(0xC0, 0xC0, 0x80);
const CString PathLaneLink =REGPATH_SINDY_E_CHILDDLG  _T("LANE_LINK");
const CString EditableNQFileds = _T("EditableNQFileds");


/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrLaneLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrLaneLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneLinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrLaneLinkDlg::CAttrLaneLinkDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrLaneLinkDlg::CAttrLaneLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_LANELINKDLG),
	m_currentOid(0L),m_iChangeType(0),m_lFeatureIndex(0L),m_lTableIndex(0L),m_lRowIndex(0L),m_bForce(false),m_bEditable(false),m_lCurrentIndex(0L),m_gridSelect(false)
{
	// [SiNDYChildDlg] 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_FROM_NODE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_FROM_NODE_ID ) )
		( IDC_EDIT_TO_NODE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_TO_NODE_ID ) )
		( IDC_EDIT_LANE_GROUP_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_GROUP_ID ) )
		( IDC_EDIT_LANE_GROUP_C, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_GROUP_C ) )
		( IDC_COMBO_ROAD_STRUCTURE_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_ROAD_STRUCTURE_C ) )
		( IDC_EDIT_LANE_NUMBER, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_NUMBER ) )
		( IDC_EDIT_LANE_COUNT, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_LANE_COUNT ) )
		( IDC_COMBO_MAX_LEGAL_SPEED_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MAX_LEGAL_SPEED_C ) )
		( IDC_COMBO_MIN_LEGAL_SPEED_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_MIN_LEGAL_SPEED_C ) )
		( IDC_COMBO_TRAVEL_DIRECTION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_TRAVEL_DIRECTION_C ) )
		( IDC_CHECK_LEFT_CHANGE_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_LEFT_CHANGE_F ) )
		( IDC_CHECK_RIGHT_CHANGE_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RIGHT_CHANGE_F ) )
		( IDC_CHECK_LEFT_PROTRUSION_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_LEFT_PROTRUSION_F ) )
		( IDC_CHECK_RIGHT_PROTRUSION_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RIGHT_PROTRUSION_F ) )
		( IDC_CHECK_RUBBING_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_RUBBING_F ) )
		( IDC_CHECK_TUNNEL_F, CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_TUNNEL_F ) )
		( IDC_EDIT_ROAD_LINK_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ROAD_LINK_ID ) )
		( IDC_COMBO_SPECULATION_C, CContainedWindow(  _T("COMBOBOX"), this, IDC_COMBO_SPECULATION_C ) )
		( IDC_EDIT_SOURCE_ID, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE_ID) )

		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ))
		( IDC_TAB_LANEATTR, CContainedWindow(  _T("TAB"), this, IDC_TAB_LANEATTR ));

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
 * @fn HWND CAttrLaneLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrLaneLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrLaneLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrLaneLinkDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrLaneLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrLaneLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
		// 名称・読みはNULL可なので、車線番号を最初にしておく
		//::SetFocus( GetDlgItem(IDC_EDIT_LANE_NUMBER) );
	}

	UpdateLaneGroup();
	GetRoadLinkID();
	UpdateUnDefOID();
	createGrid();

	// グリッドの編集可能フィールドを設定
	readReg();	

	return bRet;
}

/**
 * @fn void CAttrLaneLinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrLaneLinkDlg::CreateControlRelation()
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
		m_cControlRel.SetControl( IDC_TAB_LANEATTR,		TYPE_OTHER,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_NODE );
		m_cControlRel.SetControl( IDC_EDIT_FROM_NODE_ID,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kFromNodeID,			0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_EDIT_TO_NODE_ID,			TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kToNodeID,			0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_EDIT_LANE_GROUP_ID,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLaneGroupID,		0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_EDIT_LANE_GROUP_C,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	0,								0,	0,					   0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_COMBO_ROAD_STRUCTURE_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRoadStructureC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_LANE_NUMBER,			TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLaneNumber,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_LANE_COUNT,			TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLaneCount,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_MAX_LEGAL_SPEED_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kMaxLegalSpeedC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_MIN_LEGAL_SPEED_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kMinLegalSpeedC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_COMBO_TRAVEL_DIRECTION_C,	TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kTravelDirectionC,	0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_LEFT_CHANGE_F,		TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLeftChangeF,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_RIGHT_CHANGE_F,		TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRightChangeF,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_LEFT_PROTRUSION_F,  TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kLeftProtrusionF,	0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_RIGHT_PROTRUSION_F,	TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRightProtrusionF,	0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_RUBBING_F,			TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kRubbingF,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_CHECK_TUNNEL_F,			TYPE_CHECKBOX,		EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kTunnelF,			0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_ROAD_LINK_ID,		TYPE_EDIT,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	0,								0,	0,					   0, -1, FOLLOW_READONLY);	//表示のみのため常に読み取り専用
		m_cControlRel.SetControl( IDC_COMBO_SPECULATION_C,		TYPE_COMBO,			EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kSperculationC,		0,	lane_link::kTableName);
		m_cControlRel.SetControl( IDC_EDIT_SOURCE_ID,			TYPE_EDITSTRING,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON,	lane_link::kSourceID,			0,  lane_link::kTableName,  0, -1, FOLLOW_READONLY); //表示のみのため常に読み取り専用
		
		m_cControlRel.SetControl( IDC_STATIC_FROM_NODE_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_TO_NODE_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_GROUP_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_GROUP_C,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_ROAD_STRUCTURE_C,		TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_NUMBER,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_LANE_COUNT,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_MAX_LEGAL_SPEED_C,		TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_MIN_LEGAL_SPEED_C,		TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_TRAVEL_DIRECTION_C,	TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_CHANGE_F,			    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_PROTRUSION_F,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_ROAD_LINK_ID,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_SPECULATION_C,			TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_SOURCE_ID,			    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_BASE_GROUP,		    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_EXTENSION_GROUP,	    TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_VAR,			        TYPE_NONE,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_KIHON);
	
		m_cControlRel.SetControl( IDC_STATIC_NODELIST_INFO,		TYPE_NONE,	EDITVIEW_LANE_NODE, EDITVIEW_LANE_NODE);
		m_cControlRel.SetControl( IDC_GRID_NODELIST,			TYPE_NONE,	EDITVIEW_LANE_NODE, EDITVIEW_LANE_NODE);
	
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,	EDITVIEW_LANE_KIHON, EDITVIEW_LANE_NODE,	lane_link::kSource,				0,	lane_link::kTableName, 0, -1, FOLLOW_READONLY);
		
	}
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
*/
void CAttrLaneLinkDlg::SetToolTip(INT nTargetItem)
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

/**
 * エラーチェック
 * @TODO	
 *			
 */
BOOL CAttrLaneLinkDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == lane_link::kTableName )
		{
			//エラー処理
		}
	}

	return TRUE;
}

void CAttrLaneLinkDlg::UpdateLaneGroup()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	//セグメント属性取得
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return;
	}
	ITablePtr ipGroupTable = AheOpenSameOwnerTable(ipTable, lane_link_group::kTableName);
	if(!ipGroupTable)
	{
		return;
	}

	//車線グループIDからコード値取得
	long lane_group_oid = AheGetAttribute(ipFeature, lane_link::kLaneGroupID).lVal;
	CString where_clause;
	where_clause.Format(_T("%s=%d"), lane_link_group::kObjectID, lane_group_oid);
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());
	ipGroupTable->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	_IRowPtr ipRow;
	long lane_group = 1;
	while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
	{
		if(!ipRow)
			return;
		lane_group = AheGetAttribute(ipRow, lane_link_group::kGroupC).lVal;
	}
	//コード値から名称取得
	if(!SetDomain())
	{
		return;
	}
	CString str;
	if(lane_group_oid == UNDEF_OID)
	{
		str = UNDEF_NAME;
	}
	else
	{
		str = m_laneGroupDomain[lane_group];
	}

	//コントロール更新
	CEdit(GetDlgItem(IDC_EDIT_LANE_GROUP_C)).SetWindowText(str);
}

void CAttrLaneLinkDlg::GetRoadLinkID()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return;
	}
	ITablePtr ipGroupTable = AheOpenSameOwnerTable(ipTable, rel_road_link_lane_link::kTableName);
	if(!ipGroupTable)
	{
		return;
	}

	//グローバルIDと道路リンクGIDが一致するレコードを取得
	CString grobal_id;
	grobal_id.Format(_T("%s"),AheGetAttribute(ipFeature, nq_lane_node::kGlobalID).lVal);
	CString where_clause;
	where_clause.Format(_T("%s='%s'"), rel_road_link_lane_link::kLaneLinkGID, grobal_id);
	_ICursorPtr ipCursor;
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_WhereClause(where_clause.GetBuffer());
	ipGroupTable->Search(ipQuery, VARIANT_TRUE, &ipCursor);
	_IRowPtr ipRow;
	CString road_link_oid;
	while (ipCursor && ipCursor->NextRow(&ipRow) == S_OK)
	{
		if(!ipRow)
			return;
		road_link_oid.Format(_T("%d"),AheGetAttribute(ipRow, rel_road_link_lane_link::kRoadLinkID).lVal);
	}

	//コントロール更新
	CEdit(GetDlgItem(IDC_EDIT_ROAD_LINK_ID)).SetWindowText(road_link_oid);
}

bool CAttrLaneLinkDlg::SetDomain(bool update/* = false*/)
{
	//更新しない場合
	if(!update)
	{
		//設定済み(空でなければOK返す)
		if(!m_laneGroupDomain.empty())
		{
			return true;
		}
	}

	m_laneGroupDomain.clear();

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if(!ipFeature)
	{
		return false;
	}

	//車線グループテーブル接続
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if(!ipTable)
	{
		return false;
	}
	ITablePtr ipGroupTable = AheOpenSameOwnerTable(ipTable, lane_link_group::kTableName);
	if(!ipGroupTable)
	{
		return false;
	}

	//ドメイン値取得
	IDomainPtr ipDomain;
	long index = -1;
	ipGroupTable->FindField(const_cast<BSTR>(lane_link_group::kGroupC), &index);
	IFieldsPtr ipFields;
	ipGroupTable->get_Fields(&ipFields);
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
			m_laneGroupDomain[val.lVal] = CString(name);
		}
	}

	if(m_laneGroupDomain.empty())
	{
		return false;
	}

	return true;
}

void CAttrLaneLinkDlg::UpdateUnDefOID()
{
	//OIDがUNDEF_OIDなら表示名更新
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	if(!ipFeature)
	{
		return;
	}

	//車線グループID
	if(AheGetAttribute(ipFeature, lane_link::kLaneGroupID).lVal == UNDEF_OID)
	{
		//コントロール更新
		CEdit(GetDlgItem(IDC_EDIT_LANE_GROUP_C)).SetWindowText(UNDEF_NAME);
	}
}

// ウィンドウマネージャのマップの切り替えを行う
void CAttrLaneLinkDlg::ChangeWindowMgrMap( int iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_LANE_KIHON:		// 基本属性タブ
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_LANE_NODE:		// ノード列属性タブ
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
void CAttrLaneLinkDlg::createGrid()
{
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipNQTable = AheOpenSameOwnerTable(ipTable, nq_lane_node::kTableName);
	if (!ipNQTable) return;

	ipFeature->get_OID(&m_currentOid);

	// 始めてダイアログに表示したフィーチャであれば、ノード列グリッドを登録
	if (m_nqDataGridMap.count(m_currentOid) == 0)
	{
		TableDataGrid::Builder builder;
		builder.setTable(ipNQTable);
		builder.setGridHwnd(GetDlgItem(IDC_GRID_NODELIST));
		builder.addField(nq_lane_node::kObjectID)
			.addField(nq_lane_node::kSequence)
			.addField(nq_lane_node::kWidth)
			.addField(nq_lane_node::kLinearElement)
			.addField(nq_lane_node::kCurvature)
			.addField(nq_lane_node::kCurvatureRadius)
			.addField(nq_lane_node::kCurvatureChangeRate)
			.addField(nq_lane_node::kClothoidParameter)
			.addField(nq_lane_node::kLongitudinalSlope)
			.addField(nq_lane_node::kCrossSlope)
			.addField(nq_lane_node::kLaneNodeGID);

		TableDataGrid nqGrid = builder.build();
		m_nqDataGridMap[m_currentOid] = nqGrid;
	}

	// グリッド初期化
	m_nqDataGridMap[m_currentOid].init();

	// ノード列データをグリッドへ読み込む
	CComVariant laneLinkGid = AheGetAttribute(ipFeature, lane_link::kGlobalID);
	if (laneLinkGid.vt == VT_NULL) return;
	CString whereClause = uh::str_util::format(_T("%s='%s'"),
		nq_lane_node::kLaneLinkGID, laneLinkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].setLinkGID(laneLinkGid.bstrVal);

	m_nqDataGridMap[m_currentOid].select(whereClause);
	m_nqDataGridMap[m_currentOid].highlightChangedCell();
}

// Before Edit
void CAttrLaneLinkDlg::OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
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
void CAttrLaneLinkDlg::OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
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
		if(Button == 2) gridMenu( IDC_GRID_NODELIST, m_nqLaneNodeGrid, X, Y );
	}
}

// After Edit
void CAttrLaneLinkDlg::OnAfterEdit(LONG Row, LONG Col)
{
	m_nqDataGridMap[m_currentOid].onAfterEdit(Row, Col);

	updateChanged();
}

// 関連テーブルの更新情報の更新
void CAttrLaneLinkDlg::updateChanged()
{
	// 関連テーブル(ノード列)の変更状態更新
	CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
	pRelTableDef->setChanged(m_nqDataGridMap[m_currentOid].isChanged());

	// ボタン制御
	SetButton(Changed());
}

// 右クリックすると <削除> ポップアップ
void CAttrLaneLinkDlg::gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y )
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
			nq_lane_node::kObjectID, bstrValue);
		CString objectID = bstrValue.m_str;
		
		// 以下のif分は、新規レコードの場合、OIDをRowDataに入れて管理するための処理である
		if(bstrValue == _T(" "))
		{
			CComVariant vaValue;
			ipGrid->get_RowData( selectedRow, &vaValue );

			whereClause = uh::str_util::format(_T("%s=%d"),
			nq_lane_node::kObjectID, vaValue.lVal);

			objectID.Format(_T("%d"), vaValue.lVal);
		}

		ipGrid->RemoveItem(selectedRow);
		bool changeFlag = m_nqDataGridMap[m_currentOid].selectDelete(whereClause, objectID);
		m_nqDataGridMap[m_currentOid].highlightChangedCell();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
		pRelTableDef->setChanged(changeFlag);

		// ボタン制御
		SetButton(changeFlag);
	}
	else if(nResult == ID_ADD)
	{
		m_nqDataGridMap[m_currentOid].insert();

		CRelationalTableDef* pRelTableDef = GetCurrentFeatureDef()->GetRelationalTableDef(nq_lane_node::kTableName);
		pRelTableDef->setChanged(true);

		// ボタン制御
		SetButton(true);
	}
}

// Update Grid Data
bool CAttrLaneLinkDlg::Update()
{
	for (auto& featureDef : *m_pFeatureDefList)
	{
		if (_tcsicmp(featureDef.GetTableName(), lane_link::kTableName) != 0) continue;

		long oid(-1L);
		featureDef.GetFeature()->get_OID(&oid);

		if (!m_nqDataGridMap[oid].isChanged())	continue;

		if (!m_nqDataGridMap[oid].store())	return false;
	}
	return true;
}

// Check Data Changes
bool CAttrLaneLinkDlg::Changed()
{
	return ATTR_BASE_CLASS::Changed() || m_nqDataGridMap[m_currentOid].isChanged();
}

// get Point Geometry
void CAttrLaneLinkDlg::getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint)
{
	// ダイアログで表示している車線リンクフィーチャを取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if (!ipFeature) return;

	// グリッドで選択列のノード列レコードから、"順番","車線リンクGID"を取得する
	ITablePtr ipTable;
	ipFeature->get_Table(&ipTable);
	if (!ipTable) return;

	ITablePtr ipLaneNodeT = AheOpenSameOwnerTable(ipTable, lane_node::kTableName);
	if (!ipLaneNodeT) return;

	CString laneNodeGid, laneNodeGidNext;
	m_nqDataGridMap[m_currentOid].getSelectedNodeGid(laneNodeGid, laneNodeGidNext, nq_lane_node::kSequence, nq_lane_node::kLaneNodeGID);

	//   検索されたノード列の車線ノードGIDから次のノードを検索
	CString whereClause = uh::str_util::format(_T("%s='%s'"), lane_node::kGlobalID, laneNodeGid);

	_ICursorPtr ipLaneNodeCur;
	if (FAILED(ipLaneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipLaneNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	IFeaturePtr ipLaneNodeFeature;
	if (IFeatureCursorPtr(ipLaneNodeCur)->NextFeature(&ipLaneNodeFeature) != S_OK) return;
	if (!ipLaneNodeFeature) return;

	ipLaneNodeFeature->get_ShapeCopy(&ipFromPoint);

	whereClause = uh::str_util::format(_T("%s='%s'"), lane_node::kGlobalID, laneNodeGidNext);
	if (FAILED(ipLaneNodeT->Search(AheInitQueryFilter(nullptr, nullptr, whereClause), VARIANT_FALSE, &ipLaneNodeCur))) return;
	//   1レコードしか検索されないはずなので、その前提で処理する
	if (IFeatureCursorPtr(ipLaneNodeCur)->NextFeature(&ipLaneNodeFeature) != S_OK) return;
	if (!ipLaneNodeFeature) return;

	ipLaneNodeFeature->get_ShapeCopy(&ipToPoint);
}

// グリッドで選択しているノード列のラインをハイライトする
void CAttrLaneLinkDlg::highlightLine()
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

// 再描画イベント
LRESULT CAttrLaneLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	highlightLine();
	return 0;
}

// Tear down connection points
LRESULT CAttrLaneLinkDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	AtlAdviseSinkMap(this, false);

	//地図再描画
	m_cArcHelper.GetActiveView()->PartialRefresh(esriViewForeground, NULL, NULL);

	return 0;
}

// ノード列のジャンプ,中心へ移動
void CAttrLaneLinkDlg::moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint)
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
void CAttrLaneLinkDlg::readReg()
{
	CString value = uh::reg_util::read(EditableNQFileds, PathLaneLink);
	std::vector<CString> stringList = uh::str_util::split( value, _T("|") );
	// OBJECTID フィールドは指定されても変更不可
	stringList.erase(remove(stringList.begin(), stringList.end(), nq_lane_node::kObjectID), stringList.end());
	std::set<CString> editableFields( stringList.begin() , stringList.end());	

	m_nqDataGridMap[m_currentOid].enableEditing(editableFields);
}
