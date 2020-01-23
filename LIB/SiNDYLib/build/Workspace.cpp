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

// Workspace.cpp: CWorkspace クラスのインプリメンテーション
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
	LPCTSTR CONNECT_TO_DATABASE  = _T("データベースに接続しています：%s … \n");
	LPCTSTR FAILED_TO_CONNECT    = _T("接続に失敗");
	LPCTSTR SUCCESS_TO_CONNECT   = _T("接続に成功");
	LPCTSTR SUCCESS              = _T("成功");
	LPCTSTR FAILURE              = _T("失敗");
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

// 文字化け対策用関数
bool CWorkspace::PreConnect()
{
	IWorkspacePtr ipWorkspace = 0;
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);
	if (ipWorkspaceFactory->OpenFromFile(CComBSTR(_T("C:\\")), 0, &ipWorkspace) != S_OK)
		return false;
	return true;
}

// データベースに接続する
bool CWorkspace::Connect( LPCTSTR lpcszConnectString )
{
	// 引数チェック
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

// SDEデータベースに接続する
bool CWorkspace::ConnectToSDE( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName )
{
	bool bRet = false;	// 返り値

	LOGASSERTE_IF( lpcszUserName && lpcszPassword && lpcszVersion && lpcszInstance && lpcszServerName, sindyErr_ArgIsNull )
	{
		// 接続文字列クリア
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

// パーソナル・ジオデータベースに接続する
bool CWorkspace::ConnectToPGDB( LPCTSTR lpcszPGDBFileName )
{
	// 引数チェック
	LOGASSERTE_IF( lpcszPGDBFileName, sindyErr_ArgIsNull )
	{
		IWorkspaceFactoryPtr ipWorkspaceFactory;
		CString strExt( CString(lpcszPGDBFileName).Right(4) );
		if( strExt == _T(".mdb") )
			ipWorkspaceFactory.CreateInstance( CLSID_AccessWorkspaceFactory );
		else if( strExt == _T(".gdb") ) // ファイルジオデータベース
			ipWorkspaceFactory.CreateInstance( CLSID_FileGDBWorkspaceFactory );
		
		if( ipWorkspaceFactory )
			ipWorkspaceFactory->OpenFromFile( CComBSTR(lpcszPGDBFileName), 0, Release() );

		// 接続文字列クリア
		m_spNameString.reset( (CWorkspaceNameString*)NULL );
	}

	return ( this ) ? true : false;
}

// OLEDBに接続する
bool CWorkspace::ConnectToOLEDB( LPCTSTR lpcszConnectString )
{
	// 引数チェック
	LOGASSERTE_IF( lpcszConnectString, sindyErr_ArgIsNull )
	{
		IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_OLEDBWorkspaceFactory );
		IPropertySetPtr ipPropSet( CLSID_PropertySet );
		ipPropSet->SetProperty( CComBSTR(_T("CONNECTSTRING")), CComVariant(lpcszConnectString) );

		ipWorkspaceFactory->Open( ipPropSet, 0, Release() );

		// 接続文字列クリア
		m_spNameString.reset( (CWorkspaceNameString*)NULL );
	}

	return ( this ) ? true : false;
}

// パーソナル・ジオデータベース名かどうかを判断する
bool CWorkspace::IsPGDBFileName( LPCTSTR lpcszString )
{
	bool bRet = false;	// 返り値

	LOGASSERTE_IF( lpcszString != NULL, sindyErr_ArgIsNull )
	{
		// .mdbで終わっているかどうかで判断する
		CString strString( lpcszString );
		if( strString.Right( 4 ).CompareNoCase( _T(".mdb") ) == 0 || 
			strString.Right( 4 ).CompareNoCase( _T(".gdb") ) == 0 )
			bRet = true;
	}

	return bRet;
}

// OLEDB接続文字列名かどうかを判断する
bool CWorkspace::IsOLEDBConnectString( LPCTSTR lpcszString )
{
	bool bRet = false; // 戻り値

	LOGASSERTE_IF( lpcszString, sindyErr_ArgIsNull )
	{
		CString strString( lpcszString );
		if( strString.Left( 9 ).CompareNoCase( _T("Provider=") ) == 0 )
			bRet = true;
	}
	return bRet;
}

// テーブルをオープンする
ITablePtr CWorkspace::OpenTable( LPCTSTR lpcszTableName ) const
{
	ITablePtr ipTable;

	// 引数チェック
	LOGASSERTE_IF( lpcszTableName != NULL, sindyErr_ArgIsNull )
	{
		IFeatureWorkspacePtr ipFeatureWorkspace(p);
		if( ipFeatureWorkspace != NULL )
			ipFeatureWorkspace->OpenTable( _bstr_t(lpcszTableName), &ipTable );
	}

	TRACEMESSAGE(_T("TABLE[%s]：Open[%s]\n"), lpcszTableName, ( NULL != ipTable ) ? SUCCESS : FAILURE );

	return ipTable;
}

// 編集が既に開始されているかどうか
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

// 編集開始する
sindyErrCode CWorkspace::StartEditing( bool bWithUndoRedo ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, StartEditing( bool2VB(bWithUndoRedo) ) );
	if( SUCCEEDED( hr ) )
	{
		TRACEMESSAGE( _T("StartEditing(UndoRedo=%s)...OK\n"), bWithUndoRedo ? _T("true") : _T("false") );
	}
	// 編集開始に失敗
	else {
		emErr = sindyErr_EditStartFailed;
		TRACEMESSAGE( _T("StartEditing(UndoRedo=%s)...FAILED!! %s\n"), bWithUndoRedo ? _T("true") : _T("false"), GETCOMERRORSTRING() );
		ERRORLOG(emErr);
		// バージョン非対応の場合は失敗するのでASSERTはださない
	}

	return emErr;
}

// 編集を終了する
sindyErrCode CWorkspace::StopEditing( bool bSaveEdits ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	QUERYINVOKEMETHOD( IWorkspaceEdit, IID_IWorkspaceEdit, StopEditing( bool2VB(bSaveEdits) ) );
	if( SUCCEEDED( hr ) )
	{
		TRACEMESSAGE( _T("StopEditing(SaveEdits=%s)...OK\n"), bSaveEdits ? _T("true") : _T("false") );
	}
	// 編集終了に失敗
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
		// バージョン非対応の場合は失敗するのでASSERTはださない
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

// トランザクションを開始する
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

// トランザクションをコミットする
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

// トランザクションを破棄する
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

// サーバ名などを管理するメンバの共有コピーを返す
CSPWorkspaceNameString CWorkspace::GetNameString() const
{
	if( ! m_spNameString )
		m_spNameString.reset( new CWorkspaceNameString( (IWorkspace*)*this ) );

	return m_spNameString;
}

} // sindy
