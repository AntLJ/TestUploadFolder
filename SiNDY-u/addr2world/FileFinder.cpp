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

#include "StdAfx.h"
#include <iostream>
#include "FileFinder.h"

static char *bars[] = {"／", "―", "＼", "｜"};

/*================================================================
ファイルの検索は、まずフォルダを検索し、そのリストを作成します。
作成されたリスト直下のファイルを、指定されたパターンで検索し、リ
ストを生成します。
これは、Search関数がパターン指定することでフォルダを検索できな
いためにこのような方法をとっています。
=================================================================*/

FileFinder::FileFinder(void) :
m_FileList(),
m_DisplayStatus(false)
{
}

FileFinder::~FileFinder(void)
{
}
/* 不要な機能を除去
void FileFinder::ClearExcludeDirectory()
{
	m_ExDir.clear();
}

void FileFinder::PutExcludeDirectory(LPCTSTR lpDir)
{
	ATL::CString tmp = lpDir;
	tmp.MakeLower();
	m_ExDir.insert(tmp);
}
*/
void FileFinder::Clear()
{
	m_FileList.clear();
}

int FileFinder::GetCount() const
{
	return (int)m_FileList.size();
}

ATL::CString FileFinder::GetFilePath(int index) const
{
	if (index < 0 || index >= (int)m_FileList.size())
		return _T("");
	return m_FileList[index];
}

bool FileFinder::SearchDirectories(LPCTSTR path, bool sub_directories, std::set<ATL::CString> &directories)
{
	// 検索用の構造体
	WIN32_FIND_DATA fd;

	// 検索するファイルのパスを作成する
	ATL::CString temp;
	temp.Format(_T("%s\\*"), path);

	HANDLE handle = FindFirstFile(temp, &fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	do
	{
		// 「.」と「..」はスキップする
		if (lstrcmp(fd.cFileName, _T(".")) == 0 ||
			lstrcmp(fd.cFileName, _T("..")) == 0)
			continue;

		// ディレクトリを見つけたときの処理
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// パスの末尾にサブディレクトリを追加
			ATL::CString temp;
			temp.Format(_T("%s\\%s"), path, fd.cFileName);

			ATL::CString name = fd.cFileName;
			name.MakeLower();
			if (m_ExDir.find(name) != m_ExDir.end())
				continue;

			if (m_DisplayStatus)
			{
				std::cerr << bars[directories.size() % 4] << " ディレクトリを検索中...(" << directories.size() + 1 << " found.)\r";
			}

			// パスを一覧に追加
			directories.insert(temp);

			// サブディレクトリも検索する場合の処理
			if (sub_directories)
			{
				// サブディレクトリについても再帰的にファイルを検索する
				SearchDirectories(temp, sub_directories, directories);
			}
		}
	}
	while (FindNextFile(handle, &fd));

	// 検索終了
	FindClose(handle);

	return true;
}

bool FileFinder::Search(LPCTSTR path, LPCTSTR pattern, bool sub_directories)
{
	// 検索対象となるフォルダを検索する
	std::set<ATL::CString> directories;
	directories.insert(path); // 自分自身を格納しておく
	if (sub_directories)
	{
		if (! SearchDirectories(path, sub_directories, directories))
			return false;

		if (m_DisplayStatus)
		{
			std::cerr << std::endl;
		}
	}

	for (std::set<ATL::CString>::iterator i = directories.begin(); i != directories.end(); i++)
	{
		// 検索用の構造体
		WIN32_FIND_DATA fd;

		// 検索するファイルのパスを作成する
		ATL::CString temp;
		if (pattern != NULL)
			temp.Format(_T("%s\\%s"), *i, pattern);
		else
			temp = *i;

		// 指定したパスでパターンに一致する最初のファイルまたはディレクトリを取得する
		HANDLE handle = FindFirstFile(temp, &fd);
		if (handle == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		// 指定したパスでパターンに一致するファイルまたはディレクトリの一覧を取得する
		do
		{
			// 「.」と「..」はスキップする
			if (lstrcmp(fd.cFileName, _T(".")) == 0 ||
				lstrcmp(fd.cFileName, _T("..")) == 0)
				continue;

			// ディレクトリを見つけたときの処理
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			// ファイルを見つけたときの処理
			else
			{
				// ファイル名にパスを結合する
				ATL::CString temp;
				temp.Format(_T("%s\\%s"), *i, fd.cFileName);

				if (m_DisplayStatus)
				{
					std::cerr << bars[m_FileList.size() % 4] << " ファイルを検索中...(" << m_FileList.size() + 1 << " found.)\r";
				}

				// ファイル名を一覧に追加する
				m_FileList.push_back(temp);
			}
		}
		while (FindNextFile(handle, &fd));

		// 検索の終了
		FindClose(handle);
	}

	if (m_DisplayStatus)
	{
		std::cerr << std::endl;
	}

	return true;
}
