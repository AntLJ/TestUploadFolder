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

// CharAddrCheck.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ConnectServer.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		arctl::coinitializer aoinit;
#ifdef _UNICODE
		std::wcerr.imbue(std::locale("japanese"));
#else
		//std::cerr.imbue(std::locale("japanese"));
#endif
		if( argc < 2 )
		{
			std::cerr << "Usage: CharAddrCheck.exe <arg1config.txt> [-option]\n";
			std::cerr << "<����>\n";
			std::cerr << " <arg1>                  : config.txt�ւ̃t���p�X\n";
			std::cerr << "[�I�v�V����]\n";
			std::cerr << "  [-i (--infoaddr)]      : �P�ꃊ�X�g�o�̓��[�h�i���̃I�v�V�������Ȃ���Γǂݕϊ����[�h�j\n";
			std::cerr << "  [-g (--goupoint.txt)]  : [-i]�I�v�V�������w�肵���ꍇ�ɗL���B�ǂݕϊ����냊�X�g����P�ꃊ�X�g���쐬����B\n";
			std::cerr << "                            goupoint.txt�ւ̃t���p�X\n";
			std::cerr << "  [-c (--cityadmin.txt)] : [-i]�I�v�V�������w�肵���ꍇ�ɗL���B�ǂݕϊ����냊�X�g����P�ꃊ�X�g���쐬����B\n";
			std::cerr << "                            cityadmin.txt�ւ̃t���p�X\n";
			return 0;
		}
		CString strConf = argv[1];

		CConnectServer aConnect;

		if( !aConnect.run( strConf, argc, argv ) )
			std::cerr << "�ُ�I���v���܂����B" << std::endl;
		else
			std::cerr << "����I���v���܂����B" << std::endl;
	}
	// ��O����
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::cout << e.ErrorMessage() << std::endl;
	}

	return 0;
}
