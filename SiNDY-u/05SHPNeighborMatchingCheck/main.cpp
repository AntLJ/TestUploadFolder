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

// 05SHPNeighborMatchingCheck.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Definition.h"
#include "time.h"
#include "ReadConfig.h"
#include "NeighborMatchingCheck.h"
#include <arctl/coinitializer.h>


int _tmain(int argc, _TCHAR* argv[])
{
	int iRET=0;

	clock_t start = clock();

	try
	{
		if(argc != 2){
			std::cerr << "usage: 05SHPNeighborMatchingCheck.exe <�ݒ�t�@�C��>" << std::endl;
			return 0;
		}
		//COM�̏�����
		arctl::coinitializer init;


		//���{��t�@�C�����ɑΉ������
		std::locale loc = std::locale();
		loc = std::locale(loc, "japanese", std::locale::ctype);
		std::locale::global(loc);

		//�ݒ�t�@�C���̓ǂݍ���
		Parameters aParams = { "","","","","","","",0,false };
		CReadConfig aReadConfig;

		if(! aReadConfig.getenv(argv[1], &aParams))
		{
			return 1;
		}
		//�������s
		CNeighborMatchingCheck aNMC;
		iRET=aNMC.App( aParams );
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		iRET=2;
	}

	clock_t end = clock();
	std::cerr << (double)(end-start)/CLOCKS_PER_SEC << "sec������܂���" << std::endl;	

	return iRET;
}

