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

#pragma once

#include "tstring.h"
#include <assert.h>
#include <vector>

namespace uh
{
	/**
	 * @brief ディレクトリか？
	 * @param[in]	path	パス
	 * @retval	true	ディレクトリである
	 * @retval	false	それ以外である
	 */
	inline bool isDirectory(LPCTSTR path)
	{
		assert(path);

		return !!::PathIsDirectory(path);
	}

	/**
	 * @brief ファイルか？
	 * @param[in]	path	パス
	 * @retval	true	ファイルである
	 * @retval	false	それ以外である
	 */
	inline bool isFile(LPCTSTR path)
	{
		assert(path);

		if(::PathFileExists(path))
		{
			return ! isDirectory(path);
		}
		else
		{
			return false;
		}
	}

	/**
	 * @brief ディレクトリ直下のディレクトリ群を得る
	 * @note FindFirstFileの仕様に依存する為、specに拡張子を指定したときは、
	 *       拡張子の前方一致で絞り込まれる
	 * @param[in]	dir		ディレクトリ
	 * @param[in]	spec	ディレクトリ名フィルタ。"*.*"のように指定する。
	 * @param[out]	dirs	出力ディレクトリ群
	 */
	inline void getDirectoryNames(LPCTSTR dir, LPCTSTR spec, std::vector<tstring>& dirs)
	{
		assert(dir);
		assert(spec);

		tstring filespec = tstring(dir) + _T("\\") + spec;

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(filespec.c_str(), &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			// specに適合するファイルがないときは例外にしない
			if(::GetLastError() == ERROR_FILE_NOT_FOUND)
				return;

			std::string msg = std::string("Fail FindFirstFile: ") + toStr(filespec);
			throw std::runtime_error(msg);
		}

		do
		{
			tstring filename = fd.cFileName;
			if((filename == _T("."))||(filename == _T("..")))
				continue;

			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				dirs.push_back(filename);
			}
		} while(::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	/**
	 * @brief ディレクトリ直下のファイル群を得る
	 * @note FindFirstFileの仕様に依存する為、specに拡張子を指定したときは、
	 *       拡張子の前方一致で絞り込まれる
	 * @param[in]	dir		ディレクトリ
	 * @param[in]	spec	ファイル名フィルタ。"*.*"のように指定する。
	 * @param[out]	files	出力ファイル群
	 */
	inline void getFileNames(LPCTSTR dir, LPCTSTR spec, std::vector<tstring>& files)
	{
		assert(dir);
		assert(spec);

		tstring filespec = tstring(dir) + _T("\\") + spec;

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(filespec.c_str(), &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			// specに適合するファイルがないときは例外にしない
			if(::GetLastError() == ERROR_FILE_NOT_FOUND)
				return;

			std::string msg = std::string("Fail FindFirstFile: ") + toStr(filespec);
			throw std::runtime_error(msg);
		}

		do
		{
			tstring filename = fd.cFileName;
			if((filename == _T("."))||(filename == _T("..")))
				continue;

			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				files.push_back(filename);
			}
		} while(::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
}
