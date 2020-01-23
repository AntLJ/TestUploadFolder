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
 *	@file main.cpp
 *	@brief �G���g���|�C���g
 *	@author	F.Adachi
 *	@date	2005/10/18		�V�K�쐬
 *	$ID$
 */
// main.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <exception>
#include <arctl/coinitializer.h>
#include <atl2/io.h>
#include "CreateLinkPatch.h"
#include "GlobalFunction.h"


int main(int argc, char* argv[])
{
	bool aIsSuccess = true;

	try
	{
		const arctl::coinitializer aCoInitializer;	// Arc9.3.1�Ή�����try�X�R�[�v���Ɉړ�

		CreateLinkPatch aApp;
		aApp.init( argc, argv );

		CString aStartTime;
		GlobalFunction::getTime( aStartTime );
		std::cout << "Start at " << static_cast<const char*>(aStartTime) << std::endl;

		aApp.run();

		CString aEndTime;
		GlobalFunction::getTime( aEndTime );
		std::cout << "End at " << static_cast<const char*>(aEndTime) << std::endl;
	}
	catch (std::exception& e) 
	{
		//e.what()�ɕt�^�����t���O"R1F:"�����݂���΁A�v���O�����쐬�҂̔F�����Ă���G���[�Ȃ̂�return 1 ��Ԃ��B
		//R1F��RETURN1FLAG�̗�
		CString ErrMess = e.what();
		int flagcheck = strncmp(ErrMess,"R1F:",4);

		if(flagcheck == 0)
		{
			//�t�^�����t���O�����̍폜
			ErrMess = ErrMess.Right(ErrMess.GetLength()-ErrMess.Find(_T(":"))-1);
			
			std::cerr << CT2CA(ErrMess) << std::endl;
			std::cerr << "�ُ�I��" << std::endl;
			return 1;
		}
		else
		{
			//�t���O���Ȃ���Αz��O�̃G���[�Ȃ̂ŗ�O�łُ̈�I���Ƃ��Areturn 2 ��Ԃ��B
			std::cerr << e.what() << std::endl;
			std::cerr << "��O������" << std::endl;
			std::cerr << "�ُ�I��(��O)" << std::endl;
			return 2;
		}
	}
	catch (_com_error& e) {
		std::cerr << e << std::endl;
		std::cerr << "��O������" << std::endl;
		std::cerr << "�ُ�I��(��O)" << std::endl;
		return 2;
	}
	std::cerr << "����I��" << std::endl;
	return 0;
}
