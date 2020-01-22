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

/**
 * @brief   �C���|�[�g�p�Ή����
 */
struct ImportInfo
{
	uh::tstring tableName;	//!< �e�[�u�����i�C���|�[�g��j
	uh::tstring fileName;	//!< CSV�t�@�C�����i�C���|�[�g���j
	bool bKeepData;			//!< �f�[�^�ێ��t���O

	/**
	 * @brief   �f�t�H���g�R���X�g���N�^
	 */
	ImportInfo():bKeepData(false){}

	/**
	 * @brief   �R���X�g���N�^
	 * @param[in]   table	�e�[�u�����i�C���|�[�g��j
	 * @param[in]   file	CSV�t�@�C�����i�C���|�[�g���j
	 * @param[in]   flag	�f�[�^�ێ��t���O
	 */
	ImportInfo(const uh::tstring& table, const uh::tstring& file, bool flag)
		:tableName(table), fileName(file), bKeepData(flag){}
};

/**
 * @brief   ���C���N���X
 */
class ADAMSurveyImporter
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param[in]	args	����
	 */
	ADAMSurveyImporter( const Arguments& args )
	{
		_args = args;
	}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~ADAMSurveyImporter(){}

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:
	Arguments _args;      //!< ����
};

