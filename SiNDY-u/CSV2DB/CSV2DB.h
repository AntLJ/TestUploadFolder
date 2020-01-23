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

namespace sindy{
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}


/**
 * @brief   ���C���N���X
 */
class CCSV2DB
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCSV2DB( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	~CCSV2DB(){};

	/**
	 * @brief   ���s
	 * @note �G���[���������ꍇ�͗�O�𓊂���
	 */
	void execute();

private:

	/**
	 * @brief �Ώۃe�[�u�����擾����
	 * @note  ���s�����ꍇ�͗�O�𓊂���
	 * @param work [in] �Ώۂ̃��[�N�X�y�[�X�Bnullptr�̏ꍇ�͈����̐ڑ���
	 * @return work�ɑ��݂�������Ŏw�肵���e�[�u��
	 */
	sindy::CSPTableContainer GetDestTable( const IWorkspacePtr work = nullptr );

private:
	Arguments m_args;             //!< ����
	sindy::CTableFinder m_finder; //!< �t�@�C���_
};

