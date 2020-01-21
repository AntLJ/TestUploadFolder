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

/**
 * @file SiNDYeSettings.cpp
 * @brief CSiNDYeSettingsクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "SiNDYeSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSiNDYeSettings::CSiNDYeSettings() : m_lWarnLevel(0L), m_bUsePGDB(false), m_nTolerance(8), m_nRefreshBuffer(10)
{
}

CSiNDYeSettings::~CSiNDYeSettings()
{

}

// レジストリやsettings.iniの内容をメンバにキャッシュする
bool CSiNDYeSettings::Load()
{
	CRegKey cKey;
	TCHAR lpszBuffer[_MAX_PATH + 1];
	DWORD dwCount = _MAX_PATH;

	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\InstallRoot") ), KEY_READ  )
	{
		// InstallRoot（必須項目）
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("Path"), &dwCount ) )
		{
			PathAddBackslash( lpszBuffer ); // バックスラッシュ追加
			m_strInstallRoot = lpszBuffer;
		}
		else
			return false;
		// iniファイル（必須項目）
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("InitFileName"), &dwCount ) )
			m_strInitFileName = lpszBuffer;
		else
			return false;

		cKey.Close();
	}
	else
		return false;

	// ルールDBサーバ名（必須項目）
	dwCount = GetPrivateProfileString( _T("workdb"), _T("servername"), _T(""), lpszBuffer, _MAX_PATH, m_strInstallRoot + m_strInitFileName );
	if( dwCount != _MAX_PATH - 1 && dwCount != _MAX_PATH - 2 )
		m_strServerName = lpszBuffer;
	else
		return false;

	// 閾値
	m_nTolerance = GetPrivateProfileInt( _T("Tools"), _T("Tolerance"), 8, m_strInstallRoot + m_strInitFileName );

	if( ERROR_SUCCESS == cKey.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ), KEY_READ )
	{
		// 再描画領域の拡張幅
		DWORD lBuf = 0;
		if( ERROR_SUCCESS == cKey.QueryDWORDValue( _T("RefreshBuffer"), lBuf ) )
			m_nRefreshBuffer = (int)lBuf;

		cKey.Close();
	}

	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Version") ), KEY_READ )
	{
		// バージョン
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("Version"), &dwCount ) )
			m_strVersion = lpszBuffer;

		cKey.Close();
	}

	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Warning") ), KEY_READ )
	{
		// 警告レベル
		dwCount = 0;
		if( ERROR_SUCCESS == cKey.QueryValue( dwCount, _T("Level") ) )
			m_lWarnLevel = dwCount;

		cKey.Close();
	}

	if( ERROR_SUCCESS == cKey.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ), KEY_READ )
	{
		// ルール用PGDBファイル名
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("PGDBFileName"), &dwCount ) )
			m_strPGDBFileName = lpszBuffer;
		// ルールにPGDBを使用するかどうか
		dwCount = 0;
		if( ERROR_SUCCESS == cKey.QueryValue( dwCount, _T("UsePGDB") ) )
			m_bUsePGDB = ( dwCount == 0 ) ? false : true;

		// トレースログファイル名
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("TraceLogFileName"), &dwCount ) )
			m_strTraceLogFileName = lpszBuffer;

		// エラーログファイル名
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("ErrorLogFileName"), &dwCount ) )
			m_strErrorLogFileName = lpszBuffer;

		cKey.Close();
	}

	if( m_strTraceLogFileName.IsEmpty() ) m_strTraceLogFileName = _T("C:\\Temp\\trace.log");	// GlobalArgument.cppに設定されているものと同じにする
	if( m_strErrorLogFileName.IsEmpty() ) m_strErrorLogFileName = _T("C:\\Temp\\error.log");	// GlobalArgument.cppに設定されているものと同じにする

	return true;
}

// iniファイルにかかれている選択閾値を取得する
int CSiNDYeSettings::GetTolerance() const
{
	// 本来であればCRule::SetTolerance()があるべきなのだが、インターフェースを変更するわけにもいかない
	// ので、とりあえず毎回直読みすることにする。
	// TODO: Ver5ではメソッドを追加すること
	return GetPrivateProfileInt( _T("Tools"), _T("Tolerance"), 8, m_strInstallRoot + m_strInitFileName );
}

// ルール用のパーソナル・ジオデータベースのパスをレジストリに書き込む
bool CSiNDYeSettings::SetPGDBFileName( LPCTSTR lpcszFileName )
{
	if( lpcszFileName && lstrlen( lpcszFileName ) )
	{
		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == reg.SetValue( lpcszFileName, _T("PGDBFileName") ) )
			{
				m_strPGDBFileName = lpcszFileName;	// メンバにコピー
				return true;
			}
		}
	}

	return false;
}

// ルールデータベースにパーソナル・ジオデータベースを使用するかどうかのフラグをレジストリに書き込む
bool CSiNDYeSettings::SetUsePGDB( bool bUsePGDB )
{
	CRegKey reg;
	if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
	{
		if( ERROR_SUCCESS == reg.SetValue( (DWORD)bUsePGDB, _T("UsePGDB") ) )
		{
			m_bUsePGDB = bUsePGDB;	// メンバにコピー
			return true;
		}
	}

	return false;
}

// iniファイルに選択閾値を設定する
bool CSiNDYeSettings::SetTolerance( int nTolerance )
{
	if( ! ( m_strInstallRoot.IsEmpty() || m_strInitFileName.IsEmpty() ) )
	{
		CString strTolerance;
		strTolerance.Format(_T("%d"), nTolerance);
		if( WritePrivateProfileString( _T("Tools"), _T("Tolerance"), strTolerance, m_strInstallRoot + m_strInitFileName ) )
		{
			m_nTolerance = nTolerance; // メンバにコピー
			return true;
		}
	}

	return false;
}

// トレースログファイル名を設定する
bool CSiNDYeSettings::SetTraceLogFileName( LPCTSTR lpcszFileName )
{
	if( lpcszFileName && 0 < lstrlen( lpcszFileName ) )
	{
		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == reg.SetValue( lpcszFileName, _T("TraceLogFileName") ) )
			{
				m_strTraceLogFileName = lpcszFileName;					// メンバにコピー
				return true;
			}
		}
	}

	return false;
}

// エラーログファイル名を設定する
bool CSiNDYeSettings::SetErrorLogFileName( LPCTSTR lpcszFileName )
{
	if( lpcszFileName && 0 < lstrlen( lpcszFileName ) )
	{
		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == reg.SetValue( lpcszFileName, _T("ErrorLogFileName") ) )
			{
				m_strErrorLogFileName = lpcszFileName;				// メンバにコピー
				return true;
			}
		}
	}

	return false;
}

// 再描画拡張領域幅を設定
bool CSiNDYeSettings::SetRefreshBuffer( int nRefreshBuf )
{
	CRegKey cKey;

	if( ERROR_SUCCESS == cKey.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
	{
		if( ERROR_SUCCESS == cKey.SetDWORDValue( _T("RefreshBuffer"), (DWORD)nRefreshBuf ) )
		{
			m_nRefreshBuffer = nRefreshBuf;			// メンバにコピー
			return true;
		}
	}
	return false;
}
