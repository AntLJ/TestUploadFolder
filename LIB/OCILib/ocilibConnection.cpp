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
#include "ocilibConnection.h"

ocilibConnection::ocilibConnection()
{
	m_ocilibError = NULL;
	m_ocilibGlobal = NULL;

	m_ptszData = NULL;

	m_IsConnect = false;

	Init();
}

ocilibConnection::ocilibConnection( LPCTSTR server, LPCTSTR username, LPCTSTR password )
{
	try {
		Connect( server, username, password );
	} catch(...) {
		Disconnect();
	}
}

ocilibConnection::~ocilibConnection()
{
	Clear();
}

void ocilibConnection::Initialize()
{
	if ( m_ocilibError != NULL ) 
		delete m_ocilibError;
	m_ocilibError = NULL;

	if ( m_ocilibGlobal != NULL ) 
		delete m_ocilibGlobal;
	m_ocilibGlobal = NULL;

	if ( m_ptszData != NULL )
		delete [] m_ptszData;
	m_ptszData = NULL;

	m_ociEnvironment = NULL;
	m_ociServer = NULL;
	m_ociError = NULL;
	m_ociSession = NULL;
	m_ociSvcCtx = NULL;

	m_IsConnect = false;
}

void ocilibConnection::Init()
{
	try {
		if ( m_IsConnect == true )
			return;

		Initialize();

		if ( m_ocilibGlobal == NULL )
			m_ocilibGlobal = new ocilibGlobal();

		if ( m_ocilibError == NULL )
			m_ocilibError = new ocilibError(m_ocilibGlobal);

		sword		result;

		// OCI 関数が実行される環境を作成および初期化します。
		result = m_ocilibGlobal->EnvCreate( &m_ociEnvironment, OCI_THREADED|OCI_OBJECT|OCI_DEFAULT, NULL, 0, NULL );
		if ( OCI_SUCCESS != result || m_ociEnvironment == NULL ) {
			m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			return;
		}

		// 割当ておよび初期化済みハンドルのポインタを戻します
		result = m_ocilibGlobal->HandleAlloc( m_ociEnvironment, (void **)&m_ociError, OCI_HTYPE_ERROR, 0, NULL );
		if ( OCI_SUCCESS != result || m_ociError == NULL ) {
			m_ocilibError->ErrorEnv(result, m_ociEnvironment, _T(__FILE__), __LINE__);
			return;
		}

		// 割当ておよび初期化済みハンドルのポインタを戻します
		result = m_ocilibGlobal->HandleAlloc( m_ociEnvironment, (void **)&m_ociServer, OCI_HTYPE_SERVER, 0, NULL );
		if ( OCI_SUCCESS != result || m_ociServer == NULL ) {
			m_ocilibError->ErrorEnv(result, m_ociEnvironment, _T(__FILE__), __LINE__);
			return;
		}

		// 割当ておよび初期化済みハンドルのポインタを戻します
		result = m_ocilibGlobal->HandleAlloc( m_ociEnvironment, (void **)&m_ociSvcCtx, OCI_HTYPE_SVCCTX, 0, NULL );
		if ( OCI_SUCCESS != result || m_ociSvcCtx == NULL ) {
			m_ocilibError->ErrorEnv(result, m_ociEnvironment, _T(__FILE__), __LINE__);
			return;
		}

		// 割当ておよび初期化済みハンドルのポインタを戻します
		result = m_ocilibGlobal->HandleAlloc( m_ociEnvironment, (void **)&m_ociSession, OCI_HTYPE_SESSION, 0, NULL );
		if ( OCI_SUCCESS != result || m_ociSession == NULL ) {
			m_ocilibError->ErrorEnv(result, m_ociEnvironment, _T(__FILE__), __LINE__);
			return;
		}

		if ( m_ptszData != NULL )
			delete [] m_ptszData;
		NEW( m_ptszData, FETCH_SIZE, TCHAR );

		return;
	} catch(...) {
		Clear();
	}

	return;
}

void ocilibConnection::Clear()
{
	try {
		Disconnect();

		sword		result;

		if ( m_ociSvcCtx != NULL ) {
			// ハンドルの割当てを明示的に解除します。
			result = m_ocilibGlobal->HandleFree( m_ociSvcCtx, OCI_HTYPE_SVCCTX );
			if ( OCI_SUCCESS != result )
				m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			m_ociSvcCtx = NULL;
		}

		if ( m_ociSession != NULL ) {
			// ハンドルの割当てを明示的に解除します。
			result = m_ocilibGlobal->HandleFree( m_ociSession, OCI_HTYPE_SESSION );
			if ( OCI_SUCCESS != result )
				m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			m_ociSession = NULL;
		}

		if ( m_ociServer != NULL ) {
			// ハンドルの割当てを明示的に解除します。
			result = m_ocilibGlobal->HandleFree( m_ociServer, OCI_HTYPE_SERVER );
			if ( OCI_SUCCESS != result )
				m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			m_ociServer = NULL;
		}

		if ( m_ociError != NULL ) {
			// ハンドルの割当てを明示的に解除します。
			result = m_ocilibGlobal->HandleFree( m_ociError, OCI_HTYPE_ERROR );
			if ( OCI_SUCCESS != result )
				m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			m_ociError = NULL;
		}

		if ( m_ociEnvironment != NULL ) {
			// ハンドルの割当てを明示的に解除します。
			result = m_ocilibGlobal->HandleFree( m_ociEnvironment, OCI_HTYPE_ENV );
			if ( OCI_SUCCESS != result )
				m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			m_ociEnvironment = NULL;
		}
	} catch(...) {
	}

	Initialize();
}

bool ocilibConnection::Connect( LPCTSTR server, LPCTSTR username, LPCTSTR password )
{
	try {
		if ( m_IsConnect == true )
			return true;

		if ( server == NULL || username == NULL || password == NULL )
			return false;

		m_ServerName = server;
		m_UserName = username;
		m_Password = password;

		sword		result;

		char*	szserver = NULL;
		long	size = 0;

		WtoM2( szserver, m_ServerName, size );
		// データ・ソースへのアクセス・パスを作成します。
		result = m_ocilibGlobal->ServerAttach( m_ociServer, m_ociError, (text *)szserver, (sb4)strlen(szserver), OCI_DEFAULT );
		delete [] szserver;
		if ( OCI_SUCCESS != result ) {
			m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		// ハンドルまたは記述子の特定の属性を設定します。
		result = m_ocilibGlobal->AttrSet( m_ociSvcCtx, OCI_HTYPE_SVCCTX, m_ociServer, (sb4)sizeof(OCIServer *), OCI_ATTR_SERVER, m_ociError);
		if ( OCI_SUCCESS != result ) {
			m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		char*	szuser = NULL;

		WtoM2( szuser, m_UserName, size );
		// ハンドルまたは記述子の特定の属性を設定します。
		result = m_ocilibGlobal->AttrSet( m_ociSession, OCI_HTYPE_SESSION, (text *)szuser, (sb4)strlen(szuser), OCI_ATTR_USERNAME, m_ociError );
		delete [] szuser;
		if ( OCI_SUCCESS != result ) {
			m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		char*	szpassword = NULL;

		WtoM2( szpassword, m_Password, size );
		// ハンドルまたは記述子の特定の属性を設定します。
		result = m_ocilibGlobal->AttrSet( m_ociSession, OCI_HTYPE_SESSION, (text *)szpassword, (sb4)strlen(szpassword), OCI_ATTR_PASSWORD, m_ociError );
		delete [] szpassword;
		if ( OCI_SUCCESS != result ) {
			m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		// ユーザー・セッションを作成し、指定のサーバーに対してユーザー・セッションを開始します
		result = m_ocilibGlobal->SessionBegin( m_ociSvcCtx, m_ociError, m_ociSession, OCI_CRED_RDBMS, OCI_DEFAULT );
		if ( OCI_SUCCESS != result ) {
			m_ocilibError->Error2(result, _T(__FILE__), __LINE__);
			return false;
		}

		// ハンドルまたは記述子の特定の属性を設定します。
		result = m_ocilibGlobal->AttrSet( m_ociSvcCtx, OCI_HTYPE_SVCCTX, m_ociSession, (sb4)sizeof(OCISession *), OCI_ATTR_SESSION, m_ociError);
		if ( OCI_SUCCESS != result ) {
			m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		m_IsConnect = true;

		return true;
	} catch(...) {
		Disconnect();
	}

	return false;
}

void ocilibConnection::Disconnect()
{
	try {
		if ( m_IsConnect == true ) {
			sword		result;

			m_IsConnect = false;

			if ( m_ocilibGlobal == NULL )
				return;

			if ( m_ociServer != NULL && m_ociError != NULL ) {
				ub1			ub1Value = 0;

				// ハンドルまたは記述子の特定の属性を設定します。
				result = m_ocilibGlobal->AttrSet( m_ociServer, OCI_HTYPE_SERVER, &ub1Value, sizeof(ub1Value), OCI_ATTR_NONBLOCKING_MODE, m_ociError );
				if ( OCI_SUCCESS != result )
					m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			}

			if ( m_ociSvcCtx != NULL && m_ociError != NULL && m_ociSession != NULL ) {
				// ユーザー・セッション・コンテキストを終了します
				result = m_ocilibGlobal->SessionEnd( m_ociSvcCtx, m_ociError, m_ociSession, OCI_DEFAULT );
				if ( OCI_SUCCESS != result )
					m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			}

			if ( m_ociServer != NULL && m_ociError != NULL ) {
				// データ・ソースへのアクセスを削除します。
				result = m_ocilibGlobal->ServerDetach( m_ociServer, m_ociError, OCI_DEFAULT );
				if ( OCI_SUCCESS != result )
					m_ocilibError->Error(result, m_ociError, _T(__FILE__), __LINE__);
			}
		}
	} catch(...) {
	}
}

ocilibStatement* ocilibConnection::Prepare( LPCTSTR sql )
{
	if ( sql == NULL )
		return NULL;

	return (new ocilibStatement( this, sql ) );
}

ocilibRecordset* ocilibConnection::Select( LPCTSTR sql )
{
	if ( sql == NULL )
		return NULL;

	ocilibStatement*	Statement = Prepare( sql );

	try {
		if ( Statement != NULL ) {
			ocilibRecordset*	Recordset = Statement->Select();

			if ( Recordset != NULL ) {
				Recordset->AttachStatement( Statement );

				return Recordset;
			}
		}
	} catch(...) {
		if ( Statement != NULL )
			Statement->Release();
	}

	return NULL;
}

void ocilibConnection::Execute( LPCTSTR sql )
{
	if ( sql == NULL )
		return;

	try {
		ocilibStatement 	Statement( this, sql );

		Statement.Execute();
	} catch(...) {
	}
}
