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
#include "SiNDYConnectionFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

// ConnectionFile
//////////////////////////////////////////////////////////////////////

/**
 * @brief ファイル名を指定してワークスペースを開く
 *
 * 以下のファイルに対応。
 * - SDE接続ファイル
 * - パーソナルジオデータベースファイル
 *
 * @param lpszPath [in] ファイルパス。
 * @return ワークスペース。開くことができなかった場合はNULL。
 */
IWorkspacePtr ConnectionFile::open(LPCTSTR lpszPath)
{
	if(! lpszPath)
		return 0;

	LPCTSTR p = ::_tcsrchr(lpszPath, _T('.'));
	if(! p)
		return 0;

	IWorkspaceFactoryPtr ipWorkspaceFactory;

	if(::_tcsicmp(p, _T(".sde")) == 0) {
		ipWorkspaceFactory.CreateInstance(CLSID_SdeWorkspaceFactory);
	}
	else if(::_tcsicmp(p, _T(".mdb")) == 0) {
		ipWorkspaceFactory.CreateInstance(CLSID_AccessWorkspaceFactory);
	}

	if(ipWorkspaceFactory == 0)
		return 0;

	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszPath), 0, &ipWorkspace);
	return ipWorkspace;
}

// ConnectionFile::Folder
//////////////////////////////////////////////////////////////////////

const TCHAR ConnectionFile::Folder::m_sAliasRootString[] = _T("データベース コネクション");

// ConnectionFile::Finder
//////////////////////////////////////////////////////////////////////

ConnectionFile::Finder::Finder(const Folder& rFolder) :
m_hFindFile(0)
{
	open(rFolder.getPath());
}

ConnectionFile::Finder::~Finder()
{
	close();
}

/**
 * @param ファイル走査ハンドルを開く
 *
 * @param lpszPath [in] ファイルの操作を行うディレクトリパス。
 */
void ConnectionFile::Finder::open(LPCTSTR lpszPath)
{
	close();

	CString strFind;
	strFind.Format(_T("%s\\*.sde"), lpszPath);

	m_hFindFile = ::FindFirstFile(strFind, &m_FindData);
	m_bFirst = true;
}

/**
 * @brief ファイル走査ハンドルを閉じる。
 */
void ConnectionFile::Finder::close()
{
	if(m_hFindFile) {
		::FindClose(m_hFindFile);
		m_hFindFile = 0;
	}
}

/**
 * @brief 次のSDEファイルの情報を取得する
 *
 * @return SDEファイルの情報。操作し終わった場合NULL。インスタンスが破棄されるか、再度このメソッドが呼ばれた時点で寿命を迎える。
 */
LPWIN32_FIND_DATA ConnectionFile::Finder::next()
{
	if(! m_hFindFile)
		return 0;

	if(m_bFirst) {
		m_bFirst = false;
	}
	else {
		if(! ::FindNextFile(m_hFindFile, &m_FindData)) {
			close();
			return 0;
		}
	}

	return &m_FindData;
}

// ConnectionFile::ConsoleConnector
//////////////////////////////////////////////////////////////////////

/**
 * @param rStream [in] 出力先ストリーム。
 * @param rFolder [in] 接続ファイルフォルダ。
 */
ConnectionFile::ConsoleConnector::ConsoleConnector(
	std::ostream& rStream,
	const Folder& rFolder) :
m_rConsoleStream(rStream),
m_rFolder(rFolder)
{
}

/**
 * @brief 接続する
 *
 * @param lpszPath [in] 接続ファイルパス。エイリアスパスでもフルパスでもどちらでも可。
 * @return 接続したワークスペース。失敗した場合NULL。
 */
IWorkspacePtr ConnectionFile::ConsoleConnector::operator()(LPCTSTR lpszPath)
{
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	m_rConsoleStream.imbue(loc);
	m_rConsoleStream
		<< _T('\"')
		<< static_cast<LPCTSTR>(m_rFolder.convertPathFullPathToAlias(lpszPath))
		<< _T("\" 接続 ... ");

	IWorkspacePtr ipWorkspace = open(m_rFolder.convertPathAliasToFullPath(lpszPath));

	m_rConsoleStream << (ipWorkspace == 0 ? _T("NG") : _T("OK")) << std::endl;

	return ipWorkspace;
}
