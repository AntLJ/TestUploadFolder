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

// UseAddrCodeList.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

/**
 * @file	UseAddrCodeList.cpp
 * @brief	AddrLib��AddrCodeList�N���X��p�����Z���R�[�h���X�g���n���h������T���v���R�[�h
 */

#include "stdafx.h"
#include <SiNDYLib/Workspace.h>
#include <AddrLib/AddrCodeList.h>

using namespace std;
using namespace sindy;
using namespace addr;

// �R�[�h���X�g�o�͗p�֐��I�u�W�F�N�g
struct CodePrint
{
	void operator()( const _TCHAR* lpcszCode ){ wcout << lpcszCode << endl; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"japanese" );

	::CoInitialize(0);
	{
		// [SiNDYLib]
		// ���[�N�X�y�[�X�擾
		CWorkspace cWorkspace( _T("..\\..\\Test\\53392442.mdb") );

		// [SiNDYLib/AddrLib]
		// �s�s�n�}�s���E�t�B�[�`���N���X��CAddrCodeList��������
		// ��(3)(4)�ł����g�p���Ȃ��ꍇ(���s���E�g�p���Ȃ�)�́A�f�t�H���g�R���X�g���N�^�ŌĂяo�������ł悢)
		CAddrCodeList cList( cWorkspace.OpenTable(schema::city_admin::kTableName) );

		// [AddrLib]
		// (1) �s�s�s���E����S�����̎s�撬���R�[�h���X�g���쐬
		if( cList.MakeCodeListAll(5) )
		{
			wcout << L"[�s�s�s���E���̑S�|���S������s�撬���R�[�h���X�g����]" << endl;
			for_each( cList.begin(), cList.end(), CodePrint() );
		}
		wcout << endl;

		// (2) �s�s�s���E����w�肵���͈͓���11���Z���R�[�h���쐬
		cList.clear();
		if( cList.MakeCodeListFromRange( _T("01000000000"), _T("02000000000")) )
		{
			wcout << L"[�s�s�s���E���̑S�|���S������w��͈͓��̏Z���R�[�h���X�g����]" << endl;
			for_each( cList.begin(), cList.end(), CodePrint() );
		}
		wcout << endl;

		// (3) �O���t�@�C�������[�h
		cList.clear();
		if( cList.AddFromFile(_T("testlist.txt"), 8) )
		{
			wcout << L"[�O���t�@�C������Z���R�[�h���X�g����]" << endl;
			for_each( cList.begin(), cList.end(), CodePrint() );
		}
		wcout << endl;

		// (4) �P���ɃR�[�h�ǉ�
		cList.clear();
		const _TCHAR* szCodeList[] = { _T("01"), _T("02") };
		for( int i = 0; i < 2; ++i )
			cList.Add( szCodeList[i] );
		wcout << L"[�P���ɃR�[�h���X�g����]" << endl;
		for_each( cList.begin(), cList.end(), CodePrint() );
		wcout << endl;

	}
	::CoUninitialize();

	return 0;
}
