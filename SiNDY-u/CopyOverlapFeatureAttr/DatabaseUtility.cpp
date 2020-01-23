#include "stdafx.h"
#include "DataBaseUtility.h"
#include "Message.h"
#include <sindy/workspace.h>
#include <ArcHelperEx/AheGlobalsMisc.h>

// 設定ファイルの項目名
namespace errMsgs
{
	const wchar_t kNotAvailable[]      = L"FDO_E_RECONCILE_VERSION_NOT_AVAILABLE";
	const wchar_t kVerEdited[]         = L"FDO_E_VERSION_BEING_EDITED";
	const wchar_t kVerNotFound[]       = L"FDO_E_VERSION_NOT_FOUND";
	const wchar_t kNotEditing[]        = L"FDO_E_NOT_EDITING";
	const wchar_t kUnknown[]           = L"原因不明";
}

namespace
{

// StopEditingとStopEditingに失敗した場合リコンサイルする関数
bool stopEditing(IWorkspaceEditPtr& ipWorkspaceEdit, CString& strErrMsg)
{
	HRESULT	hr	= ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
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
	case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE: codeMsg = errMsgs::kNotAvailable; break;
	case FDO_E_VERSION_BEING_EDITED:            codeMsg = errMsgs::kVerEdited;    break;
	case FDO_E_VERSION_NOT_FOUND:               codeMsg = errMsgs::kVerNotFound;  break;
	case FDO_E_NOT_EDITING:                     codeMsg = errMsgs::kNotEditing;   break;
	case S_OK:
		// コンフリクトした場合
		if( vbConflicts )
		{
			strErrMsg = message::getMsg(message::iErrorConflicts);
			return false;
		}
		return stopEditing( ipWorkspaceEdit, strErrMsg ); // リコンサイル後は再度保存処理（StopEditing）が必要
	default:
		codeMsg = errMsgs::kUnknown; // 原因不明
		break;
	}

	strErrMsg = uh::str_util::format( message::getMsg(message::iErrorReconcile), codeMsg );
	return false;
}

} // namespace

namespace DatabaseUtility
{

// 編集開始を実行
bool startEdit(IWorkspaceEditPtr& ipWorkspace, CString& strMsg)
{
	if( FAILED ( ipWorkspace->StartEditing( VARIANT_FALSE ) )) // Undo Redo がいらないなら VARIANT_FALSE
	{
		strMsg = message::getMsg(message::iErrorStartEditing);
		return false;
	}
	if( FAILED ( ipWorkspace->StartEditOperation() ))
	{
		strMsg = message::getMsg(message::iErrorStartEditOperation);
		return false;
	}

	return true;
}

// 編集終了を実行
bool stopEdit(IWorkspaceEditPtr& ipWorkspace, CString& strMsg)
{
	// 編集の終了
	if ( FAILED ( ipWorkspace->StopEditOperation() ))
	{
		strMsg = message::getMsg(message::iErrorStopEditOperation);
		return false;
	}

	CString strErrMsg;
	if (!stopEditing(ipWorkspace, strErrMsg))
	{
		strMsg = uh::str_util::format( message::getMsg(message::iErrorStopEditing), strErrMsg );
		return false;
	}

	return true;
}

// 編集を破棄する
void abortEdit( IWorkspaceEditPtr& ipWorkspace, CString& strMsg )
{
	VARIANT_BOOL editBeginig = VARIANT_FALSE;
	ipWorkspace->IsBeingEdited(&editBeginig);
	// 編集開始していないなら何もしない
	if(editBeginig==VARIANT_FALSE) return;

	// 編集の破棄 これが失敗しても、これ以上できることはない
	if ( FAILED ( ipWorkspace->AbortEditOperation() ))
		strMsg = message::getMsg(message::iErrorAbortEditing);
}

} // DatabaseUtility
