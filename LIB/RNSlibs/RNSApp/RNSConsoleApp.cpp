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

// RNSConsoleApp.cpp: RNSConsoleApp �N���X�̃C���v�������e�[�V����
//
// $Id: RNSConsoleApp.cpp,v 1.1 2003/05/16 09:40:02 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include "RNSConsoleApp.h"
#include "RNSMessage.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RNSConsoleApp::RNSConsoleApp(const char* cAppCaption) :
	mAlreadyRun(false)
{
	setAppCaption(cAppCaption);
}

RNSConsoleApp::~RNSConsoleApp()
{

}

//////////////////////////////////////////////////////////////////////
// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief	������ݒ肷��
 *
 * @param	argc		[in]	�����̐�
 * @param	argv		[in]	����
 * @retval	true	����ɐݒ�ł���
 * @retval	false	��`����Ă��Ȃ��I�v�V�����������Ɋ܂܂�Ă���
 */
bool RNSConsoleApp::setArgs(int argc, char* argv[])
{
	// �A�v���P�[�V�������ݒ�
	setAppName(argv[0]);

	// �I�v�V�����ݒ�
	const char* pOptionSimbols = optionSimbols();
	if(pOptionSimbols) {
		mOptionSet.regestOptionSimbols(pOptionSimbols);
	}

	return mOptionSet.setOption(argc-1, argv+1, &mArgList);
}

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief	�����J�n
 *
 * @note	������Ăяo����std::logic_error()�𔭐�������B
 *
 * @retval	0	����
 * @retval	!0	���s
 */
int RNSConsoleApp::run()
{
	using namespace rns_message;

	// ������Ăяo���Ή�
	if(mAlreadyRun)
		throw std::logic_error(RNS_DEBUG_STRING("\"run\" method 2 times called"));
	mAlreadyRun = true;

	// usage�o��
	if(isNeedPrintUsage()) {
		printUsage(std::cerr);
		return 0;
	}

	// �{�����Ăяo��
	int aResult = callEntrust();

	if(aResult == 0) {
		std::cerr << appCaption().c_str() << getMsg(iSuccess) << std::endl;
	}
	else {
		std::cerr << appCaption().c_str() << getMsg(eFailure) << std::endl;
	}

	return aResult;
}

/**
 * @brief	�{�����Ăяo��
 *
 * try�\������preEntrust()�Aentrust()�AepiEntrust()���Ăяo���B\n
 * preEntrust()��epiEntrust()��false��Ԃ����ꍇ�A�܂���std::exception()����h�����ꂽ��O��߂炦���ꍇ�A-1��Ԃ��ď����𒆒f����B\n
 *
 * @retval	0	����
 * @retval	!0	���s
 */
int RNSConsoleApp::callEntrust()
{
	int aResult = 0;

	try {
		if(! preEntrust())
			return -1;

		aResult = entrust();

		if(! epiEntrust())
			return -1;
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		aResult = -1;
	}

	return aResult;
}

//////////////////////////////////////////////////////////////////////
// �I�[�o�[���C�h����鏈��
//////////////////////////////////////////////////////////////////////

/**
 * @brief	usage�o��
 *
 * @param	cStream	[in]	usage���o�͂���X�g���[��
 */
void RNSConsoleApp::printUsage(std::ostream& cStream)
{
	cStream << appCaption().c_str() << "(" << appName().c_str() << ")" << std::endl;
}
