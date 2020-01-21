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
 * @file
 * @brief ArcCatalogの「データベース コネクション」ファイルを保存しているフォルダへのパスをいじるライブラリ。
 *
 * - @ref database_connection_alias
 */
#ifndef ARCTL_CATALOG_H_
#define ARCTL_CATALOG_H_

#include <string>
#include <exception>
#include <tchar.h>
#include <errno.h>
#include <shlobj.h>
#include <TDC/useful_headers/registry_util.h>

namespace arctl {

/**
 * @brief ArcCatalog のデータベースコネクションファイルが保存されているディレクトリの絶対パスを取得する。
 */
inline std::basic_string<TCHAR> get_sde_file_directory_path()
{
	_TCHAR szPath[MAX_PATH];
	if(! SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE))
		throw std::runtime_error("SHGetSpecialFolderPath() error.");

	CString versionStr = uh::reg_util::read(
								_T("RealVersion"),
								_T("SOFTWARE\\ESRI\\ArcGIS"),
								CComVariant(), false,
								HKEY_LOCAL_MACHINE
								);
	if(versionStr.IsEmpty()){
		throw std::runtime_error("uh::reg_util::read() error.");
	}

	versionStr = versionStr.Left(versionStr.ReverseFind(_T('.')));
	CString fileDirectory;
	fileDirectory.Format(_T("\\ESRI\\Desktop%s\\ArcCatalog"),versionStr);
	switch(::_tcscat_s(szPath, MAX_PATH, fileDirectory)) {
	case EINVAL:
		throw std::runtime_error("_tcscat_s() EINVAL error.");
	case ERANGE:
		throw std::range_error("_tcscat_s() ERANGE error.");
	}

	return szPath;
}

/**
 * @brief ArcCatalog のデータベースコネクションファイルが保存されているディレクトリクラス
 */
class sde_file_directory
{
public:
	sde_file_directory() :
	m_strPath(get_sde_file_directory_path()),
	m_strAlias(_T("データベース コネクション"))
	{
	}

	/**
	 * @brief データベースコネクションファイルが保存されているディレクトリの絶対パスを取得する。
	 */
	const std::basic_string<TCHAR>& path_string() const
	{
		return m_strPath;
	}

	/**
	 * @brief 「データベース コネクション」という文字列を返す。
	 */
	const std::basic_string<TCHAR>& alias_string() const
	{
		return m_strAlias;
	}

private:
	const std::basic_string<TCHAR> m_strPath;
	const std::basic_string<TCHAR> m_strAlias;
};

/**
 * @defgroup database_connection_alias 「データベース コネクション」パス文字列相互変換
 */
//@{
/**
 * @brief データベースコネクションファイルが保存されているディレクトリのパスを「データベース コネクション」という文字列に変換する。
 *
 * @param[in,out] rPath パス。データベースコネクションファイルが保存されているディレクトリのパスじゃなくても良い。
 * @param[in] rSdeFileDir データベースコネクションファイルが保存されているディレクトリを指すオブジェクト。省略化。
 */
inline void alias_sde_file_directory_path(
	std::basic_string<TCHAR>& rPath,
	const sde_file_directory& rSdeFileDir = sde_file_directory()
)
{
	const std::basic_string<TCHAR>& rSdeFileDirPath = rSdeFileDir.path_string();
	const std::basic_string<TCHAR>::size_type nDirectoryPathLen = rSdeFileDirPath.size();
	if(::_tcsnicmp(rSdeFileDirPath.c_str(), rPath.c_str(), nDirectoryPathLen) == 0) {
		rPath = rSdeFileDir.alias_string() + (rPath.c_str() + nDirectoryPathLen);
	}
}

/**
 * @brief 「データベース コネクション」という文字列をデータベースコネクションファイルが保存されているディレクトリのパスに変換する。
 *
 * @param[in,out] rPath 「データベース コネクション」という文字列を含むかもしれない文字列。
 * @param[in] rSdeFileDir データベースコネクションファイルが保存されているディレクトリを指すオブジェクト。省略化。
 */
inline void unalias_sde_file_directory_path(
	std::basic_string<TCHAR>& rPath,
	const sde_file_directory& rSdeFileDir = sde_file_directory()
)
{
	const std::basic_string<TCHAR>& rAliasString = rSdeFileDir.alias_string();
	const std::basic_string<TCHAR>::size_type nAliasLen = rAliasString.size();
	if(::_tcsnicmp(rAliasString.c_str(), rPath.c_str(), nAliasLen) == 0) {
		rPath = rSdeFileDir.path_string() + (rPath.c_str() + nAliasLen);
	}
}
//@}

} // namespace arctl

#endif // ARCTL_CATALOG_H_
