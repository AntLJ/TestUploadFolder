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
 * @file pgdb.h
 * @brief PGDB操作系テンプレートライブラリ。
 */
#ifndef ARCTL_PGDB_FUNCTIONS_H_
#define ARCTL_PGDB_FUNCTIONS_H_

#include <functional>
#include <atl2/exception.h>
#include <CodeAnalysis/SourceAnnotations.h>

namespace arctl {

/**
 * @brief PGDB生成関数クラス。
 * @test @ref unit_test PGDBCreatorTest
 */
class pgdb_creator : public std::unary_function<LPCTSTR, IWorkspacePtr>
{
public:
//@{ @name 構築/消滅
	pgdb_creator() : m_ipWorkspaceFactory(__uuidof(AccessWorkspaceFactory)) {}
	pgdb_creator(IWorkspaceFactory* ipWorkspaceFactory) : m_ipWorkspaceFactory(ipWorkspaceFactory) {}
//@}

//@{ @name 処理
	/**
	 * @brief フルパスからPGDBを生成する。
	 *
	 * @param lpszPath [in] 生成するPGDBのフルパス。拡張子「.mdb」の有無は問わない。
	 * @return 生成したPGDB。
	 */
	IWorkspacePtr operator()([SA_Pre(Null=SA_No), SA_Pre(NullTerminated=SA_Yes)] LPCTSTR lpszPath) const
	{
		if(::_tcslen(lpszPath) >= _MAX_PATH)
			throw std::length_error("path too long.");

#pragma warning( push )
#pragma warning( disable : 4996 ) // 文字列長は直上で検査済み。
		// パスを可変領域に複写。
		TCHAR szPath[_MAX_PATH] = { 0 };
		::_tcscpy(szPath, lpszPath);
#pragma warning( pop )

		// パスから拡張子を除外。
		TCHAR* p = _tcsrchr(szPath, _T('.'));
		if(p && ::_tcsicmp(p + 1, _T("mdb")) == 0) {
			*p = 0;
		}
	
		// PGDB生成。
		IWorkspaceNamePtr ipWorkspaceName;
		atl2::valid(m_ipWorkspaceFactory->Create(0, CComBSTR(szPath), 0, 0, &ipWorkspaceName));

		IUnknownPtr ipUnknown;
		atl2::valid(INamePtr(ipWorkspaceName)->Open(&ipUnknown));

		return ipUnknown;
	}
//@}

private:
// 変数
	IWorkspaceFactoryPtr m_ipWorkspaceFactory;
};

/**
 * @brief PGDB接続関数クラス。
 */
class pgdb_connector : public std::unary_function<LPCTSTR, IWorkspacePtr>
{
public:
//@{ @name 構築/消滅
	pgdb_connector() : m_ipWorkspaceFactory(__uuidof(AccessWorkspaceFactory)) {}
	pgdb_connector(IWorkspaceFactory* ipWorkspaceFactory) : m_ipWorkspaceFactory(ipWorkspaceFactory) {}
//@}

//@{ @name 処理
	/**
	 * @brief PGDBを開く。
	 *
	 * @param lpszPath [in] パス。
	 * @return 開いたPGDB。
	 */
	IWorkspacePtr operator()(LPCTSTR lpszPath) const
	{
		IWorkspacePtr ipWorkspace;
		atl2::valid(m_ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszPath), 0, &ipWorkspace));
		return ipWorkspace;
	}
//@}

private:
// 変数
	IWorkspaceFactoryPtr m_ipWorkspaceFactory;
};

inline IWorkspacePtr create_pgdb(LPCTSTR lpszPath) { return arctl::pgdb_creator()(lpszPath); }
inline IWorkspacePtr connect_pgdb(LPCTSTR lpszPath) { return arctl::pgdb_connector()(lpszPath); }

} // namespace arctl

#endif // ARCTL_PGDB_FUNCTIONS_H_
