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
#include "Reflect.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/text_util.h>
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;
using namespace std;
using namespace uh;

CArguments::CArguments(void)
{
	m_testMode = false;
	m_forceMode = false;
	m_tdcMode = false;
	m_exceptSdeMode = false;
	m_geospaceMode = false;
	m_onlyImport = false;
	m_emptyDBMode = false;
}


CArguments::~CArguments(void)
{
}


/** 
 * @brief �g�p�@�\�� �i������Ƃ����ڂ����j
 */ 
void PrintHelp()
{
	cerr << "�ҏW�O�E�ҏW���PGDB�̍�����SiNDY�ɔ��f������v���O�����ł��B" << endl;
	cerr << "���b�V�����E�ɂ�������̂͑S�č����ւ��ΏۂɂȂ�܂��B" << endl;
	cerr << "�ݒ�t�@�C���̏������́ADiffImportPGDB.txt�������������B" << endl;
}


void OutputOption( const uh::tstring& option )
{
	cout << "#" << uh::toStr(option) << endl;
	cerr << "#" << uh::toStr(option) << endl;
}

void OutputOption( const uh::tstring& option, const uh::tstring& value )
{
	cout << "#" << uh::toStr(option) << ":" << uh::toStr(value) << endl;
	cerr << "#" << uh::toStr(option) << ":" << uh::toStr(value) << endl;
}

bool CArguments::parse( int argc, _TCHAR* argv[] )
{
	const char * const old_pgdb =     "old,O";
	const char * const new_pgdb =     "new,N";
	const char * const mesh =         "mesh,M";
	const char * const suffix =       "suffix,S";
	const char * const test_mode =    "test,T";
	const char * const force_mode =   "force,F";
	const char * const tdc_priority = "tdc_priority,J";
	const char * const except_sde =   "except_sde,E";
	const char * const geospace =     "geospace,G";
	const char * const availdate =    "availdate,D";
	const char * const logdir =       "logdir,L";
	const char * const only_import =  "only_import,I";
	const char * const empty_db =     "empty_db,P";

	// "hoge,h"�̌`����variables_map.count()�ł��܂�����ł��Ȃ��̂Ń`�F�b�N�p�ɕʓr��`
	const char * const test_mode_check =    "test";
	const char * const force_mode_check =   "force";
	const char * const tdc_priority_check = "tdc_priority";
	const char * const except_sde_check =   "except_sde";
	const char * const geospace_check =     "geospace";
	const char * const only_import_check =  "only_import";
	const char * const empty_db_check =     "empty_db";

	po::options_description desc("how to use");
	try
	{
		desc.add_options()
			(old_pgdb,     tvalue<tstring>(&m_oldPath),       "[�K�{]�ҏW�OPGDB")
			(new_pgdb,     tvalue<tstring>(&m_newPath),       "[�K�{]�ҏW��PGDB")
			(mesh,         tvalue<tstring>(&m_meshPath),      "[�K�{]���b�V�����X�g")
			(suffix,       tvalue<tstring>(&m_suffix),        "[�K�{]Suffix")
			(logdir,       tvalue<tstring>(&m_logPath),       "[�K�{]���O�o�̓f�B���N�g��")
			(test_mode,                                       "[�C��]�e�X�g���[�h")
			(force_mode,                                      "[�C��](�o�[�W������Ή���)�����C���|�[�g���[�h")
			(tdc_priority,                                    "[�C��]�����D�惂�[�h")
			(except_sde,                                      "[�C��]PGDB�EFGDB�C���|�[�g���[�h")
			(geospace,     tvalue<tstring>(&m_layerListPath), "[�C��]GEOSPACE�C���|�[�g���[�h")
			(only_import,                                     "[�C��]�}�[�W�Ȃ����[�h")
			(empty_db,                                        "[�C��]��DB�C���|�[�g���[�h")
			(availdate,    tvalue<tstring>(&m_availDate),     "[poi�̂Ƃ��K�{]�L�����t YYYY/MM/DD")
			;
	
		po::variables_map vm;
		po::store( po::parse_command_line(argc, argv, desc), vm );
		po::notify( vm );

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if( m_oldPath.empty() )
		{
			cerr << old_pgdb << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("�ҏW�OPGDB"), m_oldPath );
		}
		
		if( m_newPath.empty() )
		{
			cerr << new_pgdb << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("�ҏW��PGDB"), m_newPath );
		}
		
		if( m_meshPath.empty() )
		{
			cerr << mesh << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("���b�V�����X�g"), m_meshPath );
		}

		if( m_suffix.empty() )
		{
			cerr << suffix << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("Suffix"), m_suffix );
			if( m_suffix == _T("poi") )
			{
				if( m_availDate.empty() )
				{
					cerr << suffix << "��poi�̂Ƃ�" << availdate << "�I�v�V�����͕K�{�ł�" << endl;
					good = false;
				}
				else
					OutputOption( _T("�L�����t"), m_availDate );
			}
		}

		if( m_logPath.empty() )
		{
			cerr << logdir << "�I�v�V�����͕K�{�ł�" << endl;
			good = false;
		}
		else
		{
			OutputOption( _T("���O�o�͐�"), m_logPath );
			CLogFile logFile( m_logPath.c_str() );
			m_logFile = logFile;
		}

		// �C�ӃI�v�V�����m�F
		if( vm.count(test_mode_check) )
		{
			OutputOption( _T("�e�X�g���[�h�Ŏ��s") );
			m_testMode = true;
		}
		
		if( vm.count(force_mode_check) )
		{
			OutputOption( _T("�����C���|�[�g���[�h�Ŏ��s") );
			m_forceMode = true;
		}

		if( vm.count(tdc_priority_check) )
		{
			OutputOption( _T("�����D�惂�[�h�Ŏ��s") );
			m_tdcMode = true;
		}

		if( vm.count(except_sde_check) )
		{
			OutputOption( _T("PGDB�EFGDB�C���|�[�g���[�h�Ŏ��s") );
			m_exceptSdeMode = true;
		}

		if( vm.count(geospace_check) )
		{
			OutputOption( _T("GEOSPACE�C���|�[�g���[�h�Ŏ��s") );
			
			if( m_layerListPath.empty() )
			{
				cerr << geospace << "�����ւ����C�����X�g���w�肳��Ă��܂���" << endl;
				good = false;
			}
			else
			{
				OutputOption( _T("�����ւ����C�����X�g"), m_layerListPath );
				list<tstring> layerList = text_util::ToTStrList(m_layerListPath);
				for( auto& layer : layerList )
				{
					m_layerList.push_back( ((CString)layer.c_str()).MakeUpper() );
				}

				m_geospaceMode = true;
			}
		}

		if( vm.count(only_import_check) )
		{
			OutputOption( _T("�}�[�W�Ȃ����[�h�Ŏ��s") );
			m_onlyImport = true;
		}

		if( vm.count(empty_db_check) )
		{
			OutputOption( _T("��DB�C���|�[�g���[�h�Ŏ��s") );
			m_emptyDBMode = true;
		}

		if( !good )
		{
			cerr << desc << endl;
			PrintHelp();
			return false;
		}
	}
	catch( const std::exception& e )
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	
	return true;
}
