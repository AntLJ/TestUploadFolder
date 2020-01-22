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
#include <atl2/io.h>
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "Util.h"
#include "ImportBusPoint.h"
#include "CheckBusPoint.h"
#include "ExportBusPoint.h"

namespace po = boost::program_options;

/**
 * @brief �K�v�ȃI�v�V������񂪂��邩���`�F�b�N����
 * @param vm [in] ������map
 * @param option [in] �I�v�V��������
 * @param desctiption [in] �I�v�V�����̋L�q
 * @retval true �K�v�ȃI�v�V��������
 * @retval false �K�v�ȃI�v�V�����Ȃ�
 */
bool check_required_option(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption);


int _tmain(int argc, _TCHAR* argv[])
{
	try{
		const arctl::coinitializer aCoInitializer;

		try{
			// ���P�[���֘A�̕s��h�~
			uh::setJapaneseCharacterTypeFacet();

			po::options_description desc("Allowed options");

			const char * const k_import          = "import";
			const char * const k_auto_rpoint     = "auto_rpoint";
			const char * const k_db_check        = "db_check";
			const char * const k_export          = "export";
			const char * const k_in_list         = "in_list";
			const char * const k_import_log      = "import_log";
			const char * const k_auto_rpoint_log = "auto_rpoint_log";
			const char * const k_db_check_log    = "db_check_log";
			const char * const k_export_log      = "export_log";
			const char * const k_bus_db          = "bus_db";
			const char * const k_road_db         = "road_db";
			const char * const k_company_list    = "company_list";
			const char * const k_export_dir      = "export_dir";

			Arguments	args;
			desc.add_options()
				(k_import,          "�C���|�[�g���[�h(ON/OFF)")
				(k_auto_rpoint,     "���[�g�T���p�|�C���g���[�h(ON/OFF)")
				(k_db_check,        "DB�`�F�b�N���[�h(ON/OFF)")
				(k_export,          "�G�N�X�|�[�g���[�h(ON/OFF)")
				(k_in_list,         uh::tvalue<uh::tstring>(&args.in_list),         "[�C���|�[�g][�K�{]�������ʃt�@�C�����X�g")
				(k_import_log,      uh::tvalue<uh::tstring>(&args.import_log),      "[�C���|�[�g][�K�{]�C���|�[�g���O�t�@�C��")
				(k_auto_rpoint_log, uh::tvalue<uh::tstring>(&args.auto_rpoint_log), "[���[�g�T���pP][�K�{]���[�g�T���pP���O�t�@�C��")
				(k_db_check_log,    uh::tvalue<uh::tstring>(&args.db_check_log),    "[DB�`�F�b�N][�K�{]DB�`�F�b�N���O�t�@�C��")
				(k_export_log,      uh::tvalue<uh::tstring>(&args.export_log),      "[�G�N�X�|�[�g][�K�{]�G�N�X�|�[�g���O�t�@�C��")
				(k_bus_db,          uh::tvalue<uh::tstring>(&args.bus_db),			"[�C���|�[�g/���[�g�T���pP/DB�`�F�b�N/�G�N�X�|�[�g][�K�{]�o�X�ڑ��pDB")
				(k_road_db,         uh::tvalue<uh::tstring>(&args.road_db),			"[���[�g�T���pP/DB�`�F�b�N][�K�{]���HNW�pDB")
				(k_company_list,    uh::tvalue<uh::tstring>(&args.company_list),    "[�G�N�X�|�[�g]�G�N�X�|�[�g�Ώێ��Ǝ҃��X�g(�w�薳���F�S���Ǝ�)")
				(k_export_dir,      uh::tvalue<uh::tstring>(&args.export_dir),      "[�G�N�X�|�[�g][�K�{]�G�N�X�|�[�g�f�B���N�g��")
				;

			if(argc == 1) {
				std::cerr << desc << std::endl;
				return 1;
			}

			// argc, argv ����͂��āA���ʂ�vm�Ɋi�[
			po::variables_map vm;
			store(parse_command_line(argc, argv, desc), vm);
			notify(vm);

			if( vm.count(k_import) != 0 )      { args.bImport     = true; }
			if( vm.count(k_auto_rpoint) != 0 ) { args.bAutoRPoint = true; }
			if( vm.count(k_db_check) != 0 )    { args.bDbCheck    = true; }
			if( vm.count(k_export) != 0 )      { args.bExport     = true; }

			// �K�{�p�����[�^�`�F�b�N
			bool bMust = true;

			// ���Ȃ��Ƃ���̃��[�h���w�肷��K�v����
			bMust &= ( args.bImport || args.bAutoRPoint || args.bDbCheck || args.bExport );

			// �C���|�[�g���[�h�ŕK�{�p�����[�^
			if( args.bImport ){
				bMust &= check_required_option( vm, k_in_list,    "[�C���|�[�g][�K�{]�������ʃt�@�C�����X�g");
				bMust &= check_required_option( vm, k_import_log, "[�C���|�[�g][�K�{]�C���|�[�g���O�t�@�C��");
				bMust &= check_required_option( vm, k_bus_db,     "[�C���|�[�g/���[�g�T���pP/DB�`�F�b�N][�K�{]�o�X�ڑ��pDB");

			}

			// ���[�g�T���pP���[�h�ŕK�{�p�����[�^
			if( args.bAutoRPoint ){
				bMust &= check_required_option( vm, k_auto_rpoint_log, "[���[�g�T���pP][�K�{]���[�g�T���pP���O�t�@�C��");
				bMust &= check_required_option( vm, k_bus_db,          "[�C���|�[�g/���[�g�T���pP][�K�{]�o�X�ڑ��pDB");
				bMust &= check_required_option( vm, k_road_db,         "[���[�g�T���pP/DB�`�F�b�N][�K�{]���HNW�pDB");
			}

			// DB�`�F�b�N���[�h�ŕK�{�p�����[�^
			if( args.bDbCheck ){
				bMust &= check_required_option( vm, k_db_check_log, "[DB�`�F�b�N][�K�{]DB�`�F�b�N���O�t�@�C��");
				bMust &= check_required_option( vm, k_bus_db,       "[�C���|�[�g/���[�g�T���pP][�K�{]�o�X�ڑ��pDB");
				bMust &= check_required_option( vm, k_road_db,      "[���[�g�T���pP/DB�`�F�b�N][�K�{]���HNW�pDB");
			
				// DB�`�F�b�N�͔r������
				if( args.bImport || args.bAutoRPoint || args.bExport ){
					std::cerr << "DB�`�F�b�N���[�h�Ƒ����[�h�͓������s���Ȃ��ŉ�����" << std::endl;
					bMust = false;
				}
			}

			// �G�N�X�|�[�g���[�h�ŕK�{�p�����[�^
			if( args.bExport ){
				bMust &= check_required_option( vm, k_export_log,     "[�G�N�X�|�[�g][�K�{]�G�N�X�|�[�g���O�t�@�C��");
				bMust &= check_required_option( vm, k_bus_db,         "[�C���|�[�g/���[�g�T���pP][�K�{]�o�X�ڑ��pDB");
				bMust &= check_required_option( vm, k_export_dir,     "[�G�N�X�|�[�g][�K�{]�G�N�X�|�[�g�f�B���N�g��");
			
				// �G�N�X�|�[�g�Ώێ��Ǝ҂̎w�肪�Ȃ��ꍇ�́A�S���Ǝҏo�̓��[�h
				if( vm.count(k_company_list) == 0 ){
					args.bAllCompany = true;
				}
	
				// �G�N�X�|�[�g���[�h�͔r������
				if( args.bImport || args.bAutoRPoint || args.bDbCheck ){
					std::cerr << "�G�N�X�|�[�g���[�h�Ƒ����[�h�͓������s���Ȃ��ŉ�����" << std::endl;
					bMust = false;
				}
			}

			// �p�����[�^������ł͂Ȃ��ꍇ�́A�I�v�V�����\�����ďI��
			if( !bMust){
				std::cerr << "�o�X�f�[�^�����c�[��" << std::endl;
				std::cerr << desc << std::endl;
				return 1;
			}

			// ������
			bool bRet = false;

			// �C���|�[�g�����[�g�T���p�|�C���g
			if( args.bImport || args.bAutoRPoint ){
				CImportBusPoint app( args );
				if( app.execute() ){
					bRet = true;
				}
				else{
					bRet = false;
				}
			}
			// DB�`�F�b�N
			else if( args.bDbCheck ){
				CCheckBusPoint app( args );
				if( app.execute() ){
					bRet = true;
				}
				else{
					bRet = false;
				}
			}
			// �G�N�X�|�[�g
			else if( args.bExport ){
				CExportBusPoint app( args );
				if( app.execute() ){
					bRet = true;
				}
				else{
					bRet = false;
				}
			}
			else{
				bRet = false;
			}

			// ������̏I������
			if( bRet ){
				// ����I��
				std::cerr << "����I��" << std::endl;
				return 0;
			}
			else{
				// �ُ�I��
				std::cerr << "�ُ�I��" << std::endl;
				return 1;
			}
		}
		catch(const _com_error& e) {
			std::cerr << atl2::stringize(e) << std::endl;
			std::cerr << "�ُ�I��" << std::endl;
		}
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "�ُ�I��" << std::endl;
	}

	return 2;
}

// �K�v�ȃI�v�V������񂪂��邩���`�F�b�N����
bool check_required_option(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption)
{
	if( vm.count(option) == 0 ){
		std::cerr << desctiption << "�����ݒ�ł��B" << std::endl;
		return false;
	}
	return true;
}


