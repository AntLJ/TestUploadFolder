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
#include "Output.h"

COutput::COutput(void)
{
	m_bIsInitFile = false;
}

COutput::~COutput(void)
{
	m_cFile.Close();
}

// WinLib/atlwfile.hより
BOOL COutput::WriteFile( CString strWriteBuff )
{
#ifdef	UNICODE
	// 書き込み文字列のバイト数を取得
	int result = ::WideCharToMultiByte(CP_ACP, 0, LPCTSTR(strWriteBuff), -1, NULL, 0, NULL, NULL);

	// SJISへの変換用バッファアロケート
	char* chFileName = new char [result+1];
	::ZeroMemory( chFileName, result+1 );

	// UNICODEからSJISへ変換
	result = ::WideCharToMultiByte(CP_ACP, 0, LPCTSTR(strWriteBuff), -1, chFileName, result+1, NULL, NULL);

	// 変換バイト数が1以上の場合は書き込み
	if (result > 0){
		m_cFile.Write( chFileName, strlen( chFileName ) );
		delete [] chFileName;
		return TRUE;
	}
	delete [] chFileName;
	return FALSE;
#else
	// 確認内容を書き込む
	if( !m_cFile.Write(  LPCTSTR(strWriteBuff), strWriteBuff.GetLength() ) ) 
		return FALSE;

	return TRUE;
#endif
}

void COutput::OutputMsg( LPCTSTR msg, bool bIsWriteLog /*= true*/ )
{
	_tprintf( msg );

	if( bIsWriteLog && m_bIsInitFile )
		WriteFile( msg );
}

bool COutput::SetDBInfo( IWorkspace* ipSrcWS, IWorkspace* ipDstWS, IWorkspace* ipDstEditWS )
{
	//if( !ipSrcWS || !ipDstWS )
	//	return false;

	InitFile( GetWSProperty( ipDstWS ? ipDstWS : ipDstEditWS, CComBSTR(_T("VERSION") ) ) );

	// ユーザ@サーバ
	CString strConnectInfo = GetWSProperty( ipDstWS ? ipDstWS : ipDstEditWS, CComBSTR(_T("USER") ) );
	strConnectInfo += _T("@");
	strConnectInfo += GetWSProperty( ipDstWS ? ipDstWS : ipDstEditWS, CComBSTR(_T("DB_CONNECTION_PROPERTIES") ) );
	strConnectInfo += _T("\r\n");

	if( ipSrcWS )
	{
		strConnectInfo += _T("更新元バージョン：");
		strConnectInfo += GetWSProperty( ipSrcWS, CComBSTR(_T("VERSION") ) );
		strConnectInfo += _T("\r\n");
	}

	if( ipDstWS )
	{
		strConnectInfo += _T("更新先バージョン：");
		strConnectInfo += GetWSProperty( ipDstWS, CComBSTR(_T("VERSION") ) );
		strConnectInfo += _T("\r\n");
	}

	strConnectInfo += _T("実更新先バージョン：");
	strConnectInfo += GetWSProperty( ipDstEditWS, CComBSTR(_T("VERSION") ) );
	strConnectInfo += _T("\r\n\r\n");

	OutputMsg( strConnectInfo );

	return true;
}

CString COutput::GetWSProperty( IWorkspace * ipWorkspace, LPCTSTR lpszPropName )
{
	CString strRet;
	if( ipWorkspace )
	{
		IPropertySetPtr ipPropertySet;
		if( SUCCEEDED( ipWorkspace->get_ConnectionProperties( &ipPropertySet ) ) )
		{
			CComVariant vaVal;
			ipPropertySet->GetProperty( CComBSTR(lpszPropName), &vaVal );
			strRet = vaVal;
		}
	}
	return strRet;
}

void COutput::InitFile( LPCTSTR lpszFileName )
{
	CString strFileName;

	SYSTEMTIME stTime;
	::GetLocalTime( &stTime );

	strFileName.Format( _T("%s_%d%02d%02d.log"), lpszFileName, stTime.wYear, stTime.wMonth, stTime.wDay );

	if( SUCCEEDED( m_cFile.Create( strFileName, GENERIC_WRITE, FILE_SHARE_READ, OPEN_ALWAYS ) ) )
	{
		WriteFile( _T("CityMesh更新ツール\r\n") );
		m_bIsInitFile = true;
	}
}
