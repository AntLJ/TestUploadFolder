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

#include "stdafx.h"
#include "AttrWalkLinkDlg.h"
#include "LayoutMap.h"
#include "Map.h"
#include <WinLib/type_convert.h>

using namespace sindy;
using namespace sindy::schema;

/**
 * @brief コントロール背景色
 */
namespace ctrl_color{
	const COLORREF Modified = RGB(255,168,126);
}

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrWalkLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrWalkLinkDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrWalkLinkDlg::CAttrWalkLinkDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrWalkLinkDlg::CAttrWalkLinkDlg() 
	: ATTR_BASE_CLASS(WALKLINK_KIHON_MAP, IDD_ATTRWALKLINKDLG)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_WALKCLASS,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_WALKCLASS ) )
		( IDC_EDIT_ROADCODE,      CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ROADCODE ) )
		( IDC_BUTTON_SEARCHROADCODE, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHROADCODE ) )
		( IDC_CHECK_AUTOWALK,     CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_AUTOWALK ) )
		( IDC_CHECK_STAIR,        CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_STAIR ) )
		( IDC_CHECK_SLOPE,        CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_SLOPE ) )
		( IDC_CHECK_ESCALATOR,    CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_ESCALATOR ) )
		( IDC_CHECK_ARCADE,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_ARCADE ) )
		( IDC_CHECK_OPEN_AREA,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_OPEN_AREA ) )
		( IDC_CHECK_TUNNEL,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_TUNNEL ) )
		( IDC_CHECK_SCRAMBLE,       CContainedWindow(  _T("CHECKBOX"), this, IDC_CHECK_SCRAMBLE ) )
		( IDC_EDIT_FLOORLEVEL,    CContainedWindow(  _T("EDIT"), this, IDC_EDIT_FLOORLEVEL ) )
		( IDC_EDIT_PLATFORM,    CContainedWindow(  _T("EDIT"), this, IDC_EDIT_PLATFORM ) )
		( IDC_COMBO_FLOORMOVE,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_FLOORMOVE ) )
		( IDC_COMBO_MATERIAL,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_MATERIAL ) )
		( IDC_BUTTON_SEARCH,        CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCH ) )
		( IDC_EDIT_SEARCHRANGE,        CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SEARCHRANGE ) )
		( IDC_LIST_ROADSEARCH,        CContainedWindow(  _T("EDIT"), this, IDC_LIST_ROADSEARCH ) )
		( IDC_BUTTON_ROAD_REMOVE,        CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_ROAD_REMOVE ) )
		( IDC_BUTTON_ROAD_ACCEPT,        CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_ROAD_ACCEPT ) )
		( IDC_LIST_ROAD,        CContainedWindow(  _T("EDIT"), this, IDC_LIST_ROAD ) )
		( IDC_COMBO_ONEWAY,       CContainedWindow(  _T("COMBO"), this, IDC_COMBO_ONEWAY ) )
		( IDC_COMBO_NOPASSAGE,    CContainedWindow(  _T("COMBO"), this, IDC_COMBO_NOPASSAGE ) )
		( IDC_EDIT_SOURCE,        CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_TAB_WALKLINK,       CContainedWindow(  _T("TAB"), this, IDC_TAB_WALKLINK ) );

	// メンバ変数初期化
	m_lTabNum		= 0;
	m_iChangeType	= 0;
	m_lFeatureIndex = 0;
	m_lTableIndex	= 0;
	m_lRowIndex		= 0;
	m_bForce		= false;
	m_bEditable		= false;
	m_bIsLinkOnMesh = false;
}

/** 
 * @fn HWND CAttrWalkLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrWalkLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrWalkLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrWalkLinkDlg::Delete()
{
	// ハイライト消す
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, nullptr, nullptr);
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

void CAttrWalkLinkDlg::ClearFeatureDefs()
{
	// 紐付いてる道路を破棄（bug 12472）
	m_buddyRoads.ClearLinkMap();
	ATTR_BASE_CLASS::ClearFeatureDefs();
};

void CAttrWalkLinkDlg::SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList )
{
	// ↓でGetCurrentFeatureDef()される可能性があるので、先にセットしておく必要がある
	ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList );

	// 紐付いてる道路を取得
	m_buddyRoads.Init( pFeatureDefList, m_cArcHelper );
}

/**
 * @fn BOOL CAttrWalkLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 一通と通禁のコンボボックスをDISABLEにします。
 * 歩行者リンクが2次メッシュを跨いでいれば、対応するリンクを取得します。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */

BOOL CAttrWalkLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lTabNum, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// 道路名称を出す
	SetRoadName(IDC_EDIT_ROADCODE, IDC_STATIC_ROADNAME2);

	// [bug 5552] 2次メッシュ境界線を跨いでいる歩行者リンクの場合は反対側のリンクも同じ属性に
	m_cOverMeshLinks.clear();
	m_bIsLinkOnMesh = AheIsNetworkLinkCrossMesh( m_cArcHelper.GetMap(), GetCurrentFeatureDef()->GetFeature(), m_cOverMeshLinks );

	// 一通と通禁は編集できないようにDISABLEに
	SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
	SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );

	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable && (m_lTabNum == EDITVIEW_WALK_KIHON) )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_WALKCLASS) );
	}

	// 初期値
	m_cProperty.Init();
	GetDlgItem(IDC_EDIT_SEARCHRANGE).SetWindowText( uh::str_util::ToString(m_cProperty.GetSearchRange()) );

	// ダウンコンバートのためのコントロール表示・非表示
	SetControlState();
	InitBuddyRinks();

    return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrWalkLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 全タブ共通
		m_cControlRel.SetControl( IDC_STATIC_VAR,	TYPE_OTHER,			EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI );
		m_cControlRel.SetControl( IDC_TAB_WALKLINK,	TYPE_OTHER,			EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI );
		// 情報ソース
		m_cControlRel.SetControl( IDC_STATIC_SOURCE,	TYPE_OTHER,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,	TYPE_EDITSTRING,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KISEI,  walk_link::kSource,		NULL, walk_link::kTableName );

		// 基本タブ
		// 歩行者表示種別
		m_cControlRel.SetControl( IDC_STATIC_WALKCLASS,	TYPE_OTHER,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_WALKCLASS,	TYPE_COMBO,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kWalkClass,	NULL, walk_link::kTableName );
		// 路線コード
		m_cControlRel.SetControl( IDC_STATIC_ROADCODE,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_ROADCODE,		TYPE_EDIT,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kWalkCode,	NULL, walk_link::kTableName );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCHROADCODE,TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME2,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		// オートウォーク
		m_cControlRel.SetControl( IDC_CHECK_AUTOWALK,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kAutoWalkFlag,	NULL, walk_link::kTableName );
		// 階段フラグ
		m_cControlRel.SetControl( IDC_CHECK_STAIR,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kStairFlag,	NULL, walk_link::kTableName );
		// スロープ
		m_cControlRel.SetControl( IDC_CHECK_SLOPE,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kSlopeFlag,	NULL, walk_link::kTableName );
		// エスカレータ
		m_cControlRel.SetControl( IDC_CHECK_ESCALATOR,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kEscalatorFlag,	NULL, walk_link::kTableName );
		// 屋根付き
		m_cControlRel.SetControl( IDC_CHECK_ARCADE,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kArcadeFlag,	NULL, walk_link::kTableName );
		// 広場（bug 11190）
		m_cControlRel.SetControl( IDC_CHECK_OPEN_AREA,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kOpenAreaFlag,	NULL, walk_link::kTableName );
		// トンネル（bug 11292）
		m_cControlRel.SetControl( IDC_CHECK_TUNNEL,			TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kTunnelFlag,	NULL, walk_link::kTableName );
		// スクランブル交差点
		m_cControlRel.SetControl( IDC_CHECK_SCRAMBLE,		TYPE_CHECKBOX,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kScrambleFlag,	NULL, walk_link::kTableName );
		// 階層レベル
		m_cControlRel.SetControl( IDC_STATIC_FLOORLEVEL,	TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_FLOORLEVEL,		TYPE_EDITFLOAT,	EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kFloorLevel,	NULL, walk_link::kTableName );
		// のりば番号
		m_cControlRel.SetControl( IDC_STATIC_PLATFORM,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_PLATFORM,		TYPE_EDIT,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kPlatformNumber,NULL, walk_link::kTableName );
		// 階層移動種別
		m_cControlRel.SetControl( IDC_STATIC_FLOORMOVE,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_FLOORMOVE,		TYPE_COMBO,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kFloorMoveClass,	NULL, walk_link::kTableName );
		// 路面素材
		m_cControlRel.SetControl( IDC_STATIC_MATERIAL,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_COMBO_MATERIAL,		TYPE_COMBO,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON,  walk_link::kRoadSurface,	NULL, walk_link::kTableName );
		// 道路リンク
		m_cControlRel.SetControl( IDC_STATIC_ROADID,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_LIST_ROAD,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_LIST_ROADSEARCH,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_STATIC_SEARCHRANG,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_EDIT_SEARCHRANGE,		TYPE_OTHER,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_BUTTON_ROAD_ACCEPT,		TYPE_BUTTON,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_BUTTON_ROAD_REMOVE,		TYPE_BUTTON,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,		TYPE_BUTTON,		EDITVIEW_WALK_KIHON,	EDITVIEW_WALK_KIHON );
		
		// 規制タブ
		std::list<CString> array;
		// 一方通行
		m_cControlRel.SetControl( IDC_STATIC_ONEWAYGROUP,		TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERONEWAY,		TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_COMBO_ONEWAY,				TYPE_COMBO,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI, walk_link::kOneway, CHECK_DISPOTHER, walk_link::kTableName );
		array.clear();
		array.push_back( walk_oneway::kLinkDir );
		array.push_back( walk_oneway::kStartMonth );
		array.push_back( walk_oneway::kStartDay );
		array.push_back( walk_oneway::kStartHour );
		array.push_back( walk_oneway::kStartMin );
		array.push_back( walk_oneway::kEndMonth );
		array.push_back( walk_oneway::kEndDay );
		array.push_back( walk_oneway::kEndHour );
		array.push_back( walk_oneway::kEndMin );
		array.push_back( walk_oneway::kDayOfWeek );
		m_cControlRel.SetControl( IDC_GRID_ONEWAY,				TYPE_OWTIMEREG,	EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI, array, NULL, walk_oneway::kTableName, walk_oneway::kLinkID, IDC_COMBO_ONEWAY, FOLLOW_VALUE );

		// 通行止め
		m_cControlRel.SetControl( IDC_STATIC_NOPASSAGEGROUP,	TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERNOPASSAGE,	TYPE_OTHER,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI);
		m_cControlRel.SetControl( IDC_COMBO_NOPASSAGE,			TYPE_COMBO,		EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI,  walk_link::kNoPassage, CHECK_DISPOTHER, walk_link::kTableName );
		array.clear();
		array.push_back( walk_nopassage::kStartMonth );
		array.push_back( walk_nopassage::kStartDay );
		array.push_back( walk_nopassage::kStartHour );
		array.push_back( walk_nopassage::kStartMin );
		array.push_back( walk_nopassage::kEndMonth );
		array.push_back( walk_nopassage::kEndDay );
		array.push_back( walk_nopassage::kEndHour );
		array.push_back( walk_nopassage::kEndMin );
		array.push_back( walk_nopassage::kDayOfWeek );
		m_cControlRel.SetControl( IDC_GRID_NOPASSAGE,			TYPE_TIMEREG,	EDITVIEW_WALK_KISEI, EDITVIEW_WALK_KISEI, array, NULL, walk_nopassage::kTableName, walk_nopassage::kLinkID, IDC_COMBO_NOPASSAGE, FOLLOW_VALUE );
	}
}

//ダウンコンバートのコントロールの表示・非表示を判断する
void CAttrWalkLinkDlg::SetControlState()
{
	// 歩行者ダウンコンバートと汎用版の表示の切り替え
	int nCmdShow = SW_HIDE;
	// WINMGRに任せつつ、この関数で最終的に表示・非表示判定するため
	// タブ位置を判定にいれる(これをしないと規制の表示の時に以下で判定をする基本タブのものが表示されてしまう）
	if( m_lTabNum == EDITVIEW_WALK_KIHON && AheIsDownconvert( GetCurrentFeatureDef()->GetFeature() ) == walk_type::type_class::kGeneral )
		nCmdShow = SW_SHOW;

	::ShowWindow( GetDlgItem( IDC_STATIC_ROADID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_LIST_ROAD ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_ROAD_ACCEPT ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_ROAD_REMOVE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_LIST_ROADSEARCH ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_SEARCHRANG ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_SEARCHRANGE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCH ), nCmdShow );
}

// ウインドウマップ変更
void CAttrWalkLinkDlg::ChangeWindowMgrMap( INT nIndex )
{
	switch( nIndex )
	{
	case EDITVIEW_WALK_KIHON:	
		m_winMgr.ChangeMgrMap( WALKLINK_KIHON_MAP );
		break;
	case EDITVIEW_WALK_KISEI:
		m_winMgr.ChangeMgrMap( WALKLINK_KISEI_MAP );
		break;
	default:
		break;
	}

	// 再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);
}

// [bug 5552] 2次メッシュ境界線を跨いでいる歩行者リンクの場合は反対側のリンクも同じ属性に
void CAttrWalkLinkDlg::SyncChangedAttrToOverMeshLink( INT nId )
{
	if( m_bIsLinkOnMesh )
	{
		// 変更のあったコントロール
		CControlDef *pCtrlFeatureDef = m_cControlRel.GetControlDef( GetDlgItem(nId) );
		if( pCtrlFeatureDef )
		{
			// コントロールから値を取得
			CComVariant vaVal;
			pCtrlFeatureDef->GetControlValue( vaVal );

			// 選択されたリンクと2次メッシュを跨いで繋がってるリンクのリスト
			for( CTargetItems::const_iterator itOverMeshLink = m_cOverMeshLinks.begin(); itOverMeshLink != m_cOverMeshLinks.end(); itOverMeshLink++ )
			{
				// 2次メッシュを跨ぐリンクは、必ず属性ダイアログで拾っているはずなので、
				// 属性ダイアログが持っているフィーチャのリストから一致するものを探す
				for( std::list<CFeatureDef>::iterator itFeatures = m_pFeatureDefList->begin(); itFeatures != m_pFeatureDefList->end(); itFeatures++ )
				{
					if( AheIsSameFeature( itOverMeshLink->GetFeature(), itFeatures->GetFeature() ) )
					{
						// 属性ダイアログで保持しているリンクに変更を反映する
						CFieldDef *pFieldDef = itFeatures->GetFieldDef( pCtrlFeatureDef->GetTableName(), pCtrlFeatureDef->GetFieldName() );
						if( pFieldDef )
						{
							pFieldDef->SetNewValue( vaVal, m_hAttributeDlgWnd );
						}
					}
				}
			}
		}
	}
}

//  路線検索を行い、路線名を表示する
void CAttrWalkLinkDlg::SetRoadName(INT nCodeID, INT nLabelID )
{
	// 処理が途中で失敗した場合には、空文字列にする
	// (正常な環境で整備するなら、処理が失敗することはないのでメッセージ表記などのサポートは特にしない)
	GetDlgItem(nLabelID).SetWindowText( _T("") );

	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(GetCurrentFeatureDef()->GetFeature()), walk_code_list::kTableName);
	if( !ipTable ) return;

	// 路線番号コードをエディットボックスから取得
	CString strWalkCode;
	GetDlgItem( nCodeID ).GetWindowText( strWalkCode );

	// 取得した路線番号をテーブルから検索
	static CString strSubFields = uh::str_util::format( _T("%s,%s"),
		walk_code_list::kNameKanji, walk_code_list::kNameYomi  );
	
	_ICursorPtr ipCursor;
	ipTable->Search( 
		AheInitQueryFilter( nullptr, strSubFields, _T("%s=%s"), walk_code_list::kWalkCode, strWalkCode),
		VARIANT_FALSE, &ipCursor );

	if( !ipCursor )
		return;

	// 検索されるレコードは一つか、見つからないはず エラーチェックする必要もあるまい
	_IRowPtr ipRow;
	CString strKanjiName, strKanjiYomi;
	if( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		strKanjiName = AheGetAttribute( ipRow, walk_code_list::kNameKanji );

		// NAME_YOMIフィールドはNULL許可なので、NULLかどうか判定する
		CComVariant varKanjiYomi = AheGetAttribute( ipRow, walk_code_list::kNameYomi );
		// 文字列・数値が入っていた時は、CStringの=operatorが上手く処理してくれる
		// VARIANTがNULLのときには=operator内で落ちるので、NULLを表す文字列を入れる
		strKanjiYomi = (varKanjiYomi.vt==VT_NULL)? _T("<null>") : varKanjiYomi;
	}
	// 検索結果の文字列を路線名に書き込み
	CString strText = uh::str_util::format( _T("%s(%s)"), strKanjiName, strKanjiYomi );

	GetDlgItem(nLabelID).SetWindowText( strText );
}

// リストボックス更新
void UpdateListBox( HWND listBox, RoadList& roads )
{
	CListBox lb(listBox);
	lb.ResetContent();
	for( auto& road : roads )
	{
		// 削除されてるものは無視
		if( road.m_deleteF )
			continue;

		int idx = lb.AddString( uh::str_util::ToString( road.m_roadID ) );
		// リストボックスのアイテムに管理クラスを紐づけておく
		lb.SetItemDataPtr( idx, &road );
	}
}

// リストボックス選択アイテム削除
void RemoveSelectItem( HWND listBox )
{
	CListBox lb( listBox );
	int num = lb.GetCount();
	for( long i=0; i<num; ++i )
	{
		if( !lb.GetSel(i) )
			continue;

		// 紐付けておいたバディ情報を取り出して削除フラグを立てておく
		auto buddy = (CBuddyRoad*)lb.GetItemDataPtr(i);
		buddy->m_deleteF = true;
	}
}

// 道路紐付け初期処理
void CAttrWalkLinkDlg::InitBuddyRinks()
{
	// リストボックスに格納
	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	UpdateListBox( GetDlgItem( IDC_LIST_ROAD ), m_buddyRoads.GetBuddyRoads(walkID) );
	UpdateListBox(GetDlgItem(IDC_LIST_ROADSEARCH), m_buddyRoads.GetSearchedRoads());
	// ハイライトしておく(bug 11462)
	BOOL bHandled;
	OnRefresh( 0, 0, 0, bHandled );
}

// [検索] ボタン
LRESULT CAttrWalkLinkDlg::OnClickedArroundSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 検索範囲取得
	CString strRang;
	GetDlgItem( IDC_EDIT_SEARCHRANGE ).GetWindowText( strRang );
	long rang = _ttol( strRang );
	if( rang == 0 )
		return 0;

	// 周辺検索してリストボックスに表示
	auto walkLink = GetCurrentFeatureDef()->GetFeature();
	UpdateListBox( GetDlgItem( IDC_LIST_ROADSEARCH ), m_buddyRoads.ArroundSearch( walkLink, rang ) );

	return 0;
}

// [<] ボタン
LRESULT CAttrWalkLinkDlg::OnClickedAddBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ルールチェック（bug 11410）
	VARIANT_BOOL vb = VARIANT_FALSE;
	m_cArcHelper.GetRule()->IsCreatableLayer( CComBSTR(link_relation::kTableName), &vb );
	if( !vb )
	{
		MessageBox( AheLoadString( IDS_UNCREATIVE_LAYER ), AheLoadString( IDS_TOOLTIP ), MB_OK | MB_ICONERROR );
		return 0;
	}

	// 選択行を削除
	RemoveSelectItem( GetDlgItem( IDC_LIST_ROADSEARCH ) );

	// 検索済みから紐付けリンクに移動
	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	m_buddyRoads.AddFromSearchedRoads( walkID );

	// リストボックス更新
	UpdateListBox( GetDlgItem( IDC_LIST_ROAD ), m_buddyRoads.GetBuddyRoads( walkID ) );
	UpdateListBox( GetDlgItem( IDC_LIST_ROADSEARCH ), m_buddyRoads.GetSearchedRoads() );
	
	// リンクリレーションフラグ
	SetLinkRF();

	return 0;
}

// [>] ボタン
LRESULT CAttrWalkLinkDlg::OnClickedRemoveBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// ルールチェック（bug 11410）
	VARIANT_BOOL vb = VARIANT_FALSE;
	m_cArcHelper.GetRule()->IsDeletableLayer( CComBSTR(link_relation::kTableName), &vb );
	if( !vb )
	{
		MessageBox( AheLoadString( IDS_UNDELETABLE_LAYER ), AheLoadString( IDS_TOOLTIP ), MB_OK | MB_ICONERROR );
		return 0;
	}

	// 選択行を削除
	RemoveSelectItem( GetDlgItem( IDC_LIST_ROAD ) );

	// リストボックス更新
	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	UpdateListBox( GetDlgItem( IDC_LIST_ROAD ), m_buddyRoads.GetBuddyRoads( walkID ) );

	// リンクリレーションフラグ
	SetLinkRF();

	return 0;
}

// リンク紐付けテーブルフラグ設定
void CAttrWalkLinkDlg::SetLinkRF()
{
	auto walkLink = GetCurrentFeatureDef()->GetFeature();
	long walkID = AheGetOID( walkLink );
	BOOL buddyChange = bool2BOOL( m_buddyRoads.IsChanged(walkID) );

	// FieldDefのm_bChangedをでっち上げることによって親ダイアログを騙す
	// これによって、ツリーの変更色を制御してくれたり、OK後にUpdate()の処理が発行されるようになる
	// フィールドはなんでも良い、、、と思う
	auto fieldDef = GetCurrentFeatureDef()->GetFieldDef( walk_link::kTableName, walk_link::kModifyDate );
	fieldDef->m_bChanged = buddyChange;
	
	// リンクリレーションフラグを設定
	if( buddyChange )
	{
		AheSetAttribute( walkLink, walk_link::kLinkRelationRF, m_buddyRoads.HasBuddy( walkID ) ? 1L : 0L );
	}

	// 「OK」ボタン可否はWALK_LINK自身の属性変更も考慮する必要がある
	SetButton( ATTR_BASE_CLASS::Changed() || buddyChange);
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
}

// リストボックス背景色
LRESULT CAttrWalkLinkDlg::OnCtlColorListBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::GetDlgCtrlID( (HWND)lParam ) != IDC_LIST_ROAD )
		return 0;

	long walkID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );
	if( m_buddyRoads.IsChanged(walkID) )
	{
		static auto brush = CreateSolidBrush( ctrl_color::Modified );
		HDC hdc = (HDC)wParam;
		// 背景透過（TRANSPARENT）にすると、複数行エディットボックスでBackSpace
		// 押した時に描画がおかしくなる（文字が消されない）
		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, ctrl_color::Modified );
		return (DWORD)brush;
	}
	return 0;
}

// リスト行ダブルクリック
LRESULT CAttrWalkLinkDlg::OnListDblClk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (!m_cArcHelper.IsStartEditing())
	{
		return 0;
	}
	CButton btn;
	switch (wID)
	{
	case IDC_LIST_ROAD: 
		btn.Attach( GetDlgItem(IDC_BUTTON_ROAD_REMOVE) ); break;
	case IDC_LIST_ROADSEARCH:
		btn.Attach( GetDlgItem(IDC_BUTTON_ROAD_ACCEPT) ); break;
	default:
		return 0;
	}

	btn.Click();

	return 0;
}

// 選択行が変わった
LRESULT CAttrWalkLinkDlg::OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL);
	OnRefresh( 0, 0, 0, bHandled );
	return 0;
}

// 再描画
LRESULT CAttrWalkLinkDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	auto highliteLink = [&]( UINT listId, COLORREF color, bool selectedOnly )
	{
		CListBox lb( GetDlgItem( listId ) );
		int num = lb.GetCount();
		for( long i=0; i<num; ++i )
		{
			if( selectedOnly && !lb.GetSel(i) )
				continue;

			auto buddy = (CBuddyRoad*)lb.GetItemDataPtr(i);
			AheDrawPolyline(m_cArcHelper.GetDisplay(), buddy->m_geom, color, 5, esriSLSSolid, TRUE, TRUE);
		}
	};
	
	// Buddyは常にハイライトでリスト選択したら別の色（bug 11462）
	highliteLink( IDC_LIST_ROAD, RGB(0,0,255), false );
	highliteLink( IDC_LIST_ROAD, RGB(255,0,255), true );

	// 検索のほうはリスト選択されてるものだけ
	highliteLink( IDC_LIST_ROADSEARCH, RGB(0,255,0), true );

	return 0;
}

// OKボタンを押された後に呼び出される
BOOL CAttrWalkLinkDlg::ErrorCheck()
{
	// TODO: チェックが増えたらちゃんとどこかに分けよう

	// 階層移動種別整合性チェック（bug 11284）
	auto FloorMoveChk = [](CFeatureDef& featureDef){
		using namespace walk_link;

		// 階段とエスカレータは「上下なし」はNG
		bool isStair = (featureDef.GetFieldDef( kTableName, kStairFlag )->m_vaValue.lVal != 0);
		bool isEsclator = (featureDef.GetFieldDef( kTableName, kEscalatorFlag )->m_vaValue.lVal != 0);
		bool isNone = (featureDef.GetFieldDef( kTableName, kFloorMoveClass )->m_vaValue.lVal == floor_move_class::kNone );

		return !( (isStair && isNone) || (isEsclator && isNone) );
	};

	// エラーダイアログ表示
	auto ErrMsg = [&]( UINT stringID ){
		MessageBox( AheLoadString( stringID ), AheLoadString( IDS_CHECK_LOGIC ), MB_OK | MB_ICONERROR );
		return FALSE;
	};

	// ツリーにいる全ての歩行者リンクフィーチャが対象
	for( auto& featureDef : *m_pFeatureDefList )
	{
		if( _tcsicmp( featureDef.GetTableName(), walk_link::kTableName ) != 0 )
			continue;

		if( !FloorMoveChk( featureDef ) )
			return ErrMsg( IDS_FLOORMOVE_INTEGRATION );

	}

	UINT stringID;
	if( !m_buddyRoads.CheckLogic( stringID ) )
		return ErrMsg( stringID );

	return TRUE;
}

// 外部テーブル保存用
bool CAttrWalkLinkDlg::Update()
{
	return m_buddyRoads.UpdateLinkRelTable();
}
