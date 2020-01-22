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

// CheckAddrUsingGS.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <arctl/coinitializer.h>
#include "CheckAddrUsingGSObj.h"

namespace return_code {
	enum ECode {
		unknown			= -1,	//!< �������
		ok				= 0,	//!< ����I��
		tool_error		= 1,	//!< �c�[�����o���G���[
		exception_error	= 2,	//!< ��O�G���[
		licence_error	= 3,	//!< ���C�Z���X�F�؎��s
	};
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retcode = return_code::unknown;
	try{
		arctl::coinitializer coinitializer;
//		::setlocale(LC_ALL, "Japanese"); // ���ꂾ�� boost::program_options �œ��{�������̈��������������߂���Ȃ������c
		std::locale::global( std::locale( "Japanese", std::locale::ctype ) );

		try{
			CCheckAddrUsingGSObj cCheckAddrUsingGSObj;

			if( cCheckAddrUsingGSObj.Init(argc, argv) )
			{
				if( cCheckAddrUsingGSObj.Execute() )
				{
					std::cerr << "����I��" << std::endl;
					retcode = return_code::ok;
				}
				else
				{
					std::cerr << "�ُ�I��" << std::endl;
					retcode = return_code::tool_error;
				}
			}
		}
		catch (const _com_error& com_err)
		{
			// �c�[����O�G���[
			std::cerr << CT2A( com_err.ErrorMessage() ) << std::endl;
			retcode = return_code::exception_error;
		}
	}
	catch(const std::exception &e)
	{
		// ���C�Z���X�F�؎��s
		std::cerr << e.what() << std::endl;
		retcode = return_code::licence_error;
	}
	return retcode;
}
