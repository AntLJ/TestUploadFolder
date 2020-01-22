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

#include "stdafx.h"
#include "ToolBase.h"
#include <windows.h>
#include <winver.h>
#include <winutil.h>

using namespace std;
using namespace boost::program_options;

CToolBase::CToolBase() : m_bIsOK( false )
{
}

CToolBase::~CToolBase()
{
	CloseLogFile();
}

// ログファイルオープン
bool CToolBase::OpenLogFile( const CString& strLogFileName )
{
	if( !strLogFileName.IsEmpty() )
	{
		m_fStream.open( strLogFileName, ios::out|ios::app );
		if( m_fStream.is_open() )
		{
			StartLogFile();
			return true;
		}
	}
	WriteLog( _T("#FATAL ログファイルを開けませんでした。 : ") + strLogFileName );
	return false;
}

// ログファイルにログを書き込む
void CToolBase::WriteLog( const CString& strLog, bool bDisp )
{
	if( m_fStream )
	{
		if( m_fStream.is_open() )
			m_fStream << (LPCSTR)(CStringA)strLog << endl;
	}
	if( bDisp )
		cerr << CT2A( strLog ) << endl;
}

void CToolBase::WriteLogHeader()
{
}

// ログファイルクローズ
void CToolBase::CloseLogFile()
{
	if( m_fStream )
	{
		EndLogFile();
		m_fStream.close();
	}
}

// ログファイルにプログラム開始情報を書き込む
void CToolBase::StartLogFile()
{
	WriteLogHeader();
	WriteFileVersion();
	CString strTime;
	strTime.Format( _T("# 開始時刻 %s"), winutil::GetTime() );
	WriteLog( strTime, false );
	WriteOptionString();
}

// ログファイルにプログラム終了情報を書き込む
void CToolBase::EndLogFile()
{
	CString strTime;
	strTime.Format( _T("# 終了時刻 %s"), winutil::GetTime() );
	WriteLog( strTime, false );
	if( m_bIsOK )
		WriteLog( _T("# 正常終了"), false );
	else
		WriteLog( _T("# 異常終了"), false );
}

// ログファイルにファイルバージョンを書き込む
void CToolBase::WriteFileVersion()
{
	CString strVersion, strFile, strProduct;

	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName( NULL, szPath, sizeof(szPath) );

	DWORD dwVersionInfoSize = GetFileVersionInfoSize( const_cast<LPTSTR>(szPath), 0 );
	if( dwVersionInfoSize ) {
		BYTE* pVersionInfo = new BYTE[dwVersionInfoSize];
		GetFileVersionInfo( const_cast<LPTSTR>(szPath), 0, dwVersionInfoSize, pVersionInfo );

		LPVOID lpBuffer;
		UINT uLen;
		if( VerQueryValue( pVersionInfo, _T("\\"), &lpBuffer, &uLen ) ) {
			const VS_FIXEDFILEINFO& rFileInfo = *reinterpret_cast<VS_FIXEDFILEINFO*>(lpBuffer);
			strFile.Format(
				_T("%d.%d.%d.%d"), 
				HIWORD(rFileInfo.dwFileVersionMS), LOWORD(rFileInfo.dwFileVersionMS), 
				HIWORD(rFileInfo.dwFileVersionLS), LOWORD(rFileInfo.dwFileVersionLS) );
			strProduct.Format(
				_T("%d.%d.%d.%d"), 
				HIWORD(rFileInfo.dwProductVersionMS), LOWORD(rFileInfo.dwProductVersionMS), 
				HIWORD(rFileInfo.dwProductVersionLS), LOWORD(rFileInfo.dwProductVersionLS) );
		}

		delete pVersionInfo;
	}

	CString strPath(szPath);
	CString strModule = strPath.Mid( strPath.ReverseFind(_T('\\')) + 1 );
	WriteLog( _T("# ") + strModule + _T(" F:") + strFile + _T(" P:") + strProduct, false );
}

// ログファイルにプログラムオプションを書き込む
void CToolBase::WriteOptionString()
{
	WriteLog( _T("# オプション"), false );

	for( variables_map::const_iterator it = m_variable_map.begin(); it != m_variable_map.end(); ++it )
	{
		string option = it->first;
		variable_value vv = it->second;
		string value = vv.as<string>();
		CString strOption;
		strOption.Format( _T("# --%-10s %s"), CString( option.c_str() ), CString( value.c_str() ) );
		WriteLog( strOption , false );
	}
}
