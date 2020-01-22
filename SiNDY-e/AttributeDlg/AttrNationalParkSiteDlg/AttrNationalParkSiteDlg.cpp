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

// AttrNationalParkSiteDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrNationalParkSiteDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

// AttrNationalParkSiteDlg ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(NPSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_NPARKID_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_NPARKID)
				RCTOFIT(IDC_BUTTON_SEARCHPARK)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_PARKTYPE_C_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_PARKTYPE_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_PARKNUMBER_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_PARKNUMBER)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_NAME_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_NAME)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrNationalParkSiteDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrNationalParkSiteDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrNationalParkSiteDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrNationalParkSiteDlg::CAttrNationalParkSiteDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrNationalParkSiteDlg::CAttrNationalParkSiteDlg() 
	: ATTR_BASE_CLASS(NPSITE_MAP, IDD_NATIONALPARKSITEDLG)
{
	//各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_NPARKID, CContainedWindow(  _T("EDIT"), this, IDC_NPARKID ) )
		(IDC_BUTTON_SEARCHPARK, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHPARK ) );
}

/** 
 * @fn HWND CAttrNationalParkSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrNationalParkSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrNationalParkSiteDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrNationalParkSiteDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrNationalParkSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrNationalParkSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	//表示されている公園関連テーブルIDを元に関連情報を画面上に表示します
	//属性ダイアログを開いた瞬間は、メッセージを表示しないため、引数をfalseにする
	//TODO:IDC_NPARKIDの初期値が0になっており、新規作成->SetNationalParkNameが呼び出される→内部のエラーチェックで引っかかってメッセージ表示
	//TODO:となるため、引数がfalseのときはメッセージを表示しない、trueのときにはメッセージを表示するとしている。ロジックを修正すれば、綺麗になると思われるので、検討のこと。
	SetNationalParkName( false );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetDlgStatus(IDC_BUTTON_SEARCHPARK, TRUE );
		SetDlgStatus(IDC_NPARKID, TRUE );
		//最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_NPARKID) );
	}
	else
	{
		SetDlgStatus(IDC_BUTTON_SEARCHPARK, FALSE );
		SetDlgStatus(IDC_NPARKID, FALSE );
	}

	return bRet;
}

/**
 * @fn void CAttrNationalParkSiteDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrNationalParkSiteDlg::CreateControlRelation()
{
	ATLASSERT( SUCCEEDED(IsWindow()) );

	if( !m_cControlRel.empty() )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_NPARKID, TYPE_EDITSTRING, 0, 0, sj::nationalpark_site::kNparkID, 0, sj::nationalpark_site::kTableName );
	}

}

/**
* @brief 国立・国定公園の関連テーブルを検索し、関連情報を表示する
* @param messageFlag   [in] メッセージを表示するか否かのフラグ(新規作成したときにメッセージが表示されてしまうのを防ぐ)
*/
void CAttrNationalParkSiteDlg::SetNationalParkName( bool messageFlag )
{
	//NATIONALPARK_TABLEを開く
	ITablePtr ipTable = GetNationalParkTable();
	if( !ipTable )
	{
		if(messageFlag) AttrMessageBox( AheLoadString(IDS_FAIL_FIND_NPTABLE), AheLoadString(IDS_CAPTION), MB_OK, NULL );
		clearText();
		return;
	}

	//公園関連テーブルIDをエディットボックスから取得
	CWindow edit( GetDlgItem( IDC_NPARKID ) );
	CString strParkID;
	edit.GetWindowText( strParkID );

	// 取得した公園関連テーブルIDとOIDが同じものをテーブルから検索
	_ICursorPtr ipCursor;

	//クエリフィルタを設定
	CString  strSubFields;
	strSubFields.Format( _T("%s,%s,%s"),
		sj::nationalpark_table::kParkTypeC, sj::nationalpark_table::kParkNumber, sj::nationalpark_table::kName );
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter( nullptr, strSubFields, _T("OBJECTID=%s"), strParkID );

	//検索する
	ipTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

	//検索して何も引っかからなかった場合は処理を中断
	if( !ipCursor )
	{
		if(messageFlag) AttrMessageBox( AheLoadString(IDS_FAIL_SEARCH), AheLoadString(IDS_CAPTION), MB_OK, NULL );
		clearText();
		return;
	}

	//一致する行は1つしかないはずなので、一行だけ読み込む
	//行の読み込みに失敗したら、処理を中断
	_IRowPtr ipRow;
	if( ipCursor->NextRow( &ipRow ) != S_OK || !ipRow )
	{
		if(messageFlag)
		{
			CString errorMessage;
			errorMessage.Format( AheLoadString(IDS_FAIL_FIND_NPOBJECT), strParkID, sj::nationalpark_table::kTableName );
			AttrMessageBox( errorMessage, AheLoadString(IDS_CAPTION), MB_OK, NULL );
		}
		clearText();
		return;
	}

	//テーブルの値を取得
	//vaParkTypeは値を取得するが、特に使用しない
	CComVariant vaParkNumber, vaName, vaParkType;
	vaParkNumber = AheGetAttribute( ipRow, sj::nationalpark_table::kParkNumber );
	vaName = AheGetAttribute( ipRow, sj::nationalpark_table::kName );

	//ドメイン名を取得
	CString strDomain;
	AheGetDomainNameAndValue( ipRow, sj::nationalpark_table::kParkTypeC, strDomain, vaParkType);

	ATLASSERT( vaParkType.vt == VT_I4 && vaParkNumber.vt == VT_I4 && vaName.vt == VT_BSTR );

	//各ラベルに情報を表示
	//ドメイン名をセット
	setText(strDomain, IDC_PARKTYPE_C);

	//公園名をセット
	CString strText = vaName.bstrVal;
	setText(strText, IDC_NAME);
	//公園番号をセット
	strText.Format(_T("%d"), vaParkNumber.intVal);
	setText(strText, IDC_PARKNUMBER);
}

/**
* @brief NATIONALPARK_TABLEを開く
* @return ITablePtr テーブルのオープンに成功した場合、NULL以外が返る
*/
ITablePtr CAttrNationalParkSiteDlg::GetNationalParkTable()
{
	ITablePtr ipTable;

	//選択されているフィーチャーを取得
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return ipTable;

	//フィーチャーからテーブルを取得
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table(&ipCurrentTable);
	if( !ipCurrentFeature )
		return ipTable;

	//ipCurrentTableと同じオーナーのNATIONALPARK_TABLEを取得
	ipTable = AheOpenSameOwnerTable(ipCurrentTable, sj::nationalpark_table::kTableName );
	return ipTable;
}

/**
* @brief Textに文字列を設定する
* @param strText Textに設定する文字列
* @param nClassID 文字列を設定するTextのID
*/
void CAttrNationalParkSiteDlg::setText( const CString& strText, INT nClassID )
{
	::SetWindowText(GetDlgItem(nClassID), (LPCTSTR)strText);
}

/**
* @brief 公園種別コード、公園番号、名称に空文字を設定する
* @note 関連テーブルから情報を取得できなかった場合に使用する
*/
void CAttrNationalParkSiteDlg::clearText()
{
	::SetWindowText(GetDlgItem(IDC_PARKTYPE_C), _T(""));
	::SetWindowText(GetDlgItem(IDC_PARKNUMBER), _T(""));
	::SetWindowText(GetDlgItem(IDC_NAME), _T(""));
}
