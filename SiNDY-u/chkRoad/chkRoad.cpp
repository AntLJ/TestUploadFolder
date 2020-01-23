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

// chkRoad.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "RoadApplication.h"

bool g_bLatest = false;

void getCommandLine( const int nNum, LPTSTR lpcszToken[] )
{
	// �������w��
	for( int i = 3; i < nNum; ++i )
	{
		CString strOp(lpcszToken[i]);
		if( _tcscmp( _T("-l"), strOp ) == 0 || _tcscmp( _T("--latest"), strOp ) == 0 )
			g_bLatest = true;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	long lReturn = 0;
	try
	{
		if( argc < 3 || 4 < argc )
		{
			cerr << "Usage    �@�@�FchkRoad.exe [Config.txt] [Customer] [-l(--latest)]\n";
			cerr << "[Config.txt] �F�R���t�B�O�t�@�C���ւ̃p�X\n";
			cerr << "[Customer] �F�d������iPEC,TF,KIWI-C,BE,OTHERS�j\n";
			cerr << "[-l(latest)] �FB�ňȍ~�̃����[�X���ǂ���\n";
			return 1;
		}
		CString aConf = argv[1];
		CString strCustomer = argv[2];
		if( argc > 3 )
			getCommandLine( argc, argv );
		CRoadApplication aRoadApp;

		lReturn = aRoadApp.run( aConf, strCustomer, g_bLatest );
		switch( lReturn )
		{
			case 0:
				cerr << "����I���v���܂����B" << endl;
				break;
			case 1:
			case 2:
			default:
				cerr << "�ُ�I���v���܂����B" << endl;
				break;
			case 3:
				cerr << "�`�F�b�N�ŃG���[������܂��B" << endl;
				break;
		}
	}
	catch( ... )
	{
		cerr << "��O���������܂����B" << endl;
		cerr << "�ُ�I���v���܂����B" << endl;
		return 2;
	}

	return lReturn;
}



