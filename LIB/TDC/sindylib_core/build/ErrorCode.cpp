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
 * @file ErrorCode.cpp
 * @brief SiNDY�Ŏg�p����G���[�R�[�h�̒�`����
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "ErrorCode.h"
#include "GlobalFiles.h"
#include "GlobalFunctions.h"
#include <limits.h>
#include "sindymacroutil.h"
#include "winutil.h"
#include "resource.h"
#if defined(UNICODE) && defined(NDEBUG)
#include <stdio.h>
#endif // define(UNICODE) && define(NDEBUG)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace winutil;
namespace sindy {
namespace errorcode {

// sindyErrCode�𕶎���ɕϊ�����
LPCTSTR SINDYERRCODE2STRING( sindyErrCode code )
{
	switch( code )
	{
#ifdef UNICODE
#define ERROR_CODE(id,...) case id: return L#__VA_ARGS__; break;
#else // UNICODE
#define ERROR_CODE(id,...) case id: return #__VA_ARGS__; break;
#endif // UNICODE
#include "ErrorCodeBase.h"
#undef ERROR_CODE
		default: break;
	}
	return NULL;
}

// sindyErrCode���G���[���O�ɏo�͂���
void _ERRORLOG( sindyErrCode code, LPCTSTR lpcszFormat, ...)
{
	if( code == sindyErr_NoErr ) return;	// �G���[�����̓��O�ɏo���Ȃ�

	// �G���[���O�t�@�C���I�[�v��
	if( ! g_Errlog )
		errno_t err = _tfopen_s( &g_Errlog, g_strErrlog, _T("a") );

	va_list args = NULL;
	if( lpcszFormat )
		va_start(args, lpcszFormat);

	ErrorlogV( code, lpcszFormat, args, g_Errlog );

	// �㏈��
	va_end(args);
}

// sindyErrCode��HRESULT�l���G���[���O�ɏo�͂���
void _ERRORHRLOG( sindyErrCode code, HRESULT hr, LPCTSTR lpcszFormat, ...)
{
	if( code == sindyErr_NoErr && hr == S_OK ) return;	// �G���[�����̓��O�ɏo���Ȃ�

	// �G���[���O�t�@�C���I�[�v��
	if( ! g_Errlog )
		errno_t err = _tfopen_s( &g_Errlog, g_strErrlog, _T("a") );

	va_list args = NULL;
	if( lpcszFormat )
		va_start(args, lpcszFormat);

	ErrorlogV( code, lpcszFormat, args, g_Errlog, hr );

	// �㏈��
	va_end(args);
}

void ErrorlogV( sindyErrCode code, LPCTSTR lpcszFormat, va_list args/* = NULL*/, FILE* out/* = NULL*/, HRESULT hr/* = S_OK*/ )
{
	if( code == sindyErr_NoErr && hr == S_OK ) return;	// �G���[�����̓��O�ɏo���Ȃ�

	// �G���[�R�[�h����G���[������擾
	LPCTSTR lpszErrCode = SINDYERRCODE2STRING( code );

	// �t�H�[�}�b�g������쐬
	CString strFMsg;
	if( args )
		strFMsg.FormatV( lpcszFormat, args );
	else
		strFMsg = lpcszFormat;

	// COM�G���[������擾
	CString strCOMErr = GETCOMERRORSTRING();
	
	CString strDbgMsg;
	strDbgMsg.Format(_T("%s%s : %s\n"), strFMsg, lpszErrCode, strCOMErr );

	CString strMsg;
	strMsg.Format(_T("%s,%4d,%s,%s,%s\n"), GetTime(), code, lpszErrCode, strFMsg, strCOMErr );

#ifdef _DEBUG
#ifdef _UNICODE
	OutputDebugStringW(strDbgMsg);
#else
	OutputDebugStringA(strDbgMsg);
#endif // _UNICODE
#endif // DEBUG

	// �W���o�͂ɕ\������i�X�^�e�B�b�N�����N�ł̂݁j
#if ! defined(SINDYLIB_EXPORTS) || ! defined(USE_SINDYLIB_DLL)
	_tprintf_s( _T("%s"), strMsg );
#endif // ! defined(SINDYLIB_EXPORTS) || ! defined(USE_SINDYLIB_DLL)
	
	// �t�@�C���ɕ\������
	if( ! out )
	{
		_tprintf_s( strMsg );
		fflush( stdout );
	}
	else {
		_ftprintf_s( out, _T("%s"), strMsg );
		fflush( out );
	}
}

} // errorcode

} // sindy
