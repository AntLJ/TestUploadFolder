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
#include<iostream>
#include<io.h>
#include<cstdio>

// �K�v�ȃI�v�V������񂪂��邩���`�F�b�N����
bool Argument::checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR description) const
{
	if( vm.count(option) == 0 ){
		cerr << description << "�����ݒ�ł��B" << std::endl;
		return false;
	}
	return true;
}

// �R�}���h���C�������̉��
bool Argument::setParameter(int argc, TCHAR** argv, FileController& runLogger, FileController& errLogger)
{
	po::options_description desc("Allowed options");
	const char * const k_convMode     = "conv_mode";
	const char * const k_transResult  = "trans_result";
	const char * const k_out          = "out";
	const char * const k_runLog       = "run_log";
	const char * const k_errLog       = "err_log";

	// �R�}���h���C����������
	Arguments args;
	desc.add_options()
		(k_convMode,        uh::tvalue<uh::tstring>(&args.convMode),       "[�K�{]�I�v�V�����Fcity,addr,all")
		(k_transResult,     uh::tvalue<uh::tstring>(&args.transResult),    "[�K�{]�|�󌋉ʃt�H���_")
		(k_out,             uh::tvalue<uh::tstring>(&args.out),            "[�K�{]�ϊ����ʃt�H���_")
		(k_runLog,          uh::tvalue<uh::tstring>(&args.runLog),         "[�K�{]���s���O�t�@�C��")
		(k_errLog,          uh::tvalue<uh::tstring>(&args.errLog),         "[�K�{]�G���[���O�t�@�C��")
		;

	if(argc == 1) {
		wcerr << L"�R�}���h���C�������ɖ�肪����܂�" << endl;
		return false;
	}

	// argc, argv ����͂��āA���ʂ�vm�Ɋi�[
	po::variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	// �K�{�p�����[�^�`�F�b�N
	bool bMust = true;

	bMust &= checkParameter( vm, k_convMode,        "[�K�{]�I�v�V�����Fcity,addr,all");
	bMust &= checkParameter( vm, k_transResult,     "[�K�{]�|�󌋉ʃt�H���_");
	bMust &= checkParameter( vm, k_out,             "[�K�{]�ϊ����ʃt�H���_");
	bMust &= checkParameter( vm, k_runLog,          "[�K�{]���s���O�t�@�C��");
	bMust &= checkParameter( vm, k_errLog,          "[�K�{]�G���[���O�t�@�C��");

	// �p�����[�^������ł͂Ȃ��ꍇ�́A�I�v�V�����\�����ďI��
	if( !bMust){
		wcerr << L"������|��t�H�[�}�b�g�ϊ��c�[��" << std::endl;
		cerr << desc << std::endl;
		return false;
	}

	m_convMode      = args.convMode.c_str();
	m_transResult   = args.transResult.c_str();
	m_out           = args.out.c_str();
	m_runLog        = args.runLog.c_str();
	m_errLog        = args.errLog.c_str();

	// ���s�����O������
	if( !runLogger.initialize(m_runLog.GetString(), SJIS_CREATE) )
	{
		wcerr << (message::getMsg( eFailRunLogInitialize)) << (L">>Path:") << ( m_runLog.GetString() ) << endl;
		return false;
	}
	// �G���[���O������
	if( !errLogger.initialize(m_errLog.GetString(), UTF8_CREATE) )
	{
		wcerr << (message::getMsg( eFailErrLogInitialize)) << (L">>Path:") << ( m_errLog.GetString() ) << endl;
		return false;
	}
	//�@���������݂���ꍇ�̃`�F�b�N
	if (m_convMode != CONV_MODE_CITY && m_convMode != CONV_MODE_ADDR && m_convMode != CONV_MODE_ALL)
	{
		runLogger.log(message::getMsg( eFailAnalyzeConvMode));
		wcerr << (message::getMsg( eFailAnalyzeConvMode));
		return false;
	}
	if(!PathIsDirectory(m_transResult.GetString()))
	{
		runLogger.log(message::getMsg( eFailAnalyzeTransResult));
		wcerr << (message::getMsg( eFailAnalyzeTransResult));
		return false;
	}
	if(!PathIsDirectory(m_out.GetString()))
	{
		runLogger.log(message::getMsg( eFailAnalyzeOut));
		wcerr << (message::getMsg( eFailAnalyzeOut));
		return false;
	}
	return true;
}

// �����̎擾
const CStringW& Argument::getValue(argumentType type) const
{
	switch	( type )
	{
	case kConvMode:
		return m_convMode;
	case kTransResult:
		return m_transResult;
	case kOut:
		return m_out;
	case kRunLog:
		return m_runLog;
	case kErrLog:
		return m_errLog;
	default:
		break;
	}
	return L"";
}