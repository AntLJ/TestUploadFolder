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
#include "ocilibColumn.h"

ocilibColumn::ocilibColumn()
{
	Initialize();
}

ocilibColumn::ocilibColumn( ocilibRecordset* Recordset, LPCTSTR name, enumociDataType ociType, ub4 maxSize, ub4 FetchSize)
{
	if ( Recordset != NULL && name != NULL ) {
		m_ColumnName = name;

		switch ( ociType ) {
			case DT_LONG:
				m_ociType = SQLT_VNU;
				m_ColumnType = DT_LONG;
				m_Size = sizeof(OCINumber);
				break;
			case DT_DOUBLE:
				m_ociType = SQLT_VNU;
				m_ColumnType = DT_DOUBLE;
				m_Size = sizeof(OCINumber);
				break;
			case DT_DATE:
				m_ociType = SQLT_ODT;
				m_ColumnType = DT_DATE;
				m_Size = sizeof(OCIDate);
				break;
			case DT_TEXT:
				m_ociType = SQLT_STR;
				m_ColumnType = DT_TEXT;
				m_Size = (maxSize + sizeof(TCHAR)) * sizeof(TCHAR);
				break;
			default:
				Recordset->m_ocilibConnect->m_ocilibError->Error2( ocilibError::ECD_UNSUPORT_ORA_TYPE, _T(__FILE__), __LINE__, name );
				return;
		}

		NEW( m_Indicators, FetchSize, sb2 );

		if ( m_ColumnType == DT_TEXT ) {
			NEW( m_DataLen, FetchSize, ub2 );
		} else
			m_DataLen = NULL;

		NEW( m_FetchBuf, (m_Size * FetchSize), TCHAR );

		m_ociDef = NULL;

		if ( !m_Indicators ) {
			Cleanup();
			Recordset->m_ocilibConnect->m_ocilibError->Error2( ocilibError::ECD_NO_MEMORY, _T(__FILE__), __LINE__, name );
		}

		m_ocilibRecordset = Recordset;
	}
}

ocilibColumn::~ocilibColumn()
{
	Cleanup();
}

void ocilibColumn::Initialize()
{
	m_ColumnType = DT_UNKNOWN;
	m_ociType = 0;
	m_Size = 0;
	m_Indicators = NULL;
	m_DataLen = NULL;
	m_FetchBuf = NULL;
	m_ociDef = NULL;
	m_ocilibRecordset = NULL;
}

void ocilibColumn::Cleanup()
{
	if ( m_Indicators != NULL ) {
		delete [] m_Indicators;
		m_Indicators = NULL;
	}

	if ( m_DataLen != NULL ) {
		delete [] m_DataLen;
		m_DataLen = NULL;
	}

	if ( m_FetchBuf != NULL ) {
		delete [] m_FetchBuf;
		m_FetchBuf = NULL;
	}
}

bool ocilibColumn::IsNull() const
{
	if ( m_ocilibRecordset != NULL ) {
		ub4	row_no = static_cast<ub4>( m_ocilibRecordset->CurrentRow() % m_ocilibRecordset->FetchCount() );

		return m_Indicators[row_no] == -1;
	}

	return false;
}

LPCTSTR ocilibColumn::GetChar()
{
	if ( m_ocilibRecordset != NULL ) {
		ub4	row_no = static_cast<ub4>( m_ocilibRecordset->CurrentRow() % m_ocilibRecordset->FetchCount() );

		if ( m_Indicators[row_no] != -1 ) {
			sword	result;

			switch ( m_ColumnType ) {
				case DT_TEXT:
					if ( !IsNull() ) {
						long		len = 0;

						MtoW(m_ocilibRecordset->m_ocilibConnect->m_ptszData, (LPCSTR)m_FetchBuf + m_Size * row_no, len);

						return m_ocilibRecordset->m_ocilibConnect->m_ptszData;
					}
					break;
				case DT_DOUBLE:
					{
					double		dbvalue = 0;
					long		len = CHAR_SIZE;
					char*		szDump = (LPSTR)_alloca((len*sizeof(char)) + sizeof(char));

					// Oracle ”’lŒ^‚ðŽÀ”‚É•ÏŠ·‚µ‚Ü‚·B
					result = m_ocilibRecordset->m_ocilibConnect->m_ocilibGlobal->NumberToReal( m_ocilibRecordset->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf) + row_no, sizeof(double), &dbvalue );
					if ( result != OCI_SUCCESS )
						m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibRecordset->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

					_gcvt( dbvalue, 12, szDump );

					MtoW(m_ocilibRecordset->m_ocilibConnect->m_ptszData, szDump, len);

					return m_ocilibRecordset->m_ocilibConnect->m_ptszData;
					}
				case DT_LONG:
					{
					boolean		bIs = false;
					long		len = CHAR_SIZE;
					char*		szDump = (LPSTR)_alloca((len*sizeof(char)) + sizeof(char));

					result = m_ocilibRecordset->m_ocilibConnect->m_ocilibGlobal->NumberIsInt( m_ocilibRecordset->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf) + row_no, &bIs );
					if ( result != OCI_SUCCESS )
						m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibRecordset->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

					if ( bIs == false ) {
						char*	szFmt = NULL;
						long	FmtLen = 0;

						WtoM2( szFmt, NUMBER_FORMAT_INT, FmtLen );

						result = m_ocilibRecordset->m_ocilibConnect->m_ocilibGlobal->NumberToText( m_ocilibRecordset->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf) + row_no, reinterpret_cast<const unsigned char*>(szFmt), FmtLen, NULL, 0, (ub4*)&len, (text*)szDump );
						if ( result != OCI_SUCCESS )
							m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibRecordset->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

						delete [] szFmt;
					} else {
						ub4		value = 0;

						// Oracle ”’lŒ^‚ð®”‚É•ÏŠ·‚µ‚Ü‚·B
						result = m_ocilibRecordset->m_ocilibConnect->m_ocilibGlobal->NumberToInt( m_ocilibRecordset->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf) + row_no, sizeof(long), OCI_NUMBER_UNSIGNED, &value );
						if ( result != OCI_SUCCESS )
							m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibRecordset->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

						_ltoa( value, szDump, 10 );
					}

					MtoW(m_ocilibRecordset->m_ocilibConnect->m_ptszData, szDump, len);

					return m_ocilibRecordset->m_ocilibConnect->m_ptszData;
					}
				case DT_DATE:
					{
					long		len = CHAR_SIZE;
					TCHAR*		szDump = (LPTSTR)_alloca((len*sizeof(TCHAR)) + sizeof(TCHAR));

					ocilibDate		ocilibdate( *(reinterpret_cast<OCIDate *>(m_FetchBuf) + row_no) );

					wsprintf( szDump, _T("%04u/%02u/%02u %02u:%02u:%02u"), ocilibdate.Year(), ocilibdate.Mon(), ocilibdate.Day(), ocilibdate.Hour(), ocilibdate.Minute(), ocilibdate.Second() );

					return _CSTRING_NS::CString(szDump);
					}
			}
		}
	}

	return NULL;
}

double ocilibColumn::GetDouble() const
{
	if ( m_ocilibRecordset != NULL ) {
		ub4	row_no = static_cast<ub4>( m_ocilibRecordset->CurrentRow() % m_ocilibRecordset->FetchCount() );

		if ( m_ColumnType == DT_DOUBLE && m_Indicators[row_no] != -1 ) {
			double	value;
			sword	result;

			// Oracle ”’lŒ^‚ðŽÀ”‚É•ÏŠ·‚µ‚Ü‚·B
			result = m_ocilibRecordset->m_ocilibConnect->m_ocilibGlobal->NumberToReal( m_ocilibRecordset->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf) + row_no, sizeof(double), &value );
			if ( result != OCI_SUCCESS )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibRecordset->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

			return value;
		}
	}

	return -1.0;
}

long ocilibColumn::GetLong() const
{
	if ( m_ocilibRecordset != NULL ) {
		ub4	row_no = static_cast<ub4>( m_ocilibRecordset->CurrentRow() % m_ocilibRecordset->FetchCount() );

		if ( m_ColumnType == DT_LONG && m_Indicators[row_no] != -1 ) {
			long	value;
			sword	result;

			// Oracle ”’lŒ^‚ð®”‚É•ÏŠ·‚µ‚Ü‚·B
			result = m_ocilibRecordset->m_ocilibConnect->m_ocilibGlobal->NumberToInt( m_ocilibRecordset->m_ocilibConnect->m_ociError, reinterpret_cast<OCINumber *>(m_FetchBuf) + row_no, sizeof(long), OCI_NUMBER_UNSIGNED, &value );
			if ( result != OCI_SUCCESS )
				m_ocilibRecordset->m_ocilibConnect->m_ocilibError->Error( result, m_ocilibRecordset->m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return value;
		}
	}

	return -1;
}

ocilibDate ocilibColumn::GetDatetime() const
{
	if ( m_ocilibRecordset != NULL ) {
		ub4	row_no = static_cast <ub4>( m_ocilibRecordset->CurrentRow() % m_ocilibRecordset->FetchCount() );
		if ( m_ColumnType == DT_DATE && m_Indicators[row_no] != -1 )
			return ocilibDate(*(reinterpret_cast<OCIDate *>(m_FetchBuf) + row_no));
	}

	ocilibDate		ocilibdate( 9999, 99, 99, 99, 99, 99 );

	return ocilibdate;
}
