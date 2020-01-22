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
#include "RunLogger.h"
#include "WinLib/VersionInfo.h"
#include <WinLib/winutil.h>
#include <sindy/libschema.h>

using namespace std;
using namespace uh::str_util;
using namespace uh::variant_util;
using namespace meshtype;

void CRunLogger::header( const Arguments& args )
{
	CString toolInfo;
	CVersion ver;
	toolInfo += ver.GetOriginalFilename();
	toolInfo += _T(" FV:") + ver.GetFileVersion();
	toolInfo += _T(" PV:") + ver.GetProductVersion() + _T("\n");
	write( toolInfo );
	
	write( format( _T("FIX�o�[�W����DB:%s"), args.m_BaseDB.c_str() ) );
	write( format( _T("�ҏW�o�[�W����DB:%s"), args.m_CompDB.c_str() ) );
	write( format( _T("���s���O:%s"), args.m_Runlog.c_str() ) );
	write( format( _T("�G���[���O:%s"), args.m_Errlog.c_str() ) );
	write(
		format(_T("�w��X�V��:%s"), (!isNullOrEmpty(args.m_ModDate) ? CString(args.m_ModDate).GetString() : _T("�Ȃ�")))
		);
}

void CRunLogger::writeParseMeshResult( const MeshType& type, const map<int, CString>& mapInvalidRow )
{
	write( format( _T("�������b�V���P��:%s\n"), getStrMeshType(type).c_str() ) );

	if( mapInvalidRow.empty() )
		return;

	// ���b�V�����X�g�ɕs���ȕ������܂܂�Ă�����A���b�V�������݂��Ă���ꍇ�̓��O�Ɏc��
	for( const auto& row : mapInvalidRow )
	{
		CString msg = _T("ERROR : ");

		if( kInvalid == type )
			msg += _T("���b�V�����X�g�ɕs��������܂�");
		else
			msg += _T("���b�V���R�[�h�����݂��Ă��܂�");

		msg += format(_T("(%d�s��:\""), row.first) + row.second + _T("\")");
		write( msg );
	}
}

void CRunLogger::writeExeTime( bool isStart )
{
	write( ( isStart ? _T("�J�n����: ") : _T("\n�I������: ") ) + winutil::GetTime() + _T("\n") );
}

void CRunLogger::writeErrMsg( const CString& msg )
{
	writeLogAndConsole( _T("ERROR : ") + msg );
}

void CRunLogger::writeLogAndConsole( const CString& msg )
{
	write( msg );
	wcerr << msg.GetString() << endl;
}
