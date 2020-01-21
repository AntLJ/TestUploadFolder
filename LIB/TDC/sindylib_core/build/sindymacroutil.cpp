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
 * @file util.cpp
 * @brief ���[�e�B���e�B�[�֐��̎���
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "sindymacroutil.h"
#include "GlobalFiles.h"
#include "GlobalFunctions.h"
#include "winutil.h"

#ifdef _UNICODE
#include <stdio.h>
#endif // _UNICODE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace winutil;

bool g_bTRACEMESSAGE_STDOUT = true; // TRACEMESSAGE��W���o�͂ɏo�����ǂ���

#define MessagePump()	{ MSG msg; while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){ TranslateMessage( &msg ); DispatchMessage( &msg ); } }

// �g���[�X���o��
void TRACEMESSAGE( FILE* out, LPCTSTR lpcszFormat, ...)
{
	va_list args = NULL;
	va_start(args, lpcszFormat);

#ifdef _LIB
	TraceMessageV( lpcszFormat, args, out, true, true );
#else
	TraceMessageV( lpcszFormat, args, out, true );
#endif // ifdef _LIB

	va_end(args);
}

// �g���[�X���o��
void TRACEMESSAGE( LPCTSTR lpcszFormat, ...)
{
	// �g���[�X���O�t�@�C���I�[�v��
	if( ! sindy::g_Tracelog )
		sindy::g_Tracelog = _tfopen( sindy::g_strTracelog, _T("a") );

	va_list args = NULL;
	va_start(args, lpcszFormat);

#ifdef _LIB
	TraceMessageV( lpcszFormat, args, sindy::g_Tracelog, true, g_bTRACEMESSAGE_STDOUT );
#else
	TraceMessageV( lpcszFormat, args, sindy::g_Tracelog, true );
#endif // ifdef _LIB

	va_end(args);
}

//�g���[�X���o��
void TRACEMESSAGE( IProgressor* ipProgress, long lStep, bool bIsPercent, bool bAddMsg, LPCTSTR lpcszFormat, ...)
{
	// �g���[�X���O�t�@�C���I�[�v��
	if( ! sindy::g_Tracelog )
		sindy::g_Tracelog = _tfopen( sindy::g_strTracelog, _T("a") );

	va_list args = NULL;
	va_start(args, lpcszFormat);

#ifdef _LIB
		TraceMessageV( lpcszFormat, args, sindy::g_Tracelog, true, true, ipProgress, true, ( 0 <= lStep ) ? true : false, bIsPercent, lStep, bAddMsg );
#else
	TraceMessageV( lpcszFormat, args, sindy::g_Tracelog, true, false, ipProgress, true, ( 0 <= lStep ) ? true : false, bIsPercent, lStep, bAddMsg );
#endif // ifdef _LIB

	va_end(args);
}

void TraceMessageV( LPCTSTR lpcszFormat, 
				   va_list args/* = NULL*/, 
				   FILE* out/* = NULL*/,
				   bool bOutDebugger/* = false*/, 
				   bool bOutStdOut/* = false*/, 
				   IProgressor* ipProgress/* = NULL*/, 
				   bool bStep/* = false*/,
				   bool bUseStepValue/* = false*/,
				   bool bIsPercent/* = false*/,
				   long step/* = 0*/,
				   bool bAddMsg/* = false*/
				    )
{
	if( lpcszFormat )
	{
		// ���b�Z�[�W�쐬
		CString strMsg, strTime = GetTime();
		if( args )
			strMsg.FormatV(lpcszFormat, args);
		else
			strMsg = lpcszFormat;

		if( bOutDebugger )
		{
#ifdef DEBUG
#ifdef _UNICODE
		OutputDebugStringW(strMsg);
#else
		OutputDebugStringA(strMsg);
#endif // _UNICODE
#endif // DEBUG
		}

		// �t�@�C���o��
		if( out )
		{
#ifdef _UNICODE
			fprintf_s( out, "# %S %S", strTime, strMsg ); 
#else
			fprintf_s( out, "# %s %s", strTime, strMsg ); 
#endif // ifdef _UNICODE
			fflush( out );
		}

		// �W���o��
		if( bOutStdOut ) { _tprintf_s( _T("%s"), strMsg ); fflush( stdout ); }

		// Progressor�֏o��
		if( ipProgress )
		{
			if( bAddMsg )
			{
				CComBSTR bstrMsg;
				ipProgress->get_Message( &bstrMsg );
				bstrMsg.Append( strMsg );
				ipProgress->put_Message( bstrMsg );
			}
			else
				ipProgress->put_Message( CComBSTR(strMsg) );
			if( bStep )
			{
				IStepProgressorPtr ipStep( ipProgress );
				if( NULL != ipStep )
				{
					if( bUseStepValue )
					{
						if( bIsPercent )
						{
							long lMax, lMin, lRange;
							ipStep->get_MaxRange( &lMax );
							ipStep->get_MinRange( &lMin );
							lRange = lMax - lMin;
							ipStep->put_Position( lMin + (long)( (double)lRange * ( (double)step / 100.0 ) ) );
/*#ifdef _DEBUG
							ATLTRACE(_T("PROGRESS:%d percent.\n"), lMin + (long)( (double)lRange * ( (double)step / 100.0 ) ) );
#endif // ifdef _DEBUG*/
						}
						else
							ipStep->put_Position( step );
					}
					else
						ipStep->Step();
				}
			}
			// ���b�Z�[�W�|���v����
			MSG msg;
			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){ TranslateMessage( &msg ); DispatchMessage( &msg ); }
		}
	}
}

// COM�G���[���擾����֐�
CString GETCOMERRORSTRING()
{
	// COM�G���[������擾
	CString strCOMErr;
	IErrorInfoPtr pError;

	USES_CONVERSION;

	::GetErrorInfo( 0, &pError );
	if( pError != NULL )
	{
		CComBSTR      strDescription, strSource;
		pError->GetDescription( &strDescription );
		pError->GetSource( &strSource );

		strCOMErr.Format(_T("%s(%s)"), OLE2T(strDescription), OLE2T(strSource) );
	}
	return strCOMErr;
}

#if 0
#include "Row.h"


/**
 * @brief CRow���瑮���l�����o���B
 *
 * @note
 * Where.cpp �� Row.h ���C���N���[�h����ƁAWindows XP SP2 + ArcGIS 9.1 ������C1204�G���[���������邽�߁A�l�����o���֐��������ɗp�ӂ��ĂƂ肠��������B
 */
void GetValueByCRow(const sindy::CRow& rRow, LPCTSTR lpcszFieldName, VARIANT& vaValue)
{
	rRow.GetValueByFieldName(lpcszFieldName, vaValue);
}
#endif // if 0
