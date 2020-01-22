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

#include "StdAfx.h"
#include "AnalyzeSiNDYDataDlg.h"
#include "Workspace.h"
#include <string>
#include <math.h>
#include "ProgressDlg.h"
#include "Time.h"

CProgressDlg m_cProgressDlg;

// 進捗ダイアログを出すためのスレッド
void ThreadFunc(LPVOID vdParam) 
{
	HWND hProgressWnd = m_cProgressDlg.Create(NULL);
	BOOL bRet = ShowWindow( hProgressWnd, SW_SHOW );
	MSG msg;
	while (GetMessage(&msg , NULL , 0 , 0 )) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	HRESULT hr = m_cProgressDlg.DestroyWindow();
	return;
}

const double  EARTH_R  = 6370.0;

// ウィンドウマネージャ配置マップ
BEGIN_WINDOW_MAP(ANALYZEMAP)
	BEGINROWS(WRCT_REST, 0,RCMARGINS(2,2))
		RCSPACE(10)
		BEGINCOLS( WRCT_PCT, 0, 35 )
			BEGINCOLS( WRCT_REST, 0, 0 )
				RCSPACE(10)
				BEGINROWS( WRCT_PCT, IDC_STATIC_ANALYZE, 50 )
					RCSPACE(20)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_ANALYZECONNECTION)
						RCREST(IDC_EDIT_ANALYZECONNECTION)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_RADIO_SERVER)
						RCTOFIT(IDC_RADIO_FOLDER)
						RCREST(IDC_BUTTON_ANALYZECONNECT)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_FEATURECLASS)
						RCREST(IDC_COMBO_FEATURECLASS)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_FIELD)
						RCREST(IDC_COMBO_FIELD)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_QUERY)
						RCREST(IDC_EDIT_QUERY)
					ENDGROUP()
					RCSPACE(5)
				ENDGROUP()
				RCSPACE(10)
				BEGINROWS( WRCT_PCT, IDC_STATIC_AREA, 50 )
					RCSPACE(20)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_AREACONNECTION)
						RCREST(IDC_EDIT_AREACONNECTION)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_DUMMY2)
						RCREST(IDC_BUTTON_AREACONNECT)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_AREAFEATURECLASS)
						RCREST(IDC_COMBO_AREAFEATURECLASS)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_STATIC_LIST)
						RCREST(IDC_EDIT_LIST)
					ENDGROUP()
					RCSPACE(5)
				ENDGROUP()
				RCSPACE(10)
			ENDGROUP()
		ENDGROUP()
		BEGINCOLS( WRCT_PCT, 0, 65 )
			BEGINCOLS( WRCT_REST, 0, 0 )
				BEGINROWS( WRCT_PCT, 0, 60 )
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_RADIO_AREA)
						RCTOFIT(IDC_RADIO_LENGTH)
						RCTOFIT(IDC_RADIO_NUM)
					ENDGROUP()
					BEGINCOLS(WRCT_REST, 5, 5 )
						RCREST(IDC_GRID_ANALYZE)
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDC_BUTTON_EXPORT)
						RCSPACE(5)
						RCREST(IDC_STATIC_LOG)
					ENDGROUP()
				ENDGROUP()
				RCSPACE(5)
				BEGINROWS( WRCT_PCT, 0, 40 )
					BEGINCOLS(WRCT_TOFIT, 5, 5 )
						RCREST(IDC_STATIC_CHARTTITLE)
					ENDGROUP()
					BEGINCOLS(WRCT_REST, 5, 5 )
						BEGINROWS(WRCT_PCT, 0, 60 )
							RCREST(IDC_MSCHART)
						ENDGROUP()
						RCSPACE(5)
						BEGINROWS(WRCT_PCT, 0, 40 )
							RCREST(IDC_LEGEND)
						ENDGROUP()
					ENDGROUP()
					RCSPACE(5)
					BEGINCOLS( WRCT_TOFIT, 5, 5 )
						RCTOFIT(IDOK)
						RCTOFIT(IDCANCEL)
					ENDGROUP()
				ENDGROUP()
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

using namespace sindy;

static crd_cnv g_cCrdCnv;

CAnalyzeSiNDYDataDlg::CAnalyzeSiNDYDataDlg(void) : m_winMgr(ANALYZEMAP), m_lFieldIndex(-1)
{
}

CAnalyzeSiNDYDataDlg::~CAnalyzeSiNDYDataDlg(void)
{
}

LRESULT CAnalyzeSiNDYDataDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxWindow wnd;
	wnd = GetDlgItem(IDC_GRID_ANALYZE);
	wnd.QueryControl(&m_spAnalyzeGrid);

	AtlAdviseSinkMap(this, true);// イベント処理

	// ウィンドウマネージャによる再配置
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	// チャート
	m_wndChart.SubclassWindow(GetDlgItem(IDC_MSCHART));

	// 凡例
	m_wndLegend.SubclassWindow(GetDlgItem(IDC_LEGEND));

	CButton cButton1( GetDlgItem( IDC_RADIO_SERVER ) );
	cButton1.SetCheck( 1 );
	CButton cButton2( GetDlgItem( IDC_RADIO_NUM ) );
	cButton2.SetCheck( 1 );
	bHandled = FALSE;
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLASSERT(lParam);
	NMWINMGR& nmw = *(NMWINMGR*)lParam;
	if( nmw.code == NMWINMGR::SIZEBAR_MOVED) 
	{
		m_winMgr.MoveRect( wParam, nmw.sizebar.ptMoved, this );
		m_winMgr.SetWindowPositions(this);
		bHandled = TRUE;

		return -1;
	}
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO*>( lParam);
	m_winMgr.GetMinMaxInfo( this, pMinMaxInfo);
	pMinMaxInfo->ptMinTrackSize.x = 853;
	pMinMaxInfo->ptMinTrackSize.y = 480;
	bHandled = TRUE;
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TIPS: こうするとコンボボックスが消える問題は解消！？
	if( IsWindow() )
	{
		INT iWidth	= LOWORD(lParam);
		INT iHeight = HIWORD(lParam);

		SetRedraw(FALSE);
		m_winMgr.CalcLayout(iWidth, iHeight, this);
		m_winMgr.SetWindowPositions(this);

		SetRedraw(TRUE);
		RedrawWindow( NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_wndChart.Clear();
	m_wndLegend.Clear();
	EndDialog(wID);
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AtlAdviseSinkMap(this, FALSE);
	bHandled = FALSE;
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_wndChart.Clear();
	m_wndLegend.Clear();
	bHandled = FALSE;
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShowWindow(SW_HIDE);				// 実行時は、設定ダイアログ非表示

	CButton cButtonServer( GetDlgItem( IDC_RADIO_SERVER ) );

	CEdit cEdit( GetDlgItem( IDC_EDIT_LIST ) );
	CString strList;
	cEdit.GetWindowText( strList );

	// クエリ取得
	CEdit cEditQ( GetDlgItem( IDC_EDIT_QUERY ) );
	CString strQuery;
	cEditQ.GetWindowText( strQuery );

	// 分析できるだけの情報が揃っているかチェック
	if( CheckAnalyzeInfo() )
	{
		// スレッド作成
		_beginthread( ThreadFunc, 0, (LPVOID)m_hWnd );
		Sleep(1000); // 少し待ってやる
		ShowWindow( SW_HIDE );
		// 時間計測
		clock_t startTime, endTime;
		startTime = clock();

		CString strProgress;
		long lRow = 0;

		// サーバ（ファイル）タイプの分析
		if( cButtonServer.GetCheck() )
		{
			list< CString > listSearchArea;
			if( CreateSearchArea( strList, listSearchArea ) )
			{
				// 列を作成
				map< CString, CString > mapDomain;
				if( CreateGridCol( mapDomain ) )
				{
					m_cProgressDlg.SendMessage( WM_PROGRESSRANGE, 1, listSearchArea.size() );

					for( list< CString >::iterator it = listSearchArea.begin(); it != listSearchArea.end(); ++it, ++lRow )
					{
						strProgress.Format( _T("%s：%sを分析開始 ( %ld / %ld 件中)"), m_strSearchFieldName, *it, lRow+1, listSearchArea.size() );
						m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow );
						m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 1, (LPARAM)(LPCTSTR)(strProgress) );

						// クエリが文字列か数字かを判定
						CString strWhere;
						if( m_eSearchFieldType == esriFieldTypeString )
							strWhere.Format( _T("%s='%s'"), m_strSearchFieldName, *it );
						else
							strWhere.Format( _T("%s=%s"), m_strSearchFieldName, *it );

						IFeatureCursorPtr ipAreaFeatureCursor;
						m_ipAreaFeatureClass->Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE, &ipAreaFeatureCursor );
						if( ipAreaFeatureCursor )
							AnalyzeFeatureCursor( ipAreaFeatureCursor, lRow, *it, strQuery, mapDomain, *it );

						if( lRow == 0 )
							CreateChart( 1 );

						strProgress.Format( _T("%s：%sを分析完了 ( %ld / %ld 件中)"), m_strSearchFieldName, *it, lRow+1, listSearchArea.size() );
						m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow+1 );
					}
				}
			}
		}
		// フォルダタイプの分析
		else
		{
			// 列を作成
			map< CString, CString > mapDomain;
			if( CreateGridCol( mapDomain ) )
			{
				// フィーチャクラス名取得
				CComboBox cCombo( GetDlgItem( IDC_COMBO_FEATURECLASS ) );
				CString strCurSelText;
				cCombo.GetLBText(cCombo.GetCurSel(),strCurSelText.GetBuffer(256));
				strCurSelText.ReleaseBuffer();

				m_cProgressDlg.SendMessage( WM_PROGRESSRANGE, 1, m_listPGDBName.size() );

				for( list< CString >::const_iterator it = m_listPGDBName.begin(); it != m_listPGDBName.end(); ++it, ++lRow )
				{
					// 初期化
					m_ipAnalyzeFeatureClass = NULL;
					// プログレス設定
					strProgress.Format( _T("%sを分析開始 ( %ld / %ld 件中) "), *it, lRow+1, m_listPGDBName.size() );
					m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow );
					m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 1, (LPARAM)(LPCTSTR)(strProgress) );

					// 一行追加
					CString strName = *it;
					long l = strName.ReverseFind( '\\' );
					strName = strName.Right( strName.GetLength() - l - 1 );
					m_spAnalyzeGrid->AddItem( _T("") );
					m_spAnalyzeGrid->put_TextMatrix( lRow+1, 0, _bstr_t( *it ) );

					// PGDBオープンして、対象のフィーチャクラス取得する
					CWorkspace cWorkspace;
					cWorkspace.Connect( *it );

					// フィーチャクラスがあるかもしれない
					IEnumDatasetPtr ipEnumFeatureClass = cWorkspace.GetDatasets( esriDTFeatureClass );
					IEnumDatasetPtr ipEnumDataset = cWorkspace.GetDatasets( esriDTFeatureDataset );
					if( ipEnumFeatureClass )
					{
						IDatasetPtr ipDataset;
						while( ipEnumFeatureClass->Next( &ipDataset ) == S_OK )
						{
							CComBSTR bstrName;
							ipDataset->get_Name( &bstrName );
							if( (CString)bstrName == strCurSelText )
							{
								m_ipAnalyzeFeatureClass = ipDataset;
								break;
							}
						}
					}

					// もしなかったら、データセット内に収められてる
					if( !m_ipAnalyzeFeatureClass )
					{
						IDatasetPtr ipDataset;
						while( ipEnumDataset->Next( &ipDataset ) == S_OK )
						{
							IEnumDatasetPtr ipSubDatasets;
							((IFeatureDatasetPtr)ipDataset)->get_Subsets( &ipSubDatasets );
							IDatasetPtr ipFeatureClass;
							while( ipSubDatasets->Next( &ipFeatureClass ) == S_OK )
							{
								CComBSTR bstrName;
								ipFeatureClass->get_Name( &bstrName );
								if( (CString)bstrName == strCurSelText )
								{
									m_ipAnalyzeFeatureClass = ipFeatureClass;
									break;
								}
							}
						}
					}

					map< CString, double > mapAnalyze;
					AnalyzeData( NULL, strQuery, mapAnalyze, *it, false );
					OutputGrid( mapAnalyze, mapDomain, lRow );

					if( lRow == 0 )
						CreateChart( 1 );

					strProgress.Format( _T("%sを分析完了 ( %ld / %ld 件中) "), *it, lRow+1, m_listPGDBName.size() );
					m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow+1 );
				}
			}
		}

		m_spAnalyzeGrid->put_AutoSizeMode( flexAutoSizeColWidth );
		long lCols = 0;
		m_spAnalyzeGrid->get_Cols( &lCols );
		m_spAnalyzeGrid->AutoSize( 0, lCols -1 );


		// プログレスダイアログ閉じるメッセージ発行
		m_cProgressDlg.SendMessage(WM_PROGRESSCLOSE, 0, 0 );
		// 処理終了のため、時間計測
		endTime = clock();
		long lTime = (endTime - startTime) / (float)CLOCKS_PER_SEC;
		long lHour, lMin, lSec;
		lHour = lTime / 3600;
		lMin = (lTime - 3600*lHour) / 60;
		lSec = (lTime - 3600*lHour - 60*lMin );
		CString strMessage;
		strMessage.Format( _T("分析完了\n%d時間%d分%d秒かかりました"),lHour,lMin,lSec );
		MessageBox(strMessage, _T("AnalyzeSiNDYData"), MB_OK );
	}

	SetWindowPos( HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );	// チェック終了後再表示(必ず先頭に)
	::SetFocus( m_hWnd );	// フォーカスも当てる

	return 0;
}


LRESULT CAnalyzeSiNDYDataDlg::OnBnClickedButtonServerConnect(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CEdit cEdit;
	CComboBox cComboF;
	CString strConnetion;
	switch( wID )
	{
	case IDC_BUTTON_ANALYZECONNECT:
		{
			cEdit = GetDlgItem( IDC_EDIT_ANALYZECONNECTION );
			cComboF = GetDlgItem( IDC_COMBO_FEATURECLASS );
			// フィーチャクラス変更したら、フィールドもリセットする
			CComboBox cCombo2( GetDlgItem( IDC_COMBO_FIELD ) );
			cCombo2.ResetContent();
		 break;
		}
	case IDC_BUTTON_AREACONNECT:
		{
			cEdit = GetDlgItem( IDC_EDIT_AREACONNECTION );
			cComboF = GetDlgItem( IDC_COMBO_AREAFEATURECLASS );
		 break;
		}
	default:
		ATLASSERT( 0 );
		break;
	}

	// サーバ文字列取得
	cEdit.GetWindowText( strConnetion );
	// コンボボックス中身空にする
	cComboF.ResetContent();

	// フォルダ接続の場合には、最初に見つかったPGDBを持ってくる
	CButton cButtonServer( GetDlgItem( IDC_RADIO_SERVER ) );
	if( !cButtonServer.GetCheck() )
	{
		// 初期化
		m_listPGDBName.clear();

		WIN32_FIND_DATA		ffd;
		CString	strDirPath = strConnetion + _T("\\*");
		HANDLE hFile = FindFirstFile( strDirPath, &ffd );

		if ( hFile == INVALID_HANDLE_VALUE )
		{
			CString strErr;
			strErr.Format( _T("Can't open %s\n"), strDirPath );
			::MessageBox( NULL, strErr, _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
			return false;	// ファイルが存在しない？
		}
		FindFile( hFile, ffd, strConnetion, m_listPGDBName );
		FindClose(hFile);

		//最初の１つを取り出して、仮のワークスペースとしちゃう
		strConnetion = *(m_listPGDBName.begin());
	}

	CWorkspace cWorkspace;
	if( !cWorkspace.Connect( strConnetion ) )
	{
		CString strErr;
		strErr.Format( _T("Can't connect %s\n"), strConnetion );
		::MessageBox( NULL, strErr, _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
		return false;
	}

	switch( wID )
	{
	case IDC_BUTTON_ANALYZECONNECT:	m_ipAnalyzeWorkspace = (IWorkspace*)cWorkspace; break;
	case IDC_BUTTON_AREACONNECT:	m_ipAreaWorkspace = (IWorkspace*)cWorkspace;	 break;
	default:ATLASSERT( 0 );	break;
	}

	// フィーチャクラスがあるかもしれない
	IEnumDatasetPtr ipEnumFeatureClass = cWorkspace.GetDatasets( esriDTFeatureClass );
	if( ipEnumFeatureClass )
	{
		IDatasetPtr ipDataset;
		while( ipEnumFeatureClass->Next( &ipDataset ) == S_OK )
		{
			CComBSTR bstrName;
			ipDataset->get_Name( &bstrName );
			cComboF.AddString( (LPCTSTR)bstrName );
		}
	}

	IEnumDatasetPtr ipEnumDataset = cWorkspace.GetDatasets( esriDTFeatureDataset );
	if( !ipEnumDataset )
		return 0;

	IDatasetPtr ipDataset;
	while( ipEnumDataset->Next( &ipDataset ) == S_OK )
	{
		IEnumDatasetPtr ipSubDatasets;
		((IFeatureDatasetPtr)ipDataset)->get_Subsets( &ipSubDatasets );
		IDatasetPtr ipFeatureClass;
		while( ipSubDatasets->Next( &ipFeatureClass ) == S_OK )
		{
			CComBSTR bstrName;
			ipFeatureClass->get_Name( &bstrName );
			cComboF.AddString( (LPCTSTR)bstrName );
		}
	}

	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnCbnSelchangeComboFeatureclass(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComboBox cCombo;
	switch( wID )
	{
	case IDC_COMBO_FEATURECLASS:
		{
			cCombo = GetDlgItem( IDC_COMBO_FEATURECLASS );
			break;
		}
	case IDC_COMBO_AREAFEATURECLASS:
		{
			cCombo = GetDlgItem( IDC_COMBO_AREAFEATURECLASS );
			break;
		}
	default: break;
	}

	CString strCurSelText;
	cCombo.GetLBText(cCombo.GetCurSel(),strCurSelText.GetBuffer(256));
	strCurSelText.ReleaseBuffer();

	IEnumDatasetPtr ipEnumFeatureClass, ipEnumDataset;
	switch( wID )
	{
	case IDC_COMBO_FEATURECLASS:
		{
			m_ipAnalyzeWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumFeatureClass );
			m_ipAnalyzeWorkspace->get_Datasets( esriDTFeatureDataset, &ipEnumDataset );
			break;
		}
	case IDC_COMBO_AREAFEATURECLASS:
		{
			m_ipAreaWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumFeatureClass );
			m_ipAreaWorkspace->get_Datasets( esriDTFeatureDataset, &ipEnumDataset );
			break;
		}
	default: break;
	}

	if( ipEnumFeatureClass )
	{
		IDatasetPtr ipDataset;
		while( ipEnumFeatureClass->Next( &ipDataset ) == S_OK )
		{
			CComBSTR bstrName;
			ipDataset->get_Name( &bstrName );
			if( (CString)bstrName == strCurSelText )
			{
				switch( wID )
				{
				case IDC_COMBO_FEATURECLASS:
					{
						m_ipAnalyzeFeatureClass = ipDataset;
						break;
					}
				case IDC_COMBO_AREAFEATURECLASS:
					{
						m_ipAreaFeatureClass = ipDataset;
						break;
					}
				default: break;
				}
				break;
			}
		}
	}

	IDatasetPtr ipDataset;
	while( ipEnumDataset->Next( &ipDataset ) == S_OK )
	{
		IEnumDatasetPtr ipSubDatasets;
		((IFeatureDatasetPtr)ipDataset)->get_Subsets( &ipSubDatasets );
		IDatasetPtr ipFeatureClass;
		while( ipSubDatasets->Next( &ipFeatureClass ) == S_OK )
		{
			CComBSTR bstrName;
			ipFeatureClass->get_Name( &bstrName );
			if( (CString)bstrName == strCurSelText )
			{
				switch( wID )
				{
				case IDC_COMBO_FEATURECLASS:
					{
						m_ipAnalyzeFeatureClass = ipFeatureClass;
						break;
					}
				case IDC_COMBO_AREAFEATURECLASS:
					{
						m_ipAreaFeatureClass = ipFeatureClass;
						break;
					}
				default: break;
				}
				break;
			}
		}
	}

	if( wID == IDC_COMBO_AREAFEATURECLASS )
		return 0;

	IFieldsPtr ipFields;
	m_ipAnalyzeFeatureClass->get_Fields( &ipFields );
	long lFieldNum = 0;
	ipFields->get_FieldCount( &lFieldNum );

	CComboBox cCombo2( GetDlgItem( IDC_COMBO_FIELD ) );
	cCombo2.ResetContent();
	for( int i = 0; i < lFieldNum; ++i )
	{
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		CComBSTR bstrFieldName;
		ipField->get_Name( &bstrFieldName );
		cCombo2.AddString( (LPCTSTR)bstrFieldName );
	}
	// 初期化
	m_lFieldIndex = -1;

	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnCbnSelchangeComboField(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComboBox cCombo = GetDlgItem( IDC_COMBO_FIELD );
	CString strCurSelText;
	cCombo.GetLBText(cCombo.GetCurSel(),strCurSelText.GetBuffer(256));
	strCurSelText.ReleaseBuffer();

	IFieldsPtr ipFields;
	m_ipAnalyzeFeatureClass->get_Fields( &ipFields );
	ipFields->FindField( (bstr_t)strCurSelText, &m_lFieldIndex );
	return 0;
}

// 検索結果をファイルへ出力
LRESULT CAnalyzeSiNDYDataDlg::OnClickedExport(WORD/* wNotifyCode*/, WORD/* wID*/, HWND/* hWndCtl*/, BOOL& /*bHandled*/)
{
	// [名前を付けて保存]ダイアログ
	CFileDialog dlg(FALSE, _T("xls|txt"), _T("無題"), OFN_OVERWRITEPROMPT,
		_T("Excel ファイル (*.xls)\0*.xls\0タブ区切り テキストファイル(*.txt)\0*.txt\0すべてのファイル (*.*)\0*.*\0\0"));

	if(dlg.DoModal() == IDOK)
	{
		// ファイルが存在してたら、読み取り専用じゃないかチェック
		if( PathFileExists( dlg.m_szFileName ) )
		{
			//[Bug 6182]検索・統計ツールでファイル出力時に落ちる
			DWORD dAttr = GetFileAttributes( dlg.m_szFileName );
			if( dAttr & FILE_ATTRIBUTE_READONLY )
			{
				MessageBox( _T("選択したファイルは読み取り専用です。データ保存できませんでした"), _T("ファイル出力"), MB_OK|MB_ICONSTOP );
				return 0;
			}
		}
		// 読み取り専用以外でも落ちるかもしれないので、念のためtry catchで囲っとく
		try{
			// 拡張子がxlsならExcelファイルとして保存
			SaveLoadSettings emLoadSettings = (SaveLoadSettings)(flexFileData|flexFileTabText);
			CString strFileName( dlg.m_szFileName );
			if( 0 == strFileName.Right(4).CompareNoCase(_T(".xls")) )
				emLoadSettings = flexFileExcel;

			// [bug 7721][要望]検索結果をExcel形式で保存した際、選択している行が反転の状態にならないようにして欲しい
			m_spAnalyzeGrid->Select(0,0);// 選択行を一旦無理やりなくす
			if( FAILED( m_spAnalyzeGrid->SaveGrid( _bstr_t( dlg.m_szFileName ), emLoadSettings, VARIANT_TRUE ) ) )
				MessageBox( _T("データの保存に失敗しました。"), _T("ファイル出力"), MB_OK|MB_ICONSTOP );
		}
		catch(...)
		{
			MessageBox( _T("なんらかの理由により、データの保存に失敗しました。"), _T("ファイル出力"), MB_OK|MB_ICONSTOP );
		}
	}
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnClickedServer(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// フォルダタイプの場合には、右半分はdisableにしておく
	::EnableWindow( GetDlgItem( IDC_EDIT_AREACONNECTION ), wID == IDC_RADIO_SERVER ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_BUTTON_AREACONNECT ), wID == IDC_RADIO_SERVER ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_COMBO_AREAFEATURECLASS ), wID == IDC_RADIO_SERVER ? TRUE : FALSE );
	::EnableWindow( GetDlgItem( IDC_EDIT_LIST ), wID == IDC_RADIO_SERVER ? TRUE : FALSE );
	if( wID == IDC_RADIO_FOLDER )
	{
		m_strSearchFieldName = _T("");
		m_eSearchFieldType = (esriFieldType)0;
		m_listPGDBName.clear();
		m_ipAreaFeatureClass = NULL;
		m_ipAreaWorkspace = NULL;
	}

	return 0;
}

void __stdcall CAnalyzeSiNDYDataDlg::OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
{
	long lCol = 0, lRow = 0;
	m_spAnalyzeGrid->get_MouseCol( &lCol );
	m_spAnalyzeGrid->get_MouseRow( &lRow );

	CreateChart( lRow );
}

// リストから、探索エリアリストを抽出
bool CAnalyzeSiNDYDataDlg::CreateSearchArea( CString strList, list< CString >& listSearchArea )
{
	ifstream file( strList );
	if (! file.is_open())
		return false;

	int i = 0;
	while (file.good())
	{
		std::string strList;
		file >> strList;
		// 先頭は必ずフィールド名という縛り
		if( i == 0 )
		{
			m_strSearchFieldName = (CString)strList.c_str();
			// フィールドが文字列かどうか
			IFieldsPtr ipFields;
			m_ipAreaFeatureClass->get_Fields( &ipFields );
			IFieldPtr ipField;
			long lIndex = 0;
			ipFields->FindField( bstr_t( m_strSearchFieldName ), &lIndex );
			ipFields->get_Field( lIndex, &ipField );
			ipField->get_Type( &m_eSearchFieldType );
		}
		else
		{
			if( !strList.empty() )
				listSearchArea.push_back( (CString)strList.c_str() );
		}
		++i;
	}

	if( !m_strSearchFieldName.IsEmpty() && listSearchArea.size() > 0 )
		return true;

	return false;
}

// グリッドの先頭行を作成する
bool CAnalyzeSiNDYDataDlg::CreateGridCol( map<CString, CString >& mapDomain )
{
	CComboBox cCombo = GetDlgItem( IDC_COMBO_FIELD );
	CString strCurSelText;
	cCombo.GetLBText(cCombo.GetCurSel(),strCurSelText.GetBuffer(256));
	strCurSelText.ReleaseBuffer();

	// チャートクリア
	m_wndChart.Clear();
	m_wndLegend.Clear();

	IFieldsPtr ipFields;
	m_ipAnalyzeFeatureClass->get_Fields( &ipFields );
	IFieldPtr ipField;
	ipFields->get_Field( m_lFieldIndex, &ipField );
	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	ICodedValueDomainPtr ipCodedValueDomain( ipDomain );
	if( ipCodedValueDomain )
	{
		long lCount;
		ipCodedValueDomain->get_CodeCount( &lCount );
		for( int i = 0; i < lCount; i++ ) 
		{
			CComBSTR bstrName;
			CComVariant vaValue;
			ipCodedValueDomain->get_Value( i, &vaValue );
			ipCodedValueDomain->get_Name( i, &bstrName );
			CString strVal;
			strVal.Format( _T("%ld"), vaValue.lVal );
			mapDomain[ strVal ] = (CString)bstrName; 
		}
	}
	else
	{
		// コード値ドメインじゃない場合に、適当なサンプルを見つけてくるという
		// ことをやってあげたほうがいい

	}

	if( mapDomain.size() < 500 )
	{
		m_spAnalyzeGrid->put_Redraw( flexRDNone );	// イベント抑止
		m_spAnalyzeGrid->put_Rows(0);//初期化
		m_spAnalyzeGrid->put_Rows(1);
		m_spAnalyzeGrid->PutFixedRows(1);

		// 形状情報グリッドの初期化
		m_spAnalyzeGrid->put_Cols( mapDomain.size() + 2 );
		if( m_strSearchFieldName.IsEmpty() )
			m_spAnalyzeGrid->put_TextMatrix(0,0,bstr_t( _T("ファイル名") ) );
		else
			m_spAnalyzeGrid->put_TextMatrix(0,0,bstr_t( m_strSearchFieldName ) );
		int i = 1;
		for( map<CString, CString>::iterator it = mapDomain.begin(); it != mapDomain.end(); ++it, ++i )
			m_spAnalyzeGrid->put_TextMatrix(0,i,bstr_t( it->second ) );

		m_spAnalyzeGrid->put_TextMatrix( 0, i, _bstr_t( _T("Total") ) );
		m_spAnalyzeGrid->put_Redraw( (RedrawSettings)VARIANT_TRUE );	// イベント抑止解除
	}
	else
		return false;

	return true;

}

// グリッドの先頭行から文字列と一致するものを見つけてくる
long CAnalyzeSiNDYDataDlg::FindCol( CString strColName )
{
	long lColNum = 0;
	m_spAnalyzeGrid->get_Cols( &lColNum );
	for( int i = 0; i < lColNum; ++i )
	{
		CComBSTR bstrName;
		m_spAnalyzeGrid->get_TextMatrix( 0, i, &bstrName );
		if( (CString)bstrName == strColName )
			return i;
	}

	return -1;
}

// 分析タイプを判定する関数
long CAnalyzeSiNDYDataDlg::GetAnalyzeType()
{
	AnalyzeType eAnalyzeType = eNum;

	CButton cButtonArea( GetDlgItem( IDC_RADIO_AREA ) );
	CButton cButtonLen( GetDlgItem( IDC_RADIO_LENGTH ) );
	if( cButtonArea.GetCheck() )
		eAnalyzeType = eArea;
	else if( cButtonLen.GetCheck() )
		eAnalyzeType = eLength;
	else
		eAnalyzeType = eNum;

	return eAnalyzeType;
}

// チャートを作る関数（検索ツールから拝借）
void CAnalyzeSiNDYDataDlg::CreateChart( long lRow )
{
	if( lRow > 0 )
	{
		// 初期化
		m_wndChart.Clear();
		m_wndLegend.Clear();

		CStatic cStaticTitle( GetDlgItem( IDC_STATIC_CHARTTITLE ) );
		CString strTitle;
		if( m_strSearchFieldName.IsEmpty() )
			strTitle.Format( _T("%s's Chart"), OLE2T(m_spAnalyzeGrid->GetTextMatrix(lRow, 0) ) );
		else
			strTitle.Format( _T("%s:%s's Chart"), m_strSearchFieldName, OLE2T(m_spAnalyzeGrid->GetTextMatrix(lRow, 0) ) );
		cStaticTitle.SetWindowText( strTitle );


		m_wndChart.SetDispCount(500);
		m_wndLegend.SetDispCount(500);

		long lColNum = 0;
		m_spAnalyzeGrid->get_Cols( &lColNum );
		for( SHORT lCount = 1; lCount < lColNum-1; lCount++ ) {
			/// グラフに値をセット
			COLORREF col = m_wndChart.AddValue( (int)m_spAnalyzeGrid->GetValueMatrix(lRow, lCount) );
			CString sTmp = OLE2T(m_spAnalyzeGrid->GetTextMatrix(0, lCount ) );

			if ( sTmp.IsEmpty())
				sTmp = OLE2T(m_spAnalyzeGrid->GetTextMatrix(lCount, 1));

			m_wndLegend.AddLegend(col, sTmp);
		}
	}
}

// 分析するためにデータが揃っているかどうか調べる関数
bool CAnalyzeSiNDYDataDlg::CheckAnalyzeInfo()
{
	CButton cButtonServer( GetDlgItem( IDC_RADIO_SERVER ) );

	CEdit cEdit( GetDlgItem( IDC_EDIT_LIST ) );
	CString strList;
	cEdit.GetWindowText( strList );

	if( cButtonServer.GetCheck() )
	{
		if( !m_ipAreaFeatureClass || !m_ipAnalyzeFeatureClass || m_lFieldIndex < 0 || strList.IsEmpty() )
		{
			::MessageBox( NULL, _T("情報が足りません。"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
			return false;
		}
	}
	else
	{
		if( !m_ipAnalyzeFeatureClass || m_lFieldIndex < 0 )
		{
			::MessageBox( NULL, _T("情報が足りません。"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
			return false;
		}
	}

	AnalyzeType eAnalyzeType = (AnalyzeType)GetAnalyzeType();

	esriGeometryType type;
	m_ipAnalyzeFeatureClass->get_ShapeType( &type );

	if( eAnalyzeType == eArea && type != esriGeometryPolygon )
	{
		::MessageBox( NULL, _T("分析するフィーチャクラスがポリゴンの時は、長さの分析はできません。"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
		return false;
	}
	else if ( eAnalyzeType == eLength && type != esriGeometryPolyline )
	{
		::MessageBox( NULL, _T("分析するフィーチャクラスがポリラインの時は、面積の分析はできません。"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
		return false;
	}

	return true;
}

// フォルダの中にあるmdbファイルを見つけてくる
void CAnalyzeSiNDYDataDlg::FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, std::list<CString> &listFileName)
{
	do {
		if (lstrcmp(fd.cFileName, _T(".")) == 0 || lstrcmp(fd.cFileName, _T("..")) == 0) {
			// 何もしない
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			CString cstrPath;
			cstrPath.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrFind = cstrPath + CString(_T("\\*"));

			WIN32_FIND_DATA fd2;
			HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
			FindFile(hFind2, fd2, cstrPath, listFileName);
			FindClose(hFind2);

		} else {
			CString cstrName;
			cstrName.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrExtension = PathFindExtension(cstrName);
			if (cstrExtension == CString(_T(".mdb")))
				listFileName.push_back(cstrName);
		}
	} while (FindNextFile(hFind, &fd));
}

// フィーチャカーソルの中身を分析する関数
void CAnalyzeSiNDYDataDlg::AnalyzeFeatureCursor(IFeatureCursorPtr ipFeatureCursor, long lRow, CString strRow, CString strQuery, map< CString, CString > mapDomain, CString strRowName )
{
	// 1個しかないはず
	IFeaturePtr ipFeature;
	while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
	{
		// 一行追加
		m_spAnalyzeGrid->AddItem( _T("") );
		m_spAnalyzeGrid->put_TextMatrix( lRow+1, 0, _bstr_t( strRow ) );

		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		map< CString, double > mapAnalyze;
		AnalyzeData( ipGeom, strQuery, mapAnalyze, strRowName, true );
		OutputGrid( mapAnalyze, mapDomain, lRow );
	}
}

// フィーチャを分析する関数
void CAnalyzeSiNDYDataDlg::AnalyzeData( IGeometryPtr ipAreaGeom, CString strQuery, map< CString, double >& mapAnalyze, CString strRowName, bool bIntersect )
{
	m_cProgressDlg.SendMessage( WM_PROGRESSRANGE, 2, 100 );

	AnalyzeType eAnalyzeType = (AnalyzeType)GetAnalyzeType();
	long lFeatureNum = 0;
	m_ipAnalyzeFeatureClass->FeatureCount (  AheInitQueryFilter( AheInitSpatialFilter( NULL, ipAreaGeom ), NULL, strQuery ), &lFeatureNum );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipAnalyzeFeatureClass->Search(  AheInitQueryFilter( AheInitSpatialFilter( NULL, ipAreaGeom ), NULL, strQuery ), VARIANT_FALSE, &ipFeatureCursor );

	long lCount = 0, lDispPercent = 0;
	IFeaturePtr ipFeature;
	while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
	{
		CComVariant vaValue;
		ipFeature->get_Value( m_lFieldIndex, &vaValue );
		CString strFieldVal;
		switch( vaValue.vt )
		{
		case VT_I4: strFieldVal.Format( _T("%ld"), vaValue.lVal );break;
		default: break;
		}
		double dVal = mapAnalyze[ strFieldVal ];

		switch(eAnalyzeType)
		{
		case eArea:
			{
				// 面積にしかつかわないので、ShapeCopyを使わないで少し高速化
				IGeometryPtr ipArea;
				ipFeature->get_Shape( &ipArea );
				if( bIntersect )
				{
					// この形状は、指定したエリアからはみ出していることがあるので、探索形状でIntersectする必要がある
					IGeometryPtr ipIntersectArea;
					((ITopologicalOperatorPtr)ipArea)->Intersect( ipAreaGeom, esriGeometry2Dimension, &ipIntersectArea );
					ipArea = ipIntersectArea;
				}

				// NULLっちゃう場合も考えられる
				if( ipArea )
				{
					// 面積求める（完璧に正確ではない）
					double dArea = 0;
					((IAreaPtr)ipArea)->get_Area(&dArea);
					IPointPtr ipPoint(CLSID_Point);
					((IAreaPtr)ipArea)->QueryCentroid(ipPoint);
					double dY = 0;
					ipPoint->get_Y(&dY);
					// 現在の緯度と赤道との割合を求める
					double dRatio = EARTH_R * sin(M_PI / 2.0 - dY * M_PI / 180.0) * 2.0 * M_PI/360.0;
					dArea *= fabs(dRatio * EARTH_R * 2.0 * M_PI / 360.0);
					dArea *= 1000000.0;

					mapAnalyze[ strFieldVal ] = dVal + dArea;
				}
				break;
			}
		case eLength:
			{
				IGeometryPtr ipPolyline;
				ipFeature->get_Shape( &ipPolyline );

				// TODO:　ポリゴンでポリラインを分割する必要がある
				double dLength = 0.0;
				IGeometryCollectionPtr ipGeometryCollection(ipPolyline);
				long geometryCount;
				ipGeometryCollection->get_GeometryCount(&geometryCount);
				for (int i = 0; i < geometryCount; i++)
				{
					IGeometryPtr ipPart;
					ipGeometryCollection->get_Geometry(i, &ipPart);
					IPointCollectionPtr ipPointCollection(ipPart);
					long pointCount;
					ipPointCollection->get_PointCount(&pointCount);
					WKSPoint *points = new WKSPoint [pointCount];
					ipPointCollection->QueryWKSPoints(0, pointCount, points);
					for (int j = 1; j < pointCount; j++)
					{
						double d = g_cCrdCnv.GetDist(points[j-1].X, points[j-1].Y, points[j].X, points[j].Y);
						dLength += d;
					}
					break;
				}
				mapAnalyze[ strFieldVal ] = dVal + dLength;
				break;
			}
		case eNum:
			{
				mapAnalyze[ strFieldVal ] = dVal + 1;
				break;
			}
		default: break;
		}
		if( lCount * 100 / lFeatureNum > lDispPercent )
		{
			CString strProgress;
			strProgress.Format( _T("%s：%sを分析中...(%ld%%)"), m_strSearchFieldName, strRowName, lDispPercent );
			m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 2, (LPARAM)(LPCTSTR)(strProgress) );
			m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 2,lDispPercent );
			lDispPercent = lDispPercent + 5;
		}
		++lCount;
	}
	CString strProgress;
	strProgress.Format( _T("%s：%s分析完了"), m_strSearchFieldName, strRowName );
	m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 2, (LPARAM)(LPCTSTR)(strProgress) );
	m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 2,100 );
}

// グリッドに分析データを出力する関数
void CAnalyzeSiNDYDataDlg::OutputGrid( const map< CString, double > &mapAnalyze, const map< CString, CString >& mapDomain, long lRow )
{
	long dTotal = 0;
	for( map< CString, double >::const_iterator itRes = mapAnalyze.begin(); itRes != mapAnalyze.end(); ++itRes )
	{
		dTotal += (long)itRes->second;
		map< CString, CString >::const_iterator it = mapDomain.find( itRes->first );
		if( it != mapDomain.end() )
		{
			CString strColVal = it->second;
			long lCol = FindCol( strColVal );
			CString strAnalyzeData;
			strAnalyzeData.Format( _T("%ld"), (long)itRes->second );
			m_spAnalyzeGrid->put_TextMatrix( lRow+1,  lCol, _bstr_t( strAnalyzeData ) );
		}
	}
	// 値がないところはなんかかっこ悪いので、0で埋めておく
	long lColNum = 0;
	m_spAnalyzeGrid->get_Cols( &lColNum );
	for( int i = 0; i < lColNum; ++i )
	{
		CComBSTR bstrName;
		m_spAnalyzeGrid->get_TextMatrix( lRow+1, i, &bstrName );
		if( (CString)bstrName == _T("") )
			m_spAnalyzeGrid->put_TextMatrix( lRow+1, i, _T("0") );
	}

	CString strTotal;
	strTotal.Format( _T("%ld"), (long)dTotal );
	m_spAnalyzeGrid->put_TextMatrix( lRow+1,  lColNum-1, _bstr_t( strTotal ) );
}
