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

// ErrorInfoから情報を抽出して出力する関数
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

// StopEditingとStopEditingに失敗した場合リコンサイルする関数
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

	// リコンサイルポスト処理
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
		// コンフリクトした場合
		if( vbConflicts )
		{
			OutputErrorDiscription(_T("because competition occurred, the editing is canceled all."));
			return false;
		}
		else
			return stopEditing( ipWorkspaceEdit, bSave ); // リコンサイル後は再度保存処理（StopEditing）が必要
		break;
	default:
		codeMsg = "Unknown"; // 原因不明
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

// 編集開始を実行
bool startEdit(IWorkspaceEditPtr ipWorkspace)
{
	if( FAILED ( ipWorkspace->StartEditing( VARIANT_FALSE ) )) // Undo Redo がいらないなら VARIANT_FALSE
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

// 編集終了を実行
bool stopEdit(IWorkspaceEditPtr ipWorkspace, bool bSave)
{
	// 編集の終了
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
	// 編集開始していないなら何もしない
	if(editBeginig==VARIANT_FALSE) return;

	// 編集の破棄 これが失敗しても、これ以上できることはない
	if ( FAILED ( workspace->AbortEditOperation() ))
		RUNLOGGER.fatal(_T("AbortEditOperation failed "));
}

} // databaseUtility
