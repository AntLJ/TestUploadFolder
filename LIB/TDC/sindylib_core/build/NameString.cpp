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
 * @file NameString.cpp
 * @brief <b>�I�u�W�F�N�g���̃N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G\n
 * $Id$
 */
#include "stdafx.h"
#include "NameString.h"
#include "ErrorCode.h"
#include "sindymacroutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

CWorkspaceNameString::CWorkspaceNameString()
{
	m_bIsSDE = false;
}

CWorkspaceNameString::~CWorkspaceNameString()
{
}

bool CWorkspaceNameString::operator <( const CWorkspaceNameString& obj ) const
{
	if( m_strVersionName < obj.m_strVersionName &&
		m_strServerName < obj.m_strServerName &&
		m_strInstanceName < obj.m_strInstanceName )
		return true;
	return false;
}

bool CWorkspaceNameString::SetName( IUnknown* ipUnk )
{
	bool bRet = false;

	IPropertySetPtr ipProp( ipUnk );
HAS_PROP:
	if( NULL != ipProp )
	{
#ifdef _DEBUG
		// �v���p�e�B�g���[�X
		long lCount = 0;
		ipProp->get_Count( &lCount );
		CComVariant vaNames, vaValues;
		ipProp->GetAllProperties( &vaNames, &vaValues );

		if( ( (VT_ARRAY|VT_BSTR) & vaNames.vt ) && ( (VT_ARRAY|VT_VARIANT) & vaValues.vt ) )
		{
			SAFEARRAY* pSaName = vaNames.parray, *pSaValue = vaValues.parray;
			BSTR* pNameArray = NULL;
			VARIANT* pValueArray = NULL;
			if( SUCCEEDED( SafeArrayAccessData( pSaName, (void**)&pNameArray ) ) &&
				SUCCEEDED( SafeArrayAccessData( pSaValue, (void**)&pValueArray ) ) )
			{
				for( long i = 0; i < lCount; ++i )
				{
					USES_CONVERSION;
					CComVariant vaValue = pValueArray[i];
					vaValue.ChangeType( VT_BSTR );
					ATLTRACE(_T("PROP[%ld] : NAME[%s] VALUE[%s]\n"), i, OLE2T(pNameArray[i]), OLE2T(vaValue.bstrVal) );
				}
			}
			SafeArrayUnaccessData( pSaName );
			SafeArrayUnaccessData( pSaValue );
		}

#endif // ifdef _DEBUG
		CComVariant vaUser, vaServer, vaInstance, vaVersion, vaPath, vaDBConnectionProperties;
		ipProp->GetProperty( CComBSTR(L"SERVER"), &vaServer );
		ipProp->GetProperty( CComBSTR(L"USER"), &vaUser );
		ipProp->GetProperty( CComBSTR(L"INSTANCE"), &vaInstance );
		ipProp->GetProperty( CComBSTR(L"VERSION"), &vaVersion );
		ipProp->GetProperty( CComBSTR(L"DATABASE"), &vaPath );
		ipProp->GetProperty( CComBSTR(L"DB_CONNECTION_PROPERTIES"), &vaDBConnectionProperties );

		if( VT_BSTR == vaUser.vt ) { m_strUserName = vaUser.bstrVal;	m_strUserName.MakeUpper();	} // �啶������
		if( VT_BSTR == vaVersion.vt ) m_strVersionName = vaVersion.bstrVal;
		if( VT_BSTR == vaPath.vt ) m_strConnectString = vaPath.bstrVal;

		if( VT_BSTR == vaDBConnectionProperties.vt && 0 < wcslen( vaDBConnectionProperties.bstrVal ) )
		{ // DC�ڑ��̏ꍇ
			m_strServerName = vaDBConnectionProperties.bstrVal;
			m_strServerName.MakeLower(); // ����������

			if( VT_BSTR == vaInstance.vt ) m_strInstanceName = _T("5151");

			m_bIsSDE = true;
			m_strConnectString.Format(_T("%s[%s]@%s:%s"), m_strUserName, m_strVersionName, m_strServerName, m_strInstanceName );
		}
		else{ // DC�ڑ��ȊO�̏ꍇ
			if( VT_BSTR == vaServer.vt ){ m_strServerName = vaServer.bstrVal; m_strServerName.MakeLower(); }	// ����������
			if( VT_BSTR == vaInstance.vt ) m_strInstanceName = vaInstance.bstrVal;

			if( VT_BSTR == vaUser.vt     && 0 < wcslen( vaUser.bstrVal ) &&
				VT_BSTR == vaServer.vt   && 0 < wcslen( vaServer.bstrVal ) &&
				VT_BSTR == vaInstance.vt && 0 < wcslen( vaInstance.bstrVal ) &&
				VT_BSTR == vaVersion.vt  && 0 < wcslen( vaVersion.bstrVal ) )
			{
				m_bIsSDE = true;
				m_strConnectString.Format(_T("%s[%s]@%s:%s"), m_strUserName, m_strVersionName, m_strServerName, m_strInstanceName );
			}
			else
				m_bIsSDE = false;
		}

		bRet = true;
	}
	else {
		IWorkspacePtr ipWork( ipUnk );
HAS_WORKSPACE:
		if( NULL != ipWork )
		{
			ipWork->get_ConnectionProperties( &ipProp );
			LOGASSERTEM_IF( NULL != ipProp, sindyErr_COMFunctionFailed, _T("PropertySet�̎擾�Ɏ��s") )
				goto HAS_PROP;
		}
		else {
			IDatasetPtr ipDataset( ipUnk );
HAS_DATASET:
			if( NULL != ipDataset )
			{
				ipDataset->get_Workspace( &ipWork );
				LOGASSERTE_IF( NULL != ipWork, sindyErr_AOGetWorkspaceFailed )
					goto HAS_WORKSPACE;
			}
			else {
				_IRowPtr ipRow( ipUnk );
				LOGASSERTEM_IF( NULL != ipRow, sindyErr_ArgLimitOver, _T("�T�|�[�g�ς݂̃C���^�[�t�F�[�X�������Ȃ��I�u�W�F�N�g�̂��߁A���O���͎擾�ł��܂���B") )
				{
					ITablePtr ipTable;
					ipRow->get_Table( &ipTable );
					ipDataset = ipTable;
					LOGASSERTE_IF( NULL != ipDataset, sindyErr_COMFunctionFailed )
						goto HAS_DATASET;
				}
			}
		}
	}

	return bRet;
}

// �����\�[�X���ǂ����𔻒f����
bool CWorkspaceNameString::IsSameSource( const CWorkspaceNameString& cNameString ) const
{
	if( IsSDE() && cNameString.IsSDE() )
	{
		if( 0 == lstrcmpi( GetServerName(), cNameString.GetServerName() ) &&
			0 == lstrcmpi( GetInstanceName(), cNameString.GetInstanceName() ) &&
			0 == lstrcmpi( GetVersionName(), cNameString.GetVersionName() ) )
			return true;
	}
	else if( ! ( IsSDE() && cNameString.IsSDE() ) )
	{
		if( 0 == lstrcmpi( GetConnectString(), cNameString.GetConnectString() ) )
			return true;
	}
	return false;
}

CTableNameString::CTableNameString() : CWorkspaceNameString()
{
}

CTableNameString::~CTableNameString()
{
}

bool CTableNameString::operator<( const CTableNameString& obj ) const
{
	if( CWorkspaceNameString::operator<( obj ) &&
		m_strOwnerTableName < obj.m_strOwnerTableName )
		return true;
	return false;
}

bool CTableNameString::SetName( IUnknown* ipUnk )
{
	bool bRet = false;

	IDatasetPtr ipDataset( ipUnk );
HAS_DATASET:
	if( ipDataset )
	{
		CComBSTR bstrName;
		LOGASSERTE_IF( SUCCEEDED( ipDataset->get_Name( &bstrName ) ), sindyErr_COMFunctionFailed )
		{
			CString strName = bstrName;
			strName.MakeUpper();	// �啶������

			int nPeriodIndex = strName.Find( _T('.'), 0 );

			if( 0 > nPeriodIndex )
			{
				m_strOwnerName = _T("");
				m_strTableName = strName;
				m_strOwnerTableName = strName;
			}
			else {
				m_strOwnerName = strName.Left( nPeriodIndex );
				m_strTableName = strName.Right( strName.GetLength() - nPeriodIndex - 1 );
				m_strOwnerTableName = m_strOwnerName + _T(".") + m_strTableName;
			}

			// �G�C���A�X
			IObjectClassPtr ipObjectClass( ipDataset );
			if( ipObjectClass )
			{
				LOGASSERTE_IF( SUCCEEDED( ipObjectClass->get_AliasName( &bstrName ) ), sindyErr_COMFunctionFailed )
				{
					m_strAliasName = bstrName;
				}
			}

			bRet = true;
		}
	}
	else {
		_IRowPtr ipRow( ipUnk );
		LOGASSERTEM_IF( ipRow, sindyErr_ArgLimitOver, _T("�T�|�[�g�ς݂̃C���^�[�t�F�[�X�������Ȃ��I�u�W�F�N�g�̂��߁A���O���͎擾�ł��܂���B") )
		if( ipRow )
		{
			ITablePtr ipTable;
			ipRow->get_Table( &ipTable );
			ipDataset = ipTable;
			LOGASSERTE_IF( ipDataset, sindyErr_COMFunctionFailed )
				goto HAS_DATASET;
		}
	}

	if( bRet ) 
		bRet = CWorkspaceNameString::SetName( ipUnk );

	return bRet;
}

// �����\�[�X���ǂ����𔻒f����
bool CTableNameString::IsSameSource( const CTableNameString& cNameString ) const
{
	if( CWorkspaceNameString::operator==( cNameString ) )
	{
		bool bHasOwnerNameBase = HasOwnerName();
		bool bHasOwnerNameTest = cNameString.HasOwnerName();
		if( ! ( bHasOwnerNameBase && bHasOwnerNameTest ) ) // PGDB�̏ꍇ
			return true;
		else if( bHasOwnerNameBase && bHasOwnerNameTest )
			return ( 0 == lstrcmpi( GetOwnerName(), cNameString.GetOwnerName() ) );
	}
	return false;
}

} // sindy
