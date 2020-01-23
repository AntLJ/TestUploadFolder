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
#include "App.h"

#include <algorithm>
#include <iterator>

CApp::CApp(void)
{
}

CApp::~CApp(void)
{
}

// �g�p�@
bool CApp::printUsage()
{
	std::wcerr
		<< L"�ySiNDY-c�pPGDB �œK���c�[��" << TOOL_VERSION << L"�z\n"
		<< L"(Usage)CompactPGDB.exe [�I�v�V����] (> �i�����O)\n"
		<< L"�E�I�v�V�����ɂ���\n"
		<< L"  -f PGDB�t�@�C����\n"
		<< L"  -d PGDB�x�[�X�f�B���N�g��\n"
		<< L"  -l ���b�V�����X�g(-b �I�v�V�����w�莞�K�{)\n" << std::endl;

	return false;
}

// ������
bool CApp::init( int argc, TCHAR* argv[] )
{
	if( !checkArg(argc, argv) )
		return false;

	// �œK���t�@�C�����X�g���쐬����
	if( !m_strBaseDir.IsEmpty() ){
		
		for( std::vector<long>::const_iterator itr=m_vecMesh.begin();itr!=m_vecMesh.end();++itr )
		{
			TCHAR szPGDBFile[_MAX_PATH] = {0};
			_stprintf( szPGDBFile, _T("%s\\%d\\%d.mdb"), m_strBaseDir, (*itr)/10000, *itr );
			m_vecFile.push_back( szPGDBFile );
		}
	}

	return true;
}

// ���s
bool CApp::execute()
{
	for( std::vector<CString>::const_iterator itr=m_vecFile.begin();itr!=m_vecFile.end();++itr )
	{
		compact(*itr);
	}

	return true;
}

// �R���p�N�g��
bool CApp::compact( LPCTSTR lpszFile )
{
	std::wcout << lpszFile << L" : �œK�����E�E�E�E";

	// PGDB�֐ڑ�
	IWorkspacePtr ipWorkspace;
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	if( FAILED(ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszFile), NULL, &ipWorkspace)) ){
		std::wcout << L"PGDB�ւ̐ڑ��Ɏ��s" << std::endl;
		return false;
	}

	IDatabaseCompactPtr ipDataBaseCompact( ipWorkspace );
	if( NULL == ipDataBaseCompact ){
		std::wcout << L"���s(ipDataBaseCompact==NULL)" << std::endl;
		return false;
	}

	VARIANT_BOOL vb;
	ipDataBaseCompact->CanCompact( &vb );
	if( VARIANT_TRUE == vb ){
		ipDataBaseCompact->Compact();
		std::wcout << L"����" << std::endl;
	}
	else
		std::wcout << L"�œK���ł��܂���" << std::endl;

	return true;
}

//////////////////////////////////////
//	����J�֐�
//////////////////////////////////////

// �����`�F�b�N
bool CApp::checkArg(int argc, TCHAR *argv[])
{
	if( argc < 3 )
		return false;

	for( int i=1;i<argc-1;++i )
	{
		if( _tcscmp(argv[i], _T("-f")) == 0 ){
			m_vecFile.push_back( argv[++i] );
		}
		else if( _tcscmp(argv[i], _T("-d")) == 0 ){
			m_strBaseDir = argv[++i];
		}
		else if( _tcscmp(argv[i], _T("-l")) == 0 ){
			
			if( !loadMeshList(argv[++i]) ){
				std::wcerr << L"#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s : " << argv[i] << std::endl;
				return false;
			}

		}
	}

	return true;
}

// ���b�V�����X�g�ǂݍ���
bool CApp::loadMeshList( LPCTSTR lpszFile )
{
	std::ifstream ifs( lpszFile );
	if( !ifs )
		return false;
	copy(
		std::istream_iterator<long>(ifs),
		std::istream_iterator<long>(),
		std::back_inserter(m_vecMesh)
		);

	return true;
}
