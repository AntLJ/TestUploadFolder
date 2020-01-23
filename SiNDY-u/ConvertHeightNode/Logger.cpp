#include "stdafx.h"

#include <WinLib/VersionInfo.h>
#include <atltime.h>

#include "Logger.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// boost
#include <boost/filesystem/path.hpp>

// �V���O���g���I�u�W�F�N�g�̃X���b�h����p
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// �t�@�C���I�[�v��
bool CLogger::Initialize( const uh::tstring& filePath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	m_Log.open( filePath );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ���O�փ��b�Z�[�W�o��
void CLogger::Log( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
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
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}



// �G���[���x���t���̃��b�Z�[�W�o��
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
	// �c�[����
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}


void CErrLogger::writeHeader()
{
	Log( _T( "#SINDYSTDLOG\n") );
}

namespace log_utility
{
	// �����̏��𕶎���ŕԂ�
	uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}
