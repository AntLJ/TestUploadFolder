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
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "Util.h"
#include "BusPointExcelChk.h"

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
		::CoInitialize(NULL);

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		po::options_description desc("Allowed options");

		const char * const k_in_list		= "in_list";
		const char * const k_check_log		= "check_log";

		Arguments	Args;
		desc.add_options()
			(k_in_list,			uh::tvalue<uh::tstring>(&Args.in_list),			"[�K�{]�������ʃt�@�C�����X�g")
			(k_check_log,		uh::tvalue<uh::tstring>(&Args.check_log),		"[�K�{]�`�F�b�N���O�t�@�C��")
			;

		if(argc == 1) {
			std::cerr << desc << std::endl;
			return 1;
		}

		// argc, argv ����͂��āA���ʂ�vm�Ɋi�[
		po::variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		// �K�{�p�����[�^�`�F�b�N
		bool	bMust = true;

		bMust &= check_required_option( vm, k_in_list,		"[�K�{]�������ʃt�@�C�����X�g");
		bMust &= check_required_option( vm, k_check_log,	"[�K�{]�`�F�b�N���O�t�@�C��");

		if( !bMust){
			std::cerr << "�o�X�f�[�^Excel�`�F�b�N�c�[��" << std::endl;
			std::cerr << desc << std::endl;
			return 1;
		}

		// ������
		bool bRet = false;

		CBusPointExcelChk app( Args );
		if( app.execute() ){
			bRet = true;
		}
		else{
			bRet = false;
		}

		::CoUninitialize();

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
		::CoUninitialize();
		std::cerr << atl2::stringize(e) << std::endl;
		std::cerr << "�ُ�I��" << std::endl;
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "�ُ�I��" << std::endl;
	}

	return 1;
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


