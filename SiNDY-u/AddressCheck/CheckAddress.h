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

#include "CheckControl.h"
#include "Argument.h"

/**
 * @class	CheckAddress
 * @brief	���C�����[�`���N���X
 */
class CheckAddress
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CheckAddress(void){}

	 /**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CheckAddress(void){}

	/**
	* @brief   ������
	* @param   [in] argc 	�R�}���h���C�������̐�
	* @param   [in] argv 	�R�}���h���C�������̓��e
	* @retval  true:����
	* @retval  false:���s
	*/
	bool init( int argc, TCHAR** argv );

	/**
	* @brief   ���s
	* @retval  true:����  false:���s
	*/
	bool run();

private:	

	Argument                       m_argument;      //!< ����
	CheckControl                   m_checkControl;  //!< ����N���X

};

