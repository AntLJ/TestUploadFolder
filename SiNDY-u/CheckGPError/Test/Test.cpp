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

// Test.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <arctl/coinitializer.h>
#include <AddrLib/Initializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"Japanese" );

	// �e�X�g���ʎ��W���X�i���R���g���[���ɃA�^�b�`����
	CPPUNIT_NS::TestResultCollector result;

	try
	{
		arctl::coinitializer aoinit;

		// �����t���Z���e�[�u���E���~�ϊ��e�[�u���ǂݍ���
		if( addr::Init_CodeConverter(_T("CodeConvTable.mdb")) )
		{
			// �C�x���g�}�l�[�W���ƃe�X�g�E�R���g���[���𐶐�
			CPPUNIT_NS::TestResult controller;
			controller.addListener( &result );

			// �u.�v�Ői�s�󋵂��o�͂��郊�X�i���A�^�b�`����
			CPPUNIT_NS::BriefTestProgressListener progress;
			controller.addListener( &progress );

			// �e�X�g�E�����i�[�Ƀe�X�g�S��^���A�e�X�g����
			CPPUNIT_NS::TestRunner runner;
			runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
			runner.run( controller );

			// �e�X�g���ʂ�W���o�͂ɓf���o��
			CPPUNIT_NS::CompilerOutputter outputer( &result, CPPUNIT_NS::stdCOut() );
			outputer.write();
		}

	}
	catch( std::exception& e )
	{
		std::cout << e.what() << std::endl;
	}

	return result.wasSuccessful() ? 0 : 1;
}

