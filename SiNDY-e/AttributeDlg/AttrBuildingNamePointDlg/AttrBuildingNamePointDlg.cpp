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

// AttrBuildingNamePointDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "crd_cnv/coord_converter.h"
#include "AttrDlgLib.h"
#include <WinLib/libwinlib.h>
#include "libschema.h"
#include <TDC/common_check/NameYomiChk.h>
#include <TDC/common_check/libcommoncheck.h>
#include "AttrBuildingNamePointDlg.h"
#include "Map.h"
#include <WinLib/IMEStatus.h>


using namespace sindy::schema;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
		BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
			RCSPACE(20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BUILDINGOID)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_BUILDINGOID)
				RCTOFIT(IDC_BUTTON_SEARCHOID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAMESEPARATED)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_NAMESEPARATED)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_YOMISEPARATED)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_YOMISEPARATED)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDGCLASS1C)
				RCTOFIT(-1)
				RCSPACE(2)
				RCREST(IDC_COMBO_BLDGCLASS1C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDGCLASS2C)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_COMBO_BLDGCLASS2C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDGCLASS3C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_BLDGCLASS3C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_FLOORS)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_FLOORS)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCETYPEC)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SOURCETYPEC)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SRCOID)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_SRCOID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYDATE)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_SURVEYDATE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ANYPLACEID)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_ANYPLACEID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PRIORITYC)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_COMBO_PRIORITYC)
				RCTOFIT(IDC_BUTTON_PRIORITYC)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MATCHPER)
				RCTOFIT(-1)
				RCSPACE(3)
				RCREST(IDC_EDIT_MATCHPER)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SRCPOINTLIST)
				ENDGROUP()
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_LIST_SRCPOINT)
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
				RCSPACE(3)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

	// 素材ポイント関連情報のリスト定義
	LVCOLUMN CAttrBuildingNamePointDlg::m_ColumnInfo[] = {
		{ LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, BNP_SRC_COLUMN0_SZ, BNP_SRC_COLUMN0, 0, 0, 0, 0 },
		{ LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, BNP_SRC_COLUMN1_SZ, BNP_SRC_COLUMN1, 0, 0, 0, 0 },
		{ LVCF_FMT|LVCF_WIDTH|LVCF_TEXT, LVCFMT_LEFT, BNP_SRC_COLUMN2_SZ, BNP_SRC_COLUMN2, 0, 0, 0, 0 }
	};

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrBuildingNamePointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrBuildingNamePointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////


// コンストラクタ
CAttrBuildingNamePointDlg::CAttrBuildingNamePointDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_ATTRBUILDINGNAMEPOINTDLG), m_bShowSrcInfo(false)
{

	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_BUILDINGOID,  CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_BUILDINGOID ) )
		( IDC_BUTTON_SEARCHOID,  CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHOID ) )
		( IDC_EDIT_NAME,         CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_NAME ) )
		( IDC_EDIT_NAMESEPARATED,CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_NAMESEPARATED ) )
		( IDC_EDIT_YOMISEPARATED,CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_YOMISEPARATED ) )
		( IDC_COMBO_BLDGCLASS1C, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_BLDGCLASS1C ) )
		( IDC_COMBO_BLDGCLASS2C, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_BLDGCLASS2C ) )
		( IDC_COMBO_BLDGCLASS3C, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_BLDGCLASS3C ) )
		( IDC_EDIT_FLOORS,       CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_FLOORS ) )
		( IDC_COMBO_SOURCETYPEC, CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_SOURCETYPEC ) )
		( IDC_EDIT_SRCOID,       CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SRCOID ) )
		( IDC_EDIT_SURVEYDATE,   CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SURVEYDATE ) )
		( IDC_EDIT_ANYPLACEID,   CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_ANYPLACEID ) )
		( IDC_COMBO_PRIORITYC,   CContainedWindow(  _T("COMBO"),  this, IDC_COMBO_PRIORITYC ) )
		( IDC_BUTTON_PRIORITYC,  CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_PRIORITYC ) )
		( IDC_EDIT_MATCHPER,     CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_MATCHPER ) )
		( IDC_LIST_SRCPOINT,     CContainedWindow(  _T("EDIT"),   this, IDC_LIST_SRCPOINT ) )
		( IDC_EDIT_SOURCE,       CContainedWindow(  _T("EDIT"),   this, IDC_EDIT_SOURCE ) );

}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingNamePointDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

// ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
LRESULT CAttrBuildingNamePointDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// コントロール
	CreateControlRelation();
	// サブクラス化
	for( auto& it : m_mapSubClass)
		it.second.SubclassWindow( GetDlgItem( it.first ) );

	return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
}


// WM_COMMAND 用イベントハンドラ
LRESULT CAttrBuildingNamePointDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
	if( !m_bFinishInit ) return 0;
	INT msg = HIWORD(wParam);						// 操作メッセージ
	INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール

	if( msg == EN_SETFOCUS ){
		// エディットボックスの時に全選択にする
		SelectDlgItem( nTargetControl );

		CONVERSIONMODE selectedMode = NOCHANGE;
		switch( nTargetControl ){
			// 名称または名称（分割）の時は全角ひらがな入力にする
			case IDC_EDIT_NAME :
			case IDC_EDIT_NAMESEPARATED :
				selectedMode = HIRAGANA;
				break;
			// ヨミ（分割）の場合は全角カタカナ入力にする
			case IDC_EDIT_YOMISEPARATED:
				selectedMode = KATAKANA_FULL;
				break;
			// 階数の時は半角英数字入力にする
			case IDC_EDIT_FLOORS :
				selectedMode = ALPHA_HALF;
				break;
			default :
				break;
		}
		CIMEStatus imeStatus(m_hWnd);
		imeStatus.setIMEConversionStatus(selectedMode);
	}

	// コンボボックスのリストボックスの長さを調節
	if( msg == CBN_DROPDOWN )
		SetComboboxList( nTargetControl );

	// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
	if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
		// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
		m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
		// ダイアログを更新領域に追加します
		InvalidateRect( NULL, FALSE );
		// 変更された場合はボタンを Enable に
		SetButton( Changed() );
	}

	return 0;
}


// string型用イベントハンドラ
LRESULT CAttrBuildingNamePointDlg::OnEnUpdateEditString(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( !::IsWindow( hWndCtl ) ) return 0;
	
	CEdit edit( hWndCtl );
	CString strText;
	edit.GetWindowText( strText );

	if( wID == IDC_EDIT_NAME && strText == _T(" ") )
	{
		// 名称はNULL不許可のため新規作成時にスペースが入力されるが、不要なためスペースを削除する
		edit.SetWindowText( NULL_VALUE );
	}
	else if( strText != NULL_VALUE )
	{
		CString strChange;

		// wIDごとの個別処理
		if( wID == IDC_EDIT_NAME )
		{
			int nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszChange;
			lpszChange = (LPTSTR)_malloca( nTextLen * sizeof(TCHAR) * 2 );	// 2倍の領域を確保
			LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, strText, nTextLen, lpszChange, nTextLen * 2 );
			strChange = lpszChange;
		}
		else if( wID == IDC_EDIT_SURVEYDATE )
		{
			strChange = AheConvertToDateStr( strText );
		}

		// エディットボックスの変更
		if( strText != strChange )
		{
			int dwSelStart = 0, dwSelEnd = 0;
			edit.GetSel( dwSelStart, dwSelEnd );
			edit.SetWindowText( strChange );
			// 入力位置の修正
			int nDiff = CString_GetLength2( strText ) - CString_GetLength2( strChange );
			edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
		}
	}

	m_cControlRel.ControlChanged( wID );
	SetButton( Changed() );
	InvalidateRect( NULL, FALSE );

	return 0;
}


// 子ダイアログを作成します。
HWND CAttrBuildingNamePointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}


// 作成した子ダイアログを削除します。
void CAttrBuildingNamePointDlg::Delete()
{
	// ダイアログを閉じる時に全角ひらがな入力にする
	CIMEStatus imeStatus(m_hWnd);
	imeStatus.setIMEConversionStatus(HIRAGANA);

	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}


// 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
BOOL CAttrBuildingNamePointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		::SetFocus( GetDlgItem(IDC_EDIT_NAME) );
	}
	
	if( !m_bShowSrcInfo )
	{
		OutputBuildingSrcInfo(); // 建物ビル名称ポイントに紐づく建物ビル名称素材ポイントの情報をリスト出力する
		m_bShowSrcInfo = true;
	}

	SetToolTip( IDC_EDIT_NAME );
	return bRet;
}


// 建物ビル名称ポイントに紐づく建物ビル名称素材ポイントの情報をリスト出力する
void CAttrBuildingNamePointDlg::OutputBuildingSrcInfo()
{

	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	long lBuildingNameOID = AheGetOID( ipFeature );
	IFeatureClassPtr ipBuildingName = AheGetFeatureClass( ipFeature );

	// 建物ビル名称紐付きテーブル、建物ビル名称素材ポイント取得
	ITablePtr ipRelBuildTable = AheOpenSameOwnerTable( (ITablePtr)ipBuildingName,  sj::rel_buildingname::kTableName );
	if( !ipRelBuildTable )   return;
	IFeatureClassPtr ipBuildSrcFC = AheOpenSameOwnerTable( (ITablePtr)ipBuildingName,  sj::buildingname_src_point::kTableName );
	if( !ipBuildSrcFC )   return;

	// 建物ビル名称ポイント::OIDから建物ビル名称紐付きテーブルの該当行を取得
	CString strSubField;
	strSubField.Format( _T("%s"), sj::rel_buildingname::kSrcOID );
	CString strWhereClause;
	strWhereClause.Format( _T("%s = %ld"), sj::rel_buildingname::kBuildingNameOID, lBuildingNameOID );
	_ICursorPtr ipRelBuildCursor;
	ipRelBuildTable->Search( AheInitQueryFilter(NULL, strSubField, strWhereClause ), VARIANT_TRUE, &ipRelBuildCursor );

	CListViewCtrl lvSrcPoint = GetDlgItem( IDC_LIST_SRCPOINT );
	_IRowPtr ipRelBuildRow;
	while( ipRelBuildCursor->NextRow( &ipRelBuildRow ) == S_OK && ipRelBuildRow )
	{
		// 建物ビル名称紐付きテーブル::参照素材OIDから建物ビル名称素材ポイントの情報をリストへ出力
		long lSrcOID = AheGetAttribute( ipRelBuildRow, sj::rel_buildingname::kSrcOID ).lVal;

		IFeaturePtr ipBuildSrcFeature;
		ipBuildSrcFC->GetFeature( lSrcOID, &ipBuildSrcFeature );
		if( !ipBuildSrcFeature )
			continue;
		
		int nItemCount = lvSrcPoint.GetItemCount();
		CString strBuildSrcOID;
		strBuildSrcOID.Format( _T("%d"), AheGetOID( ipBuildSrcFeature ) );
		lvSrcPoint.InsertItem( nItemCount, strBuildSrcOID );
		lvSrcPoint.SetItemText( nItemCount, list_col::building_src::Name, CString( AheGetAttribute( ipBuildSrcFeature, sj::buildingname_src_point::kName ) ) );

		// 出典コードはコード値ドメインの文字列で出力
		CString strDomainName;
		CComVariant vaValue;
		AheGetDomainNameAndValue( ipBuildSrcFeature, sj::buildingname_src_point::kSourceType, strDomainName, vaValue );
		lvSrcPoint.SetItemText( nItemCount, list_col::building_src::SrcCode, strDomainName );
	}


}


// 子ダイアログを実際に作成します。
void CAttrBuildingNamePointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 建物ポリゴンOID
		m_cControlRel.SetControl( IDC_EDIT_BUILDINGOID, TYPE_EDIT, 0, 0, sj::buildingname_point::kBuildingOID, NULL, sj::buildingname_point::kTableName, NULL, -1, FOLLOW_READONLY );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCHOID, TYPE_BUTTON, 0, 0 );
		// 名称
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kName, NULL, sj::buildingname_point::kTableName );
		// 名称（分割）
		m_cControlRel.SetControl( IDC_EDIT_NAMESEPARATED, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kNameSeparated, NULL, sj::buildingname_point::kTableName );
		// ヨミ（分割）
		m_cControlRel.SetControl( IDC_EDIT_YOMISEPARATED, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kYomiSeparated, NULL, sj::buildingname_point::kTableName );
		// 建物種別コード1
		m_cControlRel.SetControl( IDC_COMBO_BLDGCLASS1C, TYPE_COMBO, 0, 0, sj::buildingname_point::kBldgClass1, NULL, sj::buildingname_point::kTableName );
		// 建物種別コード2
		m_cControlRel.SetControl( IDC_COMBO_BLDGCLASS2C, TYPE_COMBO, 0, 0, sj::buildingname_point::kBldgClass2, NULL, sj::buildingname_point::kTableName );
		// 建物種別コード3
		m_cControlRel.SetControl( IDC_COMBO_BLDGCLASS3C, TYPE_COMBO, 0, 0, sj::buildingname_point::kBldgClass3, NULL, sj::buildingname_point::kTableName );
		// 階数
		m_cControlRel.SetControl( IDC_EDIT_FLOORS, TYPE_EDIT, 0, 0, sj::buildingname_point::kFloors, NULL, sj::buildingname_point::kTableName );
		// 出典コード
		m_cControlRel.SetControl( IDC_COMBO_SOURCETYPEC, TYPE_COMBO, 0, 0, sj::buildingname_point::kSourceType, NULL, sj::buildingname_point::kTableName );
		// 参照素材OID
		m_cControlRel.SetControl( IDC_EDIT_SRCOID, TYPE_EDIT, 0, 0, sj::buildingname_point::kSrcOID, NULL, sj::buildingname_point::kTableName, NULL, -1, FOLLOW_READONLY );
		// 調査日
		m_cControlRel.SetControl( IDC_EDIT_SURVEYDATE, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kSurveyDate, NULL, sj::buildingname_point::kTableName );
		// AnyPlace ID
		m_cControlRel.SetControl( IDC_EDIT_ANYPLACEID, TYPE_EDITSTRING, 0, 0, sj::buildingname_point::kAnyPlaceID, NULL, sj::buildingname_point::kTableName );
		// 信頼度コード
		m_cControlRel.SetControl( IDC_COMBO_PRIORITYC, TYPE_COMBO, 0, 0, sj::buildingname_point::kPriority, NULL, sj::buildingname_point::kTableName );
		m_cControlRel.SetControl( IDC_BUTTON_PRIORITYC, TYPE_BUTTON, 0, 0 );
		// マッチング率
		m_cControlRel.SetControl( IDC_EDIT_MATCHPER, TYPE_EDIT, 0, 0, sj::buildingname_point::kMatchPer, NULL, sj::buildingname_point::kTableName, NULL, -1, FOLLOW_READONLY );

		// 建物ビル名称素材ポイントのOID・名称・出典コードのリスト
		CListViewCtrl lvSrcList = GetDlgItem( IDC_LIST_SRCPOINT );
		for(int i = 0; i < list_col::building_src::ColumnNum; ++i)
			lvSrcList.InsertColumn( i, &m_ColumnInfo[i] );

		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, NULL, sj::buildingname_point::kTableName );

	}
}


// ツールチップを作成します。
void CAttrBuildingNamePointDlg::SetToolTip(INT nTargetItem)
{
	
	CEdit edit( GetDlgItem(nTargetItem) );

	// エディットボックスにかかれている文字列を取得
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	// ツールチップ情報を作成
	CToolInfo tiEdit(
		TTF_IDISHWND | TTF_SUBCLASS,
		edit,
		(UINT_PTR)edit.m_hWnd,
		CRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
		lpszText );

	// ツールチップコントロール作成
	CToolTipCtrl tooltip;
	tooltip.Create( m_hWnd );
	tooltip.Activate( TRUE );
	tooltip.AddTool( tiEdit );

}


// 「建物ポリゴンOID取得」ボタンを押した場合の処理
LRESULT CAttrBuildingNamePointDlg::OnBnClickedButtonSearchoid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// 真位置直下にある建物ポリゴンのOIDを取得

	// 表示している建物ビル名称
	IFeaturePtr ipFeature = GetCurrentFeatureDef()->GetFeature();
	IGeometryPtr ipBuildingNameGeom = AheGetShape( ipFeature );

	// 建物ポリゴン
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ILayerPtr ipBuilding = cMap.FindLayer( nullptr, nullptr, building::kTableName );
	if( !ipBuilding )
	{
		CString strMsg;
		strMsg.Format( AheLoadString( IDS_ERRORGETBYTOC ), building::kTableName );
		AttrMessageBox( strMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		return 0;
	}
	IFeatureClassPtr ipBuildingClass = AheGetFeatureClass( ipBuilding );
	
	// 建物ビル名称の直下に存在する建物ポリゴンを取得
	CString subField;
	subField.Format( _T("%s"), ipc_feature::kObjectID );
	IQueryFilterPtr ipQuery = AheInitQueryFilter( AheInitSpatialFilter( nullptr, ipBuildingNameGeom ), subField, nullptr );

	long lFeatureCount = 0;
	 ipBuildingClass->FeatureCount( ipQuery, &lFeatureCount );
	if( lFeatureCount > 1 ) // 建物ポリゴンは一箇所に複数存在しない前提
	{
		ATLASSERT(FALSE);
		return 0;
	}

	IFeatureCursorPtr ipCursor;
	ipBuildingClass->Search( ipQuery, VARIANT_TRUE, &ipCursor );
	IFeaturePtr ipBuildingFeature;
	if( S_OK != ipCursor->NextFeature( &ipBuildingFeature ) || !ipBuildingFeature ){
		AttrMessageBox( AheLoadString( IDS_ERRORGETBUILDOID ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		return 0;
	}

	long lBuildingOID = AheGetOID( ipBuildingFeature );
	SetDlgItemInt( IDC_EDIT_BUILDINGOID, lBuildingOID, FALSE );

	return 0;
}


// 「調査結果(1)入力」ボタンを押した場合の処理
LRESULT CAttrBuildingNamePointDlg::OnBnClickedButtonPriorityc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComboBox cWAnnoClass( GetDlgItem( IDC_COMBO_PRIORITYC ) );
	cWAnnoClass.SetCurSel( sj::buildingname_point::priority_code::kEdit );

	m_cControlRel.ControlChanged( IDC_COMBO_PRIORITYC );
	InvalidateRect( NULL, FALSE );
	// 変更された場合はOKボタンを Enable に
	SetButton( Changed() );

	return 0;
}


// エラーチェック
BOOL CAttrBuildingNamePointDlg::ErrorCheck()
{
	for( auto& it : *m_pFeatureDefList )
	{
		CString strTableName = it.GetTableName();
		if( strTableName == sj::buildingname_point::kTableName )
		{
			// BUILDING_OIDが0のまま（＝直下に建物ポリゴンがない）の場合は作成させない
			CFieldDef* pFieldDef = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kBuildingOID );
			CComVariant vaBuildingOid = pFieldDef->m_vaValue;

			if( vaBuildingOid.lVal == 0 )
			{
				AttrMessageBox( AheLoadString( IDS_ERRORNOBUILDOID ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			CString errMsg;
			// 全角パイプを除いたNAME_SEPARATEDがNAMEと同一であるか（NULLの場合はチェックしない）
			pFieldDef = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kNameSeparated );
			CFieldDef* pFieldDefName = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kName );
			if(!checkNameSeparated(pFieldDef->m_vaValue, pFieldDefName->m_vaValue))
				errMsg += _T("「｜」を除いた名称(分割)が名称と一致していません。\n");

			// YOMI_SEPARATEDが全角カタカナおよび全角パイプのみであるか（NULLの場合はチェックしない）
			pFieldDef = it.GetFieldDef( sj::buildingname_point::kTableName, sj::buildingname_point::kYomiSeparated );
			if(!checkYomiSeparated(pFieldDef->m_vaValue))
				errMsg += _T("ヨミ(分割)は全角カタカナおよび「｜」のみ入力可能です。\n");

			if(!errMsg.IsEmpty()){
				AttrMessageBox( errMsg, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}
		}
	}

	return TRUE;
}


// 全角パイプを除いたNAME_SEPARATEDがNAMEと同一であるか確認
bool CAttrBuildingNamePointDlg::checkNameSeparated(const CComVariant& vaNameSep, const CComVariant& vaName){
	CString strNameSeparated = uh::str_util::ToString(vaNameSep);
	if(strNameSeparated.IsEmpty()) // 空の場合はチェックしない
		return true;

	// 全角パイプを除去して名称と一致すればOK
	strNameSeparated.Replace(_T("｜"),_T(""));
	if(strNameSeparated.Compare(uh::str_util::ToString(vaName)) == 0)
		return true;

	return false;
}


// YOMI_SEPARATEDが全角カタカナおよび全角パイプのみであるか確認
bool CAttrBuildingNamePointDlg::checkYomiSeparated(const CComVariant& vaVal){
	std::string strYomiSeparated = CT2A(uh::str_util::ToString(vaVal));
	if(strYomiSeparated.empty()) // 空の場合はチェックしない
		return true;

	// 全角パイプを除去して全角カタカナのみであればOK
	if(CheckYomiCharacter(strYomiSeparated, "｜"))
		return true;

	return false;
}

