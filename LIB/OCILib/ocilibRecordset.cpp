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
#include "ocilibRecordset.h"

ocilibRecordset::ocilibRecordset()
{
}

ocilibRecordset::ocilibRecordset( ocilibConnection* Connect, OCIStmt* ociStmt, ub4 FetchSize )
{
	Initialize();

	try {
		Attach( Connect, ociStmt, FetchSize );
	} catch(...) {
		Clear();
	}
}

ocilibRecordset::~ocilibRecordset()
{
	Clear();
}

ocilibColumn& ocilibRecordset::operator [](LPCTSTR ColumnName)
{
	map<_CSTRING_NS::CString, ocilibColumn*>::iterator it = m_mapColumns.find(ColumnName);

	if ( it == m_mapColumns.end() )
		m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NOT_FOUND_COLUMN, _T(__FILE__), __LINE__, ColumnName);

	return (*(it->second));
}

ocilibColumn& ocilibRecordset::operator [](ub4 ColumnIndex)
{
	if ( ColumnIndex < FIRST_COLUMN || ColumnIndex > (unsigned)m_vecColumns.size() )
		m_ocilibConnect->m_ocilibError->Error2(ocilibError::ECD_NOT_FOUND_COLUMN, _T(__FILE__), __LINE__, _T("%d"), (int)ColumnIndex);

	return (*(m_vecColumns.at(ColumnIndex - FIRST_COLUMN)));
}

void ocilibRecordset::GetColumns(vector<LPTSTR>* vec)
{
	if ( vec == NULL )
		return;

	LPCTSTR	lpsz = NULL;
	TCHAR*	sz = NULL;

	vector<LPTSTR>().swap( (*vec) );
	(*vec).clear();
	if ( !m_vecColumns.empty() ) {
		vector<ocilibColumn*>::const_iterator First = m_vecColumns.begin();
		vector<ocilibColumn*>::const_iterator End = m_vecColumns.end();
		for ( vector<ocilibColumn*>::const_iterator it = First; it != End; ++it ) {
			lpsz = NULL;
			sz = NULL;

			lpsz = (ocilibColumn&)(*(*it));
			if ( lpsz != NULL ) {
				NEW(sz, _tcslen(lpsz), TCHAR);
				_tcscpy( sz, lpsz );
			}

			(*vec).push_back( sz );
		}
	}

	return;
}

#ifdef _USE_HASH_MAP
void ocilibRecordset::GetColumns(hash_map<long, LPTSTR>* pmap)
#else
void ocilibRecordset::GetColumns(map<long, LPTSTR>* pmap)
#endif
{
	if ( pmap == NULL )
		return;

	LPCTSTR	lpsz = NULL;
	TCHAR*	sz = NULL;
	long	iCnt = 0;

	(*pmap).clear();
	if ( !m_vecColumns.empty() ) {
		vector<ocilibColumn*>::const_iterator First = m_vecColumns.begin();
		vector<ocilibColumn*>::const_iterator End = m_vecColumns.end();
		for ( vector<ocilibColumn*>::const_iterator it = First; it != End; ++it, ++iCnt ) {
			lpsz = NULL;
			sz = NULL;

			lpsz = (ocilibColumn&)(*(*it));
			if ( lpsz != NULL ) {
				NEW(sz, _tcslen(lpsz), TCHAR);
				_tcscpy( sz, lpsz );
			}

			(*pmap)[iCnt] = sz;
		}
	}

	return;
}

void ocilibRecordset::Initialize()
{
	m_ocilibConnect = NULL;
	m_ocilibStatement = NULL;

	m_ociStmt = NULL;

	m_FetchCount = 0;
	m_RowsFetched = 0;
	m_CurrentRow = 0;

	m_IsEOF = false;
	m_IsSetting = false;

	if ( !m_vecColumns.empty() ) {
		vector<ocilibColumn*>::iterator First = m_vecColumns.begin();
		vector<ocilibColumn*>::iterator End = m_vecColumns.end();
		for ( vector<ocilibColumn*>::iterator it = First; it != End; ++it )
			delete (*it);
	}
	vector<ocilibColumn*>().swap( m_vecColumns );
	m_vecColumns.clear();

	m_mapColumns.clear();
}

void ocilibRecordset::Clear()
{
	if ( m_ociStmt != NULL )
		m_ociStmt = NULL;

	if ( m_ocilibStatement != NULL ) {
		m_ocilibStatement->Release();
		m_ocilibStatement = NULL;
	}

	if ( !m_vecColumns.empty() ) {
		vector<ocilibColumn*>::iterator First = m_vecColumns.begin();
		vector<ocilibColumn*>::iterator End = m_vecColumns.end();
		for ( vector<ocilibColumn*>::iterator it = First; it != End; ++it )
			delete (*it);
	}
	vector<ocilibColumn*>().swap( m_vecColumns );
	m_vecColumns.clear();

	m_mapColumns.clear();
}

void ocilibRecordset::Attach( ocilibConnection* Connect, OCIStmt* ociStmt, ub4 FetchSize)
{
	m_ocilibConnect = Connect;
	m_ocilibStatement = NULL;

	m_ociStmt = ociStmt;

	m_FetchCount = FetchSize;
	m_RowsFetched = 0;
	m_CurrentRow = 0;

	m_IsEOF = false;
	m_IsSetting = false;

	Setting();
}

ub4 ocilibRecordset::ColumnCount() const
{
	sword	result;
	ub4		count = 0;

	if ( m_ociStmt == NULL )
		return count;

	// ハンドルの特定の属性を取得します。
	result = m_ocilibConnect->m_ocilibGlobal->AttrGet( m_ociStmt, OCI_HTYPE_STMT, (dvoid*)&count, NULL, OCI_ATTR_PARAM_COUNT, m_ocilibConnect->m_ociError );

	if ( result != OCI_SUCCESS )
		m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

	return count;
}

ub4	ocilibRecordset::RowCount() const
{
	sword	result;
	ub4		count = 0;

	if ( m_ociStmt == NULL || m_ocilibConnect->m_ociError == NULL )
		return count;

	// ハンドルの特定の属性を取得します。
	result = m_ocilibConnect->m_ocilibGlobal->AttrGet( m_ociStmt, OCI_HTYPE_STMT, (dvoid*)&count, NULL, OCI_ATTR_ROW_COUNT, m_ocilibConnect->m_ociError );

	if ( result != OCI_SUCCESS )
		m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);

	return count;
}

void ocilibRecordset::Setting()
{
	sword			result;
	ocilibColumn*	Column = NULL;
	OCIParam*		param_handle = NULL;
	text*			param_name = NULL;
	ub4				name_len = 0, size = 0, /*size2 = 0,size3 = 0,size4 = 0,size5 = 0,size6 = 0,*/count = ColumnCount(), position = 1;
	ub4				oci_type = 0, oci_type2 = 0;//, otype=0;
	TCHAR*			szName = NULL;
	long			len = 0;
	_CSTRING_NS::CString	strTmp;

	if ( m_ociStmt == NULL )
		return;

	if ( !m_vecColumns.empty() ) {
		vector<ocilibColumn*>::iterator First = m_vecColumns.begin();
		vector<ocilibColumn*>::iterator End = m_vecColumns.end();
		for ( vector<ocilibColumn*>::iterator it = First; it != End; ++it )
			delete (*it);
	}
	vector<ocilibColumn*>().swap( m_vecColumns );
	m_vecColumns.clear();

	m_mapColumns.clear();

	//	m_vecColumns.reserve( count );
	for ( ub4 iCnt = 0; iCnt < count; ++iCnt ) {
		Column = NULL;
		param_handle = NULL;
		param_name = NULL;
		name_len = 0;
		oci_type = 0;
		oci_type2 = 0;
		size = 0;

		// 記述ハンドルまたは文ハンドル内の指定位置にあるパラメータの記述子を戻します。
		result = m_ocilibConnect->m_ocilibGlobal->ParamGet( m_ociStmt, OCI_HTYPE_STMT, m_ocilibConnect->m_ociError, reinterpret_cast<void **>(&param_handle), iCnt + 1 );
		if ( result != OCI_SUCCESS ) {
			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return;
		}

		// ハンドルの特定の属性を取得します。
		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &param_name, &name_len, OCI_ATTR_NAME, m_ocilibConnect->m_ociError );
		if ( result != OCI_SUCCESS ) {
			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return;
		}

		// ハンドルの特定の属性を取得します。
		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &oci_type, NULL, OCI_ATTR_DATA_TYPE, m_ocilibConnect->m_ociError );
		if ( result != OCI_SUCCESS ) {
			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return;
		}

//		otype = oci_type;

		if ( oci_type == SQLT_NUM ) {
			// ハンドルの特定の属性を取得します。
			result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &oci_type2, NULL, OCI_ATTR_SCALE, m_ocilibConnect->m_ociError );
			if ( result != OCI_SUCCESS ) {
				m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
				return;
			}

			if ( oci_type2 == 0 )
				oci_type = DT_LONG;
			else
				oci_type = DT_DOUBLE;
		} else {
			switch ( oci_type ) {
				case SQLT_CHR:
				case SQLT_STR:
				case SQLT_VCS:
				case SQLT_AFC:
				case SQLT_AVC:
				case SQLT_VST:
					oci_type = DT_TEXT;
					break;
				case SQLT_DAT:
				case SQLT_ODT:
					oci_type = DT_DATE;
					break;
				default:
					oci_type = DT_UNKNOWN;
					break;
			}
		}

		// ハンドルの特定の属性を取得します。
		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &size, NULL, OCI_ATTR_DATA_SIZE, m_ocilibConnect->m_ociError );
		if ( result != OCI_SUCCESS ) {
			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
			return;
		}

//		// ハンドルの特定の属性を取得します。
//		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &size2, NULL, OCI_ATTR_CHAR_SIZE, m_ocilibConnect->m_ociError );
//		if ( result != OCI_SUCCESS ) {
//			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
//			return;
//		}
//
//		// ハンドルの特定の属性を取得します。
//		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &size3, &size4, OCI_ATTR_DATA_SIZE, m_ocilibConnect->m_ociError );
//		if ( result != OCI_SUCCESS ) {
//			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
//			return;
//		}
//
//		// ハンドルの特定の属性を取得します。
//		result = m_ocilibConnect->m_ocilibGlobal->AttrGet( param_handle, OCI_DTYPE_PARAM, &size5, &size6, OCI_ATTR_CHAR_SIZE, m_ocilibConnect->m_ociError );
//		if ( result != OCI_SUCCESS ) {
//			m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
//			return;
//		}

		if ( param_handle != NULL )
			// 割当て済み記述子の割当てを解除します。
			m_ocilibConnect->m_ocilibGlobal->DescriptorFree( param_handle, OCI_DTYPE_PARAM );

		MtoW2( szName, (LPCSTR)param_name, len );

//		strTmp.Format( _T("type:%02u, ds:%02u, cs:%02u, ds:(%02u, %02u), cs:(%02u, %02u), name:%s\n"), otype, size, size2, size3, size4, size5, size6, szName );
//		OutputDebugString( strTmp );

		Column = new ocilibColumn( this, szName, (enumociDataType)oci_type, (ub4)size, m_FetchCount );

		delete [] szName;

		m_vecColumns.push_back( Column );
		m_mapColumns[Column->ColumnName()] = Column;

		// 選択リスト内の項目を型と出力データ・バッファに関連付けます。
		result = m_ocilibConnect->m_ocilibGlobal->DefineByPos( m_ociStmt, &(Column->m_ociDef), m_ocilibConnect->m_ociError, position++, Column->m_FetchBuf, Column->m_Size, Column->m_ociType, Column->m_Indicators, Column->m_DataLen, NULL, OCI_DEFAULT );
		if ( result != OCI_SUCCESS ) {
			m_ocilibConnect->m_ocilibError->Error( result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__, Column->ColumnName() );
			return;
		}
	}

	m_IsSetting = true;
}

void ocilibRecordset::AttachStatement( ocilibStatement* Stat )
{
	m_ocilibStatement = Stat;
}

void ocilibRecordset::FetchRows()
{
	sword	result;

	if ( m_ociStmt == NULL )
		return;

	// （スクロール可能な）結果セットから行をフェッチします
	result = m_ocilibConnect->m_ocilibGlobal->StmtFetch( m_ociStmt, m_ocilibConnect->m_ociError, m_FetchCount, OCI_FETCH_NEXT, OCI_DEFAULT );
	if ( result == OCI_SUCCESS || result == OCI_NO_DATA || result == OCI_SUCCESS_WITH_INFO ) {
		ub4		old_rows_count = m_RowsFetched;

		m_RowsFetched = RowCount();

		if (m_RowsFetched - old_rows_count != m_FetchCount)
			m_IsEOF = true;
		else
			m_IsEOF = false;
	} else
		m_ocilibConnect->m_ocilibError->Error(result, m_ocilibConnect->m_ociError, _T(__FILE__), __LINE__);
}

bool ocilibRecordset::Next()
{
	++m_CurrentRow;

	if ( m_CurrentRow >= m_RowsFetched ) {
		if ( !m_IsEOF )
			FetchRows();
		else
			return false;
	}

	if ( m_CurrentRow >= m_RowsFetched )
		return false;

	return true;
}
