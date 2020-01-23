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

#include "Parameter.h"

/**
 * @enum	emCreateMode
 * @brief	�쐬���[�h
 */
enum emCreateMode
{
	kGPMode,		//!< ���|�C���g
	kKPMode,		//!< �ƌ`�����N�|�C���g
	kCSMode,		//!< CS�|�C���g
	kUnknownMode	//!< �s��
};

/**
 * @class	CApp
 * @brief	�A�v���P�[�V�����N���X
 */
class CApp
{
public:
	CApp(void) : m_emMode(kUnknownMode){}
	~CApp(void){}

	/**
	 * @brief	�g�p�@
	 */
	static void Usage();

	/**
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief	���s
	 * @return	bool
	 */
	bool Run();

private:

	emCreateMode	m_emMode;	//!< �쐬���[�h

	CParameter		m_cParam;	//!< ���s���

	std::ofstream	m_ofs;		//!< ���O�o�̓X�g���[��
};
