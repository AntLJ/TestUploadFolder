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
 * @file winutil.cpp
 * @brief ���[�e�B���e�B�[�֐��̎���
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "winutil.h"
#include "exdisp.h"
#include "shellapi.h"

namespace winutil {

bool BSTR2TCpy( LPTSTR lpszDst, BSTR bstrSrc, int nLen )
{
#ifdef _UNICODE
	if( (unsigned int)nLen >= SysStringLen(bstrSrc) + 1 )	// SysStringLen��NULL�������܂߂Ȃ����Alstrcpyn��NULL���������v������̂�+1
	{
		if( NULL != lstrcpyn( lpszDst, bstrSrc, nLen ) )
			return true;
		else {
			return false;
		}
	}
	else return false;
#else
	int nReqBufferSize = WideCharToMultiByte( GetACP(), 0, bstrSrc, -1, lpszDst, 0, NULL, NULL );	// �K�v�ȃo�b�t�@�T�C�Y���擾
	if( (int)sizeof(char*) * nLen < nReqBufferSize )
	{
		return false;
		//nRet = nReqBufferSize - sizeof(char*) * nLen;
	}
	else {
		if( 0 == WideCharToMultiByte( GetACP(), 0, bstrSrc, -1, lpszDst, sizeof(char*) * nLen, NULL, NULL ) )
		{
			return false;
			// GetLastError() �� ERROR_INSUFFICIENT_BUFFER �� ERROR_INVALID_FLAGS �� ERROR_INVALID_PARAMETER ��������͂�
		}
		else return true;
	}
#endif
}

// �E�B���h�E�̕�������擾����
CString GetWindowText( HWND hWnd )
{
	if( hWnd == NULL ) return _T("");

	USES_ATL_SAFE_ALLOCA;

	INT nLen = ::GetWindowTextLength( hWnd ) + 1;
	LPTSTR lpszText = (LPTSTR)_ATL_SAFE_ALLOCA( nLen * sizeof(TCHAR), _ATL_SAFE_ALLOCA_DEF_THRESHOLD );
	if( lpszText )
		::GetWindowText( hWnd, lpszText, nLen );
	else
		_ASSERTE( lpszText );

	CString strRet( lpszText );

	return strRet;
}

// ���Ԃ𕶎���Ŏ擾����
CString GetTime( LPSYSTEMTIME lpSystemTime/* = NULL*/ )
{
	SYSTEMTIME stSystemTime;
	if( NULL != lpSystemTime)
	{
		CopyMemory( &stSystemTime, lpSystemTime, sizeof( SYSTEMTIME));
	}
	else
	{
		::GetLocalTime( &stSystemTime);
	}
	CString cStrModifyDate;
	cStrModifyDate.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	return cStrModifyDate;
}

// ������Ń}�V�������擾����
CString GetMachineName()
{
	TCHAR				strMachineName[64];
	DWORD				nSize = 63;
	ZeroMemory(strMachineName, 64 * sizeof(TCHAR));
	::GetComputerName(strMachineName, &nSize);

	return strMachineName;
}

// �������OS�����擾����
CString GetOperatingSystem()
{
	CString strVerName;
	OSVERSIONINFO tInfo;
	ZeroMemory( &tInfo, sizeof( OSVERSIONINFO));
	tInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (::GetVersionEx(&tInfo) != 0)
	{
		switch(tInfo.dwMajorVersion)
		{
		case 3:
			strVerName = _T("Windows NT 3.51");
			break;
		case 4:
			if (tInfo.dwMinorVersion == 0 && tInfo.dwPlatformId == 1)
				strVerName = _T("Windows 95");
			else if (tInfo.dwMinorVersion == 0 && tInfo.dwPlatformId == 2)
				strVerName = _T("Windows NT 4");
			else if (tInfo.dwMinorVersion == 10)
				strVerName = _T("Windows 98");
			else
				strVerName = _T("Unknown");
			break;
		case 5:
			if (tInfo.dwMinorVersion == 0)
				strVerName = _T("Windows 2000");
			else if (tInfo.dwMinorVersion == 1)
				strVerName = _T("Windows XP");
			else
				strVerName = _T("Unknown");
			break;
		default:
			strVerName = _T("Unknown");
			break;
		}
	}
	else
	{
		strVerName.Empty();
	}

	return strVerName;
}

_COM_SMARTPTR_TYPEDEF( IWebBrowser2, __uuidof(IWebBrowser2) );

// �ʃE�B���h�E�Ńu���E�U��\������
void Navigate( LPCTSTR lpcszURL )
{
	// ���̊֐���sindykTools����R�s�[���܂����B

	// �f�t�H���g�̃u���E�U�ŉ����g���Ă��邩�擾
	CString strPath;
	CRegKey cRegKey;
	if( ERROR_SUCCESS == cRegKey.Open( HKEY_CLASSES_ROOT, _T("HTTP\\shell\\open\\command") ) )
	{
		TCHAR cPath[1024] = {0};
		ULONG ulLen = 1024;
		if( ERROR_SUCCESS == cRegKey.QueryStringValue( NULL, cPath, &ulLen ) )
			strPath = cPath;
		cRegKey.Close();
	}

	//[Bug 6227]���URL�͐V�����E�B���h�E�ŊJ���悤�ɂ��ė~�����B
	// ShellExecute���ƁAIE�����J���Ȃ��̂ŁA�ꍇ�킯
	if( strPath.Find( _T("iexplore.exe") ) > 0 )
	{
		IWebBrowser2Ptr pIE( __uuidof( InternetExplorer ) );
		if ( pIE ) {
			CComVariant dummy1, dummy2, dummy3, dummy4;
			CComVariant url( (CComBSTR)lpcszURL );

			//IE�̕\��
			pIE->Navigate2( &url, &dummy1, &dummy2, &dummy3, &dummy4 );
			pIE->put_Visible(VARIANT_TRUE);
		}
	}
	// IE�ȊO�̓^�u�u���E�U���낤�Ǝv����A�A�A�̂ŁAShellExecute�Ŋy�ɂ���Ă����
	else
		ShellExecute(0, _T("open"), lpcszURL, 0, 0, SW_SHOWNORMAL);
}

// �N���b�v�{�[�h�ɕ�������R�s�[����֐�
bool SetToClipboard( const CString& strText )
{
	int iLength = strText.GetLength();
	if (iLength > 1024)
		iLength = 1024;
	HGLOBAL hGlobal = ::GlobalAlloc(GHND, 2*( iLength + 1 ));
	if (hGlobal == NULL)
		return false;

	//�O���[�o���������̈�̃��b�N
	LPTSTR str = (LPTSTR)::GlobalLock(hGlobal);
	if( ! str )
	{
		GlobalFree(hGlobal);
		return false;
	}

	::lstrcpy(str,strText);

	//�O���[�o���������̈�̃��b�N����
	::GlobalUnlock(hGlobal);

	//	�N���b�v�{�[�h���J��
	if( !( ::OpenClipboard(NULL) ) )
	{
		GlobalFree(hGlobal);
		return false;
	}
	if( !( ::EmptyClipboard() ) )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return false;
	}
	if( ::SetClipboardData(CF_UNICODETEXT, hGlobal) == NULL )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return false;
	}
	::CloseClipboard();//�N���b�v�{�[�h�̃N���[�Y

	return true;
}

// �N���b�v�{�[�h�̕������Ԃ��֐�
CString GetFromClipboard()
{
	CString str;
	// �N���b�v�{�[�h�̃f�[�^���e�L�X�g�^�����肷��
	UINT PriList[2] = {CF_UNICODETEXT,CF_TEXT};
	int format = ::GetPriorityClipboardFormat(PriList, 2);
	if ( format == CF_TEXT || format == CF_UNICODETEXT )
	{
		OpenClipboard(NULL);
		HANDLE hText = GetClipboardData(format);
		if(hText == NULL)
			return str;
		else
		{
			LPTSTR lpData = static_cast<LPTSTR>(GlobalLock(hText));
			str = lpData;
			GlobalUnlock(hText);
		}

		CloseClipboard();
	}
	return str;
}

} // winutil
