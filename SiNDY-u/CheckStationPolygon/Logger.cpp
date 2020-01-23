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
bool CLogger::Initialize( const uh::tstring& dir_path )
{
	namespace fs = boost::filesystem;

	if( ! m_Log || m_Log.is_open() )
		return false;

	// �t�H���_�p�X���t�@�C������A��
	fs::path dir( uh::toStr( dir_path.c_str() ) );
	fs::path file( uh::toStr( getFileName() ) );
	fs::path filepath( dir/file );

	m_Log.open( filepath.string() );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ���O�փ��b�Z�[�W�o��
void CLogger::Log( const uh::tstring& msg )
{
	m_Log << uh::toStr( msg );
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
CLogger& CLogger::operator<<( const uh::tstring& msg )
{
	m_Log << uh::toStr( msg );
	return *this;
}



// �G���[���x���t���̃��b�Z�[�W�o��
void CRunLogger::Fatal( const uh::tstring& msg )
{
	Log( _T("%s FATAL -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), msg.c_str() );
}

void CRunLogger::Error( const uh::tstring& msg )
{
	Log( _T("%s ERROR -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), msg.c_str() );
}

void CRunLogger::Warn( const uh::tstring& msg )
{
	Log( _T("%s WARN  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), msg.c_str() );
}

void CRunLogger::Info( const uh::tstring& msg )
{
	Log( _T("%s INFO  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), msg.c_str() );
}

void CRunLogger::Debug( const uh::tstring& msg )
{
	Log( _T("%s DEBUG -- : %s\n"), log_utility::GetDateTimeInfo().c_str(),  msg.c_str() );
}

// ���s���O�t�@�C������Ԃ�
uh::tstring CRunLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("run.log");
}

// �w�b�_�[�o��
void CRunLogger::writeHeader()
{
	// �c�[����
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}



// �G���[���x���t���̃��b�Z�[�W�o��
void CErrLogger::Fatal( const uh::tstring& layer, long oid, const err_def::ErrPair& err )
{
	Log( _T("0\t%s\t%ld\tFATAL\t%d\t%s\t\n"), layer.c_str(), oid, err.first, err.second.c_str() );
}

void CErrLogger::Error( const uh::tstring& layer, long oid, const err_def::ErrPair& err )
{
	Log( _T("0\t%s\t%ld\tERROR\t%d\t%s\t\n"), layer.c_str(), oid, err.first, err.second.c_str() );
}

void CErrLogger::Warn( const uh::tstring& layer, long oid, const err_def::ErrPair& err )
{
	Log( _T("0\t%s\t%ld\tWARNING\t%d\t%s\t\n"), layer.c_str(), oid, err.first, err.second.c_str() );
}

void CErrLogger::Info( const uh::tstring& layer, long oid, const err_def::ErrPair& err )
{
	Log( _T("0\t%s\t%ld\tINFO\t%d\t%s\t\n"), layer.c_str(), oid, err.first, err.second.c_str() );
}

void CErrLogger::Debug( const uh::tstring& layer, long oid, const err_def::ErrPair& err )
{
	Log( _T("0\t%s\t%ld\tDEBUG\t%d\t%s\t\n"), layer.c_str(), oid, err.first, err.second.c_str() );
}

// �G���[���O�t�@�C������Ԃ�
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}

// �w�b�_�[�o��
void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG\n") );
	Log( _T( "FLAG\tLAYER\tOBJECTID\tERR_LEVEL\tERR_CODE\tMSG\tCOMMENT\n") );
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
