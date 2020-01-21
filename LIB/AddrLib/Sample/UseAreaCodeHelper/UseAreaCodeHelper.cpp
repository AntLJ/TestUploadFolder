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

// UseAreaCodeHelper.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <AddrLib/AreaCodeHelper.h>

/**
 * @file	UseAreaCodeHelper.cpp
 * @brief	AddrLib��AreaCodeHelper�N���X��p�����Z���}�X�^�ւ̃A�N�Z�X��������T���v���R�[�h
 */

using namespace std;
using namespace addr;

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, _T("Japanese") );

	CAreaCodeHelper cAreaCodeHelper;

	// DB�֐ڑ�
	if( cAreaCodeHelper.Connect(_T("y0901e/y0901e@delltcp")) )
	{
		// �Z�����̎擾
		wcout << L"[�Z�����̎擾]" << endl;
		wcout << L"01          : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("01"))) << endl;
		wcout << L"01          : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("01"))) << endl;
		wcout << L"03201       : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("03201"))) << endl;
		wcout << L"03201       : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("03201"))) << endl;
		wcout << L"14203013    : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("14203013"))) << endl;
		wcout << L"14203013    : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("14203013"))) << endl;
		wcout << L"13104092004 : " << LPCWSTR(cAreaCodeHelper.GetAddrKanji(_T("13104092004"))) << endl;
		wcout << L"13104092004 : " << LPCWSTR(cAreaCodeHelper.GetAddrKana(_T("13104092004"))) << endl;
		wcout << endl;

		// ���Z���R�[�h����V�Z���R�[�h(������Z���R�[�h)�擾
		wcout << L"[������Z���R�[�h�擾]" << endl;
		wcout << L"03307001101(��茧���S�ʎR���厚��c����c)��" 
			  << LPCWSTR(cAreaCodeHelper.GetNewestCode(_T("03307001101"))) << L"(" << LPCWSTR(cAreaCodeHelper.GetAddrKanji(cAreaCodeHelper.GetNewestCode(_T("03307001101")))) << L")" << endl;
		wcout << endl;

		// �V���Z���R�[�h���狌�Z���R�[�h(�����O�Z���R�[�h)�擾
		wcout << L"[�����O�Z���R�[�h�擾]" << endl;
		wcout << L"03201148101(��茧�����s�ʎR���c����c)��" 
			  << LPCWSTR(cAreaCodeHelper.GetPrevCode(_T("03201148101"))) << L"(" << LPCWSTR(cAreaCodeHelper.GetAddrKanji(cAreaCodeHelper.GetPrevCode(_T("03201148101")))) << L")" << endl;
		wcout << endl;
	}

	return 0;
}

