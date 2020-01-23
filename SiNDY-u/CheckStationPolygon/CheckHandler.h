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

#include "StationChecker.h"

// STL
#include <memory>

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>


/**
 * @brief   �`�F�b�N�Ǘ��N���X�̃C���^�[�t�F�[�X
 * @note    �ǂ̃`�F�b�N���s�������Ǘ�����
 * @note    ���ۂ̃`�F�b�N���W�b�N�� StationChecker �N���X �ɈϏ�
 */
class CheckHandler
{
public:

	/**
	 * @brief   ������
	 * @param   workspace [in] �`�F�b�N�Ώۂ̃��[�N�X�y�[�X
	 * @param   checker   [in] �`�F�b�N���W�b�N��ێ�����I�u�W�F�N�g
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	virtual bool init( const sindy::CWorkspace& workspace, SPStationChecker checker ) = 0;

	/**
	 * @brief   �`�F�b�N���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	virtual bool check() = 0;

	/**
	 * @brief   �`�F�b�N�Ώۃe�[�u������Ԃ�
	 * @return  �e�[�u����
	 */
	virtual uh::tstring getTarget() const = 0;

};

typedef std::shared_ptr<CheckHandler> SPCheckHandler;
