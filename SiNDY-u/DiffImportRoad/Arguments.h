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

#include "InputInfo.h"
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments()
	{
	}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @param   cInputData[out] �I�v�V�������m�ۗp�N���X
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse( int argc, TCHAR* argv[], CInputInfo& cInputInfo );

private:
	/** 
	 * @brief �g�p�@�\�� 
	 * @param[in]	App	���s���R�}���h���C���̈�� 
	 */ 
	void PrintUsage(LPCTSTR App, po::options_description& opt);

	/** 
	 * @brief �g�p�@�\�� �i������Ƃ����ڂ����j
	 * @param[in]	App	���s���R�}���h���C���̈�� 
	 */ 
	void PrintHelp(LPCTSTR App, po::options_description& opt);

public:
	uh::tstring	input;	///< ���̓p�����[�^
};
