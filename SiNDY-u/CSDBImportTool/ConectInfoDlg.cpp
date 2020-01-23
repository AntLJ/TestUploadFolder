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

// This file was generated by WTL Dialog wizard 
// ConectInfoDlg.cpp : Implementation of CConectInfoDlg

#include "stdafx.h"
#include "ConectInfoDlg.h"
#include "globalfunc.h"
#include "DBUtility.h"
#include "atldlgs.h"

// CConectInfoDlg
CConectInfoDlg::CConectInfoDlg()
{
}

CConectInfoDlg::~CConectInfoDlg()
{
}

CConectInfoDlg::CConectInfoDlg( LPCTSTR lpcszIniFilePath )
{
	Init(lpcszIniFilePath);
}

void CConectInfoDlg::Init( LPCTSTR lpcszIniFilePath )
{
	m_strUsername = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("User") );
	m_strPassword = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Pass") );
	m_strServername = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Server") );
	m_strVersion = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Version") );
	m_strFeatureclass = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Featureclass") );
	m_strTablename = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Table") );
	m_strSrcfile = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Src") );
	m_strLogFile = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Output") );
	m_strExcelSeet = gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Sheet") );
	m_nCSDBOrNAVI = ( (gf::GetIniFileValue( lpcszIniFilePath, _T("INIT"), _T("Type") )).CompareNoCase( _T("NAVI") ) == 0 ) ? 1 : 0;
	m_lAttTblIndex = -1;
}

LRESULT CConectInfoDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetWindowText( _T("CSDB/NAVI�C���|�[�g�c�[��") );
	DoDataExchange();
	return 1;  // Let the system set the focus
}

LRESULT CConectInfoDlg::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);
	int n = 0;
	m_strDataset = m_strFeatureclass.Tokenize( _T("/"), n );
	if( m_strDataset != m_strFeatureclass )
		m_strFeatureclass = m_strFeatureclass.Tokenize( _T("/"), n );
	else
		m_strDataset.Empty();

	EndDialog(wID);
	return 0;
}

LRESULT CConectInfoDlg::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

bool CConectInfoDlg::Connect()
{
	m_ipWorkspace = DBUtility::OpenSDEWorkspace( m_strUsername, m_strPassword, m_strVersion, _T("5151"), m_strServername );
	if( !m_ipWorkspace )
	{
			::MessageBox( NULL, _T("���[�N�X�y�[�X���I�[�v���ł��܂���ł����B"), _T(""), MB_OK | MB_ICONERROR );
			return false;
	}

	// �t�B�[���h�쐬�p�̃X�L�[�}��`
	tFieldSchema *tFields = (m_nCSDBOrNAVI == 0) ? g_CSDBFieldDef : g_NAVIFieldDef;
	int nFields = (m_nCSDBOrNAVI == 0) ? CSDBFIELDS : NAVIFIELDS;

	// �t�B�[�`���N���X���J���B
	m_strFeatureclass = m_strUsername + _T(".") + m_strFeatureclass;
	m_ipFeatureClass = DBUtility::OpenFeatureClass( m_ipWorkspace, m_strFeatureclass );

	if( !m_ipFeatureClass )
	{
			CString strErr;
			strErr.Format( _T("%s���I�[�v���ł��܂���ł����B"), m_strFeatureclass );

			::MessageBox( NULL, strErr, _T(""), MB_OK | MB_ICONERROR );
			return false;
	}

	m_ipFeatureClass->FindField( CComBSTR( _T("ATTRTBLID") ), &m_lAttTblIndex );
	m_ipFeatureClass->FindField( CComBSTR( _T("ID") ), &m_lIDIndex );


	// �����e�[�u�����J���B
	m_strTablename = m_strUsername + _T(".") + m_strTablename;
	((IFeatureWorkspacePtr)m_ipWorkspace)->OpenTable( CComBSTR(m_strTablename), &m_ipTable );
	if( !m_ipTable )
	{
		CString strErr;
		strErr.Format( _T("%s���I�[�v���ł��܂���ł����B"), m_strTablename );
		::MessageBox( NULL, strErr, _T(""), MB_OK | MB_ICONERROR );
		return false;
	}


	return true;
}

bool CConectInfoDlg::CheckValue()
{
	if( m_strLogFile.IsEmpty() || m_strFeatureclass.IsEmpty() || m_strPassword.IsEmpty() || m_strServername.IsEmpty() || m_strVersion.IsEmpty() ||  m_strSrcfile.IsEmpty() || m_strUsername.IsEmpty() || m_strExcelSeet.IsEmpty())
	{
		::MessageBox( NULL, _T("���͂���Ă��Ȃ����ڂ�����܂�"), _T(""), MB_OK | MB_ICONWARNING );
		return false;
	}

	if( !PathFileExists( m_strSrcfile ) )
	{
		CString strErrMsg;
		strErrMsg.Format( _T("%s\n��������܂���ł����B"), m_strSrcfile );
		::MessageBox( NULL, strErrMsg, _T(""), MB_OK | MB_ICONWARNING );
		return false;
	}
	return true;
}

void CConectInfoDlg::SetProgressMax( long lMax )
{ 
	HWND hwnd = m_hWnd;
	CProgressBarCtrl cProgress = GetDlgItem( IDC_PROGRESS );
	cProgress.SetRange( 0, lMax );
}



LRESULT CConectInfoDlg::OnButtonSelectfileBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFileDialog dlg(TRUE, _T("xls"), NULL, OFN_HIDEREADONLY | OFN_CREATEPROMPT, _T("Excel �t�@�C�� (*.xls)\0*.xls\0���ׂẴt�@�C�� (*.*)\0*.*\0\0"));

	if(dlg.DoModal() == IDOK)
	{
		m_strSrcfile = dlg.m_szFileName;
		DoDataExchange( FALSE, IDC_EDIT_SRCFILE );
	}

	return 0;
}

