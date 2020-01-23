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

#include "StdAfx.h"
#include "Arguments.h"
#include <iostream>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input  = "input";
	const char * const k_file   = "file";
	const char * const k_fcname = "fcname";
	const char * const k_log    = "log";

	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_input,  tvalue<tstring>(&input),  "[�K�{]�폜�Ώ�DB")
			(k_file,   tvalue<tstring>(&file),   "[�K�{]�폜ObjectID���X�g")
			(k_fcname, tvalue<tstring>(&fcname), "[�K�{]�폜�Ώۃt�B�[�`���N���X��")
			(k_log,    tvalue<tstring>(&log),    "[�K�{]���O")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if(input.empty()){
			cerr << k_input << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}

		if(file.empty()){
			cerr << k_file << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}

		if(fcname.empty()){
			cerr << k_fcname << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}

		if(! good)
		{
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}
