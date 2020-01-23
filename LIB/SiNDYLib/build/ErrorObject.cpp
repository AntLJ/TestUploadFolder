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
 * @brief <b>CErrorObject、CErrorObjects実装ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "ErrorObject.h"
#include "Row.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR INFO_MSG    = _T("情報"); 
	LPCTSTR WARNING_MSG = _T("警告"); 
	LPCTSTR ERROR_MSG   = _T("エラー"); 
	LPCTSTR TABLE_INFO  = _T("テーブル[%s] OID[%d] : %s: %s（%s）\n"); 
#else
	LPCTSTR INFO_MSG    = _T("Information"); 
	LPCTSTR WARNING_MSG = _T("Warning"); 
	LPCTSTR ERROR_MSG   = _T("Error"); 
	LPCTSTR TABLE_INFO  = _T("Table [%s] OID [%d]: %s: %s(%s)\n"); 
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
using namespace errorcode;

CErrorInfo::CErrorInfo(){ init(); }
CErrorInfo::~CErrorInfo(){}
CErrorInfo& CErrorInfo::operator=( const stErrorInfo& stInfo ){ m_stInfo = stInfo; return *this; }
CErrorInfo& CErrorInfo::operator=( const CErrorInfo& cInfo ){ m_stInfo = cInfo.m_stInfo; return *this; }

CErrorInfo::CErrorInfo( sindyErrCode emErr, sindyErrLevel emLevel/* = sindyErrLevel_ERR*/, IGeometry* ipGeom/* = NULL*/, long lFieldIndex/* = -1*/, long lScale/* = -1*/, LPCTSTR lpcszFormat/* = NULL*/, ... )
{
	SetErrorCode( emErr );
	// emLevelに引数が与えられなかった場合を考慮して、emLevelの値に関わらずsindyErr_NoErrならINFOとする
	// -> 主にCErrorInfos::operator=( sindyErrCode e )対策
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

void CErrorInfo::SetErrorMessage( LPCTSTR lpcszFormat, ... )
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

void CErrorInfo::SetErrorMessageV( LPCTSTR lpcszFormat, va_list args )
{
	if( args )
	{
		TCHAR szBuffer[2048] = _T("");
		// [Bug 7667]タイトルへ幅をはるかに超える値を入力→保存ボタン押下で落ちる
		// sizeof(TCHAR) * 2047だと、2 * 2047 = 4094バイトの書き込み行っちゃう
		LOGASSERTE_IF( 0 <= _vsntprintf( szBuffer, 2047, lpcszFormat, args ), sindyErr_BufferIsNotEnough );
		m_stInfo.m_strMsg = szBuffer;
	}
	else
		m_stInfo.m_strMsg = lpcszFormat;
}

void CErrorInfo::init()
{
	m_stInfo.m_lFieldIndex = -1;
	m_stInfo.m_emErr = sindyErr_NoErr;
	m_stInfo.m_lScale = -1;
	m_stInfo.m_emLevel = sindyErrLevel_INFO;
}

CErrorInfos::CErrorInfos(){}
CErrorInfos::~CErrorInfos(){}
CErrorInfos& CErrorInfos::operator=( const CErrorInfos& cInfos ){ m_listInfo = cInfos.m_listInfo; return *this; }
CErrorInfo& CErrorInfos::push_back( const CErrorInfo& cInfo ){ m_listInfo.push_back( cInfo ); return *m_listInfo.rbegin(); }
void CErrorInfos::clear(){ m_listInfo.clear(); }
sindyErrCode CErrorInfos::GetDefaultErrCode( sindyErrLevel* pLevel/* = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	sindyErrLevel emLevel = sindyErrLevel_INFO;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		switch( it->GetErrorLevel() )
		{
			case sindyErrLevel_WARN:
				if( sindyErr_NoErr == emErr )
				{
					emErr = it->GetErrorCode();
					emLevel = sindyErrLevel_WARN;
				}
				break;
			case sindyErrLevel_ERR:
				emErr = it->GetErrorCode();
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

CErrorObject::CErrorObject(){}
CErrorObject::~CErrorObject(){}
CErrorObject& CErrorObject::operator=( const CErrorObject& cErrObj )
{
	SetErrorObject( cErrObj.m_ObjData );
	m_cInfos = cErrObj.m_cInfos;

	return *this;
}

CErrorObject::CErrorObject( const CRow& cRow, const CErrorInfo& cInfo )
{
	SetErrorObject( cRow );
	push_back( cInfo );
}

CErrorObject& CErrorObject::operator=( const CRow& cRow ){ m_cInfos.clear(); SetErrorObject( cRow ); return *this; }

void CErrorObject::SetErrorObject( const CRow& cRow )
{
	CGeometry cGeom;

	if ( ! cRow.IsDeleted() ) {
		CRow cDmy = cRow; // TODO: コピー取る必要ある？？
		cDmy.CreateCache( true );
		if ( cDmy.GetShape() )
			cGeom = (IGeometry*)*cDmy.GetShape();
	}

	SetErrorObject( boost::tuple<CString, long, CGeometry>(cRow.GetTableName(), cRow.GetOID(), cGeom) );
}

void CErrorObject::SetErrorObject( const boost::tuple<CString, long, CGeometry>& obj )
{
	m_ObjData = obj;
}

void CErrorObject::Print( FILE* out, sindyLogFormat emFormat/* = sindyLogFormat_Common*/, sindyLogLevel emLevel/* = sindyLogLevel_ERR*/ ) const
{
	LPCTSTR lpcszTableName = m_ObjData.get<0>();
	long lOID = m_ObjData.get<1>();

	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( out && emLevel <= it->GetErrorLevel() )
		{
			std::pair<double,double> pairP( -1, -1 );
			if( NULL != it->GetErrorGeometry() )
			{
				CGeometry cErrGeom( it->GetErrorGeometry() );
				CGeometry::const_point_iterator itPoint = cErrGeom.point_begin();
				if( itPoint != cErrGeom.point_end() )
					pairP = itPoint.get_coords();
			}
			else {
				if( (IGeometry*)m_ObjData.get<2>() )
				{
					CGeometry::const_point_iterator itPoint = m_ObjData.get<2>().point_begin();
					if( itPoint != m_ObjData.get<2>().point_end() )
						pairP = itPoint.get_coords();
				}
			}
			SINDYCOMMONLOG( out, emFormat, lpcszTableName, lOID, pairP.first, pairP.second, it->GetErrorScale(), it->GetErrorLevel(), it->GetErrorCode(), it->GetErrorMessage() );
		}
	}
}

CErrorInfo& CErrorObject::push_back( const CErrorInfo& cInfo )
{
	m_cInfos.push_back( cInfo );
	return *m_cInfos.rbegin();
}

CErrorInfos& CErrorObject::push_back( const CErrorInfos& cInfos )
{
	for( CErrorInfos::const_iterator it = cInfos.begin(); it != cInfos.end(); ++it )
		m_cInfos.push_back( *it );

	return m_cInfos;
}

CErrorObjects::CErrorObjects()
{
	init( true );
}

CErrorObjects::~CErrorObjects()
{
}

CErrorObject& CErrorObjects::push_back( const CErrorObject& obj )
{
	if( m_bOutputInstantly && m_hFile ) obj.Print( m_hFile, m_emFormat, m_emLevel );
	m_listErrorObject.push_back( obj );
	return *m_listErrorObject.rbegin();
}

void CErrorObjects::clear()
{
	m_listErrorObject.clear();
}

void CErrorObjects::init( bool bIsConstructor/* = false*/ )
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

errorcode::sindyErrCode CErrorObjects::GetDefaultErrCode( sindyErrLevel* pLevel/* = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	sindyErrLevel emLevel = sindyErrLevel_INFO;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		sindyErrLevel emLevelTmp = sindyErrLevel_INFO;
		sindyErrCode emErrTmp = it->GetDefaultErrCode( &emLevelTmp );
		
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

LPCTSTR CErrorObjects::ToString()
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( CErrorInfos::const_iterator itInfo = it->begin(); itInfo != it->end(); ++itInfo )
		{
			if( sindyErr_NoErr != itInfo->GetErrorCode() )
			{
				CString strMsg, strLevel;
				switch( itInfo->GetErrorLevel() )
				{
					case sindyErrLevel_INFO: strLevel = INFO_MSG; break;
					case sindyErrLevel_WARN: strLevel = WARNING_MSG; break;
					case sindyErrLevel_ERR:  strLevel = ERROR_MSG; break;
					default: break;
				}
				strMsg.Format(TABLE_INFO, it->GetErrorObject()->get<0>(), it->GetErrorObject()->get<1>(), strLevel, SINDYERRCODE2STRING(itInfo->GetErrorCode()), itInfo->GetErrorMessage() );
				m_strErr += strMsg;
			}
		}
	}

	return m_strErr;
}

} // sindy
