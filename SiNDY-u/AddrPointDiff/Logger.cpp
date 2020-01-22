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
bool CLogger::Initialize( const uh::tstring& strDirPath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( strDirPath.c_str()));

	m_Log.open( filepath.string() );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ログへメッセージ出力
void CLogger::Log( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
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

// 標準出力へメッセージ出力
void CLogger::StdOut( const uh::tstring& strMessage )
{
	std::cout << uh::toStr( strMessage );
}

// << オペレータのオーバーロード
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}



// エラーレベル付きのメッセージ出力
void CRunLogger::Fatal( const uh::tstring& strMessage, bool isOutStdout )
{
	Log( uh::str_util::format(_T("%s FATAL -- : %s"), 
				log_utility::GetDateTimeInfo().c_str(), strMessage.c_str()), isOutStdout );
	if( isOutStdout )
		StdOut( strMessage );
}

void CRunLogger::Error( const uh::tstring& strMessage, bool isOutStdout )
{
	Log( uh::str_util::format(_T("%s ERROR -- : %s"),
				log_utility::GetDateTimeInfo().c_str(), strMessage.c_str()), isOutStdout );
	if( isOutStdout )
		StdOut( strMessage );
}

void CRunLogger::Warn( const uh::tstring& strMessage, bool isOutStdout )
{
	Log( uh::str_util::format(_T("%s WARN  -- : %s"),
				log_utility::GetDateTimeInfo().c_str(), strMessage.c_str()), isOutStdout );
	if( isOutStdout )
		StdOut( strMessage );
}

void CRunLogger::Info( const uh::tstring& strMessage, bool isOutStdout )
{
	Log( uh::str_util::format(_T("%s INFO  -- : %s"),
				log_utility::GetDateTimeInfo().c_str(), strMessage.c_str()), isOutStdout );
	if( isOutStdout )
		StdOut( strMessage );
}

void CRunLogger::Debug( const uh::tstring& strMessage, bool isOutStdout )
{
	Log( uh::str_util::format(_T("%s DEBUG -- : %s"),
				log_utility::GetDateTimeInfo().c_str(),  strMessage.c_str()), isOutStdout );
	if( isOutStdout )
		StdOut( strMessage );
}


// 実行ログファイル名を返す
uh::tstring CRunLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("run.log");
}


void CRunLogger::writeHeader()
{
	// ツール名
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}


// エラーログファイル名を返す
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}


void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG\n") );
}

namespace log_utility
{
	// エラーログファイル名を取得
	static uh::tstring GetLogfileNamePrefix()
	{
		// ツール名取得
		CVersion version;
		CString strToolName = version.GetInternalName();
		// 拡張子があれば削除
		if( strToolName.Find( _T('.') ) != -1 )
			strToolName = strToolName.Left( strToolName.ReverseFind(_T('.')) );;

		// 現在の日時を"YYYYMMDDHHDDSS" で取得
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("%Y%m%d%H%M%S") );

		return strToolName + _T("_") + strTime.GetString() + _T("_");
	}

	// 日時の情報を文字列で返す
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}
