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

#pragma once
#include "common.h"
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <shlwapi.h>

using boost::program_options::option_description;
using boost::program_options::value;

namespace po = boost::program_options;

class TransParam
{
public:
	TransParam(void);
	~TransParam(void);

	bool parse(int argc, _TCHAR* argv[]);

	CString GetRepDB() const { return CString(m_repDB.c_str()); };
	CString GetRepSchema() const { return CString(m_repSchema.c_str()); };
	CString GetIllSetFile() const { return CString(m_illSetFile.c_str()); };
	CString GetInputList() const { return CString(m_inputList.c_str()); };
	CString GetEggDB() const { return CString(m_eggDB.c_str()); };
	CString GetEggSchema() const { return CString(m_eggSchema.c_str()); };
	CString GetRunLog() const { return CString(m_runLog.c_str()); };
	CString GetErrLog() const { return CString(m_errLog.c_str()); };

	std::string GetOptions();

private:
	bool OptionExistChk(const po::variables_map& vm, LPCSTR option); 
	bool FileExistChk(LPCSTR option, LPCTSTR path);
	std::string GetOptionLine(LPCSTR option, uh::tstring content); 

private:
	uh::tstring	m_repDB;		// �ϊ����C���X�g�����N���v���J�ڑ����
	uh::tstring m_repSchema;	// �ϊ����C���X�g�e�[�u���i�[�X�L�[�}
	uh::tstring m_illSetFile;	// �C���X�g�������ݒ�t�@�C���p�X
	uh::tstring m_inputList;	// �C���X�g�t�@�C�����Ή��\�p�X
	uh::tstring m_eggDB;		// �ϊ���EGG�ڑ����
	uh::tstring m_eggSchema;	// �ϊ���EGG�o�̓X�L�[�}
	uh::tstring m_runLog;		// ���s���O�t�@�C���p�X
	uh::tstring m_errLog;		// �G���[���O�t�@�C���p�X
};

