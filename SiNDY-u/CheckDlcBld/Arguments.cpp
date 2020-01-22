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
	po::options_description desc("�g����");

	try
	{
		desc.add_options()
			(g_strShapeDir,         uh::tvalue<uh::tstring>(&strShapeDir),        "[�K�{]�V�F�[�v�t�@�C���f�B���N�g��")
			(g_strMeshlist,         uh::tvalue<uh::tstring>(&strMeshlist),        "[�K�{]���b�V�����X�g�̃p�X")
			(g_strSpatialDB,        uh::tvalue<uh::tstring>(&strSpatialDB),       "[�K�{] ��ԎQ�Ǝ擾�pDB�p�X")
			(g_strSpatialDBSchema,  uh::tvalue<uh::tstring>(&strSpatialDBSchema), "[�I��] ��ԎQ�Ǝ擾�pDB�̃X�L�[�}�iPostgreSQL�p�j")
			(g_strRunLog,           uh::tvalue<uh::tstring>(&strRunLog),          "[�K�{] ���s���O�p�X")
			(g_strErrLog,           uh::tvalue<uh::tstring>(&strErrLog),          "[�K�{] �G���[���O�p�X")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if(strShapeDir.empty()){
			cerr << g_strShapeDir << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		} else {
			if(!isDirectory(strShapeDir.c_str())){
				cerr << toStr(strShapeDir) << "�̓f�B���N�g���ł͂���܂���" << endl;
				good = false;
			}
		}
		if(strMeshlist.empty()){
			cerr << g_strMeshlist << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		} else {
			if(!isFile(strMeshlist.c_str())){
				cerr << toStr(strMeshlist) << "�̓t�@�C���ł͂���܂���" << endl;
				good = false;
			}
		}
		if(strSpatialDB.empty()){
			cerr << g_strSpatialDB << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}

		if(!good){
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

CString Arguments::GetOption()
{
	CString strOption = _T("[option]\n");
	if(!strShapeDir.empty() )
		strOption += GetOptionLine(g_strShapeDir, strShapeDir.c_str());

	if(!strMeshlist.empty() )
		strOption += GetOptionLine(g_strMeshlist, strMeshlist.c_str());

	if(!strSpatialDB.empty() )
		strOption += GetOptionLine(g_strSpatialDB, strSpatialDB.c_str());

	if(!strSpatialDBSchema.empty() )
		strOption += GetOptionLine(g_strSpatialDBSchema, strSpatialDBSchema.c_str());

	if(!strRunLog.empty() )
		strOption += GetOptionLine(g_strRunLog, strRunLog.c_str());

	if(!strErrLog.empty() )
		strOption += GetOptionLine(g_strErrLog, strErrLog.c_str());

	return strOption;
}

CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue)
{
	CString strOptionLine = _T("   --");
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}

