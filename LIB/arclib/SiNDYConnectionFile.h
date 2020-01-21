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

#ifndef __SINDY_CONNECTION_FILE_H__
#define __SINDY_CONNECTION_FILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include <shlobj.h>
#include "exportimport.h"
#include <TDC/useful_headers/registry_util.h>

namespace sindy {

/**
 * @brief 接続ファイル（*.sde）クラス
 */
class SINDYLIB_API ConnectionFile
{
public:
// 定義
	/**
	 * @brief 標準接続ファイルフォルダクラス
	 */
	class Folder
	{
	public:
		Folder();

		const CString& getPath() const { return m_strPath; }

		CString complementFullPath(LPCTSTR lpszFileName) const;
		CString convertPathFullPathToAlias(LPCTSTR lpszPath) const;
		CString convertPathAliasToFullPath(LPCTSTR lpszAliasPath) const;

		static SINDYLIB_API const TCHAR m_sAliasRootString[];

	private:
		CString m_strPath;
	};

	/**
	 * @brief フォルダ内SDEファイル走査クラス
	 */
	class SINDYLIB_API Finder
	{
	public:
		Finder(const Folder& rFolder);
		~Finder();

		LPWIN32_FIND_DATA next();

	private:
		void open(LPCTSTR lpszPath);
		void close();

	// 変数
		HANDLE m_hFindFile;
		bool m_bFirst;
		WIN32_FIND_DATA m_FindData;
	};

	/**
	 * @brief 接続時に、接続ファイル名と接続の正否を出力するクラス
	 */
	class SINDYLIB_API ConsoleConnector
	{
	public:
		ConsoleConnector(std::ostream& rStream, const Folder& rFolder);

		IWorkspacePtr operator()(LPCTSTR lpszPath);

	private:
	// 変数
		std::ostream& m_rConsoleStream; ///< 出力先
		const Folder& m_rFolder; ///< 接続ファイルフォルダ
	};

	static IWorkspacePtr open(LPCTSTR lpszFullPath);
};

//////////////////////////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////////////////////////

// ConnectionFile::Folder
//////////////////////////////////////////////////////////////////////

inline ConnectionFile::Folder::Folder()
{
	TCHAR szPath[MAX_PATH];
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
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, TRUE);
	m_strPath.Format(_T("%s\\ESRI\\Desktop%s\\ArcCatalog"),szPath, versionStr);
}

/**
 * @brief ファイル名に、標準接続ファイルフォルダパスを付け足す
 *
 * @param lpszFileName [in] ファイル名。
 * @return フルパス。
 */
inline CString ConnectionFile::Folder::complementFullPath(LPCTSTR lpszFileName) const
{
	CString strFullPath;
	strFullPath.Format(_T("%s\\%s"), getPath(), lpszFileName);
	return strFullPath;
}

/**
 * @brief フルパスからエイリアスパスを求める
 *
 * lpszPathがエイリアスパスの場合、そのまま戻り値になる。
 *
 * @param lpszPath [in] パス。
 * @return エイリアスパス。
 */
inline CString ConnectionFile::Folder::convertPathFullPathToAlias(LPCTSTR lpszPath) const
{
	int nLength = m_strPath.GetLength();
	if(::_tcsnicmp(lpszPath, m_strPath, nLength) == 0) {
		CString strAliasPath;
		strAliasPath.Format(_T("%s\\%s"), m_sAliasRootString, lpszPath + nLength + 1);
		return strAliasPath;
	}
	else
		return lpszPath;
}

/**
 * @brief エイリアスパスからフルパスを求める
 *
 * lpszAliasPathがフルパスの場合、そのまま戻り値になる。
 *
 * @param lpszAliasPath [in] パス。
 * @return フルパス。
 */
inline CString ConnectionFile::Folder::convertPathAliasToFullPath(LPCTSTR lpszAliasPath) const
{
	size_t nLength = ::_tcslen(m_sAliasRootString);
	if(::_tcsnicmp(lpszAliasPath, m_sAliasRootString, nLength) == 0) {
		CString strFullPath;
		strFullPath.Format(_T("%s\\%s"), m_strPath, lpszAliasPath + nLength + 1);
		return strFullPath;
	}
	else 
		return lpszAliasPath;
}

} // namespace sindy

#endif // __SINDY_CONNECTION_FILE_H__
