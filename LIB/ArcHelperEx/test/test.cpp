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

// test.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <locale>

int _tmain(int argc, TCHAR* argv[])
{
	int nRet = 0;

//	std::locale::global(std::locale(""));
//	_tsetlocale( LC_ALL, _T("") );

	CoInitialize(NULL);
	{
		esriLicenseStatus emStatus;
		IAoInitializePtr ipAoInitialize( CLSID_AoInitialize );
		ipAoInitialize->Initialize( esriLicenseProductCodeArcEditor, &emStatus );

		// �C�x���g�E�}�l�[�W���ƃe�X�g�E�R���g���[���𐶐�����
		CppUnit::TestResult controller;

		// �e�X�g���ʎ��W���X�i���R���g���[���ɃA�^�b�`����
		CppUnit::TestResultCollector result;
		controller.addListener( &result );

		// �u.�v�Ői�s�󋵂��o�͂��郊�X�i���A�^�b�`����
		CppUnit::BriefTestProgressListener progress;
		controller.addListener( &progress );

		// �e�X�g�E�����i�[�Ƀe�X�g�Q��^���A�e�X�g����
		CppUnit::TextUi::TestRunner runner;
//		runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
		runner.addTest(CppUnit::TestFactoryRegistry::getRegistry("CTestGeometryCheck").makeTest());
		runner.run( controller );

		// �e�X�g���ʂ�W���o�͂ɓf���o��
		CppUnit::CompilerOutputter outputter2( &result, std::cout );
		outputter2.write();

		// �f�o�b�O�ɏo�͂���
		std::ostringstream stream;
		stream << "************ �e�X�g�J�n ************\n";
		CppUnit::CompilerOutputter outputter( &result, stream );
		outputter.write();
		stream << "************ �e�X�g�I�� ************\n";
		// ����ꂽ�������TRACE�}�N���ŏo�͂���
		ATLTRACE(stream.str().c_str());

		nRet = runner.result().wasSuccessful() ? 0 : 1;
	}
	CoUninitialize();
	getchar();

	return nRet;
}
