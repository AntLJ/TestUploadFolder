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

#ifndef	__OCILIBCOLUMN_H_
#define	__OCILIBCOLUMN_H_

#include "ocilibRecordset.h"
#include "ocilibDate.h"

class ocilibRecordset;
class ocilibDate;

class ocilibColumn {
public:
	ocilibColumn();
	ocilibColumn( ocilibRecordset* Recordset, LPCTSTR name, enumociDataType ociType, ub4 maxSize, ub4 FetchSize = FETCH_SIZE);

	virtual ~ocilibColumn();

	inline operator LPCTSTR() { return GetChar(); };
	inline operator double() const { return GetDouble(); };
	inline operator long() const { return GetLong(); };
	inline operator ocilibDate() const { return GetDatetime(); };

public:
	bool IsNull() const;

	LPCTSTR GetChar();
	double GetDouble() const;
	long GetLong() const;
	ocilibDate GetDatetime() const;

	enumociDataType ColumnType() const { return m_ColumnType; };
	LPCTSTR ColumnName() { return m_ColumnName; };
	void Release(){};

private:
	void Initialize();
	void Cleanup();

public:
	ocilibRecordset*	m_ocilibRecordset;
	OCIDefine*			m_ociDef;
	sb2*				m_Indicators;
	ub2*				m_DataLen;
	TCHAR*				m_FetchBuf;

	ub4					m_ociType;
	int					m_Size;

private:
	_CSTRING_NS::CString				m_ColumnName;
	enumociDataType		m_ColumnType;
};

#endif	// __OCILIBCOLUMN_H_
