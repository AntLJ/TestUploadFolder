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

#include "Arguments.h"
#include <TDC/sindylib_base/TableFinder.h>

/**
 * @brief   ���C���N���X
 */
class BldNameAddrAnalyzer
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	BldNameAddrAnalyzer( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~BldNameAddrAnalyzer(){};

	/**
	 * @brief   ���s
	 * @note    �v���I�ȃG���[�����������ꍇ�͗�O�𓊂���
	 */
	void execute();

private:

	/**
	 * @brief   ������
	 * @note    ���s�����ꍇ�͗�O�𓊂���
	 */
	void init();


private:

	Arguments m_args;             //!< ����
	sindy::CTableFinder m_finder; //!< �t�@�C���_
};

