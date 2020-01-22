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

#include <WinLib/VersionInfo.h>
#include <atltime.h>

#include "Logger.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// boost
#include <boost/filesystem/path.hpp>

// シングルトンオブジェクトのスレッド制御用
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// ファイルオープン
bool CLogger::Initialize( const uh::tstring& strFilePath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	m_Log.open( uh::toStr( strFilePath.c_str() ) );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ログへメッセージ出力
void CLogger::Log( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage ) << std::endl;
}


// ログへメッセージ出力
void CLogger::Log( const TCHAR* format, ... )
{
	CString cstr;
	if( format )
	{
		va_list args;
		va_start( args, format );
		cstr.FormatV( format, args );
		va_end(args);
	}
	Log( uh::toTStr( cstr.GetString() ));
}


// << オペレータのオーバーロード
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}



// エラーレベル付きのメッセージ出力
void CRunLogger::Fatal( const uh::tstring& strMessage )
{
	Log( _T("%s FATAL -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Error( const uh::tstring& strMessage )
{
	Log( _T("%s ERROR -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Warn( const uh::tstring& strMessage )
{
	Log( _T("%s WARN  -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Info( const uh::tstring& strMessage )
{
	Log( _T("%s INFO  -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Debug( const uh::tstring& strMessage )
{
	Log( _T("%s DEBUG -- : %s"), log_utility::GetDateTimeInfo().c_str(),  strMessage.c_str() );
}


void CRunLogger::writeHeader()
{
	// ツール名
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s"), version.GetInternalName(), version.GetProductVersion() );
	Log( _T("開始時間:%s"), log_utility::GetDateTimeInfo().c_str() );
}


void CRunLogger::writeExitLog(bool ret)
{
	Log( _T("終了時間:%s"), log_utility::GetDateTimeInfo().c_str() );
	Log( ret ? _T("正常終了") : _T("異常終了") );
}

namespace log_utility
{
	// 日時の情報を文字列で返す
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}

void CErrLogger::writeHeader()
{
	// 自由形式ログフォーマット
	Log(_T("#FREESTYLELOG"));
	Log(_T("FLAG\tLAYER\tOBJECTID\tエラーメッセージ"));
}

void CErrLogger::writeError(const uh::tstring& layer, const long oid, const uh::tstring& message)
{
	CString err = uh::str_util::format(_T("0\t%s\t%ld\t%s"), layer.c_str(), oid, message.c_str());
	Log(err);
}
