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
#include "ocilibParameter.h"

ocilibParameter::ocilibParameter()
{
}

ocilibParameter::ocilibParameter(ocilibStatement* Statement, LPCTSTR name, enumociDataType type, ub4 fetch_size)
{
	Initialize();

	try {
		Attach( Statement, name, type, fetch_size );
	} catch(...) {
		Clear();
	}
}

ocilibParameter::~ocilibParameter()
{
	Clear();
}

ocilibParameter& ocilibParameter::operator =(LPCTSTR text)
{
	if ( m_ocilibStatement != NULL ) {
		if ( !text )
			m_IsSetted = false;
		else if ( m_ParamType == DT_TEXT ) {
			long	size = 0;

			WtoM( m_FetchBuf, text, size );
//			m_FetchBuf[strlen(m_FetchBuf)]= '\0';

			m_IsSetted = true;
		} else {
			if ( m_ocilibRecordset != NULL )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NG_INPUT_TYPE, _T(__FILE__), __LINE__);
		}
	}

	return *this;
}

ocilibParameter& ocilibParameter::operator =(double value)
{
	if ( m_ocilibStatement != NULL ) {
		if ( m_ParamType == DT_DOUBLE ) {
			// 実数（浮動小数点数）型をOracle 数値に変換します。
			sword result = m_ocilibStatement->m_ocilibConnect->m_ocilibGlobal->NumberFromReal( m_ocilibStatement->m_ocilibConnect->m_ociError, &value, sizeof(double), reinterpret_cast<OCINumber *>(m_FetchBuf) );
			if ( OCI_SUCCESS != result )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibStatement->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

			m_IsSetted = true;
		} else {
			if ( m_ocilibRecordset != NULL )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NG_INPUT_TYPE, _T(__FILE__), __LINE__);
		}
	}

	return *this;
}

ocilibParameter& ocilibParameter::operator =(long value)
{
	if ( m_ocilibStatement != NULL ) {
		if ( m_ParamType == DT_LONG ) {
			// 整数をOracle 数値に変換します。
			sword result = m_ocilibStatement->m_ocilibConnect->m_ocilibGlobal->NumberFromInt( m_ocilibStatement->m_ocilibConnect->m_ociError, &value, sizeof(long), OCI_NUMBER_SIGNED, reinterpret_cast<OCINumber *>(m_FetchBuf) );
			if ( OCI_SUCCESS != result )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error(result, m_ocilibStatement->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

			m_IsSetted = true;
		} else {
			if ( m_ocilibRecordset != NULL )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NG_INPUT_TYPE, _T(__FILE__), __LINE__);
		}
	}

	return *this;
}

ocilibParameter& ocilibParameter::operator =(const ocilibDate& d)
{
	if ( m_ParamType == DT_DATE ) {
		d.set(*reinterpret_cast<OCIDate*>(m_FetchBuf));

		m_IsSetted = true;
	} else {
		if ( m_ocilibRecordset != NULL )
			m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NG_INPUT_TYPE, _T(__FILE__), __LINE__);
	}

	return *this;
}

void ocilibParameter::Initialize()
{
	m_ocilibStatement = NULL;
	m_ocilibRecordset = NULL;

	m_ociBind = NULL;
	m_ociStmt = NULL;

	m_ParamType = DT_UNKNOWN;
	m_Size = 0;
	m_ociType = 0;
	m_IsSetted = false;
	m_FetchBuf = NULL;
	m_IsArray = false;
}

void ocilibParameter::Clear()
{
	if ( m_ociStmt != NULL ) {
		// ハンドルの割当てを明示的に解除します。
		m_ocilibStatement->m_ocilibConnect->m_ocilibGlobal->HandleFree(  m_ociStmt, OCI_HTYPE_STMT );
		m_ociStmt = NULL;
	}

	if ( m_ociBind != NULL )
		m_ociBind = NULL;

	m_ocilibStatement = NULL;

	if ( m_ocilibRecordset != NULL )
		delete m_ocilibRecordset;
	m_ocilibRecordset = NULL;

	if ( m_FetchBuf != NULL ) {
		delete [] m_FetchBuf;
		m_FetchBuf = NULL;
	}
}

bool ocilibParameter::Attach( ocilibStatement* Statement, LPCTSTR name, enumociDataType type, ub4 fetch_size )
{
	if ( Statement == NULL || name == NULL )
		return false;

	m_ParamName = name;

	if ( m_FetchBuf != NULL )
		delete [] m_FetchBuf;
	m_FetchBuf = NULL;

	m_IsSetted = false;

	m_ocilibStatement = NULL;
	m_ocilibRecordset = NULL;

	m_ociBind = NULL;
	m_ociStmt = NULL;

	Setup_type( name, type, fetch_size );

	if ( m_ParamType == DT_RECORDSET )
		Bind_Recordset( Statement );
	else
		Bind( Statement );

	return true;
}

bool ocilibParameter::Setup_type( LPCTSTR param_name, enumociDataType type, ub4 maxSize )
{
	if ( param_name == NULL )
		return false;

	const TCHAR	*p = param_name;

	if ( p[0] != L':' )
		return false;

	switch( type ) {
		case DT_LONG:
			m_ParamType = DT_LONG;
			m_ociType = SQLT_VNU;
			m_Size = sizeof(OCINumber);
			break;
		case DT_DOUBLE:
			m_ParamType = DT_DOUBLE;
			m_ociType = SQLT_VNU;
			m_Size = sizeof(OCINumber);
			break;
		case DT_TEXT:
			m_ParamType = DT_TEXT;
			m_ociType = SQLT_STR;
			m_Size = (ub4)(sizeof(char) * maxSize);
			break;
		case DT_DATE:
			m_ParamType = DT_DATE;
			m_ociType = SQLT_ODT;
			m_Size = sizeof(OCIDate);
			break;
		case DT_RECORDSET:
			m_ParamType = DT_RECORDSET;
			m_ociType = SQLT_RSET;
			m_Size = sizeof(OCIBind *);
			break;
		default:
			return false;
	}

	NEW( m_FetchBuf, m_Size * maxSize, char );

	return true;
}

bool ocilibParameter::Bind( ocilibStatement* Statement )
{
	if ( Statement == NULL )
		return false;

	switch( m_ParamType ) {
		case DT_LONG:
		case DT_DOUBLE:
			// Oracle 数値を0（ゼロ）に初期化します。
			Statement->m_ocilibConnect->m_ocilibGlobal->NumberSetZero( Statement->m_ocilibConnect->m_ociError, (OCINumber *)m_FetchBuf );
			break;
		case DT_DATE:
			// クライアントの現行のシステム日付およびシステム時刻を取得します。
			Statement->m_ocilibConnect->m_ocilibGlobal->DateSysDate( Statement->m_ocilibConnect->m_ociError, (OCIDate *)m_FetchBuf );
			break;
		case DT_TEXT:
			m_FetchBuf[0] = '\0';
			break;
		default:
			Statement->m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_ETC, _T(__FILE__), __LINE__, _T("サポート対象外の型です。") );
	}

	char*	szparam = NULL;
	long	size = 0;

	WtoM2( szparam, m_ParamName, size );

	// プログラム変数とSQL 文またはPL/SQL ブロック内のプレースホルダを関連付けます。
	sword result = Statement->m_ocilibConnect->m_ocilibGlobal->BindByName( Statement->m_ociStmt, &m_ociBind, Statement->m_ocilibConnect->m_ociError, (text *)szparam, -1, (text *)m_FetchBuf, m_Size, m_ociType, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT );
	delete [] szparam;
	if ( result != OCI_SUCCESS )
		Statement->m_ocilibConnect->m_ocilibError->Error(result, Statement->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__, m_ParamName );

	m_IsSetted = true;
	m_ocilibStatement = Statement;

	return true;
}

bool ocilibParameter::Bind_Recordset( ocilibStatement* Statement)
{
	if ( Statement == NULL )
		return false;

	sword	result;

	// 割当ておよび初期化済みハンドルのポインタを戻します
	result = Statement->m_ocilibConnect->m_ocilibGlobal->HandleAlloc( Statement->m_ocilibConnect->m_ociEnvironment, reinterpret_cast<void **>(&m_ociStmt), OCI_HTYPE_STMT, 0, NULL);
	if (result != OCI_SUCCESS)
		Statement->m_ocilibConnect->m_ocilibError->ErrorEnv(result, Statement->m_ocilibConnect->m_ociEnvironment, _T(__FILE__), __LINE__);

	char*	szparam = NULL;
	long	size = 0;

	WtoM2( szparam, m_ParamName, size );

	// プログラム変数とSQL 文またはPL/SQL ブロック内のプレースホルダを関連付けます。
	result = Statement->m_ocilibConnect->m_ocilibGlobal->BindByName( Statement->m_ociStmt, &m_ociBind, Statement->m_ocilibConnect->m_ociError, (text *)szparam, -1, &m_ociStmt, m_Size, m_ociType, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT );
	delete [] szparam;
	if (result != OCI_SUCCESS)
		Statement->m_ocilibConnect->m_ocilibError->Error(result, Statement->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__, m_ParamName );

	m_ocilibStatement = Statement;

	return true;
}

LPCTSTR ocilibParameter::GetChar()
{
	if ( m_ParamType == DT_TEXT && m_IsSetted ) {
		if ( IsSetted() ) {
			long	len = 0;

			MtoW(m_ocilibStatement->m_ocilibConnect->m_ptszData, m_FetchBuf, len);
			return m_ocilibStatement->m_ocilibConnect->m_ptszData;
		}
	}

	return NULL;
}

double ocilibParameter::GetDouble() const
{
	if ( m_ParamType == DT_DOUBLE && m_IsSetted ) {
		double	value;

		// Oracle 数値型を実数に変換します。
		sword result = m_ocilibStatement->m_ocilibConnect->m_ocilibGlobal->NumberToReal( m_ocilibStatement->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf), sizeof(double), &value );
		if ( OCI_SUCCESS != result )
			m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error(result, m_ocilibStatement->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

		return value;
	}

	return -1.0;
}

long ocilibParameter::GetLong() const
{
	if ( m_ParamType == DT_LONG && m_IsSetted ) {
		long	value;

		// Oracle 数値型を整数に変換します。
		sword result = m_ocilibStatement->m_ocilibConnect->m_ocilibGlobal->NumberToInt( m_ocilibStatement->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf), sizeof(long), OCI_NUMBER_SIGNED, &value );
		if ( OCI_SUCCESS != result )
			m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error(result, m_ocilibStatement->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

		return value;
	}

	return -1;
}

ocilibDate ocilibParameter::GetDatetime() const
{
	if ( m_ParamType == DT_DATE && m_IsSetted )
		return ocilibDate(*reinterpret_cast<OCIDate *>(m_FetchBuf));

	ocilibDate	ocilibdate( 9999, 99, 99, 99, 99, 99 );

	return ocilibdate;
}

ocilibRecordset& ocilibParameter::GetRecordset()
{
	if ( m_ocilibStatement != NULL ) {
		if ( m_ParamType == DT_RECORDSET ) {
			if ( !m_ocilibRecordset ) {
				m_ocilibRecordset = new ocilibRecordset( m_ocilibStatement->m_ocilibConnect, m_ociStmt );

				if ( m_ocilibRecordset != NULL )
					m_ocilibRecordset->FetchRows();
			}

			return (*m_ocilibRecordset);
		}
	}

	return (*m_ocilibRecordset);
}
