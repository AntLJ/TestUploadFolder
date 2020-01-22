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

// AddrTransResultFmtConverter.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "AddrTransResultFmtConverter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcerr.imbue(std::locale(""));
	std::wcout.imbue(std::locale(""));
	try
	{
		AddrTransResultFmtConverter addrTransResultFmtConverter;
		// ������ && �`�F�b�N�J�n
		if(addrTransResultFmtConverter.init(argc, argv) && addrTransResultFmtConverter.run())
		{
			// ����I��
			wcout << L"����I��" << endl;
			return 0;
		}
	}
	catch(const std::exception& e)
	{
		wcerr << e.what() << endl;
		wcerr << L"�ُ�I��" << endl;
		return 1;
	}
	return 0;
}