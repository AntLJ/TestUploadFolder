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
#include <atltime.h>
#include "Logger.h"
#include <boost/assign.hpp>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/str_util.h>

// 可変長引数をtstring型の文字列として整形するためのマクロ(CStringのFormatVを使用)
#define VARIABLE_LENGTH_ARG_FORMAT(tstr, format) if( format ){\
		CString cstr;\
		va_list args;\
		va_start( args, format );\
		cstr.FormatV( format, args );\
		va_end(args);\
		tstr = cstr.GetString();}

// シングルトンオブジェクトのスレッド制御用
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// ファイルオープン
bool CLogger::Initialize( const uh::tstring& filepath )
{
	if( ! m_Log || m_Log.is_open() )
		return true;

	m_Log.open( uh::toWStr( filepath ) );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ログへメッセージ出力
void CLogger::Log( const uh::tstring& strMessage, bool lineBreak /*= true*/ )
{
	if( !m_Log ) return;
	m_Log << uh::toWStr( strMessage );
	if(lineBreak) m_Log << std::endl;
}
void CLogger::Log( const std::vector<uh::tstring>& strMessages)
{
	for(const auto& elem : strMessages)
		Log( elem );
}

// ログへメッセージ出力
void CLogger::FormatLog( const TCHAR* format, ... )
{
	uh::tstring message;
	VARIABLE_LENGTH_ARG_FORMAT(message, format);
	Log( message, false);
}

// << オペレータのオーバーロード
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	if( !m_Log ) return *this;
	m_Log << uh::toWStr( strMessage );
	return *this;
}

void CRunLogger::fatal(
	long stringTableId
	, const uh::tstring& dbPath
	, const uh::tstring& layerName
	, const uh::tstring& columnName
	, const uh::tstring& comment)
{
	writeError(_T("FATAL"), stringTableId, dbPath, layerName, columnName, comment);
	setFail();
}

void CRunLogger::fatalXML( long stringTableId
	, const uh::tstring& tag
	, const uh::tstring& attrName
	, const uh::tstring& attrValue)
{
	tstringstream ss;
	ss << AheLoadString(stringTableId).GetString()
		<< _T(" : TAG[") << tag << _T("]");
	if( !attrName.empty() )
		ss << _T(" ") << attrName << _T("[") << attrValue << _T("]");
	writeError(_T("FATAL"), ss.str());
	setFail();
}

void CRunLogger::error(
	long stringTableId
	, const uh::tstring& dbPath
	, const uh::tstring& layerName
	, const uh::tstring& columnName
	, const uh::tstring& comment)
{
	writeError(_T("ERROR"), stringTableId, dbPath, layerName, columnName, comment);
	setFail();
}
void CRunLogger::warn(
	long stringTableId
	, const uh::tstring& dbPath
	, const uh::tstring& layerName
	, const uh::tstring& columnName
	, const uh::tstring& comment)
{
	writeError(_T("WARN"), stringTableId, dbPath, layerName, columnName, comment);
}

void CRunLogger::fatal( const TCHAR* format, ... )
{
	uh::tstring message;
	VARIABLE_LENGTH_ARG_FORMAT(message, format);
	writeError(_T("FATAL"), message);
	setFail();
}

void CRunLogger::error( const TCHAR* format, ... )
{
	uh::tstring message;
	VARIABLE_LENGTH_ARG_FORMAT(message, format);
	writeError(_T("ERROR"), message);
	setFail();
}

void CRunLogger::warn( const TCHAR* format, ... )
{
	uh::tstring message;
	VARIABLE_LENGTH_ARG_FORMAT(message, format);
	writeError(_T("WARN"), message);
}

void CRunLogger::info( const TCHAR* format, ... )
{
	uh::tstring message;
	VARIABLE_LENGTH_ARG_FORMAT(message, format);
	writeError(_T("INFO"), message);
}

void CRunLogger::debug( const TCHAR* format, ... )
{
	uh::tstring message;
	VARIABLE_LENGTH_ARG_FORMAT(message, format);
	writeError(_T("DEBUG"), message);
}

void CRunLogger::writeTime( const uh::tstring& comment )
{
	FormatLog( _T("# %s %s\n"), comment.c_str(), log_utility::getDateTimeInfo().c_str());
}

void CRunLogger::setFail()
{
	m_bFail = true;
}

void CRunLogger::endLog()
{
	if( !m_Log ) return;
	writeTime( AheLoadString(IDS_LOG_END_TIME).GetString() );
	const CString result = m_bFail? AheLoadString(IDS_LOG_ABEND) : AheLoadString(IDS_LOG_SUCCESS);
	CString logStr;
	logStr.Format(_T("# %s"), result);
	Log( logStr.GetString() );
	std::cout << CStringA(result).GetString() << std::endl;
}

void CRunLogger::writeError(
	const uh::tstring& strLevel, long stringTableId
	, const uh::tstring& dbPath
	, const uh::tstring& layerName
	, const uh::tstring& columnName
	, const uh::tstring& comment)
{
	tstringstream ss;
	ss << AheLoadString(stringTableId).GetString();
	if(!dbPath.empty() || !layerName.empty() || !columnName.empty() || !comment.empty())
		ss << _T(" :");
	if(!dbPath.empty())
		ss << _T(" DB[") << dbPath << _T("]");
	if(!layerName.empty())
		ss << _T(" LAYER[") << layerName << _T("]");
	if(!columnName.empty())
		ss << _T(" COLUMN[") << columnName << _T("]");
	if(!comment.empty())
		ss << _T(" ") << comment;

	writeError(strLevel, ss.str());
}
void CRunLogger::writeError( const uh::tstring& strLevel, const uh::tstring& strMsg )
{
	FormatLog( _T("# %s %s -- "), log_utility::getDateTimeInfo().c_str(), strLevel.c_str() );
	Log( strMsg );
}

void CRunLogger::writeHeader()
{
	// ツール名
	CVersion version;
	FormatLog( _T("# %s PRODUCTVERSION %s FILEVERSION %s\n"),
		version.GetInternalName(), version.GetProductVersion(), version.GetFileVersion() );
}

void CErrLogger::writeHeader()
{
	Log( _T( "#SINDYSTDLOG") );
}

namespace log_utility
{
	static uh::tstring getDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("%Y-%m-%d %H:%M:%S") );

		return uh::toTStr( strTime.GetString() );
	}
} // log_utility

#undef VARIABLE_LENGTH_ARG_FORMAT
