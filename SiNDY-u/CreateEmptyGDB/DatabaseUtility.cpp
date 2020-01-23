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
#include "DataBaseUtility.h"
#include <sindy/workspace.h>
#include <ArcHelperEx/AheGlobalsMisc.h>

namespace
{

// ErrorInfo������𒊏o���ďo�͂���֐�
void OutputErrorDiscription( const CString& strMessage )
{
	CString	errMsg	= AheGetErrorMessage();
	if( !errMsg.IsEmpty() )
	{
		CString	tmp	= AheGetErrorMessage();
		RUNLOGGER.fatal(_T("%s\t[%s]"), strMessage.GetString(), errMsg.GetString());
	}
	else
		RUNLOGGER.fatal(_T("%s\t[%s]"), strMessage.GetString(), _T("Unknown Error"));
}

// StopEditing��StopEditing�Ɏ��s�����ꍇ���R���T�C������֐�
bool stopEditing(IWorkspaceEditPtr ipWorkspaceEdit, bool bSave)
{
	HRESULT	hr	= ipWorkspaceEdit->StopEditing( bSave? VARIANT_TRUE:VARIANT_FALSE );
	if( FDO_E_VERSION_REDEFINED != hr )
	{
		if( S_OK == hr )
			return true;
		else
			return false;
	}

	// ���R���T�C���|�X�g����
	VARIANT_BOOL vbConflicts = VARIANT_FALSE;
	IVersionEdit4Ptr ipVerEdit( ipWorkspaceEdit );
	IVersionInfoPtr ipVersionInfo;
	CComBSTR bstrVersionName;
	IVersionPtr ipVersion( ipVerEdit );
	if( ipVersion )
	{
		if( SUCCEEDED( ipVersion->get_VersionInfo( &ipVersionInfo ) ) )
			ipVersionInfo->get_VersionName( &bstrVersionName );
	}
	hr = ipVerEdit->Reconcile4(
		CComBSTR( bstrVersionName ),
		VARIANT_TRUE,
		VARIANT_TRUE,
		VARIANT_FALSE,
		VARIANT_TRUE,
		&vbConflicts );

	CString codeMsg;
	switch( hr )
	{
	case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:	codeMsg = "FDO_E_RECONCILE_VERSION_NOT_AVAILABLE"; break;
	case FDO_E_VERSION_BEING_EDITED:			codeMsg = "FDO_E_VERSION_BEING_EDITED";            break;
	case FDO_E_VERSION_NOT_FOUND:				codeMsg = "FDO_E_VERSION_NOT_FOUND";               break;
	case FDO_E_NOT_EDITING:						codeMsg = "FDO_E_NOT_EDITING";                     break;
	case S_OK:
		// �R���t���N�g�����ꍇ
		if( vbConflicts )
		{
			OutputErrorDiscription(_T("because competition occurred, the editing is canceled all."));
			return false;
		}
		else
			return stopEditing( ipWorkspaceEdit, bSave ); // ���R���T�C����͍ēx�ۑ������iStopEditing�j���K�v
		break;
	default:
		codeMsg = "Unknown"; // �����s��
		break;
	}
	CString cStr;
	cStr.Format( _T("reconcile failed. code:[%s] The editing is canceled all."), codeMsg );
	OutputErrorDiscription( cStr.GetString() );

	return false;
}

} // namespace

namespace databaseUtility
{

// �ҏW�J�n�����s
bool startEdit(IWorkspaceEditPtr ipWorkspace)
{
	if( FAILED ( ipWorkspace->StartEditing( VARIANT_FALSE ) )) // Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
	{
		RUNLOGGER.fatal(_T("StartEditing failed "));
		return false;
	}
	if( FAILED ( ipWorkspace->StartEditOperation() ))
	{
		RUNLOGGER.fatal(_T("StartEditOperation failed "));
		return false;
	}

	return true;
}

// �ҏW�I�������s
bool stopEdit(IWorkspaceEditPtr ipWorkspace, bool bSave)
{
	// �ҏW�̏I��
	if ( FAILED ( ipWorkspace->StopEditOperation() ))
	{
		RUNLOGGER.fatal(_T("StopEditOperation failed "));
		return false;
	}

	if (!stopEditing(ipWorkspace, bSave))
	{
		RUNLOGGER.fatal(_T("StopEdit failed "));
		return false;
	}

	return true;
}

void abortEdit( IWorkspaceEditPtr workspace )
{
	VARIANT_BOOL editBeginig = VARIANT_FALSE;
	workspace->IsBeingEdited(&editBeginig);
	// �ҏW�J�n���Ă��Ȃ��Ȃ牽�����Ȃ�
	if(editBeginig==VARIANT_FALSE) return;

	// �ҏW�̔j�� ���ꂪ���s���Ă��A����ȏ�ł��邱�Ƃ͂Ȃ�
	if ( FAILED ( workspace->AbortEditOperation() ))
		RUNLOGGER.fatal(_T("AbortEditOperation failed "));
}

} // databaseUtility
