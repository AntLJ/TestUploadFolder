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


// �t�@�C���I�[�v��
bool CLogger::Initialize( const uh::tstring& strDirPath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	// �t�H���_�p�X�ƃt�@�C������A��
	boost::filesystem::path filepath( uh::toStr( strDirPath.c_str() ) );

	m_Log.open( filepath.string() );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ���O�փ��b�Z�[�W�o��
void CLogger::Log( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage ) << std::endl;
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



void CRunLogger::writeHeader()
{
	// �c�[����
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
	Log( _T( "\t--db          �F%s" ),		arg.m_db.c_str()		);
	Log( _T( "\t--mesh_db     �F%s" ),		arg.m_mesh_db.c_str()	);
	Log( _T( "\t--mesh_list   �F%s" ),		arg.m_mesh_list.c_str() );
	Log( _T( "\t--run_log     �F%s" ),		arg.m_run_log.c_str()	);
	Log( _T( "\t--err_log     �F%s" ),		arg.m_err_log.c_str()	);
	Log( _T( "\t--slope_max   �F%lf\n" ),	arg.m_slope_max			);
}



void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tGLOBALID\tERR_LV\tERR_MSG\tMESHCODE\tSOURCE" ) );
}

void CErrLogger::error( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source )
{
	Log( _T( "0\t%s\t%s\t%s\tERROR\t%s\t%s\t%s" ), layer, oid, gid, errMsg, meshCode, source );
}

void CErrLogger::warning( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source )
{
	Log( _T( "0\t%s\t%s\t%s\tWARNING\t%s\t%s\t%s" ), layer, oid, gid, errMsg, meshCode, source );
}

void CErrLogger::info( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source )
{
	Log( _T( "0\t%s\t%s\t%s\tINFO\t%s\t%s\t%s" ), layer, oid, gid, errMsg, meshCode, source );
}



namespace log_utility
{
	// �����̏��𕶎���ŕԂ�
	CString GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		return cTime.Format( _T( "%Y/%m/%d %H:%M:%S" ) );
	}
}