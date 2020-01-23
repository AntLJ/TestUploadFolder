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

#include "stdafx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_point_con =   "point-con";
	const char * const k_poly_con =    "poly-con";
	const char * const k_point =       "point";
	const char * const k_poly =        "polygon";
	const char * const k_pointW =      "point-where";
	const char * const k_polyW =       "poly-where";
	const char * const k_runlog =      "runlog,r";
	const char * const k_errlog =      "errlog,e";
	

	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_point_con,   tvalue<tstring>(&m_PointConnect), "[�K�{]�|�C���g�̐ڑ���")
			(k_poly_con,    tvalue<tstring>(&m_PolyConnect),  "[�K�{]�|���S���̐ڑ���")
			(k_point,       tvalue<tstring>(&m_Point),        "[�K�{]�`�F�b�N�Ώۂ̃|�C���g�̃t�B�[�`����")
			(k_poly,        tvalue<tstring>(&m_Poly),         "[�K�{]�`�F�b�N�Ώۂ̃|���S���̃t�B�[�`����")
			(k_pointW,      tvalue<tstring>(&m_PointWhere),   "[�C��]�Ώۃ|�C���g���i��WHERE��")
			(k_polyW,       tvalue<tstring>(&m_PolyWhere),    "[�C��]�Ώۃ|���S�����i��WHERE��")
			(k_runlog,      tvalue<tstring>(&m_RunLog),       "[�K�{]���s���O")
			(k_errlog,      tvalue<tstring>(&m_ErrLog),       "[�K�{]�G���[���O")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if(m_PointConnect.empty() || m_PolyConnect.empty())
		{
			cerr << "�ڑ�����w�肵�Ă�������" << endl;
			good = false;
		}

		if(m_RunLog.empty() || m_ErrLog.empty())
		{
			cerr << "���O�t�@�C�����w�肵�Ă�������" << endl;
			good = false;
		}

		if(m_Point.empty())
		{
			cerr << k_point << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}

		if(m_Poly.empty())
		{
			cerr << k_poly << "�I�v�V�����͕K�{�ł�" << endl;
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
