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

// AttrRoadLinkDlg.cpp: CAttrRoadLinkDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrRoadLinkDlg.h"
#include <ArcHelperEx/Map.h>
#include <TDC/sindylib_core/FieldMap.h>

#include "RoadMap.h"	// 道路リンクダイアログのウィンドウ配置マップ
#include <WinLib/PixTwip.h>
#include <WinLib/atlwfile.h>
#include <boost/assign.hpp> // for boost::assign

using namespace sindy::schema;
namespace
{
	// 都道府県コード格納map
	const std::map<int, CString> prefCodeMap = boost::assign::map_list_of
		(0,  _T(""))
		(1,  _T("北海道"))
		(2,  _T("青森県"))
		(3,  _T("岩手県"))
		(4,  _T("宮城県"))
		(5,  _T("秋田県"))
		(6,  _T("山形県"))
		(7,  _T("福島県"))
		(8,  _T("茨城県"))
		(9,  _T("栃木県"))
		(10, _T("群馬県"))
		(11, _T("埼玉県"))
		(12, _T("千葉県"))
		(13, _T("東京都"))
		(14, _T("神奈川県"))
		(15, _T("新潟県"))
		(16, _T("富山県"))
		(17, _T("石川県"))
		(18, _T("福井県"))
		(19, _T("山梨県"))
		(20, _T("長野県"))
		(21, _T("岐阜県"))
		(22, _T("静岡県"))
		(23, _T("愛知県"))
		(24, _T("三重県"))
		(25, _T("滋賀県"))
		(26, _T("京都府"))
		(27, _T("大阪府"))
		(28, _T("兵庫県"))
		(29, _T("奈良県"))
		(30, _T("和歌山県"))
		(31, _T("鳥取県"))
		(32, _T("島根県"))
		(33, _T("岡山県"))
		(34, _T("広島県"))
		(35, _T("山口県"))
		(36, _T("徳島県"))
		(37, _T("香川県"))
		(38, _T("愛媛県"))
		(39, _T("高知県"))
		(40, _T("福岡県"))
		(41, _T("佐賀県"))
		(42, _T("長崎県"))
		(43, _T("熊本県"))
		(44, _T("大分県"))
		(45, _T("宮崎県"))
		(46, _T("鹿児島県"))
		(47, _T("沖縄県"))
		;
}

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrRoadLinkDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrRoadLinkDlg();
}

// 路線名称コードに対する「code検索」ボタンが押された時の処理 [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchRoadCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_ROADCODE))
		SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
	return 0;
}

// 都道府県コードに対する「code検索」ボタンが押された時の処理 [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchPrefButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_PREFCODE)) {
		SetPrefCodeFromCityAdmin(IDC_EDIT_PREFCODE);
		SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);
	}
	return 0;
}

// 道路番号に対する「No.検索」ボタンが押された時の処理 [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchRoadNumberButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_PREFCODE) && CheckNumVal(IDC_EDIT_ROADNUMBER))
		SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);
	return 0;
}

// 区間別称コードに対する「code検索」ボタンが押された時の処理 [bug 12036]
LRESULT CAttrRoadLinkDlg::OnClickedSearchSectionCodeButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(CheckNumVal(IDC_EDIT_SECTIONCODE))
		SetSectionName(IDC_EDIT_SECTIONCODE, IDC_EDIT_SECTIONNAME);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrRoadLinkDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseMeshDlg::CAttrBaseMeshDlg()
 * @brief コンストラクタ
 *
 * ダイアログタイプの設定と、ブラシの初期化、その他道路リンク関連フラグをしておきます。
 */
CAttrRoadLinkDlg::CAttrRoadLinkDlg() 
	: ATTR_BASE_CLASS(KIHONMAP, IDD_ATTRROADLINKDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_DISPCLASS ),
		m_wnd1( _T("COMBOBOX"), this, IDC_COMBO_NAVICLASS ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_MANAGER ), 
		m_wnd3( _T("COMBOBOX"), this, IDC_COMBO_ROADWIDTH ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_LINKCLASS1 ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_COMBO_BATCHCHANGE ),	// [bug 9397] 
		m_wnd6( _T("EDIT"), this, IDC_EDIT_SOURCE ),

		m_wndTuika0( _T("CHECKBOX"), this, IDC_CHECK_ISLANDF ),
		m_wndTuika1( _T("CHECKBOX"), this, IDC_CHECK_BYPASS ),
		m_wndTuika2( _T("CHECKBOX"), this, IDC_CHECK_CARONLYF ),
		m_wndTuika3( _T("COMBOBOX"), this, IDC_COMBO_WIDEAREA ),
		m_wndTuika4( _T("COMBOBOX"), this, IDC_COMBO_UPDOWNC ),
		m_wndTuika5( _T("CHECKBOX"), this, IDC_CHECK_LANE ),
		m_wndTuika6( _T("EDIT"), this, IDC_EDIT_LANE ),
		m_wndTuika7( _T("EDIT"), this, IDC_EDIT_RIGHTCHANGE ),
		m_wndTuika8( _T("EDIT"), this, IDC_EDIT_LEFTCHANGE ),
		m_wndTuika9( _T("COMBO"), this, IDC_COMBO_ALLEYMAINTE ),
		m_wndTuika10( _T("EDIT"), this, IDC_EDIT_WORKYEAR ),
		m_wndTuika11( _T("CHECKBOX"), this, IDC_CHECK_WIDEAREAF ),
		m_wndTuika12( _T("COMBO"), this, IDC_COMBO_LEGALSPEED ),
		m_wndTuika13( _T("COMBO"), this, IDC_COMBO_WALKABLE ),

		m_wndKisei0( _T("COMBOBOX"), this, IDC_COMBO_ONEWAY ),
		m_wndKisei1( _T("COMBOBOX"), this, IDC_COMBO_NOPASSAGE ),

		m_wndRosen0(_T("EDIT"),   this, IDC_EDIT_ROADCODE),			   // [bug 12036]
		m_wndRosen1(_T("BUTTON"), this, IDC_BUTTON_SEARCHROADCODE),	   // [bug 12036]
		m_wndRosen2(_T("EDIT"),   this, IDC_EDIT_PREFCODE),			   // [bug 12036]
		m_wndRosen3(_T("BUTTON"), this, IDC_BUTTON_SEARCHPREFCODE),	   // [bug 12036]
		m_wndRosen4(_T("EDIT"),   this, IDC_EDIT_ROADNUMBER),		   // [bug 12036]
		m_wndRosen5(_T("BUTTON"), this, IDC_BUTTON_SEARCHROADNUMBER),  // [bug 12036]
		m_wndRosen6(_T("EDIT"),   this, IDC_EDIT_HIGHWAYNUMBER),	   // [bug 12036]
		m_wndRosen7(_T("EDIT"),   this, IDC_EDIT_SECTIONCODE),		   // [bug 12036]
		m_wndRosen8(_T("BUTTON"), this, IDC_BUTTON_SEARCHSECTIONCODE), // [bug 12036]

		m_wndDRMA0( _T("COMBOBOX"), this, IDC_COMBO_KUBUNCODE ),
		m_wndDRMA1( _T("COMBOBOX"), this, IDC_COMBO_LIMITSPEED ),
		m_wndDRMA2( _T("EDIT"), this, IDC_EDIT_TRIPSPEED ),

		m_wndParking0( _T("COMBOBOX"), this, IDC_COMBO_PARKINGCLASS ),
		m_wndParking1( _T("COMBOBOX"), this, IDC_COMBO_PARKINGLINKCLASS ),
		m_wndParking2( _T("EDIT"), this, IDC_EDIT_FLOORLEVEL ),
		m_wndParking3( _T("EDIT"), this, IDC_EDIT_MIDDLEFLOOR_LEVEL ),

		m_wndTab( _T("TAB"), this, IDC_TAB_ROADATTR )
{
	// タブ位置の初期化
	m_lTabNum = 0;
	m_iChangeType = 0;
	m_lCurrentNopassageIndex = 0;
	m_lCurrentOnewayIndex = 0;

	// 一括変更用の定義名インデックスの初期化 [bug 9397]
	m_nIdx = -1;
}

/**
 * @fn void CAttrRoadLinkDlg::LoadIniFile()
 * @brief 一括変更用のiniファイルを読み込み、内部に属性を保持する [bug 9397] 
 *
 */
void CAttrRoadLinkDlg::LoadIniFile()
{
	// レジストリからiniファイルパス取得
	GetRegCode();

	CFile	IniFile;
	if( IniFile.Open( m_strIniPath ) )
	{
		CString	strBuff;
		int key = 0;

		// ヘッダ読み飛ばし
		IniFile.GetLineText( strBuff );

		while( IniFile.GetLineText( strBuff ) ){

			int curPos = 0;

			// タブ区切りの属性値を保持
			BatchChange obj;
			obj.NAME          = strBuff.Tokenize( _T("\t"), curPos );
			obj.DISPCLASS     = strBuff.Tokenize( _T("\t"), curPos );
			obj.NAVICLASS     = strBuff.Tokenize( _T("\t"), curPos );
			obj.MANAGER       = strBuff.Tokenize( _T("\t"), curPos );
			obj.ROADWIDTH     = strBuff.Tokenize( _T("\t"), curPos );
			obj.LINKCLASS     = strBuff.Tokenize( _T("\t"), curPos );
			obj.LANE          = strBuff.Tokenize( _T("\t"), curPos );
			obj.LEGALSPEED    = strBuff.Tokenize( _T("\t"), curPos );
			obj.ALLEYMAINTE   = strBuff.Tokenize( _T("\t"), curPos );
			obj.ROADCODE      = strBuff.Tokenize( _T("\t"), curPos );
			obj.ROADNUMBER    = strBuff.Tokenize( _T("\t"), curPos );
			obj.KUBUNCODE     = strBuff.Tokenize( _T("\t"), curPos );
			obj.LIMITSPEED    = strBuff.Tokenize( _T("\t"), curPos );
			obj.TRIPSPEED     = strBuff.Tokenize( _T("\t"), curPos );

			m_mapBatchChange.insert( pair<int, BatchChange>(key++, obj) );
		}
	}

}

/**
 * @fn void CAttrRoadLinkDlg::GetRegCode()
 * @brief レジストリからiniファイルのアドレスを読み込み [bug 9397] 
 *
 */
void CAttrRoadLinkDlg::GetRegCode()
{
	// レジストリからiniファイルパスを取得
	HKEY hOpenKey = EmxCreateRegKey( _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\AttributeTool"), HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		TCHAR strIniPath[255] = _T("");

		if( EmxGetRegString( _T("RoadLinkBatchIni"), strIniPath, 255, hOpenKey ))
			m_strIniPath = strIniPath;

		RegCloseKey( hOpenKey );
	}


}

/**
 * @fn void CAttrRoadLinkDlg::AddComboValue()
 * @brief 定義名を一括変更用のコンボボックスに格納する [bug 9397] 
 *
 */
void CAttrRoadLinkDlg::AddComboValue()
{
	// コンボボックスに値を入れる
	CComboBox cCombo( GetDlgItem( IDC_COMBO_BATCHCHANGE ) );
	cCombo.ResetContent();

	INT iCurPos = 0;
	for( std::map<int, BatchChange>::const_iterator it = m_mapBatchChange.begin(); it != m_mapBatchChange.end(); ++it )
	{
		iCurPos = cCombo.AddString( it->second.NAME );
		cCombo.SetItemData( iCurPos, it->first );
	}
	cCombo.SetCurSel( m_nIdx );
}

/**
 * @fn void CAttrRoadLinkDlg::SetBatchValue(INT nID, INT nCpyID)
 * @brief 一括変更用に1属性ずつセットする [bug 9397] 
 *
 * @param	nID		[in]		設定するコントロールID		
 * @param	Value	[in]		設定する値
 */
void CAttrRoadLinkDlg::SetBatchValue(INT nID, CString& Value)
{
	// 「-」は既存値を継承するため、属性セットを行わない
	if( Value != _T("-") )
	{
		CControlDef* pControlDef = m_cControlRel.GetControlDef( ::GetDlgItem(m_hWnd, nID ) );
		CComVariant vaValue = _ttoi(Value);
		pControlDef->SetValue(vaValue);
		pControlDef->SetComboBox(vaValue.intVal);
	}
}

/** 
 * @fn HWND CAttrRoadLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrRoadLinkDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	// 親ウィンドをキープしとく
	m_hAttributeDlgWnd = hWndParent;

	try{
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	} catch(...) {
	}
	return m_hChildWnd;
}

/**
 * @fn void CAttrRoadLinkDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrRoadLinkDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrRoadLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * また、前回ダイアログを開いていたときのタブの状態が路線だった場合には、SetRoadNameで路線検索もしておく
 * 2次メッシュを跨いでいる道路リンクが選択されている場合には、関連する道路リンクも確保する
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrRoadLinkDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 

    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, m_lTabNum, bForce, bEditable );

	m_lFeatureIndex = lFeatureIndex;
	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
	m_bForce = bForce;
	m_bEditable = bEditable;

	if( m_lTabNum == 3) 
	{
		// 道路名称を出す [bug 12036]
		SetRoadName(IDC_EDIT_ROADCODE, IDC_EDIT_NAMEKANJI, IDC_EDIT_DISPLAYKANJI, IDC_EDIT_SHIELDNUMBER);
		m_bRoadCordFirstSearch = TRUE;

		// 都道府県名を出す [bug 12036]
		SetPrefName(IDC_EDIT_PREFCODE, IDC_EDIT_PREFNAME);

		// 県道名称を出す [bug 12036]
		SetPrefRoadName(IDC_EDIT_PREFCODE, IDC_EDIT_ROADNUMBER, IDC_EDIT_PREFNAME, IDC_EDIT_PREFROADNAME);

		// 区間別称を出す [bug 12036]
		SetSectionName(IDC_EDIT_SECTIONCODE, IDC_EDIT_SECTIONNAME);
	}

	// 道路リンクは、2次メッシ境界を跨いでいるかどうかを調べる必要があり、

	// 2次メッシュ境界跨いでいるかどうかフラグの初期化
	m_bIsRoadOnMesh = FALSE;
	m_pRoadOnMeshList.clear();

	// 現在選択中のフィーチャ取得
	IFeaturePtr		ipFeature = GetCurrentFeatureDef()->GetFeature();

	CTargetItems cRoadCrossMesh;
	cRoadCrossMesh.clear();

	// 2次メッシュを跨いでいるかどうかをチェックし、跨いでいれば接続している道路を取得
	if( AheIsRoadCrossMesh( m_cArcHelper.GetMap(), ipFeature, cRoadCrossMesh ) )
	{
		// 2次メッシュを跨いでいるのでフラグ変更
		m_bIsRoadOnMesh = TRUE;

		// 選択された、2次メッシュ関連の道路を登録する
		for( CTargetItems::iterator it = cRoadCrossMesh.begin(); it != cRoadCrossMesh.end(); it++ )
		{
			IFeaturePtr ipFeatureOnMesh = it->GetFeature();
			CFeatureDef featDef;
			featDef.SetArcHelper( m_cArcHelper.GetApp() );
			featDef.SetFeature( ipFeatureOnMesh );
			m_pRoadOnMeshList.push_back(featDef);
		}
	}

	// 一通と通禁は編集できないようにDISABLEに
	SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
	SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );

	// ダイアログ開くときにはエラーメッセージは出さずにタブのハイライトだけにとどめる
	// 道路リンクリレーションをチェックしタブのハイライトをする
	CString strMessage;
	CheckRoadLinkRelation( &strMessage, GetCurrentFeatureDef());

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		SetForcusAfterTabChange();
	}

	// 一括変更用ini読み込み [bug 9397] 
	LoadIniFile();

	// 読み込んだiniからコンボにキーを追加 [bug 9397] 
	AddComboValue();

	// どこかの値が変更されている場合は一括変更コンボを使用不可とする [bug 9449]
	if( Changed() )
		SetDlgStatus(IDC_COMBO_BATCHCHANGE, FALSE );

	AdjustTuikaTabLayout();

    return bRet;
}

// [bug 6712] WIDEAREA_Fフィールドの有無によってレイアウトを切り替える
void CAttrRoadLinkDlg::AdjustTuikaTabLayout()
{
	WINRECT *map = NULL;		// WinMgr制御のコントロール取得用
	bool bIsWideAreaF = false;
	if( GetCurrentFeatureDef()->GetFieldDef( _T("ROAD_LINK"), _T("WIDEAREA_F") ) )
	{
		// フィールドがある時はコンボボックスとスタティックコントロールは非表示
		::ShowWindow( GetDlgItem( IDC_COMBO_WIDEAREA ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_WIDEAREA ), SW_HIDE );
		map = m_winMgr.FindRect( IDC_STATIC_WIDEAREA );
		bIsWideAreaF = true;
	}else{
		// フィールドが無い場合はチェックボックスを非表示
		::ShowWindow( GetDlgItem( IDC_CHECK_WIDEAREAF ), SW_HIDE );
		map = m_winMgr.FindRect( IDC_CHECK_WIDEAREAF );
	}

	// 法定速度(LEGALSPEED_C)、歩行可能種別(WALKABLE_C)がない場合には、DISABLEにしておく
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && m_bEditable )
	{
		if( !GetCurrentFeatureDef()->GetFieldDef( _T("ROAD_LINK"), _T("LEGALSPEED_C") ) )
			SetDlgStatus( IDC_COMBO_LEGALSPEED, FALSE );

		if( !GetCurrentFeatureDef()->GetFieldDef( _T("ROAD_LINK"), _T("WALKABLE_C") ) )
			SetDlgStatus( IDC_COMBO_WALKABLE, FALSE );
	}

	// フィールドがあればチェックボックスを、無ければスタティックコントロールを配置
	if( map )
	{
		map->SetID( bIsWideAreaF ? IDC_CHECK_WIDEAREAF : IDC_STATIC_WIDEAREA );
		// 再配置
		m_winMgr.InitToFitSizeFromCurrent(this);
		m_winMgr.CalcLayout(this);
		m_winMgr.SetWindowPositions(this);	
	}
}


/**
 * @fn LONG CAttrRoadLinkDlg::GetTabNum()
 * @brief 現在のタブ位置を渡します
 *
 * @retval 現在のタブ位置
 */
LONG CAttrRoadLinkDlg::GetTabNum()
{
	return m_lTabNum;
}

/**
 * @fn void CAttrRoadLinkDlg::SetTabNum(LONG lTabNum)
 * @brief タブ位置をセットします
 *
 * @param lTabNum	[in]	タブ位置
 */
void CAttrRoadLinkDlg::SetTabNum(LONG lTabNum)
{
	m_lTabNum = lTabNum;
	m_iChangeType = lTabNum;
}

/**
 * @fn void CAttrRoadLinkDlg::ChangeWindowMgrMap( INT iIndex )
 * ウィンドウマネージャのマップの切り替えを行う
 *
 * @param iIndex	[in]	マップのインデックス番号
 */
void CAttrRoadLinkDlg::ChangeWindowMgrMap( INT iIndex )
{
	switch( iIndex ) {
	case EDITVIEW_ROAD_KIHON:		// 基本属性タブ
		m_winMgr.ChangeMgrMap(KIHONMAP);
		break;
	case EDITVIEW_ROAD_TUIKA:		// 追加属性タブ
		m_winMgr.ChangeMgrMap( TUIKAMAP );
		break;
	case EDITVIEW_ROAD_KOTUKISEI:	// 交通規制タブ
		m_winMgr.ChangeMgrMap(KOTUKISEIMAP);
		break;
	case EDITVIEW_ROAD_ROSEN:		// 路線番号タブ 
		m_winMgr.ChangeMgrMap(ROSENMAP);
		break;
	case EDITVIEW_ROAD_DRMA:		// DRMAタブ 
		m_winMgr.ChangeMgrMap(DRMAMAP);
		break;
	case EDITVIEW_ROAD_PARKING:		// 駐車場タブ 
		m_winMgr.ChangeMgrMap(PARKINGMAP);
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
 * @fn void CAttrRoadLinkDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrRoadLinkDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		list<CString> array;
		
		// 道路の基本属性タブ
		m_cControlRel.SetControl( IDC_TAB_ROADATTR,		TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_PARKING );
		m_cControlRel.SetControl( IDC_COMBO_BATCHCHANGE,TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);	// [bug 9397] 
		m_cControlRel.SetControl( IDC_STATIC_BATCHCHANGE,TYPE_OTHER,	EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_DISPCLASS,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("ROADCLASS_C"), NULL, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_DISPCLASS,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_NAVICLASS,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("NAVICLASS_C"), CHECK_DISPNAVI, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_NAVICLASS,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_MANAGER,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("ROADMANAGER_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_MANAGER,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_ROADWIDTH,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON, _T("ROADWIDTH_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_ROADWIDTH,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_COMBO_LINKCLASS1,	TYPE_COMBO,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON,  _T("MAIN_LINKCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LINKCLASS,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_KIHON);
		m_cControlRel.SetControl( IDC_STATIC_SOURCE,	TYPE_OTHER,		EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,EDITVIEW_ROAD_KIHON, EDITVIEW_ROAD_PARKING, _T("SOURCE"), NULL, _T("ROAD_LINK"));

		// 道路の追加属性タブ
		m_cControlRel.SetControl( IDC_CHECK_ISLANDF,		TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("ISLAND_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_BYPASS,			TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("BYPASS_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_CARONLYF,		TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("CARONLY_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_WIDEAREAF,		TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WIDEAREA_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_COMBO_WIDEAREA,		TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WIDEAREA_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_WIDEAREA,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_STATIC_UPDOWNC,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_UPDOWNC,		TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("UPDOWNCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LANE,			TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_LANE,			TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LANE_COUNT"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_CHECK_LANE,			TYPE_CHECKBOX,	EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LANEVALID_F"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_RIGHTCHANGE,	TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_RIGHTCHANGE,		TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("RIGHT_CHANGE"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LEFTCHANGE,	TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_LEFTCHANGE,		TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LEFT_CHANGE"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_ALLEYCODE,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_ALLEYMAINTE,	TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("ALLEYMAINTE_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_WORKYEAR,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_EDIT_WORKYEAR,		TYPE_EDIT,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WORK_YEAR"), NULL, _T("ROAD_LINK") );

		// 道路の交通規制タブ
		m_cControlRel.SetControl( IDC_STATIC_ONEWAYGROUP,		TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERONEWAY,		TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_COMBO_ONEWAY,				TYPE_COMBO,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, _T("ONEWAY_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		array.clear();
		array.push_back( _T("LINKDIR_C") );
		array.push_back( _T("START_MONTH") );
		array.push_back( _T("START_DAY") );
		array.push_back( _T("END_MONTH") );
		array.push_back( _T("END_DAY") );
		array.push_back( _T("START_HOUR") );
		array.push_back( _T("START_MIN") );
		array.push_back( _T("END_HOUR") );
		array.push_back( _T("END_MIN") );
		array.push_back( _T("DAYOFWEEK_C") );
		m_cControlRel.SetControl( IDC_GRID_ONEWAY,				TYPE_OWTIMEREG,	EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, array, NULL, TIME_ONEWAY_TABLE_NAME, _T("LINK_ID"), IDC_COMBO_ONEWAY, FOLLOW_VALUE );
		m_cControlRel.SetControl( IDC_STATIC_NOPASSAGEGROUP,	TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_STATIC_REGULERNOPASSAGE,	TYPE_OTHER,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI);
		m_cControlRel.SetControl( IDC_COMBO_NOPASSAGE,			TYPE_COMBO,		EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, _T("NOPASSAGE_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		array.clear();
		array.push_back( _T("START_MONTH") );
		array.push_back( _T("START_DAY") );
		array.push_back( _T("END_MONTH") );
		array.push_back( _T("END_DAY") );
		array.push_back( _T("START_HOUR") );
		array.push_back( _T("START_MIN") );
		array.push_back( _T("END_HOUR") );
		array.push_back( _T("END_MIN") );
		array.push_back( _T("DAYOFWEEK_C") );
		array.push_back( _T("SEASONREG_F") );
		m_cControlRel.SetControl( IDC_GRID_NOPASSAGE,			TYPE_TIMEREG,	EDITVIEW_ROAD_KOTUKISEI, EDITVIEW_ROAD_KOTUKISEI, array, NULL, TIME_NOPASSAGE_TABLE_NAME, _T("LINK_ID"), IDC_COMBO_NOPASSAGE, FOLLOW_VALUE );

		// 道路の路線タブ [bug 12036]
		m_cControlRel.SetControl(IDC_STATIC_ROADCODE,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_ROADCODE,				TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kRoadCode, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHROADCODE,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_NAMEKANJI,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_NAMEKANJI,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_DISPLAYKANJI,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_DISPLAYKANJI,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_SHIELDNUMBER,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_SHIELDNUMBER,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_PREFCODE,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_PREFCODE,				TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kPrefCode, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHPREFCODE,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_PREFNAME,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_PREFNAME,				TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_ROADNUMBER,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_ROADNUMBER,			TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kRoadNo, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_STATIC_ROADNUMBER2,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHROADNUMBER,	TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_PREFROADNAME,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_PREFROADNAME,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_HIGHWAYNUMBER,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_HIGHWAYNUMBER,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_SECTIONCODE,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_SECTIONCODE,			TYPE_EDIT,  EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN, road_link::kSectionCode, CHECK_DISPOTHER, road_link::kTableName);
		m_cControlRel.SetControl(IDC_BUTTON_SEARCHSECTIONCODE,	TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_STATIC_SECTIONNAME,		TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl(IDC_EDIT_SECTIONNAME,			TYPE_OTHER, EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl( IDC_STATIC_BAR,				TYPE_OTHER,	EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl( IDC_STATIC_BAR2,				TYPE_OTHER,	EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);
		m_cControlRel.SetControl( IDC_STATIC_BAR3,				TYPE_OTHER,	EDITVIEW_ROAD_ROSEN, EDITVIEW_ROAD_ROSEN);

		// 道路のDRMAタブ
		m_cControlRel.SetControl( IDC_STATIC_KUBUNCODE,		TYPE_OTHER,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA);
		m_cControlRel.SetControl( IDC_COMBO_KUBUNCODE,		TYPE_COMBO,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA, _T("KUBUN_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_LIMITSPEED,	TYPE_OTHER,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA);
		m_cControlRel.SetControl( IDC_COMBO_LIMITSPEED,		TYPE_COMBO,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA, _T("LIMITSPEED_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_TRIPSPEED,		TYPE_OTHER,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA);
		m_cControlRel.SetControl( IDC_EDIT_TRIPSPEED,		TYPE_EDIT,		EDITVIEW_ROAD_DRMA, EDITVIEW_ROAD_DRMA, _T("TRIP_SPEED"), CHECK_DISPOTHER, _T("ROAD_LINK") );

		//駐車場タブ
		m_cControlRel.SetControl( IDC_STATIC_PARKINGCLASS,		TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_COMBO_PARKINGCLASS,		TYPE_COMBO,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("PARKINGCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK"));
		m_cControlRel.SetControl( IDC_STATIC_PARKINGLINKCLASS,	TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_COMBO_PARKINGLINKCLASS,	TYPE_COMBO,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("PARKINGLINKCLASS_C"), CHECK_DISPOTHER, _T("ROAD_LINK"));
		m_cControlRel.SetControl( IDC_STATIC_FLOORLEVEL,		TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_EDIT_FLOORLEVEL,			TYPE_EDIT,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("FLOOR_LEVEL"), CHECK_DISPOTHER, _T("ROAD_LINK"));
		m_cControlRel.SetControl( IDC_STATIC_MIDDLEFLOOR_LEVEL,	TYPE_OTHER,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING);
		m_cControlRel.SetControl( IDC_EDIT_MIDDLEFLOOR_LEVEL,	TYPE_EDIT,		EDITVIEW_ROAD_PARKING, EDITVIEW_ROAD_PARKING, _T("MIDDLEFLOOR_LEVEL"), CHECK_DISPOTHER, _T("ROAD_LINK") );

		// [bug 8087] 実際にフィールドが存在しない場合、最後に追加しておかなければリレーションチェックで不整合が起こる
		m_cControlRel.SetControl( IDC_STATIC_LEGALSPEED,		TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_LEGALSPEED,			TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("LEGALSPEED_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
		m_cControlRel.SetControl( IDC_STATIC_WALKABLE,			TYPE_OTHER,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA);
		m_cControlRel.SetControl( IDC_COMBO_WALKABLE,			TYPE_COMBO,		EDITVIEW_ROAD_TUIKA, EDITVIEW_ROAD_TUIKA, _T("WALKABLE_C"), CHECK_DISPOTHER, _T("ROAD_LINK") );
	}
}

// カーソルを取得する [bug 12036]
bool CAttrRoadLinkDlg::GetCursor(const ITablePtr& ipTable, _ICursorPtr& ipCursor, const CString& strWhereClause, const CString& strSubFields)
{
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter(nullptr, strSubFields, _T("%s"), strWhereClause);
	ipTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor);

	return (ipCursor);
}

// エディットボックスに入力された数値を取得する [bug 12036]
long CAttrRoadLinkDlg::GetWindowNumberText(int nCodeID)
{
	CWindow edit(GetDlgItem(nCodeID));
	CString strCodeVal;
	edit.GetWindowText(strCodeVal);
	return _ttol(strCodeVal);
}

// 路線検索を行い、路線名を表示する [bug 12036]
void CAttrRoadLinkDlg::SetRoadName(int nCodeID, int nNameLabelID, int nDispLabelID, int nShieldNumberLabelID)
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// ROAD_CODE_LISTテーブル取得
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), road_code_list::kTableName);
	if (!ipTable) return;

	// 路線番号コードをエディットボックスから取得
	long lRoadCode = GetWindowNumberText(nCodeID);

	// クエリ文を生成
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kRoadCode, lRoadCode);
	strSubFields.Format(_T("%s,%s,%s,%s,%s"),
		road_code_list::kNameKanji, road_code_list::kNameYomi,
		road_code_list::kDisplayKanji, road_code_list::kDisplayYomi, road_code_list::kShieldNoStr);

	// カーソル取得
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))){
		AttrMessageBox(AheLoadString(IDS_FAILTOSEARCH), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi,
				vaDispKanji, vaDispYomi, vaShieldNumber;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, road_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, road_code_list::kNameYomi);
		vaDispKanji = AheGetAttribute(ipRow, road_code_list::kDisplayKanji);
		vaDispYomi = AheGetAttribute(ipRow, road_code_list::kDisplayYomi);
		vaShieldNumber = AheGetAttribute(ipRow, road_code_list::kShieldNoStr);
	}

	// 検索結果の各文字列を出力用コントロールに書き込み
	CString strNameText, strDispText, strNumText;

	strNameText.Format(_T("%s(%s)"), uh::str_util::ToString(vaKanjiName), uh::str_util::ToString(vaKanjiYomi));
	::SetWindowText(GetDlgItem(nNameLabelID), strNameText);

	strDispText.Format(_T("%s(%s)"), uh::str_util::ToString(vaDispKanji), uh::str_util::ToString(vaDispYomi));
	::SetWindowText(GetDlgItem(nDispLabelID), strDispText);

	strNumText.Format(_T("%s"), uh::str_util::ToString(vaShieldNumber));
	::SetWindowText(GetDlgItem(nShieldNumberLabelID), strNumText);
}

// 都道府県名検索を行い、都道府県名称を表示する [bug 12036]
void CAttrRoadLinkDlg::SetPrefName(int nCodeID, int nLabelID)
{
	// 都道府県コードをエディットボックスから取得
	long lPrefCode = GetWindowNumberText(nCodeID);

	// 都道府県コードから都道府県名取得
	CString strPrefName;
	const auto& itr = prefCodeMap.find(lPrefCode);
	if (itr != prefCodeMap.end()) {
		strPrefName = itr->second;
	}

	// 取得した文字列を都道府県名（IDC_EDIT_PREFNAME）に書き込み
	::SetWindowText(GetDlgItem(nLabelID), strPrefName);
}

void CAttrRoadLinkDlg::SetPrefCodeFromCityAdmin(int nCodeID)
{
	// 都道府県コードをエディットボックスから取得
	long lPrefCode = GetWindowNumberText(nCodeID);
	if (lPrefCode == 0) lPrefCode = GetPrefCodeFromCityAdmin();

	// 都道府県コード（IDC_EDIT_PREFCODE）の書き込み
	::SetWindowText(GetDlgItem(nCodeID), uh::str_util::ToString(lPrefCode));
}

// 道路リンクが存在するCITY_ADMINのCITYCODEからPREF_CODEを取得する [bug 12036]
long CAttrRoadLinkDlg::GetPrefCodeFromCityAdmin()
{
	// 道路リンクが存在するCITY_ADMINポリゴンのCITYCODE格納用vector
	set<long> cityCodeSet;

	// 選択している道路リンクのIGeometryPtrを取得
	IFeaturePtr ipRoadLinkFeature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr ipRoadLinkGeom = AheGetShape( ipRoadLinkFeature );

	// TOCに読み込まれているCITY_ADMINレイヤ取得(複数ある場合は、一番上に追加されているレイヤ)
	CMap cMap(m_cArcHelper.GetFocusMap());
	ILayerPtr ipCityAdminLayer = cMap.FindLayer(nullptr, nullptr, city_admin::kTableName);
	if (!ipCityAdminLayer) {
		AttrMessageBox(AheLoadString(IDS_NOCITYADMIN), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return 0;
	}
	IFeatureClassPtr ipCityAdminFC = AheGetFeatureClass(ipCityAdminLayer);

	// 道路リンクが存在するCITY_ADMINポリゴンのCITYCODE取得
	IQueryFilterPtr ipQuery = AheInitQueryFilter(AheInitSpatialFilter(nullptr, ipRoadLinkGeom), city_admin::kCityCode, nullptr);

	long lFeatureCount = 0;
	ipCityAdminFC->FeatureCount(ipQuery, &lFeatureCount);
	if (lFeatureCount == 0) return 0; // 道路リンクがCITY_ADMIN上にない

	IFeatureCursorPtr ipCursor;
	ipCityAdminFC->Search(ipQuery, VARIANT_TRUE, &ipCursor);

	long lCityCodeIdx = 0;
	IFieldsPtr ipFields;
	ipCursor->get_Fields(&ipFields);
	ipFields->FindField(CComBSTR(city_admin::kCityCode), &lCityCodeIdx);

	IFeaturePtr ipCityAdminFeature;
	while (ipCursor->NextFeature(&ipCityAdminFeature) == S_OK && ipCityAdminFeature)
	{
		CComVariant vaCityCode;
		ipCityAdminFeature->get_Value(lCityCodeIdx, &vaCityCode);

		CString strCityCode;
		strCityCode.Format(_T("%s"), uh::str_util::ToString(vaCityCode));
		long lPrefCode = _ttol(strCityCode.Mid(0, 2)); // 初めの二桁だけ取得

		cityCodeSet.insert(lPrefCode);
	}

	// 道路リンクが県境にある
	if (cityCodeSet.size() > 1) {
	AttrMessageBox(AheLoadString(IDS_BEYONDPREFBORDER), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
	return 0;
	}

	if (cityCodeSet.size() == 1) return *cityCodeSet.begin();

	return 0;
}

// 都道府県道名称検索を行い、都道府県道名称を表示する [bug 12036]
void CAttrRoadLinkDlg::SetPrefRoadName(int nPrefCodeID, int nRoadNumberID, int nPrefNameLabelID, int nPrefRoadLabelID)
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// PREF_ROAD_LISTテーブル取得
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), pref_road_list::kTableName);
	if (!ipTable) return;

	// 道路種別を取得
	CFieldDef *pFieldDef = GetCurrentFeatureDef()->GetFieldDef(ROAD_TABLE_NAME, road_link::kRoadClass);
	long lRoadClass = pFieldDef->m_vaValue.lVal;

	// 都道府県コードをエディットボックスから取得
	long lPrefCode = GetWindowNumberText(nPrefCodeID);

	// 道路番号をエディットボックスから取得
	long lRoadNo = GetWindowNumberText(nRoadNumberID);

	// クエリ文を生成
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld AND %s = %ld AND %s = %ld"),
		road_link::kPrefCode, lPrefCode, road_link::kRoadNo, lRoadNo, road_link::kRoadClass, lRoadClass);
	strSubFields.Format(_T("%s,%s"), pref_road_list::kNameKanji, pref_road_list::kNameYomi);

	// カーソル取得
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		AttrMessageBox(AheLoadString(IDS_FAILTOSEARCH), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, pref_road_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, pref_road_list::kNameYomi);
	}

	// 都道府県コードから都道府県名取得
	CString strPrefName;
	const auto& itr = prefCodeMap.find(lPrefCode);
	if (itr != prefCodeMap.end()) {
		strPrefName = itr->second;
	}

	// 都道府県名（IDC_EDIT_PREFNAME）の書き込み
	::SetWindowText(GetDlgItem(nPrefNameLabelID), strPrefName);

	// 道路名称（IDC_EDIT_PREFROADNAME）の書き込み
	CString strText;
	strText.Format(_T("%s(%s)"), uh::str_util::ToString(vaKanjiName), uh::str_util::ToString(vaKanjiYomi));
	::SetWindowText(GetDlgItem(nPrefRoadLabelID), strText);
}

// 区間別称コード検索を行い、区間別称名を表示する [bug 12036]
void CAttrRoadLinkDlg::SetSectionName(int nCodeID, int nLabelID)
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// SECTION_CODE_LISTテーブル取得
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), section_code_list::kTableName);
	if (!ipTable) return;

	// 区間別称コードをエディットボックスから取得
	long lSectionCode = GetWindowNumberText(nCodeID);

	// クエリ文を生成
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kSectionCode, lSectionCode);
	strSubFields.Format(_T("%s,%s"), section_code_list::kNameKanji, section_code_list::kNameYomi);

	// カーソル取得
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		AttrMessageBox(AheLoadString(IDS_FAILTOSEARCH), AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, WARNLEVEL_HIGH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;

	while (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, section_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, section_code_list::kNameYomi);
	}

	// 検索結果の文字列を区間別称（IDC_EDIT_SECTIONNAME）に書き込み
	CString strText;
	strText.Format(_T("%s(%s)"), uh::str_util::ToString(vaKanjiName), uh::str_util::ToString(vaKanjiYomi));
	::SetWindowText(GetDlgItem(nLabelID), strText);
}

// エディットボックスに入力された数値のチェックを行う [bug 12036]
bool CAttrRoadLinkDlg::CheckNumVal(int nCodeID)
{
	// 数値をエディットボックスから取得
	CWindow edit(GetDlgItem(nCodeID));
	CString strNumVal;
	edit.GetWindowText(strNumVal);
	long lNumVal = _ttol(strNumVal);

	switch (nCodeID)
	{
	case IDC_EDIT_ROADCODE:
		if (!CheckRoadCodeSectionCode(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFROADCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	case IDC_EDIT_PREFCODE:
		if (!CheckPrefCode(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFPREFCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	case IDC_EDIT_ROADNUMBER:
		if (!CheckRoadNo(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFROADNORANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	case IDC_EDIT_SECTIONCODE:
		if (!CheckRoadCodeSectionCode(lNumVal)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFSECTIONCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			return false;
		}
		break;
	}
	return true;
}

// 路線名称コードまたは区間別称コードの値をチェック [bug 12036]
bool CAttrRoadLinkDlg::CheckRoadCodeSectionCode(long lNumVal)
{
	// "0"はOK
	if (lNumVal == 0)
		return true;

	// 入力範囲のチェック
	return (1000 <= lNumVal && lNumVal <= 999999);
}

// 都道府県コードの値をチェック [bug 12036]
bool CAttrRoadLinkDlg::CheckPrefCode(long lPrefCode)
{
	// 入力範囲のチェック
	return (0 <= lPrefCode && lPrefCode <= 47);
}

// 道路番号の値をチェック [bug 12036]
bool CAttrRoadLinkDlg::CheckRoadNo(long lRoadNo)
{
	// 道路番号のチェック
	return (0 <= lRoadNo && lRoadNo <= 9999);
}

/**
 * @fn void CAttrRoadLinkDlg::SetForcusAfterTabChange()
 * @brief 現在のタブ位置に設定します
 */
void CAttrRoadLinkDlg::SetForcusAfterTabChange()
{
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
	{
		if( m_lTabNum == 0 )
			::SetFocus( GetDlgItem(IDC_COMBO_DISPCLASS) );
		else if( m_lTabNum == 1) 
			::SetFocus( GetDlgItem(IDC_CHECK_ISLANDF) );
		else if( m_lTabNum == 2) 
		{
			SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
			SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );
			::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
		}
		else if( m_lTabNum == 3) 
			::SetFocus( GetDlgItem(IDC_EDIT_ROADCODE) );
		else if( m_lTabNum == 4) 
			::SetFocus( GetDlgItem(IDC_COMBO_KUBUNCODE) );
		else if( m_lTabNum == 5) 
			::SetFocus( GetDlgItem(IDC_COMBO_PARKINGCLASS) );
	}
	else
		::SetFocus( GetDlgItem(IDC_EDIT_SOURCE) );
}

/**
 * @fn BOOL CAttrRoadLinkDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときに道路リンクのエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CAttrRoadLinkDlg::ErrorCheck()
{
	for( list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		if( !m_ipRule )
			m_ipRule = GetRule();

		CString strTableName = it->GetTableName();

		if( strTableName == ROAD_TABLE_NAME )
		{
			// フィーチャとオブジェクトID取得
			IFeaturePtr ipFeature(it->GetFeature());
			LONG lOID = 0;
			ipFeature->get_OID( &lOID );

			// 現在編集中の値を取得
			CFieldDef* pFieldDef;
			// 作業年度のチェック
			pFieldDef = GetCurrentFeatureDef()->GetFieldDef( ROAD_TABLE_NAME, _T("WORK_YEAR") );
			CComVariant vaWorkYearValue = pFieldDef->m_vaValue;
			long lWorkYear = vaWorkYearValue.lVal;
			if( !(0 <= lWorkYear && lWorkYear < 100000) )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_OUTOFWORKYEARRANGE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;					
			}

			// VICS,インターナビVICSフラグが1の場合に、経路種別が経路外道路であった場合には、警告を出す
			pFieldDef = it->GetFieldDef( ROAD_TABLE_NAME, _T("VICS_LQRF") );
			long lVICSFlag = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( ROAD_TABLE_NAME, _T("INTERNAVI_LQRF") );
			long lInterNaviFlag = pFieldDef->m_vaValue.lVal;
			pFieldDef = it->GetFieldDef( ROAD_TABLE_NAME, _T("NAVICLASS_C") );
			long lNaviClass = pFieldDef->m_vaValue.lVal;

			if( lNaviClass == 0 && (lInterNaviFlag == 1 || lVICSFlag == 1) )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_VICSERROR ), lOID);
				INT iRet = AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO, NULL );

				if( iRet == IDNO )
					return FALSE;
			}

			// 道路種別に応じて都道府県コード、道路番号の入力をチェック
			CString strPrefRoadMsg;
			if (!(CheckPrefCodeRoadNo(strPrefRoadMsg, *it))){
				CString strError;
				strError.Format(AheLoadString(IDS_ERRORANDWORNING), lOID);
				strError += strPrefRoadMsg;
				AttrMessageBox(strError, AheLoadString(IDS_CAPTION), MB_OK | MB_ICONSTOP, NULL);
				return FALSE;
			}

			// 道路リンクリレーションシップのチェック
			CString strMessage;
			LONG lCheckRoadLinkRelation = CheckRoadLinkRelation( &strMessage, &(*it) );

			if( lCheckRoadLinkRelation == 2 )
			{
				strMessage += AheLoadString( IDS_RELATIONERROR );
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
			else if( lCheckRoadLinkRelation == 1 )
			{
				strMessage += AheLoadString( IDS_RELATIONWORNING );
				INT iRet = AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO, NULL );
			
				if( iRet == IDNO )
					return FALSE;
			}

			// 番号タブに該当名称がない項目はないかチェック(保存は可能)
			CString strNonExistsNameMsg;
			strNonExistsNameMsg.Empty();
			if (!(CheckNameExists(strNonExistsNameMsg, *it))){
				CString strWarning;
				strWarning.Format(AheLoadString(IDS_NONAMEWARNING), lOID);
				strWarning += strNonExistsNameMsg;
				INT iRet = AttrMessageBox(strWarning, AheLoadString(IDS_CAPTION), MB_YESNO, NULL);

				if (iRet == IDNO)
					return FALSE;
			}
		}
	}

	return TRUE;
}

// 道路種別に応じて都道府県コード、道路番号の入力に問題がないかをチェックする [bug 12036]
bool CAttrRoadLinkDlg::CheckPrefCodeRoadNo(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// 都道府県コードが入力できる道路種別のコード値set
	static const std::set<long> okPrefCodeRoadClassSet = boost::assign::list_of
	(road_link::road_class::kPrefectureMain)
	(road_link::road_class::kPrefectureNormal)
	(road_link::road_class::kTollCountry)
	(road_link::road_class::kTollPrefectureMain)
	(road_link::road_class::kTollPrefectureNormal)
	;

	// 道路番号が入力できる道路種別のコード値set
	static const std::set<long> okRoadNoRoadClassSet = boost::assign::list_of
	(road_link::road_class::kCountry)
	(road_link::road_class::kPrefectureMain)
	(road_link::road_class::kPrefectureNormal)
	(road_link::road_class::kTollCountry)
	(road_link::road_class::kTollPrefectureMain)
	(road_link::road_class::kTollPrefectureNormal)
	;

	// 道路種別を取得
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(ROAD_TABLE_NAME, road_link::kRoadClass);
	long lRoadClass = pFieldDef->m_vaValue.lVal;

	// コード値を取得
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kPrefCode);
	// ROAD_LINKのフィールドにPREF_CODEが定義されていない場合に落ちないように
	if(!pFieldDef) return true;
	long lPrefCode = pFieldDef->m_vaValue.lVal;
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kRoadNo);
	long lRoadNo = pFieldDef->m_vaValue.lVal;

	// 道路番号、都道府県コードが共に0の場合はreturn
	if (lPrefCode == 0 && lRoadNo == 0)
		return true;

	// 道路種別が(1,2,4,103,104,106)以外のとき道路番号が入力されていないか
	auto& itrRoadNo = okRoadNoRoadClassSet.find(lRoadClass);
	if (itrRoadNo == okRoadNoRoadClassSet.end() && lRoadNo != 0) {
		strMsg += AheLoadString(IDS_CANTSPECIFIEDROADNO);
	}

	// 道路種別が(2,4,103,104,106)以外のとき都道府県コードが入力されていないか
	auto& itrPrefCode = okPrefCodeRoadClassSet.find(lRoadClass);
	if (itrPrefCode == okPrefCodeRoadClassSet.end() && lPrefCode != 0) {
		strMsg += AheLoadString(IDS_CANTSPECIFIEDPREFCODE);
	}

	return strMsg.IsEmpty();
}

/**
 * @fn LONG CAttrRoadLinkDlg::CheckRoadLinkRelation(CString *strMessage, CFeatureDef *pFeatureDef)
 * @brief 道路リンクリレーションでエラーがないかどうかチェックする
 *
 * @param strMessage		[out]	エラーメッセージの内容
 *
 * @return	0:エラーなし、チェックする必要もなかったとき
 * @return	1:ワーニングがあるとき
 * @return	2:エラーがあるとき
 */
LONG CAttrRoadLinkDlg::CheckRoadLinkRelation(CString *strMessage, CFeatureDef *pFeatureDef)
{
	LONG lCheckResult = 0;
	BOOL bWarning = FALSE;
	BOOL bError	  = FALSE;

	CFieldDef *pFieldDef;
	CComVariant vaRoadClassValue;
	pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, _T("ROADCLASS_C") );
	// ダイアログを閉じずに、選択しなおしてフィーチャを選択したときに、ダイアログは閉じられていないので、
	// GetCurrentFeatureDef()は、まだ設定されていない？とりあえず、pFieldDefがNULLのときに、チェック
	// をする必要もないので、応急処置的に、return で返して終了しておく。[Bug2430]
	if( !pFieldDef )
		return 0;
	vaRoadClassValue = pFieldDef->m_vaValue;
	LONG lRoadClass = vaRoadClassValue.lVal;
	
	DOMAINMAP* mapDomain = pFieldDef->GetDomain();
	DOMAINMAP::iterator imap;
	CString strRoadClass;

	if( mapDomain )
	{
		for( imap = mapDomain->begin(); imap != mapDomain->end(); imap++ ) {
			if( imap->code.lVal == lRoadClass )
			{
				strRoadClass = imap->code_discription;
				break;
			}
		}

		LONG lOID = 0;
		IFeaturePtr ipFeature = pFeatureDef->GetFeature();
		ipFeature->get_OID( &lOID );

		(*strMessage).Format( AheLoadString( IDS_ERRORANDWORNING ), lOID);

		CString temp;
		temp.Format( AheLoadString( IDS_WHENROADCLASS ), strRoadClass );
		(*strMessage) += temp;


		CControlDef* def = NULL;
		for( CControlRel::iterator i = m_cControlRel.begin(); i != m_cControlRel.end(); i++ )
		{
			def = &(*i);

			if( def->m_uiCheckType == CHECK_DISPNAVI ||  def->m_uiCheckType == CHECK_DISPOTHER)
			{
				if( def->m_uiCheckType & CHECK_DISPNAVI )
				{
					sindyeDispNaviRelation nCheck;
					LONG lNaviClass_c;
					CComVariant vaNaviClassValue;

					pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, _T("NAVICLASS_C") );
					vaNaviClassValue = pFieldDef->m_vaValue;
					lNaviClass_c = vaNaviClassValue.lVal;

					mapDomain = pFieldDef->GetDomain();
					CString strNaviClass;
					for( imap = mapDomain->begin(); imap != mapDomain->end(); imap++ ) {
						if( imap->code.lVal == lNaviClass_c )
						{
							strNaviClass = imap->code_discription;
							break;
						}
					}

					m_ipRule->GetDispNaviRule( lRoadClass, lNaviClass_c, &nCheck );
					if( nCheck == sindyeDispNaviError )
					{
						def->m_bErrored = TRUE;
						CString temp;
						temp.Format( AheLoadString( IDS_NAVICLASSISERROR ), strNaviClass );
						(*strMessage) += temp;
						bError = TRUE;
					}
					else if ( nCheck == sindyeDispNaviWarn )
					{
						def->m_bErrored = TRUE;
						CString temp;
						temp.Format( AheLoadString( IDS_NAVICLASSISWORNING ), strNaviClass );
						(*strMessage) += temp;
						bWarning = TRUE;
					}
					else
						def->m_bErrored = FALSE;
				}
				else if( def->m_uiCheckType & CHECK_DISPOTHER ) 
				{
					sindyeDispOtherRelation nDispOtherRel;
					CString temp111 = def->GetFieldName();
					m_ipRule->GetDispOtherRule( lRoadClass, CComBSTR(def->GetFieldName()), &nDispOtherRel );

					// [Bug3078]のバグ対応、SiNDYLib対応のSiNDYRuleを使う場合に、sindyeDispOtherRelationの初期値を
					// sindyeDispOtherNoDefで登録されてしまっているので、ちゃんとWorkEditorで保存されていない（定義されていない）
					// ルールはすべて未定義となってしまうので、強引に未定義にしておく、これをちゃんと直すには、WorkEditorか
					// SiNDYRuleを直すしかない。
					if( nDispOtherRel == sindyeDispOtherNoDef )
						nDispOtherRel = sindyeDispOtherNoRes;

					LONG lDispClass_c;
					CComVariant vaDispClassValue;

					pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
					if( pFieldDef == NULL ) break;
					vaDispClassValue = pFieldDef->m_vaValue;
					lDispClass_c = vaDispClassValue.lVal;

					mapDomain = pFieldDef->GetDomain();
					CString strDispClass;
					BOOL bIsDomain = FALSE;
					if( mapDomain )
					{
						bIsDomain = TRUE;
						for( imap = mapDomain->begin(); imap != mapDomain->end(); imap++ ) {
							if( imap->code.lVal == lDispClass_c )
							{
								strDispClass = imap->code_discription;
								break;
							}
						}
					}
					
					// 未調査の時
					if( nDispOtherRel == sindyeDispOtherNoRes ) {
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
						vaDispClassValue = pFieldDef->m_vaValue;
						lDispClass_c = vaDispClassValue.lVal;
						if( lDispClass_c == 0 )
							def->m_bErrored = FALSE;
						else
						{
							def->m_bErrored = TRUE;
							CString temp;
							if( bIsDomain )
								temp.Format( AheLoadString( IDS_ERROR1 ), pFieldDef->m_strFieldAliasName ,strDispClass );
							else
								temp.Format( AheLoadString( IDS_ERROR2 ), pFieldDef->m_strFieldAliasName ,lDispClass_c );
							(*strMessage) += temp;
							bError = TRUE;

						}
					}
					// 普通は未調査
					else if( nDispOtherRel == sindyeDispOtherUsuallyNoRes )
					{
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
						vaDispClassValue = pFieldDef->m_vaValue;
						lDispClass_c = vaDispClassValue.lVal;
						if( lDispClass_c == 0 )
							def->m_bErrored = FALSE;
						else
						{
							def->m_bErrored = TRUE;
							CString temp;
							if( bIsDomain )
								temp.Format( AheLoadString( IDS_WORNING1 ), pFieldDef->m_strFieldAliasName ,strDispClass );
							else
								temp.Format( AheLoadString( IDS_WORNING2 ), pFieldDef->m_strFieldAliasName ,lDispClass_c );
							(*strMessage) += temp;
							bWarning = TRUE;
						}

					}
					// 未調査以外の時
					else if( nDispOtherRel == sindyeDispOtherExceptNoRes ) 
					{
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, def->GetFieldName() );
						vaDispClassValue = pFieldDef->m_vaValue;
						lDispClass_c = vaDispClassValue.lVal;
						if( lDispClass_c == 0 )
						{
							def->m_bErrored = TRUE;
							CString temp;
							if( bIsDomain )
								temp.Format( AheLoadString( IDS_ERROR1 ), pFieldDef->m_strFieldAliasName ,strDispClass );
							else
								temp.Format( AheLoadString( IDS_ERROR2 ), pFieldDef->m_strFieldAliasName ,lDispClass_c );
							(*strMessage) += temp;
							bError = TRUE;
						}else
							def->m_bErrored = FALSE;
					}
					else
						def->m_bErrored = FALSE;

				}	
			}
		}
	}
	if( bError )
		lCheckResult = 2;
	else
	{
		if( bWarning )
			lCheckResult = 1;
	}

	// タブのハイライト処理
	TabHighLight();

	return lCheckResult;
}

// 番号タブに入力された各コード値に該当名称が存在するかチェックする [bug 12036]
bool CAttrRoadLinkDlg::CheckNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// 路線名称コードに関するチェック
	CheckRoadCodeNameExists(strMsg, pFeatureDef);

	// 道路名称のチェック
	CheckPrefRoadNameExists(strMsg, pFeatureDef);

	// 区間別称のチェック
	CheckSectionNameExists(strMsg, pFeatureDef);

	return strMsg.IsEmpty();
}

// 入力された路線名称コードに該当名称が存在するかチェックする [bug 12036]
void CAttrRoadLinkDlg::CheckRoadCodeNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// ROAD_CODE_LISTテーブル取得
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), road_code_list::kTableName);
	if (!ipTable) return;


	// 路線番号コードを取得
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kRoadCode);
	if (!(pFieldDef->m_bChanged)) return; //!< 編集されていなければreturn
	long lRoadCode = pFieldDef->m_vaValue.lVal;

	// 0は許可値
	if (lRoadCode == 0) return;

	// 路線番号コードの値をチェック
	if (!CheckRoadCodeSectionCode(lRoadCode)) {
		AttrMessageBox(AheLoadString(IDS_OUTOFROADCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_NAMEKANJI));
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_DISPKANJI));
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SHIELDNUMBER));
		return;
	}

	// クエリ文を生成
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kRoadCode, lRoadCode);
	strSubFields.Format(_T("%s,%s,%s,%s,%s"),
		road_code_list::kNameKanji, road_code_list::kNameYomi,
		road_code_list::kDisplayKanji, road_code_list::kDisplayYomi, road_code_list::kShieldNoStr);

	// カーソル取得
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		strMsg += AheLoadString(IDS_FAILTOSEARCH);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi,
		vaDispKanji, vaDispYomi, vaShieldNumber;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, road_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, road_code_list::kNameYomi);
		vaDispKanji = AheGetAttribute(ipRow, road_code_list::kDisplayKanji);
		vaDispYomi = AheGetAttribute(ipRow, road_code_list::kDisplayYomi);
		vaShieldNumber = AheGetAttribute(ipRow, road_code_list::kShieldNoStr);
	}
	
	// 各該当名称が無い場合は警告文を付与
	if (uh::variant_util::isNullOrEmpty(vaKanjiName)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_NAMEKANJI));
	}
	if (uh::variant_util::isNullOrEmpty(vaDispKanji)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_DISPKANJI));
	}
	if (uh::variant_util::isNullOrEmpty(vaShieldNumber)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SHIELDNUMBER));
	}
}

// 入力された都道府県コード、道路番号に該当名称が存在するかチェックする [bug 12036]
void CAttrRoadLinkDlg::CheckPrefRoadNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// PREF_ROAD_LISTテーブル取得
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), pref_road_list::kTableName);
	if (!ipTable) return;

	// 道路種別を取得
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(ROAD_TABLE_NAME, road_link::kRoadClass);
	long lRoadClass = pFieldDef->m_vaValue.lVal;

	// 都道府県コードを取得
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kPrefCode);
	// ROAD_LINKのフィールドにPREF_CODEが定義されていない場合に落ちないように
	if (!pFieldDef) return;
	long lPrefCode = pFieldDef->m_vaValue.lVal;

	// 都道府県コードが編集されているか
	if (pFieldDef->m_bChanged) {
		// 範囲外の都道府県コード値が入力されていた場合は警告表示
		if (!CheckPrefCode(lPrefCode)) {
			AttrMessageBox(AheLoadString(IDS_OUTOFPREFCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
			strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFNAME));
		}
		else {
			// 都道府県名称が無い場合は警告文を付与
			const auto& itr = prefCodeMap.find(lPrefCode);
			if (itr == prefCodeMap.end()) {
				strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFNAME));
			}
		}
	}

	// 道路番号を取得
	pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kRoadNo);
	if (!(pFieldDef->m_bChanged)) return; //!< 編集されていなければreturn
	long lRoadNo = pFieldDef->m_vaValue.lVal;

	// 範囲外の道路番号が入力されていた場合は警告表示
	if (!CheckRoadNo(lRoadNo)) {
		AttrMessageBox(AheLoadString(IDS_OUTOFROADNORANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFROADNAME));
		return;
	}

	// クエリ文を生成
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld AND %s = %ld AND %s = %ld"),
		road_link::kPrefCode, lPrefCode, road_link::kRoadNo, lRoadNo, road_link::kRoadClass, lRoadClass);
	strSubFields.Format(_T("%s,%s"), pref_road_list::kNameKanji, pref_road_list::kNameYomi);

	// カーソル取得
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		strMsg += AheLoadString(IDS_FAILTOSEARCHROADNO);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, pref_road_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, pref_road_list::kNameYomi);
	}

	// 道路名称が無い場合は警告文を付与
	if (uh::variant_util::isNullOrEmpty(vaKanjiName)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_PREFROADNAME));
	}
}

// 入力された区間別称コードに該当名称が存在するかチェックする [bug 12036]
void CAttrRoadLinkDlg::CheckSectionNameExists(CString& strMsg, CFeatureDef& pFeatureDef)
{
	// 現在選択中のフィーチャ取得
	IFeaturePtr	ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();

	// SECTION_CODE_LISTテーブル取得
	ITablePtr ipTable = AheOpenSameOwnerTable(AheGetTable(ipCurrentFeature), section_code_list::kTableName);
	if (!ipTable) return;


	// 区間別称コードを取得
	CFieldDef* pFieldDef = pFeatureDef.GetFieldDef(road_link::kTableName, road_link::kSectionCode);
	// ROAD_LINKのフィールドにSECTION_CODEが定義されていない場合に落ちないように
	if (!pFieldDef) return;
	if (!(pFieldDef->m_bChanged)) return; //!< 編集されていなければreturn
	long lSectionCode = pFieldDef->m_vaValue.lVal;

	// 0は許可値
	if (lSectionCode == 0) return;

	// 各コード値のチェック
	if (!CheckRoadCodeSectionCode(lSectionCode)) {
		AttrMessageBox(AheLoadString(IDS_OUTOFSECTIONCODERANGE), AheLoadString(IDS_CAPTION), MB_OK, WARNLEVEL_HIGH);
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SECTIONNAME));
		return;
	}

	// クエリ文を生成
	CString strWhereClause, strSubFields;
	strWhereClause.Format(_T("%s = %ld"), road_link::kSectionCode, lSectionCode);
	strSubFields.Format(_T("%s,%s"), section_code_list::kNameKanji, section_code_list::kNameYomi);

	// カーソル取得
	_ICursorPtr ipCursor;
	if (!(GetCursor(ipTable, ipCursor, strWhereClause, strSubFields))) {
		strMsg += AheLoadString(IDS_FAILTOSEARCHSECTIONCODE);
		return;
	}

	_IRowPtr ipRow;
	CComVariant vaKanjiName, vaKanjiYomi;
	if (ipCursor->NextRow(&ipRow) == S_OK && ipRow)
	{
		vaKanjiName = AheGetAttribute(ipRow, section_code_list::kNameKanji);
		vaKanjiYomi = AheGetAttribute(ipRow, section_code_list::kNameYomi);
	}

	// 該当名称が無い場合は警告文を付与
	if (uh::variant_util::isNullOrEmpty(vaKanjiName)) {
		strMsg += uh::str_util::format(AheLoadString(IDS_NONAME), AheLoadString(IDS_SECTIONNAME));
	}
}

void CAttrRoadLinkDlg::TabHighLight()
{
	// 道路リンクリレーションのエラーまたは警告の時にタブをハイライトさせるため
	CControlDef* def = NULL;
	BOOL bKihon = FALSE;
	BOOL bTuika = FALSE;
	BOOL bKoutukisei = FALSE;
	BOOL bRosen =FALSE;
	BOOL bDRMA = FALSE;
	BOOL bParking = FALSE;
	for( CControlRel::iterator i = m_cControlRel.begin(); i != m_cControlRel.end(); i++ )
	{
		def = &(*i);
		if( def->m_uiCheckType == CHECK_DISPNAVI ||  def->m_uiCheckType == CHECK_DISPOTHER)
		{
			if( def->m_bErrored )
			{
				CString strFieldName = def->GetFieldName();
				if( strFieldName == _T("NAVICLASS_C") ||
					strFieldName == _T("ROADMANAGER_C") ||
					strFieldName == _T("ROADWIDTH_C") ||
					strFieldName == _T("MAIN_LINKCLASS_C") ||
					strFieldName == _T("SUB_LINKCLASS_C") )
				{
					bKihon = TRUE;
				}
				else if( strFieldName == _T("ISLAND_F") ||
					strFieldName == _T("BYPASS_F") ||
					strFieldName == _T("CARONLY_F") ||
					strFieldName == _T("WIDEAREA_C") ||
					strFieldName == _T("LANE_COUNT") ||
					strFieldName == _T("LANEVALID_F") ||
					strFieldName == _T("UPDOWNCLASS_C") ||
					strFieldName == _T("RIGHT_CHANGE") ||
					strFieldName == _T("LEFT_CHANGE") ||
					strFieldName == _T("ALLEYMAINTE_C"))
				{
					bTuika = TRUE;
				}
				else if( strFieldName == _T("ONEWAY_C") ||
					strFieldName == _T("NOPASSAGE_C") )
				{
					bKoutukisei = TRUE;
				}
				else if( strFieldName == road_link::kRoadCode ||
					strFieldName == road_link::kPrefCode ||
					strFieldName == road_link::kRoadNo ||
					strFieldName == road_link::kSectionCode)
				{
					bRosen = TRUE;
				}
				else if( strFieldName == _T("KUBUN_C") ||
					strFieldName == _T("LIMITSPEED_C") ||
					strFieldName == _T("TRIP_SPEED") ) 
				{
					bDRMA = TRUE;
				}
				else if( strFieldName == _T("PARKINGCLASS_C") ||
					strFieldName == _T("PARKINGLINKCLASS_C") ||
					strFieldName == _T("FLOOR_LEVEL") ||
					strFieldName == _T("MIDDLEFLOOR_LEVEL") )
				{
					bParking = TRUE;
				}
			}
		}
	}
	CTabCtrl cTab( GetDlgItem( IDC_TAB_ROADATTR ) );
	if( bKihon )
		cTab.HighlightItem( 0, TRUE );
	else
		cTab.HighlightItem( 0, FALSE );
	if( bTuika )
		cTab.HighlightItem( 1, TRUE );
	else
		cTab.HighlightItem( 1, FALSE );
	if( bKoutukisei )
		cTab.HighlightItem( 2, TRUE );
	else
		cTab.HighlightItem( 2, FALSE );
	if( bRosen )
		cTab.HighlightItem( 3, TRUE );
	else
		cTab.HighlightItem( 3, FALSE );
	if( bDRMA )
		cTab.HighlightItem( 4, TRUE );
	else
		cTab.HighlightItem( 4, FALSE );
	if( bParking )
		cTab.HighlightItem( 5, TRUE );
	else
		cTab.HighlightItem( 5, FALSE );

	InvalidateRect( NULL, FALSE );
}
/**
 * @fn BOOL CAttrRoadLinkDlg::CheckReturnKeyItem(INT nClassID)
 * @brief リターンキーが押された時の動作
 *
 * @param	INT		nClassID	[i]		リターンキーが押された時のItemID
 *
 * @return	TRUE : リターンキーとしての動作(適用処理)可 , FALSE : リターンキーとしての動作不可 
 */
BOOL CAttrRoadLinkDlg::CheckReturnKeyItem(INT nClassID)
{
	// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
	// このコピーだけはやるしかないのでこれを消しては絶対にいけない！
	CopyMeshRoad();
	return TRUE;
}

void CAttrRoadLinkDlg::CopyMeshRoad()
{
	// 2次メッシュ境界線を跨いでいる道路の場合は反対側の道路も同じ属性にしなくてはいけないため
	// 以下の処理を行う
	if( m_bIsRoadOnMesh )
	{
		// とりあえず、現在保持しているリストすべてにおいてループを行う
		for( list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			IFeaturePtr temp;
			LONG lOID = 0;
			temp = it->GetFeature();
			temp->get_OID( &lOID );

			// 現在選択中の2次メッシュを跨いでいる道路リンクに関連しているリストとの照合
			CFeatureDef *pFeatureDef = NULL;
			for( list<CFeatureDef>::iterator iRoad = m_pRoadOnMeshList.begin(); iRoad != m_pRoadOnMeshList.end(); iRoad++ )
			{
				LONG lMeshOID;
				iRoad->GetFeature()->get_OID( &lMeshOID );
				if( lOID == lMeshOID )
				{
					pFeatureDef = &(*it);
					break;
				}
			}

			if( pFeatureDef )
			{
				// すべてのフィールド値をチェック
				for( list<CFieldDef>::iterator itField = GetCurrentFeatureDef()->begin(); itField != GetCurrentFeatureDef()->end(); itField++ )
				{
					CString strFieldName = itField->m_strFieldName;

					// 以下if文の中身はコピーしてしまってはいけないもの
					if( strFieldName != _T("OBJECTID") && strFieldName != _T("OPERATOR") &&
						strFieldName != _T("PURPOSE_C") && strFieldName != _T("MODIFYDATE") &&
						strFieldName != _T("UPDATETYPE_C") && strFieldName != _T("PROGMODIFYDATE") &&
						strFieldName != _T("MODIFYPROGNAME") && strFieldName != _T("TMP_MESHCODE") &&
						strFieldName != _T("TMP_PRIMID") && strFieldName != _T("TMPFROM_NODE") &&
						strFieldName != _T("TMPTO_NODE") && strFieldName != _T("FROM_NODE_ID") &&
						strFieldName != _T("ADMIN_CODE") && strFieldName != _T("FIELDSURVEY_F") &&
						strFieldName != _T("USERCLAIM_F") &&
						strFieldName != _T("TO_NODE_ID") && strFieldName != _T("TURNREG_LQRF") &&
						strFieldName != _T("DIRGUIDE_LQRF") && strFieldName != _T("GUIDE_LQRF") &&
						strFieldName != _T("LANE_LQRF") && strFieldName != _T("VICS_LQRF") &&
						strFieldName != _T("BYWAY_LQRF") && strFieldName != _T("ROUTE_LQRF") &&
						strFieldName != _T("UTURN_LQRF") && strFieldName != _T("INTERNAVI_LQRF") &&
						strFieldName != _T("SHAPE") && strFieldName != _T("SHAPE.LEN") )
					{
						// 上記のif文の条件以外はコピーする
						CFieldDef *pFieldDef;
						pFieldDef = pFeatureDef->GetFieldDef( ROAD_TABLE_NAME, itField->m_strFieldName );
						VARIANT vaValue;
						pFeatureDef->GetFeature()->get_Value( itField->m_lFieldIndex, &vaValue );
						if( itField->m_vaValue != vaValue )
						{
							pFieldDef->m_vaValue = itField->m_vaValue;
							pFieldDef->m_bChanged = TRUE;
						}
						else{
							pFieldDef->m_vaValue = itField->m_vaValue;
							pFieldDef->m_bChanged = FALSE;
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// グリッドコントロール用
//
/////////////////////////////////////////////////////////////////////////////

/**
 * @fn void CAttrRoadLinkDlg::MouseDownTimeRegGrid( INT nID, IVSFlexGridPtr spGrid, LONG& lIndex, LPCTSTR lpcszTableName, SHORT Button, SHORT Shift, FLOAT X, FLOAT Y )
 * @brief グリッドのセルをクリックしたときに呼ばれる
 *
 * 属性ダイアログで時間規制の変更ができたときには、2次メッシュを跨いでいる道路の場合には、跨いでいる道路リンクにもコピーする
 * 現在は、時間規制を昇順、降順で並べ替えるメニューを出すだけ。 
 */
void CAttrRoadLinkDlg::MouseDownTimeRegGrid( INT nID, IVSFlexGridPtr spGrid, LONG& lIndex, LPCTSTR lpcszTableName, SHORT Button, SHORT Shift, FLOAT X, FLOAT Y )
{
	LONG lCol = 0, lRow = 0;
	CAxWindow wnd = GetDlgItem( nID );
		
	spGrid->get_MouseCol( &lCol );
	spGrid->get_MouseRow( &lRow );

		
	POINT pt = { XTwipsToPixels(X), YTwipsToPixels(Y) };
	wnd.ClientToScreen( &pt );
	
	if( lRow == 0 )
	{
		// 右クリックならメニューを出す
		if( Button & 0x02 ) {

			AHE::CMenu menu;
			menu.CreateMenu( TRUE );

			menu.InsertMenu( 0, MF_BYPOSITION | MF_STRING, 1000, AheLoadString(IDS_ASCENDING) );
			menu.InsertMenu( 1, MF_BYPOSITION | MF_STRING, 1001, AheLoadString(IDS_DESCENDING) );

			INT intRet = menu.TrackPopupMenu( TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, pt.x, pt.y );
			
			LONG lBottomRow;
			spGrid->get_Rows( &lBottomRow );
			spGrid->Select( _variant_t(1L), _variant_t(lCol), _variant_t(LONG(lBottomRow-1)), _variant_t(lCol));
			if( intRet == 1000 )
				spGrid->put_Sort( flexSortGenericAscending );
			else if( intRet == 1001 )
				spGrid->put_Sort( flexSortGenericDescending );
		}
	}
}


HRESULT CAttrRoadLinkDlg::CreateActiveXControls(UINT nID)
{
	// Load dialog template and InitData
	HRSRC hDlgInit = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)_ATL_RT_DLGINIT);
	BYTE* pInitData = NULL;
	HGLOBAL hData = NULL;
	HRESULT hr = S_OK;
	if (hDlgInit != NULL)
	{
		hData = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlgInit);
		if (hData != NULL)
			pInitData = (BYTE*) ::LockResource(hData);
	}

	HRSRC hDlg = ::FindResource(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), (LPTSTR)RT_DIALOG);
	if (hDlg != NULL)
	{
		HGLOBAL hResource = ::LoadResource(_AtlBaseModule.GetResourceInstance(), hDlg);
		DLGTEMPLATE* pDlg = NULL;
		if (hResource != NULL)
		{
			pDlg = (DLGTEMPLATE*) ::LockResource(hResource);
			if (pDlg != NULL)
			{
				// Get first control on the template
				BOOL bDialogEx = _DialogSplitHelper::IsDialogEx(pDlg);
				WORD nItems = _DialogSplitHelper::DlgTemplateItemCount(pDlg);

				// Get first control on the dialog
				DLGITEMTEMPLATE* pItem = _DialogSplitHelper::FindFirstDlgItem(pDlg);
				HWND hWndPrev = GetWindow(GW_CHILD);

				// Create all ActiveX cotnrols in the dialog template and place them in the correct tab order (z-order)
				for (WORD nItem = 0; nItem < nItems; nItem++)
				{
					DWORD wID = bDialogEx ? ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : pItem->id;
					if (_DialogSplitHelper::IsActiveXControl(pItem, bDialogEx))
					{
						BYTE* pData = NULL;
						DWORD dwLen = _DialogSplitHelper::FindCreateData(wID, pInitData, &pData);
						CComPtr<IStream> spStream;
						if (dwLen != 0)
						{
							HGLOBAL h = GlobalAlloc(GHND, dwLen);
							if (h != NULL)
							{
								BYTE* pBytes = (BYTE*) GlobalLock(h);
								BYTE* pSource = pData; 
								Checked::memcpy_s(pBytes, dwLen, pSource, dwLen);
								GlobalUnlock(h);
								CreateStreamOnHGlobal(h, TRUE, &spStream);
							}
							else
							{
								hr = E_OUTOFMEMORY;
								break;
							}
						}

						CComBSTR bstrLicKey;
						hr = _DialogSplitHelper::ParseInitData(spStream, &bstrLicKey.m_str);
						if (SUCCEEDED(hr))
						{
							CAxWindow2 wnd;
							// Get control caption.
							LPWSTR pszClassName = 
								bDialogEx ? 
									(LPWSTR)(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem) + 1) :
									(LPWSTR)(pItem + 1);
							// Get control rect.
							RECT rect;
							rect.left = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->x : 
									pItem->x;
							rect.top = 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->y : 
									pItem->y;
							rect.right = rect.left + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cx : 
									pItem->cx);
							rect.bottom = rect.top + 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->cy : 
									pItem->cy);

							// Convert from dialog units to screen units
							MapDialogRect(&rect);

							// Create AxWindow with a NULL caption.
							wnd.Create(m_hWnd, 
								&rect, 
								NULL, 
								(bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->style : 
									pItem->style) | WS_TABSTOP, 
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->exStyle : 
									0,
								bDialogEx ? 
									((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->id : 
									pItem->id,
								NULL);

							if (wnd != NULL)
							{
								// Set the Help ID
								if (bDialogEx && ((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID != 0)
									wnd.SetWindowContextHelpId(((_DialogSplitHelper::DLGITEMTEMPLATEEX*)pItem)->helpID);
								// Try to create the ActiveX control.
								static WCHAR pwchLicenseKey[] =
								{
									0x004F,	0x0030,	0x0045,	0x0054,	0x0054,	0x0043,	
									0x0037,	0x0034,	0x0037
								};
								hr = wnd.CreateControlLic(pszClassName, spStream, NULL, /*bstrLicKey*/CComBSTR(pwchLicenseKey));
								if (FAILED(hr))
									break;
								// Set the correct tab position.
								if (nItem == 0)
									hWndPrev = HWND_TOP;
								wnd.SetWindowPos(hWndPrev, 0,0,0,0,SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
								hWndPrev = wnd;
							}
							else
							{
								hr = AtlHresultFromLastError();
							}
						}
					}
					else
					{
						if (nItem != 0)
							hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
					}
					pItem = _DialogSplitHelper::FindNextDlgItem(pItem, bDialogEx);
				}
			}
			else
				hr = AtlHresultFromLastError();
		}
		else
			hr = AtlHresultFromLastError();
	}
	return hr;
}

// [bug 6712] 広域フラグと離島フラグをラジオボタン式にする
LRESULT CAttrRoadLinkDlg::OnCheckWideAreaOrIsland(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CButton cWideArea( GetDlgItem( IDC_CHECK_WIDEAREAF ) ), cIsland( GetDlgItem( IDC_CHECK_ISLANDF ) );

	CComboBox CCombo( GetDlgItem( IDC_COMBO_WIDEAREA ) );
	switch( cWideArea.GetCheck() )
	{
	case BST_CHECKED:
		// [bug 6712 #20] 広域存在コードを2にして変更を通知
		if( CCombo.SetCurSel(2) != CB_ERR ) 
			OnCommand(0, MAKEWPARAM(IDC_COMBO_WIDEAREA, CBN_SELCHANGE), 0, bHandled);

		// 広域フラグと離島フラグが両方ともチェックされてたら、
		// 他方のクリックをでっちあげてチェックをはずす
		// BST_UNCHECKEDをセットするだけだと変更通知が回らない
		if( cIsland.GetCheck() == BST_CHECKED )
		{
			switch( wID )
			{
			case IDC_CHECK_ISLANDF:
				cWideArea.Click();
				// [bug 6712 #20] 広域存在コードは0にしとく
				if( CCombo.SetCurSel(0) != CB_ERR )
					OnCommand(0, MAKEWPARAM(IDC_COMBO_WIDEAREA, CBN_SELCHANGE), 0, bHandled);
				break;
			case IDC_CHECK_WIDEAREAF:
				cIsland.Click();
				break;
			default:
				break;
			};
		}
		break;
	case BST_UNCHECKED:
		// [bug 6712 #20] 広域存在コードを0にして変更を通知
		if( CCombo.SetCurSel(0) != CB_ERR )
			OnCommand(0, MAKEWPARAM(IDC_COMBO_WIDEAREA, CBN_SELCHANGE), 0, bHandled);
		break;
	default:
		break;
	}

	bHandled = FALSE;

	return 0;
}
