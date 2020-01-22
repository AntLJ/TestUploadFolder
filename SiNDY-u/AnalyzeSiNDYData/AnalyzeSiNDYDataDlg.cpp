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

// �i���_�C�A���O���o�����߂̃X���b�h
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

// �E�B���h�E�}�l�[�W���z�u�}�b�v
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

	AtlAdviseSinkMap(this, true);// �C�x���g����

	// �E�B���h�E�}�l�[�W���ɂ��Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	// �`���[�g
	m_wndChart.SubclassWindow(GetDlgItem(IDC_MSCHART));

	// �}��
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
	// TIPS: ��������ƃR���{�{�b�N�X����������͉����I�H
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
	ShowWindow(SW_HIDE);				// ���s���́A�ݒ�_�C�A���O��\��

	CButton cButtonServer( GetDlgItem( IDC_RADIO_SERVER ) );

	CEdit cEdit( GetDlgItem( IDC_EDIT_LIST ) );
	CString strList;
	cEdit.GetWindowText( strList );

	// �N�G���擾
	CEdit cEditQ( GetDlgItem( IDC_EDIT_QUERY ) );
	CString strQuery;
	cEditQ.GetWindowText( strQuery );

	// ���͂ł��邾���̏�񂪑����Ă��邩�`�F�b�N
	if( CheckAnalyzeInfo() )
	{
		// �X���b�h�쐬
		_beginthread( ThreadFunc, 0, (LPVOID)m_hWnd );
		Sleep(1000); // �����҂��Ă��
		ShowWindow( SW_HIDE );
		// ���Ԍv��
		clock_t startTime, endTime;
		startTime = clock();

		CString strProgress;
		long lRow = 0;

		// �T�[�o�i�t�@�C���j�^�C�v�̕���
		if( cButtonServer.GetCheck() )
		{
			list< CString > listSearchArea;
			if( CreateSearchArea( strList, listSearchArea ) )
			{
				// ����쐬
				map< CString, CString > mapDomain;
				if( CreateGridCol( mapDomain ) )
				{
					m_cProgressDlg.SendMessage( WM_PROGRESSRANGE, 1, listSearchArea.size() );

					for( list< CString >::iterator it = listSearchArea.begin(); it != listSearchArea.end(); ++it, ++lRow )
					{
						strProgress.Format( _T("%s�F%s�𕪐͊J�n ( %ld / %ld ����)"), m_strSearchFieldName, *it, lRow+1, listSearchArea.size() );
						m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow );
						m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 1, (LPARAM)(LPCTSTR)(strProgress) );

						// �N�G���������񂩐������𔻒�
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

						strProgress.Format( _T("%s�F%s�𕪐͊��� ( %ld / %ld ����)"), m_strSearchFieldName, *it, lRow+1, listSearchArea.size() );
						m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow+1 );
					}
				}
			}
		}
		// �t�H���_�^�C�v�̕���
		else
		{
			// ����쐬
			map< CString, CString > mapDomain;
			if( CreateGridCol( mapDomain ) )
			{
				// �t�B�[�`���N���X���擾
				CComboBox cCombo( GetDlgItem( IDC_COMBO_FEATURECLASS ) );
				CString strCurSelText;
				cCombo.GetLBText(cCombo.GetCurSel(),strCurSelText.GetBuffer(256));
				strCurSelText.ReleaseBuffer();

				m_cProgressDlg.SendMessage( WM_PROGRESSRANGE, 1, m_listPGDBName.size() );

				for( list< CString >::const_iterator it = m_listPGDBName.begin(); it != m_listPGDBName.end(); ++it, ++lRow )
				{
					// ������
					m_ipAnalyzeFeatureClass = NULL;
					// �v���O���X�ݒ�
					strProgress.Format( _T("%s�𕪐͊J�n ( %ld / %ld ����) "), *it, lRow+1, m_listPGDBName.size() );
					m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow );
					m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 1, (LPARAM)(LPCTSTR)(strProgress) );

					// ��s�ǉ�
					CString strName = *it;
					long l = strName.ReverseFind( '\\' );
					strName = strName.Right( strName.GetLength() - l - 1 );
					m_spAnalyzeGrid->AddItem( _T("") );
					m_spAnalyzeGrid->put_TextMatrix( lRow+1, 0, _bstr_t( *it ) );

					// PGDB�I�[�v�����āA�Ώۂ̃t�B�[�`���N���X�擾����
					CWorkspace cWorkspace;
					cWorkspace.Connect( *it );

					// �t�B�[�`���N���X�����邩������Ȃ�
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

					// �����Ȃ�������A�f�[�^�Z�b�g���Ɏ��߂��Ă�
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

					strProgress.Format( _T("%s�𕪐͊��� ( %ld / %ld ����) "), *it, lRow+1, m_listPGDBName.size() );
					m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 1,lRow+1 );
				}
			}
		}

		m_spAnalyzeGrid->put_AutoSizeMode( flexAutoSizeColWidth );
		long lCols = 0;
		m_spAnalyzeGrid->get_Cols( &lCols );
		m_spAnalyzeGrid->AutoSize( 0, lCols -1 );


		// �v���O���X�_�C�A���O���郁�b�Z�[�W���s
		m_cProgressDlg.SendMessage(WM_PROGRESSCLOSE, 0, 0 );
		// �����I���̂��߁A���Ԍv��
		endTime = clock();
		long lTime = (endTime - startTime) / (float)CLOCKS_PER_SEC;
		long lHour, lMin, lSec;
		lHour = lTime / 3600;
		lMin = (lTime - 3600*lHour) / 60;
		lSec = (lTime - 3600*lHour - 60*lMin );
		CString strMessage;
		strMessage.Format( _T("���͊���\n%d����%d��%d�b������܂���"),lHour,lMin,lSec );
		MessageBox(strMessage, _T("AnalyzeSiNDYData"), MB_OK );
	}

	SetWindowPos( HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );	// �`�F�b�N�I����ĕ\��(�K���擪��)
	::SetFocus( m_hWnd );	// �t�H�[�J�X�����Ă�

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
			// �t�B�[�`���N���X�ύX������A�t�B�[���h�����Z�b�g����
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

	// �T�[�o������擾
	cEdit.GetWindowText( strConnetion );
	// �R���{�{�b�N�X���g��ɂ���
	cComboF.ResetContent();

	// �t�H���_�ڑ��̏ꍇ�ɂ́A�ŏ��Ɍ�������PGDB�������Ă���
	CButton cButtonServer( GetDlgItem( IDC_RADIO_SERVER ) );
	if( !cButtonServer.GetCheck() )
	{
		// ������
		m_listPGDBName.clear();

		WIN32_FIND_DATA		ffd;
		CString	strDirPath = strConnetion + _T("\\*");
		HANDLE hFile = FindFirstFile( strDirPath, &ffd );

		if ( hFile == INVALID_HANDLE_VALUE )
		{
			CString strErr;
			strErr.Format( _T("Can't open %s\n"), strDirPath );
			::MessageBox( NULL, strErr, _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
			return false;	// �t�@�C�������݂��Ȃ��H
		}
		FindFile( hFile, ffd, strConnetion, m_listPGDBName );
		FindClose(hFile);

		//�ŏ��̂P�����o���āA���̃��[�N�X�y�[�X�Ƃ����Ⴄ
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

	// �t�B�[�`���N���X�����邩������Ȃ�
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
	// ������
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

// �������ʂ��t�@�C���֏o��
LRESULT CAnalyzeSiNDYDataDlg::OnClickedExport(WORD/* wNotifyCode*/, WORD/* wID*/, HWND/* hWndCtl*/, BOOL& /*bHandled*/)
{
	// [���O��t���ĕۑ�]�_�C�A���O
	CFileDialog dlg(FALSE, _T("xls|txt"), _T("����"), OFN_OVERWRITEPROMPT,
		_T("Excel �t�@�C�� (*.xls)\0*.xls\0�^�u��؂� �e�L�X�g�t�@�C��(*.txt)\0*.txt\0���ׂẴt�@�C�� (*.*)\0*.*\0\0"));

	if(dlg.DoModal() == IDOK)
	{
		// �t�@�C�������݂��Ă���A�ǂݎ���p����Ȃ����`�F�b�N
		if( PathFileExists( dlg.m_szFileName ) )
		{
			//[Bug 6182]�����E���v�c�[���Ńt�@�C���o�͎��ɗ�����
			DWORD dAttr = GetFileAttributes( dlg.m_szFileName );
			if( dAttr & FILE_ATTRIBUTE_READONLY )
			{
				MessageBox( _T("�I�������t�@�C���͓ǂݎ���p�ł��B�f�[�^�ۑ��ł��܂���ł���"), _T("�t�@�C���o��"), MB_OK|MB_ICONSTOP );
				return 0;
			}
		}
		// �ǂݎ���p�ȊO�ł������邩������Ȃ��̂ŁA�O�̂���try catch�ň͂��Ƃ�
		try{
			// �g���q��xls�Ȃ�Excel�t�@�C���Ƃ��ĕۑ�
			SaveLoadSettings emLoadSettings = (SaveLoadSettings)(flexFileData|flexFileTabText);
			CString strFileName( dlg.m_szFileName );
			if( 0 == strFileName.Right(4).CompareNoCase(_T(".xls")) )
				emLoadSettings = flexFileExcel;

			// [bug 7721][�v�]]�������ʂ�Excel�`���ŕۑ������ہA�I�����Ă���s�����]�̏�ԂɂȂ�Ȃ��悤�ɂ��ė~����
			m_spAnalyzeGrid->Select(0,0);// �I���s����U�������Ȃ���
			if( FAILED( m_spAnalyzeGrid->SaveGrid( _bstr_t( dlg.m_szFileName ), emLoadSettings, VARIANT_TRUE ) ) )
				MessageBox( _T("�f�[�^�̕ۑ��Ɏ��s���܂����B"), _T("�t�@�C���o��"), MB_OK|MB_ICONSTOP );
		}
		catch(...)
		{
			MessageBox( _T("�Ȃ�炩�̗��R�ɂ��A�f�[�^�̕ۑ��Ɏ��s���܂����B"), _T("�t�@�C���o��"), MB_OK|MB_ICONSTOP );
		}
	}
	return 0;
}

LRESULT CAnalyzeSiNDYDataDlg::OnClickedServer(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// �t�H���_�^�C�v�̏ꍇ�ɂ́A�E������disable�ɂ��Ă���
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

// ���X�g����A�T���G���A���X�g�𒊏o
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
		// �擪�͕K���t�B�[���h���Ƃ�������
		if( i == 0 )
		{
			m_strSearchFieldName = (CString)strList.c_str();
			// �t�B�[���h�������񂩂ǂ���
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

// �O���b�h�̐擪�s���쐬����
bool CAnalyzeSiNDYDataDlg::CreateGridCol( map<CString, CString >& mapDomain )
{
	CComboBox cCombo = GetDlgItem( IDC_COMBO_FIELD );
	CString strCurSelText;
	cCombo.GetLBText(cCombo.GetCurSel(),strCurSelText.GetBuffer(256));
	strCurSelText.ReleaseBuffer();

	// �`���[�g�N���A
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
		// �R�[�h�l�h���C������Ȃ��ꍇ�ɁA�K���ȃT���v���������Ă���Ƃ���
		// ���Ƃ�����Ă������ق�������

	}

	if( mapDomain.size() < 500 )
	{
		m_spAnalyzeGrid->put_Redraw( flexRDNone );	// �C�x���g�}�~
		m_spAnalyzeGrid->put_Rows(0);//������
		m_spAnalyzeGrid->put_Rows(1);
		m_spAnalyzeGrid->PutFixedRows(1);

		// �`����O���b�h�̏�����
		m_spAnalyzeGrid->put_Cols( mapDomain.size() + 2 );
		if( m_strSearchFieldName.IsEmpty() )
			m_spAnalyzeGrid->put_TextMatrix(0,0,bstr_t( _T("�t�@�C����") ) );
		else
			m_spAnalyzeGrid->put_TextMatrix(0,0,bstr_t( m_strSearchFieldName ) );
		int i = 1;
		for( map<CString, CString>::iterator it = mapDomain.begin(); it != mapDomain.end(); ++it, ++i )
			m_spAnalyzeGrid->put_TextMatrix(0,i,bstr_t( it->second ) );

		m_spAnalyzeGrid->put_TextMatrix( 0, i, _bstr_t( _T("Total") ) );
		m_spAnalyzeGrid->put_Redraw( (RedrawSettings)VARIANT_TRUE );	// �C�x���g�}�~����
	}
	else
		return false;

	return true;

}

// �O���b�h�̐擪�s���當����ƈ�v������̂������Ă���
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

// ���̓^�C�v�𔻒肷��֐�
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

// �`���[�g�����֐��i�����c�[������q�؁j
void CAnalyzeSiNDYDataDlg::CreateChart( long lRow )
{
	if( lRow > 0 )
	{
		// ������
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
			/// �O���t�ɒl���Z�b�g
			COLORREF col = m_wndChart.AddValue( (int)m_spAnalyzeGrid->GetValueMatrix(lRow, lCount) );
			CString sTmp = OLE2T(m_spAnalyzeGrid->GetTextMatrix(0, lCount ) );

			if ( sTmp.IsEmpty())
				sTmp = OLE2T(m_spAnalyzeGrid->GetTextMatrix(lCount, 1));

			m_wndLegend.AddLegend(col, sTmp);
		}
	}
}

// ���͂��邽�߂Ƀf�[�^�������Ă��邩�ǂ������ׂ�֐�
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
			::MessageBox( NULL, _T("��񂪑���܂���B"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
			return false;
		}
	}
	else
	{
		if( !m_ipAnalyzeFeatureClass || m_lFieldIndex < 0 )
		{
			::MessageBox( NULL, _T("��񂪑���܂���B"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
			return false;
		}
	}

	AnalyzeType eAnalyzeType = (AnalyzeType)GetAnalyzeType();

	esriGeometryType type;
	m_ipAnalyzeFeatureClass->get_ShapeType( &type );

	if( eAnalyzeType == eArea && type != esriGeometryPolygon )
	{
		::MessageBox( NULL, _T("���͂���t�B�[�`���N���X���|���S���̎��́A�����̕��͂͂ł��܂���B"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
		return false;
	}
	else if ( eAnalyzeType == eLength && type != esriGeometryPolyline )
	{
		::MessageBox( NULL, _T("���͂���t�B�[�`���N���X���|�����C���̎��́A�ʐς̕��͂͂ł��܂���B"), _T("AnalyzeSiNDYData"), MB_OK|MB_ICONWARNING );
		return false;
	}

	return true;
}

// �t�H���_�̒��ɂ���mdb�t�@�C���������Ă���
void CAnalyzeSiNDYDataDlg::FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, std::list<CString> &listFileName)
{
	do {
		if (lstrcmp(fd.cFileName, _T(".")) == 0 || lstrcmp(fd.cFileName, _T("..")) == 0) {
			// �������Ȃ�
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

// �t�B�[�`���J�[�\���̒��g�𕪐͂���֐�
void CAnalyzeSiNDYDataDlg::AnalyzeFeatureCursor(IFeatureCursorPtr ipFeatureCursor, long lRow, CString strRow, CString strQuery, map< CString, CString > mapDomain, CString strRowName )
{
	// 1�����Ȃ��͂�
	IFeaturePtr ipFeature;
	while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
	{
		// ��s�ǉ�
		m_spAnalyzeGrid->AddItem( _T("") );
		m_spAnalyzeGrid->put_TextMatrix( lRow+1, 0, _bstr_t( strRow ) );

		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		map< CString, double > mapAnalyze;
		AnalyzeData( ipGeom, strQuery, mapAnalyze, strRowName, true );
		OutputGrid( mapAnalyze, mapDomain, lRow );
	}
}

// �t�B�[�`���𕪐͂���֐�
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
				// �ʐςɂ�������Ȃ��̂ŁAShapeCopy���g��Ȃ��ŏ���������
				IGeometryPtr ipArea;
				ipFeature->get_Shape( &ipArea );
				if( bIntersect )
				{
					// ���̌`��́A�w�肵���G���A����͂ݏo���Ă��邱�Ƃ�����̂ŁA�T���`���Intersect����K�v������
					IGeometryPtr ipIntersectArea;
					((ITopologicalOperatorPtr)ipArea)->Intersect( ipAreaGeom, esriGeometry2Dimension, &ipIntersectArea );
					ipArea = ipIntersectArea;
				}

				// NULL�����Ⴄ�ꍇ���l������
				if( ipArea )
				{
					// �ʐϋ��߂�i�����ɐ��m�ł͂Ȃ��j
					double dArea = 0;
					((IAreaPtr)ipArea)->get_Area(&dArea);
					IPointPtr ipPoint(CLSID_Point);
					((IAreaPtr)ipArea)->QueryCentroid(ipPoint);
					double dY = 0;
					ipPoint->get_Y(&dY);
					// ���݂̈ܓx�Ɛԓ��Ƃ̊��������߂�
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

				// TODO:�@�|���S���Ń|�����C���𕪊�����K�v������
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
			strProgress.Format( _T("%s�F%s�𕪐͒�...(%ld%%)"), m_strSearchFieldName, strRowName, lDispPercent );
			m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 2, (LPARAM)(LPCTSTR)(strProgress) );
			m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 2,lDispPercent );
			lDispPercent = lDispPercent + 5;
		}
		++lCount;
	}
	CString strProgress;
	strProgress.Format( _T("%s�F%s���͊���"), m_strSearchFieldName, strRowName );
	m_cProgressDlg.SendMessage( WM_PROGRESSCOMMENT, 2, (LPARAM)(LPCTSTR)(strProgress) );
	m_cProgressDlg.SendMessage( WM_PROGRESSPOS, 2,100 );
}

// �O���b�h�ɕ��̓f�[�^���o�͂���֐�
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
	// �l���Ȃ��Ƃ���͂Ȃ񂩂����������̂ŁA0�Ŗ��߂Ă���
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
