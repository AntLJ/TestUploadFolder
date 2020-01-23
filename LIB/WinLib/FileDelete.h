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

#ifndef FILE_DELETE_H_
#define FILE_DELETE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>

// 定義
//////////////////////////////////////////////////////////////////////

/**
 * @brief ファイル削除関数クラス
 */
class FileDelete  
{
public:
	bool operator()(LPCTSTR lpszFilePath);

private:
	WIN32_FIND_DATA m_fd;
};

// 実装
//////////////////////////////////////////////////////////////////////

/**
 * @brief ファイルを削除する
 *
 * @param lpszFilePath [in] ファイルパス
 * @retval true 成功。
 * @retval false 失敗。
 */
inline bool FileDelete::operator ()(LPCTSTR lpszFilePath)
{
	DWORD dwAttribute = ::GetFileAttributes(lpszFilePath);
	if(dwAttribute == -1)
		return false;

	if(::_tcslen(lpszFilePath) >= MAX_PATH - 3)
		return false;

	bool bResult = true;

	if(dwAttribute & FILE_ATTRIBUTE_DIRECTORY) {
		TCHAR szChildFilePath[MAX_PATH];
		::_tcscpy(szChildFilePath, lpszFilePath);
		::_tcscat(szChildFilePath, _T("\\*"));
		LPTSTR p = ::_tcschr(szChildFilePath, _T('*'));

		HANDLE hFind = ::FindFirstFile(szChildFilePath, &m_fd);
		if(hFind != INVALID_HANDLE_VALUE) {
			FileDelete deleteFile;
			do {
				if(! (::_tcscmp(m_fd.cFileName, _T(".")) == 0 || ::_tcscmp(m_fd.cFileName, _T("..")) == 0) ) {
					::_tcscpy(p, m_fd.cFileName);
					bResult &= deleteFile(szChildFilePath);
				}
			} while(::FindNextFile(hFind, &m_fd));
			::FindClose(hFind);
		}
		bResult &= (::RemoveDirectory(lpszFilePath) != FALSE);
	}
	else {
		bResult = (::DeleteFile(lpszFilePath) != FALSE);
	}

	return bResult;
}

#endif // FILE_DELETER_H_
