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

// AttrCitySurveyPointDlg.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "AttrCitySurveyPointDlg.h"

/// ウィンドウマネージャ配置マップ（ひまわりログ用）
BEGIN_WINDOW_MAP(CITY_SURVEY_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MESH)
				RCTOFIT(IDC_EDIT_MESH)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_RESEARCHDAY)
				RCTOFIT(IDC_EDIT_RESEARCHDAY)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_IDINMESH)
				RCTOFIT(IDC_EDIT_IDINMESH)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_TARGETDATA)
				RCTOFIT(IDC_EDIT_TARGETDATA)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ATTRNAME)
				RCTOFIT(IDC_EDIT_ATTRNAME)
				RCSPACE(10)
				RCTOFIT(IDC_STATIC_LAYER)
				RCTOFIT(IDC_EDIT_LAYER)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PRENAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PRENAME)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR2)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_AFTNAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_AFTNAME)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SPACE)
				RCTOFIT(-1)
				RCREST(IDC_STATIC_COMMENT)
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_REST, 0, RCMARGINS(0,0))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_POS)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_POS)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_NAMEOK)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_NAMEOK)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_CLOSE)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_CLOSE)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_CLEAR)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_CLEAR)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_CONSTRUCT)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_CONSTRUCT)
					ENDGROUP()
					RCSPACE(2)
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_STATIC_UNCERTAIN)
						RCSPACE(10)
						RCTOFIT(IDC_CHECK_UNCERTAIN)
					ENDGROUP()
				ENDGROUP()
				BEGINCOLS(WRCT_REST, 0, 0)
					RCSPACE(10)
					BEGINROWS(WRCT_REST, 0, RCMARGINS(0,0))
						RCREST(IDC_STATIC_PIC)
						BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
							RCREST(IDC_SLIDER_PIC)
							RCTOFIT(IDC_STATIC_PICNUM)
							RCTOFIT(IDC_BUTTON_DISPLAYPICTURE)
						ENDGROUP()
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
			RCSPACE(2)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					BEGINCOLS(WRCT_TOFIT, 0, 0)
						RCTOFIT(IDC_COMBO_CONFIRM)
						RCTOFIT(IDC_BUTTON_CONFIRM)
					ENDGROUP()
				ENDGROUP()
				RCSPACE(5)
				BEGINCOLS(WRCT_REST, 0, 0)
					RCREST(IDC_STATIC_MEMO)
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * クラスの明示的リンクを行うために、このクラスはCExportDlgクラスを継承し、属性ダイアログ側でCExportDlgを作成することにより
 * 属性ダイアログ側からフルにいじることができるクラス本体が作成されます。
 * 詳しくはhttp://ptvi.hp.infoseek.co.jp/vc/p_0000010.htmlを参照のこと
 *
 * @retval CAttrCitySurveyPointDlgを返します。
 */
//[Bug 5710]の要望によりショートカットで表示できるように、CreateDlgから変更
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlgByName( LPCTSTR lpcszTableName )
{
	CExportDlg* pDlg = NULL;
	pDlg = new CAttrCitySurveyPointDlg();
	if( pDlg )
		pDlg->SetFeatureClassName( lpcszTableName );

	return pDlg;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCitySurveyPointDlg メンバ関数
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCitySurveyPointDlg::CAttrCitySurveyPointDlg()
 * @brief コンストラクタ
 *
 * ブラシの初期化をしておきます。
 */
CAttrCitySurveyPointDlg::CAttrCitySurveyPointDlg() 
	: ATTR_BASE_CLASS(CITY_SURVEY_POINT_MAP, IDD_ATTRCITYSURVEYPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_MESH ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_RESEARCHDAY ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_IDINMESH ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_TARGETDATA ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_ATTRNAME ),
		m_wnd5( _T("EDIT"), this, IDC_EDIT_LAYER ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_PRENAME ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_AFTNAME ),
		m_wnd9( _T("CHECKBOX"), this, IDC_CHECK_POS ),
		m_wnd10( _T("CHECKBOX"), this, IDC_CHECK_NAMEOK ),
		m_wnd11( _T("CHECKBOX"), this, IDC_CHECK_CLOSE ),
		m_wnd12( _T("CHECKBOX"), this, IDC_CHECK_CLEAR ),
		m_wnd13( _T("CHECKBOX"), this, IDC_CHECK_CONSTRUCT ),
		m_wnd14( _T("CHECKBOX"), this, IDC_CHECK_UNCERTAIN ),
		m_wnd15( _T("BUTTON"), this, IDC_BUTTON_DISPLAYPICTURE ),
		m_wnd16( _T("COMBO"), this, IDC_COMBO_CONFIRM ),
		m_wnd17( _T("BUTTON"), this, IDC_BUTTON_CONFIRM )
{
	m_strBaseDir.Empty();
	m_strCode2Path.Empty();
	m_strToolPath.Empty();

	m_hFont = CreateFont(
		18, 0, 0, 0, 100,                           // 高さ，幅，Ｘ角度，Ｙ角度，太さ
		FALSE, FALSE, FALSE,                        // 斜体，下線，打消線
		SHIFTJIS_CHARSET,                           // キャラクタ設定
		OUT_DEFAULT_PRECIS,                         // 出力精度
		CLIP_DEFAULT_PRECIS,                        // クリップ精度
		DEFAULT_QUALITY,                            // 品質
		FIXED_PITCH | FF_DONTCARE,                // ピッチ
		AheLoadString(IDS_FONT) );                       // フォント種類
}

/** 
 * @fn HWND CAttrCitySurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief 子ダイアログを作成します。
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)でダイアログを作成します。
 *
 * @param hWndParent	[in]	親ウィンドウハンドル
 * @param dwInitParam	[in]	ダイアログ初期化パラメータ（現在特に使用していません）
 *
 * @retval 作成された子ダイアログのハンドルを返します。
 */
HWND CAttrCitySurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCitySurveyPointDlg::Delete()
 * @brief 作成した子ダイアログを削除します。
 *
 * ダイアログを削除するときに必ず呼んで下さい。呼ばないとメモリリークしてしまいます。
 */
void CAttrCitySurveyPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // 必ずnewで確保する前提
}

/**
 * @fn BOOL CAttrCitySurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCitySurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// 直接フィーチャに関係ないエディットボックス(例：参照用の路線内シーケンスや原稿番号入力)などを制御する
	// 編集開始前と編集開始後でも編集不可のときはDISABLEにする
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetDlgStatus(IDC_EDIT_MESH, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_RESEARCHDAY, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_IDINMESH, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_TARGETDATA, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_ATTRNAME, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_LAYER, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_PRENAME, FALSE, TRUE );
		SetDlgStatus(IDC_EDIT_AFTNAME, FALSE, TRUE );
		SetDlgStatus(IDC_CHECK_POS, FALSE );
		SetDlgStatus(IDC_CHECK_NAMEOK, FALSE );
		SetDlgStatus(IDC_CHECK_CLOSE, FALSE );
		SetDlgStatus(IDC_CHECK_CLEAR, FALSE );
		SetDlgStatus(IDC_CHECK_CONSTRUCT, FALSE );
		SetDlgStatus(IDC_CHECK_UNCERTAIN, FALSE );
		// 編集開始されているときについでに、SetFocusしておく
		::SetFocus( GetDlgItem(IDC_BUTTON_CONFIRM) );
	}
	FindPicture();

    return bRet;
}

/**
 * @fn void CAttrCitySurveyPointDlg::CreateControlRelation()
 * @brief 子ダイアログを実際に作成します。
 *
 * 実際の作成はControlRelに任せます。
 */
void CAttrCitySurveyPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// 親ウインドウハンドルを設定しなおす
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// メッシュ
		m_cControlRel.SetControl( IDC_EDIT_MESH,		TYPE_EDIT,		0,	0,  _T("MESH"),			NULL, m_strFeatureClassName );
		// 調査日
		m_cControlRel.SetControl( IDC_EDIT_RESEARCHDAY,	TYPE_EDITSTRING,0,	0,  _T("RESERCH_DATE"),	NULL, m_strFeatureClassName );
		// メッシュ内ID
		m_cControlRel.SetControl( IDC_EDIT_IDINMESH,	TYPE_EDIT,		0,	0,  _T("ID_IN_MESH"),	NULL, m_strFeatureClassName );
		// 対象データ
		m_cControlRel.SetControl( IDC_EDIT_TARGETDATA,	TYPE_EDITSTRING,0,	0,  _T("TARGET_DATA"),	NULL, m_strFeatureClassName );
		// 現調前種別
		m_cControlRel.SetControl( IDC_EDIT_ATTRNAME,	TYPE_EDITSTRING,0,	0,  _T("ATTR_NAME"),	NULL, m_strFeatureClassName );
		// データ採用元
		m_cControlRel.SetControl( IDC_EDIT_LAYER,		TYPE_EDITSTRING,0,	0,  _T("LAYER"),		NULL, m_strFeatureClassName );
		// 現調前名称
		m_cControlRel.SetControl( IDC_EDIT_PRENAME,		TYPE_EDITSTRING,0,	0,  _T("PRE_NAME"),		NULL, m_strFeatureClassName );
		// 現調後名称
		m_cControlRel.SetControl( IDC_EDIT_AFTNAME,		TYPE_EDITSTRING,0,	0,  _T("AFT_NAME"),		NULL, m_strFeatureClassName );
		// メモ
		m_cControlRel.SetControl( IDC_STATIC_MEMO,		TYPE_EDITSTRING,0,	0,  _T("MEMO"),			NULL, m_strFeatureClassName );
		// 位置移動
		m_cControlRel.SetControl( IDC_CHECK_POS,		TYPE_CHECKBOX,	0,	0,  _T("POS_F"),		NULL, m_strFeatureClassName );
		// 名称OK
		m_cControlRel.SetControl( IDC_CHECK_NAMEOK,		TYPE_CHECKBOX,	0,	0,  _T("NAME_F"),		NULL, m_strFeatureClassName );
		// 閉鎖
		m_cControlRel.SetControl( IDC_CHECK_CLOSE,		TYPE_CHECKBOX,	0,	0,  _T("CLOSE_F"),		NULL, m_strFeatureClassName );
		// 更地
		m_cControlRel.SetControl( IDC_CHECK_CLEAR,		TYPE_CHECKBOX,	0,	0,  _T("CLEAR_F"),		NULL, m_strFeatureClassName );
		// 建設中
		m_cControlRel.SetControl( IDC_CHECK_CONSTRUCT,	TYPE_CHECKBOX,	0,	0,  _T("CONST_F"),		NULL, m_strFeatureClassName );
		// 名称確認不可
		m_cControlRel.SetControl( IDC_CHECK_UNCERTAIN,	TYPE_CHECKBOX,	0,	0,  _T("UNCERTAIN_F"),	NULL, m_strFeatureClassName );
		// 確認コード
		m_cControlRel.SetControl( IDC_COMBO_CONFIRM,	TYPE_COMBO,	0,	0,  _T("CONFIRM_C"),	NULL, m_strFeatureClassName );
		// 確認ボタン
		m_cControlRel.SetControl( IDC_BUTTON_CONFIRM,	TYPE_BUTTON,	0,	0 );
	}
}

void CAttrCitySurveyPointDlg::GetRegCode()
{
 	CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\TOOLS\\ATTRIBUTETOOL" ) );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		TCHAR strBaseDir[255], strCode2Path[255], strToolPath[255];

		if( EmxGetRegString( _T("SurveyDir"), strBaseDir, 255, hOpenKey ) == TRUE )
			m_strBaseDir = strBaseDir;

		if( EmxGetRegString( _T("SurveyCode2Path"), strCode2Path, 255, hOpenKey ) == TRUE )
			m_strCode2Path = strCode2Path;

		if( EmxGetRegString( _T("SurveyToolPath"), strToolPath, 255, hOpenKey ) == TRUE )
			m_strToolPath = strToolPath;

		RegCloseKey( hOpenKey );
	}
}

void CAttrCitySurveyPointDlg::FindPicture()
{
	m_vecPath.clear();
	m_lSlider = 0;

	CEdit cEditCtrl(GetDlgItem( IDC_EDIT_MESH ) );
	LPTSTR lpszStr = (LPTSTR)_alloca( ( cEditCtrl.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cEditCtrl.GetWindowText( lpszStr, cEditCtrl.GetWindowTextLength() + 1 );
	CString strMesh = lpszStr;

	CEdit cTargetEdit(GetDlgItem( IDC_EDIT_TARGETDATA ) );
	LPTSTR lpszStrTargetData = (LPTSTR)_alloca( ( cTargetEdit.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cTargetEdit.GetWindowText( lpszStrTargetData, cTargetEdit.GetWindowTextLength() + 1 );
	CString strTargetData = lpszStrTargetData;

	CEdit cIDInMeshEdit(GetDlgItem( IDC_EDIT_IDINMESH ) );
	LPTSTR lpszStrIDInMesh = (LPTSTR)_alloca( ( cIDInMeshEdit.GetWindowTextLength() + 1 ) * sizeof(TCHAR) );
	cIDInMeshEdit.GetWindowText( lpszStrIDInMesh, cIDInMeshEdit.GetWindowTextLength() + 1 );

	// 現地調査ファイルは3桁の数値でナンバリングされている
	long lIDInMesh = _ttol( lpszStrIDInMesh );
	CString strIDInMeshData;
	if( lIDInMesh < 10 )
		strIDInMeshData = _T("00") + CString(lpszStrIDInMesh);
	else if( lIDInMesh < 100 )
		strIDInMeshData = _T("0") + CString(lpszStrIDInMesh);
	else
		strIDInMeshData = lpszStrIDInMesh;

	/// Code2Pathをもとに、変換
	CString cStrPath;
	CString cStrFormatWork;
	CString cStrWork1 = m_strCode2Path;
	CString cStrWork2;
	int nIndex = 0;
	int nFind;
	do
	{
		/// \を区切りとして、処理単位の取得
		nFind = cStrWork1.Find( TEXT( '\\'));
		if( 0 <= nFind)
		{
			cStrWork2 = cStrWork1.Mid( nIndex, nFind + 1);
			cStrWork1 = cStrWork1.Mid( nFind + 1);
		}
		else
			cStrWork2 = cStrWork1.Mid( nFind + 1);

		/// 処理単位に%が含まれる場合、Format(wsprintf)して、結合
		if( 0 <= cStrWork2.Find( TEXT( '%')))
		{
			cStrFormatWork.Format( cStrWork2, strMesh);
			cStrPath += cStrFormatWork;
		}
		else
			cStrPath += cStrWork2;
	}while( -1 != nFind);

	// フォルダ内のファイル検索する
	HANDLE				hFile;
	WIN32_FIND_DATA		ffd;
	CString				strDirPath, strTmp;
	LONG				ItemCnt = 0;
	strDirPath = m_strBaseDir + strTargetData + _T("\\PHOTO\\") + cStrPath + _T("\\") + _T("*.JPG");

	hFile = FindFirstFile( strDirPath, &ffd );
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		CString strErrorMessage;
		strErrorMessage = m_strBaseDir + strTargetData + _T("\\PHOTO\\") + cStrPath + AheLoadString( IDS_NOTEXISTPICTURE );
		AheMessageBox(strErrorMessage, _T("CitySurveyPoint.dll"), MB_OK );
		return ;	// ファイルが存在しない？
	}

	// フォルダ開けたら、メッシュ内IDのつく画像を見つける
	std::vector<CString> vecPath; 
	do {
		if ( ffd.cFileName[0] != '.' ) {
			// ディレクトリ
			if ( !(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
				TCHAR		strDrive[255], strDir[512], strFileName[255], strExt[255];

				_tsplitpath( ffd.cFileName, strDrive, strDir, strFileName, strExt );
				CString strCheckFileName = strFileName;
				if ( lstrcmpi( strExt, _T(".JPG") ) == 0 && strCheckFileName.Find(strIDInMeshData) != -1 ) {
					CString strParameters;
					strParameters += m_strBaseDir + strTargetData + _T("\\PHOTO\\") + cStrPath + _T("\\") + strCheckFileName + _T(".JPG ");
					m_vecPath.push_back( strParameters );
				}
			}
		}	
	} while ( FindNextFile(hFile, &ffd) );

	FindClose(hFile);

	CTrackBarCtrl cTrackBar( GetDlgItem( IDC_SLIDER_PIC ) );
	CStatic	cStatic( GetDlgItem( IDC_STATIC_PICNUM ) );
	CString strPicNum;
	strPicNum.Format( _T("%d"), m_vecPath.size() );
	cStatic.SetWindowText( strPicNum );
	if( m_vecPath.size() > 2 )
	{
		::ShowWindow(cTrackBar, true );
		cTrackBar.SetRange(0,m_vecPath.size()-2 );
	}
	else
		::ShowWindow(cTrackBar, false );
}

