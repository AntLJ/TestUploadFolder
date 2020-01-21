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

#ifndef	__OCILIBRECORDSET_H_
#define	__OCILIBRECORDSET_H_

#include "ocilibConnection.h"
#include "ocilibStatement.h"
#include "ocilibColumn.h"

class ocilibConnection;
class ocilibStatement;
class ocilibColumn;

class ocilibRecordset {
public:
	ocilibRecordset();
	ocilibRecordset( ocilibConnection* Connect, OCIStmt* ociStmt, ub4 FetchSize = FETCH_SIZE);

	ocilibColumn& operator [](LPCTSTR ColumnName);
	ocilibColumn& operator [](ub4 ColumnIndex);

	inline bool operator ++() { return Next(); };

	virtual ~ocilibRecordset();

public:
	bool IsEOF() const { return (m_CurrentRow >= m_RowsFetched && m_IsEOF); };
	bool Next();
	inline void Release() { delete this; };

	ub4 ColumnCount() const;
	ub4 RowCount() const;
	ub4 CurrentRow() const { return m_CurrentRow; };
	ub4 FetchCount() const { return m_FetchCount; };

	void GetColumns(vector<LPTSTR>* vec);
#ifdef _USE_HASH_MAP
	void GetColumns(hash_map<long, LPTSTR>* pmap);
#else
	void GetColumns(map<long, LPTSTR>* pmap);
#endif

	void AttachStatement( ocilibStatement* Statement );
	void FetchRows();

private:
	void Initialize();
	void Clear();

	void Attach( ocilibConnection* Connect, OCIStmt* ociStmt, ub4 FetchSize = FETCH_SIZE );
	void Setting();

public:
	ocilibConnection*	m_ocilibConnect;
	ocilibStatement*	m_ocilibStatement;

private:
	OCIStmt*			m_ociStmt;

	vector<ocilibColumn*>				m_vecColumns;
	map<_CSTRING_NS::CString, ocilibColumn*>	m_mapColumns;

	ub4				m_FetchCount;
	ub4				m_RowsFetched;
	ub4				m_CurrentRow;

	bool			m_IsEOF;
	bool			m_IsSetting;
};

#endif	// __OCILIBRECORDSET_H_
