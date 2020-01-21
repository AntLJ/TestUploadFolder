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
 * @brief PGDB����n�e���v���[�g���C�u�����B
 */
#ifndef ARCTL_PGDB_FUNCTIONS_H_
#define ARCTL_PGDB_FUNCTIONS_H_

#include <functional>
#include <atl2/exception.h>
#include <CodeAnalysis/SourceAnnotations.h>

namespace arctl {

/**
 * @brief PGDB�����֐��N���X�B
 * @test @ref unit_test PGDBCreatorTest
 */
class pgdb_creator : public std::unary_function<LPCTSTR, IWorkspacePtr>
{
public:
//@{ @name �\�z/����
	pgdb_creator() : m_ipWorkspaceFactory(__uuidof(AccessWorkspaceFactory)) {}
	pgdb_creator(IWorkspaceFactory* ipWorkspaceFactory) : m_ipWorkspaceFactory(ipWorkspaceFactory) {}
//@}

//@{ @name ����
	/**
	 * @brief �t���p�X����PGDB�𐶐�����B
	 *
	 * @param lpszPath [in] ��������PGDB�̃t���p�X�B�g���q�u.mdb�v�̗L���͖��Ȃ��B
	 * @return ��������PGDB�B
	 */
	IWorkspacePtr operator()([SA_Pre(Null=SA_No), SA_Pre(NullTerminated=SA_Yes)] LPCTSTR lpszPath) const
	{
		if(::_tcslen(lpszPath) >= _MAX_PATH)
			throw std::length_error("path too long.");

#pragma warning( push )
#pragma warning( disable : 4996 ) // �����񒷂͒���Ō����ς݁B
		// �p�X���ϗ̈�ɕ��ʁB
		TCHAR szPath[_MAX_PATH] = { 0 };
		::_tcscpy(szPath, lpszPath);
#pragma warning( pop )

		// �p�X����g���q�����O�B
		TCHAR* p = _tcsrchr(szPath, _T('.'));
		if(p && ::_tcsicmp(p + 1, _T("mdb")) == 0) {
			*p = 0;
		}
	
		// PGDB�����B
		IWorkspaceNamePtr ipWorkspaceName;
		atl2::valid(m_ipWorkspaceFactory->Create(0, CComBSTR(szPath), 0, 0, &ipWorkspaceName));

		IUnknownPtr ipUnknown;
		atl2::valid(INamePtr(ipWorkspaceName)->Open(&ipUnknown));

		return ipUnknown;
	}
//@}

private:
// �ϐ�
	IWorkspaceFactoryPtr m_ipWorkspaceFactory;
};

/**
 * @brief PGDB�ڑ��֐��N���X�B
 */
class pgdb_connector : public std::unary_function<LPCTSTR, IWorkspacePtr>
{
public:
//@{ @name �\�z/����
	pgdb_connector() : m_ipWorkspaceFactory(__uuidof(AccessWorkspaceFactory)) {}
	pgdb_connector(IWorkspaceFactory* ipWorkspaceFactory) : m_ipWorkspaceFactory(ipWorkspaceFactory) {}
//@}

//@{ @name ����
	/**
	 * @brief PGDB���J���B
	 *
	 * @param lpszPath [in] �p�X�B
	 * @return �J����PGDB�B
	 */
	IWorkspacePtr operator()(LPCTSTR lpszPath) const
	{
		IWorkspacePtr ipWorkspace;
		atl2::valid(m_ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszPath), 0, &ipWorkspace));
		return ipWorkspace;
	}
//@}

private:
// �ϐ�
	IWorkspaceFactoryPtr m_ipWorkspaceFactory;
};

inline IWorkspacePtr create_pgdb(LPCTSTR lpszPath) { return arctl::pgdb_creator()(lpszPath); }
inline IWorkspacePtr connect_pgdb(LPCTSTR lpszPath) { return arctl::pgdb_connector()(lpszPath); }

} // namespace arctl

#endif // ARCTL_PGDB_FUNCTIONS_H_
