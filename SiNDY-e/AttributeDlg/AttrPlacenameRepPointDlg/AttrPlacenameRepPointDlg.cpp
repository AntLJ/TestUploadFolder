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

// AttrPlacenameRepPointDlg.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "AttrPlacenameRepPointDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ELEVENCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ELEVENCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE)
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
 * @retval CAttrPlacenameRepPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrPlacenameRepPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrPlacenameRepPointDlg::CAttrPlacenameRepPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrPlacenameRepPointDlg::CAttrPlacenameRepPointDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_PLACENAMEDLG), m_isFirst(false)
{
	// 各コントロールをサブクラス用に準備
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_ELEVENCODE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ELEVENCODE ) )
		( IDC_EDIT_NAME, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_NAME ) )
		( IDC_EDIT_ZIPCODE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ZIPCODE ) )
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

/** 
 * @fn HWND CAttrPlacenameRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrPlacenameRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrPlacenameRepPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrPlacenameRepPointDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrPlacenameRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrPlacenameRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	// ここでフラグを立てるとうまくいくので、とりあえずここで立てる
	m_isFirst=true;

	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 最初にフォーカスされるコントロール
		::SetFocus( GetDlgItem(IDC_EDIT_ELEVENCODE) );
	}

	// 市区町村コード+住所コードで11桁住所コードを生成
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( pFeatureDef )
	{
		CString strVal;
		CFieldDef* cityFieldDef = pFeatureDef->GetFieldDef( placename_rep_point::kTableName, placename_rep_point::kCityCode );
		if( cityFieldDef ){
			if( VT_BSTR == cityFieldDef->m_vaValue.vt)
				strVal = cityFieldDef->m_vaValue.bstrVal;

			CFieldDef* addrFieldDef = pFeatureDef->GetFieldDef( placename_rep_point::kTableName, placename_rep_point::kAddrCode );
			if( addrFieldDef ){
				if( VT_BSTR == addrFieldDef->m_vaValue.vt)
					strVal += addrFieldDef->m_vaValue.bstrVal;
			}
			else if( !strVal.IsEmpty() ){
				strVal.Empty();
			}
		}
		SetDlgItemText(IDC_EDIT_ELEVENCODE, strVal);
	}

	return bRet;
}

/**
 * @fn void CAttrPlacenameRepPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrPlacenameRepPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 11桁住所コード
		std::list<CString> codeList = boost::assign::list_of
			(placename_rep_point::kCityCode)
			(placename_rep_point::kAddrCode);

		m_cControlRel.SetControl( IDC_EDIT_ELEVENCODE, TYPE_EDITSTRING, 0, 0, codeList, 0, placename_rep_point::kTableName);
		// 代表点名称
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, placename_rep_point::kName, 0, placename_rep_point::kTableName );
		// 郵便番号
		m_cControlRel.SetControl( IDC_EDIT_ZIPCODE, TYPE_EDITSTRING, 0, 0, placename_rep_point::kZipCode, 0, placename_rep_point::kTableName );
		// 情報ソース
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, placename_rep_point::kTableName );
	}
}

BOOL CAttrPlacenameRepPointDlg::ErrorCheck(){
	using namespace placename_rep_point;

	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == kTableName )
		{
			// OBJECTID取得
			LONG lOID = -1;
			it->GetFeature()->get_OID(&lOID);

			// 市区町村コード取得
			CFieldDef* pFieldDef = it->GetFieldDef( kTableName, kCityCode );
			CString strCityCode;
			if( pFieldDef ){
				CComVariant vaCityCode = pFieldDef->m_vaValue;
				if( VT_BSTR == vaCityCode.vt )
					strCityCode = vaCityCode;
			}

			// 住所コード取得
			pFieldDef = it->GetFieldDef( kTableName, kAddrCode );
			CString strAddrCode;
			if( pFieldDef ){
				CComVariant vaAddrCode = pFieldDef->m_vaValue;
				if( VT_BSTR == vaAddrCode.vt )
					strAddrCode = vaAddrCode;
			}

			CString strMessage,strMessageTmp;
			// 11桁住所コードの桁数チェック
			if( strCityCode.GetLength() + strAddrCode.GetLength() != 11 )
				strMessage = AheLoadString( IDS_ELEVENFIGURES );

			// 市区町村コードが数字のみで構成されているか
			if( strCityCode.SpanIncluding( _T("1234567890") ) != strCityCode ){
				strMessage += AheLoadString( IDS_EXCEPTNUMBER_ELEVENCODE );
			}
			// 11桁住所コードに半角スペースが入っていないか
			// （ツールで属性値を格納した際に半角スペースが入る場合があるらしい）
			if( strCityCode.Find( _T(" ") ) != -1 || strAddrCode.Find( _T(" ") ) != -1){
				strMessage += AheLoadString( IDS_NONEEDSPACE );
			}

			// 郵便番号チェック
			pFieldDef = it->GetFieldDef( kTableName, kZipCode );
			CString strZipCode;
			if( pFieldDef ){
				CComVariant vaZipCode = pFieldDef->m_vaValue;
				// VT_NULLは除外
				if( VT_BSTR == vaZipCode.vt ){
					strZipCode = vaZipCode;
					// 郵便番号の桁数チェック
					if( strZipCode.GetLength() != 7 )
						strMessage += AheLoadString( IDS_NOTSEVENFIGURES );

					// 郵便番号が数字のみで構成されているか
					if( strZipCode.SpanIncluding( _T("1234567890") ) != strZipCode )
						strMessage += AheLoadString( IDS_EXCEPTNUMBER_ZIPCODE );
				}
			}

			if( !strMessage.IsEmpty() ){
				strMessageTmp.Format(AheLoadString( IDS_ERRORCOMMON ), lOID);
				strMessage = strMessageTmp + strMessage;
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				return FALSE;
			}
		}
	}
	return TRUE;
}

// fieldDefから編集前の初期値を文字列で取得する
CString CAttrPlacenameRepPointDlg::getInitStr(CFieldDef* fieldDef){
	CString ret;
	CComVariant vaInitValue = fieldDef->m_vaInitValue;
	if( VT_BSTR == vaInitValue.vt )
		ret = vaInitValue;
	return ret;
}
