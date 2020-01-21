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

#ifndef	__OCILIBPARAMETER_H_
#define	__OCILIBPARAMETER_H_

#include "ocilibStatement.h"
#include "ocilibRecordset.h"
#include "ocilibDate.h"

class ocilibStatement;
class ocilibRecordset;
class ocilibDate;

class ocilibParameter {
public:
	ocilibParameter();
	ocilibParameter( ocilibStatement* Statement, LPCTSTR name, enumociDataType type = DT_UNKNOWN, ub4 fetch_size = FETCH_SIZE );

	ocilibParameter& operator =( LPCTSTR text );
	ocilibParameter& operator =( double value );
	ocilibParameter& operator =( long value );
	ocilibParameter& operator =( const ocilibDate& d );

	inline operator LPCTSTR() { return GetChar(); };
	inline operator double() const { return GetDouble(); };
	inline operator long() const { return GetLong(); };
	inline operator ocilibDate() const { return GetDatetime(); };
	inline operator ocilibRecordset&() { return GetRecordset(); };

	virtual ~ocilibParameter();

public:
	LPCTSTR GetChar();
	double GetDouble() const;
	long GetLong() const;
	ocilibDate GetDatetime() const;
	ocilibRecordset& GetRecordset();

	bool Bind( ocilibStatement* Statement );
	bool Bind_Recordset( ocilibStatement* Statement );

	inline bool IsSetted() const { return m_IsSetted; };

	LPCTSTR ParamName() { return m_ParamName; };

private:
	void Initialize();
	void Clear();

	bool Attach( ocilibStatement* Statement, LPCTSTR name, enumociDataType type = DT_UNKNOWN, ub4 fetch_size = FETCH_SIZE );
	bool Setup_type( LPCTSTR param_name, enumociDataType type = DT_UNKNOWN, ub4 maxSize = FETCH_SIZE );

public:
	ocilibStatement*	m_ocilibStatement;
	ocilibRecordset*	m_ocilibRecordset;

private:
	OCIBind*			m_ociBind;
	OCIStmt*			m_ociStmt;

	_CSTRING_NS::CString				m_ParamName;
	enumociDataType		m_ParamType;
	ub4					m_ociType;
	ub4					m_Size;

	bool				m_IsSetted;
	char*				m_FetchBuf;

	bool				m_IsArray;
};

#endif	// __OCILIBPARAMETER_H_
