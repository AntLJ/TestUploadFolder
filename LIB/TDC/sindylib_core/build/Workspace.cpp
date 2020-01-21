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
#include "sindymacroutil.h"
#include "sindycomutil.h"
#include "winutil.h"
#include "type_convert.h"
#include "sindy/workspace.h"
#include "TDC/useful_headers/str_util.h"
#include "boost/foreach.hpp"

#define foreach BOOST_FOREACH

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

enum DBType
{
	DBTypeSDE,
	DBTypePostGIS,
};

// データベースに接続する
bool CWorkspace::Connect( LPCTSTR lpcszConnectString )
{
	// 引数チェック
	if( ! lpcszConnectString || 0 == _tcslen(lpcszConnectString) )
		return false;

	if( m_bTrace )
		TRACEMESSAGE( _T("Connect to the database：%s … \n"), lpcszConnectString );

	if( IsOLEDBConnectString( lpcszConnectString ) )
		ConnectToOLEDB( lpcszConnectString );
	else if( IsPGDBFileName( lpcszConnectString ) )
		ConnectToPGDB( lpcszConnectString );
	else if ( IsShapeFileName( lpcszConnectString ) )
		ConnectToShape( lpcszConnectString );
	else {
		// 接続文字列は以下のパターン
		// SDE/user/pass/version/5151/server
		// PGIS/user/pass/db/5432/server
		// user/pass/version/5151/server
		// アイテムが5つの場合はSDE、6つの場合は最初の文字列を見て判断する
		std::vector<CString> param = uh::str_util::split( lpcszConnectString, _T("/") );
		// パラメータ数のチェック
		long param_size = param.size();
		if( !( 5 == param_size || 6 == param_size ) )
		{
			_ASSERTE( 5 == param_size || 6 == param_size );
			return false;
		}
		// 接続タイプを確認
		DBType emType = DBTypeSDE;
		int add = 0;
		if( 6 == param.size() )
		{
			if( 0 == param[0].CompareNoCase(_T("SDE")) )
				emType = DBTypeSDE;
			else if( 0 == param[0].CompareNoCase(_T("PGIS")) )
				emType = DBTypePostGIS;
			add = 1;
		}

		switch( emType )
		{
		case DBTypePostGIS:
			ConnectToPGIS( param[0+add], param[1+add], param[2+add], param[3+add], param[4+add] );
			break;
		case DBTypeSDE:
		default:
			ConnectToSDE( param[0+add], param[1+add], param[2+add], param[3+add], param[4+add] );
			break;
		}
	}

	if( ! p )
	{
		if( m_bTrace )
			TRACEMESSAGE( _T("%s\n"), _T("Failed to connect to the database") );
		return false;
	} else {
		if( m_bTrace )
			TRACEMESSAGE( _T("%s\n"), _T("Succeeded to connect to the database") );
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

		IPropertySetPtr ipPropertySet( CLSID_PropertySet );
		// インスタンスに「:postgresql:」と入っていた場合は、PostgreSQLへのダイレクトコネクションと判断し、
		// VERSIONプロパティをDATABASEプロパティに読み替える
		if( _tcsstr( lpcszInstance, _T(":postgresql:") ) ){
			ipPropertySet->SetProperty( CComBSTR(L"SERVER"),   CComVariant( lpcszServerName ) );
			ipPropertySet->SetProperty( CComBSTR(L"PASSWORD"), CComVariant( lpcszPassword   ) );
			ipPropertySet->SetProperty( CComBSTR(L"USER"),     CComVariant( lpcszUserName   ) );
			ipPropertySet->SetProperty( CComBSTR(L"INSTANCE"), CComVariant( lpcszInstance   ) );
			ipPropertySet->SetProperty( CComBSTR(L"DATABASE"),  CComVariant( lpcszVersion    ) );
		}
		else{
			CString connectionStr;
			connectionStr.Format(_T("%s/%s/%s/%s/%s"), lpcszUserName, lpcszPassword, lpcszVersion, lpcszInstance, lpcszServerName);
			IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name(connectionStr);
			ipWorkspaceName->get_ConnectionProperties(&ipPropertySet);
		}

		IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_SdeWorkspaceFactory );
		HRESULT hr = S_OK;
		long retryLimit = 10;
		long retryCount = 0;
CONNECT:
		switch( hr = ipWorkspaceFactory->Open( ipPropertySet, NULL, Release() ) )
		{
		case 0x8004150B: // Network I/O Error
			if( retryCount++ < 10 )
			{
				// Network I/O Error なら10回だけリトライする
				Sleep(1000); // 1秒空け
				goto CONNECT;
				break;
			}
			// breakしない
		default:
			{
				LOGASSERTE_IF( SUCCEEDED( hr ), sindyErr_AOConnectDBFailed )
					bRet = true;
			}
			break;
		}
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

// Shapeに接続する
bool CWorkspace::ConnectToShape( LPCTSTR lpcszShapeFileName )
{
	// 引数チェック
	LOGASSERTE_IF( lpcszShapeFileName, sindyErr_ArgIsNull )
	{
		IWorkspaceFactoryPtr ipWorkspaceFactory;
		CString strExt( lpcszShapeFileName );
		if( PathIsDirectory(strExt) )
			ipWorkspaceFactory.CreateInstance( CLSID_ShapefileWorkspaceFactory );
		
		if( ipWorkspaceFactory )
			ipWorkspaceFactory->OpenFromFile( CComBSTR(lpcszShapeFileName), 0, Release() );

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

// PostGISに接続する
bool CWorkspace::ConnectToPGIS( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszDB, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName )
{
	LOGASSERTE_IF( lpcszUserName && lpcszPassword && lpcszDB && lpcszInstance && lpcszServerName, sindyErr_ArgIsNull )
	{
		CString conn;
		conn.Format(_T("host=%s;port=%s;user=%s;password=%s;dbname=%s"), lpcszServerName, lpcszInstance, lpcszUserName, lpcszPassword, lpcszDB );
		INamePtr ipName( create_postgis_workspace_string_name( conn ) );
		if( ipName )
		{
			IUnknownPtr ipUnk;
			ipName->Open( &ipUnk );
			*(Release()) = (IWorkspace*)ipUnk.Detach();
		}
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

// Shapeかどうか（フォルダかどうか）を判断する
bool CWorkspace::IsShapeFileName( LPCTSTR lpcszString )
{
	bool bRet = false;	// 返り値

	LOGASSERTE_IF( lpcszString != NULL, sindyErr_ArgIsNull )
	{
		CString strString( lpcszString );
		if( PathIsDirectory( strString) )
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

	if( m_bTrace )
		TRACEMESSAGE(_T("TABLE[%s]：Open[%s]\n"), lpcszTableName, ( NULL != ipTable ) ? _T("Success") : _T("Failure") );

	return ipTable;
}

// テーブルをオープンする
ITablePtr CWorkspace::OpenSameOwnersTable( LPCTSTR lpcszTableName ) const
{
	if( !GetNameString() )
		_ASSERTE( m_spNameString );

	LPCTSTR szOwner = m_spNameString->GetUserName();
	CString strTableName;
	if( 0 < lstrlen(szOwner) )
		strTableName.Format(_T("%s.%s"), szOwner, lpcszTableName );
	else
		strTableName = lpcszTableName;
	
	return OpenTable(strTableName);
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
		if( m_bTrace )
			TRACEMESSAGE( _T("StartEditing(UndoRedo=%s)...OK\n"), bWithUndoRedo ? _T("true") : _T("false") );
	}
	// 編集開始に失敗
	else {
		emErr = sindyErr_EditStartFailed;
		if( m_bTrace )
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
		if( m_bTrace )
			TRACEMESSAGE( _T("StopEditing(SaveEdits=%s)...OK\n"), bSaveEdits ? _T("true") : _T("false") );
	}
	// 編集終了に失敗
	else {
		if( hr == -2147217147 ) { // FDO_E_VERSION_REDEFINED
			emErr = sindyErr_EditMustBeReconcile;
			if( m_bTrace )
				TRACEMESSAGE( _T("StopEditing(SaveEdits=%s)...FAILED!! Call Again after reconcile.\n"), bSaveEdits ? _T("true") : _T("false") );
		}
		else {
			emErr = sindyErr_EditStopFailed;
			if( m_bTrace )
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
		if( m_bTrace )
			TRACEMESSAGE( _T("StartEditOperation...OK\n") );
	}
	else {
		emErr = sindyErr_EditOperationStartFailed;
		if( m_bTrace )
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
		if( m_bTrace )
			TRACEMESSAGE( _T("AbortEditOperation...OK\n") );
	}
	else {
		emErr = sindyErr_EditOperationAbortFailed;
		if( m_bTrace )
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
		if( m_bTrace )
			TRACEMESSAGE( _T("StopEditOperation...OK\n") );
	}
	else {
		emErr = sindyErr_EditOperationStopFailed;
		if( m_bTrace )
			TRACEMESSAGE( _T("StopEditOperation...FAILED!!\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

// トランザクションを開始する
sindyErrCode CWorkspace::StartTransaction() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	if( m_bTrace )
		TRACEMESSAGE( _T("Transaction Starting...\n") );
	QUERYINVOKEMETHOD( ITransactions, IID_ITransactions, StartTransaction() );
	if ( SUCCEEDED( hr ))
	{
		if( m_bTrace )
			TRACEMESSAGE( _T("Transaction Started.\n") );
	}
	else {
		emErr = sindyErr_EditStartFailed;
		if( m_bTrace )
			TRACEMESSAGE( _T("Transaction Start Failed.\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

// トランザクションをコミットする
sindyErrCode CWorkspace::CommitTransaction() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	if( m_bTrace )
		TRACEMESSAGE( _T("Transaction Committing...\n") );
	QUERYINVOKEMETHOD( ITransactions, IID_ITransactions, CommitTransaction() );
	if ( SUCCEEDED( hr ))
	{
		if( m_bTrace )
			TRACEMESSAGE( _T("Transaction Commited.\n") );
	}
	else {
		emErr = sindyErr_EditStopFailed;
		if( m_bTrace )
			TRACEMESSAGE( _T("Transaction Commit Failed.\n") );
		ERRORLOG(emErr);
	}
	return emErr;
}

// トランザクションを破棄する
sindyErrCode CWorkspace::AbortTransaction() const
{
	sindyErrCode emErr = sindyErr_NoErr;

	if( m_bTrace )
		TRACEMESSAGE( _T("Transaction Aborting...\n") );
	QUERYINVOKEMETHOD( ITransactions, IID_ITransactions, AbortTransaction() );
	if ( SUCCEEDED( hr ))
	{
		if( m_bTrace )
			TRACEMESSAGE( _T("Transaction Aborted.\n") );
	}
	else {
		emErr = sindyErr_EditStopFailed;
		if( m_bTrace )
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
