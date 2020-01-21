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
* @brief IniFile�֘A�𑀍삷��N���X�Q�̃C���v�������e�[�V����
*
* @author �Ð� �M�G
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
	LPCTSTR ERROR_NO_INI_FILE_IN_REGISTRY             = _T("���W�X�g����INI�t�@�C�����������Ă��܂���");
	LPCTSTR FAIL_TO_ACQUIRE_REGISTRY                  = _T("���W�X�g���擾���s");
	LPCTSTR ERROR_NO_INSTALL_PATH_IN_REGISTRY         = _T("���W�X�g���ɃC���X�g�[���p�X�������Ă��܂���");
	LPCTSTR ERROR_NO_SDE_SERVER_NAME_IN_INI           = _T("INI�t�@�C����SDE�T�[�o���������Ă��܂���");
	LPCTSTR READING_INI                               = _T("INI�t�@�C���ǂݍ���");
	LPCTSTR ERROR_NO_RASTER_FILE_PATH_IN_INI          = _T("INI�t�@�C���Ƀ��X�^�t�@�C���ݒ�t�@�C���̃p�X�������Ă��܂���");
	LPCTSTR ERROR_NO_WORK_PURPOSE_FILE_PATH_IN_INI    = _T("INI�t�@�C���ɍ�ƖړI�t�@�C���̃p�X�������Ă��܂���");
	LPCTSTR ERROR_NO_CHANGING_SYMBOL_FILE_PATH_IN_INI = _T("INI�t�@�C���ɃV���{���ύX�p�t�@�C���̃p�X�������Ă��܂���");
	LPCTSTR ERROR_NO_ADDING_LAYER_PATH_IN_INI         = _T("INI�t�@�C���Ƀ��C���ǉ��R�}���h�p�t�@�C���̃p�X�������Ă��܂���");
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
// �\�z/����
//////////////////////////////////////////////////////////////////////
/// �R���X�g���N�^
CInifileHelper::CInifileHelper() :	m_hKey(NULL),
									m_strInifilePath(_T(""))
{
	/// ������
	Initialize();
}
/// �f�X�g���N�^
CInifileHelper::~CInifileHelper()
{
	if( m_hKey != NULL )
		::RegCloseKey( m_hKey );

	m_hKey = NULL;
}

//////////////////////////////////////////////////////////////////////
// �p�u���b�N�֐�
//////////////////////////////////////////////////////////////////////
/// ���[�U�̃��[�j���O���x���擾
int CInifileHelper::GetWarningLevel( void)
{
	return GetRegInt( INSTALLROOT_KEY, LEVEL_KEY, -1);
}

// INI�t�@�C���̃t�@�C�������t���p�X�Ŏ擾
BOOL CInifileHelper::GetInifilePath( CString& strInifilePath )
{
	CString cStrInitFileName;	//!< INI�t�@�C�����ǂݍ��݃o�b�t�@

	/// SiNDY-e�̃C���X�g�[�����[�g�̃p�X�擾
	if( GetInstallRoot( strInifilePath ) != TRUE ) return FALSE;

	/// SiNDY-e��INI�t�@�C�����擾
	GetRegString( INSTALLROOT_KEY, INIFILEPATH_KEY, DEFAULT_KEY, cStrInitFileName.GetBuffer( MAX_PATH), MAX_PATH );
	cStrInitFileName.ReleaseBuffer();
	
	if( lstrcmp( cStrInitFileName, DEFAULT_KEY ) == 0 ) {
		/// �t�@�C�����擾���s
		::MessageBox( NULL, ERROR_NO_INI_FILE_IN_REGISTRY, FAIL_TO_ACQUIRE_REGISTRY, MB_OK );
		return FALSE;
	}

	/// �Ȃ���
	strInifilePath += cStrInitFileName;

	return TRUE;
}

// SiNDY-e�̃C���X�g�[�����[�g�p�X���擾
BOOL CInifileHelper::GetInstallRoot( CString& strInstallRoot )
{
	CString cStrInstallPath;		//!< �C���X�g�[�����[�g�p�X�ǂݍ��݃o�b�t�@

	/// SiNDY-e�̃C���X�g�[�����[�g�̃p�X�擾
	GetRegString( INSTALLROOT_KEY, PATH_KEY, DEFAULT_KEY, cStrInstallPath.GetBuffer( MAX_PATH), MAX_PATH );
	cStrInstallPath.ReleaseBuffer();
	
	if( lstrcmp( cStrInstallPath, DEFAULT_KEY ) == 0 ) {
		/// �p�X�擾���s
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

// �f�[�^�x�[�X�T�[�o����INI�t�@�C������擾
BOOL CInifileHelper::GetSDEServerName( CString& strDBServerName )
{
	CString cStrReadBuffer; //!< �ǂݍ��݃o�b�t�@

	/// ��ƖړI�t�@�C���̃p�X��INI�t�@�C������ǂݍ���
	GetPrivateProfileString(_T("workdb"), _T("servername"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// ���s
		::MessageBox( NULL, ERROR_NO_SDE_SERVER_NAME_IN_INI, READING_INI, MB_OK );
		return FALSE;
	}

	/// �R�s�[
	strDBServerName = cStrReadBuffer;

	return TRUE;
}

// ���X�^�t�@�C����`�t�@�C���ւ̃A�N�Z�X�w���p�N���X��������
BOOL CInifileHelper::GetRasterfile( CRasterInifileHelper& cRasterfileHelper)
{
	CString cStrReadBuffer; //!< �ǂݍ��݃o�b�t�@

	/// ���X�^�t�@�C���ݒ�t�@�C���̃p�X��INI�t�@�C������ǂݍ���
	GetPrivateProfileString(_T("rasterfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// ���s
		::MessageBox( NULL, ERROR_NO_RASTER_FILE_PATH_IN_INI, READING_INI, MB_OK );
		return FALSE;
	}

	/// �擾�ł����p�X��n��
	cRasterfileHelper.SetPrivateProfilePath( cStrReadBuffer);

	return TRUE;
}

// ���X�^�t�@�C����`�t�@�C���̃p�X���擾
BOOL CInifileHelper::GetRasterfilePath( CString& cRasterfileHelper )
{
	CString cStrReadBuffer; //!< �ǂݍ��݃o�b�t�@

	/// ��ƖړI�t�@�C���̃p�X��INI�t�@�C������ǂݍ���
	GetPrivateProfileString(_T("rasterfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// ���s
		::MessageBox( NULL, ERROR_NO_WORK_PURPOSE_FILE_PATH_IN_INI, READING_INI, MB_OK );
		return FALSE;
	}

	/// �R�s�[
	cRasterfileHelper = cStrReadBuffer;

	return TRUE;
}

// �V���{���ύX�p��`�t�@�C���̃p�X���擾
BOOL CInifileHelper::GetSymbolfilePath( CString& strSymbolfilePath)
{
	CString cStrReadBuffer; //!< �ǂݍ��݃o�b�t�@

	/// ��ƖړI�t�@�C���̃p�X��INI�t�@�C������ǂݍ���
	GetPrivateProfileString(_T("symbolfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// ���s
		::MessageBox( NULL, ERROR_NO_CHANGING_SYMBOL_FILE_PATH_IN_INI, READING_INI, MB_OK );

		return FALSE;
	}

	/// �擾�ł����p�X��n��
	strSymbolfilePath = cStrReadBuffer;

	return TRUE;
}

// �V���{���ύX�p��`�t�@�C���̃p�X���擾
BOOL CInifileHelper::GetAddLayerfilePath( CString& strAddLayerPath)
{
	CString cStrReadBuffer; //!< �ǂݍ��݃o�b�t�@

	/// ��ƖړI�t�@�C���̃p�X��INI�t�@�C������ǂݍ���
	GetPrivateProfileString(_T("addlayerfile"), _T("filepath"), DEFAULT_KEY, cStrReadBuffer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath );
	cStrReadBuffer.ReleaseBuffer();

	if( lstrcmp( cStrReadBuffer, DEFAULT_KEY ) == 0 ) {
		/// ���s
		::MessageBox( NULL, ERROR_NO_ADDING_LAYER_PATH_IN_INI, READING_INI, MB_OK );

		return FALSE;
	}

	/// �擾�ł����p�X��n��
	strAddLayerPath = cStrReadBuffer;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// �v���C�x�[�g�֐�
//////////////////////////////////////////////////////////////////////
/// �C�j�V�����C�Y
void CInifileHelper::Initialize()
{
	CString strWorkfilePath;
	/// �匳�̃L�[��ݒ�
	SetRegistryKey( INSTALL_KEY );
	/// INI�t�@�C���p�X�擾
	GetInifilePath( m_strInifilePath );
}
/////////////////////////////////////////////////////////////////////////////
//  ���[�g�L�[�̐ݒ�

void CInifileHelper::SetRegistryKey( LPCTSTR lpszRegistryKey )
{
    CString cStrOpenRegistry = SOFTWARE_KEY;

    cStrOpenRegistry += lpszRegistryKey;
    m_hKey = EmxCreateRegKey( cStrOpenRegistry, ROOT_KEY );
}
/////////////////////////////////////////////////////////////////////////////
//  int�l�̏�������

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
//  UINT�l�̎擾

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
//  ������̏�������

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
//  ������̎擾

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
			::RegCloseKey( hOpenKey );	// �K�v�Ȃ��Ȃ������_�ŕK���N���[�Y���邱�Ɓi���Ȃ��ƃ��������[�N�j

			if( bRet == TRUE ) return;
        }
    }
    lstrcpy( lpReturnedString, lpszDefault );
}
