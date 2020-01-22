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

// AttrStationAOIDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrStationAOIDlg.h"
#include <ArcHelperEx/Map.h>
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

/// リストビュー上に表示されるユニークコードのカラム名
static const _TCHAR UNIQUE_CODE[] = _T("JOIN_LINE_INFO_OID");

/// リストビュー上に表示されるカラム名
static const _TCHAR* COLUMNS[] = {
	UNIQUE_CODE,
	_T("駅名"),
	_T("路線名"),
	_T("会社名")
};

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(STATIONAOI_MAP)
	BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_RELATION_POI_TEXT)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 30)
			RCREST(IDC_RELATIONLIST)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_SEARCH_RAILWAY_TEXT)
			RCTOFIT(IDC_SEARCH_AREA_TEXT)
			RCPERCENT(IDC_METER_BOX,30)
			RCTOFIT(IDC_LESSTHAN_METER_TEXT)
			RCTOFIT(IDC_BUTTON_SEARCH_POI)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_PCT, 0, 40)
			RCREST(IDC_SEARCHLIST)
		ENDGROUP()
		RCSPACE(5)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
			RCTOFIT(IDC_BUTTON_APPLY)
		ENDGROUP()
		RCSPACE(5)
	ENDGROUP()
END_WINDOW_MAP()

/**
* @brief TOCから指定レイヤを取得
* @param layerName [in]     TOCから取得するレイヤ名
* @param stCount   [in,out] TOC上に存在するレイヤ名の数を取得
* @param ipMap     [in]     IMapPtr
*/
ILayerPtr getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap )
{
	// TOCからlayerNameを取得
	CMap cMap( ipMap );
	// TOC上のlayerNameの数をカウント
	stCount = 0;
	ILayerPtr ipLayer;
	for( auto& it : cMap )
	{
		if( AheGetFeatureClassName( it ).Compare( layerName ) == 0 )
		{
			ipLayer = it;
			++stCount;
		}
	}
	return ipLayer;
}

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrStationAOIDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrStationAOIDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrStationAOIDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrStationAOIDlg::CAttrStationAOIDlg()
	: ATTR_BASE_CLASS(STATIONAOI_MAP, IDD_STATIONAOIDLG),
	  m_IsCheckedChangeFromProg(false)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		(IDC_METER_BOX, CContainedWindow(  _T("EDIT"), this, IDC_METER_BOX ) )
		(IDC_BUTTON_APPLY, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_APPLY ) )
		(IDC_BUTTON_SEARCH_POI, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCH_POI ) );
}

/**
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrStationAOIDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrStationAOIDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
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
BOOL CAttrStationAOIDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// リストビュー等、紐付けに使用するものを初期化
	static bool initListView = initializeListView();

	if( !initListView )
	{
		SetDlgStatus( IDC_RELATIONLIST, FALSE );
		SetDlgStatus( IDC_SEARCHLIST, FALSE );
		SetDlgStatus( IDC_BUTTON_SEARCH_POI, FALSE );
		SetDlgStatus( IDC_METER_BOX, FALSE );
		SetDlgStatus( IDC_BUTTON_APPLY, FALSE );
		return bRet;
	}

	// 適用ボタン以外は操作できるようにしておく
	SetDlgStatus( IDC_RELATIONLIST, TRUE );
	SetDlgStatus( IDC_SEARCHLIST, TRUE );
	SetDlgStatus( IDC_BUTTON_SEARCH_POI, TRUE );
	SetDlgStatus( IDC_METER_BOX, TRUE );
	SetDlgStatus( IDC_BUTTON_APPLY, FALSE );

	// 適応ボタンを押した際に使用する削除用バッファは消去
	m_CheckedRemove.clear();

	long lOID = AheGetOID(GetCurrentFeatureDef()->GetFeature());
	// リストビュー上で内部的に保持しているデータがあるなら、そちらを表示する
	// 属性ダイアログを開いた後に、フィーチャーを切り替えた場合、
	if(! m_RelationView.Review( lOID ) )
	{
		// 紐付けされているPOIをリストに表示
		dispPOIInfo( IDC_RELATIONLIST );
	}
	m_SearchView.Review( lOID );

	// リレーションリストにあるものにはチェックを付ける
	m_IsCheckedChangeFromProg = true;
	auto relationIDList = m_RelationView.MakeTargetColumnList( UNIQUE_CODE );
	m_SearchView.SetCheckedOn( UNIQUE_CODE, relationIDList );
	m_IsCheckedChangeFromProg = false;

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetDlgStatus( IDC_BUTTON_APPLY, TRUE );
	}

	// エディットボックスに初期値を設定
	SetDlgItemText( IDC_METER_BOX,_T("50") );

	// 最初にフォーカスされるコントロール
	::SetFocus( GetDlgItem(IDC_METER_BOX) );

	return bRet;
}

/**
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrStationAOIDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	}
	else {
		m_cControlRel.SetHWND( m_hWnd );
	}
}

LRESULT CAttrStationAOIDlg::OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( wID != IDC_BUTTON_SEARCH_POI ) return FALSE;

	// 選択しているフィーチャーの形状を取得
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	ITopologicalOperatorPtr ipTopo( ipGeom );
	if( !ipTopo )
		return FALSE;

	// エディットボックスに入力されている値の範囲にあるものを検索
	CWindow edit( GetDlgItem( IDC_METER_BOX ) );

	// 値を取得
	CString value;
	edit.GetWindowText(value);
	int meter = std::stoi(value.GetString());
	if(meter < 0) meter = 0;

	// 単位をメートルから度に変換
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( meter, esriMeters, esriDecimalDegrees, &dConvRange );

	// エディットボックスに入力されている値の範囲にあるものを検索
	// バッファリング形状で空間検索
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );
	if( !ipGeoBuffer )
		return FALSE;

	// TOCからSTATION_POINTを取得
	int stCount = 0; //!< TOC上のSTATION_POINTの数をカウント
	ILayerPtr ipLayer = getLayer( station_point::kTableName, stCount, m_cArcHelper.GetFocusMap() );
	// STATION_POINTがTOC上に二つ以上存在する場合
	if( stCount >= 2 )
	{
		MessageBox( _T("TOCにSTATION_POINTが二つ以上存在するため\n紐付けるSTATION_POINTを特定できません"), _T("STATION_POIの特定に失敗") );
		return FALSE;
	}
	if( !ipLayer)
		return FALSE;

	CTargetItems cItems;
	AheSelectByShapeFromLayer( ipGeoBuffer, cItems, ipLayer );

	// 表示されているリストと関連するデータを消去
	m_SearchView.Delete( AheGetOID(ipFeature) );
	m_SearchView.Clear();
	for( auto& item : cItems )
	{
		long lOID = AheGetOID( item.GetFeature() );
		CString strOID;
		strOID.Format(_T("%ld"), lOID);

		AOI_MATRIX dispList = getViewList( lOID );
		for( auto& elem : dispList )
		{
			m_SearchView.AddItem( elem );
		}
	}

	// リレーションリストにあるものにはチェックを付ける
	m_IsCheckedChangeFromProg = true;
	auto relationIDList = m_RelationView.MakeTargetColumnList( UNIQUE_CODE );
	m_SearchView.SetCheckedOn( UNIQUE_CODE, relationIDList );
	m_IsCheckedChangeFromProg = false;
	return TRUE;
}

// 適応ボタンが押された時に呼ばれる処理
// SearchListでチェックの付いている行をRelationListに挿入し、チェックのない行を削除する
LRESULT CAttrStationAOIDlg::OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( wID != IDC_BUTTON_APPLY ) return FALSE;

	bool isChanged = false; //!< 変更があったかどうかのフラグ

	// チェックを外されたものをリストビューから削除
	isChanged |= removePOIFromRelationLV();

	// チェックをつけられたものをリストビューに追加
	isChanged |= addPOIToRelationLV();

	// 変更ないなら更新の必要がないのでFALSEを返す
	if( !isChanged ) return FALSE;

	// コントロール上で変更したことにするためにm_bChangedフラグをTRUEにする
	GetCurrentFeatureDef()->GetFieldDef( sj::station_aoi::kTableName, _T("OBJECTID") )->m_bChanged = TRUE;
	SetButton( TRUE );

	return TRUE;
}

// リストの行が選択された時に呼ばれる処理(特定のポイントをハイライトする)
LRESULT CAttrStationAOIDlg::OnClickedList(LPNMHDR pnmd)
{
	LPNMLISTVIEW pnmlist = (LPNMLISTVIEW)pnmd;
	if(! ( (pnmlist->uChanged & LVIF_STATE) && (pnmlist->uNewState & LVIS_SELECTED) && !(pnmlist->uOldState & LVIS_SELECTED) ) )
		return FALSE;

	auto lList = ((pnmd->idFrom==IDC_SEARCHLIST)? m_SearchView:m_RelationView).MakeTargetColumnList(UNIQUE_CODE);

	// 目的の行までイテレータを進める
	auto it = lList.begin();
	std::advance( it, pnmlist->iItem );
	if( it== lList.end() )
		return FALSE;

	long stationOID = -1; //<! ハイライトするSTATION_POINTのOBJECTIDを格納する変数
	ITablePtr ipTable = AheOpenSameOwnerTable( (ITablePtr)m_ipStationClass, join_line_info::kTableName );
	if( !ipTable )
		return FALSE;

	CString strSubField;
	strSubField.Format(_T("%s,%s"), join_line_info::kStationID, _T("OBJECTID") );
	_ICursorPtr ipCursor;

    if( FAILED(ipTable->Search(
        AheInitQueryFilter(nullptr, strSubField, _T("%s=%s"), _T("OBJECTID"), *it ),
		VARIANT_TRUE, &ipCursor) ) )
		return FALSE;

	long index = 0;
	if( FAILED(ipCursor->FindField(CComBSTR(join_line_info::kStationID),&index) ) )
		return FALSE;

	// 検索される要素は1つしかないはず
	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow )==S_OK && ipRow )
	{
		CComVariant value = AheGetAttribute( ipRow, index );
		if( value.vt == VT_EMPTY )
			return FALSE;
		// lVal以外には値が入っていないはず
		stationOID = value.lVal;
	}

	// 検索された要素をハイライト表示する
	setHighLight( stationOID );

	// FALSEを返さないと親(?)の処理(例えば、リストをクリックしたときに、その行の色を変える、といった処理)が実行されない
	return FALSE;
}

//チェックボックスがチェックされた時の処理
LRESULT CAttrStationAOIDlg::OnClickedCheckBox(LPNMHDR pnmd)
{
	// 検索ボタンが押されたときの処理では何もしない
	if( m_IsCheckedChangeFromProg ) return FALSE;

	LPNMLISTVIEW pnmlist = (LPNMLISTVIEW)pnmd;
	// uOldStateが0のときは初期化のときなので、何もしない
	if(!pnmlist->uOldState)
		return FALSE;

	UINT newState = (pnmlist->uNewState & LVIS_STATEIMAGEMASK);
	UINT oldState = (pnmlist->uOldState & LVIS_STATEIMAGEMASK);

	CString JOIN_OID = m_SearchView.GetValue( pnmlist->iItem, UNIQUE_CODE );
	// チェック有→無である場合
	if( (oldState == INDEXTOSTATEIMAGEMASK(2)) && (newState == INDEXTOSTATEIMAGEMASK(1))  )
	{
		// 編集開始されている場合
		if(m_cArcHelper.IsStartEditing())
		{
			// RelationListに値が存在し、チェックを外されたなら、記録しておく
			if( m_RelationView.IsExist( JOIN_OID, UNIQUE_CODE ) )
				m_CheckedRemove.insert( JOIN_OID );
		}
		// 編集開始されていない場合、チェックボックスの状態を元に戻す
		// リストビュー上のチェックボックスだけを操作できなくするための処理。
		// TODO:チェックだけENABLEにできるなら、そちらを利用のこと
		else
		{
			m_IsCheckedChangeFromProg = true;
			m_SearchView.ChangeChecked( pnmlist->iItem, TRUE );
			m_IsCheckedChangeFromProg = false;
		}
	}
	// チェック無→有の場合
	else if( (oldState == INDEXTOSTATEIMAGEMASK(1)) && (newState == INDEXTOSTATEIMAGEMASK(2))  )
	{
		// 編集開始されている場合
		if(m_cArcHelper.IsStartEditing())
		{
			auto it = m_CheckedRemove.find( JOIN_OID );
			if( it!= m_CheckedRemove.end() )
				m_CheckedRemove.erase( it );
		}
		// 編集開始されていない場合、チェックボックスの状態を元に戻す
		// リストビュー上のチェックボックスだけを操作できなくするための処理。
		// TODO:チェックだけENABLEにできるなら、そちらを利用のこと
		else
		{
			m_IsCheckedChangeFromProg = true;
			m_SearchView.ChangeChecked( pnmlist->iItem, FALSE );
			m_IsCheckedChangeFromProg = false;
		}
	}

	return FALSE;
}

/// STATOIN_POINTをハイライトする処理
LRESULT CAttrStationAOIDlg::OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( m_ipFeature )
		AheDrawVertex( m_cArcHelper.GetScreenDisplay(), m_ipFeature, FALSE, 5, 10, 10, 255, esriSMSSquare, TRUE );
	return FALSE;
}

// OKボタンを押された後に呼び出されるので、ここで関連テーブルを更新する
BOOL CAttrStationAOIDlg::ErrorCheck()
{
	std::map< long, AOI_MATRIX >& relationData = m_RelationView.getRelationData();

	std::list<long> eraseList;
	BOOL isFailed = TRUE;
	for( auto& elem : relationData )
	{
		if( update(elem.first, elem.second ) )
		{
			eraseList.push_back( elem.first );
		}
		else
		{
			isFailed = FALSE;
		}
	}
	for( auto& elem : eraseList )
	{
		// オブジェクト指向的に考えて人のものをこっちで消すのはいかんでしょう...
		// けど、面倒なのでこうしておく
		relationData.erase( relationData.find(elem) );
	}
	return isFailed;
}

// リストビューの初期化処理
bool CAttrStationAOIDlg::initializeListView()
{
	// 関連テーブルを管理するインスタンスの初期化
	FIELDNAMES subFields;
	subFields.push_back( sj::rel_station_poi::kJoinLineInfo );
	m_RelTableM.Initialize( sj::rel_station_poi::kTableName, sj::rel_station_poi::kAOI_ID,
		subFields, GetCurrentFeatureDef()->GetFeature() );

	// リストビューにカラムを設定
	CAOIListViewCtrl::COLUMN_NAMES columns;
	columns.push_back(COLUMNS[0]);
	columns.push_back(COLUMNS[1]);
	columns.push_back(COLUMNS[2]);
	columns.push_back(COLUMNS[3]);

	m_RelationView.Init(m_hWnd, IDC_RELATIONLIST, columns, 0);
	m_SearchView.Init(m_hWnd, IDC_SEARCHLIST, columns, LVS_EX_CHECKBOXES);

	// TOCからSTATION_POINTを取得
	int stCount = 0; //!< TOC上のSTATION_POINTの数をカウント
	ILayerPtr ipPoi = getLayer( station_point::kTableName, stCount, m_cArcHelper.GetFocusMap() );
	if( !ipPoi )
	{
		MessageBox( _T("STATION_POINTと紐付けするためには\nSTATION_POINTをTOCに追加する必要があります"), _T("STATION_POIの取得に失敗") );
		return false;
	}
	// STATION_POINTがTOC上に二つ以上存在する場合
	if( stCount >= 2 )
	{
		MessageBox( _T("TOCにSTATION_POINTが二つ以上存在するため\n紐付けるSTATION_POINTを特定できません"), _T("STATION_POIの特定に失敗") );
		return false;
	}

	m_ipStationClass = AheGetFeatureClass( ipPoi );
	if( !m_ipStationClass )
	{
		MessageBox( _T("STATION_POINTフィーチャーを開けません"), _T("STATION_POIのオープンに失敗") );
		return false;
	}

	m_LineCorpManager.setTable( (ITablePtr)m_ipStationClass );
	m_StationNameManager.setTable( (ITablePtr)m_ipStationClass );

	// 以下、テーブルの対応関係を設定する
	std::list<std::pair<CString,CString>> tempList;
	tempList.push_back( std::make_pair( join_line_info::kTableName, _T("OBJECTID") ) );
	tempList.push_back( std::make_pair( railway_line::kTableName, railway_line::kName ) );
	tempList.push_back( std::make_pair( railway_corp::kTableName, railway_corp::kName ) );
	m_LineCorpManager.SetReturnData( tempList );

	//STATION_POINTに対するJOIN_LINE_INFOの関係
	m_LineCorpManager.Join( station_point::kTableName, join_line_info::kTableName, _T("OBJECTID"), join_line_info::kStationID );
	//JOIN_LINE_INFOに対するRAILWAY_LINEの関係
	m_LineCorpManager.Join( join_line_info::kTableName, railway_line::kTableName, join_line_info::kLineID, _T("OBJECTID") );
	//RAILWAY_LINEに対するRAILWAY_CORPの関係
	m_LineCorpManager.Join( railway_line::kTableName, railway_corp::kTableName, railway_line::kCorpID, _T("OBJECTID") );

	//// 以下、駅名取得用
	// 再利用
	tempList.clear();
	tempList.push_back( std::make_pair( join_line_info::kTableName, _T("OBJECTID") ) );
	tempList.push_back( std::make_pair( station_info::kTableName, station_info::kName ) );
	m_StationNameManager.SetReturnData( tempList );

	////STATION_POINTに対するJOIN_LINE_INFOの関係
	m_StationNameManager.Join( station_point::kTableName, join_line_info::kTableName, _T("OBJECTID"), join_line_info::kStationID );
	//JOIN_LINE_INFOに対するSTATION_INFOの関係
	m_StationNameManager.Join( join_line_info::kTableName, station_info::kTableName, join_line_info::kInfoID, _T("OBJECTID") );

	return true;
}

// 指定されたオブジェクトIDの関連テーブルを更新する
bool CAttrStationAOIDlg::update( long lOID, const AOI_MATRIX& lList )
{
	// リストビュー上のJOIN_LINE_INFO_IDの一覧を取得
	auto lViewList = getUniqueData( lList );
	// DB上のJOIN_LINE_INFO_IDの一覧を取得
	auto tableList = m_RelTableM.GetRelationData( lOID, sj::rel_station_poi::kJoinLineInfo );

	// リストビューに存在し、DB上に存在しないものは追加
	auto addIDList = getDiffList( lViewList, tableList );
	// DB上に存在し、リストビューに存在しないものは削除
	auto deleteIDList = getDiffList( tableList, lViewList ); 

	// テーブルから紐付けを外された行を削除
	if( !m_RelTableM.DeleteData( lOID, deleteIDList ) )
	{
		CString strErr;
		strErr.Format(_T("%sから紐付けを外されたデータを削除することができませんでした"), sj::rel_station_poi::kTableName);
		MessageBox(strErr);
		return false;
	}
	// テーブルに行を追加
	if( !m_RelTableM.AddData( lOID, addIDList ) )
	{
		CString strErr;
		strErr.Format(_T("%sに新たに紐付けされたデータを追加することができませんでした"), sj::rel_station_poi::kTableName);
		MessageBox(strErr);
		return false;
	}
	return true;
}

// 指定されたオブジェクトIDをもつSTATION_POINTをハイライト表示する
bool CAttrStationAOIDlg::setHighLight( long lOID )
{
	// 検索条件とサブフィールドを設定
	CString strSubField;
	strSubField.Format(_T("%s,%s"), _T("SHAPE"),_T("OBJECTID"));

	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED(m_ipStationClass->Search(
		AheInitQueryFilter( nullptr, strSubField, _T("%s=%ld"), _T("OBJECTID"), lOID ),
		VARIANT_TRUE, &ipFeatureCursor)) )
		return false;

	// 1つしかないはず
	if( ipFeatureCursor->NextFeature(&m_ipFeature)!=S_OK || !m_ipFeature )
		return false;

	// 再描画命令を出して選択点を再描画させる
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	return true;
}

// 選択されたフィーチャーに関連付けられているPOIを取得し、リストビューに表示
void CAttrStationAOIDlg::dispPOIInfo( UINT nClassID )
{
	/// 現在選択中のフィーチャーのOIDを格納
	long lOID = AheGetOID( GetCurrentFeatureDef()->GetFeature() );

	/// データベース上から、選択中の駅AOIポリゴンのOIDをAOI_IDとしてもつ行のJOIN_LINE_INFO_OIDの一覧を取得
	auto joinIDList = m_RelTableM.GetRelationData( lOID, sj::rel_station_poi::kJoinLineInfo );

	// AOI_IDにlOIDを持つ行を検索
	for( auto& joinID : joinIDList )
	{
		auto railCorpList = m_LineCorpManager.GetValue(joinID, _T("OBJECTID"), join_line_info::kTableName );
		auto stationInfoList = m_StationNameManager.GetValue(joinID, _T("OBJECTID"), join_line_info::kTableName );
		auto dispList = getViewList( railCorpList, stationInfoList );
		for( auto& elem : dispList )
		{
			m_RelationView.AddItem( elem );
		}
	}
}


//以下、上記関数内で必要な処理を別関数として実装したもの

// update
//リストからユニークIDのカラムだけを抽出し、取得
AOI_COLUMN CAttrStationAOIDlg::getUniqueData( const AOI_MATRIX& lList ) const
{
	int uniqueIndex = m_RelationView.GetColumnIndex( UNIQUE_CODE );
	AOI_COLUMN retList;
	for( auto& elem : lList )
	{
		auto it = elem.begin();
		std::advance( it, uniqueIndex );
		if( it!= elem.end())
			retList.push_back( *it );
	}
	return retList;
}

std::list<CString> CAttrStationAOIDlg::getDiffList( const std::list<CString>& argList1, const std::list<CString>& argList2 ) const
{
	std::list<CString> retList;
	for(auto& val : argList1)
	{
		auto it = find( argList2.begin(), argList2.end() ,val );
		if( it==argList2.end() )
			retList.push_back( val );
	}
	return retList;
}

// OnClickedApplyButton
// 紐付けられているPOIのリストビューから、チェックを外されたPOIを消す
bool CAttrStationAOIDlg::removePOIFromRelationLV()
{
	// 削除対象に何もないなら何もしない
	if( m_CheckedRemove.empty() )
		return false;

	// 削除用関数に引き渡せる形式に変換
	AOI_COLUMN deleteList;
	for( auto& elem : m_CheckedRemove )
		deleteList.push_back( elem );

	// 引き渡したら削除対象として保持していたものは消す
	m_CheckedRemove.clear();

	return m_RelationView.DeleteItem( UNIQUE_CODE, deleteList );
}

// 紐付けられているPOIのリストビューに、チェックをつけられたPOIを足す
bool CAttrStationAOIDlg::addPOIToRelationLV()
{
	auto relationIDList = m_RelationView.MakeTargetColumnList( UNIQUE_CODE );
	int UniqueIDIndex = m_SearchView.GetColumnIndex( UNIQUE_CODE );

	auto lineList = m_SearchView.GetNonExistValue( relationIDList, true, UNIQUE_CODE );
	for( auto& elem : lineList )
	{
		m_RelationView.AddItem( elem );
	}
	return !lineList.empty();
}

// OnClickedSearchButton dispPOIInfo
// リストビューに挿入するデータを取得
AOI_MATRIX CAttrStationAOIDlg::getViewList( long lOID )
{
	CString strOID;
	strOID.Format(_T("%ld"), lOID);
	auto railCorpList = m_LineCorpManager.GetValue(strOID , _T("OBJECTID"), station_point::kTableName );
	auto stationInfoList = m_StationNameManager.GetValue(strOID , _T("OBJECTID"), station_point::kTableName );

	return getViewList(railCorpList,stationInfoList);
}

AOI_MATRIX CAttrStationAOIDlg::getViewList( AOI_MATRIX& railCorp, AOI_MATRIX& stationInfo ) const
{
	AOI_MATRIX dispList;

	// 二つのリストのサイズが異なる=データが不完全なので、そういったものは表示しない
	if( railCorp.size() != stationInfo.size() )
		return dispList;

	auto corpIt = railCorp.begin();

	for(; corpIt != railCorp.end(); ++corpIt)
	{
		// データ挿入時に使用するイテレータを定義
		auto insertIt = corpIt->begin();

		AOI_LINE addList;
		// JOIN_LINE_INFOのOBJECTIDを挿入
		addList.push_back( *insertIt );

		// 駅名を挿入(ここまで来ているなら、空になることはないはず)
		for( auto& elem : stationInfo )
		{
			// 一番目の要素==JOIN_LINE_INFOのOBJECTIDが同じなら
			if( *elem.begin() == *insertIt )
			{
				addList.push_back( *(++elem.begin()) );
				break;
			}
		}
		++insertIt;

		// 路線名を挿入
		addList.push_back( *insertIt );
		++insertIt;

		// 会社名を挿入
		addList.push_back( *insertIt );
		++insertIt;

		dispList.push_back( addList );
	}
	return dispList;
}
