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

#ifndef	__OCILIBSTATEMENT_H_
#define	__OCILIBSTATEMENT_H_

#include "ocilibConnection.h"
#include "ocilibParameter.h"
#include "ocilibRecordset.h"

class ocilibConnection;
class ocilibParameter;
class ocilibRecordset;

class ocilibStatement {
public:
	ocilibStatement();
	ocilibStatement( ocilibConnection* Connect, LPCTSTR sql );

	virtual ~ocilibStatement();

	ocilibParameter& operator []( LPCTSTR name );
	ocilibParameter& operator []( ub4 Index );

public:
	ocilibParameter& Bind( LPCTSTR name, enumociDataType type = DT_UNKNOWN, ub4 maxSize = FETCH_SIZE );

	ocilibParameter& Bind( LPCTSTR name, LPCTSTR text, enumociDataType type = DT_TEXT, ub4 maxSize = FETCH_SIZE );
	ocilibParameter& Bind( LPCTSTR name, double value, enumociDataType type = DT_DOUBLE );
	ocilibParameter& Bind( LPCTSTR name, long value, enumociDataType type = DT_LONG );
	ocilibParameter& Bind( LPCTSTR name, const ocilibDate& d, enumociDataType type = DT_DATE );

	inline void Execute() { Execute_prepar(); };
	ocilibRecordset* Select();

	inline void Release() { delete this; };

private:
	void Initialize();
	void Clear();

	bool Prepare( LPCTSTR sql );
	bool Execute_prepar();

public:
	ocilibConnection*	m_ocilibConnect;
	OCIStmt*			m_ociStmt;

private:
	ub4					m_ociType;

	vector<ocilibParameter*>			m_vecParameters;
	map<_CSTRING_NS::CString, ocilibParameter*>	m_mapParameters;

	bool				m_IsPrepared;
	bool				m_IsExecuted;
};

#endif	// __OCILIBSTATEMENT_H_
