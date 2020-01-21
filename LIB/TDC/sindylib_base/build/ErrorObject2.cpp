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

/**
 * @file ErrorObject.cpp
 * @brief <b>CErrorObject、CErrorObjectsBase実装ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include <boost/foreach.hpp>
#include "ErrorObject2.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include <TDC/useful_headers/str_util.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define foreach BOOST_FOREACH

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR DISP_TABLE_AND_OID[]        = _T("テーブル[%s] OID[%d] : %s: %s（%s）\n");
const static _TCHAR SINDY_ERROR_LEVEL_INFO[]    = _T("情報");
const static _TCHAR SINDY_ERROR_LEVEL_WARNING[] = _T("警告");
const static _TCHAR SINDY_ERROR_LEVEL_ERROR[]   = _T("エラー");
#else
const static _TCHAR DISP_TABLE_AND_OID[]        = _T("Table[%s] OID[%d] : %s: %s（%s）\n");
const static _TCHAR SINDY_ERROR_LEVEL_INFO[]    = _T("Information");
const static _TCHAR SINDY_ERROR_LEVEL_WARNING[] = _T("Warning");
const static _TCHAR SINDY_ERROR_LEVEL_ERROR[]   = _T("Error");
#endif

namespace sindy {
using namespace errorcode;

CErrorInfoBase::CErrorInfoBase(){ init(); }
CErrorInfoBase::~CErrorInfoBase(){}
CErrorInfoBase& CErrorInfoBase::operator=( const stErrorInfoBase& stInfo ){ m_stInfo = stInfo; return *this; }
CErrorInfoBase& CErrorInfoBase::operator=( const CErrorInfoBase& cInfo ){ m_stInfo = cInfo.m_stInfo; return *this; }

CErrorInfoBase::CErrorInfoBase( sindyErrCode emErr, sindyErrLevel emLevel/* = sindyErrLevel_ERR*/, IGeometry* ipGeom/* = NULL*/, long lFieldIndex/* = -1*/, long lScale/* = -1*/, LPCTSTR lpcszFormat/* = NULL*/, ... )
{
	SetErrorCode( emErr );
	// emLevelに引数が与えられなかった場合を考慮して、emLevelの値に関わらずsindyErr_NoErrならINFOとする
	// -> 主にCErrorInfosBase::operator=( sindyErrCode e )対策
	SetErrorLevel( ( sindyErr_NoErr == emErr ) ? sindyErrLevel_INFO : emLevel );
	SetErrorGeometry( ipGeom );
	SetErrorField( lFieldIndex );
	SetErrorScale( lScale );
	va_list args = NULL;
	if( lpcszFormat )
	{
		va_start( args, lpcszFormat );
		SetErrorMessageV( lpcszFormat, args );
	}
	va_end(args);
}

void CErrorInfoBase::SetErrorMessage( LPCTSTR lpcszFormat, ... )
{
	m_stInfo.m_strMsg = _T("");
	va_list args = NULL;
	if( lpcszFormat )
	{
		va_start( args, lpcszFormat );
		SetErrorMessageV( lpcszFormat, args );
	}
	va_end(args);
}

void CErrorInfoBase::SetErrorMessageV( LPCTSTR lpcszFormat, va_list args )
{
	if( args )
	{
		m_stInfo.m_strMsg.FormatV( lpcszFormat, args );
/*		TCHAR szBuffer[2048] = _T("");
		LOGASSERTE_IF( 0 <= _vsntprintf( szBuffer, sizeof(TCHAR) * 2047, lpcszFormat, args ), sindyErr_BufferIsNotEnough );
		m_stInfo.m_strMsg = szBuffer;*/
	}
	else
		m_stInfo.m_strMsg = lpcszFormat;
}

void CErrorInfoBase::init()
{
	m_stInfo.m_lFieldIndex = -1;
	m_stInfo.m_emErr = sindyErr_NoErr;
	m_stInfo.m_lScale = -1;
	m_stInfo.m_emLevel = sindyErrLevel_INFO;
}

// 文字列化
CString CErrorInfoBase::ToString() const
{
	CString strErr;
	strErr.Format( _T("%s (%s)"), 
		SINDYERRCODE2STRING( GetErrorCode() ),
		CString(GetErrorMessage()).TrimRight( _T("\n") )
	);
	return strErr;
}

CErrorInfosBase::CErrorInfosBase(){}
CErrorInfosBase::~CErrorInfosBase(){}
CErrorInfosBase& CErrorInfosBase::operator=( const CErrorInfosBase& cInfos ){ m_listInfo = cInfos.m_listInfo; return *this; }
CSPErrorInfoBase CErrorInfosBase::push_back( const CSPErrorInfoBase& cInfo ){ m_listInfo.push_back( cInfo ); return *m_listInfo.rbegin(); }
void CErrorInfosBase::clear(){ m_listInfo.clear(); }
sindyErrCode CErrorInfosBase::GetDefaultErrCode( sindyErrLevel* pLevel/* = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	sindyErrLevel emLevel = sindyErrLevel_INFO;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		switch( (*it)->GetErrorLevel() )
		{
			case sindyErrLevel_WARN:
				if( sindyErr_NoErr == emErr )
				{
					emErr = (*it)->GetErrorCode();
					emLevel = sindyErrLevel_WARN;
				}
				break;
			case sindyErrLevel_ERR:
				emErr = (*it)->GetErrorCode();
				emLevel = sindyErrLevel_ERR;
				break;
			default:
				break;
		}
		if( sindyErrLevel_ERR == emLevel ) break;
	}
	if( pLevel ) *pLevel = emLevel;

	return emErr;
}

// 文字列化
CString CErrorInfosBase::ToString( bool bUnique /*= true*/ ) const
{
	std::vector<CString> msgs;
	for( const_iterator it = begin(); it != end(); ++it )
	{
		msgs.push_back( (*it)->ToString() );
	}

	if( bUnique )
		std::unique( msgs.begin(), msgs.end() );

	return uh::str_util::join( msgs, _T("\n") );
}

CErrorObjectBase::CErrorObjectBase(){}
CErrorObjectBase::~CErrorObjectBase(){}
CErrorObjectBase& CErrorObjectBase::operator=( const CErrorObjectBase& cErrObj )
{
	SetErrorObject( cErrObj.m_ObjData );
	m_cInfos = cErrObj.m_cInfos;

	return *this;
}

CErrorObjectBase::CErrorObjectBase( const CRowBase& cRow, const CSPErrorInfoBase& cInfo )
{
	SetErrorObject( cRow );
	push_back( cInfo );
}

CErrorObjectBase& CErrorObjectBase::operator=( const CRowBase& cRow ){ m_cInfos.clear(); SetErrorObject( cRow ); return *this; }

void CErrorObjectBase::SetErrorObject( const CRowBase& cRow )
{
	CGeometryBase cGeom;

	if ( ! cRow.IsDeleted() )
		cGeom = cRow.GetShapeCopy();

	SetErrorObject( boost::tuple<CString, long, CGeometryBase>(cRow.GetTableName(), cRow.GetOID(), cGeom) );
}

void CErrorObjectBase::SetErrorObject( const boost::tuple<CString, long, CGeometryBase>& obj )
{
	m_ObjData = obj;
}

void CErrorObjectBase::Print( FILE* out, sindyLogFormat emFormat/* = sindyLogFormat_Common*/, sindyLogLevel emLevel/* = sindyLogLevel_ERR*/ ) const
{
	LPCTSTR lpcszTableName = m_ObjData.get<0>();
	long lOID = m_ObjData.get<1>();

	foreach( const CSPErrorInfoBase& info, std::make_pair( begin(), end() ) )
	{
		if( out && emLevel <= info->GetErrorLevel() )
		{
			std::pair<double,double> pairP( -1, -1 );
			if( NULL != info->GetErrorGeometry() )
			{
				CGeometryBase cErrGeom( info->GetErrorGeometry() );
				CGeometryBase::const_point_iterator itPoint = cErrGeom.point_begin();
				if( itPoint != cErrGeom.point_end() )
					pairP = itPoint.get_coords();
			}
			else {
				if( (IGeometry*)m_ObjData.get<2>() )
				{
					CGeometryBase::const_point_iterator itPoint = m_ObjData.get<2>().point_begin();
					if( itPoint != m_ObjData.get<2>().point_end() )
						pairP = itPoint.get_coords();
				}
			}
			SINDYCOMMONLOG( out, emFormat, lpcszTableName, lOID, pairP.first, pairP.second, info->GetErrorScale(), info->GetErrorLevel(), info->GetErrorCode(), info->GetErrorMessage() );
		}
	}
}

CSPErrorInfoBase& CErrorObjectBase::push_back( const CSPErrorInfoBase& cInfo )
{
	m_cInfos.push_back( cInfo );
	return *m_cInfos.rbegin();
}

CErrorInfosBase& CErrorObjectBase::push_back( const CErrorInfosBase& cInfos )
{
	for( CErrorInfosBase::const_iterator it = cInfos.begin(); it != cInfos.end(); ++it )
		m_cInfos.push_back( *it );

	return m_cInfos;
}

CErrorObjectsBase::CErrorObjectsBase()
{
	init( true );
}

CErrorObjectsBase::~CErrorObjectsBase()
{
}

CSPErrorObjectBase& CErrorObjectsBase::push_back( const CSPErrorObjectBase& obj )
{
	if( m_bOutputInstantly && m_hFile ) obj->Print( m_hFile, m_emFormat, m_emLevel );
	m_listErrorObject.push_back( obj );
	return *m_listErrorObject.rbegin();
}

void CErrorObjectsBase::clear()
{
	m_listErrorObject.clear();
}

void CErrorObjectsBase::init( bool bIsConstructor/* = false*/ )
{
	if( ! bIsConstructor )
	{
		m_strErr = _T("");
		clear();
	}
	m_hFile = NULL;
	m_bOutputInstantly = false;
	m_emFormat = sindyLogFormat_Common;
	m_emLevel = sindyLogLevel_ERR;
}

errorcode::sindyErrCode CErrorObjectsBase::GetDefaultErrCode( sindyErrLevel* pLevel/* = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	sindyErrLevel emLevel = sindyErrLevel_INFO;

	foreach( const CSPErrorObjectBase& obj, std::make_pair( begin(), end() ) )
	{
		sindyErrLevel emLevelTmp = sindyErrLevel_INFO;
		sindyErrCode emErrTmp = obj->GetDefaultErrCode( &emLevelTmp );
		
		if( emLevel <= emLevelTmp )
		{
			emErr = emErrTmp;
			emLevel = emLevelTmp;
		}
		if( emLevel == sindyErrLevel_ERR )
			break;
	}
	if( pLevel ) *pLevel = emLevel;

	return emErr;
}

LPCTSTR CErrorObjectsBase::ToString()
{
	std::set<CString> setMsg;
	foreach( const CSPErrorObjectBase& obj, std::make_pair( begin(), end() ) )
	{
		foreach( const CSPErrorInfoBase& info, std::make_pair( obj->begin(), obj->end() ) )
		{
			if( sindyErr_NoErr != info->GetErrorCode() )
			{
				CString strMsg, strLevel;
				switch( info->GetErrorLevel() )
				{
					case sindyErrLevel_INFO: strLevel = SINDY_ERROR_LEVEL_INFO;    break;
					case sindyErrLevel_WARN: strLevel = SINDY_ERROR_LEVEL_WARNING; break;
					case sindyErrLevel_ERR:  strLevel = SINDY_ERROR_LEVEL_ERROR;   break;
					default: break;
				}
				strMsg.Format(DISP_TABLE_AND_OID, obj->GetErrorObject()->get<0>(), obj->GetErrorObject()->get<1>(), strLevel, SINDYERRCODE2STRING(info->GetErrorCode()), info->GetErrorMessage() );
				setMsg.insert( strMsg );
			}
		}
	}
	foreach( const CString& msg, setMsg )
	{
		m_strErr += msg;
	}

	return m_strErr;
}

} // sindy
