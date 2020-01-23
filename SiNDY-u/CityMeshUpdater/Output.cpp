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

// WinLib/atlwfile.h���
BOOL COutput::WriteFile( CString strWriteBuff )
{
#ifdef	UNICODE
	// �������ݕ�����̃o�C�g�����擾
	int result = ::WideCharToMultiByte(CP_ACP, 0, LPCTSTR(strWriteBuff), -1, NULL, 0, NULL, NULL);

	// SJIS�ւ̕ϊ��p�o�b�t�@�A���P�[�g
	char* chFileName = new char [result+1];
	::ZeroMemory( chFileName, result+1 );

	// UNICODE����SJIS�֕ϊ�
	result = ::WideCharToMultiByte(CP_ACP, 0, LPCTSTR(strWriteBuff), -1, chFileName, result+1, NULL, NULL);

	// �ϊ��o�C�g����1�ȏ�̏ꍇ�͏�������
	if (result > 0){
		m_cFile.Write( chFileName, strlen( chFileName ) );
		delete [] chFileName;
		return TRUE;
	}
	delete [] chFileName;
	return FALSE;
#else
	// �m�F���e����������
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

	// ���[�U@�T�[�o
	CString strConnectInfo = GetWSProperty( ipDstWS ? ipDstWS : ipDstEditWS, CComBSTR(_T("USER") ) );
	strConnectInfo += _T("@");
	strConnectInfo += GetWSProperty( ipDstWS ? ipDstWS : ipDstEditWS, CComBSTR(_T("DB_CONNECTION_PROPERTIES") ) );
	strConnectInfo += _T("\r\n");

	if( ipSrcWS )
	{
		strConnectInfo += _T("�X�V���o�[�W�����F");
		strConnectInfo += GetWSProperty( ipSrcWS, CComBSTR(_T("VERSION") ) );
		strConnectInfo += _T("\r\n");
	}

	if( ipDstWS )
	{
		strConnectInfo += _T("�X�V��o�[�W�����F");
		strConnectInfo += GetWSProperty( ipDstWS, CComBSTR(_T("VERSION") ) );
		strConnectInfo += _T("\r\n");
	}

	strConnectInfo += _T("���X�V��o�[�W�����F");
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
		WriteFile( _T("CityMesh�X�V�c�[��\r\n") );
		m_bIsInitFile = true;
	}
}
