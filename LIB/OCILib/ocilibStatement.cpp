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
#include "ocilibStatement.h"

ocilibStatement::ocilibStatement()
{
}

ocilibStatement::ocilibStatement( ocilibConnection* Connect, LPCTSTR sql )
{
	Initialize();

	m_ocilibConnect = Connect;

	try {
		Prepare( sql );
	} catch(...) {
		Clear();
	}
}

ocilibStatement::~ocilibStatement()
{
	Clear();
}

ocilibParameter& ocilibStatement::operator [](LPCTSTR name)
{
	map<_CSTRING_NS::CString, ocilibParameter*>::iterator it = m_mapParameters.find(name);

	if ( it == m_mapParameters.end() )
		m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NOT_FOUND_PARAM, _T(__FILE__), __LINE__, name);

	return (*(it->second));
}

ocilibParameter& ocilibStatement::operator [](ub4 Index)
{
	if ( Index < FIRST_PARAMETER || Index > (unsigned)m_mapParameters.size() )
		m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NOT_FOUND_PARAM, _T(__FILE__), __LINE__, TEXT("%d"), (int)Index);

	return (*(m_vecParameters.at( Index - FIRST_PARAMETER )));
}

void ocilibStatement::Initialize()
{
	m_ociType = 0;
	m_IsPrepared = false;
	m_IsExecuted = false;

	if ( !m_vecParameters.empty() ) {
		vector<ocilibParameter*>::iterator First = m_vecParameters.begin();
		vector<ocilibParameter*>::iterator End = m_vecParameters.end();
		for ( vector<ocilibParameter*>::iterator it = First; it != End; ++it )
			delete (*it);
	}
	vector<ocilibParameter*>().swap( m_vecParameters );
	m_vecParameters.clear();

	m_mapParameters.clear();
}

void ocilibStatement::Clear()
{
	if ( m_ociStmt != NULL ) {
		m_ocilibConnect->m_ocilibGlobal->HandleFree( m_ociStmt, OCI_HTYPE_STMT );
		m_ociStmt = NULL;
	}

	if ( !m_vecParameters.empty() ) {
		vector<ocilibParameter*>::iterator First = m_vecParameters.begin();
		vector<ocilibParameter*>::iterator End = m_vecParameters.end();
		for ( vector<ocilibParameter*>::iterator it = First; it != End; ++it )
			delete (*it);
	}
	vector<ocilibParameter*>().swap( m_vecParameters );
	m_vecParameters.clear();

	m_mapParameters.clear();
}

ocilibParameter& ocilibStatement::Bind( LPCTSTR name, enumociDataType type, ub4 maxSize )
{
	ocilibParameter *p = new ocilibParameter( this, name, type, maxSize );

	m_vecParameters.push_back( p );
	m_mapParameters[p->ParamName()] = p;

	return (*p);
}

ocilibParameter& ocilibStatement::Bind( LPCTSTR name, LPCTSTR text, enumociDataType type, ub4 maxSize )
{
	ocilibParameter&	Parameter = Bind(name, type, maxSize);

	Parameter = text;

	return Parameter;
}

ocilibParameter& ocilibStatement::Bind( LPCTSTR name, double value, enumociDataType type )
{
	ocilibParameter&	Parameter = Bind(name, type, sizeof(OCINumber));

	Parameter = value;

	return Parameter;
}

ocilibParameter& ocilibStatement::Bind( LPCTSTR name, long value, enumociDataType type )
{
	ocilibParameter&	Parameter = Bind(name, type, sizeof(OCINumber));

	Parameter = value;

	return Parameter;
}

ocilibParameter& ocilibStatement::Bind( LPCTSTR name, const ocilibDate& d, enumociDataType type )
{
	ocilibParameter&	Parameter = Bind(name, type, sizeof(OCIDate));

	Parameter = d;

	return Parameter;
}

ocilibRecordset* ocilibStatement::Select()
{
	ocilibRecordset *ociRecord = NULL;

	try {
		if ( Execute_prepar() == true ) {
			ociRecord = new ocilibRecordset( m_ocilibConnect, m_ociStmt );

			if ( ociRecord != NULL ) {
				ociRecord->FetchRows();

				return ociRecord;
			}
		}
	} catch(...) {
		if ( ociRecord != NULL )
			delete ociRecord;
	}

	return NULL;
}

bool ocilibStatement::Prepare( LPCTSTR sql )
{
	if ( m_ocilibConnect == NULL || sql == NULL )
		return false;

	try {
		// 割当ておよび初期化済みハンドルのポインタを戻します
		sword result = m_ocilibConnect->m_ocilibGlobal->HandleAlloc( m_ocilibConnect->m_ociEnvironment, (void **)&m_ociStmt, OCI_HTYPE_STMT, 0, NULL );
		if ( OCI_SUCCESS != result || m_ociStmt == NULL ) {
			m_ocilibConnect->m_ocilibError->ErrorEnv(result, m_ocilibConnect->m_ociEnvironment, _T(__FILE__), __LINE__);
			return false;
		}

		char*	szsql = NULL;
		long	size = 0;

		WtoM2( szsql, sql, size );

		// 実行するSQL 文またはPL/SQL 文を準備します。
		result = m_ocilibConnect->m_ocilibGlobal->StmtPrepare( m_ociStmt, m_ocilibConnect->m_ociError, (text *)szsql, strlen(szsql), OCI_NTV_SYNTAX, OCI_DEFAULT );
		delete [] szsql;
		if ( OCI_SUCCESS != result ) {
			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		ub4			StatType = 0;

		// ハンドルの特定の属性を取得します。
		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( m_ociStmt, OCI_HTYPE_STMT, &StatType, NULL, OCI_ATTR_STMT_TYPE, m_ocilibConnect->m_ociError );
		if ( OCI_SUCCESS != result ) {
			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return false;
		}

		m_ociType = StatType;

		m_IsPrepared = true;
		m_IsExecuted = false;

		return true;
	} catch(...) {
	}

	return false;
}

bool ocilibStatement::Execute_prepar()
{
	if ( m_ocilibConnect == NULL || m_ociStmt == NULL )
		return false;

	// アプリケーション要求をサーバーに対応付けます
	sword result = m_ocilibConnect->m_ocilibGlobal->StmtExecute( m_ocilibConnect->m_ociSvcCtx, m_ociStmt, m_ocilibConnect->m_ociError, (m_ociType == OCI_STMT_SELECT) ? 0 : 1, 0, NULL, NULL, OCI_DEFAULT );

	if ( OCI_SUCCESS != result ) {
		m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

		if ( m_ocilibConnect->m_ocilibError->GetOraErrorCode() == 28 || m_ocilibConnect->m_ocilibError->GetOraErrorCode() == 1012 )
			m_ocilibConnect->Disconnect();

		return false;
	}

	m_IsExecuted = true;

	return true;
}
