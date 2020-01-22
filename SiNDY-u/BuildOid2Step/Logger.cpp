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

	// フォルダパスとファイル名を連結
	boost::filesystem::path filepath( uh::toStr( strDirPath.c_str() ) );

	m_Log.open( filepath.string() );
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



void CRunLogger::writeHeader()
{
	// ツール名
	CVersion version;
	Log( _T( "%s FILEVERSION %s PRODUCTVERSION %s\n" ), version.GetInternalName(), version.GetFileVersion(), version.GetProductVersion() );
}

void CRunLogger::writeInfo( const CString& info )
{
	Log( _T( "%s\t%s" ), log_utility::GetDateTimeInfo(), info );
}

void CRunLogger::writeOption( const Arguments& arg )
{
	Log( _T( "option" ) );
	Log( _T( "\t--db          ：%s" ), arg.m_db.c_str() );
	Log( _T( "\t--mesh_db     ：%s" ), arg.m_mesh_db.c_str() );
	Log( _T( "\t--mesh_list   ：%s" ), arg.m_mesh_list.c_str() );
	Log( _T( "\t--run_log     ：%s" ), arg.m_run_log.c_str() );
	Log( _T( "\t--err_log     ：%s" ), arg.m_err_log.c_str() );
	Log( _T( "\t--buf_distance：%lf\n" ), arg.m_buf_distance );
}



void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tERR_LV\tERR_MSG\tMESHCODE\tCOMMENT" ) );
}

void CErrLogger::error( const CString& layer, const CString& oid, const CString& errMsg, const CString& meshCode )
{
	Log( _T( "0\t%s\t%s\tERROR\t%s\t%s" ), layer, oid, errMsg, meshCode );
}



namespace log_utility
{
	// 日時の情報を文字列で返す
	static CString GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		return cTime.Format( _T( "[%Y/%m/%d %H:%M:%S]" ) );
	}
}
