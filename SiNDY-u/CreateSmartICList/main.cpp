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

// CreateSmartICList.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "CreateSmartICList.h"
#include <arctl/coinitializer.h>
#include "time.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int iRet=0;
	
	clock_t start = clock();
	bool aIsSuccess = true;	

	if(argv[1] == NULL)
	{
		std::cout << std::endl;
		std::cout << _T("usage: CreateSmartICList.exe <�ݒ�t�@�C��>") << std::endl;
		iRet = 1;
		return iRet;
	}

	try 
	{//�ʏ폈��
		const arctl::coinitializer aCoInitializer;

		CSmartICList cSmartICList(argv[1]);
		cSmartICList.Execute();
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		aIsSuccess = false;
	}
	catch(_com_error& e)
	{
		std::cerr << e.ErrorMessage() << std::endl;
		aIsSuccess = false;
	}

	clock_t end = clock();
	std::cerr << _T("Total:\t") << (end-start) / CLOCKS_PER_SEC << _T("sec.") << std::endl;

	if(aIsSuccess) 
	{
		std::cerr << _T("����I��") << std::endl;
	} 
	else
	{
		std::cerr << _T("�ُ�I��") << std::endl;
		iRet = 2;
	}

	return iRet;
}
