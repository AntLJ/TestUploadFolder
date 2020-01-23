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

// Workspaces.cpp: CWorkspaces �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vector>
#include "util.h"
#include "SiNDYeSettings.h"
#include "Workspaces.h"
#include "../LIB/WinLib/CStringTokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace annotation_class;
using namespace errorcode;


// ���������΍��p�֐�
bool CWorkspaces::PreConnect()
{
	IWorkspacePtr ipWorkspace = 0;
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);
	if (ipWorkspaceFactory->OpenFromFile(CComBSTR(_T("C:\\")), 0, &ipWorkspace) != S_OK)
		return false;
	return true;
}

// �f�[�^�x�[�X�ɐڑ�����
CWorkspace* CWorkspaces::Connect( LPCTSTR lpcszConnectString )
{
	// �����`�F�b�N
	if( lpcszConnectString == NULL )
	{
		_ASSERTE( lpcszConnectString != NULL );
		return NULL;
	}

	// ���ɐڑ��ς݂̃��[�N�X�y�[�X��
	if (! IsPGDBFileName(lpcszConnectString))
	{
		CStringTokenizer cStrToken(CString(lpcszConnectString), CString(_T('/')));
		if (cStrToken.CountTokens() == 5)
		{
			CString strUserName(cStrToken.GetNextToken());
			CString strPassword(cStrToken.GetNextToken());
			CString strVersion(cStrToken.GetNextToken());
			CString strInstance(cStrToken.GetNextToken());
			CString strServerName(cStrToken.GetNextToken());
			const CWorkspace *p = FindWorkspace(strUserName, strVersion, strServerName);
			if (p) return const_cast<CWorkspace *>(p);
		}
	}

	// ���X�g�ɒǉ����Ă���ڑ�
	m_listWorkspace.push_front( CWorkspace() );
	std::list<CWorkspace>::iterator it = m_listWorkspace.begin();
	if( it->Connect( lpcszConnectString ) )
		return &(*it);
	else {
		// �������ǉ��������͍̂폜
		m_listWorkspace.erase( it );
		return NULL;
	}
}

// SDE�f�[�^�x�[�X�ɐڑ�����
CWorkspace* CWorkspaces::ConnectToSDE( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName )
{
	// �����`�F�b�N
	if( lpcszUserName == NULL || lpcszPassword == NULL || lpcszVersion == NULL || lpcszInstance == NULL || lpcszServerName == NULL )
	{
		_ASSERTE( lpcszUserName != NULL && lpcszPassword != NULL && lpcszVersion != NULL && lpcszInstance != NULL && lpcszServerName != NULL );
		return NULL;
	}

	// ���ɐڑ��ς݂̃��[�N�X�y�[�X��
	const CWorkspace *p = FindWorkspace(lpcszUserName, lpcszVersion, lpcszServerName);
	if (p) return const_cast<CWorkspace *>(p);

	// ���X�g�ɒǉ����Ă���ڑ�
	m_listWorkspace.push_front( CWorkspace() );
	std::list<CWorkspace>::iterator it = m_listWorkspace.begin();
	if( it->ConnectToSDE( lpcszUserName, lpcszPassword, lpcszVersion, lpcszInstance, lpcszServerName ) )
		return &(*it);
	else {
		// �������ǉ��������͍̂폜
		m_listWorkspace.erase( it );
		return NULL;
	}
}

// �p�[�\�i���E�W�I�f�[�^�x�[�X�ɐڑ�����
CWorkspace* CWorkspaces::ConnectToPGDB( LPCTSTR lpcszPGDBFileName )
{
	// �����`�F�b�N
	if( lpcszPGDBFileName == NULL )
	{
		_ASSERTE( lpcszPGDBFileName != NULL );
		return NULL;
	}

	// ���X�g�ɒǉ����Ă���ڑ�
	m_listWorkspace.push_front( CWorkspace() );
	std::list<CWorkspace>::iterator it = m_listWorkspace.begin();
	if( it->ConnectToPGDB( lpcszPGDBFileName ) )
		return &(*it);
	else {
		// �������ǉ��������͍̂폜
		m_listWorkspace.erase( it );
		return NULL;
	}
}

// �����̐ڑ���ǉ�����
sindyErrCode CWorkspaces::AddConnection( IWorkspacePtr ipWorkspace )
{
	// �����`�F�b�N
	if( ipWorkspace == NULL ) return sindyErr_COMInterfaceIsNull;

	if( FindWorkspace( ipWorkspace ) == NULL )
	{
		m_listWorkspace.push_back( CWorkspace(ipWorkspace) );
	}

	return sindyErr_NoErr;
}

// �����̐ڑ���T��
const CWorkspace* CWorkspaces::FindWorkspace( LPCTSTR lpcszUserName,  LPCTSTR lpcszVersion, LPCTSTR lpcszServerName ) const
{
	std::list<CWorkspace>::const_iterator it = m_listWorkspace.end();
	for( it = m_listWorkspace.begin(); it != m_listWorkspace.end(); ++it )
	{
		if( ( lstrcmpi( it->GetNameString()->GetServerName(), lpcszServerName ) == 0 ) &&
			( lstrcmpi( it->GetNameString()->GetUserName(), lpcszUserName ) == 0 ) &&
			( lstrcmpi( it->GetNameString()->GetVersionName(), lpcszVersion) == 0 ) )
			break;
	}
	if( it != m_listWorkspace.end() )
		return &(*it);
	else
		return NULL;
}

// �����̐ڑ���T��
const CWorkspace* CWorkspaces::FindWorkspace( IWorkspacePtr ipWorkspace ) const
{
	if( ipWorkspace == NULL )
		return NULL;

	CWorkspace cWorkspace( ipWorkspace );
	return FindWorkspace( cWorkspace.GetNameString()->GetUserName(), cWorkspace.GetNameString()->GetVersionName(), cWorkspace.GetNameString()->GetServerName() );
}

const CWorkspace* CWorkspaces::FindWorkspaceByServerName( LPCTSTR lpcszServerName ) const
{
	std::list<CWorkspace>::const_iterator it = m_listWorkspace.end();
	for( it = m_listWorkspace.begin(); it != m_listWorkspace.end(); ++it )
	{
		if( lstrcmpi( it->GetNameString()->GetServerName(), lpcszServerName ) == 0 )
			break;
	}
	if( it != m_listWorkspace.end() )
		return &(*it);
	else
		return NULL;
}

// �p�[�\�i���E�W�I�f�[�^�x�[�X�����ǂ����𔻒f����
bool CWorkspaces::IsPGDBFileName( LPCTSTR lpcszString ) const
{
	// �����`�F�b�N
	if( lpcszString == NULL )
	{
		_ASSERTE( lpcszString != NULL );
		return false;
	}

	// .mdb�ŏI����Ă��邩�ǂ����Ŕ��f����
	CString strString( lpcszString );
	if( strString.Right( 4 ).CompareNoCase( _T(".mdb") ) == 0 )
		return true;

	return false;
}

/*void CWorkspaces::GetConnectionInfo( IWorkspacePtr ipWorkspace, DB_CONNECTION_INFO& stConnectionInfo ) const
{
	// �����`�F�b�N
	if( ipWorkspace == NULL )
	{
		_ASSERTE( ipWorkspace != NULL );
		return;
	}
	// �Ԃ�l������
	stConnectionInfo.ConnectString.Empty();
	stConnectionInfo.Instance.Empty();
	stConnectionInfo.IsPGDB = false;
	stConnectionInfo.Password.Empty();
	stConnectionInfo.ServerName.Empty();
	stConnectionInfo.UserName.Empty();
	stConnectionInfo.Version.Empty();

	// ���[�N�X�y�[�X����ڑ������擾
	IPropertySetPtr ipPropSet;
	ipWorkspace->get_ConnectionProperties( &ipPropSet );

	CComVariant vaUserName, vaPassword, vaVersion, vaInstance, vaServerName;
	if( ipPropSet != NULL )
	{
		ipPropSet->GetProperty( L"USER", &vaUserName );
		ipPropSet->GetProperty( L"PASSWORD", &vaPassword );
		ipPropSet->GetProperty( L"SERVER", &vaServerName );
		ipPropSet->GetProperty( L"INSTANCE", &vaInstance );
		ipPropSet->GetProperty( L"VERSION", &vaVersion );
	}

	vaUserName.ChangeType( VT_BSTR );
	vaPassword.ChangeType( VT_BSTR );
	vaVersion.ChangeType( VT_BSTR );
	vaInstance.ChangeType( VT_BSTR );
	vaServerName.ChangeType( VT_BSTR );

	stConnectionInfo.UserName = vaUserName.bstrVal;
	stConnectionInfo.Password = vaPassword.bstrVal;
	stConnectionInfo.Version = vaVersion.bstrVal;
	stConnectionInfo.Instance = vaInstance.bstrVal;
	stConnectionInfo.ServerName = vaServerName.bstrVal;
}*/

/*// ���L���[���e�[�u���N���X�̎Q�Ƃ�Ԃ�
CAnnotationClassTable& CWorkspaces::GetAnnotationClassTable()
{
	if( m_cAnnotationClassTable == NULL )
	{
		// ���[���p�T�[�o�����擾����
		CSiNDYeSettings cSettings;
		cSettings.Load();	//!< �G���[�ł��ǂ�

		// ���ɂ��̃T�[�o���ł̐ڑ������邩�ǂ������`�F�b�N����
		const CWorkspace* pWorkspace = FindWorkspaceByServerName( cSettings.GetServerName() );
		if( pWorkspace == NULL )
		{
			// ���[���f�[�^�x�[�X�ɐڑ�����
			if( cSettings.GetUsePGDB() )
				pWorkspace = ConnectToPGDB( cSettings.GetPGDBFileName() );
			else
				pWorkspace = ConnectToSDE( _T("ronly"), _T("ronly"), _T("SDE.DEFAULT"), _T("5151"), cSettings.GetServerName() );
		}
		// ���[���n��REFERENCE��t����iSDE�̂݁j
		if( cSettings.GetUsePGDB() )
			m_cAnnotationClassTable = pWorkspace->OpenTable( sindy::annotation_class::kTableName );
		else
			m_cAnnotationClassTable = pWorkspace->OpenTable( CString(_T("REFERENCE.")) + sindy::annotation_class::kTableName );
	}

	if( m_cAnnotationClassTable == NULL )
		_ASSERTE( m_cAnnotationClassTable != NULL );

	return m_cAnnotationClassTable;
}*/

// ���[���n�̃��[�N�X�y�[�X�ɐڑ�����
const CWorkspace* CWorkspaces::ConnectToRuleWorkspace()
{
	// ���[���p�T�[�o�����擾����
	CSiNDYeSettings cSettings;
	cSettings.Load();	//!< �G���[�ł��ǂ�

	// ���ɂ��̃T�[�o���ł̐ڑ������邩�ǂ������`�F�b�N����
	const CWorkspace* pWorkspace = FindWorkspaceByServerName( cSettings.GetServerName() );
	if( pWorkspace == NULL )
	{
		// ���[���f�[�^�x�[�X�ɐڑ�����
		if( cSettings.GetUsePGDB() )
			pWorkspace = ConnectToPGDB( cSettings.GetPGDBFileName() );
		else
			pWorkspace = ConnectToSDE( _T("ronly"), _T("ronly"), _T("SDE.DEFAULT"), _T("5151"), cSettings.GetServerName() );
	}

	return pWorkspace;
}

} // sindy
