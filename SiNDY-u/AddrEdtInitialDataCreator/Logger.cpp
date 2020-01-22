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

// �V���O���g���I�u�W�F�N�g�̃X���b�h����p
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// �t�@�C���I�[�v��
bool CLogger::Initialize( const uh::tstring& strFilePath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	// �t�H���_�p�X�ƃt�@�C������A��
	boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( strFilePath ));

	m_Log.open( filepath.string() );
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

// �W���o�͂փ��b�Z�[�W�o��
void CLogger::StdOut( const uh::tstring& strMessage )
{
	std::cout << uh::toStr( strMessage );
}

// << �I�y���[�^�̃I�[�o�[���[�h
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}


// �G���[���x���t���̃��b�Z�[�W�o��
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
	Log( _T( "#SINDYSTDLOG\n") );
}

namespace log_utility
{
	// �G���[���O�t�@�C�������擾
	static uh::tstring GetLogfileNamePrefix()
	{
		// �c�[�����擾
		CVersion version;
		CString strToolName = version.GetInternalName();
		// �g���q������΍폜
		if( strToolName.Find( _T('.') ) != -1 )
			strToolName = strToolName.Left( strToolName.ReverseFind(_T('.')) );;

		// ���݂̓�����"YYYYMMDDHHDDSS" �Ŏ擾
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("%Y%m%d%H%M%S") );

		return strToolName + _T("_") + strTime.GetString() + _T("_");
	}

	// �����̏��𕶎���ŕԂ�
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}