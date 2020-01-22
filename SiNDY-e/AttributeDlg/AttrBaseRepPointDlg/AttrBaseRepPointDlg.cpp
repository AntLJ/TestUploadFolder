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

// AttrBaseRepPointDlg.cpp: CAttrBaseRepPointDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseRepPointDlg.h"
//#include "icrsint.h"
//#import "C:\Program files\Common Files\System\Ado\msado15.dll" rename_namespace("MSXML")  rename("EOF", "ADOEOF")

//#import "c:\winnt\system32\msxml.dll" 
//using namespace MSXML;

#include "stdio.h"
#include "io.h"

/// ウィンドウマネージャ配置マップ（中縮代表点用）
BEGIN_WINDOW_MAP(BASE_REP_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADDRCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADDRCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ADMINNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ADMINNAME)
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
 * @retval CAttrBaseRepPointDlgを返します。
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseRepPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseRepPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseRepPointDlg::CAttrBaseRepPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrBaseRepPointDlg::CAttrBaseRepPointDlg()
	: ATTR_BASE_CLASS(BASE_REP_POINT_MAP, IDD_ATTRBASEREPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_ADDRCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_ADMINNAME ), 
		m_wnd3( _T("EDIT"), this, IDC_EDIT_ZIPCODE ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
	m_bDoAutoInput = FALSE;
}

/** 
 * @fn HWND CAttrBaseRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrBaseRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseRepPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrBaseRepPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrBaseRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBaseRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_EDIT_ADDRCODE) );
	}

	// [bug 8045] フィールドがあればコントロールを表示
	bool bHasZipCode = GetCurrentFeatureDef()->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ZIPCODE") );
	::EnableWindow( GetDlgItem( IDC_EDIT_ZIPCODE ), bHasZipCode ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_STATIC_ZIPCODE ), bHasZipCode ? TRUE : FALSE );


    return bRet;
}

/**
 * @fn void CAttrBaseRepPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrBaseRepPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// 中縮代表点用
		m_cControlRel.SetControl( IDC_EDIT_ADDRCODE,	TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),	NULL, BASE_REP_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_ADMINNAME,	TYPE_EDITSTRING,0,	0,	_T("ADMINNAME"),NULL, BASE_REP_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_ZIPCODE,		TYPE_EDITSTRING,0,	0,	_T("ZIPCODE"),NULL, BASE_REP_POINT_TABLE_NAME );
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,	_T("SOURCE"),	NULL, BASE_REP_POINT_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrBaseRepPointDlg::ErrorCheck()
 * @brief エラーがないかどうか調べる
 *
 * @return	エラーがなかったらTRUE、エラーがあればFALSE
 */
BOOL CAttrBaseRepPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == BASE_REP_POINT_TABLE_NAME )
		{

			// 現在編集中の値を習得
			CFieldDef* pFieldDef;
			CComVariant vaAddrCode, vaAdminName;
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ADDRCODE") );
			vaAddrCode = pFieldDef->m_vaValue;
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ADMINNAME") );
			vaAdminName = pFieldDef->m_vaValue;

			USES_CONVERSION;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			CString strAddrCode, strAddminName;
			strAddrCode.Format( _T("%s"), OLE2T(vaAddrCode.bstrVal) );
			strAddminName.Format( _T("%s"), OLE2T(vaAdminName.bstrVal) );


			if( strAddrCode.GetLength() != 11 )
			{
				strMessage.Format( AheLoadString( IDS_NOT11FIGURES ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			if( strAddrCode.Find( _T(" ") ) != -1 )
			{
				strMessage.Format(AheLoadString( IDS_NONEEDSPACE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			if( strAddminName.Find( _T(" ") ) != -1 )
			{
				strMessage.Format( AheLoadString( IDS_NOPOINTNAME ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			// [bug 8045] ZIPCODEチェック
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ZIPCODE") );
			if( pFieldDef )
			{
				CComVariant vaZipCode = pFieldDef->m_vaValue;
				CString strZipCode;
				// [bug 8188] VT_NULLは除外
				if( VT_BSTR == vaZipCode.vt )
				{
					strZipCode = vaZipCode;
					CString strZipErr;
					if( strZipCode.GetLength() != 7 )
					{
						strZipErr = AheLoadString( IDS_NOT7FIGURES );
					}
					if( strZipCode.SpanIncluding( _T("1234567890")) != strZipCode )
					{
						strZipErr += AheLoadString( IDS_EXCEPTNUMBER );
					}
					if( !strZipErr.IsEmpty() )
					{
						strMessage.Format(AheLoadString( IDS_ERRPOSTCODE ),lOID);
						AttrMessageBox( strMessage + strZipErr, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
						return FALSE;
					}
				}
			}
		}
	}

	return TRUE;
}

/**
 * @fn BOOL CAttrBaseRepPointDlg::ReadDBPathFromReg()
 * @brief レジストリキー"ChukiMaster"の値（DBファイルのフルパス）を読み込む
 *
 * @retval キーがあればTURUE、無ければFALSE
 */
BOOL CAttrBaseRepPointDlg::ReadDBPathFromReg()
{
	// レジストリからファイルパス読み込み
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if( hOpenKey )
	{
		// 
		TCHAR szFileName[1024];
		if( EmxGetRegString( _T("ChukiMaster"), (LPTSTR)szFileName, hOpenKey ) == TRUE )
		{
			m_strTableFileName = (CString)szFileName;
			RegCloseKey( hOpenKey );
			return TRUE;
		}
		// キーが見つからなかった場合、Closeしてから抜ける
		RegCloseKey( hOpenKey );
	}
	return FALSE;
}


/**
 * @fn BOOL CAttrBaseRepPointDlg::SearchNameFromFile( const CString strAddCode, CString &strAddName )
 * @brief アクセスのDBファイル(mdb)に接続して11桁コードに対応する住所の名称を取得する
 *
 * @param const CString strAddCode	[in]	11桁コード
 * @param CString &strAddName	    [out]	対応住所
 *
 * @retval 対応データが見つかった場合はTURE、無ければFALSE
 */
BOOL CAttrBaseRepPointDlg::SearchNameFromFile( const CString strAddCode, CString &strAddName )
{

	_ConnectionPtr pConnect;   // Connectionオブジェクト
	_CommandPtr pCommand;      // Commandオブジェクト
	_RecordsetPtr pRecoedset;  // Recordsetオブジェクト

	try {
		// COMの初期化処理
		::CoInitialize(NULL);

		// ADOオブジェクトの生成
		pConnect.CreateInstance(__uuidof(Connection));
		pCommand.CreateInstance(__uuidof(Command));
		pRecoedset.CreateInstance(__uuidof(Recordset));

		// データベースへの接続
		CString strConnect;
		strConnect.Format( _T( "Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s"), m_strTableFileName );
		pConnect->Open( _bstr_t(strConnect), L"", L"", adConnectUnspecified );

		// SQL文の実行
		CString strSqlQuery;
		strSqlQuery.Format( _T("SELECT * FROM Chuki_master_all where 住所コード = \"%s\" "), strAddCode ); 
		pRecoedset = pConnect->Execute( _bstr_t(strSqlQuery), NULL, NULL );

		// EOFまたはBOFだったら不一致
		if( pRecoedset->GetADOEOF() || pRecoedset->GetBOF() )
			return FALSE;
		// 値がHITしたときは返す
		else{
			// ここの"注記"はDB側が英語化されたらでいいだろう
			_variant_t vaValue = pRecoedset->GetCollect( "注記" );
			strAddName = vaValue.bstrVal;
		}

	}
	catch(_com_error &e)
	{
		MessageBox(e.Description());
		return FALSE;
	}

	return TRUE;
}
