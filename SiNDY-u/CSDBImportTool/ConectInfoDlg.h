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
// ConectInfoDlg.h : Declaration of the CConectInfoDlg

#pragma once

#include "resource.h"       // main symbols
#include <atlddx.h>
#include <atlctrls.h>		// プログレスバー使うため
// CConectInfoDlg

class CConectInfoDlg : 
	public CDialogImpl<CConectInfoDlg>,
	public CWinDataExchange<CConectInfoDlg>	
{
public:
	CConectInfoDlg();
	~CConectInfoDlg();
	CConectInfoDlg( LPCTSTR lpcszIniFilePath );
	void Init( LPCTSTR lpcszIniFilePath );
	enum { IDD = IDD_CONNECTINFODLG };

    BEGIN_MSG_MAP(CConectInfoDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
        COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
		COMMAND_HANDLER(IDC_BUTTON_SELECTFILE, BN_CLICKED, OnButtonSelectfileBnClicked)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CConectInfoDlg)
		DDX_TEXT(IDC_EDIT_SERVERNAME, m_strServername)
		DDX_TEXT(IDC_EDIT_USERNAME, m_strUsername)
		DDX_TEXT(IDC_EDIT_PASSWORD, m_strPassword)
		DDX_TEXT(IDC_EDIT_VERSION, m_strVersion)
		DDX_TEXT(IDC_EDIT_FEATURECLASS, m_strFeatureclass)
		DDX_TEXT(IDC_EDIT_SRCFILE, m_strSrcfile)
		DDX_TEXT(IDC_EDIT_TABLE, m_strTablename)
		DDX_RADIO(IDC_RADIO_CSDB, m_nCSDBOrNAVI)
		DDX_TEXT(IDC_EDIT_LOGFILE, m_strLogFile)
		DDX_TEXT(IDC_EDIT_SEETNAME, m_strExcelSeet)
	END_DDX_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnButtonSelectfileBnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	bool Connect();
	const CString& GetSrcFilePath(){ return m_strSrcfile; };
	const CString& GetLogFilePath(){ return m_strLogFile; };
	const CString& GetSheetName(){ return m_strExcelSeet; };
	int ImportType(){ return m_nCSDBOrNAVI; }
	void SetProgressMax( long lMax );
	bool CheckValue();
	ITablePtr GetTable(){ return m_ipTable; }
	IFeatureClassPtr GetFC(){ return m_ipFeatureClass; }
	CString GetFCAliass(){
		if( m_strFCAlias.IsEmpty() )
		{
			CComBSTR bstrAlias;
			GetFC()->get_AliasName( &bstrAlias );
			m_strFCAlias = bstrAlias;
		}
		return m_strFCAlias;
	}

	long AttTblIndex(){ return m_lAttTblIndex; }
	long IDIndex(){ return m_lIDIndex; }

	IWorkspacePtr m_ipWorkspace;

private:
	int m_nCSDBOrNAVI;				// CSDB/NAVIラジオボタン用　0：CSDB 1：NAVI
	CString	m_strServername,
			m_strUsername,
			m_strPassword,
			m_strVersion,
			m_strDataset,
			m_strFeatureclass,
			m_strSrcfile,
			m_strLogFile,
			m_strTablename,
			m_strExcelSeet;

	ITablePtr m_ipTable;
	IFeatureClassPtr m_ipFeatureClass;
	long m_lAttTblIndex, m_lIDIndex;
	CString m_strFCAlias;

};


