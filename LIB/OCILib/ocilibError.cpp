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
#include "ocilibError.h"

ocilibError::ocilibError()
{
	m_ocilibGlobal = NULL;
}

ocilibError::ocilibError(ocilibGlobal* Global)
{
	m_ocilibGlobal = Global;
}

ocilibError::~ocilibError()
{
}

void ocilibError::Error( sword ora_err, OCIError* ociError, LPCTSTR source, long LineNo, LPCTSTR format, ... )
{
	oracle_error( ora_err, ociError, NULL );

	if ( format ) {
		va_list	va;

		va_start( va, format );

		FormatMsg( format, va );

		va_end( va );
	}

	m_ErrorType = ETP_ORACLE;
//	m_ErrorOraCode = ora_err;
	m_ErrorWinapiCode = 0;
	m_Source = source;
	m_LineNo = LineNo;

	ErrDisp();
}

void ocilibError::ErrorEnv( sword err, OCIEnv* ociEnvironment, LPCTSTR source, long LineNo, LPCTSTR format, ... )
{
	oracle_error( err, NULL, ociEnvironment );

	if ( format ) {
		va_list	va;

		va_start( va, format );

		FormatMsg( format, va );

		va_end( va );
	}

	m_ErrorType = ETP_ORACLE;
//	m_ErrorOraCode = err;
	m_ErrorWinapiCode = 0;
	m_Source = source;
	m_LineNo = LineNo;

	ErrDisp();
}

void ocilibError::Error2( sword err, LPCTSTR source, long LineNo, LPCTSTR format, ... )
{
	ocilib_error( err );

	if ( format ) {
		va_list	va;

		va_start( va, format );

		FormatMsg( format, va );

		va_end( va );
	}

	m_ErrorType = ETP_OCILIB;
	m_ErrorOraCode = err;
	m_ErrorWinapiCode = 0;
	m_Source = source;
	m_LineNo = LineNo;

	ErrDisp();
}

void ocilibError::ErrDisp()
{
	static TCHAR* error_types_text [] = { _T("Unknown"), _T("Oracle"), _T("Win32 API"), _T("OCILib") };

	m_Error.Format( _T("Source: %s, line: %d\r\nError Type: %s\r\nCode: %d\r\nOracle Error Code: %d\r\nWin32 API Error Code: %d\r\nDescription: %s\r\n"), m_Source, m_LineNo, error_types_text[m_ErrorType], m_ErrorCode, m_ErrorOraCode, m_ErrorWinapiCode, m_Description );
	OutputDebugString( m_Error );
}

void ocilibError::oracle_error( sword err, OCIError* ociError, OCIEnv* ociEnvironment )
{
	bool	IsDetails = false;

	m_ErrorCode = err;

	switch ( m_ErrorCode ) {
		case OCI_SUCCESS:
			m_Description = _T("(OCI_SUCCESS)");
			break;
		case OCI_SUCCESS_WITH_INFO:
			m_Description = _T("(OCI_SUCCESS_WITH_INFO)");
			IsDetails = true;
			break;
		case OCI_ERROR:
			m_Description = _T("(OCI_ERROR)");
			IsDetails = true;
			break;
		case OCI_NO_DATA:
			m_Description = _T("(OCI_NO_DATA)");
			IsDetails = true;
			break;
		case OCI_INVALID_HANDLE:
			m_Description = _T("(OCI_INVALID_HANDLE)");
			break;
		case OCI_NEED_DATA:
			m_Description = _T("(OCI_NEED_DATA)");
			break;
		case OCI_STILL_EXECUTING:
			m_Description = _T("(OCI_STILL_EXECUTING)");
			IsDetails = true;
			break;
		case OCI_CONTINUE:
			m_Description = _T("(OCI_CONTINUE)");
			break;
		default:
			m_Description = _T("(UNKNOWN)");
	}

	if ( IsDetails ) {
		TCHAR *error_text = NULL;

		NEW( error_text, CHAR_SIZE, TCHAR );

		if ( m_ocilibGlobal ) {
			if ( ociError )
				// 指定されたバッファ内のエラー・メッセージとOracle エラーを戻します。
				m_ocilibGlobal->ErrorGet( (dvoid *)ociError, (ub4)1, (text *)NULL, &m_ErrorOraCode, reinterpret_cast<text *>(error_text), 2048, OCI_HTYPE_ERROR ); 
			else {
				if ( ociEnvironment )
					// 指定されたバッファ内のエラー・メッセージとOracle エラーを戻します。
					m_ocilibGlobal->ErrorGet( (dvoid *)ociEnvironment, (ub4)1, (text *)NULL, &m_ErrorOraCode, reinterpret_cast<text *>(error_text), 2048, OCI_HTYPE_ENV );
			}

			m_Description += _T(" ");

			TCHAR*	szError = NULL;
			long	len = CHAR_SIZE;

			MtoW2(szError, error_text, len );
			m_Description += szError;

			delete [] szError;
		}

		delete [] error_text;
	} else
		m_ErrorOraCode = 0;
}

void ocilibError::ocilib_error( sword err )
{
	m_ErrorCode = err;

	switch ( m_ErrorCode ) {
		case ECD_NO_MEMORY:
			m_Description = _T("(ECD_NO_MEMORY) メモリ割り当てに失敗しました。");
			break;
		case ECD_UNSUPORT_ORA_TYPE:
			m_Description = _T("(ECD_UNSUPORT_ORA_TYPE) サポート対象外の型です。");
			break;
		case ECD_NG_PARAM_PREFIX:
			m_Description = _T("(ECD_NG_PARAM_PREFIX) パラメータープレフィックスが正しくありません。");
			break;
		case ECD_NG_INPUT_TYPE:
			m_Description = _T("(ECD_NG_INPUT_TYPE) 入力データタイプが正しくありません。");
			break;
		case ECD_NG_OUTPUT_TYPE:
			m_Description = _T("(ECD_NG_OUTPUT_TYPE) 出力データタイプが正しくありません。");
			break;
		case ECD_NOT_FOUND_PARAM:
			m_Description = _T("(ECD_PARAM_NOT_FOUND) ステートメントパラメーターが見つかりませんでした。");
			break;
		case ECD_NOT_FOUND_COLUMN:
			m_Description = _T("(ECD_COLUMN_NOT_FOUND) 列が見つかりませんでした。");
			break;
		case ECD_ETC:
			m_Description = _T("(ECD_ETC) 多分、ocilibのエラー･･･");
			break;
		default:
			m_Description = _T("(Unknown) Unknown Error");
	}
}

void ocilibError::winapi_error()
{
	DWORD	dwRet;
	TCHAR*	error_text = NULL;

	m_ErrorWinapiCode = GetLastError();

	dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL, m_ErrorWinapiCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&error_text, 0, NULL );
	if ( dwRet != 0 && error_text ) {
		m_Description = error_text;
		LocalFree( (HLOCAL)error_text );
	}
}

void ocilibError::FormatMsg( LPCTSTR format, va_list va )
{
	TCHAR*	buffer = (LPTSTR)_alloca(((CHAR_SIZE/2)*sizeof(TCHAR)) + sizeof(TCHAR));

	if ( buffer ) {
		_vsntprintf( buffer, CHAR_SIZE, format, va );

		m_Description += _T(": ");
		m_Description += buffer;
	}
}
