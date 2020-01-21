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
 * @brief CSiNDYeSettings�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
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

// ���W�X�g����settings.ini�̓��e�������o�ɃL���b�V������
bool CSiNDYeSettings::Load()
{
	CRegKey cKey;
	TCHAR lpszBuffer[_MAX_PATH + 1];
	DWORD dwCount = _MAX_PATH;

	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\InstallRoot") ), KEY_READ  )
	{
		// InstallRoot�i�K�{���ځj
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("Path"), &dwCount ) )
		{
			PathAddBackslash( lpszBuffer ); // �o�b�N�X���b�V���ǉ�
			m_strInstallRoot = lpszBuffer;
		}
		else
			return false;
		// ini�t�@�C���i�K�{���ځj
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("InitFileName"), &dwCount ) )
			m_strInitFileName = lpszBuffer;
		else
			return false;

		cKey.Close();
	}
	else
		return false;

	// ���[��DB�T�[�o���i�K�{���ځj
	dwCount = GetPrivateProfileString( _T("workdb"), _T("servername"), _T(""), lpszBuffer, _MAX_PATH, m_strInstallRoot + m_strInitFileName );
	if( dwCount != _MAX_PATH - 1 && dwCount != _MAX_PATH - 2 )
		m_strServerName = lpszBuffer;
	else
		return false;

	// 臒l
	m_nTolerance = GetPrivateProfileInt( _T("Tools"), _T("Tolerance"), 8, m_strInstallRoot + m_strInitFileName );

	if( ERROR_SUCCESS == cKey.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ), KEY_READ )
	{
		// �ĕ`��̈�̊g����
		DWORD lBuf = 0;
		if( ERROR_SUCCESS == cKey.QueryDWORDValue( _T("RefreshBuffer"), lBuf ) )
			m_nRefreshBuffer = (int)lBuf;

		cKey.Close();
	}

	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Version") ), KEY_READ )
	{
		// �o�[�W����
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("Version"), &dwCount ) )
			m_strVersion = lpszBuffer;

		cKey.Close();
	}

	if( ERROR_SUCCESS == cKey.Open( HKEY_LOCAL_MACHINE, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Warning") ), KEY_READ )
	{
		// �x�����x��
		dwCount = 0;
		if( ERROR_SUCCESS == cKey.QueryValue( dwCount, _T("Level") ) )
			m_lWarnLevel = dwCount;

		cKey.Close();
	}

	if( ERROR_SUCCESS == cKey.Open( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ), KEY_READ )
	{
		// ���[���pPGDB�t�@�C����
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("PGDBFileName"), &dwCount ) )
			m_strPGDBFileName = lpszBuffer;
		// ���[����PGDB���g�p���邩�ǂ���
		dwCount = 0;
		if( ERROR_SUCCESS == cKey.QueryValue( dwCount, _T("UsePGDB") ) )
			m_bUsePGDB = ( dwCount == 0 ) ? false : true;

		// �g���[�X���O�t�@�C����
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("TraceLogFileName"), &dwCount ) )
			m_strTraceLogFileName = lpszBuffer;

		// �G���[���O�t�@�C����
		dwCount = _MAX_PATH;
		if( ERROR_SUCCESS == cKey.QueryValue( lpszBuffer, _T("ErrorLogFileName"), &dwCount ) )
			m_strErrorLogFileName = lpszBuffer;

		cKey.Close();
	}

	if( m_strTraceLogFileName.IsEmpty() ) m_strTraceLogFileName = _T("C:\\Temp\\trace.log");	// GlobalArgument.cpp�ɐݒ肳��Ă�����̂Ɠ����ɂ���
	if( m_strErrorLogFileName.IsEmpty() ) m_strErrorLogFileName = _T("C:\\Temp\\error.log");	// GlobalArgument.cpp�ɐݒ肳��Ă�����̂Ɠ����ɂ���

	return true;
}

// ini�t�@�C���ɂ�����Ă���I��臒l���擾����
int CSiNDYeSettings::GetTolerance() const
{
	// �{���ł����CRule::SetTolerance()������ׂ��Ȃ̂����A�C���^�[�t�F�[�X��ύX����킯�ɂ������Ȃ�
	// �̂ŁA�Ƃ肠�������񒼓ǂ݂��邱�Ƃɂ���B
	// TODO: Ver5�ł̓��\�b�h��ǉ����邱��
	return GetPrivateProfileInt( _T("Tools"), _T("Tolerance"), 8, m_strInstallRoot + m_strInitFileName );
}

// ���[���p�̃p�[�\�i���E�W�I�f�[�^�x�[�X�̃p�X�����W�X�g���ɏ�������
bool CSiNDYeSettings::SetPGDBFileName( LPCTSTR lpcszFileName )
{
	if( lpcszFileName && lstrlen( lpcszFileName ) )
	{
		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == reg.SetValue( lpcszFileName, _T("PGDBFileName") ) )
			{
				m_strPGDBFileName = lpcszFileName;	// �����o�ɃR�s�[
				return true;
			}
		}
	}

	return false;
}

// ���[���f�[�^�x�[�X�Ƀp�[�\�i���E�W�I�f�[�^�x�[�X���g�p���邩�ǂ����̃t���O�����W�X�g���ɏ�������
bool CSiNDYeSettings::SetUsePGDB( bool bUsePGDB )
{
	CRegKey reg;
	if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
	{
		if( ERROR_SUCCESS == reg.SetValue( (DWORD)bUsePGDB, _T("UsePGDB") ) )
		{
			m_bUsePGDB = bUsePGDB;	// �����o�ɃR�s�[
			return true;
		}
	}

	return false;
}

// ini�t�@�C���ɑI��臒l��ݒ肷��
bool CSiNDYeSettings::SetTolerance( int nTolerance )
{
	if( ! ( m_strInstallRoot.IsEmpty() || m_strInitFileName.IsEmpty() ) )
	{
		CString strTolerance;
		strTolerance.Format(_T("%d"), nTolerance);
		if( WritePrivateProfileString( _T("Tools"), _T("Tolerance"), strTolerance, m_strInstallRoot + m_strInitFileName ) )
		{
			m_nTolerance = nTolerance; // �����o�ɃR�s�[
			return true;
		}
	}

	return false;
}

// �g���[�X���O�t�@�C������ݒ肷��
bool CSiNDYeSettings::SetTraceLogFileName( LPCTSTR lpcszFileName )
{
	if( lpcszFileName && 0 < lstrlen( lpcszFileName ) )
	{
		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == reg.SetValue( lpcszFileName, _T("TraceLogFileName") ) )
			{
				m_strTraceLogFileName = lpcszFileName;					// �����o�ɃR�s�[
				return true;
			}
		}
	}

	return false;
}

// �G���[���O�t�@�C������ݒ肷��
bool CSiNDYeSettings::SetErrorLogFileName( LPCTSTR lpcszFileName )
{
	if( lpcszFileName && 0 < lstrlen( lpcszFileName ) )
	{
		CRegKey reg;
		if( ERROR_SUCCESS == reg.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
		{
			if( ERROR_SUCCESS == reg.SetValue( lpcszFileName, _T("ErrorLogFileName") ) )
			{
				m_strErrorLogFileName = lpcszFileName;				// �����o�ɃR�s�[
				return true;
			}
		}
	}

	return false;
}

// �ĕ`��g���̈敝��ݒ�
bool CSiNDYeSettings::SetRefreshBuffer( int nRefreshBuf )
{
	CRegKey cKey;

	if( ERROR_SUCCESS == cKey.Create( HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\COMMON") ) )
	{
		if( ERROR_SUCCESS == cKey.SetDWORDValue( _T("RefreshBuffer"), (DWORD)nRefreshBuf ) )
		{
			m_nRefreshBuffer = nRefreshBuf;			// �����o�ɃR�s�[
			return true;
		}
	}
	return false;
}
