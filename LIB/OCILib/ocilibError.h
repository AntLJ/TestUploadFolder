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

#ifndef	__OCILIBERROR_H_
#define	__OCILIBERROR_H_

#include "ocilibGlobal.h"

class ocilibGlobal;

class ocilibError
{
public:
	enum enumErrCode {
		ECD_NO_MEMORY = 1000,
		ECD_UNSUPORT_ORA_TYPE,
		ECD_NG_PARAM_PREFIX,
		ECD_NG_INPUT_TYPE,
		ECD_NG_OUTPUT_TYPE,
		ECD_NOT_FOUND_PARAM,
		ECD_NOT_FOUND_COLUMN,
		ECD_ETC,
	};

	enum enumErrType {
		ETP_UNKNOWN = 0,
		ETP_ORACLE,
		ETP_WINAPI,
		ETP_OCILIB,
	};

	ocilibError();
	ocilibError(ocilibGlobal* Global);
	virtual ~ocilibError();

	void Error( sword err, OCIError* ociError, LPCTSTR source = NULL, long LineNo = -1, LPCTSTR format = NULL, ... );
	void ErrorEnv( sword err, OCIEnv* ociEnvironment, LPCTSTR source = NULL, long LineNo = -1, LPCTSTR format = NULL, ... );
	void Error2( sword err, LPCTSTR source = NULL, long LineNo = -1, LPCTSTR format = NULL, ... );

public:
	void ErrDisp();

	LPCTSTR GetError() { return m_Error; };
	sb4 GetOraErrorCode() { return m_ErrorOraCode; };

private:
	void oracle_error( sword err, OCIError* ociError, OCIEnv* ociEnvironment );
	void ocilib_error( sword err );
	void winapi_error();

	void FormatMsg( LPCTSTR format, va_list va );

private:
	ocilibGlobal*	m_ocilibGlobal;

	enumErrType		m_ErrorType;
	sword			m_ErrorCode;

	sb4				m_ErrorOraCode;
	unsigned long	m_ErrorWinapiCode;
	_CSTRING_NS::CString			m_Description;
	_CSTRING_NS::CString			m_Error;
	_CSTRING_NS::CString			m_Source;
	long			m_LineNo;
};

#endif	// __OCILIBERROR_H_
