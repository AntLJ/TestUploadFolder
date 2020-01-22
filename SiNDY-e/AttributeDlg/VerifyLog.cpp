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

// VerifyLog.cpp: CVerifyLog �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerifyLog.h"
#include <crd_cnv.h>


CVerifyLog::CVerifyLog()
{
	m_UpdateFiledDomain		= NULL;	//!< �X�V���t�B�[���h�h���C��
}

CVerifyLog::~CVerifyLog()
{
	if( m_UpdateFiledDomain )
		m_UpdateFiledDomain->clear();

}
BOOL CVerifyLog::Initialize()
{
	// ���[���̏�����
	if( !m_pISiNDYRuleObj )
		m_pISiNDYRuleObj.CreateInstance( CLSID_SiNDYRuleObj );

	// [Bug4820]���O�C�������ɓ��e�m�F�{�^����2�񉟂��Ɨ�����̂ŁA���O�C�����Ă��邩
	// �ǂ����m�F���Ă���
	VARIANT_BOOL vaLogined = VARIANT_FALSE;
	m_pISiNDYRuleObj->IsLogined( &vaLogined );
	// �I�y���[�^�����擾
	if( vaLogined == VARIANT_TRUE )
	{
		CComBSTR bstrOperator;
		m_pISiNDYRuleObj->get_OperatorName( &bstrOperator );
		m_strOperator = CString( bstrOperator );
	}

	// ���݂̔N�������擾
	::GetLocalTime( &m_SystemTime);

	// �z�X�g�����擾
	if( m_strHostName.GetLength() == 0 ){
		DWORD	nMaxLen = MAX_COMPUTERNAME_LENGTH+1;
		TCHAR	chCompName[MAX_COMPUTERNAME_LENGTH+1];
		if( !::GetComputerName( chCompName, &nMaxLen ) )
			return FALSE;

		chCompName[nMaxLen+1] = NULL;
		m_strHostName = chCompName;
	}

	// SiNDY-e�C���X�g�[���f�B���N�g�����擾
	if( m_strInstallDir.GetLength() == 0 ){
		CInifileHelper cCInifileHelper;
		if( !cCInifileHelper.GetInstallRoot( m_strInstallDir )) 
			return FALSE;
	}
	
	// ���O�i�[�f�B���N�g���̑��݊m�F
	if( m_strLogDirName.GetLength() == 0 ){
		m_strLogDirName.Format( _T("%slog\\"), m_strInstallDir );

		if( !CFile::FileExists( m_strLogDirName ) ){
			if( !::CreateDirectory( m_strLogDirName, NULL ) ){
				m_strLogDirName = _T("");	// �N���A
				return FALSE;
			}
		}
	}

	// �t�@�C�������쐬
	if( m_strLogFileName.GetLength() == 0 )
		m_strLogFileName.Format( _T("%s%s_%s_%d_%02d%02d.log"),
								m_strLogDirName,	// ���O�i�[�f�B���N�g��
								m_strHostName,		// �z�X�g��
								m_strOperator,		// �I�y���[�^��
								m_SystemTime.wYear,	// �m�F�N
								m_SystemTime.wMonth,	// �m�F��
								m_SystemTime.wDay		// �m�F��
								);

	if( !CFile::FileExists( m_strLogFileName ) ){
		// ���݂��Ă��Ȃ��ꍇ�́A�V�K�Ƀt�@�C�����쐬����
		if( !m_VerifyLogFile.Create( m_strLogFileName, GENERIC_READ | GENERIC_WRITE ) ){
			m_strLogFileName = _T("");	// �쐬�ł��Ȃ��ꍇ�̓N���A���Ă���
			return FALSE;
		}
		// ���O�^�C�v����������
		CString strLogType;
		strLogType.Format( _T("#%s\r\n"), _T("VERIFYLOG") );

		m_VerifyLogFile.Write( strLogType );
	}else{
		// ���݂��Ă���ꍇ�͊Y���t�@�C�����I�[�v���ς��`�F�b�N
		if( HANDLE( m_VerifyLogFile ) == INVALID_HANDLE_VALUE ){
			// ���I�[�v���̏ꍇ�̓t�@�C�����I�[�v������
			if( !m_VerifyLogFile.Open( m_strLogFileName, GENERIC_READ | GENERIC_WRITE ) )
				return FALSE;

			// �����t�@�C���̖����ɒǉ����邽�߃t�@�C���|�C���^���t�@�C���̏I���iEOF�j�ɐݒ�B
			m_VerifyLogFile.Seek( 0, FILE_END );
		}
	}
	return TRUE;
}

BOOL CVerifyLog::AppendVerifyLog( IMap* ipMap, _IRowPtr ipRow )
{
	if( !Initialize() )
		return FALSE;

	CString	strLineBuffer;

	// - 1)�`�F�b�N�σt���O
	strLineBuffer += _T("0");	// ���0
	strLineBuffer += DELIMITA;

	// - 2)���C����
	CString strLayerName = AheGetFeatureClassName( ipRow );
	strLineBuffer += strLayerName + DELIMITA;

	// - 3)�I�u�W�F�N�g�h�c
	long nOID;
	ipRow->get_OID( &nOID );
	CString strTemp;
	strTemp.Format( _T("%d"), nOID );
	strLineBuffer += strTemp;
	strLineBuffer += DELIMITA;

	if( m_strPrevLayerName == strLayerName && m_nPrevOID == nOID ){
		AheMessageBox( NULL, AheLoadString( IDS_SAMEITEM ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING|MB_TASKMODAL, WARNLEVEL_NONE );
		// �m�F��Ƃ̕p�x�i�X�V�Ԋu�͂���قǒZ���Ȃ��ł��낤�j�ƃ��O�t�@�C���̏d�v�����疈��N���[�Y�����ق����ǂ����ȁH
		m_VerifyLogFile.Close();
		return FALSE;
	}

	m_strPrevLayerName = strLayerName;
	m_nPrevOID = nOID;

	// ��ʒ��S�ܓx�o�x����2�����b�V���R�[�h�Ɠs�s�n�}���b�V���R�[�h���擾����
	//IMapPtr ipMap( pArcHelper->GetMap() );
	IActiveViewPtr ipActiveView( ipMap );

	IEnvelopePtr ipScreenEnv = NULL;
	ipActiveView->get_Extent( &ipScreenEnv );	// �X�N���[���̋�`�ƕ��E�����擾

	IPointPtr ipPoint = NULL;
	((IAreaPtr)ipScreenEnv)->get_Centroid( &ipPoint );

	DOUBLE dLongitude, dLatitude;
	ipPoint->get_Y( &dLatitude );
	ipPoint->get_X( &dLongitude );

	// ���W�ϊ�
	crd_cnv cnv;
	int nMesh = 0, nX = 0, nY = 0;

	// - 4)2�����b�V���R�[�h
    cnv.LLtoMesh( dLongitude, dLatitude, 2, &nMesh, &nX, &nY );
	strTemp.Format( _T("%d"), nMesh );
	strLineBuffer += strTemp + DELIMITA;

	// - 5)�s�s�n�}���b�V���R�[�h
    cnv.LLtoMesh( dLongitude, dLatitude, 64, &nMesh, &nX, &nY );
	strTemp.Format( _T("%d"), nMesh );
	strLineBuffer += strTemp + DELIMITA;

	// - 6)�m�F��Ǝ��̃��[�U�� 
	// ���[�U�����擾
	strLineBuffer += m_strOperator + DELIMITA;
	
	// - 7)�m�F��Ǝ��̂o�b��
	strLineBuffer += m_strHostName + DELIMITA;

	// - 8)�m�F��Ǝ��̃{�^��������������(2003/06/26 12:00:00)
	strTemp.Format( _T("%d/%02d/%02d %02d:%02d:%02d"), 
						m_SystemTime.wYear, 
						m_SystemTime.wMonth, 
						m_SystemTime.wDay, 
						m_SystemTime.wHour, 
						m_SystemTime.wMinute, 
						m_SystemTime.wSecond );
	strLineBuffer += strTemp + DELIMITA;

	// - 9)�m�F��Ǝ���MXD�t�@�C����
	CComBSTR bstrMXDFile;
	m_pISiNDYRuleObj->get_MXDFileName( &bstrMXDFile );
	strLineBuffer += CString( bstrMXDFile ) + DELIMITA;

	// - 10)�m�F��Ǝ��̃c�[���̃o�[�W����(episode???)
	// [HKEY_LOCAL_MACHINE\SOFTWARE\INCREMENT P CORPORATION\SiNDY-e\Version] "Version"="Release1"
	// ����c�[���o�[�W�������擾
	if( m_strToolVer.GetLength() == 0 ){
		CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\Version" ) );

		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_LOCAL_MACHINE );
		if ( hOpenKey ) {
			TCHAR strToolVersion[_MAX_PATH];

			if( EmxGetRegString( _T("Version"), strToolVersion, _MAX_PATH, hOpenKey ) == TRUE )
				m_strToolVer = CString( strToolVersion ) + DELIMITA;
			else
				m_strToolVer += DELIMITA;

			RegCloseKey( hOpenKey );
		}
	}
	strLineBuffer += m_strToolVer;

	// - 11)�ŏI�ύX���̃��[�U��
	CComVariant vTemp = AheGetAttribute( ipRow, OPERATOR_FIELD );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 12)�ŏI�ύX���̍�ƖړI
	vTemp = AheGetAttribute( ipRow, PURPOSE_C_FIELD );
	CComBSTR bstrName;
	m_pISiNDYRuleObj->GetOperatingPurposeFromID( vTemp.lVal, &bstrName );
	strLineBuffer +=  CString( bstrName ) + DELIMITA;

	// - 13)�ŏI�ύX����
	vTemp = AheGetAttribute( ipRow, MODIFYDATE_FIELD );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 14)�ŏI�ύX���̕ύX���e
	vTemp = AheGetAttribute( ipRow, UPDATETYPE_CFIELD );

	if( m_UpdateFiledDomain == NULL ){
		IFieldsPtr ipFields;
		ipRow->get_Fields( &ipFields );
		long lFiledIndex;
		ipFields->FindField( CComBSTR( UPDATETYPE_CFIELD ), &lFiledIndex );

		IFieldPtr ipField;
		ipFields->get_Field( lFiledIndex, &ipField );

		// �t�B�[���h�����擾
		m_UpdateFiled.SetFieldDef( strLayerName, UPDATETYPE_CFIELD );	
		m_UpdateFiled.SetRowBuffer( lFiledIndex, ipField, ipRow );

		m_UpdateFiledDomain = m_UpdateFiled.GetDomain();
	}
	for( DOMAINMAP::iterator l = m_UpdateFiledDomain->begin(); l != m_UpdateFiledDomain->end(); l++ ) {
		if( l->code == vTemp ) {
			strTemp = l->code_discription;
			break;
		}
	}

	strLineBuffer += strTemp + DELIMITA;

	// - 15)�v���O�����ŏI�ύX����
	vTemp = AheGetAttribute( ipRow, _T("PROGMODIFYDATE") );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 16)�v���O�����ŏI�ύX���̃v���O������
	vTemp = AheGetAttribute( ipRow, _T("MODIFYPROGNAME") );
	vTemp.ChangeType( VT_BSTR );
	strLineBuffer +=  CString( vTemp.bstrVal ) + DELIMITA;

	// - 17)���̑��R�����g�s
	strLineBuffer += _T("\r\n");

	// �m�F���e����������
	m_VerifyLogFile.Write( strLineBuffer );

	// �m�F��Ƃ̕p�x�i�X�V�Ԋu�͂���قǒZ���Ȃ��ł��낤�j�ƃ��O�t�@�C���̏d�v�����疈��N���[�Y�����ق����ǂ����ȁH
	m_VerifyLogFile.Close();


	return TRUE;
}
