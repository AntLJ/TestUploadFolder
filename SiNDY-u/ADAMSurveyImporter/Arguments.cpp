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
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>

#include "TimeUtil.h"
#include "LogUtil.h"

using namespace std;
using namespace uh;
namespace po = boost::program_options;

const uh::tstring Arguments::PS	=	_T("PS");
const uh::tstring Arguments::AT	=	_T("AT");
const uh::tstring Arguments::KK	=	_T("KK");
const uh::tstring Arguments::IPC	=	_T("IPC");

Arguments::Arguments() : 
	survey_dir(_T(""))
	,import_db(_T(""))
	,company(_T(""))
	,lot(_T(""))
	,log_dir(_T(".\\"))	
	,crd_param(_T("\\\\win\\tdc\\SiNDY\\config\\TKY2JGD.par"))
	,unuse_version(false)
	,skip_delete_record(false)
{
	//���t��ݒ�
	time_t now = time(NULL);
	struct tm pnow;
	localtime_s(&pnow, &now);

	date = TimeUtil::GetTime();
}


// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const SURVERY_DIR		=	"survey_dir";
	const char * const IMPORT_DB		=	"import_db";
	const char * const COMPANY			=	"company";
	const char * const DATA				=	"date";
	const char * const LOT				=	"lot";
	const char * const LOG_DIR			=	"log_dir";
	const char * const CRD_PARAM		=	"crd_param";
	const char * const UNUSE_VERSION	=	"unuse_version";
	const char * const SKIP_DELETE_RECORD = "skip_delete_record";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(SURVERY_DIR,	tvalue<tstring>(&survey_dir	),	"[�K�{]�C���|�[�g�f�B���N�g��"			)
			(IMPORT_DB,		tvalue<tstring>(&import_db	),	"[�K�{]�C���|�[�g��DB�ڑ����"			)
			(COMPANY,		tvalue<tstring>(&company	),	"[�K�{]�f�[�^�쐬��"					)
			(DATA,			tvalue<tstring>(&date		),	"[�K�{]�f�[�^��̓�"					)
			(LOT,			tvalue<tstring>(&lot		),	"[�K�{]���b�g��"					)
			(LOG_DIR,		tvalue<tstring>(&log_dir	),	"[�K�{]���O�o�͐�t�H���_�p�X"			)
			(CRD_PARAM,		tvalue<tstring>(&crd_param	),	"[�C��]���W�ϊ��p�p�����[�^�t�@�C��"	)
			(UNUSE_VERSION,									"[�C��]�o�[�W�������g�p(�ҏW�J�n�`�I�����Ȃ�)"	)
			(SKIP_DELETE_RECORD,							"[�C��]�X�V�R�[�h�u�폜�v�̃��R�[�h���X�L�b�v���邩"	)
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		good &= CheckParam( SURVERY_DIR,	survey_dir	);
		good &= CheckParam( IMPORT_DB,		import_db	);
		good &= CheckParam( COMPANY,		company		);
		good &= CheckParam( DATA,			date		);
		good &= CheckParam( LOT,			lot			);
		good &= CheckParam( LOG_DIR,		log_dir		);

		if( vm.count(UNUSE_VERSION) != 0 )
		{
			unuse_version = true;
		}
		if( vm.count(SKIP_DELETE_RECORD) != 0 )
		{
			skip_delete_record = true;
		}

		if( ! good ){
			LogUtil::printError(desc);
			return false;
		}

		if(!CheckParamCompany(company))
		{
			LogUtil::printError(desc);
			LogUtil::printError(70, "�f�[�^�쐬��\t���̓p�����[�^���s���ł�");
			return false;
		}

		if(!CheckParamDate(date))
		{
			LogUtil::printError(desc);
			LogUtil::printError(71, "�f�[�^��̓�\t���̓p�����[�^���s���ł�");
			return false;
		}
	}
	catch(const std::exception& e){
		LogUtil::printError(desc);
		LogUtil::printError("�ُ�I�����܂���");
		LogUtil::printError("��O���o\t" +  uh::toStr(e.what()));
		return false;
	}

	//�p�����[�^�o��
	LogUtil::print("[���̓p�����[�^]");
	LogUtil::printParam(uh::toTStr(SURVERY_DIR),		survey_dir	);
	LogUtil::printParam(uh::toTStr(IMPORT_DB),			import_db	);
	LogUtil::printParam(uh::toTStr(COMPANY),			company		);
	LogUtil::printParam(uh::toTStr(DATA),				date		);
	LogUtil::printParam(uh::toTStr(LOT),				lot			);
	LogUtil::printParam(uh::toTStr(LOG_DIR),			log_dir		);
	LogUtil::printParam(uh::toTStr(CRD_PARAM),			crd_param	);
	LogUtil::printParam(uh::toTStr(UNUSE_VERSION),		unuse_version ? _T("�o�[�W�������p���Ȃ�")  : _T("�o�[�W�������p����"));
	LogUtil::printParam(uh::toTStr(SKIP_DELETE_RECORD), skip_delete_record ? _T("�X�V�R�[�h�u�폜�v�̃��R�[�h���X�L�b�v����")  : _T("�X�V�R�[�h�u�폜�v�̃��R�[�h���X�L�b�v���Ȃ�"));

	return true;
}

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << "�I�v�V�����͕K�{�ł�" << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() )){
			cerr << toStr( optValue ) << "�̓t�@�C���ł͂���܂���" << endl;
			return false;
		}
	}

	return true;
}

bool Arguments::CheckParamCompany(const uh::tstring& optValue)
{
	bool ret = false;

	if(optValue == PS)
	{
		ret = true;
	}
	else if(optValue == AT)
	{
		ret = true;
	}
	else if(optValue == KK)
	{
		ret = true;
	}
	else if(optValue == IPC)
	{
		ret = true;
	}

	return ret;
}

bool Arguments::CheckParamDate(const uh::tstring& optValue)
{
	bool ret = true;

	//�ЂƂ܂� ������4���� + "/" + ������2���� + "/" + ������2���� ���`�F�b�N

	// "/"�ŕ���
	std::vector<uh::tstring> splitValue;
	boost::algorithm::split(splitValue, optValue, boost::is_any_of("/"));

	if(splitValue.size() != 3)
	{
		return false;
	}

	//�����񐔂��`�F�b�N
	if(splitValue.at(0).size() != 4)
	{
		return false;
	}
	else
	{
		//�͈̓`�F�b�N
		int intValue = std::atoi(uh::toStr(splitValue.at(0)).c_str());
		if(intValue < 1990 || intValue > 2100)
		{
			return false;
		}
	}

	if(splitValue.at(1).size() != 2)
	{
		return false;
	}
	else
	{
		//�͈̓`�F�b�N
		int intValue = std::atoi(uh::toStr(splitValue.at(1)).c_str());
		if(intValue < 0 || intValue > 12)
		{
			return false;
		}
	}

	if(splitValue.at(2).size() != 2)
	{
		return false;
	}
	else
	{
		//�͈̓`�F�b�N
		int intValue = std::atoi(uh::toStr(splitValue.at(2)).c_str());
		if(intValue < 1 || intValue > 31)
		{
			return false;
		}
	}

	return true;
}