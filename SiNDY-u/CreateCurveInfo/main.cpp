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

// CreateCurveInfo.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>
#include <arctl/coinitializer.h>
#include <boost/program_options.hpp>
#include "CurveInfoCreator.h"
#include "OutputOptions.h"

namespace po = boost::program_options;

using namespace adlib;

/**
 * @brief �R�}���h���C�������̉�͂ƕϊ������̊J�n
 * @param[in]	vm	�R�}���h���C�������Ǘ��I�u�W�F�N�g
 * @return	�S�Ă̏���������ɏI���������ۂ�
 **/
bool process( po::variables_map& vm )
{
	using namespace curve_info;

	// ���P�[���֘A�̕s��h�~
	setJapaneseLocaleWithCNumpunctFacet();

	// �o�̓I�v�V�����̐ݒ�
	OutputOptions aOutputOptions(
		vm["max_roc_for_region_deletion"].as<long>(),
		vm["max_roc_for_region_deletion_high"].as<long>(),
		vm["max_roc_of_section"].as<long>(),
		vm["max_roc_of_section_high"].as<long>(),
		vm["curve_certify_angle"].as<long>(),
		vm["straight_angle"].as<double>(),
		vm["roc_point_interval"].as<long>(),
		vm["fitting_point_interval"].as<long>(),
		vm["fitting_threshold"].as<long>(),
		vm["section_min_angle"].as<double>()
	);

	if (vm.count("query_where_clause") != 0)
	{
		aOutputOptions.m_QueryWhereClause = vm["query_where_clause"].as<tstring>().c_str();
	}
	aOutputOptions.m_CreateEachMesh = (vm.count("create_each_mesh") != 0);
	aOutputOptions.m_SHP05DirConfig = (vm.count("shp05_dir_config") != 0);
	aOutputOptions.m_OutputVerificationLog = (vm.count("verification_logfile") != 0);
	aOutputOptions.m_LogHandlingProcess = (vm.count("log_handling_process") != 0);
	aOutputOptions.m_LogFitEvaluatingProcess = (vm.count("log_fit_evaluating_process") != 0);

	using namespace boost::filesystem;

	// �ϊ��N���X�̍쐬�Ǝ��s
	curve_info::CurveInfoCreator aCurveInfoCreator( aOutputOptions );
	aCurveInfoCreator.init(
		vm["road_db"].as<tstring>(),
		vm["mesh_db"].as<tstring>(),
		path(vm["meshlist"].as<tstring>().c_str()),
		path(vm["output_dir"].as<tstring>().c_str()),
		path(vm["logfile"].as<tstring>().c_str()),
		aOutputOptions.m_OutputVerificationLog ? path(vm["verification_logfile"].as<tstring>().c_str()) : path()
		);
	aCurveInfoCreator.convert();

	return true;
}

/**
 * @brief �G���g���|�C���g
 */
int _tmain(int argc, _TCHAR* argv[])
{
	bool aIsSuccess = true;

	try
	{
		const arctl::coinitializer aCoInitializer;

		try
		{
			// TODO: tstring�ɑΉ�
			// TODO: "road_db"�ȂǂƏ����̂��P�񂾂��ɂ���
			// TODO: �ڑ��̎������g���C�ɑΉ�
			// TODO: usage�̉��s�������v�����v�g��ŕ�����������̂��C��
			// TODO: �S�̓I�ɁA�|�C���^����NULL���ǂ����̃`�F�b�N���ʓ|�Ȃ̂ŁAstd::tr1::reference_wrapper���g���Ȃǂ��ĎQ�Ƃɂł���Ƃ���͕ς������B
			po::options_description desc("How to use");
			desc.add_options()
				("road_db,R",							po::wvalue<std::wstring>(), "[�K�{] ���H�f�[�^�x�[�X")
				("mesh_db,M",							po::wvalue<std::wstring>(),	"[�K�{] �Q�����b�V���f�[�^�x�[�X")
				("meshlist,m",							po::wvalue<std::wstring>(), "[�K�{] �o�͑ΏۂQ�����b�V���R�[�h�̃��b�V�����X�g�t�@�C���p�X or �ΏۂQ�����b�V���R�[�h ")
				("output_dir,o",						po::wvalue<std::wstring>(), "[�K�{] Shape�t�@�C���o�̓f�B���N�g���p�X")
				("logfile,l",							po::wvalue<std::wstring>(), "[�K�{] ���O�t�@�C���p�X ")
				("max_roc_for_region_deletion,r",		po::value<long>(),			"[�K�{] �̈�폜�̂��߂̍ő�ȗ����a�i�ᑬ��p�j�im�j")
				("max_roc_for_region_deletion_high,t",	po::value<long>(),			"[�K�{] �̈�폜�̂��߂̍ő�ȗ����a�i������p�j�im�j")
				("max_roc_of_section,w",				po::value<long>(),			"[�K�{] ��Ԃ̍ő�ȗ����a�i�ᑬ��p�j�im�j")
				("max_roc_of_section_high,u",			po::value<long>(),			"[�K�{] ��Ԃ̍ő�ȗ����a�i������p�j�im�j")
				("straight_angle,s",					po::value<double>(),		"[�K�{] �����ƌ��Ȃ��p�x�i�x�j")				// TODO: �f�t�H���g�l���L��
				("roc_point_interval,i",				po::value<long>(),			"[�K�{] �ȗ����a�����߂�_�̊Ԋu�im�j")				// TODO: �f�t�H���g�l���L��
				("curve_certify_angle,c",				po::value<long>(),			"[�K�{] �J�[�u�F��p�x�i�x�j")				// TODO: �f�t�H���g�l���L��
				("fitting_point_interval,F",			po::value<long>(),			"[�K�{] �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g���邩��]������_�̊Ԋu�im�j")				// TODO: �f�t�H���g�l���L��
				("fitting_threshold,f",					po::value<long>(),			"[�K�{] �~�ʂ��x�W�G�Ȑ��Ƀt�B�b�g����ƌ��Ȃ�臒l�im�j")				// TODO: �f�t�H���g�l���L��
				("section_min_angle,n",					po::value<double>(),		"[�K�{] ��ԂɑΉ�����~�ʂ̍ŏ��p�x�i�x�j")
				("query_where_clause,q",				po::wvalue<std::wstring>(), "[�C��] �Ώۃ����N�w��N�G��") // bug 9617�Œǉ�
				("create_each_mesh,e",												"[�C��] ���b�V�����Ƃ�Shape���쐬����") // bug 9292�Œǉ�
				("shp05_dir_config,d",												"[�C��] SHP05�̃f�B���N�g���\���ŏo�͂���") // bug 9704�Œǉ�
				("verification_logfile,v",				po::wvalue<std::wstring>(), "[�C��] ���ؗp���O�t�@�C��")
				("log_handling_process,p",											"[�C��] �����ߒ������O�t�@�C���ɏo�͂���")
				("log_fit_evaluating_process,a",									"[�C��] �t�B�b�g�]���ߒ������O�t�@�C���ɏo�͂���")	// ���ʂ����ɑ����̂ŁA--log_handling_process�Ƃ͕ʂɂ��Ă���
				;

			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			// �K�{�p�����[�^�̃`�F�b�N
			if (vm.count("road_db") == 0 ||
				vm.count("mesh_db") == 0 ||
				vm.count("meshlist") == 0 ||
				vm.count("output_dir") == 0 ||
				vm.count("logfile") == 0 ||
				vm.count("max_roc_for_region_deletion") == 0 ||
				vm.count("max_roc_for_region_deletion_high") == 0 ||
				vm.count("max_roc_of_section") == 0 ||
				vm.count("max_roc_of_section_high") == 0 ||
				vm.count("straight_angle") == 0 ||
				vm.count("curve_certify_angle") == 0 ||
				vm.count("roc_point_interval") == 0 ||
				vm.count("fitting_point_interval") == 0 ||
				vm.count("fitting_threshold") == 0 ||
				vm.count("section_min_angle") == 0
				)
			{
				// TODO: ��������Ȃ�����ʒm
				std::cerr << desc << std::endl;
				return 0;
			}

			aIsSuccess = process(vm);
		}
		catch (_com_error& e)
		{
			aIsSuccess = false;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		aIsSuccess = false;
	}

	std::cerr << std::endl << ( aIsSuccess ? "����I��" : "�ُ�I��" ) << std::endl;

	return aIsSuccess ? 0 : 1;
}

