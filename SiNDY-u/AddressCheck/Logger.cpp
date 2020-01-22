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

// boost
#include <boost/filesystem/path.hpp>

// �V���O���g���I�u�W�F�N�g�̃X���b�h����p
std::mutex RunLogger::s_mutex;

using namespace message;

// �t�@�C���I�[�v��
bool Logger::initialize( const CString& strDirPath )
{
	if( m_log.is_open() )
		return false;

	m_log.open( strDirPath );
	if( ! m_log.is_open() )
		return false;

	writeHeader();

	return true;
}

// ���O�փ��b�Z�[�W�o��
void Logger::log( const CString& strMessage )
{
	m_log <<  CT2A( strMessage );
}

// ���O�փ��b�Z�[�W�o��
void Logger::log( const TCHAR* format, ... )
{
	CString cstr;
	if( format )
	{
		va_list args;
		va_start( args, format );
		cstr.FormatV( format, args );
		va_end(args);
	}
	log( cstr );
}

// << �I�y���[�^�̃I�[�o�[���[�h
Logger& Logger::operator<<( const CString& strMessage )
{
	m_log << CT2A( strMessage );
	return *this;
}

// �w�b�_�[�o��
void RunLogger::writeHeader()
{
	// �c�[����
	CVersion version;
	log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}

// �w�b�_�[�o��
void ErrLogger::writeHeader()
{
	log( _T( "#SINDYSTDLOG\n") );
}

// �o�͊֐�
void ErrLogger::write( emCheck_ErrCode emCode, LPCTSTR lpcszLayer, long oID, const IGeometryPtr& ipGeometry, LPCTSTR lpcszFormat, ... )
{
	// �d�S�擾
	WKSPoint point = {};
	if( ipGeometry )
		getLonLat( ipGeometry, point );

	// ���2 - n
	va_list vaList = NULL;
	if( lpcszFormat )
		va_start( vaList, lpcszFormat );

	writeCommon( ErrInfo(emCode, lpcszLayer, oID, point), lpcszFormat, vaList );
}

// �o�x�E�ܓx�擾
void ErrLogger::getLonLat( const IGeometryPtr& ipGeometry, WKSPoint& point ) const
{
	IPointPtr ipPoint;
	esriGeometryType emGeoType;
	ipGeometry->get_GeometryType( &emGeoType );
	switch( emGeoType )
	{
	case esriGeometryPoint:
		ipPoint = ipGeometry;
		break;
	case esriGeometryPolyline:
		((IPolylinePtr)ipGeometry)->get_FromPoint( &ipPoint );
		break;
	case esriGeometryRing:
	case esriGeometryPolygon:
		((IAreaPtr)ipGeometry)->get_Centroid( &ipPoint );
		break;
	default:
		break;
	}

	if( ipPoint )
		ipPoint->QueryCoords( &(point.X), &(point.Y) );
}

// �o�͊֐�����
void ErrLogger::writeCommon( const ErrInfo& cErrInfo, LPCTSTR lpcszFormat, va_list vaInfo )
{
	// �Ή��󋵁E���C�����E�I�u�W�F�N�gID
	log( _T( "0\t"));
	log( _T( "%s\t"), cErrInfo.m_layer );
	if( cErrInfo.m_oID < 0 )
		log( _T( "\t") );
	else
		log( _T( "%d\t"), cErrInfo.m_oID );
	// �o�x�E�ܓx
	if( cErrInfo.m_point.X != 0.0 && cErrInfo.m_point.Y != 0.0 )
	{
		log( _T( "%3.20lf\t"), cErrInfo.m_point.X );
		log( _T( "%3.20lf\t"), cErrInfo.m_point.Y );
	}
	else
		log( _T( "\t\t") );

	// �G���[�R�[�h�E�G���[���x���E�G���[���b�Z�[�W�E���b�V���R�[�h
	log( _T( "%s"), getErrorString(cErrInfo.m_emCode) );
	// ���2 - n ...���2�ȍ~���o�͂��Ȃ��P�[�X�ł́A�^�u���o�͂��Ȃ��悤�ɂ���
	if( lpcszFormat )
	{
		CString strLog;
		strLog.FormatV( lpcszFormat, vaInfo );
		log( _T( "\t%s"), strLog ); 
	}

	log( _T( "\n") ); 

	flush();
}

// �G���[���x���擾
LPCTSTR ErrLogger::getErrorLevel( emErrLevel emLevel ) const
{
	switch( emLevel )
	{
	case kOK:		return _T("OK");
	case kNotice:	return _T("NOTICE");
	case kWarn:		return _T("WARNING");
	case kError:	return _T("ERROR");
	case kCritical:	return _T("CRITICAL");
	default:
		return _T("UNKNOWN");
	}
}

// �G���[���b�Z�[�W�o��
CString ErrLogger::getErrorString( emCheck_ErrCode emCode ) const
{
	CString strMsg;
	switch( emCode )
	{

#define ERROR_CODE( code, value, level, msg_jp ) case code: strMsg.Format( _T("%s\t%08x\t%s"), getErrorLevel(level), code, L#msg_jp ); break;

#include "ErrorCode.h"
#undef ERROR_CODE
	default:
		return message::getMsg( eUnknownErrorCode );
	}
	return strMsg;
}
