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

// boost
#include <boost/filesystem/path.hpp>


// �V���O���g���I�u�W�F�N�g�̃X���b�h����p
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// �t�@�C���I�[�v��
bool CLogger::Initialize( const uh::tstring& dir_path )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	// �t�H���_�p�X�ƃt�@�C������A��
	boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( dir_path.c_str()) + "\\" + uh::toStr( getFileName() ));

	m_Log.open( filepath.string() );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ���O�փ��b�Z�[�W�o��
void CLogger::Log( const uh::tstring& message )
{
	m_Log << uh::toStr( message ) << std::endl;
}


// ���O�փ��b�Z�[�W�o��
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


// << �I�y���[�^�̃I�[�o�[���[�h
CLogger& CLogger::operator<<( const uh::tstring& message )
{
	m_Log << uh::toStr( message );
	return *this;
}

// �G���[���x���t���̃��b�Z�[�W�o��
void CRunLogger::Fatal( const uh::tstring& message )
{
	Log( _T("%s FATAL -- : %s"), log_utility::GetDateTimeInfo().c_str(), message.c_str() );
}

void CRunLogger::Error( const uh::tstring& message )
{
	Log( _T("%s ERROR -- : %s"), log_utility::GetDateTimeInfo().c_str(), message.c_str() );
}

void CRunLogger::Warn( const uh::tstring& message )
{
	Log( _T("%s WARN  -- : %s"), log_utility::GetDateTimeInfo().c_str(), message.c_str() );
}

void CRunLogger::Info( const uh::tstring& message )
{
	Log( _T("%s INFO  -- : %s"), log_utility::GetDateTimeInfo().c_str(), message.c_str() );
}

void CRunLogger::Debug( const uh::tstring& message )
{
	Log( _T("%s DEBUG -- : %s"), log_utility::GetDateTimeInfo().c_str(),  message.c_str() );
}


// ���s���O�t�@�C������Ԃ�
uh::tstring CRunLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("run.log");
}

void CRunLogger::writeHeader()
{
	// �c�[����
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}

// �G���[���O�t�@�C������Ԃ�
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}

void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG") );
	Log( _T("FLAG\tLAYER\tOBJECTID\tBASEMESH\tLON\tLAT\tGLOBALID\tERR_LEVEL\tMSG\tCOMMENT") );
}

// �G���[���x���t���̃��b�Z�[�W�o��
void CErrLogger::Fatal( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg )
{
	writeErrRecord( kFatal, oid, gid, mesh_code, x, y, msg );
}

void CErrLogger::Error( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg )
{
	writeErrRecord( kErr, oid, gid, mesh_code, x, y, msg );
}

void CErrLogger::Warn( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg )
{
	writeErrRecord( kWarn, oid, gid, mesh_code, x, y, msg );
}

void CErrLogger::Info( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg )
{
	writeErrRecord( kInfo, oid, gid, mesh_code, x, y, msg );
}

void CErrLogger::Debug( long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg )
{
	writeErrRecord( kDebug, oid, gid, mesh_code, x, y, msg );
}

void CErrLogger::writeErrRecord( CErrLogger::ErrLevel err_level, long oid, const CString& gid, long mesh_code, double x, double y, const CString& msg )
{
	using namespace uh;
	using namespace sindy::schema::adam_v2;

	// �G���[�R�[�h������擾
	std::string errLevelString;
	switch ( err_level )
	{
	case kFatal:   errLevelString = "FATAL";   break;
	case kErr:     errLevelString = "ERROR";   break;
	case kWarn:    errLevelString = "WARNING"; break;
	case kInfo:    errLevelString = "INFO";    break;
	case kDebug:   errLevelString = "DEBUG";   break;
	default: break;
	}

	// �G���[���R�[�h���o��
	m_Log << "0\t" << uh::toStr( CString( lane_link::kTableName ).GetString() )		// ���C����
		  << "\t"  << std::to_string( oid )											// �Ԑ������N��OID
		  << "\t"  << std::to_string( mesh_code )									// �x�[�X���b�V���ԍ�
		  << "\t"  << std::to_string( x )											// �o�x
		  << "\t"  << std::to_string( y )											// �ܓx
		  << "\t"  << uh::toStr( gid.GetString() )									// �Ԑ������N�̃O���[�o��ID
		  << "\t"  << errLevelString												// �G���[���x��
		  << "\t"  << uh::toStr( msg.GetString() )									// ���b�Z�[�W
		  << "\n";
}

namespace log_utility
{
	// �G���[���O�t�@�C�������擾
	static uh::tstring GetLogfileNamePrefix()
	{
		// �c�[�����擾
		CVersion version;
		CString toolName = version.GetInternalName();
		// �g���q������΍폜
		if( toolName.Find( _T('.') ) != -1 )
			toolName = toolName.Left( toolName.ReverseFind(_T('.')) );;

		// ���݂̓�����"YYYYMMDDHHDDSS" �Ŏ擾
		CTime cTime = CTime::GetCurrentTime();
		const CString timeString = cTime.Format( _T("%Y%m%d%H%M%S") );

		return toolName + _T("_") + timeString.GetString() + _T("_");
	}

	// �����̏��𕶎���ŕԂ�
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}
