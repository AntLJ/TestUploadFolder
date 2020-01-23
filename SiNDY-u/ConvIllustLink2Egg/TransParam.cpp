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
#include "TransParam.h"

TransParam::TransParam(void)
{
}


TransParam::~TransParam(void)
{
}

bool TransParam::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");
	desc.add_options()
		(REPDB, uh::tvalue<uh::tstring>(&m_repDB), "�ϊ����C���X�g�����N���v���J�ڑ����(�K�{)")
		(REPSCHEMA, uh::tvalue<uh::tstring>(&m_repSchema), "�ϊ����C���X�g�e�[�u���i�[�X�L�[�}(�K�{)")
		(ILLSET, uh::tvalue<uh::tstring>(&m_illSetFile), "�C���X�g�������ݒ�t�@�C���p�X(�K�{)")
		(INPUTLIST, uh::tvalue<uh::tstring>(&m_inputList), "�C���X�g�t�@�C�����Ή��\�p�X(�K�{)")
		(EGGDB, uh::tvalue<uh::tstring>(&m_eggDB), "�ϊ���EGG�ڑ����(�K�{)")
		(EGGSCHEMA, uh::tvalue<uh::tstring>(&m_eggSchema), "�ϊ���EGG�o�̓X�L�[�}(�K�{)")
		(RUNLOG, uh::tvalue<uh::tstring>(&m_runLog), "���s���O(�K�{)")
		(ERRLOG, uh::tvalue<uh::tstring>(&m_errLog), "�G���[���O(�K�{)");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// �����������ꍇ�́A�g�������o��
	if (1 == argc) {
		std::cout << desc << std::endl;
		return false;
	}

	// �K�{�I�v�V�����̗L���`�F�b�N
	if (! OptionExistChk(vm, REPDB) || ! OptionExistChk(vm, REPSCHEMA) || 
		! OptionExistChk(vm, ILLSET) || ! OptionExistChk(vm, INPUTLIST) ||
		! OptionExistChk(vm, EGGDB) || ! OptionExistChk(vm, EGGSCHEMA) ||
		! OptionExistChk(vm, RUNLOG) || ! OptionExistChk(vm, ERRLOG)) {
			std::cout << desc << std::endl;
			return false;
	}
	// �t�@�C���p�X�p�X�`�F�b�N
	if (! FileExistChk(ILLSET, m_illSetFile.c_str()) || ! FileExistChk(INPUTLIST, m_inputList.c_str())) {
		std::cout << desc << std::endl;
		return false;
	}
	return true;

}

std::string TransParam::GetOptions()
{
	std::string options = "[option]\n";
	options.append(GetOptionLine(REPDB, m_repDB));
	options.append(GetOptionLine(REPSCHEMA, m_repSchema));
	options.append(GetOptionLine(ILLSET, m_illSetFile));
	options.append(GetOptionLine(INPUTLIST, m_inputList));
	options.append(GetOptionLine(EGGDB, m_eggDB));
	options.append(GetOptionLine(EGGSCHEMA, m_eggSchema));
	options.append(GetOptionLine(RUNLOG, m_runLog));
	options.append(GetOptionLine(ERRLOG, m_errLog));
	return options;
}

bool TransParam::OptionExistChk(const po::variables_map& vm, LPCSTR option)
{
	if (vm.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "�I�v�V�������ݒ肳��Ă��܂���" << std::endl;
		return false;
	}
	return true;
}

bool TransParam::FileExistChk(LPCSTR option, LPCTSTR path)
{
	if (! PathFileExists(path)){
		std::cerr << "ERROR\t" << option << "�I�v�V�������s���ł�" << std::endl;
		return false;
	}
	return true;
}

std::string TransParam::GetOptionLine(LPCSTR option, uh::tstring content)
{
	std::string optionLine = "  --";
	optionLine.append(option);
	if (0 != content.length()) {
		optionLine.append("\t");
		optionLine.append(uh::toStr(content));
	}
	optionLine.append("\n");

	return optionLine;
}
