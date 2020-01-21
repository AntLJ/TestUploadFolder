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
* @file InifileHelper.cpp
*
* @brief IniFile関連を操作するクラス群のインプリメンテーション
*
* @author 古川 貴宏
*
* @date   2001/12/10
*/
#include "stdafx.h"
#include "InifileHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_NO_INI_FILE_IN_REGISTRY             = _T("レジストリにINIファイル名が入っていません");
	LPCTSTR FAIL_TO_ACQUIRE_REGISTRY                  = _T("レジストリ取得失敗");
	LPCTSTR ERROR_NO_INSTALL_PATH_IN_REGISTRY         = _T("レジストリにインストールパスが入っていません");
	LPCTSTR ERROR_NO_SDE_SERVER_NAME_IN_INI           = _T("INIファイルにSDEサーバ名が入っていません");
	LPCTSTR READING_INI                               = _T("INIファイル読み込み");
	LPCTSTR ERROR_NO_RASTER_FILE_PATH_IN_INI          = _T("INIファイルにラスタファイル設定ファイルのパスが入っていません");
	LPCTSTR ERROR_NO_WORK_PURPOSE_FILE_PATH_IN_INI    = _T("INIファイルに作業目的ファイルのパスが入っていません");
	LPCTSTR ERROR_NO_CHANGING_SYMBOL_FILE_PATH_IN_INI = _T("INIファイルにシンボル変更用ファイルのパスが入っていません");
	LPCTSTR ERROR_NO_ADDING_LAYER_PATH_IN_INI         = _T("INIファイルにレイヤ追加コマンド用ファイルのパスが入っていません");
#else
	LPCTSTR ERROR_NO_INI_FILE_IN_REGISTRY             = _T("There is no ini file name in the registry.");
	LPCTSTR FAIL_TO_ACQUIRE_REGISTRY                  = _T("Failed to acquire registry.");
	LPCTSTR ERROR_NO_INSTALL_PATH_IN_REGISTRY         = _T("There is no install path in the registry.");
	LPCTSTR ERROR_NO_SDE_SERVER_NAME_IN_INI           = _T("There is no SDE server name in the INI file.");
	LPCTSTR READING_INI                               = _T("Reading INI file");
	LPCTSTR ERROR_NO_RASTER_FILE_PATH_IN_INI          = _T("There is no file path for setting raster in the INI file.");
	LPCTSTR ERROR_NO_WORK_PURPOSE_FILE_PATH_IN_INI    = _T("There is no file path of work purpose in the INI file.");
	LPCTSTR ERROR_NO_CHANGING_SYMBOL_FILE_PATH_IN_INI = _T("There is no file path for changing symbol in the INI file.");
	LPCTSTR ERROR_NO_ADDING_LAYER_PATH_IN_INI         = _T("There is no file path for adding layer in the INI file.");
#endif // SINDY_FOR_ENGLISH
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
/// コンストラクタ
CInifileHelper::CInifileHelper() :	m_hKey(NULL),
									m_strInifilePath(_T(""))
{
	/// 初期化
	Initialize();
}
/// デストラクタ
CInifileHelper::~CInifileHelper()
{
	if( m_hKey != NULL )
		::RegCloseKey( m_hKey );

	m_hKey = NULL;
}

//////////////////////////////////////////////////////////////////////
// パブリック関数
//////////////////////////////////////////////////////////////////////
/// ユーザのワーニングレベル取得
int CInifileHelper::GetWarningLevel( void)
{
	return GetRegInt( INSTALLROOT_KEY, LEVEL_KEY, -1);
}

// INIファイルのファイル名をフルパスで取得
BOOL CInifileHelper::GetInifilePath( CString& strInifilePath )
{
	CString cStrInitFileName;	//!< INIファイル名読み込みバッファ

	/// SiNDY-eのインストールルートのパス取得
	if( GetInstallRoot( strInifilePath ) != TRUE ) return FALSE;

	/// SiNDY-eのINIファイル名取得
	GetRegString( INSTALLROOT_KEY, INIFILEPATH_KEY, DEFAULT_KEY, cStrInitFileName.GetBuffer( MAX_PATH), MAX_PATH );
	cStrInitFileName.ReleaseBuffer();
	
	if( lstrcmp( cStrInitFileName, DEFAULT_KEY ) == 0 ) {
		/// ファイル名取得失敗
		::MessageBox( NULL, ERROR_NO_INI_FILE_IN_REGISTRY, FAIL_TO_ACQUIRE_REGISTRY, MB_OK );
		return FALSE;
	}

	/// つなげる
	strInifilePath += cStrInitFileName;

	return TRUE;
}

// SiNDY-eのインストールルートパスを取得
BOOL CInifileHelper::GetInstallRoot( CString& strInstallRoot )
{
	CString cStrInstallPath;		//!< インストールルートパス読み込みバッファ

	/// SiNDY-eのインストールルートのパス取得
	GetRegString( INSTALLROOT_KEY, PATH_KEY, DEFAULT_KEY, cStrInstallPath.GetBuffer( MAX_PATH), MAX_PATH );
	cStrInstallPath.ReleaseBuffer();
	
	if( lstrcmp( cStrInstallPath, DEFAULT_KEY ) == 0 ) {
		/// パス取得失敗
		::MessageBox( NULL, ERROR_NO_INSTALL_PATH_IN_REGISTRY, FAIL_TO_ACQUIRE_REGISTRY, MB_OK );
		return FALSE;
	}

	if( TEXT( '\\') != cStrInstallPath.Right( 1))
	{
		cStrInstallPath += TEXT( '\\');
	}
	strInstallRoot = cStrInstallPath;

	return TRUE;
}

// データベースサーバ名をINIファイルから取得
BOOL CInifileHelper::GetSDEServerName( CString& strDBServerName )
{
	CString cStrReadBuffer; //!< 読み込みバッファ

	/// 作業目的ファイルのパスをINIファイルから読み込み
	GetPrivateProfileString(_T("workdb"), _T("servername"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// 失敗
		::MessageBox( NULL, ERROR_NO_SDE_SERVER_NAME_IN_INI, READING_INI, MB_OK );
		return FALSE;
	}

	/// コピー
	strDBServerName = cStrReadBuffer;

	return TRUE;
}

// ラスタファイル定義ファイルへのアクセスヘルパクラスを初期化
BOOL CInifileHelper::GetRasterfile( CRasterInifileHelper& cRasterfileHelper)
{
	CString cStrReadBuffer; //!< 読み込みバッファ

	/// ラスタファイル設定ファイルのパスをINIファイルから読み込み
	GetPrivateProfileString(_T("rasterfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// 失敗
		::MessageBox( NULL, ERROR_NO_RASTER_FILE_PATH_IN_INI, READING_INI, MB_OK );
		return FALSE;
	}

	/// 取得できたパスを渡す
	cRasterfileHelper.SetPrivateProfilePath( cStrReadBuffer);

	return TRUE;
}

// ラスタファイル定義ファイルのパスを取得
BOOL CInifileHelper::GetRasterfilePath( CString& cRasterfileHelper )
{
	CString cStrReadBuffer; //!< 読み込みバッファ

	/// 作業目的ファイルのパスをINIファイルから読み込み
	GetPrivateProfileString(_T("rasterfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// 失敗
		::MessageBox( NULL, ERROR_NO_WORK_PURPOSE_FILE_PATH_IN_INI, READING_INI, MB_OK );
		return FALSE;
	}

	/// コピー
	cRasterfileHelper = cStrReadBuffer;

	return TRUE;
}

// シンボル変更用定義ファイルのパスを取得
BOOL CInifileHelper::GetSymbolfilePath( CString& strSymbolfilePath)
{
	CString cStrReadBuffer; //!< 読み込みバッファ

	/// 作業目的ファイルのパスをINIファイルから読み込み
	GetPrivateProfileString(_T("symbolfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// 失敗
		::MessageBox( NULL, ERROR_NO_CHANGING_SYMBOL_FILE_PATH_IN_INI, READING_INI, MB_OK );

		return FALSE;
	}

	/// 取得できたパスを渡す
	strSymbolfilePath = cStrReadBuffer;

	return TRUE;
}

// シンボル変更用定義ファイルのパスを取得
BOOL CInifileHelper::GetAddLayerfilePath( CString& strAddLayerPath)
{
	CString cStrReadBuffer; //!< 読み込みバッファ

	/// 作業目的ファイルのパスをINIファイルから読み込み
	GetPrivateProfileString(_T("addlayerfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// 失敗
		::MessageBox( NULL, ERROR_NO_ADDING_LAYER_PATH_IN_INI, READING_INI, MB_OK );

		return FALSE;
	}

	/// 取得できたパスを渡す
	strAddLayerPath = cStrReadBuffer;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// プライベート関数
//////////////////////////////////////////////////////////////////////
/// イニシャライズ
void CInifileHelper::Initialize()
{
	CString strWorkfilePath;
	/// 大元のキーを設定
	SetRegistryKey( INSTALL_KEY );
	/// INIファイルパス取得
	GetInifilePath( m_strInifilePath );
}
/////////////////////////////////////////////////////////////////////////////
//  ルートキーの設定

void CInifileHelper::SetRegistryKey( LPCTSTR lpszRegistryKey )
{
    CString cStrOpenRegistry = SOFTWARE_KEY;

    cStrOpenRegistry += lpszRegistryKey;
    m_hKey = EmxCreateRegKey( cStrOpenRegistry, ROOT_KEY );
}
/////////////////////////////////////////////////////////////////////////////
//  int値の書き込み

BOOL CInifileHelper::WriteRegInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue )
{
    HKEY  hOpenKey;
    
    if( m_hKey )
    {
        hOpenKey = EmxCreateRegKey( lpszSection, m_hKey);
        if( hOpenKey )
            return EmxSetRegDword( lpszEntry, nValue, hOpenKey );
    }
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//  UINT値の取得

UINT CInifileHelper::GetRegInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault )
{
    DWORD  uResult = nDefault;
    HKEY  hOpenKey;
    
    if( m_hKey )
    {
        hOpenKey = EmxCreateRegKey( lpszSection, m_hKey );
        if( hOpenKey )
        {
            if( !EmxGetRegDword( lpszEntry, &uResult, hOpenKey ) )
                uResult = nDefault;
        }
    }
    return uResult;
}
/////////////////////////////////////////////////////////////////////////////
//  文字列の書き込み

BOOL CInifileHelper::WriteRegString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue )
{
    HKEY  hOpenKey;
    
    if( m_hKey )
    {
        hOpenKey = EmxCreateRegKey( lpszSection, m_hKey );
        if( hOpenKey )
            return EmxSetRegString( lpszEntry, lpszValue, hOpenKey );
    }
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//  文字列の取得

void  CInifileHelper::GetRegString( LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, 
                    LPTSTR lpReturnedString, DWORD nSize )
{
    HKEY  hOpenKey;
    
    if( m_hKey )
    {
        hOpenKey = EmxCreateRegKey( lpszSection, m_hKey );

		BOOL bRet;
        if( hOpenKey )
        {
            bRet = EmxGetRegString( lpszEntry, lpReturnedString, nSize, hOpenKey );
			::RegCloseKey( hOpenKey );	// 必要なくなった時点で必ずクローズすること（しないとメモリリーク）

			if( bRet == TRUE ) return;
        }
    }
    lstrcpy( lpReturnedString, lpszDefault );
}
