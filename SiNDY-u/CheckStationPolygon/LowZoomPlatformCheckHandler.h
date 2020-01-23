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

#include "CheckHandler.h"


/**
 * @brief   �v���b�g�t�H�[���S�̃|���S��(LOWZOOM_PLATFORM)�̃`�F�b�N�Ǘ��N���X
 */
class LowZoomPlatformCheckHandler : public CheckHandler
{
public:
	LowZoomPlatformCheckHandler(void){}
	~LowZoomPlatformCheckHandler(void){}

	/**
	 * @brief   ������
	 * @param   workspace [in] �`�F�b�N�Ώۂ̃��[�N�X�y�[�X
	 * @param   checker   [in] �`�F�b�N���W�b�N��ێ�����I�u�W�F�N�g
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	virtual bool init( const sindy::CWorkspace& workspace, SPStationChecker checker ) override;

	/**
	 * @brief   �`�F�b�N���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	virtual bool check() override;

	/**
	 * @brief   �`�F�b�N�Ώۃe�[�u������Ԃ�
	 * @return  �e�[�u����
	 */
	virtual uh::tstring getTarget() const override { return sindy::schema::sj::lowzoom_platform::kTableName; }


private:

	SPStationChecker m_Checker;			//!< �`�F�b�J�[

	sindy::CTableFinder m_TableFinder;	//!< �e�[�u���t�@�C���_

};
