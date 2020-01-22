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

// AttrWalkNodeDlg.cpp : DLL エクスポートの実装です。


#include "stdafx.h"
#include "AttrWalkNodeDlg.h"
#include <boost/assign/list_of.hpp>
#include "Map.h"
#include <TDC/useful_headers/str_util.h>

using namespace sindy;
using namespace sindy::schema;

/// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(WALKNODE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			// ノード種別
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_WALKNODECLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_WALKNODECLASS)
			ENDGROUP()
			RCSPACE(10)

			// 道路ノードID
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ROADNODEID)
				RCTOFIT(IDC_EDIT_ROADNODEID)
				RCTOFIT(IDC_STATIC_CROSSINGNAME)
				RCREST(IDC_STATIC_CROSSINGNAME2)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHROADNODE)
				RCTOFIT(IDC_DELETE_ROADNODEID)
			ENDGROUP()
			RCSPACE(10)

			// 駅出入口ID
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_REFID)
				RCTOFIT(IDC_EDIT_STATIONGATE)
				RCTOFIT(IDC_STATIC_GATESTATION)
				RCREST(IDC_STATIC_STATIONGATE_NAME)
				RCTOFIT(IDC_BUTTON_SEARCHSTATIONGATE)
				RCTOFIT(IDC_DELETE_STATIONGATEID)
			ENDGROUP()
			RCSPACE(10)

			// 地下街出入口ID
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_UNDERGROUNDGATENODEID)
				RCTOFIT(IDC_EDIT_UNDERGROUNDGATE)
				RCTOFIT(IDC_STATIC_UNDERGROUNDGATE)
				RCREST(IDC_STATIC_UNDERGROUNDGATE_NAME)
				RCTOFIT(-1)
				RCTOFIT(IDC_BUTTON_SEARCHUNDERGROUNDGATE)
				RCTOFIT(IDC_DELETE_UNDERGROUNDGATEID)
			ENDGROUP()
			RCSPACE(10)

			BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))
					RCSPACE(50)
					RCTOFIT(IDC_BUTTON_INSERT)
				ENDGROUP()
				BEGINROWS(WRCT_REST, 0, RCMARGINS(2,2))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
						RCTOFIT(IDC_SEARCH_RESULT_LABEL)
						RCTOFIT(IDC_STATIC_SEARCH)
						RCFIXED(IDC_EDIT_SEARCH,60)
						RCTOFIT(IDC_STATIC_SEARCH2)
					ENDGROUP()
					RCTOFIT(-1)
					RCSPACE(3)
					RCREST(IDC_LIST_RESULT)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

namespace
{
_TCHAR NULL_STRING[] = _T("(null)"); //! 名称が空の場合に表示する文字列

} // namespace

/// リストビューの各カラムの情報を定義
LVCOLUMN CAttrWalkNodeDlg::m_cResultColInfo[] =
{
	{LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, 50, _T("ＩＤ"), 0, 0, 0, 0 },
	{LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, 100, _T("Unknown"), 0, 0, 0, 0 }
};

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrWalkNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrWalkNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrWalkNodeDlg::CAttrWalkNodeDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrWalkNodeDlg::CAttrWalkNodeDlg() 
	: ATTR_BASE_CLASS(WALKNODE_MAP, IDD_ATTRWALKNODEDLG)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_COMBO_WALKNODECLASS,			CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_WALKNODECLASS ) )
		( IDC_EDIT_ROADNODEID,				CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_ROADNODEID ) )
		( IDC_BUTTON_SEARCHROADNODE,		CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHROADNODE ) )
		( IDC_EDIT_STATIONGATE,				CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_STATIONGATE ) )
		( IDC_EDIT_UNDERGROUNDGATE,			CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_UNDERGROUNDGATE ) )
		( IDC_EDIT_SEARCH,					CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SEARCH ) )
		( IDC_BUTTON_SEARCHSTATIONGATE,		CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHSTATIONGATE ) )
		( IDC_BUTTON_SEARCHUNDERGROUNDGATE,	CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHUNDERGROUNDGATE ) )
		( IDC_LIST_RESULT,					CContainedWindow(  _T("LIST"),   this, IDC_LIST_RESULT ) )
		( IDC_BUTTON_INSERT,				CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_INSERT ) )
		( IDC_EDIT_SOURCE,					CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SOURCE ) )
		( IDC_DELETE_ROADNODEID,			CContainedWindow(  _T("BUTTON"), this, IDC_DELETE_ROADNODEID ) )
		( IDC_DELETE_STATIONGATEID,			CContainedWindow(  _T("BUTTON"), this, IDC_DELETE_STATIONGATEID ) );
		( IDC_DELETE_UNDERGROUNDGATEID,		CContainedWindow(  _T("BUTTON"), this, IDC_DELETE_UNDERGROUNDGATEID ) );
}

/** 
 * @fn HWND CAttrWalkNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrWalkNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrWalkNodeDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrWalkNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrWalkNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 原稿番号の設定もしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */

BOOL CAttrWalkNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	using namespace sindy::schema;

    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 入力されている関連IDに合わせて表示する名称を変更
	SetIDName( station_gate_point::kTableName, IDC_EDIT_STATIONGATE, IDC_STATIC_STATIONGATE_NAME );
	SetIDName( road_node::kTableName, IDC_EDIT_ROADNODEID, IDC_STATIC_CROSSINGNAME2 );
	SetIDName( sindyk::poi_point_org::kTableName, IDC_EDIT_UNDERGROUNDGATE, IDC_STATIC_UNDERGROUNDGATE_NAME );

	// 不正な入力を検出するための要素を初期化
	m_searchedLayer[GetCurrentFeatureDef()->GetFeature()] = nullptr;
	m_errors[GetCurrentFeatureDef()->GetFeature()].Reset();

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_COMBO_WALKNODECLASS) );

		// コントロールの有効・無効を制御する
		// ノード種別によって各フィールドの編集可否が異なる
		SetControlState(true);
	}
	else
		SetControlState(false);

	return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrWalkNodeDlg::CreateControlRelation()
{
	using namespace  sindy::schema;

	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else
	{
		m_cControlRel.SetHWND( m_hWnd );

		// 歩道種別
		m_cControlRel.SetControl( IDC_COMBO_WALKNODECLASS,	TYPE_COMBO,	0,	0,  walk_node::kNodeClass,	NULL, walk_node::kTableName );
		// 道路ノード対応ID
		m_cControlRel.SetControl( IDC_EDIT_ROADNODEID,		TYPE_EDIT,	0,	0,  walk_node::kRoadNodeID,	NULL, walk_node::kTableName );
		// 駅出入口ポイントID
		m_cControlRel.SetControl( IDC_EDIT_STATIONGATE,		TYPE_EDIT,	0,	0,  walk_node::kStationGatePointID,	NULL, walk_node::kTableName );
		// [bug 11559][e][SJ歩行者]歩行者ノードと地下街出入口POIの紐づけを出来るようにしてほしい
		// 地下街出入口ポイントID
		m_cControlRel.SetControl( IDC_EDIT_UNDERGROUNDGATE,	TYPE_EDIT,	0,	0,  walk_node::kUndergroundGatePointID,	NULL, walk_node::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0,	0,  walk_node::kSource,		NULL, walk_node::kTableName );
	}
}

/**
 * @fn void CAttrBaseRailwayDlg::SetDispButton()
 * @brief コントロールの有効・無効を判断する
 *
 */
void CAttrWalkNodeDlg::SetControlState(bool bEditable)
{
	// 一旦、全てのコントロールをFALSEにする
	::EnableWindow( GetDlgItem( IDC_COMBO_WALKNODECLASS ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_INSERT ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_ROADNODEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_DELETE_ROADNODEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHROADNODE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_SEARCH ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_STATIONGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_DELETE_STATIONGATEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHSTATIONGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_UNDERGROUNDGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_DELETE_UNDERGROUNDGATEID ), FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHUNDERGROUNDGATE ), FALSE );
	::EnableWindow( GetDlgItem( IDC_LIST_RESULT ), FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_SOURCE ), FALSE );

	// リストビューのアイテム・カラムもすべて削除
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	lv.DeleteAllItems();
	lv.DeleteColumn(0);
	lv.DeleteColumn(0);

	// 歩行者ダウンコンバートと汎用版の表示の切り替え
	int nCmdShow = SW_HIDE;
	if( AheIsDownconvert( GetCurrentFeatureDef()->GetFeature() ) == walk_type::type_class::kGeneral )
		nCmdShow = SW_SHOW;

	::ShowWindow( GetDlgItem( IDC_STATIC_ROADNODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_ROADNODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_CROSSINGNAME ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_CROSSINGNAME2 ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCHROADNODE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_DELETE_ROADNODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_REFID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_STATIONGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_GATESTATION ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_STATIONGATE_NAME ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCHSTATIONGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_DELETE_STATIONGATEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_UNDERGROUNDGATENODEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_UNDERGROUNDGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_UNDERGROUNDGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_UNDERGROUNDGATE_NAME ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCHUNDERGROUNDGATE ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_DELETE_UNDERGROUNDGATEID ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_BUTTON_INSERT ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_SEARCH_RESULT_LABEL ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_SEARCH ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_EDIT_SEARCH ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_STATIC_SEARCH2 ), nCmdShow );
	::ShowWindow( GetDlgItem( IDC_LIST_RESULT ), nCmdShow );

	// 不正な入力データがあればエラーフラグを立てる
	CheckInvaliedValue();

	// 編集可能状態でないなら以下の処理は行わない
	if( !bEditable ) return;

	// 地下街出入口対応IDフィールドが存在するかチェック
	bool existUndergroundGatePointIDField = CheckExistField( walk_node::kTableName, walk_node::kUndergroundGatePointID );

	// 情報ソースとコンボボックスは全ての種別で変更可能
	::EnableWindow( GetDlgItem( IDC_COMBO_WALKNODECLASS ), TRUE );
	::EnableWindow( GetDlgItem(IDC_EDIT_SOURCE), TRUE );
	// 消すだけならどの種別でも可能
	::EnableWindow( GetDlgItem( IDC_DELETE_ROADNODEID ), TRUE );
	::EnableWindow( GetDlgItem( IDC_DELETE_STATIONGATEID ), TRUE );
	// フィールドが存在した場合のみ可能
	if( existUndergroundGatePointIDField )
		::EnableWindow( GetDlgItem( IDC_DELETE_UNDERGROUNDGATEID ), TRUE );

	// 現在のノード種別を取得
	CComboBox combo = GetDlgItem(IDC_COMBO_WALKNODECLASS);
	DWORD_PTR nodeClass = combo.GetItemData( combo.GetCurSel() );

	// 以下、種別固有のコントロール許可処理
	// ノード種別が特定のものなら、インサートは許可しない
	switch(nodeClass)
	{
	// TODO: 特定種別でエディット系のコントロールの入力許可をしないなら、ここに種別を追加する
	case walk_node::node_class::kOutline: // 図郭上ノード
		break;
	default:
		// リスト表示・インサート・交差点検索関連は上記種別以外なら入力可能
		::EnableWindow( GetDlgItem( IDC_BUTTON_INSERT ), TRUE );
		::EnableWindow( GetDlgItem( IDC_EDIT_SEARCH ), TRUE );
		::EnableWindow( GetDlgItem( IDC_LIST_RESULT ), TRUE );
		::EnableWindow( GetDlgItem( IDC_EDIT_ROADNODEID ), TRUE );
		::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHROADNODE ), TRUE );

		// 更に検索リストビューの作成の仕方は、種別で分ける
		switch(nodeClass)
		{
		case walk_node::node_class::kGate:   /// 駅出入口ノード
			// 駅出入口なら駅出入口用のリストビューを作成し、検索ボタン等のコントロールも許可
			::EnableWindow( GetDlgItem( IDC_EDIT_STATIONGATE ), TRUE );
			::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHSTATIONGATE ), TRUE );
			// Msg 駅出入口名称, 駅出入口周辺検索結果
			CreateListView(AheLoadString(IDS_STATIONGATECOLUMN), AheLoadString(IDS_STATIONGATERESULT));
			break;
		// [bug 11559][e][SJ歩行者]歩行者ノードと地下街出入口POIの紐づけを出来るようにしてほしい
		case walk_node::node_class::kUndergroundGate:   /// 地下街出入口ノード
			// 対象フィールドが存在する場合のみ可能
			if( existUndergroundGatePointIDField )
			{
				// 地下街出入口なら地下街出入口用のリストビューを作成し、検索ボタン等のコントロールも許可
				::EnableWindow( GetDlgItem( IDC_EDIT_UNDERGROUNDGATE ), TRUE );
				::EnableWindow( GetDlgItem( IDC_BUTTON_SEARCHUNDERGROUNDGATE ), TRUE );
				// Msg 地下街出入口名称, 地下街出入口周辺検索結果
				CreateListView(AheLoadString(IDS_UNDERGROUNDGATECOLUMN), AheLoadString(IDS_UNDERGROUNDGATERESULT));
				break;
			}
			// 対象フィールドが存在しない場合はdefault処理
		default:
			// 基本的に交差点検索用のリストビューを作成する
			// Msg 交差点名称, 交差点周辺検索結果
			CreateListView(AheLoadString(IDS_CROSSNODECOLUMN), AheLoadString(IDS_CROSSRESULT));
			break;
		}
		break;
	}
}

BOOL CAttrWalkNodeDlg::ErrorCheck()
{
	for(auto& elem : m_errors )
	{
		// エラーがないなら何もしない
		if(!elem.second.IsError()) continue;

		// エラーがあるならエラー文を表示
		CString errMsg; //!< 表示するエラーメッセージを格納

		// オブジェクトIDと定型句を格納
		// Msg:%s [%ld] に下記のエラーがあります。
		errMsg.Format(AheLoadString( IDS_ERRORID ), ipc_feature::kObjectID, AheGetOID(elem.first));

		// 入力されたエラーの一覧を取得
		auto errorList = elem.second.GetStringList();
		for(auto& msg : errorList)
		{
			// 改行して追記
			errMsg.Append( _T("\n")+msg );
		}

		// 整形したエラー文を表示
		AttrMessageBox( errMsg, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return FALSE;
	}
	return TRUE;
}

IFeatureClassPtr CAttrWalkNodeDlg::GetFeatureClass( LPCTSTR fcName , bool popupMsgTOCHide/*=true*/ )
{
	bool bCancel = false;
	IFeatureClassPtr ipFC = GetSameGroupFeatureClass(fcName, GetCurrentFeatureDef()->GetFeature(), true, bCancel, popupMsgTOCHide);
	// キャンセルされた場合は特にメッセージは表示しない
	if( bCancel ) return ipFC;
	if( popupMsgTOCHide && !ipFC )
	{
		CString strMsg;
		// Msg TOCに%sがないため関連付けができません。
		strMsg.Format(AheLoadString( IDS_NOROADNODECLASS ), fcName);
		AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
	}
	return ipFC;
}

IFeatureClassPtr CAttrWalkNodeDlg::GetSameGroupFeatureClass( LPCTSTR targetName, const IFeaturePtr& ipSameGroupFeature, bool bMsgDisp, bool& bCancel, bool popupMsgTOCHide/*=true*/ )
{
	bCancel = false; //!< 処理がユーザーの入力によって中断されたらtrueにする

	bool bSameGroup = false; //!< 同じグループ内に存在するか否か

	// 検索の起点とするフィーチャクラスのワークスペースなどを取得
	IFeatureClassPtr ipGroupClass = AheGetFeatureClass( ipSameGroupFeature );
	IWorkspacePtr ipWorkspace = AheGetWorkspace( ipGroupClass );
	CString strOwnerName = AheGetFeatureClassOwnerName( ipGroupClass );
	CString strSameGroupFCName = AheGetFeatureClassName( ipSameGroupFeature );

	// TOCから検索の起点とするレイヤを取得
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ILayerPtr ipSameGroupLayer = cMap.FindLayer( ipWorkspace, strOwnerName, strSameGroupFCName );
	if( !ipSameGroupLayer )
		return nullptr;

	// 検索の起点となるレイヤが収納されている親のグループレイヤを取得する
	ICompositeLayerPtr ipWalkCompositeLayer = cMap.GetParentCompositeLayer( ipSameGroupLayer );

	std::list<CAdapt<ILayerPtr>> targetLayers; //!< グループ内から検索された対象フィーチャクラスを格納

	// 起点がグループレイヤに入っていない場合には、TOCの上から探索して最初に見つかった
	// レイヤを取得対象とする
	if( !ipWalkCompositeLayer )
	{
		targetLayers = std::move(cMap.FindLayers( nullptr, nullptr, targetName ));
	}
	// 起点がグループレイヤの中にいれば、そのグループレイヤ内にある対象フィーチャが取得対象
	else
	{
		long lLayerCount = 0;
		ipWalkCompositeLayer->get_Count( &lLayerCount );
		for( int i = 0; i < lLayerCount; ++i )
		{
			ILayerPtr ipCheckLayer;
			ipWalkCompositeLayer->get_Layer( i, &ipCheckLayer );
			CString strTargetFC = AheGetFeatureClassName( ipCheckLayer );
			if( 0 == strTargetFC.CompareNoCase( targetName ) )
				targetLayers.push_back(ipCheckLayer);
		}

		// グループ内に対象フィーチャが見つからなかったらTOC上から検索する
		if( targetLayers.empty() )
			targetLayers = std::move(cMap.FindLayers( nullptr, nullptr, targetName ));
		// グループ内から取得できていたらフラグを立てておく
		else
			bSameGroup = true;
	}

	// TOC上に対象フィーチャが見つからなかったら終了
	if( targetLayers.empty() )
		return nullptr;

	ILayerPtr ipTargetLayer; //!< 最終的に返すレイヤを格納

	// 対象の内、表示されているレイヤの数をカウント
	int visibleLayerCount = 0;
	bool bInitVisible = true; //!< 表示されているフィーチャを一つでも見つけたか否かのフラグ
	for( auto& elem : targetLayers )
	{
		VARIANT_BOOL bVisible = VARIANT_FALSE;
		elem->get_Visible(&bVisible);
		if(bVisible)
		{
			++visibleLayerCount;
			if(bInitVisible)
			{
				ipTargetLayer = elem.m_T;
				bInitVisible = false;
			}
		}
	}

	bool bMessage = false; //!< メッセージ表示フラグ
	// 表示レイヤの数によって処理を変える
	switch(visibleLayerCount)
	{
	case 0: // 非表示レイヤしか存在しない
		// [bug 11557][e][SJ歩行者]歩行者ノード表示時に警告を出さないでほしい
		if(bMsgDisp && popupMsgTOCHide )
		{
			CString strMsg;

			// グループ内のレイヤか否かで若干メッセージを変更
			// ...重複する部分が多いが、英語化を考えると分離はしない方が置換えは楽
			if(bSameGroup)
				// Msg グループ内の%sが全て非表示です。\n非表示レイヤとIDの関連付けを行いますか？
				strMsg.Format(AheLoadString(IDS_INVISIBLEGROUP), targetName);
			else
				// Msg TOC上の%sが全て非表示です。\n非表示レイヤとIDの関連付けを行いますか？
				strMsg.Format(AheLoadString(IDS_INVISIBLETOC), targetName);

			// Noが選択されたら処理を中断
			if(IDNO==AttrMessageBox(strMsg, AheLoadString( IDS_CAPTION ), MB_YESNO, NULL))
			{
				bCancel = true;
				return nullptr;
			}
		}
		// 非表示の内、最初に検索されたものを対象とする
		ipTargetLayer = targetLayers.begin()->m_T;
		// 候補が複数個あったなら、メッセージを出す
		if(targetLayers.size()>1)
			bMessage = true;
		break;
	case 1: // 表示レイヤが一つならipTargetLayerに格納されているものをそのまま対象とする
		break;
	default: // 表示レイヤが複数ならメッセージを出す
		bMessage = true;
		break;
	}

	// 候補が複数見つかった場合はメッセージを出す
	if( bMessage && bMsgDisp )
	{
		CString strMsg;

		// グループ内のレイヤか否かで若干メッセージを変更
		if(bSameGroup)
			// Msg グループ内に複数(%d個)の%sが見つかったため、一番初めに見つかった下記を関連付けの対象とします。
			strMsg.Format(AheLoadString(IDS_FINDPLURALINSAMEGROUP), targetLayers.size(), targetName);
		else
			// Msg 複数(%d個)の%sが見つかったため、一番初めに見つかった下記を関連付けの対象とします。
			strMsg.Format(AheLoadString(IDS_FINDPLURALTOC), targetLayers.size(), targetName);

		// 以下、オーナー名.フィーチャクラス名も表示
		strMsg.AppendFormat(_T("\n"));
		CString ownerName = AheGetFeatureClassOwnerName(ipTargetLayer);
		if(!ownerName.IsEmpty())
			strMsg.AppendFormat(_T("%s."), ownerName);
		strMsg.AppendFormat(_T("%s"), targetName);

		AttrMessageBox(strMsg, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
	}
	return AheGetFeatureClass(ipTargetLayer);
}

void CAttrWalkNodeDlg::SetIDName(LPCTSTR fcName, INT nCodeID, INT nLabelID )
{
	// この処理ではTOC非表示のエラーメッセージを出力しない
	IFeatureClassPtr ipFC = GetFeatureClass(fcName, false );
	if( !ipFC ) return;

	// 検索するコードをエディットボックスから取得
	CString strID;
	GetDlgItem( nCodeID ).GetWindowText( strID );
	// 何も入力されていないなら空文字列をセット
	if( strID.IsEmpty() )
	{
		GetDlgItem(nLabelID).SetWindowText( _T("") );
		return;
	}

	IFeaturePtr ipFeature;
	ipFC->GetFeature( _ttol( strID ), &ipFeature );
	// そのOIDを持つフィーチャが存在しないなら"存在しないOID"をセット
	if( !ipFeature )
	{
		// Msg 存在しないOID
		GetDlgItem(nLabelID).SetWindowText( AheLoadString(IDS_NONEXISTID) );
		return;
	}

	// 名称と読みを取得
	LONG lNameIndex = -1, lYomiIndex = -1;
	IFieldsPtr ipFields;
	ipFeature->get_Fields( &ipFields );
	if(0==CString(fcName).CompareNoCase(road_node::kTableName))
	{
		ipFields->FindField( CComBSTR(road_node::kNameKanji), &lNameIndex );
		ipFields->FindField( CComBSTR(road_node::kNameYomi), &lYomiIndex );
	}
	else if(0==CString(fcName).CompareNoCase(station_gate_point::kTableName))
	{
		ipFields->FindField( CComBSTR(station_gate_point::kName), &lNameIndex );
		ipFields->FindField( CComBSTR(station_gate_point::kYomi), &lYomiIndex );
	}
	else if(0==CString(fcName).CompareNoCase(sindyk::poi_point_org::kTableName))
	{
		ipFields->FindField( CComBSTR(sindyk::poi_point_org::kName), &lNameIndex );
		ipFields->FindField( CComBSTR(sindyk::poi_point_org::kYomi), &lYomiIndex );

		// POI_POINT_ORGの場合コンテンツコードが地下街出入口ポイントでない場合
		// "地下街出入口ポイント以外のOID"をセット
		CComVariant varContentsCode = AheGetAttribute(ipFeature, sindyk::poi_point_org::kContentsCode);
		if( UNDERGROUND_GATE_CONTENTS_CODE != varContentsCode.lVal )
		{
			GetDlgItem(nLabelID).SetWindowText( AheLoadString(IDS_NO_UNDERGROUNDGATE_CONTENTS) );
			return;
		}
	}

	// 名称を取得する
	CComVariant varKanjiName = AheGetAttribute(ipFeature, lNameIndex);
	CComVariant varKanjiYomi = AheGetAttribute(ipFeature, lYomiIndex);
	// 表示用に名称と読みを取得
	CString strKnajiName = gf::VariantToCString( varKanjiName, NULL_STRING );
	CString strKanjiYomi = gf::VariantToCString( varKanjiYomi, NULL_STRING );

	// 検索結果の文字列を名称フィールドに出力
	GetDlgItem(nLabelID).SetWindowText(
		uh::str_util::format(_T("%s(%s)"), strKnajiName, strKanjiYomi) );
}

void CAttrWalkNodeDlg::InsertToEditBox(int nCurIdx)
{
	IFeatureClassPtr& ipFC = m_searchedLayer[GetCurrentFeatureDef()->GetFeature()];
	// nullptr=リストビューに何も表示されていない状態なので、何もしない
	if(!ipFC) return;

	// ipFCが取得できたならフィーチャクラス名を取得
	CString fcName = AheGetFeatureClassName(ipFC);

	// リストビューで表示しているフィーチャクラスによって出力先を変更
	INT nEditID = 0; //!< IDの出力先となるエディットボックスのCtrlID
	INT nNameID = 0; //!< 名称の出力先となるスタティックテキストのCtrlID
	// STATION_GATE_POINTか？
	if(0==fcName.CompareNoCase(station_gate_point::kTableName))
	{
		nEditID = IDC_EDIT_STATIONGATE;
		nNameID = IDC_STATIC_STATIONGATE_NAME;
	}
	// [bug 11559][e][SJ歩行者]歩行者ノードと地下街出入口POIの紐づけを出来るようにしてほしい
	// UNDERGROUND_GATE_POINTか？
	else if(0==fcName.CompareNoCase( sindyk::poi_point_org::kTableName ))
	{
		nEditID = IDC_EDIT_UNDERGROUNDGATE;
		nNameID = IDC_STATIC_UNDERGROUNDGATE_NAME;
	}
	// ROAD_NODEか？
	else if(0==fcName.CompareNoCase(road_node::kTableName))
	{
		nEditID = IDC_EDIT_ROADNODEID;
		nNameID = IDC_STATIC_CROSSINGNAME2;
	}
	else // それ以外の場合は対応していないので何もしない
	{
		// TODO:ここに来るということは、（現時点での仕様から考えると）何かしらの改修途上であるということ
		// 改修内容に合わせて上記条件文を適切に変更する必要がある
		// 改修が正常であるなら、ここに入ることもないので、ツール使用者用のエラー文は出力しない
		// (余計な英語化対象を増やしたくない)
		ASSERT(false);
		return;
	}

	// すでに関連付けられているノードIDを取得
	CString nowID;
	GetDlgItem( nEditID ).GetWindowText( nowID );

	// リストからIDを取得
	CString strID;
	CListViewCtrl lvResult( GetDlgItem(IDC_LIST_RESULT) );
	lvResult.GetItemText( nCurIdx, 0, strID );

	// すでに関連付けられているIDに対して 入力されたなら何もしない
	// この条件を入れないと値が変わっていないのに属性変更あつかいになる
	if( 0==strID.Compare(nowID) ) return;

	// 名称を取得
	CString strName;
	lvResult.GetItemText( nCurIdx, 1, strName );

	// エディットボックスとスタティックテキストに書き込み
	SetDlgItemText( nEditID, strID );
	SetDlgItemText( nNameID, strName );

	// 不正な入力データがあればエラーフラグを立てる
	CheckInvaliedValue();
}

void CAttrWalkNodeDlg::ReplaceListViewNameColumn(UINT colNameID, UINT resultID)
{
	// カラム列名を切り変える
	// 参考：https://msdn.microsoft.com/ja-jp/library/b11bety8.aspx
	// pszTextには、SetColumnを呼び出すまで生存している文字列を渡す必要がある
	// 例えば、const_cast<LPTSTR>(AheLoadString(IDC_HOGE));のようなものは渡せないので注意
	LVCOLUMN nameColumn = m_cResultColInfo[1];
	nameColumn.mask = LVCF_TEXT; // テキストの情報だけ変更するのでマスクをかける
	CString strColumnName = AheLoadString(colNameID);
	nameColumn.pszText = const_cast<LPTSTR>(strColumnName.GetString());

	// GetColumnのときはcchTextMaxメンバ変数を設定する必要があるが、SetColumnのときは無視される
	// (pszTextがnullで終わる文字列(いわゆるnull-terminated string)であることが前提)
	CListViewCtrl( GetDlgItem(IDC_LIST_RESULT) ).SetColumn(1, &nameColumn);

	// 検索結果表示も切り替える
	SetDlgItemText(IDC_SEARCH_RESULT_LABEL, AheLoadString(resultID));
}

void CAttrWalkNodeDlg::SearchAroundPoint(LPCTSTR fcName, LPCTSTR nameFieldName, LPCTSTR yomiFieldName, LPCTSTR whereClause/*=NULL*/ )
{
	// エラーメッセージは関数内で出す
	IFeatureClassPtr ipFC = GetFeatureClass(fcName);
	if( !ipFC ) return;

	// エディットボックスから検索範囲を取得
	CString rangeValue;
	GetDlgItem( IDC_EDIT_SEARCH ).GetWindowText(rangeValue);

	long meter = 0;
	if(!rangeValue.IsEmpty()) // 空でないなら数値に変換
		meter = _ttol( rangeValue );
	if(meter < 0) meter = 0;

	// 周辺 meter メートル以内のフィーチャを検索
	IFeatureCursorPtr ipCur = gf::AroundSearch( (IFeaturePtr)(GetCurrentFeatureDef()->GetFeature()), ipFC, meter, whereClause );

	// 値を取得するフィールドのインデックス番号を取得しておく
	IFieldsPtr ipFields;
	ipFC->get_Fields( &ipFields );
	LONG lKanjiNameIndex = -1, lKanjiYomiIndex = -1;
	ipFields->FindField( CComBSTR(nameFieldName), &lKanjiNameIndex );
	ipFields->FindField( CComBSTR(yomiFieldName), &lKanjiYomiIndex );

	// 距離計算用に、編集中のフィーチャの形状を取得
	IGeometryPtr ipCurrentGeom;
	((IFeaturePtr)(GetCurrentFeatureDef()->GetFeature()))->get_ShapeCopy(&ipCurrentGeom);
	IPointPtr ipCurrentPoint(ipCurrentGeom);

	// ソート用のmultimapを用意
	// 距離順で並べ替えるため、doubleをkeyとする <距離,<オブジェクトID,名称(読み)>>
	// 同距離のものについては見つかった順になる
	std::multimap<double,std::pair<long,CString>> sortMap;
	IFeaturePtr ipFeature;
	while( S_OK == ipCur->NextFeature(&ipFeature) && ipFeature )
	{
		// 名称を取得
		CComVariant varKanji = AheGetAttribute(ipFeature, lKanjiNameIndex);
		// VARIANT型をCStringに変換
		CString strKanjiName = gf::VariantToCString( varKanji, NULL_STRING );

		// 名称を取得できないもの・名称が空のものは対応付けの対象にはなり得ないのでリストに追加しない
		// (駅出入り口はNULL不許可だが、交差点ノードには必ずしも名称が存在しないのでここで弾く)
		if( 0==strKanjiName.CompareNoCase(NULL_STRING) || strKanjiName.IsEmpty() )
			continue;

		// 形状を取得し、対象フィーチャからの距離を取得する
		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy(&ipGeom);
		IPointPtr ipPoint(ipGeom);

		// 編集中のフィーチャからの距離を取得(距離順にソートするために使用)
		double dRange = AheGetMeterLength(ipPoint, ipCurrentPoint);
		// TODO: gf::AroundSearch()が修正された場合、合わせて修正する
		// 現状、検索結果には指定範囲外のフィーチャも含まれてしまうため、ここで指定範囲外のものは除外する
		if( dRange > meter ) continue;

		// 読みを取得
		CComVariant varYomi = AheGetAttribute(ipFeature, lKanjiYomiIndex);
		// リスト上に表示する読みを取得
		CString strYomiName = gf::VariantToCString( varYomi, NULL_STRING );
		// リスト表示用に名称・読みを整形
		CString strListYomi = uh::str_util::format(_T("%s(%s)"), strKanjiName, strYomiName);
		// マルチマップへ挿入
		sortMap.insert( std::make_pair(dRange, std::make_pair(AheGetOID( ipFeature ), strListYomi)));
	}

	// マップに格納したものをリストビューに格納
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	int count = 0; //!< リストビューの行数を記録(リストビューの末尾に追加するのに必要)
	lv.DeleteAllItems();
	for(const auto& elem : sortMap)
	{
		// ID列にはオブジェクトIDを表示
		lv.AddItem( count, 0, uh::str_util::ToString(elem.second.first) );
		// 名前列には{漢字名称(読み)}のように表示する
		lv.AddItem( count, 1, elem.second.second );
		++count;
	}

	// List上に表示されているアイテムがどこのフィーチャークラスのものなのかを保持しておく
	m_searchedLayer[GetCurrentFeatureDef()->GetFeature()] = ipFC;

	if(!sortMap.empty())
	{
		// 一番始めに見つかったものはエディットボックスに入力する
		InsertToEditBox(0);
	}
}

void CAttrWalkNodeDlg::BlinkPointFeature()
{
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	// 選択中のレコードのオブジェクトIDを取得
	int nCurIdx = lv.GetSelectedIndex();
	if( -1 == nCurIdx ) return;

	// 検索ボタンが押されたときに参照したフィーチャクラスの参照を保持
	const IFeatureClassPtr& ipSearchedFC = m_searchedLayer[GetCurrentFeatureDef()->GetFeature()];

	// 検索ボタンが押されたときに参照したものがTOCから取得できたかどうかを判定
	CString targetFCName = AheGetFeatureClassName(ipSearchedFC);

	// エラーメッセージは関数内で出す
	IFeatureClassPtr ipNowTargetFC = GetFeatureClass(targetFCName);
	if( !ipNowTargetFC ) return;

	if(ipSearchedFC!=ipNowTargetFC)
	{
		// Msg TOCの構成が変更されています。検索をやり直してください。
		AttrMessageBox( AheLoadString(IDS_ONEMORESEARCH), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
		return;
	}

	CString strID;
	lv.GetItemText( nCurIdx, 0, strID );

	// クリックされたフィーチャを点滅させる
	gf::BlinkFeature( m_cArcHelper.GetScreenDisplay(), _ttol(strID), ipSearchedFC, 80, 3 );
}

void CAttrWalkNodeDlg::CreateListView(LPCTSTR nameColName, LPCTSTR resultStr)
{
	// リストビューのカラム名やスタティックな文字列を種別に合わせて変更
	CListViewCtrl lv( GetDlgItem(IDC_LIST_RESULT) );
	for( int i=0; i<m_ListViewColumnSize; ++i )
		lv.InsertColumn( i, &m_cResultColInfo[i] );
	lv.SetExtendedListViewStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	// LVCOLUMNはLPTSTRしかメンバに持てないため、LPCTSTRを渡す場合にはLPTSTRにcastする必要がある
	// ListView_SetColumn内では変更されないことが明らかなので、const_castしてLPTSTR型に変換している
	LVCOLUMN column = {LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, 120, const_cast<LPTSTR>(nameColName), 0, 0, 0, 0 };
	ListView_SetColumn(GetDlgItem(IDC_LIST_RESULT), 1, &column);
	SetDlgItemText(IDC_SEARCH_RESULT_LABEL, resultStr);
}

void CAttrWalkNodeDlg::CheckInvaliedValue()
{
	// 選択されているノード種別を取得
	CComboBox combo( GetDlgItem(IDC_COMBO_WALKNODECLASS) );
	DWORD_PTR nodeClass = combo.GetItemData( combo.GetCurSel() );

	// [bug 5954]ノード種別が「駅出入口ノード」から変更となった場合
	// ...から大分変えました

	// 保持しているエラーを一度リセットする
	m_errors[GetCurrentFeatureDef()->GetFeature()].Reset();

	// 駅出入り口対応IDコントロールに関するチェック
	CheckStationGateCtrl(nodeClass);

	// [bug 11559][e][SJ歩行者]歩行者ノードと地下街出入口POIの紐づけを出来るようにしてほしい
	// 地下街出入口対応IDコントロールに関するチェック
	CheckUndergroundGateCtrl(nodeClass);

	// 道路ノード対応IDコントロールに関するチェック
	CheckCrossNodeCtrl(nodeClass);
}

void CAttrWalkNodeDlg::CheckStationGateCtrl( DWORD_PTR nodeClass )
{
	CheckCommonGateCtrl( nodeClass, IDC_EDIT_STATIONGATE );
}

void CAttrWalkNodeDlg::CheckUndergroundGateCtrl( DWORD_PTR nodeClass )
{
	CheckCommonGateCtrl( nodeClass, IDC_EDIT_UNDERGROUNDGATE );
}

void CAttrWalkNodeDlg::CheckCommonGateCtrl( DWORD_PTR nodeClass, INT nID )
{
	using namespace sindy_error::walk_node;

	// 各ID毎の設定
	int checkNodeClass = 0;
	ErrorCode noneInputError = ErrorCode::STATIONGATE_NONE_INPUT_ERROR;
	ErrorCode inputError = ErrorCode::STATIONGATE_INPUT_ERROR;
	switch (nID)
	{
	case IDC_EDIT_STATIONGATE:
		checkNodeClass = walk_node::node_class::kGate;
		noneInputError = ErrorCode::STATIONGATE_NONE_INPUT_ERROR;
		inputError=ErrorCode::STATIONGATE_INPUT_ERROR;
		break;
	case IDC_EDIT_UNDERGROUNDGATE:
		checkNodeClass = walk_node::node_class::kUndergroundGate;
		noneInputError = ErrorCode::UNDERGROUNDGATE_NONE_INPUT_ERROR;
		inputError=ErrorCode::UNDERGROUNDGATE_INPUT_ERROR;
		break;
	default:
		// CheckCtrl系以外で使用されず、IDC_EDIT_STATIONGATE/IDC_EDIT_UNDERGROUNDGATE以外で
		// 使用されないので、そのままreturn
		return;
	}

	// チェック処理
	CControlDef* pTargetCtrl = m_cControlRel.GetControlDef( GetDlgItem(nID) );
	CComVariant vaValue;
	pTargetCtrl->GetControlValue( vaValue );
	if( checkNodeClass == nodeClass )
	{
		// IDを持たないといけないのに入力がなければエラー
		pTargetCtrl->m_bErrored = ( ( vaValue.vt == VT_NULL ) ) ? TRUE : FALSE;
		if(pTargetCtrl->m_bErrored)
		{
			// エラーならエラーコードをプッシュ
			m_errors[GetCurrentFeatureDef()->GetFeature()].Push(noneInputError);
		}
		return;
	}

	// IDを持ってはいけないのに入力があればエラー
	pTargetCtrl->m_bErrored = ( ( vaValue.vt != VT_NULL ) ) ? TRUE : FALSE;
	if(pTargetCtrl->m_bErrored)
	{
		// エラーならエラーコードをプッシュ
		m_errors[GetCurrentFeatureDef()->GetFeature()].Push(inputError);
	}
}

void CAttrWalkNodeDlg::CheckCrossNodeCtrl(DWORD_PTR nodeClass)
{
	using namespace sindy_error::walk_node;

	CControlDef* pTargetCtrl = m_cControlRel.GetControlDef( GetDlgItem(IDC_EDIT_ROADNODEID) );
	CComVariant vaValue;
	pTargetCtrl->GetControlValue( vaValue );
	// 特定種別の場合は入力を許可しないのでエラー
	switch(nodeClass)
	{
	case walk_node::node_class::kOutline:
		// IDを持ってはいけないのに入力があればエラー
		pTargetCtrl->m_bErrored = ( ( vaValue.vt != VT_NULL ) ) ? TRUE : FALSE;
		if(pTargetCtrl->m_bErrored)
		{
			// エラーならエラーコードをプッシュ
			m_errors[GetCurrentFeatureDef()->GetFeature()].Push(ErrorCode::ROADNODEID_INPUT_ERROR);
		}
		break;
	default: // その他の種別ではエラーにはしない
		pTargetCtrl->m_bErrored = FALSE;
		break;
	}
}

bool CAttrWalkNodeDlg::CheckExistField( LPCTSTR fcName, LPCTSTR fieldName )
{
	IFeatureClassPtr ipFC = GetFeatureClass( fcName, false );
	long fieldIndex = -1;
	ipFC->FindField( CComBSTR(walk_node::kUndergroundGatePointID), &fieldIndex);
	if( fieldIndex == -1 )
		return false;
	return true;
}
