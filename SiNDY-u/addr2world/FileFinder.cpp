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

static char *bars[] = {"�^", "�\", "�_", "�b"};

/*================================================================
�t�@�C���̌����́A�܂��t�H���_���������A���̃��X�g���쐬���܂��B
�쐬���ꂽ���X�g�����̃t�@�C�����A�w�肳�ꂽ�p�^�[���Ō������A��
�X�g�𐶐����܂��B
����́ASearch�֐����p�^�[���w�肷�邱�ƂŃt�H���_�������ł���
�����߂ɂ��̂悤�ȕ��@���Ƃ��Ă��܂��B
=================================================================*/

FileFinder::FileFinder(void) :
m_FileList(),
m_DisplayStatus(false)
{
}

FileFinder::~FileFinder(void)
{
}
/* �s�v�ȋ@�\������
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
	// �����p�̍\����
	WIN32_FIND_DATA fd;

	// ��������t�@�C���̃p�X���쐬����
	ATL::CString temp;
	temp.Format(_T("%s\\*"), path);

	HANDLE handle = FindFirstFile(temp, &fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	do
	{
		// �u.�v�Ɓu..�v�̓X�L�b�v����
		if (lstrcmp(fd.cFileName, _T(".")) == 0 ||
			lstrcmp(fd.cFileName, _T("..")) == 0)
			continue;

		// �f�B���N�g�����������Ƃ��̏���
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// �p�X�̖����ɃT�u�f�B���N�g����ǉ�
			ATL::CString temp;
			temp.Format(_T("%s\\%s"), path, fd.cFileName);

			ATL::CString name = fd.cFileName;
			name.MakeLower();
			if (m_ExDir.find(name) != m_ExDir.end())
				continue;

			if (m_DisplayStatus)
			{
				std::cerr << bars[directories.size() % 4] << " �f�B���N�g����������...(" << directories.size() + 1 << " found.)\r";
			}

			// �p�X���ꗗ�ɒǉ�
			directories.insert(temp);

			// �T�u�f�B���N�g������������ꍇ�̏���
			if (sub_directories)
			{
				// �T�u�f�B���N�g���ɂ��Ă��ċA�I�Ƀt�@�C������������
				SearchDirectories(temp, sub_directories, directories);
			}
		}
	}
	while (FindNextFile(handle, &fd));

	// �����I��
	FindClose(handle);

	return true;
}

bool FileFinder::Search(LPCTSTR path, LPCTSTR pattern, bool sub_directories)
{
	// �����ΏۂƂȂ�t�H���_����������
	std::set<ATL::CString> directories;
	directories.insert(path); // �������g���i�[���Ă���
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
		// �����p�̍\����
		WIN32_FIND_DATA fd;

		// ��������t�@�C���̃p�X���쐬����
		ATL::CString temp;
		if (pattern != NULL)
			temp.Format(_T("%s\\%s"), *i, pattern);
		else
			temp = *i;

		// �w�肵���p�X�Ńp�^�[���Ɉ�v����ŏ��̃t�@�C���܂��̓f�B���N�g�����擾����
		HANDLE handle = FindFirstFile(temp, &fd);
		if (handle == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		// �w�肵���p�X�Ńp�^�[���Ɉ�v����t�@�C���܂��̓f�B���N�g���̈ꗗ���擾����
		do
		{
			// �u.�v�Ɓu..�v�̓X�L�b�v����
			if (lstrcmp(fd.cFileName, _T(".")) == 0 ||
				lstrcmp(fd.cFileName, _T("..")) == 0)
				continue;

			// �f�B���N�g�����������Ƃ��̏���
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			// �t�@�C�����������Ƃ��̏���
			else
			{
				// �t�@�C�����Ƀp�X����������
				ATL::CString temp;
				temp.Format(_T("%s\\%s"), *i, fd.cFileName);

				if (m_DisplayStatus)
				{
					std::cerr << bars[m_FileList.size() % 4] << " �t�@�C����������...(" << m_FileList.size() + 1 << " found.)\r";
				}

				// �t�@�C�������ꗗ�ɒǉ�����
				m_FileList.push_back(temp);
			}
		}
		while (FindNextFile(handle, &fd));

		// �����̏I��
		FindClose(handle);
	}

	if (m_DisplayStatus)
	{
		std::cerr << std::endl;
	}

	return true;
}
