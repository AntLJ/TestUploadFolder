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

/*! \file AttributeDlgObj.cpp
	\brief CAttributeDlgObj クラスのインプリメンテーション
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2001-09-07
*/
#include "stdafx.h"
#include "AttributeDlg.h"
#include "AttributeDlgObj.h"
#include <math.h>
#include "charutil.h"
#ifdef _USE_CHILDDLG_SHORTCUT
#include "shlobj.h"
_COM_SMARTPTR_TYPEDEF(IShellLink, __uuidof(IShellLink));
#endif // ifdef _USE_CHILDDLG_SHORTCUT
#include "type_convert.h"
#include "local_define.h"

using namespace sindy::schema;

const unsigned int WM_REFRESHFEATURE  = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));       //!< 再描画の依頼を受けるためのメッセージ
const unsigned int WM_DISABLECONTROL  = RegisterWindowMessage(_T("WM_DISABLECONTROL"));       //!< 再描画の依頼を受けるためのメッセージ

typedef CExportDlg* __stdcall CreateDlg();
typedef CExportDlg* __stdcall CreateDlgByName( LPCTSTR lpcszTableName );

/// ウィンドウマネージャ配置マップ（ベース用）
BEGIN_WINDOW_MAP(BASEMAP)
	BEGINROWS(WRCT_REST,0,0)
		BEGINCOLS(WRCT_REST,0,0)
			BEGINROWS(WRCT_TOFIT,0,RCMARGINS(0,0))
				RCREST(IDC_FEATTREE)
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
					RCTOFIT(IDC_BUTTON_GRIDMODE)
					RCTOFIT(IDC_BUTTON_ALIASMODE)
				ENDGROUP()
			ENDGROUP()
			RCFIXED(ID_WIN_SIZEBAR,4)
			BEGINROWS(WRCT_REST,0,RCMARGINS(0,0))
				RCREST(IDC_STATIC_BASE)
#ifdef _FOR_ENGLISH		// [bug 11183]通常の場合、名称表示は要らない
				BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,4))
					RCSPACE(4)
					BEGINCOLS(WRCT_PCT, 0, 50 )
						RCREST(IDC_STATIC_OFFICIALNAME)
					ENDGROUP()
					RCSPACE(4)
					BEGINCOLS(WRCT_PCT, 0, 50 )
						RCREST(IDC_STATIC_ALTERNATIVENAME)
					ENDGROUP()
					RCSPACE(4)
				ENDGROUP()
#endif
			ENDGROUP()
		ENDGROUP()
		RCSPACE(4)
		RCTOFIT(-1)
		BEGINCOLS(WRCT_TOFIT,0,4)
			RCTOFIT(IDC_BUTTON_DELETEWARN)
			RCREST(IDC_STATIC_MESSAGE)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_CONFIRMATION)
			RCSPACE(4)
			RCTOFIT(IDOK)
			RCSPACE(4)
			RCTOFIT(IDC_BUTTON_CLOSE)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

#ifdef _USE_CHILDDLG_SHORTCUT
HMODULE LoadShellLinkLibrary( LPCTSTR lpcszFileName )
{
	HMODULE hMod = NULL;

	HRESULT hr = 0;
	if( lpcszFileName )
	{ 
		CString strFileName( lpcszFileName );
		// ショートカットがあるかどうかを先に確認する
		IPersistFilePtr ipFile;
		if( SUCCEEDED( ipFile.CreateInstance( CLSID_ShellLink ) ) )
		{
			// ショートカットファイルはどんな名前にしても必ず「.lnk」が後ろにつくらしい（なんじゃそりゃ！！）
			// 例えば、エクスプローラ上で「CITY_DISP_LINE.dll」というファイル名のショートカットがあるなら、それは「CITY_DISP_LINE.dll.lnk」じゃないとオープンできない
			if( SUCCEEDED( hr = ipFile->Load( CComBSTR(strFileName + _T(".lnk")), STGM_READ | STGM_SHARE_DENY_WRITE ) ) )
			{
				IShellLinkPtr ipLink;
				if( SUCCEEDED( ipFile->QueryInterface( IID_IShellLink, (void**)&ipLink ) ) )
				{
					TCHAR strShortPath[MAX_PATH + 1], strPath[MAX_PATH + 1];
					WIN32_FIND_DATA wfd = {0};
					if( SUCCEEDED( ipLink->GetPath( strShortPath, MAX_PATH, (WIN32_FIND_DATA *)&wfd, SLGP_SHORTPATH ) ) )
					{
						DWORD dwRet = GetLongPathName( strShortPath, strPath, MAX_PATH );
						if( 0 < dwRet && dwRet < MAX_PATH )
						{
							strFileName = strPath;
						}
					}
				}
			}
		}
		hMod = ::LoadLibrary( strFileName );
	}

	return hMod;
}
#endif // #ifdef _USE_CHILDDLG_SHORTCUT


// ダイアログを初期化するイベントハンドラ
LRESULT CAttributeDlgObj::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//[Bug 4694]smwでダイアログ作成されると、カーソルハンドルがNULLとなってしまうので、
	//とりあえず、カーソル割り当てておく。本来ならばsmw側で設定されておくべき
	SetClassLongPtr(m_hWnd, GCLP_HCURSOR, LONG_PTR( LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))));
	m_bClosedDialog = FALSE;
	// ツリーサブクラス化
	m_cTreeCtrl.SubclassWindow( GetDlgItem( IDC_FEATTREE ) );
	m_cTreeCtrl.ModifyStyle( NULL, TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS );

	// サブクラス化
	m_cTree.SubclassWindow( GetDlgItem( IDC_FEATTREE ) );
	m_cBtn1.SubclassWindow( GetDlgItem( IDC_BUTTON_GRIDMODE ) );
	m_cBtn2.SubclassWindow( GetDlgItem( IDC_BUTTON_ALIASMODE ) );
	m_cBtnClose.SubclassWindow( GetDlgItem( IDC_BUTTON_CLOSE ) );
	m_cBtnSave.SubclassWindow( GetDlgItem( IDOK ) );
	m_cBtnDelWarn.SubclassWindow( GetDlgItem( IDC_BUTTON_DELETEWARN ) );
	m_cBtnConfirmation.SubclassWindow( GetDlgItem( IDC_BUTTON_CONFIRMATION ) );

	// 新しいWinMgrで使い方が変わった？
	m_wndSizerBar.Create( WS_CHILD|WS_VISIBLE|SS_NOTIFY, *this, m_winMgr, ID_WIN_SIZEBAR );

	// ボタンにビットマップ割り当て
	CButton cAliasButton = GetDlgItem( IDC_BUTTON_ALIASMODE );
	CButton cGridButton  = GetDlgItem( IDC_BUTTON_GRIDMODE  );
	cAliasButton.SetBitmap( (m_bIsDBNameMode == TRUE ) ? m_hDBNameModeBmp : m_hAliasModeBmp  );
	cGridButton.SetBitmap(  (m_bIsGridMode   == TRUE ) ? m_hGridModeBmp   : m_hCommonModeBmp );

	// ウィンドウマネージャによる再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	// ダイアログサイズ変更
	// 名称系コントロール分で縦幅を+15します
	if( m_cDlgRect.Width() > 0 && m_cDlgRect.Height() > 0 )
		MoveWindow( &m_cDlgRect );
	else
		// [bug 11183]名称表示の有無により、高さを調整する
#ifdef _FOR_ENGLISH
		ResizeClient( 550, 400, FALSE );
#else
		ResizeClient( 550, 370, FALSE );
#endif

	// モーダルの場合のみ
	if( m_bIsModal )
	{
		// ツリーにフィーチャを登録（表示される前に登録する必要アリ）
		if( !m_bIsLQAttribute )
			m_cTreeCtrl.SetFeatures( &m_listFeatureDef );
		else
		{
			m_cTreeCtrl.SetLQs( &m_listLQRowDef );

			for( std::list<CLQRowDef>::iterator i = m_listLQRowDef.begin(); i != m_listLQRowDef.end(); i++ )
			{
				// フィーチャクラス名またはオブジェクトクラス名の取得
				// ここで取得した名前から、レイヤ名.dllを読み込むことになります。
				CString strTableName;
				_IRowPtr ipRow = i->GetFeature();
				ITablePtr ipTable;
				IObjectClassPtr ipObjectClass;

				ipRow->get_Table( &ipTable );
				ipObjectClass = ipTable;
				if( ipObjectClass )
				{
					CComBSTR bstrTableName;
					CString	strTemp;
					ipObjectClass->get_AliasName( &bstrTableName );
					strTemp = bstrTableName;
					strTableName = strTemp.Right( strTemp.GetLength() - strTemp.Find( _T(".") ) - 1 ).MakeUpper();
				}

				std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( strTableName );
				if( itDlg == m_mapDlg.end() )
				{
					// フィーチャの種類によって表示形態を変える
					CExportDlg* cExportDlg = NULL;

					// レイヤ別の子ダイアログを取得
					cExportDlg = SetChildDlg(strTableName);
					if( !cExportDlg )	// エイリアスで存在しない [bug 7516]
					{
						strTableName = AheGetFeatureClassName( ipRow );  // テーブル名を取得
						itDlg = m_mapDlg.find( strTableName );
						if( itDlg == m_mapDlg.end() )
						{
							cExportDlg = SetChildDlg(strTableName);
						}
					}
					if( cExportDlg )
					{
						cExportDlg->Create( m_hWnd );
						cExportDlg->SetFeatureDefList( &m_listLQRowDef );
						m_mapDlg[strTableName] = cExportDlg;
					}
				}
			}
		}
		// ダイアログにフィーチャを登録（m_listFeatureDef がセットされた後）
		SetSelectedFeaturesToDlg();
	}

	m_bIsEnding = FALSE;

	// キャンセルボタンフラグ
	m_bIsCancel = FALSE;

	// 新規作成の時は必ずsindyNotApplyRuleEnableで渡される
	// そのときはデフォルトでOKボタンが押せるようにならない
	// といけない
	if( sindyeNotApplyRuleEnable == m_sAttributeEditType )
		SetButton( TRUE );

	// 編集開始状態を取得
	esriEditState state = esriStateNotEditing;
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	ipEditor->get_EditState( &state );

	if( state != esriStateEditing )
		::SetFocus( GetDlgItem(IDC_BUTTON_CLOSE) );

	LoadProperty();

	// [bug 11183]普通の場合、名称表示は要らない
#ifdef _FOR_ENGLISH
	static const std::vector<UINT> useFontEdits = boost::assign::list_of
		( IDC_STATIC_OFFICIALNAME )
		( IDC_STATIC_ALTERNATIVENAME );

	for( const auto& ctrl : useFontEdits )
	{
		SetLocalFont( GetDlgItem( ctrl ) );
	}
#else
	CStatic cStaticOfficial( GetDlgItem( IDC_STATIC_OFFICIALNAME ) );
	CStatic cStaticAlternative( GetDlgItem( IDC_STATIC_ALTERNATIVENAME ) );

	cStaticOfficial.ShowWindow(SW_HIDE);
	cStaticAlternative.ShowWindow(SW_HIDE);
#endif

	return 0;
}

// キーが押されたとき用イベントハンドラ
LRESULT CAttributeDlgObj::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_TAB )
	{
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール
		UINT uiKeyState = GetAsyncKeyState(VK_SHIFT);
		if( (_rotl(uiKeyState, 1 ) & 0x01) == 0x01 )
			PrevDlgCtrl();
		else
			NextDlgCtrl();

		bHandled = TRUE;
		return 1;
	}
	else if( wParam == VK_RETURN )
	{
		CWindow	wnd = GetFocus();
		INT iWndID = wnd.GetDlgCtrlID();

		if ( iWndID == IDC_BUTTON_CLOSE )
		{
			m_bIsCancel = TRUE;
			// キャンセルボタンを押したと同じこと
			CloseDlg( FALSE );
			bHandled = TRUE;
			return 1;
		}
		else
		{
			CloseDlg( TRUE );
			bHandled = TRUE;
			return 1;
		}
	}
	else if( wParam == VK_ESCAPE )
	{
		m_bIsCancel = TRUE;
		// キャンセルボタンを押したと同じこと
		CloseDlg( FALSE );
		bHandled = TRUE;
		return 1;
	}

	bHandled = FALSE;
	return 0;
}

// ウィンドウマネージャ用イベントハンドラ
LRESULT CAttributeDlgObj::OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bIsEnding == TRUE ) return 0;	// 終了処理中なら全て処理キャンセル

	NMWINMGR& nmw = *(NMWINMGR*)lParam;

	if (nmw.code==NMWINMGR::GET_SIZEINFO) 
	{
		if (wParam==IDC_FEATTREE) 
		{
			CWindow wndTree = GetDlgItem(IDC_FEATTREE);
			CRect rect;
			wndTree.GetWindowRect(&rect);
			nmw.sizeinfo.szDesired = rect.Size();

			bHandled = TRUE; // handled--important!
			return -1;
		}
	} 
	else if( nmw.code == NMWINMGR::SIZEBAR_MOVED) 
	{
		m_winMgr.MoveRect( wParam, nmw.sizebar.ptMoved, this );
		m_winMgr.SetWindowPositions(this);

		ResizeClientDlg();

		bHandled = TRUE;
		return -1;
	}

	return 0;
}

// ダイアログの最大・最小を扱うイベントハンドラ
LRESULT CAttributeDlgObj::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO*>( lParam);

	m_winMgr.GetMinMaxInfo( this, pMinMaxInfo);
	bHandled = TRUE;
	return 0;
}

// ダイアログのサイズ変更があったときのイベントハンドラ
LRESULT CAttributeDlgObj::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT iWidth	= LOWORD(lParam);
	INT iHeight = HIWORD(lParam);

	m_winMgr.CalcLayout(iWidth, iHeight, this);
	m_winMgr.SetWindowPositions(this);
	ResizeClientDlg();

	bHandled = TRUE;
	return 0;
}

// * ダイアログを閉じる際のイベントハンドラ
LRESULT CAttributeDlgObj::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( lParam == -1 )
		CloseDlg( TRUE );			// 保存終了
	else
		CloseDlg( FALSE );			// 破棄終了

	return 0;
}

// 「OK」ボタンが押されたときのイベントハンドラ
LRESULT CAttributeDlgObj::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CloseDlg( TRUE );				// 保存終了
	return 0;
}

// キャンセル」ボタンが押されたときのイベントハンドラ
LRESULT CAttributeDlgObj::OnCloseClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bIsCancel = TRUE;
	CloseDlg( FALSE );				// 破棄終了
	return 0;
}

// ツリービューでフィーチャ選択が変更された時のイベントハンドラ
LRESULT CAttributeDlgObj::OnChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( lParam == CFeatureTree::FEATUREITEM || lParam == CFeatureTree::OUTERROWITEM )
		SetSelectedFeaturesToDlg();

	return 0;
}

// WM_NOTIFY 用イベントハンドラ
LRESULT CAttributeDlgObj::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_bIsEnding ) return 0;	// 終了処理中なら全て処理キャンセル

	 if( wParam == IDC_FEATTREE ) {
		NMTVCUSTOMDRAW*  lpCustomDraw = (NMTVCUSTOMDRAW*)lParam;
		if( lpCustomDraw->nmcd.dwDrawStage == CDDS_PREPAINT )
		{
			//  アイテムの描画できるようにする
			bHandled = TRUE;
			return CDRF_NOTIFYITEMDRAW;
		}
		if( lpCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT )
		{
			if( !m_bIsLQAttribute )
			{
				UINT pFlags = 0;
				CRect rect = lpCustomDraw->nmcd.rc;
				HTREEITEM item = m_cTreeCtrl.HitTest( rect.TopLeft(), &pFlags );
				if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::FEATUREITEM ) {
					CFeatureDef* pFeatureDef = m_cTreeCtrl.GetFeatureDef( item );
					if( pFeatureDef )
					{
						if( pFeatureDef->Changed() )
						{
							//  テキスト色設定
							lpCustomDraw->clrText = RGB(0,0,0);
							//  背景色設定
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
					}
				} else if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::OUTERROWITEM ) {
					CRowDef* pRowDef = m_cTreeCtrl.GetRowDef( item );
					if( pRowDef )
					{
						if( pRowDef->Changed() )
						{
							//  テキスト色設定
							lpCustomDraw->clrText = RGB(0,0,0);
							//  背景色設定
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
					}
				}
			}
			else
			{
				UINT pFlags = 0;
				CRect rect = lpCustomDraw->nmcd.rc;
				HTREEITEM item = m_cTreeCtrl.HitTest( rect.TopLeft(), &pFlags );
				if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::FEATUREITEM ) {
					CLQRowDef* pLQRowDef = m_cTreeCtrl.GetLQRowDef( item );
					if( pLQRowDef )
					{
						if( pLQRowDef->Changed() )
						{
							//  テキスト色設定
							lpCustomDraw->clrText = RGB(0,0,0);
							//  背景色設定
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
						if( pLQRowDef->m_bError )
						{
							//  テキスト色設定
							lpCustomDraw->clrText = RGB(0,0,0);
							//  背景色設定
							lpCustomDraw->clrTextBk = RGB(255,0,0);
						}
					}
				} else if( m_cTreeCtrl.ItemKind( item ) == CFeatureTree::OUTERROWITEM ) {
					CRowDef* pRowDef = m_cTreeCtrl.GetRowDef( item );
					if( pRowDef )
					{
						if( pRowDef->Changed() )
						{
							//  テキスト色設定
							lpCustomDraw->clrText = RGB(0,0,0);
							//  背景色設定
							lpCustomDraw->clrTextBk = RGB(255,168,126);
						}
					}
				}
			}
		}
	}

	return 0;
}

// フィーチャに変更があったときのイベントハンドラ
LRESULT CAttributeDlgObj::OnChangedFeature(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 変更かかっていない場合にOKボタンがDISABLEになってしまうため消去
	SetButton( (BOOL)lParam );

	return 0;
}

// エイリアスとフィールド名の切り替えボタンが押されたときのイベントハンドラ
LRESULT CAttributeDlgObj::OnClickedAlias_or_Field(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if( m_bIsGridMode)
	{
		m_bIsDBNameMode = ( m_bIsDBNameMode == TRUE ) ? FALSE : TRUE;
		CButton cAliasButton = GetDlgItem( IDC_BUTTON_ALIASMODE );
		cAliasButton.SetBitmap( (m_bIsDBNameMode == TRUE ) ? m_hDBNameModeBmp : m_hAliasModeBmp );
		if( m_cExportDlg )
		{
			m_cExportDlg->SetAliasOrField( m_bIsDBNameMode );
			SetSelectedFeaturesToDlg();
		}
	}
	return 0;
}

// コモンコントロール表示とグリッド表示を切り替えるボタンのイベントハンドラ
LRESULT CAttributeDlgObj::OnClickedCommon_Or_Grid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_bIsGridMode = ( m_bIsGridMode == TRUE ) ? FALSE : TRUE;
	CButton cGridButton  = GetDlgItem( IDC_BUTTON_GRIDMODE  );
	cGridButton.SetBitmap( (m_bIsGridMode == TRUE ) ? m_hGridModeBmp : m_hCommonModeBmp );
	SetSelectedFeaturesToDlg();

	return 0;
}

// モーダルの時にキーを捕まえる
LRESULT CAttributeDlgObj::OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTALLKEYS;
}

// 警告解除ボタンを押したときのイベントハンドラ
LRESULT CAttributeDlgObj::OnClickedDeleteWarn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;

	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );

	// 何もセットされていなかった
	if( lFeatureIndex < 0 && lTableIndex < 0 && lRowIndex < 0 )
	{
		ATLASSERT(FALSE);
		return 0;
	}

	if( (ULONG)lFeatureIndex < m_listFeatureDef.size() )
	{
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		advance( it, lFeatureIndex );
		it->SetWarnDelete();	// 警告状態を解除
		SetSelectedFeaturesToDlg();
	}

	if( (ULONG)lFeatureIndex < m_listLQRowDef.size() )
	{
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		advance( it, lFeatureIndex );
		it->SetWarnDelete();	// 警告状態を解除
		SetSelectedFeaturesToDlg();
	}
	return 0;
}

// 確認ボタンを押したときのイベントハンドラ
LRESULT CAttributeDlgObj::OnClickedConfirmation(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;

	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );

	// 何もセットされていなかった
	if( lFeatureIndex < 0 && lTableIndex < 0 && lRowIndex < 0 )
	{
		ATLASSERT(FALSE);
		return 0;
	}

	_IRowPtr ipRow;
	if( (ULONG)lFeatureIndex < m_listFeatureDef.size() )
	{
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		advance( it, lFeatureIndex );
		ipRow = it->GetFeature();
	}
	if( (ULONG)lFeatureIndex < m_listLQRowDef.size() )
	{
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		advance( it, lFeatureIndex );
		ipRow = it->GetFeature();
	}

	if( ipRow )
	{
		if( !m_cVerifyLog.AppendVerifyLog( m_cArcHelper.GetMap(), ipRow ) )
			AheMessageBox( NULL, AheLoadString( IDS_FAILTOWRITECHECKED ),  AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING|MB_TASKMODAL, WARNLEVEL_NONE );
	}

	return 0;
}

// 選択されているフィーチャを変更依頼を受けるイベントハンドラ
LRESULT CAttributeDlgObj::OnSetCurrentFeature(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return SendMessage( m_cTreeCtrl, uMsg, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////
//
// IActiveViewEvents インターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeDlgObj::ContentsChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ContentsCleared()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ItemAdded(VARIANT Item)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ItemDeleted(VARIANT Item)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ItemReordered(VARIANT Item, LONG toIndex)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::SelectionChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::ViewRefreshed(IActiveView * View, esriViewDrawPhase phase, VARIANT data, IEnvelope * Envelope)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::AfterDraw(IDisplay * Display, esriViewDrawPhase phase)
{
	switch (phase)
	{
		case esriViewNone:
			break;
		case esriViewBackground:
			break;
		case esriViewGeography:
			break;
		case esriViewGeoSelection:
			break;
		case esriViewGraphics:
			break;
		case esriViewGraphicSelection:
			break;
		case esriViewForeground:
			if( !m_bIsEnding )
			{
				if( m_cExportDlg )
					m_cExportDlg->SendMessage( WM_REFRESHFEATURE, NULL, NULL );

				::SendMessage( m_cTreeCtrl, WM_REFRESHFEATURES, NULL, NULL );
			}
			break;
		default:
			break;
	}

	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::AfterItemDraw(SHORT Index, IDisplay * Display, esriDrawPhase phase)
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::FocusMapChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeDlgObj::SpatialReferenceChanged()
{
  return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
// IEditEvents インターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeDlgObj::OnSelectionChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnCurrentLayerChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnCurrentTaskChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnSketchModified()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnSketchFinished()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::AfterDrawSketch(IDisplay * pDpy)
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnStartEditing()
{
	// ダイアログが開いている状態で、編集の開始をするとバージョンが異なるために、
	// 編集をかけても、その内容が反映されないので、ダイアログは閉じることにする
	CloseDlg( FALSE );
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnStopEditing(VARIANT_BOOL Save)
{
	// 編集中に編集終了されたらダイアログ終了処理
	// -> なぜなら編集終了するとツールが強制的に使用できなくなる（ArcMapのせい）から
	//    また、ここに来る時点で既に終了処理が始まっているらしく、フィーチャの保存が出来ない（IEditEvents2で出来るが、あえてしない）
	//    従って、全てはきしてダイアログ終了
	CloseDlg( FALSE );

	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnConflictsDetected()
{
	m_lstUpdate.clear();

	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnUndo()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnRedo()
{
	return S_OK;
}
STDMETHODIMP CAttributeDlgObj::OnCreateFeature(IObject * obj)
{
	// 属性ダイアログではフィーチャが作られることはないが、念のため
	// 作成されたフィーチャを確保
	_IRowPtr ipRow(obj);

	if ( ipRow ) {
#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("CreateFeature %d\n"), lOID);
#endif
		m_lstUpdate.push_back(ipRow);
		return S_OK;
	} else {
		return S_FALSE;
	}
}
STDMETHODIMP CAttributeDlgObj::OnChangeFeature(IObject * obj)
{
	// 変更があったフィーチャを確保
	_IRowPtr ipRow(obj);

	if ( ipRow ) {
#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("ChangeFeature %d\n"), lOID);
#endif
		m_lstUpdate.push_back(ipRow);
		return S_OK;
	} else {
		return S_FALSE;
	}
}
STDMETHODIMP CAttributeDlgObj::OnDeleteFeature(IObject * obj)
{
	// 属性ダイアログではフィーチャが削除されることはないが、念のため
	// 削除されたフィーチャを確保
	_IRowPtr ipRow(obj);

	if ( ipRow ) {
#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("DeleteFeature %d\n"), lOID);
#endif
		m_lstUpdate.push_back(ipRow);
		return S_OK;
	} else {
		return S_FALSE;
	}
}
/////////////////////////////////////////////////////////////////////////////
//
// IAttributeDlgObj インターフェース
//
/////////////////////////////////////////////////////////////////////////////

// メンバ変数を初期化する
STDMETHODIMP CAttributeDlgObj::Init(IDispatch *hook)
{
	IDispatchPtr ipDisp( hook );
	m_cArcHelper.Init( ipDisp );
	m_cTreeCtrl.Init( m_cArcHelper.GetApp() );

	// グリッド表示かコモンコントロール表示かのフラグ初期化（初期設定はコモンコントロール表示）
	m_bIsGridMode = FALSE;
	// フィールド名表示かエイリアス表示かのフラグ初期化（初期設定はエイリアス表示）
	m_bIsDBNameMode = FALSE;

	// 子ダイアログのタブ位置の初期化
	m_lRoadTabNumber = 0;
	m_lAccessTabNumber = 0;
	m_lHwyNodeTabNumber = 0;
	m_lDRMATabNumber = 0;

	// キャンセルボタンフラグ
	m_bIsCancel = FALSE;
	// 変更フラグ
	m_bIsChanged = FALSE;
	// ダイアログに入ってくる最初のフィーチャフラグ
	m_bIsFirst = TRUE;
	// モーダルかモードレスかのフラグ
	m_bIsModal = FALSE;
	// 流出リンクだけ拾ってくるフラグ
	m_lMode = 0;
	// フィーチャ再描画範囲
	m_lRefreshBuf = 5;

	m_hMessageWND = NULL;
	m_hMessageWNDEnglish = NULL;	// [bug 11183]名称チップ用

	m_bIsLineHilight   = false;
	m_bIsVertexHilight = false;

	return S_OK;
}

// フィーチャをダイアログにセットするインターフェイス
STDMETHODIMP CAttributeDlgObj::SetItem(_IRow* pRow, _ISet* pSet, IGeometry* pGeometry, VARIANT_BOOL IsCreated )
{
	// [Bug4090]で発覚したことだが、エディットタイプをいままでSetItemで初期化していたけれども、
	// それは不適切だった。put_EditTypeの後にSetItemをやっているツールもあって、そのときに初期値に
	// もどされてしまっていることがあったため、Initでやるようにする
	m_sAttributeEditType = sindyeApplyRule;
	m_lstUpdate.clear();

	// フィーチャの場合
	if( !pSet )
	{
		m_listLQ.clear();
		m_bIsLQAttribute = FALSE;
		AddFeatures( pRow );

		// 最初に選択したものをダイアログに出すためここでオブジェクトIDを取得しておく
		if( m_bIsFirst )
		{
			pRow->get_OID( &m_lFirstOID );
			m_bIsFirst = FALSE;
		}

		// フィーチャクラス取得
		IFeatureClassPtr ipFeatureClass;
		if( AheGetFeatureClass( m_cArcHelper.GetMap(), pRow, &ipFeatureClass))
		{
			CTargetItems cTargetItems;
			cTargetItems.clear();
			// 道路リンクかどうか？
			// [bug 5552] 歩行者リンクにも対応
			if( AheIsNetworkLink( ipFeatureClass ) )
			{
				// 2次メッシュを跨いでいるかどうかをチェックし、跨いでいれば接続している道路を取得
				if( AheIsNetworkLinkCrossMesh( m_cArcHelper.GetMap(), pRow, cTargetItems ) )
				{
					// 選択された、2次メッシュ関連の道路を登録する
					for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						AddFeatures( ipFeatureOnMesh );
					}
				}
			}
			// リンク内属性の場合
			else if( AheCompareFeatureName( ipFeatureClass, SEGMENTATTR_TABLE_NAME) )
			{
				// 2次メッシュを跨いでいるかどうかをチェックし、跨いでいれば接続している道路を取得
				if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), pRow, cTargetItems ) )
				{
					// 選択された、2次メッシュ関連の道路を登録する
					for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						AddFeatures( ipFeatureOnMesh );
					}
				}
			}
			// 傾斜の場合
			else if( AheCompareFeatureName( ipFeatureClass, GRAD_TABLE_NAME) )
			{
				// 2次メッシュを跨いでいるかどうかをチェックし、跨いでいれば接続している道路を取得
				if( AheIsFeatureCrossMesh( m_cArcHelper.GetMap(), pRow, cTargetItems ) )
				{
					// 選択された、2次メッシュ関連の道路を登録する
					for( CTargetItems::iterator it = cTargetItems.begin(); it != cTargetItems.end(); it++ )
					{
						IFeaturePtr ipFeatureOnMesh = it->GetFeature();
						AddFeatures( ipFeatureOnMesh );
					}
				}
			}
		}
	}
	// リンク列の場合
	else
	{
		m_cFeatureItems.clear();
		m_bIsLQAttribute = TRUE;
		AddLQs( pRow, pSet, pGeometry, ( IsCreated == VARIANT_TRUE ? TRUE : FALSE ) );
	}

	return S_OK;
}

// モードレスダイアログでセレクション内のフィーチャを表示するインターフェイス
STDMETHODIMP CAttributeDlgObj::DoModeless()
{
	// リンク列属性ダイアログの場合には、リンク列がセットされたかどうか調べる必要がある
	// これがないと、リンク列属性ダイアログを表示している状態で、さらにリンク列を選択しようとして
	// 選択できなかった場合、ツリー表示部が消えてしまうため。
	if( m_bIsLQAttribute == TRUE )
	{
		if( m_listLQ.size() < 1 )
			return S_OK;
	}

	// ダイアログ作成
	if( !IsWindow() )
		Create( m_cArcHelper.GetHWND() );

	// コネクション接続
	if( m_dwViewCookie == 0 )
		ConnectToEvents();

	// 通常のフィーチャとリンク列とで場合わけする
	if( !m_bIsLQAttribute )
	{
		if( !SetFeatures( m_cFeatureItems ) )
			return S_OK;
	}
	else
	{
		if( !SetLQs( m_listLQ ) )
			return S_OK;
	}

	ShowWindow( SW_SHOW );
	m_bIsModal = FALSE;
	// ダイアログに渡し終わった後に初期化しておかないといけない
	m_cFeatureItems.clear();
	m_listLQ.clear();

	return S_OK;
}

// SetFeature() でセットされたフィーチャをモーダルダイアログで表示する
STDMETHODIMP CAttributeDlgObj::DoModal(VARIANT_BOOL* IsChanged)
{
	ConnectToEvents();

	m_bIsModal = TRUE;
	// 通常のフィーチャとリンク列とで場合わけする
	if( !m_bIsLQAttribute )
	{
		if( !SetFeatures( m_cFeatureItems ) )
			return S_OK;
	}
	else
	{
		if( !SetLQs( m_listLQ ) )
			return S_OK;
	}
	// ダイアログに渡し終わった後に初期化しておかないといけない
	m_cFeatureItems.clear();
	CAxDialogImpl<CAttributeDlgObj>::DoModal();
	m_bIsModal = FALSE;
	if( m_bIsChanged )
		*IsChanged = ( m_bIsChanged == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

	if( m_bIsCancel )
		return S_FALSE;
	else
		return S_OK;
}

// 呼び出しもとのツールからダイアログを閉じる用のインターフェイス
STDMETHODIMP CAttributeDlgObj::Close()
{
	ConnectToEvents( FALSE );

	// ツリー内部コレクションを消去
	m_cTreeCtrl.Clear();

	// ダイアログリストを全て破棄する
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg* pTest = it->second;
		pTest->Delete();
	}

	for( std::map<CString, HMODULE>::iterator itModule = m_mapModule.begin(); itModule != m_mapModule.end(); itModule++ )
	{
		HMODULE hModule = itModule->second;
		FreeLibrary( hModule );
	}

	m_mapDlg.clear();
	m_mapModule.clear();
	m_hCurrentWnd = NULL;
	m_cExportDlg = NULL;
	// 登録用フィーチャのクリア
	m_cFeatureItems.clear();
	m_listLQ.clear();
	// フィーチャリストクリア
	m_listFeatureDef.clear();
	m_listLQRowDef.clear();
	
	// 再描画してごみを消す
	m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

	// ダイアログを消す
	if( IsWindow() )
	{
		if( m_bIsModal )
			EndDialog(0);		// モーダルの場合
		else
			DestroyWindow();	// モードレスの場合
	}

	m_bClosedDialog = TRUE;

	m_hMessageWND = NULL;
	m_hMessageWNDEnglish = NULL;	// [bug 11183]名称チップ用
	// こんなところで、フラグ初期化しておく・・・
	m_bIsFirst = TRUE;
	WriteProperty();

	return S_OK;
}

STDMETHODIMP CAttributeDlgObj::SetMode(long lMode)
{
	m_lMode = lMode;
	return S_OK;
}

STDMETHODIMP CAttributeDlgObj::GetActiveItem(_IRow **pRow)
{
	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;
	// ツリーで何を選択しているかを取得
	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );

	// 何もセットされていなかった
	if( lFeatureIndex < 0 && lTableIndex < 0 && lRowIndex < 0 )
		return S_FALSE;

	// 選択しているフィーチャのiteratorを取得
	if( !m_bIsLQAttribute )
	{
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		advance( it, lFeatureIndex );

		// 選択中のフィーチャ取得
		IFeaturePtr ipFeature;
		ipFeature = it->GetFeature();

		if( ipFeature != NULL )
		{
			*pRow = ipFeature;
			(*pRow)->AddRef();

		}else
			return S_FALSE;
	}
	else
	{
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		advance( it, lFeatureIndex );

		// 選択中のフィーチャ取得
		_IRowPtr ipRow;
		ipRow = it->GetFeature();

		if( ipRow )
		{
			*pRow = ipRow;
			(*pRow)->AddRef();

		}else
			return S_FALSE;
	}

	return S_OK;
}

// 現在のオペレーションモードを返すインターフェイス
STDMETHODIMP CAttributeDlgObj::get_DoOperation(VARIANT_BOOL *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = BOOL2VB(m_bDoOperation);
	
	return S_OK;
}

// オペレーション状態をセットするインターフェイス
STDMETHODIMP CAttributeDlgObj::put_DoOperation(VARIANT_BOOL newVal)
{
	m_bDoOperation = VB2BOOL(newVal);
	return S_OK;
}

// 現在の編集ルールを返すインターフェイス
STDMETHODIMP CAttributeDlgObj::get_EditType(sindyeAttributeEditType *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = m_sAttributeEditType;
	return S_OK;
}

// 属性編集ダイアログで、ルールに従ってダイアログのOKボタンを押せるようにするかどうかを設定する
STDMETHODIMP CAttributeDlgObj::put_EditType(sindyeAttributeEditType newVal)
{
	m_sAttributeEditType = newVal;
	return S_OK;
}

// 使用しない
STDMETHODIMP CAttributeDlgObj::put_AutoMode(VARIANT_BOOL newVal)
{
	return S_OK;
}

// コンストラクタ
CAttributeDlgObj::CAttributeDlgObj() : m_winMgr(BASEMAP), m_dwViewCookie(0), m_dwEditCookie(0), m_cDlgRect( -1, -1, -1, -1 ),
	m_cTree( WC_TREEVIEW, this, IDC_FEATTREE ),
	m_cBtn1( _T("BUTTON"), this, IDC_BUTTON_GRIDMODE ),
	m_cBtn2( _T("BUTTON"), this, IDC_BUTTON_ALIASMODE ),
	m_cBtnClose( _T("BUTTON"), this, IDC_BUTTON_CLOSE ),
	m_cBtnSave( _T("BUTTON"), this, IDOK ),
	m_cBtnDelWarn( _T("BUTTON"), this, IDC_BUTTON_DELETEWARN ),
	m_cBtnConfirmation( _T("BUTTON"), this, IDC_BUTTON_CONFIRMATION )
{
	m_hCurrentWnd = NULL;
	m_cExportDlg = NULL;
	m_cFeatureItems.clear();
	m_listLQ.clear();
}

// フィーチャをメンバ変数（m_cFeatureItems）に追加する
void CAttributeDlgObj::AddFeatures( IFeaturePtr ipFeature )
{
	m_cFeatureItems.push_back( ipFeature );
	long lSize = m_cFeatureItems.size();
}

// リンク列をメンバ変数（m_listLQ）に追加する
void CAttributeDlgObj::AddLQs( _IRowPtr ipLQRow, _ISetPtr ipLQRowSet, IGeometryPtr ipLQGeom, BOOL bIsCreated )
{
	LQROWDEF def;

	def.obj = ipLQRow;
	def.shp = ipLQGeom;
	def.deflt = bIsCreated;

	def.flg = m_cLQInfo.GetLoadQueueType( AheGetFeatureClassName( ipLQRow ), sindyeLoadQueueInfoInfoTableName );
	def.name = m_cLQInfo.GetLoadQueueInfo( (sindyeRoadQueueType)def.flg, sindyeLoadQueueInfoAliasName );

	// LQ を登録
	ipLQRowSet->Reset();
	IUnknownPtr ipUnk;
	while( SUCCEEDED( ipLQRowSet->Next( &ipUnk ) ) )
	{
		if( ipUnk == NULL ) break;

		ROWDEF lqdef;
		lqdef.obj = ipUnk;

		def.lqlist.push_back( lqdef );
	}
	m_listLQ.push_back( def );
}

// m_cFeatureItems に格納したフィーチャをツリーに表示する
BOOL CAttributeDlgObj::SetFeatures( CTargetItems cFeatureSet )
{
	// モードレスの場合にしか入る必要はないモーダルの場合は他のフィーチャー触れないのでここに入らないでよし
	if( !m_bIsModal )
	{
		// 現在編集中のフィーチャがあれば、保存してから
		m_bIsSetFeature = TRUE;
		if( !StoreFeature() )
		{
			// 保存に失敗したら登録用フィーチャのクリア
			m_cFeatureItems.clear();

			m_bIsSetFeature = FALSE;
			return FALSE;
		}
		m_bIsSetFeature = FALSE;

		// 既存のものを全てクリア
		if( IsWindow() )
			m_cTreeCtrl.DeleteAllItems();

		// 現在表示しているダイアログがあれば、それの CCrontrolRel と CFeatureDef の関係をクリア
		if( m_cExportDlg != NULL )
			m_cExportDlg->ClearFeatureDefs();

		m_listFeatureDef.clear();
	}

	if( cFeatureSet.size() == 0 )
		return FALSE;

	// フィーチャ定義クラスのリストを作成
	for( CTargetItems::iterator i = cFeatureSet.begin(); i != cFeatureSet.end(); i++ )
	{
		IFeaturePtr ipFeature = i->GetFeature();

		CFeatureDef featDef;
		featDef.clear();
		featDef.SetArcHelper( m_cArcHelper.GetApp() );
		featDef.SetFeature( ipFeature );

		// フィーチャが編集可能かどうか調べる
		ISiNDYRulePtr		ipRule;
		ipRule.CreateInstance( CLSID_SiNDYRuleObj );
		
		IWorkspacePtr ipWorkspace;
		ITablePtr ipTable;
		ipFeature->get_Table( &ipTable );
		IDatasetPtr ipDataset( ipTable );
		if( ipDataset )
			ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			_ASSERTE( ipWorkspace != NULL );
		else {
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			IWorkspaceEditPtr ipWorkspaceEdit( ipWorkspace );
			if(ipWorkspaceEdit != 0) {
				ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited );
				if( vbIsBeingEdited == VARIANT_TRUE )
				{
					if( !ipRule )
						ipRule.CreateInstance( CLSID_SiNDYRuleObj );

					IGeometryPtr ipGeometry;
					ipFeature->get_ShapeCopy( &ipGeometry );

					ipRule->IsEditableFeature( ipFeature, ipGeometry, sindyeEditAttribute, &(featDef.m_sindyeEditableType), &(featDef.m_bstrIsEditableFeature) );

					if ( featDef.m_sindyeEditableType == sindyeEditableEditable )
						featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANEDITFEATURE );
				}
				else {
					featDef.m_sindyeEditableType = sindyeEditableNoEditable;
					featDef.m_bstrIsEditableFeature = AheLoadString( IDS_DONTSTARTEDITING );
				}
			}
			else {
				featDef.m_sindyeEditableType = sindyeEditableNoEditable;
				featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANTEDITFEATURE );
			}
		}

		IDatasetEditInfoPtr ipDatasetEditInfo;
		ipDatasetEditInfo = ipDataset;
		VARIANT_BOOL vaCanEdit = VARIANT_FALSE;
		if( ipDatasetEditInfo )
			ipDatasetEditInfo->get_CanEdit( &vaCanEdit );
		if( vaCanEdit == VARIANT_FALSE )
		{
			featDef.m_sindyeEditableType = sindyeEditableNoEditable;
			featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANTEDITFEATURE );
		}
		m_listFeatureDef.push_back( featDef );
	}

	cFeatureSet.clear();

	// 最初にクリックしたものを先頭に出すための処理
	std::list<CFeatureDef>::iterator itfirst = m_listFeatureDef.begin();
	IFeaturePtr ipFeature = itfirst->GetFeature();

	LONG lOID = 0;
	ipFeature->get_OID( &lOID );
	if( lOID != m_lFirstOID )
	{
		std::list<CFeatureDef> reverse;
		reverse.clear();
		// リストを入れ替える
		// まず、リストに最初のフィーチャをいれておく
		for( std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin(); it != m_listFeatureDef.end(); it++ )
		{
			ipFeature = it->GetFeature();
			ipFeature->get_OID( &lOID );

			if( lOID == m_lFirstOID )
			{
				reverse.push_front(*it);
				break;
			}
		}
		// 終わったら、最初のフィーチャ以外を入れる
		for( std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin(); i != m_listFeatureDef.end(); i++ )
		{
			ipFeature = i->GetFeature();
			ipFeature->get_OID( &lOID );

			if( lOID != m_lFirstOID )
				reverse.push_back(*i);
		}

		// m_listFeatureDefを置き換える
		m_listFeatureDef = reverse;
	}

	// モーダル（新規作成＆注記編集）の時
	if( m_bIsModal )
	{
		// 特定のダイアログが先に出てくるようにする
		static std::set<CString> prev_layer = boost::assign::list_of
			(building::kTableName)          // 家形は階数より先
			(sj::subway_node::kTableName);  // 地下鉄ノードはリンクよりも先

		// 対象のレイヤが後尾にいたら逆順にする
		// レイヤが3つ以上あったらどうすんだという話もあるが、とりあえず家形と地下鉄NWモデルに限った話なのでまあいいか。。
		if( prev_layer.end() != prev_layer.find(AheGetFeatureClassName( m_listFeatureDef.rbegin()->GetFeature() )) )
			std::reverse(m_listFeatureDef.begin(), m_listFeatureDef.end());
	}
	// モードレスの時
	else
	{
		// ツリーにフィーチャを登録（表示される前に登録する必要アリ）
		m_cTreeCtrl.SetFeatures( &m_listFeatureDef );
		// ダイアログにフィーチャを登録（m_listFeatureDef がセットされた後）
		SetSelectedFeaturesToDlg();
	}
	return TRUE;
}

// m_listLQ に格納したフィーチャをツリーに表示する
BOOL CAttributeDlgObj::SetLQs( std::list<LQROWDEF>& listRowDef )
{
	// モードレスの場合にしか入る必要はないモーダルの場合は他のフィーチャー触れないのでここに入らないでよし
	if( !m_bIsModal )
	{
		// 現在編集中のフィーチャがあれば、保存してから
		m_bIsSetFeature = TRUE;
		if( !LQStoreFeature() )
		{
			// 保存に失敗したら登録用フィーチャのクリア
			m_listLQ.clear();
			m_bIsSetFeature = FALSE;
			return FALSE;
		}
		m_bIsSetFeature = FALSE;

		// 既存のものを全てクリア
		if( IsWindow() == TRUE )
			m_cTreeCtrl.DeleteAllItems();

		// 現在表示しているダイアログがあれば、それの CCrontrolRel と CFeatureDef の関係をクリア
		if( m_cExportDlg != NULL )
			m_cExportDlg->ClearFeatureDefs();

		m_listLQRowDef.clear();
	}
	
	if( listRowDef.size() < 1 )
		return FALSE;

	// フィーチャ定義クラスのリストを作成
	CLoadQueueInfo cLQInfo;
	for( std::list<LQROWDEF>::iterator it = listRowDef.begin(); it != listRowDef.end(); it++ )
	{
		if( !it->obj ) continue;

		CLQRowDef featDef;
		featDef.SetArcHelper( m_cArcHelper.GetApp() );
		featDef.SetLQRow( it->obj, it->shp, (sindyeRoadQueueType)it->flg, it->name, it->deflt );
		// 編集可不可を記憶しておく
		// INFの所属するワークスペースが編集開始されていなければだめなようにする
		IWorkspacePtr ipWorkspace;
		ITablePtr ipInfTable;
		it->obj->get_Table( &ipInfTable );
		IDatasetPtr ipDataset( ipInfTable );
		if( ipDataset )
			ipDataset->get_Workspace( &ipWorkspace );
		if( !ipWorkspace )
			_ASSERTE( ipWorkspace != NULL );
		else {
			VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
			IWorkspaceEditPtr ipWorkspaceEdit( ipWorkspace );
			ipWorkspaceEdit->IsBeingEdited( &vbIsBeingEdited );
			if( vbIsBeingEdited )
			{
				if( !m_ipRule )
					m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );

				m_ipRule->IsEditableFeature( it->obj, featDef.GetShape(), sindyeEditAttribute, &(featDef.m_sindyeEditableType), &(featDef.m_bstrIsEditableFeature) );

				if ( featDef.m_sindyeEditableType == sindyeEditableEditable )
					featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANEDITFEATURE );
			}
			else {
				featDef.m_sindyeEditableType = sindyeEditableNoEditable;
				featDef.m_bstrIsEditableFeature = AheLoadString( IDS_DONTSTARTEDITING );
			}
		}
		IDatasetEditInfoPtr ipDatasetEditInfo;
		ipDatasetEditInfo = ipDataset;
		VARIANT_BOOL vaCanEdit = VARIANT_FALSE;
		if( ipDatasetEditInfo )
			ipDatasetEditInfo->get_CanEdit( &vaCanEdit );
		if( !vaCanEdit )
		{
			featDef.m_sindyeEditableType = sindyeEditableNoEditable;
			featDef.m_bstrIsEditableFeature = AheLoadString( IDS_CANTEDITFEATURE );
		}

		m_listLQRowDef.push_back( featDef );

		// LQ を外部レコードとして無理やり登録
		LONG lOID = -1;
		ITablePtr ipTable;
		CRelationalTableDef cRelTableDef;

		it->obj->get_OID( &lOID );
		for( std::list<ROWDEF>::iterator itLQs = it->lqlist.begin(); itLQs != it->lqlist.end(); itLQs++ )
		{
			CRowDef rowDef;
			IFieldsPtr ipFields;
			IFieldPtr ipField;
			LONG lFieldCount = -1;
			CString strTableName = AheGetFeatureClassName( itLQs->obj );

			if( !ipTable )
				itLQs->obj->get_Table( &ipTable );
			rowDef.SetRow( itLQs->obj );
			rowDef.SetTable( ipTable );

			itLQs->obj->get_Fields( &ipFields );
			ipFields->get_FieldCount( &lFieldCount );
			for( LONG i = 0; i < lFieldCount; i++ ) 
			{
				CComBSTR bstrFieldName = 0;
				ipFields->get_Field( i, &ipField );
				ipField->get_Name( &bstrFieldName );

				CFieldDef fieldDef;

				fieldDef.SetFieldDef( strTableName, OLE2T( bstrFieldName ) );
				fieldDef.SetRowBuffer( i, ipField, itLQs->obj );
				rowDef.push_back( fieldDef );
			}
			cRelTableDef.push_back( rowDef );
		}
		cRelTableDef.SetTable( ipTable, AheGetFeatureClassName( ipTable ), lOID, INF_ID_FIELD, FALSE, TRUE );
		m_listLQRowDef.rbegin()->GetRelationalTableDefList()->push_back( cRelTableDef );
	}

	listRowDef.clear();	// 消しておく

	// モーダル（新規作成）の時
	if( m_bIsModal )
	{
		// VICSの新規作成の時に両方向を作成した場合に、最初に作成した方向のVICSを表示するために、以下の処理を行う
		BOOL	bReverse = FALSE;
		std::list<CLQRowDef> reverse;
		reverse.clear();

		LONG lFeatCount = 0;
		for( std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin(); it != m_listLQRowDef.end(); it++, lFeatCount++ )
		{
			_IRowPtr	ipRow = it->GetFeature();

			CString 	strLayerName;
			strLayerName = AheGetLayerName( m_cArcHelper.GetMap(), ipRow, _FIND_TABLE );

			// VICSかつ２つあった場合
			if( (strLayerName.Find( _T("VICS")) != -1 && lFeatCount == 1) || (strLayerName.Find( _T("INTERNAVI")) != -1 && lFeatCount == 1)  )
				bReverse = TRUE;
		}
		// 逆に入れる
		if( bReverse )
		{
			// リストを逆順にいれる
			for( std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin(); it != m_listLQRowDef.end(); it++ )
				reverse.push_front(*it);

			// これで、若い番号が先に来たことになる
			m_listLQRowDef = reverse;
		}
	}

	// モーダルじゃない場合のみ
	if( !m_bIsModal )
	{
		// 方面案内があった場合には、最初にエラーチェックをかけて、不整合がないかどうかしらべる
		for( std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin(); it != m_listLQRowDef.end(); it++ )
		{
			_IRowPtr	ipRow = it->GetFeature();

			CString 	strLayerName;
			strLayerName = AheGetLayerName( m_cArcHelper.GetMap(), ipRow, _FIND_TABLE );

			if( strLayerName.Find(_T("INF_DIRGUIDE") ) != -1 )
			{

				CExportDlg* cExportDlg;

				std::map<CString, CExportDlg*>::iterator itDir = m_mapDlg.find(  _T("INF_DIRGUIDE")  );
				if( itDir == m_mapDlg.end() )
				{
					cExportDlg = SetChildDlg( _T("INF_DIRGUIDE") );
					cExportDlg->Create( m_hWnd );
					m_mapDlg[_T("INF_DIRGUIDE")] = cExportDlg;

				}
				else
					cExportDlg = itDir->second;

				cExportDlg->SendMessage( NULL, true, 0 );
				cExportDlg->SetFeatureDefList( &m_listLQRowDef );
				cExportDlg->ErrorCheck();
				cExportDlg->SendMessage( NULL, false, 0 );
				break;
			}
		}

		// ツリーにフィーチャを登録（表示される前に登録する必要アリ）
		m_cTreeCtrl.SetLQs( &m_listLQRowDef );
		// ダイアログにフィーチャを登録（m_listFeatureDef がセットされた後）
		SetSelectedFeaturesToDlg();
	}

	return TRUE;
}

// ツリーで選んでいるフィーチャの子ダイアログを作成し、ダイアログに実際に表示する関数
BOOL CAttributeDlgObj::SetSelectedFeaturesToDlg()
{
	BOOL bEditable = TRUE;

	IUnknownPtr ipUnk;
	IRowBufferPtr ipRowBuffer;

	LONG lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;

	// ツリーで何を選択しているかを取得
	m_cTreeCtrl.GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );
	m_cTreeCtrl.m_bIsLineHilight = m_bIsLineHilight;
	m_cTreeCtrl.m_bIsVertexHilight = m_bIsVertexHilight;

	// 何もセットされていなかった
	if( lFeatureIndex < 0 )
		return FALSE;

	// 選択中のフィーチャ取得
	IFeaturePtr ipFeature;
	_IRowPtr ipRow;

	CComBSTR bstrIsEditableFeature;
	sindyeEditableType sindyEditable = sindyeEditableNoDefine;

	// 選択されているフィーチャのiteratorを取得し、情報を抜き出す
	// LQ系との場合分けをする
	if( ! m_bIsLQAttribute )
	{
		// 選択しているフィーチャのiteratorを取得
		std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
		if( m_listFeatureDef.size() > (ULONG)lFeatureIndex )
		{
			advance( it, lFeatureIndex );
			// 新規作成の場合に作成されたフィーチャをルールにかかわらず強制的にルールで制御されている各種コントロールを編集可能状態にする
			if( m_sAttributeEditType == sindyeNotApplyRuleEnable )
			{
				// [Bug 5770]新規作成の場合には、警告はなかったものとして扱わないと子ダイアログ側の最初のフォーカスセットで、
				// フォーカスがあたらなくなってしまうので、ここで警告はずしておく
				it->SetWarnDelete();
				it->m_sindyeEditableType = sindyeEditableEditable;
			}
			bstrIsEditableFeature = it->m_bstrIsEditableFeature;
			sindyEditable = it->m_sindyeEditableType;
			// 編集警告の場合はフラグをみて解除されているかどうか判断
			if( sindyeEditableWarnning == sindyEditable && it->GetWarnDelete() )
			{
				sindyEditable = sindyeEditableEditable;
				bstrIsEditableFeature = _T("");
			}
		}

		if( lTableIndex < 0 && lRowIndex < 0 )
			ipFeature = it->GetFeature();
		else {
			// フィーチャに付随しているテーブル関連（一方通行や通行禁止のテーブル）の属性を取得するため
			// 選択しているフィーチャの関連テーブルを取得
			std::list<CRelationalTableDef>* pRelTableList = it->GetRelationalTableDefList();
			if( pRelTableList )
			{
				std::list<CRelationalTableDef>::iterator itRelTableList = pRelTableList->begin();
				if( pRelTableList->size() > (ULONG)lTableIndex )
				{
					advance( itRelTableList, lTableIndex );
					std::list<CRowDef>::iterator itRowDef = itRelTableList->begin();
					if( itRelTableList->size() > (ULONG)lRowIndex )
					{
						advance( itRowDef, lRowIndex );
						ipFeature = itRowDef->GetRow();
						ipRow = itRowDef->GetRow();
					}
				}
			}
		}
	}
	else {
		// 選択しているフィーチャのiteratorを取得
		std::list<CLQRowDef>::iterator it = m_listLQRowDef.begin();
		if( m_listLQRowDef.size() > (ULONG)lFeatureIndex )
		{
			advance( it, lFeatureIndex );

			// 新規作成の場合に作成されたフィーチャをルールにかかわらず強制的にルールで制御されている各種コントロールを編集可能状態にする
			if( m_sAttributeEditType == sindyeNotApplyRuleEnable )
			{
				// [Bug 5770]新規作成の場合には、警告はなかったものとして扱わないと子ダイアログ側の最初のフォーカスセットで、
				// フォーカスがあたらなくなってしまうので、ここで警告はずしておく
				it->SetWarnDelete();
				it->m_sindyeEditableType = sindyeEditableEditable;
			}
			bstrIsEditableFeature = it->m_bstrIsEditableFeature;
			sindyEditable = it->m_sindyeEditableType;
			// [Bug 5727]リンク列がワーニングの時に編集ボタンを押しても編集できないバグ
			// 編集警告の場合はフラグをみて解除されているかどうか判断
			if( sindyeEditableWarnning == sindyEditable && it->GetWarnDelete() )
			{
				sindyEditable = sindyeEditableEditable;
				bstrIsEditableFeature = _T("");
			}

			if( lTableIndex < 0 && lRowIndex < 0 )
			{
				ipRow = it->GetFeature();
				IGeometryPtr ipGeom = it->GetShape();
				if( ipGeom != NULL )
				{
					// 少しバッファリングする
					DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 10 );
					IPolygonPtr ipPolygon;
					AheGeometry2Polygon( ipGeom, dBufferSize, &ipPolygon );
					// [Bug 4883]不正な形状を選択してしまった場合に落ちる問題があるので、NULLチェックしておく
					if( ipPolygon )
						ipPolygon->get_Envelope( &m_ipSelectFeatEnv );
				}
			}
			else {
				// フィーチャに付随しているテーブル関連（LQテーブル）の属性を取得するため
				// 選択しているフィーチャの関連テーブルを取得
				std::list<CRelationalTableDef>* pRelTableList = it->GetRelationalTableDefList();
				if( pRelTableList )
				{
					std::list<CRelationalTableDef>::iterator itRelTableList = pRelTableList->begin();
					if( pRelTableList->size() > (ULONG)lTableIndex )
					{
						advance( itRelTableList, lTableIndex );
						std::list<CRowDef>::iterator itRowDef = itRelTableList->begin();
						if( itRelTableList->size() > (ULONG)lRowIndex )
						{
							advance( itRowDef, lRowIndex );
							ipFeature = itRowDef->GetRow();
							ipRow = itRowDef->GetRow();
						}
					}
				}
			}
		}
	}
	if( ! ( ipFeature || ipRow ) )
	{
		MessageBox( AheLoadString( IDS_CANTSPECIFY ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING );
		return FALSE;
	}

	// フィーチャクラス名またはオブジェクトクラス名の取得
	// ここで取得した名前から、レイヤ名.dllを読み込むことになります。
	CString strTableName;
	if( ! m_bIsLQAttribute )
		strTableName = AheGetFeatureClassName( ipFeature );
	else
	{
		ITablePtr ipTable;
		ipRow->get_Table( &ipTable );
		CComBSTR bstrTableName;
		((IObjectClassPtr)ipTable)->get_AliasName( &bstrTableName );
		CString strTemp( bstrTableName );
		strTableName = strTemp.Right( strTemp.GetLength() - strTemp.Find( _T(".") ) - 1 ).MakeUpper();
	}

	
	// 内容確認ボタンはUPDATETYPE_CFIELDがある場合にのみ押せるようにしておくため、表示するフィーチャにUPDATETYPE_CFIELDがあるか
	// どうか確認して、ENABLEにするかDISABLEにするか決める
	if( ipFeature )
	{
		IFieldsPtr ipFields;
		ipFeature->get_Fields( &ipFields );
		long lFieldIndex = -1;
		ipFields->FindField( CComBSTR( UPDATETYPE_CFIELD ), &lFieldIndex );
		::EnableWindow( GetDlgItem(IDC_BUTTON_CONFIRMATION), ( lFieldIndex == -1 ) ? FALSE : TRUE );
	}
	else if( ipRow )
	{
		IFieldsPtr ipFields;
		ipRow->get_Fields( &ipFields );
		long lFieldIndex = -1;
		ipFields->FindField( CComBSTR( UPDATETYPE_CFIELD ), &lFieldIndex );
		::EnableWindow( GetDlgItem(IDC_BUTTON_CONFIRMATION), ( lFieldIndex == -1 ) ? FALSE : TRUE );
	}
#ifdef _FOR_ENGLISH		// [bug 11183]通常の場合、名称表示は要らない
	// 名称を表示する
	DisplayObjectName( ipFeature );
#endif

	// フィーチャの種類によって表示形態を変える
	CExportDlg* cExportDlg = NULL;

	// コモンコントロール表示の場合
	if( ! m_bIsGridMode )
	{
		// レイヤ別の子ダイアログを取得
		cExportDlg = SetChildDlg(strTableName);
		if( m_bIsLQAttribute && !cExportDlg )  // エイリアスで存在しない [bug 7516]
		{
			strTableName = AheGetFeatureClassName( ipRow );
			cExportDlg = SetChildDlg( strTableName );
		}
	}
	
	// GRID表示の場合
	// どれでもなかったらデフォルトのグリッドを表示
	bool bIsDefaultDlg = FALSE;
	if( ! cExportDlg )
	{
		if( ! m_bIsLQAttribute )
		{
			std::map<CString, CExportDlg*>::iterator it = m_mapDlg.find( _T("DEFAULT") );

			if( it == m_mapDlg.end() )
			{
				if( m_strInstallPath.IsEmpty() )
				{
					// SiNDY-eインストールルートパス取得
					CInifileHelper	cIniFile;
					cIniFile.GetInstallRoot( m_strInstallPath );
				}

				CString strModuleName;
				strModuleName += m_strInstallPath + _T("ChildDlgDLL\\DEFAULT.dll");

#ifdef _USE_CHILDDLG_SHORTCUT
				HMODULE hModule = LoadShellLinkLibrary( strModuleName );
#else
				HMODULE hModule = ::LoadLibrary( strModuleName );
#endif // #ifdef _USE_CHILDDLG_SHORTCUT

				if( hModule )
				{
					CreateDlg *pCreateTest = (CreateDlg *)GetProcAddress(hModule,"CreateDlg");

					cExportDlg = pCreateTest();
					cExportDlg->SetArcHelper( m_cArcHelper.GetApp() );
					m_mapModule[_T("DEFAULT")] = hModule;
				}
			}
			else
				cExportDlg = it->second;
		}
		else {
			std::map<CString, CExportDlg*>::iterator it = m_mapDlg.find( _T("LQDEFAULT") );
			if( it == m_mapDlg.end() )
			{
				if( m_strInstallPath.IsEmpty() )
				{
					// SiNDY-eインストールルートパス取得
					CInifileHelper	cIniFile;
					cIniFile.GetInstallRoot( m_strInstallPath );
				}

				CString strModuleName;
				strModuleName += m_strInstallPath + _T("ChildDlgDLL\\LQDEFAULT.dll");

#ifdef _USE_CHILDDLG_SHORTCUT
				HMODULE hModule = LoadShellLinkLibrary( strModuleName );
#else
				HMODULE hModule = ::LoadLibrary( strModuleName );
#endif // #ifdef _USE_CHILDDLG_SHORTCUT

				if( hModule )
				{
					CreateDlg *pCreateTest = (CreateDlg *)GetProcAddress(hModule,"CreateDlg");

					cExportDlg = pCreateTest();
					cExportDlg->SetArcHelper( m_cArcHelper.GetApp() );

					m_mapModule[_T("LQDEFAULT")] = hModule;
				}
			}
			else
				cExportDlg = it->second;
		}

		bIsDefaultDlg = TRUE;
	}

	// ここまできて、ダイアログなかったら、逝ってよし、ありえないけど・・・
	if( ! cExportDlg )
	{
		ATLASSERT( cExportDlg != NULL );
		return FALSE;
	}

	// すでにダイアログが表示されていたら、新しいものを表示しなくてはいけないので、表示されているダイアログ作成
	if( m_hCurrentWnd )
		::ShowWindow( m_hCurrentWnd, SW_HIDE );

	// 選択された子ダイアログ作成
	if( ! bIsDefaultDlg )
	{
		std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( strTableName );
		if( itDlg == m_mapDlg.end() )
		{
			m_hCurrentWnd =	cExportDlg->Create( m_hWnd, (LPARAM)(LPCTSTR)strTableName );
			m_mapDlg[strTableName] = cExportDlg;
		}
		else
			m_hCurrentWnd = cExportDlg->GetDlg();
	}
	else {
		if( ! m_bIsLQAttribute )
		{
			std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( _T("DEFAULT") );
			if( itDlg == m_mapDlg.end() )
			{
				m_hCurrentWnd =	cExportDlg->Create( m_hWnd );
				m_mapDlg[_T("DEFAULT")] = cExportDlg;
			}
			else
				m_hCurrentWnd = cExportDlg->GetDlg();
		}
		else
		{
			std::map<CString, CExportDlg*>::iterator itDlg = m_mapDlg.find( _T("LQDEFAULT") );
			if( itDlg == m_mapDlg.end() )
			{
				m_hCurrentWnd =	cExportDlg->Create( m_hWnd );
				m_mapDlg[_T("LQDEFAULT")] = cExportDlg;
			}
			else
				m_hCurrentWnd = cExportDlg->GetDlg();
		}
	}

	// エイリアス表示か、DB表示かを決定
	cExportDlg->SetAliasOrField( m_bIsDBNameMode );

	// 子ダイアログ表示
	::ShowWindow( m_hCurrentWnd, SW_SHOW );

	// 子ダイアログにフィーチャのリストを渡す
	if( m_bIsLQAttribute )
		cExportDlg->SetFeatureDefList( &m_listLQRowDef );
	else
		cExportDlg->SetFeatureDefList( &m_listFeatureDef );

	if( strTableName.Find(_T("INF_DIRGUIDE")) != -1 || strTableName.Find(_T("INF_LANE")) != -1)
		cExportDlg->SendMessage( WM_DISABLECONTROL, m_lMode, 0 );

	// 編集可能レイヤかそうでないかのチェック
	// スタティックコントロールにフィーチャの編集可不可を表示する
	CComBSTR	 bstrMsg( bstrIsEditableFeature );
	switch( m_sAttributeEditType )
	{
		case sindyeNotApplyRuleEnable:
			::EnableWindow( GetDlgItem(IDC_BUTTON_DELETEWARN), FALSE );
			bEditable = TRUE;
			break;
		case sindyeNotApplyRuleDisable:
			::EnableWindow( GetDlgItem(IDC_BUTTON_DELETEWARN), FALSE );
			bEditable = FALSE;
			break;
		default:
			::EnableWindow( GetDlgItem(IDC_BUTTON_DELETEWARN), ( sindyeEditableEditable == sindyEditable ) ? FALSE : TRUE );
			bEditable = ( sindyeEditableEditable == sindyEditable ) ? TRUE : FALSE;
			break;
	}
	// 属性編集ツールで注記を選択した場合には編集不可とする
	if( AheIsChukiLayer( strTableName ) && !m_bIsModal )
		bEditable = FALSE;

	// 編集状態表示部にフィーチャの状態を表示（子ダイアログ側で上書きすることがあるので先に）
	::SetWindowText( GetDlgItem(IDC_STATIC_MESSAGE), OLE2T(bstrMsg) );

	// 子ダイアログの各種コントロールをセッティング
	cExportDlg->SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, TRUE, bEditable );

	// メンバ変数に登録しておく
	m_lFeatureDefIndex = lFeatureIndex;
	m_cExportDlg = cExportDlg;

	SetButton( Changed() );

#ifdef _FOR_ENGLISH		// [bug 11183]通常の場合、名称表示が要らないので、相応のチップも表示しない
	InitToolTip(IDC_STATIC_OFFICIALNAME);
	InitToolTip(IDC_STATIC_ALTERNATIVENAME);
#endif
	InitToolTip(IDC_STATIC_MESSAGE);

	// ダイアログサイズの設定
	ResizeClientDlg();

	// 現在選択しているフィーチャのEnvelopeをとっておいて、ダイアログを消すときに、選択しているフィーチャの周りだけ再描画する
	if( ipFeature )
	{
		// 少しバッファリングする
		sindyeFeatureKinds kinds = AheGetFeatureKinds(ipFeature);
		IPolygonPtr ipPolygon;
		// ポリゴンの場合はバッファリングしない
		if( kinds == sindyePolygonFeature )
		{
			IGeometryPtr ipRefreshGeom;
			ipFeature->get_Shape( &ipRefreshGeom );
			ipPolygon = ipRefreshGeom;
		}
		else
		{
			DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 10 );
			AheFeature2Polygon( ipFeature, dBufferSize, &ipPolygon );
		}
		// [Bug 4883]不正な形状を選択してしまった場合に落ちる問題があるので、NULLチェックしておく
		if( ipPolygon )
			ipPolygon->get_Envelope( &m_ipSelectFeatEnv );
	}

	// こんなところで、フラグ初期化しておく・・・
	m_bIsFirst = TRUE;

	return TRUE;
}

// フィーチャの保存を管理する関数
BOOL CAttributeDlgObj::StoreFeature()
{
	// 変更があった場合保存
	// Sketchツールからよばれている場合には新規作成で属性がなにも編集されていない場合もあるので、それでも入らなくてはいけない
	// sindyeNotApplyRuleEnableはSketchからしかよばれないので
	if( Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable )
	{
		// 変更フラグを立てる
		m_bIsChanged = TRUE;

		if( ErrorCheck() )
		{
			// フィーチャの切り替えがあったときにはダイアログを出して、保存するかをたずねる
			// 変更があったかどうかを入れておかないと、新規作成からよばれたときに、最初に下のメッセージが開いてしまう
			if(	m_bIsSetFeature && Changed() )
			{
				int iRet = AheMessageBox( NULL, AheLoadString( IDS_CHECKSAVE ), AheLoadString( IDS_CAPTION ), MB_YESNO|MB_ICONEXCLAMATION|MB_TASKMODAL );
				if( iRet == IDNO )
					return TRUE;
			}

			esriEditState State;
			m_cArcHelper.GetEditor()->get_EditState( &State );
			// 属性ダイアログで編集開始を制御するかどうか
			if( m_bDoOperation && State == esriStateEditing)
			{
				// 編集開始
				ATLTRACE(_T("更新対象 %d\n"), m_lstUpdate.size());
				m_lstUpdate.clear();
				m_cArcHelper.StartEditOperation();
			}

			IEnvelopePtr ipEnv;						// 再描画領域保持用
			// 選択中のフィーチャでループをまわし、編集のかかっているフィーチャをみつけ、そのフィーチャのEnvelopをとる
			// そして、編集がかかっていれば、保存する
			ISiNDYModifyCheckPtr	ipModifyCheck;
			if( !ipModifyCheck )
				ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );

			bool bIsAnnotation = false;
			std::list<CFeatureDef> listChangedLinkandNode;
			for( std::list<CFeatureDef>::iterator itDef = m_listFeatureDef.begin(); itDef != m_listFeatureDef.end(); ++itDef ) 
			{
				IFeaturePtr ipFeature(itDef->GetFeature());
				CString strLayerName = AheGetFeatureClassName( ipFeature );
				if( itDef->Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable )
				{
					// フィールドのチェック
					if( !CheckValue( &(*itDef) ) )
					{
						// ここでエラーになった場合にはアボートしないといけない
						if( m_bDoOperation && State == esriStateEditing )
							m_cArcHelper.AbortEditOperation();	// 編集終了
						return false;
					}
					// 同時に編集されてしまった場合があるので、最終的にここでModifyチェックをする
					if( ipModifyCheck )
					{
						VARIANT_BOOL vbModified = VARIANT_FALSE;
						CComBSTR bstrMsg;
						if( SUCCEEDED( ipModifyCheck->IsModified( ipFeature, &vbModified, &bstrMsg ) ) )
						{
							if( vbModified == VARIANT_TRUE )
							{
								AheMessageBox( NULL, CString(bstrMsg), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
								AheMessageBox( NULL, AheLoadString( IDS_CLOSEANDNOTSAVE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );

								if( m_bDoOperation && State == esriStateEditing )
									m_cArcHelper.AbortEditOperation();	// 編集終了
								return TRUE;
							}
						}
					}

					// 保存
					itDef->Store( m_bDoOperation );

					// 注記レイヤは注記ツール側で独自に再描画かけるので、再描画範囲取得やネットワークチェックやらない
					if( AheIsChukiLayer( strLayerName ) )
					{
						// ここに入ってくるのは、注記ツールからしかありえない
						bIsAnnotation = true;
						continue;
					}
					//	[Bug 5370]変更されていない道路リンクのノード種別を変更してはいけない対策
					// 保存するとChangedフラグが元にFALSEになってしまうので、ここであらたに確保しておく
					// 変更がかかったリンクに関連するノードと、変更がかかったノードのみでよいはず
					if( AheIsNetwork( ipFeature ) )
						listChangedLinkandNode.push_back( *itDef );

					
					// 再描画範囲を取得
					if( ipFeature )
					{
						sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
						IPolygonPtr ipPolygon;
						// ポリゴンの場合はバッファリングしない
						if( kinds == sindyePolygonFeature )
						{
							IGeometryPtr ipRefreshGeom;
							ipFeature->get_Shape( &ipRefreshGeom );
							ipPolygon = ipRefreshGeom;
						}
						else
						{
							// 少しバッファリングする
							DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 15 );
							AheFeature2Polygon( ipFeature, dBufferSize, &ipPolygon );
						}
						// [Bug 4883]不正な形状を選択してしまった場合に落ちる問題があるので、NULLチェックしておく
						if( ipPolygon )
						{
							IEnvelopePtr ipFeatEnv;
							ipPolygon->get_Envelope( &ipFeatEnv );
							if( !ipEnv )
								ipEnv = ipFeatEnv;
							else
								ipEnv->Union( ipFeatEnv );
						}
					}
				}
			}

			// [Bug 5370]変更されていない道路リンクのノード種別を変更してはいけない対策
			// 変更のかかったリンクに関するノードと変更があったノードのノード種別の再設定
			for( std::list<CFeatureDef>::iterator it = listChangedLinkandNode.begin(); it != listChangedLinkandNode.end(); it++ ) 
			{
				// 保存するものが道路リンクの場合、ノード種別自動切換えするためにチェックする。
				// [Bug 5370]変更されていない道路リンクのノード種別を変更してはいけないため、変更がかかっている道路リンクか調べる
				if( AheIsNetworkLink( (IFeaturePtr)it->GetFeature() ) )
				{
					// [Bug 5389]歩行者NW対応：ネットワークリンクについているノードを取得
					CTargetItems	cNodeItems;
					AheGetConnectedNetworkNode( it->GetFeature(), cNodeItems );
					for(CTargetItems::iterator itNode = cNodeItems.begin(); itNode != cNodeItems.end(); ++itNode )
					{
						// 同時に編集されてしまった場合があるので、最終的にここでModifyチェックをする
						if( ipModifyCheck )
						{
							VARIANT_BOOL vbModified = VARIANT_FALSE;
							CComBSTR bstrMsg;
							if( SUCCEEDED( ipModifyCheck->IsModified( itNode->GetFeature(), &vbModified, &bstrMsg ) ) )
							{
								if( vbModified == VARIANT_TRUE )
								{
									AheMessageBox( NULL, CString(bstrMsg), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
									AheMessageBox( NULL, AheLoadString( IDS_CONFLICTNODE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );

									if( m_bDoOperation == TRUE && State == esriStateEditing )
										m_cArcHelper.AbortEditOperation();	// 編集終了
									return TRUE;
								}
							}
						}
						StoreRoadNord( itNode->GetFeature() );
					}
				}
				else if( AheIsNetworkNode( (IFeaturePtr)it->GetFeature() )  )
					StoreRoadNord( it->GetFeature() );
			}

			// 属性ダイアログ側での属性更新
			if( ! Update() )
			{
				if( m_bDoOperation && State == esriStateEditing )
					m_cArcHelper.AbortEditOperation();
				AheMessageBox( NULL, AheLoadString( IDS_UPDATE_FAILED ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
				return FALSE;
			}

			// 編集終了
			if( m_bDoOperation && State == esriStateEditing )
			{
				m_cArcHelper.StopEditOperation( AheLoadString( IDS_NAME ) );	// 編集終了
				FinalModifyCheck();
			}

			// 注記の再描画は注記側で独自にやっているので、属性ダイアログでは行わない
			if( !bIsAnnotation )
				m_cArcHelper.GetActiveView()->PartialRefresh( (esriViewDrawPhase)( esriViewGeography|esriViewGraphics ), NULL, ipEnv );
		}
		else
		{
			m_bIsEnding = FALSE;
			return FALSE;
		}
	}
	return TRUE;
}

// ノード種別の再設定を行う関数
bool CAttributeDlgObj::StoreRoadNord( IFeaturePtr ipNodeFeature )
{
	// ノード種別を取得
	IFieldsPtr		ipNodeFields;
	ipNodeFeature->get_Fields(&ipNodeFields);
	long			lNodeClass = 0;
	ipNodeFields->FindField(CComBSTR(_T("NODECLASS_C")), &lNodeClass );
	CComVariant		vNodeClass;
	ipNodeFeature->get_Value(lNodeClass, &vNodeClass);
	sindyeNodeClass	sNodeClass = sindyeNodeClass(vNodeClass.lVal);

	// [Bug 5389]歩行者NW対応：ネットワークノードについているすべてのリンクを取得
	CTargetItems	cRoadItems;
	AheGetConnectedNetworkLink( ipNodeFeature, cRoadItems );

	for ( CTargetItems::iterator itRoad = cRoadItems.begin(); itRoad != cRoadItems.end(); ++itRoad ) {
		ATLTRACE(_T("%d\n"), (*itRoad).GetOID());
	}

	// ノード種別を自動設定
	BOOL	bIsChanged = FALSE;
	if( AheResetNodeAttribute( ipNodeFeature, cRoadItems, &bIsChanged, TRUE ) == TRUE )
	{
		// ノード種別変更されていたら
		if( bIsChanged ) {
			LONG lOID = 0;
			ipNodeFeature->get_OID(&lOID);
			ATLTRACE(_T("%d\n"), lOID);
			ipNodeFeature->Store();
		}
	}
	return ( bIsChanged ) ? true : false;
}

// リンク列の保存を管理する関数
BOOL CAttributeDlgObj::LQStoreFeature()
{
	BOOL bStore = TRUE;
	if( Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable)
	{
		if( ErrorCheck() )
		{
			// フィーチャの切り替えがあったときにはダイアログを出して、保存するかをたずねる
			// 変更があったかどうかを入れておかないと、新規作成からよばれたときに、最初に下のメッセージが開いてしまう
			if(	m_bIsSetFeature && Changed())
			{
				int iRet = AheMessageBox( NULL, AheLoadString( IDS_CHECKSAVE ), AheLoadString( IDS_CAPTION ), MB_YESNO|MB_ICONEXCLAMATION|MB_TASKMODAL );
				if( iRet == IDNO )
					return TRUE;
			}

			esriEditState State;
			m_cArcHelper.GetEditor()->get_EditState( &State );
			if( m_bDoOperation && State == esriStateEditing )
			{
				// 編集開始
				ATLTRACE(_T("更新対象 %d\n"), m_lstUpdate.size());
				m_lstUpdate.clear();
				m_cArcHelper.StartEditOperation();
			}

			// 保存
			IEnvelopePtr ipEnv;						// 再描画領域保持用
			ISiNDYModifyCheckPtr	ipModifyCheck;
			for( std::list<CLQRowDef>::iterator i = m_listLQRowDef.begin(); i != m_listLQRowDef.end(); i++ ) 
			{
				if( i->Changed() || m_sAttributeEditType == sindyeNotApplyRuleEnable )
				{
					CString strLayerName = i->GetTableName();
					// フィールドのチェック
					if( !LQCheckValue( &(*i) ) )
					{
						// ここでエラーになった場合にはアボートしないといけない
						if( m_bDoOperation == TRUE && State == esriStateEditing )
							m_cArcHelper.AbortEditOperation();	// 編集終了
						return false;
					}

					_IRowPtr ipModifyCheckRow( i->GetFeature() );
					// 同時に編集されてしまった場合があるので、最終的にここでModifyチェックをする
					if( !ipModifyCheck )
						ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );
					if( ipModifyCheck )
					{
						VARIANT_BOOL vbModified = VARIANT_FALSE;
						CComBSTR bstrMsg;
						if( SUCCEEDED( ipModifyCheck->IsModified( ipModifyCheckRow, &vbModified, &bstrMsg ) ) )
						{
							if( vbModified )
							{
								AheMessageBox( NULL, CString(bstrMsg), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );
								AheMessageBox( NULL,  AheLoadString( IDS_CLOSEANDNOTSAVE ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL, NULL );

								if( m_bDoOperation && State == esriStateEditing )
									m_cArcHelper.AbortEditOperation();	// 編集終了
								return TRUE;
							}
						}
					}

					// 保存
					i->Store( m_bDoOperation );

					// 再描画範囲取得
					_IRowPtr ipRow(i->GetFeature());
					if( ipRow )
					{
						IGeometryPtr ipGeom = i->GetShape();
						// 少しバッファリングする
						DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), 10 );
						IPolygonPtr ipPolygon;
						AheGeometry2Polygon( ipGeom, dBufferSize, &ipPolygon );
						// [Bug 4883]不正な形状を選択してしまった場合に落ちる問題があるので、NULLチェックをしておく
						if( ipPolygon )
						{
							IEnvelopePtr ipFeatEnv;
							ipPolygon->get_Envelope( &ipFeatEnv );
							if( !ipEnv )
								ipEnv = ipFeatEnv;
							else
								ipEnv->Union( ipFeatEnv );
						}
					}
				}
			}
			// 編集終了
			if( m_bDoOperation && State == esriStateEditing )
			{
				m_cArcHelper.StopEditOperation( AheLoadString( IDS_NAME ) );	// 編集終了
				FinalModifyCheck();
			}

			// 変更があったレイヤのみ再描画
			if( ipEnv )
				m_cArcHelper.GetActiveView()->PartialRefresh( (esriViewDrawPhase)(esriViewGeography|esriViewGraphics), NULL, ipEnv );
		}
		else
		{
			m_bIsEnding = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}

// ダイアログを閉じる時の処理
BOOL CAttributeDlgObj::CloseDlg( BOOL bStore )
{
	m_bIsEnding = TRUE;	// 終了処理開始

	// ダイアログの位置を記憶
	GetWindowRect( &m_cDlgRect );

	// 保存するかどうか
	if( bStore )	// OKボタンを押したとき
	{	// 条件があえば保存する（特に変更などない場合は保存しない）
		if( !m_bIsLQAttribute )
		{
			if( !StoreFeature() )
				return FALSE;
		}
		else
		{	// 条件があえば保存する（特に変更などない場合は保存しない）
			if( !LQStoreFeature() )
				return FALSE;
		}
	}
	else					// キャンセルボタンを押したとき
	{
		// 破棄する際のダイアログを表示する（特に変更などない場合は出さない）
		if( Changed() )
			m_bIsChanged = TRUE;			// 変更フラグを立てる
		m_bIsCancel = TRUE;
	}

	ConnectToEvents( FALSE );

	// ツリー内部コレクションを消去
	m_cTreeCtrl.Clear();
	m_cTreeCtrl.DeleteAllItems();

	// ダイアログリストを全て破棄する
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg* cExportDlg = it->second;
		CString strLayerName = it->first;

		// タブ位置を覚えておく
		if( strLayerName == _T("ROAD_LINK") )
			m_lRoadTabNumber = cExportDlg->GetTabNum();
		else if( strLayerName.Find(_T("DRMA_LINK")) != -1 )
			m_lDRMATabNumber = cExportDlg->GetTabNum();
		else if( strLayerName == _T("ACCESS_POINT") )
			m_lAccessTabNumber = cExportDlg->GetTabNum();
		else if( strLayerName.Find(_T("VICS")) != -1 )
			cExportDlg->SendMessage( WM_REFRESHFEATURE, 1, 0);
		else if( strLayerName.Find( _T("HIGHWAY_NODE") ) != -1 )
		{
			m_lHwyNodeTabNumber = cExportDlg->GetTabNum();	// [Bug6570] 料金所レーン情報用にタブを追加したので修正
			cExportDlg->SendMessage( WM_REFRESHFEATURE, 1, 0);
		}
		cExportDlg->ClearFeatureDefs();
		cExportDlg->Delete();
	}

	for( std::map<CString, HMODULE>::iterator itModule = m_mapModule.begin(); itModule != m_mapModule.end(); itModule++ )
	{
		HMODULE hModule = itModule->second;
		FreeLibrary( hModule );
	}
	m_mapDlg.clear();
	m_mapModule.clear();
	m_hCurrentWnd = NULL;
	m_cExportDlg = NULL;
	// 登録用フィーチャのクリア
	m_cFeatureItems.clear();
	m_listLQ.clear();
	// フィーチャリストクリア
	m_listFeatureDef.clear();
	m_listLQRowDef.clear();

	// 必ず初期化して終了しておく
	m_lMode = 0;

	m_hMessageWND = NULL;
	m_hMessageWNDEnglish = NULL;	// [bug 11183]名称チップ用

	// 再描画してごみを消す
	if( m_ipSelectFeatEnv )
	{
		DOUBLE dBufferSize = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), m_lRefreshBuf );
		m_ipSelectFeatEnv->Expand(  dBufferSize, dBufferSize, VARIANT_FALSE );
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, m_ipSelectFeatEnv );
		m_ipSelectFeatEnv = NULL;
	}

	// ダイアログを消す
	if( !m_bIsModal )
		DestroyWindow();	// モードレスの場合
	else
		EndDialog(0);		// モーダルの場合

	m_bClosedDialog = TRUE;
	m_lstUpdate.clear();

	// こんなところで、フラグ初期化しておく・・・
	m_bIsFirst = TRUE;
	WriteProperty();

	return TRUE;
}

// OKボタンの表示状態を変更する
void CAttributeDlgObj::SetButton( BOOL bEdited )
{
	esriEditState state = esriStateNotEditing;
	IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
	ipEditor->get_EditState( &state );

	// 編集開始されていて、新規作成などルールに従わない場合にはOKボタン押せる状態にしておかなければ、
	// 変更が掛かっていない場合に、OKボタン押せなくなってしまう
	if( m_sAttributeEditType == sindyeNotApplyRuleEnable && state == esriStateEditing)
		bEdited = TRUE;

	if( state != esriStateEditing )	// 編集開始されたときのデータフレームがアクティブデータフレームである場合のみ押せるようにする
		bEdited = FALSE;

	::EnableWindow( GetDlgItem( IDOK ), bEdited );

	// ツリーコントロールのアイテムの色も同じタイミングで変える必要があるので再描画
	m_cTreeCtrl.Invalidate( FALSE );
}

// ダイアログをリサイズするときの関数
void CAttributeDlgObj::ResizeClientDlg()
{
	if( ::IsWindow(m_hCurrentWnd) )
	{
		CRect rect;
		::GetWindowRect( GetDlgItem(IDC_STATIC_BASE), rect );
		ScreenToClient( rect );
		// [Bug 5490]ツリー表示部にゴミが残るバグ
		HDWP hDWP = BeginDeferWindowPos( 1 );
		hDWP = ::DeferWindowPos( hDWP, m_hCurrentWnd, HWND_TOP, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
		EndDeferWindowPos( hDWP );
	}
}

// 今保持しているフィーチャに変更があるかどうか
BOOL CAttributeDlgObj::Changed()
{
	// 編集されているかどうか確認
	BOOL bChanged = FALSE;
	for( std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin(); it != m_listFeatureDef.end(); it++ )
	{
		if( it->Changed() ) 
			return TRUE;
	}

	for( std::list<CLQRowDef>::iterator itLQ = m_listLQRowDef.begin(); itLQ != m_listLQRowDef.end(); itLQ++ )
	{
		if( itLQ->Changed() ) 
			return TRUE;
	}

	return FALSE;
}

// イベントに接続する関数
BOOL CAttributeDlgObj::ConnectToEvents( BOOL bConnect/* = TRUE */ )
{
	BOOL bRet = FALSE;

	IEditorPtr ipEdit( m_cArcHelper.GetEditor() );
	if( bConnect == TRUE )
	{
		// IActiveViewEvents に接続します
		if( m_dwViewCookie != 0 )
		{
			ATLASSERT(FALSE);
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : 接続クッキーが 0 ではありませんが、強制的に接続します\n"));
		}
		if( FAILED( AtlAdvise(m_cArcHelper.GetActiveView(), this->GetUnknown(), IID_IActiveViewEvents, &m_dwViewCookie) ) )
		{
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : ActiveViewEvents への接続が失敗しました\n"));
			bRet = FALSE;
		}

		// IEditEvents に接続します
		if( m_dwEditCookie != 0 )
		{
			ATLASSERT(FALSE);
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : 接続クッキーが 0 ではありませんが、強制的に接続します\n"));
		}
		if( FAILED( AtlAdvise(ipEdit, this->GetUnknown(), IID_IEditEvents, &m_dwEditCookie) ) )
		{
			ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : EditEvents への接続が失敗しました\n"));
			bRet = FALSE;
		}
	}
	else {
		// イベント接続解除
		if( m_dwViewCookie != 0 )
		{
			if( FAILED( AtlUnadvise( m_cArcHelper.GetActiveView(), IID_IActiveViewEvents, m_dwViewCookie) ) )
			{
				ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : ActiveViewEvents への接続解除が失敗しました\n"));
				bRet = FALSE;
			} else
				m_dwViewCookie = 0;
		}

		if( m_dwEditCookie != 0 )
		{
			if( FAILED( AtlUnadvise( ipEdit, IID_IEditEvents, m_dwEditCookie) ) )
			{
				ATLTRACE(_T("CAttributeDlgObj::ConnectToEvents : EditEvents への接続解除が失敗しました\n"));
				m_dwEditCookie = 0;
				bRet = FALSE;
			} else
				m_dwEditCookie = 0;
		}
	}

	return bRet;
}

// 子ダイアログを見つけてくる関数
CExportDlg* CAttributeDlgObj::SetChildDlg(CString strClassName)
{
	CExportDlg* cExportDlg = NULL;

	CString strDRMAClassName;
	int iDRMALINKClassName = -1,iDRMAINLINEClassName = -1,iDRMAINPOINTClassName = -1,iDRMANODEClassName = -1,iVICSClassName = -1,iBAnnoName = -1, iMAnnoName = -1,iTAnnoName = -1,iAnnotation = -1,iDispline = -1;

	// DRMAリンクかどうか調べる
	iDRMALINKClassName = strClassName.Find(_T("DRMA_LINK"));
	if( iDRMALINKClassName == -1 )
		iDRMALINKClassName = strClassName.Find(_T("DRMADIFF_LINK"));

	// DRMAノードかどうか調べる
	iDRMANODEClassName = strClassName.Find(_T("DRMA_NODE"));
	if( iDRMANODEClassName == -1)
		iDRMANODEClassName = strClassName.Find(_T("DRMADIFF_NODE"));

	// DRMAリンク内属性かどうか調べる
	iDRMAINLINEClassName = strClassName.Find(_T("DRMA_INLINE"));

	// DRMAリンク内ポイントかどうか調べる
	iDRMAINPOINTClassName = strClassName.Find(_T("DRMA_INPOINT"));

	// VICSかどうか調べる
	iVICSClassName = -1;
	if( m_bIsLQAttribute == FALSE )
		iVICSClassName = strClassName.Find(_T("VICS"));

	std::map<CString, CExportDlg*>::iterator it = m_mapDlg.find( strClassName );
	if( it == m_mapDlg.end() )
	{
		if( m_strInstallPath.IsEmpty() == TRUE )
		{
			CInifileHelper	cIniFile;
			// SiNDY-eインストールルートパス取得
			cIniFile.GetInstallRoot( m_strInstallPath );
		}

		// DRMAリンクなら、クラスネームを取得し、DRMA_LINK.DLLを呼び出す
		if( iDRMALINKClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_LINK");
		}
		// DRMAノードなら、クラスネームを取得し、DRMA_NODE.DLLを呼び出しておく
		else if( iDRMANODEClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_NODE");
		}
		// DRMAリンク内属性なら、クラスネームを取得し、DRMA_INLINE.DLLを呼び出しておく
		else if( iDRMAINLINEClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_INLINE");
		}
		// DRMAリンク内ポイントなら、クラスネームを取得し、DRMA_INLINE.DLLを呼び出しておく
		else if( iDRMAINPOINTClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("DRMA_INPOINT");
		}
		// VICSなら、クラスネームを取得し、VICS.DLLを呼び出しておく
		else if( iVICSClassName != -1 )
		{
			strDRMAClassName = strClassName;
			strClassName = _T("VICS");
		}

		CString strModuleName = strClassName;
		strModuleName += _T(".dll");
		strModuleName = m_strInstallPath + _T("ChildDlgDLL\\") + strModuleName;

#ifdef _USE_CHILDDLG_SHORTCUT
		HMODULE hModule = LoadShellLinkLibrary( strModuleName );
#else
		HMODULE hModule = ::LoadLibrary( strModuleName );
#endif // #ifdef _USE_CHILDDLG_SHORTCUT

		if( hModule != NULL )
		{
			if( ! cExportDlg )
			{
				CreateDlgByName *pCreateTest = (CreateDlgByName *)GetProcAddress(hModule,"CreateDlgByName"); // 子ダイアログ統合版呼び出し
				if( pCreateTest )
					cExportDlg = pCreateTest( strClassName );
				else {
					CreateDlg *pCreateTest = (CreateDlg *)GetProcAddress(hModule,"CreateDlg");
					cExportDlg = pCreateTest();
				}
			}
			else
				_ASSERTE( ! cExportDlg );

			cExportDlg->SetArcHelper( m_cArcHelper.GetApp() );

			// タブ位置をセット
			if( strClassName == _T("ROAD_LINK") )
				cExportDlg->SetTabNum(m_lRoadTabNumber);
			else if( strClassName == _T("DRMA_LINK") )
				cExportDlg->SetTabNum(m_lDRMATabNumber);
			else if( strClassName == _T("ACCESS_POINT") )
				cExportDlg->SetTabNum(m_lAccessTabNumber);

			m_cExportDlg = cExportDlg;

			if( strClassName == _T("HIGHWAY_NODE") )
			{
				cExportDlg->SetTabNum(m_lHwyNodeTabNumber);	// [Bug6570] 料金所レーン情報用にタブを追加したので修正
			}

			if( iDRMALINKClassName != -1 || iDRMANODEClassName != -1 || iDRMAINLINEClassName != -1 || 
				iDRMAINPOINTClassName != -1 || iVICSClassName != -1 || iBAnnoName != -1 || iMAnnoName != -1 || iTAnnoName != -1 )
				m_mapModule[strDRMAClassName] = hModule;
			else
				m_mapModule[strClassName] = hModule;
		}
	}
	else
		cExportDlg = it->second;

	// VICS系子ダイアログ統合で必要だったので全ての子ダイアログで呼ぶよう修正
	if( cExportDlg != NULL )
		cExportDlg->SetFeatureClassName( strDRMAClassName.IsEmpty() ? strClassName : strDRMAClassName );

	return cExportDlg;
}

// 各種フィーチャのチェック
BOOL CAttributeDlgObj::ErrorCheck()
{
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg* cExportDlg = it->second;
		if( cExportDlg->ErrorCheck() == FALSE )
			return FALSE;
	}
	return TRUE;
}


// チェック後のデータ更新処理
bool CAttributeDlgObj::Update()
{
	for( std::map<CString, CExportDlg*>::iterator it = m_mapDlg.begin(); it != m_mapDlg.end(); it++ )
	{
		CExportDlg2* cExportDlg2 = dynamic_cast<CExportDlg2*>(it->second);
		if( ! cExportDlg2 )
			continue;

		if( ! cExportDlg2->Update() )
			return false;
	}
	return true;
}


// 属性ダイアログ関連情報をレジストリに書き込む
void CAttributeDlgObj::WriteProperty()
{
	// 初期状態の時にはフラグが初期化されていないので、強引にフラグをFALSEにしておく
	if( m_bIsGridEditMode != TRUE && m_bIsGridEditMode != FALSE)
	{
		CString strRegOpenKey;
		strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
		if ( hOpenKey ) 
		{
			m_bIsGridEditMode = FALSE;
			EmxSetRegDword( _T("GridEditMode"), (DWORD)m_bIsGridEditMode, hOpenKey );
			RegCloseKey( hOpenKey );
		}
	}
}

// 属性ダイアログ関連情報をレジストリから読み取る
void CAttributeDlgObj::LoadProperty()
{
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = NULL;
	hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		DWORD dwResult = 0;
		if( EmxGetRegDword( _T("GridEditMode"), &dwResult, hOpenKey ) )
			m_bIsGridEditMode = (BOOL)dwResult;
		if( EmxGetRegDword( _T("IsLineHilight"), &dwResult, hOpenKey ) )
			m_bIsLineHilight   = ((BOOL)dwResult)? true : false;
		if( EmxGetRegDword( _T("IsVertexHilight"), &dwResult, hOpenKey ) )
			m_bIsVertexHilight = ((BOOL)dwResult)? true : false;

		RegCloseKey( hOpenKey );
	}

	// フィーチャの再描画範囲（デフォルト：5Map単位）
	strRegOpenKey.Format( REGCOMMONOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		if( !EmxGetRegDword( _T("RefreshBuffer"), &m_lRefreshBuf, hOpenKey ) )
			m_lRefreshBuf = 5;
		RegCloseKey( hOpenKey );
	}
}

// ツールチップの初期化・更新の関数
void CAttributeDlgObj::InitToolTip(const int nID)
{
	// エディットボックスにかかれている文字列を取得
	CStatic cStatic( GetDlgItem(nID) );
	INT nTextLen = cStatic.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	cStatic.GetWindowText( lpszText, nTextLen );

	static TOOLINFO ti;
	ZeroMemory(&ti , sizeof(TOOLINFO)) ;
	// TTM_ADDTOOLメッセージを正常に送るためのおまじない（bug 3934）
	// c.f. http://ameblo.jp/blueskyame/entry-10398978729.html
	ti.cbSize = sizeof(TOOLINFO) - sizeof(void*) ;
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS ;
	ti.hwnd = GetDlgItem(nID);
	ti.uId = (UINT)GetDlgItem(nID).m_hWnd;
	CString str = lpszText;
	str.Remove( _T('\n') );
	lpszText = (LPTSTR)(LPCTSTR)str;
	ti.lpszText = lpszText;

	if( IDC_STATIC_MESSAGE == nID )
	{
		if( m_hMessageWND == NULL )
		{
			// ツールチップ作成
			HWND hToolTip;
			hToolTip =
				CreateWindowEx(
				0,				//拡張ウィンドウスタイル
				TOOLTIPS_CLASS,	//クラスネーム
				NULL,			//ウィンドウネーム
				TTS_ALWAYSTIP,	// ウィンドウスタイル
				CW_USEDEFAULT,	// Ｘ座標
				CW_USEDEFAULT,	// Ｙ座標
				CW_USEDEFAULT,	// 幅
				CW_USEDEFAULT,	// 高さ
				GetDlgItem(nID),// 親ウィンドウのハンドル
				NULL,			// メニューハンドル
				NULL,			// インスタンスハンドル
				NULL);			// WM_CREATEデータ
			m_hMessageWND = hToolTip;
			::SendMessage( m_hMessageWND, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWND, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
			::SendMessage( m_hMessageWND, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);
		}
		else
		{
			::SendMessage( m_hMessageWND, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWND, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)300);
			::SendMessage( m_hMessageWND, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
		}
	}

	// [bug 11183]名称チップ用
	if( IDC_STATIC_ALTERNATIVENAME == nID || IDC_STATIC_OFFICIALNAME == nID )
	{
		if( m_hMessageWNDEnglish == NULL )
		{
			if( !m_OfficialNameFont )
				m_OfficialNameFont.CreatePointFont( 140, AheLoadString(IDS_FONT) );

			// ツールチップ作成
			HWND hToolTip;
			hToolTip =
				CreateWindowEx(
				0,				//拡張ウィンドウスタイル
				TOOLTIPS_CLASS,	//クラスネーム
				NULL,			//ウィンドウネーム
				TTS_ALWAYSTIP,	// ウィンドウスタイル
				CW_USEDEFAULT,	// Ｘ座標
				CW_USEDEFAULT,	// Ｙ座標
				CW_USEDEFAULT,	// 幅
				CW_USEDEFAULT,	// 高さ
				GetDlgItem(nID),// 親ウィンドウのハンドル
				NULL,			// メニューハンドル
				NULL,			// インスタンスハンドル
				NULL);			// WM_CREATEデータ
			m_hMessageWNDEnglish = hToolTip;
			::SendMessage( m_hMessageWNDEnglish, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWNDEnglish, WM_SETFONT , (WPARAM)(HFONT)m_OfficialNameFont , TRUE);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)600);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETDELAYTIME, TTDT_AUTOPOP, 32000);
		}
		else
		{
			::SendMessage( m_hMessageWNDEnglish, TTM_ADDTOOL , 0 , (LPARAM)&ti);
			::SendMessage( m_hMessageWNDEnglish, WM_SETFONT , (WPARAM)(HFONT)m_OfficialNameFont , TRUE);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETMAXTIPWIDTH , 0 , (LPARAM)600);
			::SendMessage( m_hMessageWNDEnglish, TTM_SETTOOLINFO, 0, (LPARAM)&ti );
		}
	}
}

// モディファイチェックをする関数
void CAttributeDlgObj::FinalModifyCheck()
{
	VARIANT vt;
	_IRowPtr ipRow;
	sindyUpdateType ut = sindyUpdateTypeNoUpdate;
	sindyeEditType et = sindyeEditCreate;

	ATLTRACE(_T("更新対象 %d\n"), m_lstUpdate.size());

	for ( std::list<CAdapt<_IRowPtr> >::iterator itItem = m_lstUpdate.begin(); itItem != m_lstUpdate.end(); itItem++ ) {
		/// 最後のオペレーションを取得
		ipRow = (*itItem).m_T;
		vt = AheGetAttribute(ipRow, UPDATETYPE_CFIELD);

	#ifdef _DEBUG
		LONG lOID;
		ipRow->get_OID(&lOID);
		ATLTRACE(_T("%s %d %d\n"), AheGetFeatureClassName(ipRow), lOID, vt.lVal);
	#endif
		if ( vt.vt != VT_NULL ) {
			ut = static_cast<sindyUpdateType>(vt.lVal);

			switch ( ut ) {
			case sindyUpdateTypeNoUpdate:
				//!< 移行時既存
				break;
			case sindyUpdateTypeCreate:
				et = sindyeEditCreate;
				break;
			case sindyUpdateTypeDelete:
				et = sindyeEditDelete;
				break;
			case sindyUpdateTypeShape:
				et = sindyeEditMove;
				break;
			case sindyUpdateTypeProperty:
				et = sindyeEditAttribute;
				break;
			}

			if ( ut != sindyUpdateTypeNoUpdate ) {
				ISiNDYModifyCheckPtr	ipModifyCheck;
				if( ipModifyCheck == NULL )
					ipModifyCheck.CreateInstance( CLSID_SiNDYModifyCheckObj );

				if( ipModifyCheck != NULL )
				{
					VARIANT_BOOL vbModified = VARIANT_FALSE;
					CComBSTR bstrMsg;
					if( SUCCEEDED( ipModifyCheck->IsModified( ipRow, &vbModified, &bstrMsg ) ) )
					{
						if( vbModified == VARIANT_TRUE )
						{
							try {
								IOperationStackPtr ipOpStack;

								if( SUCCEEDED( ((IMxDocumentPtr)m_cArcHelper.GetDocument())->get_OperationStack( &ipOpStack ) ) ) {
									IWorkspacePtr	ipEditWorkspace;

									m_cArcHelper.GetEditor()->get_EditWorkspace( &ipEditWorkspace );
									IWorkspaceEditPtr ipWorkspaceEdit( ipEditWorkspace );

									if( SUCCEEDED( ipWorkspaceEdit->UndoEditOperation() ) ) {
										LONG lOpCount = -1;

										if( SUCCEEDED( ipOpStack->get_Count( &lOpCount ) ) ) {
											ipOpStack->Remove(lOpCount - 1);
										}
									}
								}
							} catch (...) {
								AheMessageBox(NULL, AheLoadString( IDS_FAILTOUNDO ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
							}

							break;
						}
					}
				}
			}
		}
	}
}

// 設定されている属性にDB的なエラーがないかどうかを調べる関数
BOOL CAttributeDlgObj::CheckValue(CFeatureDef *pFeatureDef)
{
	long lOID = 0;
	_IRowPtr ipRow( pFeatureDef->GetFeature() );
	ipRow->get_OID( &lOID );
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	// PGDBではesriFieldTypeIntegerの桁数チェックができないので、判別する必要がある
	CString strFeatureClassName = AheGetFeatureClassName( ipRow );
	ITablePtr ipTable = AheGetFeatureClass( ipRow );
	esriWorkspaceType WorkspaceType = esriRemoteDatabaseWorkspace;
	CComBSTR bstrName;
	if( ipTable )
	{
		IDatasetPtr ipDataset = ipTable;
		if( ipDataset )
		{
			ipDataset->get_Name( &bstrName );
			IWorkspacePtr ipWorkspace;
			ipDataset->get_Workspace( &ipWorkspace );
			ipWorkspace->get_Type( &WorkspaceType );
		}
	}
	bool bIsPGDB = false;
	if( WorkspaceType != esriRemoteDatabaseWorkspace )
		bIsPGDB = true;

	// すべてのフィールドチェック
	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; i++ )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName;
		ipField->get_Name( &bstrFieldName );
		// 現在の値を取得
		CFieldDef *cFieldDef = pFeatureDef->GetFieldDef( strFeatureClassName, bstrFieldName );
		CComVariant vaValue = cFieldDef->m_vaValue;
		if( !CheckField( ipField, vaValue, lOID, bstrName, bIsPGDB ) )
			return false;
	}
	return true;
}

// リンク列の属性にDB的なエラーがないかどうかを調べる関数
BOOL CAttributeDlgObj::LQCheckValue(CLQRowDef *pLQRowDef)
{
	long lOID = 0;
	_IRowPtr ipRow( pLQRowDef->GetFeature() );
	ipRow->get_OID( &lOID );
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	// PGDBではesriFieldTypeIntegerの桁数チェックができないので、判別する必要がある
	CString strFeatureClassName = AheGetFeatureClassName( ipRow );
	ITablePtr ipTable;
	ipRow->get_Table( &ipTable );
	esriWorkspaceType WorkspaceType = esriRemoteDatabaseWorkspace;
	CComBSTR bstrName;
	if( ipTable )
	{
		IDatasetPtr ipDataset = ipTable;
		if( ipDataset )
		{
			ipDataset->get_Name( &bstrName );
			IWorkspacePtr ipWorkspace;
			ipDataset->get_Workspace( &ipWorkspace );
			ipWorkspace->get_Type( &WorkspaceType );
		}
	}
	bool bIsPGDB = false;
	if( WorkspaceType != esriRemoteDatabaseWorkspace )
		bIsPGDB = true;

	// すべてのフィールドチェック
	long lFieldCount = 0;
	ipFields->get_FieldCount( &lFieldCount );
	for( long i = 0; i < lFieldCount; i++ )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName;
		ipField->get_Name( &bstrFieldName );
		// 現在の値を取得
		CFieldDef *cFieldDef = pLQRowDef->GetFieldDef( strFeatureClassName, bstrFieldName );
		CComVariant vaValue = cFieldDef->m_vaValue;
		if( !CheckField( ipField, vaValue, lOID, bstrName, bIsPGDB ) )
			return false;
	}
	return true;
}

// 本当ならIField::CheckValueをやればそれでいいのかもしれないが、エラーメッセージをだす必要があるので、しょうがなく
// さまざまなパターンをチェックする
bool CAttributeDlgObj::CheckField( IFieldPtr ipField, CComVariant vaValue, long lOID, CComBSTR bstrName, bool bIsPGDB )
{
	CComBSTR bstrFieldName;
	ipField->get_Name( &bstrFieldName );
	// フィールドタイプによって分ける
	esriFieldType type = esriFieldTypeSmallInteger;
	ipField->get_Type( &type );

	// NULL値だった場合にはNULLOKかどうかを調べる
	if( vaValue.vt == VT_NULL )
	{
		VARIANT_BOOL bIsNullable;
		ipField->get_IsNullable( &bIsNullable );
		if( bIsNullable )
			return true;
		else
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_NOTPERMITNULLVALUE ), bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}
	}

	// レンジドメインを持っている場合には、ドメイン内に値が入っているかどうかをチェック
	IDomainPtr ipDomain = NULL;
	IRangeDomainPtr ipRangeDomain = NULL;
	ipField->get_Domain( &ipDomain );
	ipRangeDomain = ipDomain;
	if( ipRangeDomain != NULL )
	{
		CComVariant vaMax, vaMin;
		ipRangeDomain->get_MaxValue( &vaMax );
		ipRangeDomain->get_MinValue( &vaMin );
		// VT_R8でなぜか返ってくるので、ChangeTypeであわせておく
		// 文字などは入っていないという前提に
		vaMax.ChangeType( vaValue.vt );
		vaMin.ChangeType( vaValue.vt );
		if( vaMax < vaValue || vaMin > vaValue )
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_OUTSIDEOFRANGEDOMAIN ), bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}
	}

	// esriFieldTypeIntegerまたはesriFieldTypeSingleの場合
	if( type == esriFieldTypeInteger || type == esriFieldTypeSingle)
	{
		// PGDBの場合にはチェックできないので正常として返す
		if( bIsPGDB )
			return true;

		// 正の値にしておく
		double dCheckValue = 0;
		if( type == esriFieldTypeInteger )
			dCheckValue = abs(vaValue.lVal);
		else if( type == esriFieldTypeSingle )
			dCheckValue = abs(vaValue.fltVal);		// doubleで受け取っても問題なし

		// 全桁数を取得
		long lPrecision = -1;
		ipField->get_Precision( &lPrecision );
		// 小数点以下の桁数
		long lScale = -1;
		ipField->get_Scale( &lScale );
		// 最大の値を作成
		// FLOATの場合には全桁数が４、小数点以下桁数が１となっている場合には
		// 最大値は9999ではなく、999.9までの入力となる、小数点以下桁数が２の場合には、99.99が最大値となる
		double dLimitValue = 1;
		for( int i = 0; i < lPrecision - lScale; i++ )
			dLimitValue  = dLimitValue * 10;

		if( dLimitValue - dCheckValue <= 0 )
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_OUTSIDEOFRANGE ), bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}

		if( type == esriFieldTypeSingle )
		{
			float dCheckScale = abs(vaValue.fltVal);
			for( int i = 0; i < lScale; i++ )
				dCheckScale = dCheckScale * 10.0;
			// まだ小数点以下の値があったら、いけない
			double temp;
			if( modf(dCheckScale, &temp) != 0 )
			{
				CString strMessage;
				strMessage.Format( AheLoadString( IDS_DECIMALPOINT ) ,bstrName, lOID, bstrFieldName, lScale );
				AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
				return false;
			}
		}
	}
	else if( type == esriFieldTypeString )
	{
		long lLength = 0;
		ipField->get_Length( &lLength );
		CString strValue = vaValue.bstrVal;

		// [Bug 5418]とは直接関係ないが、CString::GetLengthで文字数とってしまっていて、文字数のチェックになってしまっていたので、
		// バイト数でのチェックに修正　
		// UNICODE環境下でCString::GetLength() * sizeof( TCHAR )だと、結局、文字数*2となって、欲しい値が得られない。
		// 例えば"あa1"という文字列に対して6バイトが返ってきてしまう。半角文字を1バイトとし例を4バイトとみなしたい。
		// WideCharToMultiByteでANSIに変換（ANSIでは半角文字は1バイト、全角は2バイト）して、それに-1をしてあげると、求めるバイト数になる
		INT nByte = ::WideCharToMultiByte(CP_ACP, 0, T2CW((LPCTSTR)(strValue)), -1, NULL, 0, NULL, NULL) - 1;
		if( nByte > lLength )
		{
			CString strMessage;
			strMessage.Format( AheLoadString( IDS_OUTSIDEOFRANGE ),bstrName, lOID, bstrFieldName );
			AheMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONERROR|MB_TASKMODAL, WARNLEVEL_HIGH);
			return false;
		}
	}

	return true;
}

#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <TDC/namelib/OfficialNameTable.h>
#include <TDC/namelib/OfficialNameRow.h>
#include <TDC/namelib/AlternativeNameTable.h>
#include <TDC/namelib/AlternativeNameRow.h>
#include <TDC/namelib/TranslationTable.h>
#include <TDC/namelib/TranslationRow.h>
#include <TDC/namelib/name_globals.h>
using namespace sindy;

// 整備用のファインダーの初期化関数
bool CAttributeDlgObj::InitCollection( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::global;
	IWorkspacePtr ipWorkspace( AheGetWorkspace( ipFeature ) );
	esriWorkspaceType type = esriWorkspaceType::esriFileSystemWorkspace;
	ipWorkspace->get_Type( &type );
	CString strTableName;
	if( type == esriWorkspaceType::esriRemoteDatabaseWorkspace )
	{
		CString strOwnerName = AheGetOwnerDotClassName( ipFeature );
		strTableName.Format( _T("%s.%s"), strOwnerName.Left( strOwnerName.Find( _T(".") ) ), official_name::kTableName );
	}
	else
		strTableName = official_name::kTableName;

	ITablePtr ipOfficialName = AheOpenTableByTableName( ipWorkspace, strTableName );
	if( ipOfficialName )
	{
		// 必ず初期化
		m_cFinder.ClearCollection();
		m_cFinder.InitCollection( ipOfficialName, GetNameModel() );
		return true;
	}
	return false;
}

// フィーチャの名称IDを取得する
long CAttributeDlgObj::GetNameID( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::global;
	IFieldsPtr ipFields;
	ipFeature->get_Fields( &ipFields );
	sindy::CFieldMap cFieldMap( ipFields );
	long lIndex = cFieldMap.FindField( sindy::schema::global::name_class::kNameID );
	if( lIndex == -1 )
		return NO_NAME_ID_FIELD;

	CComVariant vaValue;
	ipFeature->get_Value( lIndex, &vaValue );
	if( vaValue.vt != VT_I4 )
		return NO_NAME_ID;
	return vaValue.lVal;
}

// 名称を表示する関数
bool CAttributeDlgObj::DisplayObjectName( IFeaturePtr ipFeature )
{
	using namespace sindy::schema::global;
	// コントロール初期化
	CStatic cStaticOfficial( GetDlgItem( IDC_STATIC_OFFICIALNAME ) );
	CStatic cStaticAlternative( GetDlgItem( IDC_STATIC_ALTERNATIVENAME ) );
	cStaticOfficial.SetWindowText( _T("") );
	cStaticAlternative.SetWindowText( _T("") );

	if( !ipFeature )
		return false;

	// 初期化
	if( !InitCollection( ipFeature ) )
		return false;

	// 名称ID取得してみる
	long lNameID = GetNameID( ipFeature );
	if( lNameID <= 0 )
		return false;

	// 名称IDあったら検索する
	CSPOfficialNameTable spOfficialNameTable = CAST_OFFICIALTABLE( m_cFinder.FindTable( official_name::kTableName ) );
	if( !spOfficialNameTable || !(ITable*)*spOfficialNameTable )
		return false;

	// 名称検索
	spOfficialNameTable->Select( lNameID );

	// ないはずはないんだが
	if( !spOfficialNameTable->empty() )
	{
		// ここまでこれたら名称もってる
		CSPOfficialNameRow spONRow = CAST_OFFICIALROW( *spOfficialNameTable->begin() );
		cStaticOfficial.SetWindowText( spONRow->GetValue( spOfficialNameTable->GetFieldMap()->FindField( official_name::kName ) ).bstrVal );

		CSPTableContainer spANTable = spONRow->GetItemTable( alternative_name::kTableName );
		if( spANTable && (ITable*)*spANTable && !spANTable->empty() )
		{
			// Alternativeは複数紐づく場合があるが、1つ目だけ選択
			CSPAlternativeNameRow spANRow = CAST_ALTERNATIVEROW( *spANTable->begin() );
			cStaticAlternative.SetWindowText( spANRow->GetValue( spANTable->GetFieldMap()->FindField( alternative_name::kName ) ).bstrVal );
		}
	}
	else
		ATLASSERT( false );

	return true;
}

// [bug 11183]表示用フォントの設定
void CAttributeDlgObj::SetLocalFont( CWindow& wnd )
{
	static CFont font;
	if( font.IsNull() )
	{
		// デフォルト： MS UI ゴシック 14pt
		CString fontName = uh::reg_util::read( reg_key::LocalFontName, reg_key::path, _T("MS UI Gothic") );
		long fontSize = uh::reg_util::read( reg_key::LocalFontSize, reg_key::path, 14 );
		font.CreatePointFont( fontSize*10, fontName );
	}

	wnd.SetFont( font );
}
