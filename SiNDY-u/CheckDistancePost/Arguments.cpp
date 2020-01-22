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
 * @class CArguments
 * @brief �R�}���h���C�������N���X
 * @file  Arguments.cpp
 * @author �r�b�g�G�C�W�@�`
 * $Id$
*/
#include "StdAfx.h"
#include <fstream>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include "Arguments.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy
{
	using namespace std;
	using namespace uh;

	namespace po = boost::program_options;

	const char * k_db =         "db";                ///< ����DB
	const char * k_roadlist =   "road_list";         ///< �ΏۘH���ꗗ�t�@�C���p�X
	const char * k_pitch =      "pitch";             ///< �s�b�`
	const char * k_outdir =     "out_dir";           ///< ��ƃ��O�o�̓p�X
	const char * k_roadclass =  "roadclass_cost";    ///< �Ώۓ��H��ʃR�X�g��`�t�@�C���p�X
	const char * k_linkclass =  "linkclass_cost";    ///< �Ώۃ����N��ʃR�X�g��`�t�@�C���p�X
	const char * k_buffer =     "getlink_buffer";    ///< �����N����o�b�t�@
	const char * k_range =      "permissible_range"; ///< ���e�͈�
	const char * k_separate =   "separate_range";    ///< 2�𕪗����͈�

	// �R�}���h���C�������̉��
	bool CArguments::parse(int argc, _TCHAR* argv[])
	{
		po::options_description desc("How to use");

		try
		{
			desc.add_options()
				(k_db,		   tvalue<tstring>(&m_strDb),        "[�K�{]���̓p�����[�^ ����DB")
				(k_roadlist,   tvalue<tstring>(&m_strList),      "[�K�{]���̓p�����[�^ �ΏۘH���ꗗ�t�@�C���p�X")
				(k_pitch,      tvalue<tstring>(&m_strPitch),     "[�K�{]���̓p�����[�^ �s�b�`[10.0/1.0/0.1]km")
				(k_outdir,     tvalue<tstring>(&m_strLogPath),   "[�K�{]���̓p�����[�^ ��ƃ��O�o�̓p�X")
				(k_roadclass,  tvalue<tstring>(&m_strRoadCost),  "[�K�{]���̓p�����[�^ �Ώۓ��H��ʃR�X�g��`�t�@�C���p�X")
				(k_linkclass,  tvalue<tstring>(&m_strLinkCost),  "[�K�{]���̓p�����[�^ �Ώۃ����N��ʃR�X�g��`�t�@�C���p�X")
				(k_buffer,     tvalue<tstring>(&m_strBuf),       "[�K�{]���̓p�����[�^ �����N����o�b�t�@[m]")
				(k_range,      tvalue<tstring>(&m_strRange),     "[�K�{]���̓p�����[�^ ���e�͈�[%]")
				(k_separate,   tvalue<tstring>(&m_strSeparate),  "[�K�{]���̓p�����[�^ 2�𕪗����͈�[%]")
				;

			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			// �K�{�p�����[�^�̃`�F�b�N
			bool good = true;

			// ����DB
			if(m_strDb.empty())
			{
				cerr << k_db << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}

			// �ΏۘH���ꗗ�t�@�C���p�X
			if(m_strList.empty())
			{
				cerr << k_roadlist << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}
			else
			{
				if(! isFile(m_strList.c_str()))
				{
					cerr << toStr(m_strList) << "�̓t�@�C���ł͂���܂���" << endl;
					good = false;
				}
			}

			// �s�b�`
			if(m_strPitch.empty())
			{
				cerr << k_pitch << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}

			// ��ƃ��O�o�̓p�X
			if(m_strLogPath.empty())
			{
				cerr << k_outdir << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}

			// �Ώۓ��H��ʃR�X�g��`�t�@�C���p�X
			if(m_strRoadCost.empty())
			{
				cerr << k_roadclass << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}
			else
			{
				if(!isFile(m_strRoadCost.c_str()))
				{
					cerr << toStr(m_strRoadCost) << "�̓t�@�C���ł͂���܂���" << endl;
					good = false;
				}
			}

			// �Ώۃ����N��ʃR�X�g��`�t�@�C���p�X
			if(m_strLinkCost.empty())
			{
				cerr << k_linkclass << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}
			else
			{
				if(!isFile(m_strLinkCost.c_str()))
				{
					cerr << toStr(m_strLinkCost) << "�̓t�@�C���ł͂���܂���" << endl;
					good = false;
				}
			}

			// �����N����o�b�t�@
			if(m_strBuf.empty())
			{
				cerr << k_buffer << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}

			// ���e�͈�
			if(m_strRange.empty())
			{
				cerr << k_range << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}

			// 2�𕪗����e����
			if(m_strSeparate.empty())
			{
				cerr << k_separate << "�I�v�V�����͕K�{�ł�" << endl;
				good = false;
			}

			GetOptionParam();

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

	// �I�v�V���������ꗗ�쐬
	void CArguments::GetOptionParam()
	{
		CString strRet, strTmp;

		strRet  = _T("#  option\n");

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_db).c_str(), m_strDb.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_roadlist).c_str(), m_strList.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_pitch).c_str(), m_strPitch.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_outdir).c_str(), m_strLogPath.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_roadclass).c_str(), m_strRoadCost.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_linkclass).c_str(), m_strLinkCost.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_buffer).c_str(), m_strBuf.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_range).c_str(), m_strRange.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_separate).c_str(), m_strSeparate.c_str());
		strRet += strTmp;

		m_strOpParam = strRet;
	}
}
