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
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR FREE_STYLE_LOG = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tMESHCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{�G���[���x��}\tCOMMENT{�G���[�R�[�h}\tCOMMENT{�G���[���b�Z�[�W}\tCOMMENT{���l}\tCOMMENT{�t���[�R�����g}\n");
#else
	LPCTSTR FREE_STYLE_LOG = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tmeshCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{error level}\tCOMMENT{error code}\tCOMMENT{error message}\tCOMMENT{note}\tCOMMENT{comment}\n");
#endif // SINDY_FOR_ENGLISH
}

void SINDYLOGHEADER( FILE* out, sindyLogFormat emFormat )
{
	CString strHeader;
	switch( emFormat )
	{
		case sindyLogFormat_Freestyle:
			strHeader= FREE_STYLE_LOG;
			break;
		case sindyLogFormat_Common:
		default:
			strHeader = _T("# SINDYSTDLOG\n");
			break;
	}
	// �t�@�C���ɏo��
	if( out )
	{
		_ftprintf( out, strHeader );
		fflush( out );
	}
}

// ���O�n���h���c�[���W���t�H�[�}�b�g�`���Ń��O���o�͂���
void SINDYCOMMONLOG( FILE* out, sindyLogFormat emFormat, LPCTSTR lpcszTableName, long lOID, const double& dX, const double& dY, long scale, sindyErrLevel emErrLevel, sindyErrCode emErr, LPCTSTR lpcszFormat/* = NULL*/, ... )
{
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
		{
			int x = 0, y = 0;
			unsigned int nMeshCode = LL2MESHcity( dX, dY, x, y );
			strLog.Format(_T("0\t%s\t%d\t%u\t%d\t%d\t%.20lf\t%.20lf\t%d\t%s\t%d\t%s\t"), lpcszTableName, lOID, nMeshCode, x, y, dY, dX, scale, strLevel, emErr, SINDYERRCODE2STRING(emErr) );
			break;
		}
		case sindyLogFormat_Common:
		default:
			strLog.Format(_T("0,%s,%d,%.20lf,%.20lf,%s,%d,%s,"), lpcszTableName, lOID, dX, dY, strLevel, emErr, SINDYERRCODE2STRING(emErr) );
			break;
	}

	if( NULL != lpcszFormat )
	{
		va_list args;
		va_start(args, lpcszFormat);

		int nBuf;
#ifdef _UNICODE
		WCHAR szBuffer[1024];
#else
		char szBuffer[1024];
#endif

		nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpcszFormat, args);
		SASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

		va_end(args);

		strLog += szBuffer;
	}
	strLog += _T("\n");

	// �t�@�C���ɏo��
	if( out )
	{
		_ftprintf( out, strLog );
		fflush( out );
	}

	// �W���o�͂ɏo��
	_tprintf( strLog );
	fflush(stdout);

	// �A�E�g�v�b�g�E�B���h�E�ɏo��
#ifdef DEBUG
#ifdef _UNICODE
	OutputDebugStringW(strLog);
#else
	OutputDebugStringA(strLog);
#endif // _UNICODE
#endif // DEBUG
}

} // sindy
