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

// Workspace.cpp: CWorkspace �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Workspace.h"
#include "util.h"
#include "sindycomutil.h"
#include "GlobalFiles.h"
#include "GlobalRules.h"
#include <sindy/workspace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR CONNECT_TO_DATABASE  = _T("Connecting to database:%s ... \n");
	LPCTSTR FAILED_TO_CONNECT    = _T("Failed to connect");
	LPCTSTR SUCCESS_TO_CONNECT   = _T("Successed to connect");
	LPCTSTR SUCCESS              = _T("Success");
	LPCTSTR FAILURE              = _T("Failure");
#else
	LPCTSTR CONNECT_TO_DATABASE  = _T("�f�[�^�x�[�X�ɐڑ����Ă��܂��F%s �c \n");
	LPCTSTR FAILED_TO_CONNECT    = _T("�ڑ��Ɏ��s");
	LPCTSTR SUCCESS_TO_CONNECT   = _T("�ڑ��ɐ���");
	LPCTSTR SUCCESS              = _T("����");
	LPCTSTR FAILURE              = _T("���s");
#endif // SINDY_FOR_ENGLISH
} // namespace

using namespace winutil;
namespace sindy {
using namespace errorcode;

CWorkspace::CWorkspace()
{
}

CWorkspace::~CWorkspace()
{
}

bool CWorkspace::operator ==( const CWorkspace& obj ) const
{
	return ( *GetNameString().get() == *obj.GetNameString().get() );
}

IPropertySetPtr CWorkspace::GetConnectionProperies() const
{
	IPropertySetPtr ipProp;
	INVOKEMETHOD( get_ConnectionProperties( &ipProp ) );
	return ipProp;
}

IEnumDatasetNamePtr CWorkspace::GetDatasetNames( esriDatasetType DatasetType ) const
{
	IEnumDatasetNamePtr ipRet;
	INVOKEMETHOD( get_DatasetNames( DatasetType, &ipRet ) );
	return ipRet;
}

IEnumDatasetPtr CWorkspace::GetDatasets( esriDatasetType DatasetType ) const
{
	IEnumDatasetPtr ipRet;
	INVOKEMETHOD( get_Datasets( DatasetType, &ipRet ) );
	return ipRet;
}

bool CWorkspace::Exists() const
{
	VARIANT_BOOL vbExists = VARIANT_FALSE;
	INVOKEMETHOD( Exists( &vbExists ) );
	return VB2bool(vbExists);
}

bool CWorkspace::IsDirectory() const
{
	VARIANT_BOOL vbDir = VARIANT_FALSE;
	INVOKEMETHOD( IsDirectory( &vbDir ) );
	return VB2bool(vbDir);
}

LPCTSTR CWorkspace::GetPathName( LPTSTR lpszPathName, int nLen ) const
{
	BSTR bstrName = 0;
	INVOKEMETHOD( get_PathName( &bstrName ) );
	if( 0 < SysStringLen(bstrName) )
	{
		LOGASSERTE_IF( BSTR2TCpy( lpszPathName, bstrName, nLen ), sindyErr_BufferIsNotEnough )
			SysFreeString(bstrName);
		return lpszPathName;
	}
	SysFreeString(bstrName);

	return (LPCTSTR)0;
}

esriWorkspaceType CWorkspace::GetType() const
{
	esriWorkspaceType type = (esriWorkspaceType)-1;
	INVOKEMETHOD( get_Type( &type ) );
	return type;
}

IWorkspaceFactoryPtr CWorkspace::GetWorkspaceFactory() const
{
	IWorkspaceFactoryPtr ipRet;
	INVOKEMETHOD( get_WorkspaceFactory( &ipRet ) );
	return ipRet;
}

// ���������΍��p�֐�
bool CWorkspace::PreConnect()
{
	IWorkspacePtr ipWorkspace = 0;
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);
	if (ipWorkspaceFactory->OpenFromFile(CComBSTR(_T("C:\\")), 0, &ipWorkspace) != S_OK)
		return false;
	return true;
}

// �f�[�^�x�[�X�ɐڑ�����
bool CWorkspace::Connect( LPCTSTR lpcszConnectString )
{
	// �����`�F�b�N
	if( lpcszConnectString == NULL )
	{
		_ASSERTE( lpcszConnectString != NULL );
		return false;
	}

	TRACEMESSAGE( CONNECT_TO_DATABASE, lpcszConnectString );

	if( IsOLEDBConnectString( lpcszConnectString ) )
		ConnectToOLEDB( lpcszConnectString );
	else if( IsPGDBFileName( lpcszConnectString ) )
		ConnectToPGDB( lpcszConnectString );
	else {
		int iStart = 0;
		CString strConnectString(lpcszConnectString);
		CString strToken( strConnectString.Tokenize( _T("/"), iStart ) );
		CString strUserName, strPassword, strVersion, strInstance, strServerName;
		for( int i = 0; i < 5; ++i )
		{
			switch( i )
			{
			case 0: strUserName   = strToken; break;
			case 1: strPassword   = strToken; break;
			case 2: strVersion    = strToken; break;
			case 3: strInstance   = strToken; break;
			case 4: strServerName = strToken; break;
			default: _ASSERTE( i < 5 ); break;
			}
			strToken = strConnectString.Tokenize( _T("/"), iStart );
		}
		if( ! strServerName.IsEmpty() )
			ConnectToSDE( strUserName, strPassword, strVersion, strInstance, strServerName );
		else
			_ASSERTE( false );
	}

	if( ! p )
	{
		TRACEMESSAGE( _T("%s\n"), FAILED_TO_CONNECT );
		return false;
	} else {
		TRACEMESSAGE( _T("%s\n"), SUCCESS_TO_CONNECT );
		return true;
	}
}

// SDE�f�[�^�x�[�X�ɐڑ�����
bool CWorkspace::ConnectToSDE( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName )
{
	bool bRet = false;	// �Ԃ�l

	LOGASSERTE_IF( lpcszUserName && lpcszPassword && lpcszVersion && lpcszInstance && lpcszServerName, sindyErr_ArgIsNull )
	{
		// �ڑ�������N���A
		m_spNameString.reset( (CWorkspaceNameString*)NULL );

		CString connectionStr;
		connectionStr.Format(_T("%s@%s(%s)"), lpcszUserName, lpcszServerName, lpcszVersion );
		IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name(connectionStr);

		IPropertySetPtr ipPropertySet( CLSID_PropertySet );
		ipWorkspaceName->get_ConnectionProperties(&ipPropertySet);

		IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_SdeWorkspaceFactory );
		LOGASSERTE_IF( SUCCEEDED( ipWorkspaceFactory->Open( ipPropertySet, NULL, Release() ) ), sindyErr_AOConnectDBFailed )
			bRet = true;
	}

	return bRet;
}

// �p�[�\�i���E�W�I�f�[�^�x�[�X�ɐڑ�����
bool CWorkspace::ConnectToPGDB( LPCTSTR lpcszPGDBFileName )
{
	// �����`�F�b�N
	LOGASSERTE_IF( lpcszPGDBFileName, sindyErr_ArgIsNull )
	{
		IWorkspaceFactoryPtr ipWorkspaceFactory;
		CString strExt( CString(lpcszPGDBFileName).Right(4) );
		if( strExt == _T(".mdb") )
			ipWorkspaceFactory.CreateInstance( CLSID_AccessWorkspaceFactory );
		else if( strExt == _T(".gdb") ) // �t�@�C���W�I�f�[�^�x�[�X
			ipWorkspaceFactory.CreateInstance( CLSID_FileGDBWorkspaceFactory );
		
		if( ipWorkspaceFactory )
			ipWorkspaceFactory->OpenFromFile( CComBSTR(lpcszPGDBFileName), 0, Release() );

		// �ڑ�������N���A
		m_spNameString.reset( (CWorkspaceNameString*)NULL );
	}

	return ( this ) ? true : false;
}

// OLEDB�ɐڑ�����
bool CWorkspace::ConnectToOLEDB( LPCTSTR lpcszConnectString )
{
	// �����`�F�b�N
	LOGASSERTE_IF( lpcszConnectString, sindyErr_ArgIsNull )
	{
		IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_OLEDBWorkspaceFactory );
		IPropertySetPtr ipPropSet( CLSID_PropertySet );
		ipPropSet->SetProperty( CComBSTR(_T("CONNECTSTRING")), CComVariant(lpcszConnectString) );

		ipWorkspaceFactory->Open( ipPropSet, 0, Release() );

		// �ڑ�������N���A
		m_spNameString.reset( (CWorkspaceNameString*)NULL );
	}

	return ( this ) ? true : false;
}

// �p�[�\�i���E�W�I�f�[�^�x�[�X�����ǂ����𔻒f����
bool CWorkspace::IsPGDBFileName( LPCTSTR lpcszString )
{
	bool bRet = false;	// �Ԃ�l

	LOGASSERTE_IF( lpcszString != NULL, sindyErr_ArgIsNull )
	{
		// .mdb�ŏI����Ă��邩�ǂ����Ŕ��f����
		CString strString( lpcszString );
		if( strString.Right( 4 ).CompareNoCase( _T(".mdb") ) == 0 || 
			strString.Right( 4 ).CompareNoCase( _T(".gdb") ) == 0 )
			bRet = true;
	}

	return bRet;
}

// OLEDB�ڑ������񖼂��ǂ����𔻒f����
bool CWorkspace::IsOLEDBConnectString( LPCTSTR lpcszString )
{
	bool bRet = false; // �߂�l

	LOGASSERTE_IF( lpcszString, sindyErr_ArgIsNull )
	{
		CString strString( lpcszString );
		if( strString.Left( 9 ).CompareNoCase( _T("Provider=") ) == 0 )
			bRet = true;
	}
	return bRet;
}

// �e�[�u�����I�[�v������
ITablePtr CWorkspace::OpenTable( LPCTSTR lpcszTableName ) const
{
	ITablePtr ipTable;

	// �����`�F�b�N
	LOGASSERTE_IF( lpcszTableName != NULL, sindyErr_ArgIsNull )
	{
		IFeatureWorkspacePtr ipFeatureWorkspace(p);
		if( ipFeatureWorkspace != NULL )
			ipFeatureWorkspace->OpenTable( _bstr_t(lpcszTableName), &ipTable );
	}

	TRACEMESSAGE(_T("TABLE[%s]�FOpen[%s]\n"), lpcszTableName, ( NULL != ipTable ) ? SUCCESS : FAILURE );

	return ipTable;
}

// �ҏW�����ɊJ�n����Ă��邩�ǂ���
bool CWorkspace::IsBeingEdited() const
{
	VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, IsBeingEdited( &vbIsBeingEdited ) );
	if( FAILED( hr ) )
	{
		ERRORLOG(sindyErr_COMFunctionFailed );
	}
	return VB2bool(vbIsBeingEdited);
}

// �ҏW�J�n����
sindyErrCode CWorkspace::StartEditing( bool bWithUndoRedo ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, StartEditing( bool2VB(bWithUndoRedo) ) );
	if( SUCCEEDED( hr ) )
	{
		TRACEMESSAGE( _T("StartEditing(UndoRedo=%s)...OK\n"), bWithUndoRedo ? _T("true") : _T("false") );
	}
	// �ҏW�J�n�Ɏ��s
	else {
		emErr = sindyErr_EditStartFailed;
		TRACEMESSAGE( _T("StartEditing(UndoRedo=%s)...FAILED!! %s\n"), bWithUndoRedo ? _T("true") : _T("false"), GETCOMERRORSTRING() );
		ERRORLOG(emErr);
		// �o�[�W������Ή��̏ꍇ�͎��s����̂�ASSERT�͂����Ȃ�
	}

	return emErr;
}

// �ҏW���I������
sindyErrCode CWorkspace::StopEditing( bool bSaveEdits ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, StopEditing( bool2VB(bSaveEdits) ) );
	if( SUCCEEDED( hr ) )
	{
		TRACEMESSAGE( _T("StopEditing(SaveEdits=%s)...OK\n"), bSaveEdits ? _T("true") : _T("false") );
	}
	// �ҏW�I���Ɏ��s
	else {
		if( hr == -2147217147 ) { // FDO_E_VERSION_REDEFINED
			emErr = sindyErr_EditMustBeReconcile;
			TRACEMESSAGE( _T("StopEditing(SaveEdits=%s)...FAILED!! Call Again after reconcile.\n"), bSaveEdits ? _T("true") : _T("false") );
		}
		else {
			emErr = sindyErr_EditStopFailed;
			TRACEMESSAGE( _T("StopEditing(SaveEdits=%s)...FAILED!!\n"), bSaveEdits ? _T("true") : _T("false") );
		}
		ERRORLOG(emErr);
		// �o�[�W������Ή��̏ꍇ�͎��s����̂�ASSERT�͂����Ȃ�
	}
	return emErr;
}

sindyErrCode CWorkspace::StartEditOperation() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, StartEditOperation() );
	if ( SUCCEEDED( hr ))
	{
		TRACEMESSAGE( _T("StartEditOperation...OK\n") );
	}
	else {
		emErr = sindyErr_EditOperationStartFailed;
		TRACEMESSAGE( _T("StopEditOperation...FAILED!!\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

sindyErrCode CWorkspace::AbortEditOperation() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, AbortEditOperation() );
	if ( SUCCEEDED( hr ))
	{
		TRACEMESSAGE( _T("AbortEditOperation...OK\n") );
	}
	else {
		emErr = sindyErr_EditOperationAbortFailed;
		TRACEMESSAGE( _T("AbortEditOperation...FAILED!!\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

sindyErrCode CWorkspace::StopEditOperation() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, StopEditOperation() );
	if ( SUCCEEDED( hr ))
	{
		TRACEMESSAGE( _T("StopEditOperation...OK\n") );
	}
	else {
		emErr = sindyErr_EditOperationStopFailed;
		TRACEMESSAGE( _T("StopEditOperation...FAILED!!\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

// �g�����U�N�V�������J�n����
sindyErrCode CWorkspace::StartTransaction() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	TRACEMESSAGE( _T("Transaction Starting...\n") );
	QUERYINVOKEMETHOD( ITransactions, IID_ITransactions, StartTransaction() );
	if ( SUCCEEDED( hr ))
	{
		TRACEMESSAGE( _T("Transaction Started.\n") );
	}
	else {
		emErr = sindyErr_EditStartFailed;
		TRACEMESSAGE( _T("Transaction Start Failed.\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

// �g�����U�N�V�������R�~�b�g����
sindyErrCode CWorkspace::CommitTransaction() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	TRACEMESSAGE( _T("Transaction Committing...\n") );
	QUERYINVOKEMETHOD( ITransactions, IID_ITransactions, CommitTransaction() );
	if ( SUCCEEDED( hr ))
	{
		TRACEMESSAGE( _T("Transaction Commited.\n") );
	}
	else {
		emErr = sindyErr_EditStopFailed;
		TRACEMESSAGE( _T("Transaction Commit Failed.\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

// �g�����U�N�V������j������
sindyErrCode CWorkspace::AbortTransaction() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	TRACEMESSAGE( _T("Transaction Aborting...\n") );
	QUERYINVOKEMETHOD( ITransactions, IID_ITransactions, AbortTransaction() );
	if ( SUCCEEDED( hr ))
	{
		TRACEMESSAGE( _T("Transaction Aborted.\n") );
	}
	else {
		emErr = sindyErr_EditStopFailed;
		TRACEMESSAGE( _T("Transaction Abort Failed.\n") );
		ERRORLOG(emErr);
	}
	return emErr;

}

// �T�[�o���Ȃǂ��Ǘ����郁���o�̋��L�R�s�[��Ԃ�
CSPWorkspaceNameString CWorkspace::GetNameString() const
{
	if( ! m_spNameString )
		m_spNameString.reset( new CWorkspaceNameString( (IWorkspace*)*this ) );

	return m_spNameString;
}

} // sindy
