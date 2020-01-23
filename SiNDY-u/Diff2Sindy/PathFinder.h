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

#include <iostream>
#include <algorithm>
#include <filesystem>
#include "tstring.h"

/**
 * @brief �p�X��T��Utility�N���X
 */
class PathFinder
{
public:
	PathFinder(){};
	~PathFinder(){};

	/**
	 * @brief  �f�B���N�g���p�X����t�@�C���p�X�̈ꗗ���擾
	 * @param  directoryName  [in]  �f�B���N�g���p�X
	 * @retval �t�@�C���p�X�̈ꗗ
	 */
	std::vector<uh::tstring> getFilePath(const uh::tstring& directoryName);

	/**
	 * @brief  �f�B���N�g���p�X����t�@�C���p�X�̈ꗗ���擾
	 * @param  directoryName  [in]  �f�B���N�g���p�X
	 * @param  filters		  [in]  �t�B���^�ɂ��镶����̃��X�g(���K�\����Ή�)
	 * @retval �t�@�C���p�X�̈ꗗ
	 */
	std::vector<uh::tstring> getFilePath(const uh::tstring& directoryName, std::vector<uh::tstring> filters);

	/**
	 * @brief  �f�B���N�g���p�X����t�@�C���p�X�̈ꗗ���擾
	 * @param  directoryName  [in]  �f�B���N�g���p�X
	 * @param  filter		  [in] �t�B���^�ɂ��镶����(���K�\����Ή�)
	 * @retval �t�@�C���p�X�̈ꗗ
	 */
	std::vector<uh::tstring> getFilePath(const uh::tstring& directoryName, uh::tstring filter);

	/**
	 * @brief  �f�B���N�g���p�X����w�肵���g���q�����t�@�C���p�X�̈ꗗ���擾
	 * @param  directoryName  [in]  �f�B���N�g���p�X
	 * @param  filter		  [in] �t�B���^�ɂ���g���q(���K�\����Ή�)
	 * @retval �t�@�C���p�X�̈ꗗ
	 */
	std::vector<uh::tstring> getFilePathByExtension(const uh::tstring& directoryName, uh::tstring filter);

	/**
	 * @brief  �f�B���N�g���p�X����w�肵���g���q�����t�@�C���p�X�̈ꗗ���擾
	 * @param  directoryName  [in]  �f�B���N�g���p�X
	 * @param  filters		  [in]  �t�B���^�ɂ���g���q�̃��X�g(���K�\����Ή�)
	 * @retval �t�@�C���p�X�̈ꗗ
	 */
	std::vector<uh::tstring> getFilePathByExtension(const uh::tstring& directoryName, std::vector<uh::tstring> filters);
};

