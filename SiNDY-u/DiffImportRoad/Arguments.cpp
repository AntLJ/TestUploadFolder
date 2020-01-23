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
#include <WinLib/arcobjects_import_highmethod.h>
#include <sindy/workspace.h>
#include <schema.h>
#include "Arguments.h"
#include <iostream>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// �R�}���h���C�������̉��
bool Arguments::parse( int argc, _TCHAR* argv[], CInputInfo& cInputInfo )
{
	const char * const k_beforepath = "beforepath";
	const char * const k_afterpath = "afterpath";
	const char * const k_sdeconnect = "sdeconnect";
	const char * const k_operator = "operator";
	const char * const k_purpose = "purpose";
	const char * const k_meshlist = "meshlist";
	const char * const k_logdir = "logdir";
	const char * const k_testmode = "testmode";
	const char * const k_except_fieldlist = "except_fieldlist";	// bug 10363
	const char * const k_help = "help";
	po::options_description desc("How to use");

	try
	{
		// �I�v�V�����̐݌v
		desc.add_options()
			(k_beforepath, uh::tvalue<uh::tstring>(),  "�ҏW�OPGDB�x�[�X�f�B���N�g���i�K�{�j")
			(k_afterpath, uh::tvalue<uh::tstring>(),   "�ҏW��PGDB�x�[�X�f�B���N�g���i�K�{�j")
			(k_sdeconnect, uh::tvalue<uh::tstring>(),  "�C���|�[�g���DB�ڑ�������i�K�{�j")
			(k_operator, uh::tvalue<uh::tstring>(),    "�C���|�[�g���̕ҏW�Җ��i�w�肵�Ȃ��ꍇ�͍X�V���Ȃ��j")
			(k_purpose, uh::tvalue<long>(),            "�C���|�[�g���̍�ƖړI(�R�[�h�l�Ŏw��)(�w�肵�Ȃ��ꍇ�͍X�V���Ȃ�)")
			(k_meshlist, uh::tvalue<uh::tstring>(),    "���b�V�����X�g�i�K�{�j")
			(k_logdir, uh::tvalue<uh::tstring>(),      "���O�t�@�C���o�͐�f�B���N�g���i�K�{�j")
			(k_testmode,                               "�e�X�g���[�h")
			(k_except_fieldlist, uh::tvalue<uh::tstring>(), "�C���|�[�g���O�t�B�[���h���X�g")	// bug 10363
			(k_help,                                   "�w���v��\��")
			;
		
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		//�w���v�\��
		if( vm.count(k_help)){
			PrintHelp(argv[0], desc);
			return false;
		}

		// �K�{�I�v�V��������
		if( !vm.count(k_beforepath) || !vm.count(k_afterpath) || !vm.count(k_sdeconnect) || !vm.count(k_meshlist) || !vm.count(k_logdir))
		{
			PrintUsage(argv[0], desc);
			return false;
		}

		//
		if( !isDirectory( vm[k_beforepath].as<uh::tstring>().c_str() ) )
		{
			cerr << "ERROR\t"<< k_beforepath << " : " << CT2A(vm[k_beforepath].as<uh::tstring>().c_str()) << " �����݂��܂���" << endl;
			return false;
		}
		if( !isDirectory( vm[k_afterpath].as<uh::tstring>().c_str() ) )
		{
			cerr << "ERROR\t"<< k_afterpath<< " : " << CT2A(vm[k_afterpath].as<uh::tstring>().c_str()) << " �����݂��܂���" << endl;
			return false;
		}
		if( !isFile( vm[k_meshlist].as<uh::tstring>().c_str() ) )
		{
			cerr << "ERROR\t" << k_meshlist << " : " << CT2A(vm[k_meshlist].as<uh::tstring>().c_str()) << " �����݂��܂���" << endl;
			return false;
		}
		if( !isDirectory( vm[k_logdir].as<uh::tstring>().c_str() ) )
		{
			cerr << "ERROR\t" << k_logdir << " : " << CT2A(vm[k_logdir].as<uh::tstring>().c_str()) << " �����݂��܂���" << endl;
			return false;
		}

		IWorkspacePtr ipWorkspace = sindy::create_workspace( vm[k_sdeconnect].as<uh::tstring>().c_str() );
		if( !ipWorkspace )
		{
			cerr << "ERROR\t" << k_sdeconnect << " : " << CT2A(vm[k_sdeconnect].as<uh::tstring>().c_str()) << " �����������m�F���Ă�������" << endl;
			return false;
		}
				
		// [bug 10363]�Ή�
		if( vm.count(k_except_fieldlist) )
		{
			if(!isFile( vm[k_except_fieldlist].as<uh::tstring>().c_str()))
			{
				cerr << "ERROR\t" << k_except_fieldlist << " : " << CT2A(vm[k_except_fieldlist].as<uh::tstring>().c_str()) << " �����݂��܂���" << endl;
				return false;
			}
			else
			{
				cInputInfo.strExceptList = vm[k_except_fieldlist].as<uh::tstring>().c_str();
			}
		}

		ifstream ifs;
		ifs.open( vm[k_meshlist].as<uh::tstring>().c_str(), ios::in);
		if( ifs.fail() )
		{
			cerr << "ERROR\t" << k_meshlist << " : " << CT2A(vm[k_meshlist].as<uh::tstring>().c_str()) << " �����������m�F���Ă�������" << endl;
			return false;
		}
		char cLine[256] = {0};
		while(ifs.getline(cLine, 256))
		{
			CString strLine(cLine);
			if( strLine.IsEmpty() )
				continue;
			cInputInfo.setMesh.insert(_ttol(strLine));
		}

		cInputInfo.strBeforeDir = vm[k_beforepath].as<uh::tstring>().c_str();
		cInputInfo.strAfterDir = vm[k_afterpath].as<uh::tstring>().c_str();
		cInputInfo.strSDEConnect = vm[k_sdeconnect].as<uh::tstring>().c_str();
		cInputInfo.strLogDir = vm[k_logdir].as<uh::tstring>().c_str();
		cInputInfo.bTestMode = ( vm.count(k_testmode) ? true : false );
		if(vm.count(k_operator))
			cInputInfo.strOperator = vm[k_operator].as<uh::tstring>().c_str();
		if(vm.count(k_purpose))
			cInputInfo.lPurpose = vm[k_purpose].as<long>();
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

/** 
 * @brief �g�p�@�\�� 
 * @param[in]	App	���s���R�}���h���C���̈��
 */ 
void Arguments::PrintUsage(LPCTSTR App, po::options_description& opt)
{
	_TCHAR AppName[FILENAME_MAX]= {0};
	_tsplitpath(App, NULL, NULL, AppName, NULL);

	cout << "���H�pPGDB������荞�݃v���O����" << endl;
	cout <<	"Usage:" << CT2A(AppName) << " [�I�v�V����]" << endl;
	cout << opt << endl;
}

/** 
 * @brief �g�p�@�\�� �i������Ƃ����ڂ����j
 * @param[in]	App	���s���R�}���h���C���̈�� 
 */ 
void Arguments::PrintHelp(LPCTSTR App, po::options_description& opt)
{
	PrintUsage(App, opt);
	cout << "�ҏW�O�E�ҏW���PGDB�̍�����SiNDY�ɔ��f������v���O�����ł��B" << endl;
	cout << "�ݒ�t�@�C���̏������́ADiffImportRoad.txt�������������B" << endl;
}
