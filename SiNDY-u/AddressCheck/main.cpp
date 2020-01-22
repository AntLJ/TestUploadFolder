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

// AddressCheck.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/flush_scope_guard.h>
#include "CheckAddress.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// �����t���b�V��
	uh::flush_scope_guard fsg;

	const arctl::coinitializer aCoInitializer;

	try
	{
		CheckAddress checkAddress;
		// ������ && �`�F�b�N�J�n
		if( checkAddress.init(argc, argv) &&  checkAddress.run())
		{
			// ����I��
			cerr << "����I��" << endl;
			return 0;
		}	
	}

	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "�ُ�I��" << endl;
		return 1;
	}
}

