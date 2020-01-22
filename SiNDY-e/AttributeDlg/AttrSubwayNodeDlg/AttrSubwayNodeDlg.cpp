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

// AttrSubwayNodeDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrSubwayNodeDlg.h"
#include "globalfunc.h"
#include <boost/assign/list_of.hpp>
#include "Map.h"
#include <TDC/useful_headers/str_util.h>

using namespace sindy::schema;

// TODO[SiNDYChildDlg] ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(NODE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NODECLASS)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_CODECLASS)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_STATIONNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_STATIONNAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINENAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LINENAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_STATIONNUM)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_STATIONNUM)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_REST, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DUMMY)
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))
					RCTOFIT(IDC_STATIC_STATIONLIST)
					RCTOFIT(-1)
					RCREST(IDC_LIST_STATION)
				ENDGROUP()
				BEGINROWS(WRCT_REST, 0, RCMARGINS(2,2))
					RCTOFIT(IDC_STATIC_LINELIST)
					RCTOFIT(-1)
					RCREST(IDC_LIST_LINE)
				ENDGROUP()
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(2,2))
					RCTOFIT(IDC_STATIC_DUMMY2)
					RCREST(-1)
					RCTOFIT(IDC_BUTTON_APPLY)
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


BEGIN_WINDOW_MAP(LINK_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LINENAME2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LINENAME2)
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

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrSubwayNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrSubwayNodeDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSubwayNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSubwayNodeDlg::CAttrSubwayNodeDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrSubwayNodeDlg::CAttrSubwayNodeDlg() 
	: ATTR_BASE_CLASS(NODE_MAP, IDD_SUBWAYNODEDLG)
{
	// TODO[SiNDYChildDlg] 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
		( IDC_EDIT_STATIONNUM, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_STATIONNUM ) );
		( IDC_COMBO_CODECLASS, CContainedWindow(  _T("COMBO"), this, IDC_COMBO_CODECLASS ) );
}

/** 
 * @fn HWND CAttrSubwayNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrSubwayNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrSubwayNodeDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrSubwayNodeDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

CTargetItems& CAttrSubwayNodeDlg::GetConnectedLink(IFeaturePtr nodePtr)
{
	if( !m_connectedLink.empty() )
		return m_connectedLink;

	AheGetConnectedNetworkLink( nodePtr, m_connectedLink );

	// 新規作成時は接続リンクが引っかからない
	if( m_connectedLink.empty() )
	{
		// ツリーには必ずリンクが1個だけいるはず
		long node_id = AheGetOID( nodePtr );
		for( auto& feature : *m_pFeatureDefList )
		{
			if(0 == _tcsicmp( sj::subway_link::kTableName, feature.GetTableName() ))
			{
				m_connectedLink.push_back( CTargetItem( feature.GetFeature() ) );
				break;
			}
		}
	}
	return m_connectedLink;
}

IFeaturePtr CAttrSubwayNodeDlg::GetCorrespNode( long node_id, const IFeaturePtr& linkPtr )
{
	CString to_or_from = 
		( AheGetAttribute( linkPtr, sj::subway_link::kToNodeID ).lVal == node_id ) ?
			sj::subway_link::kFromNodeID : sj::subway_link::kToNodeID;

	// 他方のノードを検索
	IQueryFilterPtr query = AheInitQueryFilter( nullptr, nullptr, _T("%s=%ld"), kObjectID, AheGetAttribute( linkPtr, to_or_from ).lVal );
	IFeatureCursorPtr ipCur;
	AheGetNetworkNodeFeatureClass(linkPtr)->Search( query, VARIANT_FALSE, &ipCur );
	IFeaturePtr another;
	ipCur->NextFeature( &another );

	// 新規作成の場合Serch()に引っかからない
	// FGDBでは引っかかるようだが、IRow.Store()されている必要がある旨が
	// ヘルプに記載されているので注意（bug 10103）
	// ToDo: IFeatureClass.GetFeature()でおｋ
	if( !another )
	{
		// ツリーには必ず一対のノードがいるはず
		for( auto& feature : *m_pFeatureDefList )
		{
			if(
				0 == _tcsicmp( sj::subway_node::kTableName, feature.GetTableName() ) &&
				node_id != feature.GetRowOID() )
			{
				another = feature.GetFeature();
				break;
			}
		}
	}

	return another;
}


void CAttrSubwayNodeDlg::ChangeWindowMap( bool bLink )
{
	m_winMgr.ChangeMgrMap( bLink ? LINK_MAP : NODE_MAP );
	for( auto& r: LINK_MAP )
	{
		::ShowWindow( GetDlgItem( r.GetID() ), bLink ? SW_SHOW : SW_HIDE );
	}
	for( auto& r: NODE_MAP )
	{
		::ShowWindow( GetDlgItem( r.GetID() ), bLink ? SW_HIDE : SW_SHOW );
	}
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);	
}


/**
 * @fn BOOL CAttrSubwayNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * また、原稿番号の値も初期化して、ルールから取得しておく
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrSubwayNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	bool enable = ( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable );
	
	// 編集開始してなければ触らせない
	SetDlgStatus( IDC_BUTTON_APPLY, enable ? TRUE : FALSE );
	SetDlgStatus( IDC_LIST_LINE, enable ? TRUE : FALSE );
	SetDlgStatus( IDC_LIST_STATION, enable ? TRUE : FALSE );
	// 駅名と路線名はReadOnlyに
	SetDlgStatus( IDC_EDIT_STATIONNAME, FALSE );
	SetDlgStatus( IDC_EDIT_LINENAME, FALSE );
	SetDlgStatus( IDC_EDIT_LINENAME2, FALSE );
	
	// 表示されるフィーチャ
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();

	// 駅POI
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ILayerPtr ipPoi = cMap.FindLayer( nullptr, nullptr, station_point::kTableName );
	IFeatureClassPtr ipPoiClass = AheGetFeatureClass( ipPoi );
	
	// 乗り入れ路線テーブル確保
	// リンクの場合は↓でreturnされてしまうので、必ずそれより前
	m_joinLineT = AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, join_line_info::kTableName );

	if( sj::subway_link::kTableName == m_layerName )
	{
		// 路線名取得
		long id = AheGetAttribute( ipFeature, sj::subway_link::kRailwayLineID ).lVal;
		_IRowPtr row;
		AheOpenSameOwnerTable( (ITablePtr)ipPoiClass, railway_line::kTableName )->GetRow( id, &row );

		// エディットボックスに反映
		CEdit edit = GetDlgItem(IDC_EDIT_LINENAME2);
		edit.SetWindowText( CString(AheGetAttribute( row, railway_line::kName )) );

		// リンク用のダイアログにする
		ChangeWindowMap(true);
		return bRet;
	}
	
	// 乗り入れ路線ID取得
	long join_line_id = AheGetAttribute( ipFeature, sj::subway_node::kJoinLineInfo ).lVal;

	// 図郭上ノードは弄らせない
	if( sj::subway_node::node_class::kOutline == AheGetAttribute( ipFeature, sj::subway_node::kNodeClassC ).lVal )
	{
		m_cControlRel.Visible( 0, TRUE, FALSE );
	}

	m_connectedLink.clear();

	// 駅名・路線名選択リストボックス初期化
	m_serchLists.reflesh( ipFeature, 100, ipPoiClass );

	// ノードが新規の場合は0
	if( join_line_id < 1 )
	{
		// 必ずあるはず
		auto& link = *GetConnectedLink( ipFeature ).begin();

		// 周辺の駅POIから乗り入れ路線IDを取得
		join_line_id = m_serchLists.GetJoinLineInfoIDFromRailWayLine( 
			link.GetAttribute( sj::subway_link::kRailwayLineID ).lVal );
	}

	// リスト選択して「適用」ボタンをエミュレート（エディットボックスに反映）
	m_serchLists.select( join_line_id );
	CButton( GetDlgItem(IDC_BUTTON_APPLY) ).Click();
	
	// ウィンドウマップ再配置
	ChangeWindowMap(false);
	
	if( enable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_STATIONNUM) );
	}
	return bRet;
}

void CAttrSubwayNodeDlg::SetFeatureClassName(LPCTSTR lpcszFeatureClassName)
{
	m_layerName = lpcszFeatureClassName;
}

/**
 * @fn void CAttrSubwayNodeDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrSubwayNodeDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );
	
	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// TODO[SiNDYChildDlg] ControlRelにフィールド名とテーブル名を登録
		// ノード種別
		m_cControlRel.SetControl( IDC_COMBO_CODECLASS, TYPE_COMBO, 0, 0, sj::subway_node::kNodeClassC, 0, sj::subway_node::kTableName );
		// 駅番号
		m_cControlRel.SetControl( IDC_EDIT_STATIONNUM, TYPE_EDITSTRING, 0, 0, sj::subway_node::kStationNumber, 0, sj::subway_node::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, sj::subway_node::kTableName );
	}

	// 検索用リストボックスコントロール初期化
	m_serchLists.init( GetDlgItem(IDC_LIST_STATION), GetDlgItem(IDC_LIST_LINE) );
}

// 対応する路線IDを検索する
long CAttrSubwayNodeDlg::SearchLineID( const IFeaturePtr& subway_node )
{
	long join_info_id = AheGetAttribute( subway_node, sj::subway_node::kJoinLineInfo ).lVal;
	IQueryFilterPtr query = AheInitQueryFilter( nullptr, nullptr, _T("%s=%ld"), kObjectID, join_info_id );
	
	_IRowPtr row;
	_ICursorPtr ipCur;

	long line_id = -1;
	m_joinLineT->Search( query, VARIANT_FALSE, &ipCur );
	if( S_OK == ipCur->NextRow( &row ) && row )
	{
		line_id = AheGetAttribute( row, join_line_info::kLineID ).lVal;
	}

	return line_id;
}

long GetLineID( const ITablePtr& joinLineT, long line_info )
{
	// JOIN_LINE_INFO から LINEID取得
	_IRowPtr joinLine;
	joinLineT->GetRow( line_info, &joinLine );
	return AheGetAttribute( joinLine, join_line_info::kLineID ).lVal;
}

bool CAttrSubwayNodeDlg::CheckNode( const IFeaturePtr& node, long railway_id, CTargetItems& allLinks )
{
	for( auto& feature : GetConnectedLink(node) )
	{
		// 他端ノード
		IFeaturePtr another = GetCorrespNode( AheGetOID(node), feature.GetFeature() );

		// 接続リンク取得
		CTargetItems links;
		AheGetConnectedNetworkLink( another, links );
		if( !links.empty() )
		{
			for( const auto& link : links)
			{
				//long id = link.GetAttribute( kObjectID ).lVal;
				//long railway = link.GetAttribute( sj::subway_link::kRailwayLineID ).lVal;
				//OutputDebugString( uh::str_util::format( _T("[OID]:%d [RailwayID]:%d\n"), id, railway ) );
				if( railway_id != link.GetAttribute( sj::subway_link::kRailwayLineID ).lVal )
				{
					::MessageBox( m_hWnd, AheLoadString(IDS_CONNECT_DIFFERENT_LINE), AheLoadString(IDS_DIALOG_NAME), MB_OK | MB_ICONERROR );
					return false;
				}
			}
		}
		else // 新規リンク
		{
			if( railway_id < 0 ) // 自分では路線特定できない（駅以外）
			{
				// 他端のノードを調べてもだめならダメ
				railway_id = SearchLineID( another );
				if( railway_id < 0 )
				{
					::MessageBox( m_hWnd, AheLoadString(IDS_UNKNOWN_LINE), AheLoadString(IDS_DIALOG_NAME), MB_OK | MB_ICONERROR );
					return false;
				}
			}
			else
			{
				// 他端の路線と違ってたらダメ
				if( railway_id != SearchLineID( another ) )
				{
					::MessageBox( m_hWnd, AheLoadString(IDS_DIFFERENT_LINE_ID), AheLoadString(IDS_DIALOG_NAME), MB_OK | MB_ICONERROR );
					return false;
				}
			}
		}

		allLinks.push_back( feature );
	}
	return true;
}

// OK押した後に呼ばれる（オーバーライド）
BOOL CAttrSubwayNodeDlg::ErrorCheck()
{
	IFeaturePtr nodePtr;
	long railway_id = 0;
	CTargetItems links; // 接続リンク群
	// ここでGetCurrentFeatureDef()しても望みのものは得られないことがあるので自前でノードを探す
	for( auto& def : *m_pFeatureDefList )
	{
		// GetTableName()はLPCTSTR、スキーマ定義は_TCHARなので == 比較できない！！！！
		if( 0 == _tcsicmp( def.GetTableName(), sj::subway_node::kTableName ) )
		{
			nodePtr = def.GetFeature();
			const auto& lineInfo = def.GetFieldDef( sj::subway_node::kTableName, sj::subway_node::kJoinLineInfo );
			railway_id = GetLineID( m_joinLineT, lineInfo->m_vaValue.lVal );

			//OutputDebugString( uh::str_util::format( _T("[%s]: %d [line_info] %d [railway_id] %d\n"), def.GetTableName(), AheGetOID(nodePtr), l, railway_id ));
			CTargetItems linkstemp;
			if( !CheckNode( nodePtr, railway_id, linkstemp ) )
			{
				return False;
			}

			for( auto& link : linkstemp )
			{
				link.SetAttribute( sj::subway_link::kRailwayLineID, railway_id );
				links.push_back( link );
			}
		}
	}
	if( !nodePtr )
		return FALSE;
	
	// リンクに変更があったらストアしておく
	for( auto& link : links )
	{
		link.Store();
	}

	return TRUE;
}

// リストボックス選択変更
LRESULT CAttrSubwayNodeDlg::OnSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch( wID )
	{
	case IDC_LIST_STATION:
		m_serchLists.StationSelChange();
		break;
	case IDC_LIST_LINE:
		m_serchLists.LineSelChange();
		break;
	default:
		break;
	}

	return 0;
}

// 適用ボタン押下
LRESULT CAttrSubwayNodeDlg::OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 駅名
	CEdit edit = GetDlgItem(IDC_EDIT_STATIONNAME);
	edit.SetWindowText( m_serchLists.selectedStation );

	// 路線名
	edit = GetDlgItem(IDC_EDIT_LINENAME);
	edit.SetWindowText( m_serchLists.selectedLine );

	// 変わってたら属性変更
	long join_line_info = m_serchLists.GetJoinLineInfoID();
	if( join_line_info < 0 )
		return 0;

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	if( join_line_info != AheGetAttribute(ipFeature, sj::subway_node::kJoinLineInfo).lVal )
	{
		// FieldDef::SetNewValue()しないとコントロール上で変更したことにならない
		GetCurrentFeatureDef()->GetFieldDef( sj::subway_node::kTableName, sj::subway_node::kJoinLineInfo )->SetNewValue( join_line_info, m_hWnd );
		AheSetAttribute( ipFeature, sj::subway_node::kJoinLineInfo, join_line_info );
		SetButton( TRUE );
	}
	
	return 0;
}
