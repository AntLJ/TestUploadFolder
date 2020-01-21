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
	 * @brief �f�B���N�g�����H
	 * @param[in]	path	�p�X
	 * @retval	true	�f�B���N�g���ł���
	 * @retval	false	����ȊO�ł���
	 */
	inline bool isDirectory(LPCTSTR path)
	{
		assert(path);

		return !!::PathIsDirectory(path);
	}

	/**
	 * @brief �t�@�C�����H
	 * @param[in]	path	�p�X
	 * @retval	true	�t�@�C���ł���
	 * @retval	false	����ȊO�ł���
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
	 * @brief �f�B���N�g�������̃f�B���N�g���Q�𓾂�
	 * @note FindFirstFile�̎d�l�Ɉˑ�����ׁAspec�Ɋg���q���w�肵���Ƃ��́A
	 *       �g���q�̑O����v�ōi�荞�܂��
	 * @param[in]	dir		�f�B���N�g��
	 * @param[in]	spec	�f�B���N�g�����t�B���^�B"*.*"�̂悤�Ɏw�肷��B
	 * @param[out]	dirs	�o�̓f�B���N�g���Q
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
			// spec�ɓK������t�@�C�����Ȃ��Ƃ��͗�O�ɂ��Ȃ�
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
	 * @brief �f�B���N�g�������̃t�@�C���Q�𓾂�
	 * @note FindFirstFile�̎d�l�Ɉˑ�����ׁAspec�Ɋg���q���w�肵���Ƃ��́A
	 *       �g���q�̑O����v�ōi�荞�܂��
	 * @param[in]	dir		�f�B���N�g��
	 * @param[in]	spec	�t�@�C�����t�B���^�B"*.*"�̂悤�Ɏw�肷��B
	 * @param[out]	files	�o�̓t�@�C���Q
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
			// spec�ɓK������t�@�C�����Ȃ��Ƃ��͗�O�ɂ��Ȃ�
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
