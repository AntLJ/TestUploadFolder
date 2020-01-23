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

// Workspaces.cpp: CWorkspaces クラスのインプリメンテーション
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


// 文字化け対策用関数
bool CWorkspaces::PreConnect()
{
	IWorkspacePtr ipWorkspace = 0;
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);
	if (ipWorkspaceFactory->OpenFromFile(CComBSTR(_T("C:\\")), 0, &ipWorkspace) != S_OK)
		return false;
	return true;
}

// データベースに接続する
CWorkspace* CWorkspaces::Connect( LPCTSTR lpcszConnectString )
{
	// 引数チェック
	if( lpcszConnectString == NULL )
	{
		_ASSERTE( lpcszConnectString != NULL );
		return NULL;
	}

	// 既に接続済みのワークスペースか
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

	// リストに追加してから接続
	m_listWorkspace.push_front( CWorkspace() );
	std::list<CWorkspace>::iterator it = m_listWorkspace.begin();
	if( it->Connect( lpcszConnectString ) )
		return &(*it);
	else {
		// さっき追加したものは削除
		m_listWorkspace.erase( it );
		return NULL;
	}
}

// SDEデータベースに接続する
CWorkspace* CWorkspaces::ConnectToSDE( LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName )
{
	// 引数チェック
	if( lpcszUserName == NULL || lpcszPassword == NULL || lpcszVersion == NULL || lpcszInstance == NULL || lpcszServerName == NULL )
	{
		_ASSERTE( lpcszUserName != NULL && lpcszPassword != NULL && lpcszVersion != NULL && lpcszInstance != NULL && lpcszServerName != NULL );
		return NULL;
	}

	// 既に接続済みのワークスペースか
	const CWorkspace *p = FindWorkspace(lpcszUserName, lpcszVersion, lpcszServerName);
	if (p) return const_cast<CWorkspace *>(p);

	// リストに追加してから接続
	m_listWorkspace.push_front( CWorkspace() );
	std::list<CWorkspace>::iterator it = m_listWorkspace.begin();
	if( it->ConnectToSDE( lpcszUserName, lpcszPassword, lpcszVersion, lpcszInstance, lpcszServerName ) )
		return &(*it);
	else {
		// さっき追加したものは削除
		m_listWorkspace.erase( it );
		return NULL;
	}
}

// パーソナル・ジオデータベースに接続する
CWorkspace* CWorkspaces::ConnectToPGDB( LPCTSTR lpcszPGDBFileName )
{
	// 引数チェック
	if( lpcszPGDBFileName == NULL )
	{
		_ASSERTE( lpcszPGDBFileName != NULL );
		return NULL;
	}

	// リストに追加してから接続
	m_listWorkspace.push_front( CWorkspace() );
	std::list<CWorkspace>::iterator it = m_listWorkspace.begin();
	if( it->ConnectToPGDB( lpcszPGDBFileName ) )
		return &(*it);
	else {
		// さっき追加したものは削除
		m_listWorkspace.erase( it );
		return NULL;
	}
}

// 既存の接続を追加する
sindyErrCode CWorkspaces::AddConnection( IWorkspacePtr ipWorkspace )
{
	// 引数チェック
	if( ipWorkspace == NULL ) return sindyErr_COMInterfaceIsNull;

	if( FindWorkspace( ipWorkspace ) == NULL )
	{
		m_listWorkspace.push_back( CWorkspace(ipWorkspace) );
	}

	return sindyErr_NoErr;
}

// 既存の接続を探す
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

// 既存の接続を探す
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

// パーソナル・ジオデータベース名かどうかを判断する
bool CWorkspaces::IsPGDBFileName( LPCTSTR lpcszString ) const
{
	// 引数チェック
	if( lpcszString == NULL )
	{
		_ASSERTE( lpcszString != NULL );
		return false;
	}

	// .mdbで終わっているかどうかで判断する
	CString strString( lpcszString );
	if( strString.Right( 4 ).CompareNoCase( _T(".mdb") ) == 0 )
		return true;

	return false;
}

/*void CWorkspaces::GetConnectionInfo( IWorkspacePtr ipWorkspace, DB_CONNECTION_INFO& stConnectionInfo ) const
{
	// 引数チェック
	if( ipWorkspace == NULL )
	{
		_ASSERTE( ipWorkspace != NULL );
		return;
	}
	// 返り値初期化
	stConnectionInfo.ConnectString.Empty();
	stConnectionInfo.Instance.Empty();
	stConnectionInfo.IsPGDB = false;
	stConnectionInfo.Password.Empty();
	stConnectionInfo.ServerName.Empty();
	stConnectionInfo.UserName.Empty();
	stConnectionInfo.Version.Empty();

	// ワークスペースから接続情報を取得
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

/*// 注記ルールテーブルクラスの参照を返す
CAnnotationClassTable& CWorkspaces::GetAnnotationClassTable()
{
	if( m_cAnnotationClassTable == NULL )
	{
		// ルール用サーバ名を取得する
		CSiNDYeSettings cSettings;
		cSettings.Load();	//!< エラーでも良い

		// 既にそのサーバ名での接続があるかどうかをチェックする
		const CWorkspace* pWorkspace = FindWorkspaceByServerName( cSettings.GetServerName() );
		if( pWorkspace == NULL )
		{
			// ルールデータベースに接続する
			if( cSettings.GetUsePGDB() )
				pWorkspace = ConnectToPGDB( cSettings.GetPGDBFileName() );
			else
				pWorkspace = ConnectToSDE( _T("ronly"), _T("ronly"), _T("SDE.DEFAULT"), _T("5151"), cSettings.GetServerName() );
		}
		// ルール系はREFERENCEを付ける（SDEのみ）
		if( cSettings.GetUsePGDB() )
			m_cAnnotationClassTable = pWorkspace->OpenTable( sindy::annotation_class::kTableName );
		else
			m_cAnnotationClassTable = pWorkspace->OpenTable( CString(_T("REFERENCE.")) + sindy::annotation_class::kTableName );
	}

	if( m_cAnnotationClassTable == NULL )
		_ASSERTE( m_cAnnotationClassTable != NULL );

	return m_cAnnotationClassTable;
}*/

// ルール系のワークスペースに接続する
const CWorkspace* CWorkspaces::ConnectToRuleWorkspace()
{
	// ルール用サーバ名を取得する
	CSiNDYeSettings cSettings;
	cSettings.Load();	//!< エラーでも良い

	// 既にそのサーバ名での接続があるかどうかをチェックする
	const CWorkspace* pWorkspace = FindWorkspaceByServerName( cSettings.GetServerName() );
	if( pWorkspace == NULL )
	{
		// ルールデータベースに接続する
		if( cSettings.GetUsePGDB() )
			pWorkspace = ConnectToPGDB( cSettings.GetPGDBFileName() );
		else
			pWorkspace = ConnectToSDE( _T("ronly"), _T("ronly"), _T("SDE.DEFAULT"), _T("5151"), cSettings.GetServerName() );
	}

	return pWorkspace;
}

} // sindy
