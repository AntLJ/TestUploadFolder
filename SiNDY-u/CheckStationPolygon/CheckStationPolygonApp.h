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
#include "CheckHandler.h"

/**
 * @brief   ���C���N���X
 */
class CheckStationPolygonApp
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CheckStationPolygonApp( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	~CheckStationPolygonApp(){}

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();


private:

	/**
	 * @brief   ���O�t�@�C��������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool initLog();

	/**
	 * @brief   �`�F�b�J�[�o�^
	 */
	void resistChecker();

	/**
	 * @brief   �`�F�b�J�[������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool initChecker();

	/**
	 * @brief   �`�F�b�N���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool runCheck();

	/**
	 * @brief   �㏈��
	 * @param [in] bSuccess ����I���������ۂ�
	 */
	void finish( bool bSuccess );

private:

	Arguments m_args;      //!< ����

	std::vector<SPCheckHandler> m_CheckerContainer;		//!< �`�F�b�J�[�R���e�i
};
