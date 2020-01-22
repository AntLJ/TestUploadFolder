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

// AttrCityAdminDlg.cpp: CAttrCityAdminDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCityAdminDlg.h"

/// ウィンドウマネージャ配置マップ（都市地図行政界用）
BEGIN_WINDOW_MAP(CITY_ADMIN_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT2)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT3)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRCODE2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRCODE2)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_CHECK1)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC6)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_COMBO1)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC4)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT4)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC5)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_EDIT5)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC7)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_COMBO2)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_CHECK2)
				RCTOFIT(-1)
				RCTOFIT(IDC_CITY_ADMIN_CHECK3)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC8)
				RCTOFIT(-1)
				RCREST(IDC_COMBOBOXEX_CITY_ADMIN)
			ENDGROUP()
			RCSPACE(8)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CITY_ADMIN_STATIC9)
				RCTOFIT(-1)
				RCREST(IDC_CITY_ADMIN_COMBO3)
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
 * @retval CAttrCityAdminDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityAdminDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityAdminDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCityAdminDlg::CAttrCityAdminDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCityAdminDlg::CAttrCityAdminDlg()
	: ATTR_BASE_CLASS(CITY_ADMIN_MAP, IDD_ATTRCITYADMINDLG),
		m_wnd0( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT2 ), 
		m_wnd1( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT3 ), 
		m_wnd2( _T("CHECKBOX"), this, IDC_CITY_ADMIN_CHECK1 ), 
		m_wnd3( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT4 ), 
		m_wnd4( _T("EDIT"), this, IDC_CITY_ADMIN_EDIT5 ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_CITY_ADMIN_COMBO1 ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_CITY_ADMIN_COMBO2 ),
		m_wnd7( _T("CHECKBOX"), this, IDC_CITY_ADMIN_CHECK2 ),
		m_wnd8( _T("CHECKBOX"), this, IDC_CITY_ADMIN_CHECK3 ),
		m_wnd9( _T("COMBOBOX"), this, IDC_COMBOBOXEX_CITY_ADMIN ),
		m_wnd10( _T("COMBO"), this, IDC_CITY_ADMIN_COMBO3 ),
		m_wnd11( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd12( _T("EDIT"), this, IDC_EDIT_ADDRCODE2 )
{
}

/** 
 * @fn HWND CAttrCityAdminDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCityAdminDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCityAdminDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCityAdminDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCityAdminDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief 現在表示されるべきフィーチャのFeatureDefインデックスを渡します。
 * さらに、フィーチャの属性とは関係ないコントロールの制御もしておきます。
 * ダイアログを開いたときに最初にフォーカスの当たっていて欲しいコントロールの設定もしておきます。
 * 2次メッシュを跨いで接している傾斜が選択されている場合には、関連する傾斜も確保する
 *
 * @param lFeatureIndex	[in]	フィーチャ定義クラスのインデックス番号
 * @param lTableIndex	[in]	テーブル定義クラスのインデックス番号
 * @param lRowIndex		[in]	レコード定義クラスのインデックス番号
 * @param bForce		[in]	強制的に編集するかどうか
 * @param bEditable		[in]	編集可能かどうか
 *
 * @retval 意味がない（TRUEしか返さない）
 */
BOOL CAttrCityAdminDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_CITY_ADMIN_EDIT2) );
		// PGDBモードであればDISABLEに
		if( AheIsPGDBEditMode() )
		{
			SetDlgStatus( IDC_CITY_ADMIN_CHECK1, FALSE );
			SetDlgStatus( IDC_CITY_ADMIN_CHECK3, FALSE );
			SetDlgStatus( IDC_CITY_ADMIN_COMBO3, FALSE );
		}
		SetDlgStatus( IDC_COMBOBOXEX_CITY_ADMIN, TRUE );
	}
	else
		SetDlgStatus( IDC_COMBOBOXEX_CITY_ADMIN, FALSE );

	// [bug 7588] [e][要望]属性編集ダイアログで行政界の街区色を編集したい
	CComboBoxEx cComboCity = GetDlgItem( IDC_COMBOBOXEX_CITY_ADMIN );
	CFieldDef* pColorCode = GetCurrentFeatureDef()->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("COLORCODE") );
	long lColorCode = 0;
	if( pColorCode->m_vaValue.vt != VT_NULL )
	{
		lColorCode = pColorCode->m_vaValue.lVal;
		lColorCode -= 1;	// DB内の番号から、1引いた値がコンボボックスのインデックス値になるため
	}
	else
		lColorCode = 6;		// null値の場合は、コンボボックスのインデックス値は6

	cComboCity.SetCurSel( lColorCode );

	// [bug 8043] ADDRCODE2フィールドがあればコントロールを表示
	bool bHasAddrCode2 = GetCurrentFeatureDef()->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCODE2") );
	::EnableWindow( GetDlgItem( IDC_EDIT_ADDRCODE2 ), bHasAddrCode2 ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_STATIC_ADDRCODE2 ), bHasAddrCode2 ? TRUE : FALSE );

    return bRet;
}

/**
 * @fn void CAttrCityAdminDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCityAdminDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 都市地図行政界用
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT2,		TYPE_EDITSTRING, 	0,	0,	_T("CITYCODE"),		NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT3,		TYPE_EDITSTRING, 	0,	0,	_T("ADDRCODE"),		NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_CHECK1,	TYPE_CHECKBOX,		0,	0,	_T("TMPCODE_F"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_COMBO1,	TYPE_COMBO,			0,	0,	_T("ADDRCLASS_C"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT4,		TYPE_EDIT,			0,	0,	_T("GAIKUFUGO"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_EDIT5,		TYPE_EDITSTRING, 	0,	0,	_T("EXTGAIKUFUGO"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_COMBO2,	TYPE_COMBO, 		0,	0,	_T("AREACLASS_C"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_CHECK2,	TYPE_CHECKBOX,		0,	0,	_T("MANAGE_F"),		NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_CITY_ADMIN_CHECK3,	TYPE_CHECKBOX,		0,	0,	_T("DETACH_F"),		NULL, CITY_ADMIN_TABLE_NAME );		
		m_cControlRel.SetControl( IDC_COMBOBOXEX_CITY_ADMIN, TYPE_OTHER,		0,	0);
		m_cControlRel.SetControl( IDC_CITY_ADMIN_COMBO3,	TYPE_COMBO,			0,	0,	_T("CONVTYPE_C"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE2,		TYPE_EDITSTRING,	0,	0,	_T("ADDRCODE2"),	NULL, CITY_ADMIN_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),		NULL, CITY_ADMIN_TABLE_NAME );
	}
}

/**
 * @fn void CAttrCityAdminDlg::ChangeToSeaValue()
 * @brief エリア判別種別が海になったときに、関連種別を自動的に変更する
 */
void CAttrCityAdminDlg::ChangeToSeaValue()
{
	// エリア判別種別コンボボックスの取得＆値取得
	CComboBox	cComboAreaClass( GetDlgItem( IDC_CITY_ADMIN_COMBO2 ) );
	int iAreaClass = cComboAreaClass.GetCurSel();

	// エリア判別種別の値が海（２）の時
	if( iAreaClass == 2 )
	{
		CString strCityCode = _T("00000");
		CString strAddrCode = _T("000000");
		CString strGaikuFugou = _T("");
		CString strExtGaikuFugou = _T("");

		// 市区町村コードの値を00000にセット
		SetDlgItemText( IDC_CITY_ADMIN_EDIT2, strCityCode );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT2 );

		// 住所コードの値を000000にセット
		SetDlgItemText( IDC_CITY_ADMIN_EDIT3, strAddrCode );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT3 );

		// 街区符号の値をNULLにセット
		SetDlgItemText( IDC_CITY_ADMIN_EDIT4, strGaikuFugou );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT4 );

		// 街区符号拡張の値をNULLにセット
		SetDlgItemText( IDC_CITY_ADMIN_EDIT5, strExtGaikuFugou );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_EDIT5 );

		// 住所整備種別の値を町丁目界未整備にセット
		CComboBox	cComboAddrClass( GetDlgItem( IDC_CITY_ADMIN_COMBO1 ) );
		cComboAddrClass.SetCurSel( 0 );
		m_cControlRel.ControlChanged( IDC_CITY_ADMIN_COMBO1 );
	}

}

/**
 * @fn BOOL CAttrCityAdminDlg::ErrorCheck()
 * @brief ダイアログが閉じられるときにエラーチェックを行う
 *
 * @return エラーでなかったら TRUE、エラーだったら FALSE
 */
BOOL CAttrCityAdminDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == CITY_ADMIN_TABLE_NAME )
		{
			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			LONG lCityCode, lAddrCode, lAddrClass_c, lAreaClass_c, lGaikuFugo, lOID;
			CComBSTR	bstrCityCode, bstrAddrCode, bstrGaikuFugo, bstrExtGaikuFugo;
			CString		strDummy;
			BOOL		bIsGaikuNull = FALSE;
			BOOL		bIsExtGaikuNull = FALSE;

			// 市区町村コード取得
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("CITYCODE") );
			bstrCityCode = pFieldDef->m_vaValue.bstrVal;
			strDummy = bstrCityCode.m_str;
			lCityCode = _ttol( strDummy );

			// 住所コード取得
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCODE") );
			bstrAddrCode = pFieldDef->m_vaValue.bstrVal;
			strDummy = bstrAddrCode.m_str;
			lAddrCode = _tcstol(strDummy, NULL, 16);

			// 住所整備種別取得
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCLASS_C") );
			lAddrClass_c = pFieldDef->m_vaValue.lVal;

			// 街区符号取得
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("GAIKUFUGO") );
			if( pFieldDef->m_vaValue.vt == VT_NULL )
				bIsGaikuNull = TRUE;
			else
				lGaikuFugo = pFieldDef->m_vaValue.lVal;

			// 拡張街区符号取得
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("EXTGAIKUFUGO") );
			if( pFieldDef->m_vaValue.vt == VT_NULL )
				bIsExtGaikuNull = TRUE;
			else
			{
				bstrExtGaikuFugo = pFieldDef->m_vaValue.bstrVal;

				CString strExGaikuFugo;
				strExGaikuFugo.Format( _T("%s"), CString(bstrExtGaikuFugo) );

				if( strExGaikuFugo.Find( _T(" ") ) != -1 )
				{
					AttrMessageBox( AheLoadString( IDS_UNNECCESARYSPACE ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}

				USES_CONVERSION;
				LONG lStrNum = strExGaikuFugo.GetLength();
				BOOL bIsExistWord = FALSE;
				BOOL bIsExistNum = FALSE;
				for(INT i = 0; i < lStrNum; i++ )
				{
					CString str = CString_GetAt2(strExGaikuFugo, i);
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );
					if ( isasciinumber( lpstr[0] ) )
						bIsExistNum = TRUE;
					else
						bIsExistWord = TRUE;
				}
				if( bIsExistNum && !bIsExistWord )
				{
					AttrMessageBox( AheLoadString( IDS_ONLYHALFWIDTHNUMBER ), AheLoadString( IDS_CAPTION ), MB_OK, NULL );
					return FALSE;
				}
			}

			// エリア判別種別取得
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("AREACLASS_C") );
			lAreaClass_c = pFieldDef->m_vaValue.lVal;

			it->GetFeature()->get_OID(&lOID);


			// 市区町村コードの桁数チェック
			if( bstrCityCode.Length() != 5 )
			{
				AttrMessageBox( AheLoadString( IDS_CITYCODE5FIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			// 住所コードの桁数チェック
			if( bstrAddrCode.Length() != 6 )
			{
				AttrMessageBox( AheLoadString( IDS_ADDRCODE6FIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			// [bug 8042] ADDRCODE2チェック
			pFieldDef = it->GetFieldDef( CITY_ADMIN_TABLE_NAME, _T("ADDRCODE2") );
			if( pFieldDef )
			{
				CComVariant vaAddrCode2 = pFieldDef->m_vaValue;
				// [bug 8189] VT_NULLは除外
				if( VT_BSTR == vaAddrCode2.vt )
				{
					CString strAddrCode2(vaAddrCode2);
					if( strAddrCode2.GetLength() != 6 )
					{
						AttrMessageBox( AheLoadString( IDS_ADDRCODE26FIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
						return FALSE;
					}
				}
			}

			// エリア判別種別が海の時
			if( lAreaClass_c == 2 )
			{
				// 市区町村コードのチェック
				if( lCityCode != 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_CITYCODE00000 ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

					return FALSE;
				}

				// 住所コードのチェック
				if( lAddrCode != 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_ADDRCODE000000 ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// エリア判別種別が海の時には、必ず住所整備種別は町丁目界未整備
				if( lAddrClass_c != 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_ADDRCLASSUNSPECIFY ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// 街区符号のチェック
				if( !bIsGaikuNull )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_TOWNBLOCKNULL ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// 拡張街区符号のチェック
				if( !bIsExtGaikuNull )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_EXTOWNBLOCKNULL ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}
			// エリア判別種別が海以外の時
			else
			{
				// 市区町村コードのチェック
				if( lCityCode == 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_NOTCITYCODE00000 ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// 市区町村コードのチェック
				if( lCityCode > 47999 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_CITYCODERANGE ),lOID );
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// 住所コードのチェック
				if( lAddrCode == 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_NOTADDRCODE000000 ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}

				// 住所整備種別のチェック
				if( lAddrClass_c == 0 )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_NOTADDRCLASSUNSPECIFY ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}

			
			// 街区符号のチェック	
			if( lAddrClass_c == 0 || lAddrClass_c == 3 )
			{
				// 街区符号はNULLでないといけない
				if( !bIsGaikuNull )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_TOWNBLOCKNULL2 ),lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}
			else
			{
				// 街区符号はNULLではいけない
				if( ( bIsGaikuNull && bIsExtGaikuNull ) || ( !bIsGaikuNull && !bIsExtGaikuNull ) )
				{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_INPUTTOWNBLOCKOREXTOWNBLOCK ), lOID);
					AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}
