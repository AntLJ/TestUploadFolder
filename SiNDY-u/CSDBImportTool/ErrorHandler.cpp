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

#include "StdAfx.h"
#include "ErrorHandler.h"
#include <string>
#include<iostream>
#include<fstream>

CErrorHandler::CErrorHandler(void)
{
}

CErrorHandler::~CErrorHandler(void)
{
	time_t t;
	time( &t );
	struct tm *ltm = localtime( &t );
	m_fout << "# End:" << ltm->tm_year + 1900 << '/' << ltm->tm_mon + 1 << '/' << ltm->tm_mday << '/' <<  ' ' << ltm->tm_hour << ':' << ltm->tm_min << ':' << ltm->tm_sec;
}

void CErrorHandler::Init( LPCTSTR lpszLogFile )
{
	m_fout.open( lpszLogFile );

	time_t t;
	time( &t );
	struct tm *ltm = localtime( &t );
	m_fout << "# Start:" << ltm->tm_year + 1900 << '/' << ltm->tm_mon + 1 << '/' << ltm->tm_mday << '/' <<  ' ' << ltm->tm_hour << ':' << ltm->tm_min << ':' << ltm->tm_sec << std::endl;
}

bool CErrorHandler::CheckID( CString & strID )
{
	if( strID.GetLength() == 12 )
		return true;

	CString strErr( _T("異常終了。。\nおそらく、エスケープされていない単独の\"（ダブルコーテーション）がフィールド中の文字列に含まれるため、おかしなことになってます。") );
	m_fout << CT2A( strErr ) << std::endl;
	return false;
}

void CErrorHandler::FieldErr( const CString& strID, const CString& strFieldNames )
{
	CString strErr;
	strErr.Format( _T("不正なフィールド値：%s[ %s ]" ), strID, strFieldNames );

	m_fout << CT2A( strErr ) << std::endl;
}

void CErrorHandler::GeomErr( const CString& strID )
{
	CString strErr;
	strErr.Format( _T("ジオメトリ作成失敗：%s" ), strID );

	m_fout << CT2A( strErr ) << std::endl;
}

void CErrorHandler::SetResult( HRESULT hr, const CString &strFieldName, int nIndex, ERR_CODE emCode )
{
	if( hr == S_OK )
		return;

	CString strErr;
	switch( emCode )
	{
		case ERR_FIELD:
			strErr.Format( _T("フィールド書き込み失敗：%s.[%d]( hr=%d )"), strFieldName, nIndex, hr );
			break;
		case ERR_SHAPE:
			strErr.Format( _T("ジオメトリ作成失敗：%s.[Shape]( hr=%d )"), strFieldName,  hr );
			break;
		case ERR_STORE:
			strErr.Format( _T("ストア失敗：%s( hr=%d )"), strFieldName, hr );
			break;
		case ERR_DELETE:
			strErr.Format( _T("削除失敗：%s[OID:%d]( hr = %d )"), strFieldName, nIndex, hr );
			break;
		default:
			break;
	}

	m_fout << CT2A( strErr ) << std::endl;
}
