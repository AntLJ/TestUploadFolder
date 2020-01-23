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
#include <fstream>

/**
 * @brief   ���C���N���X
 */
class CCreateNodelist
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCreateNodelist( const Arguments& args ){ m_args = args; }

	/**
	 * @brief   �f�X�g���N�^
	 */
	CCreateNodelist(){ mRunLog.close(); mErrLog.close(); }

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	 * @brief   �G���[���o�͂���
	 * @brief strErrMsg  �G���[���b�Z�[�W
	 */
	void outputErrMsg( const CString& strErrMsg );


private:

	Arguments m_args;			//!< ����

	std::ofstream mRunLog;		//!< �������O�̃t�@�C���X�g���[��
	std::ofstream mErrLog;		//!< �G���[���O�̃t�@�C���X�g���[��
};

