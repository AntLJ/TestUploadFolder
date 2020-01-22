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
#include "LogMgr.h"
#include <WinLib/winutil.h>
#include <WinLib/VersionInfo.h>

LogMgr::LogMgr()
{
}

LogMgr::LogMgr( const LogMgr& logMgr )
{
}

LogMgr::~LogMgr()
{
	writeEndTime();
	if( m_ofsRun.is_open() )	m_ofsRun.close();
	if( m_ofsErr.is_open() )	m_ofsErr.close();
}

LogMgr& LogMgr::operator = ( const LogMgr& logMgr )
{
	return *this;
}

void LogMgr::writeVersion()
{
	writeRunLog( getVersionInfo() );
}

void LogMgr::writeStartTime()
{
	writeRunLog( _T("äJénéûçè ") + winutil::GetTime() );
}

void LogMgr::writeEndTime()
{
	writeRunLog( _T("èIóπéûçè ") + winutil::GetTime() );
}

bool LogMgr::setRunLog( const CString& strFileName, const boost::program_options::variables_map* varMap )
{
	m_ofsRun.open( strFileName );
	if( !m_ofsRun.is_open() )	return false;

	writeVersion();
	writeStartTime();

	if( varMap )
		writeParamInfos( *varMap, true );

	return true;
}

bool LogMgr::setErrLog( const CString& strFileName )
{
	m_ofsErr.open( strFileName );
	if( !m_ofsErr.is_open() )	return false;

	return true;
}

void LogMgr::writeParamInfos( const boost::program_options::variables_map& varMap, bool bWithHyphen )
{
	for( auto rec : varMap )
	{
		CString strOption;
		strOption.Format( _T("    %-20s %s"), ( bWithHyphen ? _T("--") : _T("") ) + CString( rec.first.c_str() ), CString( rec.second.as<std::string>().c_str() ) );
		writeRunLog( strOption );
	}
}

void LogMgr::writeRunLog( const CString& strMessage )
{
	if( m_ofsRun.is_open() )
		m_ofsRun << CT2A( strMessage ) << std::endl;
	std::cout << CT2A( strMessage ) << std::endl;
}

void LogMgr::writeErrLog( const CString& strMessage )
{
	if( m_ofsErr.is_open() )
		m_ofsErr << CT2A( strMessage ) << std::endl;
	else
		std::cerr << CT2A( strMessage ) << std::endl;
}

void LogMgr::writeErrLog( EErrLevel errLv, const CString& strMessage )
{
	writeErrLog( getErrLvString( errLv ) + _T("\t") + strMessage );
}

CString LogMgr::getErrLvString( EErrLevel errLv )
{
	switch( errLv )
	{
	case kNone:		return _T("None");
	case kInfo:		return _T("Info");
	case kWarn:		return _T("Warn");
	case kError:	return _T("Error");
	case kFatal:	return _T("Fatal");
	default:		return _T("None");
	}
}

CString LogMgr::getVersionInfo()
{
	CVersion version;
	return version.GetInternalName() + _T(" F:") + version.GetFileVersion() + _T(" P:") + version.GetProductVersion();
}
