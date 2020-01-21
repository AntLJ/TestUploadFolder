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

/**
 * @file SiNDYLogFunctions.cpp
 * @brief <b>SiNDY�Ńn���h�����O�\�ȃ��O�̃��[�e�B���e�B�֐��Q�����t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "SiNDYLogFunctions.h"
#include "meshutil.h"
#include "sindymacroutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef SINDY_FOR_ENGLISH
const static _TCHAR STR_HEADER[] = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tMESHCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{�G���[���x��}\tCOMMENT{�G���[�R�[�h}\tCOMMENT{�G���[���b�Z�[�W}\tCOMMENT{���l}\tCOMMENT{�t���[�R�����g}\n");
#else
const static _TCHAR STR_HEADER[] = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tmeshCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{error level}\tCOMMENT{error code}\tCOMMENT{error message}\tCOMMENT{note}\tCOMMENT{comment}\n");
#endif	// SINDY_FOR_ENGLISH

namespace sindy {
using namespace errorcode;

void SINDYLOGHEADER( FILE* out, sindyLogFormat emFormat )
{
	CStringA strHeader;
	switch( emFormat )
	{
		case sindyLogFormat_Freestyle:
			strHeader = STR_HEADER;
			break;
		case sindyLogFormat_Common:
		default:
			strHeader = "# SINDYSTDLOG\n";
			break;
	}
	// �t�@�C���ɏo��
	if( out )
	{
		fprintf( out, strHeader );
		fflush( out );
	}
}

// ���O�n���h���c�[���W���t�H�[�}�b�g�`���Ń��O���o�͂���
void SINDYCOMMONLOG2V( FILE* out, sindyLogFormat emFormat, LPCTSTR lpcszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, sindyErrCode emErr, bool console, bool dbg, LPCTSTR lpcszFormat, va_list args )
{
	CString sindyErr(SINDYERRCODE2STRING(emErr));

	// �G���[���x��������쐬
	CString strLevel;
	switch( emErrLevel )
	{
		case sindyErrLevel_INFO: strLevel = _T("INFO"); break;
		case sindyErrLevel_WARN: strLevel = _T("WARN"); break;
		default:                 strLevel = _T("ERR"); break;
	}
	// �o�͕�������쐬����
	CString strLog;
	switch( emFormat )
	{
		case sindyLogFormat_Freestyle:
		case sindyLogFormat_FreestyleTab:
		{
			int x = 0, y = 0;
			unsigned int nMeshCode = LL2MESHcity( dX, dY, x, y );
			if( sindyLogFormat_FreestyleTab == emFormat )
				strLog.Format(_T("0\t%s\t%d\t%u\t%d\t%d\t%.20lf\t%.20lf\t%d\t%s\t%d\t%s\t"), lpcszTableName, lOID, nMeshCode, x, y, dY, dX, scale, strLevel, emErr, sindyErr );
			else
				strLog.Format(_T("0,%s,%d,%u,%d,%d,%.20lf,%.20lf,%d,%s,%d,%s,"), lpcszTableName, lOID, nMeshCode, x, y, dY, dX, scale, strLevel, emErr, sindyErr );
			break;
		}
		case sindyLogFormat_Common:
		case sindyLogFormat_CommonTab:
		default:
			if( sindyLogFormat_CommonTab == emFormat )
				strLog.Format(_T("0\t%s\t%d\t%.20lf\t%.20lf\t%s\t%d\t%s\t"), lpcszTableName, lOID, dX, dY, strLevel, emErr, sindyErr );
			else
				strLog.Format(_T("0,%s,%d,%.20lf,%.20lf,%s,%d,%s,"), lpcszTableName, lOID, dX, dY, strLevel, emErr, sindyErr );
			break;
	}

	if( args )
		strLog.AppendFormatV( lpcszFormat, args );
	else
		strLog += lpcszFormat;

	strLog += "\n";

	// �t�@�C���ɏo��
	CStringA strLogA(strLog);
	if( out )
	{
		fprintf( out, strLogA );
		fflush( out );
	}

	// �W���o�͂ɏo��
	if( console )
	{
		printf( strLogA );
		fflush(stdout);
	}

	// �A�E�g�v�b�g�E�B���h�E�ɏo��
#ifdef DEBUG
	OutputDebugStringA(strLogA);
#endif // DEBUG
}

// ���O�n���h���c�[���W���t�H�[�}�b�g�`���Ń��O���o�͂���
void SINDYCOMMONLOG2( FILE* out, sindyLogFormat emFormat, LPCTSTR lpcszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, sindyErrCode emErr, bool console, bool dbg, LPCTSTR lpcszFormat/* = NULL*/, ... )
{
	va_list args = NULL;
	va_start(args, lpcszFormat);
	SINDYCOMMONLOG2V( out, emFormat, lpcszTableName, lOID, dX, dY, scale, emErrLevel, emErr, console, dbg, lpcszFormat, args );
	va_end(args);
}

// ���O�n���h���c�[���W���t�H�[�}�b�g�`���Ń��O���o�͂���
void SINDYCOMMONLOG( FILE* out, sindyLogFormat emFormat, LPCTSTR lpcszTableName, long lOID, double dX, double dY, long scale, sindyErrLevel emErrLevel, sindyErrCode emErr, LPCTSTR lpcszFormat/* = NULL*/, ... )
{
	va_list args = NULL;
	va_start(args, lpcszFormat);
	SINDYCOMMONLOG2V( out, emFormat, lpcszTableName, lOID, dX, dY, scale, emErrLevel, emErr, true, true, lpcszFormat, args );
	va_end(args);
}

} // sindy
