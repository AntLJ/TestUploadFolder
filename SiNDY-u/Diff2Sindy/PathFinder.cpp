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
		// ディレクトリが見つかった場合
		if (sys::is_directory(p))
		{
			// 再帰的に取得し、現在の末尾にインサート
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

	// 全てのパスにフィルタをかける
	std::vector<uh::tstring> tmp( getFilePath(directoryName) );
	for(const auto& path : tmp )
	{
		// フィルター対象の文字列があるか
		if ( uh::tstring::npos != path.find(filter , 0 ) )
		{
			// フィルター文字列が存在する場合格納
			ret.push_back(path);
		}
	}

	return ret;
}

std::vector<uh::tstring> PathFinder::getFilePath(const uh::tstring& directoryName, std::vector<uh::tstring> filters)
{
	std::vector<uh::tstring> ret;

	// 全てのパスにフィルタをかける
	std::vector<uh::tstring> tmp( getFilePath(directoryName) );
	tmp.push_back(directoryName);// 指定したパス自身も追加する（1ファイルのみ指定にも対応するため）
	for(const auto& path : tmp )
	{
		// フィルター対象の文字列があるか
		bool isAddPath(false);
		for(const auto& filterName: filters)
		{
			if ( uh::tstring::npos != path.find(filterName , 0 ) )
			{
				isAddPath = true;
				break;
			}
		}

		// フィルター文字列が存在する場合格納
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

	// 全てのパスにフィルタをかける
	std::vector<uh::tstring> tmp( getFilePath(directoryName,extension) );
	for(const auto& path : tmp )
	{
		uh::tstring::size_type expos = path.rfind(_T(".") );
		if ( uh::tstring::npos == expos )
		{
			continue;
		}

		expos += uh::tstring(_T(".")).size();
		// 「.」から末尾まで取得しそれが指定した拡張子か判定
		uh::tstring ex = path.substr(expos);
		if (ex == extension)
		{
			// 拡張子の文字列が存在する場合格納
			ret.push_back(path);
		}
	}

	return ret;
}

std::vector<uh::tstring> PathFinder::getFilePathByExtension(const uh::tstring& directoryName, std::vector<uh::tstring> extensions)
{
	std::vector<uh::tstring> ret;

	// 全てのパスにフィルタをかける
	std::vector<uh::tstring> tmp( getFilePath(directoryName,extensions) );
	for(const auto& path : tmp )
	{
		uh::tstring::size_type expos = path.rfind(_T("."));
		if ( uh::tstring::npos == expos )
		{
			continue;
		}
		expos += uh::tstring(_T(".")).size();
		// 「.」から末尾まで取得しそれが指定した拡張子か判定
		uh::tstring ex = path.substr(expos);
		for(const auto& extension : extensions)
		{
			if (ex == extension)
			{
				// 拡張子の文字列が存在する場合格納
				ret.push_back(path);
			}
		}
	}

	return ret;
}
