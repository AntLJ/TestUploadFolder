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
 * @brief �t�@�C�������w�肵�ă��[�N�X�y�[�X���J��
 *
 * �ȉ��̃t�@�C���ɑΉ��B
 * - SDE�ڑ��t�@�C��
 * - �p�[�\�i���W�I�f�[�^�x�[�X�t�@�C��
 *
 * @param lpszPath [in] �t�@�C���p�X�B
 * @return ���[�N�X�y�[�X�B�J�����Ƃ��ł��Ȃ������ꍇ��NULL�B
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

const TCHAR ConnectionFile::Folder::m_sAliasRootString[] = _T("�f�[�^�x�[�X �R�l�N�V����");

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
 * @param �t�@�C�������n���h�����J��
 *
 * @param lpszPath [in] �t�@�C���̑�����s���f�B���N�g���p�X�B
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
 * @brief �t�@�C�������n���h�������B
 */
void ConnectionFile::Finder::close()
{
	if(m_hFindFile) {
		::FindClose(m_hFindFile);
		m_hFindFile = 0;
	}
}

/**
 * @brief ����SDE�t�@�C���̏����擾����
 *
 * @return SDE�t�@�C���̏��B���삵�I������ꍇNULL�B�C���X�^���X���j������邩�A�ēx���̃��\�b�h���Ă΂ꂽ���_�Ŏ������}����B
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
 * @param rStream [in] �o�͐�X�g���[���B
 * @param rFolder [in] �ڑ��t�@�C���t�H���_�B
 */
ConnectionFile::ConsoleConnector::ConsoleConnector(
	std::ostream& rStream,
	const Folder& rFolder) :
m_rConsoleStream(rStream),
m_rFolder(rFolder)
{
}

/**
 * @brief �ڑ�����
 *
 * @param lpszPath [in] �ڑ��t�@�C���p�X�B�G�C���A�X�p�X�ł��t���p�X�ł��ǂ���ł��B
 * @return �ڑ��������[�N�X�y�[�X�B���s�����ꍇNULL�B
 */
IWorkspacePtr ConnectionFile::ConsoleConnector::operator()(LPCTSTR lpszPath)
{
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	m_rConsoleStream.imbue(loc);
	m_rConsoleStream
		<< _T('\"')
		<< static_cast<LPCTSTR>(m_rFolder.convertPathFullPathToAlias(lpszPath))
		<< _T("\" �ڑ� ... ");

	IWorkspacePtr ipWorkspace = open(m_rFolder.convertPathAliasToFullPath(lpszPath));

	m_rConsoleStream << (ipWorkspace == 0 ? _T("NG") : _T("OK")) << std::endl;

	return ipWorkspace;
}
