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
//#include "globalfunc.h"

#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

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

	// フォルダパスとファイル名を連結
	//boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( strDirPath.c_str()) + uh::toStr( getFileName() ));

	m_Log.open( strDirPath );
	if( ! m_Log )
		throw std::runtime_error( uh::str_util::formatA(_T("Log file open failure. : %s" ), strDirPath.c_str()) );

	writeHeader();

	return true;
}

// ログへメッセージ出力
void CLogger::Log( const uh::tstring& strMessage, bool std_out )
{
	m_Log << uh::toStr( strMessage );
	if( std_out )
		std::cout << uh::toStr( strMessage );
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
	Log( uh::toTStr( cstr.GetString() ), false );
}


// << オペレータのオーバーロード
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}



// エラーレベル付きのメッセージ出力
void CRunLogger::Fatal( const uh::tstring& strMessage, bool std_err )
{
	Log( _T("%s FATAL -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
	if( std_err )
		std::cerr << uh::toStr( strMessage ) << std::endl;
}

void CRunLogger::Error( const uh::tstring& strMessage )
{
	Log( _T("%s ERROR -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Warn( const uh::tstring& strMessage )
{
	Log( _T("%s WARN  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Info( const uh::tstring& strMessage, bool std_out )
{
	Log( _T("%s INFO  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
	if( std_out )
		std::cout << uh::toStr( strMessage ) << std::endl;
}

void CRunLogger::Debug( const uh::tstring& strMessage )
{
	Log( _T("%s DEBUG -- : %s\n"), log_utility::GetDateTimeInfo().c_str(),  strMessage.c_str() );
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

// 結果出力
void CRunLogger::WriteResult( )
{

}

// エラーログファイル名を返す
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}


void CErrLogger::writeHeader()
{
	using namespace sindy::schema::sj::buildingname_src_point;

	Log( _T( "#FREESTYLELOG\n"), false );

	// ヘッダ行書き込み
	auto header = boost::assign::list_of
		(_T("LAYER"))(kObjectID)(kAddr)(kAdoption);
	Log( uh::str_util::join( header, _T("\t") )+_T("\n"), false );
}

void CErrLogger::WriteLog( const sindy::CRowContainer& row, const CString& msg )
{
	using namespace sindy::schema::sj::buildingname_src_point;
	using namespace uh::str_util;

	std::vector<CString> log = boost::assign::list_of
		( row.GetTableName() )
		( ToString(row.GetOID()) )
		( ToString(row.GetValue( kAddr ) ) )
		( ToString(row.GetValue( kAdoption ) ) )
		( msg );
	
	Log( uh::str_util::join( log, _T("\t") )+_T("\n"), false );
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
