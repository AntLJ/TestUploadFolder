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
#include "PathFinder.h"
#include <sstream>

std::vector<uh::tstring> PathFinder::getFilePath(const uh::tstring& directoryName)
{
	namespace sys = std::tr2::sys;
	std::vector<uh::tstring> ret;
	sys::wpath p(directoryName.c_str());

	for (sys::wdirectory_iterator itr(p); itr != sys::wdirectory_iterator(); ++itr)
	{
		// �f�B���N�g�������������ꍇ
		if (sys::is_directory(p))
		{
			// �ċA�I�Ɏ擾���A���݂̖����ɃC���T�[�g
			uh::tstring dirPath(directoryName + _T("\\") + itr->path().string());
			std::vector<uh::tstring> tmpvec( getFilePath( dirPath ) );
			ret.insert(ret.end(), tmpvec.begin(),tmpvec.end());
		}

		ret.push_back(directoryName + _T("\\") +itr->path().string());
	}
	return ret;
}

std::vector<uh::tstring> PathFinder::getFilePath(const uh::tstring& directoryName, uh::tstring filter)
{
	std::vector<uh::tstring> ret;

	// �S�Ẵp�X�Ƀt�B���^��������
	std::vector<uh::tstring> tmp( getFilePath(directoryName) );
	for(const auto& path : tmp )
	{
		// �t�B���^�[�Ώۂ̕����񂪂��邩
		if ( uh::tstring::npos != path.find(filter , 0 ) )
		{
			// �t�B���^�[�����񂪑��݂���ꍇ�i�[
			ret.push_back(path);
		}
	}

	return ret;
}

std::vector<uh::tstring> PathFinder::getFilePath(const uh::tstring& directoryName, std::vector<uh::tstring> filters)
{
	std::vector<uh::tstring> ret;

	// �S�Ẵp�X�Ƀt�B���^��������
	std::vector<uh::tstring> tmp( getFilePath(directoryName) );
	tmp.push_back(directoryName);// �w�肵���p�X���g���ǉ�����i1�t�@�C���̂ݎw��ɂ��Ή����邽�߁j
	for(const auto& path : tmp )
	{
		// �t�B���^�[�Ώۂ̕����񂪂��邩
		bool isAddPath(false);
		for(const auto& filterName: filters)
		{
			if ( uh::tstring::npos != path.find(filterName , 0 ) )
			{
				isAddPath = true;
				break;
			}
		}

		// �t�B���^�[�����񂪑��݂���ꍇ�i�[
		if (isAddPath)
		{
			ret.push_back(path);
		}
	}

	return ret;
}

std::vector<uh::tstring> PathFinder::getFilePathByExtension(const uh::tstring& directoryName, uh::tstring extension)
{
	std::vector<uh::tstring> ret;

	// �S�Ẵp�X�Ƀt�B���^��������
	std::vector<uh::tstring> tmp( getFilePath(directoryName,extension) );
	for(const auto& path : tmp )
	{
		uh::tstring::size_type expos = path.rfind(_T(".") );
		if ( uh::tstring::npos == expos )
		{
			continue;
		}

		expos += uh::tstring(_T(".")).size();
		// �u.�v���疖���܂Ŏ擾�����ꂪ�w�肵���g���q������
		uh::tstring ex = path.substr(expos);
		if (ex == extension)
		{
			// �g���q�̕����񂪑��݂���ꍇ�i�[
			ret.push_back(path);
		}
	}

	return ret;
}

std::vector<uh::tstring> PathFinder::getFilePathByExtension(const uh::tstring& directoryName, std::vector<uh::tstring> extensions)
{
	std::vector<uh::tstring> ret;

	// �S�Ẵp�X�Ƀt�B���^��������
	std::vector<uh::tstring> tmp( getFilePath(directoryName,extensions) );
	for(const auto& path : tmp )
	{
		uh::tstring::size_type expos = path.rfind(_T("."));
		if ( uh::tstring::npos == expos )
		{
			continue;
		}
		expos += uh::tstring(_T(".")).size();
		// �u.�v���疖���܂Ŏ擾�����ꂪ�w�肵���g���q������
		uh::tstring ex = path.substr(expos);
		for(const auto& extension : extensions)
		{
			if (ex == extension)
			{
				// �g���q�̕����񂪑��݂���ꍇ�i�[
				ret.push_back(path);
			}
		}
	}

	return ret;
}
