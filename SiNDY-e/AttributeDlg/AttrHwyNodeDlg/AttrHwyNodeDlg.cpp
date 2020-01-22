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

// AttrHwyNodeDlg.cpp: CAttrHwyNodeDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrHwyNodeDlg.h"
#include <sindy/schema.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <boost/array.hpp>
#include <TDC/useful_headers/str_util.h>
#include <WinLib/type_convert.h>

using namespace sindy::schema;

/// ウィンドウマネージャ配置マップ（hwynode、tollnode共用）
BEGIN_WINDOW_MAP(HWYTOLLNODEMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_HWYNODE, RCMARGINS(2,2))
				RCSPACE(22)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// 路線種別とハイウェイモードフラグ
					RCTOFIT(IDC_STATIC_ROADCLASS_REF)
					RCSPACE(5)
					RCREST(IDC_COMBO_HWYNODE_ROADCLASS_REF)
					RCSPACE(15)
					RCTOFIT(IDC_CHECK_ISHWMODE)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// 路線名称
					RCTOFIT(IDC_STATIC_ROADNAME_REF)
					RCSPACE(5)
					RCREST(IDC_COMBO_HWYNODE_ROADNAME_REF)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// 施設名称
					RCTOFIT(IDC_STATIC_SEQUENCE)
					RCSPACE(5)
					RCREST(IDC_COMBO_SEQUENCE)
				ENDGROUP()
				RCSPACE(4)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// IC番号
					RCTOFIT(IDC_STATIC_HWYGUIDE_NUM)
					RCSPACE(5)
					RCTOFIT(IDC_EDIT_HWYGUIDE_NUM)
				ENDGROUP()
				RCSPACE(7)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))       // 方向コードと分岐合流コード
					RCTOFIT(IDC_STATIC_DIRECTION)
					RCSPACE(5)
					RCTOFIT(IDC_COMBO_DIRECTION)
					RCSPACE(10)
					RCTOFIT(IDC_STATIC_JUNCTION)
					RCSPACE(5)
					RCTOFIT(IDC_COMBO_JUNCTION)
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))       // 料金所種別コードと案内料金所フラグ
					RCTOFIT(IDC_STATIC_TOLLCLASS)
					RCSPACE(5)
					RCTOFIT(IDC_COMBO_TOLLCLASS)
					RCSPACE(10)
					RCTOFIT(IDC_CHECK_GUIDE)
				ENDGROUP()
				RCSPACE(7)

				// 一般車両用グループ
				BEGINROWS(WRCT_TOFIT, IDC_GROUP_NOETC, RCMARGINS(0, 0))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(7,0))
						RCTOFIT(IDC_CHECK_NOETC_CAR)			// 一般車両対応フラグ
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCSPACE(15)
						RCTOFIT(IDC_STATIC_TOLLFUNC)
						RCSPACE(10)
						RCTOFIT(IDC_COMBO_TOLLFUNC)				// 料金所機能コード
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_NOGUIDETOLL)			// 非誘導料金所フラグ
					ENDGROUP()
					RCSPACE(5)
				ENDGROUP()
				RCSPACE(7)

				// ETC搭載車用グループ
				BEGINROWS(WRCT_TOFIT, IDC_GROUP_ETC, RCMARGINS(0, 0))
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(7,0))
						RCTOFIT(IDC_STATIC_ETC_FACIL)			// ETC施設コード
						RCSPACE(5)
						RCTOFIT(IDC_COMBO_ETC_FACIL)
					ENDGROUP()
					RCSPACE(4)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCSPACE(15)
						RCTOFIT(IDC_STATIC_TOLLFUNC_ETC)
						RCSPACE(10)
						RCTOFIT(IDC_COMBO_TOLLFUNC_ETC)			// 料金所機能コード(ETC)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_NOGUIDETOLL_ETC)		// 非誘導料金所フラグ(ETC)
					ENDGROUP()
					RCSPACE(4)
				ENDGROUP()
				RCSPACE(7)

				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))       // 流入ノードIDと流出ノードID
					RCTOFIT(IDC_STATIC_FROMNODEID)
					RCTOFIT(IDC_COMBO_FROMNODE_ID)
					RCSPACE(32)
					RCTOFIT(IDC_STATIC_TONODEID)
					RCTOFIT(IDC_COMBO_TONODE_ID)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(7)
		BEGINROWS(WRCT_REST, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
// ウィンドウマネージャ配置マップ（料金所レーン情報タブ用）
BEGIN_WINDOW_MAP(HWYTOLLNODEMAP2)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_HWYNODE, RCMARGINS(2,2))
				RCSPACE(20)
				BEGINROWS(WRCT_TOFIT, IDC_GROUP_TOLLGATELANE, RCMARGINS(10, 10))
					RCSPACE(5)
					RCFIXED(IDC_PICTURE_LANEIMG, 64)				// 料金所レーン情報のイメージ
					RCSPACE(10)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
						RCTOFIT(IDC_EDIT_TOLLGATELANE)			// 料金所レーン情報
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
		RCSPACE(7)
		BEGINROWS(WRCT_PCT, 0, 10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
// ウィンドウマネージャ配置マップ（料金所コード情報情報タブ用）
BEGIN_WINDOW_MAP(HWYTOLLNODEMAP3)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			BEGINROWS(WRCT_REST, IDC_TAB_HWYNODE, RCMARGINS(2,2))
				RCSPACE(22)
					BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))		// 【23-090】ETC料金所コード
						RCTOFIT(IDC_STATIC_ETC_TOLLGATE)
						RCSPACE(5)
						RCREST(IDC_EDIT_ETC_TOLLGATE)
					ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
	RCSPACE(7)
		BEGINROWS(WRCT_PCT, 0, 10)
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
 * @retval CAttrHwyNodeDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrHwyNodeDlg();
}

// CAttrHwyNodeDlgの静的メンバ変数
const long CAttrHwyNodeDlg::m_TollGateLaneStrMaxLength = 32;
const long CAttrHwyNodeDlg::m_TollGateLaneImageColumnMaxNum = 16;

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHwyNodeDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrHwyNodeDlg::CAttrHwyNodeDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrHwyNodeDlg::CAttrHwyNodeDlg() 
	: ATTR_BASE_CLASS(HWYTOLLNODEMAP, IDD_ATTRHWYNODEDLG),
	m_wnd0 ( _T("CHECKBOX"),	this, IDC_CHECK_ISHWMODE ),
	m_wnd1 ( _T("COMBOBOX"),	this, IDC_COMBO_HWYNODE_ROADCLASS_REF ), 
	m_wnd2 ( _T("COMBOBOX"),	this, IDC_COMBO_HWYNODE_ROADNAME_REF ), 
	m_wnd3 ( _T("COMBOBOX"),	this, IDC_COMBO_SEQUENCE ), 
	m_wnd4 ( _T("COMBOBOX"),	this, IDC_COMBO_DIRECTION ), 
	m_wnd5 ( _T("COMBOBOX"),	this, IDC_COMBO_JUNCTION ), 
	m_wnd7 ( _T("CHECKBOX"),	this, IDC_CHECK_DUMMY ), 
	m_wnd8 ( _T("CHECKBOX"),	this, IDC_CHECK_GUIDE ), 
	m_wnd9 ( _T("COMBOBOX"),	this, IDC_COMBO_TOLLCLASS ),
	m_wnd10( _T("COMBOBOX"),	this, IDC_COMBO_TOLLFUNC ),
	m_wnd11( _T("COMBOBOX"),	this, IDC_COMBO_FROMNODE_ID ),
	m_wnd12( _T("COMBOBOX"),	this, IDC_COMBO_TONODE_ID ),
	m_wnd13( _T("EDIT"),		this, IDC_EDIT_SOURCE ),
	m_wnd16( _T("CHECKBOX"),	this, IDC_CHECK_NOETC_CAR ),
	m_wnd17( _T("COMBOBOX"),	this, IDC_COMBO_ETC_FACIL ),
	m_wnd18( _T("COMBOBOX"),	this, IDC_COMBO_TOLLFUNC_ETC ),
	m_wnd19( _T("CHECKBOX"),	this, IDC_CHECK_NOGUIDETOLL_ETC ),
	m_wnd20( _T("EDIT"),		this, IDC_EDIT_TOLLGATELANE ),
	m_wnd21( _T("EDIT"),		this, IDC_EDIT_HWYGUIDE_NUM ),
	m_wnd22( _T("EDIT"),		this, IDC_EDIT_ETC_TOLLGATE ),
	m_wndTab( _T("TAB"),		this, IDC_TAB_HWYNODE ),
	// タブ位置の初期化
	m_CurrentTabIndex(0),
	m_ChangeType(0)
{
}

/**
 * @brief WM_NOTIFY 用イベントハンドラ
 *		タブの切り替えが発生した時の処理を行う
 */
LRESULT CAttrHwyNodeDlg::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// タブコントロールにフォーカスがある場合
	if( wParam == IDC_TAB_HWYNODE ) {
		LPNMHDR pnmh = (LPNMHDR)lParam;
		if( pnmh->code == TCN_SELCHANGE ) {
			// コントロール切り替え
			m_ChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
			
			m_cControlRel.Visible( m_ChangeType );
			ChangeWindowMgrMap( m_ChangeType );
			m_CurrentTabIndex = m_ChangeType; // タブの位置保存
			//[Bug 5510] AttrAccessPointDlg::OnNotify()と同様に、タブ切り替え時にコンボボックスが空になるのを防ぐため
			// ここでAttrHwyNodeDlg::SetCurrentFeatureDefIndex()を呼ぶ
			SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

			bHandled = TRUE;
			return 1;
		}
	}
	// タブ以外のコントロールにフォーカスがある場合
	// ATTR_BASE_CLASS::OnKeyDown()で、WM_NOTIFYがwParam==0でPostMessage()されるのでここに来る
	else if( wParam == 0 )
	{
		// コントロール切り替え
		switch( lParam )
		{
			case -2:	// Ctrl+Tabが押された場合
				if( m_ChangeType == hwynode_dialog_tab::tab_end - 1 )
					m_ChangeType = hwynode_dialog_tab::base_attr;
				else
					++m_ChangeType;
				break;
			case -1:	// Shift+Ctrl+Tabが押された場合
				if( m_ChangeType == hwynode_dialog_tab::base_attr )
					m_ChangeType = hwynode_dialog_tab::tab_end - 1;
				else
					--m_ChangeType;
				break;
			default:	// 上記以外のキー操作でここに来ることはないはず
				ATLASSERT(0);
		}

		::SendMessage(m_wndTab, TCM_SETCURSEL, m_ChangeType, 0);
		m_cControlRel.Visible( m_ChangeType );
		ChangeWindowMgrMap( m_ChangeType );
		m_CurrentTabIndex = m_ChangeType;	// タブの位置保存
		SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

		bHandled = TRUE;
		return 1;
	}

	return 0;
}

/**
 * @brief オーナードロー用関数
 * @note	オーナードローイベント発生時に呼ばれるCOwnerDraw::DrawItem()のオーバーライド
 */
void CAttrHwyNodeDlg::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	if (m_CurrentTabIndex == hwynode_dialog_tab::tollgate_lane)
	{
		DrawLaneImage();
	}
}

/** 
 * @fn HWND CAttrHwyNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrHwyNodeDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrHwyNodeDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrHwyNodeDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn void CAttrHwyNodeDlg::SetArcHelper( IApplication* ipApp )
 * @brief ATTR_BASE_CLASSにArcHelperを渡します、また、HwyHelperにもArcHelperをセットします。
 */
void CAttrHwyNodeDlg::SetArcHelper( IApplication* ipApp )
{ 
    ATTR_BASE_CLASS::SetArcHelper( ipApp );
}

/**
 * @brief ETC距離別料金用フィールドがない場合に必要になるコントロールのEnable/Disableを切り替えを行う
 * @note	[Bug6432]対応時に追加
 */
void CAttrHwyNodeDlg::SetAvailabilityOfEtcSpecificTollControls(void)
{
	// ETC距離別料金用のフィールドが存在しない場合
	if (! EtcSpecificTollFieldsExist())
	{
		// 対応するコントロールを常にDisableにする
		SetDlgStatus( IDC_CHECK_NOETC_CAR, FALSE );
		SetDlgStatus( IDC_COMBO_ETC_FACIL, FALSE );
		SetDlgStatus( IDC_COMBO_TOLLFUNC_ETC, FALSE );
		SetDlgStatus( IDC_CHECK_NOGUIDETOLL_ETC, FALSE );
		SetDlgStatus( IDC_STATIC_ETC_FACIL, FALSE );
		SetDlgStatus( IDC_STATIC_TOLLFUNC_ETC, FALSE );

		// 編集開始状態なら、一般車両対応フラグに従属するためにDisableになっているコントロールを強制的にEnableにする
		// BUG 8550 ルール的に編集不可状態のときに、料金所機能コードが編集できる -> ルール編集の判断を追加する
		if (IsStartEditing() && m_bEditable)
		{
			SetDlgStatus( IDC_COMBO_TOLLFUNC, TRUE );
			SetDlgStatus( IDC_CHECK_NOGUIDETOLL, TRUE );
		}
	}
}

/**
 * @fn BOOL CAttrHwyNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * また、ハイウェイノードは、流入、流出ノードに向けて、矢印を表示したりしなければならないので、その設定をしておきます。
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrHwyNodeDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_CurrentTabIndex, bForce, bEditable );
	
	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	// 一番最初に現在選択したハイウェイノードの下にある道路ノードを取得しておく
	// 現在選択中のフィーチャを取得
	CFeatureDef* ipFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipFeature = ipFeatureDef->GetFeature();
	// ジオメトリ取得
	IGeometryPtr ipGeometry;
	ipFeature->get_Shape( &ipGeometry );
	// 複数ハイウェイノードがある場合に、m_ipNodeUnderHwyNodeが初期化されていないと
	// あとで落ちることがあった
	m_ipNodeUnderHwyNode = NULL;
	if( ipGeometry )
	{
		// ノードレイヤを取得
		ILayerPtr ipLayer;
		CString strOwnerName = AheGetFeatureClassOwnerName( ipFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("ROAD_NODE");
		else
		{
			strOwnerName += _T(".");
			strOwnerName += _T("ROAD_NODE");
		}
		ipLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );
		// レイヤが取得できなかった場合には、パーソナルジオデータベースの可能性あり
		if( !ipLayer )
			AheGetLayerByTableName( m_cArcHelper.GetMap(), _T("Road_Node"), &ipLayer );

		if ( ipLayer )
		{
			CTargetItems cItems;
			// ハイウェイの下のノードフィーチャ取得
			AheSelectByShapeFromLayer( ipGeometry, cItems, ipLayer );

			LONG lCount = cItems.size();
			if( lCount == 1 )
				m_ipNodeUnderHwyNode = (cItems.begin())->GetFeature();
		}
	}
	
	// 路線コード、路線内シーケンスの初期値確保
	CComVariant			vaRoadSequence;
	CFieldDef*			pFieldDef;
	CComVariant			vaValue;        
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
	vaValue= pFieldDef->m_vaValue;
	m_lOrgRoadSequence = vaValue.lVal;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_CODE") );
	vaValue = pFieldDef->m_vaValue;
	m_lOrgRoadCode = vaValue.lVal;
	
	// 最初から路線コードを出すため
	SetLineClassList(m_lOrgRoadCode);
	ChangeRoadSeqCombobox();
	
	// 流入、流出ノードの設定
	SetInOutNodeComboBox();
	
	CButton	cHwyCheckBox = CButton(GetDlgItem(IDC_CHECK_ISHWMODE));
	INT iCheckBox = cHwyCheckBox.GetCheck();
	
	// ハイウェイモードがオフだったら、流出、流入ノードを描画
	if( iCheckBox == 0 )
		DrawInOutNode();

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		switch( m_CurrentTabIndex )
		{
			case hwynode_dialog_tab::base_attr:
				::SetFocus( GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF) );
				break;
			case hwynode_dialog_tab::tollgate_lane:
				::SetFocus( GetDlgItem(IDC_EDIT_TOLLGATELANE) );
				break;
			case hwynode_dialog_tab::etc_tollgate:
				::SetFocus( GetDlgItem(IDC_EDIT_ETC_TOLLGATE) );
				break;
			default:
				ATLASSERT(0);
		}
	}

	// [Bug6432] ETC距離別料金用フィールドがない場合、コントロールのEnable/Disableを切り替える
	SetAvailabilityOfEtcSpecificTollControls();
	// [Bug6570] 料金所レーン情報フィールドがない場合は、料金所レーン情報のエディットボックスをDisableにする
	if (! TollGateLaneFieldExist())
	{
		SetDlgStatus(IDC_EDIT_TOLLGATELANE, FALSE);
	}

	// ETC料金所IDフィールドがなければDisable
	if(0 > AheFindField(AheGetFields(ipFeature), highway_node::kTollgateID))
	{
		SetDlgStatus(IDC_EDIT_ETC_TOLLGATE, FALSE);
	}

	// ETC料金所IDはETC非対応のときはDisableにする
	auto fd = GetCurrentFeatureDef()->GetFieldDef( highway_node::kTableName, highway_node::kEtcFacil );
	SetDlgStatus( 
		IDC_EDIT_ETC_TOLLGATE,
		bool2BOOL( fd->m_vaValue.lVal != highway_node::etc_facil::kNoEtcFacil ));

	// ハイウェイモードフラグのチェックボックスは常にDisable（表示専用）にする（[Bug 6217]対応時に追加）
	SetDlgStatus(IDC_CHECK_ISHWMODE, FALSE);

	// 初期値に対する整合チェック
	CheckItems();

	// ノードが同位置になければ、全てDisableにする
	if( !m_ipNodeUnderHwyNode )
	{
		DisableAllControls();
	}

    return bRet;
}

/**
 * @brief 現在のタブ位置を返す
 * @note	純粋仮想関数CExportDlg::GetTabNum()の実装
 * @note	次にダイアログを開いた時にそのタブがアクティブになるようにするため、CExportDlgに現在のタブ位置を渡しておく
 * @retval 現在のタブ位置
 */
LONG CAttrHwyNodeDlg::GetTabNum()
{
	return m_CurrentTabIndex;
}

/**
 * @brief タブ位置をセットします
 * @note	純粋仮想関数CExportDlg::SetTabNum()の実装
 * @param lTabNum	[in]	タブ位置
 */
void CAttrHwyNodeDlg::SetTabNum(LONG lTabNum)
{
	m_CurrentTabIndex = lTabNum;
	m_ChangeType = lTabNum;
}

/**
 * @brief ETC距離別料金対応のためのデータモデル変更[Bug6198]で追加されたフィールドが存在するかを判定する
 * @note	最初に呼ばれた時に判定結果をm_pEtcSpecificTollFieldsExistに格納して、以後はそれを参照
 * @return	対象フィールドが全て存在すればtrue、１つでも存在しないフィールドがあればfalse
 */
bool CAttrHwyNodeDlg::EtcSpecificTollFieldsExist()
{
	bool aResult = true;
	if (m_pEtcSpecificTollFieldsExist.get() )
		aResult = *m_pEtcSpecificTollFieldsExist;
	else
	{
		using namespace sindy::schema;

		// ハイウェイノードのIFeatureClassPtrを取得
		// [Bug6432] HwyHelperを使わないように修正
		IFeaturePtr ipFeature(GetCurrentFeatureDef()->GetFeature());
		IFeatureClassPtr ipHwyNodeFeatureClass(AheGetFeatureClass(ipFeature));

		std::vector<CString> aFieldNames;
		aFieldNames.push_back(highway_node::kNoEtcCar);
		aFieldNames.push_back(highway_node::kEtcFacil);
		aFieldNames.push_back(highway_node::kEtcTollFunc);
		aFieldNames.push_back(highway_node::kEtcNoGuideToll);
		
		BOOST_FOREACH (CString aFieldName, aFieldNames)
		{
			long aIndex = ipHwyNodeFeatureClass->_FindField(aFieldName.GetString());
			if (aIndex == -1)
				aResult = false;
		}

		m_pEtcSpecificTollFieldsExist = std::auto_ptr<bool>(new bool(aResult));
	}

	return aResult;
}

/**
 * @brief 料金所レーン対応のためのデータモデル変更[Bug6557]で追加された「料金所レーン情報」フィールドが存在するかを判定する
 * @note	最初に呼ばれた時に判定結果をm_pTollGateLaneFieldExistに格納して、以後はそれを参照
 * @return	対象フィールドが全て存在すればtrue、１つでも存在しないフィールドがあればfalse
 */
bool CAttrHwyNodeDlg::TollGateLaneFieldExist()
{
	bool aResult = true;
	if (m_pTollGateLaneFieldExist.get() )
		aResult = *m_pTollGateLaneFieldExist;
	else
	{
		using namespace sindy::schema;

		// ハイウェイノードのIFeatureClassPtrを取得
		IFeaturePtr ipFeature(GetCurrentFeatureDef()->GetFeature());
		IFeatureClassPtr ipHwyNodeFeatureClass(AheGetFeatureClass(ipFeature));

		long aIndex = ipHwyNodeFeatureClass->_FindField(highway_node::kTollGateLane);
		if (aIndex == -1)
			aResult = false;
		m_pTollGateLaneFieldExist = std::auto_ptr<bool>(new bool(aResult));
	}

	return aResult;
}


/**
 * @fn void CAttrBaseMeshDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrHwyNodeDlg::CreateControlRelation()
{
	using namespace hwynode_dialog_tab;
    ATLASSERT( IsWindow() == TRUE );
	
    if( m_cControlRel.size() > 0 )
    {
        // 親ウインドウハンドルを設定しなおす
        m_cControlRel.ResetHWND( m_hWnd );
    } 
    else {
        m_cControlRel.SetHWND( m_hWnd );

		// タブ
		m_cControlRel.SetControl( IDC_TAB_HWYNODE,		TYPE_OTHER, 	base_attr,	tab_end - 1 );

		using namespace sindy::schema;
        // ハイウェイモードフラグ
		m_cControlRel.SetControl( IDC_CHECK_ISHWMODE,		TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kHwyMode,	NULL,	HIGHWAY_NODE_TABLE_NAME );
		// 路線種別
		m_cControlRel.SetControl( IDC_STATIC_ROADCLASS_REF, TYPE_OTHER, 	base_attr,	base_attr );
        m_cControlRel.SetControl( IDC_COMBO_HWYNODE_ROADCLASS_REF,	TYPE_COMBO,		base_attr,	base_attr );
		// 路線名称
		m_cControlRel.SetControl( IDC_STATIC_ROADNAME_REF, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_HWYNODE_ROADNAME_REF,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kRoadCode,	NULL,	HIGHWAY_NODE_TABLE_NAME);
		// 施設名称
		m_cControlRel.SetControl( IDC_STATIC_SEQUENCE, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_SEQUENCE,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kRoadSeq,		NULL,	HIGHWAY_NODE_TABLE_NAME );
		// IC番号(bug 10205)
		m_cControlRel.SetControl( IDC_STATIC_HWYGUIDE_NUM, TYPE_OTHER, 	0,	base_attr );
		m_cControlRel.SetControl( IDC_EDIT_HWYGUIDE_NUM,	TYPE_EDITSTRING,base_attr,	base_attr,	highway_node::kHWYGuideNumber,	NULL,	HIGHWAY_NODE_TABLE_NAME );
        // 方向コード
		m_cControlRel.SetControl( IDC_STATIC_DIRECTION, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_DIRECTION,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kDirection,	NULL,	HIGHWAY_NODE_TABLE_NAME);
        // 分岐・合流コード
		m_cControlRel.SetControl( IDC_STATIC_JUNCTION, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_JUNCTION,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kInOut,		NULL,	HIGHWAY_NODE_TABLE_NAME );
        // 料金所種別コード
		m_cControlRel.SetControl( IDC_STATIC_TOLLCLASS, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TOLLCLASS,		TYPE_COMBO,		base_attr,	base_attr,	highway_node::kTollClass,	NULL,	HIGHWAY_NODE_TABLE_NAME );
        // 案内料金所フラグ
		m_cControlRel.SetControl( IDC_CHECK_GUIDE,			TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kGuide,		NULL,	HIGHWAY_NODE_TABLE_NAME );

		// [Bug6432] ETC距離別料金用のフィールドの有無に関わらずSetControl()するように修正
		// 料金所機能コード
		m_cControlRel.SetControl( IDC_STATIC_TOLLFUNC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TOLLFUNC,		TYPE_COMBO,		base_attr,	base_attr, highway_node::kTollFunc,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_NOETC_CAR, FOLLOW_VALUEIS1,	0 );
		// 非誘導料金所フラグ
		m_cControlRel.SetControl( IDC_CHECK_DUMMY,			TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kDummyToll,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_NOETC_CAR, FOLLOW_VALUEIS1,	0 );
		// 一般車両対応フラグ
		m_cControlRel.SetControl( IDC_GROUP_NOETC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_CHECK_NOETC_CAR,		TYPE_CHECKBOX,	base_attr,	base_attr,	highway_node::kNoEtcCar,		NULL,	HIGHWAY_NODE_TABLE_NAME);
		// ETC施設コード
		m_cControlRel.SetControl( IDC_STATIC_ETC_FACIL, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_GROUP_ETC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_ETC_FACIL,		TYPE_COMBO,	base_attr,	base_attr,	highway_node::kEtcFacil,	NULL,	HIGHWAY_NODE_TABLE_NAME);
		// 料金所機能コード(ETC)
		m_cControlRel.SetControl( IDC_STATIC_TOLLFUNC_ETC, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TOLLFUNC_ETC,	TYPE_COMBO,	base_attr,	base_attr,	highway_node::kEtcTollFunc,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_COMBO_ETC_FACIL, FOLLOW_VALUE,	0 );
		// 非誘導料金所フラグ(ETC)
		m_cControlRel.SetControl( IDC_CHECK_NOGUIDETOLL_ETC,	TYPE_CHECKBOX,	0,	base_attr,	highway_node::kEtcNoGuideToll,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_COMBO_ETC_FACIL, FOLLOW_VALUE,	0 );

		// 流入ノードID
		m_cControlRel.SetControl( IDC_STATIC_FROMNODE_ID, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_FROMNODE_ID,	TYPE_COMBO,		base_attr,	base_attr, highway_node::kApproachNodeID	,NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_ISHWMODE, FOLLOW_VALUEREVERSE,	0 );
        // 流出ノードID
		m_cControlRel.SetControl( IDC_STATIC_TONODE_ID, TYPE_OTHER, 	base_attr,	base_attr );
		m_cControlRel.SetControl( IDC_COMBO_TONODE_ID,		TYPE_COMBO,		base_attr,	base_attr, highway_node::kFlowNodeID,	NULL,	HIGHWAY_NODE_TABLE_NAME,	NULL,	IDC_CHECK_ISHWMODE,	FOLLOW_VALUEREVERSE,	0 );

		// 料金所レーン情報
		m_cControlRel.SetControl( IDC_GROUP_TOLLGATELANE, TYPE_OTHER, 	tollgate_lane,	tollgate_lane );
		m_cControlRel.SetControl( IDC_PICTURE_LANEIMG, TYPE_OTHER, 	tollgate_lane,	tollgate_lane );
		m_cControlRel.SetControl( IDC_EDIT_TOLLGATELANE,	TYPE_EDITSTRING,tollgate_lane,	tollgate_lane,	highway_node::kTollGateLane,	NULL,	HIGHWAY_NODE_TABLE_NAME );

		// ETC料金所コード（【23-090】）
		m_cControlRel.SetControl( IDC_STATIC_ETC_TOLLGATE, TYPE_OTHER, 	hwynode_dialog_tab::etc_tollgate,	hwynode_dialog_tab::etc_tollgate );
		m_cControlRel.SetControl( IDC_EDIT_ETC_TOLLGATE,	TYPE_EDITSTRING,hwynode_dialog_tab::etc_tollgate,	hwynode_dialog_tab::etc_tollgate,	highway_node::kTollgateID,	NULL,	highway_node::kTableName );

		// 情報ソース
		m_cControlRel.SetControl( IDC_STATIC_SOURCE, TYPE_OTHER, 	base_attr,	tab_end - 1 );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,base_attr,	tab_end -1,	highway_node::kSource,		NULL,	HIGHWAY_NODE_TABLE_NAME );
	}
}

/**
 * @brief ウィンドウマネージャのマップの切り替えを行う
 * @param[in]	cWindowMapIndex	マップのインデックス番号
 */
void CAttrHwyNodeDlg::ChangeWindowMgrMap(int cWindowMapIndex)
{
	switch (cWindowMapIndex)
	{
		case 0:
				m_winMgr.ChangeMgrMap(HWYTOLLNODEMAP);
				break;
		case 1:
				m_winMgr.ChangeMgrMap(HWYTOLLNODEMAP2);
				break;
		case 2:
				m_winMgr.ChangeMgrMap(HWYTOLLNODEMAP3);
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
 * @fn void CAttrHwyNodeDlg::ChangeRoadSeqCombobox()
 * @brief 参照用の路線内シーケンスのコンボボックスを設定する
 *
 * Facil_Info_Pointを取得し、そのテーブルから路線番号で検索し、ひっかかったものを路線内シーケンスのコンボボックスに登録する
 */
void CAttrHwyNodeDlg::ChangeRoadSeqCombobox()
{
	// Facil_Infoテーブルを取得していなければ、取得する
	if( !m_ipFacilInfoTable )
	{
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
		
		// Facil_Info_Pointテーブル取得
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("Facil_Info_Point");
		else
			strOwnerName += _T(".Facil_Info_Point");
		m_ipFacilInfoTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );
	}
	
	// 取得した路線番号をテーブルから検索
	_ICursorPtr		ipCursor;
	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	// [Bug6217] 路線コードのエディットボックスの削除に伴い、エディットボックスではなくm_nRoadCodeから路線コードを取得するように修正
	// [Bug7173] ロケールに依存しない書き方に修正
	strWhereClause.Format(_T("ROAD_CODE = %d"), m_nRoadCode );
	strSubFields.Format(_T("NAME_KANJI,ROAD_SEQ"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(_T("NAME_KANJI,ROAD_SEQ")) );

	if( m_ipFacilInfoTable )
		m_ipFacilInfoTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	
	// 路線内シーケンス取得
	CComVariant                vaRoadSequence;
	CFieldDef*                pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
	vaRoadSequence = pFieldDef->m_vaValue;
	m_cRoadSeq.Format(_T("%d"), vaRoadSequence.lVal);
	
	// コンボボックスに入れるための値を作成する
	_IRowPtr ipRow;
	std::map<LONG, CString> mapDomain;
	if( ipCursor )
	{
		while( ipCursor->NextRow( &ipRow ) == S_OK )
		{
			if( !ipRow ) 
				break;
			
			CComVariant vaKanjiName, vaRoadSeq, vaKanaName, vaClass;
			LONG lIndex = -1;
			IFieldsPtr ipFields;
			ipRow->get_Fields( &ipFields );
			ipFields->FindField( CComBSTR(_T("NAME_KANJI")), &lIndex );
			ipRow->get_Value( lIndex, &vaKanjiName );
			ipFields->FindField( CComBSTR(_T("ROAD_SEQ")), &lIndex );
			ipRow->get_Value( lIndex, &vaRoadSeq );
			
			CString        strVal;
			strVal.Format(_T("%d"), vaRoadSeq.lVal );        
			
			USES_CONVERSION;
			CString strText;
			strText.Format(_T("%s : %s"), strVal, OLE2T(vaKanjiName.bstrVal) );	// [Bug6217] 表示方法を変更
			
			mapDomain[vaRoadSeq.lVal] = strText;
		}
		
		// コンボボックスに値を入れる
		CComboBox cCombo( GetDlgItem( IDC_COMBO_SEQUENCE ) );
		cCombo.ResetContent();
		for( std::map<LONG, CString>::iterator it = mapDomain.begin(); it != mapDomain.end(); it++ )
		{
			INT iCurPos = cCombo.AddString( it->second );
			cCombo.SetItemData( iCurPos, it->first );
			if( vaRoadSequence.lVal == it->first )
				cCombo.SetCurSel( iCurPos );
		}
	}
	
	SetRoadSequence();
}

/**
 * @fn void CAttrHwyNodeDlg::SetRoadSequence()
 * @brief 路線内シーケンスエディタに値を出力する
 * @note	[Bug6217] 路線内シーケンスのエディットボックスを削除したので、この関数の役割は値が変更されているかのチェックのみになった
 */
void CAttrHwyNodeDlg::SetRoadSequence()
{
	// 値が変更されているか確認
	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_SEQ") );
	pFieldDef->m_vaValue = _ttol(m_cRoadSeq);
	LONG        lDummy = _ttol(m_cRoadSeq);
	if( lDummy != m_lOrgRoadSequence )
	{
		pFieldDef->m_bChanged = TRUE;
		SetButton( Changed() );
	}
}

/**
 * @fn bool CAttrHwyNodeDlg::SetLineClassList(LONG nRoadCode)
 * @brief ダイアログが呼ばれたときに路線種別参照用のコンボボックスに値をセットする
 * @param nRoadCode	[in]	路線コードの値
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 * @note	[Bug6432] HwyHelperを使わないように修正
 */
bool CAttrHwyNodeDlg::SetLineClassList(LONG nRoadCode)
{
	// 路線種別リストのコンボボックス
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
	// アイテムのクリア
	cbLineClass.ResetContent();
	
	// 路線コードテーブルを取得していなければ、取得する
	if( !m_ipRoadCodeListTable )
	{
		IWorkspacePtr ipWorkspace;
		ITablePtr ipCurrentTable;
		IFeaturePtr ipCurrentFeature;
		ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
		if( !ipCurrentFeature )
			return FALSE;
		ipCurrentFeature->get_Table( &ipCurrentTable );
		IDatasetPtr ipDataset( ipCurrentTable );
		if( !ipDataset )
			return FALSE;
		ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			return FALSE;
		
		// REFERENCE.ROAD_CODE_LISTテーブル取得
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("ROAD_CODE_LIST");
		else
			strOwnerName += _T(".ROAD_CODE_LIST");
		m_ipRoadCodeListTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );
	}

	// 与えられた路線コードの路線の路線種別コードを取得
	_ICursorPtr		ipCursor;
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	CString strWhereClause, strSubFields;

	strWhereClause.Format(_T("ROAD_CODE = %ld"), nRoadCode );
	strSubFields.Format(_T("LINECLASS_C"));
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	m_ipRoadCodeListTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	_IRowPtr ipRow;

	long aLineClass = -1;	// ipCursor==NULLの場合には、未初期化のままipRoadClassが参照されてランタイムエラーが出るので、あり得ない値で初期化しておく
	if( ipCursor )
	{
		while (ipCursor->NextRow(&ipRow) == S_OK)
		{
			// ipCursorに格納されたRoad_Code_Listのレコードのうち、最初の１つからLineClass_cを取得
			aLineClass = ipRow->GetValue( ipRow->Fields->_FindField(sindy::schema::road_code_list::kLineClass) );
			break;
		}
	}

	// 路線種別コードのコード値ドメインを取得
	ATLASSERT(m_ipRoadCodeListTable);
	IFieldsPtr ipFields(m_ipRoadCodeListTable->Fields);
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
			AttrMessageBox( _T("路線種別コードのコンボボックス設定失敗"), _T("路線種別設定失敗"), MB_OK, NULL);
			return FALSE;
		}
	}
	// カーソルの設定
	cbLineClass.SetCurSel(nCurIdx);
	// 現在の路線種別の取得
	m_nLineClass = cbLineClass.GetItemData(nCurIdx);
	
	CFieldDef* pFieldDef;
	CComVariant vaRoadCode;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_CODE") );
	vaRoadCode = pFieldDef->m_vaValue;
	
	RedrawRoadNameList(vaRoadCode.lVal);

	// [Bug6217] ハイウェイノードを新規作成した時に、路線コンボボックスを変更せずにそのまま確定すると
	// 路線コードが0になってしまうのを防ぐため、ここで路線コードが更新されたものとしてCFieldDefに反映させる
	SetRoadCode(m_nRoadCode);

	return TRUE;
}

/**
 * @fn bool CAttrHwyNodeDlg::GetLineClass(int* pClass)
 * @brief 設定されている路線種別を取得する
 *
 * @param pClass	[out]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrHwyNodeDlg::GetLineClass(int* pClass)
{
	if( !pClass )
		return false;
	// コンボボックスの取得
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));

	// インデックスの取得
	int nIndex = cbLineClass.GetCurSel();
	if(nIndex == CB_ERR)
		return false;

	// 値の取得
	*pClass = cbLineClass.GetItemData(nIndex);
	return true;
}

/**
 * @fn bool CAttrHwyNodeDlg::SetLineClass(int nClass)
 * @brief 路線種別の値からコンボボックスを設定
 *
 * @param nClass	[in]	路線コードの値
 *
 * @return	路線種別が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrHwyNodeDlg::SetLineClass(int nClass)
{
	// コンボボックスの取得
	CComboBox cbLineClass = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADCLASS_REF));
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
 * @fn bool CAttrHwyNodeDlg::RedrawRoadNameList(LONG nRoadCode)
 * @brief 路線名称コンボボックスの更新
 *
 * @param nRoadCode	[in]	路線コードの値
 *
 * @return	路線名称が取得できたらTRUE、取得できなかったらFALSE
 */
bool CAttrHwyNodeDlg::RedrawRoadNameList(LONG nRoadCode)
{
	// [Bug6432] Road_Code_Listが読み込まれていなければ読み込む
	if (m_RoadCodeListArray.size() == 0)
	{
		ATLASSERT(m_ipRoadCodeListTable);
		_ICursorPtr ipCursor(m_ipRoadCodeListTable->_Search(IQueryFilterPtr(CLSID_QueryFilter), VARIANT_TRUE));

		// 取得したいフィールドのフィールドインデックスを求める
		using namespace sindy::schema::road_code_list;
		typedef CString FieldName;
		typedef long FieldIndex;
		typedef std::map<FieldName, FieldIndex> FieldIndexMap;
		FieldIndexMap aFieldIndexMap = boost::assign::map_list_of
			(CString(kRoadCode),	m_ipRoadCodeListTable->_FindField(kRoadCode))
			(CString(kNameKanji),	m_ipRoadCodeListTable->_FindField(kNameKanji))
			(CString(kNameYomi),	m_ipRoadCodeListTable->_FindField(kNameYomi))
			(CString(kLineClass),	m_ipRoadCodeListTable->_FindField(kLineClass));

		// 値を取得してコンテナに格納
		for (_IRowPtr ipRow; ipCursor->NextRow(&ipRow) == S_OK;)
		{
			RoadCodeListRecord aRecord;
			aRecord.m_ObjectID = ipRow->GetOID();
			aRecord.m_RoadCode	= ipRow->GetValue(aFieldIndexMap[kRoadCode]);
			VARIANT vaNameKanji = ipRow->GetValue(aFieldIndexMap[kNameKanji]);
			aRecord.m_NameKanji	= vaNameKanji.vt == VT_NULL ? "" : CT2A(vaNameKanji.bstrVal);
			VARIANT vaNameYomi = ipRow->GetValue(aFieldIndexMap[kNameYomi]);
			aRecord.m_NameYomi	= vaNameYomi.vt == VT_NULL ? "" : CT2A(vaNameYomi.bstrVal);
			aRecord.m_LineClass	= ipRow->GetValue(aFieldIndexMap[kLineClass]);
			m_RoadCodeListArray.insert(aRecord);
		}
	}
	
	// 路線名称コンボボックス
	CComboBox cbRoadName = CComboBox(GetDlgItem(IDC_COMBO_HWYNODE_ROADNAME_REF));
	// コンボボックスの情報のクリア
	cbRoadName.ResetContent();

	// コンボボックスにアイテムを追加
	int nCurIdx = 0;
	BOOST_FOREACH (const RoadCodeListRecord& aRecord, m_RoadCodeListArray)
	{
		// 路線種別コードが、現在選択中の路線種別コードと異なっていたらスキップ
		if (aRecord.m_LineClass != m_nLineClass)
			continue;

		CString aRoadNameStr;
		aRoadNameStr.Format(_T("%d : %s"), aRecord.m_RoadCode, CA2T(aRecord.m_NameKanji.c_str()));	// [Bug6217] 路線名称を併記するように変更

		int nIndex = cbRoadName.AddString(aRoadNameStr);
		if(nIndex != CB_ERR && nIndex != CB_ERRSPACE)
		{
			cbRoadName.SetItemData(nIndex, aRecord.m_RoadCode);
			if(aRecord.m_RoadCode == nRoadCode)
				nCurIdx = nIndex;
		}
		else
		{
			AttrMessageBox( AheLoadString( IDS_FAILTOSETROUTECLASS ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
			return false;
		}
	}

	// 現在のカーソルの設定
	cbRoadName.SetCurSel(nCurIdx);
	// 現在の路線コードの取得
	m_nRoadCode = cbRoadName.GetItemData(nCurIdx);
	
	return true;
}

/**
 * @fn void CAttrHwyNodeDlg::SetRoadCode(int nRoadCode)
 * @brief 路線コードの値を指定されたコードに置き換える
 *
 * @param nRoadCode	[in]	路線コードの値
 *
 * @note	[Bug6217] 路線コードのエディットボックスを削除したので、この関数の役割は値が変更されているかのチェックのみになった
 *			falseを返す可能性がなくなったので、戻り値をboolからvoidに変更
 */
void CAttrHwyNodeDlg::SetRoadCode(int nRoadCode)
{
	CFieldDef* pFieldDef;
	pFieldDef = GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("ROAD_CODE") );
	pFieldDef->m_vaValue = nRoadCode;
	if( nRoadCode != m_lOrgRoadCode)
	{
		pFieldDef->m_bChanged = TRUE;        
		SetButton( Changed() );
	}
}

/**
 * @fn BOOL CAttrHwyNodeDlg::ErrorCheck()
 * @brief エラーがないかどうか調べる
 *
 * ダイアログを閉じる際に、AttributeDlgObjから呼ばれる
 *  料金所種別、料金所機能の値にエラーがないかどうか調べる。
 *
 * @return	エラーがなかったらTRUE、エラーがあればFALSE
 */
BOOL CAttrHwyNodeDlg::ErrorCheck()
{
	using namespace sindy::schema::highway_node;
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();
		
		if( strTableName == HIGHWAY_NODE_TABLE_NAME )
		{	
			// 現在編集中の値を習得
			CFieldDef* pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, sindy::schema::category::highway_in_out_table::kInOut );
			long lInOut_c = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollClass );
			long lTollClass_c = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollFunc );
			long lTollFunc_c = pFieldDef->m_vaValue.lVal;
			
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kApproachNodeID );
			CComVariant vaFromNodeID = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kFlowNodeID );
			CComVariant vaToNodeID = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kHwyMode );
			long lHwyNode = pFieldDef->m_vaValue.lVal;
			
			// [bug7177]料金所レーン情報を取得
			CString strTollGateLane;
			if(TollGateLaneFieldExist()){	// まずは料金所レーン情報フィールドがあるかチェック
				pFieldDef = it->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollGateLane );
				if(pFieldDef->m_vaValue.vt == VT_BSTR)	// フィールドがあってもNULL値を許可しているためVT_EMPTYかチェック
					strTollGateLane = pFieldDef->m_vaValue.bstrVal;
			}

			BOOL bIsError = FALSE;
			
			LONG lOID = 0;
			it->GetFeature()->get_OID(&lOID);

			CString strMessage;
			// [bug7177]HWYノードに対し料金所レーン情報を整備する際、規定値外で警告を出す
			if(TollGateLaneFieldExist() && !strTollGateLane.IsEmpty()){
				// 指定の文字群（0～5）以外があると文字列長以下の出力となる
				if(_tcsspn(strTollGateLane, _T("012345")) != strTollGateLane.GetLength()){
					strMessage.Format( AheLoadString( IDS_USABLENUMBER ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
			}

			// 流入、流出ノードのチェック
			if( lHwyNode == 0 )
			{
				// [Bug 5507]ハイウェイノードの流入・流出ノードIDは空ではいけない
				if( vaFromNodeID.vt == VT_EMPTY || vaToNodeID.vt == VT_EMPTY )
				{
					strMessage.Format(AheLoadString( IDS_INVALIDNODE ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
				if( vaFromNodeID.lVal == vaToNodeID.lVal )
				{
					strMessage.Format( AheLoadString( IDS_SAMEINOUTNODE ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
			}
			else if( lHwyNode == 1 )
			{
				if( !(vaFromNodeID.lVal == 0 && vaToNodeID.lVal == 0 ))
				{
					strMessage.Format( AheLoadString( IDS_INVALIDNODE ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return FALSE;
				}
			}

			// ETC料金所コードのチェック
			// フィールド存在確認とかでネストが深くなりそうなのでラムダにしておく
			auto isExistTollgateID = [&](){
				// フィールドがなければスルー
				auto fdTollgateID = it->GetFieldDef( kTableName, kTollgateID );
				if( !fdTollgateID )
					return true;

				// VT_NULLを考慮
				CString tollGateID = uh::str_util::ToString(fdTollgateID->m_vaValue);

				// ETC非対応ならETC料金所コードはNULLでなければダメ
				auto fdFacilC = it->GetFieldDef( kTableName, kEtcFacil );
				if( fdFacilC && fdFacilC->m_vaValue.lVal == etc_facil::kNoEtcFacil )
				{ 
					if( !tollGateID.IsEmpty() )
					{
						// ETC施設コード「ETC非対応」なのにETC料金所コードが入力されています
						AttrMessageBox( 
							uh::str_util::format( AheLoadString( IDS_INVALID_ETC_CODE ), lOID ),
							AheLoadString( IDS_CAPTION ), MB_OK, NULL);
						return false;
					}
				}

				// ETC非対応以外で、ETC料金所コードがnullならスルー
				if( tollGateID.IsEmpty() )
					return true;

				// ETC_TOLLGATEテーブルに存在するコードか
				ITablePtr etcTollgateT = AheOpenSameOwnerTable( 
					ITablePtr(AheGetFeatureClass( it->GetFeature() )), etc_tollgate::kTableName );
				if( !etcTollgateT )
				{
					// ETC_TOLLGATE テーブルを開けませんでした。
					AttrMessageBox( 
						uh::str_util::format( AheLoadString( IDS_FAILED_OPEN_TOLLGATE ), tollGateID ),
						AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return false;
				}

				auto query = AheInitQueryFilter( 
					nullptr,
					etc_tollgate::kTollgateID,
					uh::str_util::format( _T("%s = '%s'"), etc_tollgate::kTollgateID, tollGateID ) );

				long num(0);
				etcTollgateT->RowCount( query, &num );
				if(num == 0)
				{
					// ETC_TOLLGATEに存在しないETC料金所コードです。
					AttrMessageBox( 
						uh::str_util::format( AheLoadString( IDS_NOTEXISTTOLLGATEID ), lOID, tollGateID ),
						AheLoadString( IDS_CAPTION ), MB_OK, NULL);
					return false;
				}

				return true;
			};

			if( !isExistTollgateID() )
				return FALSE;
		}
	}
	return TRUE;
}

/**
 * @brief 与えられた料金所種別コードの値が「本線上にあるもの」かそれ以外かを判定する
 * @note	本線上にあるのは「本線上料金所」「バリア」
 * @note	分岐合流コードと料金所種別コードの整合チェックに必要
 * @return	料金所種別コードの値が「本線上にあるもの」であるか否か
 */
bool IsMainLineTollClass(long cTollClass)
{
	bool aResult = false;
	using namespace sindy::schema::highway_node::toll_class;
	switch (cTollClass)
	{
		case kMainTollbooth:
		case kBarrier:
			aResult = true;
			break;
		case kNoTollbooth:
		case kRampTollbooth:
		case kGateway:
			break;
		default:
			ATLASSERT(0);
	}

	return aResult;
}

/**
 * @brief 与えられた料金所種別コードの値が「料金所を表すもの」かそれ以外かを判定する
 * @note	料金所を表すのは「本線上料金所」「ランプ上料金所」「バリア」
 * @note	料金所種別コードと料金所機能コードの整合チェックに必要
 * @return	料金所種別コードの値が「料金所を表すもの」であるか否か
 */
bool IsTollBoothTollClass(long cTollClass)
{
	bool aResult = false;
	using namespace sindy::schema::highway_node::toll_class;
	switch (cTollClass)
	{
		case kMainTollbooth:
		case kRampTollbooth:
		case kBarrier:
			aResult = true;
			break;
		case kNoTollbooth:
		case kGateway:
			break;
		default:
			ATLASSERT(0);
	}

	return aResult;
}


/**
 * @brief 一般車両対応フラグに従属するコントロール群の整合チェック
 * @note	CAttrHwyNodeDlg::CheckItems()で使用するために作成
 * @note	引数のうち、CControlDef*はチェック対象コントロール（エラーだと赤くなる）、それ以外はその判断基準となるフィールドの値
 * @param[in]	cCarFlag	一般車両対応フラグ
 * @param[in]	cTollClass	料金所種別コード
 * @param[in]	cTollFunc	料金所機能コード
 * @param[in]	cNoGuideToll	非誘導料金所フラグ
 * @param[in]	cpTollFuncDef	cTollFuncに対応するコントロール
 * @param[in]	cpNoGuideTollDef	cNoGuideTollに対応するコントロール
 */
void CheckDependentControlsOnNoEtcCarFlag(
	bool cNoEtcCarFlag,
	long cTollClass,
	long cTollFunc,
	bool cNoGuideToll,
	CControlDef* cpTollFuncDef,
	CControlDef* cpNoGuideTollDef)
{
	using namespace sindy::schema::highway_node;
	if (cNoEtcCarFlag)
	{
		if (IsTollBoothTollClass(cTollClass))
		{
			using namespace toll_func;
			if (cTollClass == toll_class::kBarrier)
			{
				switch (cTollFunc)
				{
					case kNoFunc:
					case kCheckTicket:
					case kAloneInvalidTicket:
						cpTollFuncDef->SetErrored(FALSE);
						break;
					default:
						cpTollFuncDef->SetErrored(TRUE);
				}
			}
			else
			{
				switch (cTollFunc)
				{
					case kTicket:
					case kAdjust:
					case kAlone:
					case kAloneTicket:
					case kAloneAdjust:
					case kCheckTicket:
					case kAloneInvalidTicket:
					case kAdjustTicket:
						cpTollFuncDef->SetErrored(FALSE);
						break;
					case kNoFunc:
						cpTollFuncDef->SetErrored(TRUE);
						break;
					default:
						ATLASSERT(0);
				}
			}
			cpNoGuideTollDef->SetErrored(FALSE);
		}
		else
		{
			cpTollFuncDef->SetErrored(cTollFunc != toll_func::kNoFunc ? TRUE : FALSE);
			cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
		}
	}
	else
	{
		cpTollFuncDef->SetErrored(cTollFunc != toll_func::kNoFunc ? TRUE : FALSE);
		cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
	}
}

/**
 * @brief ETC施設コードに従属するコントロール群の整合チェック
 * @note	CAttrHwyNodeDlg::CheckItems()で使用するために作成
 * @note	引数のうち、CControlDef*はチェック対象コントロール（エラーだと赤くなる）、それ以外はその判断基準となるフィールドの値
 * @param[in]	cEtcFacilCode	ETC施設コード
 * @param[in]	cTollClass	料金所種別コード
 * @param[in]	cTollFunc	料金所機能コード(ETC)
 * @param[in]	cNoGuideToll	非誘導料金所フラグ(ETC)
 * @param[in]	cpTollFuncDef	cTollFuncに対応するコントロール
 * @param[in]	cpNoGuideTollDef	cNoGuideTollに対応するコントロール
 */
void CheckDependentControlsOnEtcFacilCode(
	long cEtcFacilCode,
	long cTollClass,
	long cTollFunc,
	bool cNoGuideToll,
	CControlDef* cpTollFuncDef,
	CControlDef* cpNoGuideTollDef)
{
	using namespace sindy::schema::highway_node;
	switch (cEtcFacilCode)
	{
		using namespace etc_facil;
		case kNoEtcFacil:
			cpTollFuncDef->SetErrored(cTollFunc != toll_func::kNoFunc ? TRUE : FALSE);
			cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
			break;
		case kGate:
			switch (cTollClass)
			{
				using namespace toll_class;
				case kMainTollbooth:
					cpTollFuncDef->SetErrored(FALSE);
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				case kRampTollbooth:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? TRUE : FALSE);
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				case kNoTollbooth:
				case kGateway:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? FALSE : TRUE);
					cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
					break;
				case kBarrier:
					switch (cTollFunc)
					{
						using namespace toll_func;
						case kNoFunc:
						case kCheckTicket:
						case kAloneInvalidTicket:
							cpTollFuncDef->SetErrored(FALSE);
							break;
						default:
							cpTollFuncDef->SetErrored(TRUE);
					}
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				default:
					ATLASSERT(0);
			}
			break;
		case kAntenna:
			switch (cTollClass)
			{
				using namespace toll_class;
				case kMainTollbooth:
				case kRampTollbooth:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? TRUE : FALSE);
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				case kNoTollbooth:
				case kGateway:
					cpTollFuncDef->SetErrored(cTollFunc == toll_func::kNoFunc ? FALSE : TRUE);
					cpNoGuideTollDef->SetErrored(cNoGuideToll ? TRUE : FALSE);
					break;
				case kBarrier:
					switch (cTollFunc)
					{
						using namespace toll_func;
						case kNoFunc:
						case kCheckTicket:
						case kAloneInvalidTicket:
							cpTollFuncDef->SetErrored(FALSE);
							break;
						default:
							cpTollFuncDef->SetErrored(TRUE);
					}
					cpNoGuideTollDef->SetErrored(FALSE);
					break;
				default:
					ATLASSERT(0);
			}
			break;
		default:
			ATLASSERT(0);
	}
}

/**
 * @fn void CAttrHwyNodeDlg::CheckItems()
 * @brief 料金関係でエラーがないかどうか調べる
 *
 * 分岐合流コードと、料金関係の値で不整合がないかどうか調べる、エラーの際にはセルを赤色にする
 * 詳しくは[Bug1460][Bug6217]を参照のこと
 */
void CAttrHwyNodeDlg::CheckItems()
{
	using namespace sindy::schema::highway_node;

    LONG lInOut_c			= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kInOut )->m_vaValue.lVal;
    LONG lTollClass_c		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollClass )->m_vaValue.lVal;
    LONG lTollFunc_c		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kTollFunc )->m_vaValue.lVal;
	bool bNoGuideToll		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kDummyToll )->m_vaValue.lVal != 0;

	// ここからは対応表どおりに直にチェック・・・[Bug1460] → [Bug6217]でチェックを追加・修正

	// 分岐合流コードと料金所種別コード
	CControlDef* pTollClassDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLCLASS));
	switch (lInOut_c)
	{
		case in_out::kInOut:
			pTollClassDef->SetErrored(IsMainLineTollClass(lTollClass_c) ? FALSE : TRUE);
			break;
		case in_out::kIn:
		case in_out::kOut:
			pTollClassDef->SetErrored(IsMainLineTollClass(lTollClass_c) ? TRUE : FALSE);
			break;
		default:
			ATLASSERT(0);
	}

	if (EtcSpecificTollFieldsExist())
	{
		bool bNoEtcCar			= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kNoEtcCar )->m_vaValue.lVal != 0;
		LONG lEtcFacil			= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kEtcFacil )->m_vaValue.lVal;
		LONG lEtcTollFunc_c		= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kEtcTollFunc )->m_vaValue.lVal;
		bool bEtcNoGuideToll	= GetCurrentFeatureDef()->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, kEtcNoGuideToll )->m_vaValue.lVal != 0;

		// 料金所種別コードと一般車両対応フラグ＆ETC施設コード
		CControlDef* pNoEtcCarDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOETC_CAR));
		CControlDef* pEtcFacilDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_ETC_FACIL));
		switch (lTollClass_c)
		{
			using namespace toll_class;
			using namespace etc_facil;
			case kNoTollbooth:
			case kGateway:
				pNoEtcCarDef->SetErrored(bNoEtcCar ? TRUE : FALSE );
				pEtcFacilDef->SetErrored(lEtcFacil != kNoEtcFacil ? TRUE : FALSE);
				break;
			case kMainTollbooth:
			case kRampTollbooth:
			case kBarrier:
				if (! bNoEtcCar && lEtcFacil == kNoEtcFacil)
				{
					pNoEtcCarDef->SetErrored(TRUE);
					pEtcFacilDef->SetErrored(TRUE);
				}
				else
				{
					pNoEtcCarDef->SetErrored(FALSE);
					pEtcFacilDef->SetErrored(FALSE);
				}
				break;
			default:
				ATLASSERT(0);
		}

		// 一般車両対応フラグ＆それに従属するコントロールと料金所種別コード
		CheckDependentControlsOnNoEtcCarFlag(
			bNoEtcCar,
			lTollClass_c,
			lTollFunc_c,
			bNoGuideToll,
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC)),
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOGUIDETOLL))
		);

		// ETC施設コード＆それに従属するコントロールと料金所種別コード
		CheckDependentControlsOnEtcFacilCode(
			lEtcFacil,
			lTollClass_c,
			lEtcTollFunc_c,
			bEtcNoGuideToll,
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC_ETC)),
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOGUIDETOLL_ETC))
		);
	}
	else	// [Bug6198]のデータモデル変更が適用されていないデータベースの場合は、従来の方法でチェック
	{
		CControlDef* pTollFuncDef = m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC_ETC));

		// 料金所種別コードと料金所機能コード
		CheckDependentControlsOnNoEtcCarFlag(
			true,			// 一般車両対応フラグが常にONの場合と見なす
			lTollClass_c,
			lTollFunc_c,
			bNoGuideToll,
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_COMBO_TOLLFUNC)),
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_CHECK_NOGUIDETOLL))
		);
	}

	// [bug7177]料金所レーン情報のエディットボックスチェック
	// 指定値以外なら赤色にする
	if(TollGateLaneFieldExist()){
		_TCHAR aTollgateLane[m_TollGateLaneStrMaxLength + 1] = {0};
		CEdit cTollGateLaneEdit = GetDlgItem(IDC_EDIT_TOLLGATELANE);
		cTollGateLaneEdit.GetWindowText(aTollgateLane, m_TollGateLaneStrMaxLength + 1);
		CString strTollGateLane(aTollgateLane);
		if(_tcsspn(strTollGateLane, _T("012345")) != strTollGateLane.GetLength())
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_EDIT_TOLLGATELANE))->SetErrored(TRUE);
		else
			m_cControlRel.GetControlDef(::GetDlgItem(m_hWnd, IDC_EDIT_TOLLGATELANE))->SetErrored(FALSE);
	}
}

/**
 * @brief 全てのコントロールを編集不可状態にする
 * @note	CAttrHwyNodeDlg::SetInOutNodeComboBox()中に３回出てきたので関数化
 * @noet	流入・流出ノードのコンボボックスも含めて、全ての編集対象コントロールをDisableにする
 */
void CAttrHwyNodeDlg::DisableAllControls()
{	
	SetDlgStatus( IDC_CHECK_ISHWMODE, FALSE );
	SetDlgStatus( IDC_COMBO_HWYNODE_ROADCLASS_REF, FALSE );
	SetDlgStatus( IDC_COMBO_HWYNODE_ROADNAME_REF, FALSE );
	SetDlgStatus( IDC_COMBO_SEQUENCE, FALSE );
	SetDlgStatus( IDC_EDIT_HWYGUIDE_NUM, FALSE );
	SetDlgStatus( IDC_COMBO_DIRECTION, FALSE );
	SetDlgStatus( IDC_COMBO_JUNCTION, FALSE );
	SetDlgStatus( IDC_CHECK_DUMMY, FALSE );
	SetDlgStatus( IDC_CHECK_GUIDE, FALSE );
	SetDlgStatus( IDC_COMBO_TOLLCLASS, FALSE );
	SetDlgStatus( IDC_COMBO_TOLLFUNC, FALSE );
	SetDlgStatus( IDC_EDIT_SOURCE, FALSE, TRUE );
	SetDlgStatus( IDC_CHECK_NOETC_CAR, FALSE );
	SetDlgStatus( IDC_COMBO_ETC_FACIL, FALSE );
	SetDlgStatus( IDC_COMBO_TOLLFUNC_ETC, FALSE );
	SetDlgStatus( IDC_CHECK_NOGUIDETOLL_ETC, FALSE );
	SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
	SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
	SetDlgStatus( IDC_EDIT_TOLLGATELANE, FALSE );
}

/**
 * @fn void CAttrHwyNodeDlg::SetInOutNodeComboBox()
 * @brief 流出、流入ノードのコンボボックスに、接続しているノードの一覧をセットする
 *
 * ハイウェイノードの下にある、道路ノードと接続している道路リンクと接続している道路ノードをコンボボックスに入れる
 */
void CAttrHwyNodeDlg::SetInOutNodeComboBox()
{
	m_ipFromRoadGeom = NULL;
	m_ipToRoadGeom = NULL;
	m_ipDummyFrom = NULL;
	m_ipDummyTo = NULL;
	
	// チェックボックスの状態取得
	CButton	cHwyCheckBox = CButton(GetDlgItem(IDC_CHECK_ISHWMODE));
	INT iCheckBox = cHwyCheckBox.GetCheck();
	
	// 現在選択中のフィーチャを取得
	CFeatureDef* ipFeatureDef = GetCurrentFeatureDef();
	IFeaturePtr ipFeature = ipFeatureDef->GetFeature();
	// ジオメトリ取得
	IGeometryPtr ipGeometry;
	ipFeature->get_Shape( &ipGeometry );
	
	// 現在の流入、流出ノードを取得
	CFieldDef* pFieldDef;
	pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("APPROACHNODE_ID"));
	long lFromNodeID = pFieldDef->m_vaValue.lVal;
	pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("FLOWNODE_ID"));
	long lToNodeID = pFieldDef->m_vaValue.lVal;
	
	// 流入ノードコンボボックス
	CComboBox cFromNodeID = CComboBox(GetDlgItem(IDC_COMBO_FROMNODE_ID));
	CComboBox cToNodeID = CComboBox(GetDlgItem(IDC_COMBO_TONODE_ID));
	// コンボボックスの情報のクリア
	cFromNodeID.ResetContent();
	cToNodeID.ResetContent();
	
	if( iCheckBox == 0 )
	{
		if( ipGeometry )
		{			
			// ノードレイヤを取得
			ILayerPtr ipLayer;
			CString strOwnerName = AheGetFeatureClassOwnerName( ipFeature );
			if( strOwnerName.IsEmpty() )
				strOwnerName = _T("ROAD_NODE");
			else
			{
				strOwnerName += _T(".");
				strOwnerName += _T("ROAD_NODE");
			}
			ipLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );
			// レイヤが取得できなかった場合には、パーソナルジオデータベースの可能性あり
			if( ipLayer == NULL )
				AheGetLayerByTableName( m_cArcHelper.GetMap(), _T("Road_Node"), &ipLayer );
			
			if ( ipLayer )
			{
				CTargetItems cItems;
				// ハイウェイの下のノードフィーチャ取得
				AheSelectByShapeFromLayer( ipGeometry, cItems, ipLayer );

				LONG lCount = cItems.size();
					
				if( lCount == 0 )
				{
					AttrMessageBox( AheLoadString( IDS_NOTONNODE ), AheLoadString( IDS_CAPTION ),MB_OK, NULL);
					
					// とりあえず、今現在の流入、流出ノードを表示しておく
					CString strOID;
					strOID.Format(_T("%ld"),lToNodeID );
					int iComboNum = cToNodeID.AddString(strOID);
					cToNodeID.AddString(strOID);
					cToNodeID.SetItemData(iComboNum,lToNodeID);
					cToNodeID.SetCurSel(iComboNum);
					SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
					
					strOID.Format(_T("%ld"),lFromNodeID );
					iComboNum = cFromNodeID.AddString(strOID);
					cFromNodeID.AddString(strOID);
					cFromNodeID.SetItemData(iComboNum,lFromNodeID);
					cFromNodeID.SetCurSel(iComboNum);
					SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
					
					//すべての属性を編集不可状態にする
					DisableAllControls();
				}
				// 通常ハイウェイノードの下には1個のノードがある
				else if( lCount == 1 )
				{
					// フィーチャを取り出す
					m_ipNodeUnderHwyNode = (cItems.begin())->GetFeature();
					LONG lNodeIDonHwyNode;
					m_ipNodeUnderHwyNode->get_OID( &lNodeIDonHwyNode );
					
					CTargetItems cTargetRoads;
					AheGetConnectedRoad( m_ipNodeUnderHwyNode, cTargetRoads );
					
					for(CTargetItems::iterator iRoad = cTargetRoads.begin(); iRoad != cTargetRoads.end(); iRoad++)
					{
						// ハイウェイノードに接続している道路を取得
						IFeaturePtr	ipRoadFeature = iRoad->GetFeature();
						
						CTargetItems cNodeItems;
						// 道路リンクについているノードを取得
						AheGetConnectedNode( ipRoadFeature, cNodeItems );
						
						for(CTargetItems::iterator iNode = cNodeItems.begin(); iNode != cNodeItems.end(); iNode++ )
						{
							IFeaturePtr ipConnectedNode = iNode->GetFeature();
							LONG lOID = 0;
							ipConnectedNode->get_OID( &lOID );
							
							if( lOID != lNodeIDonHwyNode )
							{
								CString strOID;
								strOID.Format(_T("%ld"),lOID );
								
								int iComboNum = cFromNodeID.AddString(strOID);
								cToNodeID.AddString(strOID);
								cFromNodeID.SetItemData(iComboNum,lOID);
								cToNodeID.SetItemData(iComboNum,lOID);
								
								if( lFromNodeID == lOID )
								{
									cFromNodeID.SetCurSel(iComboNum);
									m_cControlRel.ControlChanged( IDC_COMBO_FROMNODE_ID );
								}
								if( lToNodeID == lOID )
								{
									cToNodeID.SetCurSel(iComboNum);
									m_cControlRel.ControlChanged( IDC_COMBO_TONODE_ID );
								}
							}
						}
					}
				}
			}
			else
			{
				AttrMessageBox( AheLoadString( IDS_CANTEDITATTRIBUTE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL);
				
				// とりあえず、今現在の流入、流出ノードを表示しておく
				CString strOID;
				strOID.Format(_T("%ld"),lToNodeID );
				int iComboNum = cToNodeID.AddString(strOID);
				cToNodeID.AddString(strOID);
				cToNodeID.SetItemData(iComboNum,lToNodeID);
				cToNodeID.SetCurSel(iComboNum);
				SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
				
				strOID.Format(_T("%ld"),lFromNodeID );
				iComboNum = cFromNodeID.AddString(strOID);
				cFromNodeID.AddString(strOID);
				cFromNodeID.SetItemData(iComboNum,lFromNodeID);
				cFromNodeID.SetCurSel(iComboNum);
				SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
				
				//すべての属性を編集不可状態にする
				DisableAllControls();
			}
		}
	}
	// ハイウェイモードフラグがオンのとき（流入、流出フラグdisable）
	else
	{
		CString strOID;
		strOID.Format(_T("%ld"),0 );
		
		int iComboNum = cFromNodeID.AddString(strOID);
		cToNodeID.AddString(strOID);
		cFromNodeID.SetItemData(iComboNum,0);
		cToNodeID.SetItemData(iComboNum,0);
		cFromNodeID.SetCurSel(iComboNum);
		cToNodeID.SetCurSel(iComboNum);
		m_cControlRel.ControlChanged( IDC_COMBO_FROMNODE_ID );
		m_cControlRel.ControlChanged( IDC_COMBO_TONODE_ID );
		// [Bug 5509]関連　ここで流入・流出ノードコンボボックスを制御しないといけない
		// いままでOnCommandでやっていたが、基本はControlRelでやってもらうことにしたので、ここでやらないといけなくなった
		SetDlgStatus( IDC_COMBO_FROMNODE_ID, FALSE );
		SetDlgStatus( IDC_COMBO_TONODE_ID, FALSE );
		
		if( lFromNodeID != 0 || lToNodeID != 0 )
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		if( !m_ipNodeUnderHwyNode )
		{
			AttrMessageBox( AheLoadString( IDS_NOTONNODE ), AheLoadString( IDS_CAPTION ),MB_OK, NULL);
			//すべての属性を編集不可状態にする
			DisableAllControls();
		}
	}
}

/**
 * @fn void CAttrHwyNodeDlg::DrawInOutNode()
 * @brief 流出、流入ノードへ矢印を描画する
 */
void CAttrHwyNodeDlg::DrawInOutNode()
{
	// 道路とノードのレイヤが表示されているかどうかチェック
	ILayerPtr ipRoadLayer, ipNodeLayer;

	// ノードレイヤを取得
	CString strOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_NODE");
	else
	{
		strOwnerName += _T(".");
		strOwnerName += _T("ROAD_NODE");
	}
	ipNodeLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );

	// 道路リンクレイヤを取得
	strOwnerName = AheGetFeatureClassOwnerName( GetCurrentFeatureDef()->GetFeature() );
	if( strOwnerName.IsEmpty() )
		strOwnerName = _T("ROAD_LINK");
	else
	{
		strOwnerName += _T(".");
		strOwnerName += _T("ROAD_LINK");
	}
	ipRoadLayer = AheGetLayerByName( m_cArcHelper.GetMap(), strOwnerName, _FIND_TABLE, FALSE, TRUE );

	// 現在選択されているレイヤが表示されていなければキャンセル
	VARIANT_BOOL vaIsVisible = VARIANT_FALSE;
	
	if( !ipRoadLayer )
	{
		AttrMessageBox( AheLoadString( IDS_NOTEXISTROADLAYER ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}
	ipRoadLayer->get_Visible( &vaIsVisible );
	if( !vaIsVisible )
	{
		AttrMessageBox( AheLoadString( IDS_NOTDISPLAYROADLAYER ) , AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}

	if( !ipNodeLayer )
	{
		AttrMessageBox( AheLoadString( IDS_NOTEXISTNODELAYER ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}
	ipNodeLayer->get_Visible( &vaIsVisible );
	if( !vaIsVisible )
	{
		AttrMessageBox( AheLoadString( IDS_NOTDISPLAYNODELAYER ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_LOW );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		return;
	}
	

	// ノードテーブルを取得していなければ、取得する
	if( !m_ipRoadNodeTable )
	{
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

		// NODE_TABLE_NAME取得
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentFeature );
		if( strOwnerName.IsEmpty() )
			strOwnerName = _T("ROAD_NODE");
		else
			strOwnerName += _T(".ROAD_NODE");

		m_ipRoadNodeTable = AheOpenTableByTableName( ipWorkspace, strOwnerName );
	}
	
	// 現在選択中のフィーチャを取得
	CFeatureDef* ipFeatureDef = GetCurrentFeatureDef();
	
	// 現在の流入、流出ノードを取得
	CFieldDef* pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("APPROACHNODE_ID"));
	long lFromNodeID = pFieldDef->m_vaValue.lVal;
	pFieldDef = ipFeatureDef->GetFieldDef( HIGHWAY_NODE_TABLE_NAME, _T("FLOWNODE_ID"));
	long lToNodeID = pFieldDef->m_vaValue.lVal;
	
	
	if( lFromNodeID > 0 && lFromNodeID == lToNodeID )
	{
		m_ipFromRoadGeom = NULL;
		m_ipToRoadGeom = NULL;
		
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
		AttrMessageBox( AheLoadString( IDS_SAMEINOUTNODEID ), AheLoadString( IDS_CAPTION ),MB_OK, NULL );
		return;
	}
	
	// 選択されたノードのジオメトリ取得
	// 流入の道路ジオメトリ取得
	m_ipFromRoadGeom = GetConnectedRoad( lFromNodeID, TRUE, ipFeatureDef );
	// 流出の道路ジオメトリ取得
	m_ipToRoadGeom = GetConnectedRoad( lToNodeID, FALSE, ipFeatureDef );

	// 流入、流出ライン描画
	Refresh();
}

/**
 * @fn IGeometryPtr CAttrHwyNodeDlg::GetConnectedRoad( LONG lNodeID, CFeatureDef* ipFeatureDef )
 * @brief 流入、流出の道路リンクのジオメトリを取得する
 *
 * @param lNodeID			[in]	流出、または流入ノードID
 * @param bIsIN				[in]	流出か流入か（流入の時TRUE）
 * @param ipFeatureDef		[in]	ハイウェイノードのフィーチャデフ
 *
 * @return ipRoadGeometry	流出、または流入の道路のジオメトリ
 */
IGeometryPtr CAttrHwyNodeDlg::GetConnectedRoad( LONG lNodeID, BOOL bIsIN, CFeatureDef* ipFeatureDef)
{
    _ICursorPtr			ipCursor;
    IQueryFilterPtr		ipQueryFilter( CLSID_QueryFilter );
    CString			strWhereClause, strSubFields;
	// オブジェクトIDから道路を検索
	strWhereClause.Format(_T("OBJECTID = %ld"), lNodeID );
	strSubFields.Format(_T("OBJECTID,Shape"));
	
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	m_ipRoadNodeTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );
	
    _IRowPtr ipRow = NULL;
    CComVariant vaRoad_Class(_T(""));

	IGeometryPtr	ipRoadGeometry;
    // ノードを取得
    if( ipCursor )
    {
        while( ipCursor->NextRow( &ipRow ) == S_OK )
        {
            if( !ipRow ) 
				break;
			
			// コンボボックスで設定したノードを取得
			IFeaturePtr	ipSelectedNode;
			ipSelectedNode = ipRow;
			
			LONG lSelectedNodeOID = 0;
			ipSelectedNode->get_OID(&lSelectedNodeOID);
			ipRow->get_OID(&lSelectedNodeOID);
			IGeometryPtr ipGeometry;
			ipSelectedNode->get_Shape( &ipGeometry );
			
			// ハイウェイノードの下にあるノードのオブジェクトID取得
			// 現在選択中のフィーチャを取得
			IFeaturePtr ipFeature = ipFeatureDef->GetFeature();
			
			LONG lNodeUnderHwyNodeOID = 0;
			ipFeature->get_OID(&lNodeUnderHwyNodeOID);
			
			// ハイウェイノードがノードの上にないことがあるので、その場合は以下のif文にいれたらだめ
			if( m_ipNodeUnderHwyNode )
			{
				CTargetItems cTargetRoads;
				AheGetConnectedRoad( m_ipNodeUnderHwyNode, cTargetRoads);
				
				IFeaturePtr ipSelectedRoad;
				BOOL bIsFromNodeWithSelectedRoad = FALSE;
				for(CTargetItems::iterator iRoad = cTargetRoads.begin(); iRoad != cTargetRoads.end(); iRoad++)
				{
					// ハイウェイノードに接続している道路を取得
					IFeaturePtr	ipRoadFeature = iRoad->GetFeature();
					
					IFieldsPtr	ipFields;
					LONG		lIndex = 0;
					CComVariant	vaValue;
					
					// 道路のFromノードを取得
					ipRoadFeature->get_Fields( &ipFields );
					ipFields->FindField(CComBSTR(_T("FROM_NODE_ID")),&lIndex);
					ipRoadFeature->get_Value( lIndex, &vaValue );
					long lFromNodeID = vaValue.lVal;
					
					// 道路のToノードを取得
					ipFields->FindField(CComBSTR(_T("TO_NODE_ID")),&lIndex);
					ipRoadFeature->get_Value( lIndex, &vaValue );
					long lToNodeID = vaValue.lVal;
					
					if( lSelectedNodeOID == lFromNodeID )
					{
						if( bIsIN )
							bIsFromNodeWithSelectedRoad = TRUE;
						else
							bIsFromNodeWithSelectedRoad = FALSE;

						ipSelectedRoad = ipRoadFeature;
						break;
					}
					else if( lSelectedNodeOID == lToNodeID )
					{
						if( bIsIN )
							bIsFromNodeWithSelectedRoad = FALSE;
						else
							bIsFromNodeWithSelectedRoad = TRUE;

						ipSelectedRoad = ipRoadFeature;
						break;
					}
				}
				IGeometryPtr ipRoadGeom;
				ipSelectedRoad->get_ShapeCopy( &ipRoadGeom );
				
				if( bIsFromNodeWithSelectedRoad )
					ipRoadGeometry = ipRoadGeom;
				else
				{					
					IPolylinePtr ipPolyline;
					ipPolyline = ipRoadGeom;
					
					ipPolyline->ReverseOrientation();
					ipRoadGeometry = ipPolyline;
				}
			}
		}
    }
	return ipRoadGeometry;
}

/**
 * @fn void CAttrHwyNodeDlg::Refresh()
 * @brief 描画をリフレシュする
 */
void CAttrHwyNodeDlg::Refresh()
{
	if( m_ipFromRoadGeom )
	{
		AheDrawArrowline( m_cArcHelper.GetScreenDisplay(), m_ipFromRoadGeom, RGB( 0, 255, 0 ), TRUE, TRUE );
		if( m_ipFromRoadGeom != m_ipDummyFrom && m_ipFromRoadGeom != m_ipToRoadGeom )
		{
			m_ipDummyFrom = m_ipFromRoadGeom;
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}
	if( m_ipToRoadGeom )
	{
		AheDrawArrowline( m_cArcHelper.GetScreenDisplay(), m_ipToRoadGeom, RGB( 0, 255, 255 ), TRUE, TRUE );
		if( m_ipToRoadGeom != m_ipDummyTo  && m_ipFromRoadGeom != m_ipToRoadGeom )
		{
			m_ipDummyTo = m_ipToRoadGeom;
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		}
	}
}

/**
 * @brief 一般車両対応フラグの状態によって、依存するコントロールのEnable/Disableを切り替える
 * @note	[Bug 6217]で追加
 */
void CAttrHwyNodeDlg::ChangeDependentControlsOnNoEtcCarFlag()
{
	// ETC距離別料金関連のフィールドが存在しない場合は何もしない
	if (! EtcSpecificTollFieldsExist())
		return;

	// 一般車両対応フラグがOFFならそれに依存するコントロールをDisableに、ONならEnableにする
	CButton	aNoEtcCarCheckBox(GetDlgItem(IDC_CHECK_NOETC_CAR));
	if (aNoEtcCarCheckBox.GetCheck() == BST_UNCHECKED)
	{
		// 一般車両対応フラグがOFFになったら、依存するコントロールの値を初期値（＝OFFの時に許される値）にリセットする
		CButton(GetDlgItem(IDC_CHECK_DUMMY)).SetCheck(BST_UNCHECKED);
		CComboBox(GetDlgItem(IDC_COMBO_TOLLFUNC)).SetCurSel(0);

		m_cControlRel.ControlChanged( IDC_CHECK_DUMMY );
		m_cControlRel.ControlChanged( IDC_COMBO_TOLLFUNC );
	}
}

/**
 * @brief ETC施設コードの状態によって、依存するコントロールのEnable/Disableを切り替える
 * @note	[Bug 6217]で追加
 */
void CAttrHwyNodeDlg::ChangeDependentControlsOnEtcFacilCode()
{
	// ETC距離別料金関連のフィールドが存在しない場合は何もしない
	if (! EtcSpecificTollFieldsExist())
		return;

	// ETC施設コードが「ETC非対応」ならそれに依存するコントロールをDisableに、それ以外ならEnableにする
	CComboBox aEtcFacilCodeComboBox(GetDlgItem(IDC_COMBO_ETC_FACIL));
	int aSelectedIndex = aEtcFacilCodeComboBox.GetCurSel();
	if (aSelectedIndex != CB_ERR)
	{
		long aSelectedValue = aEtcFacilCodeComboBox.GetItemData(aSelectedIndex);
		if(aSelectedValue == sindy::schema::highway_node::etc_facil::kNoEtcFacil)
		{
			// ETC施設コードが「ETC非対応」になったら、依存するコントロールの値を初期値（＝ETC非対応の時に許される値）にリセットする
			CButton(GetDlgItem(IDC_CHECK_NOGUIDETOLL_ETC)).SetCheck(BST_UNCHECKED);
			CComboBox(GetDlgItem(IDC_COMBO_TOLLFUNC_ETC)).SetCurSel(0);

			m_cControlRel.ControlChanged( IDC_CHECK_NOGUIDETOLL_ETC );
			m_cControlRel.ControlChanged( IDC_COMBO_TOLLFUNC_ETC );
		}
	}
}

/**
 * @brief 路線コードの値に従ってハイウェイモードフラグを変更
 * @note	[Bug 6217]対応時に追加
 * @param[in]	cRoadCode	路線コード
 */
void CAttrHwyNodeDlg::ChangeHwyModeCheckBox(long cRoadCode)
{
	// 路線コードが200000以上ならハイウェイモードフラグをON、200000未満ならOFFにする
	CButton(GetDlgItem(IDC_CHECK_ISHWMODE)).SetCheck(m_nRoadCode >= 200000 ? BST_CHECKED : BST_UNCHECKED);
	m_cControlRel.ControlChanged( IDC_CHECK_ISHWMODE );

	SetInOutNodeComboBox();
}

/**
 * @brief 料金所レーン情報のイメージの描画
 */
void CAttrHwyNodeDlg::DrawLaneImage(void)
{
	// ピクチャコントロールのウィンドウのデバイスコンテキストハンドルを取得
	CWindow aLaneImgWindow = GetDlgItem(IDC_PICTURE_LANEIMG);
	CDC aDC(aLaneImgWindow.GetDC());

	// ピクチャコントロールの位置に白い矩形を表示
	CRect aLaneImageRect(0, 0, 256, 64);
	aDC.SelectStockPen(WHITE_PEN);
	aDC.Rectangle(aLaneImageRect);

	// 罫線を描画
	aDC.SelectStockPen(BLACK_PEN);
	aDC.SelectStockBrush(NULL_BRUSH);
	aDC.Rectangle(aLaneImageRect);
	aDC.MoveTo(aLaneImageRect.left, aLaneImageRect.top + 32);
	aDC.LineTo(aLaneImageRect.right, aLaneImageRect.top + 32);

	// フォントの設定
	HFONT aOrgFont = aDC.SelectFont(m_TollGateLaneImgFont);

	// レーン番号を表す文字を表示
	for (long i=0; i<m_TollGateLaneStrMaxLength; ++i)
	{
		long aCurrentColumn	= i % m_TollGateLaneImageColumnMaxNum;
		long aCurrentRow	= i / m_TollGateLaneImageColumnMaxNum;

		CRect aLaneNumRect;
		aLaneNumRect.left	= aLaneImageRect.left + 16 * aCurrentColumn;
		aLaneNumRect.top	= aLaneImageRect.top + 32 * aCurrentRow;
		aLaneNumRect.right	= aLaneNumRect.left + 16;
		aLaneNumRect.bottom	= aLaneNumRect.top + 16;

		// [Bug7173] ロケールに依存しない書き方に修正
		CString aLaneNumStr;
		aLaneNumStr.Format(_T("%d"), i+1 );

		aDC.DrawText(aLaneNumStr, aLaneNumStr.GetLength(), aLaneNumRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	// フォントを元に戻す
	aDC.SelectFont(aOrgFont);

	// 料金所レーン情報エディットコントロールから文字列を取得
	std::string aTollGateLaneStr;
	{
		_TCHAR aTollGateLaneStrTmp[m_TollGateLaneStrMaxLength + 1];
		CEdit aTollGateLaneEdit = GetDlgItem(IDC_EDIT_TOLLGATELANE);
		aTollGateLaneEdit.GetWindowText(aTollGateLaneStrTmp, m_TollGateLaneStrMaxLength + 1);
		aTollGateLaneStr = CT2A(aTollGateLaneStrTmp);
	}

	// 各ビットマップを割り当てるためのメモリデバイスコンテキスト群を作成
	typedef boost::array<CDC, tollgate_lane_type::tollgate_lane_type_num> MemoryDCArray;
	MemoryDCArray aMemoryDCArray;
	BOOST_FOREACH (CDC& rCDC, aMemoryDCArray)
	{
		rCDC = CreateCompatibleDC(aDC);
	}

	aMemoryDCArray[tollgate_lane_type::impassable	].SelectBitmap(m_ImpassableImg);
	aMemoryDCArray[tollgate_lane_type::public_lane	].SelectBitmap(m_PublicImg);
	aMemoryDCArray[tollgate_lane_type::etc_lane		].SelectBitmap(m_EtcImg);
	aMemoryDCArray[tollgate_lane_type::mixed_lane	].SelectBitmap(m_MixedImg);
	aMemoryDCArray[tollgate_lane_type::switch_lane	].SelectBitmap(m_SwitchImg);
	aMemoryDCArray[tollgate_lane_type::unknown_lane	].SelectBitmap(m_UnknownImg);
	aMemoryDCArray[tollgate_lane_type::invalid		].SelectBitmap(m_InvalidImg);

	// イメージの表示
	for (unsigned long i = 0; i < aTollGateLaneStr.size(); ++i)
	{
		// 着目中の文字から表示すべきビットマップを決定
		long aTollGateLaneType = boost::lexical_cast<long>(aTollGateLaneStr[i]);
		if (aTollGateLaneType >= tollgate_lane_type::invalid)
		{
			aTollGateLaneType = tollgate_lane_type::invalid;
		}

		long aCurrentColumn	= i % m_TollGateLaneImageColumnMaxNum;
		long aCurrentRow	= i / m_TollGateLaneImageColumnMaxNum;
		aDC.BitBlt(
			16 * aCurrentColumn,
			16 + 32 * aCurrentRow,
			16,
			16,
			aMemoryDCArray[aTollGateLaneType],
			0,
			0,
			SRCAND
		);
	}
}


