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
#include "ErrObj.h"
#include "AheMeshUtil.h"

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR HEADER_COMMENT = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tMESHCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{エラーレベル}\tCOMMENT{エラーメッセージ}\tCOMMENT{備考}\tCOMMENT{フリーコメント}\n");
#else
	LPCTSTR HEADER_COMMENT = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tMESHCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{Error Level}\tCOMMENT{Error Message}\tCOMMENT{Remarks}\tCOMMENT{Free Comment}\n");
#endif // SINDY_FOR_ENGLISH
}

CErrInfo::CErrInfo( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszMessage )
{
	m_lField = lField;
	m_cPoint.X = point.X;
	m_cPoint.Y = point.Y;
	m_lScale = scale;
	m_emLevel = emLevel;
	m_strMsg = lpcszMessage;
}

void CErrInfo::init( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... )
{
	va_list args = NULL;

	if( lpcszFormat )
		va_start( args, lpcszFormat );

	initV( lField, point, scale, emLevel, lpcszFormat, args );

	if( args )
		va_end( args );
}

void CErrInfo::initV( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, va_list args )
{
	m_lField = lField;
	m_cPoint.X = point.X;
	m_cPoint.Y = point.Y;
	m_lScale = scale;
	m_emLevel = emLevel;

	if( lpcszFormat )
		m_strMsg.FormatV( lpcszFormat, args );
	else
		m_strMsg.Empty();
}

CString CErrInfo::ToString() const
{
	// エラーレベル文字列作成
	CString strLevel;
	switch( m_emLevel )
	{
		case sindyeLogLevel_INFO: strLevel = _T("[INFO]"); break;
		case sindyeLogLevel_WARN: strLevel = _T("[WARN]"); break;
		case sindyeLogLevel_ERR:
		default:                  strLevel = _T("[ERR ]"); break;
	}
	CString strMsg;
	strMsg.Format(_T("%s %s"), strLevel, m_strMsg );
	return strMsg;

}

CString CErrInfo::ToLog( sindyeLogFormat emFormat ) const
{
	// エラーレベル文字列作成
	CString strLevel;
	switch( m_emLevel )
	{
		case sindyeLogLevel_INFO: strLevel = _T("INFO"); break;
		case sindyeLogLevel_WARN: strLevel = _T("WARN"); break;
		case sindyeLogLevel_ERR:
		default:                  strLevel = _T("ERR"); break;
	}
	CString strLog;
	switch( emFormat )
	{
		case sindyeLogFormat_Freestyle:
		{
			int x = 0, y = 0;
			unsigned int nMeshCode = AheLL2MESH( m_cPoint.X, m_cPoint.Y, 64, x, y );
			strLog.Format(_T("%lu\t%d\t%d\t%.20lf\t%.20lf\t%d\t%s\t%s\t"), nMeshCode, x, y, m_cPoint.Y, m_cPoint.X, m_lScale, strLevel, m_strMsg );
			break;
		}
		case sindyeLogFormat_Common:
		default:
			strLog.Format(_T("%.20lf,%.20lf,%s,%s,"), m_cPoint.X, m_cPoint.Y, strLevel, m_strMsg );
			break;
	}
	return strLog;
}


void CErrInfoContainer::add( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... )
{
	va_list args = NULL;

	if( lpcszFormat )
		va_start( args, lpcszFormat );

	addV( lField, point, scale, emLevel, lpcszFormat, args );

	if( args )
		va_end(args);
}

void CErrInfoContainer::addV( long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, va_list args )
{
	CString strMsg;
	if( lpcszFormat )
		strMsg.FormatV( lpcszFormat, args );

	push_back( CErrInfo( lField, point, scale, emLevel, strMsg ) );
}

CString CErrInfoContainer::ToString() const
{
	CString strMsg;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( strMsg.IsEmpty() )
			strMsg = _T("  ") + it->ToString();
		else
			strMsg = strMsg + _T("\n  ") + it->ToString();
	}
	return strMsg;
}

void CErrObj::init( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom )
{
	m_strTableName = lpcszTableName;
	m_lOID = lOID;
	m_ipGeom = ipGeom;
}

CString CErrObj::ToString() const
{
	CString strMsg;
	strMsg.Format(_T("[%s.%d]\n%s"), m_strTableName, m_lOID, CErrInfoContainer::ToString() );
	return strMsg;
}

void CErrObj::ToLog( sindyeLogFormat emFormat, std::list<CString>& listLog ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		CString strLog;
		// 出力文字列を作成する
		switch( emFormat )
		{
			case sindyeLogFormat_Freestyle:
				strLog.Format(_T("0\t%s\t%d\t%s"), m_strTableName, m_lOID, it->ToLog( emFormat ) );
				break;
			case sindyeLogFormat_Common:
			default:
				strLog.Format(_T("0,%s,%d,%s"), m_strTableName, m_lOID, it->ToLog( emFormat ) );
				break;
		}
		listLog.push_back( strLog );
	}
}


void CErrObjContainer::add( LPCTSTR lpcszTableName, long lOID, IGeometry* ipGeom, long lField, const WKSPoint& point, long scale, sindyeLogLevel emLevel, LPCTSTR lpcszFormat, ... )
{
	iterator it = find( lpcszTableName, lOID );
	if( it == end() )
	{
		push_back( CErrObj( lpcszTableName, lOID, ipGeom ) );
		it = begin();
		std::advance( it, size() - 1 );
	}

	if( lpcszFormat )
	{
		va_list args;
		va_start( args, lpcszFormat );

		it->addV( lField, point, scale, emLevel, lpcszFormat, args );

		va_end(args);
	}
	else
		it->add( lField, point, scale, emLevel, NULL );
}

CErrObjContainer::iterator CErrObjContainer::find( LPCTSTR lpcszTableName, long lOID )
{
	for( std::list<CErrObj>::iterator it = begin(); it != end(); ++it )
	{
		if( it->m_lOID == lOID &&
			0 == it->m_strTableName.CompareNoCase( lpcszTableName ) )
			return it;
	}
	return end();
}

CErrObjContainer::const_iterator CErrObjContainer::find( LPCTSTR lpcszTableName, long lOID ) const
{
	for( std::list<CErrObj>::const_iterator it = begin(); it != end(); ++it )
	{
		if( it->m_lOID == lOID &&
			0 == it->m_strTableName.CompareNoCase( lpcszTableName ) )
			return it;
	}
	return end();
}

CString CErrObjContainer::ToString() const
{
	CString strRet;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( strRet.IsEmpty() )
			strRet = it->ToString();
		else
			strRet = strRet + _T("\n") + it->ToString();
	}
	return strRet;
}

void CErrObjContainer::ToLog( sindyeLogFormat emFormat, FILE* out ) const
{
	// ヘッダ出力
	CString strHeader;
	switch( emFormat )
	{
		case sindyeLogFormat_Freestyle:
			strHeader= HEADER_COMMENT;
			break;
		case sindyeLogFormat_Common:
		default:
			strHeader = _T("# SINDYSTDLOG\n");
			break;
	}
	// ファイルに出力
	_ftprintf( out, strHeader );

	// リスト出力
	std::list<CString> listLog;
	ToLog( emFormat, listLog );
	for( std::list<CString>::const_iterator it = listLog.begin(); it != listLog.end(); ++it )
	{
		_ftprintf( out, *it );
		_ftprintf( out, _T("\n") );
	}
	fflush(out);
}

void CErrObjContainer::ToLog( sindyeLogFormat emFormat, std::list<CString>& listLog ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
		it->ToLog( emFormat, listLog );
}

sindyeLogLevel CErrObjContainer::GetMaxLogLevel() const
{
	sindyeLogLevel emRet = sindyeLogLevel_INFO; // 戻り値
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( CErrObj::const_iterator itLog = it->begin(); itLog != it->end(); ++itLog )
			emRet = max( emRet, itLog->m_emLevel );
	}
	return emRet;
}
