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

// Logファイルオープン
bool FileController::initialize( const std::wstring& strDirPath,  const std::wstring&  mode )
{
	if (_wfopen_s(&m_pFile, strDirPath.c_str(), mode.c_str()) != 0)
	{
		wcerr << strDirPath << message::getMsg( eFailOpenFile );
		return false;
	}
	if (!m_pFile)
		return false;
	if (mode == SJIS_CREATE)
	{
		m_bUTF8 = false;
	}
	else
	{
		m_bUTF8 = true;
	}
	return true;
}

// ログへメッセージ出力
void FileController::log( const std::wstring&  strMessage)
{
	if (!m_bUTF8)
		fprintf(m_pFile, wstringToString(strMessage).c_str());
	else
		fwprintf(m_pFile, strMessage.c_str());
}

// wstringからstringへの変換
string FileController::wstringToString(const wstring& wstr)
{
	std::string str; 
	int nLength = WideCharToMultiByte(932, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL); 
    str.resize(nLength);
    LPSTR lpwszStr = new char[nLength];
	WideCharToMultiByte(932, 0, wstr.c_str(), -1, lpwszStr, nLength, NULL, NULL); 
    str = lpwszStr;
    delete [] lpwszStr;
	return str;
}