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

// ArcHelperEx
#include <AheGlobalsX.h>
// useful_headers
#include <TDC/useful_headers/str_util.h>
// adamlib
#include <TDC/adamlib/AdamGlobals.h>
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
	boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( strDirPath.c_str()) + "\\" + uh::toStr( getFileName() ));

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


// << オペレータのオーバーロード
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
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

// エラーレベル付きのメッセージ出力
void CRunLogger::Fatal(const uh::tstring& strMessage)
{
	Log(_T("%s FATAL -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Error(const uh::tstring& strMessage)
{
	Log(_T("%s ERROR -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Warn(const uh::tstring& strMessage)
{
	Log(_T("%s WARN  -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Info(const uh::tstring& strMessage)
{
	Log(_T("%s INFO  -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Debug(const uh::tstring& strMessage)
{
	Log(_T("%s DEBUG -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

// エラーログファイル名を返す
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}

void CErrLogger::writeHeader()
{
	Log(_T("#FREESTYLELOG\n") );
	Log(_T("FLAG\tLAYER\tOBJECTID\tGLOBALID\tERR_LEVEL\tMSG\tROAD_LINK_OID\tMAXLEGAL_SPEED_C\tBASEMESH\tCOMMENT\n"));
}

// エラーレベル付きのメッセージ出力
void CErrLogger::Fatal(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c)
{
	writeErrRecord(kFatal, lane_link, road_link, basemesh, msg, max_speed_c);
}

void CErrLogger::Error(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c)
{
	writeErrRecord(kErr, lane_link, road_link, basemesh, msg, max_speed_c);
}

void CErrLogger::Warn(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c)
{
	writeErrRecord(kWarn, lane_link, road_link, basemesh, msg, max_speed_c);
}

void CErrLogger::Info(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c)
{
	writeErrRecord(kInfo, lane_link, road_link, basemesh, msg, max_speed_c);
}

void CErrLogger::Debug(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c)
{
	writeErrRecord(kDebug, lane_link, road_link, basemesh, msg, max_speed_c);
}

void CErrLogger::writeErrRecord(CErrLogger::ErrLevel err_level, const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c)
{
	using namespace uh;

	// エラーコード文字列取得
	std::string errLevelString;
	switch (err_level)
	{
	case kFatal:   errLevelString = "FATAL";   break;
	case kErr:     errLevelString = "ERROR";   break;
	case kWarn:    errLevelString = "WARNING"; break;
	case kInfo:    errLevelString = "INFO";    break;
	case kDebug:   errLevelString = "DEBUG";   break;
	default: break;
	}

	// 車線リンクのレイヤ名、OID、GID、最高速度コード
	std::string layerName = uh::toStr(AheGetFeatureClassName(lane_link->GetTable()).GetString());
	std::string laneOid = std::to_string(lane_link->GetOID());
	std::string laneGid = uh::toStr(adam::GetGlobalId(lane_link).GetString());
	std::string maxSpeedC = (max_speed_c != 0) ? std::to_string(max_speed_c) : "";
	// 道路リンクのOID
	std::string roadOid = (road_link) ? std::to_string(road_link->GetOID()) : "";
	// ベースメッシュ番号
	std::string basemeshNo = std::to_string(basemesh);
	// メッセージ
	std::string message = uh::toStr(msg.GetString());

	// エラーレコードを出力
	m_Log << "0\t" << layerName			// レイヤ名
		<< "\t" << laneOid				// 車線リンクのOID
		<< "\t" << laneGid				// 車線リンクのグローバルID
		<< "\t" << errLevelString		// エラーレベル
		<< "\t" << message				// メッセージ
		<< "\t" << roadOid				// 道路リンクOID
		<< "\t" << maxSpeedC			// 制限速度コード(最大)
		<< "\t" << basemeshNo			// ベースメッシュ番号
		<< "\n";
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
